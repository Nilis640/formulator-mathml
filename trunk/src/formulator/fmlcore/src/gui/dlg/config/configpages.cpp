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

#include "HMathML/isettings.h"
#include "configpages.h"
#include "../../../mathml/mml_tag2mml.h"

static const QString __page_isbold("normal");
static const QColor __page_remarkBackground(0, 0, 0x3F);
static const int __page_internalWidgetWidth = 520;

/////////////////////////////////////////////////////////////////////////////

CTypesettingPage::CTypesettingPage(QWidget *parent)
    : QWidget(parent)
{
	QGroupBox *group = new QGroupBox(tr("Typesetting Rules"));

	m_isContent = new QCheckBox("\"Content MathML\" Input Mode");
	QLabel *m_isContentLabel = new QLabel( QString(
		"<p style=\"font-weight: %1; color: %2;\">Automatically detect Content MathML statements while a user inputs text "
		"(e.g., insert arithmetic operation if + is typed, or insert sin function "
		"form instead of sequential s, i, n characters).</p>").arg(__page_isbold).arg(__page_remarkBackground.name()));
	m_isContentLabel->setMaximumWidth(__page_internalWidgetWidth);
	m_isContentLabel->setWordWrap(true);

	m_isMoveOut = new QCheckBox("Move out of the current node on a lower precedence operator");
	QLabel *m_isMoveOutLabel = new QLabel( QString(
		"<p style=\"font-weight: %1; color: %2;\">If a newly typed operator has lower precedence than the current operator "
		"under the caret, then automatically move the caret to the right "
		"(i.e., out of the current operator under the caret).</p>").arg(__page_isbold).arg(__page_remarkBackground.name()));
	m_isMoveOutLabel->setMaximumWidth(__page_internalWidgetWidth);
	m_isMoveOutLabel->setWordWrap(true);

	m_isDxDt = new QCheckBox("Put an input slot for a derivative at the numerator");
	QLabel *m_isDxDtLabel = new QLabel( QString(
		"<p style=\"font-weight: %1; color: %2;\">If a user inserts a derivative in the form dx/dt, put an input slot at the "
		"numerator instead of the area after the fraction.</p>").arg(__page_isbold).arg(__page_remarkBackground.name()));
	m_isDxDtLabel->setMaximumWidth(__page_internalWidgetWidth);
	m_isDxDtLabel->setWordWrap(true);

	m_isOneCharID = new QCheckBox(" Insert times and power operations when identifiers and numbers are typed");
	QLabel *m_isOneCharIDLabel = new QLabel( QString(
		"<p style=\"font-weight: %1; color: %2;\">Identifiers are considered to be single-letter.<br/>When a user is sequentially "
		"typing one character, an exponentiation operator is inserted and power is "
		"automatically adjusted. When a user is sequentially typing characters and numbers, "
		"times operations are inserted between them automatically.</p>").arg(__page_isbold).arg(__page_remarkBackground.name()));
	m_isOneCharIDLabel->setMaximumWidth(__page_internalWidgetWidth);
	m_isOneCharIDLabel->setWordWrap(true);

	QVBoxLayout *configLayout = new QVBoxLayout();
	configLayout->addWidget( m_isContent );
	configLayout->addWidget( m_isContentLabel );
	configLayout->addSpacing(8);
	configLayout->addWidget( m_isMoveOut );
	configLayout->addWidget( m_isMoveOutLabel );
	configLayout->addSpacing(8);
	configLayout->addWidget( m_isDxDt );
	configLayout->addWidget( m_isDxDtLabel );
	configLayout->addSpacing(8);
	configLayout->addWidget( m_isOneCharID );
	configLayout->addWidget( m_isOneCharIDLabel );

	QHBoxLayout *resetLayout = new QHBoxLayout();
	QPushButton *resetBtn = new QPushButton(tr("&Reset only this page"));
	connect(resetBtn, SIGNAL(clicked()), this, SLOT(defaultSettings()));
	resetLayout->addWidget( resetBtn, 0, Qt::AlignHCenter );
	//resetLayout->addStretch( 1 );
	configLayout->addSpacing(8);
	configLayout->addLayout(resetLayout);

	group->setLayout(configLayout);
	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->addWidget(group);
	mainLayout->addStretch(1);
	setLayout(mainLayout);

	load();
}

void CTypesettingPage::load()
{
	m_isContent->setCheckState( ::isInputMode_Content() ? Qt::Checked : Qt::Unchecked );
	m_isMoveOut->setCheckState( ::isAutographEditing_MoveOutOfHighPriorityOp() ? Qt::Checked : Qt::Unchecked );
	m_isDxDt->setCheckState( ::isAutographEditing_dxdt() ? Qt::Checked : Qt::Unchecked );
	m_isOneCharID->setCheckState( ::isAutographEditing_OneCharID() ? Qt::Checked : Qt::Unchecked );

	update();
}

void CTypesettingPage::defaultSettings()
{
	m_isContent->setCheckState( Qt::Unchecked );
	m_isMoveOut->setCheckState( Qt::Unchecked );
	m_isDxDt->setCheckState( Qt::Unchecked );
	m_isOneCharID->setCheckState( Qt::Unchecked );
}

