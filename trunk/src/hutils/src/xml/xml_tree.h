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

#if !defined( __COMMON_XML_TREE_H__ )
#define __COMMON_XML_TREE_H__

///////////////////////////////////////////////////////////////////////////////

#include "../basexxx.h"

#include "HUtils/ihkarray.h"
#include "HUtils/ihkflag.h"
#include "HUtils/ihkmap.h"
#include "HUtils/iustring.h"

enum AST_TXML_ValueTypes
{
	ASTVALT_TXML_None,
	ASTVALT_TXML_Int,
	ASTVALT_TXML_Real,
	ASTVALT_TXML_String
};

enum AST_TXML_Types
{
	ASTT_TXML_None,
	ASTT_TXML_Attribute,
	ASTT_TXML_Value,
	ASTT_TXML_Tag,
	ASTT_TXML_Compound,
	ASTT_TXML_Last
};

#define FL_XML_PRINT_TAG_ONE_LINE				0x00000001
#define FL_XML_PRINT_TAG_SINGLE_SPACING			0x00000002
#define FL_XML_SELF_CLOSE_TAG					0x00000004
#define FL_XML_PROCESSING_INSTRUCTION			0x00000008

class AST_TXML_Node;
class AST_TXML_Tag;
class AST_TXML_Compound;

typedef void (*ASTNODE_PROC)( AST_TXML_Node *node, void *data );
typedef void (*ASTNODE_WITHPARENT_PROC)( AST_TXML_Tag *node, void *data, AST_TXML_Tag *parentNode );

///////////////////////////////////////////////////////////////////////////////

class HUTILS_EXPORT AST_TXML_Node
{
protected:
	enum AST_TXML_Types node_type;
public:
	long line;
	long column;
	long level;
	long shift1, shift2;
	HKLongFlag mode;

	AST_TXML_Node( void )
	{
		node_type = ASTT_TXML_None;
		line = -1;
		column = -1;
		level = -1;
		shift1 = -1;
		shift2 = -1;
		mode = HKLongFlag( 0 );
	}
	AST_TXML_Node( const AST_TXML_Node& a )
	{
		node_type = a.node_type;
		line = a.line;
		column = a.column;
		level = a.level;
		shift1 = a.shift1;
		shift2 = a.shift2;
		mode = HKLongFlag( 0 );
	}
	virtual AST_TXML_Node* Clone( void ) const
	{
		return new AST_TXML_Node( *this );
	}
	enum AST_TXML_Types getNodeType( void )
	{
		return node_type;
	}
	int is_preceded_by( AST_TXML_Node *arg ) const
	{
		if( ( getLine() > arg->getLine() ) || 
			( getLine() == arg->getLine() && getColumn() > arg->getColumn() ) )
			return 1;
		return 0;
	}
	void setLine( long ln )
	{
		line = ln;
	}
	long getLine( void ) const
	{
		return line;
	}
	void setColumn( long cl )
	{
		column = cl;
	}
	long getColumn( void ) const
	{
		return column;
	}
	void setLevel( long ln )
	{
		level = ln;
	}
	long getLevel( void ) const
	{
		return level;
	}
	long getShift1( void ) const
	{
		return shift1;
	}
	void setShift1( long ln )
	{
		shift1 = ln;
	}
	long getShift2( void ) const
	{
		return shift2;
	}
	void setShift2( long ln )
	{
		shift2 = ln;
	}
	virtual void formatBlank( void )// influences only text-Value -> filter blanks
	{}
	virtual ~AST_TXML_Node( void )
	{}
	virtual QString getDeepChildValueText( void )
	{
		return QString( _T("") );
	}
	virtual void setLevelThrouhTree( long ln = 0 )
	{
		level = ln;
	}
        virtual void getXML( QString& /*str*/, HKLongFlag /*mode*/ = HKLongFlag( 0 ) )
	{}
        virtual AST_TXML_Tag* getFirstTag( const QString& /*name*/ )
	{
		return 0;
	}
        virtual void getAllTags( const QString& /*name*/, std::vector<AST_TXML_Tag*>& /*tags*/, long /*level*/ = -1 )
	{
	}
	virtual void recursiveProcessing( void *data, ASTNODE_PROC func, int deep = 1 );
	virtual void recursiveProcessing_ParentPtr( AST_TXML_Tag *parentTag, void *data, ASTNODE_WITHPARENT_PROC func, int deep = 1 );
};

