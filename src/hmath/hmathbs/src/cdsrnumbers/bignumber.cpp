#include <math.h>
#include "bignumber.h"

const long BN_MAX_DEC_DEGREE		=	1000000000;

char	hexChars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

#define HEX_INDICATION _T("0x")

const double MAX_PREC_VALUE	=	999999999999999.0;

//////////////////////////////////////////////////////////////////////

#define GET_CARRY(c, label)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBigNumber::CBigNumber() : m_bits(START_LEN)
{
	m_bits[ 0 ] = 0;
	m_Length = START_LEN;
	m_bitLen = 0;
}

CBigNumber::CBigNumber(BN_TYPE value) : m_bits(START_LEN)
{
	m_bits[ 0 ] = value;
	RecalcSizes();
}

CBigNumber::CBigNumber(const CBigNumber& v)
{
	m_bits.resize(v.GetLength());
	m_bits = v.GetArray();
	m_Length = v.GetLength();
	m_bitLen = v.GetBitLength();
};

CBigNumber::~CBigNumber()
{
	m_bits.resize(0);
}

//////////////////////////////////////////////////////////////////////
//Functions

void CBigNumber::Resize( long newSize )
{
	if(newSize == GetLength()) return;

	if(newSize >= (long)m_bits.capacity())
		m_bits.reserve(newSize + RESERVE_SIZE);
	m_bits.resize(newSize <= 0 ? 1 : newSize);

	if(newSize > GetLength())
	{
		for( register long i = GetLength(); i < newSize; i++)
			m_bits[ i ] = 0;
	}

	RecalcSizes();
};

//////////////////////////////////////////////////////////////////////
// Operators

CBigNumber CBigNumber::operator+( CBigNumber& v )
{
	register long i;
	CBigNumber res;
	CBigNumber* pBigger;
	BN_TYPE tempRes;
	BN_TYPE	accum;
	unsigned long carry = 0, carry2 = 0;
	int maxLength, minLength;

	if(GetLength() >= v.GetLength())
	{ 
		maxLength = GetLength();
		minLength = v.GetLength();
		pBigger =  this;
	}
	else
	{
		maxLength = v.GetLength();
		minLength = GetLength();
		pBigger = (CBigNumber*)&v;
	}

	res.Resize(maxLength);

	carry = 0;
	for( i = 0; i < minLength; i++)
	{
		accum = m_bits[ i ] + carry;
		GET_CARRY( carry, addl1 );
		tempRes = accum + v[ i ];
		GET_CARRY( carry2, addl2 );
		carry |= carry2;
		res[ i ] = tempRes;

		if( i == (maxLength - 1) && carry )
		{
			res.Resize( maxLength + 1 );
			res[ maxLength ] = carry;
		}
	}

	if(maxLength > minLength)
	{
		for( i = minLength; i < maxLength; i++)
		{
			tempRes = (*pBigger)[ i ] + carry;
			GET_CARRY( carry, addl3 );
			res[ i ] = tempRes;
			if(i == (maxLength - 1) && carry)
			{
				res.Resize(maxLength + 1);
				res[maxLength] = carry;
			}
		}
	}

	res.RecalcSizes();

	return  res;
}

CBigNumber CBigNumber::operator=(const CBigNumber& v)
{
	m_bits.resize(v.GetLength());
	m_bits = v.GetArray();
	m_Length = v.GetLength();
	m_bitLen = v.GetBitLength();

	return  *this;
};

CBigNumber	CBigNumber::operator=(BN_TYPE v)
{
	Resize(1);
	m_bits[ 0 ] = v;
	UpdateNumber();
	return *this;
};


CBigNumber CBigNumber::operator-( CBigNumber& v )
{
	CBigNumber res;
	BN_TYPE tempRes;
	BN_TYPE	accum;
	register long i;
	unsigned long carry = 0, carry2 = 0;
	int maxLength, minLength;

	if(*this < v)
		throw "Result of the operation is out of range";

	maxLength = GetLength();
	minLength = v.GetLength();

	res.Resize(maxLength);

	carry = 0;
	for( i = 0; i < minLength; i++ )
	{
		accum = m_bits[ i ] - carry;
		GET_CARRY( carry, subl1 );
		tempRes = accum - v[ i ];
		GET_CARRY( carry2, subl2 );
		carry |= carry2;
		res[ i ] = tempRes;
	}

	if( maxLength > minLength )
	{
		for( i = minLength; i < maxLength; i++)
		{
			tempRes = m_bits[ i ] - carry;
			GET_CARRY( carry, subl3 );
			res[ i ] = tempRes;
		}
	}

	if(carry)
		throw "Result of the operation is out of range";

	res.UpdateNumber();

	return res;
}

