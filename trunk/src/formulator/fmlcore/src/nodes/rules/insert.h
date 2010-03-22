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

#ifndef __FORMULATOR_NODES_RULES_INSERT_H__
#define __FORMULATOR_NODES_RULES_INSERT_H__

#include "../node.h"
#include "../extcontent/ext_node.h"

CPlaneText* InsertAlnumPlaneText( SelectNode &sn, QChar ch, RealFde kegl );
CPlaneText* InsertPlaneText( SelectNode &sn, QChar ch, long style, RealFde kegl, CNodeInfo& ni, CNodeExInfo& ex_ni );
CPlaneText* InsertPlaneText( SelectNode &sn, QString &strText, long style, RealFde kegl, CNodeInfo& ni, CNodeExInfo& ex_ni );

void SetReadOnlyAttribute4AllNonEmptyNodes( CParentNode *pParentNode );

int InsertFromText( CPlaneText *pPlaneText, SelectNode &sn, int fUpCalc = -1 );
int InsertFromLine( CLineNode *pLineNode, SelectNode &sn, int fUpCalc = -1, SelectInfo *psi = NULL );
int InsertFromFrame( CFrameNode *pFrameNode, SelectNode &sn, int fUpCalc = -1, SelectInfo *psi = NULL );
int InsertFromFormula( CFormulaNode *pFormulaNode, SelectNode &sn, int fUpCalc = -1 );

CPlaneText* InsertPlaneText( SelectNode &sn, CExtNode *pNode );

/////////////////////////////////////////////////////////////////////////////
#endif//__FORMULATOR_NODES_RULES_INSERT_H__
