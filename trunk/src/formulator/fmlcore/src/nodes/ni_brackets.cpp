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

#include "HUtils/ihkstring.h"
#include "ni_brackets.h"
#include "ni_root.h"

/////////////////////////////////////////////////////////////////////////////

CLRBracketSelection::CLRBracketSelection( CRootNode& rootNode )
	: m_rootNode( rootNode )
	, m_sn()
	, m_isSNVectorLeft( -1 )
	, m_isSNLeft( -1 )
{
}

int CLRBracketSelection::isExistPosition( CNode *pNode )
{
	for( long i = 0; i < (long) m_snb.size(); i++ )
	{
		if( m_snb[ i ].GetNode() == pNode )
			return 1;
	}
	return 0;
}

void CLRBracketSelection::deletePosition( CNode *pNode )
{
	if( m_sn.GetNode() == pNode )
		ClearBrackets_SN();
	for( long i = 0; i < (long) m_snb.size(); )
	{
		if( m_snb[ i ].GetNode() == pNode )
		{
			notifyNodeOnRemoveBracket( m_snb[ i ] );
			m_snb[ i ].SetEmpty();
			m_snb.erase( m_snb.begin() + i );
		}
		else
			i++;
	}
	if( m_snb.size() == 0 )
		ClearBrackets_SNVector();
}

void CLRBracketSelection::notifyNodeOnRemoveBracket( const std::vector<SelectNode>& /*snb*/ )
{
	for( long i = 0; i < (long) m_snb.size(); i++ )
		notifyNodeOnRemoveBracket( m_snb[ i ] );
}

void CLRBracketSelection::notifyNodeOnRemoveBracket( const SelectNode& sn )
{
	if( sn.GetNode() != NULL && m_rootNode.haveChild( sn.GetNode() ) )
		sn.GetNode()->notifyNodeOnRemoveBracket( sn.GetPosition() );
}

void CLRBracketSelection::notifyNodeOnCreateBracket( const SelectNode& sn, int isLeft )
{
	if( sn.GetNode() != 0 )
		sn.GetNode()->notifyNodeOnCreateBracket( sn.GetPosition(), isLeft );
}

/////////////////////////////////////////////////////////////////////////////
