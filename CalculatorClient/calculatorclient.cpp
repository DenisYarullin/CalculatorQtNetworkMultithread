#include <QLCDNumber>
#include <QPushButton>
#include <QTextEdit>
#include <QGridLayout>
#include <QTime>
#include <QComboBox>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include "calculatorclient.h"
#include "networksettings.h"

CalculatorClient::CalculatorClient(QWidget *parent)
    : QWidget(parent), nextBlockSize(0), settings(new SettingsDialog(this)), tcpSocket(new QTcpSocket(this))
{
    connect(settings, &SettingsDialog::connectClicked, this, &CalculatorClient::slotTryConnect);
    connect(tcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(tcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,      SLOT(slotError(QAbstractSocket::SocketError)));


    QPushButton* btnSettings = new QPushButton("Network settings");
    connect(btnSettings, SIGNAL(clicked()), SLOT(slotOpenSettings()));


    plcd = new QLCDNumber(12);
    plcd->setSegmentStyle(QLCDNumber::Flat);
    plcd->setMinimumSize(300, 100);

    lblExpression = new QLabel;
    lblExpression->setAlignment(Qt::AlignRight);

    QChar buttons[4][4] = {{'7', '8', '9', '/'},
                           {'4', '5', '6', '*'},
                           {'1', '2', '3', '-'},
                           {'0', '.', '=', '+'}
                          };

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(btnSettings, 0, 0, 1, 1);
    layout->addWidget(plcd, 1, 0, 1, 4);
    layout->addWidget(lblExpression, 2, 0, 1, 3);
    layout->addWidget(createButton("CE"), 2, 3);

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            layout->addWidget(createButton(buttons[i][j]), i + 3, j);

    txtInfo = new QTextEdit;
    txtInfo->setReadOnly(true);
    layout->addWidget(txtInfo, 7, 0, 2, 4);

    setLayout(layout);


}

CalculatorClient::~CalculatorClient()
{

}

QPushButton *CalculatorClient::createButton(const QString &str)
{
    QPushButton* button = new QPushButton(str);
    button->setMinimumSize(80, 80);
    connect(button, &QPushButton::clicked, this, &CalculatorClient::slotButtonClicked);
    return button;
}

void CalculatorClient::slotButtonClicked()
{
    QString str = ((QPushButton*)sender())->text();

    if (str == "CE")
    {
        strDisplay = "";
        plcd->display("0");
        lblExpression->setText("");
        return;
    }
    else if (!strDisplay.isEmpty() && !strDisplay[strDisplay.size() - 1].isDigit() && !str[0].isDigit())
        QMessageBox::warning(this, tr("Warning"), tr("Invalid input!"));
    else
    {
        if (!strDisplay.isEmpty() && strDisplay[strDisplay.size() - 1].isDigit() && str == "=")
        {
            slotSendToServer();
        }
        else
        {
            strDisplay += str;
            lblExpression->setText(tr("<font size=20>%1</font>").arg(strDisplay));
        }
    }
}

void CalculatorClient::slotReadyRead()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_3);
    for (;;) {
        if (!nextBlockSize) {
            if (tcpSocket->bytesAvailable() < (int)sizeof(quint16)) {
                break;
            }
            in >> nextBlockSize;
        }

        if (tcpSocket->bytesAvailable() < nextBlockSize) {
            break;
        }
        QTime   time;
        QString str;
        in >> time >> str;

        int pos = 0;
        QRegExp rxOperands("(\\d+)\\.?(\\d+)?");
        if ((pos = rxOperands.indexIn(str, pos)) != -1)
        {
            plcd->display(rxOperands.cap(0));
        }

        txtInfo->append(time.toString() + " " + str);
        nextBlockSize = 0;
    }
}

void CalculatorClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(tcpSocket->errorString())
                    );
    txtInfo->append(strError);
    settings->setDialogMessage(strError);
}

void CalculatorClient::slotSendToServer()
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    if (!tcpSocket->isOpen())
    {
        txtInfo->append("Connection not found!");
    }
    else
    {
        out << quint16(0) << QTime::currentTime() << strDisplay;
        out.device()->seek(0);
        out << quint16(arrBlock.size() - sizeof(quint16));

        tcpSocket->write(arrBlock);
    }

}

void CalculatorClient::slotConnected()
{
    txtInfo->append("Received the connected() signal");
    settings->setDialogMessage("Received the connected() signal");
}

void CalculatorClient::slotOpenSettings()
{
    settings->show();
}

void CalculatorClient::slotTryConnect(QString& host, int port)
{
    tcpSocket->abort();
    tcpSocket->connectToHost(host, port);
}


//void Client::readFortune()
//{
//    in.startTransaction();

//    QString nextFortune;
//    in >> nextFortune;

//    if (!in.commitTransaction())
//        return;

//    if (nextFortune == currentFortune) {
//        QTimer::singleShot(0, this, &Client::requestNewFortune);
//        return;
//    }

//    currentFortune = nextFortune;
//    statusLabel->setText(currentFortune);
//    getFortuneButton->setEnabled(true);
//}
