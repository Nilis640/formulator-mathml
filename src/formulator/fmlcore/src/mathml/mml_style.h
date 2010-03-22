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

#if !defined( __COMMON_LANGUAGE_MATHML_FORMAT_MML_STYLE_H__ )
#define __COMMON_LANGUAGE_MATHML_FORMAT_MML_STYLE_H__

#include <vector>
#include <QDataStream>

#include "../fmlcore_global.h"
#include "HUtils/ihkflag.h"
#include "HUtils/ihkstring.h"
#include "HUtils/iustring.h"
#include "HUtils/ixml.h"
#include "HUtils/ihkmap.h"
#include "HUtils/idrawstruct.h"

//#include "mml_defs.h"
//#include "mml_operator.h"

class HMathOpAttr;

///////////////////////////////////////////////////////////////////////////////

#define MML_FSTYLE_FIXED			1
#define MML_FSTYLE_VARIABLE			2

#define MML_MSPACE_WIDTH_DEFAULT	0
#define MML_MSPACE_HEIGHT_DEFAULT	0
#define MML_MSPACE_DEPTH_DEFAULT	0

#define MML_MATHSIZE_MIN			5.0
#define MML_MATHSIZE_MAX			127.0

#define MML_MATHSIZE_SMALL			0
#define MML_MATHSIZE_NORMAL			1
#define MML_MATHSIZE_BIG			2

#define MML_MATHVARIANT_NUMBER		11

#define HF_FLAG_MO_FORM_LO			0x00000001
#define HF_FLAG_MO_FORM_HI			0x00000002
#define HF_FLAG_MO_FORM_MASK		0x00000003
#define HF_FLAG_MO_FENCE			0x00000004
#define HF_FLAG_MO_ACCENT			0x00000008
#define HF_FLAG_MO_SEPARATOR		0x00000010
#define HF_FLAG_MO_LARGEOP			0x00000020
#define HF_FLAG_MO_SYMMETRIC		0x00000040
#define HF_FLAG_MO_MOVABLELIMITS	0x00000080
#define HF_FLAG_MO_STRETCHY			0x00000100

///////////////////////////////////////////////////////////////////////////

#define MML_MPADDED_UNITS_LEN			4

#define MML_MPADDED_UNITS_WIDTH			0
#define MML_MPADDED_UNITS_LSPACE		1
#define MML_MPADDED_UNITS_HEIGHT		2
#define MML_MPADDED_UNITS_DEPTH			3

QString mml_getMPaddedUnitName( int i );

///////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ HMathMLAttr
{
public:
	HKLongFlag mo_flag;
	RealFde lspace, rspace;
	qint16 maxsize, minsize, width, height, depth;
	HKStringMap m_filtered, m_unfiltered;

public:
	HMathMLAttr( QString _tag_name = _T(""), QString _tag_body = _T("") );
	HMathMLAttr( const HMathMLAttr& a );
	~HMathMLAttr( void );

	void operator=( const HMathMLAttr& a );
	void setMODefault( const HMathOpAttr& a );
	void reset( QString _tag_name = _T(""), QString _tag_body = _T("") );
	void set( const SYSINFO_Unit2PX& unit2px, AST_TXML_Tag *_token_tag = 0, QString _tag_body = _T("") );
	void set( AST_TXML_Tag *_token_tag, QString _tag_body = _T("") );
	int getChangedAttr_2_String( const SYSINFO_Unit2PX& unit2px, QString& dest, const HMathMLAttr& mml_attr, QString& name );

	void setMapAttr( HKStringMap& _mp, QString key, QString value );
	void printMapAttr( HKStringMap& _mp, QString& dest );

	void setFilteredAttr( const HMathMLAttr& a );
	void setFilteredAttr( QString key, QString value );
	void printFilteredAttr( QString& dest );

	void setUnfilteredAttr( const HMathMLAttr& a );
	void setUnfilteredAttr( QString key, QString value );
	void printUnfilteredAttr( QString& dest );

	const HKStringMap& getFilteredAttr() const;
	void getFilteredAttrCopy( std::vector<QString>& dest ) const;
	const HKStringMap& getUnfilteredAttr() const;
	void getUnfilteredAttrCopy( std::vector<QString>& dest ) const;
	void getAttrCopy( const HKStringMap& a, std::vector<QString>& dest ) const;

	int getFilteredAttr( QString attr_name, QString& attr_value ) const;
	QString getFilteredAttr( QString attr_name ) const;
	int getUnfilteredAttr( QString attr_name, QString& attr_value ) const;
	QString getUnfilteredAttr( QString attr_name ) const;

	void accountAttr( const QString& attr_name );
	void accountCommonAttr( void );
	void accountMOAttr( void );
	void accountMSAttr( void );
	void accountMSPACEAttr( void );

	QString printPreservingExistingUnits( QString attrName, double currentVal, int is_y, const SYSINFO_Unit2PX& unit2px ) const;
	int isAble2Glue() const;

	int isMOSeparator( void ) const { return mo_flag.isSetFlag( HF_FLAG_MO_SEPARATOR ); }
	void setMOSeparator( void ){ mo_flag.setFlag( HF_FLAG_MO_SEPARATOR ); }
	void clearMOSeparator( void ){ mo_flag.clearFlag( HF_FLAG_MO_SEPARATOR ); }
	int isMOStretchy( void ) const { return mo_flag.isSetFlag( HF_FLAG_MO_STRETCHY ); }
	void setMOStretchy( void ){ mo_flag.setFlag( HF_FLAG_MO_STRETCHY ); }
	void clearMOStretchy( void ){ mo_flag.clearFlag( HF_FLAG_MO_STRETCHY ); }
	int isMOFence( void ) const { return mo_flag.isSetFlag( HF_FLAG_MO_FENCE ); }
	void setMOFence( void ){ mo_flag.setFlag( HF_FLAG_MO_FENCE ); }
	void clearMOFence( void ){ mo_flag.clearFlag( HF_FLAG_MO_FENCE ); }
	int isMOAccent( void ) const { return mo_flag.isSetFlag( HF_FLAG_MO_ACCENT ); }
	void setMOAccent( void ){ mo_flag.setFlag( HF_FLAG_MO_ACCENT ); }
	void clearMOAccent( void ){ mo_flag.clearFlag( HF_FLAG_MO_ACCENT ); }
	int isMOLargeop( void ) const { return mo_flag.isSetFlag( HF_FLAG_MO_LARGEOP ); }
	void setMOLargeop( void ){ mo_flag.setFlag( HF_FLAG_MO_LARGEOP ); }
	void clearMOLargeop( void ){ mo_flag.clearFlag( HF_FLAG_MO_LARGEOP ); }
	int isMOSymmetric( void ) const { return mo_flag.isSetFlag( HF_FLAG_MO_SYMMETRIC ); }
	void setMOSymmetric( void ){ mo_flag.setFlag( HF_FLAG_MO_SYMMETRIC ); }
	void clearMOSymmetric( void ){ mo_flag.clearFlag( HF_FLAG_MO_SYMMETRIC ); }
	int isMOMovable( void ) const { return mo_flag.isSetFlag( HF_FLAG_MO_MOVABLELIMITS ); }
	void setMOMovable( void ){ mo_flag.setFlag( HF_FLAG_MO_MOVABLELIMITS ); }
	void clearMOMovable( void ){ mo_flag.clearFlag( HF_FLAG_MO_MOVABLELIMITS ); }
	int getMOForm( void ) const { return mo_flag.getValue() & HF_FLAG_MO_FORM_MASK; }
	void getMOForm2String( QString& dest ) const;
	void setMOForm( int form );

	void parseMPaddedAttr( RealFde* newValues, RealFde* oldValues, int* iRet, const SYSINFO_Unit2PX& unit2px ) const;
	void parseMPaddedAttr( int selfIdx, RealFde* newValues, RealFde* oldValues, int* iRet, const SYSINFO_Unit2PX& unit2px ) const;
};

