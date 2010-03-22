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

#if !defined( __FORMULATOR_FMLCORE_BTN_ATTR_H__ )
#define __FORMULATOR_FMLCORE_BTN_ATTR_H__

#include <QDataStream>

#include "HUtils/ixml.h"
#include "HUtils/ihkflag.h"

#include "../fmlcore_global.h"
#include "../mathml/mml_style.h"

/////////////////////////////////////////////////////////////////////

#define HF_FLAG_FSTYLE_UNICODE			0x00000001
#define HF_FLAG_OP_STRETCHY				0x00000002
#define HF_FLAG_OP_IS_FENCE				0x00000004
#define HF_FLAG_OP_IS_ACCENT			0x00000008
#define HF_FLAG_OP_IS_SEPARATOR			0x00000010
#define HF_FLAG_OP_IS_LARGEOP			0x00000020
#define HF_FLAG_OP_IS_SYMMETRIC			0x00000040
#define HF_FLAG_OP_IS_MOVABLELIMITS		0x00000080
#define HF_FLAG_CANNOT_DRAW_TEXT		0x00000100
#define HF_FLAG_ERROR_MISS_BUTTON		0x00000200
#define HF_FLAG_HIDDEN					0x00000400
// node can be edited, but can't be deleted, copied, cutted alone
#define HF_FLAG_CANNOT_BE_MOVED			0x00000800
// node can't be edited, but can be deleted, copied, cutted alone
#define HF_FLAG_CANNOT_BE_EDITED		0x00001000
// read-only node can't be altered in other way as a part of the parent node
// (can't be edited, deleted, copied, cutted alone - but parent can be ...)
#define HF_FLAG_READ_ONLY				(HF_FLAG_CANNOT_BE_MOVED | HF_FLAG_CANNOT_BE_EDITED)

#define HF_FLAG_RWMML_SUPPORT			0x00002000
#define HF_FLAG_OP_FORM_POSTFIX			0x00004000
//
#define HF_FLAG_EXTERNAL_CONTENT		0x00008000
//
#define HF_FLAG_AUTODETECT_CONTENT		0x00010000
#define HF_FLAG_FORCE_RENDER			0x00020000

//
#define HF_FLAG_DISABLED				0x00040000

// somewhere there is a reference to this node that used it for sake of conversion to MathML (content)
#define HF_FLAG_ISREFERENCED				0x00080000
#define HF_FLAG_CONTENT_REFERENCE			0x00100000
#define HF_FLAG_IGNORE_CONTENT_CONVERSION	0x00200000
#define HF_FLAG_OP_FORM_INFIX				0x00400000
#define HF_FLAG_CONTENT_EAT_LEFT_OP			0x00800000
#define HF_FLAG_FRAME_HIDDEN				0x01000000	// we will draw this formula, but without a frame

