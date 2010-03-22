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

#if !defined( __HUTILS_MESSAGE_H__ )
#define __HUTILS_MESSAGE_H__

#include "../basexxx.h"
#include "linemsg.h"

struct HUTILS_EXPORT DRMessage
{
	QString		text;
	QString		c_arg1;
	QString		c_arg2;
	long		n_arg1;
	long		n_arg2;
	long		id;

	DRMessage()
	{
		id = n_arg1 = n_arg2 = -1;
		text = c_arg1 = c_arg2 = "";
	}
	DRMessage( const DRMessage& a )
	{
		*this = a;
	}
	DRMessage( long _id, QString _text = QString(), QString _c_arg1 = QString(), 
		long _n_arg1 = -1, QString _c_arg2 = QString(), long _n_arg2 = -1 )
	{
		id = _id;
		text = _text;
		c_arg1 = _c_arg1;
		c_arg2 = _c_arg2;
		n_arg1 = _n_arg1;
		n_arg2 = _n_arg2;
	}
	DRMessage& operator=( const DRMessage& a )
	{
		id = a.id;
		text = a.text;
		c_arg1 = a.c_arg1;
		c_arg2 = a.c_arg2;
		n_arg1 = a.n_arg1;
		n_arg2 = a.n_arg2;
		return *this;
	}
	QString Format();
	const QString& getText();
};

// class DRMessageManager
// ~~~~~ ~~~~~~~~~~~~~~~~
//
class HUTILS_EXPORT DRMessageManager : public std::vector<DRMessage>
{
protected :
	QString m_fileName;

public :
	DRMessageManager() : m_fileName( _T("") )
	{}
	void setDefaultProperty_FileName( QString str )
	{
		m_fileName = str;
	}
	const QString& getDefaultProperty_FileName( void )
	{
		return m_fileName;
	}
	QString getFormatedText( long index, long& adid )
	{
		adid = at( index ).id;
		return at( index ).Format();
	}
	void AddLine( long _id, QString _text, long _n_arg1, QString _c_arg2 = _T(""), long _n_arg2 = -1 );
	void AddLine( long _id, QString _text, QString _c_arg1 = _T(""), long _n_arg1 = -1, QString _c_arg2 = _T(""), long _n_arg2 = -1 );
	void AddLine( DRMessage& alm );
};

inline
void DRMessageManager::AddLine( long _id, QString _text, long _n_arg1, QString _c_arg2, long _n_arg2 )
{
	push_back( DRMessage(_id, _text, m_fileName, _n_arg1, _c_arg2, _n_arg2) );
}

inline
void DRMessageManager::AddLine( long _id, QString _text, QString _c_arg1, long _n_arg1, QString _c_arg2, long _n_arg2 )
{
	push_back( DRMessage(_id, _text, _c_arg1, _n_arg1, _c_arg2, _n_arg2) );
}

inline
void DRMessageManager::AddLine( DRMessage& alm )
{
	push_back( alm );
}

#endif
