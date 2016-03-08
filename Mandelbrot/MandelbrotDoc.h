// MandelbrotDoc.h : interface of the CMandelbrotDoc class
//


#pragma once


class CMandelbrotDoc : public CDocument
{
protected: // create from serialization only
	CMandelbrotDoc();
	DECLARE_DYNCREATE(CMandelbrotDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CMandelbrotDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


