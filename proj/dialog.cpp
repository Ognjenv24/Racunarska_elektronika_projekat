#include "dialog.h"
#include "ui_dialog.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>
#include <wiringPi.h>
#include <lcd.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>

// Declare global variables for the LCD setup
const int RS = 3;
const int EN = 14;
const int D0 = 4;
const int D1 = 12;
const int D2 = 13;
const int D3 = 6;
int lcd_h;

// Declare the sensor ID
const char sensor_id[] = "28-00000xxxxxxxx";  // Replace with your sensor ID

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    manager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);

    // Initialize wiringPi and the LCD
    if (wiringPiSetup() < 0) {
        fprintf(stderr, "Error initializing wiringPi: %s\n", strerror(errno));
    }
    
    lcd_h = lcdInit(2, 16, 4, RS, EN, D0, D1, D2, D3, D0, D1, D2, D3);
    if (lcd_h < 0) {
        fprintf(stderr, "Error initializing LCD: %s\n", strerror(errno));
    }

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
    timer->start(2000); // Refresh every 2 seconds

    refreshData();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::refreshData()
{
    // Read the temperature from the DS18B20 sensor
    double sensorTemp = readDS18B20Temperature(sensor_id);
    if (sensorTemp != -999.0) {
        updateTemperature(sensorTemp);
        
        // Send the temperature to the server
        QNetworkRequest request(QUrl("http://127.0.0.1:5000/update_temperature"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QJsonObject json;
        json["temperature"] = sensorTemp;
        QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());

        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            reply->deleteLater();
        });
    }

    // Request fan speed
    QNetworkRequest request(QUrl("http://127.0.0.1:5000/fan_speed"));
    QNetworkReply *reply = manager->get(request);
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


double Dialog::readDS18B20Temperature(const char* sensorID)
{
    char dev_name[100];
    char buffer[100];
    char tmp2[10];
    char *tmp1;
    const char devname_head[] = "/sys/devices/w1_bus_master1/";
    const char devname_end[] = "/w1_slave";
    int fd;
    long value;
    int integer, decimal;
    char ch = 't';
    
    // Construct the full device path
    strcpy(dev_name, devname_head);
    strcat(dev_name, sensorID);
    strcat(dev_name, devname_end);
    
    // Open the device file
    if ((fd = open(dev_name, O_RDONLY)) < 0) {
        perror("Error opening sensor device");
        return -999.0;  // Return an error value
    }
    
    // Read the device file
    int ret = read(fd, buffer, sizeof(buffer));
    if (ret < 0) {
        perror("Error reading sensor device");
        close(fd);
        return -999.0;
    }

    // Parse the temperature from the file
    tmp1 = strchr(buffer, ch);
    sscanf(tmp1, "t=%s", tmp2);
    value = atol(tmp2);  // Convert the temperature string to a number
    close(fd);

    // Calculate the integer and decimal parts
    integer = value / 1000;
    decimal = value % 1000;

    // Return the temperature as a floating-point value
    return integer + (decimal / 1000.0);
}

void Dialog::updateTemperature(double temp)
{
    ui->lineEditTemperature->setText(QString::number(temp));

    // Clear the LCD and display the current temperature
    lcdClear(lcd_h);
    lcdPosition(lcd_h, 0, 0); // Position at the first line
    lcdPrintf(lcd_h, "Temp: %.2f C", temp);
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
