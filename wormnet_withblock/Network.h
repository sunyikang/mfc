// Network.h: interface for the CNetwork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETWORK_H__F132604C_5180_44C9_8C23_C0F08522C676__INCLUDED_)
#define AFX_NETWORK_H__F132604C_5180_44C9_8C23_C0F08522C676__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Data.h"
#include "FileOperate.h"

class CNetwork  
{
public:
	CData			m_data;			//唯一的数据对象
	CFileOperate	m_file;			//文件操作对象
public:
	void GetRandomNum(const long &Max, long &RandomNum);
	void GetSTnum(int &num);
	void InitialRandomNum();
	bool IsPathOpen(CConnection *path, int SID, int TID);
	void InsertDataToList(CList *H, const int &newID, const int &newDegree);
	void KillRoute();

	bool InfectTarget();
	bool KillTarget_HaveInfected();
	bool KillTarget_Repeat();
	bool KillTarget_NoPath();



	bool PartenSourceAndTarget();
	int FindHostID(long index, int Status);

	void Simulate();
	CNetwork();
	virtual ~CNetwork();
};

#endif // !defined(AFX_NETWORK_H__F132604C_5180_44C9_8C23_C0F08522C676__INCLUDED_)
