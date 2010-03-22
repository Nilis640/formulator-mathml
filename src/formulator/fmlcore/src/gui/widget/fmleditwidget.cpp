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

#include "../document/fmldocument.h"
#include "fmleditwidget.h"

#include "../../nodes/ni_root.h"
#include "../../nodes/rules/sel.h"
#include "../../fmlcore_global.h"

#include "../dlg/config/configdlg.h"

/////////////////////////////////////////////////////////////////////////////

QFormulatorEditWidget::QFormulatorEditWidget( QWidget *parent, QObject *uiUpdater )
	: QFormulatorWidget( parent, uiUpdater )
{
	setCursor(Qt::IBeamCursor);
	setMouseTracking( true );
	setAcceptDrops( true );
	m_cursorBlinkTimer = new QTimer( this );
	connect(m_cursorBlinkTimer, SIGNAL(timeout()), this, SLOT(switchCursor()));
	clearRemovedSelection();
	setVerbose( true );
	setPrintingMode( false );
	setBlinkingCursorEnabled( true );
}

QFormulatorEditWidget::~QFormulatorEditWidget()
{
}

/////////////////////////////////////////////////////////////////////////////

void QFormulatorEditWidget::setBlinkingCursorEnabled( bool enable )
{
    if( enable && QApplication::cursorFlashTime() > 0 )
        m_cursorBlinkTimer->start( QApplication::cursorFlashTime() / 2 );
    else
        m_cursorBlinkTimer->stop();
    m_cursorOn = enable;
    repaintCursor();
}

void QFormulatorEditWidget::repaintCursor( PointFde pt, SizeFde sz )
{
	pt -= PointFde(1.0, 1.0);
	sz += SizeFde(2.0, 2.0);
	CFmlDrawEngineQt::LPtoDPEx( pt, scale(), scale() );
	CFmlDrawEngineQt::LPtoDPEx( sz, scale(), scale() );
	update( int(pt.x()), int(pt.y()), int(sz.width() + 0.5), int(sz.height() + 0.5) );
}

void QFormulatorEditWidget::repaintCursor()
{
	repaintCursor( caretPos(), caretSize() );
}

void QFormulatorEditWidget::switchCursor()
{
	m_cursorOn = !m_cursorOn;
	repaintCursor();
}

/////////////////////////////////////////////////////////////////////////////

void QFormulatorEditWidget::paintEvent(QPaintEvent *e)
{
	QFormulatorWidget::paintEvent(e);
}

void QFormulatorEditWidget::focusInEvent( QFocusEvent *e )
{
	QWidget::focusInEvent(e);
	if( hasFocus() && !m_cursorBlinkTimer->isActive() )
		setBlinkingCursorEnabled( true );

	update();
	//cbFireEvent( FORMULIB_CALLBACK_SETFOCUS );
}

void QFormulatorEditWidget::focusOutEvent( QFocusEvent *e )
{
	QWidget::focusOutEvent(e);
	if( !hasFocus() && m_cursorBlinkTimer->isActive() )
		setBlinkingCursorEnabled( false );

	update();
	//cbFireEvent( FORMULIB_CALLBACK_KILLFOCUS );
}

