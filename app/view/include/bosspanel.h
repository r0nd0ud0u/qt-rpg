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
  void SetSelectedGameChoiceBtn(const bool value);
  void SetPlayingStatus();
  void SetSelectStatus();

  Character *m_Boss = nullptr;

signals:
  void SigPanelSelectCharacter(const QString &);
  void SigUpdateCharacterPlaying(const QString &);

private slots:
  void on_edit_button_clicked();

  void on_selectPushButton_clicked();

private:
  Ui::BossPanel *ui;

  void UpdateActiveInfoWidget();
};

#endif // BOSSPANEL_H
