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

#include <cstdio>
#include "typetable.h"

/////////////////////////////////////////
//	Implementation
/////////////////////////////////////////
#define TYPETABLE_DL 81920
CDsrDataTypeTable::CDsrDataTypeTable( void ) :
	MIArray<CDsrDataType_Node>( TYPETABLE_DL, 0, TYPETABLE_DL )
{
	CDsrDataType_Node *node;
        for(long i = 0; i < DSRDATA_TYPE_LAST; i++)
            reserved_idx[ i ] = 0;

	// reserved record to make indexes be positive numbers
	Add( (CDsrDataType_Node*)0 );

	// reserved record for an error type
	node = new CDsrDataType_Node( DSRDATA_TYPE_ERROR );
	reserved_idx[ DSRDATA_TYPE_ERROR ] = Add( node );
	node->setTypeTableIdx( reserved_idx[ DSRDATA_TYPE_ERROR ] );
}

CDsrDataTypeTable::~CDsrDataTypeTable( void )
{
	//long i = ArrayDim();
}

int CDsrDataTypeTable::Add( CDsrDataType_Node *n )
{
	return MIArray<CDsrDataType_Node>::Add( n );
}

CDsrDataType_Node* CDsrDataTypeTable::makeDataTypeNode( CDsrDataType_Node *node )
{
	if( !node ) return 0;

	long idx;
	if( (idx = getTypeRecordIdx( node->getType() )) != 0 )
	{
		delete node;
		return getTypeRecord( node->getType() );
	}
	node->setTypeTableIdx( Add( node ) );
	return node;
}

CDsrDataType_Node* CDsrDataTypeTable::makeDataTypeNode( enum CDsrDataTypes tp )
{
	long idx;
	if( (idx = getTypeRecordIdx( tp )) != 0 ) return getTypeRecord( tp );

	switch( tp )
	{
	case DSRDATA_TYPE_FUNCTION:
		{
		CDsrDataType_Function *node = new CDsrDataType_Function();
		node->setTypeTableIdx( Add( node ) );
		return node;
		}
		break;
	case DSRDATA_TYPE_COMPOSITION:
		{
		CDsrDataType_Composition *node = new CDsrDataType_Composition();
		node->setTypeTableIdx( Add( node ) );
		return node;
		}
		break;
	case DSRDATA_TYPE_VECTOR:
		{
		CDsrDataType_Vector *node = new CDsrDataType_Vector();
		node->setTypeTableIdx( Add( node ) );
		return node;
		}
		break;
	case DSRDATA_TYPE_ABS_SET:
		{
		CDsrDataType_Set *node = new CDsrDataType_Set();
		node->setTypeTableIdx( Add( node ) );
		return node;
		}
		break;
	case DSRDATA_TYPE_POLYNOM:
		{
		CDsrDataType_Polynomial *node = new CDsrDataType_Polynomial();
		node->setTypeTableIdx( Add( node ) );
		return node;
		}
		break;
	case DSRDATA_TYPE_RATIONAL_FUN:
		{
		CDsrDataType_RationalFun *node = new CDsrDataType_RationalFun();
		node->setTypeTableIdx( Add( node ) );
		return node;
		}
		break;
	default:
		{
		CDsrDataType_Node *node = new CDsrDataType_Node( tp );
		node->setTypeTableIdx( Add( node ) );
		return node;
		}
	}
	return 0;
}

