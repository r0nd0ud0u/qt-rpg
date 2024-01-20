#ifndef ATTAKVIEW_H
#define ATTAKVIEW_H

#include <QWidget>

namespace Ui {
class AttakView;
}

class AttakView : public QWidget
{
    Q_OBJECT

public:
    explicit AttakView(QWidget *parent = nullptr);
    ~AttakView();
    void AddAttak();

private:
    Ui::AttakView *ui;
};

#endif // ATTAKVIEW_H
