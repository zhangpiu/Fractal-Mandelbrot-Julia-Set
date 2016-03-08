#pragma once

#include "ximage.h"
#include "Complex.h"

#define WIDTH  1600
#define HEIGHT 1200
#define MAXCOLOR  128

// f(z) = z^2 + C
static const CComplex C_value[] =
{
	{ 0.2850,  0.0100},
	{ 0.4000,  0.3000},
	{ 0.4500, -0.1428},
	{-0.8000,  0.1560},
	{-0.8350, -0.2321},
	{ 0.2850,  0.0000},
	{ 0.2790,  0.0000},
	{ 0.4000,  0.0000},
	{ -0.70176, -0.3842 },
};

static const CString C_name[] =
{
	_T("0.2850+0.0100i"),
	_T("0.4000+0.3000i"),
	_T("0.4500-0.1428i"),
	_T("-0.800+0.1560i"),
	_T("-0.835-0.2321i"),
	_T("0.2850+0.0000i"),
	_T("0.2790+0.0000i"),
	_T("0.4000+0.0000i"),
	_T("-0.70176-0.3842i"),
};

// Iteration number
static const int iter_num[] =
{
	100,
	200,
	500,
	1000,
	2000,
	3000,
	5000,
	10000,
};

// Iteration name
static const CString iter_name[] =
{
	_T("100"),
	_T("200"),
	_T("500"),
	_T("1000"),
	_T("2000"),
	_T("3000"),
	_T("5000"),
	_T("10000"),
};

enum iter_type
{
	iter_100 = 0,
	iter_200,
	iter_500,
	iter_1000,
	iter_2000,
	iter_3000,
	iter_5000,
	iter_10000,
};


class CFractal
{
public:
	CFractal();
	virtual ~CFractal();

	void InitPallette(double h1 = 137.0, double h2 = 30.0);
	COLORREF HSL2RGB(double h, double s, double l);
	
	void SetCoordinate(CRect rectFrame, CRect selArea);

	void SetIteration(int iter = iter_num[iter_1000])
	{
		m_nIteration = iter;
		UpdateImagePixel();
	}

	int GetIteration() const
	{
		return m_nIteration;
	}

	virtual void ResetCoordinate() = 0;
	virtual void CalcImagePixel() = 0;
	void SetImagePixel();
	void UpdateImagePixel(bool bRecalculate = true);

	const CxImage &GetImage() const
	{
		return *m_pxim;
	}

	

protected:
	COLORREF m_crPallette[MAXCOLOR];

	long double m_dFromX;
	long double m_dFromY;
	long double m_dToX;
	long double m_dToY;

	CxImage *m_pxim;
	int m_nIteration;

	int m_matImageInfo[WIDTH][HEIGHT];
};

class CMandelSet : public CFractal
{
public:
	CMandelSet();
	~CMandelSet();

	void ResetCoordinate();
	void CalcImagePixel();

	int GetExponent() const
	{
		return m_nExponent;
	}

	void SetExponent(int n = 2);

private:
	int m_nExponent;
};

class CJuliaSet : public CFractal
{
public:
	CJuliaSet();
	~CJuliaSet();

	void ResetCoordinate();
	void CalcImagePixel();

	CComplex GetConstant() const
	{
		return m_cConstant;
	}

	void SetConstant(const CComplex &c = C_value[0]);

private:
	CComplex m_cConstant;
};

