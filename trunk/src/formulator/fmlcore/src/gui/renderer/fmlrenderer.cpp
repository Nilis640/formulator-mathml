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

#include "fmlrenderer.h"

#if !defined( FML_NO_RENDERER )

#include "../widget/fmlwidget.h"
#include "../../button/toolbarset.h"
#include "../../settings/options.h"
#include "../../version/version.h"

/////////////////////////////////////////////////////////////////////////////

QFormulatorRenderer::QFormulatorRenderer(QObject *parent)
	: QObject(parent)
{
	init();
}

QFormulatorRenderer::QFormulatorRenderer(const QString& filename, QObject *parent)
	: QObject(parent)
{
	init();
	load(filename);
}

QFormulatorRenderer::QFormulatorRenderer(const QByteArray& contents, QObject *parent)
	: QObject(parent)
{
	init();
	load(contents);
}

QFormulatorRenderer::~QFormulatorRenderer()
{
	if( m_engine )
	{
		delete m_engine;
		m_engine = 0;
	}
}

void QFormulatorRenderer::init()
{
	FBtnToolbarSet *toolbarSet = ::getCurrentToolBarSet();
	if( toolbarSet == 0 || toolbarSet->getToolBarCount() == 0 )
	{
		::setCurrentFormulatorContentMathML_TranslateTimes( ::Formulib_GetTimes2Times() );
		::setCurrentFormulatorStyle_TranslatePref( ::Formulib_GetTranslator() );
		::setCurrentFormulatorOptionsValue( ::Formulib_GetOptionsValue() );
		QFormulatorWidget::initFormulatorCore();
	}

	m_engine = new QFormulatorWidget();
	m_engine->initialUpdate();
	setMargins(0, 0, 0, 0);
	m_isValid = false;
}

QString QFormulatorRenderer::text()
{
	return m_engine->text();
}

QSize QFormulatorRenderer::defaultSize() const
{
	return m_engine->size();
}

bool QFormulatorRenderer::isValid() const
{
	return m_isValid;
}

void QFormulatorRenderer::setScale( double value )
{
	m_engine->setScale( value );
}

double QFormulatorRenderer::scale()
{
	return m_engine->scale();
}

void QFormulatorRenderer::setBackColor( QColor color )
{
	m_engine->setBackColor( color );
}

QColor QFormulatorRenderer::backColor()
{
	return m_engine->backColor();
}

void QFormulatorRenderer::setLineSpacing( double value )
{
	m_engine->setLineSpacing( value );
}

double QFormulatorRenderer::lineSpacing()
{
	return m_engine->lineSpacing();
}

void QFormulatorRenderer::margins( double& l, double& t, double& r, double& b )
{
	m_engine->indents(l, t, r, b);
}

void QFormulatorRenderer::setMargins( double l, double t, double r, double b )
{
	m_engine->setIndents(l, t, r, b);
}

bool QFormulatorRenderer::load(const QString& filename)
{
	m_isValid = (m_engine->fileOpenSilent( filename ) == 1);
	if( m_isValid )
		emit repaintNeeded();
	return m_isValid;
}

bool QFormulatorRenderer::load(const QByteArray& contents)
{
	m_engine->clear();
	m_isValid = (m_engine->insertMathML( __FMLCORE_Product_Title, QString(contents), 0 ) == 1);
	if( m_isValid )
		emit repaintNeeded();
	return m_isValid;
}

bool QFormulatorRenderer::reload()
{
	m_isValid = (m_engine->refreshViaMathML() == 1);
	if( m_isValid )
		emit repaintNeeded();
	return m_isValid;
}

void QFormulatorRenderer::render(QPainter *painter)
{
	QRectF bounds(0, 0, m_engine->size().width(), m_engine->size().height());
	m_engine->render(painter, bounds);
}

void QFormulatorRenderer::render(QPainter *painter, const QRectF& bounds)
{
	m_engine->render(painter, bounds);
}

bool QFormulatorRenderer::exportImage(const QString& filename)
{
	return (m_engine->fileExportImage(filename) == 1) ? true : false;
}

/////////////////////////////////////////////////////////////////////////////

#endif
