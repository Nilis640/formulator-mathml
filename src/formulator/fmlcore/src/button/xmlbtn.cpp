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

#include <QtAlgorithms>

#include "HUtils/idfas.h"
#include "HUtils/imatchtemplate.h"

#include "../settings/options.h"
#include "../dictionary/op_map.h"

#include "../mathml/mml_operator.h"
#include "../mathml/mml_format.h"
#include "../mathml/mml_units.h"

#include "calc/bulanapi.h"
#include "calc/vmi.h"
#include "xmlbtn.h"
#include "btn_tags.h"

///////////////////////////////////////////////////////////////////////////////

int FXmlBtn::Read_XML( AST_TXML_Tag *root, DRMessageManager& dmm, long dmm_page )
{
	int ret = 0;
	FBtnVM vm(coord_calculation, dmm, dmm_page);
	SetToMathML( root, vm );

	QString& _tag = to_mathml.type;
	QString& _body = to_mathml.value;

	struct HMathMLAttr mml_attr( _tag, _body );
	HMathOpAttr *opinfo = ::mml_is_token_op_tag( _tag ) ? ::getOperatorEntryByMML( _body ) : 0;
	if( opinfo ) mml_attr.setMODefault( *opinfo );
	proplist.setMathMLAttr( mml_attr );
	AddButtonData( root, vm );

	switch( getNodeType() )
	{
	case NODE_MATHML:
		if( (ret = AddNodeType_NODE_MATHML( root, vm )) )
			return ret;
		break;
	case NODE_PLANETEXT:
		if( (ret = AddGraphicsData( root, vm )) ||
			(ret = CompileCalculationScheme( root, vm )) )
			return ret;
		break;
	case NODE_FORMULA:
		if( (ret = AddChildData( root, vm )) || 
			(ret = AddGraphicsData( root, vm )) ||
			(ret = CompileCalculationScheme( root, vm )) )
			return ret;
		break;
	default:
		break;
	}
	if( getNodeType() != NODE_MATHML && to_mathml.mathml_template_string.length() )
	{
		QVector<QString> args;
		for( long i = 0; i < getChildCount(); i++ )
		{
			args.push_back( QString() );
		}
		if( HKMatchTemplate().Treate( to_mathml.mathml_template_string, args ) == -1 )
		{
			QString warn2 = _T("number of empty slots in the button differs from slots number in the MathML conversion template");
			vm.dmm.AddLine( vm.dmm_page, warn2, root->getLine() );
			ret = -1;
		}
	}

	return ret;
}

