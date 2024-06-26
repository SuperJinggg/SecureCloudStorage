#include "functiontest.h"

FunctionTest::FunctionTest(QObject *parent) : QObject(parent)
{

}

void FunctionTest::initTestCase(){
    if(nullptr==uploadoperation){
        uploadoperation = new UploadOperation();
    }
    if(nullptr==downloadoperation){
        downloadoperation = new DownloadOperation();
    }
}


void FunctionTest::cleanupTestCase(){
    if(nullptr!=uploadoperation){
        delete uploadoperation;
        uploadoperation = nullptr;
    }
    if(NULL!=downloadoperation){
        delete downloadoperation;
        downloadoperation = NULL;
    }
}

void FunctionTest::testcryptoalg(){
    QString inputstr = "7890121234567890123456789012345678901234561234567890123456789012";
    QFETCH(QString,inputstr);
    char output [83]="\0";
    QByteArray sessinkeyarray = sessionkey.toUtf8();
    char * key = sessinkeyarray.data();
    int keylen = sessinkeyarray.length();
    QByteArray encryptkeybytearray = inputstr.toUtf8();
    char* input = encryptkeybytearray.data();
    int inputlen = encryptkeybytearray.length();
    char cipherkeyout[109] = "\0";
    uploadoperation->aes_cbc_pkcs5_encrypt(key,keylen,input,inputlen,cipherkeyout);
    QString outencryptkey(cipherkeyout);
    qDebug()<<"outencryptkey is "<<outencryptkey;
    downloadoperation->aes_cbc_pkcs5_decrypt(key,keylen,cipherkeyout,strlen(cipherkeyout),output);
    QString decryptstr(output);
    QCOMPARE(inputstr,decryptstr);
}

//void FunctionTest::init(){
//    qDebug()<<"init";
//}
//void FunctionTest::cleanup(){
//    qDebug()<<"cleanup";
//}

void FunctionTest::testcryptoalg_data(){
    QTest::addColumn<QString> ("inputstr");
    QTest::addRow("parameter1")<<"7890121234567890123456789012345678901234561234567890123456789012";
    QTest::addRow("parameter2")<<"9012789012123456712345678890345678901234561234567890123456789012";
}

//QTEST_MAIN(FunctionTest);//调用QTEST_MAIN()函数启动测试类
