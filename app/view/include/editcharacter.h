#ifndef EDITCHARACTER_H
#define EDITCHARACTER_H

#include <QWidget>

class Character;
class EditStatPanel;

namespace Ui {
class EditCharacter;
}

class EditCharacter : public QWidget
{
    Q_OBJECT

public:
    explicit EditCharacter(QWidget *parent = nullptr);
    ~EditCharacter();
    void InitEditCharacter();
    void AddCharacter(Character* ch) const;

private:
    Ui::EditCharacter *ui;
    std::vector<EditStatPanel*> m_PanelList;
};

#endif // EDITCHARACTER_H
