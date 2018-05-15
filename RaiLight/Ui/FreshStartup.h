#pragma once
#include "ui_FreshStartup.h"
#include <QWidget>

namespace rail::control
{
    class ICore;
}

class FreshStartup : public QWidget
{
    Q_OBJECT
public:
    FreshStartup(rail::control::ICore *_coreController);
signals:
    void createNewSeedClicked();
    void restoreSeedClicked();

private:
    Ui::FreshStartup ui;

    rail::control::ICore *coreController;
};