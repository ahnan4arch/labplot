#include "TitleDialog.h"
#include "gui/LabelWidget.h"
#include "..
TitleDialog::TitleDialog(MainWin *mw, Label *title)	: KDialog(mw){

	kDebug()<<"TitleDialog()"<<endl;
	setCaption(i18n("Title"));

	labelWidget = new LabelWidget(this);
	labelWidget->setLabel(title);
	this->setMainWidget( labelWidget );
	this->setButtons( KDialog::Ok | KDialog::Cancel | KDialog::Apply);

	connect( this, SIGNAL( applyClicked() ), this, SLOT( apply() ) );
	connect( this, SIGNAL( okClicked() ), this, SLOT( ok() ) );
	connect( this, SIGNAL( changed( bool ) ), this, SLOT( enableButtonApply( bool ) ) );

	this->enableButtonApply( false );
	resize( QSize(300,400) );

}

void TitleDialog::saveSettings() {
	labelWidget->save();

	//TODO close the dialog

}

void TitleDialog::apply() {
	labelWidget->save();

	//TODO
	// mw->update()
// 	Worksheet *w = mw->activeWorksheet();
// 	if(w != 0)
// 		w->repaint();
}
