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

//#include "HUtils/idfas.h"
#include "param.h"
#include "tags.h"

///////////////////////////////////////////////////////////////////////////////

long convertXMLType2ParameterType( enum AST_TXML_ValueTypes tp )
{
	if( tp == ASTVALT_TXML_None )
		return HKParameterType_NONE;
	else if( tp == ASTVALT_TXML_Int )
		return HKParameterType_Long;
	else if( tp == ASTVALT_TXML_Real )
		return HKParameterType_Double;
	else if( tp == ASTVALT_TXML_String )
		return HKParameterType_String;
	else
		return HKParameterType_NONE;
}

int ConvertAttr2ParameterType( AST_TXML_Attribute *attr, long tp )
{
	int ret = 0;
	switch( tp )
	{
	case HKParameterType_NONE:
		ret = -1;
		break;
	case HKParameterType_BOOL:
	case HKParameterType_Long:
		attr->convert2Integer();
		if( attr->getValueType() != ASTVALT_TXML_Int )
			ret = -1;
		break;
	case HKParameterType_Double:
		attr->convert2Float();
		if( attr->getValueType() != ASTVALT_TXML_Real )
			ret = -1;
		break;
	case HKParameterType_String:
		break;
	case HKParameterType_Array:
		ret = -1;
		break;
	default:
		ret = -1;
	}
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

void HKParameter::reset( void )
{
	switch ( getType() )
	{
	case HKParameterType_BOOL: 
	case HKParameterType_Long:
		m_long = 0;
		break;
	case HKParameterType_Double:
		m_double = 0.0;
		break;
	case HKParameterType_String:
		if ( m_string )
		{
			delete m_string;
			m_string = 0;
		}
		break;
	case HKParameterType_Array:
		if ( m_array )
		{
			delete m_array;
			m_array = 0;
		}
		break;
	}
	type = HKParameterType_NONE;
	caption.clear();
}

void HKParameter::init( long new_type )
{
	if ( getType() != HKParameterType_NONE ) reset();
	switch ( new_type )
	{
	case HKParameterType_NONE:
		m_string = 0;
		break;
	case HKParameterType_BOOL: 
	case HKParameterType_Long: 
		m_long = 0;
		break;
	case HKParameterType_Double: 
		m_double = 0.0;
		break;
	case HKParameterType_String:
		m_string = new QString(_T(""));
		break;
	case HKParameterType_Array:
		m_array = new HKParameterArray;
		break;
	default:
		new_type = HKParameterType_NONE;
		m_string = 0;
		break;
	}
	type = new_type;
}

void HKParameter::create( long value, long tp )
{
	if( tp == HKParameterType_BOOL )
		setType( HKParameterType_BOOL );
	else
		setType( HKParameterType_Long );
	m_long = value;
}

void HKParameter::create( double value )
{
	setType( HKParameterType_Double );
	m_double = value;
}

void HKParameter::create( const QString& value )
{
	setType( HKParameterType_String );
	*m_string = value;
}

void HKParameter::create( const HKParameterArray& value )
{
	setType( HKParameterType_Array );
	*m_array = value;
}

void HKParameter::create( const HKParameter& param )
{
	setCaption( param.getCaption() );
	switch ( param.getType() )
	{
	case HKParameterType_BOOL: 
	case HKParameterType_Long: 
		create( param.m_long, param.getType() );
		break;
	case HKParameterType_Double: 
		create( param.m_double );
		break;
	case HKParameterType_String:
		create( *param.m_string );
		break;
	case HKParameterType_Array:
		create( *param.m_array );
		break;
	}
}

void HKParameter::create( AST_TXML_Attribute& attr )
{
	setCaption( QString( _T("") ) );
	long tp = ::convertXMLType2ParameterType( attr.getValueType() );
	switch ( tp )
	{
	case HKParameterType_BOOL: 
	case HKParameterType_Long: 
		create( attr.getIntValue(), tp );
		break;
	case HKParameterType_Double: 
		create( attr.getFloatValue() );
		break;
	case HKParameterType_String:
		create( attr.getStringValue() );
		break;
	}
}

int HKParameter::operator == ( const HKParameter& param ) const
{
	if( getType() != param.getType() ) return 0;
	switch ( param.type )
	{
	case HKParameterType_BOOL: 
		return (m_long && param.m_long) || (!m_long && !param.m_long);
		break;
	case HKParameterType_Long: 
		return m_long == param.m_long;
		break;
	case HKParameterType_Double: 
		return m_double == param.m_double;
		break;
	case HKParameterType_String:
		if( m_string && param.m_string )
			return *m_string == *param.m_string;
		else 
			return !m_string && !param.m_string;
		break;
	case HKParameterType_Array:
		if( m_array && param.m_array )
			return *m_array == *param.m_array;
		else 
			return !m_array && !param.m_array;
		break;
	}
	return 0;
}
/*
void HKParameter::store( CArchive& ar )
{
	ar << type;
	ar << caption;
	switch ( type )
	{
	case HKParameterType_BOOL: 
	case HKParameterType_Long: 
		ar << m_long;
		break;
	case HKParameterType_Double: 
		ar << m_double;
		break;
	case HKParameterType_String:
		if( m_string )
			ar << *m_string;
		else
			throw EMID_INTERNAL_ERROR;
		break;
	case HKParameterType_Array:
		if( m_array )
			ar << *m_array;
		else
			throw EMID_INTERNAL_ERROR;
		break;
	}
}

void HKParameter::load( CArchive& ar )
{
	long 		tp;
	QString	cp;
	long				lng;
	double				dbl;
	QString			str;
	HKParameterArray	arr;

	ar >> tp;
	ar >> cp;
	switch ( tp )
	{
	case HKParameterType_BOOL: 
		ar >> lng;
		create( lng, HKParameterType_BOOL );
		break;
	case HKParameterType_Long: 
		ar >> lng;
		create( lng );
		break;
	case HKParameterType_Double: 
		ar >> dbl;
		create( dbl );
		break;
	case HKParameterType_String:
		ar >> str;
		create( str );
		break;
	case HKParameterType_Array:
		ar >> arr;
		create( arr );
		break;
	default:
		setType( type );
	}
	setCaption( cp );
}
*/

///////////////////////////////////////////////////////////////////////////////

int HKParameter::Read_XML( AST_TXML_Tag *root, QString& propname, DRMessageManager& dmm, long dmm_page )
{
	long ret = 0;
	if( !root ) return -1;
	propname = _T("");

	// mandatory
	AST_TXML_Attribute	*name = 0, *type = 0, *defaultv = 0, *caption = 0;
	AST_TXML_Compound *comp = root->getAttributes();
	AST_TXML_Node *node = 0;
	if( !comp ) return -1;
	for( long j = 0; j < comp->getChildNumber(); j++ )
	{
		node = comp->getChild( j );
		if( node->getNodeType() != ASTT_TXML_Attribute ) continue;
		if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_name ) )
			name = (AST_TXML_Attribute*)node;
		else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_default ) )
			defaultv = (AST_TXML_Attribute*)node;
		else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_type ) )
			type = (AST_TXML_Attribute*)node;
		else if ( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_caption ) )
			caption = (AST_TXML_Attribute*)node;
	}

	// type
	if( type && type->getValueType() == ASTVALT_TXML_String )
	{
		if( !xml_strcmp( type->getStringValue(), FBL_VAL_none ) )
			setType( HKParameterType_NONE );
		else if( !xml_strcmp( type->getStringValue(), FBL_VAL_bool ) )
			setType( HKParameterType_BOOL );
		else if( !xml_strcmp( type->getStringValue(), FBL_VAL_long ) )
			setType( HKParameterType_Long );
		else if( !xml_strcmp( type->getStringValue(), FBL_VAL_double ) )
			setType( HKParameterType_Double );
		else if( !xml_strcmp( type->getStringValue(), FBL_VAL_string ) )
			setType( HKParameterType_String );
		else if( !xml_strcmp( type->getStringValue(), FBL_VAL_array ) )
			setType( HKParameterType_Array );
		else
		{
			QString err1 = _T("property type %1 is unknown");
			dmm.AddLine( dmm_page, err1, root->getLine(), type->getStringValue() );
			ret = -1;
			return ret;
		}
	}
	else
	{
		QString err2 = _T("property type field is mandatory");
		dmm.AddLine( dmm_page, err2, root->getLine() );
		ret = -1;
		return ret;
	}

	// name
	if( !name || name->getValueType() != ASTVALT_TXML_String )
	{
		QString err3 = _T("property name field is mandatory");
		dmm.AddLine( dmm_page, err3, root->getLine() );
		ret = -1;
		return ret;
	}
	else
		propname = name->getStringValue();

	// caption
	if( caption && caption->getValueType() == ASTVALT_TXML_String )
		setCaption( caption->getStringValue() );
	else
	{
		setCaption( propname );
		QString err4 = _T("can't find caption field");
		dmm.AddLine( dmm_page, err4, root->getLine() );
	}

	// defaultv
	if( defaultv )
	{
		int suc = 1;
		switch( getType() )
		{
		case HKParameterType_BOOL:
			defaultv->convert2Integer();
			if( defaultv->getValueType() == ASTVALT_TXML_Int )
				create( defaultv->getIntValue(), HKParameterType_BOOL );
			else
				suc = 0;
			break;
		case HKParameterType_Long:
			defaultv->convert2Integer();
			if( defaultv->getValueType() == ASTVALT_TXML_Int )
				create( defaultv->getIntValue() );
			else
				suc = 0;
			break;
		case HKParameterType_Double:
			defaultv->convert2Float();
			if( defaultv->getValueType() == ASTVALT_TXML_Real )
				create( defaultv->getFloatValue() );
			else
				suc = 0;
			break;
		case HKParameterType_String:
			create( defaultv->getStringValue() );
			break;
		default:
			suc = 0;
		}
		if( !suc )
		{
			QString warn3 = _T("default field has a wrong type");
			dmm.AddLine( dmm_page, warn3, root->getLine() );
		}
	}

	return ret;
}

///////////////////////////////////////////////////////////////////////////////

int HKParameterList::Read_XML( AST_TXML_Tag *root, DRMessageManager& dmm, long dmm_page )
{
	long ret = 0;
	AST_TXML_Tag* tag = root->getFirstTag( FBL_TAG_property_list );
	if( !tag )
		return 0;

	std::vector<AST_TXML_Tag*> arrtags;
	tag->getAllTags( FBL_TAG_property, arrtags, 1 );

	long i;
	for( i = 0; i < (long) arrtags.size(); i++ )
	{
		if( arrtags[ i ] )
		{
			HKParameter item;
			QString name;
			if( !item.Read_XML( arrtags[ i ], name, dmm, dmm_page ) &&
				name != _T("") )
			{
				if ( isParameter( name ) )
				{
					QString err3 = _T("property already exists");
					dmm.AddLine( dmm_page, err3, arrtags[ i ]->getLine() );
				}
				else
					setParameter( name, item );
			}
		}
	}
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
