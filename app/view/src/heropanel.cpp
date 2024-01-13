#include "heropanel.h"
#include "ui_heropanel.h"

HeroPanel::HeroPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HeroPanel)
{
    ui->setupUi(this);
}

HeroPanel::~HeroPanel()
{
    delete ui;
}
