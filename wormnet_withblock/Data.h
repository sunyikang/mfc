// Data.h: interface for the CData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_H__3727C29D_0A9A_4D61_A221_467A1FE86309__INCLUDED_)
#define AFX_DATA_H__3727C29D_0A9A_4D61_A221_467A1FE86309__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////
//							      全局变量								//
//////////////////////////////////////////////////////////////////////////
//const long	ADDRESS_SPACE=256*256*256*256;	//地址空间 2的32次方
const int	SCAN_SPEED=900;				//蠕虫扫描速率	 =900台/分
const int	SIMULATETIME=1000;			//仿真时间	时间间隔1分钟
const int	ROUTENUM=522;				//路由器数量	522	
const int	HOSTNUM=256;				//每个路由器下的漏洞主机个数	256	
const int	ShortestPath_MaxCol=600;	//ShortestPath.txt文件的最大列号
const long	NET_MAXLOAD=ROUTENUM*HOSTNUM*SCAN_SPEED;	//网络最大负载量
//////////////////////////////////////////////////////////////////////////
//							      类型声明								//
//////////////////////////////////////////////////////////////////////////
typedef struct CConnection		//连接类型
{
	int		ID;					//节点ID
	struct CConnection* pNext;	//指向下一个位置
}CConnection;

typedef struct CList			//链表类型
{
	int		ID;					//节点ID
	int		degree;				//节点度
	struct CList* next;		//指向下一个位置
}CList;

typedef struct CSourceTarget		//源和目标的映射类型
{
	long	source;					//源节点ID
	long	target;					//目的节点ID
	struct CSourceTarget* pNext;	//指向下一个位置
}CSourceTarget;

class CData  
{
public:
	//记录所有主机的感染状态
	int	m_nWormActive;					//活动的蠕虫数量	 
	int m_nWormSleep;					//休眠的蠕虫数量
	int m_aHostStatus[ROUTENUM*HOSTNUM];//所有主机状态 （0未感染；1感染；2感染休眠）
										//将感染指被确定为目标
	//记录目标主机的数量和对应关系
	int m_nTargetNum;				//每个时刻新找到的目标主机总量
	CSourceTarget	*m_pSourceTarget;	//源和目的映射
	
	//记录仿真时刻的蠕虫
	int	m_aWormNumTimes[SIMULATETIME];	//每个仿真时刻结束时已染主机总数
public:
	bool CalulateNewTargetNum(const int &RepeatTime);
	CData();
	virtual ~CData();
};

#endif // !defined(AFX_DATA_H__3727C29D_0A9A_4D61_A221_467A1FE86309__INCLUDED_)
