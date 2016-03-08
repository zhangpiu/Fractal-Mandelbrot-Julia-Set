#include "stdafx.h"
#include "Fractal.h"


CFractal::CFractal()
	: m_pxim(nullptr)
	, m_nIteration(iter_num[iter_1000])
{
	m_pxim = new CxImage(WIDTH, HEIGHT, 24);
	memset(m_matImageInfo, 0, WIDTH * HEIGHT * sizeof(int));
	InitPallette();
}

CFractal::~CFractal()
{
	if (m_pxim)
	{
		delete m_pxim;
	}
}


/*---------------------------------------------------------------------------------/
| function:      SetCoordinate
| description:   Recalculate the coordinate according to the selected area
|
| note:          [10/28/2015 vodka]
|---------------------------------------------------------------------------------*/
void CFractal::SetCoordinate(CRect rectFrame, CRect selArea)
{
	long double cx = m_dToX - m_dFromX;
	long double cy = m_dToY - m_dFromY;
	long double oldFromX = m_dFromX;
	long double oldFromY = m_dFromY;

	m_dFromX = oldFromX + cx * selArea.left / static_cast<long double>(rectFrame.Width());
	m_dToX   = oldFromX + cx * selArea.right / static_cast<long double>(rectFrame.Width());
	
	m_dFromY = oldFromY + cy * selArea.top / static_cast<double>(rectFrame.Height());
	m_dToY   = oldFromY + cy * selArea.bottom / static_cast<double>(rectFrame.Height());
}


/*---------------------------------------------------------------------------------/
| function:      HSL2RGB
| description:   Convert HSL space to RGB space.
|
| note:          [10/28/2015 vodka]
|---------------------------------------------------------------------------------*/
COLORREF CFractal::HSL2RGB(double h, double s, double l)
{
	const double C = (1 - fabs(2 * l - 1)) * s; // chroma
	const double H = h / 60;
	const double X = C * (1 - fabs(fmod(H, 2) - 1));
	double rgb1[3] = { 0 };

	if (H > 0 && H < 1)  rgb1[0] = C, rgb1[1] = X, rgb1[2] = 0;
	else if (H >= 1 && H < 2) rgb1[0] = X, rgb1[1] = C, rgb1[2] = 0;
	else if (H >= 2 && H < 3) rgb1[0] = 0, rgb1[1] = C, rgb1[2] = X;
	else if (H >= 3 && H < 4) rgb1[0] = 0, rgb1[1] = X, rgb1[2] = C;
	else if (H >= 4 && H < 5) rgb1[0] = X, rgb1[1] = 0, rgb1[2] = C;
	else if (H >= 5 && H < 6) rgb1[0] = C, rgb1[1] = 0, rgb1[2] = X;
	else                      rgb1[0] = 0, rgb1[1] = 0, rgb1[2] = 0;

	const double m = l - 0.5 * C;

	return RGB((rgb1[0] + m) * 255, (rgb1[1] + m) * 255, (rgb1[2] + m) * 255);
}


/*---------------------------------------------------------------------------------/
| function:      InitPallette
| description:   Init the pallette.
|
| note:          [10/28/2015 vodka]
|---------------------------------------------------------------------------------*/
void CFractal::InitPallette(double h1 /* = 137.0 */, double h2 /* = 30.0 */)
{
	for (int i = 0; i < MAXCOLOR / 2; ++i)
	{
		m_crPallette[i] = HSL2RGB(h1, 1.0, i * 2.0 / double(MAXCOLOR));
		m_crPallette[MAXCOLOR - 1 - i] = HSL2RGB(h2, 1.0, i * 2.0 / double(MAXCOLOR));
	}
}

