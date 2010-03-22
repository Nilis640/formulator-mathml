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

#include <QtGui>

#include "characterwidget.h"
#include "characterdlg.h"
#include "HUtils/idrawstruct.h"
#include "HMathML/isettings.h"

/////////////////////////////////////////////////////////////////////////////

CInsertCharacterDialog::CInsertCharacterDialog( QColor backColor, QWidget *parent )
	: QDialog( parent )
{
	const FS_LogFont_Color& lfc = ::getFontStyle( ::getCurrentFormulatorStyle_OnChar() );
	QFont currentFmlFont( lfc.m_name, (int) ::getCurrentKegl(), lfc.m_isBold ? QFont::Bold : QFont::Normal, lfc.m_isItalic ? true : false );
	currentFmlFont.setFixedPitch( lfc.m_isFixed );
	currentFmlFont.setStyleHint( lfc.m_hint );
	m_backColor = backColor;
	m_hasStyledText = false;

	QWidget *centralWidget = new QWidget;

    QLabel *fontLabel = new QLabel(tr("Font:"));
    fontCombo = new QFontComboBox();
	fontCombo->setCurrentFont( currentFmlFont );
    QLabel *sizeLabel = new QLabel(tr("Size:"));
    sizeCombo = new QComboBox;
    QLabel *styleLabel = new QLabel(tr("Style:"));
    styleCombo = new QComboBox;

	scrollArea = new QScrollArea;
    characterWidget = new CInsertCharacterWidget;
    scrollArea->setWidget(characterWidget);
	characterWidget->updateStyle( m_initFontStyle = QFontDatabase().styleString( currentFmlFont ) );
	characterWidget->updateSize( QString().number( m_initFontSize = currentFmlFont.pointSize() ) );
	characterWidget->updateFont( currentFmlFont );

    findStyles(fontCombo->currentFont());
    findSizes(fontCombo->currentFont());
    lineEdit = new QLineEdit;

	QPushButton *colorButton = new QPushButton(tr("&Set Color"));
	colorButton->setIcon(QIcon(":/images/setcolor.png"));
	QLabel *labelSampleTextTitle = new QLabel("Sample text:");
	labelSampleText = new QLabel();
	labelSampleText->setFrameShape( QFrame::Box );
	updateSampleText();

    clipboard = QApplication::clipboard();
    QPushButton *styledInsertButton = new QPushButton(tr("Insert with style"));
	styledInsertButton->setIcon(QIcon(":/images/insertasstyledtext.png"));
    QPushButton *plainInsertButton = new QPushButton(tr("Insert plain text"));
	plainInsertButton->setIcon(QIcon(":/images/insertasplaintext.png"));
    QPushButton *clipboardButton = new QPushButton(tr("&Copy to clipboard"));
	clipboardButton->setIcon(QIcon(":/images/copy.png"));
    QPushButton *closeButton = new QPushButton(tr("&Cancel"));
	closeButton->setIcon(QIcon(":/images/exit.png"));

	connect(colorButton, SIGNAL(clicked()), this, SLOT(updateColor()));
	connect(fontCombo, SIGNAL(currentFontChanged(const QFont &)), this, SLOT(findStyles(const QFont &)));
    connect(fontCombo, SIGNAL(currentFontChanged(const QFont &)), this, SLOT(findSizes(const QFont &)));
    connect(fontCombo, SIGNAL(currentFontChanged(const QFont &)), this, SLOT(updateFont(const QFont &)));
    connect(sizeCombo, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(updateSize(const QString &)));
    connect(styleCombo, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(updateStyle(const QString &)));
    connect(characterWidget, SIGNAL(characterSelected(const QString &)), this, SLOT(insertCharacter(const QString &)));
    connect(clipboardButton, SIGNAL(clicked()), this, SLOT(updateClipboard()));
    connect(styledInsertButton, SIGNAL(clicked()), this, SLOT(styledInsert()));
    connect(plainInsertButton, SIGNAL(clicked()), this, SLOT(plainInsert()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *controlsLayout = new QHBoxLayout;
    controlsLayout->addWidget(fontLabel);
    controlsLayout->addWidget(fontCombo, 1);
    controlsLayout->addWidget(sizeLabel);
    controlsLayout->addWidget(sizeCombo, 1);
    controlsLayout->addWidget(styleLabel);
    controlsLayout->addWidget(styleCombo, 1);
    controlsLayout->addSpacing(4);
    controlsLayout->addWidget(colorButton);
    controlsLayout->addWidget(labelSampleTextTitle);
    controlsLayout->addWidget(labelSampleText);
    controlsLayout->addStretch(1);

    QHBoxLayout *lineLayout = new QHBoxLayout;
    lineLayout->addWidget(lineEdit, 1);
    lineLayout->addSpacing(12);
    lineLayout->addWidget(styledInsertButton);
    lineLayout->addWidget(plainInsertButton);
    lineLayout->addWidget(clipboardButton);
    lineLayout->addWidget(closeButton);

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addLayout(controlsLayout);
    centralLayout->addWidget(scrollArea, 1);
    centralLayout->addSpacing(4);
    centralLayout->addLayout(lineLayout);
    centralWidget->setLayout(centralLayout);

    QVBoxLayout *upperLayout = new QVBoxLayout;
	upperLayout->addWidget(centralWidget);
	setLayout(upperLayout);

	setWindowTitle(tr("Insert Character"));
	characterWidget->setFocus();
}

CInsertCharacterDialog::~CInsertCharacterDialog()
{
}

const QFont& CInsertCharacterDialog::currentFont()
{
	return characterWidget->currentFont();
}

const QColor& CInsertCharacterDialog::currentColor()
{
	return characterWidget->currentColor();
}

void CInsertCharacterDialog::updateFont(const QFont &font)
{
	characterWidget->updateFont(font);
	updateSampleText();
}

void CInsertCharacterDialog::updateSize(const QString &fontSize)
{
	characterWidget->updateSize(fontSize);
	updateSampleText();
}

void CInsertCharacterDialog::updateStyle(const QString &fontStyle)
{
	characterWidget->updateStyle(fontStyle);
	updateSampleText();
}

void CInsertCharacterDialog::updateSampleText()
{
	QString text = QString("<span style=\"background-color: %1; color: %2; font-size: %3pt; font-family: %4; font-weight: %5; font-style: %6;\">ABCabc</span>")
						.arg(m_backColor.name())
						.arg(currentColor().name())
						.arg(currentFont().pointSize())
						.arg(currentFont().family())
						.arg(currentFont().bold() ? "bold" : "normal")
						.arg(currentFont().italic() ? "italic" : "normal");
	labelSampleText->setText( text );
}

void CInsertCharacterDialog::updateColor()
{
	QColor newColor = QColorDialog::getColor( currentColor(), this );
	characterWidget->updateColor( newColor );
	updateSampleText();
}

void CInsertCharacterDialog::findStyles(const QFont &font)
{
    QFontDatabase fontDatabase;
    QString currentItem = styleCombo->currentText();
	if( currentItem.isEmpty() )
		currentItem = m_initFontStyle;
    styleCombo->clear();

    QString style;
    foreach( style, fontDatabase.styles( font.family() ) )
	{
        styleCombo->addItem( style );
	}

    int styleIndex = styleCombo->findText( currentItem );
	styleCombo->setCurrentIndex( styleIndex == -1 ? 0 : styleIndex );
}

void CInsertCharacterDialog::findSizes(const QFont &font)
{
    QFontDatabase fontDatabase;
    QString currentSize = sizeCombo->currentText();
	if( currentSize.isEmpty() )
		currentSize = QString::number( m_initFontSize );

	sizeCombo->blockSignals( true );
    sizeCombo->clear();
    int size;
    if( fontDatabase.isSmoothlyScalable( font.family(), fontDatabase.styleString( font )) )
	{
        foreach( size, QFontDatabase::standardSizes() )
		{
            sizeCombo->addItem( QVariant(size).toString() );
            sizeCombo->setEditable( true );
        }

    }
	else
	{
        foreach( size, fontDatabase.smoothSizes( font.family(), fontDatabase.styleString( font ) ) )
		{
            sizeCombo->addItem( QVariant(size).toString() );
            sizeCombo->setEditable( false );
        }
    }
    sizeCombo->blockSignals( false );

    int sizeIndex = sizeCombo->findText( currentSize );
	sizeCombo->setCurrentIndex( sizeIndex == -1 ? qMax(0, sizeCombo->count() / 3) : sizeIndex );
}

void CInsertCharacterDialog::insertCharacter(const QString &character)
{
    lineEdit->insert(character);
}

void CInsertCharacterDialog::updateClipboard()
{
    clipboard->setText( lineEdit->text(), QClipboard::Clipboard );
    clipboard->setText( lineEdit->text(), QClipboard::Selection );
}

void CInsertCharacterDialog::updateDocument( bool styledText )
{
	updateClipboard();
	m_text = lineEdit->text();
	m_hasStyledText = styledText;
	accept();
}

/////////////////////////////////////////////////////////////////////////////
