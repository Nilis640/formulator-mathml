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

#include "otherstyledlg.h"
#include "HUtils/idrawstruct.h"
#include "HMathML/isettings.h"

/////////////////////////////////////////////////////////////////////////////

COtherStyleDialog::COtherStyleDialog( QColor backColor, QWidget *parent )
	: QDialog( parent )
	, m_lfc( ::getFontStyle( ::getCurrentUserChoiceStyle() ) )
	, m_currentFont( m_lfc.m_name, (int) ::getCurrentKegl(), m_lfc.m_isBold ? QFont::Bold : QFont::Normal, m_lfc.m_isItalic ? true : false )
{
	m_currentFont.setFixedPitch( m_lfc.m_isFixed );
	m_currentFont.setStyleHint( m_lfc.m_hint );
	m_currentColor = m_lfc.m_color;
	m_backColor = backColor;
	m_currentStyle = QFontDatabase().styleString( m_currentFont );

    QLabel *fontLabel = new QLabel(tr("Font:"));
    fontCombo = new QFontComboBox();
	fontCombo->setCurrentFont( m_currentFont );
    QLabel *styleLabel = new QLabel(tr("Style:"));
    styleCombo = new QComboBox;
    findStyles( fontCombo->currentFont() );

	QPushButton *colorButton = new QPushButton(tr("&Set Color"));
	colorButton->setIcon(QIcon(":/images/setcolor.png"));
	//QLabel *labelSampleTextTitle = new QLabel("Sample text:");
	labelSampleText = new QLabel();
	labelSampleText->setFrameShape( QFrame::Box );
	labelSampleText->setBackgroundRole( QPalette::Light );
	labelSampleText->setAutoFillBackground( true );
	updateSampleText();

    QPushButton *okButton = new QPushButton(tr("O&k"));
	//okButton->setIcon(QIcon(":/images/ok.png"));
    //QPushButton *applyButton = new QPushButton(tr("&Apply"));
	//applyButton->setIcon(QIcon(":/images/wizard.png"));
    QPushButton *closeButton = new QPushButton(tr("&Cancel"));
	//closeButton->setIcon(QIcon(":/images/exit.png"));

	connect(fontCombo, SIGNAL(currentFontChanged(const QFont &)), this, SLOT(findStyles(const QFont &)));
    connect(fontCombo, SIGNAL(currentFontChanged(const QFont &)), this, SLOT(updateFont(const QFont &)));
    connect(styleCombo, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(updateStyle(const QString &)));
	connect(colorButton, SIGNAL(clicked()), this, SLOT(updateColor()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    //connect(applyButton, SIGNAL(clicked()), this, SLOT(apply()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *controlsLayout1 = new QHBoxLayout();
    controlsLayout1->addWidget(fontLabel);
    controlsLayout1->addWidget(fontCombo, 1);
    controlsLayout1->addWidget(styleLabel);
    controlsLayout1->addWidget(styleCombo, 1);
    controlsLayout1->addWidget(colorButton);
    controlsLayout1->addStretch(1);
    QHBoxLayout *controlsLayout2 = new QHBoxLayout();
    //controlsLayout2->addWidget(labelSampleTextTitle);
    //controlsLayout2->addStretch(1);
    controlsLayout2->addWidget(labelSampleText);
    QHBoxLayout *controlsLayout3 = new QHBoxLayout();
    controlsLayout3->addWidget(okButton);
    controlsLayout3->addWidget(closeButton);
    //controlsLayout3->addStretch(1);
    QVBoxLayout *controlsLayout = new QVBoxLayout();
	controlsLayout->addLayout(controlsLayout1);
	controlsLayout->addSpacing(8);
	controlsLayout->addLayout(controlsLayout2);
	controlsLayout->addSpacing(16);
	controlsLayout->addLayout(controlsLayout3);
	controlsLayout->setAlignment(controlsLayout3, Qt::AlignHCenter);
    controlsLayout->addStretch(1);

    //QHBoxLayout *centralLayout = new QHBoxLayout();
    //centralLayout->addLayout(controlsLayout);

    setLayout(controlsLayout/*centralLayout*/);
	setWindowTitle(tr("Select Other Style"));
}

COtherStyleDialog::~COtherStyleDialog()
{
}

void COtherStyleDialog::updateFont(const QFont &font)
{
	m_currentFont = font;
	updateStyle( m_currentStyle );
	updateSampleText();
}

void COtherStyleDialog::updateStyle(const QString &fontStyle)
{
	m_currentStyle = fontStyle;
    QFontDatabase fontDatabase;
    const QFont::StyleStrategy oldStrategy = m_currentFont.styleStrategy();
    m_currentFont = fontDatabase.font(m_currentFont.family(), fontStyle, m_currentFont.pointSize());
    m_currentFont.setStyleStrategy(oldStrategy);
	updateSampleText();
}

void COtherStyleDialog::updateSampleText()
{
	QString text = QString("<span style=\"background-color: %1; color: %2; font-size: %3pt; font-family: %4; font-weight: %5; font-style: %6;\">Sample Text: 2 + 2</span>")
						.arg(m_backColor.name())
						.arg(currentColor().name())
						.arg(currentFont().pointSize())
						.arg(currentFont().family())
						.arg(currentFont().bold() ? "bold" : "normal")
						.arg(currentFont().italic() ? "italic" : "normal");
	labelSampleText->setText( text );
}

void COtherStyleDialog::updateColor()
{
	m_currentColor = QColorDialog::getColor( currentColor(), this );
	updateSampleText();
}

void COtherStyleDialog::findStyles(const QFont &font)
{
    QFontDatabase fontDatabase;
    QString currentItem = styleCombo->currentText();
	if( currentItem.isEmpty() )
		currentItem = m_currentStyle;
    styleCombo->clear();

    QString style;
    foreach( style, fontDatabase.styles( font.family() ) )
	{
        styleCombo->addItem( style );
	}

    int styleIndex = styleCombo->findText( currentItem );
	styleCombo->setCurrentIndex( styleIndex == -1 ? 0 : styleIndex );
}

/////////////////////////////////////////////////////////////////////////////

void COtherStyleDialog::apply()
{
	
}

/////////////////////////////////////////////////////////////////////////////
