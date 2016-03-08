// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Mandelbrot.h"

#include "MainFrm.h"
#include "MandelbrotView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CBCGPFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPFrameWnd)
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(BCGM_ON_RIBBON_CUSTOMIZE, OnRibbonCustomize)
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	if (!CreateRibbonBar ())
	{
		TRACE0("Failed to create ribbon bar\n");
		return -1;      // fail to create
	}


	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: add your status bar panes here:
	m_wndStatusBar.AddElement (new CBCGPRibbonStatusBarPane (
		ID_STATUSBAR_PANE1, _T("Ready"), TRUE), _T("Ready"));
	m_wndStatusBar.AddExtendedElement (new CBCGPRibbonStatusBarPane (
		ID_STATUSBAR_PANE2, _T("by vodka."), TRUE), _T("by vodka."));

	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_ANY);
	return 0;
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers




BOOL CMainFrame::CreateRibbonBar ()
{
	m_wndRibbonBar.EnableCustomization();

	if (!m_wndRibbonBar.Create(this))
	{
		return FALSE;
	}

	// Load panel images:
	m_PanelImages.SetImageSize (CSize (16, 16));
	m_PanelImages.Load (IDB_RIBBON_ICONS);

	// Init main button:
	m_MainButton.SetImage (IDB_RIBBON_MAIN);
	m_MainButton.SetToolTipText (_T("File"));
	m_MainButton.SetText (_T("File\nf"));

	m_wndRibbonBar.SetMainButton (&m_MainButton, CSize (45, 45));

	CBCGPRibbonMainPanel* pMainPanel = m_wndRibbonBar.AddMainCategory (
		_T("File"), IDB_RIBBON_FILESMALL, IDB_RIBBON_FILELARGE);

	pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_NEW, _T("&New"), 0, 0));
	pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_OPEN, _T("&Open..."), 1, 1));

	pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_SAVE, _T("&Save"), 2, 2));

	pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_SAVE_AS, _T("Save &As..."), 3, 3));

	pMainPanel->AddRecentFilesList (_T("Recent Documents"), 300, TRUE /* Show Pins */);

	pMainPanel->AddToBottom (new CBCGPRibbonMainPanelButton (ID_TOOLS_OPTIONS, _T("Opt&ions"), 9));
	pMainPanel->AddToBottom (new CBCGPRibbonMainPanelButton (ID_APP_EXIT, _T("E&xit"), 8));

	// Add "Home" category with "Clipboard" panel:
	CBCGPRibbonCategory* pCategory = m_wndRibbonBar.AddCategory (
		_T("&Home"),
		IDB_RIBBON_HOMESMALL,
		IDB_RIBBON_HOMELARGE);

	// Create "Clipboard" panel:
	CBCGPRibbonPanel* pPanelFractal = pCategory->AddPanel (
		_T("Fractal"), 
		m_PanelImages.ExtractIcon (1));

	CBCGPRibbonButton* pBtnMandelSet = new CBCGPRibbonButton(ID_FRACTAL_MANDELBROTSET, _T("Mandelbrot Set\nm"), 0, 0);
	CBCGPRibbonButton* pBtnJuliaSet = new CBCGPRibbonButton(ID_FRACTAL_JULIASET, _T("Julia Set\nj"), 1, 1);
	CBCGPRibbonButton* pBtnReset = new CBCGPRibbonButton (ID_FRACTAL_RESET, _T("Reset\nr"), 2, 2);
	//pBtnReset->SetMenu (IDR_PASTE_MENU, TRUE);
	pPanelFractal->Add(pBtnMandelSet);
	pPanelFractal->Add(pBtnJuliaSet);
	pPanelFractal->Add(pBtnReset);

