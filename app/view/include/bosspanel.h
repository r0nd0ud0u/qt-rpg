#ifndef BOSSPANEL_H
#define BOSSPANEL_H

#include <QWidget>

class Character;

namespace Ui {
class BossPanel;
}

class BossPanel : public QWidget
{
    Q_OBJECT

public:
    explicit BossPanel(QWidget *parent = nullptr);
    ~BossPanel();
    void UpdatePanel(Character *boss);
    void SetActive(bool activated);

    Character* m_Boss = nullptr;

private:
    Ui::BossPanel *ui;
};

#endif // BOSSPANEL_H
