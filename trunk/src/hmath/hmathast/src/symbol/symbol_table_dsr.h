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

#if !defined( __PARSEDSR_SYMBOL_TABLE_H__ )
#define __PARSEDSR_SYMBOL_TABLE_H__

#include <vector>

#include "HUtils/imessage.h"
#include "HMathBS/inumber.h"
#include "HMathBS/iarray.h"
#include "../mminfo/mm_info.h"
#include "../type/typetable.h"
#include "../operator/operator.h"
#include "smbrecord.h"
#include "optable.h"

///////////////////////////////////////////////////////////////////////////////

class MMD_Object;
class MMD_Vector;
class MMD_Set;
class MMD_Group;
class MMD_Polynomial;
class MMD_RationalFun;
class MMD_String;

///////////////////////////////////////////////////////////////////////////////

class __HMATHLNGDLL__ CParseDsrSymbolTable : public CDsrOptionTable
{
protected:
	long					m_var_number;
	long					m_fun_number;

	MArray<CParseDsrSymbol> m_table;
	HKMap<QString, long>	m_map;

	CDsrDataTypeTable		m_type_table;

	DRMessageManager*		m_messanger;
	long					m_message_page;
	QString					m_infile;

	enum CDsrMathModel		m_current_task_id;
	long					m_current_subtask_id;

	int						m_is_local;
	MArray<long>			m_used_xvar;
	MArray<long>			m_used_dxvar;

	std::vector<CParseDsrSymbolTable*>	m_local_smbtable;

public:
	CParseDsrSymbolTable( int is_local = 0 );
	~CParseDsrSymbolTable( void );
	int Add( CParseDsrSymbol& toadd );
	long getSymbolNumber( void );
	CParseDsrSymbol* getSymbol( long idx );
	long* is_find( QString name );
	int is_predefined( long symbol_id );
	CParseDsrSymbolTable *getNewLocalSmbTable( void );

	void debugPrint( QString& dest, long flags );

	void FlushUsedXVar( void );
	long AddUsedXVar( long id );
	MArray<long>& getUsedXVar( void );
	void FlushUsedDXVar( void );
	long AddUsedDXVar( long id );
	MArray<long>& getUsedDXVar( void );

	CDsrDataTypeTable& getTypeTable( void );
	CDsrDataType_Node* cloneDataTypeNode( CDsrDataType_Node *node );
	CDsrDataType_Node* makeDefaultDataTypeNode( void );
	enum CDsrDataTypes getDefaultDataTypeID( void );

	void setMessanger( DRMessageManager *dmm, long _message_page, QString _infile );
	DRMessageManager*  getMessanger( void );
	void AddLine( QString _text, long _n_arg1 = -1, QString _c_arg2 = _T(""), long _n_arg2 = -1 );

	void setTaskID( enum CDsrMathModel m_current_task_id = DSRMM_NONE, long m_current_subtask_id = -1 );

	long getNewVarNo( void );
	long getNewFunNo( void );
	long getVarMaxNo( void );
	long getFunMaxNo( void );
	void allocateVariables( enum CDsrFunctionListID address_scheme = VARIABLE_USER );

protected:
	std::vector<MMD_Object*> m_object_table;

public:
	MMD_Vector* makeUniWord( MMD_Vector* node );
	MMD_Set* makeUniWord( MMD_Set* node );
	MMD_Group* makeUniWord( MMD_Group* node );
	MMD_Polynomial* makeUniWord( MMD_Polynomial* node );
	MMD_RationalFun* makeUniWord( MMD_RationalFun* node );
	MMD_String* makeUniWord( MMD_String* node );
	void ObjectTableFlush( void );

	MMD_Vector* makeUniWord_Vector( CDSRArray<CDSRNatural>& res );
	MMD_Vector* makeUniWord_Vector( CDSRArray<CDSRInteger>& res );
	MMD_Vector* makeUniWord_Vector( CDSRArray<CDSRRational>& res );
	MMD_Vector* makeUniWord_Vector( CDSRArray<CDSRReal>& res );
	MMD_Vector* makeUniWord_Vector( CDSRArray<CDSRComplex>& res );

