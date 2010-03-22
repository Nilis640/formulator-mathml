#ifndef __FMLCORE_DLG_BUILDTOOLBARSETDLG_H__
#define __FMLCORE_DLG_BUILDTOOLBARSETDLG_H__

#include <QDialog>
#include "../GeneratedFiles/ui_buildtoolbarset.h"
#include "../../fmlcore_global.h"

class __HERMITECH_FORMULIB__ CBuildToolbarsetDlg
	: public QDialog//, public Ui::BuildToolbarset
{
    Q_OBJECT

public:
    CBuildToolbarsetDlg();
    ~CBuildToolbarsetDlg();

private slots:
	void slotAdd();
	void slotDelete();
	void slotClear();
	void slotBuild();
	void slotBrowse();

private:
	Ui::BuildToolbarset m_ui;
};

#endif
