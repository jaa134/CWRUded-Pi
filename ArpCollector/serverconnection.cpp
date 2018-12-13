#include "serverconnection.h"

ServerConnection::ServerConnection(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);

    connect(manager, &QNetworkAccessManager::finished, this, &ServerConnection::onReply);
}

void ServerConnection::send(QString uri, QString location_name, Sniffer::State state) {
    QVariantMap mapData;
    mapData.insert("location_name", location_name);
    mapData.insert("extent", state.numConnections);
    QJsonDocument jsonData = QJsonDocument::fromVariant(mapData);

    QNetworkRequest request;
    request.setUrl(uri);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));

    manager->put(request, jsonData.toJson());
    qDebug(logInfo()) << "Sent JSON: " + jsonData.toJson();
    emit sent();
}

void ServerConnection::onReply(QNetworkReply* reply) {
    qDebug() << reply->readAll();
    emit sent();
}