void QFormulatorEditWidget::keyPressEvent( QKeyEvent *e )
{
	switch( e->key() )
	{
    case Qt::Key_Up:
		if( e->modifiers() == Qt::NoModifier || e->modifiers() == Qt::KeypadModifier  )
			OnMoveUp();
		else if( (e->modifiers() & Qt::ControlModifier) && (e->modifiers() & Qt::ShiftModifier) )
			OnMoveCtrlShiftUp();
		else if( e->modifiers() & Qt::ShiftModifier )
			OnMoveShiftUp();
		else if( e->modifiers() & Qt::ControlModifier )
			OnMoveCtrlUp();
        return;
    case Qt::Key_Down:
		if( e->modifiers() == Qt::NoModifier || e->modifiers() == Qt::KeypadModifier  )
			OnMoveDown();
		else if( (e->modifiers() & Qt::ControlModifier) && (e->modifiers() & Qt::ShiftModifier) )
			OnMoveCtrlShiftDown();
		else if( e->modifiers() & Qt::ShiftModifier )
			OnMoveShiftDown();
		else if( e->modifiers() & Qt::ControlModifier )
			OnMoveCtrlDown();
        return;
    case Qt::Key_Left:
		if( e->modifiers() == Qt::NoModifier || e->modifiers() == Qt::KeypadModifier  )
			OnMoveLeft();
		else if( (e->modifiers() & Qt::ControlModifier) && (e->modifiers() & Qt::ShiftModifier) )
			OnMoveCtrlShiftLeft();
		else if( e->modifiers() & Qt::ShiftModifier )
			OnMoveShiftLeft();
		else if( e->modifiers() & Qt::ControlModifier )
			OnMoveCtrlLeft();
        return;
    case Qt::Key_Right:
		if( e->modifiers() == Qt::NoModifier || e->modifiers() == Qt::KeypadModifier  )
			OnMoveRight();
		else if( (e->modifiers() & Qt::ControlModifier) && (e->modifiers() & Qt::ShiftModifier) )
			OnMoveCtrlShiftRight();
		else if( e->modifiers() & Qt::ShiftModifier )
			OnMoveShiftRight();
		else if( e->modifiers() & Qt::ControlModifier )
			OnMoveCtrlRight();
        return;
	case Qt::Key_Home:
		if( e->modifiers() == Qt::NoModifier || e->modifiers() == Qt::KeypadModifier  )
			OnMoveHome();
		else if( (e->modifiers() & Qt::ControlModifier) && (e->modifiers() & Qt::ShiftModifier) )
			OnMoveCtrlShiftHome();
		else if( e->modifiers() & Qt::ShiftModifier )
			OnMoveShiftHome();
		else if( e->modifiers() & Qt::ControlModifier )
			OnMoveCtrlHome();
        return;
	case Qt::Key_End:
		if( e->modifiers() == Qt::NoModifier || e->modifiers() == Qt::KeypadModifier  )
			OnMoveEnd();
		else if( (e->modifiers() & Qt::ControlModifier) && (e->modifiers() & Qt::ShiftModifier) )
			OnMoveCtrlShiftEnd();
		else if( e->modifiers() & Qt::ShiftModifier )
			OnMoveShiftEnd();
		else if( e->modifiers() & Qt::ControlModifier )
			OnMoveCtrlEnd();
		break;
	case Qt::Key_PageUp:
		if( e->modifiers() == Qt::NoModifier || e->modifiers() == Qt::KeypadModifier  )
			OnMovePageUp();
		else if( (e->modifiers() & Qt::ControlModifier) && (e->modifiers() & Qt::ShiftModifier) )
			OnMoveCtrlShiftPageUp();
		else if( e->modifiers() & Qt::ShiftModifier )
			OnMoveShiftPageUp();
		else if( e->modifiers() & Qt::ControlModifier )
			OnMoveCtrlPageUp();
        return;
	case Qt::Key_PageDown:
		if( e->modifiers() == Qt::NoModifier || e->modifiers() == Qt::KeypadModifier  )
			OnMovePageDown();
		else if( (e->modifiers() & Qt::ControlModifier) && (e->modifiers() & Qt::ShiftModifier) )
			OnMoveCtrlShiftPageDown();
		else if( e->modifiers() & Qt::ShiftModifier )
			OnMoveShiftPageDown();
		else if( e->modifiers() & Qt::ControlModifier )
			OnMoveCtrlPageDown();
        return;
	case Qt::Key_Backspace:
		if( e->modifiers() == Qt::NoModifier )
			BackSpace();
        return;
	case Qt::Key_Delete:
		if( e->modifiers() == Qt::NoModifier || e->modifiers() == Qt::KeypadModifier )
			Delete();
        return;
    case Qt::Key_Return:
		if( e->modifiers() == Qt::NoModifier || e->modifiers() == Qt::KeypadModifier  )
		{
			rootNode()->MoveReturn();
			updateViewDocument();
			//cbFireEvent( FORMULIB_CALLBACK_ENTER, NULL, NULL );
		}
        return;
    case Qt::Key_Escape:
		if( e->modifiers() == Qt::NoModifier )
		{
			rootNode()->Escape();
			updateFormulatorWidget();
		}
        return;
	default:
		break;
	}

	if( e->matches( QKeySequence::Copy ) )
	{
		editCopy();
		return;
	}
	else if( e->matches( QKeySequence::Cut ) )
	{
		editCut();
		return;
	}
	else if( e->matches( QKeySequence::Paste ) )
	{
		editPaste();
		return;
	}
	if( e->matches( QKeySequence::Delete ) )
	{
		Delete();
		return;
	}
	else if( e->matches( QKeySequence::Undo ) )
	{
		editUndo();
		return;
	}
	else if( e->matches( QKeySequence::Redo ) )
	{
		editRedo();
		return;
	}
	else if( e->matches( QKeySequence::SelectAll ) )
	{
		editSelectAll();
	}
	else if( e->matches( QKeySequence::ZoomIn ) )
	{
		double value = qMin( scale() + DEFAULT_VIEWSCALE_STEP, DEFAULT_VIEWSCALE_MAX );
		setScale( value );
	}
	else if( e->matches( QKeySequence::ZoomOut ) )
	{
		double value = qMax( scale() - DEFAULT_VIEWSCALE_STEP, DEFAULT_VIEWSCALE_MIN );
		setScale( value );
	}
	else if( e->text().length() > 0 )
	{
		for( long i = 0; i < e->text().length(); i++ )
			rootNode()->Char( e->text().at( i ).unicode(), undoRedo() );
		updateViewDocument();
		//cbFireEvent( FORMULIB_CALLBACK_CHAR, nChar, NULL );
	}

	QFormulatorWidget::keyPressEvent( e );
}

