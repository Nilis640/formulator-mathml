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

#include "toolbar.h"
#include "btn_tags.h"

///////////////////////////////////////////////////////////////////////////////

int FBtnToolbar::Read_XML( AST_TXML_Tag *root, DRMessageManager& dmm, long dmm_page )
{
	AddToolBarData( root, dmm, dmm_page );

	int ret = 0;
	long i, j;
	std::vector<AST_TXML_Tag*> b_tags;
	root->getAllTags( FBL_TAG_button, b_tags, 1 );

	for( j = 0; j < (long) b_tags.size(); j++ )
	{
		FXmlBtn node_template;
		if( node_template.Read_XML(b_tags[ j ], dmm, dmm_page) != 0 )
			ret = -1;
		else
		{
			i = size();
			push_back( node_template );
			(*this)[i].setButtonNo(i);
			(*this)[i].setParentToolBar(getToolBarNo());
		}
	}
	return ret;
}

int FBtnToolbar::AddToolBarData( AST_TXML_Tag *root, DRMessageManager& dmm, long dmm_page )
{
	AST_TXML_Compound *comp = root->getAttributes();
	if ( !comp ) return -1;

	AST_TXML_Attribute *_visible = 0, *_name = 0, *_id = 0, *_group = 0, *_caption = 0, 
		*_image = 0, *_image_h = 0, *_image_d = 0, 
		*_image_title = 0, *_image_title_h = 0, *_image_title_d = 0, *_rows = 0, *_columns = 0;
	AST_TXML_Node *node;

	for( long j = 0; j < comp->getChildNumber(); j++ )
	{
		node = comp->getChild( j );
		if ( node->getNodeType() != ASTT_TXML_Attribute ) continue;

		if ( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_visible ) )
			_visible = (AST_TXML_Attribute*)node;
		else if ( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_id ) )
			_id = (AST_TXML_Attribute*)node;
		else if ( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_group ) )
			_group = (AST_TXML_Attribute*)node;
		else if ( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_name ) )
			_name = (AST_TXML_Attribute*)node;
		else if ( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_caption ) )
			_caption = (AST_TXML_Attribute*)node;
		else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_image ) )
			_image = (AST_TXML_Attribute*)node;
		else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_image_hot ) )
			_image_h = (AST_TXML_Attribute*)node;
		else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_image_disabled ) )
			_image_d = (AST_TXML_Attribute*)node;
		else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_image_title ) )
			_image_title = (AST_TXML_Attribute*)node;
		else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_image_title_hot ) )
			_image_title_h = (AST_TXML_Attribute*)node;
		else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_image_title_disabled ) )
			_image_title_d = (AST_TXML_Attribute*)node;
		else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_rows ) )
			_rows = (AST_TXML_Attribute*)node;
		else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_columns ) )
			_columns = (AST_TXML_Attribute*)node;
	}

	if( _visible && _visible->getValueType() == ASTVALT_TXML_String )
		setVisible( html_strcmp(_name->getStringValue(), FBL_VAL_no) ? 0 : -1 );
	if( _id && _id->getValueType() == ASTVALT_TXML_String )
		setID( _id->getStringValue() );

	if( _rows )
	{
		_rows->convert2Integer();
		if( _rows->getValueType() != ASTVALT_TXML_Int )
			dmm.AddLine( dmm_page, _T("%1 field must be of integer type, wrong value is ignored"), root->getLine(), FBL_ATTR_rows );
		else
		{
			long rows = _rows->getIntValue();
			if( rows < 0 )
				dmm.AddLine( dmm_page, _T("%1 field must be greater-than or equal to zero, wrong value is ignored"), root->getLine(), FBL_ATTR_rows );
			else
				setRows( rows );
		}
	}
	if( _columns )
	{
		_columns->convert2Integer();
		if( _columns->getValueType() != ASTVALT_TXML_Int )
			dmm.AddLine( dmm_page, _T("%1 field must be of integer type, wrong value is ignored"), root->getLine(), FBL_ATTR_columns );
		else
		{
			long columns = _columns->getIntValue();
			if( columns < 0 )
				dmm.AddLine( dmm_page, _T("%1 field must be greater-than or equal to zero, wrong value is ignored"), root->getLine(), FBL_ATTR_columns );
			else
				setColumns( columns );
		}
	}

	if( _group && _group->getValueType() == ASTVALT_TXML_String )
		setGroup( _group->getStringValue() );
	if( _name && _name->getValueType() == ASTVALT_TXML_String )
		setName( _name->getStringValue() );
	if( _caption && _caption->getValueType() == ASTVALT_TXML_String )
		setCaption( _caption->getStringValue() );
	if( _image && _image->getValueType() == ASTVALT_TXML_String )
		setImage( _image->getStringValue() );
	if( _image_h && _image_h->getValueType() == ASTVALT_TXML_String )
		setImage_h( _image_h->getStringValue() );
	if( _image_d && _image_d->getValueType() == ASTVALT_TXML_String )
		setImage_d( _image_d->getStringValue() );
	if( _image_title && _image_title->getValueType() == ASTVALT_TXML_String )
		setImageTitle( _image_title->getStringValue() );
	if( _image_title_h && _image_title_h->getValueType() == ASTVALT_TXML_String )
		setImageTitle_h( _image_title_h->getStringValue() );
	if( _image_title_d && _image_title_d->getValueType() == ASTVALT_TXML_String )
		setImageTitle_d( _image_title_d->getStringValue() );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
