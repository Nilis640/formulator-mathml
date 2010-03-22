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

///////////////////////////////////////////////////////////////////////////////

#include "HUtils/idfas.h"
#include "xml_tree.h"

///////////////////////////////////////////////////////////////////////////////

AST_TXML_Compound::~AST_TXML_Compound( void )
{
	for( long i = 0; i < (long) stmt.ArrayDim(); i++ )
		if( stmt[ i ] ) delete stmt[ i ];
}

AST_TXML_Tag* AST_TXML_Compound::getChildByName( const QString& name )
{
	for( long i = 0; i < (long) stmt.ArrayDim(); i++ )
	{
		if( stmt[ i ] && 
			stmt[ i ]->getNodeType() == ASTT_TXML_Tag &&
			((AST_TXML_Tag*) stmt[ i ])->getTagName() == name )
		{
			return (AST_TXML_Tag*) stmt[ i ];
		}
	}
	return NULL;
}

QString AST_TXML_Compound::getDeepChildValueText( void )
{
	QString ret = _T("");
	for( long i = 0; i < (long) stmt.ArrayDim(); i++ )
		if( stmt[ i ] ) ret += stmt[ i ]->getDeepChildValueText();
	return ret;
}

AST_TXML_Compound::AST_TXML_Compound( const AST_TXML_Compound& a )
	: AST_TXML_Node( a )
	, stmt(MIArray<AST_TXML_Node>::LeaveItems, a.getChildNumber(), 0, 8)
{
	for( long i = 0; i < a.getChildNumber(); i++ )
		if( a.stmt[ i ] ) stmt.Add( a.stmt[ i ]->Clone() );
}

void AST_TXML_Compound::Append( const AST_TXML_Compound& a )
{
	for( long i = 0; i < a.getChildNumber(); i++ )
		if( a.stmt[ i ] ) stmt.Add( a.stmt[ i ]->Clone() );
}

void AST_TXML_Compound::Prepend( std::vector<AST_TXML_Tag*>& nodeList, int isMove )
{
	stmt.insert( stmt.begin(), nodeList.begin(), nodeList.end() );
	if( isMove ) nodeList.clear();
}

void AST_TXML_Compound::AppendAttribute( const AST_TXML_Compound& a )
{
	for( long i = 0; i < a.getChildNumber(); i++ )
	{
		if( a.stmt[ i ] && 
			a.stmt.at(i)->getNodeType() == ASTT_TXML_Attribute &&
			!getFirstAttribute( ((AST_TXML_Attribute*)a.stmt[ i ])->getAttributeName() ) )
			stmt.Add( a.stmt[ i ]->Clone() );
	}
}

void AST_TXML_Compound::detachChildren( void )
{
	for( long i = 0; i < (long)stmt.ArrayDim(); i++ )
		stmt[ i ] = 0;
	stmt.Flush();
}

void AST_TXML_Compound::formatBlank( void )
{
	for( long i = 0; i < (long)stmt.ArrayDim(); i++ )
		if( stmt[ i ] ) stmt[ i ]->formatBlank();
}

void AST_TXML_Compound::getXML( QString& str, HKLongFlag mode )
{
	for( long i = 0; i < (long)stmt.ArrayDim(); i++ )
		if( stmt[ i ] ) stmt[ i ]->getXML( str, mode );
}
void AST_TXML_Compound::setLevelThrouhTree( long ln )
{
	level = ln;
	for( long i = 0; i < (long)stmt.ArrayDim(); i++ )
		if( stmt[ i ] ) stmt[ i ]->setLevelThrouhTree( ln );
}

int AST_TXML_Attribute::convert2Integer( void )
{
	int ret;
	switch( type )
	{
	case ASTVALT_TXML_Int:
		ret = ELH_NO_ERROR;
		break;
	case ASTVALT_TXML_Real:
		ret = ELH_SOFT_ERROR;
		type = ASTVALT_TXML_Int;
		larg = long( farg );
		break;
	case ASTVALT_TXML_String:
		ret = _hatol( QStringRef(&sarg), larg );
		if( !ret ) type = ASTVALT_TXML_Int;
		break;
	default:
		ret = ELH_CRITICAL_ERROR;
	}
	return ret;
}