void QFormulatorEditWidget::mousePressEvent( QMouseEvent *e )
{
	if( e->button() == Qt::LeftButton )
	{
		PointFde ptf( e->x(), e->y() );
		RectFde rcf = clientRect();
		CFmlDrawEngineQt::DPtoLPEx( ptf, scale(), scale() );
		CFmlDrawEngineQt::DPtoLPEx( rcf, scale(), scale() );

		if( rootNode()->PtInSelection( rcf, ptf ) == 0 )
		{
			editCopy(); // to enable interoperating with Other Applications

			CFrameNode *pFrameNode = rootNode()->RemoveOrCopySelection( 0 );
			if( pFrameNode )
			{
				CRootNode *pRootNode = new CRootNode( this );
				// zero marginds when inserting from a server
				pRootNode->setIndents( 0, 0, 0, 0 );
				pRootNode->Insert( pFrameNode );
				QPixmap pixmap;
				if( ::isOption_Perfomance_CopyImage() )
					pixmap = drawSelection2QPixmap( true );
				QImage pixImg = pixmap.toImage();
				QMimeData *mimeData = CFormulatorDoc::copyToMimeData( pRootNode, pixmap.isNull() ? 0 : &pixImg );
				if( mimeData )
				{
					QDrag *drag = new QDrag( this );
					drag->setMimeData( mimeData );
					if( !pixmap.isNull() )
					{
						const int dragPixmapMaxSize = 256;
						if( qMax(pixmap.width(), pixmap.height()) > dragPixmapMaxSize )
							drag->setPixmap( pixmap.width() > pixmap.height() ? pixmap.scaledToWidth( dragPixmapMaxSize ) : pixmap.scaledToHeight( dragPixmapMaxSize ) );
						else
							drag->setPixmap( pixmap );
					}
					drag->setHotSpot( QPoint(0, 0) );

					clearRemovedSelection();
					Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::MoveAction);
					if( dropAction == Qt::MoveAction && isRemovedSelection() == 0 )
					{
						CFrameNode *pRemovedFrame = rootNode()->RemoveOrCopySelection( 1 );
						if( pRemovedFrame )
						{
							delete pRemovedFrame;
							setRemovedSelection();
						}
						if( !hasFocus() ) setFocus();
						updateViewDocument();
					}
				}

				delete pRootNode;
				delete pFrameNode;
			}
		}
		else if( rootNode()->MoveCaret( rcf, ptf, e->modifiers() & Qt::ShiftModifier ) == 0 )
		{
			updateFormulatorWidget();
		}

		//cbFireEvent( FORMULIB_CALLBACK_LMOUSECLICK, nFlags, MAKEWPARAM( point.x(), point.y() ) );
	}
	else if( e->button() == Qt::RightButton )
	{
		//cbFireEvent( FORMULIB_CALLBACK_RMOUSECLICK, nFlags, MAKEWPARAM( point.x(), point.y() ) );
	}
}