void FXmlBtn::AddButtonData( AST_TXML_Tag *root, FBtnVM& vm )
{
	HKStringMap attrMap;
	root->setAttrMap( attrMap );
	HKStringMap::iterator it;

	QString attrName;
	long value = 0;
	bool ok;

	if( (it = attrMap.find(FBL_ATTR_rows)) != attrMap.end() )
	{
		unsigned long rows = 0;
		if( !html_strcmp(it->second, FBL_VAL_ask) )
			input_properties.push_back( QString(FBL_ATTR_rows) );
		else
		{
			if( (rows = it->second.toULong(&ok)) == 0 || !ok )
				vm.dmm.AddLine( vm.dmm_page, QString("%1 field must be of integer type, wrong value is ignored"), root->getLine(), FBL_ATTR_rows );
			else
				this->rows = rows;
		}
	}
	if( (it = attrMap.find(FBL_ATTR_columns)) != attrMap.end() )
	{
		unsigned long columns = 0;
		if( !html_strcmp(it->second, FBL_VAL_ask) )
			input_properties.push_back( QString(FBL_ATTR_columns) );
		else
		{
			if( (columns = it->second.toULong(&ok)) == 0 || !ok )
				vm.dmm.AddLine( vm.dmm_page, QString("%1 field must be of integer type, wrong value is ignored"), root->getLine(), FBL_ATTR_columns );
			else
				this->columns = columns;
		}
	}
	if( (it = attrMap.find(FBL_ATTR_encoding)) != attrMap.end() )
		input_properties.push_back( it->second );
	if( (it = attrMap.find(FBL_ATTR_cannot_draw_width)) != attrMap.end() )
		m_cannot_draw_width = it->second;
	if( (it = attrMap.find(FBL_ATTR_cannot_draw_height)) != attrMap.end() )
		m_cannot_draw_height = it->second;
	if( (it = attrMap.find(FBL_ATTR_disabledFrame)) != attrMap.end() && !html_strcmp(it->second, FBL_VAL_yes) )
		proplist.setFrameHidden();
	if( (it = attrMap.find(FBL_ATTR_eatleftop)) != attrMap.end() )
	{
		if( !html_strcmp(it->second, FBL_VAL_yes ))
			proplist.setContentEatLeftOp();
		else if( !html_strcmp(it->second, FBL_VAL_no) )
			proplist.clearContentEatLeftOp();
	}
	if( (it = attrMap.find(FBL_ATTR_name)) != attrMap.end() )
		setToolTip( formula_type_name = it->second );
	if( (it = attrMap.find(FBL_ATTR_id)) != attrMap.end() )
	{
		setID( it->second );
	}
	if( (it = attrMap.find(FBL_ATTR_stretchy)) != attrMap.end() )
	{
		if( !html_strcmp(it->second, FBL_VAL_yes ))
			proplist.setStretchy();
		else if( !html_strcmp(it->second, FBL_VAL_no) )
			proplist.clearStretchy();
	}
	if( (it = attrMap.find(FBL_ATTR_symmetric)) != attrMap.end() )
	{
		if( !html_strcmp( it->second, FBL_VAL_yes ) )
			proplist.setSymmetric();
		else if( !html_strcmp( it->second, FBL_VAL_no ) )
			proplist.clearSymmetric();
	}
	if( (it = attrMap.find(FBL_ATTR_mathml_support)) != attrMap.end() )
	{
		if( !html_strcmp( it->second, FBL_VAL_yes ) )
			proplist.setRWMMLSupport();
		else if( !html_strcmp( it->second, FBL_VAL_no ) )
			proplist.clearRWMMLSupport();
	}
	if( (it = attrMap.find(FBL_ATTR_mathml_support)) != attrMap.end() )
	{
		if( !html_strcmp( it->second, FBL_VAL_postfix ) )
			proplist.setRWMMLPostfix();
		else if( !html_strcmp( it->second, FBL_VAL_infix ) )
			proplist.setRWMMLInfix();
		else
			proplist.setRWMMLPrefix();
	}
	if( (it = attrMap.find(FBL_ATTR_bevelled)) != attrMap.end() )
	{
		if( !html_strcmp( it->second, FBL_VAL_yes ) )
			proplist.setUnfilteredAttr( FBL_ATTR_bevelled, FBL_VAL_true );
		if( !html_strcmp( it->second, FBL_VAL_no ) )
			proplist.setUnfilteredAttr( FBL_ATTR_bevelled, FBL_VAL_false );
	}
	if( (it = attrMap.find(FBL_ATTR_largeop)) != attrMap.end() )
	{
		if( !html_strcmp( it->second, FBL_VAL_yes ) )
			proplist.setLargeOp();
		else if( !html_strcmp( it->second, FBL_VAL_no ) )
			proplist.clearLargeOp();
	}
	if( (it = attrMap.find(FBL_ATTR_lspace)) != attrMap.end() )
	{
		RealFde value = 0.0;
		if( ::mml_convertSpaceName2FloatValue( 0/*x*/, it->second, value, ::getCurrentFormulatorStyle().getUnit2PX() ) != -1 )
			proplist.setLSpace( value );
	}
	if( (it = attrMap.find(FBL_ATTR_rspace)) != attrMap.end() )
	{
		RealFde value = 0.0;
		if( ::mml_convertSpaceName2FloatValue( 0/*x*/, it->second, value, ::getCurrentFormulatorStyle().getUnit2PX() ) != -1 )
			proplist.setRSpace( value );
	}
	if( (it = attrMap.find(FBL_ATTR_type)) != attrMap.end() )
	{
		if( !html_strcmp( it->second, FBL_VAL_text ) )
			node_type = NODE_PLANETEXT;
		else if( !html_strcmp( it->second, FBL_VAL_formula ) )
			node_type = NODE_FORMULA;
		else if( !html_strcmp( it->second, FBL_VAL_mathml ) )
			node_type = NODE_MATHML;
		else
		{
			node_type = NODE_FORMULA;
			QString warn2 = _T("type \"%1\" is unknown and ignored");
			vm.dmm.AddLine( vm.dmm_page, warn2, root->getLine(), it->second );
		}
	}
	if( (it = attrMap.find(FBL_ATTR_value)) != attrMap.end() )
	{
		LCEntity la_ent;
		long ret = la_ent.Analisys( it->second.midRef(0) );
		if( ret == LCEntity::Number )
		{
			proplist.setUnicode();
			text_value = QString(QChar(la_ent.getVal()));
		}
		else
			text_value = it->second;
	}

	if( (it = attrMap.find(FBL_ATTR_vertical_center_type)) != attrMap.end() )
	{
		if( node_type == NODE_PLANETEXT )
			vm.dmm.AddLine( vm.dmm_page, _T("attribute \"%1\" is not for plane text, it's ignored"), root->getLine(), it->second );
		else
		{
			attrName = it->second;
			if( !html_strcmp( attrName, FBL_VAL_object ) || !html_strcmp( it->second, FBL_VAL_child ) )
			{
				ok = false;
				if( (it = attrMap.find(FBL_ATTR_vertical_center_value)) != attrMap.end())
					value = it->second.toLong(&ok);
				if( ok )
				{
					vertical_center_type = FBtnChildPos::Child;
					vertical_center_value = value;
				}
				else
					vm.dmm.AddLine( vm.dmm_page, _T("vertical center value is missing or has the wrong type, vertical center option is ignored"), root->getLine() );
			}
			else if( !html_strcmp( attrName, FBL_VAL_average ) )
			{
				ok = false;
				if( (it = attrMap.find(FBL_ATTR_vertical_center_value)) != attrMap.end())
					value = it->second.toLong(&ok);
				if( ok )
				{
					vertical_center_type = FBtnChildPos::Child_Average;
					vertical_center_value = value;
				}
				else
					vm.dmm.AddLine( vm.dmm_page, _T("vertical center value is missing or has the wrong type, vertical center option is ignored"), root->getLine() );
			}
			else if( !html_strcmp( attrName, FBL_VAL_blank_average ) )
			{
				vertical_center_type = FBtnChildPos::Frame_Blank_Average;
			}
			else if( !html_strcmp( attrName, FBL_VAL_half ) )
			{
				vertical_center_type = FBtnChildPos::Half;
			}
			else if( !html_strcmp( attrName, FBL_VAL_frame ) )
			{
				ok = false;
				if( (it = attrMap.find(FBL_ATTR_vertical_center_value)) != attrMap.end())
					value = it->second.toLong(&ok);
				if( ok )
				{
					vertical_center_type = FBtnChildPos::Frame;
					vertical_center_value = value;
				}
				else
					vm.dmm.AddLine( vm.dmm_page, _T("vertical center value is missing or has the wrong type, vertical center option is ignored"), root->getLine() );
			}
			else
				vm.dmm.AddLine( vm.dmm_page, _T("vertical center type \'%1\' is unknown, it's ignored"), root->getLine(), it->second );
		}
	}

	if( (it = attrMap.find(FBL_ATTR_style)) != attrMap.end() )
	{
		if( node_type != NODE_PLANETEXT )
			vm.dmm.AddLine( vm.dmm_page, _T("style \"%1\" is for plane text only, it's ignored"), root->getLine(), it->second );
		long id = ::getCurrentFormulatorStyle().getStyleByName( it->second );
		if( id >= 0 )
			text_style = id;
		else
		{
			text_style = ::getCurrentFormulatorStyle_OnChar();
			vm.dmm.AddLine( vm.dmm_page, _T("style \"%1\" is unknown and ignored"), root->getLine(), it->second );
		}
	}

	// validity checks
	if( getNodeType() == NODE_FORMULA && !html_strcmp( getTextValue(), FBL_VAL_table ) && (this->rows == 0 || this->columns == 0) )
	{
		text_value = _T("");
		vm.dmm.AddLine( vm.dmm_page, _T("for the \"table\" template rows and columns must be greater-than or equal to 1, template is ignored"), root->getLine() );
	}

	AST_TXML_Tag *hotkeys = root->getFirstTag( FBL_TAG_hotkeys );
	if ( hotkeys )
		AddAccels( hotkeys, vm );
}

