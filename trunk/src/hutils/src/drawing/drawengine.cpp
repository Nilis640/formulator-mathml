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

#include "fdedraw_qt.h"
#include "drawengine.h"

/////////////////////////////////////////////////////////////////////
//
//
//
//
//
/////////////////////////////////////////////////////////////////////

CFmlDrawEngine::CFmlDrawEngine( QPaintDevice * /*device*/ )
	: m_printing( false )
	, m_states()
{
}

CFmlDrawEngine::CFmlDrawEngine()
	: m_printing( false )
	, m_states()
{
}

/////////////////////////////////////////////////////////////////////

CFmlDrawEngine::~CFmlDrawEngine( void )
{
}

/////////////////////////////////////////////////////////////////////
//
//
//
//
//
/////////////////////////////////////////////////////////////////////

static enum FmlDrawEngineTypes g_fmlDrawEngineType = FmlDrawEngine_None;

/////////////////////////////////////////////////////////////////////

quint32 StartUpFDE( enum FmlDrawEngineTypes type )
{
	g_fmlDrawEngineType = type;

	switch( g_fmlDrawEngineType )
	{
	case FmlDrawEngine_Qt:
		return 1;
	case FmlDrawEngine_Gdi:
	case FmlDrawEngine_GgiPlus: 
	case FmlDrawEngine_OpenLG: 
	case FmlDrawEngine_DirectDraw: 
	case FmlDrawEngine_Direct3D9: 
	case FmlDrawEngine_Direct3D10: 
		return 0;
	default:
		break;
	}

	return 0;
}

quint32 ShutDownFDE( void )
{
	switch( g_fmlDrawEngineType )
	{
	case FmlDrawEngine_Qt:
		return 1;
	case FmlDrawEngine_Gdi:
	case FmlDrawEngine_GgiPlus: 
	case FmlDrawEngine_OpenLG: 
	case FmlDrawEngine_DirectDraw: 
	case FmlDrawEngine_Direct3D9: 
	case FmlDrawEngine_Direct3D10: 
		return 0;
	default:
		break;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////

CFmlDrawEngine* CreateFDE( QPaintDevice *device )
{
	if( g_fmlDrawEngineType == FmlDrawEngine_None ) return NULL;

	switch( g_fmlDrawEngineType )
	{
	case FmlDrawEngine_Qt: 
		return new CFmlDrawEngineQt( device );
	case FmlDrawEngine_Gdi:
	case FmlDrawEngine_GgiPlus: 
	case FmlDrawEngine_OpenLG: 
	case FmlDrawEngine_DirectDraw: 
	case FmlDrawEngine_Direct3D9: 
	case FmlDrawEngine_Direct3D10: 
		return NULL;
	default:
		break;
	}

	return NULL;
}

CFmlDrawEngine* CreateFDE( QPainter& painter )
{
	if( g_fmlDrawEngineType == FmlDrawEngine_None ) return NULL;

	switch( g_fmlDrawEngineType )
	{
	case FmlDrawEngine_Qt: 
		return new CFmlDrawEngineQt( painter );
	case FmlDrawEngine_Gdi:
	case FmlDrawEngine_GgiPlus: 
	case FmlDrawEngine_OpenLG: 
	case FmlDrawEngine_DirectDraw: 
	case FmlDrawEngine_Direct3D9: 
	case FmlDrawEngine_Direct3D10: 
		return NULL;
	default:
		break;
	}

	return NULL;
}

void DeleteFDE( CFmlDrawEngine*& pFde )
{
	if( pFde != NULL )
	{
		delete pFde;
		pFde = NULL;
	}
}

/////////////////////////////////////////////////////////////////////
