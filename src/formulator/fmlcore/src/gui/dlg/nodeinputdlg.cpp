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

#include "nodeinputdlg.h"
#include "HMathML/isettings.h"

/////////////////////////////////////////////////////////////////////////////

CMmlDeclareDlg::CMmlDeclareDlg( QWidget *parent )
	: QDialog( parent )
{
	m_type = new QComboBox();
	m_type->setEditable( true );
	m_type->setDuplicatesEnabled( false );
	m_type->addItem("");
	m_type->addItem("integer");
	m_type->addItem("real");
	m_type->addItem("rational");
	m_type->addItem("complex-cartesian");
	m_type->addItem("complex-polar");
	m_type->addItem("complex");
	m_type->addItem("constant");
	m_type->addItem("function");
	m_type->addItem("interval");
	m_type->addItem("list");
	m_type->addItem("matrix");
	m_type->addItem("set");
	m_type->addItem("vector");
	m_type->setCurrentIndex( 0 );
    QLabel *typeLabel = new QLabel(tr("&Type:"));
	typeLabel->setBuddy( m_type );

    QLabel *argLabel = new QLabel(tr("&Arguments:"));
	m_arg = new QSpinBox();
	m_arg->setRange( 0, 30 );
	m_arg->setValue( 0 );
	argLabel->setBuddy( m_arg );

	m_occurance = new QComboBox();
	m_occurance->setEditable( false );
	m_occurance->setDuplicatesEnabled( false );
	m_occurance->addItem("<none>");
	m_occurance->addItem("prefix");
	m_occurance->addItem("infix");
	m_occurance->addItem("function-model");
	m_occurance->setCurrentIndex( 0 );
    QLabel *occuranceLabel = new QLabel(tr("&Occurance:"));
	occuranceLabel->setBuddy( m_occurance );

	m_isSecondSlot = new QCheckBox(tr("Insert a slot into the constructor for initializing of the variable"));
	m_isSecondSlot->setCheckState( Qt::Checked );

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Vertical);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	QGridLayout *upperLayout = new QGridLayout();
	upperLayout->addWidget( typeLabel, 0, 0, Qt::AlignRight );
	upperLayout->addWidget( m_type, 0, 1, Qt::AlignLeft );
	upperLayout->addWidget( argLabel, 1, 0, Qt::AlignRight );
	upperLayout->addWidget( m_arg, 1, 1, Qt::AlignLeft );
	upperLayout->addWidget( occuranceLabel, 2, 0, Qt::AlignRight );
	upperLayout->addWidget( m_occurance, 2, 1, Qt::AlignLeft );
	upperLayout->setVerticalSpacing(8);
	upperLayout->setHorizontalSpacing(16);

	QVBoxLayout *leftLayout = new QVBoxLayout();
	leftLayout->addLayout( upperLayout );
	leftLayout->addWidget( m_isSecondSlot );
	leftLayout->addStretch( 1 );

	QVBoxLayout *rightLayout = new QVBoxLayout();
	rightLayout->addWidget( buttonBox );
	rightLayout->addStretch( 1 );

	QHBoxLayout *centralLayout = new QHBoxLayout();
	centralLayout->addLayout( leftLayout );
	centralLayout->addLayout( rightLayout );

	setLayout(centralLayout);

	setWindowTitle(tr("Attributes of the 'declare' constructor"));
}

/////////////////////////////////////////////////////////////////////////////

CMmlCiDlg::CMmlCiDlg( QWidget *parent )
	: QDialog( parent )
{
	m_type = new QComboBox();
	m_type->setEditable( true );
	m_type->setDuplicatesEnabled( false );
	m_type->addItem("");
	m_type->addItem("integer");
	m_type->addItem("real");
	m_type->addItem("rational");
	m_type->addItem("complex-cartesian");
	m_type->addItem("complex-polar");
	m_type->addItem("complex");
	m_type->addItem("constant");
	m_type->addItem("function");
	m_type->addItem("interval");
	m_type->addItem("list");
	m_type->addItem("matrix");
	m_type->addItem("set");
	m_type->addItem("vector");
	m_type->setCurrentIndex( 0 );
    QLabel *typeLabel = new QLabel(tr("&Type:"));
	typeLabel->setBuddy( m_type );

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Vertical);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	QGridLayout *upperLayout = new QGridLayout();
	upperLayout->addWidget( typeLabel, 0, 0, Qt::AlignRight );
	upperLayout->addWidget( m_type, 0, 1, Qt::AlignLeft );
	upperLayout->setHorizontalSpacing(16);

	QVBoxLayout *leftLayout = new QVBoxLayout();
	leftLayout->addLayout( upperLayout );
	leftLayout->addStretch( 1 );

	QVBoxLayout *rightLayout = new QVBoxLayout();
	rightLayout->addWidget( buttonBox );
	rightLayout->addStretch( 1 );

	QHBoxLayout *centralLayout = new QHBoxLayout();
	centralLayout->addLayout( leftLayout );
	centralLayout->addLayout( rightLayout );

	setLayout(centralLayout);

	setWindowTitle(tr("Token element: Identifier"));
}

