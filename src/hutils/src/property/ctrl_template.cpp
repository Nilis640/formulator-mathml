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

#include "ctrl_template.h"

void GUICtrlTemplate::InitAdditionalProperty( AST_TXML_Compound *comp )
{
	switch( m_id )
	{
	case GUI_CTRL_TEMPLATE_STATIC_TEXT:
		InitAdditionalProperty_STATIC_TEXT( comp );
		break;
	case GUI_CTRL_TEMPLATE_EDIT_BOX:
		InitAdditionalProperty_EDIT_BOX( comp );
		break;
	case GUI_CTRL_TEMPLATE_DROPDOWN_COMBO_BOX:
		InitAdditionalProperty_DROPDOWN_COMBO_BOX( comp );
		break;
	case GUI_CTRL_TEMPLATE_OPEN_FILE_EDIT_BOX:
		InitAdditionalProperty_OPEN_FILE_EDIT_BOX( comp );
		break;
	case GUI_CTRL_TEMPLATE_FORMULATOR_EDIT_BOX:
		InitAdditionalProperty_FORMULATOR_EDIT_BOX( comp );
		break;
	case GUI_CTRL_TEMPLATE_COLOR_COMBO_BOX:
		InitAdditionalProperty_COLOR_COMBO_BOX( comp );
		break;
	}
}

void GUICtrlTemplate::InitAdditionalProperty_STATIC_TEXT( AST_TXML_Compound * /*comp*/ )
{
}

void GUICtrlTemplate::InitAdditionalProperty_EDIT_BOX( AST_TXML_Compound *comp )
{
	if( isParameter( INT_PROP_NAME__TYPE ) )
	{
		HKParameter& prop = getParameter( INT_PROP_NAME__TYPE );
		if ( prop == HKParameter( QString( HKParameterTypeName_Long ) ) )
		{
			AST_TXML_Attribute	*_min_attr = 0, *_max_attr = 0;
			AST_TXML_Node *node;
			for( long j = 0; j < comp->getChildNumber(); j++ )
			{
				node = comp->getChild( j );
				if( node->getNodeType() != ASTT_TXML_Attribute ) continue;
				if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_min ) )
					_min_attr = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_max ) )
					_max_attr = (AST_TXML_Attribute*)node;
			}
			if( _min_attr )
			{
				_min_attr->convert2Integer();
				if( _min_attr->getValueType() == ASTVALT_TXML_Int )
				{
					setParameter(
						INT_PROP_NAME__MIN,
						HKParameter(_min_attr->getIntValue()) );
				}
			}
			if( _max_attr )
			{
				_max_attr->convert2Integer();
				if( _max_attr->getValueType() == ASTVALT_TXML_Int )
				{
					setParameter(
						INT_PROP_NAME__MAX,
						HKParameter(_max_attr->getIntValue()) );
				}
			}
		}
		else if ( prop == HKParameter( QString( HKParameterTypeName_Double ) ) )
		{
			AST_TXML_Attribute	*_min_attr = 0, *_max_attr = 0;
			AST_TXML_Node *node;
			for( long j = 0; j < comp->getChildNumber(); j++ )
			{
				node = comp->getChild( j );
				if( node->getNodeType() != ASTT_TXML_Attribute ) continue;
				if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_min ) )
					_min_attr = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_max ) )
					_max_attr = (AST_TXML_Attribute*)node;
			}
			if( _min_attr )
			{
				_min_attr->convert2Integer();
				if( _min_attr->getValueType() == ASTVALT_TXML_Real )
				{
					setParameter(
						INT_PROP_NAME__MIN,
						HKParameter(_min_attr->getFloatValue()) );
				}
			}
			if( _max_attr )
			{
				_max_attr->convert2Integer();
				if( _max_attr->getValueType() == ASTVALT_TXML_Real )
				{
					setParameter(
						INT_PROP_NAME__MAX,
						HKParameter(_max_attr->getFloatValue()) );
				}
			}
		}
	}
}

void GUICtrlTemplate::InitAdditionalProperty_DROPDOWN_COMBO_BOX( AST_TXML_Compound * /*comp*/ )
{
}

void GUICtrlTemplate::InitAdditionalProperty_OPEN_FILE_EDIT_BOX( AST_TXML_Compound *comp )
{
	AST_TXML_Attribute	*__filter_type = 0, *__filter_name = 0;
	AST_TXML_Node *node;
	for( long j = 0; j < comp->getChildNumber(); j++ )
	{
		node = comp->getChild( j );
		if( node->getNodeType() != ASTT_TXML_Attribute ) continue;
		if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_filter_type ) )
			__filter_type = (AST_TXML_Attribute*)node;
		else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_filter_name ) )
			__filter_name = (AST_TXML_Attribute*)node;
	}
	if( __filter_type && __filter_type->getValueType() == ASTVALT_TXML_String )
		setParameter( INT_PROP_NAME__FILTER__TYPE, HKParameter( __filter_type->getStringValue() ) );
	if( __filter_name && __filter_name->getValueType() == ASTVALT_TXML_String )
		setParameter( INT_PROP_NAME__FILTER__NAME, HKParameter( __filter_name->getStringValue() ) );
}

