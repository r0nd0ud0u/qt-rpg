#include "stuffpanel.h"
#include "ui_stuffpanel.h"

StuffPanel::StuffPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StuffPanel)
{
    ui->setupUi(this);
}

StuffPanel::~StuffPanel()
{
    delete ui;
}

void StuffPanel::AddName(QString name){
    ui->stuff_name->setText(name);
}
