// Network.cpp: implementation of the CNetwork class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WormNet.h"
#include "Network.h"
#include "Route.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern	CRoute	RouteArray[ROUTENUM];		//路由列表

CNetwork::CNetwork()
{
	
}

CNetwork::~CNetwork()
{

}

/*============================================================================
目的：
		仿真函数
============================================================================*/
void CNetwork::Simulate()
{
	int i=0;
	int	RepeatTime=1;
	printf("===========================================================================\n");
	printf("         Welcome to Route-Topo and Shortest-Path Building Program          \n");
	printf("===========================================================================\n");
	printf("\n\n------------------------Read ShortestPath from File------------------------\n");
	m_file.SetNetwork(this);
	m_file.FromFileShortestPath();
	m_file.FromFileRouteDegree();
	printf("\n\n------------------------Kill Some Focus Route------------------------\n");
	KillRoute();
	printf("\n\n------------------------Worm Spreading...----------------------------\n");
	InitialRandomNum();				//初始化随机数
	for(i=0; i<SIMULATETIME; i++)	//时间循环
		{
		m_data.m_aWormNumTimes[i]=m_data.m_nWormActive+m_data.m_nWormSleep;
		printf("\n\nTime:%d, ActiveWrom:%d, SleepWorm:%d", 
				i, m_data.m_nWormActive, m_data.m_nWormSleep);
		if(m_data.m_aWormNumTimes[i]==ROUTENUM*HOSTNUM)
			continue;		
		if(!m_data.CalulateNewTargetNum(RepeatTime))
			{
			RepeatTime++;
			}
		else
			{
			printf(", NewTarget:%d", m_data.m_nTargetNum);
			RepeatTime=1;
			PartenSourceAndTarget();
			KillTarget_Repeat();
			KillTarget_HaveInfected();
			KillTarget_NoPath();
			InfectTarget();
			}
		}
	//保存结果
	printf("\n\n------------------------Save result to File------------------------\n");
	m_file.ToFileWormNum();
	//测试
	scanf("%d",&i);
}

/*============================================================================
目的：
		源虫和目的地址(ROUTENUM - m_nWormActive)之间配对
		(m_nWormActive) ---- (ROUTENUM - m_nWormActive - m_nWormSleep)
============================================================================*/
bool CNetwork::PartenSourceAndTarget()
{
	int i=0;
	long index=0;
	long SourceID=0;
	long TargetID=0;
	//指针
	CSourceTarget	*L=m_data.m_pSourceTarget;	
	CSourceTarget	*N=NULL;	
	for(i=0; i<m_data.m_nTargetNum; i++)
		{
		//源
		GetRandomNum(m_data.m_nWormActive + m_data.m_nWormSleep, index);
		SourceID=FindHostID(index, 1);				//1:被感染主机
		//目的
		GetRandomNum(ROUTENUM*HOSTNUM, TargetID);	//0：未被感染主机
		//建立新映射元素
		N=new CSourceTarget();
		N->source=SourceID;
		N->target=TargetID;
		N->pNext=NULL;
		//插入映射表
		L->pNext=N;
		L=N;
		}
//	printf("PartenNum:%d",i);
	return true;
}

/*============================================================================
目的：
		按照Direction在m_aHostStatus数组中找一个状态是Status的排index的主机ID
参数：
		Direction：		1正向	－1反向
		Status：		0正常	1感染	2感染休眠
		index：			不能超过数组长度
返回：
		返回-1或者特别大的数，表示没有找到
		其他都表示正常
============================================================================*/
int CNetwork::FindHostID(long index, int status)
{
	if(index<0 || index>ROUTENUM*HOSTNUM || status>2 || status<0)
		{
		printf("Wrong in FindHostID 1!\n");
		getchar();
		return -1;
		}
	long ID=-1;
	int num=0;
	while(num<=index)
		{
		ID++;
		if(ID<0 || ID>ROUTENUM*HOSTNUM)
			{
			printf("Wrong in FindHostID 2!\n");
			int RealNum=0;
			for(int k=0; k<ROUTENUM*HOSTNUM; k++)
				{
				if(m_data.m_aHostStatus[k]==status)
					RealNum++;
				}
			return -1;
			}
		if(m_data.m_aHostStatus[ID]==status)
			num++;
		}

/*	bool Direction=true;
	//判断检索方向
	if(status==0)
		{
		if(index < (ROUTENUM*HOSTNUM-m_data.m_nWormActive-m_data.m_nWormSleep)/2)
			{
			Direction=true;
			}
		else
			{
			Direction=false;
			}
		}
	else
		{
		if(index < (m_data.m_nWormActive+m_data.m_nWormSleep)/2)
			{
			Direction=true;
			}
		else
			{
			Direction=false;
			}
		}
	//找到主机ID
	if(Direction)
		{
		i=-1;
		num=0;
		while(num<=index)
			{
			i++;
			if(m_data.m_aHostStatus[i]==status)
				num++;
			}
		}
	else
		{
		i=ROUTENUM*HOSTNUM;
		num=ROUTENUM*HOSTNUM;
		while(num>index)
			{
			i--;
			if(m_data.m_aHostStatus[i]==status)
				num--;
			}
		}*/
	return ID;
}

