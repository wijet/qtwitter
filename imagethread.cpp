#include "imagethread.h"

QWaitCondition gwc;
QMutex gmutex;

ImageThread::ImageThread() : QThread() {
  connect( &imageDownload, SIGNAL( imageDownloaded( const QString&, const QImage& ) ), this, SLOT( saveImage( const QString&, const QImage& ) ));
//  connect( &http, SIGNAL( errorMessage( const QString& ) ), this, SLOT( popupError( const QString& ) ) );
//  connect( &imageDownload, SIGNAL( errorMessage( const QString& ) ), this, SLOT( popupError( const QString& ) ) );
//  connect( &http, SIGNAL( dataParsed( const QString& ) ), this, SLOT( updateText( const QString& ) ) );
  connect( &http, SIGNAL( newEntry( const Entry& ) ), this, SLOT( addEntry( const Entry& ) ));
  connect( &http, SIGNAL( xmlParsed() ), this, SLOT( downloadImages() ) );
//  connect( &imageDownload, SIGNAL( dataParsed( const QString& ) ), this, SLOT( updateText( const QString& ) ) );
  connect( &imageDownload, SIGNAL( imageDownloaded( const QString&, const QImage& ) ), this, SLOT( saveImage( const QString&, const QImage& ) ));  
}

void ImageThread::run() {
  qDebug() << "Will be downloading images now.";
  for ( int i = 0; i < entries.size(); i++ ) {
    qDebug() << i << "image: " << entries[i].image();
    if ( imagesHash.contains( entries[i].image() ) ) {
      qDebug() << "Oh Yes, it contains!";
      saveImage( entries[i].image(), imagesHash[ entries[i].image() ] );
    } else {
      qDebug() << "Noes, iz gonna download...";
      gmutex.lock();
      imageDownload.setUrl( entries[i].image() );
      gwc.wakeAll();
      gwc.wait( &gmutex );
      gmutex.unlock();
    }
  }
}

void ImageThread::addEntry( const Entry &entry )
{
  entries << entry;
}

void ImageThread::downloadImages() {
  imageDownload.count = entries.size();
  imageDownload.start();
  start();

  wait();
  qDebug() << "Now ready to display";
  emit readyToDisplay( entries, imagesHash );
}

void ImageThread::saveImage ( const QString &imageUrl, const QImage &image ) {
  imagesHash[ imageUrl ] = image;
  qDebug() << "setting imagesHash[" << imageUrl << "]";
  qDebug() << "Am I ever here?";
  //imageDownload.wc.wakeAll();
  //gettingImage.wakeAll();
  //mutex.unlock();
}