void QFormulatorEditWidget::mouseMoveEvent( QMouseEvent *e )
{
	if( e->buttons() == Qt::NoButton )
	{
		PointFde ptf( e->x(), e->y() );
		RectFde rcf = clientRect();
		CFmlDrawEngineQt::DPtoLPEx( ptf, scale(), scale() );
		CFmlDrawEngineQt::DPtoLPEx( rcf, scale(), scale() );
		if( ::TestSelection( rcf, rootNode()->GetSelectInfo(), ptf ) == 0 )
			setCursor(Qt::ArrowCursor);
		else
			setCursor(Qt::IBeamCursor);
		return;
	}
	else if( e->buttons() & Qt::LeftButton )
	{
		PointFde ptf( e->x(), e->y() );
		RectFde rcf = clientRect();
		CFmlDrawEngineQt::DPtoLPEx( ptf, scale(), scale() );
		CFmlDrawEngineQt::DPtoLPEx( rcf, scale(), scale() );
		if( rootNode()->MoveCaret( rcf, ptf, -1 ) == 0 )
			updateFormulatorWidget();
	}
}

void QFormulatorEditWidget::mouseDoubleClickEvent( QMouseEvent *e )
{
	if( !rootNode() ) return;

	if( e->button() == Qt::LeftButton )
	{
		PointFde ptf( e->x(), e->y() );
		RectFde rcf = clientRect();
		CFmlDrawEngineQt::DPtoLPEx( ptf, scale(), scale() );
		CFmlDrawEngineQt::DPtoLPEx( rcf, scale(), scale() );

		if( rootNode()->DblClk( rcf, ptf ) == 0 )
			updateFormulatorWidget();

		//cbFireEvent( FORMULIB_CALLBACK_LMOUSEDBLCLICK, nFlags, MAKEWPARAM( point.x(), point.y() ) );
	}
	else if( e->button() == Qt::RightButton )
	{
		//cbFireEvent( FORMULIB_CALLBACK_RMOUSEDBLCLICK, nFlags, MAKEWPARAM( point.x(), point.y() ) );
	}
}

void QFormulatorEditWidget::mouseReleaseEvent( QMouseEvent * /*e*/ )
{
	//cbFireEvent( FORMULIB_CALLBACK_LMOUSECLICK, nFlags, MAKEWPARAM( point.x(), point.y() ) );
}

/////////////////////////////////////////////////////////////////////////////
//QHoverEvent
/////////////////////////////////////////////////////////////////////////////

void QFormulatorEditWidget::dragEnterEvent( QDragEnterEvent *event )
{
	if( event->mimeData() && (event->dropAction() == Qt::CopyAction || event->dropAction() == Qt::MoveAction) )
	{
		if( document()->isAcceptedMimeFormat( event->mimeData() ) )
		{
			event->acceptProposedAction();

			setBlinkingCursorEnabled( false );
			rootNode()->GetDrag() = rootNode()->GetCaret();
			updateFormulatorWidget();
		}
	}
}

