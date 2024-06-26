#include "testlogingui.h"

TestLoginGui::TestLoginGui(QObject *parent) : QObject(parent)
{

}

void TestLoginGui::test_logingui(){
    QFETCH(QTestEventList,events);
    QFETCH(QTestEventList,events2);
    QFETCH(QString,uexpected);
    QFETCH(QString,pexpected);
    LoginWidget login;

    events.simulate((login.ui->lineEdit_username));
    events2.simulate((login.lineEdit_password));


    QCOMPARE(login.ui->lineEdit_username->text(),uexpected);
    QCOMPARE(login.lineEdit_password->text(),pexpected);


    //    QTest::keyClicks(login.ui->lineEdit_username,"normaluser01");
    //    QTest::keyClicks(login.lineEdit_password,"qwertyuiop!@#123");
}
void TestLoginGui::test_logingui_data(){
    QTest::addColumn<QTestEventList>("events");
    QTest::addColumn<QTestEventList>("events2");
    QTest::addColumn<QString>("uexpected");
    QTest::addColumn<QString>("pexpected");
    QTestEventList list1;
    list1.addKeyClicks("normaluser01");
    QTestEventList listpassword;
    listpassword.addKeyClicks("qwertyuiop!@#123");
    QTest::newRow("parameter1")<<list1<<listpassword<<"normaluser01"<<"qwertyuiop!@#123";

    QTestEventList list2;
    list2.addKeyClicks("normaluser02");
    QTestEventList list2password;
    list2password.addKeyClicks("qwertyuiop!@#123");
    QTest::newRow("parameter2")<<list2<<list2password<<"normaluser02"<<"qwertyuiop!@#123";
}

//QTEST_MAIN(TestLoginGui);
