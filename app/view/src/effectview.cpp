#include "effectview.h"
#include "ui_effectview.h"

EffectView::EffectView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EffectView)
{
    ui->setupUi(this);
}

EffectView::~EffectView()
{
    delete ui;
}
