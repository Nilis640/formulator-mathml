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

#ifndef __FMLCORE_DLG_OTHERSTYLEDLG_H__
#define __FMLCORE_DLG_OTHERSTYLEDLG_H__

#include <QMainWindow>
#include <QString>
#include <QDialog>

QT_BEGIN_NAMESPACE
class QComboBox;
class QFontComboBox;
class QCheckBox;
class QColorDialog;
class QToolButton;
class QLabel;
QT_END_NAMESPACE

class FS_LogFont_Color;
#include "../../fmlcore_global.h"

class __HERMITECH_FORMULIB__ COtherStyleDialog : public QDialog
{
    Q_OBJECT

protected:
	const FS_LogFont_Color& m_lfc;
	QString m_currentStyle;
	QColor m_backColor;
	QFont m_currentFont;
	QColor m_currentColor;

public:
    COtherStyleDialog( QColor backColor, QWidget *parent =  0 );
    ~COtherStyleDialog();

	const QString& currentStyle() { return m_currentStyle; }
	const QFont& currentFont() { return m_currentFont; }
	const QColor& currentColor() { return m_currentColor; }

protected slots:
    void apply();
    void findStyles(const QFont &font);
    void updateFont(const QFont &font);
    void updateStyle(const QString &fontStyle);
	void updateColor();
	void updateSampleText();

protected:
    QComboBox *styleCombo;
	QLabel *labelSampleText;
    QFontComboBox *fontCombo;
};

#endif