/////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ FBtnFlags : public HKLongFlag
{
public:
	FBtnFlags() : HKLongFlag()
		{ setRWMMLSupport(); }

	int isMissButton( void ) const { return isSetFlag( HF_FLAG_ERROR_MISS_BUTTON ); }
	void setMissButton( void ){ setFlag( HF_FLAG_ERROR_MISS_BUTTON ); }

	int canBeMoved( void ) const { return !isCannotBeMoved(); }
	int isCannotBeMoved( void ) const { return isSetFlag( HF_FLAG_CANNOT_BE_MOVED ); }
	void setCannotBeMoved( void ){ setFlag( HF_FLAG_CANNOT_BE_MOVED ); }
	void clearCannotBeMoved( void ){ clearFlag( HF_FLAG_CANNOT_BE_MOVED ); }

	int canBeEdited( void ) const { return !isCannotBeEdited(); }
	int isCannotBeEdited( void ) const { return isSetFlag( HF_FLAG_CANNOT_BE_EDITED ); }
	void setCannotBeEdited( void ){ setFlag( HF_FLAG_CANNOT_BE_EDITED ); }
	void clearCannotBeEdited( void ){ clearFlag( HF_FLAG_CANNOT_BE_EDITED ); }

	int isReadOnly( void ) const { return isSetFlag( HF_FLAG_READ_ONLY ); }
	void setReadOnly( void ){ setFlag( HF_FLAG_READ_ONLY ); }
	void clearReadOnly( void ){ clearFlag( HF_FLAG_READ_ONLY ); }

	int isContentEatLeftOp( void ) const { return isSetFlag( HF_FLAG_CONTENT_EAT_LEFT_OP ); }
	void setContentEatLeftOp( void ){ setFlag( HF_FLAG_CONTENT_EAT_LEFT_OP ); }
	void clearContentEatLeftOp( void ){ clearFlag( HF_FLAG_CONTENT_EAT_LEFT_OP ); }

	int isExtNode( void ) const { return isSetFlag( HF_FLAG_EXTERNAL_CONTENT ); }
	void setExtNode( void ){ setFlag( HF_FLAG_EXTERNAL_CONTENT ); }
	void clearExtNode( void ){ clearFlag( HF_FLAG_EXTERNAL_CONTENT ); }

	int isAutodetectContent( void ) const { return isSetFlag( HF_FLAG_AUTODETECT_CONTENT ); }
	void setAutodetectContent( void ){ setFlag( HF_FLAG_AUTODETECT_CONTENT ); }
	void clearAutodetectContent( void ){ clearFlag( HF_FLAG_AUTODETECT_CONTENT ); }

	int isForceRender( void ) const { return isSetFlag( HF_FLAG_FORCE_RENDER ); }
	void setForceRender( void ){ setFlag( HF_FLAG_FORCE_RENDER ); }
	void clearForceRender( void ){ clearFlag( HF_FLAG_FORCE_RENDER ); }
	                              
	int isIgnoreContentConversion( void ) const { return isSetFlag( HF_FLAG_IGNORE_CONTENT_CONVERSION ); }
	void setIgnoreContentConversion( void ){ setFlag( HF_FLAG_IGNORE_CONTENT_CONVERSION ); }
	void clearIgnoreContentConversion( void ){ clearFlag( HF_FLAG_IGNORE_CONTENT_CONVERSION ); }

	int isHidden( void ) const { return isSetFlag( HF_FLAG_HIDDEN ); }
	void setHidden( void ){ setFlag( HF_FLAG_HIDDEN ); }
	void clearHidden( void ){ clearFlag( HF_FLAG_HIDDEN ); }

	int isFrameHidden( void ) const { return isSetFlag( HF_FLAG_FRAME_HIDDEN ); }
	void setFrameHidden( void ){ setFlag( HF_FLAG_FRAME_HIDDEN ); }
	void clearFrameHidden( void ){ clearFlag( HF_FLAG_FRAME_HIDDEN ); }

	int isDisabled( void ) const { return isSetFlag( HF_FLAG_DISABLED ); }
	void setDisabled( void ){ setFlag( HF_FLAG_DISABLED ); }
	void clearDisabled( void ){ clearFlag( HF_FLAG_DISABLED ); }

	int isHiddenNDisabled( void ) const { return isHidden() && isDisabled(); }
	void setHiddenNDisabled( void ){ setHidden(); setDisabled(); }
	void clearHiddenNDisabled( void ){ clearHidden(); clearDisabled(); }

	int isReferenced( void ) const { return isSetFlag( HF_FLAG_ISREFERENCED ); }
	void setReferenced( void ){ setFlag( HF_FLAG_ISREFERENCED ); }
	void clearReferenced( void ){ clearFlag( HF_FLAG_ISREFERENCED ); }

	int isContentReference( void ) const { return isSetFlag( HF_FLAG_CONTENT_REFERENCE ); }
	void setContentReference( void ){ setFlag( HF_FLAG_CONTENT_REFERENCE ); }
	void clearContentReference( void ){ clearFlag( HF_FLAG_CONTENT_REFERENCE ); }

	int isRWMMLSupport( void ) const { return isSetFlag( HF_FLAG_RWMML_SUPPORT ); }
	void setRWMMLSupport( void ){ setFlag( HF_FLAG_RWMML_SUPPORT ); }
	void clearRWMMLSupport( void ){ clearFlag( HF_FLAG_RWMML_SUPPORT ); }

	int isRWMMLPostfix( void ) const { return isSetFlag( HF_FLAG_OP_FORM_POSTFIX ); }
	void setRWMMLPostfix( void ){ clearRWMMLInfix(); setFlag( HF_FLAG_OP_FORM_POSTFIX ); }
	void clearRWMMLPostfix( void ){ clearFlag( HF_FLAG_OP_FORM_POSTFIX ); }

	int isRWMMLPrefix( void ) const { return !isSetFlag( HF_FLAG_OP_FORM_POSTFIX ) && !isSetFlag( HF_FLAG_OP_FORM_INFIX ); }
	void setRWMMLPrefix( void ){ clearRWMMLPostfix(); clearRWMMLInfix(); }

	int isRWMMLInfix( void ) const { return !isRWMMLPostfix() && isSetFlag( HF_FLAG_OP_FORM_INFIX ); }
	void setRWMMLInfix( void ){ clearRWMMLPostfix(); setFlag( HF_FLAG_OP_FORM_INFIX ); }
	void clearRWMMLInfix( void ){ clearFlag( HF_FLAG_OP_FORM_INFIX ); }

	int isUnicode( void ) const { return isSetFlag( HF_FLAG_FSTYLE_UNICODE ); }
	void setUnicode( void ){ setFlag( HF_FLAG_FSTYLE_UNICODE ); }
	void clearUnicode( void ){ clearFlag( HF_FLAG_FSTYLE_UNICODE ); }

	int isStretchy( void ) const { return isSetFlag( HF_FLAG_OP_STRETCHY ); }
	void setStretchy( void ){ setFlag( HF_FLAG_OP_STRETCHY ); }
	void clearStretchy( void ){ clearFlag( HF_FLAG_OP_STRETCHY ); }

	int isFence( void ) const { return isSetFlag( HF_FLAG_OP_IS_FENCE ); }
	void setFence( void ){ setFlag( HF_FLAG_OP_IS_FENCE ); }
	void clearFence( void ){ clearFlag( HF_FLAG_OP_IS_FENCE ); }

	int isSymmetric( void ) const { return isSetFlag( HF_FLAG_OP_IS_SYMMETRIC ); }
	void setSymmetric( void ){ setFlag( HF_FLAG_OP_IS_SYMMETRIC ); }
	void clearSymmetric( void ){ clearFlag( HF_FLAG_OP_IS_SYMMETRIC ); }

	int isLargeOp( void ) const { return isSetFlag( HF_FLAG_OP_IS_LARGEOP ); }
	void setLargeOp( void ){ setFlag( HF_FLAG_OP_IS_LARGEOP ); }
	void clearLargeOp( void ){ clearFlag( HF_FLAG_OP_IS_LARGEOP ); }

	int isCannotDrawText( void ) const { return isSetFlag( HF_FLAG_CANNOT_DRAW_TEXT ); }
	void setCannotDrawText( void ){ setFlag( HF_FLAG_CANNOT_DRAW_TEXT ); }
};