CBigNumber	CBigNumber::operator*(CBigNumber& v)
{
	if((GetLength() == 1 && m_bits[ 0 ] == 0) || (v.GetLength() == 1 && v[0] == 0))
		return CBigNumber((BN_TYPE)0);
	CBigNumber		res;
	register long	i, j;
	BN_TYPE			accum;
	BN_TYPE			carry;
	long			len1, len2;
	HALF_BN_TYPE*	pV1	= NULL;
	HALF_BN_TYPE*	pV2	= NULL;
	HALF_BN_TYPE*	pRes	= NULL;


	len1 = GetLength();
	len2 = v.GetLength();
	res.Resize(len1 * len2 + 1);	//set  the result length
	len1 *= 2;						//length in HALF_BN_TYPE
	len2 *= 2;						//length in HALF_BN_TYPE

	pV1 = (HALF_BN_TYPE*) &(m_bits.front());
	pV2 = (HALF_BN_TYPE*) &(v.GetArray().front());
	pRes = (HALF_BN_TYPE*) &(res.GetArray().front());

	carry = 0;
	for(i = 0; i < len1; i++)
	{
		for(j = 0; j < len2; j++)
		{
			accum = ((BN_TYPE)pV1[ i ]) * (BN_TYPE)pV2[j] + pRes[i + j] + carry;
			pRes[i + j] = (HALF_BN_TYPE)accum;
			carry = accum >> BITS_COUNT_IN_HALF_BN_TYPE;
		}

		while(carry)
		{
			accum = (BN_TYPE)pRes[i + j] + carry;
			pRes[i + j] = (HALF_BN_TYPE)accum;
			carry =  accum >> BITS_COUNT_IN_HALF_BN_TYPE;
		}
	}

	res.UpdateNumber();

	return res;
};

CBigNumber CBigNumber::operator/(CBigNumber& v)
{
	CBigNumber c;
	CBigNumber m;

	div(v, c, m);
	return c;
};

CBigNumber CBigNumber::operator%(CBigNumber& v)
{
	CBigNumber c;
	CBigNumber m;

	div(v, c, m);
	return m;
};

bool CBigNumber::operator==(CBigNumber& v)
{
	if(GetLength() != v.GetLength())
		return false;
	return (m_bits == v.GetArray() ? true : false);
};

bool CBigNumber::operator!=(CBigNumber& v)
{
	if(GetLength() != v.GetLength())
		return true;
	return (m_bits == v.GetArray() ? false : true );
};

bool CBigNumber::operator<(CBigNumber& v)
{
	if(GetLength() < v.GetLength())
		return true;
	if(GetLength() > v.GetLength())
		return false;

	register long i;
	CDSRArray<BN_TYPE>& bits = v.GetArray();
	for(i = GetLength() - 1; i >= 0; i--)
	{
		if(m_bits[ i ] > bits[ i ])
			return false;
		else
			if(m_bits[ i ] < bits[ i ])
				return true;
	}

	return false;
};

bool CBigNumber::operator>(CBigNumber& v)
{
	if(GetLength() > v.GetLength())
		return true;
	if(GetLength() < v.GetLength())
		return false;

	register long i;
	CDSRArray<BN_TYPE>& bits = v.GetArray();
	for(i = GetLength() - 1; i >= 0; i--)
	{
		if(m_bits[ i ] < bits[ i ])
			return false;
		else
			if(m_bits[ i ] > bits[ i ])
				return true;
	}

	return false;
};

CBigNumber	CBigNumber::operator >> (int offset)
{
	if(offset == 0)	return *this;
	if(offset < 0)	return ((*this) << abs(offset));

	if(GetLength() == 1 && m_bits[ 0 ] == 0) //if value == 0
		return *this;

	CBigNumber ret;
	BN_TYPE dwOff, carryOff;
	BN_TYPE carry, temp;
	long i, n;

	n = BITS2BN_TYPE(offset); //from bit to BN_TYPE

	if(n >= GetLength()) return ret;

	ret.Resize(GetLength() - n);
	dwOff = offset & MAX_BIT_NUMBER_IN_BN_TYPE;
	carryOff = BITS_COUNT_IN_BN_TYPE - dwOff;
	carry = 0;

	for(i = (GetLength() - 1); i >= n; i--)
	{
		temp = m_bits[ i ];
		ret[i - n] = (temp >> dwOff) + carry;
		if(dwOff)
			carry = temp << carryOff;
		else 
			carry = 0;
	}

	ret.UpdateNumber();
	return ret;
};

