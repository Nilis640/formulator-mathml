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

#ifndef __FMLCORE_UTILS_UPDATEREQUEST_H__
#define __FMLCORE_UTILS_UPDATEREQUEST_H__

#include <QDialog>
#include <QBuffer>

QT_BEGIN_NAMESPACE
class QHttp;
class QHttpResponseHeader;
class QByteArray;
class QString;
QT_END_NAMESPACE

class CUpdateRequest : public QObject
{
	Q_OBJECT

public:
    CUpdateRequest( QObject *uiobject );
    ~CUpdateRequest();
	QString text();

protected slots:
    void requestFinished( int id, bool error );
	void dataReadProgress( int done, int total );
	void readResponseHeader(const QHttpResponseHeader &responseHeader);

protected:
    QHttp*			m_http;
    int				m_id;
	QBuffer			m_buffer;
	QString			m_errorMsg;
	bool			m_isHttpAborted;
	enum
	{
		MaxExpectedReplyTotal = 1024
	};

signals:
	void requestOk();
};

#endif
