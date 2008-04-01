#ifndef LABEL_H
#define LABEL_H

#include <QString>
#include <QFont>
#include <QColor>
#include <QPoint>

#include "../definitions.h"
#include "../Point.h"

class Label {

public:
	Label( QString text=QString("") );
	void draw(QPainter *p, QPoint pos, QPoint size, int w, int h, double phi);
	void draw(QPainter *p, Point pos, Point size, int w, int h, double phi);


/*	QStringList Info();
	void save(QTextStream *ts);
	void open(QTextStream *ts, int version, bool skip=true);
	QDomElement saveXML(QDomDocument doc);
	void openXML(QDomNode node);
	void saveSettings(KConfig *config, QString entry);
	void readSettings(KConfig *config, QString entry);
	void draw(class Worksheet *ws, QPainter *p, Point pos,Point size, int w, int h, double phi);
										// draw the label with given width and height
	bool inside(int x0, int y0, Point pos, Point size, int w, int h);
	bool insideY(int x0, int y0, Point pos, Point size, int w, int h);
	bool insideZ(int x0, int y0, Point pos, Point size, int w, int h);	// for 3d y-axes
*/
// 	void setPosition(double X, double Y) { m_x=X; m_y=Y; }

	ACCESSFUNC(short, m_positionType, positionType, PositionType);
	ACCESSFUNC(QPoint, m_position, position, Position);

	ACCESSFUNCFLAG(m_fillingEnabled, Filling);
	ACCESSFUNC(QColor, m_fillingColor, fillingColor, FillingColor);
	ACCESSFUNCFLAG(m_boxEnabled, Box);
	ACCESSFUNCFLAG(m_shadowEnabled, Shadow);

	ACCESSFUNC(QFont, m_textFont, textFont, TextFont);
	ACCESSFUNC(QColor, m_textColor, textColor, TextColor);
	ACCESSFUNCFLAG(m_texEnabled, Tex);
	ACCESSFUNC(QString, m_text, text, Text);

private:
	short m_positionType;
	QPoint m_position;
	double m_rotation;		// label rotation

	bool m_fillingEnabled;
	QColor m_fillingColor;
	bool m_boxEnabled;
	bool m_shadowEnabled;

	QFont m_textFont;			// label font
	QColor m_textColor;
	QString m_text;			// label string
	bool m_texEnabled;
};

#endif //LABEL_H
