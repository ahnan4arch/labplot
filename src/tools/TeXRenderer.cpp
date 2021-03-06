/***************************************************************************
    File                 : TeXRenderer.cc
    Project              : LabPlot
    Description          : TeX renderer class
    --------------------------------------------------------------------
    Copyright            : (C) 2008-2016 by Alexander Semke (alexander.semke@web.de)
    Copyright            : (C) 2012 by Stefan Gerlach (stefan.gerlach@uni-konstanz.de)

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
#include "TeXRenderer.h"
#include "backend/lib/macros.h"

#include <KConfigGroup>
#include <KDebug>
#include <KGlobal>
#include <KConfig>

#include <QImage>
#include <QColor>
#include <QDir>
#include <QTemporaryFile>
#include <QTextStream>
#include <QProcess>

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QStandardPaths>
#else
#include <KStandardDirs>
#endif


/*!
	\class TeXRenderer
	\brief Implements rendering of latex code to a PNG image.

	Uses latex engine specified by the user (default xelatex) to render LaTeX text

	\ingroup tools
*/
QImage TeXRenderer::renderImageLaTeX(const QString& teXString, bool* success, const TeXRenderer::Formatting& format) {
	const QColor& fontColor =format.fontColor;
	const int fontSize = format.fontSize;
	const QString& fontFamily = format.fontFamily;
	const int dpi = format.dpi;

	//determine the temp directory where the produced files are going to be created
	QString tempPath;
#ifdef Q_OS_LINUX
	//on linux try to use shared memory device first if available
	static bool useShm = QDir("/dev/shm/").exists();
	if (useShm)
		tempPath = "/dev/shm/";
	else
		tempPath = QDir::tempPath();
#else
	tempPath = QDir::tempPath();
#endif

	//create a temporary file
	QTemporaryFile file(tempPath + QDir::separator() + "labplot_XXXXXX.tex");
	// FOR DEBUG: file.setAutoRemove(false);
	// DEBUG("temp file path = " << file.fileName().toUtf8().constData());
	if(file.open()) {
		QDir::setCurrent(tempPath);
	} else {
		qWarning() << "Couldn't open the file " << file.fileName();
		*success = false;
		return QImage();
	}

	//determine latex engine to be used
	KConfigGroup group = KGlobal::config()->group(QLatin1String("Settings_Worksheet"));
	QString engine = group.readEntry("LaTeXEngine", "pdflatex");

	// create latex code
	QTextStream out(&file);
	int headerIndex = teXString.indexOf("\\begin{document}");
	QString body;
	if (headerIndex != -1) {
		//user provided a complete latex document -> extract the document header and body
		QString header = teXString.left(headerIndex);
		int footerIndex = teXString.indexOf("\\end{document}");
		body = teXString.mid(headerIndex + 16, footerIndex - headerIndex - 16);
		out << header;
	} else {
		//user simply provided a document body (assume it's a math. expression) -> add a minimal header
		out << "\\documentclass{minimal}";
		if (teXString.indexOf('$') == -1)
			body = '$' + teXString + '$';
		else
			body = teXString;

		//replace line breaks with tex command for a line break '\\'
		body = body.replace(QLatin1String("\n"), QLatin1String("\\\\"));
	}

	if (engine == "xelatex" || engine == "lualatex") {
		out << "\\usepackage{xltxtra}";
		out << "\\defaultfontfeatures{Ligatures=TeX}";
		if (!fontFamily.isEmpty())
			out << "\\setmainfont[Mapping=tex-text]{" << fontFamily << "}";
	}

	out << "\\usepackage{color}";
	out << "\\usepackage[active,displaymath,textmath,tightpage]{preview}";
	// TODO: this fails with pdflatex
	//out << "\\usepackage{mathtools}";
	out << "\\definecolor{fontcolor}{rgb}{" << fontColor.redF() << ',' << fontColor.greenF() << ',' << fontColor.blueF() << "}";
	out << "\\begin{document}";
	out << "\\begin{preview}";
	out << "\\fontsize{" << QString::number(fontSize) << "}{" << QString::number(fontSize) << "}\\selectfont";
	out << "\\color{fontcolor}";
	out << body;
	out << "\\end{preview}";
	out << "\\end{document}";
	out.flush();
	if (engine == "latex")
		return imageFromDVI(file, dpi, success);
	else
		return imageFromPDF(file, dpi, engine, success);
}

