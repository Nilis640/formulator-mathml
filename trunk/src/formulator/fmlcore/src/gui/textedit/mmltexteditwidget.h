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

#ifndef __FMLCORE_DLG_MATHMLTEXTEDITWIDGET_H__
#define __FMLCORE_DLG_MATHMLTEXTEDITWIDGET_H__

#include <QDockWidget>

QT_BEGIN_NAMESPACE
class QAction;
class QToolButton;
class QPushButton;
class QHBoxLayout;
class QSettings;
class QTextEdit;
class QMathMLHighlighter;
QT_END_NAMESPACE

class QMathMLTextEdit
	: public QDockWidget
{
    Q_OBJECT

protected:
    QTextEdit			*m_editor;
    QMathMLHighlighter	*m_highlighter;
	bool				m_isReadOnly;
	bool				m_wrapMode;
	bool				m_isDelayedUpdate;

    //QAction *undoAct, *redoAct, *cutAct, *copyAct, *pasteAct, *uploadAct;
	QToolButton *btnUndo, *btnRedo, *btnCut, *btnCopy, *btnPaste, *btnUpload, *btnLoad, *btnColorMode;
	//QPushButton 
	QToolButton *btnWrapMode, *btnUpdateMode;

public:
    QMathMLTextEdit( bool isReadOnly = false, const QString &text = QString(), QWidget *parent = 0 );

	QString text();
	bool isWrapMode();

	bool delayedUpdate();
	void setDelayedUpdate( bool enable );

	void readSettings( QSettings& settings );
	void writeSettings( QSettings& settings );

public slots:
    void setText( const QString &text = QString() );

protected:
	//virtual void contextMenuEvent( QContextMenuEvent *e );
	virtual void focusInEvent( QFocusEvent * event );

    void setupEditor( bool isReadOnly );
	//void setupActions();
	QHBoxLayout* setupToolLayout();
	void setupCentralWidget();
	void setupConnection();

protected slots:
    void clipboardDataChanged();
	void upload();
	void load();
	void setWrapMode( bool enabled );
	void setHighlightMode( bool enabled );
	void setUpdateMode( bool enabled );

signals:
	void uploadRequested();
	void loadRequested();
};

#endif
