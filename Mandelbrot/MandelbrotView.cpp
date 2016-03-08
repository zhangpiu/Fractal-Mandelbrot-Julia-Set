// MandelbrotView.cpp : implementation of the CMandelbrotView class
//

#include "stdafx.h"
#include "Mandelbrot.h"

#include "MandelbrotDoc.h"
#include "MandelbrotView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMandelbrotView

IMPLEMENT_DYNCREATE(CMandelbrotView, CBCGPFormView)

BEGIN_MESSAGE_MAP(CMandelbrotView, CBCGPFormView)
	//ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_SAVE_AS, CMandelbrotView::OnFileSaveAs)
	ON_COMMAND(ID_FRACTAL_MANDELBROTSET, OnFractalMandelbrotSet)
	ON_COMMAND(ID_FRACTAL_JULIASET, OnFractalJuliaSet)
	ON_COMMAND(ID_FRACTAL_RESET, OnFractalReset)

	ON_COMMAND(ID_MANDELBROTSET_EXPONENT, OnDummy)
	ON_EN_CHANGE(ID_MANDELBROTSET_EXPONENT, OnMandelExponent)

	ON_COMMAND(ID_JULIASET_CONSTANT, OnJuliaConstant)

	ON_COMMAND(ID_FRACTAL_ITERATION, OnChangeIteration)
	
	ON_COMMAND(ID_FRACTAL_COLORPICKER_MAIN, OnChangeColor)
	ON_COMMAND(ID_FRACTAL_COLORPICKER_VICE, OnChangeColor)
END_MESSAGE_MAP()

// CMandelbrotView construction/destruction

CMandelbrotView::CMandelbrotView()
	: CBCGPFormView(CMandelbrotView::IDD)
	, m_pFractal(nullptr)
	, m_pMandelbrot(nullptr)
	, m_pJulia(nullptr)
	, m_nImageOrgX(0)
	, m_nImageOrgY(0)
	, m_nImageWidth(0)
	, m_nImageHeight(0)
{

	EnableVisualManagerStyle();

	m_pMandelbrot = new CMandelSet;
	m_pJulia      = new CJuliaSet;

#ifndef _DEBUG
	m_pFractal = m_pMandelbrot;
	m_pFractal->UpdateImagePixel();

	UpdateFractalImage();
#endif // !_DEBUG
}

CMandelbrotView::~CMandelbrotView()
{
	if (m_pMandelbrot)
	{
		delete m_pMandelbrot;
	}

	if (m_pJulia)
	{
		delete m_pJulia;
	}
}

void CMandelbrotView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BOOL CMandelbrotView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CBCGPFormView::PreCreateWindow(cs);
}

void CMandelbrotView::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CMandelbrotView diagnostics

#ifdef _DEBUG
void CMandelbrotView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

void CMandelbrotView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}

CMandelbrotDoc* CMandelbrotView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMandelbrotDoc)));
	return (CMandelbrotDoc*)m_pDocument;
}
#endif //_DEBUG


// CMandelbrotView message handlers

/*---------------------------------------------------------------------------------/
| function:          OnDraw
| description:       
| input:
| output:
| return:            none
| note:              [10/26/2015 vodka]
|---------------------------------------------------------------------------------*/
void CMandelbrotView::OnDraw(CDC *pDC)
{
	CMandelbrotDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDC dc;
	CDC* pDrawDC = pDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap = 0;

	// only paint the rect that needs repainting
	CRect client;
	pDC->GetClipBox(client);
	CRect rect = client;
	DocToClient(rect);

	if (!pDC->IsPrinting())
	{
		// draw to offscreen bitmap for fast looking repaints
		if (dc.CreateCompatibleDC(pDC))
		{
			if (bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()))
			{
				OnPrepareDC(&dc, NULL);
				pDrawDC = &dc;

				// offset origin more because bitmap is just piece of the whole drawing
				dc.OffsetViewportOrg(-rect.left, -rect.top);
				pOldBitmap = dc.SelectObject(&bitmap);
				dc.SetBrushOrg(rect.left % 8, rect.top % 8);

				// might as well clip to the same rectangle
				dc.IntersectClipRect(client);
			}
		}
	}

	// paint background
	CBrush brush;
	if (!brush.CreateSolidBrush(RGB(255, 255, 255)))
		return;

	brush.UnrealizeObject();
	pDrawDC->FillRect(client, &brush);


	// Draw the mandelbrot set
	DrawImage(pDrawDC);

	// Draw the rectangle
	DrawRectangle(pDrawDC);

	if (pDrawDC != pDC)
	{
		pDC->SetViewportOrg(0, 0);
		pDC->SetWindowOrg(0, 0);
		pDC->SetMapMode(MM_TEXT);
		dc.SetViewportOrg(0, 0);
		dc.SetWindowOrg(0, 0);
		dc.SetMapMode(MM_TEXT);
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),
			&dc, 0, 0, SRCCOPY);
		dc.SelectObject(pOldBitmap);
	}
}