void FXmlBtn::AddAccels( AST_TXML_Tag *root, FBtnVM& /*vm*/ )
{
	AST_TXML_Compound *comp;
	AST_TXML_Node *node;

	std::vector<AST_TXML_Tag*> keys;
	root->getAllTags( FBL_TAG_hotkey, keys );

	for( long i = 0; i < (long) keys.size(); i++ )
	{
		std::vector<AST_TXML_Tag*> sequences;
		keys[ i ]->getAllTags( FBL_TAG_sequence, sequences );

		HKAccel accel;
		for( long k = 0; k < (long) sequences.size(); k++ )
		{
			if ( (comp = sequences[ k ]->getAttributes()) == 0 ) continue;

			HKAccelCode accelcode;
			for( long j = 0; j < (long) comp->getChildNumber(); j++ )
			{
				node = comp->getChild( j );
				if( node->getNodeType() != ASTT_TXML_Attribute ) continue;

				if( ((AST_TXML_Attribute*) node)->isAttribute( FBL_ATTR_ctrl ) &&
					html_strcmp( ((AST_TXML_Attribute*) node)->getStringValue(), FBL_VAL_yes ) == 0 )
					accelcode.setFlag( HK_Key_Ctrl );
				else if( ((AST_TXML_Attribute*) node)->isAttribute( FBL_ATTR_alt ) &&
					html_strcmp( ((AST_TXML_Attribute*) node)->getStringValue(), FBL_VAL_yes ) == 0 )
					accelcode.setFlag( HK_Key_Alt );
				else if( ((AST_TXML_Attribute*) node)->isAttribute( FBL_ATTR_shift ) && 
					html_strcmp( ((AST_TXML_Attribute*) node)->getStringValue(), FBL_VAL_yes ) == 0 )
					accelcode.setFlag( HK_Key_Shift );
				else if( ((AST_TXML_Attribute*) node)->isAttribute( FBL_ATTR_key ) )
				{
					if ( ((AST_TXML_Attribute*) node)->getStringValue().length() > 1 )
					{
						if ( html_strcmp( ((AST_TXML_Attribute*) node)->getStringValue(), FBL_VAL_left ) == 0 )
							accelcode.setKey( 0x0025 );
						else if ( html_strcmp( ((AST_TXML_Attribute*) node)->getStringValue(), FBL_VAL_right ) == 0 )
							accelcode.setKey( 0x0027 );
						else if ( html_strcmp( ((AST_TXML_Attribute*) node)->getStringValue(), FBL_VAL_up ) == 0 )
							accelcode.setKey( 0x0026 );
						else if ( html_strcmp( ((AST_TXML_Attribute*) node)->getStringValue(), FBL_VAL_down ) == 0 )
							accelcode.setKey( 0x0028 );
						else if ( html_strcmp( ((AST_TXML_Attribute*) node)->getStringValue(), FBL_VAL_tab ) == 0 )
							accelcode.setKey( 0x0009 );
						else if ( html_strcmp( ((AST_TXML_Attribute*) node)->getStringValue(), FBL_VAL_enter ) == 0 )
							accelcode.setKey( 0x000D );
					}
					else
					{
						accelcode.setKey( ((AST_TXML_Attribute*) node)->getStringValue().at( 0 ).unicode() );
					}
				}
			}
			if ( accelcode.getAccelCode() )
				accel.addAccelCode( accelcode );
		}
		if ( accel.getAccelCodeCount() )
			addAccel( accel );
	}
}

void FXmlBtn::SetFormulaName( AST_TXML_Tag *root )
{
	AST_TXML_Attribute* attr = root->getAttribute( FBL_ATTR_name );
	if( attr && attr->getValueType() == ASTVALT_TXML_String )
		formula_type_name = attr->getStringValue();
}

void FXmlBtn::SetToMathML( AST_TXML_Tag *root, FBtnVM& /*vm*/ )
{
	AST_TXML_Tag *tag = root->getFirstTag( FBL_TAG_mathml );
	if( !tag ) return;
	AST_TXML_Compound *comp;
	comp = tag->getAttributes();
	if( !comp ) return;
	AST_TXML_Node *node;
	to_mathml.reset();

	for( long i = 0; i < comp->getChildNumber(); i++ )
	{
		node = comp->getChild( i );
		if( node->getNodeType() != ASTT_TXML_Attribute ||
			((AST_TXML_Attribute*)node)->getValueType() != ASTVALT_TXML_String )
			continue;
		if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_value ) )
			to_mathml.value = ((AST_TXML_Attribute*)node)->getStringValue();
		else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_name ) )
			to_mathml.name = ((AST_TXML_Attribute*)node)->getStringValue();
		else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_left ) )
			to_mathml.left = ((AST_TXML_Attribute*)node)->getStringValue();
		else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_right ) )
			to_mathml.right = ((AST_TXML_Attribute*)node)->getStringValue();

		else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_mathml_template ) )
			to_mathml.mathml_template_string = ((AST_TXML_Attribute*)node)->getStringValue();
		else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_type ) )
			to_mathml.type = ((AST_TXML_Attribute*)node)->getStringValue();
		else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_frame ) )
			to_mathml.frame = ((AST_TXML_Attribute*)node)->getStringValue();
	}
}

int FXmlBtn::AddNodeType_NODE_MATHML(AST_TXML_Tag *root, FBtnVM& vm)
{
	long ret = 0;
	AST_TXML_Tag* tag = root->getFirstTag( FBL_TAG_math );
	if( !tag )
	{
		QString err2 = _T("tag \"math\" is expected");
		vm.dmm.AddLine( vm.dmm_page, err2, root->getLine() );
		ret = -1;
	}
	else
	{
		node_mathml_string = _T("");
		tag->getXML( node_mathml_string );
	}
	return ret;
}

int FXmlBtn::CompileCalculationScheme( AST_TXML_Tag *root, FBtnVM& vm )
{
	long ret = 0, i;
	AST_TXML_Tag* tag;

	QString err1 = _T("graphics position field %1 is undefined (there is no formula)");
	QString str = _T("");
	QString _text = _T("");	

	if( (tag = root->getFirstTag( FBL_TAG_expr )) != 0 && (_text = tag->getText()) != _T("") )
	{
		PiCodeASTStorage* bms = 0;
		QString preline;
		long count = tag->getLine() - 1;
		for( i = 0; i < count; i++ )
			preline += _T("\n\r");
		_text.insert( 0, preline );
		if( LS_CompileBulanExpr(_text, bms, vm.dmm, vm.dmm_page) || !bms )
		{
			QString err2 = _T("error(s) in graphics expression");
			vm.dmm.AddLine( vm.dmm_page, err2, tag->getLine() );
			ret = -1;
		}
		else
		{
			vm.FlushCode();
			bms->getInstruction( vm );
			if( ret == 0 ) ret = - vm.isError();
			if( bms ) delete bms;
		}
	}

	QVector<QPair<long, long> > err_pairs;
	vm.CheckAssignTable( err_pairs );
	for( i = 0; i < (long) err_pairs.size(); i++ )
	{
		if( (unsigned long) err_pairs[ i ].first >= (unsigned long) ar_graphics.size() || 
			(unsigned long) err_pairs[ i ].first >= (unsigned long) vm.getPrimitiveNumber() ||
			(unsigned long) err_pairs[ i ].second > 3 )
			throw "Internal error";

		if( ar_graphics[ err_pairs[ i ].first ].coord_type == COORD_TYPE_FORMULA )
		{
			str = vm.getPrimitiveName( err_pairs[ i ].first ) + QString( _T(".") ) + getGraphicsFieldName( err_pairs[ i ].second );
			vm.dmm.AddLine( vm.dmm_page, err1, tag->getLine(), str );
			ret = -1;
		}
	}

	return ret;
}

/////////////////////////////////////////////////////////////////////
int bb_read_attr_linethickness( AST_TXML_Tag *node, const QString& attr_name, RealFde& child_width );
int bb_read_attr_align_vertical( AST_TXML_Tag *node, const QString& attr_name, long& child_align );
int bb_read_attr_align_vertical( AST_TXML_Tag *node, const QString& attr_name, std::vector<long>& child_align, long defaultAlign );
int bb_read_attr_align_horizontal( AST_TXML_Tag *node, const QString& attr_name, long& child_align );
int bb_read_attr_align_horizontal( AST_TXML_Tag *node, const QString& attr_name, std::vector<long>& child_align, long defaultAlign );
int bb_read_attr_table_lines( AST_TXML_Tag *node, const QString& attr_name, std::vector<long>& penStyle );

