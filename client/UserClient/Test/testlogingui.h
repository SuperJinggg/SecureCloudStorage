#ifndef TESTLOGINGUI_H
#define TESTLOGINGUI_H

#include <QObject>
#include <QtTest>
#include "./Login/loginwidget.h"
#include "ui_loginwidget.h"
class TestLoginGui : public QObject
{
    Q_OBJECT
public:
    explicit TestLoginGui(QObject *parent = nullptr);

signals:

public slots:
private slots:
    void test_logingui();
    void test_logingui_data();
};

#endif // TESTLOGINGUI_H