/*---------------------------------------------------------------------------------/
| function:          DocToClient
| description:
| input:
| output:
| return:            none
| note:              [10/26/2015 vodka]
|---------------------------------------------------------------------------------*/
void CMandelbrotView::DocToClient(CRect& rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(rect);
	rect.NormalizeRect();
}


/*---------------------------------------------------------------------------------/
| function:          DrawRectangle
| description:
| input:
| output:
| return:            none
| note:              [10/26/2015 vodka]
|---------------------------------------------------------------------------------*/
void CMandelbrotView::DrawRectangle(CDC *pDC)
{
	CRect rect(m_ptRectStart, m_ptRectEnd);

	rect.NormalizeRect();

	if (rect.IsRectNull()) return;

	CPen pen, *pOldPen = nullptr;
	CBrush brush, *pOldBrush = nullptr;

	pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
	brush.CreateStockObject(NULL_BRUSH);

	pOldPen = pDC->SelectObject(&pen);
	pOldBrush = pDC->SelectObject(&brush);

	pDC->Rectangle(&rect);

	pDC->SelectObject(pOldPen);

	pen.DeleteObject();
	brush.DeleteObject();
}

/*---------------------------------------------------------------------------------/
| function:          DrawImage
| description:
| input:
| output:
| return:            none
| note:              [10/27/2015 vodka]
|---------------------------------------------------------------------------------*/
void CMandelbrotView::DrawImage(CDC *pDC)
{
	m_xim.Draw(pDC->GetSafeHdc(), m_nImageOrgX, m_nImageOrgY, m_nImageWidth, m_nImageHeight);
}


/*---------------------------------------------------------------------------------/
| function:          OnLButtonDown
| description:
| input:
| output:
| return:            none
| note:              [10/26/2015 vodka]
|---------------------------------------------------------------------------------*/
void CMandelbrotView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBCGPFormView::OnLButtonDown(nFlags, point);

	CPaintDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);

	m_ptRectStart = point;
	m_ptRectEnd   = point;

	Invalidate(FALSE);
}

/*---------------------------------------------------------------------------------/
| function:          OnLButtonUp
| description:
| input:
| output:
| return:            none
| note:              [10/26/2015 vodka]
|---------------------------------------------------------------------------------*/
void CMandelbrotView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CBCGPFormView::OnLButtonUp(nFlags, point);

	CClientDC dc(this);

	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	
	if (point == m_ptRectStart) return;

	// Adjust the selected area to 4 : 3
	CRect outFrame(0, 0, m_nImageWidth, m_nImageHeight);
	CRect selArea(m_ptRectStart.x - m_nImageOrgX, m_ptRectStart.y - m_nImageOrgY, point.x - m_nImageOrgX, point.y - m_nImageOrgY);
	selArea.NormalizeRect();

	if (selArea.IsRectEmpty())
	{
		return;
	}

	if (selArea.Width() * 0.75 < selArea.Height())
	{
		selArea.bottom += 3 - (selArea.Height() % 3);
		selArea.left -= selArea.Height() / 3 * 4 / 2 - selArea.Width() / 2;
		selArea.right = selArea.left + selArea.Height() / 3 * 4;
	}
	else
	{
		selArea.right += 4 - (selArea.Width() % 4);
		selArea.top -= selArea.Width() * 3 / 4 / 2 - selArea.Height() / 2;
		selArea.bottom = selArea.top + selArea.Width() * 3 / 4;
	}

	// Recalculate the image
	AfxGetMainWnd()->BeginWaitCursor();
	m_pFractal->SetCoordinate(outFrame, selArea);
	m_pFractal->UpdateImagePixel();

	UpdateFractalImage();
	m_xim.Resample2(m_nImageWidth, m_nImageHeight);
	AfxGetMainWnd()->EndWaitCursor();

	// Make the rectangle null
	m_ptRectEnd = m_ptRectStart;

	Invalidate(FALSE);
}