int FXmlBtn::AddChildData_read_align( AST_TXML_Tag *node, FBtnChildPos& child, FBtnVM& /*vm*/ )
{
	long v = VERTICAL_CENTER, h = HORISONTAL_CENTER;
	::bb_read_attr_align_vertical( node, QString(FBL_ATTR_valign), v );
	::bb_read_attr_align_horizontal( node, QString(FBL_ATTR_halign), h );
	child.align = v | h;
	return 0;
}

int FXmlBtn::AddChildData( AST_TXML_Tag *root, FBtnVM& vm )
{
	long ret = 0;
	AST_TXML_Tag* tag = root->getFirstTag( FBL_TAG_frames );
	if( !tag )
	{
		if( !html_strcmp( getTextValue(), FBL_VAL_table ) )
			return 0;
		else
		{
			vm.dmm.AddLine( vm.dmm_page, _T("definition of frames is mandatory part of the formula node description"), root->getLine() );
			return -1;
		}
	}

	long i, j;
	AST_TXML_Compound *comp = 0;
	AST_TXML_Node *node;

	std::vector<AST_TXML_Tag*> arrtags;
	tag->getAllTags( FBL_TAG_frame, arrtags, 1 );

	for( i = 0; i < (long) arrtags.size(); i++ )
	{
		if( arrtags[ i ] )
		{
			AST_TXML_Attribute	*name = 0, *left = 0, *right = 0, *top = 0, 
								*bottom = 0, *valign = 0, *halign = 0, 
								*level = 0;
			if( !(comp = arrtags[ i ]->getAttributes()) ) continue;
			for( j = 0; j < comp->getChildNumber(); j++ )
			{
				node = comp->getChild( j );
				if( node->getNodeType() != ASTT_TXML_Attribute ) continue;
				if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_name ) )
					name = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_level ) )
					level = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_left ) )
					left = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_top ) )
					top = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_right ) )
					right = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_bottom ) )
					bottom = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_valign ) )
					valign = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_halign ) )
					halign = (AST_TXML_Attribute*)node;
			}

			struct FBtnChildPos child;
			if( name && name->getValueType() == ASTVALT_TXML_String )
				vm.AddObject( name->getStringValue(), arrtags[ i ]->getLine() );
			else
				vm.AddObject( QString("%1").arg(i), arrtags[ i ]->getLine() );
			if( left && top && right && bottom )
			{
				if( left->getValueType() != ASTVALT_TXML_Real && left->convert2Float() != ELH_NO_ERROR )
				{
					vm.dmm.AddLine( vm.dmm_page, _T("child frame position fields left must be of integer or real type"), arrtags[ i ]->getLine() );
					ret = -1;
					continue;
				}
				if( top->getValueType() != ASTVALT_TXML_Real && top->convert2Float() != ELH_NO_ERROR )
				{
					vm.dmm.AddLine( vm.dmm_page, _T("child frame position fields top must be of integer or real type"), arrtags[ i ]->getLine() );
					ret = -1;
					continue;
				}
				if( right->getValueType() != ASTVALT_TXML_Real && right->convert2Float() != ELH_NO_ERROR )
				{
					vm.dmm.AddLine( vm.dmm_page, _T("child frame position fields right must be of integer or real type"), arrtags[ i ]->getLine() );
					ret = -1;
					continue;
				}
				if( bottom->getValueType() != ASTVALT_TXML_Real && bottom->convert2Float() != ELH_NO_ERROR )
				{
					vm.dmm.AddLine( vm.dmm_page, _T("child frame position fields bottom must be of integer or real type"), arrtags[ i ]->getLine() );
					ret = -1;
					continue;
				}
    
				child.pos.setLeft( left->getFloatValue() );
				child.pos.setTop( top->getFloatValue() );
				child.pos.setRight( right->getFloatValue() );
				child.pos.setBottom( bottom->getFloatValue() );
			}

			AddChildData_read_align( arrtags[ i ], child, vm );

			if( level )
			{
				if( level->getValueType() == ASTVALT_TXML_Int && level->getIntValue() >= 0 )
					child.level = level->getIntValue();
				else if( level->getValueType() == ASTVALT_TXML_String )
				{
					if( !html_strcmp( level->getStringValue(), FBL_VAL_parent ) ||
						!html_strcmp( level->getStringValue(), FBL_VAL_frame ) )
						child.level = FTEXT_HEIGHT_FRAME;
					else if( !html_strcmp( level->getStringValue(), FBL_VAL_child ) ||
							!html_strcmp( level->getStringValue(), FBL_VAL_child1 ) )
						child.level = FTEXT_HEIGHT_CHILD1;
				}
			}

			FBtn::AddChildData( child );
		}
	}
	return ret;
}

/////////////////////////////////////////////////////////////////////

int FXmlBtn::AddGraphicsData_read_width( AST_TXML_Tag *node, FBtnGraphics& child, FBtnVM& /*vm*/ )
{
	child.width = LINETHICKNESS_MEDIUM;
	return ::bb_read_attr_linethickness( node, FBL_ATTR_width, child.width );
}

