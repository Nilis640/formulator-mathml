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

#if !defined( __HMATH_CMMCALC_CMMCALC__ )
#define __HMATH_CMMCALC_CMMCALC__

///////////////////////////////////////////////////////////////////////////////

#include <QVector>
#include <QString>

#include <vector>

#include "HUtils/ihkstring.h"
#include "HUtils/ihkmap.h"
#include "HUtils/imessage.h"
#include "HUtils/iustring.h"
#include "HUtils/ixml.h"

#include "../mminfo/mm_domain.h"
#include "../symbol/smbrecord.h"

#include "cmmlCalcTag.h"
#include "cmmlCalcErr.h"

///////////////////////////////////////////////////////////////////////////////

#define CMMCALC_INTERNAL_VAR_NAME_LEN 64

inline
QString generateInternalVarName()
{
	return _T("$") + ::generateTextSeed( CMMCALC_INTERNAL_VAR_NAME_LEN );
}

inline
int isInternalVarName( const QString& name )
{
	return (name.length() > CMMCALC_INTERNAL_VAR_NAME_LEN) && name[ 0 ] == _T('$');
}

///////////////////////////////////////////////////////////////////////////////

class CDsrDataType_Node;
class CParseDsrSymbol;
class CParseDsrSymbolTable;
class CDsrAST_Node;
class CDsrAST_StmtList;
class CDSR_VMEval;

///////////////////////////////////////////////////////////////////////////////

class __HMATHLNGDLL__ CMMLCalc
{
protected:
	QString m_taskName;
	DRMessageManager& m_dmm;
	long m_dmmPage;

	CParseDsrSymbolTable* m_dsrSymbolTable;
	CParseDsrSymbolTable* m_localSymbolTable;

	CDsrAST_StmtList* m_ast;
	CDSR_VMEval *m_vm;

	QVector<QString> m_var2mml;

public:
	CMMLCalc( DRMessageManager& dmm, long dmmPage, QString taskName );
	~CMMLCalc();

	QString getErrorMsg( int errCode );
	void debugPrint( QString& astTreeDump );
	CDSR_VMEval* getVM();

	const QVector<QString>& getInternalVars();

	int CreateAST( AST_TXML_Tag* mathRoot );
	int CreateVM( enum CDsrMathModel taskID, long subtaskID );

	int execMain();

protected:
	void dispose();

	void manualAddAstNode( CDsrAST_Node *newRec );
	int addDsrSymbol2Table( CParseDsrSymbolTable* dsrSymbolTable, QString name, enum CDsrSymbolTableRecordType type, CDsrDataType_Node *dataType );
	int addDsrSymbol2Table( CParseDsrSymbolTable* dsrSymbolTable, CParseDsrSymbol& smb );
	long lookupSymbol( AST_TXML_Tag* xmlNode, CParseDsrSymbol*& dest );
	long lookupSymbol( QString varName, CParseDsrSymbol*& smb );

	CDsrAST_Node* Create( AST_TXML_Tag* xmlNode );
	CDsrAST_Node* Create_declare( AST_TXML_Tag* declareNode, std::vector<AST_TXML_Tag*>& nodeCollection );
	CDsrAST_Node* Create_lambda( AST_TXML_Tag* lambdaNode, std::vector<AST_TXML_Tag*>& nodeCollection, QString& newFunctionName );
	CDsrAST_Node* Create_set( AST_TXML_Tag* setNode, std::vector<AST_TXML_Tag*>& nodeCollection );
	CDsrAST_Node* Create_vector( AST_TXML_Tag* setNode, std::vector<AST_TXML_Tag*>& nodeCollection );
	CDsrAST_Node* Create_matrix( AST_TXML_Tag* setNode, std::vector<AST_TXML_Tag*>& nodeCollection );
	CDsrAST_Node* Create_condition( AST_TXML_Tag* xmlNode );
	CDsrAST_Node* Create_piecewise( AST_TXML_Tag* lambdaNode, std::vector<AST_TXML_Tag*>& nodeCollection );
	CDsrAST_Node* Create_constant( QString varName, long line );
	CDsrAST_Node* Create_ci( AST_TXML_Tag* ciNode );
	CDsrAST_Node* Create_cn( AST_TXML_Tag* cnNode );
	CDsrAST_Node* Create_apply( AST_TXML_Tag* applyNode, std::vector<AST_TXML_Tag*>& nodeCollection );
	CDsrAST_Node* Create_apply_DsrOp( AST_TXML_Tag* applyNode, std::vector<AST_TXML_Tag*>& nodeCollection, QString opName, int opID, long line, int& iRes );
	CDsrAST_Node* Create_apply_GeneralCall(  AST_TXML_Tag* applyNode, std::vector<AST_TXML_Tag*>& nodeCollection, QString opName, int opID, long line, int& iRes );

	CDsrAST_Node* constructVector( unsigned long rows, unsigned long columns, long line, QString tagName, std::vector<AST_TXML_Tag*>& nodeCollection );
	CDsrAST_Node* constructConstRational( long valLongA, const QString& aVal, long valLongB, const QString& bVal, long line );
	CDsrAST_Node* constructConstComplexCartesian( double valDblA, const QString& aVal, double valDblB, const QString& bVal, long line );
	CDsrAST_Node* constructConstComplexPolar( double valDblA, const QString& aVal, double valDblB, const QString& bVal, long line );

	CDsrAST_Node* constructFunctionCall( AST_TXML_Tag* applyNode, std::vector<AST_TXML_Tag*>& nodeCollection, QString opName, long opLine, int& iRes );
	CDsrAST_Node* constructFunctionCall( AST_TXML_Tag* applyNode, std::vector<AST_TXML_Tag*>& nodeCollection, CParseDsrSymbolTable* dsrSymbolTable, QString opName, long opLine, int& iRes );
	CDsrAST_Node* constructFunctionCall( AST_TXML_Tag* applyNode, std::vector<AST_TXML_Tag*>& nodeCollection, long idx, CParseDsrSymbol *smb, long opLine, int& iRes );

	void setLocalSymbolTable();
	void setGlobalSymbolTable();
	CParseDsrSymbolTable* getCurrentSymbolTable();
};

///////////////////////////////////////////////////////////////////////////////

#endif
