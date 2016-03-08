#include "stdafx.h"
#include "Complex.h"

CComplex operator +(const CComplex &lhs, const CComplex &rhs)
{
	CComplex result(lhs);
	result += rhs;

	return result;
}

CComplex operator -(const CComplex &lhs, const CComplex &rhs)
{
	CComplex result(lhs);
	result -= rhs;

	return result;
}

CComplex operator *(const CComplex &lhs, const CComplex &rhs)
{
	long double A = (lhs.m_real + lhs.m_imag) * rhs.m_real;
	long double B = (rhs.m_real + rhs.m_imag) * lhs.m_imag;
	long double C = (lhs.m_imag - lhs.m_real) * rhs.m_imag;

	CComplex result(A - B, B - C);

	return result;
}

bool operator ==(const CComplex &lhs, const CComplex &rhs)
{
	return lhs.m_real == rhs.m_real && lhs.m_imag == rhs.m_imag;
}