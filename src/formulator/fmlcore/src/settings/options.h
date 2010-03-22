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

#if !defined( __FORMULATOR_OLIMP_H__ )
#define __FORMULATOR_OLIMP_H__

//#include <QString>
//#include <QDataStream>
#include <QLocale>
#include <vector>

#include "HUtils/ihkflag.h"
#include "HUtils/imessage.h"

#include "../fmlcore_global.h"
#include "../style/style.h"
#include "settings.h"

/////////////////////////////////////////////////////////////////////

struct __HERMITECH_FORMULIB__ FORMULATOR__TheOneStyle
{
	FORMULATOR__TheOneStyle();
	virtual ~FORMULATOR__TheOneStyle();

	FS_TextStyles		formula_style;
	qint32				onchar_style;
	qint32				onuser_style;
	RealFde				onuser_kegl;
	qint32				onuser_kegl_level;
	RealFde				onuser_kegl_offset;
};

/////////////////////////////////////////////////////////////////////

inline
QDataStream& operator << ( QDataStream& ar, const FORMULATOR__TheOneStyle& lp )
{
	return ar << lp.formula_style << lp.onchar_style << lp.onuser_style << lp.onuser_kegl << lp.onuser_kegl_level << lp.onuser_kegl_offset;
}

inline
QDataStream& operator >> ( QDataStream& ar, FORMULATOR__TheOneStyle& lp )
{
	return ar >> lp.formula_style >> lp.onchar_style >> lp.onuser_style >> lp.onuser_kegl >> lp.onuser_kegl_level >> lp.onuser_kegl_offset;
}

/////////////////////////////////////////////////////////////////////

struct __HERMITECH_FORMULIB__ FORMULATOR__TheOneState
{
	FORMULATOR__TheOneState();

	qint32		translator_pref;
	qint32		contentTimes;
	QLocale		currentLocale;
	HKLongFlag	m_flags;
};

/////////////////////////////////////////////////////////////////////

inline
QDataStream& operator << ( QDataStream& ar, const FORMULATOR__TheOneState& lp )
{
	return ar << lp.translator_pref << lp.contentTimes << lp.currentLocale << lp.m_flags;
}

inline
QDataStream& operator >> ( QDataStream& ar, FORMULATOR__TheOneState& lp )
{
	return ar >> lp.translator_pref >> lp.contentTimes >> lp.currentLocale >> lp.m_flags;
}

/////////////////////////////////////////////////////////////////////

typedef QMap<QString, FORMULATOR__TheOneStyle*>		mapPtr2OneStyle;
typedef QMap<QString, FORMULATOR__TheOneState*>		mapPtr2OneState;

/////////////////////////////////////////////////////////////////////
//
//
//

class FBtnToolbarSet;
class __HERMITECH_FORMULIB__ FORMULATOR__GlobalsStorage
{
protected:
	DRMessageManager			m_messageManager;
	FBtnToolbarSet*				m_toolBarSet;

	qint32						m_fStyle;
	mapPtr2OneStyle				m_styles;
	FORMULATOR__TheOneStyle*	m_pStyle;
	mapPtr2OneState				m_states;
	FORMULATOR__TheOneState*	m_pState;

public:
	FORMULATOR__GlobalsStorage();
	virtual ~FORMULATOR__GlobalsStorage();

public:
	// project
	int createProject( const QString& strKey );
	int deleteProject( const QString& strKey );
	int clearProjects( void );

	int setActiveProject( const QString& strKey );

public:
	// styles
	int createStyle( const QString& strKey );
	int deleteStyle( const QString& strKey );
	int clearStyles( void );

	int saveStyles( int is_local_user );
	int loadStyles( int is_local_user );

	FORMULATOR__TheOneStyle* getActiveStyle( void )
		{ return m_pStyle; }

	FORMULATOR__TheOneStyle* setActiveStyle( const QString& strKey )
	{
		mapPtr2OneStyle::iterator it = m_styles.find( strKey );
		return (m_pStyle = (it != m_styles.end() ? it.value() : NULL));
	}

public:
	// state
	int createState( const QString& strKey );
	int deleteState( const QString& strKey );
	int clearStates( void );

	FORMULATOR__TheOneState* getActiveState( void )
		{ return m_pState; }

	FORMULATOR__TheOneState* setActiveState( const QString& strKey )
	{
		mapPtr2OneState::iterator it = m_states.find( strKey );
		return (m_pState = (it != m_states.end() ? it.value() : NULL));
	}

public:
	// toolbar set
	void saveToolBarSet( const QString& path );
	void loadToolBarSet( const QString& path );

