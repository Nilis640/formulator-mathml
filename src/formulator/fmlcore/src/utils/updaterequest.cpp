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
#include <QtNetwork>

#include "HUtils/iversion.h"
#include "HMathML/isettings.h"
#include "updaterequest.h"

CUpdateRequest::CUpdateRequest( QObject *uiobject )
{
	m_buffer.open( QIODevice::WriteOnly );
	m_isHttpAborted = false;
	m_http = new QHttp( this );
	m_http->setHost( __COMPANY_HOST_ );
	QString uid = QUrl::toPercentEncoding(::Formulib_GetCheck4UpdatesUid());
	QString ver = QUrl::toPercentEncoding(::Formulib_GetCheck4UpdatesBuild());
	QString connectRequest = QString("/hdata/update_fwqt.php?uid=%1&product=fw&ver=%2").arg(uid).arg(ver);
	m_id = m_http->get(connectRequest, &m_buffer);

    connect(m_http, SIGNAL(requestFinished(int, bool)), this, SLOT(requestFinished(int, bool)));
    connect(m_http, SIGNAL(dataReadProgress(int, int)), this, SLOT(dataReadProgress(int, int)));
    connect(m_http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)), this, SLOT(readResponseHeader(const QHttpResponseHeader &)));
    connect(this, SIGNAL(requestOk()), uiobject, SLOT(updateOk()));
}

CUpdateRequest::~CUpdateRequest()
{
	if( m_http->hasPendingRequests() )
		m_http->abort();
	delete m_http;
}

void CUpdateRequest::requestFinished( int id, bool error )
{
	if( m_id != id || m_isHttpAborted ) return;
	if( !error )
	{
		m_errorMsg = "";
		emit requestOk();
	}
	else
		m_errorMsg = m_http->errorString();
}

void CUpdateRequest::dataReadProgress( int done, int total )
{
	if( done > MaxExpectedReplyTotal || total > MaxExpectedReplyTotal )
	{
		m_isHttpAborted = true;
		m_http->abort();
	}
}

QString CUpdateRequest::text()
{
	return QString(m_buffer.buffer());
}

void CUpdateRequest::readResponseHeader(const QHttpResponseHeader &responseHeader)
{
    switch (responseHeader.statusCode())
	{
    case 200:                   // Ok
    case 301:                   // Moved Permanently
    case 302:                   // Found
    case 303:                   // See Other
    case 307:                   // Temporary Redirect
        // these are not error conditions
        break;

    default:
        m_isHttpAborted = true;
        m_http->abort();
    }
}

/////////////////////////////////////////////////////////////////////////////
