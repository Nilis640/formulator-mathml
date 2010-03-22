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

#ifndef __HUTILS_BASEPARS_H__
#define __HUTILS_BASEPARS_H__

#include "baselex.h"
#include "HUtils/ilinemsg.h"

//////////////////////////////////////////////////////////////////////
// Possible types of parser's actions
enum ActionType
{
	_Halt,
	_Error,
	_Shift,
	_Reduce
};

struct ActionItem
{
	unsigned short value:14;
	unsigned short select:2;
	
	ActionItem( unsigned short avalue = 0, enum ActionType aselect = _Error ) 
		: value( avalue ), select( aselect )
	{
	}

	unsigned short hash( void ) const
	{
		return *((unsigned short*) this);
	}

	int operator == ( const ActionItem& a2 ) const
	{
		return ( value == a2.value ) && ( select == a2.select );
	}
};

typedef ActionItem* PtrActionItem;

//////////////////////////////////////////////////////////////////////

// LR-table file header (non unicode)
#define  LRTABLE_HEADER "CC Levitsky"
#define  HEADERLEN  ( sizeof( LRTABLE_HEADER ) )
struct LRTDATA 
{
	char			Header[ HEADERLEN ];     // Header.
	unsigned short	StatesQuant;             // States quantity.
	unsigned short	RulesQuant;              // States quantity.
	unsigned short	Terminals;               // Terminals quantity.
	unsigned short	NonTerminals;            // Nonterminals quantity.
	unsigned short	CtrlSum;                 // Control sum.
};
#define LRTDATASIZE  sizeof( struct LRTDATA )

//////////////////////////////////////////////////////////////////////

class HUTILS_EXPORT ActionT
{
public :
	ActionItem **Table;

	LEXEM    *NonTerms;
	unsigned short* RuleLen;
	unsigned short RulesQuant;
	unsigned short TableStates;
	unsigned short TableTNT;

	ActionT();
	ActionItem* operator()( unsigned short row, unsigned short column ) const;
	~ActionT();
};

//////////////////////////////////////////////////////////////////////

inline
ActionItem* ActionT::operator()( unsigned short row, unsigned short column ) const
{
	return Table[ row - 1 ] + (column - 1);
}

inline
ActionT::ActionT()
{
	Table = 0;
	RuleLen = 0;
	NonTerms = 0;
	RulesQuant = TableStates = TableTNT = 0;
}

inline
ActionT::~ActionT()
{
	if( Table )
	{
		delete[] Table;
		Table = 0;
	}
}

//////////////////////////////////////////////////////////////////////

HUTILS_EXPORT ActionT* createActionT( LRTDATA *action_data );

//////////////////////////////////////////////////////////////////////

class HUTILS_EXPORT BaseParser
{
public :
	enum ShouldExit
	{
		NotExit,
		Exit
	};
	enum ErrorLevel
	{
		LevelNone = 0,
		LevelIgnore,
		LevelAbort
	};

protected:
	bool SemanticErrorHappened;
	HKLineMessageTable *warning, *error;

	ActionT*	actions;
	Lex*		scaner;

	MArray<Lexem>			LexStack;
	MArray<unsigned short>	StateStack;
	unsigned short LexPoint, StatePoint;

public :
	BaseParser( Source& src, ActionT *arg_actions );
	virtual ~BaseParser( void );
	void SetNewLex( Source& src );
	virtual const QString& GetLexError( void );
	unsigned long GetLine( void );
	unsigned long GetCol( void );
	unsigned Make( void );

	const std::vector<HKLineMessage>& GetErrorTable();
	const std::vector<HKLineMessage>& GetWarningTable();
	int SetError( const QString& str, unsigned line = 0 );
	int SetError( const ActionItem& ai, unsigned line );
	int SetWarning( const QString& str, unsigned line = 0 );
	unsigned IsError();
	unsigned IsWarning();
	virtual void SetSemanticError();
	bool IsSemanticError();

protected:
	void     LexPush( Lexem& aLex );
	void     StatePush( unsigned short aState );
	void     Pop( unsigned short anRuleLength );
	unsigned TopState( void );

	virtual void NewLex( Source& aSource );
	virtual void BeforeMake( void );
	virtual void AfterMake ( void );
	virtual void CheckDeed ( ActionItem *act );
	virtual void GenCode( unsigned RuleNumber );
	virtual BaseParser::ShouldExit TryToSweep( void );
};


//////////////////////////////////////////////////////////////////////

inline
const std::vector<HKLineMessage>& BaseParser::GetErrorTable()
{
	return error->getTable();
}

inline
const std::vector<HKLineMessage>& BaseParser::GetWarningTable()
{
	return warning->getTable();
}

inline
void BaseParser::SetSemanticError()
{
	SemanticErrorHappened = true;
}

inline
bool BaseParser::IsSemanticError()
{
	return SemanticErrorHappened;
}

inline
unsigned BaseParser::IsError()
{
	return error->hasMessages();
}

inline
unsigned BaseParser::IsWarning()
{
	return warning->hasMessages();
}

//*****************************************************************************
// BaseParser::LexPush - pushs lexem to stack.
//*****************************************************************************
inline void BaseParser::LexPush( Lexem& aLex )
{
	LexStack[ LexPoint++ ] = aLex;
}

//*****************************************************************************
// BaseParser::StatePush - pushs lexem to stack.
//*****************************************************************************
inline void BaseParser::StatePush( unsigned short aState )
{
	StateStack[ StatePoint++ ] = aState;
}

//*****************************************************************************
// BaseParser::Pop - pops lexems and statements stacks.
//*****************************************************************************
inline void BaseParser::Pop( unsigned short anRuleLength )
{
	StatePoint -= anRuleLength;
	LexPoint   -= anRuleLength;
}

//*****************************************************************************
// BaseParser::StatePush - returns parser's state.
//*****************************************************************************
inline unsigned BaseParser::TopState( void )
{
	return StateStack[ StatePoint - 1 ];
}

inline
BaseParser::BaseParser( Source& src, ActionT *arg_actions ) 
	: LexStack( 32, 0, 16 )
	, StateStack( 32, 0, 16 )
{
	LexPoint = 0;
	StatePoint = 0;
	error   = new HKLineMessageTable();
	warning = new HKLineMessageTable();
	SemanticErrorHappened = 0;
	actions = arg_actions;
	NewLex( src );
}

inline
void BaseParser::SetNewLex( Source& src )
{
	NewLex( src );
}

inline
unsigned long BaseParser::GetLine( void )
{
	return scaner->GetLine();
}

inline
unsigned long BaseParser::GetCol( void )
{
	return scaner->GetCol();
}

inline
int BaseParser::SetError( const QString& str, unsigned line )
{
	error->setMessage( str, line );
	return 0;
}

inline
int BaseParser::SetError( const ActionItem& /*ai*/, unsigned line )
{
	error->setMessage( _T("Syntax error"), line );
	return 0;
}

inline
int BaseParser::SetWarning( const QString& str, unsigned line )
{
	warning->setMessage( str, line );
	return 0;
}

//////////////////////////////////////////////////////////////////////

#endif
