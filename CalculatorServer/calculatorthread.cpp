#include "calculatorthread.h"

#include <QTime>
#include <QDataStream>

CalculatorThread::CalculatorThread(int socketDescriptor, QObject *parent)
    : QThread(parent), nextBlockSize(0), socketDescriptor(socketDescriptor) {}


CalculatorThread::~CalculatorThread()
{
}


void CalculatorThread::slotDisconnect()
{
    tcpSocket->close();
    tcpSocket->deleteLater();
    emit message("Client disconnected!");
    quit();
}


void CalculatorThread::run()
{
    tcpSocket = new QTcpSocket;
    if (!tcpSocket->setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket->error());
        return;
    }

    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnect()), Qt::DirectConnection);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()), Qt::DirectConnection);
    sendToClient("Server Response: Connected!");

    exec();
}

double CalculatorThread::calculate(const QString &str)
{
    int pos = 0;
    QStringList listOperands;
    QRegExp rxOperands("(\\d+)\\.?(\\d+)?");
    while ((pos = rxOperands.indexIn(str, pos)) != -1)
    {
        listOperands << rxOperands.cap(0);
        pos += rxOperands.matchedLength();
    }

    pos = 0;
    QStringList listOperators;
    QRegExp rxOperators("[\\+\\-\\/\\*]{1}");
    while ((pos = rxOperators.indexIn(str, pos)) != -1)
    {
        listOperators << rxOperators.cap(0);
        pos += rxOperators.matchedLength();
    }

    QString operator_;
    double operand = 0.0, result = 0.0;

    if (!listOperands.empty())
    {
        result = listOperands.front().toDouble();
        listOperands.pop_front();

        while (!listOperands.empty())
        {
            if (!listOperators.empty())
            {
                operator_ = listOperators.front();
                listOperators.pop_front();

                operand = listOperands.front().toDouble();
                listOperands.pop_front();

                if (operator_ == "+")
                    result += operand;
                else if (operator_ == "-")
                     result -= operand;
                else if (operator_ == "*")
                     result *= operand;
                else
                     result /= operand;
            }
        }
    }
    return result;
}


void CalculatorThread::slotReadClient()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_3);
    for (;;)
    {
        if (!nextBlockSize)
        {
            if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
            {
                break;
            }
            in >> nextBlockSize;
        }

        if (tcpSocket->bytesAvailable() < nextBlockSize)
        {
            break;
        }
        QTime   time;
        QString str;
        in >> time >> str;

        QString strMessage = time.toString() + " " + "Client has sent - " + str;
        emit message(strMessage);

        nextBlockSize = 0;

        double result = calculate(str);
        emit message("Last expression is equal " +  QString().setNum(result));

        sendToClient("Server Response: Received \"" + QString().setNum(result )+ "\"");
    }
}


 void CalculatorThread::sendToClient(const QString& str)
 {
     QByteArray  arrBlock;
     QDataStream out(&arrBlock, QIODevice::WriteOnly);
     out.setVersion(QDataStream::Qt_5_3);
     out << quint16(0) << QTime::currentTime() << str;

     out.device()->seek(0);
     out << quint16(arrBlock.size() - sizeof(quint16));

     tcpSocket->write(arrBlock);
 }


//connect(tcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
 //tcpSocket.disconnectFromHost();
  //tcpSocket.waitForDisconnected();

// void Server::sendToClient()
// {
// //! [5]
//     QByteArray block;
//     QDataStream out(&block, QIODevice::WriteOnly);
//     out.setVersion(QDataStream::Qt_4_0);

//     out << fortunes.at(qrand() % fortunes.size());
// //! [4] //! [7]

//     QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
//     connect(clientConnection, &QAbstractSocket::disconnected,
//             clientConnection, &QObject::deleteLater);
// //! [7] //! [8]

//     clientConnection->write(block);
//     clientConnection->disconnectFromHost();
// //! [5]
// }
// //! [8]
