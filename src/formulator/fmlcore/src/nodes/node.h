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

#if !defined( __FORMULATOR_NODE_H__ )
#define __FORMULATOR_NODE_H__

#include <vector>
#include "HUtils/idrawing.h"

#include "../fmlcore_global.h"
#include "../mathml/mml_operator.h"
#include "../mathml/mml_format.h"
#include "../mathml/mml_defs.h"
#include "info/nodeinfo.h"
#include "info/nodeexinfo.h"
#include "info/n_formula_calc.h"
#include "node2proplist.h"

/////////////////////////////////////////////////////////////////////////////

class CNodeExInfo;
  class CPlaneText;
  class CFormulaNode;

class CNodeInfo;
  class CNode;
    class CPlaneText;
    class CParentNode;
      class CLineNode;
      class CFrameNode;
		class CRootNode;
      class CFormulaNode;

/////////////////////////////////////////////////////////////////////////////

#define HF_PLANETEXT_HUB_SYMBOL	' '
typedef void (*NODE_TREE_ITERATOR)( CNode*, void* );

class CCM_apply_args;

#define SVG_NodeTemplate_Parameter_X		0
#define SVG_NodeTemplate_Parameter_Y		1
#define SVG_NodeTemplate_Parameter_CX		2
#define SVG_NodeTemplate_Parameter_CY		3

/////////////////////////////////////////////////////////////////////////////
// class CNode

class __HERMITECH_FORMULIB__ CNode : public CNodeInfo
{
protected:
	const qint32		m_nNodeType;			
	qint32				m_nLevel;				
	QString				m_strName;				
	RealFde				m_vcenter;				
	RealFde				m_left, m_right;		
	RealFde				m_top, m_bottom;		
	RealFde				m_aPosition[ 4 ];		
	CNode*				m_pNext;				
	CNode*				m_pPrev;				
	CParentNode*		m_pParent;				
	HKMap<long, unsigned char> m_brackets;		

public:
        long GetType() const;
	long GetLevel() const;

	CParentNode* GetParent() const;
	CParentNode* GetRootParent() const;
	void SetParent( CParentNode *pParent );
	CNode* GetNext() const;
	void SetNext( CNode* pNode );
	CNode* GetPrev() const;
	void SetPrev( CNode *pNode );

	CNode* GetNodeFromType( long nNodeType, long *pnPos = NULL ) const;
	CLineNode* GetLineNode( long *pnPos = NULL ) const;
	CFrameNode* GetFrameNode( long *pnPos = NULL ) const;
	long GetParentFrameCount() const;

	RectFde GetRect() const;
	SizeFde GetSize() const;
	void SetSize( const SizeFde& sz );

	RealFde GetCoord( long i ) const;
	void SetCoord( long i, RealFde value );

	PointFde GetAbsolutePosition( void );
	PointFde GetPosition( void ) const;
	void SetPosition( const PointFde& pt );
	void SetAlignmentPosition( const RectFde &rect, unsigned int nType, RealFde maxVC );
	RealFde& getPosition( long i );
	void getKernelPositionPtr( RealFde*& a );

	RealFde GetVerticalCenter();
	void SetVerticalCenter( RealFde vcenter );

	RealFde getLeftIndent( void ) const;
	RealFde getTopIndent( void ) const;
	RealFde getRightIndent( void ) const;
	RealFde getBottomIndent( void ) const;
	void setLeftIndent( RealFde a );
	void setTopIndent( RealFde a );
	void setRightIndent( RealFde a );
	void setBottomIndent( RealFde a );
	void setIndents( RealFde l, RealFde t, RealFde r, RealFde b );
	void incLeftIndent( RealFde a );
	void incTopIndent( RealFde a );
	void incRightIndent( RealFde a );
	void incBottomIndent( RealFde a );

	void SetName( const QString& str );
	const QString& GetName( void );

	RealFde LengthPttoNode( PointFde& pt, RealFde kX );

public:
	void UseBtnCalc_InitByThisSize( FBtn& btn );
	void UseBtnCalc_ChangeThisMargin( FBtn& btn );
	void UseBtnCalc_ChangeThisSize( FBtn& btn );
	static int UpdateTextNode( const FBtn& btn, QString& text, long& style, CNodeInfo& _info, CNodeExInfo& _ex_info );
	static CFormulaNode* CreateFormulaNode( const FBtn& btn );

protected:
	CNode( long nNodeType );
	CNode( CNode* pNode );

public:
	virtual ~CNode();

public:
	virtual void Store( QDataStream& ar, SelectNode& sn, const std::vector<SelectNode>& bracketSn ) const;
	virtual void Load( QDataStream& ar, SelectNode& sn, std::vector<SelectNode>& bracketSn, int isLeftBrLoad );
	void StoreBrackets( QDataStream& ar, const std::vector<SelectNode>& bracketSn ) const;
	void LoadBrackets( QDataStream& ar, std::vector<SelectNode>& bracketSn, int isLeftBrLoad );

	virtual int CopyNodeInfo( CNode* pNode );

	virtual int Draw( CFmlDrawEngine& fde, const PointFde& tl, const RectFde& rc ) = 0;

	virtual void RecalculateSize( int bRecalc ) = 0;
	virtual void RecalculateSizeFull() = 0;

	virtual RealFde getAscent();
	virtual RealFde getDescent();

	virtual void SetLevel( long nLevel );

	virtual QString GetNodeText() { return QString(); };

	virtual int RunCalc() { return 0; };

        virtual void read_mathml( AST_TXML_Tag * /*node*/ ) {};
        virtual void read_mathml( AST_TXML_Compound * /*_math_line*/ ) {};
        virtual QString to_mathml( long /*nLevel*/, long& nodesCount ) { nodesCount = 0; return QString( _T( "" ) ); };
	virtual QString getMathMLMarkup2Text();
	virtual QString getMathMLNodeDescription();
	void setTextPropertyList( CNode_TextPropertyList& props, std::vector<char>& supports, std::map<QString,QString>& attrMap );
	virtual void setTextPropertyList( CNode_TextPropertyList& props, std::vector<char>& supports );
	virtual void cleanSpareAutodetect();

