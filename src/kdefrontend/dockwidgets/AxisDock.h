/***************************************************************************
    File                 : AxisDock.h
    Project              : LabPlot
    Description          : axes widget class
    --------------------------------------------------------------------
    Copyright            : (C) 2011-2014 by Alexander Semke (alexander.semke@web.de)
    Copyright            : (C) 2013 by Stefan Gerlach (stefan.gerlach@uni-konstanz.de)

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor,                    *
 *   Boston, MA  02110-1301  USA                                           *
 *                                                                         *
 ***************************************************************************/
#ifndef AXISDOCK_H
#define AXISDOCK_H

#include "ui_axisdock.h"
#include "backend/worksheet/plots/cartesian/Axis.h"

class AbstractAspect;
class LabelWidget;
class TreeViewComboBox;
class AspectTreeModel;
class AbstractColumn;

class AxisDock : public QWidget{
	Q_OBJECT

public:
	explicit AxisDock(QWidget*);
    ~AxisDock();

	void setAxes(QList<Axis*>);
	void activateTitleTab();

private:
	Ui::AxisDock ui;
	QList<Axis*> m_axesList;
	Axis* m_axis;
	AspectTreeModel* m_aspectTreeModel;
	LabelWidget* labelWidget;
	TreeViewComboBox* cbMajorTicksColumn;
	TreeViewComboBox* cbMinorTicksColumn;
	bool m_dataChanged;
	bool m_initializing;

	void setModel();
	void setModelIndexFromColumn(TreeViewComboBox*, const AbstractColumn*);

	void load();
	void loadConfig(KConfig&);

private slots:
	void init();

	//SLOTs for changes triggered in AxisDock
	//"General"-tab
	void nameChanged();
	void commentChanged();
	void visibilityChanged(bool);
	void orientationChanged(int);
	void positionChanged(int);
	void positionChanged();
	void scaleChanged(int);
	void autoScaleChanged(int);
	void startChanged();
	void endChanged();
	void zeroOffsetChanged();
	void scalingFactorChanged();

	//Line-Tab
  	void lineStyleChanged(int);
	void lineColorChanged(const QColor&);
	void lineWidthChanged(double);
	void lineOpacityChanged(int);
	void arrowPositionChanged(int);
	void arrowTypeChanged(int);
	void arrowSizeChanged(int);

	//"Major ticks"-tab
	void majorTicksDirectionChanged(int);
	void majorTicksTypeChanged(int);
 	void majorTicksNumberChanged(int);
	void majorTicksIncrementChanged();
	void majorTicksColumnChanged(const QModelIndex&);
	void majorTicksLineStyleChanged(int);
	void majorTicksColorChanged(const QColor&);
	void majorTicksWidthChanged(double);
	void majorTicksLengthChanged(double);
	void majorTicksOpacityChanged(int);

	//"Minor ticks"-tab
	void minorTicksDirectionChanged(int);
	void minorTicksTypeChanged(int);
 	void minorTicksNumberChanged(int);
	void minorTicksIncrementChanged();
	void minorTicksColumnChanged(const QModelIndex&);
	void minorTicksLineStyleChanged(int);
	void minorTicksColorChanged(const QColor&);
	void minorTicksWidthChanged(double);
	void minorTicksLengthChanged(double);
	void minorTicksOpacityChanged(int);

	//"Extra ticks"-tab

	//"Tick labels"-tab
	void labelsFormatChanged(int);
	void labelsPrecisionChanged(int);
	void labelsAutoPrecisionChanged(int);
	void labelsPositionChanged(int);
	void labelsOffsetChanged(double);
	void labelsRotationChanged(int);
	void labelsFontChanged(const QFont&);
	void labelsFontColorChanged(const QColor&);
	void labelsPrefixChanged();
	void labelsSuffixChanged();
	void labelsOpacityChanged(int);

	//"Grid"-tab
  	void majorGridStyleChanged(int);
	void majorGridColorChanged(const QColor&);
	void majorGridWidthChanged(double);
	void majorGridOpacityChanged(int);

  	void minorGridStyleChanged(int);
	void minorGridColorChanged(const QColor&);
	void minorGridWidthChanged(double);
	void minorGridOpacityChanged(int);

	//SLOTs for changes triggered in Axis
	//General-Tab
	void axisDescriptionChanged(const AbstractAspect*);
	void axisOrientationChanged(Axis::AxisOrientation);
	void axisPositionChanged(Axis::AxisPosition);
	void axisPositionChanged(float);
	void axisScaleChanged(Axis::AxisScale);
	void axisAutoScaleChanged(bool);
	void axisStartChanged(float);
	void axisEndChanged(float);
	void axisZeroOffsetChanged(qreal);
	void axisScalingFactorChanged(qreal);

	//line
	void axisLinePenChanged(const QPen&);
	void axisLineOpacityChanged(qreal);
	void axisArrowTypeChanged(Axis::ArrowType);
	void axisArrowPositionChanged(Axis::ArrowPosition);
	void axisArrowSizeChanged(float);

	//ticks
	void axisMajorTicksDirectionChanged(Axis::TicksDirection);
	void axisMajorTicksTypeChanged(Axis::TicksType);
	void axisMajorTicksNumberChanged(int);
	void axisMajorTicksIncrementChanged(qreal);
	void axisMajorTicksPenChanged(const QPen&);
	void axisMajorTicksLengthChanged(qreal);
	void axisMajorTicksOpacityChanged(qreal);
	void axisMinorTicksDirectionChanged(Axis::TicksDirection);
	void axisMinorTicksTypeChanged(Axis::TicksType);
	void axisMinorTicksNumberChanged(int);
	void axisMinorTicksIncrementChanged(qreal);
	void axisMinorTicksPenChanged(const QPen&);
	void axisMinorTicksLengthChanged(qreal);
	void axisMinorTicksOpacityChanged(qreal);

	//labels
	void axisLabelsFormatChanged(Axis::LabelsFormat);
	void axisLabelsAutoPrecisionChanged(bool);
	void axisLabelsPrecisionChanged(int);
	void axisLabelsPositionChanged(Axis::LabelsPosition);
	void axisLabelsOffsetChanged(float);
	void axisLabelsRotationAngleChanged(qreal);
	void axisLabelsFontChanged(const QFont&);
	void axisLabelsFontColorChanged(const QColor&);
	void axisLabelsPrefixChanged(const QString&);
	void axisLabelsSuffixChanged(const QString&);
	void axisLabelsOpacityChanged(qreal);

	//grids
	void axisMajorGridPenChanged(const QPen&);
	void axisMajorGridOpacityChanged(qreal);
	void axisMinorGridPenChanged(const QPen&);
	void axisMinorGridOpacityChanged(qreal);

	void axisVisibilityChanged(bool);

	//save/load template
	void loadConfigFromTemplate(KConfig&);
	void saveConfigAsTemplate(KConfig&);

signals:
	void info(const QString&);
};

#endif
