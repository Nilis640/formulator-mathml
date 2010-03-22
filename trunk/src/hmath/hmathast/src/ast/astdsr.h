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

#if !defined( __PARSEDSR_CDsrAST_H__ )
#define __PARSEDSR_CDsrAST_H__

#include <vector>

#include "HUtils/ixml.h"
#include "HUtils/ihkstring.h"
#include "HUtils/ihkarray.h"
#include "../hmathast_global.h"
#include "../type/typetable.h"
#include "../symbol/symbol_table_dsr.h"

#define CompoundID_CONTEXT__NONE		-1
#define CompoundID_CONTEXT__SINGLE		0
#define CompoundID_CONTEXT__TYPESPEC	1
#define CompoundID_CONTEXT__FUNCTION	2

enum DsrAST_Types
{
	DsrASTT_None = 0,

	DsrASTT_StmtList,
	DsrASTT_Compound,
	DsrASTT_ArgList,
	DsrASTT_NullStmt,

	DsrASTT_If,

	DsrASTT_ConstInt,
	DsrASTT_ConstReal,
	DsrASTT_ConstString,
	DsrASTT_ConstStringList,

	DsrASTT_Expr,
	DsrASTT_Function,
//	DsrASTT_IdxList,
	DsrASTT_IdxListItem,
	DsrASTT_CompoundID,
	DsrASTT_PrimaryID,
	DsrASTT_Special,

	DsrASTT_TypeSpec,

	DsrASTT_ObjDef_Assign,
	DsrASTT_ObjDef_Compound,
	DsrASTT_ObjHead,
	DsrASTT_ObjArg,

	DsrASTT_Keyword,
	DsrASTT_Op,
	DsrASTT_Separator,

	DsrASTT_Remark,
	DsrASTT_Blank,
	DsrASTT_0d0a,
	DsrASTT_unknown,

	DsrASTT_Last
};

extern QString DsrAST_Types_Name[ DsrASTT_Last ];

class CDsrAST_Node;
class CDsrAST_StmtList;
class CDsrAST_Compound;
class CDsrAST_ArgList;
class CDsrAST_NullStmt;
class CDsrAST_If;
class CDsrAST_ConstInt;
class CDsrAST_ConstReal;
class CDsrAST_ConstString;
class CDsrAST_ConstStringList;
class CDsrAST_Expr;
class CDsrAST_Function;
//class CDsrAST_IdxList;
class CDsrAST_IdxListItem;
class CDsrAST_CompoundID;
class CDsrAST_PrimaryID;
class CDsrAST_Special;
class CDsrAST_TypeSpec;
class CDsrAST_ObjDef_Assign;
class CDsrAST_ObjDef_Compound;
class CDsrAST_ObjHead;
class CDsrAST_ObjArg;
class CDsrAST_Keyword;
class CDsrAST_Op;
class CDsrAST_Separator;
class CDsrAST_Remark;
class CDsrAST_Blank;
class CDsrAST_0d0a;
class CDsrAST_unknown;

class VM_Dsr;
class CDSR_VMEval;
class CDSR_VMEval_Line;

/////////////////////////////////////////
//	Node		:	;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_Node
{
//protected:
public:
	CDsrAST_StmtList *before_char, *after_char;	// plain characters before and after the node
	enum DsrAST_Types type;
	long line;
	long level;
	CDsrDataType_Node *data_type;
	long run_time_stack_overhead;
public:
	CDsrAST_Node( void );
	virtual ~CDsrAST_Node( void );
	enum DsrAST_Types getType( void );
	void AddBefore( CDsrAST_Node* n );
	void AddAfter( CDsrAST_Node* n );
	void setLine( long ln );
	long getLine( void );
	void setLevel( long lv );
	long getLevel( void );
	void setDataType( CDsrDataType_Node* lv );
	CDsrDataType_Node* getDataType( void );
	virtual long getRTStackOverhead( void );
	virtual void setAddrScheme4LValue( CParseDsrSymbolTable& smbtable );
	virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long ln = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void getBeforeInstruction( VM_Dsr& vm );
	virtual void getAfterInstruction( VM_Dsr& vm );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void convertArgVar2Polynomial( CParseDsrSymbolTable& smbtable, long max_num_arg );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
};