	void notifyNodeOnRemoveBracket( long bracketPosition );
	void notifyNodeOnCreateBracket( long bracketPosition, int isLeft );
	int hasBracket( long bracketPosition, int isLeft );
	int DrawBrackets( CFmlDrawEngine& fde, const PointFde& tl, const RectFde& rc );

	CLineNode* getParentLineNode( int useReadonly, long *childIndexOfRequest = 0 );
	CFormulaNode* getParentFormulaNode( int useIgnoreNode, long *childIndexOfRequest = 0 );
	CFormulaNode* getParentContentFormulaNode( int useIgnoreNode, long *childIndexOfRequest = 0 );

	int isContentTextOperator();
	int isContentApply();

public:
	virtual void iterateNodeTree( NODE_TREE_ITERATOR call_back, void *arg );
	virtual void iterateNodeTree_ToWidth( NODE_TREE_ITERATOR call_back, void *arg );
};

/////////////////////////////////////////////////////////////////////////////
// CParentNode
/////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CParentNode : public CNode
{
private:
	QVector<CNode*>		m_children;
protected:
	qint32					m_nNumLine;
	qint32					m_nNumColumn;

public:
	CParentNode( long nNodeType );

protected:
	CParentNode( CParentNode *pNode );
	CParentNode( CParentNode *pNode, long nFrom, long nTo );
	virtual ~CParentNode();

public:
	long GetNumLine( void );
	long GetNumColumn( void );
	void SetNumLine( long nNumLine );
	void SetNumColumn( long nNumColumn );

	long GetChildCount() const;
	CNode* GetFirstChild();
	CNode* GetLastChild();
	CNode* GetChild( long no );

	void Add_UseAsVector( CNode *pNode );
	void AddChild( CNode *pNode );
	void InsertChild( long no, CNode *pNode );
	void DeleteChild( long no );
	void DeleteAllChild();
	CNode* RemoveChild( long no );
	void ExchangeChild( long no, CNode *pNewNode, CNode *&pOldNode );
	void ExchangeChild( CNode *pChildNode, CNode *pNewNode );

	void InsertChildren( QVector<CNode*> &children, long nBefore = -1 );
	void RemoveOrCopyChildren( QVector<CNode*> &children, long nFrom = 0, long nTo = -1, int fRemove = -1 );

	long GetChildIndex( CNode *pNode );
	void DeleteChild( CNode *pNode );
	void AddChildAfter( CNode* pNode, CNode* pAfterNode );

	int IsValidNode( CNode *pNode );

	virtual long GetVisibleEdge( const RectFde &rc, const PointFde &tl, CNode *&pFirst, CNode *&pLast );

public:
	virtual void SetLevel( long nLevel );
	void setPrecedenceAsFirstChild( void );
	short recalculatePrecedenceOnChildren( void );
	void setReadOnlyAttribute4AllNonEmptyNodes();
	int setReadOnlyAttribute4AllNonReadOnlyNodes();
	virtual void cleanSpareAutodetect();

	int haveChild( CNode *pNode );

	virtual void Store( QDataStream& ar, SelectNode& sn, const std::vector<SelectNode>& bracketSn ) const;
	virtual void Load( QDataStream& ar, SelectNode& sn, std::vector<SelectNode>& bracketSn, int isLeftBrLoad );

	virtual void RecalculateSizeChild();
	virtual void RecalculateSizeFull();

	virtual QString to_mathml( long nLevel, long& nodesCount );

public:
	virtual void iterateNodeTree( NODE_TREE_ITERATOR call_back, void *arg );
	virtual void iterateNodeTree_ToWidth( NODE_TREE_ITERATOR call_back, void *arg );
	virtual void iterateNodeTree4Referenced( const QString& id, CFormulaNode*& pDest );
};

/////////////////////////////////////////////////////////////////////////////
// class CPlaneText
/////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CPlaneText
	: public CNode
	, public CNodeExInfo
{
protected:
	QString				m_string;			
	qint32				m_nFStyleNo;		
	RealFde				m_kegl;				
	FS_LogFont_Color*	m_plfc;				

public:
	// text operations
	long GetStringLength( void );
	void SetString( const QString& lpszString );
	const QString& GetString( void ) const;

	virtual void Add( QChar ch );
	virtual void Add( QString &str );
	virtual void Insert( long npos, QChar ch );
	virtual void Insert( long npos, QString &str );
	virtual void Delete( long npos );
	virtual void DeleteLast();

	virtual int isSingle( void );
	int isUnicode( void );
	int isNewPlaneText( RealFde kegl, long fstyle );
	int isGluing( CPlaneText *pPlaneText );

	virtual void Glue( CPlaneText *pPlaneText );

	long getFStyle( void ) const;
	void setFStyle( long nFStyle, const FS_LogFont_Color& lfs );

	const FS_LogFont_Color& getOtherStyleFont( void );
	const FS_LogFont_Color& getNeedFont( void );

	FS_LogFont getFont( void );

	RealFde getLeftAdding( void );
	RealFde getRightAdding( void );

	RealFde getTextKeglSize( void ) const;
	void setTextKeglSize( RealFde kegl );

	void setMathSpace( CNodeInfo_Property& ni );

protected:
	virtual int canDraw( SizeFde& sz, RealFde& height );

public:
	CPlaneText();
	CPlaneText( QString strText, long nStyle, CNodeInfo &ni, CNodeExInfo &ex_ni );
	CPlaneText( CPlaneText* pPlaneText );
	CPlaneText( CPlaneText* pPlaneText, long nFrom, long nTo );
	virtual ~CPlaneText();

public:
	virtual void Store( QDataStream& ar, SelectNode& sn, const std::vector<SelectNode>& bracketSn ) const;
	virtual void Load( QDataStream& ar, SelectNode& sn, std::vector<SelectNode>& bracketSn, int isLeftBrLoad );

	virtual void SetLevel( long nLevel );

	virtual int CopyText( const QString& lpszString, long nFStyle, const FS_LogFont_Color& lfs, int fUnicode );
	virtual int CopyNodeInfo( CNode* pNode );

	virtual int Draw( CFmlDrawEngine& fde, const PointFde& tl, const RectFde& rc );

	virtual QString GetNodeText();

	virtual int RunCalc();

	virtual RectFde getDrawSize( long frp, long nto, RealFde *pascent = NULL );
	virtual RectFde getDrawSize( long pos = -1, RealFde *pascent = NULL );
	virtual long getPosFromPoint( const PointFde &pt );

	virtual void RecalculateSize( int bRecalc );
	virtual void RecalculateSizeFull();

	virtual RealFde getAscent();
	virtual RealFde getDescent();
	virtual RealFde getLeftBearing();
	virtual RealFde getRightBearing();
	virtual RealFde getTopBearingMin( long nfr, long nto );

	virtual QString getMathMLNodeDescription();
	QString to_mathml__getOneTag( std::vector<QString>* pConstructedTags = 0 );
	virtual QString to_mathml( long nLevel, long& nodesCount );
	QString to_mathml__content( int& retry, long level, long& nodesCount );
	QString to_mathml__get_attribute_str( QString& _tag, QString& _body, int *isDefaultMMLChanged = 0 );
	int to_mathml__getTag_n_Text(	std::vector<QString>& v_tag, std::vector<QString>& v_text, 
									std::vector<QString>& v_attr, long level );

	virtual void setTextPropertyList( CNode_TextPropertyList& props, std::vector<char>& supports );

protected:
	void RecalculateSize_Base( SizeFde& nSize, RealFde& ascent );
	RealFde Recalculate_VerticalCenter( SizeFde nSize, RealFde ascent, int cannotdraw );
};

