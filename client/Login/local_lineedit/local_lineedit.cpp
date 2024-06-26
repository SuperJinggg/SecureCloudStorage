#include "local_lineedit.h"

Local_LineEdit::Local_LineEdit(QWidget  *parent) : QLineEdit(parent)
{

}

void Local_LineEdit::focusInEvent(QFocusEvent *e){
    QLineEdit::focusInEvent(e);
    emit(signal_linedit_focussed(true));
}

void Local_LineEdit::focusOutEvent(QFocusEvent *e){
    QLineEdit::focusOutEvent(e);
    emit(signal_linedit_focussed(false));
}


Local_LineEdit::~Local_LineEdit(){

}
