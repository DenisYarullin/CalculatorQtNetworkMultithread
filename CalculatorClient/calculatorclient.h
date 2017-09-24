#ifndef CALCULATORCLIENT_H
#define CALCULATORCLIENT_H

#include <QWidget>
#include <QStack>
#include <QTcpSocket>

class QLCDNumber;
class QPushButton;
class QComboBox;
class QLineEdit;
class QTextEdit;
class SettingsDialog;
class QLabel;

class CalculatorClient : public QWidget
{
    Q_OBJECT

private:
    QLCDNumber* plcd;
    QLabel* lblExpression;
    QStack<QString> stk;
    QString strDisplay;

    QTextEdit*  txtInfo;

    QTcpSocket* tcpSocket;
    quint16 nextBlockSize;

    SettingsDialog* settings;

public:
    CalculatorClient(QWidget *parent = 0);
    ~CalculatorClient();
    QPushButton *createButton(const QString& str);

public slots:
    void slotButtonClicked();

private slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotSendToServer();
    void slotConnected();
    void slotOpenSettings();
    void slotTryConnect(QString &, int);
};

#endif // CALCULATORCLIENT_H