/////////////////////////////////////////////////////////////////////////////

#define CLineNode_read_mathml_MAYBE_EMPTY_NESTING_FRAMES	0
#define CLineNode_read_mathml_WITHOUT_EMPTY_NESTING_FRAMES	1

/////////////////////////////////////////////////////////////////////////////
// class CLineNode

class __HERMITECH_FORMULIB__ CLineNode 
	: public CParentNode
{
protected:
	RealFde		max_cy_notstretchy;

public:
	void AddListInTail( CLineNode *pNode );
	void AddPartOfListInTail( CLineNode *pNode, CNode *pNodeFrom );

	RealFde getMaxCYNotStretchy( void );

	int isGluing( CLineNode *pLineNode );

public:
	CLineNode();
	CLineNode( CLineNode* pLineNode );
	CLineNode( CLineNode* pLineNode, long nFrom, long nTo );
	virtual ~CLineNode();

public:
	virtual void Store( QDataStream& ar, SelectNode& sn, const std::vector<SelectNode>& bracketSn ) const;
	virtual void Load( QDataStream& ar, SelectNode& sn, std::vector<SelectNode>& bracketSn, int isLeftBrLoad );

	// draw
	virtual int Draw( CFmlDrawEngine& fde, const PointFde& tl, const RectFde& rc );

	virtual QString GetNodeText();

	virtual void RecalculateSize( int bRecalc );

	virtual QString to_mathml( long nLevel, long& nodesCount );
	virtual void read_mathml( AST_TXML_Compound *node, int mode = CLineNode_read_mathml_MAYBE_EMPTY_NESTING_FRAMES );
	virtual void read_mathml( AST_TXML_Tag *node );
	virtual QString getMathMLNodeDescription();

protected:
	void RecalculateSize_ReArrangeChildPos( SizeFde& sz );

public:
	long getPosFromPoint( const RectFde &rc, const PointFde &pt );

	CNode* read_mathml_mrow2empty_frame( AST_TXML_Tag *node );

	void PumpLine2Line( CLineNode*& source );
	void PumpLine2LineAtHead( CLineNode*& source );
	CFormulaNode* getContentFormulaChild( long idx );
	CFormulaNode* getLastContentFormulaChild();
	CFormulaNode* getFormulaChild( long idx );
	CFormulaNode* getFirstFormulaChild();
	CFormulaNode* getLastFormulaChild();
	virtual void cleanSpareAutodetect();
	CFormulaNode* convertDivSup2Nop( CFormulaNode *&opFormula, enum TAG2MML op, int isReverse );

	CFormulaNode* getContentTokenParent_Transparent();
	void setCMIgnore4Children( void );

	virtual long GetVisibleEdge( 
		const RectFde &rc, const PointFde &tl, CNode *&pFirst, CNode *&pLast );

	void InsertMML_CM_wrap_into_brackets();
	void InsertMML_CM_remove_outof_brackets( CLineNode *bracketed_line );

	CNode* InsertMMLTokenNode( AST_TXML_Tag *node, std::vector<CPlaneText*> *nodes_dump = 0 );
	CFormulaNode* InsertMMLFormulaNode( AST_TXML_Tag *node );
	CFormulaNode* InsertMMLFormulaNode_MTABLE( AST_TXML_Tag *node );
	void InsertMMLFormulaNode_MTABLE_readAttr( AST_TXML_Tag *node, CFormulaNode *new_node );
	CFormulaNode* InsertMMLFormulaNode_MSPACE( AST_TXML_Tag *node );
	CFormulaNode* InsertMMLFormulaNode_MFENCED( AST_TXML_Tag *node );
	CFormulaNode* InsertMMLFormulaNode_MFRAC( AST_TXML_Tag *node );
	CFormulaNode* InsertMMLFormulaNode_MSTYLE( AST_TXML_Tag *node );
	CFormulaNode* InsertMMLFormulaNode_MSTYLE_recursive( AST_TXML_Tag *node, const AST_TXML_Compound* _new_attr, std::map<QString, int>& map_isMStyleOwn );
	CFormulaNode* InsertMMLFormulaNode_MPADDED( AST_TXML_Tag *node );
	CFormulaNode* InsertMMLFormulaNode_MUNDER( AST_TXML_Tag *node );
	CFormulaNode* InsertMMLFormulaNode_MOVER( AST_TXML_Tag *node );
	CFormulaNode* InsertMMLFormulaNode_MUNDEROVER( AST_TXML_Tag *node );
	CFormulaNode* InsertMMLFormulaNode_MMULTISCRIPTS( AST_TXML_Tag *node );
	CFormulaNode* InsertMMLFormulaNode_MENCLOSE( AST_TXML_Tag *node );
	CFormulaNode* InsertMMLFormulaNode_MACTION( AST_TXML_Tag *node );
	CFormulaNode* InsertMMLFormulaNode_MPHANTOM( AST_TXML_Tag *node );
	CFormulaNode* InsertMMLFormulaNode_MROW( AST_TXML_Tag *node );
	CFormulaNode* InsertMMLFormulaNode_HIDDEN_EMPTY_SLOT( AST_TXML_Tag *node );
	CFormulaNode* InsertMMLFormulaNode_MERROR( QString message );
	CFormulaNode* InsertMMLFormulaNode_OTHERS( FXmlBtn *ptrFXmlBtn, AST_TXML_Tag *node, int is_hidden = 0 );
	CFormulaNode* InsertMMLFormulaNode_OTHERS( FXmlBtn *ptrFXmlBtn, AST_TXML_Compound *node, int is_hidden = 0 );
	CFormulaNode* InsertMMLFormulaNode_OTHERS_byID( const QString& _id, AST_TXML_Tag *node );
	CFormulaNode* InsertMMLFormulaNode_OTHERS_byMML( AST_TXML_Tag *node );
	CFormulaNode* InsertMMLFormulaNode_OTHERS_byMML( QString mml_name, AST_TXML_Compound *node );

	CFormulaNode* InsertMML_CM_semantics( enum TAG2MML tag_id, AST_TXML_Tag *xml_node );
	CFormulaNode* InsertMML_CM_annotation( enum TAG2MML tag_id, AST_TXML_Tag *xml_node );
	CFormulaNode* InsertMML_CM_annotation_xml( enum TAG2MML tag_id, AST_TXML_Tag *xml_node );
	CFormulaNode* InsertMML_CM_piecewise( enum TAG2MML tag_id, AST_TXML_Tag *xml_node );
	CFormulaNode* InsertMML_CM_standalone_piece( enum TAG2MML tag_id, AST_TXML_Tag *xml_node );
	CFormulaNode* InsertMML_CM_standalone_transparent( enum TAG2MML tag_id, AST_TXML_Tag *xml_node );
	CPlaneText* InsertMML_CM_standalone_opaque( enum TAG2MML tag_id, AST_TXML_Tag *xml_node );
	CPlaneText* InsertMML_CM_standalone_operator( enum TAG2MML tag_id, AST_TXML_Tag *xml_node, long newfstyle = FSTYLE_NONE );
	CFormulaNode* InsertMML_CM_standalone_diff_bvar( QString tagname, QString optext, AST_TXML_Tag *xml_node );
	void InsertMML_CM_standalone_diff_bvar_degree( QString tagname, QString optext, AST_TXML_Tag *xml_node, AST_TXML_Tag *degree_aspect = 0 );

	CFormulaNode* InsertMML_CM_standalone_operator_msup( enum TAG2MML tag_id, AST_TXML_Tag *xml_node );
	CFormulaNode* InsertMML_CM_construct_empty_vector( AST_TXML_Tag *xml_node );
	CFormulaNode* InsertMML_CM_construct_vector( enum TAG2MML tag_id, QString rows, AST_TXML_Tag *xml_node );

	CFormulaNode* InsertMML_CM_construct_set( enum TAG2MML tag_id, AST_TXML_Tag *xml_node );
	void InsertMML_CM_construct_set__parse( AST_TXML_Compound *_tags, int& is_x, 
		std::vector<AST_TXML_Tag*>& bvar_nodes, std::vector<AST_TXML_Tag*>& cond_nodes );
	CFormulaNode* InsertMML_CM_construct_list__constructive( enum TAG2MML tag_id, AST_TXML_Compound *_tags, QString _l, QString _r, QString _coma, QString _head, QString *comaList = 0, long comaListLen = -1, std::vector<AST_TXML_Tag*>* pBvarNodes = 0 );
	CFormulaNode* InsertMML_CM_construct_interval( enum TAG2MML tag_id, AST_TXML_Tag *xml_node );
	CFormulaNode* InsertMML_CM_construct_lamda( enum TAG2MML tag_id, AST_TXML_Tag *xml_node );
	CFormulaNode* InsertMML_CM_construct_lambda__domainofapplication( enum TAG2MML tag_id, AST_TXML_Compound *_tags, QString _head );
	CFormulaNode* InsertMML_CM_construct_declare( enum TAG2MML tag_id, AST_TXML_Tag *xml_node );

	void InsertMML_CM_create_markup_rigid_token( QString tag, QString name );

	void _CM_tune_Node_property( CNode *txt_node, AST_TXML_Tag *xml_node, long newfstyle = FSTYLE_NONE );
	void _CM_basic_Node_property( CNode *txt_node, AST_TXML_Tag *xml_node );
	
	CFormulaNode* InsertMMLContent_ci( AST_TXML_Tag *node );
	CFormulaNode* InsertMMLContent_csymbol( AST_TXML_Tag *node );
	CFormulaNode* InsertMMLContent_cn( AST_TXML_Tag *node );
	void InsertMMLContent__read_attributes( CNode *node, AST_TXML_Tag *xml_node, QString tagname, long is_content );
	CFormulaNode* InsertMMLContent_cn_simple( AST_TXML_Tag *node, QString base );
	CFormulaNode* InsertMMLContent_cn_rational( AST_TXML_Tag *node, QString base );
	CFormulaNode* InsertMMLContent_cn_complex_cartesian( AST_TXML_Tag *node, QString base );
	CFormulaNode* InsertMMLContent_cn_complex_polar( AST_TXML_Tag *node, QString base );

	CFormulaNode* InsertMML_CM_apply( enum TAG2MML tag_id, AST_TXML_Tag *xml_node );
	CFormulaNode* InsertMML_CM_apply__list( CCM_apply_args& _args );
	CFormulaNode* InsertMML_CM_apply__special( CCM_apply_args& _args );

	void		  InsertMML_CM_apply__read_arg( CLineNode *ln, long k, CCM_apply_args& _args );
	void		  InsertMML_CM_apply__read_op( AST_TXML_Tag *_item_node, QString _item_str, int read_only, long form );
	void		  InsertMML_CM_apply__read_op( CLineNode *lbuf, QString _item_str, int readonly, long form );

	CFormulaNode *createEmptyForm__exp( std::vector<CLineNode*>& to_read, CCM_apply_args& _args );
	CFormulaNode *createEmptyForm__pow( std::vector<CLineNode*>& to_read, CCM_apply_args& _args );
	CFormulaNode *createEmptyForm__inverse( std::vector<CLineNode*>& to_read, CCM_apply_args& _args );
	CFormulaNode *createEmptyForm__transpose( std::vector<CLineNode*>& to_read, CCM_apply_args& _args );
	CFormulaNode *createEmptyForm__conjugate( std::vector<CLineNode*>& to_read, CCM_apply_args& _args );
	CFormulaNode *createEmptyForm__abs( std::vector<CLineNode*>& to_read, CCM_apply_args& _args );
	CFormulaNode *createEmptyForm__floor( std::vector<CLineNode*>& to_read, CCM_apply_args& _args );
	CFormulaNode *createEmptyForm__ceiling( std::vector<CLineNode*>& to_read, CCM_apply_args& _args );
	CFormulaNode *createEmptyForm__mean( std::vector<CLineNode*>& to_read, CCM_apply_args& _args );
	CFormulaNode *createEmptyForm__divide( std::vector<CLineNode*>& to_read, CCM_apply_args& _args );
	CFormulaNode *createEmptyForm__quotient( std::vector<CLineNode*>& to_read, CCM_apply_args& _args );
	CFormulaNode *createEmptyForm__root( std::vector<CLineNode*>& to_read, CCM_apply_args& _args );
	CFormulaNode *createEmptyForm__log( std::vector<CLineNode*>& to_read, CCM_apply_args& _args );
	CFormulaNode *createEmptyForm__sum( std::vector<CLineNode*>& to_read, CCM_apply_args& _args, QString optext, QString optag, int isLargeOpAttr = 0 );
	int createEmptyForm__sum_parse( CCM_apply_args& _args );
	CFormulaNode *createEmptyForm__maxmin( CCM_apply_args& _args );
	CFormulaNode *createEmptyForm__moment( std::vector<CLineNode*>& to_read, CCM_apply_args& _args, int& moment_about );
	CFormulaNode *createEmptyForm__selector( std::vector<CLineNode*>& to_read, CCM_apply_args& _args );
	CFormulaNode *createEmptyForm__forall_exists( std::vector<CLineNode*>& to_read, CCM_apply_args& _args, QString op );
	int createEmptyForm__forall_exists_parse( CCM_apply_args& _args );
	CFormulaNode *createEmptyForm__calculus( std::vector<CLineNode*>& to_read, CCM_apply_args& _args );
	CFormulaNode *createEmptyForm__diff( std::vector<CLineNode*>& to_read, CCM_apply_args& _args );
	CFormulaNode *createEmptyForm__partialdiff( std::vector<CLineNode*>& to_read, CCM_apply_args& _args );
	CFormulaNode *createEmptyForm__diff_dxdt( 
		QString d_tag, QString d_text, std::vector<long>& bvar_idx_ar, 
		AST_TXML_Tag *degree, AST_TXML_Tag *degree_aspect,
		std::vector<CLineNode*>& to_read, CCM_apply_args& _args, int is_fenced );
};

