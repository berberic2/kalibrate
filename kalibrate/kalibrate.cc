/*
 * This file is part of the source of kalibrate, a gui for the 
 * openCV-camera-calibration
 * 
 * Copyright (C) 2010 M G Berberich
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string>
#include <iostream>
#include <exception>

#include <QTreeWidgetItem>

#include <KAboutData>
#include <KCmdLineArgs>
#include <KApplication>
#include <KMessageBox>
#include <KLocale>

#include "../config.h"

#include "gui.h"

int main(int argc, char **argv)
{
  std::vector<std::string> rrds;
  KAboutData about("kalibrate", "kalibrate", 
	ki18n("Kalibrate"), VERSION, 
	ki18n("Camera-calibration using OpenCV"),
 	KAboutData::License_GPL,
	ki18n("© 2010 M G Berberich"),
	ki18n("Maintainer and developer"),
	"http://www.forwiss.uni-passau.de/~berberic/Linux/kalibrate.html",
	"berberic@fmi.uni-passau.de");
  about.addAuthor(ki18n("M G Berberich"), KLocalizedString(),
	"M G Berberich <berberic@fmi.uni-passau.de>", 
	"http://www.forwiss.uni-passau.de/~berberic");
  about.setTranslator(ki18nc("NAME OF TRANSLATORS", "Your names"),
	ki18nc("EMAIL OF TRANSLATORS", "Your emails"));
  KCmdLineArgs::init(argc, argv, &about);

  KCmdLineOptions options;
  options.add("+[file]", ki18n("A kalibrate-project-file to open"));
  KCmdLineArgs::addCmdLineOptions( options );

  KApplication application;
  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();  
  if (application.isSessionRestored()) {
    kRestoreMainWindows<KalibrateGui>();
  } else {      
    KalibrateGui *gui = new KalibrateGui;
    // handling arguments
    //    if(args->count() == 1) gui->load(args->arg(0));
    gui->setObjectName("kalibrate");
    gui->show();
  }

  return application.exec();
}
