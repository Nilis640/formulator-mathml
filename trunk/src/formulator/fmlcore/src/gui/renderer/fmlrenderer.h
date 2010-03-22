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

#if !defined( __FMLCORE_FMLRENDERER_H__ )
#define __FMLCORE_FMLRENDERER_H__

#if !defined( FML_NO_RENDERER )

#include <QObject>
#include <QSize>
#include <QColor>
#include <QByteArray>
#include <QXmlStreamReader>
#include <QPainter>

#include "../../fmlcore_global.h"

class QFormulatorWidget;

/////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ QFormulatorRenderer
	: public QObject
{
	Q_OBJECT

protected:
	QFormulatorWidget		*m_engine;
	bool					m_isValid;

public:
	QFormulatorRenderer(QObject *parent = 0);
	QFormulatorRenderer(const QString& filename, QObject *parent = 0);
	QFormulatorRenderer(const QByteArray& contents, QObject *parent = 0);
	~QFormulatorRenderer();

	QString text();
	QSize defaultSize() const;
	bool isValid() const;

	void setScale(double value);
	double scale();

	void setBackColor(QColor color);
	QColor backColor();

	void setLineSpacing(double value);
	double lineSpacing();

	void margins( double& l, double& t, double& r, double& b );
	void setMargins( double l, double t, double r, double b );

public slots:
	bool load(const QString& filename);
	bool load(const QByteArray& contents);

	bool reload();

	void render(QPainter *painter);
	void render(QPainter *painter, const QRectF& bounds );

	bool exportImage(const QString& filename);

protected:
	void init();

signals:
	void repaintNeeded();
};

#endif	// FML_NO_RENDERER

/////////////////////////////////////////////////////////////////////////////

#endif
