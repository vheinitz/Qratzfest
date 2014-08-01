#ifndef QRATZFEST_H
#define QRATZFEST_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>
#include <QProcess>

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
    void processDisconnected ();
    void processError ( QAbstractSocket::SocketError socketError );
    void processReadyRead ();


private:
    Ui::Qratzfest *ui;
    QTcpSocket *_scratchLink;
    QTimer _connectionCheck;
	QProcess *_scratchExe;
};

#endif // QRATZFEST_H
