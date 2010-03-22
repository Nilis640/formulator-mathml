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

#include "ext_node.h"
#include "extc_test.h"
#include "extc_img.h"

///////////////////////////////////////////////////////////////////////////////

void CExtNodeContent_Base::Store( QDataStream& ar ) const
{
	ar << m_classid << m_size;
}

void CExtNodeContent_Base::Load( QDataStream& ar )
{
	ar >> m_classid >> m_size;
}

///////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define CExtNode_DEFAULT_INDENT	1.0

//////////////////////////////////////////////////////////////////////

CExtNode::CExtNode() 
	: CPlaneText()
{
	CPlaneText::Add( _T(' ') );

	m_pContent = NULL;
	setExtNode();
}

CExtNode::CExtNode( QString /*strText*/, long nStyle, CNodeInfo &ni, CNodeExInfo &ex_ni )
	: CPlaneText( QString(), nStyle, ni, ex_ni )
{
	m_pContent = NULL;
	setExtNode();
}

CExtNode::CExtNode( CExtNode* pExtNode ) 
	: CPlaneText( (CPlaneText*) pExtNode )
{
	m_pContent = NULL;
	setExtNode();

	if( pExtNode )
	{
		CExtNodeContent_Base *_content = pExtNode->getContent();
		if( _content )
			setContent( _content->Clone() );
		else
			setContent( NULL );
	}
}

//////////////////////////////////////////////////////////////////////

CExtNode::~CExtNode()
{
	releaseContent();
}

//////////////////////////////////////////////////////////////////////

int CExtNode::CopyText( const QString& /*lpszString*/, long nFStyle, const FS_LogFont_Color& lfs, int fUnicode )
	{ return CPlaneText::CopyText( _T(" "), nFStyle, lfs, fUnicode ); }

int CExtNode::CopyNodeInfo( CNode* pNode )
{
	CPlaneText::CopyNodeInfo( pNode );

	if( pNode && pNode->isExtNode() )
	{
		CExtNodeContent_Base *_content = ((CExtNode*) pNode)->getContent();
		if( _content )
			setContent( _content->Clone() );
		else
			setContent( NULL );
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////

void CExtNode::Store( QDataStream& ar, SelectNode& sn, const std::vector<SelectNode>& bracketSn ) const
{
	CPlaneText::Store( ar, sn, bracketSn );

	qint32 content_program_id = -1;
	if( getContent() != NULL )
		content_program_id = getContent()->getClassID();

	ar << content_program_id;
	if( content_program_id != -1 )
		getContent()->Store( ar );
}

void CExtNode::Load( QDataStream& ar, SelectNode& sn, std::vector<SelectNode>& bracketSn, int isLeftBrLoad )
{
	CPlaneText::Load( ar, sn, bracketSn, isLeftBrLoad );

	qint32 content_program_id = -1;
	ar >> content_program_id;

	CExtNodeContent_Base* pContent = NULL; 
	switch( content_program_id )
	{
	case EXTNODECONTENT_ID_TEST:
		pContent = new CExtNodeContent_Test();
		pContent->Load( ar );
		break;
	case EXTNODECONTENT_ID_IMG:
		pContent = new CExtNodeContent_Image();
		pContent->Load( ar );
		break;
	default:
		break;
	}
	setContent( pContent );
}

//////////////////////////////////////////////////////////////////////

int CExtNode::Draw( CFmlDrawEngine& fde, const PointFde& tl, const RectFde& rc )
{
	if( getContent() )
		getContent()->Draw( fde, tl, rc );
	return 1;
}

//////////////////////////////////////////////////////////////////////

RectFde CExtNode::getDrawSize( long nfr, long nto, RealFde *pascent )
{
	return (getContent() != NULL ? getContent()->getDrawSize() :
		CPlaneText::getDrawSize( nfr, nto, pascent ));
}

RectFde CExtNode::getDrawSize( long pos, RealFde *pascent )
{
	return getDrawSize( 0, pos, pascent );
}

long CExtNode::getPosFromPoint( const PointFde &pt )
{
	if( getContent() == NULL )
		return CPlaneText::getPosFromPoint( pt );

	PointFde apt = pt - GetAbsolutePosition();
	RectFde rc = getContent()->getDrawSize();
	return (apt.x() <= (rc.width() / 2.0) ? 0 : 1);
}

///////////////////////////////////////////////////////////////////////////////

void CExtNode::RecalculateSize( int bRecalc )
{
	if( getContent() == NULL )
	{
		CPlaneText::RecalculateSize( bRecalc );
	}
	else
	{
		RealFde ascent = 0.0;
		SizeFde sz = getContent()->getDrawSize().size();
		sz.rwidth() += getLeftIndent() + getRightIndent();
		sz.rheight() += getTopIndent() + getBottomIndent();

		if( GetSize() != sz )
		{
			SetSize( sz );
			SetVerticalCenter( getContent()->Recalculate_VerticalCenter( sz, ascent ) );
			if( bRecalc )
				GetParent()->RecalculateSize( bRecalc );
		}
	}
}

void CExtNode::RecalculateSizeFull()
{
	RecalculateSize( 0 );
}

///////////////////////////////////////////////////////////////////////////////
