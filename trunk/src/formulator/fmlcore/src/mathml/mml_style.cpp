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

#include "HUtils/ihtmlcolor.h"

#include "../settings/options.h"
#include "../style/style.h"

#include "mml_defs.h"
#include "mml_format.h"
#include "mml_units.h"
#include "mml_operator.h"
#include "mml_style.h"

///////////////////////////////////////////////////////////////////////////////

QString mml_getMPaddedUnitName( int i )
{
	static QString __mPaddedUnits[ MML_MPADDED_UNITS_LEN ] = 
	{ FBL_ATTR_width, FBL_ATTR_lspace, FBL_ATTR_height, FBL_ATTR_depth };
	if( i < 0 || i >= MML_MPADDED_UNITS_LEN ) return _T("");
	return __mPaddedUnits[ i ];
}

///////////////////////////////////////////////////////////////////////////////

int HMathMLAttr::isAble2Glue() const
{
	QString tmp;
	long cb = (long) m_filtered.size();
	if( getFilteredAttr( FBL_ATTR_mathvariant, tmp ) != -1 ) cb--;
	if( getFilteredAttr( FBL_ATTR_mathsize, tmp ) != -1 ) cb--;
	if( getFilteredAttr( FBL_ATTR_mathcolor, tmp ) != -1 ) cb--;
	if( getFilteredAttr( FBL_ATTR_fontsize, tmp ) != -1 ) cb--;
	if( getFilteredAttr( FBL_ATTR_fontweight, tmp ) != -1 ) cb--;
	if( getFilteredAttr( FBL_ATTR_fontstyle, tmp ) != -1 ) cb--;
	if( getFilteredAttr( FBL_ATTR_fontfamily, tmp ) != -1 ) cb--;

	if( m_unfiltered.size() > 0 || 
		cb > 0 ||
		mo_flag.getValue() & ~HF_FLAG_MO_FORM_MASK ||
		lspace != 0.0 || rspace != 0.0 ||
		maxsize != -1 || minsize != 1  ||
		width != MML_MSPACE_WIDTH_DEFAULT ||
		height != MML_MSPACE_HEIGHT_DEFAULT ||
		depth != MML_MSPACE_DEPTH_DEFAULT )
		return 0;
	return 1;
}

void HMathMLAttr::setMapAttr( HKStringMap& _mp, QString key, QString value )
{
	_mp[ key ] = value;
}
void HMathMLAttr::setFilteredAttr( QString key, QString value )
{
	setMapAttr( m_filtered, key, value );
}
void HMathMLAttr::setUnfilteredAttr( QString key, QString value )
{
	setMapAttr( m_unfiltered, key, value );
}

void HMathMLAttr::setFilteredAttr( const HMathMLAttr& a )
{
	m_unfiltered = a.m_filtered;
}
void HMathMLAttr::setUnfilteredAttr( const HMathMLAttr& a )
{
	m_unfiltered = a.m_unfiltered;
}

const HKStringMap& HMathMLAttr::getFilteredAttr() const
{
	return m_filtered;
}

void HMathMLAttr::getFilteredAttrCopy( std::vector<QString>& dest ) const
{
	getAttrCopy( m_filtered, dest );
}

const HKStringMap& HMathMLAttr::getUnfilteredAttr() const
{
	return m_unfiltered;
}

void HMathMLAttr::getUnfilteredAttrCopy( std::vector<QString>& dest ) const
{
	getAttrCopy( m_unfiltered, dest );
}

void HMathMLAttr::getAttrCopy( const HKStringMap& a, std::vector<QString>& dest ) const
{
	dest.clear();
	std::map<QString, QString>::const_iterator _nav = a.begin();
	while( _nav != a.end() )
	{
		dest.push_back( _nav->first );
		dest.push_back( _nav->second );
		_nav++;
	}
}

void HMathMLAttr::printMapAttr( HKStringMap& _mp, QString& dest )
{
	// all the rest
	QString tmp = _T("");
	QString key = _T(""), value = _T("");
	bool fContinue = _mp.tofirst();
	while( fContinue )
	{
		_mp.get( key, value );
		tmp += MAKE_MATHML_ATTR( key, hstr_escape( value ) );
		fContinue = _mp.tonext();
	}
	if( tmp.length() )
		dest += tmp;
}
void HMathMLAttr::printFilteredAttr( QString& dest )
{
	printMapAttr( m_filtered, dest );
}
void HMathMLAttr::printUnfilteredAttr( QString& dest )
{
	printMapAttr( m_unfiltered, dest );
}

///////////////////////////////////////////////////////////////////////////////

HMathMLAttr::~HMathMLAttr( void )
{
}

HMathMLAttr::HMathMLAttr( QString _tag_name, QString _tag_body )
{
	reset( _tag_name, _tag_body );
}

HMathMLAttr::HMathMLAttr( const HMathMLAttr& a )
{
	*this = a;
}

void HMathMLAttr::operator = ( const HMathMLAttr& a )
{
	// mo
	mo_flag = a.mo_flag;

	lspace = a.lspace;
	rspace = a.rspace;
	maxsize = a.maxsize;
	minsize = a.minsize;

	// mspace
	width = a.width;
	height = a.height;
	depth = a.depth;
	
	// filtered
	m_filtered = a.m_filtered;
	// unfiltered
	m_unfiltered = a.m_unfiltered;
}

void HMathMLAttr::setMODefault( const HMathOpAttr& a )
{
	setMOForm( a.form );
	a.fence ? setMOFence() : clearMOFence();
	a.separator ? setMOSeparator() : clearMOSeparator();
	a.accent ? setMOAccent() : clearMOAccent();
	a.largeop ? setMOLargeop() : clearMOLargeop();
	a.movablelimits ? setMOMovable() : clearMOMovable();
	a.stretchy ? setMOStretchy() : clearMOStretchy();
	clearMOSymmetric();

	lspace = a.lspace;
	rspace = a.rspace;
	minsize = a.minsize;
}

