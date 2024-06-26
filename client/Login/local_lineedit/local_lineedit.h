#ifndef LOCAL_LINEEDIT_H
#define LOCAL_LINEEDIT_H

#include <QObject>
#include <QLineEdit>

class Local_LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    Local_LineEdit(QWidget  *parent = 0);
    ~Local_LineEdit();


signals:
    void signal_linedit_focussed(bool hasFocus);

public slots:

protected:
  virtual void focusInEvent(QFocusEvent *e);
  virtual void focusOutEvent(QFocusEvent *e);


};

#endif // LOCAL_LINEEDIT_H
