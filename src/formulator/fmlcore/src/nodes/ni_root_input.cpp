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

#include "mathml/n_cm_treetemplate.h"
#include "rules/nav.h"
#include "rules/edit.h"
#include "ni_root.h"
#include "../dictionary/op_autokeyword.h"
#include "../dictionary/op_iddefs.h"

#include "../gui/widget/fmlbasewidget.h"
#include "../gui/dlg/nodeinputdlg.h"

/////////////////////////////////////////////////////////////////////////////
//
// CRootNode
//
/////////////////////////////////////////////////////////////////////////////

void CRootNode::createAutodetectMulticharOp()
{
	if( m_AutodetectMulticharOp )
	{
		delete m_AutodetectMulticharOp;
		m_AutodetectMulticharOp = 0;
	}
	m_AutodetectMulticharOp = new HAutodetectMulticharOp( *this );
}

void CRootNode::deleteAutodetectMulticharOp( void )
{
	if( m_AutodetectMulticharOp != NULL )
	{
		delete m_AutodetectMulticharOp;
		m_AutodetectMulticharOp = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////

int CRootNode::InsertMathML_PreprocessButtonContent( 
	FXmlBtn *&ptrFXmlBtn, TXML_Storage* bms )
{
	Q_ASSERT( owner() );
	static QString typeV(FBL_ATTR_type);
	static QString nargsV(FBL_ATTR_nargs);
	static QString occurrenceV(FBL_ATTR_occurrence);
	static QString sepV(FBL_TAG_sep);
	static QString baseV(FBL_ATTR_base);

	int iRet = 0;
	if( ptrFXmlBtn->getID() == HF_BTNID_DECLARE_INPUT )
	{
		CMmlDeclareDlg dlg;
		if( owner()->callDialog( &dlg ) == QDialog::Accepted )
		{
			QString type = dlg.type();
			int argc = dlg.argc();
			QString occurance = dlg.occurance();
			bool isSecondSlot = dlg.isSecondSlot();

			AST_TXML_Tag* _ci = bms->getFirstTag( FBL_TAG_declare );
			if( _ci )
			{
				AST_TXML_Compound *_attr = _ci->getAttributes() ? _ci->getAttributes() : new AST_TXML_Compound();
				if( type.length() ) _attr->Add( new AST_TXML_Attribute( &typeV, &type ) );
				QString sV = QString("%1").arg(argc);
				if( argc > 0 ) _attr->Add( new AST_TXML_Attribute( &nargsV, &sV ) );
				if( type != CONTENT_MATHML_CN_TYPE_NONE )
					_attr->Add( new AST_TXML_Attribute( &occurrenceV, &occurance ) );
				if( isSecondSlot )
				{
					AST_TXML_Node* _node2clone = 0;	// used as a template node to copy in rows and columns
					AST_TXML_Compound* _tags = _ci->getTags();
					if( _tags && _tags->getChildNumber() > 0 && (_node2clone = _tags->getChild( _tags->getChildNumber() - 1 )) != 0 )
						_tags->Add( _node2clone->Clone() );
				}
				_ci->setAttributes( _attr );
			}
		}
		else
			iRet = -1;
	}
	else if( ptrFXmlBtn->getID() == HF_BTNID_CI_INPUT )
	{
		CMmlCiDlg dlg;
		if( owner()->callDialog( &dlg ) == QDialog::Accepted )
		{
			QString type = dlg.type();
			AST_TXML_Tag* _ci = bms->getFirstTag( FBL_TAG_ci );
			if( _ci && !_ci->getAttributes() && type.length() )
			{
				AST_TXML_Compound *_attr = new AST_TXML_Compound();
				if( type.length() )
					_attr->Add( new AST_TXML_Attribute( &typeV, &type ) );
				_ci->setAttributes( _attr );
			}
		}
		else
			iRet = -1;
	}
	else if( ptrFXmlBtn->getID() == HF_BTNID_CN_INPUT )
	{
		CMmlCnDlg dlg;
		if( owner()->callDialog( &dlg ) == QDialog::Accepted )
		{
			QString type = dlg.type();
			int base = dlg.base();
			AST_TXML_Tag* _cn = bms->getFirstTag( FBL_TAG_cn );
			if( _cn && !_cn->getAttributes() && 
				((type.length() && type != CONTENT_MATHML_CN_TYPE_NONE) || base != CONTENT_MATHML_CN_DEFAULT_BASE) )
			{
				AST_TXML_Compound *_attr = new AST_TXML_Compound();
				AST_TXML_Compound *_tags = 0;
				if( type.length() && type != CONTENT_MATHML_CN_TYPE_NONE )
				{
					_attr->Add( new AST_TXML_Attribute( &typeV, &type ) );
					if( type == FBL_VAL_rational ||
						type == FBL_VAL_complex_cartesian ||
						type == FBL_VAL_complex_polar )
					{
						_tags = _cn->getTags();
						if( !_tags ) _tags = new AST_TXML_Compound();
						_tags->Add( new AST_TXML_Tag( &sepV, (AST_TXML_Compound*)0, (AST_TXML_Tag*)0 ) );
						// additional fix of the vertical center for rational numbers
						if( type == FBL_VAL_rational )
						{
							FXmlBtn *newCalcCenter = ::getCurrentToolBarSet()->getButtonCalc( QString(HF_BTNID_CN_RATIONAL) );
							if( newCalcCenter ) ptrFXmlBtn = newCalcCenter;
						}
					}
				}
				if( base != CONTENT_MATHML_CN_DEFAULT_BASE )
				{
					QString sV = QString("%1").arg(base);
					_attr->Add( new AST_TXML_Attribute( &baseV, &sV ) );
				}
				_cn->setAttributes( _attr );
				if( _tags ) _cn->setTags( _tags );
			}
		}
		else
			iRet = -1;
	}
	else if( ptrFXmlBtn->getID() == HF_BTNID_QUALIFIER1_INPUT || 
			 ptrFXmlBtn->getID() == HF_BTNID_QUALIFIER2_INPUT ||
			 ptrFXmlBtn->getID() == HF_BTNID_QUALIFIER3_INPUT ||
			 ptrFXmlBtn->getID() == HF_BTNID_QUALIFIER4_INPUT )
	{
		CMmlQualifierDlg dlg( ptrFXmlBtn->getName(), 
			!(ptrFXmlBtn->getID() == HF_BTNID_QUALIFIER1_INPUT || ptrFXmlBtn->getID() == HF_BTNID_QUALIFIER2_INPUT) );
		if( owner()->callDialog( &dlg ) == QDialog::Accepted )
		{
			QString op = dlg.op();
			int argc = dlg.bvar();
			if( op.length() )
			{
				AST_TXML_Node* _node2clone = 0;
				AST_TXML_Compound* _tags = 0;
				AST_TXML_Tag* _parent2clone = bms->getFirstTag( FBL_TAG_apply );
				if( _parent2clone )
				{
					_tags = _parent2clone->getTags();
					if( _tags && _tags->getChildNumber() >= 1 )
					{
						_node2clone = _tags->getChild( 0 );
						if( _node2clone != 0 )
						{
							for( long i = 0; i < argc - 1; i++ )
								_tags->InsertChild( 0, _node2clone->Clone() );
						}
						_tags->InsertChild( 0, new AST_TXML_Tag( &op, 0, (AST_TXML_Tag*)0 ) );
					}
				}
			}
		}
		else
			iRet = -1;
	}
	else
	{
		if ( ptrFXmlBtn->isPropertyInput() )
		{
			int a = ptrFXmlBtn->isPropertyInput( FBL_ATTR_rows );
			int b = ptrFXmlBtn->isPropertyInput( FBL_ATTR_columns );
			if( a || b )
			{
				QString strTitle = _T("");
				long args_plus = 0, minArgs = 1, arg2Clone = -1, pos2Insert = -1;
				QString tagName = _T("");
				if( ptrFXmlBtn->getID() == HF_BTNID_SELECTOR_INPUT )
				{
					strTitle = QObject::tr("Indexing into vectors, matrices and lists");
					args_plus = 2;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_APPLY_INPUT )
				{
					strTitle = QObject::tr("Arguments of a function or operator to be applied");
					args_plus = 1;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_COMPOSE_INPUT )
				{
					strTitle = QObject::tr("Arguments of the function composition operator");
					args_plus = 1;
					minArgs = 2;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_AND_INPUT )
				{
					strTitle = QString(QObject::tr("Arguments of the %1 operator")).arg(QObject::tr("'and'"));
					args_plus = 1;
					minArgs = 2;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_OR_INPUT )
				{
					strTitle = QString(QObject::tr("Arguments of the %1 operator")).arg(QObject::tr("'or'"));
					args_plus = 1;
					minArgs = 2;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_XOR_INPUT )
				{
					strTitle = QString(QObject::tr("Arguments of the %1 operator")).arg(QObject::tr("'exclusive or'"));
					args_plus = 1;
					minArgs = 2;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_PLUS_INPUT )
				{
					strTitle = QString(QObject::tr("Arguments of the %1 operator")).arg(QObject::tr("'plus'"));
					args_plus = 1;
					minArgs = 1;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_EQ_INPUT )
				{
					strTitle = QString(QObject::tr("Arguments of the %1 operator")).arg(QObject::tr("'equals'"));
					args_plus = 1;
					minArgs = 2;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_EQUIVALENT_INPUT )
				{
					strTitle = QString(QObject::tr("Arguments of the %1 operator")).arg(QObject::tr("'equivalence'"));
					args_plus = 1;
					minArgs = 2;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_GT_INPUT )
				{
					strTitle = QString(QObject::tr("Arguments of the %1 operator")).arg(QObject::tr("'greater than'"));
					args_plus = 1;
					minArgs = 2;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_LT_INPUT )
				{
					strTitle = QString(QObject::tr("Arguments of the %1 operator")).arg(QObject::tr("'less than'"));
					args_plus = 1;
					minArgs = 2;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_GEQ_INPUT )
				{
					strTitle = QString(QObject::tr("Arguments of the %1 operator")).arg(QObject::tr("'greater than or equal'"));
					args_plus = 1;
					minArgs = 2;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_LEQ_INPUT )
				{
					strTitle = QString(QObject::tr("Arguments of the %1 operator")).arg(QObject::tr("'less than or equal'"));
					args_plus = 1;
					minArgs = 2;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_TIMES_INPUT )
				{
					strTitle = QString(QObject::tr("Arguments of the %1 operator")).arg(QObject::tr("'times'"));
					args_plus = 1;
					minArgs = 2;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_GCD_INPUT )
				{
					strTitle = QString(QObject::tr("Arguments of the %1 operator")).arg(QObject::tr("'greatest common divisor'"));
					args_plus = 1;
					minArgs = 1;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_LCM_INPUT )
				{
					strTitle = QString(QObject::tr("Arguments of the %1 operator")).arg(QObject::tr("'lowest common multiple'"));
					args_plus = 1;
					minArgs = 1;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_MAX_INPUT )
				{
					strTitle = QString(QObject::tr("Arguments of the %1 operator")).arg(QObject::tr("'maximum'"));
					args_plus = 1;
					minArgs = 1;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_MIN_INPUT )
				{
					strTitle = QString(QObject::tr("Arguments of the %1 operator")).arg(QObject::tr("'minimum'"));
					args_plus = 1;
					minArgs = 1;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_MAX_BVAR_INPUT )
				{
					strTitle = QString(QObject::tr("Number of bound variables in the %1 operator")).arg(QObject::tr("'maximum'"));
					args_plus = 3;
					minArgs = 1;
					arg2Clone = 1;
					pos2Insert = 1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_MIN_BVAR_INPUT )
				{
					strTitle = QString(QObject::tr("Number of bound variables in the %1 operator")).arg(QObject::tr("'minimum'"));
					args_plus = 3;
					minArgs = 1;
					arg2Clone = 1;
					pos2Insert = 1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_FORALL_BVAR_INPUT )
				{
					strTitle = QString(QObject::tr("Number of bound variables in the %1 operator")).arg(QObject::tr("'forall'"));
					args_plus = 2;
					minArgs = 1;
					arg2Clone = 1;
					pos2Insert = 1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_EXISTS_BVAR_INPUT )
				{
					strTitle = QString(QObject::tr("Number of bound variables in the %1 operator")).arg(QObject::tr("'exists'"));
					args_plus = 2;
					minArgs = 1;
					arg2Clone = 1;
					pos2Insert = 1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_FORALL_BVARCOND_INPUT )
				{
					strTitle = QString(QObject::tr("Conditions of the %1 operator")).arg(QObject::tr("'forall'"));
					args_plus = 3;
					minArgs = 1;
					arg2Clone = 1;
					pos2Insert = 1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_EXISTS_BVARCOND_INPUT )
				{
					strTitle = QString(QObject::tr("Conditions of the %1 operator")).arg(QObject::tr("'exists'"));
					args_plus = 3;
					minArgs = 1;
					arg2Clone = 1;
					pos2Insert = 1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_PIECEWISE_INPUT )
				{
					strTitle = QObject::tr("Number of pieces in the 'piecewise' declaration");
					args_plus = 0;
					minArgs = 1;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_piecewise;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_PIECEWISEOTHER_INPUT )
				{
					strTitle = QObject::tr("Number of pieces in the 'piecewise' declaration");
					args_plus = 1;
					minArgs = 1;
					arg2Clone = 0;
					pos2Insert = -1;
					tagName = FBL_TAG_piecewise;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_PD1_INPUT || 
						 ptrFXmlBtn->getID() == HF_BTNID_PD2_INPUT )
				{
					strTitle = QString(QObject::tr("Number of bound variables in the %1 operator")).arg(QObject::tr("'partialdiff'"));
					args_plus = 2;
					minArgs = 1;
					arg2Clone = 1;
					pos2Insert = 1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_PD3_INPUT )
				{
					strTitle = QString(QObject::tr("Number of bound variables in the %1 operator")).arg(QObject::tr("'partialdiff'"));
					args_plus = 3;
					minArgs = 1;
					arg2Clone = 1;
					pos2Insert = 1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_PDLIST_INPUT )
				{
					strTitle = QString(QObject::tr("Number of indices in the %1 operator")).arg(QObject::tr("'partialdiff'"));
					args_plus = 0;
					minArgs = 1;
					arg2Clone = 0;
					pos2Insert = -1;
					tagName = FBL_TAG_list;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_SET_INPUT )
				{
					strTitle = QString(QObject::tr("Number of elements in the %1 constructor")).arg(QObject::tr("'set'"));
					args_plus = 0;
					minArgs = 1;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_set;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_SET_BVAR_INPUT )
				{
					strTitle = QString(QObject::tr("Number of bound variables in the %1 constructor")).arg(QObject::tr("'set'"));
					args_plus = 2;
					minArgs = 1;
					arg2Clone = 0;
					pos2Insert = 1;
					tagName = FBL_TAG_set;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_LIST_INPUT )
				{
					strTitle = QString(QObject::tr("Number of elements in the %1 constructor")).arg(QObject::tr("'list'"));
					args_plus = 0;
					minArgs = 1;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_list;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_LIST_BVAR_INPUT )
				{
					strTitle = QString(QObject::tr("Number of bound variables in the %1 constructor")).arg(QObject::tr("'list'"));
					args_plus = 2;
					minArgs = 1;
					arg2Clone = 0;
					pos2Insert = 1;
					tagName = FBL_TAG_list;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_UNION_INPUT )
				{
					strTitle = QString(QObject::tr("Arguments of the %1 operator")).arg(QObject::tr("'union'"));
					args_plus = 1;
					minArgs = 2;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_INTERSECT_INPUT )
				{
					strTitle = QString(QObject::tr("Arguments of the %1 operator")).arg(QObject::tr("'intersect'"));
					args_plus = 1;
					minArgs = 2;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_CARTESIAN_INPUT )
				{
					strTitle = QString(QObject::tr("Arguments of the %1 operator")).arg(QObject::tr("'Cartesian product'"));
					args_plus = 1;
					minArgs = 2;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_MEAN_INPUT )
				{
					strTitle = QString(QObject::tr("Arguments of the %1 operator")).arg(QObject::tr("'mean'"));
					args_plus = 1;
					minArgs = 1;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_SDEV_INPUT )
				{
					strTitle = QString(QObject::tr("Arguments of the %1 operator")).arg(QObject::tr("'sdev'"));
					args_plus = 1;
					minArgs = 1;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}

				else if( ptrFXmlBtn->getID() == HF_BTNID_VARIANCE_INPUT )
				{
					strTitle = QString(QObject::tr("Arguments of the %1 operator")).arg(QObject::tr("'variance'"));
					args_plus = 1;
					minArgs = 1;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_MEDIAN_INPUT )
				{
					strTitle = QString(QObject::tr("Arguments of the %1 operator")).arg(QObject::tr("'median'"));
					args_plus = 1;
					minArgs = 1;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}
				else if( ptrFXmlBtn->getID() == HF_BTNID_MODE_INPUT )
				{
					strTitle = QString(QObject::tr("Arguments of the %1 operator")).arg(QObject::tr("'mode'"));
					args_plus = 1;
					minArgs = 1;
					arg2Clone = -1;
					pos2Insert = -1;
					tagName = FBL_TAG_apply;
				}

				if( strTitle.length() > 0 )
				{
					QMmlSelectorDlg dlg( strTitle, minArgs );
					ptrFXmlBtn->setRows( 1 );
					ptrFXmlBtn->setColumns( 1 );
					if( owner()->callDialog( &dlg ) == QDialog::Accepted )
					{
						int argc = dlg.argc();
						int i, lim;
						AST_TXML_Node* _node2clone = 0;	// used as a template node to copy in rows and columns
						AST_TXML_Compound* _tags = 0;
						AST_TXML_Tag* _parent2clone = bms->getFirstTag( tagName );
						if( _parent2clone )
						{
							_tags = _parent2clone->getTags();
							if( _tags && _tags->getChildNumber() >= 1 )
							{
								if( arg2Clone >= 0 && _tags->getChildNumber() > arg2Clone )
									_node2clone = _tags->getChild( arg2Clone );
								else
									_node2clone = _tags->getChild( _tags->getChildNumber() - 1 );
								if( _node2clone != 0 )
								{
									lim = argc + args_plus - _tags->getChildNumber();
									for( i = 0; i < lim; i++ )
									{
										if( pos2Insert >= 0 && _tags->getChildNumber() > pos2Insert )
											_tags->InsertChild( pos2Insert, _node2clone->Clone() );
										else
											_tags->Add( _node2clone->Clone() );
									}
								}
							}
						}
					}
					else
						iRet = -1;
				}
				else
				{
					CMmlTableDlg dlg( a, a ? 5 : ptrFXmlBtn->getRows(), b, b ? 5 : ptrFXmlBtn->getColumns() );
					if( owner()->callDialog( &dlg ) == QDialog::Accepted )
					{
						int rows = dlg.rows();
						int columns = dlg.columns();
						ptrFXmlBtn->setRows( rows );
						ptrFXmlBtn->setColumns( columns );
    
						int i, lim;
						AST_TXML_Node* _node2clone = 0;	// used as a template node to copy in rows and columns
						AST_TXML_Compound* _tags = 0;
						AST_TXML_Tag* _matrix2clone = 0, *_parent2clone = bms->getFirstTag( FBL_TAG_vector );
						if( _parent2clone )
						{
							_tags = _parent2clone->getTags();
							if( _tags && _tags->getChildNumber() > 0 && (_node2clone = _tags->getChild( 0 )) != 0 )
							{
								lim = ptrFXmlBtn->getRows() - _tags->getChildNumber();
								for( i = 0; i < lim; i++ )
									_tags->Add( _node2clone->Clone() );
							}
						}
						else if( (_matrix2clone = bms->getFirstTag( FBL_TAG_matrix )) != 0 )
						{
							if( (_parent2clone = _matrix2clone->getFirstTag( FBL_TAG_matrixrow )) != 0 )
							{
								_tags = _parent2clone->getTags();
								if( _tags && _tags->getChildNumber() > 0 && (_node2clone = _tags->getChild( 0 )) != 0 )
								{
									lim = ptrFXmlBtn->getColumns() - _tags->getChildNumber();
									for( i = 0; i < lim; i++ )
										_tags->Add( _node2clone->Clone() );
								}
    
								_tags = _matrix2clone->getTags();
								if( _tags && _tags->getChildNumber() > 0 && (_node2clone = _tags->getChild( 0 )) != 0 )
								{
									lim = ptrFXmlBtn->getRows() - _tags->getChildNumber();
									for( i = 0; i < lim; i++ )
										_tags->Add( _node2clone->Clone() );
								}
							}
						}
					}
					else
						iRet = -1;
				}
			}
		}
	}

	return iRet;
}

/////////////////////////////////////////////////////////////////////////////
//

HAutodetectMulticharOp::HAutodetectMulticharOp( CRootNode& rootNode )
{
	m_line = new CLineNode();
	const int opNameLen = 4;
	static const QString opName[ opNameLen ] = { _T("+-"), _T(">="), _T("<="), _T("!=") };

	QString btnID;
	for( long i = 0; i < opNameLen; i++ )
	{
		if( getCMKeyword2ButtonID( opName[ i ], btnID ) )
			init( rootNode, m_data2, opName[ i ], btnID );
	}
	
	const HKMap<QString, QString>& opList = ::getText2ButtonIDMap();
	for( HKMap<QString, QString>::const_iterator iter = opList.begin(); iter != opList.end(); iter++ )
	{
		init( rootNode, m_data1, iter->first, iter->second );
	}
}

HAutodetectMulticharOp::~HAutodetectMulticharOp()
{
	if( m_line )
	{
		m_line->DeleteAllChild();
		delete m_line;
		m_line = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////

void HAutodetectMulticharOp::init( CRootNode& rootNode, HKMap<QString, CPlaneText*>& data, QString opName, QString btnID )
{
	CFormulaNode *frmParentNode = 0;
	CNode *pNode = 0, *pOpNode = 0;

	long tbr, btn;
	if( ::getCurrentToolBarSet()->getButtonCoordByID( btnID, tbr, btn ) != -1 )
	{
		if( rootNode.Insert( ::getCurrentToolBarSet()->getButtonCalc( tbr, btn ) ) != -1 )
			pNode = rootNode.GetCaret().GetNode();
	}

	if( !pNode )
		return;
	if( pNode->GetType() == NODE_LINE )
	{
		short parent_precedence = mmlPrecedence_Atom;
		short parent_assoc = 0;
		if( (frmParentNode = pNode->getParentContentFormulaNode( 1 )) != 0 )
		{
			for( CNode *curNode = frmParentNode; curNode != 0; curNode = curNode->GetParent() )
			{
				if( curNode->GetType() == NODE_FORMULA && curNode->to_mathml_data.is_content != 0 &&
						::mml_is_cm_apply( curNode->to_mathml_data.name ) )
				{
					if( btnID == HF_BTNID_PLUS_MINUS )
						parent_precedence = mmlPrecedence_add;
					else
						parent_precedence = ((CFormulaNode*) curNode)->to_mathml_data.precedence;
					parent_assoc = ((CFormulaNode*) curNode)->to_mathml_data.assoc;
					break;
				}
			}
			if( (pOpNode = frmParentNode->GetPrev()) == 0 )
				pOpNode = frmParentNode->GetNext();
		}
		if( pOpNode && pOpNode->GetType() == NODE_FORMULA )
		{
			pOpNode = ((CFormulaNode*) pOpNode)->getFormulaDeepChild( 0, 0 );
			if( pOpNode && pOpNode->GetType() == NODE_PLANETEXT && pOpNode->isCannotBeMoved() )
			{
				((CPlaneText*) pOpNode)->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__APPLY_OPERATOR_RECURSIVE_TREE;
				((CPlaneText*) pOpNode)->to_mathml_data.name = _T("");
				((CPlaneText*) pOpNode)->to_mathml_data.reserved = FBL_TAG_csymbol;
			}
		}
		if( pOpNode && pOpNode->GetType() == NODE_PLANETEXT && pOpNode->isCannotBeMoved() )
		{
			CPlaneText *toStore = new CPlaneText( (CPlaneText*) pOpNode );
			toStore->to_mathml_data.precedence = parent_precedence;
			toStore->to_mathml_data.assoc = parent_assoc;
			data[ opName ] = toStore;
			m_line->AddChild( toStore );
		}
	}

	::MoveLeftRight( rootNode.GetCaret(), rootNode.GetSelectInfo(), 1, 1, 1, 0 );
	::MoveDel( rootNode.GetCaret(), rootNode.GetSelectInfo(), rootNode.getAutodetectMulticharOp(), NULL );
	Q_ASSERT( isValidPosition( rootNode.GetCaret() ) );
}

/////////////////////////////////////////////////////////////////////////////

CPlaneText* HAutodetectMulticharOp::getOperator1( QString opName )
{
	CPlaneText **pp = m_data1.is_find( opName );
	return pp ? *pp : 0;
}

CPlaneText* HAutodetectMulticharOp::getOperator2( QString opName )
{
	CPlaneText **pp = m_data2.is_find( opName );
	return pp ? *pp : 0;
}

/////////////////////////////////////////////////////////////////////////////