	FBtnToolbarSet* getToolBarSet( void )
		{ return m_toolBarSet; }

public:
	// other
	DRMessageManager* getCurrentDRMessageManager( void )
		{ return &m_messageManager; }
	FBtnToolbarSet* getCurrentToolBarSet( void )
		{ return getToolBarSet(); }
	const SYSINFO_Unit2PX& getCurrentUnit2PX( void )
		{ return getCurrentFormulatorStyle().getUnit2PX(); }
	FS_TextStyles& getCurrentFormulatorStyle( void )
		{ return getActiveStyle()->formula_style; }
	long getCurrentFormulatorStyle_OnChar( void )
		{ return ( getActiveStyle()->onchar_style ); }
	void setCurrentFormulatorStyle_OnChar( long st )
		{ getActiveStyle()->onchar_style = st; }

	long getCurrentUserChoiceStyle( void )
		{ return ( getActiveStyle()->onuser_style ); }
	void setCurrentUserChoiceStyle( long st )
		{ getActiveStyle()->onuser_style = st; }

	RealFde getCurrentKegl( void )
		{ return getActiveStyle()->onuser_kegl; }
	void setCurrentKegl( RealFde onuser_kegl )
	{
		getActiveStyle()->onuser_kegl = onuser_kegl;
		getActiveStyle()->formula_style.kegl2LevelOffset( getActiveStyle()->onuser_kegl, 
			getActiveStyle()->onuser_kegl_level, getActiveStyle()->onuser_kegl_offset );
	}

	void setCurrentKeglLevel( long st )
		{ getActiveStyle()->onuser_kegl_level = st; }
	long getCurrentKeglLevel( void )
		{ return ( getActiveStyle()->onuser_kegl_level ); }
	RealFde getCurrentKeglOffset( void )
		{ return ( getActiveStyle()->onuser_kegl_offset ); }

	////////////////////////////////////////////////////////
	//////////////////// OPTIONS ///////////////////////////

	long getCurrentFormulatorState_TranslatePref( void )
		{ return ( getActiveState()->translator_pref ); }
	void setCurrentFormulatorState_TranslatePref( long st )
		{ getActiveState()->translator_pref = st; }

	long getCurrentFormulatorContentMathML_TranslateTimes( void );
	void setCurrentFormulatorContentMathML_TranslateTimes( long st );

	void setOptionsValue( long value )
		{ getActiveState()->m_flags.setValue( value ); }
	long getOptionsValue( void )
		{ return getActiveState()->m_flags.getValue(); }

