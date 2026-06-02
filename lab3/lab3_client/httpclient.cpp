#include "httpclient.h"

#include <QJsonDocument>
#include <QNetworkRequest>
#include <QUrl>

HttpClient& HttpClient::instance()
{
    static HttpClient client;
    return client;
}

HttpClient::HttpClient(QObject *parent)
    : QObject(parent)
{
}

QNetworkRequest HttpClient::makeRequest(const QString &url) const
{
    QNetworkRequest request{QUrl(url)};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    return request;
}

void HttpClient::handleReply(QNetworkReply *reply, const QString &action)
{
    connect(reply, &QNetworkReply::finished, this, [this, reply, action]() {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        int errorCode = static_cast<int>(reply->error());
        QString errorString = reply->errorString();
        QByteArray response = reply->readAll();

        emit responseReady(action, statusCode, errorCode, errorString, response);

        reply->deleteLater();
    });
}

void HttpClient::listGames()
{
    QNetworkRequest request = makeRequest(baseUrl);
    QNetworkReply *reply = manager.get(request);
    handleReply(reply, "LIST(список)");
}

void HttpClient::getGame(int id)
{
    QNetworkRequest request = makeRequest(baseUrl + "/" + QString::number(id));
    QNetworkReply *reply = manager.get(request);
    handleReply(reply, "GET(получить)");
}

void HttpClient::createGame(const BoardGame &game)
{
    QJsonDocument document(game.toJson());
    QByteArray body = document.toJson(QJsonDocument::Compact);

    QNetworkRequest request = makeRequest(baseUrl);
    QNetworkReply *reply = manager.post(request, body);
    handleReply(reply, "POST(добавить)");
}

void HttpClient::updateGame(int id, const BoardGame &game)
{
    QJsonDocument document(game.toJson());
    QByteArray body = document.toJson(QJsonDocument::Compact);

    QNetworkRequest request = makeRequest(baseUrl + "/" + QString::number(id));
    QNetworkReply *reply = manager.sendCustomRequest(request, "PATCH", body);
    handleReply(reply, "PATCH(изменить)");
}

void HttpClient::deleteGame(int id)
{
    QNetworkRequest request = makeRequest(baseUrl + "/" + QString::number(id));
    QNetworkReply *reply = manager.deleteResource(request);
    handleReply(reply, "DELETE(удалить)");
}