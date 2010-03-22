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

#if !defined( __FORMULATOR_DFA_LA_INSERTSTYLE_H__ )
#define __FORMULATOR_DFA_LA_INSERTSTYLE_H__

#include "HUtils/ilimauto.h"

#define LFInsertStyleStates   4
#define LFInsertStyleAlphabet 3

// class LFInsertStyle
// ~~~~~ ~~~~~
class LFInsertStyle : public LAuto
{
public :
	QString text;

public :
	LFInsertStyle( void );
	QString& getText( void );
	void reset( void );
	~LFInsertStyle(){}

protected:
	virtual long What( const QChar S );
	virtual void Deeds( void );
};


inline
void LFInsertStyle::reset( void )
{
	text = _T("");
	LAuto::Reset();
}

#endif