	void setInputMode_Presentation()
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__INPUT_MODE_CONTENT ); }
	void setInputMode_Content()
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__INPUT_MODE_CONTENT ); }
	int isInputMode_Content()
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__INPUT_MODE_CONTENT ); }
	int isInputMode_Presentation()
		{ return !isInputMode_Content(); }

	void setOption_UnicodeAsName()
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__UNICODE_AS_NAME ); }
	void clearOption_UnicodeAsName()
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__UNICODE_AS_NAME ); }
	int isOption_UnicodeAsName()
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__UNICODE_AS_NAME ); }

	void setOption_XHTMLAsBlock()
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__XHTML_AS_BLOCK ); }
	void clearOption_XHTMLAsBlock()
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__XHTML_AS_BLOCK ); }
	int isOption_XHTMLAsBlock()
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__XHTML_AS_BLOCK ); }

	void setOption_MulticharMINotItalic()
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__MULTICHAR_MI_NOT_ITALIC ); }
	void clearOption_MulticharMINotItalic()
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__MULTICHAR_MI_NOT_ITALIC ); }
	int isOption_MulticharMINotItalic()
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__MULTICHAR_MI_NOT_ITALIC ); }

	void setOption_RelativeImgPath()
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__RELATIVE_IMG_PATH ); }
	void clearOption_RelativeImgPath()
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__RELATIVE_IMG_PATH ); }
	int isOption_RelativeImgPath()
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__RELATIVE_IMG_PATH ); }

	void setOption_ShowNavInfo()
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__SHOW_NAVIGATE_INFO ); }
	void clearOption_ShowNavInfo()
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__SHOW_NAVIGATE_INFO ); }
	int isOption_ShowNavInfo()
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__SHOW_NAVIGATE_INFO ); }

	void setOption_ShowNodeTipsInfo()
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__SHOW_NODE_TIPS_INFO ); }
	void clearOption_ShowNodeTipsInfo()
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__SHOW_NODE_TIPS_INFO ); }
	int isOption_ShowNodeTipsInfo()
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__SHOW_NODE_TIPS_INFO ); }

	void setOption_ShowWarning_RefreshMathML()
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__SHOW_REFRESHMATHML_WARN ); }
	void clearOption_ShowWarning_RefreshMathML()
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__SHOW_REFRESHMATHML_WARN ); }
	int isOption_ShowWarning_RefreshMathML()
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__SHOW_REFRESHMATHML_WARN ); }

	void setOption_ShowWarning_InsertMathML()
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__SHOW_INSERTMATHML_WARN ); }
	void clearOption_ShowWarning_InsertMathML()
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__SHOW_INSERTMATHML_WARN ); }
	int isOption_ShowWarning_InsertMathML()
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__SHOW_INSERTMATHML_WARN ); }

	void setOption_ShowNesting( void )
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__SHOWNESTING ); }
	void clearOption_ShowNesting( void )
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__SHOWNESTING ); }
	int isOption_ShowNesting( void )
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__SHOWNESTING ); }

	void setOption_ShowReadOnly( void )
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__SHOWREADONLY ); }
	void clearOption_ShowReadOnly( void )
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__SHOWREADONLY ); }
	int isOption_ShowReadOnly( void )
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__SHOWREADONLY ); }

	void setOption_AutographEditing()
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__AUTOGRAPH_EDITING_STYLE ); }
	void clearOption_AutographEditing()
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__AUTOGRAPH_EDITING_STYLE ); }
	int isOption_AutographEditing()
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__AUTOGRAPH_EDITING_STYLE ); }

	void setOption_AutographEditing_OneCharID()
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__AUTOGRAPH_EDITING_STYLE_ONECHARID ); }
	void clearOption_AutographEditing_OneCharID()
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__AUTOGRAPH_EDITING_STYLE_ONECHARID ); }
	int isOption_AutographEditing_OneCharID()
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__AUTOGRAPH_EDITING_STYLE_ONECHARID ); }

	void setOption_AutographEditing_MoveOutOfHighPriorityOp()
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__AUTOGRAPH_EDITING_STYLE_MOVEOUT ); }
	void clearOption_AutographEditing_MoveOutOfHighPriorityOp()
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__AUTOGRAPH_EDITING_STYLE_MOVEOUT ); }
	int isOption_AutographEditing_MoveOutOfHighPriorityOp()
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__AUTOGRAPH_EDITING_STYLE_MOVEOUT ); }

	void setOption_AutographEditing_dxdt()
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__AUTOGRAPH_EDITING_STYLE_DXDT ); }
	void clearOption_AutographEditing_dxdt()
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__AUTOGRAPH_EDITING_STYLE_DXDT ); }
	int isOption_AutographEditing_dxdt()
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__AUTOGRAPH_EDITING_STYLE_DXDT ); }

	void setOption_RenderArcFunAsPower()
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__AUTOGRAPH_RENDER2_ARCFUN ); }
	void clearOption_RenderArcFunAsPower()
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__AUTOGRAPH_RENDER2_ARCFUN ); }
	int isOption_RenderArcFunAsPower()
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__AUTOGRAPH_RENDER2_ARCFUN ); }

	void setOption_PureContentMathMLOutput()
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__AUTOGRAPH_PURE_CONTENT_OUT ); }
	void clearOption_PureContentMathMLOutput()
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__AUTOGRAPH_PURE_CONTENT_OUT ); }
	int isOption_PureContentMathMLOutput()
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__AUTOGRAPH_PURE_CONTENT_OUT ); }

	void setOption_AutographDiff()
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__AUTOGRAPH_DIFF_AS_YX ); }
	void clearOption_AutographDiff()
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__AUTOGRAPH_DIFF_AS_YX ); }
	int isOption_AutographDiff()
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__AUTOGRAPH_DIFF_AS_YX ); }

	void setShouldRefreshMathML()
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__SHOULD_REFRESH_MATHML ); }
	void clearShouldRefreshMathML()
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__SHOULD_REFRESH_MATHML ); }
	int isShouldRefreshMathML()
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__SHOULD_REFRESH_MATHML ); }

	void setMathMLStylePriority()
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__MATHML_STYLE_HAS_PRIORITY ); }
	void clearMathMLStylePriority()
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__MATHML_STYLE_HAS_PRIORITY ); }
	int isMathMLStylePriority()
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__MATHML_STYLE_HAS_PRIORITY ); }

	void setRenderMode_Force( int val = 1 )
		{ val ? getActiveState()->m_flags.setFlag( OLIMP_MATHML_FLAG__MROW_FORCE_RENDER ) :
				getActiveState()->m_flags.clearFlag( OLIMP_MATHML_FLAG__MROW_FORCE_RENDER ); }
	int isRenderMode_Force()
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_MATHML_FLAG__MROW_FORCE_RENDER ); }

	////////////////////////////////////////////////////////

	void setComaAsSLIST()
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__USE_COMA_AS_LOCALE_SLIST ); }
	void clearComaAsSLIST()
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__USE_COMA_AS_LOCALE_SLIST ); }
	int isComaAsSLIST()
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__USE_COMA_AS_LOCALE_SLIST ); }

	QString getComaUsingCurrentLocale()
		{ return isComaAsSLIST() ? "," : QString(getActiveState()->currentLocale.groupSeparator()); }
	void setCurrentLocale( enum QLocale::Language l, enum QLocale::Country c )
		{ getActiveState()->currentLocale = QLocale(l, c); }
	QLocale getCurrentLocale( void )
		{ return getActiveState()->currentLocale; }

	//
	//
	//

	void setOption_Perfomance_CopyImage( void )
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__PERFOMANCE_COPYIMAGE ); }
	void clearOption_Perfomance_CopyImage( void )
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__PERFOMANCE_COPYIMAGE ); }
	int isOption_Perfomance_CopyImage( void )
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__PERFOMANCE_COPYIMAGE ); }

	void setOption_Perfomance_UpdateMathMLView( void )
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__PERFOMANCE_UPDATEMMLTEXT ); }
	void clearOption_Perfomance_UpdateMathMLView( void )
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__PERFOMANCE_UPDATEMMLTEXT ); }
	int isOption_Perfomance_UpdateMathMLView( void )
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__PERFOMANCE_UPDATEMMLTEXT ); }

	//
	//
	//

	void setOption_Check4Updates( void )
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__CHECK4UPDATES ); }
	void clearOption_Check4Updates( void )
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__CHECK4UPDATES ); }
	int isOption_Check4Updates( void )
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__CHECK4UPDATES ); }

	////////////////////////////////////////////////////////

	void setOption_CalcTextByGlyph()
		{ getActiveState()->m_flags.setFlag( OLIMP_OPTION_FLAG__CALC_TEXTRECT_BY_GLYPH ); }
	void clearOption_CalcTextByGlyph()
		{ getActiveState()->m_flags.clearFlag( OLIMP_OPTION_FLAG__CALC_TEXTRECT_BY_GLYPH ); }
	int isOption_CalcTextByGlyph()
		{ return getActiveState()->m_flags.isSetFlag( OLIMP_OPTION_FLAG__CALC_TEXTRECT_BY_GLYPH ); }

};