void CTypesettingPage::loadPresetWord()
{
	defaultSettings();
}

void CTypesettingPage::loadPresetPmml()
{
	defaultSettings();
}

void CTypesettingPage::loadPresetCmml()
{
	defaultSettings();
	m_isContent->setCheckState( Qt::Checked );
}

void CTypesettingPage::loadPresetCmmlPmml()
{
	defaultSettings();
	m_isContent->setCheckState( Qt::Checked );
}

void CTypesettingPage::loadPresetCmmlAutograph()
{
	m_isContent->setCheckState( Qt::Checked );
	m_isMoveOut->setCheckState( Qt::Checked );
	m_isDxDt->setCheckState( Qt::Checked );
	m_isOneCharID->setCheckState( Qt::Checked );
}

void CTypesettingPage::save()
{
	if( m_isContent->checkState() == Qt::Checked )
		::setInputMode_Content();
	else
		::setInputMode_Presentation();
	if( m_isMoveOut->checkState() == Qt::Checked )
		::setAutographEditing_MoveOutOfHighPriorityOp();
	else
		::clearAutographEditing_MoveOutOfHighPriorityOp();
	if( m_isDxDt->checkState() == Qt::Checked )
		::setAutographEditing_dxdt();
	else
		::clearAutographEditing_dxdt();
	if( m_isOneCharID->checkState() == Qt::Checked )
		::setAutographEditing_OneCharID();
	else
		::clearAutographEditing_OneCharID();
}

/////////////////////////////////////////////////////////////////////////////

CTranslatePage::CTranslatePage(QWidget *parent)
    : QWidget(parent)
{
	QGroupBox *group = new QGroupBox(tr("Translate to MathML"));

	m_isSymbolUnicode = new QCheckBox("Use symbol names for Unicode characters");
	QLabel *m_isSymbolUnicodeLabel = new QLabel( QString(
		"<p style=\"font-weight: %1; color: %2;\">Use Entity names instead of Unicode characters "
		"numeric codes while converting a formula to MathML.</p>").arg(__page_isbold).arg(__page_remarkBackground.name()));
	m_isSymbolUnicodeLabel->setMaximumWidth(__page_internalWidgetWidth);
	m_isSymbolUnicodeLabel->setWordWrap(true);

	m_displayBlockBtn = new QRadioButton("block");
	m_displayInlineBtn = new QRadioButton("inline");
	QButtonGroup *displayBtnGroup = new QButtonGroup();
	displayBtnGroup->addButton(m_displayBlockBtn);
	displayBtnGroup->addButton(m_displayInlineBtn);
	QLabel *displayLabel = new QLabel( QString(
		"<p style=\"font-weight: %1; color: %2;\">Attribute 'display' of the root tag &lt;math&gt; value "
		"will have this value for usage in XHTML.</p>").arg(__page_isbold).arg(__page_remarkBackground.name()));
	displayLabel->setMaximumWidth(__page_internalWidgetWidth);
	displayLabel->setWordWrap(true);
	QVBoxLayout *displayLayout1 = new QVBoxLayout();
	displayLayout1->addWidget(m_displayBlockBtn);
	displayLayout1->addWidget(m_displayInlineBtn);
	displayLayout1->addStretch(1);
	QHBoxLayout *displayLayout2 = new QHBoxLayout();
	displayLayout2->addLayout(displayLayout1);
	displayLayout2->addWidget(displayLabel);
	QGroupBox *displayGroup = new QGroupBox(tr("\'Display\' attribute"));
	displayGroup->setLayout(displayLayout2);

	m_namespaceNoBtn = new QRadioButton("no namespace");
	m_namespaceMBtn = new QRadioButton("\'m\'");
	m_namespaceXmlnsBtn = new QRadioButton("\'xmlns\'");
	QButtonGroup *namespaceBtnGroup = new QButtonGroup();
	namespaceBtnGroup->addButton(m_namespaceNoBtn);
	namespaceBtnGroup->addButton(m_namespaceMBtn);
	namespaceBtnGroup->addButton(m_namespaceXmlnsBtn);
	QLabel *namespaceLabel = new QLabel( QString(
		"<p style=\"font-weight: %1; color: %2;\">How to use namespace when "
		"translating a document to MathML.</p>").arg(__page_isbold).arg(__page_remarkBackground.name()));
	//namespaceLabel->setMaximumWidth(__page_internalWidgetWidth / 2);
	namespaceLabel->setWordWrap(true);
	QVBoxLayout *namespaceLayout1 = new QVBoxLayout();
	namespaceLayout1->addWidget(m_namespaceNoBtn);
	namespaceLayout1->addWidget(m_namespaceMBtn);
	namespaceLayout1->addWidget(m_namespaceXmlnsBtn);
	namespaceLayout1->addStretch(1);
	QHBoxLayout *namespaceLayout2 = new QHBoxLayout();
	namespaceLayout2->addLayout(namespaceLayout1);
	//namespaceLayout2->addStretch(1);
	namespaceLayout2->addWidget(namespaceLabel);
	QGroupBox *namespaceGroup = new QGroupBox(tr("Namespace"));
	namespaceGroup->setLayout(namespaceLayout2);

	m_timesTimesBtn = new QRadioButton("&&times;");
	m_timesSdotBtn = new QRadioButton("&&sdot;");
	m_timesInvisibleBtn = new QRadioButton("&&InvisibleTimes;");
	QButtonGroup *timesBtnGroup = new QButtonGroup();
	timesBtnGroup->addButton(m_timesTimesBtn);
	timesBtnGroup->addButton(m_timesSdotBtn);
	timesBtnGroup->addButton(m_timesInvisibleBtn);
	QLabel *timesLabel = new QLabel( QString(
		"<p style=\"font-weight: %1; color: %2;\">How to render &lt;times/&gt; "
		"element in Content MathML.</p>").arg(__page_isbold).arg(__page_remarkBackground.name()));
	timesLabel->setMaximumWidth(__page_internalWidgetWidth);
	timesLabel->setWordWrap(true);
	QVBoxLayout *timesLayout1 = new QVBoxLayout();
	timesLayout1->addWidget(m_timesTimesBtn);
	timesLayout1->addWidget(m_timesSdotBtn);
	timesLayout1->addWidget(m_timesInvisibleBtn);
	timesLayout1->addStretch(1);
	QHBoxLayout *timesLayout2 = new QHBoxLayout();
	timesLayout2->addLayout(timesLayout1);
	timesLayout2->addWidget(timesLabel);
	QGroupBox *timesGroup = new QGroupBox(tr("<times/>"));
	timesGroup->setLayout(timesLayout2);

	m_isPureContent = new QCheckBox(" Export only Content MathML markup");
	QLabel *m_isPureContentLabel = new QLabel( QString(
		"<p style=\"font-weight: %1; color: %2;\">Do NOT keep Presentation MathML tags "
		"and attributes for color and font when exporting Content MathML.</p>").arg(__page_isbold).arg(__page_remarkBackground.name()));
	m_isPureContentLabel->setMaximumWidth(__page_internalWidgetWidth);
	m_isPureContentLabel->setWordWrap(true);

	QVBoxLayout *configLayout = new QVBoxLayout();
	configLayout->addWidget( m_isSymbolUnicode );
	configLayout->addWidget( m_isSymbolUnicodeLabel );
	configLayout->addSpacing(8);
	configLayout->addWidget( displayGroup );
	configLayout->addSpacing(8);
	configLayout->addWidget( namespaceGroup );
	configLayout->addSpacing(8);
	configLayout->addWidget( timesGroup );
	configLayout->addSpacing(8);
	configLayout->addWidget( m_isPureContent );
	configLayout->addWidget( m_isPureContentLabel );

	QHBoxLayout *resetLayout = new QHBoxLayout();
	QPushButton *resetBtn = new QPushButton(tr("&Reset only this page"));
	connect(resetBtn, SIGNAL(clicked()), this, SLOT(defaultSettings()));
	resetLayout->addWidget( resetBtn, 0, Qt::AlignHCenter );
	//resetLayout->addStretch( 1 );
	configLayout->addSpacing(8);
	configLayout->addLayout(resetLayout);

	group->setLayout(configLayout);
	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->addWidget(group);
	mainLayout->addStretch(1);
	setLayout(mainLayout);

	load();
}

