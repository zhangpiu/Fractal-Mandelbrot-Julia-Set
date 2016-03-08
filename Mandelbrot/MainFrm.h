// MainFrm.h : interface of the CMainFrame class
//


#pragma once

class CMandelbrotView;

class CMainFrame : public CBCGPFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
	CBCGPSplitterWnd m_wndSplitter;
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
	CMandelbrotView* GetRightPane();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CBCGPRibbonStatusBar	m_wndStatusBar;
	CBCGPRibbonBar			m_wndRibbonBar;
	CBCGPRibbonMainButton	m_MainButton;
	CBCGPToolBarImages		m_PanelImages;
	CBCGPMenuBar			m_wndMenuBar;
	CBCGPToolBar			m_wndToolBar;
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnRibbonCustomize (WPARAM wp, LPARAM lp);
	afx_msg void OnToolsOptions();
	DECLARE_MESSAGE_MAP()

	BOOL CreateRibbonBar ();
	void ShowOptions (int nPage);

public:
	void InitPanel();
	void AddMandelPanel();
	void AddJuliaPanel();
	void AddTwoColorSliders(CBCGPRibbonPanel *pPanelFractal);

	CBCGPBaseRibbonElement* GetRibbonElement(UINT uiCmdID) const
	{
		return m_wndRibbonBar.FindByID(uiCmdID);
	}

// 	CMandelbrotView* GetView() const
// 	{
// 		return static_cast<CMandelbrotView*>(GetActiveView());
// 	}
};