void QFormulatorEditWidget::dragLeaveEvent( QDragLeaveEvent *event )
{
	setBlinkingCursorEnabled( true );
	rootNode()->GetDrag().SetEmpty();
	updateFormulatorWidget();
	event->accept();
}

void QFormulatorEditWidget::dragMoveEvent( QDragMoveEvent *event )
{
	if( event->mimeData() )
	{
		if( document()->isAcceptedMimeFormat( event->mimeData() )
			/*&& event->answerRect().intersects( geometry() )*/ )
		{
			event->acceptProposedAction();

			PointFde ptf( event->pos().x(), event->pos().y() );
			RectFde rcf = clientRect();
			CFmlDrawEngineQt::DPtoLPEx( ptf, scale(), scale() );
			CFmlDrawEngineQt::DPtoLPEx( rcf, scale(), scale() );
			if( rootNode()->DragCaret( rcf, ptf ) == 0 )
			{
				RectFde dragRect = rootNode()->GetDragRect();
				updateScrollPosition( dragRect.topLeft(), dragRect.size(), 50, 50 );
				update();
			}
		}
	}
}

void QFormulatorEditWidget::dropEvent( QDropEvent *event )
{
	if( event->mimeData() && (event->dropAction() == Qt::CopyAction || event->dropAction() == Qt::MoveAction) )
	{
		PointFde ptf( event->pos().x(), event->pos().y() );
		RectFde rcf = clientRect();
		CFmlDrawEngineQt::DPtoLPEx( ptf, scale(), scale() );
		CFmlDrawEngineQt::DPtoLPEx( rcf, scale(), scale() );
		if( rootNode()->DrInSelection() == 0 || rootNode()->PtInSelection( rcf, ptf ) == 0 )
		{
			setBlinkingCursorEnabled( true );
			rootNode()->GetDrag().SetEmpty();
			rootNode()->GetSelectInfo().SetEmpty();
			rootNode()->MoveCaret( rcf, ptf, 0 );
			updateFormulatorWidget();
		}
		else
		{
			if( event->dropAction() == Qt::MoveAction && isRemovedSelection() == 0 )
			{
				CFrameNode *pFrameNode = rootNode()->RemoveOrCopySelection( 1 );
				if( pFrameNode )
				{
					delete pFrameNode;
					setRemovedSelection();
				}
			}
			else if( event->dropAction() == Qt::CopyAction )
			{
				rootNode()->GetSelectInfo().SetEmpty();
			}

			int iRet = document()->paste( event->mimeData(), false );

			setBlinkingCursorEnabled( true );
			rootNode()->GetDrag().SetEmpty();
			updateViewDocument();

			if( !hasFocus() ) setFocus();
			if( iRet ) event->acceptProposedAction();
		}
	}
	else
	{
		setBlinkingCursorEnabled( true );
		rootNode()->GetDrag().SetEmpty();
		updateFormulatorWidget();
	}
}

/////////////////////////////////////////////////////////////////////////////

void QFormulatorEditWidget::customEvent_MousePressOnParentWindow( QMouseEvent *e )
{
	mousePressEvent( e );
}

void QFormulatorEditWidget::customEvent_MouseMoveOnParentWindow( QMouseEvent *e )
{
	if( e->buttons() & Qt::LeftButton )
	{
		QPoint pos = e->pos();
		mapFromParent( pos );
		PointFde ptf( pos.x(), pos.y() );
		RectFde rcf = clientRect();
		CFmlDrawEngineQt::DPtoLPEx( ptf, scale(), scale() );
		CFmlDrawEngineQt::DPtoLPEx( rcf, scale(), scale() );
		if( rootNode()->MoveCaret( rcf, ptf, -1 ) == 0 )
			updateFormulatorWidget();
	}
	//mouseMoveEvent( e );
}

/////////////////////////////////////////////////////////////////////////////

