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

#include "../operator.h"
#include "../smbtbllink.h"

///////////////////////////////////////////////////////////////////////////////

CDsrDataType_Node* __ml_makeAssignNodeSet( 
						CDsrDataTypeTable& typetable, 
						enum CDsrDataTypes arg )
{
	CDsrDataType_Composition *args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
	CDsrDataType_Node *node;
	node = typetable.makeDataTypeNode( new CDsrDataType_Set( typetable.makeDataTypeNode( arg ) ) );
	node->setLValue();
	args->Add( node );
	node = typetable.makeDataTypeNode( new CDsrDataType_Set( typetable.makeDataTypeNode( arg ) ) );
	args->Add( node );
	node = typetable.makeDataTypeNode( new CDsrDataType_Set( typetable.makeDataTypeNode( arg ) ) );
	return typetable.makeDataTypeNode( new CDsrDataType_Function( args, node ) );
}

CDsrDataType_Node* __ml_makeAssignNode_Polynomial( 
						CDsrDataTypeTable& typetable, 
						enum CDsrDataTypes arg )
{
	CDsrDataType_Composition *args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
	CDsrDataType_Node *node;
	node = typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( arg ) ) );
	node->setLValue();
	args->Add( node );
	node = typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( arg ) ) );
	args->Add( node );
	node = typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( arg ) ) );
	return typetable.makeDataTypeNode( new CDsrDataType_Function( args, node ) );
}

CDsrDataType_Node* __ml_makeAssignNode_RationalFun( 
						CDsrDataTypeTable& typetable, 
						enum CDsrDataTypes arg )
{
	CDsrDataType_Composition *args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
	CDsrDataType_Node *node;
	node = typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( arg ) ) );
	node->setLValue();
	args->Add( node );
	node = typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( arg ) ) );
	args->Add( node );
	node = typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( arg ) ) );
	return typetable.makeDataTypeNode( new CDsrDataType_Function( args, node ) );
}

CDsrDataType_Node* __ml_makeAssignNode_Vector( 
						CDsrDataTypeTable& typetable, 
						enum CDsrDataTypes arg )
{
	CDsrDataType_Composition *args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
	CDsrDataType_Node *node;
	node = typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( arg ) ) );
	node->setLValue();
	args->Add( node );
	node = typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( arg ) ) );
	args->Add( node );
	node = typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( arg ) ) );
	return typetable.makeDataTypeNode( new CDsrDataType_Function( args, node ) );
}

CDsrDataType_Node* __ml_makeAssignNode( 
						CDsrDataTypeTable& typetable, 
						enum CDsrDataTypes arg )
{
	CDsrDataType_Composition *args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
	CDsrDataType_Node *node = typetable.makeDataTypeNode( arg );
	node->setLValue();
	args->Add( node );
	args->Add( typetable.makeDataTypeNode( arg ) );
	node = typetable.makeDataTypeNode( arg );
	return typetable.makeDataTypeNode( new CDsrDataType_Function( args, node ) );
}

CDsrDataType_Node* __ml_makeFunctionNode_Poly2Fun( 
	CDsrDataTypeTable& typetable, enum CDsrDataTypes arg )
{
	CDsrDataType_Composition *args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
	CDsrDataType_Node *resnode;
	resnode = typetable.makeDataTypeNode( DSRDATA_TYPE_POLYNOM );
	resnode->setNestedType( typetable.makeDataTypeNode( arg ) );
	args->Add( resnode );
	resnode = typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL_FUN );
	resnode->setNestedType( typetable.makeDataTypeNode( arg ) );
	return typetable.makeDataTypeNode( new CDsrDataType_Function( args, resnode ) );
}

CDsrDataType_Node* __ml_makeFunctionNode_Polynomial( 
	CDsrDataTypeTable& typetable, enum CDsrDataTypes arg, enum CDsrDataTypes res )
{
	CDsrDataType_Composition *args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
	args->Add( typetable.makeDataTypeNode( arg ) );
	CDsrDataType_Node *resnode = typetable.makeDataTypeNode( DSRDATA_TYPE_POLYNOM );
	resnode->setNestedType( typetable.makeDataTypeNode( res ) );
	return typetable.makeDataTypeNode( new CDsrDataType_Function( args, resnode ) );
}

CDsrDataType_Node* __ml_makeFunctionNode_RationalFun( 
	CDsrDataTypeTable& typetable, enum CDsrDataTypes arg, enum CDsrDataTypes res )
{
	CDsrDataType_Composition *args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
	args->Add( typetable.makeDataTypeNode( arg ) );
	CDsrDataType_Node *resnode = typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL_FUN );
	resnode->setNestedType( typetable.makeDataTypeNode( res ) );
	return typetable.makeDataTypeNode( new CDsrDataType_Function( args, resnode ) );
}

CDsrDataType_Node* __ml_makeFunctionNode( CDsrDataTypeTable& typetable, 
	CDsrDataType_Node* arg1, CDsrDataType_Node* arg2, CDsrDataType_Node* arg3, CDsrDataType_Node* res )
{
	CDsrDataType_Composition *args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
	args->Add( arg1 );
	args->Add( arg2 );
	args->Add( arg3 );
	CDsrDataType_Node *resnode = res;
	return typetable.makeDataTypeNode( new CDsrDataType_Function( args, resnode ) );
}

