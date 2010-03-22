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

#if !defined( __COMMON_LIMAUTO_H__ )
#define __COMMON_LIMAUTO_H__

#include <QDataStream>
#include "../basexxx.h"

// class LAuto
// ~~~~~ ~~~~~
class HUTILS_EXPORT LAuto
{
	
protected :

	qint32	CurrentState;	// Текущее состояние конечного автомата
	qint32	PrevState;		// Предыдущее состояние конечного автомата
	qint32	nStates,		// Количество состояний конечного автомата
			nAlphabet;		// Количество символов входного алфавита
	long*	Jmp;			// Таблица переходов
	long*	EndStates;		// Вектор флагов последних
							// состояний конечного автомата
	qint32	read;			// Количество символов, принятых конечным
							// автоматом в последний раз
	QChar	character;		// Текущий символ на входе
	
protected :
	
	virtual	void Deeds( void );						// Реализация действий, выполняемых при 
													// переходе между состояниями
	virtual	long What( const QChar nextS ) = 0;		// Перекодирование символа входного
													// алфавита в индекс таблицы переходов
	long	GetPreviousState( void );				// Предыдущее состояние

	void FreeMemory( void );
	
public :
	
	LAuto( void );
	LAuto( long anSt, long anAB, long* aJmp, long* aEndStates );
	virtual ~LAuto( void );
	
	int Analisys( const QStringRef& aText );	// Рабочий цикл анализа входного текста
	long GetRead( void );					// Количество символов, принятых в последний раз
	void Reset( void );

	int ReadFile( QString path );
	int WriteFile( QString path );

	HUTILS_EXPORT friend QDataStream& operator << ( QDataStream& ar, const LAuto& lp );
	HUTILS_EXPORT friend QDataStream& operator >> ( QDataStream& ar, LAuto& lp );
};


inline
LAuto::LAuto( long anSt, long anAB, long* aJmp, long* aEndStates )
//     ~~~~~
	: CurrentState( 1 ), PrevState( 0 ), nStates( anSt ), nAlphabet( anAB ), read( 0 ), character( 0 )
{
	Jmp = new long[ nAlphabet * nStates ];
	memcpy( Jmp, aJmp, sizeof( long ) * nAlphabet * nStates );
	EndStates = new long[ nStates ];
	memcpy( EndStates, aEndStates, sizeof( long ) * nStates );
}

inline
LAuto::LAuto( void )
//     ~~~~~
	: CurrentState( 1 ), PrevState( 0 ), nStates( 0 ),
		nAlphabet( 0 ), read( 0 ), character( 0 )
{
	Jmp = 0;
	EndStates = 0;
}

inline
long LAuto::GetPreviousState( void )
//              ~~~~~~~~~~~~~~~~
{
	return PrevState;
}

inline
void LAuto::Deeds( void )
//          ~~~~~
{}

inline
long LAuto::GetRead( void )
//              ~~~~~~~
{
	return read;
}

inline
LAuto::~LAuto( void )
//     ~~~~~~
{
	FreeMemory();
}

inline
void LAuto::Reset( void )
{
	CurrentState = 1;
	PrevState = 0;
	read = 0;
	character = 0;
}

#endif
