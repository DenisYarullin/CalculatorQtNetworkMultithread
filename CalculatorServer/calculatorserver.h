#ifndef CALCULATORSERVER_H
#define CALCULATORSERVER_H

#include <QTcpServer>

class CalculatorServer : public QTcpServer
{
    Q_OBJECT

public:
    CalculatorServer(QObject *parent = 0);

signals:
    void message(const QString& str);

protected:
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif // CALCULATORSERVER_H
