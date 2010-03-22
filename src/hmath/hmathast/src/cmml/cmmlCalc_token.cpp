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

#include "cmmlCalc.h"
#include "HUtils/idfas.h"
#include "../ast/astdsr.h"
#include "../vm/vmeval.h"
#include "../symbol/symbol_table_dsr.h"

///////////////////////////////////////////////////////////////////////////////

void parseCnAttr( AST_TXML_Tag *node, QString& base, long& typeidx )
{
	static QString cn_types[] = 
	{
		QString( _T("real") ),
		QString( _T("integer") ),
		QString( _T("rational") ),
		QString( _T("complex-cartesian") ),
		QString( _T("complex-polar") ),
		QString( _T("constant") )
	};
	static long sizearr = sizeof(cn_types) / sizeof(cn_types[0]);
	static HKMap2Long<QString> cn2type(cn_types, sizearr);

	base = CONTENT_MATHML_CN_DEFAULT_BASE_STR;
	typeidx = 0;

	AST_TXML_Attribute *base_attr = node->getAttribute( FBL_ATTR_base );
	if( base_attr )
	{
		QString bs = base_attr->getStringValue();
		long _b = bs.length();
		if( _b >= 2 && _b <= 36 )
			base = bs;
	}

	AST_TXML_Attribute *type_attr = node->getAttribute( FBL_ATTR_type );
	if( type_attr )
	{
		long *p = cn2type.isFind(type_attr->getStringValue());
		if( p ) typeidx = *p;
	}
}

///////////////////////////////////////////////////////////////////////////////

CDsrAST_Node* CMMLCalc::Create_constant( QString varName, long line )
{
	if( !m_dsrSymbolTable ) return NULL;
	long *nameID = m_dsrSymbolTable->is_find( varName );
	if( !nameID )
	{
		m_dmm.AddLine( m_dmmPage, getErrorMsg(CMMLError_UnknownConstant), QString(m_taskName), line, varName );
		return NULL;
	}
	CDsrAST_CompoundID *compoundNode = new CDsrAST_CompoundID();
	CDsrAST_PrimaryID *primaryNode = new CDsrAST_PrimaryID( *nameID );
	primaryNode->line = line;
	compoundNode->Add( primaryNode );
	compoundNode->line = line;
	return compoundNode;
}

CDsrAST_Node* CMMLCalc::Create_ci( AST_TXML_Tag* ciNode )
{
	if( !m_dsrSymbolTable ) return NULL;

	CDsrAST_Node *pRes = NULL;
	int iRes = CMMLError_NONE;

	if( !ciNode ) return pRes;
	if( !ciNode->isValue() )
	{
		iRes = CMMLError_EmptyCI;
	}
	else
	{
		CParseDsrSymbol *smb = NULL;
		long idx = lookupSymbol( ciNode->getText(), smb );
		if( idx == -1 )
		{
			AST_TXML_Attribute *attrType = ciNode->getAttribute( FBL_ATTR_type );
			CDsrDataType_Node *typeNode = new CDsrDataType_Node( attrType ? ::mapMMLValueTypeAttr2Dsr( attrType->getStringValue() ) : DSRDATA_TYPE_REAL );
			typeNode->setLValue();
			idx = addDsrSymbol2Table( getCurrentSymbolTable(), ciNode->getText(), SMBTABLE_TYPE_VARIABLE, typeNode );
		}
		CDsrAST_CompoundID *compoundNode = new CDsrAST_CompoundID();
		CDsrAST_PrimaryID *primaryNode = new CDsrAST_PrimaryID( idx );
		primaryNode->line = ciNode->getLine();
		compoundNode->Add( primaryNode );
		compoundNode->line = ciNode->getLine();
		pRes = compoundNode;
	}

	if( iRes != CMMLError_NONE )
		m_dmm.AddLine( m_dmmPage, getErrorMsg(iRes), QString(m_taskName), ciNode->getLine(), ciNode->getTagName() );
	return pRes;
}

