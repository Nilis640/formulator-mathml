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

#if !defined( __COMMON_LANGUAGE_MATHNML_DFA_LA_MMLCHAR_H__ )
#define __COMMON_LANGUAGE_MATHNML_DFA_LA_MMLCHAR_H__

///////////////////////////////////////////////////////////////////////////////

#include "../fmlcore_global.h"
#include "HUtils/ilimauto.h"

#define LMMLCharStates   37
#define LMMLCharAlphabet 36

// class LMMLChar
// ~~~~~ ~~~~~~~~
class __HERMITECH_FORMULIB__ LMMLChar : public LAuto
{
public :
	QString text;

public :
	LMMLChar( int _is_style_greek = 0, int _is_style_number = 0, int _is_style_variable = 0, int _is_style_text = 0 );
	QString& getText( void );
	QString getTag( void );
	QString getAttr( void );
	void reset( void );
	~LMMLChar(){}

  protected:
	int is_style_greek, is_style_text;

	virtual long What( const QChar S );
	virtual void Deeds( void );
};


inline
void LMMLChar::reset( void )
{
	text = _T("");
	LAuto::Reset();
}

///////////////////////////////////////////////////////////////////////////////

#endif