void HMathMLAttr::reset( QString _tag_name, QString _tag_body )
{
	setFilteredAttr( FBL_ATTR_mathvariant, FBL_VAL_normal );

	setMOForm( HMathOpAttr_FORM_INFIX );
	clearMOFence();
	clearMOSeparator();
	clearMOAccent();
	clearMOLargeop();
	clearMOMovable();
	clearMOStretchy();
	clearMOSymmetric();

	lspace = 0.0;
	rspace = 0.0;
	maxsize = -1; // infinity
	minsize = 1;
	width = MML_MSPACE_WIDTH_DEFAULT;
	height = MML_MSPACE_HEIGHT_DEFAULT;
	depth = MML_MSPACE_DEPTH_DEFAULT;
	m_unfiltered.erase( m_unfiltered.begin(), m_unfiltered.end() );

	if( _tag_name != _T("") )
	{
		if( ::mml_is_token_identifier_tag( _tag_name ) )
		{
			//if( ::isOption_MulticharMINotItalic() && _tag_body.length() == 1 && isalpha( _tag_body[ 0 ] ) )
			if( _tag_body.length() > 0 && _tag_body[ 0 ].isLetter() )
				setFilteredAttr( FBL_ATTR_mathvariant, FBL_VAL_italic );
		}
		else if( ::mml_is_token_string_tag( _tag_name ) )
		{
			setFilteredAttr( FBL_ATTR_lquote, _T("\"") );
			setFilteredAttr( FBL_ATTR_rquote, _T("\"") );
		}
		else if( ::mml_is_token_op_tag( _tag_name ) )
		{
			if( mml_convertSpaceName2FloatValue( 0/*x*/, FBL_VAL_thickmathspace, lspace, ::getCurrentFormulatorStyle().getUnit2PX() ) == -1 || lspace < 0 )
				lspace = 0.0;
			if( mml_convertSpaceName2FloatValue( 0/*x*/, FBL_VAL_thickmathspace, rspace, ::getCurrentFormulatorStyle().getUnit2PX() ) == -1 || rspace < 0 )
				rspace = 0.0;
		}
	}
}

void HMathMLAttr::accountAttr( const QString& attr_name )
{
	m_unfiltered.erase( QString( attr_name ) );
}

void HMathMLAttr::accountCommonAttr( void )
{
	static QString _common_attr_name[] = 
	{
		FBL_ATTR_mathvariant,
		FBL_ATTR_fontweight,
		FBL_ATTR_fontstyle,
		FBL_ATTR_fontfamily,
		FBL_ATTR_mathcolor,
		FBL_ATTR_color,
		FBL_ATTR_mathbackground,
		FBL_ATTR_mathsize,
		FBL_ATTR_fontsize,
		FBL_ATTR_background
	};
	static const long sizearr = sizeof( _common_attr_name ) / sizeof( _common_attr_name[0] );
	for( long i = 0; i < sizearr; i++ )
		accountAttr( _common_attr_name[ i ] );
}

void HMathMLAttr::accountMOAttr( void )
{
	static QString _common_attr_name[] = 
	{
		FBL_ATTR_form,
		FBL_ATTR_fence,
		FBL_ATTR_separator,
		FBL_ATTR_stretchy,
		FBL_ATTR_symmetric,
		FBL_ATTR_largeop,
		FBL_ATTR_movablelimits,
		FBL_ATTR_accent,
		FBL_ATTR_lspace,
		FBL_ATTR_rspace//,
		//FBL_ATTR_maxsize,
		//FBL_ATTR_minsize
	};
	static const long sizearr = sizeof( _common_attr_name ) / sizeof( _common_attr_name[0] );
	for( long i = 0; i < sizearr; i++ )
		accountAttr( _common_attr_name[ i ] );
}

void HMathMLAttr::accountMSAttr( void )
{
	static QString _common_attr_name[] = 
	{
		FBL_ATTR_lquote,
		FBL_ATTR_rquote
	};
	static const long sizearr = sizeof( _common_attr_name ) / sizeof( _common_attr_name[0] );
	for( long i = 0; i < sizearr; i++ )
		accountAttr( _common_attr_name[ i ] );
}

void HMathMLAttr::accountMSPACEAttr( void )
{
	static QString _common_attr_name[] = 
	{
		FBL_ATTR_width,
		FBL_ATTR_height,
		FBL_ATTR_depth,
		FBL_ATTR_linebreak
	};
	static const long sizearr = sizeof( _common_attr_name ) / sizeof( _common_attr_name[0] );
	for( long i = 0; i < sizearr; i++ )
		accountAttr( _common_attr_name[ i ] );
}

void HMathMLAttr::set( AST_TXML_Tag *_token_tag, QString _tag_body )
{
	set( ::getCurrentUnit2PX(), _token_tag, _tag_body );
}