CDsrDataType_Node* CDsrDataTypeTable::cloneDataTypeNode( CDsrDataType_Node *clone_node )
{
	if( !clone_node ) return 0;

	switch( clone_node->getType() )
	{
	case DSRDATA_TYPE_FUNCTION:
		{
		return __cloneDataTypeNode_FUNCTION( clone_node );
		}
		break;
	case DSRDATA_TYPE_COMPOSITION:
		{
		return __cloneDataTypeNode_COMPOSITION( clone_node );
		}
		break;
	case DSRDATA_TYPE_VECTOR:
		{
		return __cloneDataTypeNode_VECTOR( clone_node );
		}
		break;
	case DSRDATA_TYPE_ABS_SET:
		{
		return __cloneDataTypeNode_SET( clone_node );
		}
		break;
	case DSRDATA_TYPE_POLYNOM:
		{
		return __cloneDataTypeNode_POLYNOM( clone_node );
		}
		break;
	case DSRDATA_TYPE_RATIONAL_FUN:
		{
		return __cloneDataTypeNode_RATIONAL_FUN( clone_node );
		}
	default:
		{
		CDsrDataType_Node *node = new CDsrDataType_Node( clone_node->getType() );
		node->setTypeTableIdx( Add( node ) );
		node->copyModifier( clone_node );
		return node;
		}
	}
	return 0;
}

CDsrDataType_Node* CDsrDataTypeTable::__cloneDataTypeNode_FUNCTION( CDsrDataType_Node *clone_node )
{
		CDsrDataType_Function *_clone_node = (CDsrDataType_Function*)clone_node;
		CDsrDataType_Function *node = new CDsrDataType_Function();
		node->setTypeTableIdx( Add( node ) );
		node->n_arg = cloneDataTypeNode( _clone_node->n_arg );
		node->n_res = cloneDataTypeNode( _clone_node->n_res );
		node->copyModifier( _clone_node );
		return node;
}

CDsrDataType_Node* CDsrDataTypeTable::__cloneDataTypeNode_COMPOSITION( CDsrDataType_Node *clone_node )
{
		CDsrDataType_Composition *_clone_node = (CDsrDataType_Composition*)clone_node;
		CDsrDataType_Composition *node = new CDsrDataType_Composition();
		node->setTypeTableIdx( Add( node ) );
		for( long i = 0; i < _clone_node->getChildNumber(); i++ )
			node->Add( cloneDataTypeNode( _clone_node->at( i ) ) );
		node->copyModifier( _clone_node );
		return node;
}

CDsrDataType_Node* CDsrDataTypeTable::__cloneDataTypeNode_VECTOR( CDsrDataType_Node *clone_node )
{
		CDsrDataType_Vector *_clone_node = (CDsrDataType_Vector*)clone_node;
		CDsrDataType_Vector *node = new CDsrDataType_Vector();
		node->setTypeTableIdx( Add( node ) );
		node->n_arg = cloneDataTypeNode( _clone_node->n_arg );
		node->copyModifier( _clone_node );
		node->dim.Copy( _clone_node->dim );
		return node;
}

CDsrDataType_Node* CDsrDataTypeTable::__cloneDataTypeNode_SET( CDsrDataType_Node *clone_node )
{
		CDsrDataType_Set *_clone_node = (CDsrDataType_Set*)clone_node;
		CDsrDataType_Set *node = new CDsrDataType_Set();
		node->setTypeTableIdx( Add( node ) );
		node->n_arg = cloneDataTypeNode( _clone_node->n_arg );
		node->copyModifier( _clone_node );
		return node;
}

CDsrDataType_Node* CDsrDataTypeTable::__cloneDataTypeNode_POLYNOM( CDsrDataType_Node *clone_node )
{
		CDsrDataType_Polynomial *_clone_node = (CDsrDataType_Polynomial*)clone_node;
		CDsrDataType_Polynomial *node = new CDsrDataType_Polynomial();
		node->setTypeTableIdx( Add( node ) );
		node->n_arg = cloneDataTypeNode( _clone_node->n_arg );
		node->copyModifier( _clone_node );
		return node;
}

CDsrDataType_Node* CDsrDataTypeTable::__cloneDataTypeNode_RATIONAL_FUN( CDsrDataType_Node *clone_node )
{
		CDsrDataType_RationalFun *_clone_node = (CDsrDataType_RationalFun*)clone_node;
		CDsrDataType_RationalFun *node = new CDsrDataType_RationalFun();
		node->setTypeTableIdx( Add( node ) );
		node->n_arg = cloneDataTypeNode( _clone_node->n_arg );
		node->copyModifier( _clone_node );
		return node;
}
