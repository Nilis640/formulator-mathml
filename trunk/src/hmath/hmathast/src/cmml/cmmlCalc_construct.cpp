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
#include "../ast/astdsr.h"
#include "../vm/vmeval.h"
#include "../symbol/symbol_table_dsr.h"

///////////////////////////////////////////////////////////////////////////////

CDsrAST_Node* CMMLCalc::Create_declare( AST_TXML_Tag* declareNode, std::vector<AST_TXML_Tag*>& nodeCollection )
{
	if( !m_dsrSymbolTable ) return NULL;

	CDsrAST_Node *pRes = NULL;
	int iRes = CMMLError_NONE;

	if( !declareNode ) return pRes;
	if( nodeCollection.size() == 0 )
	{
		iRes = CMMLError_ConstructorItemsExpected;
	}
	else
	{
		QString itemName = nodeCollection[ 0 ]->getText();
		long *nameID = getCurrentSymbolTable()->is_find( itemName );
		if( nameID )
			iRes = CMMLError_SymbolAlreadyDefined;
		else
		{
			AST_TXML_Attribute *attrType = declareNode->getAttribute( FBL_ATTR_type );
			enum CDsrDataTypes tp = attrType ? ::mapMMLValueTypeAttr2Dsr( attrType->getStringValue() ) : DSRDATA_TYPE_REAL;
			CDsrDataType_Node *typeNode = new CDsrDataType_Node( tp );
			if( tp == DSRDATA_TYPE_FUNCTION )
			{
				if( nodeCollection.size() == 1 )
				{
					// TO DO:
					//The declaration
					//
					//<declare type="function" nargs="2">
					//  <ci> f </ci>
					//  <apply>
					//    <plus/>
					//    <ci> F </ci><ci> G </ci>
					//  </apply>
					//</declare>
					//
					//declares f to be a two-variable function with the property that f(x, y) = (F+ G)(x, y). 
					addDsrSymbol2Table( getCurrentSymbolTable(), itemName, SMBTABLE_TYPE_FUNCTION, NULL );
				}
				else
				{
					QString tagName = nodeCollection[ 1 ]->getTagName();
					if( !::xml_strcmp( tagName, FBL_TAG_lambda ) )
					{
						std::vector<AST_TXML_Tag*> lambdaCollection;
						nodeCollection[ 1 ]->getAllTags2Vector( lambdaCollection );
						pRes = Create_lambda( nodeCollection[ 1 ], lambdaCollection, itemName );
					}
					else
					{
						CParseDsrSymbol *smb = NULL;
						lookupSymbol( nodeCollection[ 1 ], smb );
						if( smb )
						{
							smb->name = itemName;
							getCurrentSymbolTable()->Add( *smb );
						}
						else
							iRes = CMMLError_SymbolInitError;
					}
				}
			}
			else
			{
				typeNode->setLValue();
				long idx = addDsrSymbol2Table( getCurrentSymbolTable(), itemName, SMBTABLE_TYPE_VARIABLE, typeNode );
				if( nodeCollection.size() > 1 )
				{
					CDsrAST_Node *exprNode = Create( nodeCollection[ 1 ] );
					if( exprNode )
					{
						exprNode->line = nodeCollection[ 1 ]->getLine();

						CDsrAST_CompoundID *compoundNode = new CDsrAST_CompoundID();
						CDsrAST_PrimaryID *primaryNode = new CDsrAST_PrimaryID( idx );
						primaryNode->line = nodeCollection[ 0 ]->getLine();
						compoundNode->Add( primaryNode );

						pRes = new CDsrAST_Expr( new CDsrAST_Op( OPERATOR_op_assign, declareNode->getLine() ), compoundNode, exprNode );
						pRes->line = declareNode->getLine();
					}
					else
						iRes = CMMLError_SymbolInitError;
				}
			}
		}
	}

	if( iRes != CMMLError_NONE )
		m_dmm.AddLine( m_dmmPage, getErrorMsg(iRes), QString(m_taskName), declareNode->getLine(), declareNode->getTagName() );
	return pRes;
}

