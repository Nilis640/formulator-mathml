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

#if !defined( __COMMON_LA_XMLSTR_H__ )
#define __COMMON_LA_XMLSTR_H__

#include "../limauto.h"

#define LXMLStringStates   22
#define LXMLStringAlphabet 18

// class LXMLString
// ~~~~~ ~~~~~~~
class HUTILS_EXPORT LXMLString : public LAuto
{

public :

	enum LXMLStringTypes
	{
		String = 1
	};

	QString read_string;	// принятая строка
	QString read_buff;	// принятая строка
	unsigned long	pre_int;	// константа целого типа
	unsigned long	mnt_int;	// константа целого типа
	short int		is_big;		// 
	short int		is_numentity_notchar;		// 
	QChar quat;

	LXMLString( QChar _quat );
	void Reset( QChar _quat ) { LAuto::Reset(); setQuat( _quat ); }
	void setQuat( QChar _quat ) { quat = _quat; }
	const QChar* data();

protected:

	virtual long What( const QChar S );
	virtual void Deeds( void );
};


inline
const QChar* LXMLString::data()
//       ~~~~~~~~~~~~~~~~~~~~
{
	return read_string.data();
}

#endif
