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

#include <QtGlobal>
#include <QtGui>

#include "HUtils/idrawing.h"
#include "HMathML/isettings.h"
#include "HMathML/itoolbarset.h"
#include "HMathML/idictionary.h"

#include "fmleditwindow.h"
#include "../widget/fmleditwidget.h"
#include "../../version/version.h"

//////////////////////////////////////////////////////////////////////////////

QFormulatorEditWindow::QFormulatorEditWindow( QWidget *parent, QObject *uiUpdater )
	: QFormulatorViewWindow( 0, parent, uiUpdater )
{
	m_fmlEditWidget = new QFormulatorEditWidget( this, uiUpdater );
	QFormulatorViewWindow::init( m_fmlEditWidget, uiUpdater );
}

QFormulatorEditWindow::~QFormulatorEditWindow()
{
}

//////////////////////////////////////////////////////////////////////////////

void QFormulatorEditWindow::mousePressEvent( QMouseEvent *e )
{
	QScrollArea::mousePressEvent( e );
	fmlEditWidget()->customEvent_MousePressOnParentWindow( e );
	//if( geometry().contains(e->pos()) )
	//{
	//	QString str = QString("%1, %2").arg(e->pos().x()).arg(e->pos().y());
	//	qWarning(str.toAscii());
	//}
}

void QFormulatorEditWindow::mouseMoveEvent( QMouseEvent *e )
{
	QScrollArea::mouseMoveEvent( e );
	fmlEditWidget()->customEvent_MouseMoveOnParentWindow( e );
}

void QFormulatorEditWindow::focusInEvent( QFocusEvent *e )
{
	fmlEditWidget()->setFocus();
	QScrollArea::focusInEvent( e );
}

void QFormulatorEditWindow::contextMenuEvent( QContextMenuEvent *e )
{
    QMenu menu( this );
	if( undoAct ) menu.addAction( undoAct );
	if( redoAct ) menu.addAction( redoAct );
	if( !menu.isEmpty() )
		menu.addSeparator();
	if( cutAct ) menu.addAction( cutAct );
	if( copyAct ) menu.addAction( copyAct );
	if( pasteAct ) menu.addAction( pasteAct );
	if( cutAct || copyAct || pasteAct )
		menu.addSeparator();
	if( selectAllAct )
	{
		menu.addAction( selectAllAct );
	}
	if( evaluateAct && insertAssignAct )
	{
		menu.addSeparator();
		QMenu *eval = menu.addMenu(tr("&Calculation"));
		if( eval )
		{
			eval->setIcon(QIcon(":/images/calc.png"));
			eval->addAction( evaluateAct );
			eval->addAction( insertAssignAct );
		}
	}
	//if( propertiesAct )
	//{
	//	menu.addSeparator();
	//	menu.addAction( propertiesAct );
	//}
	if( !menu.isEmpty() )
		menu.exec( e->globalPos() );
}

//////////////////////////////////////////////////////////////////////////////

void QFormulatorEditWindow::undo()
{
	fmlEditWidget()->editUndo();
}

void QFormulatorEditWindow::redo()
{
	fmlEditWidget()->editRedo();
}

void QFormulatorEditWindow::cut()
{
	fmlEditWidget()->editCut();
}

void QFormulatorEditWindow::copy()
{
	fmlEditWidget()->editCopy();
}

void QFormulatorEditWindow::paste()
{
	fmlEditWidget()->editPaste();
}

void QFormulatorEditWindow::selectAll()
{
	fmlEditWidget()->editSelectAll();
}

//////////////////////////////////////////////////////////////////////////////

void QFormulatorEditWindow::evaluate()
{
	fmlWidget()->evaluate();
}

//////////////////////////////////////////////////////////////////////////////

void QFormulatorEditWindow::insertSymbol()
{
	fmlEditWidget()->insertSymbol();
}

void QFormulatorEditWindow::insertImage()
{
	fmlEditWidget()->insertImage();
}

void QFormulatorEditWindow::insertButton( const QString& buttonID )
{
	fmlEditWidget()->insertNewNode( buttonID );
}

//////////////////////////////////////////////////////////////////////////////

void QFormulatorEditWindow::setStyle( int no )
{
	if( !::isOtherStyle( no ) )
	{
		::setCurrentUserChoiceStyle( no );
		fmlWidget()->applyStyle( no );
		updateSelectStyleUI();
	}
}

void QFormulatorEditWindow::setStyleOther()
{
	if( fmlWidget()->setStyleOther() )
	{
		updateSelectStyleUI();
	}
}

//////////////////////////////////////////////////////////////////////////////

void QFormulatorEditWindow::setSize( int no )
{
	if( !::isOtherSizeIdx( no ) )
	{
		fmlWidget()->applyKegl( ::getKegl( no ) );
		updateSelectSizeUI();
	}
}

void QFormulatorEditWindow::setSizeLargeSymbol()
{
	setSize( FTEXT_HEIGHT_LARGEOP );
	updateSelectSizeUI();
}

void QFormulatorEditWindow::setSizeRegular()
{
	setSize( FTEXT_HEIGHT_FRAME );
	updateSelectSizeUI();
}

void QFormulatorEditWindow::setSizeSubscript()
{
	setSize( FTEXT_HEIGHT_CHILD1 );
	updateSelectSizeUI();
}

void QFormulatorEditWindow::setSizeSubSubscript()
{
	setSize( FTEXT_HEIGHT_CHILD2 );
	updateSelectSizeUI();
}

void QFormulatorEditWindow::setSizeOther()
{
	if( fmlWidget()->setSizeOther() )
		updateSelectSizeUI();
}

void QFormulatorEditWindow::setSizeSmaller()
{
	fmlWidget()->incDecKegl( -1 );
	updateSelectSizeUI();
}

void QFormulatorEditWindow::setSizeLarger()
{
	fmlWidget()->incDecKegl( 1 );
	updateSelectSizeUI();
}

//////////////////////////////////////////////////////////////////////////////

bool QFormulatorEditWindow::formatDefineStyle()
{
    if( fmlWidget()->formatDefineStyle( styleNumber, m_styleTitle, m_styleID ) )
	{
		updateSelectStyleCombo();
		return true;
	}
	return false;
}

bool QFormulatorEditWindow::formatDefineSize()
{
	return fmlWidget()->formatDefineSize() ? true : false;
}

QString QFormulatorEditWindow::caretInfo()
{
    return fmlEditWidget()->caretInfo();
}

//////////////////////////////////////////////////////////////////////////////

void QFormulatorEditWindow::initGui( QMainWindow *uiUpdater )
{
	QFormulatorEditWindow::initActions( uiUpdater );
	QFormulatorEditWindow::initCommonGui( uiUpdater );
	QFormulatorEditWindow::initXmlBtnGui( uiUpdater );
	QFormulatorEditWindow::adjustViewMenuToggleToolbars();
}

