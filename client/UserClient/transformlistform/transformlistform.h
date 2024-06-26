#ifndef TRANSFORMLISTFORM_H
#define TRANSFORMLISTFORM_H

#include <QWidget>

namespace Ui {
class TransformListForm;
}

class TransformListForm : public QWidget
{
    Q_OBJECT

public:
    explicit TransformListForm(QWidget *parent = nullptr);
    ~TransformListForm();

private:
    Ui::TransformListForm *ui;
};

#endif // TRANSFORMLISTFORM_H
