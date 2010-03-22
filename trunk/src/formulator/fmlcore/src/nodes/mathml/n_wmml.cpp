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

#include <QPrinter>

#include "../../dictionary/entity_map.h"
#include "../../dictionary/op_autokeyword.h"
#include "../../dictionary/op_iddefs.h"
#include "n_cm_treetemplate.h"
#include "../node.h"

///////////////////////////////////////////////////////////////////////////////

void mmlTreeIterator__removePresentationMathML( AST_TXML_Node *node, void * /*data*/ )
{
	if( !node || node->getNodeType() != ASTT_TXML_Tag ) return;

	AST_TXML_Compound *_children = ((AST_TXML_Tag*) node)->getTags();
	if( !_children ) return;

	for( long i = 0; i < _children->getChildNumber(); i++ )
	{
		if( _children->getChild( i ) && _children->getChild( i )->getNodeType() == ASTT_TXML_Tag )
		{
			AST_TXML_Tag *child = (AST_TXML_Tag*)(_children->getChild( i ));
			enum TAG2MML mmlTag = ::mml_getTag2MML( child->getTagName() );
			if( ::mml_isContentTag( mmlTag ) || mmlTag == TAG2MML_math ) continue;

			AST_TXML_Compound *childContent = child->getTags();
			if( childContent )
				childContent = (AST_TXML_Compound*) childContent->Clone();
			if( childContent )
			{
				child->getTags()->detachChildren();
				_children->Erase( i );

				long cb = childContent->getChildNumber();
				_children->MoveChildren( i, childContent );
				delete childContent;

				i += cb - 1;
			}
			else
			{
				_children->Erase( i );
				i--;
			}
		}
	}
}

QString CFrameNode::to_mathml_pureContent( QString mathmlText )
{
	long cb = 0;
	return to_mathml_pureContent( mathmlText, 0, cb );
}

QString CFrameNode::to_mathml_pureContent( QString mathmlText, long nLevel, long& nodesCount )
{
	DRMessageManager dmm;
	long dmm_page = 0;
	TXML_Storage* bms = 0;

	if( !LS_CompileXML( _T("(internal postprocessing)"), mathmlText, bms, dmm, dmm_page ) && bms )
	{
		AST_TXML_Tag* root = bms->getFirstTag( FBL_TAG_math );
		if( root )
		{
			root->recursiveProcessing( 0, mmlTreeIterator__removePresentationMathML, 1 /* deep */ );
			root->deleteValueChildren();
			CFrameNode *pFrameNode = new CFrameNode();
			pFrameNode->DeleteAllChild();
			pFrameNode->read_mathml( root );
			mathmlText = pFrameNode->to_mathml( nLevel, nodesCount );
		}
		delete bms;
	}
	return mathmlText;
}

QString CFrameNode::to_mathml_pureContent( )
{
	long cb = 0;
	return to_mathml_pureContent( 0, cb );
}

QString CFrameNode::to_mathml_pureContent( long nLevel, long& nodesCount )
{
	QString mathmlText = to_mathml( nLevel, nodesCount );
	return CFrameNode::to_mathml_pureContent( mathmlText, nLevel, nodesCount );
}

void mmlTreeIterator__addNamespaceM( AST_TXML_Node *node, void *data )
{
	if( node && node->getNodeType() == ASTT_TXML_Tag && data )
	{
		std::vector<long> *pVec = (std::vector<long>*)data;
		pVec->push_back( node->getShift1() );
		pVec->push_back( node->getShift2() );
	}
}

QString CFrameNode::to_mathml()
{
	long cb = 0;
	return to_mathml( 0, cb );
}