/////////////////////////////////////////////////////////////////////
//
//
//

__HERMITECH_FORMULIB__
FORMULATOR__GlobalsStorage& getProjectGlobalsStorage( void );

/////////////////////////////////////////////////////////////////////
//
// project
//
/////////////////////////////////////////////////////////////////////

inline
int FORMULATOR_createProject( const QString& strKey )
	{ return getProjectGlobalsStorage().createProject( strKey ); }
inline
int FORMULATOR_deleteProject( const QString& strKey )
	{ return getProjectGlobalsStorage().deleteProject( strKey ); }
inline
int FORMULATOR_clearProjects( void )
	{ return getProjectGlobalsStorage().clearProjects(); }

inline
int FORMULATOR_setActiveProject( const QString& strKey )
	{ return getProjectGlobalsStorage().setActiveProject( strKey ); }

/////////////////////////////////////////////////////////////////////
//
// style
//
/////////////////////////////////////////////////////////////////////

inline
int FORMULATOR_createStyle( const QString& strKey )
	{ return getProjectGlobalsStorage().createStyle( strKey ); }
inline
int FORMULATOR_deleteStyle( const QString& strKey )
	{ return getProjectGlobalsStorage().deleteStyle( strKey ); }
inline
int FORMULATOR_clearStyles( void )
	{ return getProjectGlobalsStorage().clearStyles(); }

inline
FORMULATOR__TheOneStyle* FORMULATOR_getActiveStyle( void )
	{ return getProjectGlobalsStorage().getActiveStyle(); }
inline
FORMULATOR__TheOneStyle* FORMULATOR_setActiveStyle( const QString& strKey )
	{ return getProjectGlobalsStorage().setActiveStyle( strKey ); }

inline
int FORMULATOR_saveStyles( int is_local_user )
	{ return getProjectGlobalsStorage().saveStyles( is_local_user ); }
inline
int FORMULATOR_loadStyles( int is_local_user )
	{ return getProjectGlobalsStorage().loadStyles( is_local_user ); }