// 	pPanelClipboard->Add (new CBCGPRibbonButton (ID_EDIT_CUT, _T("Cut\nx"), 1));
// 	pPanelClipboard->Add (new CBCGPRibbonButton (ID_EDIT_COPY, _T("Copy\nc"), 2));
// 	pPanelClipboard->Add (new CBCGPRibbonButton (ID_EDIT_CLEAR, _T("Erase\nr"), 3));

	pPanelFractal->SetKeys (_T("zc"));

	AddMandelPanel();
	//AddJuliaPanel();
	
	// Create "<TODO>" panel:
// 	CBCGPRibbonPanel* pPanelTODO = pCategory->AddPanel (
// 		_T("<TODO>"));
// 	pPanelTODO->Add (new CBCGPRibbonLabel (_T("TODO: add elements here")));
// 
// 	pPanelTODO->SetKeys (_T("zt"));


// 	CBCGPRibbonColorButton* pBtnPaperColor = new CBCGPRibbonColorButton(ID_VIEW_PAPERCOLOR, _T("Paper Color\ng"), TRUE, 13, 1);
// 	pBtnPaperColor->SetAlwaysLargeImage();
// 	pBtnPaperColor->SetDefaultCommand(FALSE);
// 	pBtnPaperColor->EnableAutomaticButton(_T("&Automatic"), RGB(255, 255, 255));
// 	pBtnPaperColor->EnableOtherButton(_T("&More Colors..."), _T("More Colors"));
// 	pBtnPaperColor->SetColumns(10);
// 	pBtnPaperColor->SetColorBoxSize(CSize(17, 17));
// 	pBtnPaperColor->AddColorsGroup(_T("Theme Colors"), m_lstMainColors);
// 	pBtnPaperColor->AddColorsGroup(_T(""), m_lstAdditionalColors, TRUE);
// 	pBtnPaperColor->AddColorsGroup(_T("Standard Colors"), m_lstStandardColors);

	//pPanelOperations->Add(pBtnPaperColor);

	// Add some hidden (non-ribbon) elements:
	CBCGPRibbonUndoButton* pUndo = new CBCGPRibbonUndoButton (ID_EDIT_UNDO, _T("Undo"), 4);

	pUndo->AddUndoAction (_T("Undo Item 1"));
	pUndo->AddUndoAction (_T("Undo Item 2"));
	pUndo->AddUndoAction (_T("Undo Item 3"));
	pUndo->AddUndoAction (_T("Undo Item 4"));
	pUndo->AddUndoAction (_T("Undo Item 5"));

	pCategory->AddHidden (pUndo);

	// Add "<TODO>" category with "Clipboard" panel:
	CBCGPRibbonCategory* pTODOCategory = m_wndRibbonBar.AddCategory (
		_T("&TODO"), 
		0 /* TODO: ID-category small images */, 
		0 /* TODO: ID-category large images */);

	// Add quick access toolbar commands:
	CBCGPRibbonQATDefaultState qatState;

	qatState.AddCommand (ID_FILE_NEW, FALSE);
	qatState.AddCommand (ID_FILE_OPEN, FALSE);
	qatState.AddCommand (ID_FILE_SAVE);
	qatState.AddCommand (ID_EDIT_UNDO);

	m_wndRibbonBar.SetQuickAccessDefaultState (qatState);

	// Add "Style" button to the right of tabs:
// 	CBCGPRibbonButton* pStyleButton = new CBCGPRibbonButton (-1, _T("Style\ns"), -1, -1);
// 	pStyleButton->SetMenu (IDR_THEME_MENU, TRUE /* Right align */);
// 
// 	m_wndRibbonBar.AddToTabs (pStyleButton);

	// Add "About" button to the right of tabs:
	m_wndRibbonBar.AddToTabs (new CBCGPRibbonButton (ID_APP_ABOUT, _T("\na"), m_PanelImages.ExtractIcon (0)));
 // USE_RIBBON_DESIGNER

	m_wndRibbonBar.EnablePrintPreview(FALSE);

	return TRUE;
}

LRESULT CMainFrame::OnRibbonCustomize (WPARAM wp, LPARAM /*lp*/)
{
	ShowOptions (wp == 0 ? 1 : 0);
	return 1;
}

