#include "hostpage.h"
#include "ui_hostpage.h"

HostPage::HostPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HostPage)
{
    ui->setupUi(this);
}

HostPage::~HostPage()
{
    delete ui;
}

void HostPage::on_pushButton_clicked()
{
    emit showGameDisplay();
}