/////////////////////////////////////////////////////////////////////
//
// toolbar
//

inline
void FORMULATOR_saveToolBarSet( QString path = FmlCoreSettings::getXmlBtnFile() )
	{ getProjectGlobalsStorage().saveToolBarSet( path ); }

inline
void FORMULATOR_loadToolBarSet( QString path = FmlCoreSettings::getXmlBtnFile() )
	{ getProjectGlobalsStorage().loadToolBarSet( path ); }

inline
FBtnToolbarSet* getCurrentToolBarSet( void )
	{ return getProjectGlobalsStorage().getCurrentToolBarSet(); }

/////////////////////////////////////////////////////////////////////
//
//
//

inline
DRMessageManager* getCurrentDRMessageManager( void )
	{ return getProjectGlobalsStorage().getCurrentDRMessageManager(); }

/////////////////////////////////////////////////////////////////////
//
//
//

inline
const SYSINFO_Unit2PX& getCurrentUnit2PX( void )
	{ return getProjectGlobalsStorage().getCurrentUnit2PX(); }
inline
FS_TextStyles& getCurrentFormulatorStyle( void )
	{ return getProjectGlobalsStorage().getCurrentFormulatorStyle(); }

inline
long getCurrentFormulatorStyle_OnChar( void )
	{ return getProjectGlobalsStorage().getCurrentFormulatorStyle_OnChar(); }
inline
void setCurrentFormulatorStyle_OnChar( long st )
	{ getProjectGlobalsStorage().setCurrentFormulatorStyle_OnChar( st ); }

inline
long getCurrentUserChoiceStyle( void )
	{ return getProjectGlobalsStorage().getCurrentUserChoiceStyle(); }
inline
void setCurrentUserChoiceStyle( long st )
	{ getProjectGlobalsStorage().setCurrentUserChoiceStyle( st ); }

/////////////////////////////////////////////////////////////////////
//
//
//
//

inline
RealFde getCurrentKegl( void )
	{ return getProjectGlobalsStorage().getCurrentKegl(); }
inline
void setCurrentKegl( RealFde kegl )
	{ getProjectGlobalsStorage().setCurrentKegl( kegl ); }

inline
void setCurrentKeglLevel( long level )
	{ getProjectGlobalsStorage().setCurrentKeglLevel( level ); }
inline
long getCurrentKeglLevel( void )
	{ return getProjectGlobalsStorage().getCurrentKeglLevel(); }
inline
RealFde getCurrentKeglOffset( void )
	{ return getProjectGlobalsStorage().getCurrentKeglOffset(); }

/////////////////////////////////////////////////////////////////////
//
//
// options
//
//

inline
long getCurrentFormulatorStyle_TranslatePref( void )
	{ return getProjectGlobalsStorage().getCurrentFormulatorState_TranslatePref(); }
inline
void setCurrentFormulatorStyle_TranslatePref( long st )
	{ getProjectGlobalsStorage().setCurrentFormulatorState_TranslatePref( st ); }
inline
long getCurrentFormulatorContentMathML_TranslateTimes( void )
	{ return getProjectGlobalsStorage().getCurrentFormulatorContentMathML_TranslateTimes(); }
inline
void setCurrentFormulatorContentMathML_TranslateTimes( long st )
	{ getProjectGlobalsStorage().setCurrentFormulatorContentMathML_TranslateTimes( st ); }

inline
void setCurrentFormulatorOptionsValue( long value )
	{ getProjectGlobalsStorage().setOptionsValue( value ); }
inline
long getCurrentFormulatorOptionsValue( void )
	{ return getProjectGlobalsStorage().getOptionsValue(); }

inline
void setCurrentFormulatorNestingState( int show )
	{ show ? getProjectGlobalsStorage().setOption_ShowNesting() : getProjectGlobalsStorage().clearOption_ShowNesting(); }
inline
int getCurrentFormulatorNestingState( void )
	{ return (getProjectGlobalsStorage().isOption_ShowNesting() ? 1 : 0); }

inline
void setCurrentFormulatorReadOnlyState( long show )
	{ show ? getProjectGlobalsStorage().setOption_ShowReadOnly() : getProjectGlobalsStorage().clearOption_ShowReadOnly(); }
inline
int getCurrentFormulatorReadOnlyState( void )
	{ return (getProjectGlobalsStorage().isOption_ShowReadOnly() ? 1 : 0); }

inline
void setOption_UnicodeAsName()
	{ getProjectGlobalsStorage().setOption_UnicodeAsName(); }
inline
void clearOption_UnicodeAsName()
	{ getProjectGlobalsStorage().clearOption_UnicodeAsName(); }
