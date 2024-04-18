#ifndef BOSSPANEL_H
#define BOSSPANEL_H

#include <QMouseEvent>
#include <QWidget>

class Character;

namespace Ui {
class BossPanel;
}

class BossPanel : public QWidget {
  Q_OBJECT

public:
  explicit BossPanel(QWidget *parent = nullptr);
  ~BossPanel();
  void UpdatePanel(Character *boss);
  void SetActive(bool activated);
  void SetSelected(const bool selected);
  void mousePressEvent(QMouseEvent *event) override;
  bool GetActive() const;
  ;

  Character *m_Boss = nullptr;

signals:
  void SigSelectedCharacterOnPanel(const QString &);

private slots:
  void on_edit_button_clicked();

private:
  Ui::BossPanel *ui;
};

#endif // BOSSPANEL_H