/////////////////////////////////////////
//	StmtList	:	Node*;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_StmtList : public CDsrAST_Node
{
public:
	MIArray<CDsrAST_Node> stmt;

	CDsrAST_StmtList( void );
	virtual ~CDsrAST_StmtList( void );
	int Add( CDsrAST_Node *node );
	void Join( CDsrAST_StmtList *listNode );
	int AddLeft( CDsrAST_Node *node );
	virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long ln = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void convertArgVar2Polynomial( CParseDsrSymbolTable& smbtable, long max_num_arg );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable, bool ignoreSemanticError = false );
};

/////////////////////////////////////////
//	Compound	:	Keyword, StmtList, Keyword;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_Compound : public CDsrAST_Node
{
public:
	CDsrAST_StmtList *stmt;
	CDsrAST_Keyword	*k_begin, *k_end;

	CDsrAST_Compound( CDsrAST_StmtList *astmt = 0, CDsrAST_Keyword *ak_begin = 0, CDsrAST_Keyword	*ak_end = 0 );
	virtual ~CDsrAST_Compound( void );
	virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long ln = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void convertArgVar2Polynomial( CParseDsrSymbolTable& smbtable, long max_num_arg );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
};

/////////////////////////////////////////
//	ArgList		:	Expr*, Separator;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_ArgList : public CDsrAST_Node
{
public:
	MIArray<CDsrAST_Node> expr;
	CDsrAST_Separator	*sep;
	bool is_recursive_delete;

	CDsrAST_ArgList( CDsrAST_Separator *asep = 0 );
	void setRecursiveDeleteStatus( bool rd );
	virtual ~CDsrAST_ArgList( void );
	int Add( CDsrAST_Node *node );
	void Add( MIArray<CDsrAST_Node>& expr );
	void setSeparator( CDsrAST_Separator *asep );
	long getArgNumber( void );
	CDsrAST_Separator* getSeparator( void );
	virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long lv = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void convertArgVar2Polynomial( CParseDsrSymbolTable& smbtable, long max_num_arg );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
	void convertArgTypes( const __CDSRTypeConversionOpVector& opc, CParseDsrSymbolTable& smbtable );
	long convertArgTypes( CParseDsrSymbolTable& smbtable, long idx, enum CDsrFunctionListID opc );
};

/////////////////////////////////////////
//	NullStmt	:	Separator;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_NullStmt : public CDsrAST_Node
{
public:
	CDsrAST_Separator	*sep;

	CDsrAST_NullStmt( CDsrAST_Separator	*asep = 0 );
	virtual ~CDsrAST_NullStmt( void );
	//virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long lv = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
};

/////////////////////////////////////////
//	If			:	Keyword, Expr, Keyword, Node, Keyword, Node;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_If : public CDsrAST_Node
{
public:
	CDsrAST_Node	*n_expr;
	CDsrAST_Node	*n_then, *n_else;
	CDsrAST_Keyword	*k_if, *k_then, *k_else;

	CDsrAST_If( CDsrAST_Node* _expr = 0,
				CDsrAST_Node* _then = 0,
				CDsrAST_Node* _else = 0, 
				CDsrAST_Keyword	*_k_if = 0, 
				CDsrAST_Keyword *_k_then = 0, 
				CDsrAST_Keyword *_k_else = 0 );
	virtual ~CDsrAST_If( void );
			void setElseNode( CDsrAST_Node* _else );
	virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long lv = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void convertArgVar2Polynomial( CParseDsrSymbolTable& smbtable, long max_num_arg );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
};

/////////////////////////////////////////
//	ConstInt	:	long;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_ConstInt : public CDsrAST_Node
{
public:
	long value;
	QString aspect;

	CDsrAST_ConstInt( long _value = 0, QString _aspect = _T("") );
	virtual ~CDsrAST_ConstInt( void );
	QString getAspect( void );
	//virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
};

