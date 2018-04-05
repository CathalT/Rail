#pragma once

#include "ui_NewPasswordScreen.h"

#include <QWidget>

namespace rail::control
{
    class ICore;
}

class NewPasswordScreen : public QWidget
{
    Q_OBJECT
public:
    NewPasswordScreen(rail::control::ICore* _coreController, const QString& _seed);

signals:
    void onPasswordMatch(const QString);


private slots:
    void onConfirmClicked();
    void onLineEditChanged(int, int);

private:
    Ui::NewPassswordScreen ui;

    const QString seed;

    rail::control::ICore* coreController;
};
