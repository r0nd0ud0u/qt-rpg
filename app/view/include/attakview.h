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
    void UpdateAttak(QString name);
    void RemoveAttak();
private:
    Ui::AttakView *ui;
};

#endif // ATTAKVIEW_H