void CTranslatePage::load()
{
	m_isSymbolUnicode->setCheckState( ::isOption_UnicodeAsName() ? Qt::Checked : Qt::Unchecked );

	if( ::isOption_XHTMLAsBlock() )
		m_displayBlockBtn->setChecked( true );
	else
		m_displayInlineBtn->setChecked( true );

	switch(::getCurrentFormulatorStyle_TranslatePref())
	{
	case TRANSLATE_2_MATHML2_NO_NAMESPACE:
		m_namespaceNoBtn->setChecked( true );
		break;
	case TRANSLATE_2_MATHML2_NAMESPACE_M:
		m_namespaceMBtn->setChecked( true );
		break;
	case TRANSLATE_2_MATHML2_NAMESPACE_ATTR:
	default:
		m_namespaceXmlnsBtn->setChecked( true );
		break;
	}

	switch(::getCurrentFormulatorContentMathML_TranslateTimes())
	{
	case TRANSLATE_CONTENT_MATHML2_TIMES:
		m_timesTimesBtn->setChecked( true );
		break;
	case TRANSLATE_CONTENT_MATHML2_SDOT:
		m_timesSdotBtn->setChecked( true );
		break;
	case TRANSLATE_CONTENT_MATHML2_INVISIBLETIMES:
	default:
		m_timesInvisibleBtn->setChecked( true );
		break;
	}

	m_isPureContent->setCheckState( ::isPureContentMathMLOutput() ? Qt::Checked : Qt::Unchecked );

	update();
}

void CTranslatePage::defaultSettings()
{
	m_isSymbolUnicode->setCheckState( Qt::Checked );
	m_displayInlineBtn->setChecked( true );
	m_namespaceNoBtn->setChecked( true );
	m_timesTimesBtn->setChecked( true );
	m_isPureContent->setCheckState( Qt::Unchecked );
}

void CTranslatePage::loadPresetWord()
{
	defaultSettings();
}

void CTranslatePage::loadPresetPmml()
{
	defaultSettings();
}

void CTranslatePage::loadPresetCmml()
{
	defaultSettings();
	m_isPureContent->setCheckState( Qt::Checked );
}

