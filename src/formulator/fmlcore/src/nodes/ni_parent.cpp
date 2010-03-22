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

#include "node.h"
#include "dump/dumptags.h"
#include "extcontent/ext_node.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

inline
CNode* CreateCopy( CNode *pNode )
{
	if( pNode )
	{
		switch( pNode->GetType() )
		{
			case NODE_PLANETEXT:
				if( pNode->isExtNode() )
					pNode = new CExtNode( (CExtNode*) pNode );
				else
					pNode = new CPlaneText( (CPlaneText*) pNode );
			break;
			case NODE_LINE:
				pNode = new CLineNode( (CLineNode*) pNode );
			break;
			case NODE_FRAME:
				pNode = new CFrameNode( (CFrameNode*) pNode );
			break;
			case NODE_FORMULA:
				pNode = new CFormulaNode( (CFormulaNode*) pNode );
			break;
			default: 
				Q_ASSERT( 0 );
			break;
		}
	}
	return pNode;
}

inline
CNode* CreateNode( long type, long is_ext_node )
{
	CNode *pNode = NULL;
	switch ( type )
	{
		case NODE_PLANETEXT:
			if( is_ext_node )
				pNode = new CExtNode();
			else
				pNode = new CPlaneText();
			break;
		case NODE_LINE:
			pNode = new CLineNode();
			break;
		case NODE_FRAME:
			pNode = new CFrameNode();
			break;
		case NODE_FORMULA:
			pNode = new CFormulaNode();
			break;
		default:
			Q_ASSERT( 0 );
			break;
	}
	return pNode;
}

//////////////////////////////////////////////////////////////////////

CParentNode::CParentNode( long nNodeType ) 
	: CNode( nNodeType )
	, m_children()
	, m_nNumLine( 1 )
	, m_nNumColumn( 1 )
{
}

CParentNode::CParentNode( CParentNode *pNode ) 
	: CNode( pNode )
	, m_children()
	, m_nNumLine( pNode->m_nNumLine )
	, m_nNumColumn( pNode->m_nNumColumn )
{
	for( long i = 0; i < pNode->GetChildCount(); i++ )
		AddChild( CreateCopy( pNode->GetChild( i ) ) );
}

CParentNode::CParentNode( CParentNode *pNode, long nFrom, long nTo )
	: CNode( pNode )
	, m_children()
	, m_nNumLine( pNode->m_nNumLine )
	, m_nNumColumn( pNode->m_nNumColumn )
{
	if( nTo < 0 || nTo > pNode->GetChildCount() ) 
		nTo = pNode->GetChildCount();
	while ( nFrom < nTo )
	{
		AddChild( pNode->RemoveChild( nFrom ) );
		nTo--;
	}
}

CParentNode::~CParentNode()
{
	DeleteAllChild();
}

//////////////////////////////////////////////////////////////////////

void CParentNode::Store( QDataStream& ar, SelectNode& sn, const std::vector<SelectNode>& bracketSn ) const
{
	CNode::Store( ar, sn, bracketSn );

	ar << m_nNumLine;
	ar << m_nNumColumn;

	qint32 count = GetChildCount();
	ar << count;
	for( long i = 0; i < m_children.size(); i++ )
	{
		count = m_children.at(i)->GetType();
		ar << count;
		count = (m_children.at(i)->isExtNode() ? 1 : 0);
		ar << count;
		m_children.at(i)->Store( ar, sn, bracketSn );
	}
}

