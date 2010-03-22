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

#ifndef __FMLCORE_DLG_MATHMLHIGHLIGHTER_H__
#define __FMLCORE_DLG_MATHMLHIGHLIGHTER_H__

#include <QSyntaxHighlighter>
#include <QHash>
#include <QMap>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class QMathMLHighlighter
	: public QSyntaxHighlighter
{
   Q_OBJECT

protected:
	bool					m_enableMode;
	QMap<QString, bool>		m_tagMap;
	QMap<QString, QString>	m_attrMap;

public:
    QMathMLHighlighter( QTextDocument *parent = 0 );

	bool isHighlightMode();

public slots:
	void setHighlightMode( bool enabled );

protected:
    void highlightBlock( const QString &text );

protected:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat knownTagFormat, knownAttrFormat;
    QTextCharFormat tagNameFormat, attrNameFormat, commentFormat, opFormat, quotationFormat;

signals:
	void dummySignal();
};

#endif