QString CFrameNode::to_mathml( long level, long& nodesCount )
{
	nodesCount = 0;

	QString sattr = _T("");
	sattr = MAKE_MATHML_ATTR( FBL_ATTR_columnalign, FBL_VAL_left );
	sattr += MAKE_MATHML_ATTR( FBL_ATTR_linebreak, FBL_VAL_true );

	long dummy = 0;
	if( level == 0 )
	{
		//iterateNodeTree( TRACE_setRecursiveLevel, 0 );

		nodesCount = 1;

		QString tmp_mml;
		switch( ::getCurrentFormulatorStyle_TranslatePref() )
		{
		case TRANSLATE_2_MATHML2_NAMESPACE_ATTR:
			if( 0 /*isPureContentMathMLOutput()*/ )
			{
				return ::mml_root_tag_xmlns( CParentNode::to_mathml( level + 1, nodesCount ) );
			}
			else
			{
				return ::mml_root_tag_xmlns( GetChildCount() > 1 ?
					::mml_table_tag( CParentNode::to_mathml( level + 2, dummy ), sattr, level + 1 ) :
					CParentNode::to_mathml( level + 1, dummy )
				);
			}
		case TRANSLATE_2_MATHML2_NAMESPACE_M:
			{
			if( 0 /*isPureContentMathMLOutput()*/ )
			{
				tmp_mml = ::mml_root_tag( CParentNode::to_mathml( level + 1, nodesCount ) );
			}
			else
			{
				tmp_mml = ::mml_root_tag( GetChildCount() > 1 ?
					::mml_table_tag( CParentNode::to_mathml( level + 2, dummy ), sattr, level + 1 ) :
					CParentNode::to_mathml( level + 1, dummy ) );
			}

			DRMessageManager dmm;
			long dmm_page = 0;
			TXML_Storage* bms = 0;
	        
			if( !LS_CompileXML( _T("(internal postprocessing)"), tmp_mml, bms, dmm, dmm_page ) && bms )
			{
				AST_TXML_Tag* root = bms->getFirstTag( FBL_TAG_math );
				if( root )
				{
					std::vector<long> shift2add_m;
					root->recursiveProcessing( &shift2add_m, mmlTreeIterator__addNamespaceM );
					sort( shift2add_m.begin(), shift2add_m.end() );
					long cumulative_pos = 0;
					for( long i = 0; i < (long) shift2add_m.size(); i++ )
					{
						if( shift2add_m[ i ] >= 0 && 
							cumulative_pos + shift2add_m[ i ] < (long) tmp_mml.length() )
						{
							tmp_mml.insert( cumulative_pos + shift2add_m[ i ], TRANSLATE_2_MATHML2_NAMESPACE_M_PREFIXNAME );
							cumulative_pos += QString(TRANSLATE_2_MATHML2_NAMESPACE_M_PREFIXNAME).length();
						} 
					}
				}
				delete bms;
			}
			
			return tmp_mml;
			}
		default:
			if( 0 /*isPureContentMathMLOutput()*/ )
			{
				return ::mml_root_tag( CParentNode::to_mathml( level + 1, nodesCount ) );
			}
			else
			{
				return ::mml_root_tag( GetChildCount() > 1 ?
					::mml_table_tag( CParentNode::to_mathml( level + 2, dummy ), sattr,level + 1 ) :
					CParentNode::to_mathml( level + 1, dummy ) );
			}
		}
	}

	if( /*!isPureContentMathMLOutput() &&*/
		GetChildCount() > 1 && to_mathml_data.is_content != CONTENT_MATHML_CONVERTION2__FRAME_NOT_MULTILINE )
	{
		nodesCount = 1;
		return ::mml_table_tag( CParentNode::to_mathml( level + 1, dummy ), sattr, level );
	}

	return CParentNode::to_mathml( level, nodesCount );
}

QString CFrameNode::to_mathml_4_html( long level, int is_block )
{
	QString ret = _T("");
	long dummy = 0;

	if( 0 /*isPureContentMathMLOutput()*/ )
	{
		if( level == 0 )
		{
			ret  = ::mml_root_tag_4_html_mathml( is_block, CParentNode::to_mathml( level + 1, dummy ) );
			::mml_validate4html( ret );
		}
		else
			ret = CParentNode::to_mathml( level, dummy );
	}
	else
	{
		if( level == 0 )
		{
			ret  = ::mml_root_tag_4_html_mathml( is_block, GetChildCount() > 1 ?
						::mml_table_tag( CParentNode::to_mathml( level + 2, dummy ), QString( _T("") ), level + 1 ) :
						CParentNode::to_mathml( level + 1, dummy )
			);
			::mml_validate4html( ret );
		}
		else
			ret = (GetChildCount() > 1 ? ::mml_table_tag( CParentNode::to_mathml( level + 1, dummy ), QString( _T("") ), level ) :
										 CParentNode::to_mathml( level, dummy ));
	}

	return ret;
}

