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

#include "la_mmlchar.h"
#include "mml_defs.h"
#include "mml_char.h"

///////////////////////////////////////////////////////////////////////////////

QString LMML_Tags[ LMMLCharStates ] = {
	QString( _T("") ),	// none
	QString( _T("mo") ),  // ?   
	QString( _T("mo") ),  // .   
	QString( _T("mo") ),  // ,   
	QString( _T("mo") ),  // <   
	QString( _T("mo") ),  // >   
	QString( _T("mo") ),  // /   
	QString( _T("mo") ),  // "   
	QString( _T("mo") ),  // :   
	QString( _T("mo") ),  // ;   
	QString( _T("mo") ),  // [   
	QString( _T("mo") ),  // ]   
	QString( _T("mo") ),  // {   
	QString( _T("mo") ),  // }   
	QString( _T("mo") ),  // ~   
	QString( _T("mo") ),  // `   
	QString( _T("mo") ),  // !   
	QString( _T("mo") ),  // @   
	QString( _T("mo") ),  // #   
	QString( _T("mtext") ),  // $   
	QString( _T("mo") ),  // %   
	QString( _T("mo") ),  // ^   
	QString( _T("mo") ),  // &   
	QString( _T("mo") ),  // *   
	QString( _T("mo") ),  // _   
	QString( _T("mo") ),  // -   
	QString( _T("mo") ),  // +   
	QString( _T("mo") ),  // =   
	QString( _T("mo") ),  // |   
	QString( _T("mo") ),  // '\'
	QString( _T("mo") ),  // (   
	QString( _T("mo") ),  // )   
	QString( _T("mtext") ),  // tb  
	QString( _T("mtext") ),  // sp  
	QString( _T("mi") ),  // a-z 
	QString( _T("mn") ),  // 0-9 
	QString( _T("mtext") )// else
};

QString LMML_Attr[ LMMLCharStates ] = {
	QString( _T("") ),	// none
	QString( _T("") ),  // ?   
	QString( _T("") ),  // .   
	QString( _T("") ),  // ,   
	QString( _T("") ),  // <   
	QString( _T("") ),  // >   
	QString( _T("") ),  // /   
	QString( _T("") ),  // "   
	QString( _T("") ),  // :   
	QString( _T("") ),  // ;   
	QString( _T("stretchy='false'") ),  // [   
	QString( _T("stretchy='false'") ),  // ]   
	QString( _T("") ),  // {   
	QString( _T("") ),  // }   
	QString( _T("") ),  // ~   
	QString( _T("") ),  // `   
	QString( _T("") ),  // !   
	QString( _T("") ),  // @   
	QString( _T("") ),  // #   
	QString( _T("") ),  // $   
	QString( _T("") ),  // %   
	QString( _T("") ),  // ^   
	QString( _T("") ),  // &   
	QString( _T("") ),  // *   
	QString( _T("") ),  // _   
	QString( _T("") ),  // -   
	QString( _T("") ),  // +   
	QString( _T("") ),  // =   
	QString( _T("") ),  // |   
	QString( _T("") ),  // '\'
	QString( _T("stretchy='false'") ),  // (   
	QString( _T("stretchy='false'") ),  // )   
	QString( _T("") ),  // tb  
	QString( _T("") ),  // sp  
	QString( _T("") ),  // a-z 
	QString( _T("") ),  // 0-9 
	QString( _T("") )   // else
};