/////////////////////////////////////////////////////////////////////////////
// class CFrameNode
/////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CFrameNode 
	: public CParentNode
{
protected:
	RealFde		m_lineSpacing;

public:
	CFrameNode();
	CFrameNode( CFrameNode* pNode );
	virtual ~CFrameNode();

	RealFde GetLineSpacing( void );
	void SetLineSpacing( RealFde linespacing );

public:

	virtual int Draw( CFmlDrawEngine& fde, const PointFde& tl, const RectFde& rc );

	virtual QString GetNodeText( void );
	virtual void RecalculateSize( int bRecalc );
	virtual long GetVisibleEdge( const RectFde &rc, const PointFde &tl, CNode *&pFirst, CNode *&pLast );

protected:
	void RecalculateSize_ReArrangeChildPos( SizeFde &sz );

public:
	virtual void read_mathml( AST_TXML_Tag *node );
	virtual void read_mathml( AST_TXML_Compound *_math_line );
	virtual QString to_mathml_pureContent( );
	virtual QString to_mathml_pureContent( long nLevel, long& nodesCount );
	virtual QString to_mathml( );
	virtual QString to_mathml( long nLevel, long& nodesCount );
	virtual QString to_mathml_4_html( long nLevel = 0, int is_block = 1 );
	void setMathML2StringVector( std::vector<QString>& dest, long level );

	int read_mathml( const QString& task, const QString& text );

public:
	static QString to_mathml_pureContent( QString txt );
	static QString to_mathml_pureContent( QString txt, long nLevel, long& nodesCount );
};

