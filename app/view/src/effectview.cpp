#include "effectview.h"
#include "ui_effectview.h"

EffectView::EffectView(QWidget *parent)
    : QWidget(parent), ui(new Ui::EffectView) {
  ui->setupUi(this);
}

EffectView::~EffectView() {
  m_EffectTable.clear();
  for (auto *it : m_AtkPanels) {
    delete it;
    it = nullptr;
  }
  m_AtkPanels.clear();
  delete ui;
}

void EffectView::SetVectorSize(const size_t size) {
  m_EffectTable.clear();
  m_EffectTable.resize(size);
}
void EffectView::SetIndex(const int index) { m_Index = index; }

const std::vector<effectParam> &EffectView::GetTable() const {
  return m_EffectTable[m_Index];
}

void EffectView::InitValues(const std::vector<effectParam> &table) {
  // update effect list,
  m_EffectTable[m_Index] = table;
  InitEditAtkPanels();
}

void EffectView::InitEditAtkPanels() {
  auto *layout = ui->main_widget->layout();
  if (layout == nullptr) {
    return;
  }
  while (auto item = layout->takeAt(0)) {
    delete item->widget();
  }
  m_AtkPanels.clear();
  for (const auto &it : m_EffectTable[m_Index]) {
    AddEditAtkPanel(it);
  }
}

void EffectView::AddEditAtkPanel(const effectParam &ep) {
  auto *panel = new EditAtkPanel();
  panel->UpdatePanel(ep);
  ui->main_widget->layout()->addWidget(panel);
  m_AtkPanels.push_back(panel);
  connect(panel, &EditAtkPanel::SigValueChanged, this,
          &EffectView::SlotValueChange);
  connect(panel, &EditAtkPanel::SigRemove, this, &EffectView::SlotRemove);
}

void EffectView::SlotValueChange() {
  std::vector<effectParam> tmp;
  for (const auto &panel : m_AtkPanels) {
    tmp.push_back(panel->GetEffectParam());
  }
  m_EffectTable[m_Index] = tmp;
  emit SigTableUpdated();
}

void EffectView::on_add_effect_Button_clicked() {
  effectParam ep;
  // add a new panel
  AddEditAtkPanel(ep);
  // update the effect table
  m_EffectTable[m_Index].push_back(ep);
}

void EffectView::SlotRemove(EditAtkPanel *panel) { RemovePanel(panel); }

void EffectView::RemovePanel(EditAtkPanel *panel) {
  if (panel == nullptr) {
    return;
  }
  auto *lay = ui->main_widget->layout();

  int removeIndex = 0;
  for (const auto *it : m_AtkPanels) {
    if (it == panel) {
      break;
    }
    removeIndex++;
  }

  const auto newEnd = std::remove_if(
      m_AtkPanels.begin(), m_AtkPanels.end(), [&panel](const EditAtkPanel *p) {
        if (p == nullptr) {
          return false;
        }
        return p == panel; // remove elements where this is true
      });
  m_AtkPanels.erase(newEnd, m_AtkPanels.end());
  auto *widget = lay->itemAt(removeIndex)->widget();
  widget->hide();
  lay->removeItem(lay->itemAt(removeIndex));
  lay->removeWidget(widget);
  delete widget;

  // m_AtkPanels and m_EffectTable[m_Index] are built in the same order
  // => same index
  std::vector<effectParam> tmp;
  for (int i = 0; i < m_EffectTable[m_Index].size(); i++) {
    if (i == removeIndex) {
      continue;
    }
    tmp.push_back(m_EffectTable[m_Index][i]);
  }
  // update m_EffectTable[m_Index]
  m_EffectTable[m_Index].clear();
  m_EffectTable[m_Index] = tmp;
}
