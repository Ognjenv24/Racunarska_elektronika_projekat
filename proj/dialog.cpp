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