void CMainFrame::OnToolsOptions()
{
	ShowOptions (0);
}

void CMainFrame::ShowOptions (int nPage)
{
	// Create custom categories:
	
	// "Popular" items:
	
	CList<UINT, UINT> lstPopular;
	
	lstPopular.AddTail (ID_FILE_NEW);
	lstPopular.AddTail (ID_FILE_OPEN);
	lstPopular.AddTail (ID_FILE_SAVE);
	lstPopular.AddTail (ID_EDIT_UNDO);
	
	// Hidden commands:
	CList<UINT,UINT> lstHidden;
	m_wndRibbonBar.GetItemIDsList (lstHidden, TRUE);
	
	// All commands:
	CList<UINT,UINT> lstAll;
	m_wndRibbonBar.GetItemIDsList (lstAll);

	// Create "Customize QAT" page:
	CBCGPRibbonCustomizeQATPage pageCustomizeQAT(&m_wndRibbonBar);
	
	pageCustomizeQAT.AddCustomCategory (_T("Popular Commands"), lstPopular);
	pageCustomizeQAT.AddCustomCategory (_T("Commands not in the Ribbon"), lstHidden);
	pageCustomizeQAT.AddCustomCategory (_T("All Commands"), lstAll);
	
	// Create "Customize Ribbon" page:
	CBCGPRibbonCustomizeRibbonPage pageCustomizeRibbon(&m_wndRibbonBar);
	
	pageCustomizeRibbon.AddCustomCategory (_T("Popular Commands"), lstPopular);
	pageCustomizeRibbon.AddCustomCategory (_T("Commands not in the Ribbon"), lstHidden);
	pageCustomizeRibbon.AddCustomCategory (_T("All Commands"), lstAll);
	
	// Create property sheet:
	CBCGPPropertySheet propSheet (_T("Options"), this, nPage);
	propSheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	propSheet.EnableLayout();
	
	propSheet.SetLook(CBCGPPropertySheet::PropSheetLook_List, 150 /* List width */);
	
	propSheet.AddPage (&pageCustomizeRibbon);
	
	propSheet.AddPage (&pageCustomizeQAT);

	// TODO: add your option pages here:
	// COptionsPage1 pageOptions1;
	// propSheet.AddPage (&pageOptions1);
	//
	// COptionsPage1 pageOptions2;
	// propSheet.AddPage (&pageOptions2);
	
	if (propSheet.DoModal() != IDOK)
	{
		return;
	}
}
 // WORKSPACEBAR
 // OUTPUTBAR
 // PROPERTYBAR
 // UI_TYPE_RIBBON


/*---------------------------------------------------------------------------------/
| function:          InitPanel
| description:
|
| note:              [10/29/2015 vodka]
|---------------------------------------------------------------------------------*/
void CMainFrame::InitPanel()
{
	CBCGPRibbonCategory *pCategory = m_wndRibbonBar.GetActiveCategory();
	ASSERT(pCategory);

	while (pCategory->GetPanelCount() > 1)
	{
		pCategory->RemovePanel(1);
	}

	m_wndRibbonBar.ForceRecalcLayout();
}