void CParentNode::Load( QDataStream& ar, SelectNode& sn, std::vector<SelectNode>& bracketSn, int isLeftBrLoad )
{
	DeleteAllChild();

	// load
	CNode::Load( ar, sn, bracketSn, isLeftBrLoad );

	CNode *pNode, *pPrev = NULL;

	ar >> m_nNumLine;
	ar >> m_nNumColumn;

	qint32 count;
	ar >> count;
	for( qint32 i = 0; i < count; i++ )
	{
		qint32 type;
		ar >> type;

		qint32 is_ext_node = 0;
		ar >> is_ext_node;

		pNode = CreateNode( type, is_ext_node );
		pNode->Load( ar, sn, bracketSn, isLeftBrLoad );

		pNode->SetParent( this );
		pNode->SetPrev( pPrev );
		if( pPrev )
			pPrev->SetNext( pNode );
		pPrev = pNode;
		m_children.push_back( pNode );
	}
}

//////////////////////////////////////////////////////////////////////

void CParentNode::SetLevel( long nLevel )
{
	CNode::SetLevel( nLevel );
	for( long i = 0; i < GetChildCount(); i++ )
		GetChild( i )->SetLevel( nLevel );
}

//////////////////////////////////////////////////////////////////////

long CParentNode::GetChildIndex( CNode *pNode )
{
	for( long i = 0; i < GetChildCount(); i++ )
		if( GetChild( i ) == pNode )
			return i;
	return -1;
}

//////////////////////////////////////////////////////////////////////

void CParentNode::InsertChild( long no, CNode *pNode )
{
	Q_ASSERT( pNode != NULL );
	pNode->SetParent( this );
	pNode->SetLevel( GetLevel() );
	if( GetChildCount() )
	{
		if( no < 0 || no >= GetChildCount() )
		{
			// add as "last" child
			pNode->SetPrev( GetLastChild() );
			GetLastChild()->SetNext( pNode );
			pNode->SetNext( NULL );
			m_children.push_back( pNode );
		}
		else if( no == 0 )
		{
			// add as "first" child
			pNode->SetPrev( NULL );
			pNode->SetNext( GetFirstChild() );
			GetFirstChild()->SetPrev( pNode );
			m_children.insert( no, pNode );
		}
		else
		{
			// insert child
			pNode->SetPrev( GetChild( no - 1 ) );
			pNode->SetNext( GetChild( no ) );
			GetChild( no - 1 )->SetNext( pNode );
			GetChild( no )->SetPrev( pNode );
			m_children.insert( no, pNode );
		}
	}
	else
	{
		// add child to empty array
		pNode->SetPrev( NULL );
		pNode->SetNext( NULL );
		m_children.push_back( pNode );
	}
}

void CParentNode::Add_UseAsVector( CNode *pNode )
{
	m_children.push_back( pNode );
}

//////////////////////////////////////////////////////////////////////

void CParentNode::InsertChildren( QVector<CNode*> &children, long nBefore )
{
	if( children.size() )
	{
		if( nBefore == -1 ) nBefore = (long) m_children.size();
		if( m_children.size() )
		{
			CNode *pPrev = (nBefore - 1 > -1 && nBefore - 1 < (long) m_children.size() ? m_children[ nBefore - 1 ] : NULL);
			CNode *pNext = (nBefore > -1 && nBefore < (long) m_children.size() ? m_children[ nBefore ]: NULL);
			if( pNext ) pNext->SetPrev( children[ children.size() - 1 ] );
			if( pPrev ) pPrev->SetNext( children[ 0 ] );
			children[ children.size() - 1 ]->SetNext( pNext );
			children[ 0 ]->SetPrev( pPrev );
		}
		for( long i = 0; i < children.size(); i++ )
		{
			children[ i ]->SetParent( this );
			children[ i ]->SetLevel( GetLevel() );
			m_children.insert( nBefore + i, children[ i ] );
		}
		children.clear();
	}
}

