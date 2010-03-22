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

#include "../../dictionary/op_map.h"
#include "../../dictionary/entity_map.h"
#include "../node.h"

///////////////////////////////////////////////////////////////////////////////

int isEqual_EntityName( QString a, QString b )
{
	unsigned short an = ::mapString2UnicodeValue( a );
	unsigned short bn = ::mapString2UnicodeValue( b );
	return an == bn;
}

///////////////////////////////////////////////////////////////////////////////

CFormulaNode* CLineNode::InsertMMLFormulaNode_MUNDER( AST_TXML_Tag *node )
{
/*@ralslot              @lalslot                        @dalslot
  <munder>              <munder>                        <munder>         
   <mo>&xrarr;</mo>      <mo>&xlarr;</mo>                <mo>&harr;</mo> 
   <mrow></mrow>         <mrow></mrow>                   <mrow></mrow>   
  </munder>             </munder>                       </munder>        


  @ddalslot             @ddlsalslot                      @ddslalslot
  <munder>              <munder>                         <munder>                    
   <mo>&rlarr;</mo>      <munder accentunder='true'>      <munder accentunder='true'>
   <mrow></mrow>          <mo>&xrarr;</mo>                 <mo>&rarr;</mo>           
  </munder>               <mo>&larr;</mo>                  <mo>&xlarr;</mo>          
                         </munder>                        </munder>                  
                         <mrow></mrow>                    <mrow></mrow>              
                        </munder>                        </munder>                   


  @hddalslot            @hddlsalslot                     
  <munder>              <munder>                         
   <mo>&rlhar;</mo>      <munder accentunder='true'>     
   <mrow></mrow>          <mo>&rharu;</mo>               
  </munder>               <mo>&lhard;</mo>                
                         </munder>                       
                         <mrow></mrow>                   
                        </munder>                        
*/
	CFormulaNode *new_node = 0;
	int ret = -1;
	// special cases
	AST_TXML_Compound *_tags, *_utags;
	AST_TXML_Tag *_mo = 0, *_mo1 = 0, *_mo2 = 0;
	if( (_tags = node->getTags()) != 0 && 
		_tags->getChildNumber() == 2 &&
		_tags->getChild( 0 )->getNodeType() == ASTT_TXML_Tag &&
		_tags->getChild( 1 )->getNodeType() == ASTT_TXML_Tag )
	{
		QString button_id = _T("");
		AST_TXML_Tag *_arg1 = 0, *_arg2 = 0;

		_mo = (AST_TXML_Tag*)(_tags->getChild( 0 ));
		_arg1 = (AST_TXML_Tag*)(_tags->getChild( 1 ));

		if( /*::mml_is_token_op_tag( _mo->getTagName() ) && */
			_mo->getText().length() && _mo->getText()[ 0 ] == _T('&') )
		{
			if( ::isEqual_EntityName( _mo->getText(), _T("&xrarr;") ) ||
				::isEqual_EntityName( _mo->getText(), _T("&rarr;") ) )
				button_id = _T("@ralslot");
			else if( ::isEqual_EntityName( _mo->getText(), _T("&xlarr;") ) ||
					 ::isEqual_EntityName( _mo->getText(), _T("&larr;") ) )
				button_id = _T("@lalslot");
			else if( ::isEqual_EntityName( _mo->getText(), _T("&harr;") ) )
				button_id = _T("@dalslot");
			else if( ::isEqual_EntityName( _mo->getText(), _T("&rlarr;") ) )
				button_id = _T("@ddalslot");
			else if( ::isEqual_EntityName( _mo->getText(), _T("&rlhar;") ) )
				button_id = _T("@hddalslot");
		}
		else if( ::mml_is_under_tag( _mo->getTagName() ) &&
				 (_utags = _mo->getTags()) != 0 && 
				 _utags->getChildNumber() == 2 &&
				 _utags->getChild( 0 )->getNodeType() == ASTT_TXML_Tag &&
				 _utags->getChild( 1 )->getNodeType() == ASTT_TXML_Tag )
		{
			_mo1 = (AST_TXML_Tag*)(_utags->getChild( 0 ));
			_mo2 = (AST_TXML_Tag*)(_utags->getChild( 1 ));
			if( /*::mml_is_token_op_tag( _mo1->getTagName() ) &&
				::mml_is_token_op_tag( _mo2->getTagName() ) &&*/
				_mo1->getText().length() && _mo1->getText()[ 0 ] == _T('&') &&
				_mo2->getText().length() && _mo2->getText()[ 0 ] == _T('&') )
			{
				if( ::isEqual_EntityName( _mo1->getText(), _T("&xrarr;") )
					&& ::isEqual_EntityName( _mo2->getText(), _T("&larr;") ) )
					button_id = _T("@ddlsalslot");
				else if( ::isEqual_EntityName( _mo1->getText(), _T("&rarr;") )
					&& ::isEqual_EntityName( _mo2->getText(), _T("&xlarr;") ) )
					button_id = _T("@ddslalslot");
				else if( ::isEqual_EntityName( _mo1->getText(), _T("&rharu;") ) && 
						 ::isEqual_EntityName( _mo2->getText(), _T("&lhard;") ) )
					button_id = _T("@hddlsalslot");
			}
		}

		// templates from uobar.xml
		if( !button_id.length() )
		{
/*
<munder>
  <mrow></mrow>
  <mo stretchy="true">&tilde; | &circ; | &frown; (@tilde @hat @frown)</mo>
</mover>
<munder>
  <mrow></mrow>
  <mo>&macr;   &rarr;     &larr;     &rharu;    &harr;</mo>
      @overbar @raoverbar @laoverbar @rhoverbar @daoverbar
</mover>

@doverbar
<munder>
  <munder>
    <mrow></mrow>
    <mo>&macr;</mo>
  </mover>
  <mo>&macr;</mo>
</mover>
*/
			if( /*::mml_is_token_op_tag( _arg1->getTagName() ) && */_arg1->getText().length() )
			{
				struct HMathMLAttr mml_attr( /*_arg1->getTagName()*/QString(FBL_TAG_mo), _arg1->getText() );
				HMathOpAttr *opinfo = ::getOperatorEntryByMML( _arg1->getText() );
				if( opinfo ) mml_attr.setMODefault( *opinfo );
				mml_attr.set( ::getCurrentFormulatorStyle().getUnit2PX(), _arg1, _arg1->getText() );

				int isaccent = 0;
				AST_TXML_Attribute *accent_attr;
				accent_attr = node->getAttribute( FBL_ATTR_accent );
				if( accent_attr )
					isaccent = !xml_strcmp( accent_attr->getStringValue(), FBL_VAL_true );

				if( mml_attr.isMOStretchy() || mml_attr.isMOAccent() || isaccent )
				{
					if( ::isEqual_EntityName( _arg1->getText(), _T("&macr;") ) || 
						::isEqual_EntityName( _arg1->getText(), _T("&UnderBar;") ) )
					{
						if( ::mml_is_under_tag( _mo->getTagName() ) &&
							(_utags = _mo->getTags()) != 0 &&
							 _utags->getChildNumber() == 2 &&
							_utags->getChild( 0 )->getNodeType() == ASTT_TXML_Tag &&
							(_mo1 = (AST_TXML_Tag*)(_utags->getChild( 0 ))) &&
							_utags->getChild( 1 )->getNodeType() == ASTT_TXML_Tag &&
							(_mo2 = (AST_TXML_Tag*)(_utags->getChild( 1 ))) &&
							/*::mml_is_token_op_tag( _mo2->getTagName() ) &&*/
							_mo2->getText().length() && 
							(::isEqual_EntityName( _mo2->getText(), _T("&macr;") ) || 
							 ::isEqual_EntityName( _mo2->getText(), _T("&UnderBar;") )) )
						{
							button_id = _T("@dunderbar");
							_arg1 = _mo1;
						}
						else
						{
							button_id = _T("@underbar");
							_arg1 = _mo;
						}
					}
					else if( ::isEqual_EntityName( _arg1->getText(), _T("&rarr;") ) )
					{
						button_id = _T("@raunderbar");
						_arg1 = _mo;
					}        
					else if( ::isEqual_EntityName( _arg1->getText(), _T("&UnderBrace;") ) ||
							 !xml_strcmp( _arg1->getText(), _T("&bcub;") ) )
					{
						button_id = _T("@underbrace");
						_arg1 = _mo;
					}        
					else if( ::isEqual_EntityName( _arg1->getText(), _T("&larr;") ) )
					{
						button_id = _T("@launderbar");
						_arg1 = _mo;
					}
					else if( ::isEqual_EntityName( _arg1->getText(), _T("&rharu;") ) )
					{
						button_id = _T("@rhunderbar");
						_arg1 = _mo;
					}        
					else if( ::isEqual_EntityName( _arg1->getText(), _T("&harr;") ) )
					{
						button_id = _T("@daunderbar");
						_arg1 = _mo;
					}
				}
			}
		}

		if( button_id.length() && _arg1 )
		{
			AST_TXML_Compound *_a = 0;
			AST_TXML_Compound *_t = new AST_TXML_Compound();
			AST_TXML_Tag *_tmp_node;
			if( _arg1 )
				_t->Add( _arg1 );
			if( _arg2 )
				_t->Add( _arg2 );
			QString empty_name = _T("");
			_tmp_node = new AST_TXML_Tag( &empty_name, _a, _t );
			new_node = InsertMMLFormulaNode_OTHERS_byID( button_id, _tmp_node );
			if( new_node )
			{
				ret = 0;
				new_node->readUnfilteredAttr( node );
			}
			for( long i = 0; i < _t->getChildNumber(); i++ )
				_t->ReplaceChild( i, 0 );
			delete _tmp_node;
		}
	}
	if( ret == -1 )
	{
		new_node = InsertMMLFormulaNode_OTHERS_byMML( node );
		if( new_node )
		{
			new_node->readUnfilteredAttr( node );
		}
	}
	return new_node;
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

CFormulaNode* CLineNode::InsertMMLFormulaNode_MOVER( AST_TXML_Tag *node )
{
/*
 @rauslot           @lauslot            @dauslot          
 <mover>            <mover>             <mover>           
  <mo>&xrarr;</mo>   <mo>&xlarr;</mo>    <mo>&harr;</mo>  
  <mrow></mrow>      <mrow></mrow>       <mrow></mrow>    
 </mover>           </mover>            </mover>          

 @ddauslot           @hddauslot
 <mover>             <mover>           
  <mo>&rlarr;</mo>    <mo>&rlharr;</mo>
  <mrow></mrow>       <mrow></mrow>    
 </mover>            </mover>                            

 @ddlsauslot                  @ddslauslot                  @hddslauslot
 <mover>                      <mover>                      <mover>                
  <munder accentunder='true'>  <munder accentunder='true'>   <mover accent='true'>
   <mo>&xrarr;</mo>             <mo>&rarr;</mo>                <mo>&lhard;</mo>   
   <mo>&larr;</mo>              <mo>&xlarr;</mo>               <mo>&rharu;</mo>   
  </munder>                    </munder>                     </mover>             
  <mrow></mrow>                <mrow></mrow>                 <mrow></mrow>        
 </mover>                     </mover>                     </mover>               
*/
	CFormulaNode *new_node = 0;
	int ret = -1;
	// special cases
	AST_TXML_Compound *_tags, *_utags;
	AST_TXML_Tag *_mo = 0, *_mo1 = 0, *_mo2 = 0;
	if( (_tags = node->getTags()) != 0 && 
		_tags->getChildNumber() == 2 &&
		_tags->getChild( 0 )->getNodeType() == ASTT_TXML_Tag &&
		_tags->getChild( 1 )->getNodeType() == ASTT_TXML_Tag )
	{
		QString button_id = _T("");
		AST_TXML_Tag *_arg1 = 0, *_arg2 = 0;

		_mo = (AST_TXML_Tag*)(_tags->getChild( 0 ));
		_arg1 = (AST_TXML_Tag*)(_tags->getChild( 1 ));


		if( /*::mml_is_token_op_tag( _mo->getTagName() ) && */
			_mo->getText().length() && _mo->getText()[ 0 ] == _T('&') )
		{
			if( ::isEqual_EntityName( _mo->getText(), _T("&xrarr;") ) ||
				::isEqual_EntityName( _mo->getText(), _T("&rarr;") ) )
				button_id = _T("@rauslot");
			else if( ::isEqual_EntityName( _mo->getText(), _T("&xlarr;") ) ||
					 ::isEqual_EntityName( _mo->getText(), _T("&larr;") ) )
				button_id = _T("@lauslot");
			else if( ::isEqual_EntityName( _mo->getText(), _T("&harr;") ) )
				button_id = _T("@dauslot");
			else if( ::isEqual_EntityName( _mo->getText(), _T("&rlarr;") ) )
				button_id = _T("@ddauslot");
			else if( ::isEqual_EntityName( _mo->getText(), _T("&rlhar;") ) )
						button_id = _T("@hddauslot");
		}
		else if( (_utags = _mo->getTags()) != 0 && 
				 _utags->getChildNumber() == 2 &&
				 _utags->getChild( 0 )->getNodeType() == ASTT_TXML_Tag &&
				 _utags->getChild( 1 )->getNodeType() == ASTT_TXML_Tag )
		{
			_mo1 = (AST_TXML_Tag*)(_utags->getChild( 0 ));
			_mo2 = (AST_TXML_Tag*)(_utags->getChild( 1 ));
			if( /*::mml_is_token_op_tag( _mo1->getTagName() ) &&*/
				/*::mml_is_token_op_tag( _mo2->getTagName() ) &&*/
				_mo1->getText().length() && _mo1->getText()[ 0 ] == _T('&') &&
				_mo2->getText().length() && _mo2->getText()[ 0 ] == _T('&') )
			{
				if( ::mml_is_under_tag( _mo->getTagName() ) )
				{
				if( ::isEqual_EntityName( _mo1->getText(), _T("&xrarr;") )
					&& ::isEqual_EntityName( _mo2->getText(), _T("&larr;") ) )
						button_id = _T("@ddlsauslot");
				else if( ::isEqual_EntityName( _mo1->getText(), _T("&rarr;") )
					&& ::isEqual_EntityName( _mo2->getText(), _T("&xlarr;") ) )
						button_id = _T("@ddslauslot");
				}
				else if( ::mml_is_over_tag( _mo->getTagName() ) )
				{
					if( ::isEqual_EntityName( _mo1->getText(), _T("&lhard;") ) && 
						::isEqual_EntityName( _mo2->getText(), _T("&rharu;") ) )
						button_id = _T("@hddslauslot");
				}
			}
		}

		// templates from uobar.xml
		if( !button_id.length() )
		{
/*
<mover>
  <mrow></mrow>
  <mo stretchy="true">&tilde; | &circ; | &frown; (@tilde @hat @frown)</mo>
</mover>
<mover>
  <mrow></mrow>
  <mo>&macr;   &rarr;     &larr;     &rharu;    &harr;</mo>
      @overbar @raoverbar @laoverbar @rhoverbar @daoverbar
</mover>

@doverbar
<mover>
  <mover>
    <mrow></mrow>
    <mo>&macr;</mo>
  </mover>
  <mo>&macr;</mo>
</mover>
*/
			if( /*::mml_is_token_op_tag( _arg1->getTagName() ) && */_arg1->getText().length() )
			{
				struct HMathMLAttr mml_attr( /*_arg1->getTagName()*/QString(FBL_TAG_mo), _arg1->getText() );
				HMathOpAttr *opinfo = ::getOperatorEntryByMML( _arg1->getText() );
				if( opinfo ) mml_attr.setMODefault( *opinfo );
				mml_attr.set( ::getCurrentFormulatorStyle().getUnit2PX(), _arg1, _arg1->getText() );

				int isaccent = 0;
				AST_TXML_Attribute *accent_attr;
				accent_attr = node->getAttribute( FBL_ATTR_accent );
				if( accent_attr )
					isaccent = !xml_strcmp( accent_attr->getStringValue(), FBL_VAL_true );

				if( mml_attr.isMOStretchy() || mml_attr.isMOAccent() || isaccent )
				{
 					if( ::isEqual_EntityName( _arg1->getText(), _T("&tilde;") ) || 
						::isEqual_EntityName( _arg1->getText(), _T("&sim;") ) || 
						::isEqual_EntityName( _arg1->getText(), _T("~") ) )
					{
						button_id = _T("@tilde");
						_arg1 = _mo;
					}
					else if( ::isEqual_EntityName( _arg1->getText(), _T("&circ;") ) || 
							 ::isEqual_EntityName( _arg1->getText(), _T("&Hat;") ) || 
							 ::isEqual_EntityName( _arg1->getText(), _T("^") ) )
					{
						button_id = _T("@hat");
						_arg1 = _mo;
					}
					else if( ::isEqual_EntityName( _arg1->getText(), _T("&frown;") ) )
					{
						button_id = _T("@frown");
						_arg1 = _mo;
					}
					else if( ::isEqual_EntityName( _arg1->getText(), _T("&macr;") ) ||
							 ::isEqual_EntityName( _arg1->getText(), _T("&UnderBar;") ) )
					{
						if( ::mml_is_over_tag( _mo->getTagName() ) &&
							(_utags = _mo->getTags()) != 0 &&
							 _utags->getChildNumber() == 2 &&
							_utags->getChild( 0 )->getNodeType() == ASTT_TXML_Tag &&
							(_mo1 = (AST_TXML_Tag*)(_utags->getChild( 0 ))) &&
							_utags->getChild( 1 )->getNodeType() == ASTT_TXML_Tag &&
							(_mo2 = (AST_TXML_Tag*)(_utags->getChild( 1 ))) &&
							/*::mml_is_token_op_tag( _mo2->getTagName() ) &&*/
							_mo2->getText().length() && 
							(::isEqual_EntityName( _mo2->getText(), _T("&macr;") ) || 
							 ::isEqual_EntityName( _mo2->getText(), _T("&UnderBar;") ) ))
						{
							button_id = _T("@doverbar");
							_arg1 = _mo1;
						}
						else
						{
							button_id = _T("@overbar");
							_arg1 = _mo;
						}
					}
					else if( ::isEqual_EntityName( _arg1->getText(), _T("&OverBrace;") ) ||
							 !xml_strcmp( _arg1->getText(), _T("&tcub;") ) )
					{
						button_id = _T("@overbrace");
						_arg1 = _mo;
					}        
					else if( ::isEqual_EntityName( _arg1->getText(), _T("&rarr;") ) )
					{
						button_id = _T("@raoverbar");
						_arg1 = _mo;
					}
					else if( ::isEqual_EntityName( _arg1->getText(), _T("&larr;") ) )
					{
						button_id = _T("@laoverbar");
						_arg1 = _mo;
					}
					else if( ::isEqual_EntityName( _arg1->getText(), _T("&rharu;") ) )
					{
						button_id = _T("@rhoverbar");
						_arg1 = _mo;
					}
					else if( ::isEqual_EntityName( _arg1->getText(), _T("&harr;") ) )
					{
						button_id = _T("@daoverbar");
						_arg1 = _mo;
					}
				}
			}
		}

		if( button_id.length() && _arg1 )
		{
			AST_TXML_Compound *_a = 0;
			AST_TXML_Compound *_t = new AST_TXML_Compound();
			AST_TXML_Tag *_tmp_node;
			if( _arg1 )
				_t->Add( _arg1 );
			if( _arg2 )
				_t->Add( _arg2 );
			QString empty_name = _T("");
			_tmp_node = new AST_TXML_Tag( &empty_name, _a, _t );
			new_node = InsertMMLFormulaNode_OTHERS_byID( button_id, _tmp_node );
			if( new_node )
			{
				ret = 0;
				new_node->readUnfilteredAttr( node );
			}
			for( long i = 0; i < _t->getChildNumber(); i++ )
				_t->ReplaceChild( i, 0 );
			delete _tmp_node;
		}
	}
	if( ret == -1 )
	{
		new_node = InsertMMLFormulaNode_OTHERS_byMML( node );
		if( new_node )
		{
			new_node->readUnfilteredAttr( node );
		}
	}
	return new_node;
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

CFormulaNode* CLineNode::InsertMMLFormulaNode_MUNDEROVER( AST_TXML_Tag *node )
{
/*
@raulslot          @laulslot          @daulslot        @ddaulslot         
<munderover>       <munderover>       <munderover>     <munderover>       
 <mo>&xrarr;</mo>   <mo>&xlarr;</mo>   <mo>&harr;</mo>  <mo>&rlarr;</mo>  
 <mrow></mrow>      <mrow></mrow>      <mrow></mrow>    <mrow></mrow>     
 <mrow></mrow>      <mrow></mrow>      <mrow></mrow>    <mrow></mrow>     
</munderover>      </munderover>      </munderover>    </munderover>      

@hddlsauslot       @hddslalslot       @hddaulslot      
<munderover>       <munderover>       <munderover>     
  <mo>&rharu;</mo>   <mo>&lhard;</mo>  <mo>&rlhar;</mo>
  <mo>&lhard;</mo>   ///$$$ 1st slot   <mrow></mrow>   
  ///$$$ 1st slot    <mo>&rharu;</mo>  <mrow></mrow>   
</munderover>      </munder>          </munderover>    

@ddlsaulslot                     @ddslaulslot                     @hddlsaulslot                  
<munderover accentunder='true'>  <munderover accentunder='true'>  <munderover accentunder='true'>
 <mo>&xrarr;</mo>                 <mo>&rarr;</mo>                  <mo>&rharu;</mo>              
 <munder>                         <munder>                         <munder>                      
  <mo>&larr;</mo>                  <mo>&xlarr;</mo>                 <mo>&lhard;</mo>             
  <mrow></mrow>                    <mrow></mrow>                    <mrow></mrow>                
 </munder>                        </munder>                        </munder>                     
 <mrow></mrow>                    <mrow></mrow>                    <mrow></mrow>                 
</munderover>                    </munderover>                    </munderover>                  

@hddslaulslot                  
<munderover accentunder='true'>
  <mo>&lhard;</mo>             
  ///$$$ 1st slot              
  <mover>                      
    <mo>&rharu;</mo>           
    ///$$$ 2nd slot            
  </mover>                     
</munderover>                  
*/
	CFormulaNode *new_node = 0;
	int ret = -1;
	AST_TXML_Compound *_tags, *_utags;
	AST_TXML_Tag *_mo;
	if( (_tags = node->getTags()) != 0 && 
		_tags->getChildNumber() == 3 &&
		_tags->getChild( 0 )->getNodeType() == ASTT_TXML_Tag &&
		_tags->getChild( 1 )->getNodeType() == ASTT_TXML_Tag &&
		_tags->getChild( 2 )->getNodeType() == ASTT_TXML_Tag )
	{
		QString button_id = _T("");
		AST_TXML_Tag *_arg1 = 0, *_arg2 = 0;

		_mo = (AST_TXML_Tag*)(_tags->getChild( 0 ));
		_arg1 = (AST_TXML_Tag*)(_tags->getChild( 1 ));
		_arg2 = (AST_TXML_Tag*)(_tags->getChild( 2 ));

		AST_TXML_Tag *_mo1 = 0, *_mo2 = 0;
		if( /*::mml_is_token_op_tag( _mo->getTagName() ) && */
			_mo->getText().length() && _mo->getText()[ 0 ] == _T('&') )
		{
			if( ::isEqual_EntityName( _mo->getText(), _T("&xrarr;") ) )
			{
				if( ::mml_is_under_tag( _arg1->getTagName() ) &&
					(_utags = _arg1->getTags()) != 0 &&
					 _utags->getChildNumber() == 2 &&
					_utags->getChild( 0 )->getNodeType() == ASTT_TXML_Tag &&
					(_mo1 = (AST_TXML_Tag*)(_utags->getChild( 0 ))) &&
					_utags->getChild( 1 )->getNodeType() == ASTT_TXML_Tag &&
					(_mo2 = (AST_TXML_Tag*)(_utags->getChild( 1 ))) &&
					/*::mml_is_token_op_tag( _mo1->getTagName() ) &&*/
					_mo1->getText().length() && 
					::isEqual_EntityName( _mo1->getText(), _T("&larr;") ) )
				{
					button_id = _T("@ddlsaulslot");
					_arg1 = _mo2;
				}
				else
					button_id = _T("@raulslot");
			}       
			else if( ::isEqual_EntityName( _mo->getText(), _T("&xlarr;") ) )
				button_id = _T("@laulslot");
			else if( ::isEqual_EntityName( _mo->getText(), _T("&harr;") ) )
				button_id = _T("@daulslot");
			else if( ::isEqual_EntityName( _mo->getText(), _T("&rlarr;") ) )
				button_id = _T("@ddaulslot");
			else if( ::isEqual_EntityName( _mo->getText(), _T("&rlhar;") ) )
				button_id = _T("@hddaulslot");
			else if( /*::mml_is_token_op_tag( _arg1->getTagName() ) &&*/
					 ::isEqual_EntityName( _mo->getText(), _T("&rharu;") ) &&
					 ::isEqual_EntityName( _arg1->getText(), _T("&lhard;") ) )
			{
				button_id = _T("@hddlsauslot");
				_arg1 = _arg2;
				_arg2 = 0;
			}
			else if( /*::mml_is_token_op_tag( _arg2->getTagName() ) &&*/
					 ::isEqual_EntityName( _mo->getText(), _T("&lhard;") ) &&
					 ::isEqual_EntityName( _arg2->getText(), _T("&rharu;") ) )
			{
				button_id = _T("@hddslalslot");
				_arg2 = 0;
			}
			else if( ::mml_is_under_tag( _arg1->getTagName() ) &&
					 (_utags = _arg1->getTags()) != 0 &&
					 _utags->getChildNumber() == 2 &&
					 _utags->getChild( 0 )->getNodeType() == ASTT_TXML_Tag &&
					 _utags->getChild( 1 )->getNodeType() == ASTT_TXML_Tag )
			{
				AST_TXML_Tag *_mo1 = (AST_TXML_Tag*)(_utags->getChild( 0 ));
				AST_TXML_Tag *_mo2 = (AST_TXML_Tag*)(_utags->getChild( 1 ));
				if( /*::mml_is_token_op_tag( _mo1->getTagName() ) &&*/
					_mo1->getText().length() && _mo1->getText()[ 0 ] == _T('&') )
				{
					if( ::isEqual_EntityName( _mo->getText(), _T("&xrarr;") )
						&& ::isEqual_EntityName( _mo1->getText(), _T("&larr;") ) )
					{
						button_id = _T("@ddlsaulslot");
						_arg1 = _mo2;
					}
					else if( ::isEqual_EntityName( _mo->getText(), _T("&rarr;") )
						&& ::isEqual_EntityName( _mo1->getText(), _T("&xlarr;") ) )
					{
						button_id = _T("@ddslaulslot");
						_arg1 = _mo2;
					}
					else if( ::isEqual_EntityName( _mo->getText(), _T("&rharu;") ) && 
						::isEqual_EntityName( _mo1->getText(), _T("&lhard;") ) )
					{
						button_id = _T("@hddlsaulslot");
						_arg1 = _mo2;
					}
				}
			}
			else if( ::mml_is_over_tag( _arg2->getTagName() ) &&
					 (_utags = _arg2->getTags()) != 0 &&
					 _utags->getChildNumber() == 2 &&
					 _utags->getChild( 0 )->getNodeType() == ASTT_TXML_Tag &&
					 _utags->getChild( 1 )->getNodeType() == ASTT_TXML_Tag )
			{
				AST_TXML_Tag *_mo1 = (AST_TXML_Tag*)(_utags->getChild( 0 ));
				AST_TXML_Tag *_mo2 = (AST_TXML_Tag*)(_utags->getChild( 1 ));
				if( /*::mml_is_token_op_tag( _mo1->getTagName() ) &&*/
					_mo1->getText().length() && _mo1->getText()[ 0 ] == _T('&') )
				{
					if( ::isEqual_EntityName( _mo->getText(), _T("&lhard;") ) && 
						::isEqual_EntityName( _mo1->getText(), _T("&rharu;") ) )
					{
						button_id = _T("@hddslaulslot");
						_arg2 = _mo2;
					}
				}
			}
		}

		if( button_id.length() && _arg1 )
		{
			AST_TXML_Compound *_a = 0;
			AST_TXML_Compound *_t = new AST_TXML_Compound();
			AST_TXML_Tag *_tmp_node;
			if( _arg1 )
				_t->Add( _arg1 );
			if( _arg2 )
				_t->Add( _arg2 );
			QString empty_name = _T("");
			_tmp_node = new AST_TXML_Tag( &empty_name, _a, _t );
			new_node = InsertMMLFormulaNode_OTHERS_byID( button_id, _tmp_node );
			if( new_node )
			{
				ret = 0;
				new_node->readUnfilteredAttr( node );
			}
			for( long i = 0; i < _t->getChildNumber(); i++ )
				_t->ReplaceChild( i, 0 );
			delete _tmp_node;
		}
	}
	if( ret == -1 )
	{
		new_node = InsertMMLFormulaNode_OTHERS_byMML( node );
		if( new_node )
		{
			new_node->readUnfilteredAttr( node );
		}
	}
	return new_node;
}

///////////////////////////////////////////////////////////////////////////////
