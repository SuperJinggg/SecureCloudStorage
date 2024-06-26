#ifndef AUDITENTITY_H
#define AUDITENTITY_H
#include <QDateTime>

class AuditEntity
{
public:
    AuditEntity();

private:
    int id;
    QDateTime time; //时间
    QString ipaddress;//客户端IP地质
    QString username;//用户名
    QString role;//用户角色
    QString method;//操作方法,例如账户登录,账户退出,文件上传
    QString module;//模块,登录模块,文件浏览模块
    QString object;//访问资源,目标等,登录模块是xxxx端,文件上传是xxx文件
    QString result;//成功或者失败
    QString status;//状态,开始状态,执行状态,中断状态,异常状态,终止状态等.

public:
    int getid();
    QDateTime gettime(); //时间
    QString getipaddress();//客户端IP地质
    QString getusername();//用户名
    QString getrole();//用户角色
    QString getmethod();//操作方法,例如账户登录,账户退出,文件上传
    QString getmodule();//模块,登录模块,文件浏览模块
    QString getobject();//访问资源,目标等,登录模块是xxxx端,文件上传是xxx文件
    QString getresult();//成功或者失败
    QString getstatus();



    void setid(int id);
    void settime(QDateTime time); //时间
    void setipaddress(QString ipaddress);//客户端IP地质
    void setusername(QString username);//用户名
    void  setrole(QString role);//用户角色
   void   setmethod(QString method);//操作方法,例如账户登录,账户退出,文件上传
    void  setmodule(QString module);//模块,登录模块,文件浏览模块
    void  setobject(QString object);//访问资源,目标等,登录模块是xxxx端,文件上传是xxx文件
   void   setresult(QString result );//成功或者失败
    void  setstatus(QString status);



};

#endif // AUDITENTITY_H