void CParentNode::RemoveOrCopyChildren( QVector<CNode*> &children, long nFrom, long nTo, int fRemove )
{
	if( nTo == -1 ) nTo = (long) m_children.size();
	Q_ASSERT( nTo <= (long) m_children.size() );
	Q_ASSERT( nTo >= nFrom );
	children.resize( nTo - nFrom );
	if( children.size() )
	{
		for( long i = 0; i < (long) children.size(); i++ )
		{
			children[ i ] = (fRemove ? m_children[ nFrom + i ] : CreateCopy( m_children[ nFrom + i ] ) );
			children[ i ]->SetParent( NULL );
		}

		children[ 0 ]->SetPrev( NULL );
		children[ children.size() - 1 ]->SetNext( NULL );
		if( fRemove )
		{
			m_children.remove( nFrom, nTo - nFrom );
			CNode *pPrev = (nFrom - 1 > -1 && nFrom - 1 < (long) m_children.size()) ? m_children[ nFrom - 1 ] : NULL;
			CNode *pNext = (nFrom > -1 && nFrom < (long) m_children.size()) ? m_children[ nFrom ] : NULL;
			if( pPrev ) pPrev->SetNext( pNext );
			if( pNext ) pNext->SetPrev( pPrev );
		}
	}
}

//////////////////////////////////////////////////////////////////////

void CParentNode::ExchangeChild( CNode *pChildNode, CNode *pNewNode )
{
	long pos = GetChildIndex( pChildNode );
	if( pos == -1 ) return;
	CNode *pNode = 0;
	ExchangeChild( pos, pNewNode, pNode );
}

void CParentNode::ExchangeChild( long no, CNode *pNewNode, CNode *&pOldNode )
{
	Q_ASSERT( pNewNode != NULL );

	pOldNode = GetChild( no );
	if( !pOldNode ) return;

	pNewNode->SetParent( pOldNode->GetParent() );
	pNewNode->SetLevel( pOldNode->GetLevel() );
	pNewNode->SetNext( pOldNode->GetNext() );
	pNewNode->SetPrev( pOldNode->GetPrev() );

	m_children[ no ] = pNewNode;

	if( pOldNode->GetPrev() )
		pOldNode->GetPrev()->SetNext( pNewNode );
	if( pOldNode->GetNext() )
		pOldNode->GetNext()->SetPrev( pNewNode );

	pOldNode->SetNext( NULL );
	pOldNode->SetPrev( NULL );
	pOldNode->SetParent( NULL );
}

CNode* CParentNode::RemoveChild( long no )
{
	CNode *pNode = GetChild( no );

	if( pNode )
	{
		if( pNode->GetPrev() )
			pNode->GetPrev()->SetNext( pNode->GetNext() );
		if( pNode->GetNext() )
			pNode->GetNext()->SetPrev( pNode->GetPrev() );
		pNode->SetNext( NULL );
		pNode->SetPrev( NULL );
		pNode->SetParent( NULL );

		m_children.remove( no );
	}

	return pNode;
}

void CParentNode::DeleteChild( long no )
{
	delete RemoveChild( no );
}

void CParentNode::DeleteAllChild()
{
	for( long no = 0; no < GetChildCount(); no++ )
		delete GetChild( no );
	m_children.clear();
}

//////////////////////////////////////////////////////////////////////

void CParentNode::RecalculateSizeChild()
{
	for( long no = 0; no < GetChildCount(); no++ )
		GetChild( no )->RecalculateSize( 0 );
}

void CParentNode::RecalculateSizeFull()
{
	for( long no = 0; no < GetChildCount(); no++ )
		GetChild( no )->RecalculateSizeFull();
	RecalculateSize( 0 );
}

//////////////////////////////////////////////////////////////////////

long CParentNode::GetVisibleEdge( const RectFde& /*rc*/, const PointFde& /*tl*/, CNode*& pFirst, CNode*& pLast )
{
	pFirst = pLast = NULL;
	return 0;
}

//////////////////////////////////////////////////////////////////////

void CParentNode::iterateNodeTree( NODE_TREE_ITERATOR call_back, void *arg )
{
	for( long i = 0; i < GetChildCount(); i++ )
		GetChild( i )->iterateNodeTree( call_back, arg );
	call_back( this, arg );
}

