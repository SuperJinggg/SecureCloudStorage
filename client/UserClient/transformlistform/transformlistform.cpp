#include "transformlistform.h"
#include "ui_transformlistform.h"

TransformListForm::TransformListForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransformListForm)
{
    ui->setupUi(this);
}

TransformListForm::~TransformListForm()
{
    delete ui;
}
