#ifndef CALCULATORTHREAD_H
#define CALCULATORTHREAD_H

#include <QThread>
#include <QTcpSocket>

class CalculatorThread : public QThread
{
    Q_OBJECT

private:
    quint16 nextBlockSize;
    int socketDescriptor;
    QString text;
    QTcpSocket* tcpSocket;

private:
    void sendToClient(const QString& str);

public:
    CalculatorThread(int socketDescriptor, QObject *parent = 0);
    ~CalculatorThread();
    double calculate(const QString& str);
    void run() override;

signals:
    void error(QTcpSocket::SocketError socketError);
    void message(const QString& str);

public slots:
    void slotDisconnect();
    void slotReadClient();
};

#endif // CALCULATORTHREAD_H