class AST_TXML_Attribute;
class HUTILS_EXPORT AST_TXML_Compound : public AST_TXML_Node
{
public:
	MIArray<AST_TXML_Node> stmt;

	AST_TXML_Compound( void )
		: AST_TXML_Node()
		, stmt(MIArray<AST_TXML_Node>::LeaveItems, 8, 0, 8)
	{
		node_type = ASTT_TXML_Compound;
	}
	AST_TXML_Compound( const AST_TXML_Compound& a );
	void Prepend( std::vector<AST_TXML_Tag*>& nodeList, int isMove = 1 );
	void Append( const AST_TXML_Compound& a );
	void AppendAttribute( const AST_TXML_Compound& a );
	virtual AST_TXML_Node* Clone( void ) const
	{
		return new AST_TXML_Compound( *this );
	}
	void InsertChild( long i, AST_TXML_Node *new_node )
	{
		if( i >= 0 && i <= (long) stmt.ArrayDim() )
			stmt.insert( stmt.begin() + i, new_node );
	}
	void MoveChildren( long i, AST_TXML_Compound *new_node )
	{
		if( new_node && i >= 0 && i <= (long) stmt.ArrayDim() )
		{
			stmt.insert( stmt.begin() + i, new_node->stmt.begin(), new_node->stmt.end() );
			new_node->stmt.clear();
		}
	}
	void ReplaceChild( long i, AST_TXML_Node *new_node )
	{
		if( i >= 0 && i < (long) stmt.ArrayDim() )
			stmt[ i ] = new_node;
	}
	void EraseAndReplaceChild( long i, AST_TXML_Node *new_node )
	{
		if( i >= 0 && i < (long) stmt.ArrayDim() )
		{
			if( stmt[ i ] )
			{
				delete stmt[ i ];
				stmt[ i ] = 0;
			}
			stmt[ i ] = new_node;
		}
	}
	void detachChildren( void );
	int Add( AST_TXML_Node *node )
	{
		return stmt.Add( node );
	}
	void Flush( void )
	{
		stmt.Flush();
	}
	virtual void formatBlank( void );
	virtual ~AST_TXML_Compound( void );
	virtual void getXML( QString& str, HKLongFlag mode = HKLongFlag( 0 ) );
	void Detach( long i )
	{
		if( i >= 0 && i < (long) stmt.ArrayDim() )
		{
			stmt[ i ] = 0;
			stmt.erase(stmt.begin() + i);
		}
	}
	void Erase( long i )
	{
		if( i >= 0 && i < (long) stmt.ArrayDim() )
		{
			if( stmt[ i ] )
			{
				delete stmt[ i ];
				stmt[ i ] = 0;
			}
			stmt.erase(stmt.begin() + i);
		}
	}
	long getChildNumber( void ) const
	{
		return stmt.ArrayDim();
	}
	AST_TXML_Node* getChild( long idx )
	{
		if( idx >= 0 && idx < (long) stmt.ArrayDim() )
			return stmt[ idx ];
		return 0;
	}
	AST_TXML_Tag* getChildByName( const QString& name );
	void setAttrMap( HKStringMap& tomap );
	AST_TXML_Attribute* getFirstAttribute( const QString& name );
	virtual void setLevelThrouhTree( long ln = 0 );
	virtual AST_TXML_Tag* getFirstTag( const QString& name );
	virtual void getAllTags( const QString& name, std::vector<AST_TXML_Tag*>& tags, long level = -1 );
	virtual QString getDeepChildValueText( void );
	virtual void recursiveProcessing( void *data, ASTNODE_PROC func, int deep = 1 );
	virtual void recursiveProcessing_ParentPtr( AST_TXML_Tag *parentTag, void *data, ASTNODE_WITHPARENT_PROC func, int deep = 1 );
};

class HUTILS_EXPORT AST_TXML_Attribute : public AST_TXML_Node
{
public:
	QString name;
	enum AST_TXML_ValueTypes type;
	long	larg;
	double	farg;
	QString	sarg;