inline
int isOption_UnicodeAsName()
	{ return getProjectGlobalsStorage().isOption_UnicodeAsName(); }

inline
void setOption_XHTMLAsBlock()
	{ getProjectGlobalsStorage().setOption_XHTMLAsBlock(); }
inline
void clearOption_XHTMLAsBlock()
	{ getProjectGlobalsStorage().clearOption_XHTMLAsBlock(); }
inline
int isOption_XHTMLAsBlock()
	{ return getProjectGlobalsStorage().isOption_XHTMLAsBlock(); }

inline
void setOption_MulticharMINotItalic()
	{ getProjectGlobalsStorage().setOption_MulticharMINotItalic(); }
inline
void clearOption_MulticharMINotItalic()
	{ getProjectGlobalsStorage().clearOption_MulticharMINotItalic(); }
inline
int isOption_MulticharMINotItalic()
	{ return getProjectGlobalsStorage().isOption_MulticharMINotItalic(); }

inline
void setOption_RelativeImgPath()
	{ getProjectGlobalsStorage().setOption_RelativeImgPath(); }
inline
void clearOption_RelativeImgPath()
	{ getProjectGlobalsStorage().clearOption_RelativeImgPath(); }
inline
int isOption_RelativeImgPath()
	{ return getProjectGlobalsStorage().isOption_RelativeImgPath(); }

inline
void setOption_ShowNavInfo()
	{ getProjectGlobalsStorage().setOption_ShowNavInfo(); }
inline
void clearOption_ShowNavInfo()
	{ getProjectGlobalsStorage().clearOption_ShowNavInfo(); }
inline
int isOption_ShowNavInfo()
	{ return getProjectGlobalsStorage().isOption_ShowNavInfo(); }

inline
void setOption_ShowWarning_RefreshMathML()
	{ getProjectGlobalsStorage().setOption_ShowWarning_RefreshMathML(); }
inline
void clearOption_ShowWarning_RefreshMathML()
	{ getProjectGlobalsStorage().clearOption_ShowWarning_RefreshMathML(); }
inline
int isOption_ShowWarning_RefreshMathML()
	{ return getProjectGlobalsStorage().isOption_ShowWarning_RefreshMathML(); }

inline
void setOption_ShowWarning_InsertMathML()
	{ getProjectGlobalsStorage().setOption_ShowWarning_InsertMathML(); }
inline
void clearOption_ShowWarning_InsertMathML()
	{ getProjectGlobalsStorage().clearOption_ShowWarning_InsertMathML(); }
inline
int isOption_ShowWarning_InsertMathML()
	{ return getProjectGlobalsStorage().isOption_ShowWarning_InsertMathML(); }

inline
void setOption_ShowNodeTipsInfo()
	{ getProjectGlobalsStorage().setOption_ShowNodeTipsInfo(); }
inline
void clearOption_ShowNodeTipsInfo()
	{ getProjectGlobalsStorage().clearOption_ShowNodeTipsInfo(); }
inline
int isOption_ShowNodeTipsInfo()
	{ return getProjectGlobalsStorage().isOption_ShowNodeTipsInfo(); }

inline
void setInputMode_Presentation()
	{ getProjectGlobalsStorage().setInputMode_Presentation(); }

inline
void setInputMode_Content()
	{ getProjectGlobalsStorage().setInputMode_Content(); }

inline
int isInputMode_Content()
	{ return getProjectGlobalsStorage().isInputMode_Content(); }

inline
int isInputMode_Presentation()
	{ return getProjectGlobalsStorage().isInputMode_Presentation(); }

inline
void setRenderMode_Force( int val = 1 )
	{ getProjectGlobalsStorage().setRenderMode_Force( val ); }

inline
int isRenderMode_Force()
	{ return getProjectGlobalsStorage().isRenderMode_Force(); }

inline
void setShouldRefreshMathML()
	{ getProjectGlobalsStorage().setShouldRefreshMathML(); }

inline
void clearShouldRefreshMathML()
	{ getProjectGlobalsStorage().clearShouldRefreshMathML(); }

inline
int isShouldRefreshMathML()
	{ return getProjectGlobalsStorage().isShouldRefreshMathML(); }

inline
void setMathMLStylePriority()
	{ getProjectGlobalsStorage().setMathMLStylePriority(); }

inline
void clearMathMLStylePriority()
	{ getProjectGlobalsStorage().clearMathMLStylePriority(); }

inline
int isMathMLStylePriority()
	{ return getProjectGlobalsStorage().isMathMLStylePriority(); }