int AST_TXML_Attribute::convert2Float( void )
{
	int ret;
	switch( type )
	{
	case ASTVALT_TXML_Int:
		farg = double( larg );
		ret = ELH_NO_ERROR;
		type = ASTVALT_TXML_Real;
		break;
	case ASTVALT_TXML_Real:
		ret = ELH_NO_ERROR;
		break;
	case ASTVALT_TXML_String:
		ret = _hatof( QStringRef(&sarg), farg );
		if( !ret ) type = ASTVALT_TXML_Real;
		break;
	default:
		ret = ELH_CRITICAL_ERROR;
	}
	return ret;
}

void AST_TXML_Attribute::getXML( QString& str, HKLongFlag /*mode*/ )
{
	if( name.length() == 0 )
	{
		switch( type )
		{
		case ASTVALT_TXML_Int:	//? delete this code in very near future
			str += QString(" %1").arg(larg);
			break;
	    
		case ASTVALT_TXML_Real:	//? delete this code in very near future
			str += QString(" %1").arg(farg);
			break;
	    
		case ASTVALT_TXML_String:
			str += QString(" %1").arg(sarg);
			break;

		default:
			break;
		}
	}
	else
	{
		switch( type )
		{
		case ASTVALT_TXML_Int:	//? delete this code in very near future
			str += QString(" %1=%2").arg(name).arg(larg);
			break;
	    
		case ASTVALT_TXML_Real:	//? delete this code in very near future
			str += QString(" %1=%2").arg(name).arg(farg);
			break;
	    
		case ASTVALT_TXML_String:
			str += QString(" %1=\"%2\"").arg(name).arg(sarg);
			break;

		default:
			break;
		}
	}
}

void AST_TXML_Tag::deleteValueChildren( void )
{
	if( children )
	{
		for( long i = 0; i < children->getChildNumber(); )
		{
			if( children->getChild( i ) )
			{
				if( children->getChild( i )->getNodeType() == ASTT_TXML_Value )
					children->Erase( i );
				else
					i++;
			}
			else
				i++;
		}
	}
}

void AST_TXML_Tag::deleteTagChildren( void )
{
	if( children )
	{
		for( long i = 0; i < children->getChildNumber(); )
		{
			if( children->getChild( i ) )
			{
				if( children->getChild( i )->getNodeType() == ASTT_TXML_Tag )
					children->Erase( i );
				else
					i++;
			}
			else
				i++;
		}
	}
}

void AST_TXML_Tag::deleteChild( long idx )
{
	if( children )
		children->Erase( idx );
	text = regetSumText();
}

QString AST_TXML_Tag::regetSumText( void )
{
	existTagChild = 0;
	text = _T("");
	if( children )
	{
		for( long i = 0; i < children->getChildNumber(); i++ )
		{
			if( children->getChild( i ) )
			{
				switch( children->getChild( i )->getNodeType() )
				{
				case ASTT_TXML_Value:
					text += ((AST_TXML_Value*)children->getChild( i ))->getText();
					break;
				case ASTT_TXML_Tag:
					existTagChild = 1;
					break;
				default:
					break;
				}
			}
		}
	}
	return text;
}

void AST_TXML_Tag::putValue2Tag( long index, QString tagname, AST_TXML_Compound *_attr )
{
	if( !children || 
		!children->getChild( index ) || 
		children->getChild( index )->getNodeType() != ASTT_TXML_Value )
		return;
	AST_TXML_Tag *new_tag = new AST_TXML_Tag( &tagname, _attr, (AST_TXML_Value*)(children->getChild( index )) );
	children->ReplaceChild( index, new_tag );
	text = regetSumText();
	existTagChild = 1;
}