/////////////////////////////////////////////////////////////////////////////

CMmlCnDlg::CMmlCnDlg( QWidget *parent )
	: QDialog( parent )
{
	m_type = new QComboBox();
	m_type->setEditable( false );
	m_type->setDuplicatesEnabled( false );
	m_type->addItem("<none>");
	m_type->addItem("real");
	m_type->addItem("integer");
	m_type->addItem("rational");
	m_type->addItem("complex-cartesian");
	m_type->addItem("complex-polar");
	m_type->addItem("constant");
	m_type->setCurrentIndex( 0 );
    QLabel *typeLabel = new QLabel(tr("&Type:"));
	typeLabel->setBuddy( m_type );

    QLabel *baseLabel = new QLabel(tr("&Base:"));
	m_base = new QSpinBox();
	m_base->setRange( 2, 36 );
	m_base->setValue( 10 );
	baseLabel->setBuddy( m_base );

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Vertical);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	QGridLayout *upperLayout = new QGridLayout();
	upperLayout->addWidget( typeLabel, 0, 0, Qt::AlignRight );
	upperLayout->addWidget( m_type, 0, 1, Qt::AlignLeft );
	upperLayout->addWidget( baseLabel, 1, 0, Qt::AlignRight );
	upperLayout->addWidget( m_base, 1, 1, Qt::AlignLeft );
	upperLayout->setVerticalSpacing(8);
	upperLayout->setHorizontalSpacing(16);

	QVBoxLayout *leftLayout = new QVBoxLayout();
	leftLayout->addLayout( upperLayout );
	leftLayout->addStretch( 1 );

	QVBoxLayout *rightLayout = new QVBoxLayout();
	rightLayout->addWidget( buttonBox );
	rightLayout->addStretch( 1 );

	QHBoxLayout *centralLayout = new QHBoxLayout();
	centralLayout->addLayout( leftLayout );
	centralLayout->addLayout( rightLayout );

	setLayout(centralLayout);

	setWindowTitle(tr("Token element: Number"));
}

/////////////////////////////////////////////////////////////////////////////

CMmlQualifierDlg::CMmlQualifierDlg( QString title, bool argsOn, QWidget *parent )
	: QDialog( parent )
{
	m_op = new QComboBox();
	m_op->setEditable( false );
	m_op->setDuplicatesEnabled( false );
	m_op->addItem("and");
	m_op->addItem("cartesianproduct");
	m_op->addItem("compose");
	m_op->addItem("eq");
	m_op->addItem("gcd");
	m_op->addItem("geq");
	m_op->addItem("gt");
	m_op->addItem("int");
	m_op->addItem("intersect");
	m_op->addItem("lcm");
	m_op->addItem("leq");
	m_op->addItem("limit");
	m_op->addItem("lt");
	m_op->addItem("median");
	m_op->addItem("mode");
	m_op->addItem("or");
	m_op->addItem("plus");
	m_op->addItem("product");
	m_op->addItem("prsubset");
	m_op->addItem("sdev");
	m_op->addItem("subset");
	m_op->addItem("sum");
	m_op->addItem("times");
	m_op->addItem("union");
	m_op->addItem("variance");
	m_op->addItem("xor");
	m_op->setCurrentIndex( 0 );
    QLabel *opLabel = new QLabel(tr("&Operator:"));
	opLabel->setBuddy( m_op );

	QLabel *bvarLabel = new QLabel(tr("&Number of bvar elements:"), this);
	m_bvar = new QSpinBox( this );
	m_bvar->setRange( 1, 30 );
	m_bvar->setValue( 1 );
	bvarLabel->setBuddy( m_bvar );

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Vertical);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	QGridLayout *upperLayout = new QGridLayout();
	upperLayout->addWidget( opLabel, 0, 0, Qt::AlignRight );
	upperLayout->addWidget( m_op, 0, 1, Qt::AlignLeft );
	if( argsOn )
	{
		upperLayout->addWidget( bvarLabel, 1, 0, Qt::AlignRight );
		upperLayout->addWidget( m_bvar, 1, 1, Qt::AlignLeft );
	}
	else
	{
		bvarLabel->setEnabled( false );
		bvarLabel->setVisible( false );
		m_bvar->setEnabled( false );
		m_bvar->setVisible( false );
	}
	upperLayout->setVerticalSpacing(8);
	upperLayout->setHorizontalSpacing(16);

	QVBoxLayout *leftLayout = new QVBoxLayout();
	leftLayout->addLayout( upperLayout );
	leftLayout->addStretch( 1 );

	QVBoxLayout *rightLayout = new QVBoxLayout();
	rightLayout->addWidget( buttonBox );
	rightLayout->addStretch( 1 );

	QHBoxLayout *centralLayout = new QHBoxLayout();
	centralLayout->addLayout( leftLayout );
	centralLayout->addLayout( rightLayout );

	setLayout(centralLayout);

	setWindowTitle(title);
}

