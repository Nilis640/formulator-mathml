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

#if !defined( __COMMON_LONGFLAG_H__ )
#define __COMMON_LONGFLAG_H__

#include <QDataStream>

template<class T> class HKFlag
{
protected:
	T __flag_value;
public:
	HKFlag( T init = 0 )
	{	resetFlag( init );				}
	T isSetFlag( T flags ) const
	{	return __flag_value & flags;	}
	void setFlag( T flags )
	{	__flag_value |= flags;			}
	void clearFlag( T flags )
	{	__flag_value &= ~flags;			}
	void resetFlag( T init = 0 )
	{	__flag_value = init;			}
	T getValue( void ) const
	{	return __flag_value;			}
	void setValue( T s )
	{	__flag_value = s;				}

	T operator = ( const T v )
	{	return __flag_value = v;	}
	T operator = ( const HKFlag& flag )
	{	return __flag_value = flag.__flag_value;	}
	int operator == ( const HKFlag& flag )
	{	return __flag_value == flag.__flag_value;	}
	int operator != ( const HKFlag& flag )
	{	return __flag_value != flag.__flag_value;	}
};

typedef HKFlag<qint32>					HKLongFlag;

inline
QDataStream& operator << ( QDataStream& ar, const HKLongFlag& lp )
{
	return ar << lp.getValue();
}

inline
QDataStream& operator >> ( QDataStream& ar, HKLongFlag& lp )
{
	qint32 v;
	QDataStream& ret = ar >> v;
	lp.setValue( v );
	return ret;
}

typedef HKFlag<qint16>					HKShortFlag;

inline
QDataStream& operator << ( QDataStream& ar, const HKShortFlag& lp )
{
	return ar << lp.getValue();
}

inline
QDataStream& operator >> ( QDataStream& ar, HKShortFlag& lp )
{
	qint16 v;
	QDataStream& ret = ar >> v;
	lp.setValue( v );
	return ret;
}

#endif