/*---------------------------------------------------------------------------------/
| function:      SetImagePixel
| description:   
|
| note:          [10/29/2015 vodka]
|---------------------------------------------------------------------------------*/
void CFractal::SetImagePixel()
{
	if (!m_pxim) return;

	CxImage *pxim = m_pxim;

	int width = pxim->GetWidth();
	int height = pxim->GetHeight();

#ifndef _DEBUG
#pragma omp parallel for
#endif
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			COLORREF color = (m_matImageInfo[j][i] == m_nIteration) ? 0 : m_crPallette[m_matImageInfo[j][i] % MAXCOLOR];
			pxim->info.pImage[(height - 1 - i) * pxim->info.dwEffWidth + j * 3 + 2] = GetRValue(color);
			pxim->info.pImage[(height - 1 - i) * pxim->info.dwEffWidth + j * 3 + 1] = GetGValue(color);
			pxim->info.pImage[(height - 1 - i) * pxim->info.dwEffWidth + j * 3 + 0] = GetBValue(color);
		}
	}
}

/*---------------------------------------------------------------------------------/
| function:      UpdateImagePixel
| description:   Calculate the pixel color.
|
| note:          [10/29/2015 vodka]
|---------------------------------------------------------------------------------*/
void CFractal::UpdateImagePixel(bool bRecalculate /* = true */)
{
	if (bRecalculate)
	{
		CalcImagePixel();
	}
	
	SetImagePixel();
}


//////////////////////////////////////////////////////////////////////////
CMandelSet::CMandelSet()
	: m_nExponent(2)
{
	ResetCoordinate();
}

CMandelSet::~CMandelSet()
{

}

void CMandelSet::ResetCoordinate()
{
	m_dFromX = -2.1;
	m_dToX = 1.1;
	m_dFromY = -1.2;
	m_dToY = 1.2;
}

void CMandelSet::SetExponent(int n /* = 2 */)
{
	m_nExponent = n;
	ResetCoordinate();
	UpdateImagePixel();
}


/*---------------------------------------------------------------------------------/
| function:      CalcImagePixel
| description:   Calculate the pixel color.
|
| note:          [10/29/2015 vodka]
|---------------------------------------------------------------------------------*/
void CMandelSet::CalcImagePixel()
{
	if (!m_pxim) return;

	CxImage *pxim = m_pxim;

	int width = pxim->GetWidth();
	int height = pxim->GetHeight();

#ifndef _DEBUG
#pragma omp parallel for
#endif
	for (int i = 0; i < height; ++i)
	{
		CComplex C(0, m_dFromY + (m_dToY - m_dFromY) * i / static_cast<long double>(height));

		for (int j = 0; j < width; ++j)
		{
			C.setReal(m_dFromX + (m_dToX - m_dFromX) * j / static_cast<long double>(width));

			CComplex Z;
			int k = 0;
			for (k = 0; k < m_nIteration; ++k)
			{
				if (modulus_square(Z) > 4.0) break;
				// f(z) = z^n + C;
				Z = complex_power(Z, m_nExponent) + C;
			}

			m_matImageInfo[j][i] = k;
		}
	}
}


CJuliaSet::CJuliaSet()
	: m_cConstant(C_value[0])
{
	ResetCoordinate();
}

CJuliaSet::~CJuliaSet()
{

}

void CJuliaSet::ResetCoordinate()
{
	m_dFromX = -1.6;
	m_dToX = 1.6;
	m_dFromY = -1.2;
	m_dToY = 1.2;
}



/*---------------------------------------------------------------------------------/
| function:      CalcImagePixel
| description:   Calculate the pixel color.
|
| note:          [10/29/2015 vodka]
|---------------------------------------------------------------------------------*/
void CJuliaSet::CalcImagePixel()
{
	if (!m_pxim) return;

	CxImage *pxim = m_pxim;

	int width = pxim->GetWidth();
	int height = pxim->GetHeight();

#ifndef _DEBUG
#pragma omp parallel for
#endif
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			CComplex Z(m_dFromX + (m_dToX - m_dFromX) * j / static_cast<long double>(width),
				m_dFromY + (m_dToY - m_dFromY) * i / static_cast<long double>(height));

			int k = 0;
			for (k = 0; k < m_nIteration; ++k)
			{
				if (modulus_square(Z) > 4.0) break;
				Z = Z * Z + m_cConstant;
			}

			m_matImageInfo[j][i] = k;
		}
	}
}

void CJuliaSet::SetConstant(const CComplex &c /* = C_value[0] */)
{
	m_cConstant = c;
	ResetCoordinate();
	UpdateImagePixel();
}