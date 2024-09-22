#ifndef HEROPANEL_H
#define HEROPANEL_H

#include <QMouseEvent>
#include <QWidget>

#include "character.h"
#include "playersmanager.h"

namespace Ui {
class HeroPanel;
}

class HeroPanel : public QWidget {
  Q_OBJECT

public:
  explicit HeroPanel(QWidget *parent = nullptr);
  ~HeroPanel();
  void UpdatePanel(Character *hero, const std::vector<GameAtkEffects>& table);
  void SetActive(const bool activated);
  bool GetActive() const;
  void SetSelected(const bool selected);
  void mousePressEvent(QMouseEvent *event) override;
  void SetPixmap(const QString &name);
  void InitComboBox();
  void SetSelectedGameChoiceBtn(const bool value);
  void SetPlayingStatus();
  void SetSelectStatus();

  Character *m_Heroe = nullptr;

private slots:
  void on_edit_button_clicked() const;

  void on_form_comboBox_currentTextChanged(const QString &arg1);

  void on_pushButton_clicked();

  signals:
  void SigPanelSelectCharacter(const Character*);
  void SigUpdateCharaForm(const QString &, const QString &);
  void SigUpdateCharacterPlaying(const QString &);

private:
  Ui::HeroPanel *ui;

  void UpdateActiveRightWidget();
};

#endif // HEROPANEL_H
