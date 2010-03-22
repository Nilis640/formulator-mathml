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

#if !defined( __FORMULATOR_DICTIONARY_OP_MAP_H__ )
#define __FORMULATOR_DICTIONARY_OP_MAP_H__

#include "HUtils/ihkstring.h"
#include "HUtils/idfas.h"

#include "../fmlcore_global.h"
#include "op_iddefs.h"

class HMathOpAttr;

///////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ HMathOpAttrDFA
{
protected:
	LKeyword *ptr_la_keyword_bydsr;
	LKeyword *ptr_la_keyword_bymml;
public:
	HMathOpAttrDFA( void );
	~HMathOpAttrDFA( void );

	LKeyword* getByDSR( void );
	LKeyword* getByMML( void );

	void InitByDSR( void );
	void InitByMML( void );
};

///////////////////////////////////////////////////////////////////////////////

inline
HMathOpAttrDFA::HMathOpAttrDFA( void )
{
	InitByDSR();
	InitByMML();
}

inline
HMathOpAttrDFA::~HMathOpAttrDFA( void )
{
	if( ptr_la_keyword_bydsr ) delete ptr_la_keyword_bydsr;
	if( ptr_la_keyword_bymml ) delete ptr_la_keyword_bymml;
}

inline
LKeyword* HMathOpAttrDFA::getByDSR( void )
{
	return ptr_la_keyword_bydsr;
}

inline
LKeyword* HMathOpAttrDFA::getByMML( void )
{
	return ptr_la_keyword_bymml;
}

///////////////////////////////////////////////////////////////////////////////

__HERMITECH_FORMULIB__ HMathOpAttr* getOperatorEntryByName( const QString& name, int mode );
__HERMITECH_FORMULIB__ HMathOpAttr* getOperatorEntryByMML( const QString& name );
__HERMITECH_FORMULIB__ HMathOpAttr* getOperatorEntryByDSR( const QString& name );

///////////////////////////////////////////////////////////////////////////////

#endif