CDsrAST_Node* CMMLCalc::Create_lambda( AST_TXML_Tag* lambdaNode, std::vector<AST_TXML_Tag*>& nodeCollection, QString& newFunctionName )
{
	if( !m_dsrSymbolTable ) return NULL;

	CDsrAST_Node *pRes = NULL;
	int iRes = CMMLError_NONE;

	if( !lambdaNode ) return pRes;
	unsigned long i = 0;
	if( nodeCollection.size() == 0 )
	{
		iRes = CMMLError_ConstructorItemsExpected;
	}
	else
	{
		// current scope is LOCAL
		setLocalSymbolTable();

		if( newFunctionName.length() == 0 )
			newFunctionName = ::generateInternalVarName();

		CDsrAST_ArgList *argsNode = NULL;
		if( !::xml_strcmp( nodeCollection[ 0 ]->getTagName(), FBL_TAG_bvar ) )
		{
			argsNode = new CDsrAST_ArgList();
			argsNode->line = nodeCollection[ 0 ]->getLine();
			
			for( i = 0; i < nodeCollection.size() && !::xml_strcmp( nodeCollection[ i ]->getTagName(), FBL_TAG_bvar ); i++ )
			{
				CDsrAST_Node *node = Create( nodeCollection[ i ] );
				if( node )
					argsNode->Add( node );
			}
		}

		long *nameID = m_dsrSymbolTable->is_find( CMMLCALC_KW_FUNCTION ), idx = -1;
		if( nameID == NULL )
			m_dmm.AddLine( m_dmmPage, getErrorMsg(CMMLError_Internal_NoRecord), QString(m_taskName), lambdaNode->getLine(), CMMLCALC_KW_FUNCTION );
		else
			idx = *nameID;
		CDsrAST_Keyword *keywordNode = new CDsrAST_Keyword( idx, DSRDATA_TYPE_FUNCTION );
		keywordNode->line = lambdaNode->getLine();

		CDsrAST_PrimaryID *primaryNode = new CDsrAST_PrimaryID( addDsrSymbol2Table( getCurrentSymbolTable(), newFunctionName, SMBTABLE_TYPE_FUNCTION, NULL ) );
		primaryNode->line = lambdaNode->getLine();

		CDsrAST_ObjArg *objArgNode = new CDsrAST_ObjArg( (CDsrAST_Separator*) NULL, (CDsrAST_Separator*) NULL, argsNode, (CDsrAST_TypeSpec*) NULL );
		CDsrAST_ObjHead *headNode = new CDsrAST_ObjHead( keywordNode, primaryNode, objArgNode );
		headNode->line = lambdaNode->getLine();

		// TO DO: implement "domainofapplication":

		CDsrAST_Node *bodyNode = NULL;
		if( i == nodeCollection.size() - 1 )
		{
			bodyNode = Create( nodeCollection[ i ] );
		}
		else if( i < nodeCollection.size() - 1 )
		{
			CDsrAST_StmtList *stmtNode = new CDsrAST_StmtList();
			for( unsigned long j = i; j < nodeCollection.size(); j++ )
			{
				bodyNode = Create( nodeCollection[ j ] );
				if( bodyNode )
					stmtNode->Add( bodyNode );
			}
			bodyNode = new CDsrAST_Compound( stmtNode, NULL, NULL );
			bodyNode->line = nodeCollection[ i ]->getLine();
		}
		else
			iRes = CMMLError_FunctionBodyExpected;

		if( bodyNode->getType() == DsrASTT_Compound )
			pRes = new CDsrAST_ObjDef_Compound( headNode, (CDsrAST_Compound*) bodyNode, getCurrentSymbolTable() );
		else
			pRes = new CDsrAST_ObjDef_Assign( headNode, new CDsrAST_Op( OPERATOR_op_assign, lambdaNode->getLine() ), bodyNode, (CDsrAST_Separator*) NULL, getCurrentSymbolTable() );
		pRes->line = lambdaNode->getLine();

		// current scope is GLOBAL
		setGlobalSymbolTable();
	}

	if( iRes != CMMLError_NONE )
		m_dmm.AddLine( m_dmmPage, getErrorMsg(iRes), QString(m_taskName), lambdaNode->getLine(), lambdaNode->getTagName() );
	return pRes;
}

///////////////////////////////////////////////////////////////////////////////

CDsrAST_Node* CMMLCalc::Create_set( AST_TXML_Tag* setNode, std::vector<AST_TXML_Tag*>& nodeCollection )
{
	if( !m_dsrSymbolTable ) return NULL;

	CDsrAST_Node *pRes = NULL;
	int iRes = CMMLError_NONE;

	if( !setNode ) return pRes;
	if( nodeCollection.size() == 0 )
	{
		pRes = Create_constant( ::ml_Op2String( FORMULA_set_empty ), setNode->getLine() );
	}
	else
	{
		CDsrAST_Node *node;
		CDsrAST_ArgList *argsNode = new CDsrAST_ArgList();
		argsNode->line = nodeCollection[ 0 ]->getLine();
		for( unsigned long i = 0; i < nodeCollection.size(); i++ )
		{
			if( (node = Create( nodeCollection[ i ] )) != NULL )
				argsNode->Add( node );
		}
		pRes = new CDsrAST_Expr( new CDsrAST_Op( OPERATOR_curly_brackets, setNode->getLine() ), argsNode, NULL );
		pRes->line = setNode->getLine();
	}

	if( iRes != CMMLError_NONE )
		m_dmm.AddLine( m_dmmPage, getErrorMsg(iRes), QString(m_taskName), setNode->getLine(), setNode->getTagName() );
	return pRes;
}