void AST_TXML_Tag::putValue2Tag( QString tagname, AST_TXML_Compound *_attr )
{
	if( !children ) return;

	for( long i = 0; i < children->getChildNumber(); i++ )
	{
		if( children->getChild( i ) )
		{
			existTagChild = 1;
			if( children->getChild( i )->getNodeType() == ASTT_TXML_Value )
			{
				AST_TXML_Tag *new_tag = new AST_TXML_Tag( &tagname, _attr, (AST_TXML_Value*)(children->getChild( i )) );
				children->ReplaceChild( i, new_tag );
			}
		}
	}

	text = _T("");
}

void AST_TXML_Tag::setTagName( QString nm )
{
	int pos = nm.indexOf(':');
	if( pos == -1 )
		name = nm;
	else
	{
		ns = nm.mid( 0, pos );
		name = nm.mid( pos + 1 );
	}
}

AST_TXML_Tag::AST_TXML_Tag( QString *_name, AST_TXML_Compound *_attr, AST_TXML_Value *_value, HKLongFlag _mode )
{
	node_type = ASTT_TXML_Tag;
	ns = _T("");
	setTagName( *_name );
	attr = _attr;
	flags = _mode;
	if( _value )
	{
		children = new AST_TXML_Compound();
		children->Add( _value );
	}
	else
		children = 0;
	text = regetSumText();
}

AST_TXML_Tag::AST_TXML_Tag( QString *_name, AST_TXML_Compound *_attr, 
							AST_TXML_Tag* _tag, HKLongFlag _mode )
{
	node_type = ASTT_TXML_Tag;

	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( _tag );

	ns = _T("");
	setTagName( *_name );
	attr = _attr;
	flags = _mode;
	children = _tags;
	text = regetSumText();
}

AST_TXML_Tag::AST_TXML_Tag( QString *_name, AST_TXML_Compound *_attr, AST_TXML_Compound *_tags, HKLongFlag _mode )
{
	node_type = ASTT_TXML_Tag;
	ns = _T("");
	setTagName( *_name );
	attr = _attr;
	flags = _mode;
	children = _tags;
	text = regetSumText();
}

void AST_TXML_Tag::getXML( QString& str, HKLongFlag mode )
{
	int spc = mode.isSetFlag( FL_XML_PRINT_TAG_SINGLE_SPACING ) ? 1 : 2;
	QString _tag_name = getNS().length() ? (getNS() + _T(":")) : _T("");
	_tag_name += getTagName();

	QString pre1 = _T(""), pre2 = _T("");
	if( getLevel() > 0 ) 
		pre1 = QString( spc * getLevel(), QChar(' ') );
	if( !mode.isSetFlag( FL_XML_PRINT_TAG_ONE_LINE ) ) pre2 = pre1;

	if( str.length() > 0 && str[ str.length() - 1 ] != _T('\n') ) str += _T('\n');

	if( mode.isSetFlag( FL_XML_PROCESSING_INSTRUCTION ) )
	{
		QString te = mode.isSetFlag( FL_XML_SELF_CLOSE_TAG ) ? _T("/>") : _T("?>");
		str += pre1 + _T("<?") + _tag_name;
		if( isAttr() ) attr->getXML( str, mode );
		str += te + _T("\n");
		return;
	}

	if( getTags() )
	{
		int is_n = 0, is_empty = 0;
		QString _t = _T(""), _a = _T("");
		getTags()->getXML( _t, mode );
		if( isAttr() )
			attr->getXML( _a, mode );
		if( _t.length() + _a.length() + 2 * _tag_name.length() < 60 )
			is_n = 1;
		str += pre1 + _T("<") + _tag_name;
		str += _a;
		if( _t.length() == 0 )
		{
			is_empty = 1;
			str += _T("/");
		}
		if( mode.isSetFlag( FL_XML_PRINT_TAG_ONE_LINE ) || 
			!is_empty || is_n || str[ str.length() - 1 ] == _T('\n') )
			str += _T(">");
		else
			str += _T(">\n");
		if( !is_empty )
		{
			str += _t; 
			int is_endl = str[ str.length() - 1 ] == _T('\n');
			if( !mode.isSetFlag( FL_XML_PRINT_TAG_ONE_LINE ) && !is_n && !is_endl )
				str += _T('\n') + QString( _T("</") ) + _tag_name + QString( _T(">\n") );
			else
			{
				if( is_endl ) str += pre2;
				str += QString( _T("</") ) + _tag_name + QString( _T(">\n") );
			}
		}
	}
	else
	{
		if( flags.isSetFlag( FL_XML_SELF_CLOSE_TAG ) )
		{
			str += pre1 + _T("<") + _tag_name;
			if( isAttr() ) attr->getXML( str, mode );
			str += _T("/>\n");
		}
		else
		{
			str += pre1 + _T("<") + _tag_name;
			if( isAttr() ) attr->getXML( str, mode );
			if( !mode.isSetFlag( FL_XML_PRINT_TAG_ONE_LINE ) ) str += _T(">\n");
			else  str += _T(">");
			str += pre2 + QString( _T("</") ) + _tag_name + QString( _T(">\n") );
		}
	}
}