CDsrAST_Node* CMMLCalc::Create_cn( AST_TXML_Tag* cnNode )
{
	if( !m_dsrSymbolTable ) return NULL;

	CDsrAST_Node *pRes = NULL;
	int iRes = CMMLError_NONE;

	if( !cnNode ) return pRes;
	if( !cnNode->isValue() )
	{
		iRes = CMMLError_EmptyCI;
	}
	else
	{
		QString base = _T("");
		long typeidx = -1;
		::parseCnAttr( cnNode, base, typeidx );
		if( typeidx == CONTENT_MATHML_CN_TYPE_CONSTANT )
		{
			CParseDsrSymbol *smb = NULL;
			long idx = lookupSymbol( cnNode->getText(), smb );
			if( idx == -1 )
				iRes = CMMLError_UnknownConstant;
			else
			{
				CDsrAST_CompoundID *compoundNode = new CDsrAST_CompoundID();
				CDsrAST_PrimaryID *primaryNode = new CDsrAST_PrimaryID( idx );
				primaryNode->line = cnNode->getLine();
				compoundNode->Add( primaryNode );
				pRes = compoundNode;
			}
		}
		else
		{
			QString aVal( _T("") ), bVal( _T("") );
			long state = 1;
			for( long i = 0; i < cnNode->getTags()->getChildNumber(); i++ )
			{
				AST_TXML_Node *node = cnNode->getTags()->getChild( i );
				if( !node ) continue;
				switch( node->getNodeType() )
				{
				case ASTT_TXML_Value:
					if( state == 1 )
					{ aVal = ((AST_TXML_Value*) node)->getText(); state = 2; }
					else if( state == 3 )
					{ bVal = ((AST_TXML_Value*) node)->getText(); state = 4; }
					else
					{ iRes = CMMLError_CNSyntax; break; }
					break;
				case ASTT_TXML_Tag:
					if( !xml_strcmp( ((AST_TXML_Tag*) node)->getTagName(), FBL_TAG_sep ) && state == 2 )
					{ state = 3; }
					else
					{ iRes = CMMLError_CNSyntax; break; }
				default:
					break;
				}
			}
			if( iRes == CMMLError_NONE )
			{
				int pos = aVal.indexOf(_T("&minus;"));
				if( pos != -1 ) aVal.replace( pos, strlen("&minus;"), "-" );
				if( state == 4 )
				{
					pos = bVal.indexOf(_T("&minus;"));
					if( pos != -1 ) bVal.replace( pos, strlen("&minus;"), "-" );
				}

				switch( typeidx )
				{
				case CONTENT_MATHML_CN_TYPE_REAL:
					{
						double valDbl;
						if( state == 2 && ::_hatof_base( QStringRef(&aVal), base, valDbl ) == ELH_NO_ERROR )
							pRes = new CDsrAST_ConstReal( valDbl, aVal );
						else
							iRes = CMMLError_CNSyntax;
					}
					break;
				case CONTENT_MATHML_CN_TYPE_INTEGER:
					{
						long valLong;
						if( state == 2 && ::_hatol_base( QStringRef(&aVal), base, valLong ) == ELH_NO_ERROR )
							pRes = new CDsrAST_ConstInt( valLong, aVal );
						else
							iRes = CMMLError_CNSyntax;
					}
					break;
				case CONTENT_MATHML_CN_TYPE_RATIONAL:
					{
						long valLongA, valLongB;
						if( state == 4 && ::_hatol_base( QStringRef(&aVal), base, valLongA ) == ELH_NO_ERROR && ::_hatol_base( QStringRef(&bVal), base, valLongB ) == ELH_NO_ERROR )
							pRes = constructConstRational( valLongA, aVal, valLongB, bVal, cnNode->getLine() );
						else
							iRes = CMMLError_CNSyntax;
					}
					break;
				case CONTENT_MATHML_CN_TYPE_COMPLEX_C:
					{
						double valDblA, valDblB;
						if( state == 4 && ::_hatof_base( QStringRef(&aVal), base, valDblA ) == ELH_NO_ERROR && ::_hatof_base( QStringRef(&bVal), base, valDblB ) == ELH_NO_ERROR )
							pRes = constructConstComplexCartesian( valDblA, aVal, valDblB, bVal, cnNode->getLine() );
						else
							iRes = CMMLError_CNSyntax;
					}
					break;
				case CONTENT_MATHML_CN_TYPE_COMPLEX_P:
					{
						double valDblA, valDblB;
						if( state == 4 && ::_hatof_base( QStringRef(&aVal), base, valDblA ) == ELH_NO_ERROR && ::_hatof_base( QStringRef(&bVal), base, valDblB ) == ELH_NO_ERROR )
							pRes = constructConstComplexPolar( valDblA, aVal, valDblB, bVal, cnNode->getLine() );
						else
							iRes = CMMLError_CNSyntax;
					}
					break;
				default:
					iRes = CMMLError_CNSyntax_Type;
					break;
				}
			}
		}
	}
	if( pRes )
		pRes->line = cnNode->getLine();
	if( iRes != CMMLError_NONE )
		m_dmm.AddLine( m_dmmPage, getErrorMsg(iRes), QString(m_taskName), cnNode->getLine(), cnNode->getTagName() );
	return pRes;
}