void CTranslatePage::loadPresetCmmlPmml()
{
	defaultSettings();
}

void CTranslatePage::loadPresetCmmlAutograph()
{
	loadPresetCmml();
}

void CTranslatePage::save()
{
	if( m_isSymbolUnicode->checkState() == Qt::Checked )
		::setOption_UnicodeAsName();
	else
		::clearOption_UnicodeAsName();

	if( m_displayBlockBtn->isChecked() )
		::setOption_XHTMLAsBlock();
	else
		::clearOption_XHTMLAsBlock();

	if( m_namespaceNoBtn->isChecked() )
		::setCurrentFormulatorStyle_TranslatePref(TRANSLATE_2_MATHML2_NO_NAMESPACE);
	else if( m_namespaceMBtn->isChecked() )
		::setCurrentFormulatorStyle_TranslatePref(TRANSLATE_2_MATHML2_NAMESPACE_M);
	else if( m_namespaceXmlnsBtn->isChecked() )
		::setCurrentFormulatorStyle_TranslatePref(TRANSLATE_2_MATHML2_NAMESPACE_ATTR);

	if( m_timesTimesBtn->isChecked() )
		::setCurrentFormulatorContentMathML_TranslateTimes(TRANSLATE_CONTENT_MATHML2_TIMES);
	else if( m_timesSdotBtn->isChecked() )
		::setCurrentFormulatorContentMathML_TranslateTimes(TRANSLATE_CONTENT_MATHML2_SDOT);
	else if( m_timesInvisibleBtn->isChecked() )
		::setCurrentFormulatorContentMathML_TranslateTimes(TRANSLATE_CONTENT_MATHML2_INVISIBLETIMES);

	if( m_isPureContent->checkState() == Qt::Checked )
		::setPureContentMathMLOutput();
	else
		::clearPureContentMathMLOutput();
}

/////////////////////////////////////////////////////////////////////////////

CRenderingPage::CRenderingPage(QWidget *parent)
    : QWidget(parent)
{
	QGroupBox *group = new QGroupBox(tr("MathML Rendering"));

	m_isMultipleItalic = new QCheckBox("Multiple characters <mi> are italic");
	QLabel *m_isMultipleItalicLabel = new QLabel( QString(
		"<p style=\"font-weight: %1; color: %2;\">Identifiers (content of &lt;mi&gt; tag) are rendered "
		"as italic even if they have more than one character length.</p>").arg(__page_isbold).arg(__page_remarkBackground.name()));
	m_isMultipleItalicLabel->setMaximumWidth(__page_internalWidgetWidth);
	m_isMultipleItalicLabel->setWordWrap(true);

	m_isForceMathML = new QCheckBox("Force using default values of \'mathvariant\' and \'mathcolor\'");
	QLabel *m_isForceMathMLLabel = new QLabel( QString(
		"<p style=\"font-weight: %1; color: %2;\">Use default values of Presentation MathML token style "
		"attributes instead of current font and color settings of editor’s styles.</p>").arg(__page_isbold).arg(__page_remarkBackground.name()));
	m_isForceMathMLLabel->setMaximumWidth(__page_internalWidgetWidth);
	m_isForceMathMLLabel->setWordWrap(true);

	m_isShowHidden = new QCheckBox("Show hidden markup");
	QLabel *m_isShowHiddenLabel = new QLabel( QString(
		"<p style=\"font-weight: %1; color: %2;\">Invisible Content MathML elements (e.g., declare) "
		"will be rendered and allow editing.</p>").arg(__page_isbold).arg(__page_remarkBackground.name()));
	m_isShowHiddenLabel->setMaximumWidth(__page_internalWidgetWidth);
	m_isShowHiddenLabel->setWordWrap(true);

	m_isDiff = new QCheckBox("Use f' and f'' to render differentiation");
	QLabel *m_isDiffLabel = new QLabel( QString(
		"<p style=\"font-weight: %1; color: %2;\">Use f&apos; and f&apos;&apos; to render the derivative of the first "
		"and the second degree with respect to a bound variable.</p>").arg(__page_isbold).arg(__page_remarkBackground.name()));
	m_isDiffLabel->setMaximumWidth(__page_internalWidgetWidth);
	m_isDiffLabel->setWordWrap(true);

	m_isArc = new QCheckBox("Render ARC function as an inverse functions (e.g., sin-1)");

	m_isCalcTextByGlyph = new QCheckBox( tr("Justify text height to the highest symbol") );

	m_listComaBtn = new QRadioButton("Use coma symbol \",\"");
	m_listLocaleBtn = new QRadioButton("Use current locale");
	QButtonGroup *listBtnGroup = new QButtonGroup();
	listBtnGroup->addButton(m_listComaBtn);
	listBtnGroup->addButton(m_listLocaleBtn);
	QVBoxLayout *listLayout1 = new QVBoxLayout();
	listLayout1->addWidget(m_listComaBtn);
	listLayout1->addWidget(m_listLocaleBtn);
	listLayout1->addStretch(1);
	QGroupBox *listGroup = new QGroupBox(tr("List items and function arguments separator"));
	listGroup->setLayout(listLayout1);

	QVBoxLayout *configLayout = new QVBoxLayout();
	configLayout->addWidget( m_isMultipleItalic );
	configLayout->addWidget( m_isMultipleItalicLabel );
	configLayout->addSpacing( 8 );
	configLayout->addWidget( m_isForceMathML );
	configLayout->addWidget( m_isForceMathMLLabel );
	configLayout->addSpacing( 8 );
	configLayout->addWidget( m_isShowHidden );
	configLayout->addWidget( m_isShowHiddenLabel );
	configLayout->addSpacing( 8 );
	configLayout->addWidget( m_isDiff );
	configLayout->addWidget( m_isDiffLabel );
	configLayout->addSpacing( 8 );
	configLayout->addWidget( m_isArc );
	configLayout->addSpacing( 8 );
	configLayout->addWidget( m_isCalcTextByGlyph );
	configLayout->addSpacing( 8 );
	configLayout->addWidget( listGroup );

	QHBoxLayout *resetLayout = new QHBoxLayout();
	QPushButton *resetBtn = new QPushButton(tr("&Reset only this page"));
	connect(resetBtn, SIGNAL(clicked()), this, SLOT(defaultSettings()));
	resetLayout->addWidget( resetBtn, 0, Qt::AlignHCenter );
	//resetLayout->addStretch( 1 );
	configLayout->addSpacing(8);
	configLayout->addLayout(resetLayout);

	group->setLayout(configLayout);
	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->addWidget(group);
	mainLayout->addStretch(1);
	setLayout(mainLayout);

	load();
}

