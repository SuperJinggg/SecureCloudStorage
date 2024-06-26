/* DO NOT EDIT THIS FILE - it is machine generated */

#ifndef _Included_YunLock
#define _Included_YunLock

#include <QtCore/qglobal.h>
#include <QCoreApplication>
#include <QObject>
#include <QTimer>
#include <QDebug>

#ifdef Q_OS_WIN32
#include <windows.h>
#endif
#ifdef Q_OS_WIN64
#include <windows.h>
#endif
#ifdef Q_OS_LINUX
#include <link.h>
#include <dlfcn.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string>
using namespace std;
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     YunLock
 * Method:    testfun01
 * Signature: ()string;
 */


//encrypt file
/*
 * Class:     YunLock
 * Method:    CRE_EncryptFile
 * Signature: (string;string;string;)string;
 */
string  YunLock_EncryptFile
(string, string, string);


//decrypt file
/*
 * Class:     YunLock
 * Method:    CRE_DecryptFile
 * Signature: (string;string;string;)string;
 */
string  YunLock_DecryptFile
(string, string, string);




//Generate cipher index
/*
 * Class:     YunLock
 * Method:    CRE_GenerateCipherIndex
 * Signature: (string;string;)string;
 */
string  YunLock_GenerateCipherIndex
(string ,string);


//encrypt keyword
/*
 * Class:     YunLock
 * Method:    YunLock_EncryptKeyword
 * Signature: (string)string;
 */
string YunLock_EncryptKeyword(string keyword);



#ifdef __cplusplus
}
#endif
#endif