void QFormulatorEditWindow::initCommonGui( QMainWindow *uiUpdater )
{
	QFormulatorViewWindow::initFileMenu( uiUpdater );
	QFormulatorEditWindow::initEditMenu( uiUpdater );
	QFormulatorViewWindow::initViewMenu( uiUpdater );
	QFormulatorEditWindow::initInsertMenu( uiUpdater );
	QFormulatorEditWindow::initFormatMenu( uiUpdater );

	QFormulatorViewWindow::initFileToolbar( uiUpdater );
	QFormulatorEditWindow::initEditToolbar( uiUpdater );
	QFormulatorViewWindow::initExportToolbar( uiUpdater );
	QFormulatorViewWindow::initViewToolbar( uiUpdater );
	QFormulatorEditWindow::initInsertToolbar( uiUpdater );
	QFormulatorEditWindow::initFormatToolbar( uiUpdater );
}

void QFormulatorEditWindow::initActions( QMainWindow *uiUpdater )
{
	QFormulatorViewWindow::initActions( uiUpdater );

    evaluateAct = new QAction(tr("Evaluate current"), uiUpdater);
	evaluateAct->setShortcut( QKeySequence( Qt::CTRL | Qt::Key_Equal ) );
    evaluateAct->setStatusTip(tr("Insert evaluation of the Content MathML text under the caret"));
    connect(evaluateAct, SIGNAL(triggered()), uiUpdater, SLOT(evaluate()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), evaluateAct, SLOT(setEnabled(bool)));

	insertAssignAct = new QAction(tr("Assignment"), uiUpdater);
    insertAssignAct->setStatusTip(tr("Insert an empty assignment statement"));
    connect(insertAssignAct, SIGNAL(triggered()), uiUpdater, SLOT(insertAssign()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), insertAssignAct, SLOT(setEnabled(bool)));

	undoAct = new QAction(QIcon(":/images/undo.png"), tr("Undo"), uiUpdater);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last action"));
	undoAct->setEnabled( false );
    connect(undoAct, SIGNAL(triggered()), uiUpdater, SLOT(undo()));
	//connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), undoAct, SLOT(setEnabled(bool)));
	connect(uiUpdater, SIGNAL(undoAvailableSignal(bool)), undoAct, SLOT(setEnabled(bool)));

    redoAct = new QAction(QIcon(":/images/redo.png"), tr("Redo"), uiUpdater);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the previously undone action"));
	redoAct->setEnabled( false );
    connect(redoAct, SIGNAL(triggered()), uiUpdater, SLOT(redo()));
	//connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), redoAct, SLOT(setEnabled(bool)));
	connect(uiUpdater, SIGNAL(redoAvailableSignal(bool)), redoAct, SLOT(setEnabled(bool)));

    cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), uiUpdater);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
	cutAct->setEnabled( false );
    connect(cutAct, SIGNAL(triggered()), uiUpdater, SLOT(cut()));
	//connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), cutAct, SLOT(setEnabled(bool)));
	connect(uiUpdater, SIGNAL(copyAvailableSignal(bool)), cutAct, SLOT(setEnabled(bool)));

    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), uiUpdater);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
	copyAct->setEnabled( false );
    connect(copyAct, SIGNAL(triggered()), uiUpdater, SLOT(copy()));
	//connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), copyAct, SLOT(setEnabled(bool)));
	connect(uiUpdater, SIGNAL(copyAvailableSignal(bool)), copyAct, SLOT(setEnabled(bool)));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), uiUpdater);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
	pasteAct->setEnabled( false );
    connect(pasteAct, SIGNAL(triggered()), uiUpdater, SLOT(paste()));
	connect(uiUpdater, SIGNAL(hasClipboardForChildSignal(bool)), pasteAct, SLOT(setEnabled(bool)));

    selectAllAct = new QAction(tr("Select All"), uiUpdater);
    selectAllAct->setShortcuts(QKeySequence::SelectAll);
    selectAllAct->setStatusTip(tr("Select the entire document"));
    connect(selectAllAct, SIGNAL(triggered()), uiUpdater, SLOT(selectAll()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), selectAllAct, SLOT(setEnabled(bool)));

    insertSymbolAct = new QAction(QIcon(":/images/insertsymbol.png"), tr("Insert Symbol..."), uiUpdater);
    insertSymbolAct->setEnabled(true/*false*/);
    insertSymbolAct->setStatusTip(tr("Inserts a symbol into the current document"));
    connect(insertSymbolAct, SIGNAL(triggered()), uiUpdater, SLOT(insertSymbol()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), insertSymbolAct, SLOT(setEnabled(bool)));

    insertImageAct = new QAction(QIcon(":/images/insertimage.png"), tr("Insert Image..."), uiUpdater);
    insertImageAct->setEnabled(true/*false*/);
    insertImageAct->setStatusTip(tr("Inserts an image into the current document"));
    connect(insertImageAct, SIGNAL(triggered()), uiUpdater, SLOT(insertImage()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), insertImageAct, SLOT(setEnabled(bool)));

    formatStyleMathAct = new QAction(tr("Auto Math"), uiUpdater);
    //formatStyleMathAct->setCheckable(true);
    formatStyleMathAct->setData(QVariant(int(FSTYLE_NONE)));
    //formatStyleMathAct->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_NONE );
    formatStyleMathAct->setShortcut(tr("Ctrl+Shift+M"));
    formatStyleMathAct->setStatusTip(tr("Choose Style automatically"));
    connect(formatStyleMathAct, SIGNAL(triggered()), uiUpdater, SLOT(setStyleSelected()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), formatStyleMathAct, SLOT(setEnabled(bool)));

    formatStyleTextAct = new QAction(tr("Text"), uiUpdater);
    //formatStyleTextAct->setCheckable(true);
    formatStyleTextAct->setData(QVariant(int(FSTYLE_TEXT)));
    //formatStyleTextAct->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_TEXT );
    formatStyleTextAct->setShortcut(tr("Ctrl+Shift+E"));
    formatStyleTextAct->setStatusTip(tr("Apply Text font and character style"));
    connect(formatStyleTextAct, SIGNAL(triggered()), uiUpdater, SLOT(setStyleSelected()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), formatStyleTextAct, SLOT(setEnabled(bool)));

	formatStyleFunctionAct = new QAction(tr("Function"), uiUpdater);
    //formatStyleFunctionAct->setCheckable(true);
    formatStyleFunctionAct->setData(QVariant(int(FSTYLE_FUNCTION)));
    //formatStyleFunctionAct->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_FUNCTION );
    formatStyleFunctionAct->setShortcut(tr("Ctrl+Shift+F"));
    formatStyleFunctionAct->setStatusTip(tr("Apply Function font and character style"));
    connect(formatStyleFunctionAct, SIGNAL(triggered()), uiUpdater, SLOT(setStyleSelected()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), formatStyleFunctionAct, SLOT(setEnabled(bool)));

    formatStyleVariableAct = new QAction(tr("Variable"), uiUpdater);
    //formatStyleVariableAct->setCheckable(true);
    formatStyleVariableAct->setData(QVariant(int(FSTYLE_VARIABLE)));
    //formatStyleVariableAct->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_VARIABLE );
    formatStyleVariableAct->setShortcut(tr("Ctrl+Shift+V"));
    formatStyleVariableAct->setStatusTip(tr("Apply Variable font and character style"));
    connect(formatStyleVariableAct, SIGNAL(triggered()), uiUpdater, SLOT(setStyleSelected()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), formatStyleVariableAct, SLOT(setEnabled(bool)));

    formatStyleNumberAct = new QAction(tr("Number"), uiUpdater);
    //formatStyleNumberAct->setCheckable(true);
    formatStyleNumberAct->setData(QVariant(int(FSTYLE_NUMBER)));
    //formatStyleNumberAct->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_NUMBER );
    formatStyleNumberAct->setShortcut(tr("Ctrl+Shift+N"));
    formatStyleNumberAct->setStatusTip(tr("Apply Number font and character style"));
    connect(formatStyleNumberAct, SIGNAL(triggered()), uiUpdater, SLOT(setStyleSelected()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), formatStyleNumberAct, SLOT(setEnabled(bool)));

    formatStyleGreekAct = new QAction(tr("Greek"), uiUpdater);
    //formatStyleGreekAct->setCheckable(true);
    formatStyleGreekAct->setData(QVariant(int(FSTYLE_GREEK)));
    //formatStyleGreekAct->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_GREEK );
    formatStyleGreekAct->setShortcut(tr("Ctrl+Shift+G"));
    formatStyleGreekAct->setStatusTip(tr("Apply Greek font and character style"));
    connect(formatStyleGreekAct, SIGNAL(triggered()), uiUpdater, SLOT(setStyleSelected()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), formatStyleGreekAct, SLOT(setEnabled(bool)));

    formatStyleVectorMatrixAct = new QAction(tr("Vector-Matrix"), uiUpdater);
    //formatStyleVectorMatrixAct->setCheckable(true);
    formatStyleVectorMatrixAct->setData(QVariant(int(FSTYLE_VECTOR)));
    //formatStyleVectorMatrixAct->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_VECTOR );
    formatStyleVectorMatrixAct->setShortcut(tr("Ctrl+Shift+B"));
    formatStyleVectorMatrixAct->setStatusTip(tr("Apply Vector/Matrix font and character style"));
    connect(formatStyleVectorMatrixAct, SIGNAL(triggered()), uiUpdater, SLOT(setStyleSelected()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), formatStyleVectorMatrixAct, SLOT(setEnabled(bool)));

	formatStyleFixedAct = new QAction(tr("Fixed"), uiUpdater);
    //formatStyleFixedAct->setCheckable(true);
    formatStyleFixedAct->setData(QVariant(int(FSTYLE_FIXED)));
    //formatStyleFixedAct->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_FIXED );
    formatStyleFixedAct->setShortcut(tr("Ctrl+Shift+X"));
    formatStyleFixedAct->setStatusTip(tr("Apply Fixed font and character style"));
    connect(formatStyleFixedAct, SIGNAL(triggered()), uiUpdater, SLOT(setStyleSelected()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), formatStyleFixedAct, SLOT(setEnabled(bool)));

    formatStyleOpAct = new QAction(tr("Operator"), uiUpdater);
    //formatStyleOpAct->setCheckable(true);
    formatStyleOpAct->setData(QVariant(int(FSTYLE_OP)));
    //formatStyleOpAct->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_OP );
    formatStyleOpAct->setShortcut(tr("Ctrl+Shift+O"));
    formatStyleOpAct->setStatusTip(tr("Apply Operator font and character style"));
    connect(formatStyleOpAct, SIGNAL(triggered()), uiUpdater, SLOT(setStyleSelected()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), formatStyleOpAct, SLOT(setEnabled(bool)));

    formatStyleExtraMathAct = new QAction(tr("Extra Math"), uiUpdater);
    //formatStyleExtraMathAct->setCheckable(true);
    formatStyleExtraMathAct->setData(QVariant(int(FSTYLE_MTEXTRA)));
    //formatStyleExtraMathAct->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_MTEXTRA );
    //formatStyleExtraMathAct->setShortcut(tr("Ctrl+Shift+R"));
    formatStyleExtraMathAct->setStatusTip(tr("Apply Extra Math font and character style"));
    connect(formatStyleExtraMathAct, SIGNAL(triggered()), uiUpdater, SLOT(setStyleSelected()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), formatStyleExtraMathAct, SLOT(setEnabled(bool)));

    formatStyleUser1Act = new QAction(tr("User 1"), uiUpdater);
    //formatStyleUser1Act->setCheckable(true);
    formatStyleUser1Act->setData(QVariant(int(FSTYLE_USER1)));
    //formatStyleUser1Act->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_USER1 );
    formatStyleUser1Act->setShortcut(tr("Ctrl+Shift+U"));
    formatStyleUser1Act->setStatusTip(tr("Apply User1 font and character style"));
    connect(formatStyleUser1Act, SIGNAL(triggered()), uiUpdater, SLOT(setStyleSelected()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), formatStyleUser1Act, SLOT(setEnabled(bool)));

    formatStyleUser2Act = new QAction(tr("User 2"), uiUpdater);
    //formatStyleUser2Act->setCheckable(true);
    formatStyleUser2Act->setData(QVariant(int(FSTYLE_USER2)));
    //formatStyleUser2Act->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_USER2 );
    formatStyleUser2Act->setShortcut(tr("Ctrl+Alt+Shift+U"));
    formatStyleUser2Act->setStatusTip(tr("Apply User2 font and character style"));
    connect(formatStyleUser2Act, SIGNAL(triggered()), uiUpdater, SLOT(setStyleSelected()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), formatStyleUser2Act, SLOT(setEnabled(bool)));

	formatStyleOtherAct = new QAction(tr("Other..."), uiUpdater);
    //formatStyleOtherAct->setCheckable(true);
    formatStyleOtherAct->setData(QVariant(int(FSTYLE_DEFAULT_FONT_NUMBER)));
    //formatStyleOtherAct->setChecked( ::getCurrentUserChoiceStyle() >= FSTYLE_DEFAULT_FONT_NUMBER );
    formatStyleOtherAct->setStatusTip(tr("Apply a font directly, without using styles"));
    connect(formatStyleOtherAct, SIGNAL(triggered()), uiUpdater, SLOT(setStyleSelected()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), formatStyleOtherAct, SLOT(setEnabled(bool)));

	styleActionGroup = new QActionGroup(uiUpdater);
	//styleActionGroup->addAction(formatStyleMathAct);
	//styleActionGroup->addAction(formatStyleTextAct);
	//styleActionGroup->addAction(formatStyleFunctionAct);
	//styleActionGroup->addAction(formatStyleVariableAct);
	//styleActionGroup->addAction(formatStyleNumberAct);
	//styleActionGroup->addAction(formatStyleGreekAct);
	//styleActionGroup->addAction(formatStyleVectorMatrixAct);
	//styleActionGroup->addAction(formatStyleFixedAct);
	//styleActionGroup->addAction(formatStyleOpAct);
	//styleActionGroup->addAction(formatStyleExtraMathAct);
	//styleActionGroup->addAction(formatStyleUser1Act);
	//styleActionGroup->addAction(formatStyleUser2Act);
	//styleActionGroup->addAction(formatStyleOtherAct);
	//styleActionGroup->setExclusive( false );
 //   connect(styleActionGroup, SIGNAL(triggered(QAction*)), uiUpdater, SLOT(setStyleSelected(QAction*)));

	formatDefineStyleAct = new QAction(tr("Define Styles..."), uiUpdater);
    formatDefineStyleAct->setStatusTip(tr("Define fonts and colors to be used with predefined styles"));
    connect(formatDefineStyleAct, SIGNAL(triggered()), uiUpdater, SLOT(formatDefineStyle()));
	connect(uiUpdater, SIGNAL(hasChildSignal(bool)), formatDefineStyleAct, SLOT(setEnabled(bool)));

    formatSizeLargeSymbolAct = new QAction(tr("Large Symbol"), uiUpdater);
    //formatSizeLargeSymbolAct->setCheckable(true);
    //formatSizeLargeSymbolAct->setChecked( ::getCurrentKeglLevel() == FTEXT_HEIGHT_LARGEOP );
    formatSizeLargeSymbolAct->setStatusTip(tr("Change characters size to be a Large Symbol"));
    connect(formatSizeLargeSymbolAct, SIGNAL(triggered()), uiUpdater, SLOT(setSizeLargeSymbol()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), formatSizeLargeSymbolAct, SLOT(setEnabled(bool)));

    formatSizeRegularAct = new QAction(tr("Regular"), uiUpdater);
    //formatSizeRegularAct->setCheckable(true);
    //formatSizeRegularAct->setChecked( ::getCurrentKeglLevel() == FTEXT_HEIGHT_FRAME );
    formatSizeRegularAct->setStatusTip(tr("Change characters size to be a Regular text"));
    connect(formatSizeRegularAct, SIGNAL(triggered()), uiUpdater, SLOT(setSizeRegular()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), formatSizeRegularAct, SLOT(setEnabled(bool)));

    formatSizeSubscriptAct = new QAction(tr("Subscript"), uiUpdater);
    //formatSizeSubscriptAct->setCheckable(true);
    //formatSizeSubscriptAct->setChecked( ::getCurrentKeglLevel() == FTEXT_HEIGHT_CHILD1 );
    formatSizeSubscriptAct->setStatusTip(tr("Change characters size to be a Subscript text"));
    connect(formatSizeSubscriptAct, SIGNAL(triggered()), uiUpdater, SLOT(setSizeSubscript()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), formatSizeSubscriptAct, SLOT(setEnabled(bool)));

    formatSizeSubSubscriptAct = new QAction(tr("Sub-Subscript"), uiUpdater);
    //formatSizeSubSubscriptAct->setCheckable(true);
    //formatSizeSubSubscriptAct->setChecked( ::getCurrentKeglLevel() == FTEXT_HEIGHT_CHILD2 );
    formatSizeSubSubscriptAct->setStatusTip(tr("Change characters size to be a Sub-Subscript text"));
    connect(formatSizeSubSubscriptAct, SIGNAL(triggered()), uiUpdater, SLOT(setSizeSubSubscript()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), formatSizeSubSubscriptAct, SLOT(setEnabled(bool)));

    formatSizeOtherAct = new QAction(tr("Other..."), uiUpdater);
    //formatSizeOtherAct->setCheckable(true);
	//formatSizeOtherAct->setChecked( ::isOtherSize( ::getCurrentKegl() ) );
    formatSizeOtherAct->setStatusTip(tr("Change characters size directly, without using size definitions"));
    connect(formatSizeOtherAct, SIGNAL(triggered()), uiUpdater, SLOT(setSizeOther()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), formatSizeOtherAct, SLOT(setEnabled(bool)));

	sizeActionGroup = new QActionGroup(uiUpdater);
	//sizeActionGroup->addAction(formatSizeLargeSymbolAct);
	//sizeActionGroup->addAction(formatSizeRegularAct);
	//sizeActionGroup->addAction(formatSizeSubscriptAct);
	//sizeActionGroup->addAction(formatSizeSubSubscriptAct);
	//sizeActionGroup->addAction(formatSizeOtherAct);

	formatSizeSmallerAct = new QAction(tr("Smaller"), uiUpdater);
    formatSizeSmallerAct->setShortcut(tr("Alt+<"));
    formatSizeSmallerAct->setStatusTip(tr("Reduce characters size by one increment"));
    connect(formatSizeSmallerAct, SIGNAL(triggered()), uiUpdater, SLOT(setSizeSmaller()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), formatSizeSmallerAct, SLOT(setEnabled(bool)));

	formatSizeLargerAct = new QAction(tr("Larger"), uiUpdater);
    formatSizeLargerAct->setShortcut(tr("Alt+>"));
    formatSizeLargerAct->setStatusTip(tr("Increase characters size by one increment"));
    connect(formatSizeLargerAct, SIGNAL(triggered()), uiUpdater, SLOT(setSizeLarger()));
	connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), formatSizeLargerAct, SLOT(setEnabled(bool)));

	formatDefineSizeAct = new QAction(tr("Define Size..."), uiUpdater);
    formatDefineSizeAct->setStatusTip(tr("Define character sizes to be used with predefined size types"));
    connect(formatDefineSizeAct, SIGNAL(triggered()), uiUpdater, SLOT(formatDefineSize()));
	connect(uiUpdater, SIGNAL(hasChildSignal(bool)), formatDefineSizeAct, SLOT(setEnabled(bool)));
}

