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

#include <vector>
#include "../mathml/mml_operator.h"
#include "../settings/options.h"
#include "op_map.h"

///////////////////////////////////////////////////////////////////////////////

HMathOpAttr* getOperatorEntryByMML( const QString& name )
{
	static HMathOpAttr coa;
	HMathOpAttr *p;
	p = getOperatorEntryByName( name, 0 );
	if( p )
	{
		coa = *p;
		coa.lspace = coa.lspace * ::getCurrentFormulatorStyle().getEM();
		coa.rspace = coa.rspace * ::getCurrentFormulatorStyle().getEM();
		return &coa;
	}
	return p;
}

HMathOpAttr* getOperatorEntryByDSR( const QString& name )
{
	static HMathOpAttr coa;
	HMathOpAttr *p;
	p = getOperatorEntryByName( name, 1 );
	if( p )
	{
		coa = *p;
		coa.lspace = coa.lspace * ::getCurrentFormulatorStyle().getEM();
		coa.rspace = coa.rspace * ::getCurrentFormulatorStyle().getEM();
		return &coa;
	}
	return p;
}

HMathOpAttr* getOperatorEntryByName( const QString& name, int mode )
{
	static HMathOpAttrDFA __hf_mathOpAttrDFA__;

	if( mode == __GLOBAL_INIT_MODE ) // just to init static variable
		return 0;

	if( mode == 0 )
	{
		LKeyword *k = __hf_mathOpAttrDFA__.getByMML();
		if( k )
		{
			int ret = k->Analisys( name.midRef(0) );
			long kread = k->GetRead();
			k->Reset();
			if( ret && ret >= 1 && ret <= HMathOpAttr_OPERATOR_NUMBER && kread == name.length() )
				return &HMathOpAttr_OperatorDictionary[ ret - 1 ];
		}
		return 0;
	}
	else if( mode == 1 )
	{
		LKeyword *k = __hf_mathOpAttrDFA__.getByDSR();
		if( k )
		{
			int ret = k->Analisys( name.midRef(0) );
			long kread = k->GetRead();
			k->Reset();
			if( ret && ret >= 1 && ret <= HMathOpAttr_OPERATOR_NUMBER && kread == name.length() )
				return &HMathOpAttr_OperatorDictionary[ ret - 1 ];
		}
		return 0;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

void HMathOpAttrDFA::InitByDSR( void )
{
	QString path = FmlCoreSettings::getOperatorByDSRFile();
	if( path != _T("") )
	{
		ptr_la_keyword_bydsr = new LKeyword();
		if( ptr_la_keyword_bydsr->ReadFile( path ) == -1 && ::shouldCreateCTSOnMissing() )
		{
			if( ptr_la_keyword_bydsr )
			{ 
				delete ptr_la_keyword_bydsr; 
				ptr_la_keyword_bydsr = 0; 
			}
			std::vector<QString> dsrname;
			for( long i = 0; i < HMathOpAttr_OPERATOR_NUMBER; i++ )
				dsrname.push_back( HMathOpAttr_OperatorDictionary[ i ].dsrname );
			ptr_la_keyword_bydsr = new LKeyword( &(dsrname[0]), HMathOpAttr_OPERATOR_NUMBER );
			ptr_la_keyword_bydsr->WriteFile( path );
		}
	}
	else
	{
		if( ::shouldCreateCTSOnMissing() )
		{
			std::vector<QString> dsrname;
			for( long i = 0; i < HMathOpAttr_OPERATOR_NUMBER; i++ )
				dsrname.push_back( HMathOpAttr_OperatorDictionary[ i ].dsrname );
			ptr_la_keyword_bydsr = new LKeyword( &(dsrname[0]), HMathOpAttr_OPERATOR_NUMBER );
		}
		else
			ptr_la_keyword_bydsr = new LKeyword();
	}
}

void HMathOpAttrDFA::InitByMML( void )
{
	QString path = FmlCoreSettings::getOperatorByMMLFile();
	if( path != _T("") )
	{
		ptr_la_keyword_bymml = new LKeyword();
		if( ptr_la_keyword_bymml->ReadFile( path ) == -1 && ::shouldCreateCTSOnMissing() )
		{
			if( ptr_la_keyword_bymml )
			{ 
				delete ptr_la_keyword_bymml; 
				ptr_la_keyword_bymml = 0; 
			}
			std::vector<QString> name;
			for( long i = 0; i < HMathOpAttr_OPERATOR_NUMBER; i++ )
				name.push_back( HMathOpAttr_OperatorDictionary[ i ].mmlname );
			ptr_la_keyword_bymml = new LKeyword( &(name[0]), HMathOpAttr_OPERATOR_NUMBER );
			ptr_la_keyword_bymml->WriteFile( path );
		}
	}
	else
	{
		if( ::shouldCreateCTSOnMissing() )
		{
			std::vector<QString> name;
			for( long i = 0; i < HMathOpAttr_OPERATOR_NUMBER; i++ )
				name.push_back( HMathOpAttr_OperatorDictionary[ i ].mmlname );
			ptr_la_keyword_bymml = new LKeyword( &(name[0]), HMathOpAttr_OPERATOR_NUMBER );
		}
		else
			ptr_la_keyword_bymml = new LKeyword();
	}
}

///////////////////////////////////////////////////////////////////////////////