/////////////////////////////////////////////////////////////////////////////
// class CFormulaNode
/////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CFormulaNode
	: public CParentNode
	, public CNodeExInfo
	, public CFormulaExCalc
{
protected:
	QVector<qint32>	m_frame_corrector;

public:
	CFormulaNode();
	CFormulaNode( CFormulaNode *pFormulaNode );
	virtual ~CFormulaNode();

public:
	CNode* getFormulaDeepChild( long frame_idx, long child_idx = 0 );
	CFormulaNode* getFormulaDeepFormulaChild( long frame_idx, long child_idx = 0 );
	CLineNode* getFormulaLineChild( long frame_idx, long child_idx = 0 );
	CLineNode* getContentFormulaSingleLineChild();
	CNode* getSingleChild();
	CNode* getSingleChild( long frame_idx );
	CNode* getSingleFormulaChild( long frame_idx );
	CFormulaNode* getSingleFormulaChild_ContentEmptyFrame( long frame_idx );
	CPlaneText* getSingleFormulaChild_ContentText( long frame_idx );
	CNode* getSingleFormulaChild();
	CPlaneText* getSingleTextChild();
	CNode* getSingleContentFormulaChild();
	CFormulaNode* getRecursiveApplyOperand( bool lastChild, QString exceptId );
	long getUpperFormulaChildrenCount4Content();
	CFormulaNode* passAutodetectParent( CFormulaNode *upperLimit );
	CFormulaNode* passAutodetectParent_SingleFormulaChild( CFormulaNode *upperLimit );
	CFormulaNode* passParentApplyBrackets( CFormulaNode *upperLimit );
	CFormulaNode* passAutodetectSingleFormulaChild();
	CFormulaNode* passAutodetectSingleFormulaChildren();
	void adjustReadOnlyAttr( CLineNode *templateLine = NULL );
	void adjustReadOnlyAttr4Parent();
	static CFormulaNode* reorderFormulaBracketsTree( CFormulaNode *slot, CFormulaNode *pslot, CFormulaNode *sharedFormula, int shouldExchangeReadOnlyAttr );

	long GetChildFrameAlignType( long child_idx );
	void SetChildFrameAlignType( long child_idx, long t );

	int correctFrameIndent4TextPrimitive( CFrameNode *node );

public:
	void UpdateFormulaNode( const FBtn& btn );

public:
	virtual void Store( QDataStream& ar, SelectNode& sn, const std::vector<SelectNode>& bracketSn ) const;
	virtual void Load( QDataStream& ar, SelectNode& sn, std::vector<SelectNode>& bracketSn, int isLeftBrLoad );

	virtual int CopyNodeInfo( CNode* pNode );

	virtual void SetLevel( long nLevel );

	virtual int Draw( CFmlDrawEngine& fde, const PointFde& tl, const RectFde& rc );

	virtual int CreateGraphPrimIndirect( int nCount, CGraphPrimFormula *grpr );
	virtual int CreateGraphPrimIndirect( const QVector<CGraphPrimFormula>& srcGraphics );
	virtual int RunCalc();

	virtual RealFde getAscent( const SizeFde &sz );
	virtual void RecalculateSize( int bRecalc );

	void calculatePadded_Vertical( RealFde *destDim );
	void getChildDimensions_Vertical( RealFde *destDim );
	void getChildDimensions_Horizontal( RealFde *destDim );

	QString getApplyOp();
	virtual QString getMathMLNodeDescription();
	QString getMathMLNodeDescription_IterateParent();
	void setContentTokenParent_Transparent();
	CFormulaNode* getSemanticsOrCTokenNodeWrapped();
	virtual void setTextPropertyList( CNode_TextPropertyList& props, std::vector<char>& supports );

	void getPrecedence4Autodetect( short& precedence, short& assoc );
	CLineNode* CM_WrapIntoBracketsOnPrecedence();
	CLineNode* CM_WrapIntoBracketsOnPrecedence( short parent_precedence, short parent_assoc, int bNeedWrap, short position );
	void CM_WrapChildrenIntoBracketsOnPrecedence( CFrameNode *frameLeft, CFrameNode *frameRight );
	void CM_WrapChildrenIntoBracketsOnPrecedence();

	virtual QString to_mathml( long nLevel, long& nodesCount );
			QString to_mathml__template( long level, long& nodesCount );
			QString to_mathml__tagname( long level, long& nodesCount );
			QString to_mathml__table( long level, long& nodesCount );
			QString to_mathml__subsup( long level, long& nodesCount );
			QString to_mathml__tagname__MFRAC_getAttr( QString& sattr, long level );
			QString to_mathml__content( int& retry, long level, long& nodesCount );
			QString to_mathml__reference( long level, long& nodesCount, const QString& id );
			QString to_mathml__reference_interval( long level, long& nodesCount, CFormulaNode *intervalNode );
			QString to_mathml__content_semantics_literally( QString name, QString sattr, long level, long& nodesCount );
			QString to_mathml__content_semantics_pm( QString name, QString sattr, long level, long& nodesCount );
			QString to_mathml__content_apply_rule( QString name, QString sattr, long level, long& nodesCount );
			QString to_mathml__content_apply_addon_rule( QString name, QString sattr, long level, long& nodesCount );
			QString to_mathml__content_apply_addon_rule_backrecursive( QString name, QString sattr, long level, long& nodesCount );
			QString to_mathml__content_apply_momentabout_addon_rule( QString name, QString sattr, long level, long& nodesCount );
			QString to_mathml__content_set_rule( QString name, QString sattr, long level, long& nodesCount );
			QString to_mathml__content_recursive_tree( QString name, QString sattr, long level, long is_newline, long& nodesCount );
			QString to_mathml__content_backrecursive_tree( QString name, QString sattr, long level, long is_newline, long& nodesCount );
			QString to_mathml__content_matrix_body( long level );
			QString to_mathml__content_harvest_plaintext( long child_no, long& is_ok, long level, long& is_newline );
			void   to_mathml__content_xmlapply( long level );
			void   to_mathml__content_xmlapply_action_setproperty( AST_TXML_Tag* action, long level );

protected:
	void Recalculate_VerticalCenter( const SizeFde &sz );
};

