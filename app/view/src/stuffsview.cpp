#include "stuffsview.h"
#include "ui_stuffsview.h"

StuffsView::StuffsView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StuffsView)
{
    ui->setupUi(this);
}

StuffsView::~StuffsView()
{
    delete ui;
}

void StuffsView::AddStuff(QString name){

    StuffPanel* panel = new StuffPanel();
    panel->AddName(name);
    m_StuffList.push_back(panel);
    ui->verticalLayout->addWidget(panel);

}
