#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QString>

#include "book.h"

class HttpClient : public QObject
{
    Q_OBJECT

public:
    static HttpClient& instance();

    void listGames();
    void getGame(int id);
    void createGame(const BoardGame &game);
    void updateGame(int id, const BoardGame &game);
    void deleteGame(int id);

signals:
    void responseReady(
        const QString &action,
        int statusCode,
        int errorCode,
        const QString &errorString,
        const QByteArray &response
    );

private:
    explicit HttpClient(QObject *parent = nullptr);

    HttpClient(const HttpClient&) = delete;
    HttpClient& operator=(const HttpClient&) = delete;

    QNetworkAccessManager manager;
    QString baseUrl = "http://localhost/api/boardgame";

    QNetworkRequest makeRequest(const QString &url) const;
    void handleReply(QNetworkReply *reply, const QString &action);
};

#endif // HTTPCLIENT_H