#ifndef QRATZFEST_H
#define QRATZFEST_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>
#include <QProcess>
#include "comport/comport.h"

namespace Ui {
class Qratzfest;
}

class Qratzfest : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Qratzfest(QWidget *parent = 0);
    ~Qratzfest();
    
private slots:
    void on_bSelectScratchExe_clicked();
    
	void checkConnection();
	void checkComPort();
    void processDisconnected ();
    void processError ( QAbstractSocket::SocketError socketError );
    void processReadyRead ();


    void on_cbSendData_textChanged(const QString &arg1);
	void broadcast(const QStringList &arg1);

    void on_actionInfo_triggered();

    void on_eSendSerData_returnPressed();

    void on_actionLoad_profile_triggered();

    void on_actionSave_profile_triggered();

private:
    Ui::Qratzfest *ui;
    QTcpSocket *_scratchLink;
    QTimer _connectionCheck;
	QTimer _comPortCheck;
	QString _currentComport;
	QProcess *_scratchExe;
	COMPort *_comport;
};

#endif // QRATZFEST_H
