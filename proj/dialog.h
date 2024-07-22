#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void refreshData();
    void adjustTemperature(int amount);
    void setFanSpeed(const QString &speed);
    void toggleAC();

private:
    Ui::Dialog *ui;
    QNetworkAccessManager *manager;
    void updateTemperature(double temp);
    void updateFanSpeed(const QString &speed);
    void updateACStatus(const QString &status);
};

#endif // DIALOG_H