	MMD_Vector* makeUniWord_Vector( CDSRMMatrix<CDSRNatural>& res );
	MMD_Vector* makeUniWord_Vector( CDSRMMatrix<CDSRInteger>& res );
	MMD_Vector* makeUniWord_Vector( CDSRMMatrix<CDSRRational>& res );
	MMD_Vector* makeUniWord_Vector( CDSRMMatrix<CDSRReal>& res );
	MMD_Vector* makeUniWord_Vector( CDSRMMatrix<CDSRComplex>& res );

	MMD_Vector* makeUniWord_Vector( CDSRMVector<CDSRNatural>& res );
	MMD_Vector* makeUniWord_Vector( CDSRMVector<CDSRInteger>& res );
	MMD_Vector* makeUniWord_Vector( CDSRMVector<CDSRRational>& res );
	MMD_Vector* makeUniWord_Vector( CDSRMVector<CDSRReal>& res );
	MMD_Vector* makeUniWord_Vector( CDSRMVector<CDSRComplex>& res );

	int convert2DSRNaturalArray( MMD_Vector *vec, CDSRArray<CDSRNatural>& res );
	int convert2DSRIntegerArray( MMD_Vector *vec, CDSRArray<CDSRInteger>& res );
	int convert2DSRRationalArray( MMD_Vector *vec, CDSRArray<CDSRRational>& res );
	int convert2DSRRealArray( MMD_Vector *vec, CDSRArray<CDSRReal>& res );
	int convert2DSRComplexArray( MMD_Vector *vec, CDSRArray<CDSRComplex>& res );

	int convert2DSRNaturalMMatrix( MMD_Vector *vec, CDSRMMatrix<CDSRNatural>& res );
	int convert2DSRIntegerMMatrix( MMD_Vector *vec, CDSRMMatrix<CDSRInteger>& res );
	int convert2DSRRationalMMatrix( MMD_Vector *vec, CDSRMMatrix<CDSRRational>& res );
	int convert2DSRRealMMatrix( MMD_Vector *vec, CDSRMMatrix<CDSRReal>& res );
	int convert2DSRComplexMMatrix( MMD_Vector *vec, CDSRMMatrix<CDSRComplex>& res );

	int convert2DSRNaturalMVector( MMD_Vector *vec, CDSRMVector<CDSRNatural>& res );
	int convert2DSRIntegerMVector( MMD_Vector *vec, CDSRMVector<CDSRInteger>& res );
	int convert2DSRRationalMVector( MMD_Vector *vec, CDSRMVector<CDSRRational>& res );
	int convert2DSRRealMVector( MMD_Vector *vec, CDSRMVector<CDSRReal>& res );
	int convert2DSRComplexMVector( MMD_Vector *vec, CDSRMVector<CDSRComplex>& res );

	MMD_Polynomial* makeUniWord_Polynomial( CDSRArray<CDSRInteger>& res );
	MMD_Polynomial* makeUniWord_Polynomial( CDSRArray<CDSRReal>& res );
	MMD_Polynomial* makeUniWord_Polynomial( CDSRArray<CDSRComplex>& res );

	int convert2DSRIntegerArray( MMD_Polynomial *vec, CDSRArray<CDSRInteger>& res );
	int convert2DSRRealArray( MMD_Polynomial *vec, CDSRArray<CDSRReal>& res );
	int convert2DSRComplexArray( MMD_Polynomial *vec, CDSRArray<CDSRComplex>& res );

};

///////////////////////////////////////////////////////////////////////////////

inline
long CParseDsrSymbolTable::getNewVarNo( void )
{
	return m_var_number++;
}

inline
long CParseDsrSymbolTable::getNewFunNo( void )
{
	return m_fun_number++;
}

inline
long CParseDsrSymbolTable::getVarMaxNo( void )
{
	return m_var_number - 1;
}

inline
long CParseDsrSymbolTable::getFunMaxNo( void )
{
	return m_fun_number - 1;
}

