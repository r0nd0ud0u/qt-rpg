#ifndef STUFFSVIEW_H
#define STUFFSVIEW_H

#include <QWidget>

#include "stuffpanel.h"
#include "stuff.h"

struct EditStuff {
    Stuff m_Stuff;
    QString m_Name;
    QString m_BodyPart;
    bool updated = false;
};


namespace Ui {
class StuffsView;
}

class StuffsView : public QWidget
{
    Q_OBJECT

public:
    explicit StuffsView(QWidget *parent = nullptr);
    ~StuffsView();
    EditStuff Save();


private:
    Ui::StuffsView *ui;
    std::vector<StuffPanel*> m_StuffList;
    std::vector<EditStuff> m_EditStuffList;

    void InitEditStuffsView();
};

#endif // STUFFSVIEW_H
