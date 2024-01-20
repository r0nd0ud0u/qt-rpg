#include "dotview.h"
#include "ui_dotview.h"

DotView::DotView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DotView)
{
    ui->setupUi(this);
}

DotView::~DotView()
{
    delete ui;
}
