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

#include "HMathML/isettings.h"

#include "mmltexteditwidget.h"
#include "mathmlhighlighter.h"

QMathMLTextEdit::QMathMLTextEdit( bool isReadOnly, const QString& text, QWidget *parent )
    : QDockWidget( tr("MathML Text Editor"), parent )
	, m_editor( 0 )
	, m_wrapMode( true )
	, m_isDelayedUpdate( false )
{
	m_isReadOnly = isReadOnly;
    setupEditor( m_isReadOnly );
	setText( text );

	//setupActions();
	setupCentralWidget();
	setupConnection();
}

/////////////////////////////////////////////////////////////////////////////

void QMathMLTextEdit::setUpdateMode( bool enabled )
{
	if( enabled )
		::setOption_UpdateMathMLView();
	else
		::clearOption_UpdateMathMLView();
}

/////////////////////////////////////////////////////////////////////////////

void QMathMLTextEdit::upload()
{
	emit uploadRequested();
}

void QMathMLTextEdit::load()
{
	emit loadRequested();
}

/////////////////////////////////////////////////////////////////////////////

void QMathMLTextEdit::setWrapMode( bool enabled )
{
	m_wrapMode = enabled;
	if( m_editor )
		m_editor->setWordWrapMode( isWrapMode() ? QTextOption::WrapAtWordBoundaryOrAnywhere : QTextOption::NoWrap );
}

bool QMathMLTextEdit::isWrapMode()
{
	return m_wrapMode;
}

void QMathMLTextEdit::setHighlightMode( bool enabled )
{
	if( m_highlighter )
	{
		QApplication::setOverrideCursor( Qt::WaitCursor );
		m_highlighter->setHighlightMode( enabled );
		QApplication::restoreOverrideCursor();
	}
}

/////////////////////////////////////////////////////////////////////////////

void QMathMLTextEdit::setText( const QString& text )
{
	setDelayedUpdate( false );
	if( m_editor->toPlainText() != text )
		m_editor->setPlainText( text );
}

void QMathMLTextEdit::focusInEvent (QFocusEvent *e)
{
	if( e->gotFocus() && delayedUpdate() )
	{
		load();
	}
}

QString QMathMLTextEdit::text()
{
	return m_editor->toPlainText();
}

bool QMathMLTextEdit::delayedUpdate()
{
	return m_isDelayedUpdate;
}

void QMathMLTextEdit::setDelayedUpdate( bool enable )
{
	m_isDelayedUpdate = enable;
}

void QMathMLTextEdit::setupEditor( bool isReadOnly )
{
	const FS_LogFont_Color& lfc = ::getFontStyle( FSTYLE_FIXED );
	QFont font( lfc.m_name, 10, /*QFont::Bold*/QFont::Normal, false );
	font.setFixedPitch( lfc.m_isFixed );
	font.setStyleHint( lfc.m_hint );

    m_editor = new QTextEdit;
    m_editor->setFont(font);
	m_editor->setReadOnly( isReadOnly );
	m_editor->setAcceptRichText( false );

	setWrapMode( true );

	m_highlighter = new QMathMLHighlighter( m_editor->document() );
}

/////////////////////////////////////////////////////////////////////////////

//void QMathMLTextEdit::setupActions()
//{
//    undoAct = new QAction(QIcon(":/images/undo.png"), tr("Undo"), this);
//    undoAct->setStatusTip(tr("Undo the last action"));
//    redoAct = new QAction(QIcon(":/images/redo.png"), tr("Redo"), this);
//    redoAct->setStatusTip(tr("Redo the previously undone action"));
//    cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
//    cutAct->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
//    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
//    copyAct->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
//    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
//    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
//    pasteAct->setEnabled(!QApplication::clipboard()->text().isEmpty());
//    uploadAct = new QAction(QIcon(":/images/reload.png"), tr("&Upload"), this);
//    uploadAct->setStatusTip(tr("Copy editor contents as an expression to the current document"));
//}

void QMathMLTextEdit::clipboardDataChanged()
{
    //pasteAct->setEnabled(!QApplication::clipboard()->text().isEmpty());
	btnPaste->setEnabled(!QApplication::clipboard()->text().isEmpty());
}

