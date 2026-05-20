#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

// Класс модели данных из ЛР1/ЛР2
class BoardGame
{
public:
    int id = 0;
    QString title;
    QString publisher;
    QString releaseDate;
    int playersCount = 0;
    int playTimeMinutes = 0;
    QString price;
    bool isCooperative = false;
    QString description;
    QString createdAt;

    void print() const
    {
        qDebug() << "-----------------------------";
        qDebug() << "ID:" << id;
        qDebug() << "Название:" << title;
        qDebug() << "Издатель:" << publisher;
        qDebug() << "Дата выпуска:" << releaseDate;
        qDebug() << "Количество игроков:" << playersCount;
        qDebug() << "Время игры:" << playTimeMinutes;
        qDebug() << "Цена:" << price;
        qDebug() << "Кооперативная:" << isCooperative;
        qDebug() << "Описание:" << description;
        qDebug() << "Создано:" << createdAt;
    }
};

// Паттерн Адаптер: преобразует JSON в C++ объект BoardGame
class BoardGameJsonAdapter
{
public:
    static BoardGame fromJsonObject(const QJsonObject& obj)
    {
        BoardGame game;

        game.id = obj.value("id").toInt();
        game.title = obj.value("title").toString();
        game.publisher = obj.value("publisher").toString();
        game.releaseDate = obj.value("release_date").toString();
        game.playersCount = obj.value("players_count").toInt();
        game.playTimeMinutes = obj.value("play_time_minutes").toInt();
        game.price = obj.value("price").toString();
        game.isCooperative = obj.value("is_cooperative").toBool();
        game.description = obj.value("description").toString();
        game.createdAt = obj.value("created_at").toString();

        return game;
    }
};

// Паттерн Синглтон: один общий API-клиент на всё приложение
class ApiClient : public QObject
{
public:
    static ApiClient& instance()
    {
        static ApiClient client;
        return client;
    }

    void getBoardGames()
    {
        QUrl url("http://localhost/api/boardgame");
        QNetworkRequest request(url);

        QNetworkReply* reply = manager.get(request);

        connect(reply, &QNetworkReply::finished, this, [reply]() {
            QByteArray response = reply->readAll();

            if (reply->error() != QNetworkReply::NoError) {
                qDebug() << "Ошибка запроса:" << reply->errorString();
                reply->deleteLater();
                QCoreApplication::quit();
                return;
            }

            QJsonParseError parseError;
            QJsonDocument document = QJsonDocument::fromJson(response, &parseError);

            if (parseError.error != QJsonParseError::NoError) {
                qDebug() << "Ошибка парсинга JSON:" << parseError.errorString();
                reply->deleteLater();
                QCoreApplication::quit();
                return;
            }

            if (!document.isArray()) {
                qDebug() << "Ошибка: сервер вернул не JSON-массив";
                reply->deleteLater();
                QCoreApplication::quit();
                return;
            }

            QJsonArray array = document.array();

            qDebug() << "Получено объектов:" << array.size();

            int count = 0;

            for (const QJsonValue& value : array) {
                if (!value.isObject()) {
                    continue;
                }

                BoardGame game = BoardGameJsonAdapter::fromJsonObject(value.toObject());
                game.print();

                count++;

                if (count >= 5) {
                    break;
                }
            }

            qDebug() << "Выведены первые 5 объектов из API";

            reply->deleteLater();
            QCoreApplication::quit();
        });
    }

private:
    ApiClient() = default;

    QNetworkAccessManager manager;
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "ЛР3: Qt-клиент запущен";
    qDebug() << "Отправляем запрос к серверу ЛР2...";

    ApiClient::instance().getBoardGames();

    return app.exec();
}