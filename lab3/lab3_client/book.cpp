#include "book.h"

#include <QVariant>

BoardGame::BoardGame()
{
}

BoardGame BoardGame::fromJson(const QJsonObject &json)
{
    BoardGame game;

    game.id = json.value("id").toInt();
    game.title = json.value("title").toString();
    game.publisher = json.value("publisher").toString();
    game.releaseDate = json.value("release_date").toString();
    game.playersCount = json.value("players_count").toInt();
    game.playTimeMinutes = json.value("play_time_minutes").toInt();
    game.price = json.value("price").toVariant().toDouble();
    game.isCooperative = json.value("is_cooperative").toBool();
    game.description = json.value("description").toString();
    game.createdAt = json.value("created_at").toString();

    return game;
}

QJsonObject BoardGame::toJson() const
{
    QJsonObject json;

    json["title"] = title;
    json["publisher"] = publisher;
    json["release_date"] = releaseDate;
    json["players_count"] = playersCount;
    json["play_time_minutes"] = playTimeMinutes;
    json["price"] = price;
    json["is_cooperative"] = isCooperative;
    json["description"] = description;

    return json;
}

QString BoardGame::toText() const
{
    return QString(
        "ID: %1\n"
        "Название: %2\n"
        "Издатель: %3\n"
        "Дата выпуска: %4\n"
        "Количество игроков: %5\n"
        "Время игры: %6 минут\n"
        "Цена: %7\n"
        "Кооперативная: %8\n"
        "Описание: %9\n"
        "Создано: %10\n"
    )
        .arg(id)
        .arg(title)
        .arg(publisher)
        .arg(releaseDate)
        .arg(playersCount)
        .arg(playTimeMinutes)
        .arg(price)
        .arg(isCooperative ? "да" : "нет")
        .arg(description)
        .arg(createdAt);
}