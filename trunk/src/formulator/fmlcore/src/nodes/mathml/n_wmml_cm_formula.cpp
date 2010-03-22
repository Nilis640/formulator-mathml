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

#include "HUtils/idfas.h"
#include "../../dictionary/op_iddefs.h"
#include "n_cm_treetemplate.h"
#include "../node.h"

///////////////////////////////////////////////////////////////////////////////

#define to_mathml__content__NEWLINE 	1
#define to_mathml__content__NO_NEWLINE	0
QString CFormulaNode::to_mathml__content( int& retry, long level, long& nodesCount )
{
	nodesCount = 0;

	long is_ok = 0, dummy = 0, is_newline = 0;
	QString body = _T(""), result = _T(""), sattr = _T(""), name = to_mathml_data.name;
	retry = 0;	// must NOT be re-read as a presentation MathML

	if( isIgnoreContentConversion() ) return result;

	printUnfilteredAttr( sattr );
	switch( to_mathml_data.is_content )
	{
		case CONTENT_MATHML_CONVERTION2__UNKNOWN_XML:
		case CONTENT_MATHML_CONVERTION2__TEMPLATE:
			nodesCount = 1;
			body = to_mathml__template( level + 1, dummy );
			result = name.length() ? 
				::mml_tag_nl( name, body, sattr, level ) : body;
			break;

		case CONTENT_MATHML_CONVERTION2__ONLY_TEXT_BODY:
			body = to_mathml__content_harvest_plaintext( 0, is_ok, level, is_newline );
			if( is_ok )
			{
				nodesCount = 1;
				result = name.length() ?
					::mml_tag_no_nl( name, body, sattr, level ) : body;
			}
			else
			{
				result = to_mathml__content_recursive_tree( 
					name, sattr, level, is_newline, nodesCount );
			}
			break;

		case CONTENT_MATHML_CONVERTION2__APPLY_OPERATOR_RECURSIVE_TREE:
			if( name != FBL_TAG_csymbol )
			{
				result = to_mathml__content_recursive_tree( 
					name, sattr, level, to_mathml__content__NEWLINE, nodesCount );
			}
			else
			{
				body = to_mathml__content_harvest_plaintext( 0, is_ok, level, is_newline );
				if( is_ok )
				{
					nodesCount = 1;
					result = name.length() ?
						::mml_tag_no_nl( name, body, sattr, level ) : body;
				}
				else
				{
					result = to_mathml__content_recursive_tree( 
						name, sattr, level, is_newline, nodesCount );
				}
			}
			break;

		case CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE:
		case CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE_PART1:
		case CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE_PART2:
			result = to_mathml__content_recursive_tree( 
				name, sattr, level, to_mathml__content__NEWLINE, nodesCount );
			break;

		case CONTENT_MATHML_CONVERTION2__BACKRECURSIVE_TREE:
			result = to_mathml__content_backrecursive_tree( 
				name, sattr, level, to_mathml__content__NEWLINE, nodesCount );
			break;

		case CONTENT_MATHML_CONVERTION2__PARSE_NAME_AND_RECURSIVE_CM_TREE:
			to_mathml__content_xmlapply( level );
			if( GetFirstChild() )
				result = GetFirstChild()->to_mathml( level, nodesCount );
			else
			{
				nodesCount = 0;
				result = _T("");
			}
			break;

		case CONTENT_MATHML_CONVERTION2__SELF_CLOSING:
			nodesCount = 1;
			result = name.length() ? ::mml_tag_self_nl( name, sattr, level ) : _T("");
			if( getAddonNode() && getAddonNode()->GetType() == NODE_LINE )
			{
				long cb = 0;
				result += ((CLineNode*)getAddonNode())->CParentNode::to_mathml( level, cb );
				if( cb != -1 )
					nodesCount += cb;
				else
					nodesCount = -1;
			}
			break;

		case CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE_MATRIX:
			if( name.length() )
			{
				nodesCount = 1;
				body = to_mathml__content_matrix_body( level + 1 );
				result = ::mml_tag_nl( name, body, sattr, level );
			}
			else
				result = CParentNode::to_mathml( level, nodesCount );
			break;

		case CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE_SET:
			result = to_mathml__content_set_rule( name, sattr, level, nodesCount );
			break;

		case CONTENT_MATHML_CONVERTION2__RECURSIVE_APPLY:
			result = to_mathml__content_apply_rule( name, sattr, level, nodesCount );
			break;

		case CONTENT_MATHML_CONVERTION2__RECURSIVE_APPLY_ADDON_OP:
			result = to_mathml__content_apply_addon_rule( name, sattr, level, nodesCount );
			break;

		case CONTENT_MATHML_CONVERTION2__RECURSIVE_APPLY_ADDON_OP__BACKRECURSIVE_TREE:
			result = to_mathml__content_apply_addon_rule_backrecursive( name, sattr, level, nodesCount );
			break;

		case CONTENT_MATHML_CONVERTION2__RECURSIVE_APPLY_MOMENTABOUT_ADDON_OP:
			result = to_mathml__content_apply_momentabout_addon_rule( name, sattr, level, nodesCount );
			break;

		case CONTENT_MATHML_CONVERTION2__SEMANTICS_LITERALLY:
			result = to_mathml__content_semantics_literally( name, sattr, level, nodesCount );
			break;

		case CONTENT_MATHML_CONVERTION2__SEMANTICS_PRESENTATION_SUBSTITUTE:
			result = to_mathml__content_semantics_pm( name, sattr, level, nodesCount );
			break;
	
		default:
			break;
	}

	if( to_mathml_data.right == ::getCM_sep() )
	{
		if( nodesCount != -1 ) nodesCount++;
		if( result.length() && result[ result.length() - 1 ] == _T('\n') )
			result.insert( result.length() - 1, ::getCM_sep() );
		else
			result += ::getCM_sep();
	}
	else if( to_mathml_data.left == ::getCM_sep() )
	{
		if( nodesCount != -1 ) nodesCount++;
		result.insert( 0, ::getCM_sep() );
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////

QString CFormulaNode::to_mathml__content_semantics_literally( 
	QString name, QString sattr, long level, long& nodesCount )
{
	nodesCount = 0;

	long dummy = 0;
	if( name.length() )
	{
		QString body = CParentNode::to_mathml( level + 1, dummy );
		QString addon = getAddonNode() && getAddonNode()->GetType() == NODE_LINE ? 
				((CLineNode*)getAddonNode())->CParentNode::to_mathml( level + 1, dummy ) : _T("");
		nodesCount = 1;
		return ::mml_tag_nl( name, body + addon, sattr, level );
	}
	return CParentNode::to_mathml( level, nodesCount );
}

QString CFormulaNode::to_mathml__content_semantics_pm( 
	QString name, QString sattr, long level, long& nodesCount )
{
	nodesCount = 0;

	long dummy = 0;
	CLineNode *_addon_line = 0;
	CNode *_content_node = 0;

	if( getAddonNode() == 0 || 
		getAddonNode()->GetType() != NODE_LINE ||
		(_addon_line = (CLineNode*)getAddonNode())->GetChildCount() == 0 ||
		_addon_line->GetFirstChild() == 0 )
		return to_mathml__content_semantics_literally( name, sattr, level, nodesCount );

	_content_node = _addon_line->GetFirstChild();
	if( name.length() )
	{
		QString body = _content_node->to_mathml( level + 1, dummy );
		QString pm = CParentNode::to_mathml( level + 1, dummy );
		QString addon = _T("");

		_content_node = _content_node->GetNext();
		while( _content_node )
		{
			addon += _content_node->to_mathml( level + 1, dummy );
			_content_node = _content_node->GetNext();
		}

		nodesCount = 1;
		return ::mml_tag_nl( name, body + pm + addon, sattr, level );
	}
	return CParentNode::to_mathml( level, nodesCount );
}

///////////////////////////////////////////////////////////////////////////////

QString CFormulaNode::to_mathml__content_recursive_tree( 
	QString name, QString sattr, long level, long is_newline, long& nodesCount )
{
	nodesCount = 0;

	long dummy = 0;
	if( name.length() )
	{
		nodesCount = 1;
		QString body = CParentNode::to_mathml( level + 1, dummy );

		if( to_mathml_data.is_content == CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE_PART1 )
		{
			return is_newline ? ::mml_tag_nl_part1( name, body, sattr, level ) : 
								::mml_tag_no_nl_part1( name, body, sattr, level );
		}
		else if( to_mathml_data.is_content == CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE_PART2 )
		{
			return is_newline ? ::mml_tag_nl_part2( name, body, sattr, level ) : 
								::mml_tag_no_nl_part2( name, body, sattr, level );
		}
		else
		{
			return is_newline ? ::mml_tag_nl( name, body, sattr, level ) : 
								::mml_tag_no_nl( name, body, sattr, level );
		}
	}
	return CParentNode::to_mathml( level, nodesCount );
}

QString CFormulaNode::to_mathml__content_backrecursive_tree( 
	QString name, QString sattr, long level, long is_newline, long& nodesCount )
{
	nodesCount = 0;
	QString body = _T("");

	long dummy = 0;
	if( name.length() )
	{
		nodesCount = 1;
		CNode *pCurNode = GetLastChild();
		for( long j = 0; pCurNode && j < GetChildCount(); j++ )
		{
			body += pCurNode->to_mathml( level + 1, dummy );
			pCurNode = pCurNode->GetPrev();
		}
		return is_newline ? ::mml_tag_nl( name, body, sattr, level ) : 
							::mml_tag_no_nl( name, body, sattr, level );
	}
	return CParentNode::to_mathml( level, nodesCount );
}

///////////////////////////////////////////////////////////////////////////////

void _CM_apply_readApplyOperator( CParentNode *node, QString& out, long level, QString *opTagSeparated )
{
	long dummy = 0;
	CNode *pCurNode = node->GetFirstChild();
	while( pCurNode )
	{
		if( pCurNode->to_mathml_data.is_content ==
			CONTENT_MATHML_CONVERTION2__APPLY_OPERATOR_RECURSIVE_TREE )
		{
			if( opTagSeparated && pCurNode->to_mathml_data.name.length() )
			{
				*opTagSeparated = pCurNode->to_mathml_data.name;
			}
			out += pCurNode->to_mathml( level, dummy );
			pCurNode->to_mathml_data.is_content =
				CONTENT_MATHML_CONVERTION2__HIDE_APPLY_OPERATOR_RECURSIVE_TREE;
		}
		else
		{
			switch( pCurNode->GetType() )
			{
				case NODE_LINE:
					::_CM_apply_readApplyOperator( (CParentNode*)pCurNode, out, level, opTagSeparated );
					break;
				case NODE_FRAME:
					::_CM_apply_readApplyOperator( (CParentNode*)pCurNode, out, level, opTagSeparated );
					break;
				case NODE_FORMULA:
					if( pCurNode->to_mathml_data.is_content ==
							CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE )
					::_CM_apply_readApplyOperator( (CParentNode*)pCurNode, out, level, opTagSeparated );
					break;
				default:
					break;
			}
	    }
		pCurNode = pCurNode->GetNext();
	}
}

void _CM_apply_returnBackApplyOperator( CParentNode *node )
{
	CNode *pCurNode = node->GetFirstChild();
	while( pCurNode )
	{
		if( pCurNode->to_mathml_data.is_content ==
			CONTENT_MATHML_CONVERTION2__HIDE_APPLY_OPERATOR_RECURSIVE_TREE )
		{
			pCurNode->to_mathml_data.is_content =
				CONTENT_MATHML_CONVERTION2__APPLY_OPERATOR_RECURSIVE_TREE;
		}
		else
		{
			switch( pCurNode->GetType() )
			{
				case NODE_LINE:
					::_CM_apply_returnBackApplyOperator( (CParentNode*)pCurNode );
					break;
				case NODE_FRAME:
					::_CM_apply_returnBackApplyOperator( (CParentNode*)pCurNode );
					break;
				case NODE_FORMULA:
					if( pCurNode->to_mathml_data.is_content ==
							CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE )
					::_CM_apply_returnBackApplyOperator( (CParentNode*)pCurNode );
					break;
				default:
					break;
			}
	    }
		pCurNode = pCurNode->GetNext();
	}
}

QString CFormulaNode::to_mathml__content_apply_rule( 
	QString name, QString sattr, long level, long& nodesCount )
{
	nodesCount = 1;

	long dummy = 0;
	QString op = _T(""), arg = _T("");
	::_CM_apply_readApplyOperator( this, op, level + 1 );
	arg += CParentNode::to_mathml( level + 1, dummy );
	::_CM_apply_returnBackApplyOperator( this );
	return ::mml_tag_nl( name, op + arg, sattr, level );
}

QString CFormulaNode::to_mathml__content_apply_momentabout_addon_rule( 
	QString name, QString sattr, long level, long& nodesCount )
{
	nodesCount = 1;

	long dummy = 0;
	CLineNode *_l = 0;
	CNode *lch = 0;
	int is_moment_about = 0;
	QString head = _T(""), about = _T(""), arg = _T("");
	if( getAddonNode() && getAddonNode()->GetType() == NODE_LINE )
	{
		_l = (CLineNode*)getAddonNode();
		if( _l->GetChildCount() > 1 
			&& (lch = _l->GetLastChild()) != 0 
			&& lch->GetType() == NODE_PLANETEXT )
		{
			CNode *pCurNode = _l->GetFirstChild();
			for( long j = 0; pCurNode && j < _l->GetChildCount(); j++ )
			{
				if( pCurNode->GetNext() )
					head += pCurNode->to_mathml( level, dummy );
				else
					about += pCurNode->to_mathml( level, dummy );
				pCurNode = pCurNode->GetNext();
			}
			is_moment_about = 1;
		}
	}
	if( getAddonNode() && !is_moment_about )
		head = getAddonNode()->to_mathml( level + 1, dummy );

	CNode *pCurNode = GetFirstChild();
	CFormulaNode *_msup = getFormulaDeepFormulaChild( 0, 0/*the first child*/ );
	if( _msup && _msup->getCalcButtonID() == HF_BTNID_SUP &&
		_msup->to_mathml_data.name == _T("") &&
		_msup->to_mathml_data.is_content == CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE )
	{
		CNode *_n = _msup->GetLastChild();
		for( long i = 0; _n && i < _msup->GetChildCount(); i++ )
		{
			arg += _n->to_mathml( level + 1, dummy );
			if( i == 0 ) arg += about;
			_n = _n->GetPrev();
		}
		_msup->setIgnoreContentConversion();
		arg += pCurNode->to_mathml( level + 1, dummy );
		_msup->clearIgnoreContentConversion();
	}
	else
	{
		arg += pCurNode->to_mathml( level + 1, dummy );
		arg += about;
	}

	pCurNode = pCurNode->GetNext();
	while( pCurNode )
	{
		arg += pCurNode->to_mathml( level + 1, dummy );
		pCurNode = pCurNode->GetNext();
	}

	return ::mml_tag_nl( name, head + arg, sattr, level );
}

QString CFormulaNode::to_mathml__content_apply_addon_rule_backrecursive( 
	QString name, QString sattr, long level, long& nodesCount )
{
	nodesCount = 0;

	long cb = 0;
	QString ret = _T("");
	if( name.length() == 0 )
	{
		QString op = _T("");
		if( getAddonNode() )
		{
			op = getAddonNode()->to_mathml( level, cb );
			if( cb == -1 )
				nodesCount = -1;
			else
				nodesCount += cb;
		}

		QString arg = _T("");
		CNode *pCurNode = GetLastChild();
		for( long j = 0; pCurNode && j < GetChildCount(); j++ )
		{
			arg += pCurNode->to_mathml( level, cb );
			pCurNode = pCurNode->GetPrev();
			if( nodesCount != -1 )
			{
				if( cb == -1 )
					nodesCount = -1;
				else
					nodesCount += cb;
			}
		}
		ret = op + arg;
	}
	else
	{
		QString op = getAddonNode() ? getAddonNode()->to_mathml( level + 1, cb ) : _T("");
		nodesCount = 1;
		QString arg = _T("");
		CNode *pCurNode = GetLastChild();
		for( long j = 0; pCurNode && j < GetChildCount(); j++ )
		{
			arg += pCurNode->to_mathml( level + 1, cb );
			pCurNode = pCurNode->GetPrev();
		}
		ret = ::mml_tag_nl( name, op + arg, sattr, level );
	}
	return ret;
}

QString CFormulaNode::to_mathml__content_apply_addon_rule( 
	QString name, QString sattr, long level, long& nodesCount )
{
	nodesCount = 0;

	long cb = 0;
	QString ret = _T("");
	if( name.length() == 0 )
	{
		QString op = _T("");
		if( getAddonNode() )
		{
			op = getAddonNode()->to_mathml( level, cb );
			if( cb == -1 )
				nodesCount = -1;
			else
				nodesCount += cb;
		}

		QString arg = CParentNode::to_mathml( level, cb );
		if( nodesCount != -1 )
		{
			if( cb == -1 )
				nodesCount = -1;
			else
				nodesCount += cb;
		}
		ret = op + arg;
	}
	else
	{
		QString op = _T("");

		if( getAddonNode() && getAddonNode()->GetType() == NODE_LINE )
		{
			CLineNode *addonLine = (CLineNode*)getAddonNode();
			cb = 0;
			CNode *pCurNode = addonLine->GetFirstChild();
			for( long j = 0; pCurNode && j < addonLine->GetChildCount(); j++ )
			{
				if( pCurNode->isContentReference() )
				{
					if( pCurNode->GetType() == NODE_FORMULA &&
						pCurNode->to_mathml_data.is_content == CONTENT_MATHML_CONVERTION2__INTERVAL_REFERENCE )
						op += to_mathml__reference_interval( level + 1, cb, (CFormulaNode*)pCurNode );
					else
						op += to_mathml__reference( level + 1, cb, pCurNode->to_mathml_data.mathml_template_string );
				}
				else
					op += pCurNode->to_mathml( level + 1, cb );

				pCurNode = pCurNode->GetNext();
			}
		}

		QString arg = CParentNode::to_mathml( level + 1, cb );
		nodesCount = 1;
		ret = ::mml_tag_nl( name, op + arg, sattr, level );
	}
	return ret;
}

QString CFormulaNode::to_mathml__reference( long level, long& nodesCount, const QString& id )
{
	CFormulaNode* pDest = 0;
	iterateNodeTree4Referenced( id, pDest );
	if( !pDest ) return _T("");
	int wasIgnore = pDest->isIgnoreContentConversion();

	if( wasIgnore ) pDest->clearIgnoreContentConversion();

	QString ret = pDest->to_mathml( level, nodesCount );

	if( wasIgnore ) pDest->setIgnoreContentConversion();
	return ret;
}

QString CFormulaNode::to_mathml__reference_interval( long level, long& nodesCount, CFormulaNode *intervalNode )
{
	QString ret = _T(""), a = _T(""), b = _T("");
	a = to_mathml__reference( level + 1, nodesCount, ::getContentMathMLReferenceID_interval( 0 ) );
	if( a.length() )
	{
		b = to_mathml__reference( level + 1, nodesCount, ::getContentMathMLReferenceID_interval( 1 ) );
		if( b.length() )
		{
			QString sattr = _T(""), name = intervalNode->to_mathml_data.name;
			intervalNode->printUnfilteredAttr( sattr );
			ret = ::mml_tag_nl( name, a + b, sattr, level );
		}
	}
	return ret;
}

QString CFormulaNode::to_mathml__content_set_rule( 
	QString name, QString sattr, long level, long& nodesCount )
{
	nodesCount = 0;

	long dummy = 0;
	do
	{
		if( GetChildCount() != 1 ) break;
		CNode *f = GetChild( 0 );
		if( !f || f->GetType() != NODE_FRAME) break;
		if( !((CFrameNode*)f)->GetFirstChild() ||
			((CFrameNode*)f)->GetFirstChild()->GetType() != NODE_LINE)
			break;
		CLineNode *l = (CLineNode*)(((CFrameNode*)f)->GetFirstChild());
		if( l->GetChildCount() == 0 ) break;

		CNode *pCurNode = l->GetFirstChild(), *pCondNode = 0, *pBvarWrapNode = 0;
		long j, cb = 0, bvar_cb = 0, meaning_cb = 0, bvarwrap_cb = 0;
		for( j = 0; pCurNode && j < l->GetChildCount(); j++ )
		{
			if( pCurNode->to_mathml_data.name == FBL_TAG_condition )
			{
				pCondNode = pCurNode;
				cb++;
			}
			else if( pCurNode->to_mathml_data.name == FBL_TAG_bvar )
			{
				bvar_cb++;
			}
			else if( pCurNode->to_mathml_data.mathml_template_string == ::getContentMathMLReferenceID_separatorbvar() )
			{
				pBvarWrapNode = pCurNode;
				bvarwrap_cb++;
			}
			else if( !pCurNode->isIgnoreContentConversion() )
				meaning_cb++;
			pCurNode = pCurNode->GetNext();
		}

		if( !pCondNode || cb != 1 ) break;

		QString result = _T(""), bvar = _T(""), bvarwrap = _T(""), cond = _T(""), body = _T("");
		CNode *addon = getAddonNode();
		if( (addon && bvar_cb == 0 && bvarwrap_cb == 0 && meaning_cb) || (!addon && (bvar_cb != 0 || bvarwrap_cb != 0) ) )
		{
			long nestedLevel = name.length() > 0 ? level + 1 : level;
			CNode *pCurNode = l->GetFirstChild();
			for( j = 0; pCurNode && j < l->GetChildCount(); j++ )
			{
				if( pBvarWrapNode != 0 && pBvarWrapNode == pCurNode )
					bvarwrap += pCurNode->to_mathml( nestedLevel, dummy );
				else if( !xml_strcmp( pCurNode->to_mathml_data.name, FBL_TAG_condition ) )
					cond += pCurNode->to_mathml( nestedLevel, dummy );
				else if( !xml_strcmp( pCurNode->to_mathml_data.name, FBL_TAG_bvar ) )
					bvar += pCurNode->to_mathml( nestedLevel, dummy );
				else
					body += pCurNode->to_mathml( nestedLevel, dummy );
				pCurNode = pCurNode->GetNext();
			}
			if( addon && !bvar.length() && !bvarwrap.length() )
			{
				switch(addon->GetType())
				{
				case NODE_FRAME:
					bvar += ((CFrameNode*)addon)->CParentNode::to_mathml( nestedLevel, dummy );
					break;
				case NODE_LINE:
					bvar += ((CLineNode*)addon)->CParentNode::to_mathml( nestedLevel, dummy );
					break;
				default:
					bvar += addon->to_mathml( nestedLevel, dummy );
				}
			}
		}
		else
			break;

		if( name.length() > 0 )
		{
			nodesCount = 1;
			return ::mml_tag_nl( name, bvar + bvarwrap + cond + body, sattr, level );
		}

		nodesCount = dummy;
		return (bvar + bvarwrap + cond + body);
	}
	while(0);
	return to_mathml__content_recursive_tree( name, sattr, level, to_mathml__content__NEWLINE, nodesCount );
}

QString CFormulaNode::to_mathml__content_matrix_body( long level )
{
	long i, j, dummy = 0;
	CNode *pCurNode = 0;
	CFormulaNode *_node = 0;
	CLineNode *_line = getFormulaLineChild( 0 );
	if( !_line ) return _T("");

	pCurNode = _line->GetFirstChild();
	for( i = 0; pCurNode && i < _line->GetChildCount(); i++ )
	{
		if( pCurNode->GetType() == NODE_FORMULA )
		{
			_node = (CFormulaNode*)pCurNode;
			break;
		}
		pCurNode = pCurNode->GetNext();
	}
	if( !_node ) return _T("");

	QString _t, _l;
	pCurNode = _node->GetFirstChild();
	_t = _T("");
	for( i = 0; pCurNode && i < _node->GetNumLine(); i++ )
	{
		_l = _T("");
		for( j = 0; pCurNode && j < _node->GetNumColumn(); j++ )
		{
			_l += pCurNode->to_mathml( level + 1, dummy );
			pCurNode = pCurNode->GetNext();
		}
		_t += ::mml_tag_nl( QString(FBL_TAG_matrixrow), _l, QString( _T("") ), level );
	}
	return _t;
}

///////////////////////////////////////////////////////////////////////////////

QString CFormulaNode::to_mathml__content_harvest_plaintext( 
		long child_no, long& is_ok, long level, long& is_newline )
{
	is_newline = to_mathml__content__NEWLINE;
	is_ok = 0;
	QString result = _T("");

	CLineNode *line = 0;
	CFrameNode *frame = 0;
	CNode *node = GetChild( child_no );

	if( !node || node->GetType() != NODE_FRAME )
		return result;

	frame = (CFrameNode*)node;

	CNode *pCurLineNode = frame->GetFirstChild();
	while ( pCurLineNode )
	{
		if( pCurLineNode->GetType() != NODE_LINE )
			return result;
		line = (CLineNode*)pCurLineNode;
		CNode *pCurPTNode = line->GetFirstChild();
		while ( pCurPTNode )
		{
			if( pCurPTNode->GetType() != NODE_PLANETEXT )
			{
				if( pCurPTNode->to_mathml_data.right == ::getCM_sep() )
				{
					is_newline = to_mathml__content__NO_NEWLINE;
				}
				return result;	// there is some internal formatting of the 'cn' node
			}
			std::vector<QString> v_tag, v_text, v_attr;
			if( ((CPlaneText*)pCurPTNode)->to_mathml__getTag_n_Text( v_tag, v_text, v_attr, level ) )
			{
				if( v_text.size() == 1 && frame->GetChildCount() == 1 && line->GetChildCount() == 1 )
					result = v_text[ 0 ];
				return result;
			}
			for( long i = 0; i < (long) v_text.size(); i++ )
				result += v_text[ i ];
			pCurPTNode = pCurPTNode->GetNext();
		}
		pCurLineNode = pCurLineNode->GetNext();
	}
	is_ok = 1;
	return result;
}

///////////////////////////////////////////////////////////////////////////////

void CFormulaNode::to_mathml__content_xmlapply( long level )
{
	DRMessageManager dmm;
	long dmm_page = 0;
	TXML_Storage* bms = 0;

	if( LS_CompileXML( _T(""), to_mathml_data.name, bms, dmm, dmm_page ) || !bms )
		return;
	AST_TXML_Tag* root = bms->getFirstTag( FBL_TAG_apply );
	if( !root || !root->isExistTagChild() )
	{
		if( bms ) { delete bms; bms = 0; }
		return;
	}
	AST_TXML_Compound *_tags = root->getTags();
	if( !_tags )
	{
		if( bms ) { delete bms; bms = 0; }
		return;
	}

	for( long i = 0; i < _tags->getChildNumber(); i++ )
	{
		if( _tags->getChild( i ) && 
			_tags->getChild( i )->getNodeType() == ASTT_TXML_Tag )
		{
			AST_TXML_Tag* action = (AST_TXML_Tag*)_tags->getChild( i );
			if( !xml_strcmp(action->getTagName(), FBL_TAG_set_property) )
				to_mathml__content_xmlapply_action_setproperty( action, level );
		}
	}

	if( bms ) { delete bms; bms = 0; }
}

void CFormulaNode::to_mathml__content_xmlapply_action_setproperty( 
	AST_TXML_Tag* action, long level )
{
	AST_TXML_Compound *comp = action->getAttributes();
	if( !comp ) return;
	AST_TXML_Attribute *_dest = 0, *_src = 0, *_propname = 0, *_type = 0;
	AST_TXML_Node *node;
	for( long j = 0; j < comp->getChildNumber(); j++ )
	{
		node = comp->getChild( j );
		if( node->getNodeType() != ASTT_TXML_Attribute ) continue;

		if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_name ) )
			_propname = (AST_TXML_Attribute*)node;
		else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_node ) )
			_dest = (AST_TXML_Attribute*)node;
		else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_value ) )
			_src = (AST_TXML_Attribute*)node;
		else if( ((AST_TXML_Attribute*)node)->isAttribute( FBL_ATTR_type ) )
			_type = (AST_TXML_Attribute*)node;
	}
	if( !_dest || !_src || !_propname || !_type ) return;

	_dest->convert2Integer();
	if( _dest->getValueType() != ASTVALT_TXML_Int ) return;
	_src->convert2Integer();
	if( _src->getValueType() != ASTVALT_TXML_Int ) return;
	long dest_idx = _dest->getIntValue(), src_idx = _src->getIntValue();

	if( dest_idx < 0 || src_idx < 0 || 
		dest_idx >= GetChildCount() ||
		src_idx  >= GetChildCount() )
		return;

	CFormulaNode *dest_node = this;
	for(;;)
	{
		dest_node = dest_node->getFormulaDeepFormulaChild( dest_idx );
		if( !dest_node || (dest_node->to_mathml_data.is_content == 0 && !dest_node->isIgnoreContentConversion()))
			return;
		if( dest_node->to_mathml_data.name.length() && !dest_node->isIgnoreContentConversion() )
			break;
	}

	long is_ok = 0, is_newline = 0;
	QString prop_value = to_mathml__content_harvest_plaintext( src_idx, is_ok, level, is_newline );
	if( !is_ok ) return;

	if( !xml_strcmp( _type->getStringValue(), FBL_VAL_integer ) )
	{
		LSDigit ldigit;
		long i = 0;
		while( i < prop_value.size() && _isspace(prop_value.at(i)) ) i++;
		int r = ldigit.Analisys( prop_value.midRef(i) );
		i += ldigit.GetRead();
		while( i < prop_value.size() && _isspace(prop_value.at(i)) ) i++;
		if( r != LSDigit::Integer || i < prop_value.size() ) return;
		prop_value = QString("%1").arg(ldigit.mnt_int);
	}

	dest_node->setUnfilteredAttr( _propname->getStringValue(), prop_value );
}

