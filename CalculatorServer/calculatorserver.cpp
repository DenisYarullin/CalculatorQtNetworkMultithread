#include "calculatorserver.h"
#include "calculatorthread.h"

CalculatorServer::CalculatorServer(QObject *parent)
    : QTcpServer(parent) {}


void CalculatorServer::incomingConnection(qintptr socketDescriptor)
{
    CalculatorThread *thread = new CalculatorThread(socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, &CalculatorThread::message, this, &CalculatorServer::message);
    thread->start();
}
