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

void StuffPanel::Init(const QString &name) {
    m_Name = name;
    ui->stat_label->setText(name);
}