/////////////////////////////////////////
//	ConstReal	:	double;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_ConstReal : public CDsrAST_Node
{
public:
	double value;
	QString aspect;

	CDsrAST_ConstReal( double _value = 0.0, QString _aspect = _T("") );
	virtual ~CDsrAST_ConstReal( void );
	QString getAspect( void );
	//virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
};

/////////////////////////////////////////
//	ConstString	:	QString;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_ConstString : public CDsrAST_Node
{
public:
	QString value;

	CDsrAST_ConstString( QString _value = _T("") );
	void append( const QString& );
	QString getAspect( void );
	virtual ~CDsrAST_ConstString( void );
	//virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
};

/////////////////////////////////////////
//	CDsrAST_ConstStringList : ConstString*;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_ConstStringList : public CDsrAST_Node
{
public:
	MIArray<CDsrAST_ConstString> stmt;

	CDsrAST_ConstStringList( void );
	virtual ~CDsrAST_ConstStringList( void );
	int Add( CDsrAST_ConstString *node );
	//virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long ln = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
};

/////////////////////////////////////////
//	Expr		:	Node, Op, Node;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_Expr : public CDsrAST_Node
{
public:
	CDsrAST_Op* op;
	CDsrAST_ArgList *args;
	enum CDsrFunctionListID fun_id;
	long fun_id_param;
	MArray<long> m_used_xvar, m_used_dxvar;
	int is_koshi;

	CDsrAST_Expr( CDsrAST_Op *_op = 0, CDsrAST_Node *_arg1 = 0, CDsrAST_Node *_arg2 = 0 );
	virtual ~CDsrAST_Expr( void );
	virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long lv = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
			void getInstruction_equation( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void convertArgVar2Polynomial( CParseDsrSymbolTable& smbtable, long max_num_arg );
	virtual void setAddrScheme4LValue( CParseDsrSymbolTable& smbtable );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
			int placeDataType_assign( CParseDsrSymbolTable& smbtable, CDsrDataType_Node*& left_arg_fun_node );
			int placeDataType_square_brackets( CParseDsrSymbolTable& smbtable );
			int placeDataType_type_spec( CParseDsrSymbolTable& smbtable, CParseDsrSymbol *smb );
			void reportTypeError( CParseDsrSymbolTable& smbtable );
};

/////////////////////////////////////////
//	Function	:	CompoundID, Separator, ArgList, Separator;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_Function : public CDsrAST_Node
{
public:
	CDsrAST_CompoundID* name;
	CDsrAST_Separator *lb, *rb;
	CDsrAST_ArgList *args;
	enum CDsrFunctionListID fun_id;
	long fun_id_param;

	CDsrAST_Function( CDsrAST_CompoundID* _name = 0, CDsrAST_Separator *_lb = 0, CDsrAST_Separator *_rb = 0, CDsrAST_ArgList *_arg = 0 );
	virtual ~CDsrAST_Function( void );
	virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long lv = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void convertArgVar2Polynomial( CParseDsrSymbolTable& smbtable, long max_num_arg );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
			void placeDataType_args( CParseDsrSymbolTable& smbtable, CDsrDataType_Composition *arg_type );
			void reportTypeError( CParseDsrSymbolTable& smbtable, CParseDsrSymbol *smb );
};

/////////////////////////////////////////
//	IdxListItem	:	Separator, ArgList, Separator;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_IdxListItem : public CDsrAST_Node
{
public:
	CDsrAST_Separator *lb, *rb;
	CDsrAST_Node *expr;

	CDsrAST_IdxListItem( CDsrAST_Separator *alb = 0, CDsrAST_Node *aexpr = 0, CDsrAST_Separator *arb = 0 );
	virtual ~CDsrAST_IdxListItem( void );
	virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long lv = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
};

/////////////////////////////////////////
//	CompoundID	:	PrimaryID*, Separator;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_CompoundID : public CDsrAST_Node
{
public:
	MIArray<CDsrAST_PrimaryID> stmt;
	CDsrAST_Separator *sep;

	CDsrAST_CompoundID( CDsrAST_Separator *asep = 0 );
	virtual ~CDsrAST_CompoundID( void );
	CParseDsrSymbol* getSymbol( CParseDsrSymbolTable& smbtable );
	void setContext( long ct );
	void setSeparator( CDsrAST_Separator *asep );
	CDsrAST_Separator* getSeparator( void );
	int Add( CDsrAST_PrimaryID *node );
	virtual void setAddrScheme4LValue( CParseDsrSymbolTable& smbtable );
	virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long lv = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
};

/////////////////////////////////////////
//	PrimaryID	:	long, Special;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_PrimaryID : public CDsrAST_Node
{
public:
	CParseDsrSymbolTable *local_smbtable;	// local smb table if non-zero
	long id;		// name id in the symbol table
	CDsrAST_Special* spec;
	long context;	// one of: CompoundID_CONTEXT__NONE     - just name
					//         CompoundID_CONTEXT__SINGLE   - just name
					//         CompoundID_CONTEXT__TYPESPEC - type name
					//         CompoundID_CONTEXT__FUNCTION - name(...)
	int is_push_by_address;	// push this instance of the Variable by address (not by value)

	CDsrAST_PrimaryID( long _id = -1, CDsrAST_Special *_spec = 0 );
	virtual ~CDsrAST_PrimaryID( void );
	CParseDsrSymbol* getSymbol( CParseDsrSymbolTable& smbtable );
	long getContext( void );
	int isLocalSmbTable( void );
	virtual void setAddrScheme4LValue( CParseDsrSymbolTable& smbtable );
	virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long lv = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
};

/////////////////////////////////////////
//	Special		:	PrimaryID, Separator, ArgList, Separator;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_Special : public CDsrAST_Node
{
public:
	CDsrAST_PrimaryID *name;
	CDsrAST_Separator *lb, *rb;
	CDsrAST_ArgList *args;
	enum CDsrFunctionListID fun_id;
	long fun_id_param;

	CDsrAST_Special( CDsrAST_PrimaryID *_name = 0, CDsrAST_Separator *_lb = 0, CDsrAST_Separator *_rb = 0, CDsrAST_ArgList *_arg = 0 );
	virtual ~CDsrAST_Special( void );
	CParseDsrSymbol* getSymbol( CParseDsrSymbolTable& smbtable );
	virtual void setAddrScheme4LValue( CParseDsrSymbolTable& smbtable );
	virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long lv = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void convertArgVar2Polynomial( CParseDsrSymbolTable& smbtable, long max_num_arg );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
			void placeDataType_args( CParseDsrSymbolTable& smbtable, CDsrDataType_Composition *arg_type );
			void reportTypeError( CParseDsrSymbolTable& smbtable, CParseDsrSymbol *smb );
			void passArgList2Out( CParseDsrSymbolTable& smbtable );
			int placeDataType_prime( CParseDsrSymbolTable& smbtable, CDsrDataType_Composition *arg_type, CParseDsrSymbol *smb );
			CDsrDataType_Node* placeDataType_create_table( 
									CDsrDataType_Node* /*op_type*/, 
									//enum CDsrFunctionListID& fun_id,
									long& fun_id_param,
									CDsrDataType_Composition* /*args_type*/,
									std::vector<__CDSRTypeConversionOpVector>& __opc,
									long& cb,
									CParseDsrSymbolTable& smbtable,
									CParseDsrSymbol *smb );
};

/////////////////////////////////////////
//	TypeSpec	:	Op, PrimaryID, Separator, ArgList, Separator;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_TypeSpec : public CDsrAST_Node
{
public:
	CDsrAST_Op *op;
	CDsrAST_PrimaryID *name;
	CDsrAST_Separator *lb, *rb;
	CDsrAST_ArgList *args;

	CDsrAST_TypeSpec( CDsrAST_Op *_op = 0, CDsrAST_PrimaryID *_name = 0, CDsrAST_Separator *_lb = 0, CDsrAST_Separator *_rb = 0, CDsrAST_ArgList *_arg = 0 );
	virtual ~CDsrAST_TypeSpec( void );
	virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long lv = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
};

/////////////////////////////////////////
//	ObjDef_Assign	:	ObjHead, Op, Expr, Separator;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_ObjDef_Assign : public CDsrAST_Node
{
public:
	CDsrAST_ObjHead *head;
	CDsrAST_Op *op;
	CDsrAST_Node *expr;
	CDsrAST_Separator *sep;
	CParseDsrSymbolTable *local_smbtable;	// local smb table if non-zero
	enum CDsrFunctionListID fun_id;
	long fun_id_param;

	CDsrAST_ObjDef_Assign( CDsrAST_ObjHead *_head = 0, CDsrAST_Op *_op = 0, CDsrAST_Node *_expr = 0, CDsrAST_Separator *_sep = 0, CParseDsrSymbolTable *_local_smbtable = 0 );
	virtual ~CDsrAST_ObjDef_Assign( void );
	virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long lv = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
			void reportTypeError( CParseDsrSymbolTable& smbtable, CParseDsrSymbol *smb, CDsrDataType_Node *ret_type );
};

/////////////////////////////////////////
//	ObjDef_Compound	:	ObjHead, Compound;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_ObjDef_Compound : public CDsrAST_Node
{
public:
	CDsrAST_ObjHead *head;
	CDsrAST_Compound *compound;
	CParseDsrSymbolTable *local_smbtable;	// local smb table if non-zero

	CDsrAST_ObjDef_Compound( CDsrAST_ObjHead *_head = 0, CDsrAST_Compound *_compound = 0, CParseDsrSymbolTable *_local_smbtable = 0 );
	virtual ~CDsrAST_ObjDef_Compound( void );
	virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long lv = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
};

/////////////////////////////////////////
//	ObjHead			:	Keyword, PrimaryID, ObjArg;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_ObjHead : public CDsrAST_Node
{
public:
	CDsrAST_Keyword *keyword;
	CDsrAST_PrimaryID *name;
	CDsrAST_ObjArg *args;

	CDsrAST_ObjHead( CDsrAST_Keyword *_keyword = 0, CDsrAST_PrimaryID *_name = 0, CDsrAST_ObjArg *_args = 0 );
	virtual ~CDsrAST_ObjHead( void );
	CParseDsrSymbol* getSymbol( CParseDsrSymbolTable& smbtable );
	virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long lv = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
};
	
/////////////////////////////////////////
//	ObjArg			:	Separator, ArgList, Separator, TypeSpec;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_ObjArg : public CDsrAST_Node
{
public:
	CDsrAST_Separator *lb, *rb;
	CDsrAST_ArgList *args;
	CDsrAST_TypeSpec *typespec;

	enum CDsrDataTypes keyword_type;	// one of: group, polynomial, rational function, function

	CDsrAST_ObjArg( CDsrAST_Separator *_lb = 0, CDsrAST_Separator *_rb = 0, CDsrAST_ArgList *_name = 0, CDsrAST_TypeSpec *_args = 0 );
	virtual ~CDsrAST_ObjArg( void );
	void setTypeSpec( CDsrAST_TypeSpec *aargs );
	void setKeywordType( enum CDsrDataTypes tp );
	enum CDsrDataTypes getKeywordType( void );
	virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long lv = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
			void placeDataType_args( CParseDsrSymbolTable& smbtable, CDsrDataType_Node *type_spec );
};

/////////////////////////////////////////
//	Keyword		:	long;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_Keyword : public CDsrAST_Node
{
public:
	long id;		// keyword id in the symbol table
	enum CDsrDataTypes kw_type;

	CDsrAST_Keyword( long _id = -1, enum CDsrDataTypes tp = DSRDATA_TYPE_NONE );
	virtual ~CDsrAST_Keyword( void );
	CParseDsrSymbol* getSymbol( CParseDsrSymbolTable& smbtable );
	enum CDsrDataTypes getKeywordType( void );
	//virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long lv = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
};

/////////////////////////////////////////
//	Op			:	long;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_Op : public CDsrAST_Node
{
public:
	long id;		// named constants from "common\language\dsr\operator\operator.h"
	long object_id;	// id of parent function if it exists

	CDsrAST_Op( long _id = -1 );
	CDsrAST_Op( long _id, long _line );
	virtual ~CDsrAST_Op( void );
	CParseDsrSymbol* getSymbol( CParseDsrSymbolTable& smbtable );
	CParseDsrSymbol* getObjectContextSmb( CParseDsrSymbolTable& smbtable );
	long getObjectContext( CParseDsrSymbolTable& smbtable );
	virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long lv = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
};
	
/////////////////////////////////////////
//	Separator	:	long;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_Separator : public CDsrAST_Node
{
public:
	long id;	// index of the separator in terms of the grammar lexem numbers

	CDsrAST_Separator( long _id = -1 );
	virtual ~CDsrAST_Separator( void );
	//virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long lv = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
};
	
/////////////////////////////////////////
//	Remark		:	QString;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_Remark : public CDsrAST_Node
{
public:
	QString value;

	CDsrAST_Remark( QString value = _T("") );
	virtual ~CDsrAST_Remark( void );
	//virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long lv = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
};
	
/////////////////////////////////////////
//	Blank		:	QString;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_Blank : public CDsrAST_Node
{
public:
	QString value;

	CDsrAST_Blank( QString value = _T("") );
	virtual ~CDsrAST_Blank( void );
	//virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long lv = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
};

/////////////////////////////////////////
//	0d0a		:	;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_0d0a : public CDsrAST_Node
{
public:
	QString value;

	CDsrAST_0d0a( QString value = _T("") );
	virtual ~CDsrAST_0d0a( void );
	//virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long lv = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
};
	
/////////////////////////////////////////
//	unknown		:	QString;
/////////////////////////////////////////
class __HMATHLNGDLL__ CDsrAST_unknown : public CDsrAST_Node
{
public:
	QString value;

	CDsrAST_unknown( QString value = _T("") );
	virtual ~CDsrAST_unknown( void );
	//virtual void setObjectContextThroughTree( long o_id = -1 );
	virtual void setLevelThroughTree( long lv = 0 );
	virtual void getInstruction( VM_Dsr& vm );
	virtual void getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest );
	virtual void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable, long call_level );
	virtual void placeDataType( CParseDsrSymbolTable& smbtable );
};