void QFormulatorEditWindow::initEditMenu( QMainWindow *uiUpdater )
{
	editMenu = uiUpdater->menuBar()->addMenu(tr("&Edit"));
    editMenu->setObjectName(QString(FORMULATOR_QTGUI_OBJECT_NAME_MENU).arg("Edit"));
	editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();
    editMenu->addAction(selectAllAct);
}

void QFormulatorEditWindow::initInsertMenu( QMainWindow *uiUpdater )
{
    insertMenu = uiUpdater->menuBar()->addMenu(tr("&Insert"));
    insertMenu->setObjectName(QString(FORMULATOR_QTGUI_OBJECT_NAME_MENU).arg("Insert"));
    insertMenu->addAction(insertSymbolAct);
    insertMenu->addAction(insertImageAct);
	QMenu *evaluateMenu = insertMenu->addMenu(tr("&Calculation"));
	if( evaluateMenu )
	{
		evaluateMenu->setIcon(QIcon(":/images/calc.png"));
		evaluateMenu->addAction(evaluateAct);
		evaluateMenu->addAction(insertAssignAct);
	}
}

void QFormulatorEditWindow::initFormatMenu( QMainWindow *uiUpdater )
{
    formatMenu = uiUpdater->menuBar()->addMenu(tr("&Format"));
    formatMenu->setObjectName(QString(FORMULATOR_QTGUI_OBJECT_NAME_MENU).arg("Format"));
	styleMenu = formatMenu->addMenu(tr("&Style"));
	sizeMenu = formatMenu->addMenu(tr("Si&ze"));

	styleMenu->addAction(formatStyleMathAct);
	styleMenu->addAction(formatStyleTextAct);
	styleMenu->addAction(formatStyleFunctionAct);
	styleMenu->addAction(formatStyleVariableAct);
	styleMenu->addAction(formatStyleNumberAct);
	styleMenu->addAction(formatStyleGreekAct);
	styleMenu->addAction(formatStyleVectorMatrixAct);
	styleMenu->addAction(formatStyleFixedAct);
	styleMenu->addAction(formatStyleOpAct);
	styleMenu->addAction(formatStyleExtraMathAct);
	styleMenu->addAction(formatStyleUser1Act);
	styleMenu->addAction(formatStyleUser2Act);
	styleMenu->addSeparator();
	styleMenu->addAction(formatStyleOtherAct);

	sizeMenu->addAction(formatSizeLargeSymbolAct);
	sizeMenu->addAction(formatSizeRegularAct);
	sizeMenu->addAction(formatSizeSubscriptAct);
	sizeMenu->addAction(formatSizeSubSubscriptAct);
	sizeMenu->addSeparator();
	sizeMenu->addAction(formatSizeOtherAct);
	sizeMenu->addSeparator();
	sizeMenu->addAction(formatSizeSmallerAct);
	sizeMenu->addAction(formatSizeLargerAct);

	formatMenu->addSeparator();
	formatMenu->addAction( formatDefineStyleAct );
	formatMenu->addAction( formatDefineSizeAct );
}