// TEX -> PDF -> PNG
QImage TeXRenderer::imageFromPDF(const QTemporaryFile& file, const int dpi, const QString& engine, bool* success) {
	QFileInfo fi(file.fileName());
	QProcess latexProcess;
#if defined(HAVE_WINDOWS)
	latexProcess.setNativeArguments("-interaction=batchmode " + file.fileName());
	latexProcess.start(engine, QStringList() << "");
#else
	latexProcess.start(engine, QStringList() << "-interaction=batchmode" << file.fileName());
#endif
	if (!latexProcess.waitForFinished()) {
		kWarning() << engine << "process failed." << endl;
		DEBUG("latex process failed!");
		*success = false;
		QFile::remove(fi.completeBaseName() + ".aux");
		QFile::remove(fi.completeBaseName() + ".log");
		return QImage();
	}
	*success = (latexProcess.exitCode() == 0);
	if (*success == false)
		WARNING("latex exit code = " << latexProcess.exitCode());

	QFile::remove(fi.completeBaseName() + ".aux");
	QFile::remove(fi.completeBaseName() + ".log");

	// convert: PDF -> PNG
	QProcess convertProcess;
#if defined(HAVE_WINDOWS)
	// need to set path to magick coder modules (which are in the labplot2 directory)
	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
	env.insert("MAGICK_CODER_MODULE_PATH", qPrintable(qgetenv("PROGRAMFILES") + QString("\\labplot2")));
	convertProcess.setProcessEnvironment(env);
#endif
	convertProcess.start("convert", QStringList() << "-density" << QString::number(dpi) + 'x' + QString::number(dpi)
							<< fi.completeBaseName() + ".pdf" << fi.completeBaseName() + ".png");
	if (!convertProcess.waitForFinished()) {
		kWarning() << "convert process failed." << endl;
		DEBUG("convert process failed!");
		*success = false;
		QFile::remove(fi.completeBaseName() + ".pdf");
		return QImage();
	}

	// read png file and clean up
	QImage image;
	image.load(fi.completeBaseName() + ".png");

	// final clean up
	QFile::remove(fi.completeBaseName() + ".png");
	QFile::remove(fi.completeBaseName() + ".pdf");

	return image;
}

// TEX -> DVI -> PS -> PNG
QImage TeXRenderer::imageFromDVI(const QTemporaryFile& file, const int dpi, bool* success) {
	QFileInfo fi(file.fileName());
	QProcess latexProcess;
	latexProcess.start("latex", QStringList() << "-interaction=batchmode" << file.fileName());
	if (!latexProcess.waitForFinished()) {
		kWarning() << "latex process failed." << endl;
		QFile::remove(fi.completeBaseName() + ".aux");
		QFile::remove(fi.completeBaseName() + ".log");
		return QImage();
	}

	*success = (latexProcess.exitCode() == 0);

	QFile::remove(fi.completeBaseName() + ".aux");
	QFile::remove(fi.completeBaseName() + ".log");

	// dvips: DVI -> PS
	QProcess dvipsProcess;
	dvipsProcess.start("dvips", QStringList() << "-E" << fi.completeBaseName());
	if (!dvipsProcess.waitForFinished()) {
		kWarning() << "dvips process failed." << endl;
		QFile::remove(fi.completeBaseName() + ".dvi");
		return QImage();
	}

	// convert: PS -> PNG
	QProcess convertProcess;
#if defined(HAVE_WINDOWS)
	// need to set path to magick coder modules (which are in the labplot2 directory)
	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
	env.insert("MAGICK_CODER_MODULE_PATH", qPrintable(qgetenv("PROGRAMFILES") + QString("\\labplot2")));
	convertProcess.setProcessEnvironment(env);
#endif
	convertProcess.start("convert", QStringList() << "-density" << QString::number(dpi) + 'x' + QString::number(dpi)
			<< fi.completeBaseName() + ".ps" << fi.completeBaseName() + ".png");
	if (!convertProcess.waitForFinished()) {
		kWarning() << "convert process failed." << endl;
		QFile::remove(fi.completeBaseName() + ".dvi");
		QFile::remove(fi.completeBaseName() + ".ps");
		return QImage();
	}

	// read png file
	QImage image;
	image.load(fi.completeBaseName() + ".png", "png");

	// final clean up
	QFile::remove(fi.completeBaseName() + ".png");
	QFile::remove(fi.completeBaseName() + ".dvi");
	QFile::remove(fi.completeBaseName() + ".ps");

	return image;
}

bool TeXRenderer::enabled() {
	KConfigGroup group = KGlobal::config()->group(QLatin1String("Settings_Worksheet"));
	QString engine = group.readEntry("LaTeXEngine", "pdflatex");
	if (engine.isEmpty() || !executableExists(engine)) {
		WARNING("LaTeX engine does not exist");
		return false;
	}

	//engine found, check the presence of other required tools (s.a. TeXRenderer.cpp):
	//to convert the generated PDF/PS files to PNG we need 'convert' from the ImageMagic package
	if (!executableExists(QLatin1String("convert"))) {
		WARNING("program \"convert\" does not exist");
		return false;
	}

	//to convert the generated PS files to DVI we need 'dvips'
	if (engine == "latex") {
		if (!executableExists(QLatin1String("dvips"))) {
			WARNING("program \"dvips\" does not exist");
			return false;
		}
	}

#if defined(_WIN64)
	if (!executableExists(QLatin1String("gswin64c")) && !QDir(qgetenv("PROGRAMFILES") + QString("/gs")).exists() 
		&& !QDir(qgetenv("PROGRAMFILES(X86)") + QString("/gs")).exists()) {
		WARNING("ghostscript (64bit) does not exist");
		return false;
	}
#elif defined(HAVE_WINDOWS)
	if (!executableExists(QLatin1String("gswin32c")) && !QDir(qgetenv("PROGRAMFILES") + QString("/gs")).exists()) {
		WARNING("ghostscript (32bit) does not exist");
		return false;
	}
#endif

	return true;
}

bool TeXRenderer::executableExists(const QString& exe) {
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
		return !QStandardPaths::findExecutable(exe).isEmpty();
#else
		return !KStandardDirs::findExe(exe).isEmpty();
#endif
}
