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

/////////////////////////////////////////////////////////////////////////////

const static int CELLSIZE = 32;
const static int COLUMNS = 16;

CInsertCharacterWidget::CInsertCharacterWidget(QWidget *parent)
    : QWidget(parent)
{
    squareSize = CELLSIZE;
    columns = COLUMNS;
    lastKey = -1;
	color = QColor(0, 0, 0);
    setMouseTracking( true );
}

void CInsertCharacterWidget::updateFont(const QFont &font)
{
    displayFont.setFamily(font.family());
    squareSize = qMax(CELLSIZE, QFontMetrics(displayFont).xHeight() * 3);
    adjustSize();
    update();
}

void CInsertCharacterWidget::updateSize(const QString &fontSize)
{
    displayFont.setPointSize(fontSize.toInt());
    squareSize = qMax(CELLSIZE, QFontMetrics(displayFont).xHeight() * 3);
    adjustSize();
    update();
}

void CInsertCharacterWidget::updateStyle(const QString &fontStyle)
{
    QFontDatabase fontDatabase;
    const QFont::StyleStrategy oldStrategy = displayFont.styleStrategy();
    displayFont = fontDatabase.font(displayFont.family(), fontStyle, displayFont.pointSize());
    displayFont.setStyleStrategy(oldStrategy);
    squareSize = qMax(CELLSIZE, QFontMetrics(displayFont).xHeight() * 3);
    adjustSize();
    update();
}

void CInsertCharacterWidget::updateFontMerging(bool enable)
{
	displayFont.setStyleStrategy( enable ? QFont::PreferDefault : QFont::NoFontMerging );
    adjustSize();
    update();
}

void CInsertCharacterWidget::updateColor( const QColor &fontColor )
{
	color = fontColor;
    update();
}

QSize CInsertCharacterWidget::sizeHint() const
{
    return QSize(columns * squareSize, (65536 / columns) * squareSize);
}

void CInsertCharacterWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint widgetPosition = mapFromGlobal(event->globalPos());
    uint key = (widgetPosition.y() / squareSize) * columns + widgetPosition.x() / squareSize;

    QString text = QString::fromLatin1("<p>Character: <span style=\"font-size: 24pt; font-family: %1\">").arg(displayFont.family())
                  + QChar(key)
                  + QString::fromLatin1("</span><p>Value: 0x")
                  + QString::number(key, 16);
    QToolTip::showText(event->globalPos(), text, this);
}

void CInsertCharacterWidget::mousePressEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton)
	{
        lastKey = (event->y() / squareSize) * columns + event->x() / squareSize;
        if( QChar(lastKey).category() != QChar::NoCategory )
            emit characterSelected(QString(QChar(lastKey)));
        update();
    }
    else
        QWidget::mousePressEvent( event );
}

void CInsertCharacterWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter( this );
    painter.fillRect( event->rect(), QBrush(Qt::white) );
    painter.setFont( displayFont );

    QRect redrawRect = event->rect();
    int beginRow = redrawRect.top() / squareSize;
    int endRow = redrawRect.bottom() / squareSize;
    int beginColumn = redrawRect.left() / squareSize;
    int endColumn = redrawRect.right() / squareSize;

    painter.setPen(QPen(Qt::gray));
    for (int row = beginRow; row <= endRow; ++row)
        for (int column = beginColumn; column <= endColumn; ++column)
            painter.drawRect( column * squareSize, row * squareSize, squareSize, squareSize);

    QFontMetrics fontMetrics( displayFont );
    for (int row = beginRow; row <= endRow; ++row)
	{
        for (int column = beginColumn; column <= endColumn; ++column)
		{
			//painter.setPen( QPen(Qt::black) );
			painter.setPen( color );
            int key = row * columns + column;
            painter.setClipRect( column * squareSize, row * squareSize, squareSize, squareSize );

            if (key == lastKey)
                painter.fillRect(column*squareSize + 1, row*squareSize + 1, squareSize, squareSize, QBrush(Qt::yellow));

            painter.drawText(column*squareSize + (squareSize / 2) - fontMetrics.width(QChar(key))/2,
                             row*squareSize + 4 + fontMetrics.ascent(),
                             QString(QChar(key)));
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
