#include "attakview.h"
#include "ui_attakview.h"

#include <QPushButton>

AttakView::AttakView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AttakView)
{
    ui->setupUi(this);
    AddAttak();
}

AttakView::~AttakView()
{
    delete ui;
}

void AttakView::AddAttak(){
    for (int i = 0; i < 12; i++) {
        auto* button = new QPushButton();
        ui->attak_lay->addWidget(button);

    }
}
