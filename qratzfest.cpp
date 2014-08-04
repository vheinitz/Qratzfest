#include "qratzfest.h"
#include "ui_qratzfest.h"

Qratzfest::Qratzfest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Qratzfest),
	_scratchLink(0),
	_scratchExe(0),
	_comport(0)
{
    ui->setupUi(this);
	
	connect(&_connectionCheck, SIGNAL(timeout()), this, SLOT(checkConnection()) );
	connect(&_comPortCheck, SIGNAL(timeout()), this, SLOT(checkComPort()) );

    _connectionCheck.setInterval(200);
	_comPortCheck.setInterval(100);
	
	
	
	_connectionCheck.start();
	//_comport = new COMPort();
	TSerialInterfaceInfoList coms = COMPort::availableInterfaces();
	for (TSerialInterfaceInfoList::const_iterator it = coms.begin(), end = coms.end(); it!=end; ++it)
	{
		ui->cbComPort->addItem( QString( it->first.c_str() ) );
	}
	_comPortCheck.start();
}

Qratzfest::~Qratzfest()
{
    delete ui;
}

void Qratzfest::on_bSelectScratchExe_clicked()
{

}

void Qratzfest::checkComPort()
{
	if (!_comport )
	{
		if ( !ui->cbComPort->currentText().isEmpty() )
		{
			_comport = new COMPort(/*QString("\\\\.\\%1").arg(ui->cbComPort->currentText()).toStdString().c_str()*/);
			if( !_comport->open( QString("\\\\.\\%1").arg(ui->cbComPort->currentText()).toStdString().c_str() ) )
			{
				delete _comport;
				_comport = 0;
				_currentComport.clear();
				return;
			}
			_currentComport = ui->cbComPort->currentText();
		}
	}
	else
	{
		if ( _currentComport != ui->cbComPort->currentText()  )
		{
			delete _comport;
			_comport = 0;
			_currentComport.clear();
			return;
		}
		else
		{
			/*
			bool dcd = ms.DCD;
			bool cts = ms.CTS;
			bool ri = ms.RI;
			bool dtr = ms.DTR;
			bool rts = ms.RTS;
			*/
			QStringList vars;
			if (_comport->DCD())
				vars <<"IN1_ON";
			else
				vars <<"IN1_OFF";

			broadcast( vars );
			vars.clear();

			if (_comport->DSR())
				vars <<"IN2_ON";
			else
				vars <<"IN2_OFF";

			broadcast( vars );
			vars.clear();

			if (_comport->CTS())
				vars <<"IN3_ON";
			else
				vars <<"IN3_OFF";

			broadcast( vars );
			vars.clear();

			if (_comport->RI())
				vars <<"IN4_ON";
			else
				vars <<"IN4_OFF";

			broadcast( vars );

		}

	}
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
			QString data = _scratchLink->read(imlen);
			ui->tLog->appendPlainText( data );
			if( _comport )
			{
				QStringList vars = data.split(" \"");
				foreach (QString v, vars)
				{
					QString k = v.section("\"",0,0);
					QString val = v.section(" ",1);
					
					if (k == "out1")
						_comport->setBreak(val.toInt());
					else if (k == "out2")
						_comport->setDTR(val.toInt());
					else if (k == "out3")
						_comport->setRTS(val.toInt());
				}
			}
		}
	}
}


void Qratzfest::on_cbSendData_textChanged(const QString &arg1)
{
    if ( _scratchLink )
    {
        QString sendData = ui->cbSendData->currentText();
        QByteArray msg;
		msg.resize(4);
        msg[0]=0;
        msg[1]=0;
        msg[2]=0;
        msg[3]=sendData.size();
        msg.append( sendData );
        _scratchLink->write(msg);
    }
}

void Qratzfest::broadcast(const QStringList &vars)
{
	QString data = "broadcast";
    if ( _scratchLink )
    {
        foreach (QString v, vars)
		{
			data+=QString(" \"%1\"").arg(v);
		}
		QString sendData = data;
        QByteArray msg;
		msg.resize(4);
        msg[0]=0;
        msg[1]=0;
        msg[2]=0;
        msg[3]=sendData.size();
        msg.append( sendData );
        _scratchLink->write(msg);
    }
}

