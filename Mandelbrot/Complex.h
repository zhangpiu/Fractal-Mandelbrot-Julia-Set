#pragma once
class CComplex
{
public:
	~CComplex(){};

	CComplex(long double real = 0, long double imag = 0)
		: m_real(real)
		, m_imag(imag)
	{}

	CComplex& operator +=(const CComplex &rhs)
	{
		this->m_real += rhs.m_real;
		this->m_imag += rhs.m_imag;

		return *this;
	}

	CComplex& operator -=(const CComplex &rhs)
	{
		this->m_real -= rhs.m_real;
		this->m_imag -= rhs.m_imag;

		return *this;
	}

	friend CComplex operator +(const CComplex &lhs, const CComplex &rhs);
	friend CComplex operator -(const CComplex &lhs, const CComplex &rhs);
	friend CComplex operator *(const CComplex &lhs, const CComplex &rhs);
	friend bool operator ==(const CComplex &lhs, const CComplex &rhs);
	
	inline friend double modulus_square(const CComplex & c)
	{
		return c.m_real * c.m_real + c.m_imag * c.m_imag;
	}

	inline friend CComplex complex_power(const CComplex &c, unsigned int n)
	{
		CComplex result(1);

		for (unsigned int i = 0; i < n; ++i)
			result = result * c;

		return result;
	}


	void setReal(long double real)
	{
		this->m_real = real;
	}

	void setImag(long double imag)
	{
		this->m_imag = imag;
	}


private:
	long double m_real;
	long double m_imag;
};

