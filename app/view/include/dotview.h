#ifndef DOTVIEW_H
#define DOTVIEW_H

#include <QWidget>

#include "effect.h"

namespace Ui {
class DotView;
}

class DotView : public QWidget {
  Q_OBJECT

public:
  explicit DotView(QWidget *parent = nullptr);
  ~DotView();

  void SetHotDotValues(const int hot, const int dotf);
  void SetBufDebufValues(const int buf, const int debuf);
  void UpdateData(const std::optional<AllEffectsType> &effects,
                  const std::optional<AllEffectsType> &bufDebuf);

private slots:
  void onBufLabel();
  void onDebufLabel();
  void onHotLabel();
  void onDotLabel();

private:
  Ui::DotView *ui;
  QStringList m_BufTexts;
  QStringList m_DebufTexts;
  QStringList m_HotTexts;
  QStringList m_DotTexts;

  void ResetAllToolTips();
};

#endif // DOTVIEW_H
