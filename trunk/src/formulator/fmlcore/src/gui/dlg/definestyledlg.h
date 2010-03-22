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

#ifndef __FMLCORE_DLG_DEFINESTYLEDLG_H__
#define __FMLCORE_DLG_DEFINESTYLEDLG_H__

#include <QMainWindow>
#include <QString>
#include <QDialog>
#include <QVector>

QT_BEGIN_NAMESPACE
class QComboBox;
class QFontComboBox;
class QCheckBox;
class QColorDialog;
class QPushButton;
class QLabel;
QT_END_NAMESPACE

#include "../../fmlcore_global.h"

/////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CDefineStyleDialog : public QDialog
{
    Q_OBJECT

protected:
	int						m_styleNumber;
	const QString*			m_styleTitle;
	const int*				m_styleID;

	QColor					m_backColor;
	QVector<QFont>			m_currentFont;
	QVector<QString>		m_currentStyle;
	QVector<QColor>			m_currentColor;

	QVector<QLabel*>		m_label;
	QVector<QFontComboBox*>	m_font;
	QVector<QComboBox*>		m_style;
	QVector<QPushButton*>	m_btn;
	QVector<QLabel*>		m_sample;

public:
    CDefineStyleDialog( QColor backColor, int styleNumber, const QString* styleTitle, const int* styleID, QWidget *parent =  0 );
    ~CDefineStyleDialog();

public slots:
	void factorySettings();
	virtual void accept();

protected slots:
	void updateFont( const QFont &font );
	void updateStyle( const QString &fontStyle );
	void updateColor();
	void findStyles( const QFont &font );

protected slots:
	void findStyles( const QFont &font, int i );
	void updateStyle( const QString &fontStyle, int i );
	void updateSampleText( int i );
	void alignSampleText();
};

/////////////////////////////////////////////////////////////////////////////

#endif
