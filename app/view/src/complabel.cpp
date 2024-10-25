#include "complabel.h"

#include <QToolTip>

////////////////////////////////////////////////////////
// Constructors, destructors

CompLabel::CompLabel(QWidget* parent)
    : QLabel(parent)
{
}

void CompLabel::mousePressEvent(QMouseEvent *event){
    //emit SigCompLabelClicked();
}

void CompLabel::mouseMoveEvent(QMouseEvent *event){
    emit SigCompLabelClicked();
}
