#include "effectview.h"
#include "ui_effectview.h"

#include "common.h"

EffectView::EffectView(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::EffectView)
{
  ui->setupUi(this);
  m_EffectTable.resize(3);
}

EffectView::~EffectView()
{
  delete ui;
}

void
EffectView::InitComboBoxes()
{
  // init only one the combo boxes
  if (m_FirstShow) {
    return;
  }
  m_FirstShow = true;

  // disconnect signals combo boxes
  disconnect(
    ui->effect_comboBox, &QComboBox::currentTextChanged, nullptr, nullptr);
  disconnect(
    ui->effect_comboBox_2, &QComboBox::currentTextChanged, nullptr, nullptr);
  disconnect(
    ui->effect_comboBox_3, &QComboBox::currentTextChanged, nullptr, nullptr);

  //  ui->effect_comboBox->setEnabled(true);
  //  ui->effect_comboBox_2->setEnabled(false);
  //  ui->effect_comboBox_3->setEnabled(false);
  for (const auto& target : EFFECTS) {
    ui->effect_comboBox->addItem(target);
    ui->effect_comboBox_2->addItem(target);
    ui->effect_comboBox_3->addItem(target);
  }

  // Re-activate them
  connect(ui->effect_comboBox,
          &QComboBox::currentTextChanged,
          this,
          &EffectView::on_effect_comboBox_currentTextChanged);
  connect(ui->effect_comboBox_2,
          &QComboBox::currentTextChanged,
          this,
          &EffectView::on_effect_comboBox_2_currentTextChanged);
  connect(ui->effect_comboBox_3,
          &QComboBox::currentTextChanged,
          this,
          &EffectView::on_effect_comboBox_3_currentTextChanged);
}

void
EffectView::on_effect_comboBox_currentTextChanged(const QString& arg1)
{
  m_EffectTable[0].effect = arg1;
}

void
EffectView::on_effect_comboBox_2_currentTextChanged(const QString& arg1)
{
  m_EffectTable[1].effect = arg1;
}

void
EffectView::on_effect_comboBox_3_currentTextChanged(const QString& arg1)
{
  m_EffectTable[2].effect = arg1;
}

void
EffectView::on_value_spinBox_valueChanged(int arg1)
{
  m_EffectTable[0].value = arg1;
}

void
EffectView::on_value_spinBox_2_valueChanged(int arg1)
{
  m_EffectTable[1].value = arg1;
}

void
EffectView::on_value_spinBox_3_valueChanged(int arg1)
{
  m_EffectTable[2].value = arg1;
}

const std::vector<effectParam>&
EffectView::GetTable()
{
  return m_EffectTable;
}

void
EffectView::on_checkBox_stateChanged(int arg1)
{
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
  }
}

void
EffectView::on_checkBox_2_stateChanged(int arg1)
{
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
  }
}

void
EffectView::on_checkBox_3_stateChanged(int arg1)
{
    if (arg1 == 2) {
  ui->effect_comboBox_3->setEnabled(true);
        ui->value_spinBox_3->setEnabled(true);}
  else {
      ui->effect_comboBox_3->setEnabled(false);
      ui->value_spinBox_3->setEnabled(false);
      ui->effect_comboBox_3->setCurrentIndex(0);
      ui->effect_comboBox_3->setEnabled(false);
  }
}