///////////////////////////////////////////////////////////////////////////////

inline
int CParseDsrSymbolTable::Add( CParseDsrSymbol& toadd )
{
	long idx = m_table.Add( toadd );
	m_map[ toadd.name ] = idx;
	return idx;
}

inline
long* CParseDsrSymbolTable::is_find( QString name )
{
	return ( m_map.find( name ) == m_map.end() ) ? 0 : &m_map[ name ];
}

inline
CParseDsrSymbol* CParseDsrSymbolTable::getSymbol( long idx )
{
	return ( idx >= 0 && idx < (long) m_table.ArrayDim() ) ? &m_table[ idx ] : 0;
}

inline
long CParseDsrSymbolTable::getSymbolNumber( void )
{
	return (long) m_table.ArrayDim();
}

inline
int CParseDsrSymbolTable::is_predefined( long symbol_id )
{
	CParseDsrSymbol *smb;
	if( symbol_id == -1 || (smb = getSymbol( symbol_id )) == 0 )
		return 0;
	return smb->is_predefined();
}

///////////////////////////////////////////////////////////////////////////////

inline
void CParseDsrSymbolTable::FlushUsedXVar( void )
{
	m_used_xvar.Flush();
}

inline
long CParseDsrSymbolTable::AddUsedXVar( long id )
{
	long i;
	if( (i = m_used_xvar.Find( id )) == INT_MAX )
		return m_used_xvar.Add( id );
	return i;
}

inline
MArray<long>& CParseDsrSymbolTable::getUsedXVar( void )
{
	return m_used_xvar;
}

inline
void CParseDsrSymbolTable::FlushUsedDXVar( void )
{
	m_used_dxvar.Flush();
}

inline
long CParseDsrSymbolTable::AddUsedDXVar( long id )
{
	long i;
	if( (i = m_used_dxvar.Find( id )) == INT_MAX )
		return m_used_dxvar.Add( id );
	return i;
}

inline
MArray<long>& CParseDsrSymbolTable::getUsedDXVar( void )
{
	return m_used_dxvar;
}

///////////////////////////////////////////////////////////////////////////////

inline
CDsrDataTypeTable& CParseDsrSymbolTable::getTypeTable( void )
{
	return m_type_table;
}

inline
CDsrDataType_Node* CParseDsrSymbolTable::cloneDataTypeNode( CDsrDataType_Node *clone_node )
{
	return getTypeTable().cloneDataTypeNode( clone_node );
}

inline
CDsrDataType_Node* CParseDsrSymbolTable::makeDefaultDataTypeNode( void )
{
	return getTypeTable().makeDataTypeNode( (enum CDsrDataTypes)::getCDsrMMDefaultType( m_current_task_id, m_current_subtask_id ) );
}

inline
enum CDsrDataTypes CParseDsrSymbolTable::getDefaultDataTypeID( void )
{
	return (enum CDsrDataTypes) ::getCDsrMMDefaultType( m_current_task_id, m_current_subtask_id );
}

///////////////////////////////////////////////////////////////////////////////

inline
void CParseDsrSymbolTable::setMessanger( DRMessageManager *dmm = 0, long _message_page = 0, QString _infile = _T("") )
{
	m_messanger = dmm;
	m_infile = _infile;
	m_message_page = _message_page;
}

inline
DRMessageManager* CParseDsrSymbolTable::getMessanger( void )
{
	return m_messanger;
}

inline
void CParseDsrSymbolTable::AddLine( QString _text, long _n_arg1, QString _c_arg2, long _n_arg2 )
{
	if( getMessanger() )
		getMessanger()->AddLine( m_message_page, _text, m_infile, _n_arg1, _c_arg2, _n_arg2 );
}

inline
void CParseDsrSymbolTable::setTaskID( enum CDsrMathModel _m_current_task_id, long _m_current_subtask_id )
{
	m_current_task_id = _m_current_task_id;
	m_current_subtask_id = _m_current_subtask_id;
}

///////////////////////////////////////////////////////////////////////////////

#endif
