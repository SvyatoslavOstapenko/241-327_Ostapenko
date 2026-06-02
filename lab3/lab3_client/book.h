#ifndef BOOK_H
#define BOOK_H

#include <QString>
#include <QJsonObject>

class BoardGame
{
public:
    int id = 0;
    QString title;
    QString publisher;
    QString releaseDate;
    int playersCount = 0;
    int playTimeMinutes = 0;
    double price = 0.0;
    bool isCooperative = false;
    QString description;
    QString createdAt;

    BoardGame();

    static BoardGame fromJson(const QJsonObject &json);
    QJsonObject toJson() const;
    QString toText() const;
};

#endif // BOOK_H