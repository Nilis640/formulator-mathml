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

#if !defined( __FORMULATOR_NODEINFO_H__ )
#define __FORMULATOR_NODEINFO_H__

#include "../../button/xmlbtn.h"
#include "nodeinfo_property.h"

/////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CNodeInfo_Alignment
{
protected:
	qint32 type, value;
public:
	CNodeInfo_Alignment( void )
		{ type = FBtnChildPos::Default; value = 0; }
	void setNodeInfo_Alignment( const CNodeInfo_Alignment& a )
		{ type = a.type; value = a.value; }
	void setSpecialVerticalCenter( long _type, long _value )
		{ type = _type; value = _value; }
	long getAlignmentType( void ) const
		{ return type; }
	long getAlignmentValue( void ) const
		{ return value; }

	friend QDataStream& operator << ( QDataStream& ar, const CNodeInfo_Alignment& lp );
	friend QDataStream& operator >> ( QDataStream& ar, CNodeInfo_Alignment& lp );
};

/////////////////////////////////////////////////////////////////////

inline
QDataStream& operator << ( QDataStream& ar, const CNodeInfo_Alignment& lp )
{
	return ar << lp.type << lp.value;
}

inline
QDataStream& operator >> ( QDataStream& ar, CNodeInfo_Alignment& lp )
{
	return ar >> lp.type >> lp.value;
}

/////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CNodeInfo_MathMLFormat
{
public:
	FBtnMMLData to_mathml_data;
public:
	void setMathMLFormat( const FBtnMMLData& _to_mathml_data )
		{ to_mathml_data = _to_mathml_data; }
	const FBtnMMLData& getMathMLFormat() const
		{ return to_mathml_data; }
	void setNodeInfo_MathMLFormat( const CNodeInfo_MathMLFormat& a )
		{ setMathMLFormat( a.getMathMLFormat() ); }
	void setNodeInfo_setMathMLNodeName( QString aname )
		{ to_mathml_data.name = aname; }
};

/////////////////////////////////////////////////////////////////////

inline
QDataStream& operator << ( QDataStream& ar, const CNodeInfo_MathMLFormat& lp )
{
	return ar << lp.to_mathml_data;
}

inline
QDataStream& operator >> ( QDataStream& ar, CNodeInfo_MathMLFormat& lp )
{
	return ar >> lp.to_mathml_data;
}

/////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CNodeInfo
	: public CNodeInfo_MathMLFormat
	, public CNodeInfo_Alignment
	, public CNodeInfo_Property
{
public:
	CNodeInfo( void ) {}
	CNodeInfo( CNodeInfo *pNodeInfo )
		{ setNodeInfo( *pNodeInfo ); }
	int isAble2Glue() const
		{ return CNodeInfo_Property::isAble2Glue(); }
	const CNodeInfo& operator = ( const CNodeInfo& a )
		{ setNodeInfo( a ); return *this; }
	virtual void setNodeInfo( const CNodeInfo& a )
	{ 
		setNodeInfo_MathMLFormat( a );
		setNodeInfo_Alignment( a );
		setNodeInfo_Property( a );
	}
};

/////////////////////////////////////////////////////////////////////

inline
QDataStream& operator << ( QDataStream& ar, const CNodeInfo& lp )
{
	return ar << (const CNodeInfo_MathMLFormat&) lp << (const CNodeInfo_Alignment&) lp << (const CNodeInfo_Property&) lp;
}

inline
QDataStream& operator >> ( QDataStream& ar, CNodeInfo& lp )
{
	return ar >> (CNodeInfo_MathMLFormat&) lp >> (CNodeInfo_Alignment&) lp >> (CNodeInfo_Property&) lp;
}

/////////////////////////////////////////////////////////////////////

#endif
