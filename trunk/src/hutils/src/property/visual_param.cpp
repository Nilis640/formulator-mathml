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

#include <QFile>
#include "visual_param.h"
#include "tags.h"

HKVisualParameterList::HKVisualParameterList( void )
	: m_visual_list()
{
}

HKVisualParameterList::HKVisualParameterList( const HKVisualParameterList& p )
	: m_visual_list( p.m_visual_list )
{
}

HKVisualParameterList::~HKVisualParameterList()
{
}

int HKVisualParameter::Read_XML( AST_TXML_Tag *root, QString& propname, DRMessageManager& dmm, long dmm_page )
{
	long ret = 0;
	if( !root ) return -1;
	propname = _T("");
	ret = HKParameter::Read_XML( root, propname, dmm, dmm_page );
	if( ret == -1 ) return ret;
	AST_TXML_Attribute *control = root->getAttribute( FBL_ATTR_control );
	AST_TXML_Attribute *page_title = root->getAttribute( FBL_ATTR_page_title );

	Read_XML_VisualFeatures( root, page_title, control, dmm, dmm_page );
	getGUICtrl().InitAdditionalProperty( root->getAttributes() );

	return ret;
}

int HKVisualParameter::Read_XML_VisualFeatures( 
		AST_TXML_Tag *root, 
		AST_TXML_Attribute *page_title,  
		AST_TXML_Attribute *control,
		DRMessageManager& dmm, long dmm_page )
{
	if( page_title )
	{
		if( page_title->getValueType() == ASTVALT_TXML_Int )
			setPropertyPageNo( page_title->getIntValue() );
		else if( page_title->getValueType() == ASTVALT_TXML_String )
			setPropertyPageTitle( page_title->getStringValue() );
	}
	if( control && control->getValueType() == ASTVALT_TXML_String )
	{
		if( !xml_strcmp( control->getStringValue(), FBL_VAL_color_combo_box ) )
		{
			GUICtrlTemplate ctrl( GUI_CTRL_TEMPLATE_COLOR_COMBO_BOX, getType() );
			setGUICtrl( ctrl );
		}
		else if( !xml_strcmp( control->getStringValue(), FBL_VAL_edit_box ) )
		{
			GUICtrlTemplate ctrl( GUI_CTRL_TEMPLATE_EDIT_BOX, getType() );
			setGUICtrl( ctrl );
		}
		else if( !xml_strcmp( control->getStringValue(), FBL_VAL_open_file_edit_box ) )
		{
			GUICtrlTemplate ctrl( GUI_CTRL_TEMPLATE_OPEN_FILE_EDIT_BOX, getType() );
			setGUICtrl( ctrl );
		}
		else if( !xml_strcmp( control->getStringValue(), FBL_VAL_formulator_edit_box ) )
		{
			GUICtrlTemplate ctrl( GUI_CTRL_TEMPLATE_FORMULATOR_EDIT_BOX, getType() );
			setGUICtrl( ctrl );
		}
		else if( !xml_strcmp( control->getStringValue(), FBL_VAL_dropdown_combo_box ) )
		{
			GUICtrlTemplate ctrl( GUI_CTRL_TEMPLATE_DROPDOWN_COMBO_BOX, getType() );
			setGUICtrl( ctrl );
			QString warn2 = _T("property control (dropdown combo box) is empty");
			dmm.AddLine( dmm_page, warn2, root->getLine() );
		}
		else
		{
			QString warn1 = _T("property control name field %1 is unknown");
			dmm.AddLine( dmm_page, warn1, root->getLine(), control->getStringValue() );
		}
	}
	else
		Read_XML_ControlTag( root, dmm, dmm_page );
	return 0;
}


