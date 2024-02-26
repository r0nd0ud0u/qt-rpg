#include "stuffsview.h"
#include "ui_stuffsview.h"

#include "common.h"

StuffsView::StuffsView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StuffsView)
{
    ui->setupUi(this);
    InitEditStuffsView();
}

StuffsView::~StuffsView()
{
    delete ui;
}

void StuffsView::InitEditStuffsView() {

    for(const auto& stats : ALL_STATS){
        if(stats.isEmpty()){
            continue;
        }
        auto* stuff = new StuffPanel();
        stuff->Init(stats);
        if(stuff == nullptr){
            continue;
        }
        ui->scrollAreaWidgetContents->layout()->addWidget(stuff);
        m_StuffList.push_back(stuff);
    }
}