void QFormulatorEditWindow::initEditToolbar( QMainWindow *uiUpdater )
{
	editToolBar = uiUpdater->addToolBar(tr("Edit"));
	editToolBar->setObjectName(QString(FORMULATOR_QTGUI_OBJECT_NAME_TOOLBAR).arg("Edit"));
	editToolBar->setIconSize( QSize(IconSizeX, IconSizeY) );

	editToolBar->addAction(cutAct);
	editToolBar->addAction(copyAct);
	editToolBar->addAction(pasteAct);
	editToolBar->addSeparator();
	editToolBar->addAction(undoAct);
	editToolBar->addAction(redoAct);
}

void QFormulatorEditWindow::initInsertToolbar( QMainWindow *uiUpdater )
{
	insertToolBar = uiUpdater->addToolBar(tr("Insert"));
	insertToolBar->setObjectName(QString(FORMULATOR_QTGUI_OBJECT_NAME_TOOLBAR).arg("Insert"));
	insertToolBar->setIconSize( QSize(IconSizeX, IconSizeY) );

	insertToolBar->addAction(insertSymbolAct);
	insertToolBar->addAction(insertImageAct);
}

void QFormulatorEditWindow::initFormatToolbar( QMainWindow *uiUpdater )
{
	formatToolBar = uiUpdater->addToolBar(tr("Format"));
	formatToolBar->setObjectName(QString(FORMULATOR_QTGUI_OBJECT_NAME_TOOLBAR).arg("Format"));
	formatToolBar->setIconSize( QSize(IconSizeX, IconSizeY) );
	QFormulatorEditWindow::initStyleSizeWidgets( uiUpdater );

	if( selectStyleLabel && selectStyleCombo )
	{
		formatToolBar->addWidget( selectStyleLabel );
		formatToolBar->addWidget( selectStyleCombo );
	}
	formatToolBar->addSeparator();
	if( selectSizeLabel && selectSizeCombo )
	{
		formatToolBar->addWidget( selectSizeLabel );
		formatToolBar->addWidget( selectSizeCombo );
	}
}