int FXmlBtn::AddGraphicsData( AST_TXML_Tag *root, FBtnVM& vm )
{
	long ret = 0;
	AST_TXML_Tag* tag = root->getFirstTag( FBL_TAG_objects );
	if( !tag ) return 0;

	long i, j;
	AST_TXML_Compound *comp = 0;
	AST_TXML_Node *node;

	std::vector<AST_TXML_Tag*> arrtags;
	tag->getAllTags( FBL_TAG_object, arrtags, 1 );

	for( i = 0; i < (long) arrtags.size(); i++ )
	{
		if( arrtags[ i ] )
		{
			// mandatory
			AST_TXML_Attribute	*left = 0, *right = 0, *top = 0, 
								*bottom = 0, *shape = 0;
			// non mandatory
			AST_TXML_Attribute	*name = 0, *type = 0, *width = 0,
								*pen_style = 0, *style = 0, *size = 0, *color = 0,
								*color_red = 0, *color_green = 0, 
								*color_blue = 0, *value = 0, *height = 0, 
								*_alt = 0, *_stretchy = 0, *_expr_enable = 0,
								*_bind_to = 0, *valign = 0, *halign = 0,
								*_offset_x = 0, *_offset_y = 0;

			if( !(comp = arrtags[ i ]->getAttributes()) ) continue;
			for( j = 0; j < comp->getChildNumber(); j++ )
			{
				node = comp->getChild( j );
				if( node->getNodeType() != ASTT_TXML_Attribute ) continue;
				if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_name ) )
					name = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_left ) )
					left = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_top ) )
					top = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_right ) )
					right = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_bottom ) )
					bottom = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_shape ) )
					shape = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_type ) )
					type = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_height ) )
					height = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_width ) )
					width = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_style ) )
					style = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_pen_style ) )
					pen_style = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_color_red ) )
					color_red = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_color_green ) )
					color_green = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_color_blue ) )
					color_blue = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_color ) )
					color = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_size ) )
					size = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_value ) )
					value = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_alt_group ) )
					_alt = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_stretchy ) )
					_stretchy = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_enable ) )
					_expr_enable = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_bind_to ) )
					_bind_to = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_halign ) )
					halign = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_valign ) )
					valign = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_offset_direction_x ) )
					_offset_x = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_offset_direction_y ) )
					_offset_y = (AST_TXML_Attribute*)node;
			}

			struct FBtnGraphics child;
			if( name && name->getValueType() == ASTVALT_TXML_String )
			{
				child.name = name->getStringValue();
			}
			else
			{
				child.name = QString("$%1$").arg(i);
			}
			child.line = arrtags[ i ]->getLine();

			if( !shape )
			{
				vm.dmm.AddLine( vm.dmm_page, _T("graphics shape field is mandatory"), arrtags[ i ]->getLine() );
				ret = -1;
				continue;
			}
			if( shape->getValueType() != ASTVALT_TXML_String )
			{
				vm.dmm.AddLine( vm.dmm_page, _T("graphics shape field must be of integer type"), arrtags[ i ]->getLine() );
				ret = -1;
				continue;
			}
			if( !html_strcmp( shape->getStringValue(), FBL_VAL_line ) )
				child.grType = FBtnGraphics::GRAPH_PRIM_LINE;
			else if( !html_strcmp( shape->getStringValue(), FBL_VAL_rectangle ) )
				child.grType = FBtnGraphics::GRAPH_PRIM_RECT;
			else if( !html_strcmp( shape->getStringValue(), FBL_VAL_ellipse ) )
			{
				child.grType = FBtnGraphics::GRAPH_PRIM_RECT;
				child.align |= ELLIPSE_MASK_ALIGN;
			}
			else if( !html_strcmp( shape->getStringValue(), FBL_VAL_text ) )
				child.grType = FBtnGraphics::GRAPH_PRIM_TEXT;
			else if( !html_strcmp( shape->getStringValue(), FBL_VAL_spline ) )
				child.grType = FBtnGraphics::GRAPH_PRIM_SPLINE;
			else if( !html_strcmp( shape->getStringValue(), FBL_VAL_spline_ex ) )
				child.grType = FBtnGraphics::GRAPH_PRIM_SPLINE_EX;
			else
			{
				vm.dmm.AddLine( vm.dmm_page, _T("graphics shape field must have one of the following value: line, rectangle, ellipse, text"), arrtags[ i ]->getLine() );
				ret = -1;
				child.grType = FBtnGraphics::GRAPH_PRIM_NONE;
				continue;
			}

			if( type && type->getValueType() == ASTVALT_TXML_String )
			{
				if( !html_strcmp( type->getStringValue(), FBL_VAL_percent ) )
					child.coord_type = COORD_TYPE_RATIO;
				else if( !html_strcmp( type->getStringValue(), FBL_VAL_formula ) )
					child.coord_type = COORD_TYPE_FORMULA;
			}

			if( _expr_enable && _expr_enable->getValueType() == ASTVALT_TXML_String )
			{
				if( !html_strcmp( _expr_enable->getStringValue(), FBL_VAL_on ) )
					child.expr_enable = 1;
				else if( !html_strcmp( _expr_enable->getStringValue(), FBL_VAL_off ) )
					child.expr_enable = 0;
			}

			AddGraphicsData_read_width( arrtags[ i ], child, vm );

			child.stretchy = 0;
			if( _stretchy )
			{
				if( _stretchy->getValueType() == ASTVALT_TXML_String )
					child.stretchy = !html_strcmp( _stretchy->getStringValue(), FBL_VAL_yes ) ? 1 : 0;
				else if( _stretchy->convert2Integer() == ELH_NO_ERROR )
					child.stretchy = _stretchy->getIntValue();
				else
					vm.dmm.AddLine( vm.dmm_page, _T("\'stretchy\' property must be logical value, assume FALSE by default"), arrtags[ i ]->getLine() );
			}

			int v = VERTICAL_CENTER, h = HORISONTAL_CENTER;
			if( valign && valign->getValueType() == ASTVALT_TXML_String )
			{
				if( !html_strcmp( valign->getStringValue(), FBL_VAL_center ) )
					v = VERTICAL_CENTER;
				else if( !html_strcmp( valign->getStringValue(), FBL_VAL_top ) )
					v = TO_TOP;
				else if( !html_strcmp( valign->getStringValue(), FBL_VAL_bottom ) )
					v = TO_BOTTOM;
			}
			if( halign && halign->getValueType() == ASTVALT_TXML_String )
			{
				if( !html_strcmp( halign->getStringValue(), FBL_VAL_center ) )
					h = HORISONTAL_CENTER;
				else if( !html_strcmp( halign->getStringValue(), FBL_VAL_left ) )
					h = TO_LEFT;
				else if( !html_strcmp( halign->getStringValue(), FBL_VAL_right ) )
					h = TO_RIGHT;
			}
			if( child.align & ELLIPSE_MASK_ALIGN )
				child.align = v | h | ELLIPSE_MASK_ALIGN;
			else
				child.align = v | h;

			child.bind_to = -1;
			if( _bind_to )
			{
				if( child.grType != FBtnGraphics::GRAPH_PRIM_TEXT )
					vm.dmm.AddLine( vm.dmm_page, _T("\'bind-to\' property is for text objects, it is ignored"), arrtags[ i ]->getLine() );
				else if( _bind_to->convert2Integer() == ELH_NO_ERROR && 
					_bind_to->getIntValue() > 0 && _bind_to->getIntValue() <= getChildCount() )
					child.bind_to = _bind_to->getIntValue();
				else
					vm.dmm.AddLine( vm.dmm_page, _T("\'bind-to\' property must be a positive number of child frame, it is disabled"), arrtags[ i ]->getLine() );
			}

			if( child.coord_type == COORD_TYPE_RATIO )
			{
				if( child.is_binded_text() )
				{
					if( left || top || right || bottom )
					{
						QString warn1 = _T("for the bound text object position fields (left, top, right, bottom) are ignored");
						vm.dmm.AddLine( vm.dmm_page, warn1, arrtags[ i ]->getLine() );
					}
				}
				else
				{
					if( !left || !top || !right || !bottom )
					{
						QString err3 = _T("for the percent scheme of drawing graphics position fields (left, top, right, bottom) are mandatory");
						vm.dmm.AddLine( vm.dmm_page, err3, arrtags[ i ]->getLine() );
						ret = -1;
						continue;
					}

					if( left->getValueType() != ASTVALT_TXML_Real && left->convert2Float() != ELH_NO_ERROR )
					{
						QString err4 = _T("graphics position fields left must be of integer or real type");
						vm.dmm.AddLine( vm.dmm_page, err4, arrtags[ i ]->getLine() );
						ret = -1;
						continue;
					}
					if( top->getValueType() != ASTVALT_TXML_Real && top->convert2Float() != ELH_NO_ERROR )
					{
						QString err4 = _T("graphics position fields top must be of integer or real type");
						vm.dmm.AddLine( vm.dmm_page, err4, arrtags[ i ]->getLine() );
						ret = -1;
						continue;
					}
					if( right->getValueType() != ASTVALT_TXML_Real && right->convert2Float() != ELH_NO_ERROR )
					{
						QString err4 = _T("graphics position fields right must be of integer or real type");
						vm.dmm.AddLine( vm.dmm_page, err4, arrtags[ i ]->getLine() );
						ret = -1;
						continue;
					}
					if( bottom->getValueType() != ASTVALT_TXML_Real && bottom->convert2Float() != ELH_NO_ERROR )
					{
						QString err4 = _T("graphics position fields bottom must be of integer or real type");
						vm.dmm.AddLine( vm.dmm_page, err4, arrtags[ i ]->getLine() );
						ret = -1;
						continue;
					}

					child.pos.setLeft( left->getFloatValue() );
					child.pos.setTop( top->getFloatValue() );
					child.pos.setRight( right->getFloatValue() );
					child.pos.setBottom( bottom->getFloatValue() );
				}
			}
			else
			{
				if( left || top || right || bottom )
				{
					QString warn1 = _T("for the formula scheme of drawing graphics position fields (left, top, right, bottom) are ignored");
					vm.dmm.AddLine( vm.dmm_page, warn1, arrtags[ i ]->getLine() );
				}
			}

			if( value ) child.text_reserve = value->getStringValue();

			if( height && height->getValueType() == ASTVALT_TXML_String )
			{
				if( !html_strcmp( height->getStringValue(), FBL_VAL_frame ) ||
					!html_strcmp( height->getStringValue(), FBL_VAL_parent ) )
					child.height = FTEXT_HEIGHT_FRAME;
				else if( !html_strcmp( height->getStringValue(), FBL_VAL_child ) ||
						 !html_strcmp( height->getStringValue(), FBL_VAL_child1 ) )
					child.height = FTEXT_HEIGHT_CHILD1;
				else if( !html_strcmp( height->getStringValue(), FBL_VAL_child2 ) )
					child.height = FTEXT_HEIGHT_CHILD2;
				else if( !html_strcmp( height->getStringValue(), FBL_VAL_default ) )
					child.height = FTEXT_HEIGHT_FRAME;
				else
					child.height = FTEXT_HEIGHT_FRAME;
			}
			else
				child.height = FTEXT_HEIGHT_FRAME;

			if( style && style->getValueType() == ASTVALT_TXML_String )
			{
				long id = ::getCurrentFormulatorStyle().getStyleByName( style->getStringValue() );
				if( id >= 0 )
					child.style = id;
				else
				{
					child.style = getCurrentFormulatorStyle_OnChar();
					QString warn4 = _T("style \"%1\" is unknown and ignored");
					vm.dmm.AddLine( vm.dmm_page, warn4, root->getLine(), style->getStringValue() );
				}
			}

			if( pen_style && pen_style->getValueType() == ASTVALT_TXML_String )
			{
				if( pen_style->getStringValue() == FBL_VAL_solid )
					child.style = Qt::SolidLine;
				else if( pen_style->getStringValue() == FBL_VAL_dash )
					child.style = Qt::DashLine;
				else if( pen_style->getStringValue() == FBL_VAL_dot )
					child.style = Qt::DotLine;
				else if( pen_style->getStringValue() == FBL_VAL_dash_dot )
					child.style = Qt::DashDotLine;
				else if( pen_style->getStringValue() == FBL_VAL_dash_dot_dot )
					child.style = Qt::DashDotDotLine;
				else
					child.style = Qt::SolidLine;
			}

			if( size && size->getValueType() == ASTVALT_TXML_String )
			{
				if( !html_strcmp( size->getStringValue(), FBL_VAL_fixed ) )
					child.sizeType = FIXED_SIZE;
				else if( !html_strcmp( size->getStringValue(), FBL_VAL_variable ) )
					child.sizeType = VARIABLE_SIZE;
			}

			unsigned char r = 0, g = 0, b = 0;
			if( color )
			{
				if( color_red )
				{
					QString warn2 = _T("attribute \"color_red\" is ignored");
					vm.dmm.AddLine( vm.dmm_page, warn2, arrtags[ i ]->getLine() );
				}
				if( color_green )
				{
					QString warn3 = _T("attribute \"color_green\" is ignored");
					vm.dmm.AddLine( vm.dmm_page, warn3, arrtags[ i ]->getLine() );
				}
				if( color_blue )
				{
					QString warn4 = _T("attribute \"color_blue\" is ignored");
					vm.dmm.AddLine( vm.dmm_page, warn4, arrtags[ i ]->getLine() );
				}
				if( !html_strcmp( color->getStringValue(), FBL_VAL_black ) )
					r = g = b = 0;
				else if( !html_strcmp( color->getStringValue(), FBL_VAL_white ) )
					r = g = b = 255;
				else if( !html_strcmp( color->getStringValue(), FBL_VAL_grey ) )
					r = g = b = 127;
				else if( !html_strcmp( color->getStringValue(), FBL_VAL_red ) )
					r = 127;
				else if( !html_strcmp( color->getStringValue(), FBL_VAL_green ) )
					g = 127;
				else if( !html_strcmp( color->getStringValue(), FBL_VAL_blue ) )
					b = 127;
				else if( !html_strcmp( color->getStringValue(), FBL_VAL_light_red ) )
					r = 255;
				else if( !html_strcmp( color->getStringValue(), FBL_VAL_light_green ) )
					g = 255;
				else if( !html_strcmp( color->getStringValue(), FBL_VAL_light_blue ) )
					b = 255;
				else
				{
					QString warn5 = _T("color value %1 is unknown and ignored");
					vm.dmm.AddLine( vm.dmm_page, warn5, arrtags[ i ]->getLine(), color->getStringValue() );
				}				
			}
			else
			{
				QString warn6 = _T("color value must be in range from 0 to 255");
				if( color_red && (color_red->getValueType() == ASTVALT_TXML_Int ||
					(color_red->getValueType() == ASTVALT_TXML_String 
					&& color_red->convert2Integer() == ELH_NO_ERROR)) )
				{
					if( color_red->getIntValue() < 0 || color_red->getIntValue() > 255 )
						vm.dmm.AddLine( vm.dmm_page, warn6, arrtags[ i ]->getLine() );
					else
						r = (unsigned char) color_red->getIntValue();
				}
				if( color_green && (color_green->getValueType() == ASTVALT_TXML_Int ||
					(color_green->getValueType() == ASTVALT_TXML_String 
					&& color_green->convert2Integer() == ELH_NO_ERROR)) )
				{
					if( color_green->getIntValue() < 0 || color_green->getIntValue() > 255 )
						vm.dmm.AddLine( vm.dmm_page, warn6, arrtags[ i ]->getLine() );
					else
						g = (unsigned char) color_green->getIntValue();
				}
				if( color_blue && (color_blue->getValueType() == ASTVALT_TXML_Int ||
					(color_blue->getValueType() == ASTVALT_TXML_String 
					&& color_blue->convert2Integer() == ELH_NO_ERROR)) )
				{
					if( color_blue->getIntValue() < 0 || color_blue->getIntValue() > 255 )
						vm.dmm.AddLine( vm.dmm_page, warn6, arrtags[ i ]->getLine() );
					else
						b = (unsigned char) color_blue->getIntValue();
				}
			}
			child.color = QColor( r, g, b );

			child.altGroup = 0;
			if( _alt )
			{
				if( _alt->convert2Integer() == ELH_NO_ERROR && _alt->getIntValue() >= 0 )
					child.altGroup = _alt->getIntValue();
				else
				{
					QString warn7 = _T("number of a group of alternative graphics must be integer non-negative value, assume 0 by default");
					vm.dmm.AddLine( vm.dmm_page, warn7, arrtags[ i ]->getLine() );
				}
			}

			if( child.grType == FBtnGraphics::GRAPH_PRIM_SPLINE || child.grType == FBtnGraphics::GRAPH_PRIM_SPLINE_EX )
			{
				if( AddGraphicsData_spline( arrtags[ i ], child, vm ) == -1 )
				{
					ret = -1;
					continue;
				}
			}

			if( _offset_x )
			{
				if( _offset_x->getValueType() == ASTVALT_TXML_Real || (_offset_x->getValueType() == ASTVALT_TXML_String && _offset_x->convert2Float() == ELH_NO_ERROR) )
				{
					child.offset_direction_x = (RealFde) _offset_x->getFloatValue();
				}
				else if( _offset_x->getValueType() == ASTVALT_TXML_Int || (_offset_x->getValueType() == ASTVALT_TXML_String && _offset_x->convert2Integer() == ELH_NO_ERROR) )
				{
					child.offset_direction_x = (RealFde) _offset_x->getIntValue();
				}
				else
				{
					QString warn = _T("attribute \"offset direction x\" is ignored");
					vm.dmm.AddLine( vm.dmm_page, warn, arrtags[ i ]->getLine() );
				}
			}
			if( _offset_y )
			{
				if( _offset_y->getValueType() == ASTVALT_TXML_Real || (_offset_y->getValueType() == ASTVALT_TXML_String && _offset_y->convert2Float() == ELH_NO_ERROR) )
				{
					child.offset_direction_y = (RealFde) _offset_y->getFloatValue();
				}
				else if( _offset_y->getValueType() == ASTVALT_TXML_Int || (_offset_y->getValueType() == ASTVALT_TXML_String && _offset_y->convert2Integer() == ELH_NO_ERROR) )
				{
					child.offset_direction_y = (RealFde) _offset_y->getIntValue();
				}
				else
				{
					QString warn = _T("attribute \"offset direction y\" is ignored");
					vm.dmm.AddLine( vm.dmm_page, warn, arrtags[ i ]->getLine() );
				}
			}

			FBtn::AddGraphicsData( child );
		}
	}
	if( ar_graphics.size() )
		qSort( ar_graphics.begin(), ar_graphics.end() );
	for( i = 0; i < (long) ar_graphics.size(); i++ )
		vm.AddPrimitive( ar_graphics[ i ].name, ar_graphics[ i ].line );

	validateVerticalCenter( vm );
	return ret;
}

