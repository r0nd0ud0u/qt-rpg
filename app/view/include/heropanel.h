#ifndef HEROPANEL_H
#define HEROPANEL_H

#include <QWidget>
#include <QMouseEvent>

#include "character.h"

namespace Ui {
class HeroPanel;
}

class HeroPanel : public QWidget
{
    Q_OBJECT

public:
    explicit HeroPanel(QWidget *parent = nullptr);
    ~HeroPanel();
    void UpdatePanel(Character* hero);
    void SetActive(const bool activated);
    void SetSelected(const bool selected);
    void mousePressEvent(QMouseEvent *event) override;
    void SetPixmap(const QString& name);
    void InitComboBox();

    Character* m_Heroe = nullptr;

private slots:
    void on_edit_button_clicked() const;

    void on_form_comboBox_currentTextChanged(const QString &arg1);

signals:
    void SigPanelSelectCharacter(QString);
    void SigUpdateCharaForm(const QString&,const QString&);

private:
    Ui::HeroPanel *ui;


};

#endif // HEROPANEL_H
