#include "editatkpanel.h"
#include "ui_editatkpanel.h"

EditAtkPanel::EditAtkPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EditAtkPanel)
{
    ui->setupUi(this);
}

EditAtkPanel::~EditAtkPanel()
{
    delete ui;
}
