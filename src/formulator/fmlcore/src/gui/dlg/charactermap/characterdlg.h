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

#ifndef __FMLCORE_DLG_CHARACTERDLG_H__
#define __FMLCORE_DLG_CHARACTERDLG_H__

#include <QMainWindow>
#include <QString>
#include <QDialog>

QT_BEGIN_NAMESPACE
class QClipboard;
class QComboBox;
class QFontComboBox;
class QLineEdit;
class QScrollArea;
class QCheckBox;
class QColorDialog;
class QToolButton;
class QLabel;
QT_END_NAMESPACE

class CInsertCharacterWidget;

#include "../../../fmlcore_global.h"

class __HERMITECH_FORMULIB__ CInsertCharacterDialog : public QDialog
{
    Q_OBJECT

protected:
	QString m_text;
	bool m_hasStyledText;
	int m_initFontSize;
	QColor m_backColor;
	QString m_initFontStyle;

public:
    CInsertCharacterDialog( QColor backColor, QWidget *parent =  0 );
    ~CInsertCharacterDialog();

	bool hasStyledText() { return m_hasStyledText; }
	QString text() { return m_text; }
	const QFont& currentFont();
	const QColor& currentColor();

protected:
	void updateDocument( bool styledText );

public slots:
    void findStyles(const QFont &font);
    void findSizes(const QFont &font);
    void insertCharacter(const QString &character);
    void updateFont(const QFont &font);
    void updateSize(const QString &fontSize);
    void updateStyle(const QString &fontStyle);
	void updateColor();
	void updateSampleText();
    void updateClipboard();
	void styledInsert() { updateDocument( true ); }
	void plainInsert() { updateDocument( false ); }

private:
	CInsertCharacterWidget *characterWidget;
    QClipboard *clipboard;
    QComboBox *styleCombo;
    QComboBox *sizeCombo;
	QLabel *labelSampleText;
    QFontComboBox *fontCombo;
    QLineEdit *lineEdit;
    QScrollArea *scrollArea;
    QCheckBox *fontMerging;

//signals:
//    void characterSelected(const QString &character);
};

#endif
