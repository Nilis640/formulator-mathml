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

#include "definestyledlg.h"
#include "HUtils/idrawstruct.h"
#include "HMathML/isettings.h"

/////////////////////////////////////////////////////////////////////////////


CDefineStyleDialog::CDefineStyleDialog( QColor backColor, int styleNumber, const QString* styleTitle, const int* styleID, QWidget *parent )
	: QDialog( parent )
	, m_styleNumber( styleNumber )
	, m_styleTitle( styleTitle )
	, m_styleID( styleID )
	, m_backColor( backColor )
{
	Q_ASSERT( styleNumber > 0 );

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Vertical);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	connect(buttonBox->addButton(tr("&Reset"), QDialogButtonBox::ResetRole), SIGNAL(clicked()), this, SLOT(factorySettings()));

	QGridLayout *centralLayout = new QGridLayout();
	int col = 0;

	for( int i = 0; i < styleNumber; i++ )
	{
		const FS_LogFont_Color& lfc = ::getFontStyle( m_styleID[ i ] );
		QFont font( lfc.m_name, (int) ::getCurrentKegl(), lfc.m_isBold ? QFont::Bold : QFont::Normal, lfc.m_isItalic ? true : false );
		font.setFixedPitch( lfc.m_isFixed );
		font.setStyleHint( lfc.m_hint );
		m_currentFont.push_back( font );
		m_currentStyle.push_back( QFontDatabase().styleString( font ) );
		m_currentColor.push_back( lfc.m_color );

		col = 0;
		QLabel *fontLabel = new QLabel(m_styleTitle[ i ] + ":");
		m_label.push_back( fontLabel );

		QFontComboBox* fontCombo = new QFontComboBox();
		fontLabel->setBuddy( fontCombo );
		fontCombo->setCurrentFont( font );
		m_font.push_back( fontCombo );

		QComboBox* styleCombo = new QComboBox();
		m_style.push_back( styleCombo );

		QPushButton *colorButton = new QPushButton(tr("&Set Color"));
		colorButton->setIcon(QIcon(":/images/setcolor.png"));
		m_btn.push_back( colorButton );

		QLabel* labelSampleText = new QLabel();
		labelSampleText->setFrameShape( QFrame::Box );
		labelSampleText->setBackgroundRole( QPalette::Light );
		labelSampleText->setAutoFillBackground( true );
		m_sample.push_back( labelSampleText );

		if( i )
		{
			centralLayout->addWidget( fontLabel, i - 1, col++, Qt::AlignRight );
			centralLayout->addWidget( fontCombo, i - 1, col++, Qt::AlignLeft );
			centralLayout->addWidget( styleCombo, i - 1, col++, Qt::AlignLeft );
			centralLayout->addWidget( colorButton, i - 1, col++, Qt::AlignLeft );
			centralLayout->addWidget( labelSampleText, i - 1, col++, Qt::AlignLeft );
			findStyles( font, i );
			updateSampleText( i );

			connect(fontCombo, SIGNAL(currentFontChanged(const QFont &)), this, SLOT(findStyles(const QFont &)));
			connect(fontCombo, SIGNAL(currentFontChanged(const QFont &)), this, SLOT(updateFont(const QFont &)));
			connect(styleCombo, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(updateStyle(const QString &)));
			connect(colorButton, SIGNAL(clicked()), this, SLOT(updateColor()));
		}
	}
	alignSampleText();

	centralLayout->setVerticalSpacing(8);
	centralLayout->setHorizontalSpacing(16);
	QHBoxLayout *mainLayout = new QHBoxLayout();
	mainLayout->addLayout( centralLayout );
	mainLayout->addWidget( buttonBox );
	setLayout(mainLayout);
	setWindowTitle(tr("Define Style"));
}

CDefineStyleDialog::~CDefineStyleDialog()
{
}

/////////////////////////////////////////////////////////////////////////////

void CDefineStyleDialog::updateFont( const QFont &font )
{
	int i;
	for( i = 0; i < m_font.size(); i++ )
	{
		if( m_font[ i ] == sender() )
			break;
	}
	if( i < m_font.size() )
	{
		m_currentFont[ i ] = font;
		updateStyle( m_currentStyle[ i ], i );
		updateSampleText( i );
	}
}

void CDefineStyleDialog::updateStyle( const QString &fontStyle )
{
	int i;
	for( i = 0; i < m_style.size(); i++ )
	{
		if( m_style[ i ] == sender() )
			break;
	}
	if( i < m_style.size() )
		updateStyle( fontStyle, i );
}

