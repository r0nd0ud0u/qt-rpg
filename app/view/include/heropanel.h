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

    Character* m_Heroe = nullptr;

private slots:
    void on_pushButton_clicked();

    void on_edit_button_clicked() const;

signals:
    void addStuff();
    void selectCharacter(QString);

private:
    Ui::HeroPanel *ui;

};

#endif // HEROPANEL_H
