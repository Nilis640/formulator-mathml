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

#ifndef __FORMULATOR_NODES_RULES_NAV_H__
#define __FORMULATOR_NODES_RULES_NAV_H__

#include "../node.h"

/////////////////////////////////////////////////////////////////////////////

//#ifdef _DEBUG 
inline
int isValidPosition( SelectNode &sn )
{
	return sn.GetNode() != NULL && 
		(sn.GetNode()->GetType() == NODE_LINE ||
		(sn.GetNode()->GetType() == NODE_PLANETEXT && sn.GetPosition() > 0 && 
		 sn.GetPosition() < ((CPlaneText*) sn.GetNode())->GetStringLength()));
}
//#endif // _DEBUG

/////////////////////////////////////////////////////////////////////////////

void GetPrevNextInLine( SelectNode &sn, CNode *&pPrev, CNode *&pNext );
void GetCaretInfo( const SelectNode& sn, PointFde &pt, SizeFde &sz );

/////////////////////////////////////////////////////////////////////////////

long GetEmptyLineCount( CParentNode *pParentNode );
int CaretToFirstEmptyLine( CParentNode *pParentNode, SelectNode &sn );
int CaretToSecondEmptyLine( CParentNode *pParentNode, SelectNode &sn );

/////////////////////////////////////////////////////////////////////////////

int MoveLeftRight( SelectNode &sn, SelectInfo &si, int fLeft, int fSel, int fWord, int fAllowIntoText );
int MoveUpDown( const RectFde &rc, const PointFde &pt, SelectNode &sn, SelectInfo &si, int fUp, int fSel );
int MoveHomeEnd( CFrameNode *pFrameNode, SelectNode &sn, SelectInfo &si, int fHome, int fSel, int fCtrl );

/////////////////////////////////////////////////////////////////////////////

void SimultaneousSorting( std::vector<RealFde> &values, std::vector<CNode*> &nodes );

/////////////////////////////////////////////////////////////////////////////

inline 
RealFde GetDistance_ShortestNormal( RealFde x, RealFde l, RealFde r )
{
	if( x < l )
		return (l - x);
	else if( x > r )
		return (x - r);
	else
		return 0.0;
}

inline
RealFde GetDistance_Point2Line( const PointFde &pt, const RectFde &rc )
{
	RealFde x = std::min( 
		GetDistance_ShortestNormal( pt.x(), rc.left(), rc.left() ), 
		GetDistance_ShortestNormal( pt.x(), rc.right(), rc.right() ) );
	RealFde y = GetDistance_ShortestNormal( pt.y(), rc.top(), rc.bottom() );
	return x * x + y * y;
}

inline
RealFde GetDistance_Point2Parent( const PointFde &pt, const RectFde &rc )
{
	RealFde x = GetDistance_ShortestNormal( pt.x(), rc.left(), rc.right() );
	RealFde y = GetDistance_ShortestNormal( pt.y(), rc.top(), rc.bottom() );
	return x * x + y * y;
}

inline
RealFde GetDistance_Point2Text( const PointFde &pt, const RectFde &rc )
{
	if( rc.contains( pt ) ) return 0.0;
	return GetDistance_Point2Parent( pt, rc );
}

/////////////////////////////////////////////////////////////////////////////

CNode* GetPropertyNode( SelectNode &sn, SelectInfo &si );
int SelectPropertyNode( CNode *pNode, SelectNode &sn, SelectInfo &si );

/////////////////////////////////////////////////////////////////////////////

#endif