void HMathMLAttr::set( const SYSINFO_Unit2PX& unit2px, AST_TXML_Tag *_token_tag, QString _tag_body )
{
	if( !_token_tag || !_token_tag->isAttr() )
		return;

	_token_tag->setAttrMap( m_unfiltered );	// read all attributes
	if( ::mml_is_token_tag( _token_tag->getTagName() ) )
		accountCommonAttr();

	AST_TXML_Attribute *_attr;

	// double-struck is ignoring YET
	_attr = _token_tag->getAttribute( FBL_ATTR_mathvariant );
	if( _attr )
	{
		setFilteredAttr( FBL_ATTR_mathvariant, _attr->getStringValue() );
	}
	else
	{
		AST_TXML_Attribute *_fontweight = _token_tag->getAttribute( FBL_ATTR_fontweight );
		AST_TXML_Attribute *_fontstyle = _token_tag->getAttribute( FBL_ATTR_fontstyle );
		if( _fontweight || _fontstyle )
		{
			int isbold = 0;
			if( _fontweight && !xml_strcmp( _fontweight->getStringValue(), FBL_VAL_bold ) )
				isbold = 1;
			int isitalic = ::mml_is_italic_token_mi( _token_tag->getTagName(), _tag_body ), isexplicit_normal = 0;
			if( _fontstyle )
			{
				if( !xml_strcmp( _fontstyle->getStringValue(), FBL_VAL_italic ) )
					isitalic = 1;
				else if( !xml_strcmp( _fontstyle->getStringValue(), FBL_VAL_normal ) )
				{
					isitalic = 0;
					isexplicit_normal = 1;
				}
			}
			if( isbold )
			{
				if( isitalic )
					setFilteredAttr( FBL_ATTR_mathvariant, FBL_VAL_bold_italic );
				else
					setFilteredAttr( FBL_ATTR_mathvariant, FBL_VAL_bold );
			}
			else
			{
				if( isitalic )
					setFilteredAttr( FBL_ATTR_mathvariant, FBL_VAL_italic );
				else
				{
					if( isexplicit_normal )
						setFilteredAttr( FBL_ATTR_mathvariant, FBL_VAL_normal );
				}
			}
		}
	}
	_attr = _token_tag->getAttribute( FBL_ATTR_fontfamily );
	if( _attr )
	{
		setFilteredAttr( FBL_ATTR_fontfamily, _attr->getStringValue() );
	}
	_attr = _token_tag->getAttribute( FBL_ATTR_mathcolor );
	if( _attr ) 
	{
		setFilteredAttr( FBL_ATTR_mathcolor, _attr->getStringValue() );
	}
	else
	{
		_attr = _token_tag->getAttribute( FBL_ATTR_color );
		if( _attr ) 
			setFilteredAttr( FBL_ATTR_mathcolor, _attr->getStringValue() );
	}
	_attr = _token_tag->getAttribute( FBL_ATTR_mathsize );
	if( _attr ) 
	{
		setFilteredAttr( FBL_ATTR_mathsize, _attr->getStringValue() );
	}
	else
	{
		_attr = _token_tag->getAttribute( FBL_ATTR_fontsize );
		if( _attr ) 
			setFilteredAttr( FBL_ATTR_mathsize, _attr->getStringValue() );
	}
	_attr = _token_tag->getAttribute( FBL_ATTR_mathbackground );
	if( _attr ) 
	{
		setFilteredAttr( FBL_ATTR_mathbackground, _attr->getStringValue() );
	}
	else
	{
		_attr = _token_tag->getAttribute( FBL_ATTR_background );
		if( _attr ) 
			setFilteredAttr( FBL_ATTR_mathbackground, _attr->getStringValue() );
	}

	if( ::mml_is_token_op_tag( _token_tag->getTagName() ) )
	{
		accountMOAttr();
		_attr = _token_tag->getAttribute( FBL_ATTR_form );
		if( _attr )
		{
			if( !xml_strcmp( _attr->getStringValue(), FBL_VAL_prefix ) )
			{
				setMOForm( HMathOpAttr_FORM_PREFIX );
			}
			else if( !xml_strcmp( _attr->getStringValue(), FBL_VAL_postfix ) )
			{
				setMOForm( HMathOpAttr_FORM_POSTFIX );
			}
			else if( !xml_strcmp( _attr->getStringValue(), FBL_VAL_infix ) )
			{
				setMOForm( HMathOpAttr_FORM_INFIX );
			}
		}
		_attr = _token_tag->getAttribute( FBL_ATTR_fence );
		if( _attr )
		{
			if( !xml_strcmp( _attr->getStringValue(), FBL_VAL_true ) )
			{
				setMOFence();
			}
			else if( !xml_strcmp( _attr->getStringValue(), FBL_VAL_false ) )
			{
				clearMOFence();
			}
			else
			{
				clearMOFence();
				//error: Error encountered and repaired: wrong value of the "fence" attribute
			}
		}
		_attr = _token_tag->getAttribute( FBL_ATTR_separator );
		if( _attr )
		{
			if( !xml_strcmp( _attr->getStringValue(), FBL_VAL_true ) )
			{
				setMOSeparator();
			}
			else if( !xml_strcmp( _attr->getStringValue(), FBL_VAL_false ) )
			{
				clearMOSeparator();
			}
			else
			{
				clearMOSeparator();
				//error: Error encountered and repaired: wrong value of the "separator" attribute
			}
		}
		_attr = _token_tag->getAttribute( FBL_ATTR_stretchy );
		if( _attr )
		{
			if( !xml_strcmp( _attr->getStringValue(), FBL_VAL_true ) )
			{
				setMOStretchy();
			}
			else if( !xml_strcmp( _attr->getStringValue(), FBL_VAL_false ) )
			{
				clearMOStretchy();
			}
			else
			{
				clearMOStretchy();
				//error: Error encountered and repaired: wrong value of the "stretchy" attribute
			}
		}
		_attr = _token_tag->getAttribute( FBL_ATTR_symmetric );
		if( _attr )
		{
			if( !xml_strcmp( _attr->getStringValue(), FBL_VAL_true ) )
			{
				setMOSymmetric();
			}
			else if( !xml_strcmp( _attr->getStringValue(), FBL_VAL_false ) )
			{
				clearMOSymmetric();
			}
			else
			{
				clearMOSymmetric();
				//error: Error encountered and repaired: wrong value of the "symmetric" attribute
			}
		}
		_attr = _token_tag->getAttribute( FBL_ATTR_largeop );
		if( _attr )
		{
			if( !xml_strcmp( _attr->getStringValue(), FBL_VAL_true ) )
			{
				setMOLargeop();
			}
			else if( !xml_strcmp( _attr->getStringValue(), FBL_VAL_false ) )
			{
				clearMOLargeop();
			}
			else
			{
				clearMOLargeop();
				//error: Error encountered and repaired: wrong value of the "symmetric" attribute
			}
		}
		_attr = _token_tag->getAttribute( FBL_ATTR_movablelimits );
		if( _attr )
		{
			if( !xml_strcmp( _attr->getStringValue(), FBL_VAL_true ) )
			{
				setMOMovable();
			}
			else if( !xml_strcmp( _attr->getStringValue(), FBL_VAL_false ) )
			{
				clearMOMovable();
			}
			else
			{
				clearMOMovable();
				//error: Error encountered and repaired: wrong value of the "symmetric" attribute
			}
		}
		_attr = _token_tag->getAttribute( FBL_ATTR_accent );
		if( _attr )
		{
			if( !xml_strcmp( _attr->getStringValue(), FBL_VAL_true ) )
			{
				setMOAccent();
			}
			else if( !xml_strcmp( _attr->getStringValue(), FBL_VAL_false ) )
			{
				clearMOAccent();
			}
			else
			{
				clearMOAccent();
				//error: Error encountered and repaired: wrong value of the "symmetric" attribute
			}
		}
		_attr = _token_tag->getAttribute( FBL_ATTR_lspace );
		if( _attr )
		{
			if( _attr->getValueType() == ASTVALT_TXML_String )
			{
				RealFde value = lspace;			
				if( ::mml_convertSpaceName2FloatValue( 0/*x*/, _attr->getStringValue(), value, unit2px ) != -1 )
				{
					lspace = (RealFde)::hround( value );
					setFilteredAttr( FBL_ATTR_lspace, _attr->getStringValue() );
				}
			}
			else
			{
				//error: Error encountered and repaired: "lspace" attribute must be a QString
			}
		}
		_attr = _token_tag->getAttribute( FBL_ATTR_rspace );
		if( _attr )
		{
			if( _attr->getValueType() == ASTVALT_TXML_String )
			{
				RealFde value = rspace;
				if( ::mml_convertSpaceName2FloatValue( 0/*x*/, _attr->getStringValue(), value, unit2px ) != -1 )
				{
					rspace = (RealFde)::hround( value );
					setFilteredAttr( FBL_ATTR_rspace, _attr->getStringValue() );
				}
			}
			else
			{
				//error: Error encountered and repaired: "rspace" attribute must be a QString
			}
		}

		// с этими атрибутами - полный бред, и если в будущем в Формуляторе появится поддержка 
		// _T('minsize') и _T('maxsize') на уровне прорисовки, то их придется сделать заново на базе плавающего числа, 
		// с тем чтобы поддерживать понятия _T('изменять в разы') (т.е. значение атрибута без употребления юнита, 
		// см. мануал по MathML - 2.4.4.2 Attributes with units)
		// 
		// а сейчас реализуем по схеме unfilteredAttr
#if 0
		_attr = _token_tag->getAttribute( FBL_ATTR_maxsize );
		if( _attr )
		{
			if( _attr->getValueType() == ASTVALT_TXML_String )
			{
				RealFde value = 1;
				if( ::mml_convertSpaceName2FloatValue( 1, _attr->getStringValue(), value, unit2px ) != -1 )
				{
					maxsize = ::hround( value );
					setFilteredAttr( FBL_ATTR_maxsize, _attr->getStringValue() );
				}
			}
			else
			{
				//error: Error encountered and repaired: "maxsize" attribute must be a QString
			}
		}
		_attr = _token_tag->getAttribute( FBL_ATTR_minsize );
		if( _attr )
		{
			if( _attr->getValueType() == ASTVALT_TXML_String )
			{
				RealFde value = 1;
				if( ::mml_convertSpaceName2FloatValue( 1/*y*/, _attr->getStringValue(), value, unit2px ) != -1 )
				{
					minsize = ::hround( value );
					setFilteredAttr( FBL_ATTR_minsize, _attr->getStringValue() );
				}
			}
			else
			{
				//error: Error encountered and repaired: "minsize" attribute must be a QString
			}
		}
#endif
	}
	else if( ::mml_is_token_space_tag( _token_tag->getTagName() ) )
	{
		accountMSPACEAttr();
		_attr = _token_tag->getAttribute( FBL_ATTR_linebreak );
		if( _attr ) 
		{
			setFilteredAttr( FBL_ATTR_linebreak, _attr->getStringValue() );
		}
		_attr = _token_tag->getAttribute( FBL_ATTR_width );
		if( _attr )
		{
			if( _attr->getValueType() == ASTVALT_TXML_String )
			{
				RealFde value = width;
				if( ::mml_convertSpaceName2FloatValue( 0/*x*/, _attr->getStringValue(), value, unit2px ) != -1 )
				{
					width = (short)::hround( value );
					setFilteredAttr( FBL_ATTR_width, _attr->getStringValue() );
				}
			}
			else
			{
				//error: Error encountered and repaired: "width" attribute must be a QString
			}
		}
		_attr = _token_tag->getAttribute( FBL_ATTR_height );
		if( _attr )
		{
			if( _attr->getValueType() == ASTVALT_TXML_String )
			{
				RealFde value = height;
				if( ::mml_attr_unit_2_value( 1/*x*/, value, _attr->getStringValue(), unit2px ) != -1 )
				{
					height = (short)::hround( value );
					setFilteredAttr( FBL_ATTR_height, _attr->getStringValue() );
				}
			}
			else
			{
				//error: Error encountered and repaired: "height" attribute must be a QString
			}
		}
		_attr = _token_tag->getAttribute( FBL_ATTR_depth );
		if( _attr )
		{
			if( _attr->getValueType() == ASTVALT_TXML_String )
			{
				RealFde value = depth;
				if( ::mml_attr_unit_2_value( 1/*y*/, value, _attr->getStringValue(), unit2px ) != -1 )
				{
					depth = (short)::hround( value );
					setFilteredAttr( FBL_ATTR_depth, _attr->getStringValue() );
				}
			}
			else
			{
				//error: Error encountered and repaired: "depth" attribute must be a QString
			}
		}
	}
	else if( ::mml_is_token_string_tag( _token_tag->getTagName() ) )
	{
		accountMSAttr();
		_attr = _token_tag->getAttribute( FBL_ATTR_lquote );
		if( _attr )
		{
			setFilteredAttr( FBL_ATTR_lquote, _attr->getStringValue() );
		}
		_attr = _token_tag->getAttribute( FBL_ATTR_rquote );
		if( _attr )
		{
			setFilteredAttr( FBL_ATTR_rquote, _attr->getStringValue() );
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////
// Returns: 0 - no changes, 1 - there were changes and a new attr QString is formed
//////////////////////////////////////////////////////////////////////////////////
int HMathMLAttr::getChangedAttr_2_String( const SYSINFO_Unit2PX& unit2px, QString& dest, const HMathMLAttr& mml_attr, QString& _tag_name )
{
	long initialLength = dest.length();
	QString _tmp;

	if( ::mml_is_token_string_tag( _tag_name ) )
	{
		_tmp = getFilteredAttr( FBL_ATTR_lquote );
		if( _tmp != mml_attr.getFilteredAttr( FBL_ATTR_lquote ) )
			dest += MAKE_MATHML_ATTR( FBL_ATTR_lquote, ::hstr_escape( _tmp ) );
		_tmp = getFilteredAttr( FBL_ATTR_rquote );
		if( _tmp != mml_attr.getFilteredAttr( FBL_ATTR_rquote ) )
			dest += MAKE_MATHML_ATTR( FBL_ATTR_rquote, ::hstr_escape( _tmp ) );
	}
	else if( ::mml_is_token_op_tag( _tag_name ) )
	{
		QString tmp;
		if( lspace != mml_attr.lspace )
		{
			dest += printPreservingExistingUnits( FBL_ATTR_lspace, lspace, 0, unit2px );
		}
		if( rspace != mml_attr.rspace )
		{
			dest += printPreservingExistingUnits( FBL_ATTR_rspace, rspace, 0, unit2px );
		}
#if 0
		// с этими атрибутами пока - полный бред, игнорируем
		if( maxsize != mml_attr.maxsize )
		{
			dest += printPreservingExistingUnits( FBL_ATTR_maxsize, maxsize, 1, unit2px );
		}
		if( minsize != mml_attr.minsize )
		{
			dest += printPreservingExistingUnits( FBL_ATTR_minsize, minsize, 1, unit2px );
		}
#endif

		if( (isMOFence() && !mml_attr.isMOFence()) || (!isMOFence() && mml_attr.isMOFence()) )
			dest += MAKE_MATHML_ATTR( FBL_ATTR_fence, QString( isMOFence() ? FBL_VAL_true : FBL_VAL_false ) );
		if( (isMOSeparator() && !mml_attr.isMOSeparator()) || (!isMOSeparator() && mml_attr.isMOSeparator()) )
			dest += MAKE_MATHML_ATTR( FBL_ATTR_separator, QString( isMOSeparator() ? FBL_VAL_true : FBL_VAL_false ) );
		if( (isMOStretchy() && !mml_attr.isMOStretchy()) || (!isMOStretchy() && mml_attr.isMOStretchy()) )
			dest += MAKE_MATHML_ATTR( FBL_ATTR_stretchy, QString( isMOStretchy() ? FBL_VAL_true : FBL_VAL_false ) );
		if( (isMOSymmetric() && !mml_attr.isMOSymmetric()) || (!isMOSymmetric() && mml_attr.isMOSymmetric()) )
			dest += MAKE_MATHML_ATTR( FBL_ATTR_symmetric, QString( isMOSymmetric() ? FBL_VAL_true : FBL_VAL_false ) );
		if( (isMOLargeop() && !mml_attr.isMOLargeop()) || (!isMOLargeop() && mml_attr.isMOLargeop()) )
			dest += MAKE_MATHML_ATTR( FBL_ATTR_largeop, QString( isMOLargeop() ? FBL_VAL_true : FBL_VAL_false ) );
		if( (isMOMovable() && !mml_attr.isMOMovable()) || (!isMOMovable() && mml_attr.isMOMovable()) )
			dest += MAKE_MATHML_ATTR( FBL_ATTR_movablelimits, QString( isMOMovable() ? FBL_VAL_true : FBL_VAL_false ) );
		if( (isMOAccent() && !mml_attr.isMOAccent()) || (!isMOAccent() && mml_attr.isMOAccent()) )
			dest += MAKE_MATHML_ATTR( FBL_ATTR_accent, QString( isMOAccent() ? FBL_VAL_true : FBL_VAL_false ) );

		if( getMOForm() != mml_attr.getMOForm() )
		{
			QString tmp = _T("");
			getMOForm2String( tmp );
			if( tmp.length() )
				dest += MAKE_MATHML_ATTR( FBL_ATTR_form, tmp );
		}
	}
	else if( ::mml_is_token_space_tag( _tag_name ) )
	{
		if( width != mml_attr.width )
		{
			dest += printPreservingExistingUnits( FBL_ATTR_width, width, 0, unit2px );
		}
		if( height != mml_attr.height )
		{
			dest += printPreservingExistingUnits( FBL_ATTR_height, height, 1, unit2px );
		}
		if( depth != mml_attr.depth )
		{
			dest += printPreservingExistingUnits( FBL_ATTR_depth, depth, 1, unit2px );
		}
		_tmp = getFilteredAttr( FBL_ATTR_linebreak );
		if( _tmp != mml_attr.getFilteredAttr( FBL_ATTR_linebreak ) )
			dest += MAKE_MATHML_ATTR( FBL_ATTR_linebreak, hstr_escape( _tmp ) );
	}

	// all the rest
	printUnfilteredAttr( dest );

	return (dest.length() > initialLength ? 1 : 0);
}

//////////////////////////////////////////////////////////////////////////////

int HMathMLAttr::getFilteredAttr( QString attr_name, QString& attr_value ) const
{
	const QString& _Key = attr_name;
	const QString *_val = m_filtered.is_find( _Key );
	if( _val )
	{
		attr_value = *_val;
		return 0;
	}
	return -1;
}

QString HMathMLAttr::getFilteredAttr( QString attr_name ) const
{
	const QString& _Key = attr_name;
	const QString *_val = m_filtered.is_find( _Key );
	if( _val )
		return *_val;
	return _T("");
}

int HMathMLAttr::getUnfilteredAttr( QString attr_name, QString& attr_value ) const
{
	const QString& _Key = attr_name;
	const QString *_val = m_unfiltered.is_find( _Key );
	if( _val )
	{
		attr_value = *_val;
		return 0;
	}
	return -1;
}

QString HMathMLAttr::getUnfilteredAttr( QString attr_name ) const
{
	const QString& _Key = attr_name;
	const QString *_val = m_unfiltered.is_find( _Key );
	if( _val )
		return *_val;
	return _T("");
}

void HMathMLAttr::setMOForm( int form )
{
	switch( form )
	{
	case HF_FLAG_MO_FORM_LO:
		mo_flag.setFlag( HF_FLAG_MO_FORM_LO ); 
		break;
	case HF_FLAG_MO_FORM_HI:
		mo_flag.setFlag( HF_FLAG_MO_FORM_HI ); 
		break;
	default:
		mo_flag.clearFlag( HF_FLAG_MO_FORM_LO );
		mo_flag.clearFlag( HF_FLAG_MO_FORM_HI );
	}
}

void HMathMLAttr::getMOForm2String( QString& dest ) const
{
	switch( getMOForm() )
	{
	case HMathOpAttr_FORM_INFIX:
		dest = FBL_VAL_infix;
		break;
	case HMathOpAttr_FORM_POSTFIX:
		dest = FBL_VAL_postfix;
		break;
	case HMathOpAttr_FORM_PREFIX:
		dest = FBL_VAL_prefix;
		break;
	default:
		dest = _T("");
	}
}

///////////////////////////////////////////////////////////////////////////////
// point_size - font size (in points before function starts, in pixel after the 
// function starts, but in points (!) when the function finishes)
///////////////////////////////////////////////////////////////////////////////
int mml_mathsize2pointsize( const SYSINFO_Unit2PX& unit2px, QString mathsize_str, RealFde& point_size )
{
	int ret = 1;
	RealFde value = point_size * unit2px.pt2px_y;	// parameter must be in pixel
													// in order to support % units
	if( mathsize_str.length() )
	{
		if( !xml_strcmp( mathsize_str, FBL_VAL_normal ) )
			point_size = MML_MATHSIZE_NORMAL;
		else if( !xml_strcmp( mathsize_str, FBL_VAL_small ) )
			point_size = MML_MATHSIZE_SMALL;
		else if( !xml_strcmp( mathsize_str, FBL_VAL_big ) )
			point_size = MML_MATHSIZE_BIG;
		else if( ::mml_attr_unit_2_value( 1/*y*/, value, mathsize_str, unit2px ) != -1 )
		{
			ret = 0;
			point_size = value / unit2px.pt2px_y;
			if( point_size < MML_MATHSIZE_MIN )
				point_size = MML_MATHSIZE_MIN;
			else if( point_size > MML_MATHSIZE_MAX )
				point_size = MML_MATHSIZE_MAX;
		}
	}
	else
		ret = -1;

	return ret;
}

int mml_mathcolor2color( QString val, QColor& dest_color, QColor default_color )
{
	int ret = -1;
	if( val.length() != 0 )
	{
		unsigned short r = 0, g = 0, b = 0;
		if( ::html_color_name2rgb( val, r, g, b ) != -1 )
			ret = 0;
		else
		{
			long i;
			for( i = 1; i < (long) val.length(); i++ )
				if( !isxdigit( val.at(i).toAscii() ) ) break;
			if( val[ 0 ] == _T('#') && i == (long) val.length() )
			{
				if( val.length() == 4 )
				{// _T('#rgb') = _T('#rrggbb', e.g.: _T('#abf') = 0xaabbff
					r = hex2dec( val[ 1 ] ); r = (r << 4) | r;
					g = hex2dec( val[ 2 ] ); g = (g << 4) | g;
					b = hex2dec( val[ 3 ] ); b = (b << 4) | b;
					ret = 0;
				}
				else if( val.length() == 7 )
				{// _T('#rrggbb'
					r = (hex2dec( val[ 1 ] ) << 4) | hex2dec( val[ 2 ] );
					g = (hex2dec( val[ 3 ] ) << 4) | hex2dec( val[ 4 ] );
					b = (hex2dec( val[ 5 ] ) << 4) | hex2dec( val[ 6 ] );
					ret = 0;
				}
			}
		}
		dest_color.setRgb(r, g, b);
	}
	else
		dest_color = default_color;

	//else
	//	ret = 0;	// if ret == 0 => default WILL NOT be taken from the corresponding style color
	return ret;
}

long mml_mathvariant2font( QString& name )
{
	static QString _mathvariant_name[ MML_MATHVARIANT_NUMBER ] =
	{
		FBL_VAL_monospace, FBL_VAL_bold_italic, 
		FBL_VAL_bold, FBL_VAL_italic,
		FBL_VAL_normal, FBL_VAL_bold_script, 
		FBL_VAL_script, FBL_VAL_sans_serif_bold_italic, 
		FBL_VAL_bold_sans_serif, FBL_VAL_sans_serif_italic,
		FBL_VAL_sans_serif
	};
	for( long i = 0; i < MML_MATHVARIANT_NUMBER; i++ )
		if( !xml_strcmp( _mathvariant_name[ i ], name ) )
			return i;
	return -1;
}

long mml_mathvariant2font( AST_TXML_Tag *_token_tag )
{
	if( _token_tag && _token_tag->isAttr() )
	{
		AST_TXML_Attribute *_attr = _token_tag->getAttribute( FBL_ATTR_mathvariant );
		if( _attr )
		{
			QString name = _attr->getStringValue();
			return mml_mathvariant2font( name );
		}
	}
	return -1;
}

long mml_mathvariant2font( AST_TXML_Tag *_token_tag, long *onlydue2fontfamily )
{
	*onlydue2fontfamily = 0;
	if( _token_tag && _token_tag->isAttr() )
	{
		AST_TXML_Attribute *_attr = _token_tag->getAttribute( FBL_ATTR_mathvariant );
		if( _attr )
		{
			QString name = _attr->getStringValue();
			long ff = mml_mathvariant2font( name );
			if( ff != -1 ) return ff;
		}
		_attr = _token_tag->getAttribute( FBL_ATTR_fontfamily );
		if( _attr && _attr->getStringValue().length() )
		{
			*onlydue2fontfamily = 1;
			return 0;
		}
	}
	return -1;
}

int mml_mathvariant2font( long isMathvariantPriority, long styleNo, 
	const QVector<FS_LogFont_Color>& current_style_set, HMathMLAttr& mml_attr, FS_LogFont_Color& dest_font, long *nested_return )
{
	QString _ff = mml_attr.getFilteredAttr( FBL_ATTR_fontfamily );
	QString _mv = mml_attr.getFilteredAttr( FBL_ATTR_mathvariant );
	if( _ff.length() )
	{
		long r = mml_mathvariant2font( _mv );
		if( nested_return )
			*nested_return = r;
		bool b = false, i = false;
		switch( r )
		{
		case M2F_monospace:
		case M2F_bold_script: 
		case M2F_script: 
		case M2F_sans_serif_bold_italic: 
		case M2F_bold_sans_serif: 
		case M2F_sans_serif_italic:
		case M2F_sans_serif:
			return mml_mathvariant2font( isMathvariantPriority, styleNo, current_style_set, _mv, dest_font );
		case M2F_bold_italic:
			b = true;
			i = true;
			break;
		case M2F_bold:
			b = true;
			i = false;
			break;
		case M2F_italic:
			b = false;
			i = true;
			break;
		case -1:
		case M2F_normal:
			b = false;
			i = false;
			break;
		default:
			break;
		}
		dest_font.m_name = _ff;
		dest_font.m_isFixed = false;
		dest_font.m_isBold = b;
		dest_font.m_isItalic = i;
		return 0;
	}
	return mml_mathvariant2font( isMathvariantPriority, styleNo, current_style_set, _mv, dest_font );
}

#define MML_mathvariant2font_monospace					0
#define MML_mathvariant2font_bold_italic				1
#define MML_mathvariant2font_bold						2
#define MML_mathvariant2font_italic						3
#define MML_mathvariant2font_normal						4
#define MML_mathvariant2font_bold_script				5
#define MML_mathvariant2font_script						6
#define MML_mathvariant2font_sans_serif_bold_italic		7
#define MML_mathvariant2font_bold_sans_serif			8
#define MML_mathvariant2font_sans_serif_italic			9
#define MML_mathvariant2font_sans_serif					10

static int _mathvariant_value[ MML_MATHVARIANT_NUMBER ][ 4 ] =
{
	// pitch, bold, italic
	{ true,		0,	0,	QFont::Courier },	// monospace
	{ false,	1,	1,	QFont::Times },		// bold italic
	{ false,	1,	0,	QFont::Times },		// bold
	{ false,	0,	1,	QFont::Times },		// italic
	{ false,	0,	0,	QFont::Times },		// normal
	{ false,	1,	0,	QFont::Decorative },// bold script
	{ false,	0,	0,	QFont::Decorative },// script
	{ false,	1,	1,	QFont::SansSerif },	// sans serif bold italic
	{ false,	1,	0,	QFont::SansSerif },	// bold sans serif
	{ false,	0,	1,	QFont::SansSerif },	// sans serif italic
	{ false,	0,	0,	QFont::SansSerif }	// sans serif
};

static QString _mathvariant_face[ MML_MATHVARIANT_NUMBER ] =
{
	_T("Courier New"),		// monospace
	_T("Times New Roman"),	// bold italic
	_T("Times New Roman"),	// bold
	_T("Times New Roman"),	// italic
	_T("Times New Roman"),	// normal
	_T("Decor"),			// bold script
	_T("Decor"),			// script
	_T("Arial"),			// sans serif bold italic
	_T("Arial"),			// bold sans serif
	_T("Arial"),			// sans serif italic
	_T("Arial")				// sans serif
};

int mml_mathvariant2font( long isMathvariantPriority, long styleNo, 
	const QVector<FS_LogFont_Color>& current_style_set, QString& name, FS_LogFont_Color& dest_font )
{
	long i = ::mml_mathvariant2font( name );
	if( i < 0 || i >= MML_MATHVARIANT_NUMBER )
		return -1;

	dest_font.m_isFixed = _mathvariant_value[ i ][ 0 ];
	dest_font.m_hint = (QFont::StyleHint) _mathvariant_value[ i ][ 3 ];

	if( isMathvariantPriority || styleNo < 0 || styleNo >= (long)current_style_set.size() )
	{
		// MathML defaults have priority
		dest_font.m_isBold = _mathvariant_value[ i ][ 1 ];
		dest_font.m_isItalic = _mathvariant_value[ i ][ 2 ];

		if( MML_mathvariant2font_monospace == i && MML_FSTYLE_FIXED < current_style_set.size() )
			dest_font.m_name = current_style_set[ MML_FSTYLE_FIXED ].m_name;
		else
		{
			if( styleNo >= 0 && styleNo < (long)current_style_set.size() )
				dest_font.m_name = current_style_set[ styleNo ].m_name;
			else 
				dest_font.m_name = _mathvariant_face[ i ];
		}
	}
	else
	{
		dest_font.m_isBold = current_style_set[ styleNo ].m_isBold;
		dest_font.m_isItalic = current_style_set[ styleNo ].m_isItalic;
		dest_font.m_name = current_style_set[ styleNo ].m_name;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// mml_getTextAttr2FontColor - converts MathML tag to font color
///////////////////////////////////////////////////////////////////////////////

int mml_getTextAttr2FontColor( QColor& dest_color, AST_TXML_Tag *_token_tag )
{
	if( _token_tag && _token_tag->isAttr() )
	{
		AST_TXML_Compound *comp = _token_tag->getAttributes();
		if( !comp ) return -1;
		AST_TXML_Attribute	*_mc = _token_tag->getAttribute( FBL_ATTR_mathcolor );
		if( !_mc ) _mc = _token_tag->getAttribute( FBL_ATTR_color );
		if( _mc )
			return mml_mathcolor2color( _mc->getStringValue(), dest_color, QColor(0, 0, 0) );
	}
	return -1;
}

///////////////////////////////////////////////////////////////////////////////
// mml_getStyle2TextAttr - converts font style and color to MathML attributes
///////////////////////////////////////////////////////////////////////////////

void mml_getStyle2TextPointSize( QString& dest, RealFde _size, const QString& /*tag*/, const QString& /*body*/ )
{
	if( _size >= MML_MATHSIZE_MIN && _size <= MML_MATHSIZE_MAX )
		dest.append( QString(" %1 = \'%2pt\'").arg(FBL_ATTR_mathsize).arg(_size) );
}

//void mml_getStyle2TextAttrColor( QString& dest, QColor _color, const QString& tag, const QString& body )
//{
//	if( _color != QColor(0, 0, 0) )
//	{
//		unsigned short r = _color.red(), g = _color.green(), b = _color.blue();
//		QString colorName = ::html_rgb2color_name( r, g, b );
//		if( colorName.length() )
//			dest.append( QString(" %1=\'%2\'")
//				.arg(FBL_ATTR_mathcolor)
//				.arg(colorName) );
//		else
//			dest.append( QString(" %1=\'#%2%3%4\'")
//				.arg(FBL_ATTR_mathcolor)
//				.arg(r, 2, 10, QChar('0'))
//				.arg(g, 2, 10, QChar('0'))
//				.arg(b, 2, 10, QChar('0')) );
//	}
//}

void mml_getStyle2TextAttrColor( QString& dest, QColor _color, const QString& /*tag*/, const QString& /*body*/ )
{
	if( _color != QColor(0, 0, 0) )
		dest.append( QString(" %1=\'%2\'").arg(FBL_ATTR_mathcolor).arg(_color.name()) );
}

void mml_getStyle2TextAttrFont( QString& dest, 
					 			FS_LogFont_Color& _font, 
					 			const QString& tag,
					 			const QString& body,
								const HMathMLAttr& attr,
								int isForcedFontFamily )
{
	QString arg2;
	if( _font.m_isFixed )
		arg2 = FBL_VAL_monospace;
	else
	{
		if( _font.m_hint == QFont::Decorative )
			arg2 = _font.m_isBold ? FBL_VAL_bold_script : FBL_VAL_script;
		else if( _font.m_hint == QFont::SansSerif )
		{
			if( _font.m_isBold )
				arg2 = _font.m_isItalic ? FBL_VAL_sans_serif_bold_italic : FBL_VAL_bold_sans_serif;
			else
				arg2 = _font.m_isItalic ? FBL_VAL_sans_serif_italic : FBL_VAL_sans_serif;
		}
		else
		{
			if( _font.m_isBold )
				arg2 = _font.m_isItalic ? FBL_VAL_bold_italic : FBL_VAL_bold;
			else
			{
				if( _font.m_isItalic )
				{
					if( !(body.length() == 1 && body.at(0).isLetter() && !xml_strcmp(tag, FBL_TAG_mi)) )
						arg2 = FBL_VAL_italic;
				}
				else
				{
					if( xml_strcmp(tag, FBL_TAG_mn) && xml_strcmp(tag, FBL_TAG_mo) && xml_strcmp(tag, FBL_TAG_mtext) )
						arg2 = FBL_VAL_normal;
				}
			}
		}
	}

	if( arg2.length() )
		dest.append( QString("%1=\'%2\'").arg(FBL_ATTR_mathvariant).arg(arg2) );

	QString _fontfamily = isForcedFontFamily ? _font.m_name : attr.getFilteredAttr( FBL_ATTR_fontfamily );
	if( _fontfamily.length() )
		dest.append( QString(" %1=\'%2\'").arg(FBL_ATTR_fontfamily).arg(_fontfamily) );
}

QString HMathMLAttr::printPreservingExistingUnits( QString attrName, double currentVal, int is_y, const SYSINFO_Unit2PX& unit2px ) const
{
	QString strResult;

	int ret;
	QString strDest, buffer;

	// use one of namedspace's if we can
	if( ::mml_isAttrTakingNamedspace( attrName ) )
	{
		ret = ::mml_convertFloatValue2SpaceName( strDest, (RealFde)currentVal, unit2px );
		if( ret != -1 && strDest.length() > 0 )
		{
			strResult = MAKE_MATHML_ATTR( attrName, strDest );
			return strResult;
		}
	}

	strDest = _T("");
	QString prevVal = _T("");
	double dVal = 1.0;
	if( getFilteredAttr( attrName, prevVal ) != -1 && prevVal > 0 &&
		::mml_findUnit( strDest, dVal, ret, prevVal, is_y, unit2px ) != -1 )
	{
		// use the previous user's choice of units
		const static double minMeaning = 0.001;
		double result2Print = currentVal / dVal;
		long roundedVal = ::hround( result2Print );
		if( ret || fabs(result2Print - roundedVal) < minMeaning )
		{
			// result need to be rounded (px, pt, pc)
			buffer = QString("%1%2").arg(roundedVal).arg(strDest);
		}
		else
			buffer = QString("%1%2").arg(result2Print, 0, 'g', 2).arg(strDest);
		strResult = MAKE_MATHML_ATTR( attrName, buffer );
	}
	else
	{
		// use _T('px') unit if don't know the previous user choice
		buffer = QString("%1%2").arg(::hround(currentVal)).arg(MML_UNIT_NAME__PX);
		strResult = MAKE_MATHML_ATTR( attrName, buffer );
	}
	return strResult;
}

QString mml_get_space_template( const HMathMLAttr& attr, const SYSINFO_Unit2PX& unit2px )
{
	static const QString template_s = _T("<mspace");
	static const QString template_f = _T("/>\n");
	QString template_body = _T("");

	if( attr.width != MML_MSPACE_WIDTH_DEFAULT )
	{
		template_body += attr.printPreservingExistingUnits( FBL_ATTR_width, attr.width, 0, unit2px );
	}

	if( attr.height != MML_MSPACE_HEIGHT_DEFAULT )
	{
		template_body += attr.printPreservingExistingUnits( FBL_ATTR_height, attr.height, 1, unit2px );
	}

	if( attr.depth != MML_MSPACE_DEPTH_DEFAULT )
	{
		template_body += attr.printPreservingExistingUnits( FBL_ATTR_depth, attr.depth, 1, unit2px );
	}

	QString _linebreak = attr.getFilteredAttr( FBL_ATTR_linebreak );
	if( _linebreak.length() && _linebreak.length() < 512 )
		template_body.append( QString(" linebreak='%1'").arg(_linebreak) );
	static QString result = template_s + template_body + template_f;
	return result;
}

//////////////////////////////////////////////////////////////////////////////

void HMathMLAttr::parseMPaddedAttr( RealFde* newValues, RealFde* oldValues, int* iRet, const SYSINFO_Unit2PX& unit2px ) const
{
	for( long i = 0; i < MML_MPADDED_UNITS_LEN; i++ )
		parseMPaddedAttr( i, newValues, oldValues, iRet, unit2px );
}

void HMathMLAttr::parseMPaddedAttr( int selfIdx, RealFde* newValues, RealFde* oldValues, int* iRet, const SYSINFO_Unit2PX& unit2px ) const
{
	QString attrValue;
	if( getUnfilteredAttr( ::mml_getMPaddedUnitName( selfIdx ), attrValue ) == 0 )
		iRet[ selfIdx ] = ::mml_attr_paddedunit_2_value( selfIdx, attrValue, newValues, oldValues, unit2px );
	else
		iRet[ selfIdx ] = -1;
}

//////////////////////////////////////////////////////////////////////////////
