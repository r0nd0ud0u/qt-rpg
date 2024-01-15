#include "CompButtons.h"

#include <QColorDialog>


////////////////////////////////////////////////////////
// Constructors, destructors

CompColorButton::CompColorButton(QWidget* parent)
    : QPushButton(parent)
{
    this->setMaximumSize(28, 28);
    this->setMinimumSize(28, 28);
    this->SetColor(QColor(0, 255, 255));
}

////////////////////////////////////////////////////////
// Methods

void CompColorButton::SetColor(QColor c)
{
    this->m_color = c;
    QString style("CompColorButton {"
                  "  border-radius : 3px;"
                  "  background: %1;"
                  "  border : 1px solid %2;"
                  "}"
                  "CompColorButton:hover {"
                  "    border : 1px solid grey;"
                  "}"
                  "CompColorButton:disabled {"
                  "    background : #222222;"
                  "}");
    style = style.arg(c.name(), c.name());
    this->setStyleSheet(style);
}

void CompColorButton::mouseReleaseEvent(QMouseEvent* e)
{
    QColor result = QColorDialog::getColor(m_color, this);
    if (result.isValid())
    {
        this->SetColor(result);
    }
    emit ColorChanged();
    QPushButton::mouseReleaseEvent(e);
}