void QFormulatorEditWindow::adjustViewMenuToggleToolbars()
{
	QFormulatorViewWindow::adjustViewMenuToggleToolbars();
	if( toggleToolbarMenu )
	{
		if( editToolBar )   toggleToolbarMenu->addAction(editToolBar->toggleViewAction());
		if( insertToolBar ) toggleToolbarMenu->addAction(insertToolBar->toggleViewAction());
		if( formatToolBar ) toggleToolbarMenu->addAction(formatToolBar->toggleViewAction());
		toggleToolbarMenu->addSeparator();
		QMap<QString,QToolBar*>::const_iterator i = toolbarGroups.constBegin();
		while( i != toolbarGroups.constEnd() )
		{
			if( i.value() != 0 ) toggleToolbarMenu->addAction(i.value()->toggleViewAction());
			i++;
		}
	}
}

void QFormulatorEditWindow::initXmlBtnGui( QMainWindow *uiUpdater )
{
	if( !::getCurrentToolBarSet() || ::getCurrentToolBarSet()->getToolBarCount() == 0 ) return;

	insertXmlBtnMenu.clear();
	insertXmlBtn.clear();
	insertMenu->addSeparator();

	int /*k[ 4 ], */b, cb, w;
	QAction *act;
	QDir rootDataDir( FmlCoreSettings::getRootDir() );
	QString groupsImgPath = FmlCoreSettings::getXmlBtnGroupsImageDir();
	for( long t = 1; t < ::getCurrentToolBarSet()->getToolBarCount(); t++ )
	{
		const FBtnToolbar& toolbar = ::getCurrentToolBarSet()->at(t);
		if( !toolbar.getVisible() ) continue;

		long buttonsCount = 0;
		for( b = 0; b < toolbar.getButtonCount(); b++ )
		{
			if( toolbar.at(b).getID().length() == 0 || toolbar.at(b).getName().length() == 0 )
				continue;
			buttonsCount++;
		}

		// calculate visisble buttons
		long nCols = toolbar.getColumns(), nRows = toolbar.getRows();
		// calculate rows and cols
		if( nCols > 0 )
		{
			nRows = buttonsCount / nCols;
			if( buttonsCount % nCols )
				nRows++;
		}
		else if( nRows > 0 )
		{
			nCols = buttonsCount / nRows;
			if( buttonsCount % nRows )
				nCols++;
		}
		else if( nCols == 0 )
		{
			nRows = (long) sqrt( (double) buttonsCount );
			nCols = buttonsCount / nRows;
			if( buttonsCount % nRows )
				nCols++;
		}
		else 
		{
			nCols = (long) sqrt( (double) buttonsCount );
			nRows = buttonsCount / nCols;
			if( buttonsCount % nCols )
				nRows++;
		}

		QString groupName = toolbar.getGroup();
		QMap<QString,QMenu*>::iterator grItem = menuGroups.find(groupName);
		QMenu *menuItem = 0;
		if( grItem == menuGroups.end() )
		{
			menuGroups.insert(groupName, menuItem = insertMenu->addMenu(groupName));
			QString imgFile = QString("%1/%2").arg(groupsImgPath).arg(groupName.toLower());
			if(QFile::exists(imgFile))
				menuItem->setIcon(QIcon(imgFile));
		}
		else
		{
			menuItem = grItem.value();
		}

		QMap<QString,QToolBar*>::iterator grTb = toolbarGroups.find(groupName);
		QToolBar *toolbarItem = 0;
		if( grTb == toolbarGroups.end() )
		{
                        toolbarGroups.insert(groupName, toolbarItem = uiUpdater->addToolBar(groupName));
			toolbarItem->setObjectName(QString(FORMULATOR_QTGUI_OBJECT_NAME_TOOLBAR).arg(groupName));
			toolbarItem->setIconSize( QSize(IconSizeX, IconSizeY) );
			uiUpdater->insertToolBarBreak(toolbarItem);
		}
		else
			toolbarItem = grTb.value();

		QString tlbIconPath = QString("%1/%2").arg(rootDataDir.absolutePath()).arg(toolbar.getImageTitle());
		//QIcon tlbIcon( tlbIconPath );

		QPixmap tblPixmap( tlbIconPath );
		QBitmap mask = tblPixmap.createMaskFromColor ( QColor(255, 0, 255) );
		tblPixmap.setMask( mask );
		QIcon tlbIcon( tblPixmap );

		//QIcon tlbIcon( tlbIconPath );
		QString btnIconPath = QString("%1/%2").arg(rootDataDir.absolutePath()).arg(toolbar.getImage());
		QPixmap btnPixmap( btnIconPath );
		w = btnPixmap.width() / buttonsCount;

		menuItem = menuItem->addMenu(tlbIcon, toolbar.getName());
		menuItem->setStatusTip(toolbar.getName());
		insertXmlBtnMenu.push_back(menuItem);

		QToolButton *toolbuttonItem = new QToolButton( toolbarItem );
		toolbuttonItem->setIcon( tlbIcon );
		toolbuttonItem->setPopupMode( QToolButton::InstantPopup );
		toolbuttonItem->setStatusTip(toolbar.getName());
		toolbuttonItem->setToolTip(toolbar.getName());
		//
		QMenu *toolMenu = new QMenu();
		QGridLayout *menuLayout = new QGridLayout();
		menuLayout->setVerticalSpacing(8);
		menuLayout->setHorizontalSpacing(8);
		toolMenu->setLayout( menuLayout );
		toolbuttonItem->setMenu(toolMenu);
		//toolbuttonItem->setArrowType( Qt::NoArrow );
		toolbarItem->addWidget(toolbuttonItem);
		connect(uiUpdater, SIGNAL(hasChildSignal(bool)), toolbarItem, SLOT(setEnabled(bool)));
		//connect(toolMenu, SIGNAL(hovered(QAction*)), uiUpdater, SLOT(showSenderStatusText(QAction*)));

		// "cb" is button number if not considering "empty buttons" (used for icons and menu items numeration)
		long posX = 0, posY = 0;
		for( cb = b = 0; b < toolbar.getButtonCount(); b++ )
		{
			const FXmlBtn& btn = toolbar.at(b);
			if( btn.getID().length() == 0 || btn.getName().length() == 0 )
				continue;

			QIcon btnIcon(btnPixmap.copy(cb * w, 0, w, btnPixmap.height()));
			act = new QAction(btnIcon, QString("%1. %2").arg(cb + 1, 2).arg(btn.getName()), uiUpdater);
			QList<QKeySequence> shortcuts;
			for( long i = 0; i < btn.getAccelCount(); i++ )
			{
#ifdef Q_WS_MAC
				if( btn.getAccel(i).getAccelCodeCount() == 1 )
#endif
				{
					//const HKAccel& accel = btn.getAccel(i);
					int k[4] = {0, 0, 0, 0};
					QFormulatorEditWindow::initXmlBtnAccel( btn.getAccel(i), k );
					QKeySequence shortcutKey( k[0], k[1], k[2], k[3] );
					if( !shortcutKey.isEmpty() )
					{
						//QString a = shortcutKey.toString();
						shortcuts.append(shortcutKey);
					}
				}
			}
			if( shortcuts.size() > 0 )
				act->setShortcuts(shortcuts);
			act->setStatusTip(btn.getToolTip());
			act->setToolTip(btn.getToolTip());
			act->setData(btn.getID());
			connect(act, SIGNAL(triggered()),uiUpdater, SLOT(insertButton()));
			connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), act, SLOT(setEnabled(bool)));
			insertXmlBtn.push_back(act);
			menuItem->addAction(act);

			QToolButton *tbBtn = new QToolButton();
			tbBtn->setIcon( btnIcon );
			QSize tbBtnIconSz(w, btnPixmap.height());
			tbBtn->setIconSize( tbBtnIconSz );
			//tbBtn->setText( btn.getID() );
			//tbBtn->setStatusTip(btn.getToolTip());
			//tbBtn->setToolTip(btn.getToolTip());
			tbBtn->setToolButtonStyle( Qt::ToolButtonIconOnly );

			//toolMenu->addAction(act);
			//connect(toolMenu, SIGNAL(hovered(QAction*)), uiUpdater, SLOT(showSenderStatusText(QAction*)));
			tbBtn->setDefaultAction(act);
			//tbBtn->setAutoRaise( true );
#ifndef Q_WS_MAC
			connect(act, SIGNAL(hovered()), uiUpdater, SLOT(showSenderStatusText()));
#endif
			//connect(tbBtn, SIGNAL(clicked()), uiUpdater, SLOT(insertButtonViaToolbar()));
			menuLayout->addWidget( tbBtn, posY, posX );

			cb++;
			if( posX + 1 >= nCols )
			{
				posX = 0;
				posY++;
			}
			else
				posX++;
		}
    }
}