/////////////////////////////////////////////////////////////////////////////

QHBoxLayout* QMathMLTextEdit::setupToolLayout()
{
	QHBoxLayout *toolLayout = new QHBoxLayout();

	btnUndo = new QToolButton();
	btnUndo->setIcon(QIcon(":/images/undo.png"));
	btnUndo->setToolTip(tr("Undo the last action"));
	connect(btnUndo, SIGNAL(clicked()), m_editor, SLOT(undo()));
	toolLayout->addWidget( btnUndo );

	btnRedo = new QToolButton();
	btnRedo->setIcon(QIcon(":/images/redo.png"));
	btnRedo->setToolTip(tr("Redo the previously undone action"));
	connect(btnRedo, SIGNAL(clicked()), m_editor, SLOT(redo()));
	toolLayout->addWidget( btnRedo );

	toolLayout->addSpacing( 6 );

	btnCut = new QToolButton();
	btnCut->setIcon(QIcon(":/images/cut.png"));
	btnCut->setToolTip(tr("Cut the current selection's contents to the clipboard"));
	connect(btnCut, SIGNAL(clicked()), m_editor, SLOT(cut()));
	toolLayout->addWidget( btnCut );

	btnCopy = new QToolButton();
	btnCopy->setIcon(QIcon(":/images/copy.png"));
	btnCopy->setToolTip(tr("Copy the current selection's contents to the clipboard"));
	connect(btnCopy, SIGNAL(clicked()), m_editor, SLOT(copy()));
	toolLayout->addWidget( btnCopy );

	btnPaste = new QToolButton();
	btnPaste->setIcon(QIcon(":/images/paste.png"));
	btnPaste->setToolTip(tr("Paste the clipboard's contents into the current selection"));
	connect(btnPaste, SIGNAL(clicked()), m_editor, SLOT(paste()));
	toolLayout->addWidget( btnPaste );

	toolLayout->addSpacing( 6 );

	btnUpload = new QToolButton();
	btnUpload->setIcon(QIcon(":/images/send.png"));
	btnUpload->setToolTip(tr("Send editor MathML text to the current document"));
	connect(btnUpload, SIGNAL(clicked()), this, SLOT(upload()));
	toolLayout->addWidget( btnUpload );

	btnLoad = new QToolButton();
	btnLoad->setIcon(QIcon(":/images/get.png"));
	btnLoad->setToolTip(tr("Update MathML text of the editor according to the current formula contents"));
	connect(btnLoad, SIGNAL(clicked()), this, SLOT(load()));
	toolLayout->addWidget( btnLoad );

	toolLayout->addSpacing( 6 );

	btnUpdateMode = new QToolButton();
	btnUpdateMode->setObjectName("__qt__fmlide_widget_QMathMLTextEdit_button_UpdateMode");
	btnUpdateMode->setText("Update Mode");
	btnUpdateMode->setIcon(QIcon(":/images/connect.png"));
	btnUpdateMode->setToolTip(tr("Set on/off \"on-the-fly\" update mode for the MathML Editor"));
	btnUpdateMode->setToolButtonStyle( Qt::ToolButtonIconOnly );
	btnUpdateMode->setCheckable( true );
	btnUpdateMode->setChecked( ::isOption_UpdateMathMLView() );
	connect(btnUpdateMode, SIGNAL(toggled(bool)), this, SLOT(setUpdateMode(bool)));
	toolLayout->addWidget( btnUpdateMode );

	btnWrapMode = new QToolButton();
	btnWrapMode->setObjectName("__qt__fmlide_widget_QMathMLTextEdit_button_WrapMode");
	btnWrapMode->setText("Wrapped");
	btnWrapMode->setIcon(QIcon(":/images/contents.png"));
	btnWrapMode->setToolTip(tr("Set on/off text wrap mode"));
	//btnWrapMode->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
	btnWrapMode->setToolButtonStyle( Qt::ToolButtonIconOnly );
	btnWrapMode->setCheckable( true );
	btnWrapMode->setChecked( isWrapMode() );
	connect(btnWrapMode, SIGNAL(toggled(bool)), this, SLOT(setWrapMode(bool)));
	toolLayout->addWidget( btnWrapMode );

	if( m_highlighter )
	{
		btnColorMode = new QToolButton();
		btnColorMode->setObjectName("__qt__fmlide_widget_QMathMLTextEdit_button_ColorMode");
		btnColorMode->setText("Highlighted");
		btnColorMode->setIcon(QIcon(":/images/colorize.png"));
		btnColorMode->setToolTip(tr("Set on/off text highlighting"));
		btnColorMode->setToolButtonStyle( Qt::ToolButtonIconOnly );
		btnColorMode->setCheckable( true );
		btnColorMode->setChecked( m_highlighter->isHighlightMode() );
		connect(btnColorMode, SIGNAL(toggled(bool)), this, SLOT(setHighlightMode(bool)));
		toolLayout->addWidget( btnColorMode );
	}

	toolLayout->addStretch( 1 );

	return toolLayout;
}