void FXmlBtn::validateVerticalCenter( FBtnVM& vm )
{
	if( vertical_center_type == FBtnChildPos::Child &&
		(vertical_center_value < 1 || vertical_center_value > ar_graphics.size()) )
	{
		vm.dmm.AddLine( vm.dmm_page, QString("vertical center value (%1) is out of the childs range, vertical center option is ignored").arg(vertical_center_value) );
		vertical_center_type = FBtnChildPos::Default;
		vertical_center_value = 0;
	}
}

int FXmlBtn::AddGraphicsData_spline( AST_TXML_Tag *tag, FBtnGraphics &child, FBtnVM& vm )
{
	if( !tag )
		return 0;

	long ret = 0;
	long i, j;
	AST_TXML_Compound *comp = 0;
	AST_TXML_Node *node;

	std::vector<AST_TXML_Tag*> arrtags;
	tag->getAllTags( FBL_TAG_point, arrtags, 1 );

	child.point_type.clear();
	child.point_x.clear();
	child.point_y.clear();

	char c_value;
	RealFde f_value;

	for( i = 0; i < (long) arrtags.size(); i++ )
	{
		if( arrtags[ i ] )
		{
			if( !(comp = arrtags[ i ]->getAttributes()) ) continue;

			AST_TXML_Attribute	*type = 0, *x = 0, *y = 0;

			for( j = 0; j < comp->getChildNumber(); j++ )
			{
				node = comp->getChild( j );
				if( node->getNodeType() != ASTT_TXML_Attribute ) continue;
				if( ((AST_TXML_Attribute*) node)->isAttribute( FBL_ATTR_type ) )
					type = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_x ) )
					x = (AST_TXML_Attribute*)node;
				else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_y ) )
					y = (AST_TXML_Attribute*)node;
			}

			if( type == 0 )
			{
				QString err1 = _T("point type field is mandatory");
				vm.dmm.AddLine( vm.dmm_page, err1, arrtags[ i ]->getLine() );
				ret = -1;
				continue;
			}
			if( x == 0 )
			{
				QString err2 = _T("x coordinate field is mandatory");
				vm.dmm.AddLine( vm.dmm_page, err2, arrtags[ i ]->getLine() );
				ret = -1;
				continue;
			}
			if( y == 0 )
			{
				QString err3 = _T("y coordinate field is mandatory");
				vm.dmm.AddLine( vm.dmm_page, err3, arrtags[ i ]->getLine() );
				ret = -1;
				continue;
			}

			if( type && type->getValueType() == ASTVALT_TXML_String )
			{
				if( !html_strcmp( type->getStringValue(), FBL_VAL_lineto ) )
					c_value = spln_point_type_lineto;
				else if( !html_strcmp( type->getStringValue(), FBL_VAL_splnto ) )
					c_value = spln_point_type_splnto;
				else if( !html_strcmp( type->getStringValue(), FBL_VAL_moveto ) )
					c_value = spln_point_type_moveto;
				else
				{
					QString err4 = _T("\'type\' property must be \'moveto\', \'lineto\' or \'splnto\'");
					vm.dmm.AddLine( vm.dmm_page, err4, arrtags[ i ]->getLine() );
					ret = -1;
					continue;
				}
				child.point_type.push_back( c_value );
			}
			else
			{
				QString err5 = _T("\'type\' property must be QString value");
				vm.dmm.AddLine( vm.dmm_page, err5, arrtags[ i ]->getLine() );
				ret = -1;
				continue;
			}

			if( x )
			{
				if( x->getValueType() == ASTVALT_TXML_Real || (x->getValueType() == ASTVALT_TXML_String && x->convert2Float() == ELH_NO_ERROR) )
				{
					f_value = (RealFde) x->getFloatValue();
					child.point_x.push_back( f_value );
				}
				else if( x->getValueType() == ASTVALT_TXML_Int || (x->getValueType() == ASTVALT_TXML_String && x->convert2Integer() == ELH_NO_ERROR) )
				{
					f_value = (RealFde) x->getIntValue();
					child.point_x.push_back( f_value );
				}
				else
				{
					QString err6 = _T("\'x\' property must be integer or real value");
					vm.dmm.AddLine( vm.dmm_page, err6, arrtags[ i ]->getLine() );
					ret = -1;
					continue;
				}
			}

			if( y )
			{
				if( y->getValueType() == ASTVALT_TXML_Real || (y->getValueType() == ASTVALT_TXML_String && y->convert2Float() == ELH_NO_ERROR) )
				{
					f_value = (RealFde) y->getFloatValue();
					child.point_y.push_back( (RealFde) f_value );
				}
				else if( y->getValueType() == ASTVALT_TXML_Int || (y->getValueType() == ASTVALT_TXML_String && y->convert2Integer() == ELH_NO_ERROR) )
				{
					f_value = (RealFde) y->getIntValue();
					child.point_y.push_back( f_value );
				}
				else
				{
					QString err7 = _T("\'y\' property must be integer or real value");
					vm.dmm.AddLine( vm.dmm_page, err7, arrtags[ i ]->getLine() );
					ret = -1;
					continue;
				}
			}
		}
	}

	if( child.point_type.size() > 0 )
	{
		child.point_type[ child.point_type.size() - 1 ] |= spln_point_type_clsfig;
	}

	return ret;
}

