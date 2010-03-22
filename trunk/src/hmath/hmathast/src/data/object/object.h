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

#ifndef __PARSEDSR_DATA_MMD_OBJECT_H__
#define __PARSEDSR_DATA_MMD_OBJECT_H__

#include "HMathBS/iarray.h"
#include "HMathBS/inumber.h"
#include "../../hmathast_global.h"
#include "../smbtbllink.h"

///////////////////////////////////////////////////////////////////////////////

class __HMATHLNGDLL__ MMD_Object
{
protected:
	CDsrDataType_Node *m_type;
	CParseDsrSymbolTable* m_smbtable;
public:
	MMD_Object( CParseDsrSymbolTable& _smbtable, CDsrDataType_Node *_type = 0 );
	MMD_Object( CParseDsrSymbolTable& _smbtable, enum CDsrDataTypes _type );
	MMD_Object( const MMD_Object& o );
	MMD_Object operator=( const MMD_Object& o );
	CParseDsrSymbolTable& getSmbTable( void ) const;
	void setDataType( CDsrDataType_Node *tp );
	CDsrDataType_Node* getDataType( void ) const;
	enum CDsrDataTypes getType( void );
	virtual QString getTypeName( void );
	virtual MMD_Object* copy( const MMD_Object *src );
	virtual ~MMD_Object( void );
	virtual QString getTextView( void );
	virtual QString getFormulatorView( void );
	virtual QString getMmlView( void );

	int operator == ( MMD_Object& a ) const 
	{
		return *m_type == *(a.m_type);
	}
};

///////////////////////////////////////////////////////////////////////////////

class __HMATHLNGDLL__ MMD_ObjectHeap : MIArray<MMD_Object>
{
public:
	MMD_ObjectHeap( void );
	~MMD_ObjectHeap( void );
};

inline
MMD_ObjectHeap::MMD_ObjectHeap( void ) :
	MIArray<MMD_Object>( MIArray<MMD_Object>::LeaveItems, 256, 0, 256 )
{}

///////////////////////////////////////////////////////////////////////////////

#endif