double AST_TXML_Attribute::getNumericValue( void )
{
	switch( getValueType() )
	{
	case ASTVALT_TXML_Int:
		return larg;
	case ASTVALT_TXML_Real:
		return farg;
	case ASTVALT_TXML_String:
		return sarg.toDouble();
	default:
		break;
	}
	return 0.0;
}

QString& AST_TXML_Attribute::getStringValue( void )
{
	switch( getValueType() )
	{
	case ASTVALT_TXML_Int:
		sarg = QString("%1").arg(larg);
		break;

	case ASTVALT_TXML_Real:
		sarg = QString("%1").arg(farg);
		break;

	default:
		break;
	}
	return sarg;
}

void AST_TXML_Compound::setAttrMap( HKStringMap& tomap )
{
	tomap.erase( tomap.begin(), tomap.end() );
	for( long i = 0; i < (long)stmt.ArrayDim(); i++ )
		if( stmt[ i ] && stmt[ i ]->getNodeType() == ASTT_TXML_Attribute )
			((AST_TXML_Attribute*)stmt[ i ])->setAttrMap( tomap );
}

AST_TXML_Attribute* AST_TXML_Compound::getFirstAttribute( const QString& name )
{
	for( long i = 0; i < (long)stmt.ArrayDim(); i++ )
		if( stmt[ i ] && 
			stmt[ i ]->getNodeType() == ASTT_TXML_Attribute &&
			!xml_strcmp( ((AST_TXML_Attribute*)stmt[ i ])->getAttributeName(), name ) )
			return (AST_TXML_Attribute*)stmt[ i ];
	return 0;
}

AST_TXML_Tag* AST_TXML_Compound::getFirstTag( const QString& name )
{
	AST_TXML_Tag* ret = 0;
	for( long i = 0; i < (long)stmt.ArrayDim(); i++ )
		if( stmt[ i ] && 
			(ret = stmt[ i ]->getFirstTag( name )) != 0 )
			break;
	return ret;
}

void AST_TXML_Compound::getAllTags( const QString& name, std::vector<AST_TXML_Tag*>& tags, long level )
{
	if( !level )
		return;
	for( long i = 0; i < (long)stmt.ArrayDim(); i++ )
		if( stmt[ i ] )
			stmt[ i ]->getAllTags( name, tags, (level == -1) ? -1 : --level );
}

AST_TXML_Attribute *AST_TXML_Tag::getAttribute( const QString& attr_name )
{
	if( !attr ) return 0;
	AST_TXML_Node *node;
	for( long i = 0; i < (long)attr->getChildNumber(); i++ )
	{
		node = attr->getChild( i );
		if( node->getNodeType() != ASTT_TXML_Attribute )
			continue;
		if( ((AST_TXML_Attribute*)node)->isAttribute( attr_name ) )
			return (AST_TXML_Attribute*)node;
	}
	return 0;
}