///////////////////////////////////////////////////////////////////////////////

QString CFormulaNode::getApplyOp()
{
	QString op = _T(""), out = _T("");
	CNode *pNode = 0;

	switch( to_mathml_data.is_content )
	{
	case CONTENT_MATHML_CONVERTION2__RECURSIVE_APPLY:
		::_CM_apply_readApplyOperator( this, out, 2, &op );
		::_CM_apply_returnBackApplyOperator( this );
		if( op.length() )
			return QString( _T("\"") ) + op + QString( _T("\"") );
	case CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE:
	case CONTENT_MATHML_CONVERTION2__RECURSIVE_APPLY_MOMENTABOUT_ADDON_OP:
	case CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE_SET:
	case CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE_MATRIX:
	case CONTENT_MATHML_CONVERTION2__APPLY_OPERATOR_RECURSIVE_TREE:
	case CONTENT_MATHML_CONVERTION2__BACKRECURSIVE_TREE:
		pNode = getFormulaDeepChild( 0, 0 );
		break;

	case CONTENT_MATHML_CONVERTION2__RECURSIVE_APPLY_ADDON_OP:
	case CONTENT_MATHML_CONVERTION2__RECURSIVE_APPLY_ADDON_OP__BACKRECURSIVE_TREE:
		if( getAddonNode() && getAddonNode()->GetType() == NODE_LINE )
			pNode = ((CLineNode*)getAddonNode())->GetChild( 0 );
		break;
	default:
		break;
	}
	if( pNode && pNode->to_mathml_data.is_content == CONTENT_MATHML_CONVERTION2__SELF_CLOSING &&
		pNode->to_mathml_data.name.length() > 0 )
	{
		op = QString( _T("\"") ) + pNode->to_mathml_data.name + QString( _T("\"") );
	}
	return op;
}

///////////////////////////////////////////////////////////////////////////////