long CMMLCalc::lookupSymbol( QString varName, CParseDsrSymbol*& smb )
{
	long *nameID = NULL, idx = -1;
	if( getCurrentSymbolTable() != m_dsrSymbolTable )
	{
		nameID = getCurrentSymbolTable()->is_find( varName );
		if( nameID )
		{
			idx = *nameID;
			smb = getCurrentSymbolTable()->getSymbol( idx );
			return idx;
		}
	}
	nameID = m_dsrSymbolTable->is_find( varName );
	if( nameID )
	{
		idx = *nameID;
		smb = m_dsrSymbolTable->getSymbol( idx );
	}
	return idx;
}

long CMMLCalc::lookupSymbol( AST_TXML_Tag* xmlNode, CParseDsrSymbol*& dest )
{
	QString opName;
	enum TAG2MML tagID = ::mml_getTag2MML( xmlNode->getTagName() );
	switch( tagID )
	{
	case TAG2MML_ci:
	case TAG2MML_cn:
	case TAG2MML_csymbol:
		opName = xmlNode->getText();
		break;
	default:
		opName = xmlNode->getTagName();
	}
	return lookupSymbol( opName, dest );
}

int CMMLCalc::addDsrSymbol2Table( CParseDsrSymbolTable* dsrSymbolTable, QString name, enum CDsrSymbolTableRecordType type, CDsrDataType_Node *dataType )
{
	return dsrSymbolTable->Add( *(new CParseDsrSymbol( name, type, dataType )) );
}

int CMMLCalc::addDsrSymbol2Table( CParseDsrSymbolTable* dsrSymbolTable, CParseDsrSymbol& smb )
{
	return dsrSymbolTable->Add( smb );
}

CDsrAST_Node* CMMLCalc::constructConstRational( long valLongA, const QString& aVal, long valLongB, const QString& bVal, long line )
{
	CDsrAST_Node *arg1 = new CDsrAST_ConstInt( valLongA, aVal );
	arg1->line = line;
	CDsrAST_Node *arg2 = new CDsrAST_ConstInt( valLongB, bVal );
	arg2->line = line;
	return new CDsrAST_Expr( new CDsrAST_Op( OPERATOR_op_div, line ), arg1, arg2 );	// line will be set in the outer function
}

CDsrAST_Node* CMMLCalc::constructConstComplexCartesian( double valDblA, const QString& aVal, double valDblB, const QString& bVal, long line )
{
	long *nameID = m_dsrSymbolTable->is_find( ::ml_Op2String( FORMULA_imaginaryi ) );
	if( !nameID ) return NULL;
	CDsrAST_Node *sp = new CDsrAST_PrimaryID( *nameID );
	sp->line = line;
	sp = new CDsrAST_Special( (CDsrAST_PrimaryID*) sp, (CDsrAST_Separator*) NULL, (CDsrAST_Separator*) NULL, (CDsrAST_ArgList*) NULL );
	sp->line = line;

	CDsrAST_Node *arg1 = new CDsrAST_ConstReal( valDblA, aVal );
	arg1->line = line;
	CDsrAST_Node *arg2 = new CDsrAST_ConstReal( valDblB, bVal );
	arg2->line = line;

	arg2 = new CDsrAST_Expr( new CDsrAST_Op( OPERATOR_op_mul, line ), arg2, sp );
	arg2->line = line;
	return new CDsrAST_Expr( new CDsrAST_Op( OPERATOR_op_add, line ), arg1, arg2 );	// line will be set in the outer function
}

CDsrAST_Node* CMMLCalc::constructConstComplexPolar( double valDblA, const QString& aVal, double valDblB, const QString& bVal, long line )
{
	if( !m_dsrSymbolTable ) return NULL;

	CDsrAST_ArgList *argsNode = new CDsrAST_ArgList();
	argsNode->line = line;
	CDsrAST_Node *node = new CDsrAST_ConstReal( valDblA, aVal );
	node->line = line;
	argsNode->Add( node );
	node = new CDsrAST_ConstReal( valDblB, bVal );
	node->line = line;
	argsNode->Add( node );
	node = NULL;
	long *nameID = m_dsrSymbolTable->is_find( ::ml_Op2String( FUNCTION_std_polar ) );
	if( nameID )
	{
		CDsrAST_CompoundID *compoundNode = new CDsrAST_CompoundID();
		CDsrAST_PrimaryID *primaryNode = new CDsrAST_PrimaryID( *nameID );
		primaryNode->line = line;
		compoundNode->Add( primaryNode );
		compoundNode->line = line;
		node = new CDsrAST_Function( compoundNode, NULL, NULL, argsNode );
	}
	else
		delete argsNode;
	return node; // line will be set in the outer function
}

///////////////////////////////////////////////////////////////////////////////
