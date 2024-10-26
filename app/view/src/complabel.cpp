#include "complabel.h"

#include <QToolTip>

////////////////////////////////////////////////////////
// Constructors, destructors

CompLabel::CompLabel(QWidget* parent)
    : QLabel(parent)
{
}

void CompLabel::mouseMoveEvent(QMouseEvent *event){
    emit SigCompLabelClicked();
}