void GUICtrlTemplate::InitAdditionalProperty_FORMULATOR_EDIT_BOX( AST_TXML_Compound * /*comp*/ )
{
}

void GUICtrlTemplate::InitAdditionalProperty_COLOR_COMBO_BOX( AST_TXML_Compound * /*comp*/ )
{
}

void GUICtrlTemplate::InitProperty( void )
{
	Flush();
	switch( m_id )
	{
	case GUI_CTRL_TEMPLATE_STATIC_TEXT:
		InitProperty_STATIC_TEXT();
		break;
	case GUI_CTRL_TEMPLATE_EDIT_BOX:
		InitProperty_EDIT_BOX();
		break;
	case GUI_CTRL_TEMPLATE_DROPDOWN_COMBO_BOX:
		InitProperty_DROPDOWN_COMBO_BOX();
		break;
	case GUI_CTRL_TEMPLATE_OPEN_FILE_EDIT_BOX:
		InitProperty_OPEN_FILE_EDIT_BOX();
		break;
	case GUI_CTRL_TEMPLATE_FORMULATOR_EDIT_BOX:
		InitProperty_FORMULATOR_EDIT_BOX();
		break;
	case GUI_CTRL_TEMPLATE_COLOR_COMBO_BOX:
		InitProperty_COLOR_COMBO_BOX();
		break;
	}
}

void GUICtrlTemplate::InitProperty_STATIC_TEXT( void )
{
	setParameter( INT_PROP_NAME__NAME, 
		HKParameter( QString( GUI_CTRL_TEMPLATE_NAME_STATIC_TEXT ) ) );
}

void GUICtrlTemplate::InitProperty_EDIT_BOX( void )
{
	setParameter( INT_PROP_NAME__NAME, 
		HKParameter( QString( GUI_CTRL_TEMPLATE_NAME_EDIT_BOX ) ) );
	switch( m_type )
	{
	case HKParameterType_NONE:
		setParameter( INT_PROP_NAME__TYPE, HKParameter( QString( HKParameterTypeName_NONE ) ) );
		break;
	case HKParameterType_BOOL:           
		setParameter( INT_PROP_NAME__TYPE, HKParameter( QString( HKParameterTypeName_BOOL) ) );
		break;
	case HKParameterType_Long:
		setParameter( INT_PROP_NAME__TYPE, HKParameter( QString( HKParameterTypeName_Long) ) );
		break;
	case HKParameterType_Double:
		setParameter( INT_PROP_NAME__TYPE, HKParameter( QString( HKParameterTypeName_Double ) ) );
		break;
	case HKParameterType_String:
		setParameter( INT_PROP_NAME__TYPE, HKParameter( QString( HKParameterTypeName_String ) ) );
		break;
	case HKParameterType_Array:
		setParameter( INT_PROP_NAME__TYPE, HKParameter( QString( HKParameterTypeName_Array ) ) );
		break;
	}
}

void GUICtrlTemplate::InitProperty_DROPDOWN_COMBO_BOX( void )
{
	setParameter( INT_PROP_NAME__NAME, 
		HKParameter( QString( GUI_CTRL_TEMPLATE_NAME_DROPDOWN_COMBO_BOX ) ) );
}

int GUICtrlTemplate::AddComboItem( QString& _title, HKParameter& _value )
{                                                               
	if ( _value.getType() != getType() ) return -1;
	if ( isParameter( INT_PROP_NAME__VALUE ) )
	{
		HKParameter& v = getParameter( INT_PROP_NAME__VALUE );
		if( v.getType() != HKParameterType_Array ||
			v.m_array == 0 || v.m_array->size() % 2 )
			return -1;
		v.m_array->push_back( HKParameter(_title) );
		v.m_array->push_back( _value );
	}
	else
	{
		HKParameterArray *m_array = new HKParameterArray;
		m_array->push_back( HKParameter(_title) );
		m_array->push_back( _value );
		HKParameter p( *m_array );
		setParameter( INT_PROP_NAME__VALUE, p );
		delete m_array;
	}
	return 0;
}

void GUICtrlTemplate::InitProperty_OPEN_FILE_EDIT_BOX( void )
{
	setParameter( INT_PROP_NAME__NAME, 
		HKParameter( QString( GUI_CTRL_TEMPLATE_NAME_OPEN_FILE_EDIT_BOX ) ) );
	setParameter( INT_PROP_NAME__FILTER__TYPE, 
		HKParameter( QString( GUI_CTRL_TEMPLATE_PARAM_NAME_PREDEFINED ) ) );
	setParameter( INT_PROP_NAME__FILTER__NAME, 
		HKParameter( QString( GUI_CTRL_TEMPLATE_PARAM_NAME_ALLFILES ) ) );
}

void GUICtrlTemplate::InitProperty_FORMULATOR_EDIT_BOX( void )
{
	setParameter( INT_PROP_NAME__NAME, 
		HKParameter( QString( GUI_CTRL_TEMPLATE_NAME_FORMULATOR_EDIT_BOX ) ) );
}

void GUICtrlTemplate::InitProperty_COLOR_COMBO_BOX( void )
{
	setParameter( INT_PROP_NAME__NAME, 
		HKParameter( QString( GUI_CTRL_TEMPLATE_NAME_COLOR_COMBO_BOX ) ) );
}
