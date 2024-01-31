#include "effectview.h"
#include "ui_effectview.h"

#include "common.h"

EffectView::EffectView(QWidget *parent)
    : QWidget(parent), ui(new Ui::EffectView) {
  ui->setupUi(this);
  m_EffectTable.resize(3);
}

EffectView::~EffectView() { delete ui; }

void EffectView::InitComboBoxes() {
  // init only one the combo boxes
  if (m_FirstShow) {
    return;
  }
  m_FirstShow = true;

  // disconnect signals combo boxes
  disconnect(ui->effect_comboBox, &QComboBox::currentTextChanged, nullptr,
             nullptr);
  disconnect(ui->effect_comboBox_2, &QComboBox::currentTextChanged, nullptr,
             nullptr);
  disconnect(ui->effect_comboBox_3, &QComboBox::currentTextChanged, nullptr,
             nullptr);

  //  ui->effect_comboBox->setEnabled(true);
  //  ui->effect_comboBox_2->setEnabled(false);
  //  ui->effect_comboBox_3->setEnabled(false);
  for (const auto &target : EFFECTS) {
    ui->effect_comboBox->addItem(target);
    ui->effect_comboBox_2->addItem(target);
    ui->effect_comboBox_3->addItem(target);
  }

  // Re-activate them
  connect(ui->effect_comboBox, &QComboBox::currentTextChanged, this,
          &EffectView::on_effect_comboBox_currentTextChanged);
  connect(ui->effect_comboBox_2, &QComboBox::currentTextChanged, this,
          &EffectView::on_effect_comboBox_2_currentTextChanged);
  connect(ui->effect_comboBox_3, &QComboBox::currentTextChanged, this,
          &EffectView::on_effect_comboBox_3_currentTextChanged);
}

void EffectView::on_effect_comboBox_currentTextChanged(const QString &arg1) {
  m_EffectTable[0].effect = arg1;
  m_EffectTable[0].updated = true;
}

void EffectView::on_effect_comboBox_2_currentTextChanged(const QString &arg1) {
  m_EffectTable[1].effect = arg1;
  m_EffectTable[1].updated = true;
}

void EffectView::on_effect_comboBox_3_currentTextChanged(const QString &arg1) {
  m_EffectTable[2].effect = arg1;
  m_EffectTable[2].updated = true;
}

void EffectView::on_value_spinBox_valueChanged(int arg1) {
  m_EffectTable[0].value = arg1;
  m_EffectTable[0].updated = true;
}

void EffectView::on_value_spinBox_2_valueChanged(int arg1) {
  m_EffectTable[1].value = arg1;
  m_EffectTable[1].updated = true;
}

void EffectView::on_value_spinBox_3_valueChanged(int arg1) {
  m_EffectTable[2].value = arg1;
  m_EffectTable[2].updated = true;
}

const std::vector<effectParam> &EffectView::GetTable() { return m_EffectTable; }

void EffectView::on_checkBox_stateChanged(int arg1) {
  if (arg1 == 2) {
    ui->effect_comboBox->setEnabled(true);
    ui->value_spinBox->setEnabled(true);
    ui->checkBox_2->setEnabled(true);
  } else {
    // checkbox1
    ui->effect_comboBox->setEnabled(false);
    ui->value_spinBox->setEnabled(false);
    ui->effect_comboBox->setCurrentIndex(0);
    // checkbox2
    ui->effect_comboBox_2->setCurrentIndex(0);
    ui->effect_comboBox_2->setEnabled(false);
    ui->value_spinBox_2->setEnabled(false);
    ui->checkBox_2->setEnabled(false);
    // checkbox3
    ui->checkBox_3->setEnabled(false);
    ui->effect_comboBox_3->setCurrentIndex(0);
    ui->effect_comboBox_3->setEnabled(false);
    ui->value_spinBox_3->setEnabled(false);
    for (auto& it : m_EffectTable) {
        it = effectParam();
    }
  }
}

void EffectView::on_checkBox_2_stateChanged(int arg1) {
  if (arg1 == 2) {
    ui->effect_comboBox_2->setEnabled(true);
    ui->value_spinBox_2->setEnabled(true);
    ui->checkBox_3->setEnabled(true);
  } else {
    // checkbox2
    ui->effect_comboBox_2->setCurrentIndex(0);
    ui->effect_comboBox_2->setEnabled(false);
    ui->value_spinBox_2->setEnabled(false);
    ui->checkBox_2->setEnabled(false);
    // checkbox3
    ui->checkBox_3->setEnabled(false);
    ui->effect_comboBox_3->setCurrentIndex(0);
    ui->effect_comboBox_3->setEnabled(false);
    ui->value_spinBox_3->setEnabled(false);
    // effect table
    m_EffectTable[1] = effectParam();
    m_EffectTable[2] = effectParam();
  }
}

void EffectView::on_checkBox_3_stateChanged(int arg1) {
  if (arg1 == 2) {
    ui->effect_comboBox_3->setEnabled(true);
    ui->value_spinBox_3->setEnabled(true);
  } else {
    ui->effect_comboBox_3->setEnabled(false);
    ui->value_spinBox_3->setEnabled(false);
    ui->effect_comboBox_3->setCurrentIndex(0);
    ui->effect_comboBox_3->setEnabled(false);
    m_EffectTable[2] = effectParam();
  }
}

void EffectView::SetValues(const std::vector<effectParam>& table){
    if(!table.empty() && !table[0].effect.isEmpty()){
        ui->effect_comboBox->setCurrentText(table[0].effect);
        ui->value_spinBox->setValue(table[0].value);
        ui->checkBox->setCheckState(Qt::CheckState::Checked);
    } else{
        ui->effect_comboBox->setCurrentText("");
        ui->value_spinBox->setValue(0);
        ui->checkBox->setCheckState(Qt::CheckState::Unchecked);
        return;
    }
    if(table.size() > 1 && !table[1].effect.isEmpty()){
        ui->effect_comboBox_2->setCurrentText(table[1].effect);
        ui->value_spinBox_2->setValue(table[1].value);
        ui->checkBox_2->setCheckState(Qt::CheckState::Checked);
    }else{
        ui->effect_comboBox_2->setCurrentText("");
        ui->value_spinBox_2->setValue(0);
        ui->checkBox_2->setCheckState(Qt::CheckState::Unchecked);
        return;
    }
    if(table.size() > 2 && !table[2].effect.isEmpty()){
        ui->effect_comboBox_3->setCurrentText(table[1].effect);
        ui->value_spinBox_3->setValue(table[1].value);
        ui->checkBox_3->setCheckState(Qt::CheckState::Checked);
    }else{
        ui->effect_comboBox_3->setCurrentText("");
        ui->value_spinBox_3->setValue(0);
        ui->checkBox_3->setCheckState(Qt::CheckState::Unchecked);
        return;
    }

    //update effect list,
    m_EffectTable = table;
}
