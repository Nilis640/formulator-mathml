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

#include "definesizedlg.h"
#include "HUtils/idrawstruct.h"
#include "HMathML/isettings.h"

/////////////////////////////////////////////////////////////////////////////

CDefineSizeDialog::CDefineSizeDialog( QWidget *parent )
	: QDialog( parent )
{
    QLabel *largeLabel = new QLabel(tr("&Large Symbol (pt):"));
	largeEdit = new QSpinBox();
	largeEdit->setRange(DEFAULT_SYMBOLSIZE_MIN, DEFAULT_SYMBOLSIZE_MAX);
	largeEdit->setValue( (int) ::getKegl(FTEXT_HEIGHT_LARGEOP) );
	largeLabel->setBuddy( largeEdit );

	QLabel *regularLabel = new QLabel(tr("&Regular (pt):"));
	regularEdit = new QSpinBox();
	regularEdit->setRange(DEFAULT_SYMBOLSIZE_MIN, DEFAULT_SYMBOLSIZE_MAX);
	regularEdit->setValue( (int) ::getKegl(FTEXT_HEIGHT_FRAME) );
	regularLabel->setBuddy( regularEdit );

	QLabel *subLabel = new QLabel(tr("&Subscript (pt):"));
	subEdit = new QSpinBox();
	subEdit->setRange(DEFAULT_SYMBOLSIZE_MIN, DEFAULT_SYMBOLSIZE_MAX);
	subEdit->setValue( (int) ::getKegl(FTEXT_HEIGHT_CHILD1) );
	subLabel->setBuddy( subEdit );

	QLabel *subsubLabel = new QLabel(tr("Su&b-Subscript (pt):"));
	subsubEdit = new QSpinBox();
	subsubEdit->setRange(DEFAULT_SYMBOLSIZE_MIN, DEFAULT_SYMBOLSIZE_MAX);
	subsubEdit->setValue( (int) ::getKegl(FTEXT_HEIGHT_CHILD2) );
	subsubLabel->setBuddy( subsubEdit );

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Vertical);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	connect(buttonBox->addButton(tr("&Reset"), QDialogButtonBox::ResetRole), SIGNAL(clicked()), this, SLOT(factorySettings()));

	QGridLayout *centralLayout = new QGridLayout();
	centralLayout->addWidget( largeLabel, 0, 0, Qt::AlignRight );
	centralLayout->addWidget( largeEdit, 0, 1, Qt::AlignLeft );
	centralLayout->addWidget( regularLabel, 1, 0, Qt::AlignRight );
	centralLayout->addWidget( regularEdit, 1, 1, Qt::AlignLeft );
	centralLayout->addWidget( subLabel, 2, 0, Qt::AlignRight );
	centralLayout->addWidget( subEdit, 2, 1, Qt::AlignLeft );
	centralLayout->addWidget( subsubLabel, 3, 0, Qt::AlignRight );
	centralLayout->addWidget( subsubEdit, 3, 1, Qt::AlignLeft );
	centralLayout->setVerticalSpacing(8);
	centralLayout->setHorizontalSpacing(16);
	QHBoxLayout *mainLayout = new QHBoxLayout();
	mainLayout->addLayout( centralLayout );
	mainLayout->addWidget( buttonBox );
	setLayout(mainLayout);

	setWindowTitle(tr("Define Size"));
}

CDefineSizeDialog::~CDefineSizeDialog()
{
}

void CDefineSizeDialog::accept()
{
	::getCurrentFormulatorStyle().setKegl( FTEXT_HEIGHT_LARGEOP, largeEdit->value() );
	::getCurrentFormulatorStyle().setKegl( FTEXT_HEIGHT_FRAME, regularEdit->value() );
	::getCurrentFormulatorStyle().setKegl( FTEXT_HEIGHT_CHILD1, subEdit->value() );
	::getCurrentFormulatorStyle().setKegl( FTEXT_HEIGHT_CHILD2, subsubEdit->value() );
	QDialog::accept();
}

void CDefineSizeDialog::factorySettings()
{
	largeEdit->setValue( (int) FKEGL_LARGEOP );
	regularEdit->setValue( (int) FKEGL_FRAME );
	subEdit->setValue( (int) FKEGL_CHILD1 );
	subsubEdit->setValue( (int) FKEGL_CHILD2 );
}

/////////////////////////////////////////////////////////////////////////////
