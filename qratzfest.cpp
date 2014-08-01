#include "qratzfest.h"
#include "ui_qratzfest.h"

Qratzfest::Qratzfest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Qratzfest),
	_scratchLink(0),
	_scratchExe(0)
{
    ui->setupUi(this);
	
    _connectionCheck.setInterval(200);
	connect(&_connectionCheck, SIGNAL(timeout()), this, SLOT(checkConnection()) );
	_connectionCheck.start();
}

Qratzfest::~Qratzfest()
{
    delete ui;
}

void Qratzfest::on_bSelectScratchExe_clicked()
{

}

void Qratzfest::checkConnection()
{
	if ( !_scratchLink ) //not connected
	{
		_scratchLink = new QTcpSocket(this);
		_scratchLink->connectToHost( "localhost", 42001 );
		connect(_scratchLink, SIGNAL(readyRead()), this, SLOT(processReadyRead()) );
		connect(_scratchLink, SIGNAL(disconnected()), this, SLOT(processDisconnected()) );
		connect(_scratchLink, SIGNAL(error()), this, SLOT(processError ( QAbstractSocket::SocketError ) ) );
		_connectionCheck.setInterval(4000);
	}
}

void Qratzfest::processDisconnected ()
{
	_scratchLink->deleteLater();
	_scratchLink=0;
}

void Qratzfest::processError ( QAbstractSocket::SocketError socketError )
{
	_scratchLink->deleteLater();
	_scratchLink=0;
}

void Qratzfest::processReadyRead ()
{
	if ( _scratchLink )
	{
		while ( _scratchLink->bytesAvailable() )
		{
			QByteArray mlen = _scratchLink->read(4);
			qint64 imlen=0;
			imlen = mlen[0]<<12 | mlen[1]<<8 | mlen[2]<<4 | mlen[3];
			QByteArray data = _scratchLink->read(imlen);
			ui->tLog->appendPlainText( data );
		}
		
		
		
	}
}

