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

#ifndef __FORMULATOR_ROOTNODE_H__
#define __FORMULATOR_ROOTNODE_H__

#include "ni_brackets.h"
#include "node.h"
#include "extcontent/ext_node.h"

class CFormulatorView;
class CUndoRedo;
class HAutodetectMulticharOp;
class QFormulatorBaseWidget;

/////////////////////////////////////////////////////////////////////////////

#define FORMULIB_ROOTNODE_ONCHAR_FLAG			0x0001	// ON after OnChar, Insert from Clipboard or from Toolbars
#define __HERMITECH_TOOLBAR_MATHML__			_T("Hermitech Toolbar MathML Button Contents")

class __HERMITECH_FORMULIB__ CRootNode 
	: public CFrameNode
	, public CLRBracketSelection
{
public:
	CRootNode( QFormulatorBaseWidget *owner );
	virtual ~CRootNode();

public:
	virtual void Store( QDataStream& ar ) const;
	virtual void Load( QDataStream& ar );

	// drawing
	virtual int Draw( CFmlDrawEngine& fde, const PointFde& tl, const RectFde& rc );

	virtual void RecalculateSize( int bRecalc );
	virtual void RecalculateSizeFull();

public:
	SelectNode& GetCaret();
	int GetSelectInfo( PointFde &sb, PointFde &se );
	SelectInfo& GetSelectInfo();
	SelectNode& GetDrag();
	RectFde GetDragRect();

	CFormulaNode* GetContentFormulaUnderCaret();
	bool PrepareContentEqual2Evaluate( CFormulaNode *&formulaNode, CUndoRedo *pUndoRedo );

	int GetCaretInfo( PointFde &pt, SizeFde &sz );
	int GetCaretMessage( QString& rMessage );
	RealFde GetCurrentKegl();

public:
	// keyboard handlers
	int MoveLeftRight( int fLeft, int fSel, int fWord );
	int MoveHomeEnd( int fHome, int fSel, int fCtrl );
	int MoveUpDown( RectFde& rc, PointFde& pt, SizeFde& sz, int fUp, int fSel );
	int MovePgUpDown( RectFde& rc, SizeFde& ofs, int fUp, int fSel );

	int DragCaret( RectFde& rc, PointFde pt );
	int MoveCaret( RectFde& rc, PointFde pt, int fSel );
	int MoveCaret( CNode *pNode, int isRight );
	int DblClk( RectFde& rc, PointFde pt );

	int MoveDel( CUndoRedo *pUndoRedo );
	int MoveBs( CUndoRedo *pUndoRedo );
	int MoveReturn();
	int Char( unsigned int nChar, CUndoRedo *pUndoRedo );
	int Escape();

	int ApplyStyle( long style );
	int ApplyKegl( RealFde kegl );
	int IncDecKegl( RealFde value );

	// other function
	CNode* GetPropertyNode();
	int SelectPropertyNode( CNode *pNode );

	int SelectAll();
	int ClearAll();

	int PtInSelection( RectFde& rc, PointFde& pt );
	int DrInSelection();
	int IsSelection();

	CFrameNode* RemoveOrCopySelection( int fRemove );

public:

	// insert external node
	int InsertExternalNode( CExtNode *pNode );

	// insert plane text
	int InsertPlaneText( FXmlBtn *ptrFXmlBtn );
	int InsertPlaneText( QString &strText, int fUnicode );
	int InsertPlaneText( QString &strText, long style, RealFde kegl );
	int InsertPlaneTextByDFA( const QString& strText );

	int InsertMathML( FXmlBtn *ptrFXmlBtn, QString strSubstitution = "", int shouldMakeReadOnly = 1 );
	int InsertMathML( 
		const QString& strTask, const QString& strText, 
		CFormulaNode** pInsertedFormula = NULL, 
		FXmlBtn **ptrFXmlBtn = NULL,
		int shouldMakeReadOnly = 1 );
	int InsertMathML( const QString& strTask, const QString& strText, int fExclamation );
	int InsertMathML( 
		const QString& strTask, TXML_Storage* bms, 
		CFormulaNode** pInsertedFormula = NULL, 
		FXmlBtn **ptrFXmlBtn = NULL,
		int shouldMakeReadOnly = 1 );
	int InsertMathML_PreprocessButtonContent( 
		FXmlBtn *&ptrFXmlBtn, TXML_Storage* bms );
	int InsertMathML_Force( const QString& strTask, const QString& strText );

	int InsertFormula( FXmlBtn *ptrFXmlBtn, CFormulaNode **pFormulaNode );
	int Insert( CNode* pNode );
	int Insert( FXmlBtn *ptrFXmlBtn, int shouldMakeReadOnly = 1 );

	int ReplaceMathML( const QString& strTask, const QString& strText, int fExclamation );

public:
	QString to_xhtml( const QString& path2xsl, const QString& originator, const QString& title );
	void useLinebreakPosition( CFrameNode *node );

protected:
	void SetOnCharFlag();
	void ClearOnCharFlag();
	int IsOnCharFlag();

	int SetBracketsByChar( unsigned int nChar );
	int SetContentBrackets( int isLeft );
	long SetContentBrackets_traversal(
						int isLeft, const SelectNode& bracket, 
						std::vector<CBracketCoord>& cutted,
						const std::vector<CBracketCoord>& stopLineNode );
	int SetContentBrackets_breakLine( 
						int isLeft, const std::vector<CBracketCoord>& cutted, long& lslotPos,
						CFormulaNode *&lslot, CFormulaNode *&plslot );
	CFormulaNode* SetContentBrackets_breakLine_searchUpwardParent2Cut( CFormulaNode *pslot, CFormulaNode *upperLimit );
	CFormulaNode* SetContentBrackets_reorder( CFormulaNode *lslot, CFormulaNode *plslot, CFormulaNode *sharedFormula );

public:
	void createAutodetectMulticharOp();
	void deleteAutodetectMulticharOp();
	HAutodetectMulticharOp* getAutodetectMulticharOp();

protected:
	QFormulatorBaseWidget* owner();

protected:
	QFormulatorBaseWidget		*m_owner;
	HKShortFlag				m_shFlag;
	SelectNode				m_sn;		// current caret
	SelectInfo				m_si;		// selection information
	SelectNode				m_sndrag;	// drag caret
	RectFde 				m_rcdrag;	// drag rect
	HAutodetectMulticharOp*	m_AutodetectMulticharOp;
};