/*---------------------------------------------------------------------------------/
| function:          OnMouseMove
| description:
| input:
| output:
| return:            none
| note:              [10/26/2015 vodka]
|---------------------------------------------------------------------------------*/
void CMandelbrotView::OnMouseMove(UINT nFlags, CPoint point)
{
	CBCGPFormView::OnMouseMove(nFlags, point);

	if (nFlags == MK_LBUTTON)
	{
		CPaintDC dc(this);
		OnPrepareDC(&dc);
		dc.DPtoLP(&point);

		m_ptRectEnd = point;
	}

	Invalidate(FALSE);
}


/*---------------------------------------------------------------------------------/
| function:          OnSize
| description:
| input:
| output:
| return:            none
| note:              [10/26/2015 vodka]
|---------------------------------------------------------------------------------*/
void CMandelbrotView::OnSize(UINT nType, int cx, int cy)
{
	// Adjust the proportion of width : height = 4 : 3
	if (cx * 3 / 4 < cy)
	{
		m_nImageWidth = cx;
		m_nImageHeight = m_nImageWidth * 3 / 4;
		m_nImageOrgX = 0;
		m_nImageOrgY = (cy - m_nImageHeight) / 2;
	}
	else
	{
		m_nImageHeight = cy;
		m_nImageWidth = m_nImageHeight * 4 / 3;
		m_nImageOrgY = 0;
		m_nImageOrgX = (cx - m_nImageWidth) / 2;
	}
}


/*---------------------------------------------------------------------------------/
| function:          OnFileSaveAs
| description:
| input:
| output:
| return:            none
| note:              [10/28/2015 vodka]
|---------------------------------------------------------------------------------*/
void CMandelbrotView::OnFileSaveAs()
{
	CString strFileName = _T("Fractal");
	CFileDialog dlg(FALSE, ".bmp", strFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "BMP Files (*.bmp)|*.bmp|");
	
	if (dlg.DoModal() == IDOK) strFileName = dlg.GetPathName();
	else   return;

// 	HWND hwnd = ::GetDesktopWindow();
// 	CRect rectDesktop;
// 	::GetClientRect(hwnd, &rectDesktop);
// 
// 	int width = rectDesktop.Width();
// 	int height = rectDesktop.Height();
// 
// 	if (width * 0.75 < height)
// 		width = height * 4 / 3;
// 	else
// 		height = width * 3 / 4;
// 
// 	m_ximorg.Resample2(width, height);
	AfxGetMainWnd()->BeginWaitCursor();
	m_ximorg.Save(strFileName, CXIMAGE_FORMAT_BMP);
	AfxGetMainWnd()->EndWaitCursor();
}


/*---------------------------------------------------------------------------------/
| function:          OnFractalMandelbrotSet
| description:
| input:
| output:
| return:            none
| note:              [10/28/2015 vodka]
|---------------------------------------------------------------------------------*/
void CMandelbrotView::OnFractalMandelbrotSet()
{
 	if (dynamic_cast<CMandelSet *>(m_pFractal)) return;
 	
 	GetMainFrame()->AddMandelPanel();
 
 	m_pFractal = m_pMandelbrot;
 	
 	AfxGetMainWnd()->BeginWaitCursor();
 	m_pFractal->ResetCoordinate();
 	m_pFractal->SetIteration();
 	m_pFractal->InitPallette();
 	m_pFractal->UpdateImagePixel();
 	UpdateFractalImage();
 	AfxGetMainWnd()->EndWaitCursor();
}