/////////////////////////////////////////////////////////////////////////////

CMmlTableDlg::CMmlTableDlg( bool isRows, int rowNumber, bool isColumns, int colNumber, QWidget *parent )
	: QDialog( parent )
{
	QLabel *rowsLabel = new QLabel(tr("&Rows:"), this);
	m_rows = new QSpinBox( this );
	m_rows->setRange( 1, 50 );
	m_rows->setValue( rowNumber > 0 ? rowNumber : 1 );
	rowsLabel->setBuddy( m_rows );

	QLabel *columnsLabel = new QLabel(tr("&Columns:"), this);
	m_columns = new QSpinBox( this );
	m_columns->setRange( 1, 50 );
	m_columns->setValue( colNumber > 0 ? colNumber : 1 );
	columnsLabel->setBuddy( m_columns );

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Vertical);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	QGridLayout *upperLayout = new QGridLayout();
	if( isRows )
	{
		upperLayout->addWidget( rowsLabel, 0, 0, Qt::AlignRight );
		upperLayout->addWidget( m_rows, 0, 1, Qt::AlignLeft );
	}
	else
	{
		rowsLabel->setEnabled( false );
		rowsLabel->setVisible( false );
		m_rows->setEnabled( false );
		m_rows->setVisible( false );
	}
	if( isColumns )
	{
		upperLayout->addWidget( columnsLabel, 1, 0, Qt::AlignRight );
		upperLayout->addWidget( m_columns, 1, 1, Qt::AlignLeft );
	}
	else
	{
		columnsLabel->setEnabled( false );
		columnsLabel->setVisible( false );
		m_columns->setEnabled( false );
		m_columns->setVisible( false );
	}
	upperLayout->setVerticalSpacing(8);
	upperLayout->setHorizontalSpacing(16);

	QVBoxLayout *leftLayout = new QVBoxLayout();
	leftLayout->addLayout( upperLayout );
	leftLayout->addStretch( 1 );

	QVBoxLayout *rightLayout = new QVBoxLayout();
	rightLayout->addWidget( buttonBox );
	rightLayout->addStretch( 1 );

	QHBoxLayout *centralLayout = new QHBoxLayout();
	centralLayout->addLayout( leftLayout );
	centralLayout->addLayout( rightLayout );

	setLayout(centralLayout);

	setWindowTitle(tr("Table Properties"));
}

/////////////////////////////////////////////////////////////////////////////

QMmlSelectorDlg::QMmlSelectorDlg( QString title, int minArgs, QWidget *parent )
	: QDialog( parent )
{
	QLabel *argcLabel = new QLabel(tr("&Arguments:"), this);
	m_argc = new QSpinBox( this );
	m_argc->setRange( minArgs, 30 );
	m_argc->setValue( minArgs );
	argcLabel->setBuddy( m_argc );

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Vertical);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	QGridLayout *upperLayout = new QGridLayout();
	upperLayout->addWidget( argcLabel, 0, 0, Qt::AlignRight );
	upperLayout->addWidget( m_argc, 0, 1, Qt::AlignLeft );
	upperLayout->setVerticalSpacing(8);
	upperLayout->setHorizontalSpacing(16);

	QVBoxLayout *leftLayout = new QVBoxLayout();
	leftLayout->addLayout( upperLayout );
	leftLayout->addStretch( 1 );

	QVBoxLayout *rightLayout = new QVBoxLayout();
	rightLayout->addWidget( buttonBox );
	rightLayout->addStretch( 1 );

	QHBoxLayout *centralLayout = new QHBoxLayout();
	centralLayout->addLayout( leftLayout );
	centralLayout->addLayout( rightLayout );

	setLayout(centralLayout);

	setWindowTitle(title);
}

/////////////////////////////////////////////////////////////////////////////
