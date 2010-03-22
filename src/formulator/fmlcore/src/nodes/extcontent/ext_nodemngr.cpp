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

#include <QDir>
#include "HUtils/iloadimage.h"
#include "../../settings/options.h"
#include "../node.h"
#include "ext_nodemngr.h"
#include "ext_node.h"
#include "extc_classid.h"

/////////////////////////////////////////////////////////////////////////////

CExtNode* CreateExtNode( long externalNodeClassID, void *pData )
{
	switch( externalNodeClassID )
	{
	case EXTNODECONTENT_ID_TEST:
		{
			CExtNodeContent_Test *_obj = new CExtNodeContent_Test();
			_obj->Init( pData );

			CExtNode *pNewNode = new CExtNode();
			pNewNode->setContent( _obj );

			return pNewNode;
		}
		break;
	case EXTNODECONTENT_ID_IMG:
		{
			CExtNodeContent_Image *_obj = new CExtNodeContent_Image();
			_obj->Init( pData );

			CExtNode *pNewNode = new CExtNode();
			pNewNode->setContent( _obj );

			return pNewNode;
		}
		break;
	default:
		break;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

void* PrepareExtNodeData( long extNodeClasssID, const QString& strImgPath, const QString& strDocPath )
{
	switch( extNodeClasssID )
	{
	case EXTNODECONTENT_ID_TEST:
		break;

	case EXTNODECONTENT_ID_IMG:
		{
			QString strPathName = _T("");
			if( strImgPath != NULL )
			{
				strPathName = strImgPath;
			}
			else
			{
				int iRet = ::LoadImagePath( strPathName );
				if( iRet == -1 || strPathName == _T( "" ) )
					return NULL;
			}

			if( strDocPath.length() > 0 )
				strPathName = QDir(strDocPath).relativeFilePath(strPathName);
			return new CExtNodeContent_Image_Data( 
				EXTNODECONTENT_ID_IMG__FLAG_ATTACHED, strPathName );
		}
		break;

	default:
		break;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