///////////////////////////////////////////////////////////////////////////////

CDsrAST_Node* CMMLCalc::constructVector( unsigned long rows, unsigned long columns, long line, QString tagName, std::vector<AST_TXML_Tag*>& nodeCollection )
{
	CDsrAST_Node *pRes = NULL;
	int iRes = CMMLError_NONE;

	if( nodeCollection.size() == 0 )
	{
		iRes = CMMLError_ConstructorItemsExpected;
	}
	else
	{
		long *nameID = m_dsrSymbolTable->is_find( ::ml_Op2String( FORMULA_table ) );
		if( nameID )
		{
			CDsrAST_Node *node;
			CDsrAST_ArgList *argsNode = new CDsrAST_ArgList();
			argsNode->line = nodeCollection[ 0 ]->getLine();
			// rows number
			node = new CDsrAST_ConstInt( rows );
			node->line = line;
			argsNode->Add( node );
			// columns number
			node = new CDsrAST_ConstInt( columns );
			node->line = line;
			argsNode->Add( node );
			// actual vector items
			for( unsigned long i = 0; i < nodeCollection.size(); i++ )
			{
				if( (node = Create( nodeCollection[ i ] )) != NULL )
					argsNode->Add( node );
			}
			CDsrAST_PrimaryID *primaryNode = new CDsrAST_PrimaryID( *nameID );
			primaryNode->line = line;
			pRes = new CDsrAST_Special( primaryNode, NULL, NULL, argsNode );
			pRes->line = line;
		}
		else
		{
			iRes = CMMLError_Internal_NoRecord;
		}
	}

	if( iRes != CMMLError_NONE )
		m_dmm.AddLine( m_dmmPage, getErrorMsg(iRes), QString(m_taskName), line, tagName );
	return pRes;
}

CDsrAST_Node* CMMLCalc::Create_vector( AST_TXML_Tag* vecNode, std::vector<AST_TXML_Tag*>& nodeCollection )
{
	if( !m_dsrSymbolTable || !vecNode ) return NULL;
	return constructVector( (unsigned long) nodeCollection.size(), 1, vecNode->getLine(), vecNode->getTagName(), nodeCollection );
}

CDsrAST_Node* CMMLCalc::Create_matrix( AST_TXML_Tag* matrixNode, std::vector<AST_TXML_Tag*>& nodeCollection )
{
	if( !m_dsrSymbolTable || !matrixNode ) return NULL;

	CDsrAST_Node *pRes = NULL;
	int iRes = CMMLError_NONE;

	if( nodeCollection.size() == 0 )
	{
		iRes = CMMLError_ConstructorItemsExpected;
	}
	else
	{
		std::vector<AST_TXML_Tag*> itemsCollection;
		unsigned long columns = 0, k, cb;
		long j;
		for( k = 0; k < nodeCollection.size(); k++ )
		{
			if( ::xml_strcmp( nodeCollection[ k ]->getTagName(), FBL_TAG_matrixrow ) || nodeCollection[ k ]->getTags() == NULL )
			{
				itemsCollection.clear();
				break;
			}
			else
			{
				cb = 0;
				for( j = 0; j < nodeCollection[ k ]->getTags()->getChildNumber(); j++ )
				{
					AST_TXML_Node *_xml_child = nodeCollection[ k ]->getTags()->getChild( j );
					if( _xml_child && _xml_child->getNodeType() == ASTT_TXML_Tag )
					{
						itemsCollection.push_back( (AST_TXML_Tag*) _xml_child );
						cb++;
					}
				}
				if( k == 0 )
					columns = cb;
				else if( columns != cb )
				{
					itemsCollection.clear();
					break;
				}
			}
		}

		if( itemsCollection.size() == 0 || nodeCollection.size() * columns != itemsCollection.size() )
			iRes = CMMLError_MatrixSyntax;
		else
			pRes = constructVector( (unsigned long) nodeCollection.size(), columns, matrixNode->getLine(), matrixNode->getTagName(), itemsCollection );
	}

	if( iRes != CMMLError_NONE )
		m_dmm.AddLine( m_dmmPage, getErrorMsg(iRes), QString(m_taskName), matrixNode->getLine(), matrixNode->getTagName() );
	return pRes;
}

CDsrAST_Node* CMMLCalc::Create_condition( AST_TXML_Tag* xmlNode )
{
	CDsrAST_Node *pRes = NULL;
	AST_TXML_Node *xmlChild = NULL;
	if( !xmlNode->getTags() || xmlNode->getTags()->getChildNumber() != 1 || (xmlChild = xmlNode->getTags()->getChild( 0 )) == NULL || xmlChild->getNodeType() != ASTT_TXML_Tag )
		m_dmm.AddLine( m_dmmPage, getErrorMsg(CMMLError_ArgNumberMismatch_No), QString(m_taskName), xmlNode->getLine(), xmlNode->getTagName(), 1 );
	else
		 pRes = Create( (AST_TXML_Tag*) xmlChild );
	return pRes;
}