/*============================================================================
目的：
		剔除目标主机中已经被感染的
============================================================================*/
bool CNetwork::KillTarget_HaveInfected()
{
	int i=0;
	CSourceTarget	*pGuard;
	CSourceTarget	*p;
	for(pGuard=m_data.m_pSourceTarget; pGuard->pNext!=NULL; pGuard=pGuard->pNext)
		{
		if(m_data.m_aHostStatus[pGuard->pNext->target]==1)
			{
			p=pGuard->pNext;
			pGuard->pNext=p->pNext;
			delete p;
			if(pGuard->pNext==NULL)
				break;
			}
		}
	return true;
}

/*============================================================================
目的：
		在映射列表m_pSourceTarget中，剔除源、目的主机最短路径不通的
============================================================================*/
bool CNetwork::KillTarget_NoPath()
{
	int i=0;
	int	SourceRID=-1;				//源路由ID
	int	TargetRID=-1;				//目标路由ID
	CConnection		*con;			//最短路径链表头指针
	CSourceTarget	*pGuard=NULL;
	CSourceTarget	*p=NULL;
	//处理m_pSourceTarget后面的节点
/*	//测试
	int num=0;
	GetSTnum(num);*/
	for(pGuard=m_data.m_pSourceTarget; pGuard->pNext!=NULL; )
		{
		SourceRID=(pGuard->pNext->source)/HOSTNUM;
		TargetRID=(pGuard->pNext->target)/HOSTNUM;
		if(SourceRID!=TargetRID)		//不同局域网
			{
			//得到最短路径
			con=&RouteArray[SourceRID].Table[TargetRID];
			if(!IsPathOpen(con, SourceRID, TargetRID))		//不通,pGuard不能后移
				{
				p=pGuard->pNext;
				pGuard->pNext=p->pNext;
				delete p;
				if(pGuard->pNext==NULL)
					break;
				}
			else											//通
				{
				pGuard=pGuard->pNext;						//只有通的情况下才后移
			//	printf("\nPass!\n");
				}
			}
		else							//同一局域网
			{
			pGuard=pGuard->pNext;						//只有通的情况下才后移
			//printf("\nSame Network!\n");
			}
		}
	return true;
}

/*============================================================================
目的：
		在映射列表m_pSourceTarget中，剔除“目标主机ID”与其他映射重复的映射
============================================================================*/
bool CNetwork::KillTarget_Repeat()
{
	int	ID1=-1;
	int	ID2=-1;
	CSourceTarget	*F=NULL;
	CSourceTarget	*S=NULL;
	CSourceTarget	*P=NULL;
	for(F=m_data.m_pSourceTarget->pNext; F->pNext!=NULL; F=F->pNext)
		{
		ID1=F->target;
		for(S=F; S->pNext!=NULL; S=S->pNext)
			{
			ID2=S->pNext->target;
			if(ID1==ID2)
				{
				P=S->pNext->pNext;
				delete S->pNext;
				S->pNext=P;
				if(S->pNext==NULL)
					break;
				}
			}
		}
	return true;
}

/*============================================================================
目的：
		感染目标主机
操作：
		需要改变 m_aHostStatus m_aWormNumTimes m_nWormActive
		需要清零 m_nTargetNum 
		需要释放 m_pSourceTarget
============================================================================*/
bool CNetwork::InfectTarget()
{
	int WormNum=0;
	int	ID=-1;
	int i=0;
	CSourceTarget	*P=NULL;
	CSourceTarget	*L=NULL;
	//用于找数值的指针
	for(P=m_data.m_pSourceTarget->pNext; P!=NULL; P=P->pNext)
		{
		ID=P->target;
		m_data.m_aHostStatus[ID]=1;	
		}
	for(i=0; i<ROUTENUM*HOSTNUM; i++)
		{
		if(m_data.m_aHostStatus[i]==1)
			WormNum++;
		}
	printf(", RealInfectNum:%d",WormNum-m_data.m_nWormActive);
	m_data.m_nWormActive=WormNum;
	//清零
	m_data.m_nTargetNum=0;
	//释放
	for(L=m_data.m_pSourceTarget,P=L->pNext; P!=NULL; P=P->pNext)
		{
		L->pNext=P->pNext;
		delete P;
		P=L;
		}
	return true;
}