/////////////////////////////////////////////////////////////////////

inline
QDataStream& operator << ( QDataStream& ar, const FBtnFlags& lp )
{
	return ar << (const HKLongFlag&) lp;
}

inline
QDataStream& operator >> ( QDataStream& ar, FBtnFlags& lp )
{
	return ar >> (HKLongFlag&) lp;
}

/////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ FBtnAttr : public FBtnFlags
{
protected:
	HMathMLAttr	m_mmlAttr;

public:
	FBtnAttr() : FBtnFlags() {}
	virtual ~FBtnAttr() {}

	void setMathMLAttr( const HMathMLAttr& a );
	const HMathMLAttr& getMathMLAttr( void );
	const FBtnAttr& operator = ( const FBtnAttr& v );

	void setUnfilteredAttr( QString key, QString value )
		{ m_mmlAttr.setUnfilteredAttr( key, value ); }
	void setUnfilteredAttr( const HMathMLAttr& a )
		{ m_mmlAttr.setUnfilteredAttr( a ); }
	void printUnfilteredAttr( QString& dest )
		{ m_mmlAttr.printUnfilteredAttr( dest ); }

	void setLSpace( double _lspace )
		{ m_mmlAttr.lspace = (RealFde) _lspace; }
	void setRSpace( double _rspace )
		{ m_mmlAttr.rspace = (RealFde) _rspace; }
	RealFde getLSpace() const
		{ return m_mmlAttr.lspace; }
	RealFde getRSpace() const
		{ return m_mmlAttr.rspace; }
	void setMMLWidth( short val )
		{ m_mmlAttr.width = val; }
	short getMMLWidth() const
		{ return m_mmlAttr.width; }
	void setMMLHeight( short val )
		{ m_mmlAttr.height = val; }
	short getMMLHeight() const
		{ return m_mmlAttr.height; }
	void setMMLDepth( short val )
		{ m_mmlAttr.depth = val; }
	short getMMLDepth() const
		{ return m_mmlAttr.depth; }

	virtual int isAble2Glue() const;
	void setColorAttr( const HMathMLAttr& a );
	void readUnfilteredAttr( AST_TXML_Tag *xml_node );

	friend QDataStream& operator << ( QDataStream& ar, const FBtnAttr& lp );
	friend QDataStream& operator >> ( QDataStream& ar, FBtnAttr& lp );
};

/////////////////////////////////////////////////////////////////////

inline
QDataStream& operator << ( QDataStream& ar, const FBtnAttr& lp )
{
	return ar << (const FBtnFlags&) lp << lp.m_mmlAttr;
}

inline
QDataStream& operator >> ( QDataStream& ar, FBtnAttr& lp )
{
	return ar >> (FBtnFlags&) lp >> lp.m_mmlAttr;
}

/////////////////////////////////////////////////////////////////////

inline
const FBtnAttr& FBtnAttr::operator = ( const FBtnAttr& v )
{
	setValue( v.getValue() );
	m_mmlAttr = v.m_mmlAttr;
	return *this;
}

inline
int FBtnAttr::isAble2Glue() const
{
	long issupport = HF_FLAG_RWMML_SUPPORT;
	return (getValue() & ~issupport || !m_mmlAttr.isAble2Glue()) ? 0 : 1;
}

inline
void FBtnAttr::readUnfilteredAttr( AST_TXML_Tag *xml_node )
{
	struct HMathMLAttr mml_attr( xml_node->getTagName() );
	mml_attr.set( xml_node );
	setUnfilteredAttr( mml_attr );
}

/////////////////////////////////////////////////////////////////////

#endif