/////////////////////////////////////////////////////////////////////////////
// CNode inline methods
/////////////////////////////////////////////////////////////////////////////

inline 
long CNode::GetType() const
	{ return m_nNodeType; }

inline 
long CNode::GetLevel() const
	{ return m_nLevel; }

inline 
CParentNode* CNode::GetParent() const
	{ return m_pParent; }

inline 
void CNode::SetParent( CParentNode* pParent )
	{ m_pParent = pParent; }

inline 
CNode* CNode::GetNext() const
	{ return m_pNext; }

inline 
void CNode::SetNext( CNode* pNode)
	{ m_pNext = (CNode*) pNode; }

inline 
CNode* CNode::GetPrev() const
	{ return m_pPrev; }

inline 
void CNode::SetPrev( CNode* pNode )
	{ m_pPrev = (CNode*) pNode; }

inline
CLineNode* CNode::GetLineNode( long *pnPos ) const
	{ return (CLineNode*) GetNodeFromType( NODE_LINE, pnPos ); }

inline
CFrameNode* CNode::GetFrameNode( long *pnPos ) const
	{ return (CFrameNode*) GetNodeFromType( NODE_FRAME, pnPos ); }


inline
RealFde CNode::getLeftIndent( void ) const
	{ return m_left; }
inline
RealFde CNode::getTopIndent( void ) const
	{ return m_top; }