/////////////////////////////////////////////////////////////////////////////

inline 
QFormulatorBaseWidget* CRootNode::owner()
{
	return m_owner;
}

/////////////////////////////////////////////////////////////////////////////

inline 
SelectNode& CRootNode::GetCaret()
	{ return m_sn; }

inline 
SelectInfo& CRootNode::GetSelectInfo()
	{ return m_si; }

inline
int CRootNode::IsSelection()
	{ return GetSelectInfo().IsSelection(); }

inline
SelectNode& CRootNode::GetDrag()
	{ return m_sndrag; }

inline
RectFde CRootNode::GetDragRect()
	{ return m_rcdrag; }

/////////////////////////////////////////////////////////////////////////////

inline
int CRootNode::IsOnCharFlag()
	{ return m_shFlag.isSetFlag( FORMULIB_ROOTNODE_ONCHAR_FLAG ); }
inline
void CRootNode::SetOnCharFlag()
	{ m_shFlag.setFlag( FORMULIB_ROOTNODE_ONCHAR_FLAG ); }
inline
void CRootNode::ClearOnCharFlag()
	{ m_shFlag.clearFlag( FORMULIB_ROOTNODE_ONCHAR_FLAG ); }

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

inline
HAutodetectMulticharOp* CRootNode::getAutodetectMulticharOp()
	{ return m_AutodetectMulticharOp; }

/////////////////////////////////////////////////////////////////////////////
//
// HAutodetectMulticharOp
//
/////////////////////////////////////////////////////////////////////////////

class HAutodetectMulticharOp
{
protected:
	CLineNode *m_line;
	HKMap<QString, CPlaneText*> m_data1, m_data2;

public:
	HAutodetectMulticharOp( CRootNode& rootNode );
	~HAutodetectMulticharOp();

	CPlaneText* getOperator1( QString opName );
	CPlaneText* getOperator2( QString opName );

protected:
	void init( CRootNode& rootNode, HKMap<QString, CPlaneText*>& data, QString opName, QString btnID );
};

/////////////////////////////////////////////////////////////////////////////

#endif
