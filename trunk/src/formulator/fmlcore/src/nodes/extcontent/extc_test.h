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

#if !defined( __FORMULATOR_EXTCONTENT_EXTC_TEST_H__ )
#define __FORMULATOR_EXTCONTENT_EXTC_TEST_H__

#include "../../fmlcore_global.h"
#include "ext_node.h"

/////////////////////////////////////////////////////////////////////////////

#define EXTNODECONTENT_ID_TEST	0

/////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CExtNodeContent_Test : public CExtNodeContent_Base
{
public:
	CExtNodeContent_Test( void );
	CExtNodeContent_Test( const CExtNodeContent_Test& _content );

public:
	virtual void Init( void *pData );
	virtual CExtNodeContent_Base* Clone( void );

	virtual void Store( QDataStream& ar ) const;
	virtual void Load( QDataStream& ar );

	void Draw( CFmlDrawEngine& fde, const PointFde& tl, const RectFde& rc );
};

/////////////////////////////////////////////////////////////////////////////

#endif//__FORMULATOR_EXTCONTENT_EXTC_TEST_H__
