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

#if !defined( __FORMULATOR_NODES_SELECTNODE_H__ )
#define __FORMULATOR_NODES_SELECTNODE_H__

#include "../../fmlcore_global.h"

/////////////////////////////////////////////////////////////////////

class CNode;

class __HERMITECH_FORMULIB__ SelectNode
{
public:
	SelectNode( CNode *pNode = NULL, long nPosition = 0 );

	void SetEmpty();
	int IsEmpty() const;

	CNode* GetNode() const;
	void SetNode( CNode *pNode );

	long GetPosition() const;
	void SetPosition( long nPosition );
	void IncPosition();
	void DecPosition();

	SelectNode& operator = ( const SelectNode& sn );

	int operator == ( const SelectNode &sn ) const;
	int operator == ( SelectNode &sn );

	int operator != ( const SelectNode &sn ) const;
	int operator != ( SelectNode &sn );

protected:
	CNode	*m_pNode;
	long	m_nPosition;
};

/////////////////////////////////////////////////////////////////////

inline
SelectNode::SelectNode( CNode *pNode, long nPosition ) 
	: m_pNode( pNode ), m_nPosition( nPosition )
{
}

inline
void SelectNode::SetEmpty()
{
	SetNode( NULL );
	SetPosition( 0 );
}

inline
int SelectNode::IsEmpty() const
{
	return GetNode() == NULL;
}

inline
CNode* SelectNode::GetNode() const
{
	return m_pNode;
}

inline
void SelectNode::SetNode( CNode *pNode )
{
	m_pNode = pNode;
}

inline
long SelectNode::GetPosition() const
{
	return m_nPosition;
}

inline
void SelectNode::SetPosition( long nPosition )
{
	m_nPosition = nPosition;
}

inline
void SelectNode::IncPosition()
{
	m_nPosition++;
}

inline
void SelectNode::DecPosition()
{
	m_nPosition--;
}

inline
SelectNode& SelectNode::operator = ( const SelectNode& sn )
{
	SetNode( sn.GetNode() );
	SetPosition( sn.GetPosition() );
	return *this;
}

inline
int SelectNode::operator == ( const SelectNode &sn ) const
{
	return (GetNode() && GetNode() == sn.GetNode() && GetPosition() == sn.GetPosition());
}

inline
int SelectNode::operator == ( SelectNode &sn ) 
{
	return (GetNode() && GetNode() == sn.GetNode() && GetPosition() == sn.GetPosition());
}

inline
int SelectNode::operator != ( const SelectNode &sn ) const
{
	return (GetNode() && (GetNode() != sn.GetNode() || GetPosition() != sn.GetPosition()));
}

inline
int SelectNode::operator != ( SelectNode &sn ) 
{
	return (GetNode() && (GetNode() != sn.GetNode() || GetPosition() != sn.GetPosition()));
}

/////////////////////////////////////////////////////////////////////////////

class SelectInfo
{
public:
	SelectInfo();
	SelectInfo( SelectInfo &si );

	int IsSelection();
	void SetEmpty();

	SelectNode& GetBegin();
	SelectNode& GetEnd();

	SelectInfo& operator = ( SelectInfo &si );

	int operator == ( SelectInfo &si );

protected:
	SelectNode	m_sb;
	SelectNode	m_se;
};

/////////////////////////////////////////////////////////////////////

inline
SelectInfo::SelectInfo() 
	: m_sb(), m_se()
{
}

inline
SelectInfo::SelectInfo( SelectInfo &si ) 
	: m_sb( si.GetBegin() ), m_se( si.GetEnd() )
{
}

inline
int SelectInfo::IsSelection()
{
	return GetBegin() != GetEnd();
}

inline
void SelectInfo::SetEmpty()
{
	GetBegin().SetEmpty();
	GetEnd().SetEmpty();
}

inline
SelectNode& SelectInfo::GetBegin()
{
	return m_sb;
}

inline
SelectNode& SelectInfo::GetEnd()
{
	return m_se;
}

inline
SelectInfo& SelectInfo::operator = ( SelectInfo &si )
{
	GetBegin() = si.GetBegin();
	GetEnd() = si.GetEnd();
	return *this;
}

inline
int SelectInfo::operator == ( SelectInfo &si ) 
{
	return (GetBegin() == si.GetBegin() && GetEnd() == si.GetEnd());
}

/////////////////////////////////////////////////////////////////////

#endif
