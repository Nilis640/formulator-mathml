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

#ifndef __FMLCORE_DLG_EVALUATE_H__
#define __FMLCORE_DLG_EVALUATE_H__

#include "HUtils/imessage.h"
#include "../fmlcore_global.h"

class CMMLCalc;

class __HERMITECH_FORMULIB__ QFormulatorCalc : public QObject
{
    //Q_OBJECT

protected:
	DRMessageManager m_dmm;
	long m_dmmPage;
	QString m_taskID, m_cmmlText, m_varText;
	TXML_Storage* m_xmlTree;
	CMMLCalc	*m_cmmlCalc;

public:
	QFormulatorCalc( QString taskID );
	~QFormulatorCalc();

	void init();
	void dispose();

	int compile( QString cmmlText );
	int compile( QString varText, QString mainBodyText, int useOnlyAssign = 0 );
	int evaluate();

	QStringList report();
	QString variableValue( QString name );
	QStringList itemsValue();
	QString lastItemValue();

	void setRealPrecision( int digits );
};

#endif

/////////////////////////////////////////////////////////////////////////////