QString LMML_Presentation[ LMMLCharAlphabet ] = {
	QString( _T("") ),                        // ?   
	QString( _T("") ),                        // .   
	QString( _T("") ),                        // ,   
	QString( _T("&lt;") ),                    // <   
	QString( _T("&gt;") ),                    // >   
	QString( _T("&sol;") ),                   // /   
	QString( _T("&quot;") ),                  // "   
	QString( _T("") ),                        // :   
	QString( _T("") ),                        // ;   
	QString( _T("") ),                        // [   
	QString( _T("") ),                        // ]   
	QString( _T("&lcub;") ),                  // {   
	QString( _T("&rcub;") ),                  // }   
	QString( _T("&sim;") ),                   // ~   
	QString( _T("&grave;") ),                 // `   
	QString( _T("") ),                        // !   
	QString( _T("&commat;") ),                // @   
	QString( _T("&num;") ),                   // #   
	QString( _T("") ),                        // $   
	QString( _T("") ),                        // %   
	QString( _T("&circ;") ),                  // ^   
	QString( _T("&amp;") ),                   // &   
	QString( _T("&lowast;") ),                // *   
	QString( _T("") ),//QString( _T("&lowbar;") ),                // _   
	QString( _T("&minus;") ),//QString( _T("&minus;") ),                 // -   
	QString( _T("") ),                        // +   
	QString( _T("") ),                        // =   
	QString( _T("|") ),                       // |   
	QString( _T("&bsol;") ),                  // '\'   
	QString( _T("") ),                        // (   
	QString( _T("") ),                        // )   
	QString( _T("&Tab;") ),                   // tb  
	QString( _T(" ") ),/*"&nbsp;") ),*/                  // sp  
	QString( _T("") ),                        // a-z 
	QString( _T("") ) ,                       // 0-9 
	QString( _T("") )                         // else
};

QString LMML_Presentation4MTEXT[ LMMLCharAlphabet ] = {
	QString( _T("") ),                        // ?   
	QString( _T("") ),                        // .   
	QString( _T("") ),                        // ,   
	QString( _T("&lt;") ),                    // <   
	QString( _T("&gt;") ),                    // >   
	QString( _T("") ),                        // /   
	QString( _T("") ),                        // "   
	QString( _T("") ),                        // :   
	QString( _T("") ),                        // ;   
	QString( _T("") ),                        // [   
	QString( _T("") ),                        // ]   
	QString( _T("") ),                        // {   
	QString( _T("") ),                        // }   
	QString( _T("") ),                        // ~   
	QString( _T("") ),                        // `   
	QString( _T("") ),                        // !   
	QString( _T("") ),                        // @   
	QString( _T("") ),                        // #   
	QString( _T("") ),                        // $   
	QString( _T("") ),                        // %   
	QString( _T("") ),                        // ^   
	QString( _T("&amp;") ),                   // &   
	QString( _T("") ),                        // *   
	QString( _T("") ),                        // _   
	QString( _T("") ),                        // -   
	QString( _T("") ),                        // +   
	QString( _T("") ),                        // =   
	QString( _T("") ),                        // |   
	QString( _T("") ),                        // '\'   
	QString( _T("") ),                        // (   
	QString( _T("") ),                        // )   
	QString( _T("&Tab;") ),                   // tb  
	QString( _T("") ),                        // sp  
	QString( _T("") ),                        // a-z 
	QString( _T("") ) ,                       // 0-9 
	QString( _T("") )                         // else
};