/*---------------------------------------------------------------------------------/
| function:          OnFractalJuliaSet
| description:
| input:
| output:
| return:            none
| note:              [10/28/2015 vodka]
|---------------------------------------------------------------------------------*/
void CMandelbrotView::OnFractalJuliaSet()
{
	if (dynamic_cast<CJuliaSet *>(m_pFractal)) return;

	GetMainFrame()->AddJuliaPanel();

	m_pFractal = m_pJulia;
	
	AfxGetMainWnd()->BeginWaitCursor();
	m_pFractal->ResetCoordinate();
	m_pFractal->SetIteration();
	m_pFractal->InitPallette();
	m_pFractal->UpdateImagePixel();
	UpdateFractalImage();
	AfxGetMainWnd()->EndWaitCursor();
}

/*---------------------------------------------------------------------------------/
| function:          OnFractalReset
| description:
| input:
| output:
| return:            none
| note:              [10/28/2015 vodka]
|---------------------------------------------------------------------------------*/
void CMandelbrotView::OnFractalReset()
{
	AfxGetMainWnd()->BeginWaitCursor();

	// Reset
	m_pFractal->ResetCoordinate();
	m_pFractal->UpdateImagePixel();
	UpdateFractalImage();

	AfxGetMainWnd()->EndWaitCursor();
}

/*---------------------------------------------------------------------------------/
| function:          UpdateFractalImage
| description:       When the fractal image recalculated, update the image on screen.
|
| note:              [10/29/2015 vodka]
|---------------------------------------------------------------------------------*/
void CMandelbrotView::UpdateFractalImage()
{
	m_ximorg.Copy(m_pFractal->GetImage());
	m_xim.Copy(m_ximorg);

	Invalidate(FALSE);
}


/*---------------------------------------------------------------------------------/
| function:          OnMandelExponent
| description:
|
| note:              [10/29/2015 vodka]
|---------------------------------------------------------------------------------*/
void CMandelbrotView::OnMandelExponent()
{
#define MIN_EXPONENT      2
#define MAX_EXPONENT      10

	CBCGPRibbonEdit* pEdit = DYNAMIC_DOWNCAST(CBCGPRibbonEdit, GetMainFrame()->GetRibbonElement(ID_MANDELBROTSET_EXPONENT));

	CString strEdit;
	bool bDigit = true;

	// Check the input valid or not.
	strEdit = pEdit->GetEditText();
	for (int i = 0; i < strEdit.GetLength(); ++i)
	{
		if (strEdit[i] < '0' || strEdit[i] > '9')
		{
			bDigit = false;
			break;
		}
	}

	CMandelSet *pMandel = static_cast<CMandelSet *>(m_pFractal);
	int oldExponent = pMandel->GetExponent();
	int newExponent = atoi(strEdit);

	// If the input is valid, recalculate the fractal image.
	if (bDigit && newExponent >= MIN_EXPONENT && newExponent <= MAX_EXPONENT)
	{
		strEdit.Format(_T("%d"), newExponent);

		// Only not equal to the old exponent, then update.
		if (oldExponent != newExponent)
		{
			AfxGetMainWnd()->BeginWaitCursor();
			pMandel->SetExponent(newExponent);
			UpdateFractalImage();
			AfxGetMainWnd()->EndWaitCursor();
		}
	}
	else
	{
		::AfxMessageBox(_T("Exponent ranges from 2 to 10!"));
		strEdit.Format("%d", oldExponent);
	}
	
	// Update the edit box.
	pEdit->SetEditText(strEdit);


#undef MIN_EXPONENT
#undef MAX_EXPONENT
}


/*---------------------------------------------------------------------------------/
| function:          OnJuliaConstant
| description:
|
| note:              [10/29/2015 vodka]
|---------------------------------------------------------------------------------*/
void CMandelbrotView::OnJuliaConstant()
{
	CBCGPRibbonComboBox* pComboxConstant = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox, GetMainFrame()->GetRibbonElement(ID_JULIASET_CONSTANT));

	int newIndex = pComboxConstant->GetCurSel();

	CJuliaSet *pJulia = static_cast<CJuliaSet *>(m_pFractal);

	CComplex oldConstant = pJulia->GetConstant();
	CComplex newConstant = C_value[newIndex];

	if (oldConstant == newConstant) return;

	AfxGetMainWnd()->BeginWaitCursor();
	pJulia->SetConstant(newConstant);
	UpdateFractalImage();
	AfxGetMainWnd()->EndWaitCursor();
}


