#include "effectview.h"
#include "ui_effectview.h"

//#include "common.h"

EffectView::EffectView(QWidget *parent)
    : QWidget(parent), ui(new Ui::EffectView) {
  ui->setupUi(this);
}

EffectView::~EffectView() { delete ui; }

// void EffectView::InitComboBoxes() {
//   // init only one the combo boxes
//   if (m_FirstShow) {
//     return;
//   }
//   m_FirstShow = true;

//   // disconnect signals combo boxes
//   DisconnectComboSig();

//   for (const auto &stat : ALL_STATS) {
//       ui->stats_comboBox->addItem(stat);
//   }
//   for (const auto &target : EFFECTS) {
//     ui->effect_comboBox->addItem(target);
//   }
//   for (const auto &target : ALL_TARGETS) {
//       ui->target_comboBox->addItem(target);
//   }
//   for (const auto &reach : ALL_REACH) {
//       ui->reach_comboBox->addItem(reach);
//   }

//   // Re-activate them
//   // effect
//   ReconnectComboSig();
// }

void EffectView::SetVectorSize(const size_t size){
    m_EffectTable.clear();
    m_EffectTable.resize(size);
}
void EffectView::SetIndex(const int index) {
    m_Index = index;
}

// void EffectView::on_effect_comboBox_currentTextChanged(const QString &arg1) {
//   m_EffectTable[m_Index][0].effect = arg1;
//   m_EffectTable[m_Index][0].updated = true;
//   emit SigTableUpdated();
// }

// void EffectView::on_value_spinBox_valueChanged(int arg1) {
//   m_EffectTable[m_Index][0].value = arg1;
//   m_EffectTable[m_Index][0].updated = true;
//   emit SigTableUpdated();
// }

const std::vector<effectParam> &EffectView::GetTable() const { return m_EffectTable[m_Index]; }

// void EffectView::on_checkBox_stateChanged(int arg1) {
//   if (arg1 == 2) {
//         //1
//     ui->effect_comboBox->setEnabled(true);
//     ui->value_spinBox->setEnabled(true);
//     ui->value_spinBox->setEnabled(true);
//     ui->nb_turns_spinBox->setEnabled(true);
//     ui->target_comboBox->setEnabled(true);
//     ui->reach_comboBox->setEnabled(true);
//     ui->stats_comboBox->setEnabled(true);
//     ui->effect_value_spinbox->setEnabled(true);
//   } else {
//     // checkbox1
//     ui->effect_comboBox->setEnabled(false);
//     ui->value_spinBox->setEnabled(false);
//     ui->effect_comboBox->setCurrentIndex(0);
//     ui->nb_turns_spinBox->setEnabled(false);
//     ui->target_comboBox->setEnabled(false);
//     ui->reach_comboBox->setEnabled(false);
//     ui->stats_comboBox->setEnabled(false);
//     ui->effect_value_spinbox->setEnabled(false);
//     ui->stats_comboBox->setCurrentIndex(0);
//     ui->target_comboBox->setCurrentIndex(0);
//     ui->reach_comboBox->setCurrentIndex(0);
//     for (auto& it : m_EffectTable) {
//         if(m_Index < it.size()){
//             it[m_Index] = effectParam();
//         }
//     }
//   }
//   emit SigTableUpdated();
// }

void EffectView::InitValues(const std::vector<effectParam>& table){
    //update effect list,
    m_EffectTable[m_Index] = table;
    InitEditAtkPanels();
}

// void EffectView::on_reach_comboBox_currentTextChanged(const QString &arg1)
// {
//     m_EffectTable[m_Index][0].reach = arg1;
//     m_EffectTable[m_Index][0].updated = true;
//     emit SigTableUpdated();
// }


// void EffectView::on_stats_comboBox_currentTextChanged(const QString &arg1)
// {
//     m_EffectTable[m_Index][0].statsName = arg1;
//     m_EffectTable[m_Index][0].updated = true;
//     emit SigTableUpdated();
// }

// void EffectView::on_target_comboBox_currentTextChanged(const QString &arg1)
// {
//     m_EffectTable[m_Index][0].target = arg1;
//     m_EffectTable[m_Index][0].updated = true;
//     emit SigTableUpdated();
// }

// void EffectView::on_nb_turns_spinBox_valueChanged(int arg1)
// {
//     m_EffectTable[m_Index][0].nbTurns = arg1;
//     m_EffectTable[m_Index][0].updated = true;
//     emit SigTableUpdated();
// }

// void EffectView::on_effect_value_spinbox_valueChanged(int arg1)
// {
//     m_EffectTable[m_Index][0].subValueEffect = arg1;
//     m_EffectTable[m_Index][0].updated = true;
//     emit SigTableUpdated();
// }

void EffectView::InitEditAtkPanels() {
    auto* layout = ui->main_widget->layout();
    if(layout == nullptr){
        return;
    }
    // for(int i = 0; i< ui->main_widget->layout()->count(); i++){
    //     auto *widget = layout->itemAt(i)->widget();
    //     widget->hide();
    //     layout->removeItem(layout->itemAt(i));
    //     layout->removeWidget(widget);
    //     delete widget;
    // }
    while (auto item = layout->takeAt(0)) {
        delete item->widget();
    }
    m_AtkPanels.clear();
    for (const auto &it : m_EffectTable[m_Index]) {
        AddEditAtkPanel(it);
    }
}

void EffectView::AddEditAtkPanel(const effectParam& ep) {
    auto *panel = new EditAtkPanel();
    panel->UpdatePanel(ep);
    ui->main_widget->layout()->addWidget(panel);
    m_AtkPanels.push_back(panel);
    connect(panel, &EditAtkPanel::SigValueChanged, this,
            &EffectView::SlotValueChange);
}

void EffectView::SlotValueChange(){
    std::vector<effectParam> tmp;
    for(const auto& panel : m_AtkPanels){
        tmp.push_back(panel->GetEffectParam());
    }
    m_EffectTable[m_Index] = tmp;
    emit SigTableUpdated();
}