	AST_TXML_Attribute( enum AST_TXML_ValueTypes _type = ASTVALT_TXML_None )
	{
		node_type = ASTT_TXML_Attribute;
		type = _type;
		name = _T("");
	}
	AST_TXML_Attribute( QString* _name, long *l )
	{
		node_type = ASTT_TXML_Attribute;
		type = ASTVALT_TXML_Int;
		name = *_name;
		larg = *l;
	}
	AST_TXML_Attribute( QString* _name, double *l )
	{
		node_type = ASTT_TXML_Attribute;
		type = ASTVALT_TXML_Real;
		name = *_name;
		farg = *l;
	}
	AST_TXML_Attribute( QString* _name, QString *l )
	{
		node_type = ASTT_TXML_Attribute;
		type = ASTVALT_TXML_String;
		name = *_name;
		sarg = *l;
	}
	AST_TXML_Attribute( const AST_TXML_Attribute& a ) :
		AST_TXML_Node( a )
	{
		name = a.name;
		type = a.type;
		larg = a.larg;
		farg = a.farg;
		sarg = a.sarg;
	}
	virtual AST_TXML_Node* Clone( void ) const
	{
		return new AST_TXML_Attribute( *this );
	}
	virtual ~AST_TXML_Attribute( void )
	{}
	int convert2Float( void );
	int convert2Integer( void );
	virtual void getXML( QString& str, HKLongFlag mode = HKLongFlag( 0 ) );
	enum AST_TXML_ValueTypes getValueType( void )
	{
		return type;
	}
	QString& getAttributeName( void )
	{
		return name;
	}
	int isAttribute( const QString& n )
	{
		return !::xml_strcmp( name, n );
	}
	long getIntValue( void )
	{
		return larg;
	}
	double getFloatValue( void )
	{
		return farg;
	}
	double getNumericValue( void );
	QString& getStringValue( void );
	void setStringValue( QString str )
	{
		sarg = str;
	}
	void setAttrMap( HKStringMap& tomap )
	{
		tomap[ name ] = getStringValue();
	}
	virtual void recursiveProcessing( void *data, ASTNODE_PROC func, int deep = 1 );
};

class HUTILS_EXPORT AST_TXML_Value : public AST_TXML_Node
{
public:
	QString text;

	AST_TXML_Value( QString *_text )
	{
		node_type = ASTT_TXML_Value;
		text = *_text;
	}
	AST_TXML_Value( const AST_TXML_Value& a ) :
		AST_TXML_Node( a )
	{
		text = a.text;
	}
	virtual AST_TXML_Node* Clone( void ) const
	{
		return new AST_TXML_Value( *this );
	}
	virtual ~AST_TXML_Value( void )
	{}
	virtual void formatBlank( void );
        virtual void getXML( QString& str, HKLongFlag /*mode*/ = HKLongFlag( 0 ) )
	{
		str += text;
	}
	QString& getText( void )
	{
		return text;
	}
	void setText( QString& src )
	{
		text = src;
	}
	virtual QString getDeepChildValueText( void )
	{
		return getText();
	}
	virtual void recursiveProcessing( void *data, ASTNODE_PROC func, int deep = 1 );
};

