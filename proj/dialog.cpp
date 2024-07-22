#include "dialog.h"
#include "ui_dialog.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    manager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);

    // Connect buttons to their functions
    connect(ui->buttonIncreaseTemp, &QPushButton::clicked, this, [this]() { adjustTemperature(1); });
    connect(ui->buttonDecreaseTemp, &QPushButton::clicked, this, [this]() { adjustTemperature(-1); });
    connect(ui->buttonFanLow, &QPushButton::clicked, this, [this]() { setFanSpeed("low"); });
    connect(ui->buttonFanMedium, &QPushButton::clicked, this, [this]() { setFanSpeed("medium"); });
    connect(ui->buttonFanHigh, &QPushButton::clicked, this, [this]() { setFanSpeed("high"); });
    connect(ui->buttonToggleAC, &QPushButton::clicked, this, [this]() { toggleAC(); });

    // Timer to refresh temperature and fan speed
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Dialog::refreshData);
    timer->start(2000); // Refresh every 5 seconds

    refreshData();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::refreshData()
{
    // Request temperature
    QNetworkRequest request(QUrl("http://127.0.0.1:5000/temperature"));
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject obj = doc.object();
        updateTemperature(obj["current_temperature"].toDouble());
        reply->deleteLater();
    });

    // Request fan speed
    request.setUrl(QUrl("http://127.0.0.1:5000/fan_speed"));
    reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject obj = doc.object();
        updateFanSpeed(obj["fan_speed"].toString());
        reply->deleteLater();
    });

    // Request AC status
    request.setUrl(QUrl("http://127.0.0.1:5000/ac_status"));
    reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject obj = doc.object();
        QString status = obj["ac_status"].toString();
        updateACStatus(status);
        reply->deleteLater();
    });
}

void Dialog::updateTemperature(double temp)
{
    ui->lineEditTemperature->setText(QString::number(temp));
}

void Dialog::updateFanSpeed(const QString &speed)
{
    ui->lineEditFanSpeed->setText(speed);
}

void Dialog::updateACStatus(const QString &status)
{
    ui->labelACStatus->setText(status);
}

void Dialog::adjustTemperature(int amount)
{
    bool ok;
    double currentTemp = ui->lineEditTemperature->text().toDouble(&ok);

    if (!ok) {
        // Handle invalid temperature value
        QMessageBox::warning(this, "Invalid Temperature", "Current temperature is not valid.");
        return;
    }

    double newTemp = currentTemp + amount;

    // Restrict the temperature range to 16°C - 32°C
    if (newTemp < 16) {
        newTemp = 16;
        QMessageBox::warning(this, "Temperature Adjustment", "Temperature cannot go below 16°C.");
    }
    if (newTemp > 32) {
        newTemp = 32;
        QMessageBox::warning(this, "Temperature Adjustment", "Temperature cannot go above 32°C.");
    }

    // Update the temperature in the GUI
    ui->lineEditTemperature->setText(QString::number(newTemp));

    // Update the temperature on the server
    QNetworkRequest request(QUrl("http://127.0.0.1:5000/update_temperature"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json["temperature"] = newTemp;
    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        refreshData();
    });
}


void Dialog::setFanSpeed(const QString &speed)
{
    QNetworkRequest request(QUrl("http://127.0.0.1:5000/set_fan_speed"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json["fan_speed"] = speed;
    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        refreshData();
    });
}

void Dialog::toggleAC()
{
    // Determine current status from the label
    QString currentStatus = ui->labelACStatus->text();
    QString newStatus = (currentStatus == "On") ? "Off" : "On";

    // Update the UI immediately
    updateACStatus(newStatus);

    // Send the status change to the server
    QNetworkRequest request(QUrl("http://127.0.0.1:5000/toggle_ac"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json["ac_status"] = newStatus;
    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        // Optionally refresh data to confirm server status
        // refreshData();
    });
}
