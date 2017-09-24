#include "calculatorclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CalculatorClient calculator;
    calculator.setWindowTitle("Calculator");
    calculator.resize(460, 400);
    calculator.show();

    return a.exec();
}
