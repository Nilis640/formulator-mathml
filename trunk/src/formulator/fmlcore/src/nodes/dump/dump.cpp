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

#include "../node.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG 

/////////////////////////////////////////////////////////////////////////////
// show equation structure from pNode 
/////////////////////////////////////////////////////////////////////////////

CNode* root( CNode* pNode )
{
	while( pNode->GetParent() )
		pNode = pNode->GetParent();
	return pNode;
}

void info( CNode *pNode, int level )
{
	static QString tagname[] = { _T("mml"), _T("slot"), _T("line"), _T("text"), _T("form"), _T("none") };

	QString str( _T('\t'), level );
	QString value;
	value.Format( _T("<%s ptr='0x%p' e='%c' m='%c' "), 
				tagname[ pNode->GetType() ], pNode, 
				pNode->isCannotBeEdited() ? '-' : '+',
				pNode->isCannotBeMoved() ? '-' : '+' );
	str += value;

	switch( pNode->GetType() )
	{
	case NODE_FRAME:
		str += _T(">\n");
		break;
	case NODE_LINE:
		str += _T(">\n");
		break;
	case NODE_FORMULA:
		value.Format( _T("auto='%s' name='%s' "), 
				((CFormulaNode*) pNode)->isAutodetectContent() ? _T("+") : _T("-"),
				((CFormulaNode*) pNode)->to_mathml_data.name );
		str += value;
		str += _T(">\n");
		break;
	case NODE_PLANETEXT:
		if( !pNode->isExtNode() && ((CPlaneText*) pNode)->GetStringLength() && ((CPlaneText*) pNode)->GetString()[ 0 ] < 256 )
			value.Format( _T("value='%s' "), ((CPlaneText*) pNode)->GetString() );
		//if( !pNode->isExtNode() )
		//	value.Format( _T("value='%s' "), ((CPlaneText*) pNode)->GetString() );
		//else
		//	value.Format( _T("classid='%d' "), ((CExtNode*) pNode)->getContent() ? ((CExtNode*) pNode)->getContent()->getClassID() : -1 );
		str += value;
		str += _T("/>\n");
		break;
	default:
		break;
	}
	TRACE( (const QString&) str );
}

void dump( CNode *pNode, int level )
{
	static QString tagname[] = { _T("mml"), _T("slot"), _T("line"), _T("text"), _T("form"), _T("none") };

	switch( pNode->GetType() )
	{
	//case NODE_FRAME:
	//case NODE_LINE:
	//	{
	//		CParentNode *pParentNode = (CParentNode*) pNode;
	//		if ( pParentNode->GetParent() == NULL )
	//			TRACE( _T("<!-- DUMP BEGIN -->\n") );

	//		int nCount = pParentNode->GetChildCount();
	//		for ( int i = 0; i < nCount; i++ )
	//			dump( pParentNode->GetChild( i ), level );

	//		if ( pParentNode->GetParent() == NULL )
	//			TRACE( _T("<!-- DUMP END -->\n") );
	//	}
	//	break;
	case NODE_FRAME:
	case NODE_LINE:
	case NODE_FORMULA:
		{
			CParentNode *pParentNode = (CParentNode*) pNode;
			if ( pParentNode->GetParent() == NULL )
				TRACE( _T("<!-- DUMP BEGIN -->\n") );

			info( pNode, level );

			int nCount = pParentNode->GetChildCount();
			for ( int i = 0; i < nCount; i++ )
			{
				level++;
				dump( pParentNode->GetChild( i ), level );
				level--;
			}

			QString str( _T('\t'), level );
			QString value;
			value.Format( _T("</%s>\n"), tagname[ pNode->GetType() ] );
			str += value;
			TRACE( (const QString&) str );

			if ( pParentNode->GetParent() == NULL )
				TRACE( _T("<!-- DUMP END -->\n") );
		}
		break;
	case NODE_PLANETEXT:
		{
			info( pNode, level );
		}
		break;
	default:
		break;
	}
}

#endif // _DEBUG

/////////////////////////////////////////////////////////////////////////////




#if 0

/////////////////////////////////////////////////////////////////////////////
//
// copy.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "formulator\FormuLib\src\nodes\node.h"
#include "formulator\FormuLib\src\nodes\ext_node.h"

#ifdef _DEBUG 

/////////////////////////////////////////////////////////////////////////////
// show equation structure from pNode 
/////////////////////////////////////////////////////////////////////////////

CNode* root( CNode* pNode )
{
	while( pNode->GetParent() )
		pNode = pNode->GetParent();
	return pNode;
}

void info( CNode *pNode, int level )
{
	QString value;
	QString str( _T('\t'), level );
	switch( pNode->GetType() )
	{
	case NODE_FRAME:
		value.Format( _T("Frame: 0x%p"), pNode );
		break;
	case NODE_LINE:
		value.Format( _T("Line: 0x%p"), pNode);
		break;
	case NODE_FORMULA:
		value.Format( _T("Formula: 0x%p (%s, %s)"), pNode, ((CFormulaNode*) pNode)->isAutodetectContent() ? _T("AUTO") : _T(""), ((CFormulaNode*) pNode)->to_mathml_data.name );
		break;
	case NODE_PLANETEXT:
		if( !pNode->isExtNode() )
			value.Format( _T("Text: 0x%p (%s)"), pNode, ((CPlaneText*) pNode)->GetString() );
		else
			value.Format( _T("Text: 0x%p (%d)"), pNode, ((CExtNode*) pNode)->getContent() ? ((CExtNode*) pNode)->getContent()->getClassID() : -1 );
		break;
	default:
		break;
	}
	str += value;
///*
	str += _T('\t';)
	str += (pNode->isCannotBeEdited() ? _T("-e") : _T("+e"));
	str += (pNode->isCannotBeMoved() ? _T("-m") : _T("+m"));
	str += _T('\n');
	//str += value;
//*/
/*
	value.Format( "\tps: (%d, %d)", pNode->GetPosition().x(), pNode->GetPosition().y() );
	str += value;
	value.Format( "\tsz: (%d, %d)", pNode->GetSize().cx, pNode->GetSize().cy );
	str += value;
	value.Format( "\tvc: (%d)", pNode->GetVerticalCenter() );
	str += value;

	value.Format( "\tin: (%d, %d, %d, %d)", 
		pNode->getLeftIndent(), pNode->getTopIndent(),
		pNode->getRightIndent(), pNode->getBottomIndent() );
	str += value;
*/
	value.Format( _T("\tlevel: (%d)"), pNode->GetLevel() );
	str += value;

	str += _T('\n');
	TRACE( (const QString&) str );
}

void dump( CNode *pNode, int level )
{
	switch( pNode->GetType() )
	{
	case NODE_FRAME:
	case NODE_LINE:
	case NODE_FORMULA:
		{
			CParentNode *pParentNode = (CParentNode*) pNode;
			if ( pParentNode->GetParent() == NULL )
				TRACE( _T("=========== dump ===========\n") );

			info( pNode, level );

			int nCount = pParentNode->GetChildCount();
			for ( int i = 0; i < nCount; i++ )
			{
				level++;
				dump( pParentNode->GetChild( i ), level );
				level--;
			}

			if ( pParentNode->GetParent() == NULL )
				TRACE( _T("=========== dump ===========\n") );
		}
		break;
	case NODE_PLANETEXT:
		info( pNode, level );
		break;
	default:
		break;
	}
}

#endif // _DEBUG

#endif