void CParentNode::iterateNodeTree_ToWidth( NODE_TREE_ITERATOR call_back, void *arg )
{
	call_back( this, arg );
	for( long i = 0; i < GetChildCount(); i++ )
		GetChild( i )->iterateNodeTree_ToWidth( call_back, arg );
}

//////////////////////////////////////////////////////////////////////

void CParentNode::setPrecedenceAsFirstChild( void )
{
	if( to_mathml_data.precedence == mmlPrecedence_Atom && GetFirstChild() )
	{
		to_mathml_data.precedence = GetFirstChild()->to_mathml_data.precedence;
		to_mathml_data.assoc = GetFirstChild()->to_mathml_data.assoc;
	}
}

short CParentNode::recalculatePrecedenceOnChildren( void )
{
	if( GetChildCount() == 1 && GetFirstChild() )
	{
		to_mathml_data.precedence = GetFirstChild()->to_mathml_data.precedence;
		to_mathml_data.assoc = GetFirstChild()->to_mathml_data.assoc;
	}
	else
	{
		to_mathml_data.precedence = mmlPrecedence_Atom;
		to_mathml_data.assoc = 0;
	}
	return to_mathml_data.precedence;
}

/////////////////////////////////////////////////////////////////////////////

void CParentNode::setReadOnlyAttribute4AllNonEmptyNodes()
{
	CNode *pNode;
	for( long i = 0; i < GetChildCount(); i++ )
	{
		pNode = GetChild( i );
		if( pNode->GetType() == NODE_PLANETEXT )
		{
			pNode->setReadOnly();
		}
		else if( ((CParentNode*) pNode)->GetChildCount() )
		{
			pNode->setReadOnly();
			((CParentNode*) pNode)->setReadOnlyAttribute4AllNonEmptyNodes();
		}
	}
}

int CParentNode::setReadOnlyAttribute4AllNonReadOnlyNodes()
{
	int nRet = 1;
	CNode *pNode;
	CFormulaNode *pForm;
	for( long i = 0; i < GetChildCount(); i++ )
	{
		pNode = GetChild( i );
		if( pNode->GetType() == NODE_PLANETEXT )
		{
			if( !pNode->isReadOnly() )
				nRet = 0;
			else
				pNode->setReadOnly();
		}
		else if( ((CParentNode*) pNode)->GetChildCount() )
		{
			int n = ((CParentNode*) pNode)->setReadOnlyAttribute4AllNonReadOnlyNodes();
			switch( pNode->GetType() )
			{
			case NODE_FORMULA:
				pForm = ((CFormulaNode*)pNode);
				if( !pForm->isReadOnly() )
				{
					if( ::mml_is_cm_token( pForm->to_mathml_data.name ) )
					{
						if( n && pForm->GetParent() )
						{
							CFormulaNode *frmParent = pForm->GetParent()->getParentContentFormulaNode( 1 );
							if( frmParent )
								pForm->setReadOnly();
						}
					}
					else
						nRet = 0;
				}
				break;
			case NODE_LINE:
				if( n ) pNode->setReadOnly();
				break;
			case NODE_FRAME:
				pNode->setReadOnly();
				break;
			default:
				break;
			}
		}
		else
			nRet = 0;
	}
	return nRet;
}

//////////////////////////////////////////////////////////////////////

void CParentNode::cleanSpareAutodetect()
{
	for( long no = 0; no < GetChildCount(); no++ )
		GetChild( no )->cleanSpareAutodetect();
}

int CParentNode::haveChild( CNode *pNode )
{
	for( long no = 0; no < GetChildCount(); no++ )
	{
		CNode *child = GetChild( no );
		if( child == pNode ) return 1;

		switch( child->GetType() )
		{
		case NODE_FRAME:
		case NODE_LINE:
		case NODE_FORMULA:
			if( ((CParentNode*) child)->haveChild( pNode ) ) return 1;
			break;
		default:
			break;
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////
