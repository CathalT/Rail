#pragma once

#include "ui_RaiLight.h"

#include <QtWidgets\QMainWindow>

#include <memory>

namespace rail::control
{
    class ICore;
}

class QStringListModel;

class RaiLight : public QMainWindow
{
    Q_OBJECT

public:
    RaiLight(rail::control::ICore *_coreController);
    ~RaiLight();

public slots:
    void goToMainWindow();
    void goToFreshStartup();
    void goToLockScreen();
    void goToNewPasswordScreen();

    void deleteChildren();


private:
    Ui::RaiLightClass ui;

    rail::control::ICore *coreController;
};