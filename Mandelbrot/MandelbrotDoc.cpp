// MandelbrotDoc.cpp : implementation of the CMandelbrotDoc class
//

#include "stdafx.h"
#include "Mandelbrot.h"

#include "MandelbrotDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMandelbrotDoc

IMPLEMENT_DYNCREATE(CMandelbrotDoc, CDocument)

BEGIN_MESSAGE_MAP(CMandelbrotDoc, CDocument)
END_MESSAGE_MAP()


// CMandelbrotDoc construction/destruction

CMandelbrotDoc::CMandelbrotDoc()
{
	// TODO: add one-time construction code here

}

CMandelbrotDoc::~CMandelbrotDoc()
{
}

BOOL CMandelbrotDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMandelbrotDoc serialization

void CMandelbrotDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CMandelbrotDoc diagnostics

#ifdef _DEBUG
void CMandelbrotDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMandelbrotDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMandelbrotDoc commands
