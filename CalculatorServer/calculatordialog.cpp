#include "calculatordialog.h"
#include <QMessageBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLabel>

CalculatorDialog::CalculatorDialog(QWidget *parent)
    : QWidget(parent)
{
    if (!server.listen(QHostAddress::Any, 2323))
    {
        QMessageBox::critical(0, "Server Error", "Unable to start the server:" + server.errorString());
        server.close();
        return;
    }

    connect(&server, &CalculatorServer::message, this, &CalculatorDialog::showMessage);

    txt = new QTextEdit;
    txt->setReadOnly(true);

    QVBoxLayout* vbxLayout = new QVBoxLayout;
    vbxLayout->addWidget(new QLabel("<H1>Server</H1>"));
    vbxLayout->addWidget(txt);
    setLayout(vbxLayout);
}

CalculatorDialog::~CalculatorDialog()
{

}

void CalculatorDialog::showMessage(const QString &message)
{
    txt->append(message);
}