void CRenderingPage::load()
{
	m_isMultipleItalic->setCheckState( !::isOption_MulticharMINotItalic() ? Qt::Checked : Qt::Unchecked );
	m_isForceMathML->setCheckState( ::isMathMLStylePriority() ? Qt::Checked : Qt::Unchecked );
	m_isShowHidden->setCheckState( ::isRenderMode_Force() ? Qt::Checked : Qt::Unchecked );
	m_isDiff->setCheckState( ::isAutographDiff() ? Qt::Checked : Qt::Unchecked );
	m_isArc->setCheckState( ::isRenderArcFunAsPower() ? Qt::Checked : Qt::Unchecked );
	m_isCalcTextByGlyph->setCheckState( ::isOption_CalcTextByGlyph() ? Qt::Checked : Qt::Unchecked );

	if( ::isComaAsSLIST() )
		m_listComaBtn->setChecked( true );
	else
		m_listLocaleBtn->setChecked( true );

	update();
}

void CRenderingPage::defaultSettings()
{
	m_isMultipleItalic->setCheckState( Qt::Checked );
	m_isForceMathML->setCheckState( Qt::Unchecked );
	m_isShowHidden->setCheckState( Qt::Unchecked );
	m_isDiff->setCheckState( Qt::Unchecked );
	m_isArc->setCheckState( Qt::Unchecked );
	m_isCalcTextByGlyph->setCheckState( Qt::Unchecked );
	m_listComaBtn->setChecked( true );
}

void CRenderingPage::loadPresetWord()
{
	defaultSettings();
}

void CRenderingPage::loadPresetPmml()
{
	defaultSettings();
	m_isForceMathML->setCheckState( Qt::Checked );
}

void CRenderingPage::loadPresetCmml()
{
	loadPresetPmml();
}

void CRenderingPage::loadPresetCmmlPmml()
{
	loadPresetPmml();
}

void CRenderingPage::loadPresetCmmlAutograph()
{
	loadPresetPmml();
	m_isDiff->setCheckState( Qt::Checked );
	m_isArc->setCheckState( Qt::Checked );
}

void CRenderingPage::save()
{
	if( m_isMultipleItalic->checkState() == Qt::Checked )
		::clearOption_MulticharMINotItalic();
	else
		::setOption_MulticharMINotItalic();
	if( m_isForceMathML->checkState() == Qt::Checked )
		::setMathMLStylePriority();
	else
		::clearMathMLStylePriority();
	if( m_isShowHidden->checkState() == Qt::Checked )
		::setRenderMode_Force( 1 );
	else
		::setRenderMode_Force( 0 );
	if( m_isDiff->checkState() == Qt::Checked )
		::setAutographDiff();
	else
		::clearAutographDiff();
	if( m_isArc->checkState() == Qt::Checked )
		::setRenderArcFunAsPower();
	else
		::clearRenderArcFunAsPower();

	if( m_isCalcTextByGlyph->checkState() == Qt::Checked )
		::setOption_CalcTextByGlyph();
	else
		::clearOption_CalcTextByGlyph();

	if( m_listComaBtn->isChecked() )
		::setComaAsSLIST();
	else
		::clearComaAsSLIST();
}

/////////////////////////////////////////////////////////////////////////////