CDsrDataType_Node* __ml_makeFunctionNode( CDsrDataTypeTable& typetable, 
	enum CDsrDataTypes arg1, enum CDsrDataTypes arg2, enum CDsrDataTypes arg3, enum CDsrDataTypes res )
{
	CDsrDataType_Composition *args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
	args->Add( typetable.makeDataTypeNode( arg1 ) );
	args->Add( typetable.makeDataTypeNode( arg2 ) );
	args->Add( typetable.makeDataTypeNode( arg3 ) );
	CDsrDataType_Node *resnode = typetable.makeDataTypeNode( res );
	return typetable.makeDataTypeNode( new CDsrDataType_Function( args, resnode ) );
}

CDsrDataType_Node* __ml_makeFunctionNode( CDsrDataTypeTable& typetable, 
	CDsrDataType_Node* arg1, CDsrDataType_Node* arg2, CDsrDataType_Node* res )
{
	CDsrDataType_Composition *args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
	args->Add( arg1 );
	args->Add( arg2 );
	CDsrDataType_Node *resnode = res;
	return typetable.makeDataTypeNode( new CDsrDataType_Function( args, resnode ) );
}

CDsrDataType_Node* __ml_makeFunctionNode( CDsrDataTypeTable& typetable, 
	enum CDsrDataTypes arg1, enum CDsrDataTypes arg2, enum CDsrDataTypes res )
{
	CDsrDataType_Composition *args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
	args->Add( typetable.makeDataTypeNode( arg1 ) );
	args->Add( typetable.makeDataTypeNode( arg2 ) );
	CDsrDataType_Node *resnode = typetable.makeDataTypeNode( res );
	return typetable.makeDataTypeNode( new CDsrDataType_Function( args, resnode ) );
}

CDsrDataType_Node* __ml_makeFunctionNode( CDsrDataTypeTable& typetable, 
	CDsrDataType_Node* arg1, CDsrDataType_Node* res )
{
	CDsrDataType_Composition *args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
	args->Add( arg1 );
	CDsrDataType_Node *resnode = res;
	return typetable.makeDataTypeNode( new CDsrDataType_Function( args, resnode ) );
}

CDsrDataType_Node* __ml_makeFunctionNode( CDsrDataTypeTable& typetable, 
	enum CDsrDataTypes arg1, enum CDsrDataTypes res )
{
	CDsrDataType_Composition *args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
	args->Add( typetable.makeDataTypeNode( arg1 ) );
	CDsrDataType_Node *resnode = typetable.makeDataTypeNode( res );
	return typetable.makeDataTypeNode( new CDsrDataType_Function( args, resnode ) );
}

CDsrDataType_Node* __ml_makeFunctionNode_2Vector( CDsrDataTypeTable& typetable, 
	enum CDsrDataTypes arg )
{
	CDsrDataType_Composition *args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
	args->Add( typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( arg ) ) ) );
	CDsrDataType_Node *resnode = typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( arg ) ) );
	return typetable.makeDataTypeNode( new CDsrDataType_Function( args, resnode ) );
}

CDsrDataType_Node* __ml_makeFunctionNode_3Vector( CDsrDataTypeTable& typetable, 
	enum CDsrDataTypes arg )
{
	CDsrDataType_Composition *args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
	args->Add( typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( arg ) ) ) );
	args->Add( typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( arg ) ) ) );
	CDsrDataType_Node *resnode = typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( arg ) ) );
	return typetable.makeDataTypeNode( new CDsrDataType_Function( args, resnode ) );
}

CDsrDataType_Node* __ml_makeFunctionNode_2Polynomial( CDsrDataTypeTable& typetable, 
	enum CDsrDataTypes arg )
{
	CDsrDataType_Composition *args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
	args->Add( typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( arg ) ) ) );
	CDsrDataType_Node *resnode = typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( arg ) ) );
	return typetable.makeDataTypeNode( new CDsrDataType_Function( args, resnode ) );
}

CDsrDataType_Node* __ml_makeFunctionNode_3Polynomial( CDsrDataTypeTable& typetable, 
	enum CDsrDataTypes arg )
{
	CDsrDataType_Composition *args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
	args->Add( typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( arg ) ) ) );
	args->Add( typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( arg ) ) ) );
	CDsrDataType_Node *resnode = typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( arg ) ) );
	return typetable.makeDataTypeNode( new CDsrDataType_Function( args, resnode ) );
}

CDsrDataType_Node* __ml_makeFunctionNode_2RationalFun( CDsrDataTypeTable& typetable, 
	enum CDsrDataTypes arg )
{
	CDsrDataType_Composition *args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
	args->Add( typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( arg ) ) ) );
	CDsrDataType_Node *resnode = typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( arg ) ) );
	return typetable.makeDataTypeNode( new CDsrDataType_Function( args, resnode ) );
}

CDsrDataType_Node* __ml_makeFunctionNode_3RationalFun( CDsrDataTypeTable& typetable, 
	enum CDsrDataTypes arg )
{
	CDsrDataType_Composition *args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
	args->Add( typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( arg ) ) ) );
	args->Add( typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( arg ) ) ) );
	CDsrDataType_Node *resnode = typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( arg ) ) );
	return typetable.makeDataTypeNode( new CDsrDataType_Function( args, resnode ) );
}

///////////////////////////////////////////////////////////////////////////////
