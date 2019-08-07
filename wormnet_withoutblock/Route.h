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
	//��Ա����
public:
	int			RouteID;			//����ID
	bool		status;				//����״̬��true������false���Σ�
	int			degree;				//�ڵ����Ӷ�
	CConnection Table[ROUTENUM];	//·�ɱ���¼�������ڵ������

public:
	CRoute();
	virtual ~CRoute();
};

#endif // !defined(AFX_ROUTE_H__94EAB138_52BF_4703_8A98_28E6C0496AB8__INCLUDED_)
