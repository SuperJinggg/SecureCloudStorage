#ifndef FUNCTIONTEST_H
#define FUNCTIONTEST_H

#include <QtTest>
#include <QObject>
#include <QDebug>
#include <QLineEdit>
#include "./inc/sha256.h"
#include "./inc/base64_enc.h"
#include "./inc/hmac_sha256.h"
#include "./securitycloudstorageclientwidget.h"

class FunctionTest : public QObject
{
    Q_OBJECT
public:
    explicit FunctionTest(QObject *parent = nullptr);

signals:

public slots:

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testcryptoalg();
    void testcryptoalg_data();

private:
    UploadOperation *uploadoperation {uploadoperation = nullptr};
    DownloadOperation *downloadoperation {downloadoperation = nullptr};
};

#endif // FUNCTIONTEST_H