/*---------------------------------------------------------------------------------/
| function:          AddMandelPanel
| description:
|
| note:              [10/29/2015 vodka]
|---------------------------------------------------------------------------------*/
void CMainFrame::AddMandelPanel()
{
	CBCGPRibbonCategory *pCategory = m_wndRibbonBar.GetActiveCategory();
	ASSERT(pCategory);

	InitPanel();

	CBCGPRibbonPanel *pPanelMandel = pCategory->AddPanel(
		_T("Mandelbrot Set"),
		m_PanelImages.ExtractIcon(1));
	
	// Add exponent edit box.
	CString strExponent;
	strExponent.Format(_T("%d"), 2);
	CBCGPRibbonEdit *pBtnExponent = new CBCGPRibbonEdit(ID_MANDELBROTSET_EXPONENT, 60, _T("Exponent: "));
	pBtnExponent->EnableSpinButtons(2, 10);
	pBtnExponent->SetToolTipText(_T("Exponent"));
	pBtnExponent->SetEditText(strExponent);
	
	pPanelMandel->Add(pBtnExponent);

	// Add iteration combo box.
	CBCGPRibbonComboBox *pComboIteration = new CBCGPRibbonComboBox(ID_FRACTAL_ITERATION, FALSE, 50, _T("Iteration: "));
	pComboIteration->SetTextAlign(TA_RIGHT);

	for (int i = 0; i < sizeof(iter_name) / sizeof(CString); ++i)
		pComboIteration->AddItem(iter_name[i]);
	
	pComboIteration->SelectItem(iter_1000);

	pPanelMandel->Add(pComboIteration);

	AddTwoColorSliders(pPanelMandel);

	m_wndRibbonBar.ForceRecalcLayout();
}

/*---------------------------------------------------------------------------------/
| function:          AddJuliaPanel
| description:
|
| note:              [10/29/2015 vodka]
|---------------------------------------------------------------------------------*/
void CMainFrame::AddJuliaPanel()
{
	CBCGPRibbonCategory *pCategory = m_wndRibbonBar.GetActiveCategory();
	ASSERT(pCategory);

	InitPanel();

	CBCGPRibbonPanel *pPanelJulia = pCategory->AddPanel(
		_T("Julia Set"),
		m_PanelImages.ExtractIcon(1));

	// Add constant combo box.
	CBCGPRibbonComboBox *pComboxConstant = new CBCGPRibbonComboBox(ID_JULIASET_CONSTANT, FALSE, 80, _T("Constant: "));
	pComboxConstant->SetTextAlign(TA_RIGHT);

	for (int i = 0; i < sizeof(C_name) / sizeof(CString); ++i)
	{
		pComboxConstant->AddItem(C_name[i]);
	}

	pComboxConstant->SelectItem(0);

	pPanelJulia->Add(pComboxConstant);

	// Add iteration combo box.
	CBCGPRibbonComboBox *pComboIteration = new CBCGPRibbonComboBox(ID_FRACTAL_ITERATION, FALSE, 80, _T("Iteration: "));
	pComboIteration->SetTextAlign(TA_RIGHT);

	for (int i = 0; i < sizeof(iter_name) / sizeof(CString); ++i)
		pComboIteration->AddItem(iter_name[i]);

	pComboIteration->SelectItem(iter_1000);

	pPanelJulia->Add(pComboIteration);

	
	AddTwoColorSliders(pPanelJulia);

	m_wndRibbonBar.ForceRecalcLayout();
}

/*---------------------------------------------------------------------------------/
| function:          AddTwoColorSliders
| description:
|
| note:              [10/30/2015 vodka]
|---------------------------------------------------------------------------------*/
void CMainFrame::AddTwoColorSliders(CBCGPRibbonPanel *pPanelFractal)
{
	// Add two color slider.
	pPanelFractal->AddSeparator();
	pPanelFractal->Add(new CBCGPRibbonLabel(_T("Main color:")));

	CBCGPRibbonSlider *pSliderMain = new CBCGPRibbonSlider(ID_FRACTAL_COLORPICKER_MAIN);
	pSliderMain->SetRange(0, 359);
	pSliderMain->SetToolTipText(_T("HUE"));
	pSliderMain->SetPos(137);
	pPanelFractal->Add(pSliderMain);

	pPanelFractal->AddSeparator();
	pPanelFractal->Add(new CBCGPRibbonLabel(_T("Vice color:")));

	CBCGPRibbonSlider *pSliderVice = new CBCGPRibbonSlider(ID_FRACTAL_COLORPICKER_VICE);
	pSliderVice->SetRange(0, 359);
	pSliderVice->SetToolTipText(_T("HUE"));
	pSliderVice->SetPos(30);
	pPanelFractal->Add(pSliderVice);

	m_wndRibbonBar.ForceRecalcLayout();
}
