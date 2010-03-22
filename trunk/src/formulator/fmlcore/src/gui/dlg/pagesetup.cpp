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

#include "pagesetup.h"
#include "HMathML/isettings.h"

/////////////////////////////////////////////////////////////////////////////

CPageSetupDialog::CPageSetupDialog( RectFde rcIndents, RealFde linespacing, QWidget *parent )
	: QDialog( parent )
{
	QGroupBox *group = new QGroupBox(tr("On-Screen Document Margins:"));

    QLabel *leftLabel = new QLabel(tr("&Left (px):"));
	leftEdit = new QSpinBox();
	leftEdit->setMinimum( 0 );
	leftEdit->setValue( (int) rcIndents.left() );
	leftLabel->setBuddy( leftEdit );

	QLabel *topLabel = new QLabel(tr("&Top (px):"));
	topEdit = new QSpinBox();
	topEdit->setMinimum( 0 );
	topEdit->setValue( (int) rcIndents.top() );
	topLabel->setBuddy( topEdit );

	QLabel *rightLabel = new QLabel(tr("&Right (px):"));
	rightEdit = new QSpinBox();
	rightEdit->setMinimum( 0 );
	rightEdit->setValue( (int) rcIndents.right() );
	rightLabel->setBuddy( rightEdit );

	QLabel *bottomLabel = new QLabel(tr("&Bottom (px):"));
	bottomEdit = new QSpinBox();
	bottomEdit->setMinimum( 0 );
	bottomEdit->setValue( (int) rcIndents.bottom() );
	bottomLabel->setBuddy( bottomEdit );

	QLabel *linespacingLabel = new QLabel(tr("Line &Spacing (%):"));
	linespacingEdit = new QSpinBox();
	linespacingEdit->setRange(1, 1000);
	linespacingEdit->setValue( (int) (100.0 * linespacing) );
	linespacingLabel->setBuddy( linespacingEdit );

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Vertical);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	QGridLayout *centralLayout = new QGridLayout();
	centralLayout->addWidget( leftLabel, 0, 0, Qt::AlignRight );
	centralLayout->addWidget( leftEdit, 0, 1, Qt::AlignLeft );
	centralLayout->addWidget( topLabel, 1, 0, Qt::AlignRight );
	centralLayout->addWidget( topEdit, 1, 1, Qt::AlignLeft );
	centralLayout->addWidget( rightLabel, 2, 0, Qt::AlignRight );
	centralLayout->addWidget( rightEdit, 2, 1, Qt::AlignLeft );
	centralLayout->addWidget( bottomLabel, 3, 0, Qt::AlignRight );
	centralLayout->addWidget( bottomEdit, 3, 1, Qt::AlignLeft );
	centralLayout->setVerticalSpacing(8);
	centralLayout->setHorizontalSpacing(16);
	group->setLayout( centralLayout );

	QHBoxLayout *lsLayout = new QHBoxLayout();
	lsLayout->addWidget( linespacingLabel );
	lsLayout->addWidget( linespacingEdit );

	QPushButton *pageSetup = new QPushButton(tr("&Print Page Setup..."));
	connect(pageSetup, SIGNAL(clicked()), this, SLOT(pageSetup()));

	QVBoxLayout *vertLayout = new QVBoxLayout();
	vertLayout->addWidget( group );
	vertLayout->addSpacing( 8 );
	vertLayout->addLayout( lsLayout );
	vertLayout->addSpacing( 8 );
	vertLayout->addWidget( pageSetup );
	QHBoxLayout *mainLayout = new QHBoxLayout();
	mainLayout->addLayout( vertLayout );
	mainLayout->addSpacing( 20 );
	mainLayout->addWidget( buttonBox );
	setLayout(mainLayout);

	setWindowTitle(tr("Document Page Setup"));
}

CPageSetupDialog::~CPageSetupDialog()
{
}

RectFde CPageSetupDialog::indents()
{
	return RectFde( PointFde(leftEdit->value(), topEdit->value()), PointFde(rightEdit->value(), bottomEdit->value()) );
}

RealFde CPageSetupDialog::lineSpacing()
{
	return linespacingEdit->value() / 100.0;
}

/////////////////////////////////////////////////////////////////////////////

void CPageSetupDialog::pageSetup()
{
	QPageSetupDialog dlg;
	dlg.exec();
}

void CPageSetupDialog::accept()
{
	QDialog::accept();
}

/////////////////////////////////////////////////////////////////////////////
