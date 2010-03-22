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

#ifndef __FMLCORE_DLG_NODEINPUTDLG_H__
#define __FMLCORE_DLG_NODEINPUTDLG_H__

#include <QString>
#include <QDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>

#include "HUtils/idrawstruct.h"
#include "../../fmlcore_global.h"

/////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CMmlDeclareDlg : public QDialog
{
protected:
	QComboBox *m_type, *m_occurance;
	QCheckBox *m_isSecondSlot;
	QSpinBox *m_arg;

public:
	CMmlDeclareDlg( QWidget *parent = 0 );

	QString type()
		{ return m_type->currentText(); }
	QString occurance()
		{ return m_occurance->currentText(); }
	bool isSecondSlot()
		{ return m_isSecondSlot->checkState() == Qt::Checked; }
	int argc()
		{ return m_arg->value(); }
};

/////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CMmlCiDlg : public QDialog
{
protected:
	QComboBox *m_type;

public:
	CMmlCiDlg( QWidget *parent = 0 );

	QString type()
		{ return m_type->currentText(); }
};

/////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CMmlCnDlg : public QDialog
{
protected:
	QComboBox *m_type;
	QSpinBox *m_base;

public:
	CMmlCnDlg( QWidget *parent = 0 );

	QString type()
		{ return m_type->currentText(); }
	int base()
		{ return m_base->value(); }
};

/////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CMmlQualifierDlg : public QDialog
{
protected:
	QComboBox *m_op;
	QSpinBox *m_bvar;

public:
	CMmlQualifierDlg( QString title, bool argsOn, QWidget *parent = 0 );

	QString op()
		{ return m_op->currentText(); }
	int bvar()
		{ return m_bvar->value(); }
};

/////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CMmlTableDlg : public QDialog
{
protected:
	QSpinBox *m_rows, *m_columns;

public:
	CMmlTableDlg( bool isRows, int rowNumber, bool isColumns, int colNumber, QWidget *parent = 0 );

	int rows()
		{ return m_rows->value(); }
	int columns()
		{ return m_columns->value(); }
};

/////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ QMmlSelectorDlg : public QDialog
{
protected:
	QSpinBox *m_argc;

public:
	QMmlSelectorDlg( QString title, int minArgs, QWidget *parent = 0 );

	int argc()
		{ return m_argc->value(); }
};

/////////////////////////////////////////////////////////////////////////////

#endif