int HKVisualParameter::Read_XML_ControlTag( AST_TXML_Tag *root, DRMessageManager& dmm, long dmm_page )
{
	long k;
	AST_TXML_Tag* control_tag = root->getFirstTag( FBL_TAG_control );
	AST_TXML_Compound *intcomp = 0;
	AST_TXML_Node *node = 0;

	if( control_tag )
	{
		AST_TXML_Attribute	*__name = 0;
		intcomp = control_tag->getAttributes();
		for( long j = 0; intcomp && j < intcomp->getChildNumber(); j++ )
		{
			node = intcomp->getChild( j );
			if( node->getNodeType() != ASTT_TXML_Attribute ) continue;
			if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_name ) )
				__name = (AST_TXML_Attribute*)node;
		}

		if( __name && __name->getValueType() == ASTVALT_TXML_String )
		{
			if( !xml_strcmp( __name->getStringValue(), FBL_VAL_color_combo_box ) )
			{
				GUICtrlTemplate ctrl( GUI_CTRL_TEMPLATE_COLOR_COMBO_BOX, getType() );
				setGUICtrl( ctrl );
			}
			else if( !xml_strcmp( __name->getStringValue(), FBL_VAL_edit_box ) )
			{
				GUICtrlTemplate ctrl( GUI_CTRL_TEMPLATE_EDIT_BOX, getType() );
				setGUICtrl( ctrl );
			}
			else if( !xml_strcmp( __name->getStringValue(), FBL_VAL_open_file_edit_box ) )
			{
				GUICtrlTemplate guic( GUI_CTRL_TEMPLATE_OPEN_FILE_EDIT_BOX, getType() );
				setGUICtrl( guic );
				getGUICtrl().InitAdditionalProperty( control_tag->getAttributes() );
			}
			else if( !xml_strcmp( __name->getStringValue(), FBL_VAL_formulator_edit_box ) )
			{
				GUICtrlTemplate ctrl( GUI_CTRL_TEMPLATE_FORMULATOR_EDIT_BOX, getType() );
				setGUICtrl(  ctrl );
			}
			else if( !xml_strcmp( __name->getStringValue(), FBL_VAL_dropdown_combo_box ) )
			{
				GUICtrlTemplate guic( GUI_CTRL_TEMPLATE_DROPDOWN_COMBO_BOX, getType() );

				std::vector<AST_TXML_Tag*> _tags;
				//root->getAllTags( FBL_TAG_item, _tags, 2 );
				control_tag->getAllTags( FBL_TAG_item, _tags, 1 );

				if( !_tags.size() )
				{
					QString warn2 = _T("property control (dropdown combo box) is empty");
					dmm.AddLine( dmm_page, warn2, root->getLine() );
				}
				else
				{
					for( k = 0; k < (long)_tags.size(); k++ )
					{
						AST_TXML_Attribute *__v = _tags[ k ]->getAttribute( FBL_ATTR_value );
						AST_TXML_Attribute *__t = _tags[ k ]->getAttribute( FBL_ATTR_title );
						if( !__v || !__t )
						{
							QString err2 = _T("\"title\" and \"value\" item fields for the property dropdown combo box are mandatory");
							dmm.AddLine( dmm_page, err2, _tags[ k ]->getLine() );
						}
						else if( __t->getValueType() != ASTVALT_TXML_String )
						{
							QString err2 = _T("\"title\" item fields for the property dropdown combo box must be of string type");
							dmm.AddLine( dmm_page, err2, _tags[ k ]->getLine() );
						}
						else if( ::ConvertAttr2ParameterType( __v, getType() ) )
						{
							QString err2 = _T("type of \"value\" item fields for the property dropdown combo box must be of the same type as the property");
							dmm.AddLine( dmm_page, err2, _tags[ k ]->getLine() );
						}
						else
						{
							QString str( __t->getStringValue() );
							HKParameter param( *__v );
							guic.AddComboItem( str, param );
						}
					}
				}
				setGUICtrl( guic );
			}
			else
			{
				QString warn1 = _T("property control name field %1 is unknown");
				dmm.AddLine( dmm_page, warn1, root->getLine(), __name->getStringValue() );
			}
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////

int HKVisualParameterList::Read_XML( AST_TXML_Tag *root, DRMessageManager& dmm, long dmm_page )
{
	long ret = 0;
	AST_TXML_Tag* tag = root->getFirstTag( FBL_TAG_property_list );
	if( !tag )
		return 0;

	std::vector<AST_TXML_Tag*> arrtags;
	tag->getAllTags( FBL_TAG_property, arrtags, 1 );

	long i;
	for( i = 0; i < (long)arrtags.size(); i++ )
	{
		if( arrtags[ i ] )
		{
			HKVisualParameter item;
			QString name;
			if ( !item.Read_XML( arrtags[ i ], name, dmm, dmm_page ) && name != _T("") )
			{
				if( isParameter( name ) )
				{
					QString err3 = _T("property already exists");
					dmm.AddLine( dmm_page, err3, arrtags[ i ]->getLine() );
				}
				else
				{
					setParameter( name, item );
				}
			}
		}
	}

	return ret;
}

///////////////////////////////////////////////////////////////////////////

HKVisualParameterBar::HKVisualParameterBar( void )
	: HKVisualParameterList(), pages(), 
	name( _T("") ), title( _T("") ), fast_navigate_control( _T("") )
{
}

HKVisualParameterBar::HKVisualParameterBar( HKVisualParameterBar& p ) : 
	HKVisualParameterList( p ), pages( p.pages ), 
	name( p.name ), title( p.title ), fast_navigate_control( p.fast_navigate_control )
{
}

HKVisualParameterBar::~HKVisualParameterBar()
{
}

void HKVisualParameterBar::Read_XML( AST_TXML_Tag *root, DRMessageManager& dmm, long dmm_page )
{
	AST_TXML_Tag* tag = root->getFirstTag( FBL_TAG_property_bar );
	if( !tag ) return;

	long i, j;
	AST_TXML_Compound *comp = 0;
	AST_TXML_Node *node;

	AST_TXML_Attribute	*_name = 0, *_title = 0, *_fast_navigate = 0;
	if( (comp = tag->getAttributes()) != 0 )
	{
		for( j = 0; j < comp->getChildNumber(); j++ )
		{
			node = comp->getChild( j );
			if( node->getNodeType() != ASTT_TXML_Attribute ) continue;
			if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_name ) )
				_name = (AST_TXML_Attribute*)node;
			else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_title ) )
				_title = (AST_TXML_Attribute*)node;
			else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_fast_navigate ) )
				_fast_navigate = (AST_TXML_Attribute*)node;
		}

		// title
		if( _title && _title->getValueType() == ASTVALT_TXML_String )
			title = _title->getStringValue();

		// fast_navigate
		if( _fast_navigate && _fast_navigate->getValueType() == ASTVALT_TXML_String )
			fast_navigate_control = _fast_navigate->getStringValue();
	}

	std::vector<AST_TXML_Tag*> arrtags;
	tag->getAllTags( FBL_TAG_property_page, arrtags, 1 );
	struct HKVisualParameterPage prop_page;

	for( i = 0; i < (long)arrtags.size(); i++ )
	{
		if( arrtags[ i ] )
		{
			// mandatory
			AST_TXML_Attribute	*_type = 0, *_title = 0;
			if( !(comp = arrtags[ i ]->getAttributes()) ) continue;
			for( j = 0; j < comp->getChildNumber(); j++ )
			{
				node = comp->getChild( j );
				if( node->getNodeType() != ASTT_TXML_Attribute ) continue;
				if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_type ) )
					_type = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_title ) )
					_title = (AST_TXML_Attribute*)node;
			}

			// title
			if( _title && _title->getValueType() == ASTVALT_TXML_String )
				prop_page.title = _title->getStringValue();
			else
				prop_page.title = _T("");

			// type
			if( _type && _type->getValueType() == ASTVALT_TXML_String )
			{
				prop_page.type = 
					xml_strcmp( _type->getStringValue(), FBL_VAL_union ) ? 
						HKP_VISUAL_PARAMETER_PAGE__REGULAR : HKP_VISUAL_PARAMETER_PAGE__ALL;
			}
			else
				prop_page.type = HKP_VISUAL_PARAMETER_PAGE__REGULAR;
			
			pages.push_back( prop_page );
		}
	}

	// properties
	HKVisualParameterList::Read_XML( root, dmm, dmm_page );

	AdjustPropertyPages();
}