inline
void setComaAsSLIST()
	{ getProjectGlobalsStorage().setComaAsSLIST(); }

inline
void clearComaAsSLIST()
	{ getProjectGlobalsStorage().clearComaAsSLIST(); }

inline
int isComaAsSLIST()
	{ return getProjectGlobalsStorage().isComaAsSLIST(); }

inline
void setAutographEditing()
	{ getProjectGlobalsStorage().setOption_AutographEditing(); }

inline
void clearAutographEditing()
	{ getProjectGlobalsStorage().clearOption_AutographEditing(); }

inline
int isAutographEditing()
	{ return getProjectGlobalsStorage().isOption_AutographEditing(); }

inline
void setAutographEditing_OneCharID()
	{ getProjectGlobalsStorage().setOption_AutographEditing_OneCharID(); }

inline
void clearAutographEditing_OneCharID()
	{ getProjectGlobalsStorage().clearOption_AutographEditing_OneCharID(); }

inline
int isAutographEditing_OneCharID()
	{ return getProjectGlobalsStorage().isOption_AutographEditing_OneCharID(); }

inline
void setAutographEditing_MoveOutOfHighPriorityOp()
	{ getProjectGlobalsStorage().setOption_AutographEditing_MoveOutOfHighPriorityOp(); }

inline
void clearAutographEditing_MoveOutOfHighPriorityOp()
	{ getProjectGlobalsStorage().clearOption_AutographEditing_MoveOutOfHighPriorityOp(); }

inline
int isAutographEditing_MoveOutOfHighPriorityOp()
	{ return getProjectGlobalsStorage().isOption_AutographEditing_MoveOutOfHighPriorityOp(); }

inline
void setAutographEditing_dxdt()
	{ getProjectGlobalsStorage().setOption_AutographEditing_dxdt(); }

inline
void clearAutographEditing_dxdt()
	{ getProjectGlobalsStorage().clearOption_AutographEditing_dxdt(); }

inline
int isAutographEditing_dxdt()
	{ return getProjectGlobalsStorage().isOption_AutographEditing_dxdt(); }

inline
void setRenderArcFunAsPower()
	{ getProjectGlobalsStorage().setOption_RenderArcFunAsPower(); }

inline
void clearRenderArcFunAsPower()
	{ getProjectGlobalsStorage().clearOption_RenderArcFunAsPower(); }

inline
int isRenderArcFunAsPower()
	{ return getProjectGlobalsStorage().isOption_RenderArcFunAsPower(); }

inline
void setPureContentMathMLOutput()
	{ getProjectGlobalsStorage().setOption_PureContentMathMLOutput(); }

inline
void clearPureContentMathMLOutput()
	{ getProjectGlobalsStorage().clearOption_PureContentMathMLOutput(); }

inline
int isPureContentMathMLOutput()
	{ return getProjectGlobalsStorage().isOption_PureContentMathMLOutput(); }

inline
void setAutographDiff()
	{ getProjectGlobalsStorage().setOption_AutographDiff(); }

inline
void clearAutographDiff()
	{ getProjectGlobalsStorage().clearOption_AutographDiff(); }

inline
int isAutographDiff()
	{ return getProjectGlobalsStorage().isOption_AutographDiff(); }

inline
void setAutographEnvironment()
{
	::setAutographEditing();
	::setAutographEditing_OneCharID();
	::setAutographEditing_MoveOutOfHighPriorityOp();
	::setAutographEditing_dxdt();
	::setRenderArcFunAsPower();
	::setPureContentMathMLOutput();
	::setAutographDiff();
}

/////////////////////////////////////////////////////////////////////

inline
QString getComaUsingCurrentLocale( void )
	{ return getProjectGlobalsStorage().getComaUsingCurrentLocale(); }

inline
void FORMULATOR_setCurrentLocale( enum QLocale::Language l, enum QLocale::Country c )
	{ getProjectGlobalsStorage().setCurrentLocale( l, c ); }

inline
QLocale FORMULATOR_getCurrentLocale( void )
	{ return getProjectGlobalsStorage().getCurrentLocale(); }


/////////////////////////////////////////////////////////////////////

inline
void setOption_Perfomance_CopyImage( void )
	{ getProjectGlobalsStorage().setOption_Perfomance_CopyImage(); }

inline
void clearOption_Perfomance_CopyImage( void )
	{ getProjectGlobalsStorage().clearOption_Perfomance_CopyImage(); }

inline
int isOption_Perfomance_CopyImage( void )
	{ return getProjectGlobalsStorage().isOption_Perfomance_CopyImage(); }

/////////////////////////////////////////////////////////////////////