void CFrameNode::setMathML2StringVector( std::vector<QString>& dest, long level )
{
	dest.clear();

	long i, j, cb;
	QString arg;

	CNode *pCurNodeL = GetFirstChild();
	for( j = 0; pCurNodeL && j < GetChildCount(); j++ )
	{
		if( pCurNodeL->GetType() != NODE_LINE ) continue;
		CNode *pCurNode = ((CLineNode*)pCurNodeL)->GetFirstChild();
		for( i = 0; pCurNode && i < ((CLineNode*)pCurNodeL)->GetChildCount(); i++ )
		{
			cb = 0;
			arg = pCurNode->to_mathml( level, cb );
			if( cb > 1 )
			{
				arg = pCurNode->to_mathml( level + 1, cb );	// just to get pretty-printing, could be erased to speed up converting to MathML
				arg = mml_row_tag_nl( arg, level );
			}
			dest.push_back( arg );
			pCurNode = pCurNode->GetNext();
		}
		pCurNodeL = pCurNodeL->GetNext();
	}
}

QString CParentNode::to_mathml( long level, long& nodesCount )
{
	nodesCount = 0;

	QString ret = _T("");
	long cb = 0;
	CNode *pCurNode = GetFirstChild();
	for( long j = 0; pCurNode && j < GetChildCount(); j++ )
	{
		QString body = _T(""), name = _T(""), sattr = _T("");
		int isContent = pCurNode->to_mathml_data.is_content || pCurNode->isIgnoreContentConversion() || pCurNode->isAutodetectContent();
		if( GetType() == NODE_LINE && !isContent )
		{
			CFormulaNode *frmNode = getParentContentFormulaNode( 1 /*use 'ignored' nodes also*/ );
			if( frmNode && !::mml_is_cm_token( frmNode->to_mathml_data.name ) && 
				frmNode->getSemanticsOrCTokenNodeWrapped() == 0 && !frmNode->isAutodetectContent() )
			{
				if( pCurNode->GetType() == NODE_PLANETEXT )
				{
					CPlaneText *planeNode = (CPlaneText*)pCurNode;
					body = planeNode->GetNodeText();
					switch( planeNode->getFStyle() )
					{
					case FSTYLE_FUNCTION:
					case FSTYLE_VARIABLE:
					case FSTYLE_VECTOR:
						body = ::mml_tag_no_nl( QString(FBL_TAG_ci), body, sattr, level );
						break;
					case FSTYLE_NUMBER:
						body = ::mml_tag_no_nl( QString(FBL_TAG_cn), body, sattr, level );
						break;
					default:
						{
							std::vector<QString> dest_str;
							std::vector<long> dest_style;
							QString sV(FBL_TAG_mtext);
							::getMathMLChar2Style( dest_str, dest_style, body, sV, __GLOBAL_WORK_MODE );
							int wasConverted = 0;
							if( dest_str.size() == dest_style.size() && dest_str.size() == 1 )
							{
								if( ::detectContentTag( name, body, sattr, dest_str[ 0 ], dest_style[ 0 ] ) != -1 )
								{
									body = body.length() ? 
											::mml_tag_no_nl( name, body, sattr, level ) : 
											::mml_tag_self_nl( name, sattr, level );
									wasConverted = 1;
								}
							}
							if( !wasConverted )
							{
								body = pCurNode->to_mathml( level + 1, cb );
								body = ::mml_tag_nl( QString(FBL_TAG_csymbol), body, QString( _T("") ), level );
							}
						}
					}
				}
				else if( pCurNode->GetType() == NODE_FORMULA )
				{
					body = pCurNode->to_mathml( level + 1, cb );
					if( body.length() )
						body = ::mml_tag_no_nl( QString(FBL_TAG_csymbol), body, sattr, level );
					else
						body = ::mml_tag_self_nl( QString(FBL_TAG_csymbol), sattr, level );
				}
			}
		}

		if( body.length() )
		{
			cb = 1;
			ret += body;
		}
		else
			ret += pCurNode->to_mathml( level, cb );

		if( nodesCount != -1 )
		{
			if( cb != -1 )
				nodesCount += cb;
			else
				nodesCount = -1;
		}

		pCurNode = pCurNode->GetNext();
	}
	return ret;
}