/*---------------------------------------------------------------------------------/
| function:          OnChangeIteration
| description:
|
| note:              [10/29/2015 vodka]
|---------------------------------------------------------------------------------*/
void CMandelbrotView::OnChangeIteration()
{
	CBCGPRibbonComboBox* pComboxConstant = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox, GetMainFrame()->GetRibbonElement(ID_FRACTAL_ITERATION));

	int newIndex = pComboxConstant->GetCurSel();

	int oldIteration = m_pFractal->GetIteration();
	int newIteration = iter_num[newIndex];

	if (oldIteration == newIteration) return;

	AfxGetMainWnd()->BeginWaitCursor();
	m_pFractal->SetIteration(newIteration);
	UpdateFractalImage();
	AfxGetMainWnd()->EndWaitCursor();
}


/*---------------------------------------------------------------------------------/
| function:          OnChangeColor
| description:
|
| note:              [10/29/2015 vodka]
|---------------------------------------------------------------------------------*/
void CMandelbrotView::OnChangeColor()
{
	CBCGPRibbonSlider *pSliderMain = DYNAMIC_DOWNCAST(CBCGPRibbonSlider, GetMainFrame()->GetRibbonElement(ID_FRACTAL_COLORPICKER_MAIN));
	CBCGPRibbonSlider *pSliderVice = DYNAMIC_DOWNCAST(CBCGPRibbonSlider, GetMainFrame()->GetRibbonElement(ID_FRACTAL_COLORPICKER_VICE));

	int h1 = pSliderMain->GetPos();
	int h2 = pSliderVice->GetPos();

	m_pFractal->InitPallette(h1, h2);
	m_pFractal->UpdateImagePixel(false);

	UpdateFractalImage();
}



/*---------------------------------------------------------------------------------/
| function:          OnDummy
| description:       Do nothing, just enable some buttons.
|
| note:              [10/29/2015 vodka]
|---------------------------------------------------------------------------------*/
void CMandelbrotView::OnDummy() {}



/*---------------------------------------------------------------------------------/
| function:          PreTranslateMessage
| description:       If press the VK_LEFT, VK_UP, VK_RIGHT, VK_DOWN when slider main
|              or slider vice on focus, then redraw the image.
|
| note:              [10/30/2015 vodka]
|---------------------------------------------------------------------------------*/
BOOL CMandelbrotView::PreTranslateMessage(MSG* pMsg)
{
	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient(GetMainFrame()->GetSafeHwnd(), &pt);

	CBCGPRibbonSlider *pSliderMain = DYNAMIC_DOWNCAST(CBCGPRibbonSlider, GetMainFrame()->GetRibbonElement(ID_FRACTAL_COLORPICKER_MAIN));
	CBCGPRibbonSlider *pSliderVice = DYNAMIC_DOWNCAST(CBCGPRibbonSlider, GetMainFrame()->GetRibbonElement(ID_FRACTAL_COLORPICKER_VICE));

	CRect rectMain = pSliderMain->GetRect();
	CRect rectVice = pSliderVice->GetRect();

	CBCGPRibbonSlider *pSlider = nullptr;

	if (rectMain.PtInRect(pt))
	{
		pSlider = pSliderMain;
	}

	if (rectVice.PtInRect(pt))
	{
		pSlider = pSliderVice;
	}

	if (pSlider && pMsg->message == WM_KEYDOWN)
	{
		bool redraw = false;
		int pos = pSlider->GetPos();
		int posMin = pSlider->GetRangeMin();
		int posMax = pSlider->GetRangeMax();

		if (pMsg->wParam == VK_LEFT)
		{
			pos -= 1;
			pos = max(pos, posMin);
			redraw = true;
		}

		if (pMsg->wParam == VK_DOWN)
		{
			pos -= 10;
			pos = max(pos, posMin);
			redraw = true;
		}

		if (pMsg->wParam == VK_RIGHT)
		{
			pos += 1;
			pos = min(pos, posMax);
			redraw = true;
		}

		if (pMsg->wParam == VK_UP)
		{
			pos += 10;
			pos = min(pos, posMax);
			redraw = true;
		}

		if (redraw)
		{
			pSlider->SetPos(pos);
			OnChangeColor();
		}
	}

	return CBCGPFormView::PreTranslateMessage(pMsg);
}