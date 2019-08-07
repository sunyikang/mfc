// Node.h: interface for the CRoute class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROUTE_H__94EAB138_52BF_4703_8A98_28E6C0496AB8__INCLUDED_)
#define AFX_ROUTE_H__94EAB138_52BF_4703_8A98_28E6C0496AB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Data.h"

class CRoute  
{
	//成员变量
public:
	int			RouteID;			//主机ID
	bool		status;				//工作状态（true正常，false屏蔽）
	int			degree;				//节点连接度
	CConnection Table[ROUTENUM];	//路由表，记录与其他节点的连接

public:
	CRoute();
	virtual ~CRoute();
};

#endif // !defined(AFX_ROUTE_H__94EAB138_52BF_4703_8A98_28E6C0496AB8__INCLUDED_)