void CParentNode::iterateNodeTree4Referenced( const QString& id, CFormulaNode*& pDest )
{
	CNode *pNode = 0;
	for( long i = 0; pDest == 0 && i < GetChildCount(); i++ )
	{
		if( (pNode = GetChild( i )) == 0 ) continue;

		if( pNode->GetType() == NODE_FORMULA )
		{
			CFormulaNode *pFormulaNode = (CFormulaNode*)pNode;
			if( pFormulaNode->isReferenced() && pFormulaNode->to_mathml_data.mathml_template_string == id )
			{
				pDest = pFormulaNode;
				return;
			}

			int nCall = 1;
			if( pFormulaNode->to_mathml_data.is_content )
			{
				if( pFormulaNode->getAddonNode() && pFormulaNode->getAddonNode()->GetType() == NODE_LINE )
				{
					CLineNode *addonLine = (CLineNode*)(pFormulaNode->getAddonNode());
					CNode *pCurNode = addonLine->GetFirstChild();
					for( long j = 0; pCurNode && j < addonLine->GetChildCount(); j++ )
					{
						if( pCurNode->isContentReference() )
						{
							nCall = 0;
							break;
						}
						pCurNode = pCurNode->GetNext();
					}
				}
			}
			if( nCall )
				pFormulaNode->iterateNodeTree4Referenced( id, pDest );
		}
		else if( pNode->GetType() == NODE_FRAME || pNode->GetType() == NODE_LINE )
			((CParentNode*)pNode)->iterateNodeTree4Referenced( id, pDest );
	}
}

QString CLineNode::to_mathml( long level, long& nodesCount )
{
	nodesCount = 0;
	int isContent = to_mathml_data.is_content || isIgnoreContentConversion() || isAutodetectContent();

	long dummy = 0;
	CNode *parent = GetFrameNode();
	QString ret = _T("");
	if( parent == 0 || parent->GetType() != NODE_FRAME )
		return CParentNode::to_mathml( level, nodesCount );

	if( ((CFrameNode*)parent)->GetChildCount() > 1 &&
		((CFrameNode*)parent)->to_mathml_data.is_content != 
			CONTENT_MATHML_CONVERTION2__FRAME_NOT_MULTILINE )
	{
		nodesCount = 1;

		long cb = 0;
		return ::mml_tablerow_tag( 
					::mml_tablecell_tag(
						GetFirstChild() == 0 ? 
							::mml_row_tag_no_nl( QString( _T("") ), level + 2 ) :
							CParentNode::to_mathml( level + 2, cb ), 
						level + 1),
					level );
	}
	else if( parent->GetParent() == 0 ) // parent frame is root
	{
		if( GetChildCount() == 1 && 
			GetFirstChild()->GetType() == NODE_FORMULA && 
			((CFormulaNode*)GetFirstChild())->getCalcButtonID() == HF_BTNID_EMPTY_FRAME )
			return CParentNode::to_mathml( level, nodesCount );
		else
		{
			int isChildContent = 1;
			CNode *pTmpCurNode = GetFirstChild();
			for( long k = 0; pTmpCurNode && k < GetChildCount() && isChildContent; k++ )
			{
				isChildContent = pTmpCurNode->to_mathml_data.is_content || pTmpCurNode->isIgnoreContentConversion() || pTmpCurNode->isAutodetectContent();
				pTmpCurNode = pTmpCurNode->GetNext();
			}
			if( isChildContent )
				return CParentNode::to_mathml( level, nodesCount );
			else
			{
				nodesCount = 1;
				return ::mml_row_tag_nl( CParentNode::to_mathml( level + 1, dummy ), level );
			}
		}
	}
	else if( parent->GetParent()->GetType() == NODE_FORMULA )
	{
		CFormulaNode *parent_formula = (CFormulaNode*)(parent->GetParent());
		if( parent_formula->getCalcButtonID() == HF_BTNID_EMPTY_FRAME )
		{
			switch( GetChildCount() )
			{
			case 0:
				nodesCount = 0;
				return QString( _T("") );
			default:
				return CParentNode::to_mathml( level, nodesCount );
			}
		}
		else
		{
			switch( GetChildCount() )
			{
			case 0:
				nodesCount = 1;
				return isContent ? QString( _T("") ) : ::mml_row_tag_no_nl( QString( _T("") ), level );
			case 1:
				return CParentNode::to_mathml( level, nodesCount );
			default:
				if( !parent_formula->to_mathml_data.is_content )
				{
					nodesCount = 1;
					return ::mml_row_tag_nl( CParentNode::to_mathml( level + 1, dummy ), level );
				}
				else
				{
					return CParentNode::to_mathml( level, nodesCount );
				}
			}
		}
	}
	Q_ASSERT( 0 );
	return CParentNode::to_mathml( level, nodesCount );
}