CAppearancePage::CAppearancePage(QWidget *parent)
    : QWidget(parent)
{
	QGroupBox *group = new QGroupBox(tr("Document Appearance"));

	QGroupBox *onScreenGroup = new QGroupBox(tr("On-Screen Margins:"));

    QLabel *leftLabel = new QLabel(tr("&Left (px):"));
	leftEdit = new QSpinBox();
	leftEdit->setRange( 0, 2000 );
	leftLabel->setBuddy( leftEdit );

	QLabel *topLabel = new QLabel(tr("&Top (px):"));
	topEdit = new QSpinBox();
	topEdit->setRange( 0, 2000 );
	topLabel->setBuddy( topEdit );

	QLabel *rightLabel = new QLabel(tr("&Right (px):"));
	rightEdit = new QSpinBox();
	rightEdit->setRange( 0, 2000 );
	rightLabel->setBuddy( rightEdit );

	QLabel *bottomLabel = new QLabel(tr("&Bottom (px):"));
	bottomEdit = new QSpinBox();
	bottomEdit->setRange( 0, 2000 );
	bottomLabel->setBuddy( bottomEdit );

	QLabel *linespacingLabel = new QLabel(tr("Line &Spacing (%):"));
	linespacingEdit = new QSpinBox();
	linespacingEdit->setRange(1, 1000);
	linespacingLabel->setBuddy( linespacingEdit );

	//QPushButton *resetBtn = new QPushButton(tr("Reset"));
	//connect(resetBtn, SIGNAL(clicked()), this, SLOT(resetSettings()));

	QGridLayout *centralLayout = new QGridLayout();
	centralLayout->addWidget( topLabel, 0, 0, Qt::AlignRight );
	centralLayout->addWidget( topEdit, 0, 1, Qt::AlignLeft );
	centralLayout->addWidget( bottomLabel, 0, 2, Qt::AlignRight );
	centralLayout->addWidget( bottomEdit, 0, 3, Qt::AlignLeft );
	centralLayout->addWidget( leftLabel, 1, 0, Qt::AlignRight );
	centralLayout->addWidget( leftEdit, 1, 1, Qt::AlignLeft );
	centralLayout->addWidget( rightLabel, 1, 2, Qt::AlignRight );
	centralLayout->addWidget( rightEdit, 1, 3, Qt::AlignLeft );
	centralLayout->setVerticalSpacing(8);
	centralLayout->setHorizontalSpacing(16);
	onScreenGroup->setLayout( centralLayout );

	QHBoxLayout *lsLayout = new QHBoxLayout();
	lsLayout->addWidget( linespacingLabel );
	lsLayout->addWidget( linespacingEdit );
	lsLayout->addStretch( 1 );

	//QHBoxLayout *btnLayout = new QHBoxLayout();
	//btnLayout->addWidget( resetBtn, 1, Qt::AlignHCenter );

	QVBoxLayout *configLayout = new QVBoxLayout();
	configLayout->addWidget( onScreenGroup );
	configLayout->addSpacing( 8 );
	configLayout->addLayout( lsLayout );
	//configLayout->addSpacing( 8 );
	//configLayout->addLayout( btnLayout );
	configLayout->addSpacing( 8 );

	QHBoxLayout *resetLayout = new QHBoxLayout();
	QPushButton *resetBtn = new QPushButton(tr("&Reset only this page"));
	connect(resetBtn, SIGNAL(clicked()), this, SLOT(defaultSettings()));
	resetLayout->addWidget( resetBtn, 0, Qt::AlignHCenter );
	//resetLayout->addStretch( 1 );
	configLayout->addSpacing(8);
	configLayout->addLayout(resetLayout);

	group->setLayout(configLayout);
	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->addWidget(group);
	mainLayout->addStretch(1);
	setLayout(mainLayout);

	load();
}

void CAppearancePage::load()
{
	RectFde rcIndents;
	::Formulib_GetIndents( rcIndents );
	leftEdit->setValue( (int) rcIndents.left() );
	topEdit->setValue( (int) rcIndents.top() );
	rightEdit->setValue( (int) rcIndents.right() );
	bottomEdit->setValue( (int) rcIndents.bottom() );
	linespacingEdit->setValue( (int) (100.0 * ::Formulib_GetLineSpacing()) );

	update();

	update();
}

void CAppearancePage::defaultSettings()
{
	leftEdit->setValue( (int) DEFAULT_INDENTS );
	topEdit->setValue( (int) DEFAULT_INDENTS );
	rightEdit->setValue( (int) DEFAULT_INDENTS );
	bottomEdit->setValue( (int) DEFAULT_INDENTS );
	linespacingEdit->setValue( (int) (100.0 * DEFAULT_LINESPACING) );
}

void CAppearancePage::loadPresetWord()
{
	//defaultSettings();
}

void CAppearancePage::loadPresetPmml()
{
	//defaultSettings();
}

void CAppearancePage::loadPresetCmml()
{
	//defaultSettings();
}

void CAppearancePage::loadPresetCmmlPmml()
{
	//defaultSettings();
}

void CAppearancePage::loadPresetCmmlAutograph()
{
	//defaultSettings();
}

void CAppearancePage::save()
{
	::Formulib_SetLineSpacing( linespacingEdit->value() / 100.0 );
	RectFde rect( PointFde(leftEdit->value(), topEdit->value()), PointFde(rightEdit->value(), bottomEdit->value()) );
	::Formulib_SetIndents( rect );
}

void CAppearancePage::resetSettings()
{
	leftEdit->setValue( (int) DEFAULT_INDENTS );
	topEdit->setValue( (int) DEFAULT_INDENTS );
	rightEdit->setValue( (int) DEFAULT_INDENTS );
	bottomEdit->setValue( (int) DEFAULT_INDENTS );
	linespacingEdit->setValue( (int) (100.0 * DEFAULT_LINESPACING) );
}