void CDefineStyleDialog::updateStyle( const QString &fontStyle, int i )
{
	m_currentStyle[ i ] = fontStyle;
	QFontDatabase fontDatabase;
	const QFont::StyleStrategy oldStrategy = m_currentFont[ i ].styleStrategy();
	m_currentFont[ i ] = fontDatabase.font( m_currentFont[ i ].family(), m_currentStyle[ i ], m_currentFont[ i ].pointSize() );
	m_currentFont[ i ].setStyleStrategy( oldStrategy );
	updateSampleText( i );
}

void CDefineStyleDialog::updateColor()
{
	int i;
	for( i = 0; i < m_btn.size(); i++ )
	{
		if( m_btn[ i ] == sender() )
			break;
	}
	if( i < m_btn.size() )
	{
		m_currentColor[ i ] = QColorDialog::getColor( m_currentColor[ i ], this );
		updateSampleText( i );
	}
}

void CDefineStyleDialog::updateSampleText( int i )
{
	QString text = QString("<span style=\"background-color: %1; color: %2; font-size: %3pt; font-family: %4; font-weight: %5; font-style: %6;\">Sample Text</span>")
						.arg(m_backColor.name())
						.arg(m_currentColor[ i ].name())
						.arg(m_currentFont[ i ].pointSize())
						.arg(m_currentFont[ i ].family())
						.arg(m_currentFont[ i ].bold() ? "bold" : "normal")
						.arg(m_currentFont[ i ].italic() ? "italic" : "normal");
	m_sample[ i ]->setText( text );
}

void CDefineStyleDialog::alignSampleText()
{
	return;
	int i, width = 0;
	for( i = 0; i < m_sample.size(); i++ )
		width = qMax( width, m_sample[ i ]->width() );
	for( i = 0; i < m_sample.size(); i++ )
		m_sample[ i ]->setMinimumSize( width, 0 );
}

void CDefineStyleDialog::findStyles( const QFont &font )
{
	int i;
	for( i = 0; i < m_font.size(); i++ )
	{
		if( m_font[ i ] == sender() )
			break;
	}
	if( i < m_font.size() )
		findStyles( font, i );
}

void CDefineStyleDialog::findStyles( const QFont &font, int i )
{
	QFontDatabase fontDatabase;
	QString currentItem = m_style[ i ]->currentText();
	if( currentItem.isEmpty() )
		currentItem = m_currentStyle[ i ];
	m_style[ i ]->clear();

	QString style;
	foreach( style, fontDatabase.styles( font.family() ) )
		m_style[ i ]->addItem( style );

	int styleIndex = m_style[ i ]->findText( currentItem );
	m_style[ i ]->setCurrentIndex( styleIndex == -1 ? 0 : styleIndex );
}

/////////////////////////////////////////////////////////////////////////////

void CDefineStyleDialog::accept()
{
	int i;
	for( i = 1; i < m_styleNumber; i++ )
	{
		::getCurrentFormulatorStyle().getFontStyle( m_styleID[i] ).m_name = m_currentFont[ i ].family();
		::getCurrentFormulatorStyle().getFontStyle( m_styleID[i] ).m_isBold = m_currentFont[ i ].bold();
		::getCurrentFormulatorStyle().getFontStyle( m_styleID[i] ).m_isItalic = m_currentFont[ i ].italic();
		::getCurrentFormulatorStyle().getFontStyle( m_styleID[i] ).m_color = m_currentColor[ i ];
		::getCurrentFormulatorStyle().getFontStyle( m_styleID[i] ).m_isFixed = m_currentFont[ i ].fixedPitch();
	}

	QDialog::accept();
}

void CDefineStyleDialog::factorySettings()
{
	FS_TextStyles defaultStyle;
	int idx;
	for( int i = 1; i < m_styleNumber; i++ )
	{
		const FS_LogFont_Color& lfc = defaultStyle.getFontStyle( m_styleID[ i ] );
		QFont font( lfc.m_name, (int) ::getCurrentKegl(), lfc.m_isBold ? QFont::Bold : QFont::Normal, lfc.m_isItalic ? true : false );
		font.setFixedPitch( lfc.m_isFixed );
		font.setStyleHint( lfc.m_hint );
		m_currentFont[ i ] = font;
		m_currentStyle[ i ] = QFontDatabase().styleString( font );
		m_currentColor[ i ] = lfc.m_color;
		m_font[ i ]->blockSignals( true );
		m_font[ i ]->setCurrentFont( font );
		m_font[ i ]->blockSignals( false );
		m_style[ i ]->blockSignals( true );
		if( (idx = m_style[ i ]->findText( m_currentStyle[ i ] )) != -1 )
			m_style[ i ]->setCurrentIndex( idx );
		m_style[ i ]->blockSignals( false );
		updateSampleText( i );
	}
}

/////////////////////////////////////////////////////////////////////////////