/////////////////////////////////////////////////////////////////////

int bb_read_attr_linethickness( AST_TXML_Tag *node, const QString& attr_name, RealFde& child_width )
{
	if( !node ) return 0;
	AST_TXML_Attribute *width = node->getAttribute( attr_name );
	if( width && width->getValueType() == ASTVALT_TXML_String )
	{
		if( !html_strcmp( width->getStringValue(), FBL_VAL_medium ) )
			child_width = LINETHICKNESS_MEDIUM;
		else if( !html_strcmp( width->getStringValue(), FBL_VAL_thin ) )
			child_width = LINETHICKNESS_THIN;
		else if( !html_strcmp( width->getStringValue(), FBL_VAL_thick ) )
			child_width = LINETHICKNESS_THICK;
		else
		{
			RealFde value = child_width;
			if( ::mml_attr_unit_2_value( 0, value, width->getStringValue(), ::getCurrentFormulatorStyle().getUnit2PX() ) != -1 )
				child_width = value;
		}
	}
	return 0;
}

int bb_read_attr_align_vertical( AST_TXML_Tag *node, const QString& attr_name, long& child_align )
{
	if( !node ) return 0;
	AST_TXML_Attribute *align = node->getAttribute( attr_name );
	if( align )
	{
		QString value = align->getStringValue();
		if( !html_strcmp(value, FBL_VAL_center) )
			child_align = VERTICAL_CENTER;
		else if( !html_strcmp(value, FBL_VAL_top) )
			child_align = TO_TOP;
		else if( !html_strcmp(value, FBL_VAL_bottom) )
			child_align = TO_BOTTOM;
		else if( !html_strcmp(value, FBL_VAL_baseline) )
			child_align = TO_BASELINE;
	}
	return 0;
}