void QMathMLTextEdit::setupCentralWidget()
{
	QDialog *centralDlg = new QDialog();
	centralDlg->setModal( false );
	QHBoxLayout *toolLayout = setupToolLayout();
	QVBoxLayout *centralLayout = new QVBoxLayout();
	centralLayout->addLayout( toolLayout );
	centralLayout->addWidget( m_editor, 1 );
	centralDlg->setLayout( centralLayout );
	setWidget( centralDlg );
}

void QMathMLTextEdit::setupConnection()
{
	connect(m_editor->document(), SIGNAL(undoAvailable(bool)), btnUndo, SLOT(setEnabled(bool)));
	connect(m_editor->document(), SIGNAL(redoAvailable(bool)), btnRedo, SLOT(setEnabled(bool)));
	btnCut->setEnabled(false);
	btnCopy->setEnabled(false);
	connect(m_editor, SIGNAL(copyAvailable(bool)), btnCut, SLOT(setEnabled(bool)));
	connect(m_editor, SIGNAL(copyAvailable(bool)), btnCopy, SLOT(setEnabled(bool)));

	//connect(uploadAct, SIGNAL(triggered()), this, SLOT(upload()));

	//connect(m_editor->document(), SIGNAL(undoAvailable(bool)), undoAct, SLOT(setEnabled(bool)));
	//connect(m_editor->document(), SIGNAL(redoAvailable(bool)), redoAct, SLOT(setEnabled(bool)));
	//connect(undoAct, SIGNAL(triggered()), m_editor, SLOT(undo()));
	//connect(redoAct, SIGNAL(triggered()), m_editor, SLOT(redo()));

	//cutAct->setEnabled(false);
	//copyAct->setEnabled(false);

	//connect(cutAct, SIGNAL(triggered()), m_editor, SLOT(cut()));
	//connect(copyAct, SIGNAL(triggered()), m_editor, SLOT(copy()));
	//connect(pasteAct, SIGNAL(triggered()), m_editor, SLOT(paste()));

	//connect(m_editor, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));
	//connect(m_editor, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));

	connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));
}

/////////////////////////////////////////////////////////////////////////////

void QMathMLTextEdit::readSettings( QSettings& settings )
{
	setWrapMode(settings.value("FmlIde/mmleditor/wrapMode", QVariant(true)).toBool());
	if( btnWrapMode ) btnWrapMode->setChecked( isWrapMode() );

	if( m_highlighter )
	{
		m_highlighter->setHighlightMode(settings.value("FmlIde/mmleditor/colorMode", QVariant(true)).toBool());
		if( btnColorMode ) btnColorMode->setChecked( m_highlighter->isHighlightMode() );
	}
}

void QMathMLTextEdit::writeSettings( QSettings& settings )
{
	settings.setValue("FmlIde/mmleditor/wrapMode", isWrapMode());
	settings.setValue("FmlIde/mmleditor/colorMode", m_highlighter ? m_highlighter->isHighlightMode() : false);
}

/////////////////////////////////////////////////////////////////////////////