inline
RealFde CNode::getRightIndent( void ) const
	{ return m_right; }
inline
RealFde CNode::getBottomIndent( void ) const
	{ return m_bottom; }
inline
void CNode::setLeftIndent( RealFde a )
	{ m_left = a; }
inline
void CNode::setTopIndent( RealFde a )
	{ m_top = a; }
inline
void CNode::setRightIndent( RealFde a )
	{ m_right = a; }
inline
void CNode::setBottomIndent( RealFde a )
	{ m_bottom = a; }
inline
void CNode::setIndents( RealFde l, RealFde t, RealFde r, RealFde b )
{
	m_left = l;
	m_top = t;
	m_right = r;
	m_bottom = b;
}
inline
void CNode::incLeftIndent( RealFde a )
	{ m_left += a; }
inline
void CNode::incTopIndent( RealFde a )
	{ m_top += a; }
inline
void CNode::incRightIndent( RealFde a )
	{ m_right += a; }
inline
void CNode::incBottomIndent( RealFde a )
	{ m_bottom += a; }

inline
void CNode::SetName( const QString& str)
	{ m_strName = str; }
inline
const QString& CNode::GetName( void )
	{ return m_strName; }

inline
RealFde& CNode::getPosition( long i )
	{ return m_aPosition[ i ]; }

inline
void CNode::getKernelPositionPtr( RealFde*& a )
	{ a = m_aPosition; }

inline 
void CNode::SetLevel( long nLevel )
	{ m_nLevel = nLevel; }

inline 
RealFde CNode::GetVerticalCenter( void )
	{ return m_vcenter; }

inline 
void CNode::SetVerticalCenter( RealFde vcenter )
	{ m_vcenter = vcenter; }

inline 
RectFde CNode::GetRect( void ) const
{
	return RectFde( 
		PointFde(m_aPosition[ SVG_NodeTemplate_Parameter_X ], m_aPosition[ SVG_NodeTemplate_Parameter_Y ]),
		PointFde(m_aPosition[ SVG_NodeTemplate_Parameter_X ] + m_aPosition[ SVG_NodeTemplate_Parameter_CX ],
		m_aPosition[ SVG_NodeTemplate_Parameter_Y ] + m_aPosition[ SVG_NodeTemplate_Parameter_CY ]) );
}

inline 
void CNode::SetCoord( long i, RealFde value )
	{ m_aPosition[ i ] = value; }

inline 
RealFde CNode::GetCoord( long i ) const
	{ return m_aPosition[ i ]; }

inline 
SizeFde CNode::GetSize( void ) const
{
	return SizeFde( 
		m_aPosition[ SVG_NodeTemplate_Parameter_CX ], 
		m_aPosition[ SVG_NodeTemplate_Parameter_CY ] );
}

inline 
void CNode::SetSize( const SizeFde& sz )
{
	m_aPosition[ SVG_NodeTemplate_Parameter_CX ] = sz.width();
	m_aPosition[ SVG_NodeTemplate_Parameter_CY ] = sz.height();
}

inline 
PointFde CNode::GetPosition( void ) const
{
	return PointFde( 
		m_aPosition[ SVG_NodeTemplate_Parameter_X ], 
		m_aPosition[ SVG_NodeTemplate_Parameter_Y ] );
}

inline
void CNode::SetPosition( const PointFde& pt )
{
	m_aPosition[ SVG_NodeTemplate_Parameter_X ] = pt.x();
	m_aPosition[ SVG_NodeTemplate_Parameter_Y ] = pt.y();
}

inline
int CNode::isContentTextOperator()
{
	return GetType() == NODE_PLANETEXT && to_mathml_data.is_content && isCannotBeEdited() && isCannotBeMoved();
}

inline
int CNode::isContentApply()
{
	return GetType() == NODE_FORMULA && to_mathml_data.is_content && ::mml_is_cm_apply( to_mathml_data.name );
}

/////////////////////////////////////////////////////////////////////////////
// CParentNode inline methods
/////////////////////////////////////////////////////////////////////////////

inline 
long CParentNode::GetNumLine( void )
	{ return m_nNumLine; }

inline 
void CParentNode::SetNumLine( long nNumLine )
	{ m_nNumLine = nNumLine; }

inline 
long CParentNode::GetNumColumn( void )
	{ return m_nNumColumn; }

inline
void CParentNode::SetNumColumn( long nNumColumn )
	{ m_nNumColumn = nNumColumn; }

inline 
long CParentNode::GetChildCount() const
	{ return (long) m_children.size(); }

inline 
CNode* CParentNode::GetFirstChild()
	{ return (GetChildCount() ? m_children[ 0 ] : NULL); }

inline 
CNode* CParentNode::GetLastChild()
	{ return (GetChildCount() ? m_children[ GetChildCount() - 1 ] : NULL); }

inline
CNode* CParentNode::GetChild( long no )
	{ return (no >= 0 && no < GetChildCount()) ? m_children[ no ] : NULL; }

inline
void CParentNode::AddChild( CNode *pNode )
	{ InsertChild( -1, pNode ); }

inline
void CParentNode::AddChildAfter( CNode* pNode, CNode* pAfterNode )
	{ InsertChild( GetChildIndex( pAfterNode ) + 1, pNode ); }

inline
void CParentNode::DeleteChild( CNode *pNode )
	{ DeleteChild( GetChildIndex( pNode ) ); }

/////////////////////////////////////////////////////////////////////////////
// CPlaneText inline methods
/////////////////////////////////////////////////////////////////////////////

inline
const FS_LogFont_Color& CPlaneText::getOtherStyleFont( void )
	{ return (m_plfc != NULL ? *m_plfc : ::getFontStyle( FSTYLE_VARIABLE )); }

inline 
long CPlaneText::GetStringLength( void )
	{ return (long) m_string.length(); }

inline 
void CPlaneText::SetString( const QString& lpszString )
	{ m_string = lpszString; }

inline 
const QString& CPlaneText::GetString( void ) const
	{ return m_string; }

inline 
void CPlaneText::Add( QChar ch )
	{ m_string += ch; }