long LMMLCharLeapes[ LMMLCharStates * LMMLCharAlphabet ] =
{
	//		?	.	,	<	>	/	_T("	:	;	[	]	{	}	~	`	!	@	#	$	%	^	&	*	_	-	+	=	|	'\'	(	)	tb	sp	a-z	0-9	else
	/* 1 */	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	15,	16,	17,	18,	19,	20,	21,	22,	23,	24,	25,	26,	27,	28,	29,	30,	31,	32,	33,	34,	35,	36,	37,	
	/* 2 */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// answer
	/* 3 */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// dot
	/* 4 */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// coma
	/* 5 */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// lt
	/* 6 */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// bt
	/* 7 */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// slash
	/* 8 */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// quatation
	/* 9 */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// colon
	/* 10*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// semicolon
	/* 11*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// lab
	/* 12*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// rab
	/* 13*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// lfb
	/* 14*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// rfb
	/* 15*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// tilda
	/* 16*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// grave
	/* 17*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// not
	/* 18*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// commat
	/* 19*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// num
	/* 20*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// dollar
	/* 21*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// mod
	/* 22*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// pow
	/* 23*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// and
	/* 24*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// mul
	/* 25*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// under
	/* 26*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// minus
	/* 27*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// plus
	/* 28*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// equal
	/* 29*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// verbar
	/* 30*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// back slash
	/* 31*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// lb
	/* 32*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// rb
	/* 33*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	33,	34,	0,	0,	0,	// tb
	/* 34*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	33,	34,	0,	0,	0,	// sp
	/* 35*/	0,	35,	35,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	35,	0,	0,	0,	0,	0,	0,	0,	0,	35,	0,	0,	0,	0,	0,	0,	0,	35,	35,	35,	35,	0,	// a-z
	/* 36*/	0,	36,	36,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	36,	0,	0,	0,	36,	0,	0,	0,	0,	0,	0,	0,	36,	36,	36,	36,	0,	// 0-9
	/* 37*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	37	// else
};

long LMMLCharsEndStates[ LMMLCharStates ] =
{
	0,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1
};

long LMMLChar::What( const QChar S )
//             ~~~~
{
	for(;;)
	{
		if( S == _T('\0') )
			return 0;
		switch( S.unicode() )
		{
		case _T('?'): return 1;
		case _T('.'): return 2;
		case _T(','): return 3;
		case _T('<'): return 4;
		case _T('>'): return 5;
		case _T('/'): return 6;
		case _T('"'): return 7;
		case _T(':'): return 8;
		case _T(';'): return 9;
		case _T('['): return 10;
		case _T(']'): return 11;
		case _T('{'): return 12;
		case _T('}'): return 13;
		case _T('~'):
		case 0x223C:
			return 14;	// unicode	&amp;#x223C;
		case _T('`'): return 15;
		case _T('!'): return 16;
		case _T('@'): return 17;
		case _T('#'): return 18;
		case _T('$'): return 19;
		case _T('%'): return 20;
		case _T('^'): return 21;
		case _T('&'): return 22;
		case _T('*'):
		case 0x2217:
			return 23;	// unicode	&amp;#x2217;
		case _T('_'): return 24;
		case _T('-'):
		case 0x2212:
			return 25;
		case _T('+'): return 26;
		case _T('='): return 27;
		case _T('|'): return 28;
		case _T('\\'): return 29;
		case _T('('): return 30;
		case _T(')'): return 31;
		case _T('\t'):return 32;
		case _T(' '): return 33;
		default:
			break;
		}
		if( S.isLetter() ) return 34;
		if( S.isDigit() ) return 35;
		return 36;
	}
}

void LMMLChar::Deeds( void )
//             ~~~~~
{
	QString tmp;
	long idx;
	switch( CurrentState )
	{
	case 1 :
		text = _T("");
		break;
	default :
		idx = What( character ) - 1;
		if( idx >= 0 && idx < LMMLCharAlphabet )
		{
			tmp = is_style_text ? LMML_Presentation4MTEXT[ idx ] : LMML_Presentation[ idx ];
			if( tmp.length() == 0 )
			{
				if( is_style_greek )
					text += mml_getGreek( character );
				else if( character != QChar(0) )
					text.append( character );
			}
			else
				text += tmp;
		}
		break;
	}
}

LMMLChar::LMMLChar( int _is_style_greek, int _is_style_number, int _is_style_variable, int _is_style_text )
//        ~~~~~~~~
	: LAuto( LMMLCharStates, LMMLCharAlphabet, LMMLCharLeapes, LMMLCharsEndStates )
{
	text = _T("");
	is_style_greek = _is_style_greek;
	is_style_text = _is_style_text;

	// choose an initial state depending on style of the text
	if( _is_style_number || _is_style_variable )
	{
		QChar simu = _is_style_number ? _T('1') : _T('a');
		long pos = What( simu ), resState;
		if( pos != 0 && (resState = Jmp[ (CurrentState - 1) * nAlphabet + (pos - 1) ]) != 0 )
		{
			CurrentState = resState;
		}
	}
}

QString& LMMLChar::getText( void )
{
	return text;
}

QString LMMLChar::getTag( void )
{
	return LMML_Tags[ CurrentState - 1 ];
}

QString LMMLChar::getAttr( void )
{
	return LMML_Attr[ CurrentState - 1 ];
}

///////////////////////////////////////////////////////////////////////////////
