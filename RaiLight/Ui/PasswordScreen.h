#pragma once

#include "ui_PasswordScreen.h"

#include <QWidget>

namespace rail::control
{
    class ICore;
}

class PasswordScreen : public QWidget
{
    Q_OBJECT
public:
    PasswordScreen(rail::control::ICore *_coreController);

public slots:
    void onPasswordEntered();
   
signals:
    void backButtonClicked();
    void passwordCorrect();
    void passwordWrong();

private slots:
    void wrongPasswordEntered();

private:
    Ui::PasswordScreen ui;

    rail::control::ICore *coreController;
};