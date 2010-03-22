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

#if !defined( __PARSEDSR_VM_VMIEVAL_H__ )
#define __PARSEDSR_VM_VMIEVAL_H__

#include <vector>

#include "../ast/astdsr.h"
#include "vmidsr.h"
#include "vm_eval_cmd.h"
#include "vm_eval_equ.h"
#include "vm_eval_fun.h"

#define VMEval_CODE_SEGMENT_ID__MAIN		0
#define VMEval_CODE_SEGMENT_ID__FUNCTION	1
#define VMEval_CODE_SEGMENT_ID__EQUATION_L	2
#define VMEval_CODE_SEGMENT_ID__EQUATION_R	3
#define VMEval_CODE_SEGMENT_ID__RESERVED	4

class __HMATHLNGDLL__ CDSR_VMEval : public VM_Dsr
{
public:

	int is_error;						// error flag

	CDSR_VMEval_Line	m_cmd_table;	// main procedure body
	CDSR_VMEval_FT		m_fun_table;	// table of user defined functions
	CDSR_VMEval_SE		m_equ_table;	// system of equations

	CDSR_VMEval_Line	m_reserved4call;// reserved for functions call outside of the main procedure body

	long m_context;						// main body, function, equation or reserved body
	long m_context_no;					// number of function or equation if context is for them
	MArray<long>		m_var_id_table;	// variable id's (in terms of Symbol Table)

	CDSR_VMEval_Line	*m_IP_base_ptr;		// pointer to the next instruction segment
	long				m_IP;			// pointer to the next instruction
	UniWordVector		m_var_table;	// static variables
	UniWordVector		m_run_stack;	// run stack
	long				m_run_stack_ptr;// stack pointer
	UniWordVector		m_stack;		// stack
	long				m_SP;			// stack pointer
	long				m_BP;			// base pointer -> SS[ BP ] - 1st local, SS[ BP - 1 ] - 2nd local etc.

public:

	// construction
	CDSR_VMEval( CParseDsrSymbolTable*& _ptr_symbol_table, long init_mode = VM_Dsr_MODE__BINCODE );
	virtual ~CDSR_VMEval();
	void InitVarTable( void );
	void InitFunTable( void );
	//////////////////////////

	// service
	int isError( void );
	void setError( void );
	void resetError( void );
	//////////////////////////

	// generating code
	void getInstruction( CDsrAST_Node* root );
	void Equation_AddPrimeIndex( long idx );
	//////////////////////////

	// equation
	long getEquation_Number( void );
	long getEquation_UnknownNumber( void );
	enum CDsrDataTypes getEquation_Type( void );
	int getEquation_isLinear( void );
	int getEquation_isAlgebraic( void );
	int getEquation_isDifferential( void );
	int getEquation_isKoshi( void );
	void getEquation_getXVariableNameList( std::vector<QString>& dest );
	UniWordVector& getEquation_LinearA( void );
	UniWordVector& getEquation_LinearB( void );
	int InitSLAU_AB( void );
	int getEquation_getXVar( long x_var_order_no, UniWord& value );
	int getEquation_setXVar( long x_var_order_no, UniWord& value );
	int getEquation_getLeftKoshiVariable( long eq_no, long& var_id, long& var_idx );
	//=== ODE
	int getEquation_setInitialXVar( long eq_no, UniWord& value );
	int getEquation_getTimeVar( UniWord& value );
	int getEquation_setTimeVar( UniWord& value );
	//////////////////////////

	// function
	int getFunction_Begin( void );
	int getFunction_End( void );
	long getFunction_Number( void );
	QString getFunction_getName( long fun_no );
	long getFunction_getNoByName( QString name );
	long getFunction_getArgNumber( long fun_no );
	long getFunction_getResNumber( long fun_no );
	CDsrDataType_Node* getFunction_getArgType( long fun_no );
	CDsrDataType_Node* getFunction_getArgType( long fun_no, long arg_no );
	CDsrDataType_Node* getFunction_getResType( long fun_no );
	int getFunction_setArg( long fun_no, long arg_no, UniWord& value );
	int getFunction_getRes( long fun_no, UniWord& res_value, long res_no = 0 );
	CDSR_VMEval_Line* getFunction_CmdLine( long fun_no );
	//////////////////////////

	// variables
	long getVariable_getNoByName( QString name );
	long getVariable_getNoByID( long id );
	long getVariable_getSmbID( long var_no );
	int getVariable_getType( long no, enum CDsrDataTypes& type );
	QString getVariable_Name( long no );
	int getVariable_Number( void );
	int getVariable_Begin( void );
	int getVariable_End( void );
	int getVariable_getValue( long var_no, UniWord& value );
	int getVariable_getValue( QString varname, UniWord& value );
	int getVariable_setValue( long var_no, UniWord& value );
	int getVariable_setValue( QString varname, UniWord& value );
	int getVariable_getVectorValue( long var_no, long var_idx, UniWord& value );
	int getVariable_setVectorValue( long var_no, long var_idx, UniWord& value );
	//////////////////////////

	// print values
	void printOneVariableValue2Text( QString& dest, long var_no = -1, int show_reserved = 0 );
	void printVariableValue2Text( QString& dest, long var_no = -1, int show_reserved = 0 );
	void printOneVariableValue2Formulator( QString& dest, long var_no = -1, int show_reserved = 0 );
	void printVariableValue2Formulator( QString& dest, long var_no = -1, int show_reserved = 0 );
	//////////////////////////

	// run-time execution
	int initExecution( CDSR_VMEval_Line& initline );
	int execCommand( void );
	int execCommand( CDSR_VMEval_Cmd& call );
	int execFunction( long fun_no, UniWordVector& res, const UniWordVector& args );
	int execFunction( QString fun_name, UniWordVector& res, const UniWordVector& args );
	int execEquationLeft( long equ_no, UniWord *retvalue = 0 );
	int execEquationRight( long equ_no, UniWord *retvalue = 0 );
	//////////////////////////

	// run stack
	void push2RunStack( UniWord& x );
	//////////////////////////

	// diagnostics
	//QString getMathErrorMessage( CDSRMathErr& math_err );
	//////////////////////////
};

///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__ QString getTextView( CParseDsrSymbolTable& smbtable, CDSR_VMEval& c );

///////////////////////////////////////////////////////////////////////////////

#endif