int bb_read_attr_align_horizontal( AST_TXML_Tag *node, const QString& attr_name, long& child_align )
{
	if( !node ) return 0;
	AST_TXML_Attribute *align = node->getAttribute( attr_name );
	if( align && align->getValueType() == ASTVALT_TXML_String )
	{
		if( !html_strcmp( align->getStringValue(), FBL_VAL_center ) )
			child_align = HORISONTAL_CENTER;
		else if( !html_strcmp( align->getStringValue(), FBL_VAL_left ) )
			child_align = TO_LEFT;
		else if( !html_strcmp( align->getStringValue(), FBL_VAL_right ) )
			child_align = TO_RIGHT;
	}
	return 0;
}

int bb_read_attr_align_vertical( AST_TXML_Tag *node, const QString& attr_name, std::vector<long>& child_align, long defaultAlign )
{
	if( !node ) return 0;

	child_align.clear();
	AST_TXML_Attribute *align = node->getAttribute( attr_name );
	if( align )
	{
		QString body, tmp;
		long align2add;
		if( align->getValueType() == ASTVALT_TXML_String && (body = align->getStringValue().trimmed()).length() > 0 )
		{
			QStringList lst = body.split(" ", QString::SkipEmptyParts);
			for( long i = 0; i < lst.size(); i++ )
			{
				tmp = lst.at(i).trimmed();
				if( !html_strcmp(tmp, FBL_VAL_top) )
					align2add = TO_TOP;
				else if( !html_strcmp(tmp, FBL_VAL_bottom) )
					align2add = TO_BOTTOM;
				else if( !html_strcmp(tmp, FBL_VAL_baseline) )
					align2add = TO_BASELINE;
				else if( !html_strcmp(tmp, FBL_VAL_center) )
					align2add = VERTICAL_CENTER;
				else
					align2add = defaultAlign;
				child_align.push_back( align2add );
			}
		}
	}
	return 0;
}

int bb_read_attr_align_horizontal( AST_TXML_Tag *node, const QString& attr_name, std::vector<long>& child_align, long defaultAlign )
{
	if( !node ) return 0;

	child_align.clear();
	AST_TXML_Attribute *align = node->getAttribute( attr_name );
	if( align )
	{
		QString body, tmp;
		long align2add;
		if( align->getValueType() == ASTVALT_TXML_String && (body = align->getStringValue().trimmed()).length() > 0 )
		{
			QStringList lst = body.split(" ", QString::SkipEmptyParts);
			for( long i = 0; i < lst.size(); i++ )
			{
				tmp = lst.at(i).trimmed();
				if( !html_strcmp(tmp, FBL_VAL_left) )
					align2add = TO_LEFT;
				else if( !html_strcmp(tmp, FBL_VAL_right) )
					align2add = TO_RIGHT;
				else if( !html_strcmp(tmp, FBL_VAL_center) )
					align2add = HORISONTAL_CENTER;
				else
					align2add = defaultAlign;
				child_align.push_back( align2add );
			}
		}
	}
	return 0;
}

int bb_read_attr_table_lines( AST_TXML_Tag *node, const QString& attr_name, std::vector<long>& penStyle )
{
	if( !node ) return 0;

	penStyle.clear();
	AST_TXML_Attribute *align = node->getAttribute( attr_name );
	if( align )
	{
		QString body, tmp;
		if( align->getValueType() == ASTVALT_TXML_String && (body = align->getStringValue().trimmed()).length() > 0 )
		{
			QStringList lst = body.split(" ", QString::SkipEmptyParts);
			for( long i = 0; i < lst.size(); i++ )
			{
				tmp = lst.at(i).trimmed();
				long oneStyle = -1;
				if( !html_strcmp(tmp, FBL_VAL_solid) )
					oneStyle = Qt::SolidLine;
				else if( !html_strcmp(tmp, FBL_VAL_dashed) )
					oneStyle = Qt::DashLine;
				//else if( !html_strcmp(tmp, FBL_VAL_none) )
				//	oneStyle = -1;
				//if( oneStyle != -1 )
					penStyle.push_back( oneStyle );
			}
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////
