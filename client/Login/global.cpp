#include "global.h"

//QString sessionid = "81a385a7f73b5ff37e0b914cf6baff7e6bc9971fc945399024e5c5055e26a2d4";
//QString sessionkey = "2c416d768f99459a88d6c3ca1e432cfe";
//QString role = "普通用户";
//QString username = "username";
//QString lashauthtime = "2020-03-11 15:22:47.799";
//QString lastauthipaddress = "192.168.56.1";
QString sessionid = "";
QString sessionkey = "";
QString role = "";
QString username = "";
QString lashauthtime = "";
QString lastauthipaddress = "";

QString current_selected_folder_uniqueid;

QString url = "https://127.0.0.1:9443/";

LocalLogDialog *locallogdialog =nullptr;
int loglevel = 0;
QString file_path="";
float currentversion = 1.0;