/////////////////////////////////////////////////////////////////////////////

CWarningsPage::CWarningsPage(QWidget *parent)
    : QWidget(parent)
{
	const int __page_internalWidgetWidth = 520;
	QGroupBox *group = new QGroupBox(tr("Warnings and Information"));

	m_isShowNavigation = new QCheckBox("Show MathML navigation info");
	QLabel *m_isShowNavigationLabel = new QLabel( QString(
		"<p style=\"font-weight: %1; color: %2;\">Display navigation information about the current and "
		"the parent MathML nodes in the Status Bar.</p>").arg(__page_isbold).arg(__page_remarkBackground.name()));
	m_isShowNavigationLabel->setMaximumWidth(__page_internalWidgetWidth);
	m_isShowNavigationLabel->setWordWrap(true);

	//m_isShowRefresh = new QCheckBox("Show \"Refresh MathML\" warning");
	//QLabel *m_isShowRefreshLabel = new QLabel( QString(
	//	"<p style=\"font-weight: %1; color: %2;\">Display a warning (an icon to the right of the Status Bar) "
	//	"when the current document needs to be refreshed through MathML due to reflect changes "
	//	"in general MathML settings.</p>").arg(__page_isbold).arg(__page_remarkBackground.name()));
	//m_isShowRefreshLabel->setMaximumWidth(__page_internalWidgetWidth);
	//m_isShowRefreshLabel->setWordWrap(true);

	m_isShowClipboard = new QCheckBox("Show warning when inserting plain text from Clipboard");
	QLabel *m_isShowClipboardLabel = new QLabel( QString(
		"<p style=\"font-weight: %1; color: %2;\">Show warning message if the clipboard text can't be converted "
		"into equations because of error(s).</p>").arg(__page_isbold).arg(__page_remarkBackground.name()));
	m_isShowClipboardLabel->setMaximumWidth(__page_internalWidgetWidth);
	m_isShowClipboardLabel->setWordWrap(true);

	QVBoxLayout *configLayout = new QVBoxLayout();
	configLayout->addWidget( m_isShowNavigation );
	configLayout->addWidget( m_isShowNavigationLabel );
	configLayout->addSpacing(8);
	//configLayout->addWidget( m_isShowRefresh );
	//configLayout->addWidget( m_isShowRefreshLabel );
	//configLayout->addSpacing(8);
	configLayout->addWidget( m_isShowClipboard );
	configLayout->addWidget( m_isShowClipboardLabel );

	QHBoxLayout *resetLayout = new QHBoxLayout();
	QPushButton *resetBtn = new QPushButton(tr("&Reset only this page"));
	connect(resetBtn, SIGNAL(clicked()), this, SLOT(defaultSettings()));
	resetLayout->addWidget( resetBtn, 0, Qt::AlignHCenter );
	//resetLayout->addStretch( 1 );
	configLayout->addSpacing(8);
	configLayout->addLayout(resetLayout);

	group->setLayout(configLayout);
	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->addWidget(group);
	mainLayout->addStretch(1);
	setLayout(mainLayout);

	load();
}

void CWarningsPage::load()
{
	m_isShowNavigation->setCheckState( ::isOption_ShowNavInfo() ? Qt::Checked : Qt::Unchecked );
	//m_isShowRefresh->setCheckState( ::isOption_ShowWarning_RefreshMathML() ? Qt::Checked : Qt::Unchecked );
	m_isShowClipboard->setCheckState( ::isOption_ShowWarning_InsertMathML() ? Qt::Checked : Qt::Unchecked );

	update();
}

void CWarningsPage::defaultSettings()
{
	m_isShowNavigation->setCheckState( Qt::Unchecked );
	//m_isShowRefresh->setCheckState( Qt::Unchecked );
	m_isShowClipboard->setCheckState( Qt::Checked );
}

void CWarningsPage::loadPresetWord()
{
	m_isShowNavigation->setCheckState( Qt::Unchecked );
	m_isShowClipboard->setCheckState( Qt::Checked );
}

void CWarningsPage::loadPresetPmml()
{
	m_isShowNavigation->setCheckState( Qt::Checked );
	//m_isShowRefresh->setCheckState( Qt::Checked );
	m_isShowClipboard->setCheckState( Qt::Checked );
}

void CWarningsPage::loadPresetCmml()
{
	loadPresetPmml();
}

void CWarningsPage::loadPresetCmmlPmml()
{
	loadPresetPmml();
}

void CWarningsPage::loadPresetCmmlAutograph()
{
	loadPresetPmml();
}

void CWarningsPage::save()
{
	if( m_isShowNavigation->checkState() == Qt::Checked )
		::setOption_ShowNavInfo();
	else
		::clearOption_ShowNavInfo();
	//if( m_isShowRefresh->checkState() == Qt::Checked )
	//	::setOption_ShowWarning_RefreshMathML();
	//else
	//	::clearOption_ShowWarning_RefreshMathML();
	if( m_isShowClipboard->checkState() == Qt::Checked )
		::setOption_ShowWarning_InsertMathML();
	else
		::clearOption_ShowWarning_InsertMathML();
}

