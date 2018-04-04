#include "XMariaDB.h"

XMariaDB& XMariaDB::GetInstance()
{
	static XMariaDB mariadb;
	return mariadb;
}

bool XMariaDB::HasConnect()
{
	return !_Connects.empty();
}

XMariaDBConnect* XMariaDB::GetConnect()
{
	std::lock_guard<std::mutex> lock(_Mutex);

	if (_Connects.empty())
		return nullptr;

	XMariaDBConnect* connect = _Connects.back();
	_Connects.pop_back();
	return connect;
}

void XMariaDB::PushConnect(XMariaDBConnect* pConnect)
{
	std::lock_guard<std::mutex> lock(_Mutex);

	_Connects.push_back(pConnect);
}


XMariaDB::XMariaDB()
{
	Init();
}

XMariaDB::~XMariaDB()
{
	Done();
}

void XMariaDB::Init()
{
	for (int i = 0; i < _MARIADB_CONNECT_NUM_; ++i)
	{
		XMariaDBConnect* pConnect = new XMariaDBConnect();
		_Connects.push_back(pConnect);
	}
}

void XMariaDB::Done()
{
	for (std::list<XMariaDBConnect*>::iterator iter = _Connects.begin(); iter != _Connects.end(); ++iter)
	{
		delete *iter;
	}
	_Connects.clear();
}