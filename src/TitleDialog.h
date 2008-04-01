#ifndef TITLEDIALOG_H
#define TITLEDIALOG_H

#include "kialog.h"

class LabelWidget;
class Label;

class TitleDialog: public KDialog{
	Q_OBJECT

public:
	TitleDialog(MainWin *mw, Label *title=0);

private:
	LabelWidget* labelWidget;
	Label *title;

private slots:
	void apply();
	void ok();
};

#endif //TITLEDIALOG_H
