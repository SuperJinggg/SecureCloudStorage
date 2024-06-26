#include <QtTest>
#include <QDebug>
#include <QLineEdit>
#include "./inc/sha256.h"
#include "./inc/base64_enc.h"
#include "./inc/hmac_sha256.h"
#include "./securitycloudstorageclientwidget.h"
#include "transformentity/downloadoperation.h"
// add necessary includes here

class Testname : public QObject
{
    Q_OBJECT

public:
    Testname();
    ~Testname();

private slots:
    void testcreyptoalg();
    void testcreyptoalg_data();
//    void testgui();

};

Testname::Testname()
{

}

Testname::~Testname()
{

}



void Testname::testcreyptoalg()
{
//    qDebug()<<"1212";
    QFETCH(QString,inputstr);

    //    QString inputstr = "1234567890123456789012345678901212345678901234567890123456789012";
    char output [83]="\0";

    QByteArray sessinkeyarray = sessionkey.toUtf8();
    char * key = sessinkeyarray.data();
    int keylen = sessinkeyarray.length();
    QByteArray encryptkeybytearray = inputstr.toUtf8();
    char* input = encryptkeybytearray.data();
    int inputlen = encryptkeybytearray.length();
    char cipherkeyout[109] = "\0";
    UploadOperation *upload = new UploadOperation();
    upload->aes_cbc_pkcs5_encrypt(key,keylen,input,inputlen,cipherkeyout);
    QString outencryptkey(cipherkeyout);
    qDebug()<<"outencryptkey is "<<outencryptkey;
    DownloadOperation *downloadoperation = new DownloadOperation();
    downloadoperation->aes_cbc_pkcs5_decrypt(key,keylen,cipherkeyout,strlen(cipherkeyout),output);
    QString decryptstr(output);
    QCOMPARE(inputstr,decryptstr);
    //    QLineEdit lineedit;
    //    QTest::keyClicks(&lineedit,"hello world1");
    //    QCOMPARE(lineedit.text(),QString("hello world"));
    //    SecurityCloudStorageClientWidget w;

    //    w.show();
    //    QTest::qWait(1000);
//    qDebug()<<"2222";
//    QTest::qExec(this);
}
void Testname::testcreyptoalg_data(){
    QTest::addColumn<QString> ("inputstr");
    QTest::addRow("parameter1")<<"7890121234567890123456789012345678901234561234567890123456789012";
    QTest::addRow("parameter2")<<"9012789012123456712345678890345678901234561234567890123456789012";
}


//void Testname::testgui(){
//    OnlineUpdateDialog *onlineupdate = new OnlineUpdateDialog();
//    onlineupdate->get_version();
//    QTest::qWait(1000);
//}

//QTEST_MAIN(Testname);
//QTEST_APPLESS_MAIN(Testname)

#include "tst_testname.moc"