void AST_TXML_Value::formatBlank( void )
{
	text = ::formatBlankAsXML( text );
}

void AST_TXML_Tag::formatBlank( void )
{
	if( attr )
		for( long i = 0; i < attr->getChildNumber(); i++ )
			if( attr->getChild( i ) ) attr->getChild( i )->formatBlank();
	if( children )
	{
		for( long i = 0; i < children->getChildNumber(); i++ )
			if( children->getChild( i ) ) children->getChild( i )->formatBlank();
		regetSumText();
	}
}

///////////////////////////////////////////////////////////////////////////////

QString formatBlankAsXML( const QString& text )
{
	QString ret = _T("");
	int st = 0, ost = 0;
	static int tbl[ 3 ][ 2 ] = 
	{
		{1, 0}, {1, 2}, {1, 2}
	};
	for( long i = 0; i < text.length(); i++ )
	{
		ost = st;
		if( text.at(i).isSpace() )
			st = tbl[ st ][ 1 ];
		else
			st = tbl[ st ][ 0 ];
		if( st == 1 )
		{
			if( ost == 2 )
				ret.append( QChar(' ') );
			ret.append( text.at(i) );
		}
	}
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

void AST_TXML_Node::recursiveProcessing( void *data, ASTNODE_PROC func, int /*deep*/ )
{
	func( this, data );
}

void AST_TXML_Compound::recursiveProcessing( void *data, ASTNODE_PROC func, int deep )
{
	if( !deep )
		func( this, data );
	for( long i = 0; i < getChildNumber(); i++ )
	{
		if( getChild( i ) )
			getChild( i )->recursiveProcessing( data, func, deep );
	}
	if( deep )
		func( this, data );
}

void AST_TXML_Attribute::recursiveProcessing( void *data, ASTNODE_PROC func, int /*deep*/ )
{
	func( this, data );
}

void AST_TXML_Value::recursiveProcessing( void *data, ASTNODE_PROC func, int /*deep*/ )
{
	func( this, data );
}

void AST_TXML_Tag::recursiveProcessing( void *data, ASTNODE_PROC func, int deep )
{
	if( !deep )
		func( this, data );
	if( getAttributes() )
		getAttributes()->recursiveProcessing( data, func, deep );
	if( getTags() )
		getTags()->recursiveProcessing( data, func, deep );
	if( deep )
		func( this, data );
}

void AST_TXML_Tag::getAllTags2Vector( std::vector<AST_TXML_Tag*>& lambdaCollection )
{
	lambdaCollection.clear();
	if( getTags() )
	{
		for( long i = 0; i < getTags()->getChildNumber(); i++ )
		{
			AST_TXML_Node *_xml_child = getTags()->getChild( i );
			if( _xml_child && _xml_child->getNodeType() == ASTT_TXML_Tag )
				lambdaCollection.push_back( (AST_TXML_Tag*) _xml_child );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

void AST_TXML_Node::recursiveProcessing_ParentPtr( AST_TXML_Tag * /*parentTag*/, void * /*data*/, ASTNODE_WITHPARENT_PROC /*func*/, int /*deep*/ )
{}

void AST_TXML_Compound::recursiveProcessing_ParentPtr( AST_TXML_Tag *parentTag, void *data, ASTNODE_WITHPARENT_PROC func, int deep )
{
	for( long i = 0; i < getChildNumber(); i++ )
	{
		if( getChild( i ) && getChild( i )->getNodeType() == ASTT_TXML_Tag )
			getChild( i )->recursiveProcessing_ParentPtr( parentTag, data, func, deep );
	}
}

void AST_TXML_Tag::recursiveProcessing_ParentPtr( AST_TXML_Tag *parentTag, void *data, ASTNODE_WITHPARENT_PROC func, int deep )
{
	if( !deep )
		func( this, data, parentTag );
	if( getTags() )
		getTags()->recursiveProcessing_ParentPtr( this, data, func, deep );
	if( deep )
		func( this, data, parentTag );
}

///////////////////////////////////////////////////////////////////////////////