/////////////////////////////////////////////////////////////////////

inline
QDataStream& operator << ( QDataStream& ar, const HMathMLAttr& lp )
{
	return ar << lp.mo_flag << lp.lspace << lp.rspace << lp.maxsize << lp.minsize << 
				 lp.width << lp.height << lp.depth << lp.m_filtered << lp.m_unfiltered;
}

inline
QDataStream& operator >> ( QDataStream& ar, HMathMLAttr& lp )
{
	return ar >> lp.mo_flag >> lp.lspace >> lp.rspace >> lp.maxsize >> lp.minsize >> 
				 lp.width >> lp.height >> lp.depth >> lp.m_filtered >> lp.m_unfiltered;
}

/////////////////////////////////////////////////////////////////////

enum Mathvariant2FontEnum
{
	M2F_monospace = 0,
	M2F_bold_italic, 
	M2F_bold, 
	M2F_italic,
	M2F_normal, 
	M2F_bold_script, 
	M2F_script, 
	M2F_sans_serif_bold_italic, 
	M2F_bold_sans_serif, 
	M2F_sans_serif_italic,
	M2F_sans_serif
};

__HERMITECH_FORMULIB__ long mml_mathvariant2font( QString& name );
__HERMITECH_FORMULIB__ long mml_mathvariant2font( AST_TXML_Tag *_token_tag );
__HERMITECH_FORMULIB__ long mml_mathvariant2font( AST_TXML_Tag *_token_tag, long *onlydue2fontfamily );
__HERMITECH_FORMULIB__ int mml_mathvariant2font( long isMathvariantPriority, long styleNo, const QVector<FS_LogFont_Color>& current_style_set, HMathMLAttr& mml_attr, FS_LogFont_Color& dest_font, long *nested_return = 0 );
__HERMITECH_FORMULIB__ int mml_mathvariant2font( long isMathvariantPriority, long styleNo, const QVector<FS_LogFont_Color>& current_style_set, QString& name, FS_LogFont_Color& dest_font );
__HERMITECH_FORMULIB__ int mml_mathcolor2color( QString val, QColor& dest_color, QColor default_color );
__HERMITECH_FORMULIB__ int mml_mathsize2pointsize( const SYSINFO_Unit2PX& unit2px, QString val, RealFde& point_size );
__HERMITECH_FORMULIB__ int mml_getTextAttr2FontColor( QColor& dest_color, AST_TXML_Tag *_token_tag );
__HERMITECH_FORMULIB__ void mml_getStyle2TextAttrColor( QString& dest, QColor _color, const QString& tag, const QString& body );
__HERMITECH_FORMULIB__ void mml_getStyle2TextAttrFont( QString& dest, FS_LogFont_Color& _font, const QString& tag, const QString& body, const HMathMLAttr& attr, int isForcedFontFamily );
__HERMITECH_FORMULIB__ void mml_getStyle2TextPointSize( QString& dest, RealFde _size, const QString& tag, const QString& body );
__HERMITECH_FORMULIB__ QString mml_get_space_template( const HMathMLAttr& attr, const SYSINFO_Unit2PX& unit2px );

/////////////////////////////////////////////////////////////////////

#endif
