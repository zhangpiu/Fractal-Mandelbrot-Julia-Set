// MandelbrotView.h : interface of the CMandelbrotView class
//


#pragma once
#include "MandelbrotDoc.h"
#include "Fractal.h"
#include "MainFrm.h"

class CMandelbrotView : public CBCGPFormView
{
protected: // create from serialization only
	CMandelbrotView();
	DECLARE_DYNCREATE(CMandelbrotView)

public:
	enum{ IDD = IDD_MANDELBROT_FORM };

// Attributes
public:
	CMandelbrotDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CMandelbrotView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	//afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	afx_msg void OnFractalMandelbrotSet();
	afx_msg void OnFractalJuliaSet();
	afx_msg void OnFractalReset();
	
	afx_msg void OnMandelExponent();
	afx_msg void OnJuliaConstant();
	
	afx_msg void OnDummy();

	afx_msg void OnChangeIteration();
	afx_msg void OnChangeColor();

	DECLARE_MESSAGE_MAP()

private:
	CFractal   *m_pFractal;
	CMandelSet *m_pMandelbrot;
	CJuliaSet  *m_pJulia;

	int m_nImageOrgX;
	int m_nImageOrgY;
	int m_nImageWidth;
	int m_nImageHeight;

	CPoint m_ptRectStart;
	CPoint m_ptRectEnd;

	CxImage m_ximorg;
	CxImage m_xim;
	

public:
	virtual void OnDraw(CDC* pDC);
	afx_msg void OnFileSaveAs();

private:
	void DrawRectangle(CDC *pDC);
	void DrawImage(CDC *pDC);
	void DocToClient(CRect& rect);

	void UpdateFractalImage();

	CMainFrame* GetMainFrame() const
	{
		return static_cast<CMainFrame *>(AfxGetMainWnd());
	}

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#ifndef _DEBUG  // debug version in MandelbrotView.cpp
inline CMandelbrotDoc* CMandelbrotView::GetDocument() const
   { return reinterpret_cast<CMandelbrotDoc*>(m_pDocument); }
#endif
