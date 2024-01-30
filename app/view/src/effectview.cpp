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

  ui->effect_comboBox->setEnabled(true);
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
}

void EffectView::on_effect_comboBox_2_currentTextChanged(const QString &arg1) {
    m_EffectTable[1].effect = arg1;
}

void EffectView::on_effect_comboBox_3_currentTextChanged(const QString &arg1) {
    m_EffectTable[2].effect = arg1;
}

void EffectView::on_value_spinBox_valueChanged(int arg1) {
    m_EffectTable[0].value = arg1;
}

void EffectView::on_value_spinBox_2_valueChanged(int arg1) {
    m_EffectTable[1].value = arg1;
}

void EffectView::on_value_spinBox_3_valueChanged(int arg1) {
    m_EffectTable[2].value = arg1;
}

const std::vector<effectParam>& EffectView::GetTable(){
    return m_EffectTable;
}
