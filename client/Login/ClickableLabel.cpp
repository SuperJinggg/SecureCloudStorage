//
// Created by Mr Black on 2023/6/14.
//

#include "ClickableLabel.h"

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f)
	: QLabel(parent) {

}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
	emit clicked();
}