void HKVisualParameterBar::AdjustPropertyPages( void )
{
	if ( tofirst() )
	{
		long i = 0;
		QString key;
		HKVisualParameter param;
		do
		{
			get( key, param );
			if ( param.getPropertyPageTitle().length() )
				for ( i = 0; i < (long) pages.size(); i++ )
					if ( pages[ i ].title == param.getPropertyPageTitle() )
					{
						param.setPropertyPageNo( i );
						break;
					}
			if ( i == (long) pages.size() )
				param.setPropertyPageNo( -1 );
			setParameter( key, param );
		}
		while ( tonext() );
	}
}

/*
int HKVisualParameterBar::LoadTemplate( const QString& sect, const QString& key, DRMessageManager& dmm )
{
	CString strPath;
	CString strSect = __COMPANY_NAME__;
	strSect += sect;

	::GetSysParameter( HKEY_LOCAL_MACHINE, strSect, key, strPath );

	return LoadTemplateFile( (const QString&) strPath, dmm );
}
*/

int HKVisualParameterBar::LoadTemplateFile( const QString& path, DRMessageManager& dmm )
{
	Flush();

	//
	QFile file( path );
	if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) ) return -1;
	QByteArray readBytes = file.readAll();
	file.close();
	if( readBytes.size() == 0 ) return -1;

	QString old_filename = dmm.getDefaultProperty_FileName();
	dmm.setDefaultProperty_FileName( path );

	long dmm_page = 0;
	TXML_Storage* bms = 0;
	LS_CompileXML( path, QString(readBytes), bms, dmm, dmm_page );
	if ( bms == NULL ) return -1;

	std::vector<AST_TXML_Tag*> root_tags;
	bms->getAllTags( FBL_TAG_template, root_tags );

	// read xml template
	if ( root_tags.size() )
		Read_XML( root_tags[ 0 ], dmm, dmm_page );

	// 
	delete bms;

	return 0;
}

///////////////////////////////////////////////////////////////////////////
