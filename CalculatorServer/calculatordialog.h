#ifndef CALCULATORDIALOG_H
#define CALCULATORDIALOG_H

#include "calculatorserver.h"
#include <QWidget>

class QTextEdit;

class CalculatorDialog : public QWidget
{
    Q_OBJECT

public:
    CalculatorDialog(QWidget *parent = 0);
    ~CalculatorDialog();

private slots:
    void showMessage(const QString &message);

private:
    CalculatorServer server;
    QTextEdit* txt;
};

#endif // CALCULATORDIALOG_H
