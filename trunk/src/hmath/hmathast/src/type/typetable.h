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

#if !defined( __PARSEDSR_OPERATOR_TYPETABLE_H__ )
#define __PARSEDSR_OPERATOR_TYPETABLE_H__

#include "HUtils/iautoheap.h"
#include "typedsr.h"

class __HMATHLNGDLL__ CDsrDataTypeTable : public MIArray<CDsrDataType_Node>
{
protected:
	long reserved_idx[ DSRDATA_TYPE_LAST ];

	CDsrDataType_Node* __cloneDataTypeNode_FUNCTION( CDsrDataType_Node *clone_node );
	CDsrDataType_Node* __cloneDataTypeNode_COMPOSITION( CDsrDataType_Node *clone_node );
	CDsrDataType_Node* __cloneDataTypeNode_VECTOR( CDsrDataType_Node *clone_node );
	CDsrDataType_Node* __cloneDataTypeNode_SET( CDsrDataType_Node *clone_node );
	CDsrDataType_Node* __cloneDataTypeNode_POLYNOM( CDsrDataType_Node *clone_node );
	CDsrDataType_Node* __cloneDataTypeNode_RATIONAL_FUN( CDsrDataType_Node *clone_node );

public:
	CDsrDataTypeTable( void );
	~CDsrDataTypeTable( void );
	void Init( void );
	int Add( CDsrDataType_Node *n );
	long getTypeRecordIdx( enum CDsrDataTypes tp );
	CDsrDataType_Node* getTypeRecord( enum CDsrDataTypes tp );
	CDsrDataType_Node* makeDataTypeNode( CDsrDataType_Node *node );
	CDsrDataType_Node* makeDataTypeNode( enum CDsrDataTypes tp );
	CDsrDataType_Node* cloneDataTypeNode( CDsrDataType_Node *clone_node );
};

inline
long CDsrDataTypeTable::getTypeRecordIdx( enum CDsrDataTypes tp )
{
	return reserved_idx[ tp ];
}

inline
CDsrDataType_Node* CDsrDataTypeTable::getTypeRecord( enum CDsrDataTypes tp )
{
	return at( getTypeRecordIdx( tp ) );
}

#endif
