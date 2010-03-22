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

#include <time.h>

#include "undoredo.h"

/////////////////////////////////////////////////////////////////////////////

void CUndoRedo::timerEvent( QTimerEvent *event )
{
	Q_ASSERT( m_nTimerID == event->timerId() );
        if( m_nTimerID == event->timerId() )
            CheckPoint( 1 );
}

/////////////////////////////////////////////////////////////////////////////

CUndoRedo::CUndoRedo( CHaveUndoRedo *document, long nUndoLevels )
	: m_document( document )
	, m_nUndoLevels( nUndoLevels )
	, m_nTickCount( 0 )
	, m_nTimeStep( 1000 )
	, m_nTimerID( 0 )
{
	Q_ASSERT( m_document );
} 

CUndoRedo::~CUndoRedo() 
{
	ClearUndoList();
	ClearRedoList();
}

/////////////////////////////////////////////////////////////////////////////

void CUndoRedo::ClearRedoList()
{
	QByteArray* pData;
	while( !m_redoList.isEmpty() )
	{
		pData = m_redoList.takeFirst();
		if( pData ) delete pData;
	}
}

void CUndoRedo::ClearUndoList()
{
	QByteArray* pData;
	while( !m_undoList.isEmpty() )
	{
		pData = m_undoList.takeFirst();
		if( pData ) delete pData;
	}
}

void CUndoRedo::AddUndo( QByteArray *pData ) 
{
	// Remove old state if there are more than max allowed
	if ( m_undoList.size() > m_nUndoLevels && !m_undoList.isEmpty() )
	{
		QByteArray* pPrevData = m_undoList.takeLast();
		if( pPrevData ) delete pPrevData;
	}
	if ( pData )
		m_undoList.prepend( pData );

	emit undoAvailableSignal( CanUndo() );
	emit redoAvailableSignal( CanRedo() );
}

void CUndoRedo::AddRedo( QByteArray *pData ) 
{
	// Move state to head of redo list
	if ( pData )
		m_redoList.prepend( pData );

	emit undoAvailableSignal( CanUndo() );
	emit redoAvailableSignal( CanRedo() );
}

/////////////////////////////////////////////////////////////////////////////

void CUndoRedo::Store( QByteArray *pData ) const
{
	Q_ASSERT( pData && m_document );
	QDataStream dataStream( pData, QIODevice::WriteOnly );
	if( pData && m_document ) m_document->StoreDocument( dataStream );
}

void CUndoRedo::Load( QByteArray *pData ) 
{
	Q_ASSERT( pData && m_document );
	QDataStream dataStream( *pData );
	if( pData && m_document ) m_document->LoadDocument( dataStream );
}

/////////////////////////////////////////////////////////////////////////////

void CUndoRedo::Undo() 
{
	if ( CanUndo() )
	{
		KillTimer();	// for prevent saving of undo state 

		QByteArray *pData = m_undoList.takeFirst();
		if( pData ) delete pData;
		pData = new QByteArray();
		Store( pData );
		AddRedo( pData );
		pData = (QByteArray*) m_undoList.first();
		Load( pData );
	}
}

void CUndoRedo::Redo() 
{
	if ( CanRedo() )
	{
		KillTimer();	// for prevent saving of redo state 

		QByteArray *pData = (QByteArray*) m_redoList.takeFirst();
		if( pData )
		{
			AddUndo( pData );
			Load( pData );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

void CUndoRedo::Clear()
{
	KillTimer();
	ClearUndoList();
	ClearRedoList();
}

int CUndoRedo::Init()
{
	Clear();

	QByteArray *pData = new QByteArray();
	Store( pData );
	AddUndo( pData );

	return UpdateTickCount( 1 );
}

int CUndoRedo::CheckPoint( int fForcedCheck ) 
{
	Q_ASSERT( GetUndoCount() );

	KillTimer();
	if ( fForcedCheck )
	{
		QByteArray *pData = new QByteArray();
		Store( pData );
		AddUndo( pData );
		ClearRedoList();
		return 1;
	}
	SetTimer();
	return 0;
}

int CUndoRedo::UpdateTickCount( int fForcedCheck )
{
	unsigned int nTickCount = clock();
	if ( m_nTickCount == 0 ) m_nTickCount = nTickCount;
	if ( nTickCount - m_nTickCount > m_nTimeStep || fForcedCheck )
	{
		m_nTickCount = nTickCount;
		return 1;
	}
	return 0;
}

void CUndoRedo::SetTimer()
{
	m_nTimerID = startTimer( m_nTimeStep );
	Q_ASSERT( m_nTimerID != 0 );
}

void CUndoRedo::KillTimer()
{
	if( m_nTimerID ) killTimer( m_nTimerID );
}

/////////////////////////////////////////////////////////////////////////////
