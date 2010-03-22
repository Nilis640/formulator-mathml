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

#if !defined( __FORMULATOR_FMLCORE_BTN_TOOLBAR_H__ )
#define __FORMULATOR_FMLCORE_BTN_TOOLBAR_H__

#include "xmlbtn.h"

///////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ FBtnToolbar : public QVector<FXmlBtn>, public HKToolBar
{
public:
	FBtnToolbar() {}
	virtual long getButtonCount() const
		{ return QVector<FXmlBtn>::size(); }
	virtual const HKToolBarButton* getButton( long i ) const
		{ return &at(i); }
	QString getToolbarDescr( void ) const
		{
			QString ret = getName() + _T("\r\n");
			for( long i = 0; i < size(); i++ )
				ret.append( QString("%1\t%2\r\n").arg( getButton(i)->getID() ).arg( getButton(i)->getToolTip() ) );
			return ret;
		}

	int Read_XML( AST_TXML_Tag *root, DRMessageManager& dmm, long dmm_page = 0 );

protected:
	int AddToolBarData( AST_TXML_Tag *root, DRMessageManager& dmm, long dmm_page );
};

/////////////////////////////////////////////////////////////////////

inline
QDataStream& operator << ( QDataStream& ar, const FBtnToolbar& lp )
{
	return ar << (const QVector<FXmlBtn>&) lp << (const HKToolBar&) lp;
}

inline
QDataStream& operator >> ( QDataStream& ar, FBtnToolbar& lp )
{
	return ar >> (QVector<FXmlBtn>&) lp >> (HKToolBar&) lp;
}

/////////////////////////////////////////////////////////////////////

#endif
