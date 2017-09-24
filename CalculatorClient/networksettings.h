#ifndef NETWORKSETTINGS_H
#define NETWORKSETTINGS_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = Q_NULLPTR);
    void setDialogMessage(const QString&);

signals:
    void connectClicked(QString&, int);

private slots:
    void connectToServer();

private:
    QComboBox *hostCombo;
    QLineEdit *portLineEdit;
    QLabel *statusLabel;
    QPushButton *connectButton;

    QString currentFortune;
};


#endif // NETWORKSETTINGS_H
