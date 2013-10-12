#include "LineEdit.h"

LineEdit::LineEdit(QWidget *parent) : QLineEdit(parent)
{


}



void LineEdit::keyPressEvent(QKeyEvent* event)
{
    emit keyPressed(event->key());
    QLineEdit::keyPressEvent(event);
}
