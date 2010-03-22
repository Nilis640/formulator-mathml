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

#ifndef __PARSEDSR_DATA_MMD_GROUP_H__
#define __PARSEDSR_DATA_MMD_GROUP_H__

#include "HUtils/ihkstring.h"
#include "HMathBS/iarray.h"
#include <QStringList>
#include <vector>
#include <list>
#include "../object/obj_vec.h"

///////////////////////////////////////////////////////////////////////////////

typedef CDSRMatrix<long> MMD_GroupMulTable;

class __HMATHLNGDLL__ MMD_GroupString : public MArray<long>
{
public:
	MMD_GroupString( void );
	MMD_GroupString( const MMD_GroupString& ms );
	virtual MMD_GroupString* copy( const MMD_GroupString *src );
	virtual ~MMD_GroupString( void );
	friend int compare( const MMD_GroupString& a, const MMD_GroupString& b );
};

///////////////////////////////////////////////////////////////////////////////

class __HMATHLNGDLL__ MMD_GroupStringArray : public MArray<MMD_GroupString>
{
public:
	MMD_GroupStringArray( void );
	MMD_GroupStringArray( const MMD_GroupStringArray& ms );
	virtual MMD_GroupStringArray* copy( const MMD_GroupStringArray *src );
	virtual ~MMD_GroupStringArray( void );
};

///////////////////////////////////////////////////////////////////////////////

#define commonGetNeutralElementName()		_T("E")
#define commonGetGroupMultiplicationSign()	_T("*")
#define commonGetGroupPodPrefix()			_T("P")

typedef std::list<long> list_of_integers;
typedef std::vector<list_of_integers> str_id_int_vector;

enum GroupReadyTo {
	GRT_Raw,
	GRT_Ready,
	GRT_Ready2Genes,
	GRT_Ready2Pods
};

///////////////////////////////////////////////////////////////////////////////

class __HMATHLNGDLL__ MMD_Group :  public MMD_Object
{
public :
	enum NotGroupError
	{
		ValidGroup = 0,
		Range,
		NotUnique,
		NotNeutral
	};

	struct {
		long items_c;
		QString* items;
		long rules_c;
		QString* left, *right;
	} inf_genes;

	struct InfPod {
		long pod_count;
		long pod_power;
		std::vector<long> pod;
	} inf_pods;

protected:
	enum GroupReadyTo	ready;
	long			grp_power;
	MIArray<QString>		str_id;
	MMD_GroupMulTable	table;
	MIArray<long>		subgr_int;
	CDSRBits		subgrp_det;

protected:
	bool GenerateGroup( long items_c, QString* items, long rules_c, QString* left, QString* right );
	bool GenerateGroup( long pod_count, long pod_power, MArray<long>& pod );
	long IdentifyFactor( std::list<long>& to_treat, long rules_c, MMD_GroupStringArray& left, MMD_GroupStringArray& right, str_id_int_vector& str_id_int );
	bool StopGenerating( unsigned long steps, unsigned long gpow );
	bool IsValidGenes( long item_c, long argc, QString* left, QString *right, QString *items, MMD_GroupStringArray& tmp_left_idx, MMD_GroupStringArray& tmp_right_idx );
	void Unroll( long i, std::list<long>::iterator it, std::list<long>& to_treat, MArray<long>& mem_left, MArray<long>& mem_right );
	void DeduceNewGenes( MMD_GroupStringArray& tmp_left_idx, MMD_GroupStringArray& tmp_right_idx );
	bool Equilibrium( MMD_GroupString& left_idx, MMD_GroupString& right_idx );
	long AddNewGene( long i, long j, MMD_GroupStringArray& tmp_left_idx, MMD_GroupStringArray& tmp_right_idx );
	void FillInNewGene( long len, MMD_GroupString& ab_x__left, MMD_GroupString& ab_x__right, MMD_GroupString& bc_y__left, MMD_GroupString& bc_y__right, MMD_GroupString& ay_xc__left, MMD_GroupString& ay_xc__right );
	void SimplifyNewGenes( MMD_GroupString& a, MMD_GroupStringArray& tmp_left_idx, MMD_GroupStringArray& tmp_right_idx );

	bool CheckPod( long pod_count, long pod_power, long i, long j, MArray<long>& pod );
	long FindPod( long pod_count, long pod_power, MArray<long>& pod, long *new_pod );

public :
	MMD_Group( CParseDsrSymbolTable& _smbtable );
	MMD_Group( CParseDsrSymbolTable& _smbtable, long power, QString* names, long **arg_table );
	MMD_Group( CParseDsrSymbolTable& _smbtable, long power, QString* names, MMD_GroupMulTable& arg_table );
	MMD_Group( const MMD_Group& ms );
	virtual MMD_Object* copy( const MMD_Group *src );
	virtual ~MMD_Group( void );

	enum GroupReadyTo GetReady( void );
	long& GetTable( long a, long b ); // a - row, b - column
	MMD_GroupMulTable& GetTable( void ); // a - row, b - column
	QString GetGroupNeutralElementName( void );
	long GetPower( void );
	long GetInv( long index );
	QString GetInv( const QString& a );
	long GetIndex( const QString& name );
	QString GetString( long index );
	long GetIndex2Index( long a, long b );
	QString GetString2String( const QString& a, const QString& b );
	long EvalString2Index( const QStringList& names );
	long EvalString2Index( const QString& names );
	QString EvalString2String( const QStringList& names );
	QString EvalString2String( const QString& names );
	int IsNotValid( MMD_GroupMulTable& _table, long _grp_power );
	MMD_Group* GetSubGroup( CDSRBitsStorage& det );
	MIArray<long>& FillSubGroupList( void );

	//=== step by step ====================
	/*
	-1 => last step
	 0 => step without result
	 1 =>step with result
	*/
	long StartSubGroupList( void );
	long StepSubGroupList( void );
	MIArray<long>& GetSubGroupList( void );
	//=====================================

	long Factorization( CDSRBitsStorage& det, long *chosen, bool left = true );
	int IsNormalSubGroup( CDSRBitsStorage& det );
	MMD_Group* GetFactorGroup( CDSRBitsStorage& det, bool check_normal = false );
	void ConstructGetReady( void );
	void ReleaseGetReady( void );
	void GetReady2GenerateGroup( long items_c, QString* items, long rules_c, QString* left, QString* right );
	void GetReady2GenerateGroup( long pod_count, long pod_power, MArray<long>& pod );
	bool StartGroupGeneration( void );
};

///////////////////////////////////////////////////////////////////////////////

typedef MMD_GroupString* CDSRGroupStrStorage;
typedef MMD_Group* CDSRGroupStorage;

#endif
