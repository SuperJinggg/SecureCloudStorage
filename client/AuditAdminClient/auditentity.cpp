#include "auditentity.h"

AuditEntity::AuditEntity()
{

}

int AuditEntity::getid()
{

	return id;

}
QDateTime AuditEntity::gettime()
{

	return time;

}
QString AuditEntity::getipaddress()
{

	return ipaddress;

}
QString AuditEntity::getusername()
{
	return username;

}
QString AuditEntity::getrole()
{
	return this->role;
}

QString AuditEntity::getmethod()
{
	return method;
}

QString AuditEntity::getmodule()
{
	return module;
}

QString AuditEntity::getobject()
{
	return object;
}

QString AuditEntity::getresult()
{
	return result;
}
QString AuditEntity::getstatus()
{
	return status;
}

void AuditEntity::setid(int id)
{
	this->id = id;
}

void AuditEntity::settime(QDateTime time)
{
	this->time = time;
}

void AuditEntity::setipaddress(QString ipaddress)
{
	this->ipaddress = ipaddress;
}
void AuditEntity::setusername(QString username)
{
	this->username = username;
}
void AuditEntity::setrole(QString role)
{
	this->role = role;
}
void AuditEntity::setmethod(QString method)
{
	this->method = method;
}
void AuditEntity::setmodule(QString module)
{
	this->module = module;
}
void AuditEntity::setobject(QString object)
{
	this->object = object;
}
void AuditEntity::setresult(QString result)
{
	this->result = result;
}
void AuditEntity::setstatus(QString status)
{
	this->status = status;
}