inline 
void CPlaneText::Add( QString &str )
	{ m_string += str; }

inline 
void CPlaneText::Insert( long npos, QChar ch )
	{ m_string.insert( npos, ch ); }

inline 
void CPlaneText::Insert( long npos, QString &str )
	{ m_string.insert( npos, str ); }

inline 
void CPlaneText::Delete( long npos )
	{ m_string.remove( npos, 1 ); }

inline 
void CPlaneText::DeleteLast( void )
	{ Delete( m_string.length() - 1 ); }

inline 
int CPlaneText::isUnicode( void )
	{ return CNodeInfo_Property::isUnicode(); }

inline
int CPlaneText::isSingle( void )
	{ return !CNodeInfo::isAble2Glue() || !CNodeExInfo::isAble2Glue(); }

inline 
void CPlaneText::Glue( CPlaneText *pPlaneText )
	{ m_string += pPlaneText->GetString(); }

inline 
long CPlaneText::getFStyle( void ) const
	{ return m_nFStyleNo; }

//////////////////////////////////////////////////////////////////////

inline
FS_LogFont CPlaneText::getFont( void )
	{ return FS_LogFont( getNeedFont(), ::getCurrentFormulatorStyle().kegl2pixel( getTextKeglSize() ) ); }

//////////////////////////////////////////////////////////////////////

inline
RealFde CPlaneText::getTextKeglSize( void ) const
	{ return (m_kegl == NODE_PLANETEXT__POINT_SIZE_DEFAULT ? ::getKegl( GetLevel() ) : m_kegl ); }

/////////////////////////////////////////////////////////////////////////////
// CLineNode inline methods
/////////////////////////////////////////////////////////////////////////////

inline 
RealFde CLineNode::getMaxCYNotStretchy( void )
	{ return max_cy_notstretchy; }

inline 
int CLineNode::isGluing( CLineNode *pLineNode )
	{ return (canBeMoved() && pLineNode->canBeMoved() && canBeEdited() && pLineNode->canBeEdited()); }

/////////////////////////////////////////////////////////////////////////////
// CFrameNode inline methods
/////////////////////////////////////////////////////////////////////////////

inline
RealFde CFrameNode::GetLineSpacing( void )
	{ return m_lineSpacing; }

/////////////////////////////////////////////////////////////////////////////
// CFormulaNode inline methods
/////////////////////////////////////////////////////////////////////////////

inline 
long CFormulaNode::GetChildFrameAlignType( long child_idx )
{
	if( child_idx >= 0 && child_idx < GetSlotCount() )
		return GetSlot( child_idx ).GetAlignType();
	return 0;
}

inline 
void CFormulaNode::SetChildFrameAlignType( long child_idx, long t )
{
	if( child_idx >= 0 && child_idx < GetSlotCount() )
		GetSlot( child_idx ).SetAlignType( t );
}

inline 
int CFormulaNode::correctFrameIndent4TextPrimitive( CFrameNode *node )
	{ return correctFrameIndentByIndex( this, GetChildIndex( node ) ); }

/////////////////////////////////////////////////////////////////////////////

inline
long _CM_getPrecedence( CNode *node )
        { return node ? (long) node->to_mathml_data.precedence : (long) mmlPrecedence_Atom; }

inline
short _CM_getAssoc( CNode *node )
	{ return node ? node->to_mathml_data.assoc : 0; }

/////////////////////////////////////////////////////////////////////////////


__HERMITECH_FORMULIB__ RealFde calculateCurrentHeight( CNode *pNode, int ignoreIndent, long level, RealFde ascent );
__HERMITECH_FORMULIB__ RealFde calculateCurrentDepth( long level, RealFde ascent );
__HERMITECH_FORMULIB__ RealFde calculateCurrentTextAscent( long level );
__HERMITECH_FORMULIB__ RealFde calculateCurrentTextDescent( long level );

__HERMITECH_FORMULIB__ void _CM_apply_readApplyOperator( CParentNode *node, QString& out, long level, QString *opTagSeparated = 0 );
__HERMITECH_FORMULIB__ void _CM_apply_returnBackApplyOperator( CParentNode *node );
__HERMITECH_FORMULIB__ void getMMultiscriptsAttr( long nChildCoded, QString& buttonID, long& base, long& sw, long& nw, long& se, long& ne );

//=== NODE TREE ITERATORS
__HERMITECH_FORMULIB__ void mmlTreeIterator__mapUnicodeValue2EntityName( AST_TXML_Node *node, void *data );
void accountLinebreaks( CNode*, void* );
void write2log_MissButton( CNode*, void* );
void TRACE_setRecursiveLevel( CNode* node, void* arg );
void setRecursiveLevel( CNode* node, void* arg );
void setRecursiveReadOnly_n_IgnoreContent( CNode* node, void* /*arg*/ );
void clearRecursiveReadOnly_n_IgnoreContent( CNode* node, void* /*arg*/ );
void setRecursiveReadOnly( CNode* node, void* /*arg*/ );
void clearRecursiveReadOnly( CNode* node, void* /*arg*/ );
void setRecursiveIgnoreContent( CNode* node, void* /*arg*/ );
void clearRecursiveIgnoreContent( CNode* node, void* /*arg*/ );
void setIgnoreContent( CNode* node, void* /*arg*/ );
void setRecursiveCannotEdit( CNode* node, void* /*arg*/ );
void clearRecursiveCannotEdit( CNode* node, void* /*arg*/ );
void setRecursiveImmovable( CNode* node, void* /*arg*/ );
void clearRecursiveImmovable( CNode* node, void* /*arg*/ );
void search4ContentChildren( CNode* node, void* arg );

/////////////////////////////////////////////////////////////////////////////

inline
AST_TXML_Compound* CompileMathML( const QString& strTask, const QString& strText )
{
	TXML_Storage* bms = NULL;
	DRMessageManager dmm;
	long dmm_page = 0;

	if( ::LS_CompileXML( strTask, strText, bms, dmm, dmm_page ) && bms != NULL )
	{
		delete bms;
		bms = NULL;
	}

	return bms;
}

/////////////////////////////////////////////////////////////////////////////

#endif//__FORMULATOR_NODE_H__
