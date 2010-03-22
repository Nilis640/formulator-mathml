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

#ifndef __FORMULATOR_NODES_RULES_EDIT_H__
#define __FORMULATOR_NODES_RULES_EDIT_H__

#include "../node.h"

class CUndoRedo;
class HAutodetectMulticharOp;

void BreakPlaneText( SelectNode &sn, int fUpCalc = 1, SelectNode *psb = NULL, SelectNode *pse = NULL );
void BreakLine( SelectNode &sn, int fUpCalc = 1, SelectNode *psb = NULL, SelectNode *pse = NULL );

int GlueText( SelectNode &sn, CNode *pPrev, int fUpCalc = 1, SelectNode *psb = NULL, SelectNode *pse = NULL );
int GlueLine( SelectNode &sn, CNode *pPrev, int fUpCalc = 1, SelectNode *psb = NULL, SelectNode *pse = NULL );
int GlueTextInLine( CLineNode *pLineNode, SelectInfo *psi = NULL );

int MoveDelBs_RemoveOperatorFromApply( CPlaneText *pOperatorNode, CLineNode *pParentLine, SelectNode &sn, HAutodetectMulticharOp *autoMulticharOp );
int MoveDelBs_RemoveBrackets( CFormulaNode* bracketsNode, SelectNode &sn, int isSNBeforeOpNode, int isSNBeforeBracket );

int MoveDel( SelectNode &sn, SelectInfo &si, HAutodetectMulticharOp *autoMulticharOp, CUndoRedo *pUndoRedo );
int MoveBs( SelectNode &sn, SelectInfo &si, HAutodetectMulticharOp *autoMulticharOp, CUndoRedo *pUndoRedo );

int OnChar( QChar ch, SelectNode &sn, SelectInfo &si );

class HAutodetectMulticharOp;
int OnOperator( QChar ch, SelectNode &sn, SelectInfo &si, QString& btnID, QString& btnIDParam, HAutodetectMulticharOp* autoMulticharOp, int isOnCharState, const SelectNode &snlb, CUndoRedo *pUndoRedo );

int IsMoveDelCheck( SelectNode &sn, SelectInfo &si );
int IsMoveBsCheck( SelectNode &sn, SelectInfo &si );

/////////////////////////////////////////////////////////////////////////////

int CutLeftRight( SelectNode &sn, SelectInfo &si, 
	CFrameNode **ppFrameNodeL, CFrameNode **ppFrameNodeR );

/////////////////////////////////////////////////////////////////////////////

int PasteLeftRight( SelectNode &sn, SelectInfo &si, 
	CFrameNode *pFrameNodeL, CFrameNode *pFrameNodeR, CFormulaNode *pFormulaNode );

/////////////////////////////////////////////////////////////////////////////

#endif
