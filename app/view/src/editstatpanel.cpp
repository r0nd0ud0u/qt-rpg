#include "editstatpanel.h"
#include "ui_editstatpanel.h"

EditStatPanel::EditStatPanel(QWidget *parent)
    : QWidget(parent), ui(new Ui::EditStatPanel) {
  ui->setupUi(this);
}

EditStatPanel::~EditStatPanel() { delete ui; }

void EditStatPanel::Init(const QString &name, const Stats &stats) {
  ui->stats_name_lbl->setText(name);
}

void EditStatPanel::on_max_spinBox_valueChanged(int arg1) { m_MaxValue = arg1; }

void EditStatPanel::SetStatsValue(const StatsType &statsType){
    ui->max_spinBox->setValue(statsType.m_RawMaxValue);
}