///////////////////////////////////////////////////////////////////////////////

CDsrAST_Node* CMMLCalc::Create_piecewise( AST_TXML_Tag* ifNode, std::vector<AST_TXML_Tag*>& nodeCollection )
{
	if( !m_dsrSymbolTable ) return NULL;

	CDsrAST_Node *pRes = NULL;
	int iRes = CMMLError_NONE;

	if( !ifNode ) return pRes;

	if( nodeCollection.size() == 0 )
	{
		iRes = CMMLError_ConstructorItemsExpected;
	}
	else
	{
		CDsrAST_Node *ifAst = NULL/*, *thenAst = NULL, *elseAst = NULL*/;
		AST_TXML_Tag* otherwiseNode = ifNode->getTagChildByName( FBL_TAG_otherwise );
		for( unsigned long i = 0; i < nodeCollection.size(); i++ )
		{
			if( ::xml_strcmp( nodeCollection[ i ]->getTagName(), FBL_TAG_otherwise ) == 0 )
				continue;
			else if( ::xml_strcmp( nodeCollection[ i ]->getTagName(), FBL_TAG_piece ) == 0 )
			{
				std::vector<AST_TXML_Tag*> pieceTags;
				nodeCollection[ i ]->getAllTags2Vector( pieceTags );
				if( pieceTags.size() != 2 )
				{
					iRes = CMMLError_PiecewiseSyntax_Piece2Arg;
					break;
				}
				else
				{
					CDsrAST_Expr *arg2 = NULL;
					CDsrAST_Node *firstNode = Create( pieceTags[ 0 ] );
					CDsrAST_Node *secondNode = Create( pieceTags[ 1 ] );
					enum DsrAST_Types secondNodeType = secondNode ? secondNode->getType() : DsrASTT_None;
					switch( secondNodeType )
					{
					case DsrASTT_Expr:
						arg2 = (CDsrAST_Expr*) secondNode;
						break;
					case DsrASTT_Function:
					case DsrASTT_CompoundID:
					case DsrASTT_PrimaryID:
					case DsrASTT_Special:
						{
							long line = pieceTags[ 1 ]->getLine();
							CDsrAST_Node *trueNode = Create_constant( ::ml_Op2String( CONST_true ), line );
							if( trueNode )
							{
								arg2 = new CDsrAST_Expr( new CDsrAST_Op( OPERATOR_op_compare_e, line ), secondNode, trueNode );
								arg2->line = line;
							}
						}
						break;
					default:
						break;
					}

					if( !arg2 )
					{
						if( firstNode )
						{
							delete firstNode;
							firstNode = NULL;
						}
						if( secondNode )
						{
							delete secondNode;
							secondNode = NULL;
						}
						iRes = CMMLError_PiecewiseSyntax_Piece2Apply;
						break;
					}
					else
					{
						CDsrAST_If *node = new CDsrAST_If( arg2, firstNode, NULL, NULL, NULL, NULL );
						if( ifAst )
							((CDsrAST_If*) ifAst)->setElseNode( node );
						else
							pRes = node;
						ifAst = node;
					}
				}
				
			}
			else
			{
				iRes = CMMLError_PiecewiseSyntax;
				break;
			}
		}
		if( otherwiseNode && ifAst )
		{
			CDsrAST_Node *node = NULL;
			std::vector<AST_TXML_Tag*> otherwiseTags;
			otherwiseNode->getAllTags2Vector( otherwiseTags );
			if( otherwiseTags.size() == 1 )
			{
				node = Create( otherwiseTags[ 0 ] );
				if( node ) ((CDsrAST_If*) ifAst)->setElseNode( node );
			}
			else if( otherwiseTags.size() > 1 )
			{
				CDsrAST_StmtList *stmtNode = new CDsrAST_StmtList();
				for( unsigned long j = 0; j < otherwiseTags.size(); j++ )
				{
					node = Create( otherwiseTags[ j ] );
					if( node ) stmtNode->Add( node );
				}
				node = new CDsrAST_Compound( stmtNode, NULL, NULL );
				node->line = otherwiseNode->getLine();
				((CDsrAST_If*) ifAst)->setElseNode( node );
			}
		}
	}

	if( iRes != CMMLError_NONE )
	{
		if( pRes )
		{
			delete pRes;
			pRes = NULL;
		}
		m_dmm.AddLine( m_dmmPage, getErrorMsg(iRes), QString(m_taskName), ifNode->getLine(), ifNode->getTagName() );
	}
	return pRes;
}

///////////////////////////////////////////////////////////////////////////////