///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
long getFunctionArgMatch( long op_id, CDsrDataType_Function *funt, 
						  CDsrDataType_Composition *argt, 
						  CDsrDataType_Node*& result, 
						  __CDSRTypeConversionOpVector& type_conversion );
__HMATHLNGDLL__
long getFunctionArgMatch( long op_id, CDsrDataType_Composition *funt, 
						  CDsrDataType_Composition *argt, 
						  CDsrDataType_Node*& result, 
						  std::vector<__CDSRTypeConversionOpVector>& opc, long& _cb );
__HMATHLNGDLL__
CDsrDataType_Node* CompareOp2Args(	long op_id, CDsrDataType_Node* op_type, 
									//enum CDsrFunctionListID& fun_id,
									long& fun_id_param,
									CDsrDataType_Composition* args_type,
									std::vector<__CDSRTypeConversionOpVector>& opc,
									long& cb,
									CParseDsrSymbolTable& smbtable,
									CParseDsrSymbol *smb );

__HMATHLNGDLL__
enum CDsrDataTypes convert2MaxScalarType( CParseDsrSymbolTable& smbtable, CDsrAST_ArgList *args );

__HMATHLNGDLL__
CDsrDataType_Node* correctOpDataType2New( CParseDsrSymbolTable& smbtable, long new_fun_id, CDsrAST_Op *op );

///////////////////////////////////////////////////////////////////////////////

typedef CDsrAST_StmtList CDsrASTStorage;

QString getCDsrASTNodeName( CDsrAST_Node *node );

#endif
