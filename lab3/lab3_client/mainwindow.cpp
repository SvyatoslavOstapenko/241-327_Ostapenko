#include "mainwindow.h"

#include <QCheckBox>
#include <QDebug>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include "httpclient.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("ЛР3 Qt REST-клиент — настольные игры");
    resize(900, 700);

    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    QLabel *titleLabel = new QLabel("Лабораторная работа №3. Qt клиент для REST API настольных игр", this);
    mainLayout->addWidget(titleLabel);

    QFormLayout *formLayout = new QFormLayout();

    idEdit = new QLineEdit(this);
    idEdit->setPlaceholderText("ID для GET, PATCH, DELETE");

    titleEdit = new QLineEdit(this);
    titleEdit->setText("Новая настольная игра");

    publisherEdit = new QLineEdit(this);
    publisherEdit->setText("ООО Издатель");

    releaseDateEdit = new QLineEdit(this);
    releaseDateEdit->setText("2026-06-02");

    playersCountSpin = new QSpinBox(this);
    playersCountSpin->setRange(1, 100);
    playersCountSpin->setValue(4);

    playTimeSpin = new QSpinBox(this);
    playTimeSpin->setRange(1, 10000);
    playTimeSpin->setValue(60);

    priceSpin = new QDoubleSpinBox(this);
    priceSpin->setRange(0, 1000000);
    priceSpin->setDecimals(2);
    priceSpin->setValue(1500.00);

    cooperativeCheck = new QCheckBox("Да", this);

    descriptionEdit = new QTextEdit(this);
    descriptionEdit->setPlainText("Описание настольной игры");

    formLayout->addRow("ID:", idEdit);
    formLayout->addRow("Название:", titleEdit);
    formLayout->addRow("Издатель:", publisherEdit);
    formLayout->addRow("Дата выпуска:", releaseDateEdit);
    formLayout->addRow("Количество игроков:", playersCountSpin);
    formLayout->addRow("Время игры, минут:", playTimeSpin);
    formLayout->addRow("Цена:", priceSpin);
    formLayout->addRow("Кооперативная:", cooperativeCheck);
    formLayout->addRow("Описание:", descriptionEdit);

    mainLayout->addLayout(formLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *listButton = new QPushButton("LIST(список)", this);
    QPushButton *getButton = new QPushButton("GET(получить)", this);
    QPushButton *postButton = new QPushButton("POST(добавить)", this);
    QPushButton *patchButton = new QPushButton("PATCH(изменить)", this);
    QPushButton *deleteButton = new QPushButton("DELETE(удалить)", this);

    buttonLayout->addWidget(listButton);
    buttonLayout->addWidget(getButton);
    buttonLayout->addWidget(postButton);
    buttonLayout->addWidget(patchButton);
    buttonLayout->addWidget(deleteButton);

    mainLayout->addLayout(buttonLayout);

    outputEdit = new QTextEdit(this);
    outputEdit->setReadOnly(true);
    outputEdit->setPlaceholderText("Здесь будет ответ сервера");
    mainLayout->addWidget(outputEdit);

    setCentralWidget(central);

    connect(listButton, &QPushButton::clicked, this, &MainWindow::onListClicked);
    connect(getButton, &QPushButton::clicked, this, &MainWindow::onGetClicked);
    connect(postButton, &QPushButton::clicked, this, &MainWindow::onPostClicked);
    connect(patchButton, &QPushButton::clicked, this, &MainWindow::onPatchClicked);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteClicked);

    connect(
        &HttpClient::instance(),
        &HttpClient::responseReady,
        this,
        &MainWindow::showResponse
    );
}

BoardGame MainWindow::gameFromInputs() const
{
    BoardGame game;

    game.title = titleEdit->text();
    game.publisher = publisherEdit->text();
    game.releaseDate = releaseDateEdit->text();
    game.playersCount = playersCountSpin->value();
    game.playTimeMinutes = playTimeSpin->value();
    game.price = priceSpin->value();
    game.isCooperative = cooperativeCheck->isChecked();
    game.description = descriptionEdit->toPlainText();

    return game;
}

void MainWindow::onListClicked()
{
    outputEdit->setPlainText("Отправлен запрос LIST(список)...");
    HttpClient::instance().listGames();
}

void MainWindow::onGetClicked()
{
    int id = idEdit->text().toInt();
    outputEdit->setPlainText("Отправлен запрос GET(получить)...");
    HttpClient::instance().getGame(id);
}

void MainWindow::onPostClicked()
{
    outputEdit->setPlainText("Отправлен запрос POST(добавить)...");
    HttpClient::instance().createGame(gameFromInputs());
}

void MainWindow::onPatchClicked()
{
    int id = idEdit->text().toInt();
    outputEdit->setPlainText("Отправлен запрос PATCH(изменить)...");
    HttpClient::instance().updateGame(id, gameFromInputs());
}

void MainWindow::onDeleteClicked()
{
    int id = idEdit->text().toInt();
    outputEdit->setPlainText("Отправлен запрос DELETE(удалить)...");
    HttpClient::instance().deleteGame(id);
}

void MainWindow::showResponse(
    const QString &action,
    int statusCode,
    int errorCode,
    const QString &errorString,
    const QByteArray &response
)
{
    QString result;

    result += "Метод: " + action + "\n";
    result += "HTTP статус: " + QString::number(statusCode) + "\n";
    result += "Код ошибки Qt: " + QString::number(errorCode) + "\n";

    if (errorCode != 0) {
        result += "Ошибка: " + errorString + "\n";
    }

    result += "-----------------------------\n";

    QJsonParseError parseError;
    QJsonDocument document = QJsonDocument::fromJson(response, &parseError);

    if (response.isEmpty()) {
        result += "Ответ сервера пустой.";
    } else if (parseError.error != QJsonParseError::NoError) {
        result += QString::fromUtf8(response);
    } else if (document.isArray()) {
        QJsonArray array = document.array();

        result += "Получено объектов: " + QString::number(array.size()) + "\n";
        result += "-----------------------------\n";

        for (const QJsonValue &value : array) {
            if (value.isObject()) {
                BoardGame game = BoardGame::fromJson(value.toObject());
                result += game.toText();
                result += "-----------------------------\n";
            }
        }
    } else if (document.isObject()) {
        QJsonObject object = document.object();

        if (object.contains("results") && object.value("results").isArray()) {
            QJsonArray array = object.value("results").toArray();

            result += "Получено объектов: " + QString::number(array.size()) + "\n";
            result += "-----------------------------\n";

            for (const QJsonValue &value : array) {
                if (value.isObject()) {
                    BoardGame game = BoardGame::fromJson(value.toObject());
                    result += game.toText();
                    result += "-----------------------------\n";
                }
            }
        } else if (object.contains("id") || object.contains("title")) {
            BoardGame game = BoardGame::fromJson(object);
            result += game.toText();
        } else {
            result += QString::fromUtf8(document.toJson(QJsonDocument::Indented));
        }
    }

    outputEdit->setPlainText(result);
    qDebug().noquote() << result;
}