class HUTILS_EXPORT AST_TXML_Tag : public AST_TXML_Node
{
protected:
	AST_TXML_Compound *attr, *children;
	QString ns;		// namespace
	QString name;
	QString text;
	HKLongFlag flags;
	int existTagChild;
public:
	AST_TXML_Tag( QString *_name, AST_TXML_Compound *_attr = 0, AST_TXML_Tag* _tag = 0, HKLongFlag _mode = HKLongFlag( 0 ) );
	AST_TXML_Tag( QString *_name, AST_TXML_Compound *_attr = 0, AST_TXML_Compound *_tags = 0, HKLongFlag _mode = HKLongFlag( 0 ) );
	AST_TXML_Tag( QString *_name, AST_TXML_Compound *_attr = 0, AST_TXML_Value *_value = 0, HKLongFlag _mode = HKLongFlag( 0 ) );
	AST_TXML_Tag( const AST_TXML_Tag& a ) :
		AST_TXML_Node( a )
	{
		attr = a.attr ? (AST_TXML_Compound*)a.attr->Clone() : 0;
		children = a.children ? (AST_TXML_Compound*)a.children->Clone() : 0;
		ns = a.ns;
		name = a.name;
		text = a.text;
		flags = a.flags;
		existTagChild = a.existTagChild;
	}
	long getChildNumber()
	{
		return children ? children->getChildNumber() : 0;
	}
	HKLongFlag& getFlags( void )
	{
		return flags;
	}
	virtual AST_TXML_Node* Clone( void ) const
	{
		return new AST_TXML_Tag( *this );
	}
	QString regetSumText( void );
	const QString& getText( void )
	{
		return text;
	}
	void setAttributes( AST_TXML_Compound* _attr )
	{
		attr = _attr;
	}
	AST_TXML_Compound *getAttributes( void )
	{
		return attr;
	}
	void setTags( AST_TXML_Compound* _children )
	{
		children = _children;
		text = regetSumText();
	}
	AST_TXML_Compound* getTags( void )
	{
		return children;
	}
	QString& getNS( void )
	{
		return ns;
	}
	QString& getTagName( void )
	{
		return name;
	}
	void setTagName( QString nm );
	int isAttr( void )
	{
		return attr != 0;
	}
	int isValue( void )
	{
		return text != _T("");
	}
	void deleteChild( long idx );
	AST_TXML_Value* getValueChild( long idx )
	{
		if( children && children->getChild( idx ) && 
			children->getChild( idx )->getNodeType() == ASTT_TXML_Value )
			return (AST_TXML_Value*)(children->getChild( idx ));
		return 0;
	}
	AST_TXML_Tag* getTagChild( long idx )
	{
		if( children && children->getChild( idx ) && 
			children->getChild( idx )->getNodeType() == ASTT_TXML_Tag )
			return (AST_TXML_Tag*)(children->getChild( idx ));
		return 0;
	}
	AST_TXML_Tag* getTagChildByName( const QString& name )
	{
		return children ? children->getChildByName( name ) : 0;
	}
	int isExistTagChild( void )
	{
		return existTagChild;
	}
	void putValue2Tag( long index, QString tagname, AST_TXML_Compound *_attr = 0 );
	void putValue2Tag( QString tagname, AST_TXML_Compound *_attr = 0 );
	void deleteValueChildren( void );
	void deleteTagChildren( void );
	virtual void formatBlank( void );
	virtual ~AST_TXML_Tag( void )
	{
		if( attr ) delete attr;
		if( children ) delete children;
	}
        virtual QString getDeepChildValueText( void )
	{
		return getTags() ? getTags()->getDeepChildValueText() : QString( _T("") );
	}
	virtual void getXML( QString& str, HKLongFlag mode = HKLongFlag( 0 ) );
	virtual void setLevelThrouhTree( long ln = 0 )
	{
		level = ln;
		if( attr ) attr->setLevelThrouhTree( ln );
		if( children ) children->setLevelThrouhTree( ln + 1 );
	}
	AST_TXML_Attribute *getAttribute( const QString& attr_name );
	void setAttrMap( HKStringMap& tomap )
	{
		if( attr ) attr->setAttrMap( tomap );
	}
	virtual AST_TXML_Tag* getFirstTag( const QString& tag_name )
	{
		if( !::xml_strcmp( name, tag_name ) ) return this;
		else if( getTags() ) return getTags()->getFirstTag( tag_name );
		return 0;
	}
	virtual void getAllTags( const QString& tag_name, std::vector<AST_TXML_Tag*>& arrtags, long level = -1 )
	{
		if( !::xml_strcmp( name, tag_name ) )
			arrtags.push_back( this );
		if( getTags() ) getTags()->getAllTags( tag_name, arrtags, level );
	}
	void getAllTags2Vector( std::vector<AST_TXML_Tag*>& lambdaCollection );
	virtual void recursiveProcessing( void *data, ASTNODE_PROC func, int deep = 1 );
	virtual void recursiveProcessing_ParentPtr( AST_TXML_Tag *parentTag, void *data, ASTNODE_WITHPARENT_PROC func, int deep = 1 );
};

#define TXML_Storage AST_TXML_Compound

HUTILS_EXPORT 
QString formatBlankAsXML( const QString& text );

#endif