inline
void setOption_UpdateMathMLView( void )
	{ getProjectGlobalsStorage().setOption_Perfomance_UpdateMathMLView(); }

inline
void clearOption_UpdateMathMLView( void )
	{ getProjectGlobalsStorage().clearOption_Perfomance_UpdateMathMLView(); }

inline
int isOption_UpdateMathMLView( void )
	{ return getProjectGlobalsStorage().isOption_Perfomance_UpdateMathMLView(); }

/////////////////////////////////////////////////////////////////////

inline
void setOption_Check4Updates( void )
	{ getProjectGlobalsStorage().setOption_Check4Updates(); }

inline
void clearOption_Check4Updates( void )
	{ getProjectGlobalsStorage().clearOption_Check4Updates(); }

inline
int isOption_Check4Updates( void )
	{ return getProjectGlobalsStorage().isOption_Check4Updates(); }

/////////////////////////////////////////////////////////////////////

inline
void setOption_CalcTextByGlyph()
	{ getProjectGlobalsStorage().setOption_CalcTextByGlyph(); }
inline
void clearOption_CalcTextByGlyph()
	{ getProjectGlobalsStorage().clearOption_CalcTextByGlyph(); }
inline
int isOption_CalcTextByGlyph()
	{ return getProjectGlobalsStorage().isOption_CalcTextByGlyph(); }

/////////////////////////////////////////////////////////////////////

inline
int shouldCreateCTSOnMissing( int new_value = -1 )
{
	static int bShouldCreateCTSOnMissing = 1;
	int ret = bShouldCreateCTSOnMissing;
	if( new_value != -1 )
		bShouldCreateCTSOnMissing = (new_value != 0);
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
//
//
//
//
//

inline
const FS_LogFont_Color& getFontStyle( long style )
	{ return getProjectGlobalsStorage().getCurrentFormulatorStyle().getFontStyle( style ); }

inline
const FS_LogFont_Color& getFontStyleFormula( long style )
	{ return getProjectGlobalsStorage().getCurrentFormulatorStyle().getFontStyle( style ); }

///////////////////////////////////////////////////////////////////////////////

inline
RealFde getKegl( long level ) 
	{ return getProjectGlobalsStorage().getCurrentFormulatorStyle().getKegl( level ); }

inline
void kegl2LevelOffset( RealFde kegl, qint32& level, RealFde& offset )
	{ getProjectGlobalsStorage().getCurrentFormulatorStyle().kegl2LevelOffset( kegl, level, offset ); }

inline
void kegl2LevelOffset( RealFde kegl, QVector<long> &level, RealFde& offset )
	{ getProjectGlobalsStorage().getCurrentFormulatorStyle().kegl2LevelOffset( kegl, level, offset ); }

///////////////////////////////////////////////////////////////////////////////

inline
void updateDefaults( void )
	{ getProjectGlobalsStorage().getCurrentFormulatorStyle().updateDefaults(); }

inline
const SizeFde& getCurrentDefaultSize( long level )
	{ return getProjectGlobalsStorage().getCurrentFormulatorStyle().getDefaultSize( level ); }

inline
RealFde getCurrentDefaultAscent( long level )
	{ return getProjectGlobalsStorage().getCurrentFormulatorStyle().getDefaultAscent( level ); }

inline
RealFde getCurrentDefaultDescent( long level )
	{ return getProjectGlobalsStorage().getCurrentFormulatorStyle().getDefaultDescent( level ); }

/////////////////////////////////////////////////////////////////////

inline
int isOtherStyle( int style )
{
	return (style < FSTYLE_NONE || style >= FSTYLE_DEFAULT_FONT_NUMBER);
}

inline
int isOtherSize( RealFde kegl )
{
	qint32 level;
	RealFde offset;
	::kegl2LevelOffset( kegl, level, offset );
	return offset != 0;
}

inline
int isOtherSizeIdx( int no )
{
	return (no < FTEXT_HEIGHT_LARGEOP || no > FTEXT_HEIGHT_LAST);
}

/////////////////////////////////////////////////////////////////////
__HERMITECH_FORMULIB__ void defaultFormulatorSettings();
__HERMITECH_FORMULIB__ void presetWordFormulatorSettings();
__HERMITECH_FORMULIB__ void presetPmmlFormulatorSettings();
__HERMITECH_FORMULIB__ void presetCmmlFormulatorSettings();
__HERMITECH_FORMULIB__ void presetCmmlPmmlFormulatorSettings();
__HERMITECH_FORMULIB__ void presetCmmlAutographFormulatorSettings();
/////////////////////////////////////////////////////////////////////

#endif