/*============================================================================
目的：
		找到度最大的i个路由节点，把他们的状态设为“屏蔽”
操作：
		改变RouteArray[P->ID].status
============================================================================*/
void CNetwork::KillRoute()
{
	int i=0;
	int KillNum=0;				//删除数
	CList *H=NULL;				//哨兵指针head
	CList *P=NULL;				
	CList *N=NULL;				//New

	//输入K掉的路由数
INPUT_KILLNUM:
	printf("\nKill the Route Num:");
	scanf("%d",&KillNum);
	if(KillNum<0 || KillNum>ROUTENUM)
		{
		printf("The KillNum has no meaning!");
		goto	INPUT_KILLNUM;
		}
	if(KillNum==0)
		{
		return;
		}
	//建立列表，存储RouteID，按照RouteDegree大小排列，列表头空出来
	H=new CList();
	H->ID=-1;
	H->degree=0;
	H->next=NULL;
	P=H;
	for(i=0; i<KillNum; i++)
		{	
		N=new CList();
		N->ID=-1;
		N->degree=0;
		N->next=NULL;
		P->next=N;
		P=N;
		}
	//在RouteArray中找KillNum个最大数值
	for(i=0; i<ROUTENUM; i++)
		{
		InsertDataToList(H, i, RouteArray[i].degree);
		}
	//列举K掉的路由ID
	printf("\nKilled Routes: ");
	for(i=0,P=H->next; P!=NULL; P=P->next,i++)
		{
		printf("\nID:%d, Degree:%d", P->ID, P->degree);
		if(P->ID<0 || P->ID>ROUTENUM)
			{
			printf("Wrong in KillRoute() 1!");
			getchar();
			return;
			}
		RouteArray[P->ID].status=false;		//路由本身改变状态
		}
	//测试
	if(KillNum!=i)
		{
			printf("Wrong in KillRoute() 2!");
			getchar();
		}
}

/*============================================================================
目的：
		列表插值，插入一个节点，删掉最小的一个节点
条件：
		H是列表头指针，头位置是空的
操作：
		改变Min，列表的最小值
============================================================================*/
void CNetwork::InsertDataToList(CList *H, const int &newID, const int &newDegree)
{
	CList *P=NULL;
	CList *N=NULL;
	for(P=H; P->next!=NULL; P=P->next)
		{
		if(newDegree > P->next->degree)	//删
			{	
			//建新节点
			N=new CList();			
			N->ID=newID;
			N->degree=newDegree;
			N->next=NULL;
			//插入新节点
			N->next=P->next;
			P->next=N;
			//删掉最后一个点
			for(;P->next->next!=NULL; P=P->next);
			delete P->next;
			P->next=NULL;
			break;
			}
		}
}

/*============================================================================
目的：
		判断此路径是否畅通
		path[i].status是否等于false
返回：
		false不通  true通
============================================================================*/
bool CNetwork::IsPathOpen(CConnection *path, int SID, int TID)
{
	if(SID<0 || SID>ROUTENUM || TID<0 || TID>ROUTENUM)
		{
		printf("Wrong in IsPathOpen()!");
		getchar();
		return false;
		}
	//检查path
	for(CConnection *p=path; p->pNext!=NULL; p=p->pNext)
		{
		if(!RouteArray[p->pNext->ID].status)
			{
			return false;
			}
		}
	//检查SID,TID
	if(!RouteArray[SID].status || !RouteArray[TID].status)
		{
		return false;
		}
	return true;
}

/*============================================================================
目的：
		得到范围Scope中的随机数，放入RandomNum中
说明：
		srand(**)用于rand()的初始化，只能初始化一次
============================================================================*/
void CNetwork::InitialRandomNum()
{
	//随机种子
	static int randomseed=0;
	int stime;
	long ltime;
	//设置rand()的随机数种子，产生随机目标
	ltime=time(NULL);
	stime=(unsigned int)ltime;
	srand(stime);
}

/*============================================================================
目的：
		得到链表的长度
============================================================================*/
void CNetwork::GetSTnum(int &num)
{
	num=0;
	CSourceTarget *R;
	for(R=m_data.m_pSourceTarget->pNext ; R!=NULL; R=R->pNext,num++);
}

/*============================================================================
目的：
		得到0－－Max的随机数
============================================================================*/
void CNetwork::GetRandomNum(const long &Max, long &RandomNum)
{
 	RandomNum=rand()*rand()%Max;
}