CBigNumber	CBigNumber::operator << (int offset)
{
	if(offset == 0)	return *this;
	if(offset < 0)	return ((*this) >> abs(offset));

	if(GetLength() == 1 && m_bits[ 0 ] == 0) //if value == 0
		return *this;

	CBigNumber ret;
	BN_TYPE dwOff, carryOff;
	int i, n;
	BN_TYPE carry;

	n = BITS2BN_TYPE(offset); //from bit to BN_TYPE
	ret.Resize(GetLength() + n + 1);
	dwOff = offset & MAX_BIT_NUMBER_IN_BN_TYPE;
	carryOff = BITS_COUNT_IN_BN_TYPE - dwOff;
	carry = 0;

	for (i = 0; i < GetLength(); i++)
	{
		ret[i + n] = (m_bits[ i ] << dwOff) + carry;
		if (dwOff)
			carry = m_bits[ i ] >> carryOff;
		else 
			carry = 0;
	}

	if(carry)
		ret[i + n] = carry;

	ret.UpdateNumber();
	return ret;
};

//
//////////////////////////////////////////////////////////////////////

void CBigNumber::UpdateNumber()
{
	long index = ((long) m_bits.size())  - 1;

	while( index > 0 && m_bits[ index ] == 0 )
		index--;
	if( (index + 1) != GetLength() )
		m_bits.resize( index + 1 );
	RecalcSizes();
}

void CBigNumber::RecalcSizes()
{ 
	m_Length = (long) m_bits.size(); 
	m_bitLen = m_Length * BITS_COUNT_IN_BN_TYPE;
	while( m_bitLen > 0 && !testbit( m_bitLen - 1 ) )
		m_bitLen--;
};


// c = (*this) / b
// m = (*this) % b
void CBigNumber::div(CBigNumber &b, CBigNumber &c, CBigNumber &m)
{
	if(b == 0) return; // division by zero

	if((*this) < b)
	{
		c = (BN_TYPE)0;
		m = *this;
		return;
	}

	if((*this) == b)
	{
		c = (BN_TYPE)1;
		m = (BN_TYPE)0;
		return;
	}

	CBigNumber temp;
	long k;
	long p;


	p = GetBitLength() - b.GetBitLength();
	m = *this;
	c = (BN_TYPE)0;

	temp = b << p;      // length(ret) = length(d)
	k = temp.GetLength() * BITS_COUNT_IN_BN_TYPE - 1; // k = number of last bit
	c.Resize(GetLength());

	// binary division in (*this) column
	do
	{
		if (temp <= m)
		{
			m = m - temp;
			c[BITS2BN_TYPE(k)] |= ((BN_TYPE)1L) << (k & MAX_BIT_NUMBER_IN_BN_TYPE); // set bit
		}
		p--;
		k--;  				// next bit
		temp.HalfDiv();		// ret=ret>>1
	} while(p >= 0);

	c.UpdateNumber();
	m.UpdateNumber();
	c = c >> (k + 1);  // corect value
}

void CBigNumber::HalfDiv()
{
	CBigNumber temp;
	BN_TYPE carry;
	long i;

	carry = 0;
	temp.Resize(GetLength());
	for (i = GetLength() - 1; i >= 0; i--)
	{
		// *this = *this >> 1;
		temp[ i ] = (m_bits[ i ] >> 1) + (carry << MAX_BIT_NUMBER_IN_BN_TYPE);
		carry = m_bits[ i ] & 1;
	}

	temp.UpdateNumber();
	*this = temp;
}

CBigNumber CBigNumber::SquareRoot()
{
	if(GetLength() < 2 && m_bits[0] < (BN_TYPE)(BN_TYPE_SIGN)MAX_PREC_VALUE)
		return CBigNumber((BN_TYPE)(BN_TYPE_SIGN)sqrt((long double)(BN_TYPE_SIGN)m_bits[0]));

	CBigNumber x0, x1, x, prevX;

	x0 = (*this) >> ((*this).GetBitLength() / 2 + 1);
	x1 = (*this) >> ((*this).GetBitLength() / 2 - 2);

	do
	{
		prevX = x;
		x = x0 + x1;
		x.HalfDiv();
		if(x * x > (*this))
			x1 = x;
		else
			x0 = x;
	}while(x0 != x1 && prevX != x);

	return x0;
}

void CBigNumber::setbit(unsigned long bit, unsigned char value)
{
	long item = (long) BITS2BN_TYPE(bit);

	if( item >= GetLength()) Resize(item + 1);

	if(value)
	{
		m_bits[item] |= (((BN_TYPE)1) << (bit & MAX_BIT_NUMBER_IN_BN_TYPE));
	}
	else
	{
		m_bits[item] &= ~(((BN_TYPE)1) << (bit & MAX_BIT_NUMBER_IN_BN_TYPE));
	}
};
