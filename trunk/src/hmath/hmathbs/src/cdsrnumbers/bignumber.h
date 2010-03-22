/****************************************************************************
**
** Copyright (C) 2010 Andriy Kovalchuk, Vyacheslav Levytskyy,
** Igor Samolyuk, Valentyn Yanchuk (aka "Hermitech Laboratory")
**
** All rights reserved.
** Contact: Hermitech Laboratory (info@mmlsoft.com)
**
** This file is a part of the Formulator MathML Editor project
** (http://www.mmlsoft.com).
**
** Commercial Usage
** Licensees holding valid Formulator Commercial licenses may use this 
** file in accordance with the Formulator Commercial License Agreement 
** provided with the Software or, alternatively, in accordance with the 
** terms contained in a written agreement between you and 
** Hermitech Laboratory.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Hermitech Laboratory at info@mmlsoft.com.
**
****************************************************************************/

#if !defined(__COMMON_BIGNUMBER_H__)
#define __COMMON_BIGNUMBER_H__

#include "../hmathbs_global.h"
#include "../cdsrarrays/cdsrarray.h"

typedef quint64 BN_TYPE;
typedef qint64  BN_TYPE_SIGN;
typedef unsigned long		HALF_BN_TYPE;
const BN_TYPE	START_LEN						= 1,
                                CARRY_FLAG				= 0x01,
                                HEX_BITS_PER_NUMBER			= 4,
				HEX_NUMB_PER_BN_TYPE			= sizeof(BN_TYPE) * 2,
				BITS_COUNT_IN_BN_TYPE			= sizeof(BN_TYPE) * 8,
				MAX_BIT_NUMBER_IN_BN_TYPE		= BITS_COUNT_IN_BN_TYPE - 1,
				BITS_COUNT_IN_HALF_BN_TYPE		= BITS_COUNT_IN_BN_TYPE / 2,
                                RESERVE_SIZE				= 0;

#define	BITS2BN_TYPE(bits) (bits >> 6)

class MATHBASE_API CBigNumber  
{
public:
	CBigNumber();
	CBigNumber(BN_TYPE value);
	CBigNumber(const CBigNumber& v);
	virtual ~CBigNumber();
	
	void Resize(long newSize);
	void RecalcSizes();
	
	
	void UpdateNumber();
	void div(CBigNumber& b, CBigNumber& c, CBigNumber& m);
	void HalfDiv();
	BN_TYPE testbit(unsigned long bit) {return m_bits[BITS2BN_TYPE(bit)] & (((BN_TYPE) 1L) << (bit & MAX_BIT_NUMBER_IN_BN_TYPE));};
	BN_TYPE _testbit(unsigned long bit) {return (m_bits[BITS2BN_TYPE(bit)] >> (bit & MAX_BIT_NUMBER_IN_BN_TYPE)) & 1L;};
	void setbit(unsigned long bit, unsigned char value = 1);

protected:
	long m_bitLen;				//длина в битах
	long m_Length;				//длина в BN_TYPE
	CDSRArray<BN_TYPE> m_bits;	//само число
	//std::string m_forReturn;

public :
	long GetLength() const { return m_Length; };
	long GetBitLength() const { return m_bitLen; };
	CDSRArray<BN_TYPE>& GetArray() const { return (CDSRArray<BN_TYPE>&)m_bits; };
	//std::string ConvertToStringHex();
	CBigNumber SquareRoot();

public:
	CBigNumber	operator+(CBigNumber& v);
	CBigNumber	operator+(BN_TYPE v)
	{
		CBigNumber op2(v);
		return (*this + op2);
	}
	CBigNumber	operator-(CBigNumber& v);
	CBigNumber	operator-(BN_TYPE v)
	{
		CBigNumber op2(v);
		return (*this - op2);
	}
	CBigNumber	operator*(CBigNumber& v);
	CBigNumber	operator*(BN_TYPE v)
	{
		CBigNumber op2(v);
		return (*this * op2);
	}
	CBigNumber	operator/(CBigNumber& v);
	CBigNumber	operator/(BN_TYPE v)
	{
		CBigNumber op2(v);
		return (*this / op2);
	}
	CBigNumber	operator%(CBigNumber& v);
	CBigNumber	operator%(BN_TYPE v)
	{
		CBigNumber op2(v);
		return (*this % op2);
	}
	CBigNumber	operator=(const CBigNumber& v);
	CBigNumber	operator=(BN_TYPE v);
	BN_TYPE&	operator[](long index) { return m_bits[ index ]; };

	CBigNumber	operator++() { return (*this = *this + 1); };
	CBigNumber	operator++(int) { CBigNumber res(*this); *this = *this + 1; return res; };
	CBigNumber	operator--() { return (*this = *this - 1); };
	CBigNumber	operator--(int) { CBigNumber res(*this); *this = *this - 1; return res; };

	CBigNumber	operator+=(CBigNumber& v) { return(*this = *this + v); };
	CBigNumber	operator+=(BN_TYPE v) { return(*this = *this + v); };
	CBigNumber	operator-=(CBigNumber& v) { return(*this = *this - v); };
	CBigNumber	operator-=(BN_TYPE v) { return(*this = *this - v); };
	CBigNumber	operator*=(CBigNumber& v) { return(*this = *this * v); };
	CBigNumber	operator*=(BN_TYPE v) { return(*this = *this * v); };
	
	bool		operator==(CBigNumber& v);
	bool		operator==(BN_TYPE v)
	{
		CBigNumber op2(v);
		return (*this == op2);
	}
	bool		operator!=(CBigNumber& v);
	bool		operator!=(BN_TYPE v)
	{
		CBigNumber op2(v);
		return (*this != op2);
	}
	bool		operator< (CBigNumber& v);
	bool		operator< (BN_TYPE v)
	{
		CBigNumber op2(v);
		return (*this < op2);
	}
	bool		operator> (CBigNumber& v);
	bool		operator> (BN_TYPE v)
	{
		CBigNumber op2(v);
		return (*this > op2);
	}
	bool		operator<=(CBigNumber& v) { return (*this < v || *this == v);};
	bool		operator<=(BN_TYPE v)
	{
		CBigNumber op2(v);
		return (*this <= op2);
	}
	bool		operator>=(CBigNumber& v) { return (*this > v || *this == v);};
	bool		operator>=(BN_TYPE v)
	{
		CBigNumber op2(v);
		return (*this >= op2);
	}
	
	CBigNumber	operator >> (int offset);
	CBigNumber	operator << (int offset);
};

#endif