int QFormulatorEditWidget::editUndo()
{
	if( undoRedo()->CanUndo() )
	{
		undoRedo()->Undo();
		rootNode()->RecalculateSizeFull();
		updateFormulatorWidget();
		setModified();
		setMmlChanged( true );

		emit docModifiedSignal( true );
	}
	return 1;
}

int QFormulatorEditWidget::editRedo()
{
	if( undoRedo()->CanRedo() )
	{
		undoRedo()->Redo();
		rootNode()->RecalculateSizeFull();
		updateFormulatorWidget();
		setModified();
		setMmlChanged( true );

		emit docModifiedSignal( true );
	}
	return 1;
}

int QFormulatorEditWidget::editCut()
{
	QApplication::setOverrideCursor( Qt::WaitCursor );
	QImage img;
	if( ::isOption_Perfomance_CopyImage() )
		img = drawSelection2Image( true );
	if( document()->copy( img.isNull() ? 0 : &img, true ) )
		updateViewDocument();
	QApplication::restoreOverrideCursor();
	return 1;
}

int QFormulatorEditWidget::editCopy()
{
	QApplication::setOverrideCursor( Qt::WaitCursor );
	QImage img;
	if( ::isOption_Perfomance_CopyImage() )
		img = drawSelection2Image( true );
	document()->copy( img.isNull() ? 0 : &img );
	QApplication::restoreOverrideCursor();
	return 1;
}

int QFormulatorEditWidget::editPaste()
{
	QApplication::setOverrideCursor( Qt::WaitCursor );
	if( document()->paste() ) updateViewDocument();
	QApplication::restoreOverrideCursor();
	return 1;
}

int QFormulatorEditWidget::editDelete()
{
	Delete();
	return 1;
}

int QFormulatorEditWidget::editSelectAll()
{
	if( rootNode()->SelectAll() == 0 )
		updateFormulatorWidget();
	return 1;
}

/////////////////////////////////////////////////////////////////////////////

void QFormulatorEditWidget::BackSpace()
{
	if( rootNode()->MoveBs( undoRedo() ) == 0 )
		updateViewDocument();
	else if( rootNode()->IsSelection() )
		updateFormulatorWidget();
}

void QFormulatorEditWidget::Delete( void )
{
	if( rootNode()->MoveDel( undoRedo() ) == 0 )
		updateViewDocument();
	else if( rootNode()->IsSelection() )
		update();
}

void QFormulatorEditWidget::Clear( void )
{
	if( rootNode()->ClearAll() == 0 )
		updateViewDocument();
}

/////////////////////////////////////////////////////////////////////////////

void QFormulatorEditWidget::MoveLR( int fLeft, int fSelect, int fWord )
{
	if( rootNode()->MoveLeftRight( fLeft, fSelect, fWord ) == 0 )
		updateFormulatorWidget();
}

void QFormulatorEditWidget::MoveUD( int fUp, int fSelect )
{
	RectFde rcf = clientRect();
	CFmlDrawEngineQt::DPtoLPEx( rcf, scale(), scale() );

	PointFde ptf = caretPos();
	SizeFde szf = caretSize();

	if( rootNode()->MoveUpDown( rcf, ptf, szf, fUp, fSelect ) == 0 )
		updateFormulatorWidget();
}

void QFormulatorEditWidget::MoveHE( int fHome, int fSelect, int fCtrl )
{
	if( rootNode()->MoveHomeEnd( fHome, fSelect, fCtrl ) == 0 )
		updateFormulatorWidget();
}

void QFormulatorEditWidget::MovePG( int fUp, int fSel )
{
	RectFde rcf = clientRect();
	SizeFde ofs( 0, 0 );
	CFmlDrawEngineQt::DPtoLPEx( rcf, scale(), scale() );
	CFmlDrawEngineQt::DPtoLPEx( ofs, scale(), scale() );
	if( rootNode()->MovePgUpDown( rcf, ofs, fUp, fSel ) == 0 )
		updateFormulatorWidget();
}

/////////////////////////////////////////////////////////////////////////////
