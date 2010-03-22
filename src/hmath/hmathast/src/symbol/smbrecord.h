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

#if !defined( __PARSEDSR_SYMBOL_SMBRECORD_H__ )
#define __PARSEDSR_SYMBOL_SMBRECORD_H__

#include "../hmathast_global.h"
#include "HUtils/ihkmap.h"
#include "HUtils/ihkstring.h"
#include "../type/typedsr.h"
#include "../operator/operator.h"

///////////////////////////////////////////////////////////////////////////////

enum CDsrSymbolTableRecordType
{
	SMBTABLE_TYPE_NONE = 0,
	SMBTABLE_TYPE_KEYWORD,
	SMBTABLE_TYPE_VARIABLE,
	SMBTABLE_TYPE_FUNCTION,
	SMBTABLE_TYPE_FORMULA
};

class CParseDsrSymbolTable;
class __HMATHLNGDLL__ CParseDsrSymbol
{
public:
	QString name;
	enum CDsrSymbolTableRecordType smb_record_type;
	CDsrDataType_Node *data_type;
	long param1;	// reserved for fun rec
	long param2;	// reserved-1 for var rec
	long param3;	// reserved-2 for var rec

public:
	CParseDsrSymbol( void )
	{
		name = _T("");
		data_type = 0;
		smb_record_type = SMBTABLE_TYPE_NONE;
		param1 = param2 = param3 = 0;
	}
	~CParseDsrSymbol( void )
	{}
	CParseDsrSymbol( QString _name, enum CDsrSymbolTableRecordType _type = SMBTABLE_TYPE_VARIABLE, CDsrDataType_Node *_data_type = 0 )
	{
		name = _name;
		smb_record_type = _type;
		data_type = _data_type;
		param1 = param2 = param3 = 0;
	}
	CDsrDataType_Node* getDataType( void )
	{
		return data_type;
	}
	void setDataType( CDsrDataType_Node *_data_type )
	{
		data_type = _data_type;
	}
	int is_predefined( void )
	{
		return param1 > FUNCTION_STD_NONE && param1 < FUNCTION_STD_LAST;
	}
	int hasSpecialName( void )
	{
		return name.length() && (name[ 0 ] == _T('@'));
	}
	int isVariable( void )
	{
		return smb_record_type == SMBTABLE_TYPE_VARIABLE;
	}
	int isFunction( void )
	{
		return smb_record_type == SMBTABLE_TYPE_FUNCTION;
	}
	long getArgNumber( long no = 0 );
	long getResNumber( long no = 0 );
	
	void debugPrint( QString& dest, CParseDsrSymbolTable *smbtable );
};

///////////////////////////////////////////////////////////////////////////////

#endif
