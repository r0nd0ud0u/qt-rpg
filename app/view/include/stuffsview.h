#ifndef STUFFSVIEW_H
#define STUFFSVIEW_H

#include <QWidget>

#include "stuffpanel.h"

namespace Ui {
class StuffsView;
}

class StuffsView : public QWidget
{
    Q_OBJECT

public:
    explicit StuffsView(QWidget *parent = nullptr);
    ~StuffsView();



private:
    Ui::StuffsView *ui;
    std::vector<StuffPanel*> m_StuffList;

    void InitEditStuffsView();
};

#endif // STUFFSVIEW_H
