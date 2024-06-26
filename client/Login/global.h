#ifndef GLOBAL_H
#define GLOBAL_H
#include <QString>
#include "./LocalLog/locallogdialog.h"

extern QString sessionid;
extern QString sessionkey;
extern QString role;
extern QString username;
extern QString lashauthtime;
extern QString lastauthipaddress;
extern QString url;
extern QString current_selected_folder_uniqueid;
extern LocalLogDialog *locallogdialog;
extern QString file_path;
extern int loglevel;
#define _SKLOIS_CONTENTSCROOL_DEBUG_

extern float currentversion;
#endif // GLOBAL_H