void QFormulatorEditWindow::initXmlBtnAccel( const HKAccel& accel, int *k )
{
	for( long j = 0; j < ((accel.getAccelCodeCount() <= 4) ? accel.getAccelCodeCount() : 4); j++ )
	{
		short key = accel.getKey( j ), flag = accel.getFlag( j );
		QChar keyCh = QChar( key ).toUpper();

		if( flag & HK_Key_Ctrl )
			k[j] |= Qt::CTRL;
		if( flag & HK_Key_Alt )
			k[j] |= Qt::ALT;
		if( flag & HK_Key_Shift )
			k[j] |= Qt::SHIFT;

		if( key == 0x0025 )
			k[j] |= Qt::Key_Left;
		else if( key == 0x0026 )
			k[j] |= Qt::Key_Up;
		else if( key == 0x0027 )
			k[j] |= Qt::Key_Right;
		else if( key == 0x0028 )
			k[j] |= Qt::Key_Down;
		else if( key == 0x0009 )
			k[j] |= Qt::Key_Tab;
		else if( key == 0x000D )
			k[j] |= Qt::Key_Return;
		else
		{
			if( keyCh.isDigit() )
				k[j] |= (Qt::Key_0 + keyCh.toAscii() - '0');
			else if( keyCh.toAscii() >= 'A' && keyCh.toAscii() <= 'Z' )
				k[j] |= (Qt::Key_A + keyCh.toAscii() - 'A');
			else
			{
				switch(keyCh.toAscii())
				{
				case ' ':
					k[j] |= Qt::Key_Space;
					break;
				case '!':
					k[j] |= Qt::Key_Exclam;
					break;
				case '\"':
					k[j] |= Qt::Key_QuoteDbl;
					break;
				case '#':
					k[j] |= Qt::Key_NumberSign;
					break;
				case '$':
					k[j] |= Qt::Key_Dollar;
					break;
				case '%':
					k[j] |= Qt::Key_Percent;
					break;
				case '&':
					k[j] |= Qt::Key_Ampersand;
					break;
				case '\'':
					k[j] |= Qt::Key_Apostrophe;
					break;
				case '(':
					k[j] |= Qt::Key_ParenLeft;
					break;
				case ')':
					k[j] |= Qt::Key_ParenRight;
					break;
				case '*':
					k[j] |= Qt::Key_Asterisk;
					break;
				case '+':
					k[j] |= Qt::Key_Plus;
					break;
				case ',':
					k[j] |= Qt::Key_Comma;
					break;
				case '-':
					k[j] |= Qt::Key_Minus;
					break;
				case '.':
					k[j] |= Qt::Key_Period;
					break;
				case '/':
					k[j] |= Qt::Key_Slash;
					break;
				case ':':
					k[j] |= Qt::Key_Colon;
					break;
				case ';':
					k[j] |= Qt::Key_Semicolon;
					break;
				case '<':
					k[j] |= Qt::Key_Less;
					break;
				case '=':
					k[j] |= Qt::Key_Equal;
					break;
				case '>':
					k[j] |= Qt::Key_Greater;
					break;
				case '?':
					k[j] |= Qt::Key_Question;
					break;
				case '@':
					k[j] |= Qt::Key_At;
					break;
				case '[':
					k[j] |= Qt::Key_BracketLeft;
					break;
				case '\\':
					k[j] |= Qt::Key_Backslash;
					break;
				case ']':
					k[j] |= Qt::Key_BracketRight;
					break;
				case '^':
					k[j] |= Qt::Key_AsciiCircum;
					break;
				case '_':
					k[j] |= Qt::Key_Underscore;
					break;
				case '`':
					k[j] |= Qt::Key_QuoteLeft;
					break;
				case '{':
					k[j] |= Qt::Key_BraceLeft;
					break;
				case '|':
					k[j] |= Qt::Key_Bar;
					break;
				case '}':
					k[j] |= Qt::Key_BraceRight;
					break;
				case '~':
					k[j] |= Qt::Key_AsciiTilde;
					break;
				default:
					k[0] = k[1] = k[2] = k[3] = 0;
					return;
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////

void QFormulatorEditWindow::initFormulatorCore()
{
	QFormulatorViewWindow::initFormulatorCore();
}

void QFormulatorEditWindow::showFormulatorSplash( QMainWindow *uiUpdater )
{
	QFormulatorViewWindow::showFormulatorSplash( uiUpdater );
}

void QFormulatorEditWindow::hideFormulatorSplash( QMainWindow *uiUpdater )
{
	QFormulatorViewWindow::hideFormulatorSplash( uiUpdater );
}

//////////////////////////////////////////////////////////////////////////////

void QFormulatorEditWindow::initStyleSizeWidgets( QMainWindow *uiUpdater )
{
	if( !selectStyleLabel )
		selectStyleLabel = new QLabel(tr("Style: "));
	if( !selectStyleCombo )
	{
		selectStyleCombo = new QComboBox();
		selectStyleCombo->setEditable(false);
		selectStyleCombo->setDuplicatesEnabled(false);
		selectStyleCombo->setIconSize( formatToolBar ? formatToolBar->iconSize() : QSize(QStyle::PM_ToolBarIconSize, QStyle::PM_ToolBarIconSize) );
		for( int i = 0; i < styleNumber; i++ )
			selectStyleCombo->addItem( m_styleTitle[ i ], QVariant(m_styleID[i]) );
		connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), selectStyleCombo, SLOT(setEnabled(bool)));
		connect(selectStyleCombo, SIGNAL(activated(int)), uiUpdater, SLOT(setStyleByCombo(int)));
		updateSelectStyleCombo();
	}

	if( !selectSizeLabel )
		selectSizeLabel = new QLabel(tr("Size: "));
	if( !selectSizeCombo )
	{
		selectSizeCombo = new QComboBox();
		selectSizeCombo->setEditable(false);
		selectSizeCombo->setDuplicatesEnabled(false);
		selectSizeCombo->setIconSize( formatToolBar ? formatToolBar->iconSize() : QSize(QStyle::PM_ToolBarIconSize, QStyle::PM_ToolBarIconSize) );
		selectSizeCombo->addItem( tr("Large Symbol"), QVariant(int(FTEXT_HEIGHT_LARGEOP)) );
		selectSizeCombo->addItem( tr("Regular Size"), QVariant(int(FTEXT_HEIGHT_FRAME)) );
		selectSizeCombo->addItem( tr("Subscript"), QVariant(int(FTEXT_HEIGHT_CHILD1)) );
		selectSizeCombo->addItem( tr("Sub-Subscript"), QVariant(int(FTEXT_HEIGHT_CHILD2)) );
		selectSizeCombo->addItem( tr("Other..."), QVariant(int(FTEXT_HEIGHT_LAST + 1)) );
		updateSelectSizeCombo();
		connect(uiUpdater, SIGNAL(hasEditChildSignal(bool)), selectSizeCombo, SLOT(setEnabled(bool)));
		connect(selectSizeCombo, SIGNAL(activated(int)), uiUpdater, SLOT(setSizeByCombo(int)));
	}
}

void QFormulatorEditWindow::updateSelectStyleCombo()
{
	selectStyleCombo->blockSignals( true );

	int selectStyleComboIdx = selectStyleCombo->findData( QVariant(int(::getCurrentUserChoiceStyle())) );
	if( selectStyleComboIdx != -1 )
		selectStyleCombo->setCurrentIndex( selectStyleComboIdx );
	else
	{
		selectStyleComboIdx = selectStyleCombo->findData(QVariant(int(FSTYLE_DEFAULT_FONT_NUMBER)));
		if( selectStyleComboIdx != -1 )
			selectStyleCombo->setCurrentIndex( selectStyleComboIdx );
	}

	QSize sz = selectStyleCombo->iconSize();
	int pointSize = 10;//FKEGL_LARGEOP;//FKEGL_FRAME
	QVector<QPixmap> pxVec;
	for( int i = 0; i < selectStyleCombo->count() - 1; i++ )
		pxVec.push_back(QPixmap(QFormulatorWidget::drawSampleText(m_styleID[i], pointSize, "Aa", sz)));
	pxVec.push_back(QPixmap(QFormulatorWidget::drawSampleText(::getCurrentUserChoiceStyle(), pointSize, "Aa", sz)));
	for( int i = 0; i < selectStyleCombo->count(); i++ )
		selectStyleCombo->setItemIcon( i, QIcon(pxVec[i]) );

	selectStyleCombo->blockSignals( false );
}

void QFormulatorEditWindow::updateSelectStyleUI()
{
    formatStyleMathAct->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_NONE );
    formatStyleTextAct->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_TEXT );
    formatStyleFunctionAct->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_FUNCTION );
    formatStyleVariableAct->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_VARIABLE );
    formatStyleNumberAct->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_NUMBER );
    formatStyleGreekAct->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_GREEK );
    formatStyleVectorMatrixAct->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_VECTOR );
    formatStyleFixedAct->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_FIXED );
    formatStyleOpAct->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_OP );
    formatStyleExtraMathAct->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_MTEXTRA );
    formatStyleUser1Act->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_USER1 );
    formatStyleUser2Act->setChecked( ::getCurrentUserChoiceStyle() == FSTYLE_USER2 );
    formatStyleOtherAct->setChecked( ::getCurrentUserChoiceStyle() >= FSTYLE_DEFAULT_FONT_NUMBER );
	updateSelectStyleCombo();
}

