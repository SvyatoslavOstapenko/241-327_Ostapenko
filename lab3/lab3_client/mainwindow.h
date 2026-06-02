#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QByteArray>
#include <QString>

class QLineEdit;
class QSpinBox;
class QDoubleSpinBox;
class QCheckBox;
class QTextEdit;

#include "book.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QLineEdit *idEdit;
    QLineEdit *titleEdit;
    QLineEdit *publisherEdit;
    QLineEdit *releaseDateEdit;
    QSpinBox *playersCountSpin;
    QSpinBox *playTimeSpin;
    QDoubleSpinBox *priceSpin;
    QCheckBox *cooperativeCheck;
    QTextEdit *descriptionEdit;
    QTextEdit *outputEdit;

    BoardGame gameFromInputs() const;
    void showResponse(
        const QString &action,
        int statusCode,
        int errorCode,
        const QString &errorString,
        const QByteArray &response
    );

private slots:
    void onListClicked();
    void onGetClicked();
    void onPostClicked();
    void onPatchClicked();
    void onDeleteClicked();
};

#endif // MAINWINDOW_H