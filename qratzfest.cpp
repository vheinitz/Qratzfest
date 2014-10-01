#include "qratzfest.h"
#include "ui_qratzfest.h"
#include <QMessageBox>
#include <persistence.h>

static const char * Version = "0.3";

Qratzfest::Qratzfest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Qratzfest),
	_scratchLink(0),
	_scratchExe(0),
	_comport(0)
{
    ui->setupUi(this);

	PERSISTENCE_INIT("Valentin Heinitz", "Qratzfest");
	//PERSISTENT("NameOut1", ui->eNameOut1, "text");

	
	connect(&_connectionCheck, SIGNAL(timeout()), this, SLOT(checkConnection()) );
	connect(&_comPortCheck, SIGNAL(timeout()), this, SLOT(checkComPort()) );

    _connectionCheck.setInterval(500);
	_comPortCheck.setInterval(500);
	
	
	
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
		if ( !ui->cbComPort->currentText().isEmpty() && (ui->cbComPort->currentText().section("COM",1).toInt() > 4) )
		{
			_comport = new COMPort(/*QString("\\\\.\\%1").arg(ui->cbComPort->currentText()).toStdString().c_str()*/);
			if( !_comport->open( QString("\\\\.\\%1").arg(ui->cbComPort->currentText()).toStdString().c_str() ) )
			{
				delete _comport;
				_comport = 0;
				_currentComport.clear();
				return;
			}
			_comport->setBitRate(9600);
		    _comport->setParity(COMPort::None); 
		    _comport->setStopBits(COMPort::sb1); 
		    _comport->setDataBits(COMPort::db8); 
			_comport->setxONxOFF(false);
		    _comport->setNonBlockingMode( ); 
			_comport->setHandshaking(false);
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
			QStringList vars;
			char buf[100]={0};
			int br=0; // bytes read
			char *ch=buf;
			QString dataFromSer;

			ch=buf;
			*ch='\0';
			do
			{
				br = _comport->read( ch, 1 ); // Only to keep buffer small
				++ch;
			}while( br!=0 );
			*ch='\0';

			dataFromSer = buf;
			dataFromSer = dataFromSer.simplified();
			if( !dataFromSer.isEmpty() )
			{
				ui->tLog->appendPlainText(dataFromSer);
			}

			vars.clear();

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
		connect(_scratchLink, SIGNAL(error( QAbstractSocket::SocketError)), this, SLOT(processError ( QAbstractSocket::SocketError ) ) );
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
			
			QStringList vars = data.split(" \"");
			foreach (QString v, vars)
			{
				QString k = v.section("\"",0,0);
				QString val = v.section(" ",1).simplified();
				
				const char *wcmd = "1\n";
				 TODO check values for key in table and send corresponding message
				
				if ( k == "M1" )
				{
					if( _comport )
					{
						if (val == "1")					
						{					
							_comport->write('1');
						}
						_comport->write('\n');
					}
				}				
				*/

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
		ui->tLog->appendPlainText( QString(">>> %1").arg(data) );
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

void Qratzfest::on_actionInfo_triggered()
{
	QMessageBox msg;
	msg.setText(tr("<h1>Qratzfest</h1><br/>Hardware-proxy for MIT Scratch. Version %1<br/><small>Copyright: Valentin Heinitz, vheinitz@googlemail.com<br/>License: MIT-License</small>").arg(Version)); 
	msg.exec();
}

void Qratzfest::on_eSendSerData_returnPressed()
{

	QByteArray data = ui->eSendSerData->text().toAscii();
	if( _comport )
	{
		_comport->write( data, data.size());
	}
}

void Qratzfest::on_actionLoad_profile_triggered()
{

}

void Qratzfest::on_actionSave_profile_triggered()
{

}