/////////////////////////////////////////////////////////////////////////////

CPerfomancePage::CPerfomancePage(QWidget *parent)
    : QWidget(parent)
{
	const int __page_internalWidgetWidth = 520;
	QGroupBox *group = new QGroupBox(tr("Perfomance (affects speed for big documents)"));
	QGroupBox *group2 = new QGroupBox(tr("Automatic Updates"));

	m_isCopyImage = new QCheckBox("Copy image of formula when copying formula to Clipboard");
	QLabel *m_isCopyImageLabel = new QLabel( QString(
		"<p style=\"font-weight: %1; color: %2;\">In addition to text format and native Formulator format "
		"a raster image will be copied to Clipboard also.</p>").arg(__page_isbold).arg(__page_remarkBackground.name()));
	m_isCopyImageLabel->setMaximumWidth(__page_internalWidgetWidth);
	m_isCopyImageLabel->setWordWrap(true);

	m_isUpdateMmlEditor = new QCheckBox("Update MathML editor on-the-fly");
	QLabel *m_isUpdateMmlEditorLabel = new QLabel( QString(
		"<p style=\"font-weight: %1; color: %2;\">Refresh MathML Editor contents automatically each time "
		"when the current document is changed or a user switches between documents, in order to reflect in "
		"text changes in formula visual appearance. If you turn this option off, then a button </p>"
		"<p align=\"center\"><img src=\":/images/get.png\"></img></p>"
		"<p style=\"font-weight: %3; color: %4;\">should be clicked to update MathML editor contents according "
		"to formula visual appearance.</p>")
		.arg(__page_isbold).arg(__page_remarkBackground.name())
		.arg(__page_isbold).arg(__page_remarkBackground.name()));
	m_isUpdateMmlEditorLabel->setMaximumWidth(__page_internalWidgetWidth);
	m_isUpdateMmlEditorLabel->setWordWrap(true);

	m_isUpdate = new QCheckBox("Allow automatic update requests (http://www.mmlsoft.com)");
	QLabel *m_isUpdateLabel = new QLabel( QString(
		"<p style=\"font-weight: %1; color: %2;\">Check periodically if a new version of the software is available.</p>")
		.arg(__page_isbold).arg(__page_remarkBackground.name()));
	m_isUpdateLabel->setMaximumWidth(__page_internalWidgetWidth);
	m_isUpdateLabel->setWordWrap(true);

	QVBoxLayout *configLayout = new QVBoxLayout();
	configLayout->addWidget( m_isCopyImage );
	configLayout->addWidget( m_isCopyImageLabel );
	configLayout->addSpacing(8);
	configLayout->addWidget( m_isUpdateMmlEditor );
	configLayout->addWidget( m_isUpdateMmlEditorLabel );
	group->setLayout(configLayout);

	QVBoxLayout *configLayout2 = new QVBoxLayout();
	configLayout2->addWidget( m_isUpdate );
	configLayout2->addWidget( m_isUpdateLabel );
	group2->setLayout(configLayout2);

	QHBoxLayout *resetLayout = new QHBoxLayout();
	QPushButton *resetBtn = new QPushButton(tr("&Reset only this page"));
	connect(resetBtn, SIGNAL(clicked()), this, SLOT(defaultSettings()));
	resetLayout->addWidget( resetBtn, 0, Qt::AlignHCenter );

	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->addWidget(group);
	mainLayout->addSpacing(8);
	mainLayout->addWidget(group2);
	mainLayout->addSpacing(8);
	mainLayout->addLayout(resetLayout);
	mainLayout->addStretch(1);
	setLayout(mainLayout);

	load();
}

void CPerfomancePage::load()
{
	m_isCopyImage->setCheckState( ::isOption_Perfomance_CopyImage() ? Qt::Checked : Qt::Unchecked );
	m_isUpdateMmlEditor->setCheckState( ::isOption_UpdateMathMLView() ? Qt::Checked : Qt::Unchecked );
	m_isUpdate->setCheckState( ::isOption_Check4Updates() ? Qt::Checked : Qt::Unchecked );

	update();
}

void CPerfomancePage::defaultSettings()
{
	m_isCopyImage->setCheckState( Qt::Checked );
	m_isUpdateMmlEditor->setCheckState( Qt::Checked );
	m_isUpdate->setCheckState( Qt::Checked );
}

void CPerfomancePage::loadPresetWord()
{
}

void CPerfomancePage::loadPresetPmml()
{
}

void CPerfomancePage::loadPresetCmml()
{
}

void CPerfomancePage::loadPresetCmmlPmml()
{
}

void CPerfomancePage::loadPresetCmmlAutograph()
{
}

void CPerfomancePage::save()
{
	if( m_isCopyImage->checkState() == Qt::Checked )
		::setOption_Perfomance_CopyImage();
	else
		::clearOption_Perfomance_CopyImage();
	if( m_isUpdateMmlEditor->checkState() == Qt::Checked )
		::setOption_UpdateMathMLView();
	else
		::clearOption_UpdateMathMLView();
	if( m_isUpdate->checkState() == Qt::Checked )
		::setOption_Check4Updates();
	else
		::clearOption_Check4Updates();
}

/////////////////////////////////////////////////////////////////////////////
