/***************************************************************************
 *   Copyright (C) 2008-2009 by Dominik Kapusta       <d@ayoy.net>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "mainwindow.h"
#include "settings.h"
#include "core.h"
#include "loopedsignal.h"

//Q_IMPORT_PLUGIN(qjpeg)
//Q_IMPORT_PLUGIN(qgif)


int main( int argc, char **argv )
{
  QApplication app( argc, argv );
  qApp->setWindowIcon( QIcon( ":/icons/twitter_48.png" ) );

  MainWindow qtwitter;
  LoopedSignal *loopedsignal = new LoopedSignal( &qtwitter );
  Core *core = new Core( &qtwitter );
  Settings *settings = new Settings( &qtwitter, loopedsignal, core, &qtwitter );

  QObject::connect( loopedsignal, SIGNAL(ping()), &qtwitter, SLOT(updateTweets()) );
  QObject::connect( &qtwitter, SIGNAL(get()), core, SLOT(get()) );
  QObject::connect( &qtwitter, SIGNAL(openBrowser()), core, SLOT(openBrowser()) );
  QObject::connect( &qtwitter, SIGNAL(post(QByteArray)), core, SLOT(post(QByteArray)) );
  QObject::connect( &qtwitter, SIGNAL(settingsDialogRequested()), settings, SLOT( show() ) );
  QObject::connect( core, SIGNAL(authDataSet(QAuthenticator)), settings, SLOT(setAuthDataInDialog(QAuthenticator)) ) ;
  QObject::connect( core, SIGNAL(switchToPublic()), settings, SLOT(switchToPublic()) );
  QObject::connect( core, SIGNAL(errorMessage(QString)), &qtwitter, SLOT(popupError(QString)) );
  QObject::connect( core, SIGNAL(readyToDisplay(ListOfEntries,MapStringImage)), &qtwitter, SLOT(display(ListOfEntries,MapStringImage)) );
  QObject::connect( core, SIGNAL(updateNeeded()), &qtwitter, SLOT(updateTweets()) );

  qtwitter.show();

  if ( !loopedsignal->isRunning() )
    loopedsignal->start();

  return app.exec();
}
