#include "dotview.h"
#include "ui_dotview.h"

#include "complabel.h"

#include <QToolTip>

const int DURATION_TOOLTIP = 3000;

DotView::DotView(QWidget *parent) : QWidget(parent), ui(new Ui::DotView) {
  ui->setupUi(this);
  setStyleSheet(
      "#hot_label{ background:     #00ff00;} #dot_label{ background:     "
      "#ff0000;} #buf_label{ background:     #33a2ff;} #debuf_label{ "
      "background: "
      "#ff8d33;}");

  connect(ui->buf_label, &CompLabel::SigCompLabelClicked, this,
          &DotView::onBufLabel);
  connect(ui->debuf_label, &CompLabel::SigCompLabelClicked, this,
          &DotView::onDebufLabel);
  connect(ui->hot_label, &CompLabel::SigCompLabelClicked, this,
          &DotView::onHotLabel);
  connect(ui->dot_label, &CompLabel::SigCompLabelClicked, this,
          &DotView::onDotLabel);
}

DotView::~DotView() { delete ui; }

void DotView::SetHotDotValues(const int hot, const int dot) {
  ui->hot_nb_label->setText(QString::number(hot));
  ui->dot_nb_label->setText(QString::number(dot));
}

void DotView::SetBufDebufValues(const int buf, const int debuf) {
  ui->buf_label_nb->setText(QString::number(buf));
  ui->debuf_label_nb->setText(QString::number(debuf));
}

void DotView::onBufLabel() {
  ResetAllToolTips();
  const auto text = m_BufTexts.join("\n");
  ui->buf_label->setToolTip(text);
  ui->buf_label->setToolTipDuration(DURATION_TOOLTIP);
}

void DotView::onDebufLabel() {
  ResetAllToolTips();
  const auto text = m_DebufTexts.join("\n");
  ui->buf_label->setToolTip(text);
  ui->debuf_label->setToolTip("onDebufLabel");
  ui->debuf_label->setToolTipDuration(DURATION_TOOLTIP);
}

void DotView::onHotLabel() {
  ResetAllToolTips();
  const auto text = m_HotTexts.join("\n");
  ui->hot_label->setToolTip(text);
  ui->hot_label->setToolTipDuration(DURATION_TOOLTIP);
}

void DotView::onDotLabel() {
  ResetAllToolTips();
  const auto text = m_DotTexts.join("\n");
  ui->dot_label->setToolTip(text);
  ui->dot_label->setToolTipDuration(DURATION_TOOLTIP);
}

void DotView::ResetAllToolTips() {
  ui->dot_label->setToolTip("");
  ui->hot_label->setToolTip("");
  ui->debuf_label->setToolTip("");
  ui->buf_label->setToolTip("");
}

void DotView::UpdateData(const std::optional<AllEffectsType> &effects,
                         const std::optional<AllEffectsType> &bufDebuf) {
  m_HotTexts.clear();
  m_DotTexts.clear();
  m_BufTexts.clear();
  m_DebufTexts.clear();
  if (effects.has_value()) {
    m_HotTexts = effects->hot.effects;
    m_DotTexts = effects->dot.effects;
    m_BufTexts = effects->buf.effects;
    m_DebufTexts = effects->debuf.effects;
  }
  if (bufDebuf.has_value()) {
    m_BufTexts += bufDebuf->buf.effects;
    m_DebufTexts += m_DebufTexts;
  }
}
