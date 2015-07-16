/***************************************************************************
    File                 : Axes.h
    Project              : LabPlot
    Description          : 3D plot axes
    --------------------------------------------------------------------
    Copyright            : (C) 2015 by Minh Ngo (minh@fedoraproject.org)

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

#ifndef PLOT3D_AXES_H
#define PLOT3D_AXES_H

#include "backend/core/AbstractAspect.h"

#include <QColor>

#include <vtkSmartPointer.h>

class vtkProp;
class vtkActor;
class vtkRenderer;

class AxesPrivate;
class Axes : public AbstractAspect {
		Q_OBJECT
		Q_DECLARE_PRIVATE(Axes)
		Q_DISABLE_COPY(Axes)
	public:
		enum AxesType {
			AxesType_Cube = 0,
			AxesType_Plain = 1,
			AxesType_NoAxes = 2
		};

		Axes(vtkRenderer& renderer);
		~Axes();

		void updateBounds();

		bool operator==(vtkProp* prop) const;
		bool operator!=(vtkProp* prop) const;

		bool isShown() const;

		void setType(AxesType type);
		void setFontSize(int fontSize);
		void setWidth(double width);
		void setXLabelColor(const QColor& color);
		void setYLabelColor(const QColor& color);
		void setZLabelColor(const QColor& color);

		typedef Axes BaseClass;
		typedef AxesPrivate Private;

	signals:
		friend class AxesSetTypeCmd;
		friend class AxesSetFontSizeCmd;
		friend class AxesSetWidthCmd;
		friend class AxesSetXLabelColorCmd;
		friend class AxesSetYLabelColorCmd;
		friend class AxesSetZLabelColorCmd;
		void typeChanged(Axes::AxesType);
		void fontSizeChanged(int);
		void widthChanged(double);
		void xLabelColorChanged(const QColor&);
		void yLabelColorChanged(const QColor&);
		void zLabelColorChanged(const QColor&);

	private:
		const QScopedPointer<AxesPrivate> d_ptr;
};

#endif