void QFormulatorEditWindow::updateSelectSizeCombo()
{
	selectSizeCombo->blockSignals( true );

	int selectSizeComboIdx = -1;
	if( !::isOtherSize( ::getCurrentKegl() ) )
		selectSizeComboIdx = selectSizeCombo->findData( QVariant(int(::getCurrentKeglLevel())) );
	if( selectSizeComboIdx == -1 )
	{
		selectSizeComboIdx = selectSizeCombo->findText(tr("Other..."));
		if( selectSizeComboIdx != -1 )
			selectSizeCombo->setCurrentIndex( selectSizeComboIdx );
	}
	else
		selectSizeCombo->setCurrentIndex( selectSizeComboIdx );

	QSize sz = selectSizeCombo->iconSize();
	const int knownLevels = 5;
	int pointSize;
	RealFde pointSizeVec[ knownLevels ] = {FKEGL_LARGEOP, FKEGL_FRAME, FKEGL_CHILD1, FKEGL_CHILD2, (FKEGL_FRAME + FKEGL_CHILD1) / 2.0 };
	for( int i = 0; i < selectSizeCombo->count(); i++ )
	{
		pointSize = (int) ((i < knownLevels) ? pointSizeVec[ i ] : ::getCurrentKegl());
		selectSizeCombo->setItemIcon( i, QIcon(QPixmap(QFormulatorWidget::drawSampleText(FSTYLE_KEYWORD, pointSize, QString(QChar(0x00BD)), sz))) );
	}

	selectSizeCombo->blockSignals( false );
}

void QFormulatorEditWindow::updateSelectSizeUI()
{
    formatSizeLargeSymbolAct->setChecked( ::getCurrentKeglLevel() == FTEXT_HEIGHT_LARGEOP );
    formatSizeRegularAct->setChecked( ::getCurrentKeglLevel() == FTEXT_HEIGHT_FRAME );
    formatSizeSubscriptAct->setChecked( ::getCurrentKeglLevel() == FTEXT_HEIGHT_CHILD1 );
    formatSizeSubSubscriptAct->setChecked( ::getCurrentKeglLevel() == FTEXT_HEIGHT_CHILD2 );
	formatSizeOtherAct->setChecked( ::isOtherSize( ::getCurrentKegl() ) );
	updateSelectSizeCombo();
}

//////////////////////////////////////////////////////////////////////////////

QAction *QFormulatorEditWindow::undoAct = 0;
QAction *QFormulatorEditWindow::redoAct = 0;
QAction *QFormulatorEditWindow::cutAct = 0;
QAction *QFormulatorEditWindow::copyAct = 0;
QAction *QFormulatorEditWindow::pasteAct = 0;
QAction *QFormulatorEditWindow::selectAllAct = 0;
QAction *QFormulatorEditWindow::evaluateAct = 0;
QAction *QFormulatorEditWindow::insertAssignAct = 0;
QAction *QFormulatorEditWindow::insertSymbolAct = 0;
QAction *QFormulatorEditWindow::insertImageAct = 0;
QAction *QFormulatorEditWindow::formatStyleMathAct = 0;
QAction *QFormulatorEditWindow::formatStyleTextAct = 0;
QAction *QFormulatorEditWindow::formatStyleFunctionAct = 0;
QAction *QFormulatorEditWindow::formatStyleVariableAct = 0;
QAction *QFormulatorEditWindow::formatStyleNumberAct = 0;
QAction *QFormulatorEditWindow::formatStyleGreekAct = 0;
QAction *QFormulatorEditWindow::formatStyleVectorMatrixAct = 0;
QAction *QFormulatorEditWindow::formatStyleFixedAct = 0;
QAction *QFormulatorEditWindow::formatStyleOpAct = 0;
QAction *QFormulatorEditWindow::formatStyleExtraMathAct = 0;
QAction *QFormulatorEditWindow::formatStyleUser1Act = 0;
QAction *QFormulatorEditWindow::formatStyleUser2Act = 0;
QAction *QFormulatorEditWindow::formatStyleOtherAct = 0;
QAction *QFormulatorEditWindow::formatSizeLargeSymbolAct = 0;
QAction *QFormulatorEditWindow::formatSizeRegularAct = 0;
QAction *QFormulatorEditWindow::formatSizeSubscriptAct = 0;
QAction *QFormulatorEditWindow::formatSizeSubSubscriptAct = 0;
QAction *QFormulatorEditWindow::formatSizeOtherAct = 0;
QAction *QFormulatorEditWindow::formatSizeSmallerAct = 0;
QAction *QFormulatorEditWindow::formatSizeLargerAct = 0;
QAction *QFormulatorEditWindow::formatDefineStyleAct = 0;
QAction *QFormulatorEditWindow::formatDefineSizeAct = 0;
QActionGroup *QFormulatorEditWindow::styleActionGroup = 0;
QActionGroup *QFormulatorEditWindow::sizeActionGroup = 0;
QMenu *QFormulatorEditWindow::formatMenu = 0;
QMenu *QFormulatorEditWindow::styleMenu = 0;
QMenu *QFormulatorEditWindow::sizeMenu = 0;
QVector<QAction*> QFormulatorEditWindow::insertXmlBtn;
QMenu *QFormulatorEditWindow::editMenu = 0;
QMenu *QFormulatorEditWindow::insertMenu = 0;
QVector<QMenu*> QFormulatorEditWindow::insertXmlBtnMenu;
QMap<QString,QMenu*> QFormulatorEditWindow::menuGroups;
QToolBar *QFormulatorEditWindow::editToolBar = 0;
QToolBar *QFormulatorEditWindow::insertToolBar = 0;
QToolBar *QFormulatorEditWindow::formatToolBar = 0;
QComboBox *QFormulatorEditWindow::selectStyleCombo;
QLabel *QFormulatorEditWindow::selectStyleLabel = 0;
QComboBox *QFormulatorEditWindow::selectSizeCombo;
QLabel *QFormulatorEditWindow::selectSizeLabel = 0;
QFontComboBox *QFormulatorEditWindow::fontCombo;
QComboBox *QFormulatorEditWindow::fontStyleCombo;
QComboBox *QFormulatorEditWindow::fontSizeCombo;
QMap<QString,QToolBar*> QFormulatorEditWindow::toolbarGroups;

//////////////////////////////////////////////////////////////////////////////
