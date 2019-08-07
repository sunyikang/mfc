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
extern	CRoute	RouteArray[ROUTENUM];		//·���б�

CNetwork::CNetwork()
{
	
}

CNetwork::~CNetwork()
{

}

/*============================================================================
Ŀ�ģ�
		���溯��
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
	InitialRandomNum();				//��ʼ�������
	for(i=0; i<SIMULATETIME; i++)	//ʱ��ѭ��
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
	//������
	printf("\n\n------------------------Save result to File------------------------\n");
	m_file.ToFileWormNum();
	//����
	scanf("%d",&i);
}

/*============================================================================
Ŀ�ģ�
		Դ���Ŀ�ĵ�ַ(ROUTENUM - m_nWormActive)֮�����
		(m_nWormActive) ---- (ROUTENUM - m_nWormActive - m_nWormSleep)
============================================================================*/
bool CNetwork::PartenSourceAndTarget()
{
	int i=0;
	long index=0;
	long SourceID=0;
	long TargetID=0;
	//ָ��
	CSourceTarget	*L=m_data.m_pSourceTarget;	
	CSourceTarget	*N=NULL;	
	for(i=0; i<m_data.m_nTargetNum; i++)
		{
		//Դ
		GetRandomNum(m_data.m_nWormActive + m_data.m_nWormSleep, index);
		SourceID=FindHostID(index, 1);				//1:����Ⱦ����
		//Ŀ��
		GetRandomNum(ROUTENUM*HOSTNUM, TargetID);	//0��δ����Ⱦ����
		//������ӳ��Ԫ��
		N=new CSourceTarget();
		N->source=SourceID;
		N->target=TargetID;
		N->pNext=NULL;
		//����ӳ���
		L->pNext=N;
		L=N;
		}
//	printf("PartenNum:%d",i);
	return true;
}

/*============================================================================
Ŀ�ģ�
		����Direction��m_aHostStatus��������һ��״̬��Status����index������ID
������
		Direction��		1����	��1����
		Status��		0����	1��Ⱦ	2��Ⱦ����
		index��			���ܳ������鳤��
���أ�
		����-1�����ر���������ʾû���ҵ�
		��������ʾ����
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
	//�жϼ�������
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
	//�ҵ�����ID
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
Ŀ�ģ�
		�޳�Ŀ���������Ѿ�����Ⱦ��
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
Ŀ�ģ�
		��ӳ���б�m_pSourceTarget�У��޳�Դ��Ŀ���������·����ͨ��
============================================================================*/
bool CNetwork::KillTarget_NoPath()
{
	int i=0;
	int	SourceRID=-1;				//Դ·��ID
	int	TargetRID=-1;				//Ŀ��·��ID
	CConnection		*con;			//���·������ͷָ��
	CSourceTarget	*pGuard=NULL;
	CSourceTarget	*p=NULL;
	//����m_pSourceTarget����Ľڵ�
/*	//����
	int num=0;
	GetSTnum(num);*/
	for(pGuard=m_data.m_pSourceTarget; pGuard->pNext!=NULL; )
		{
		SourceRID=(pGuard->pNext->source)/HOSTNUM;
		TargetRID=(pGuard->pNext->target)/HOSTNUM;
		if(SourceRID!=TargetRID)		//��ͬ������
			{
			//�õ����·��
			con=&RouteArray[SourceRID].Table[TargetRID];
			if(!IsPathOpen(con, SourceRID, TargetRID))		//��ͨ,pGuard���ܺ���
				{
				p=pGuard->pNext;
				pGuard->pNext=p->pNext;
				delete p;
				if(pGuard->pNext==NULL)
					break;
				}
			else											//ͨ
				{
				pGuard=pGuard->pNext;						//ֻ��ͨ������²ź���
			//	printf("\nPass!\n");
				}
			}
		else							//ͬһ������
			{
			pGuard=pGuard->pNext;						//ֻ��ͨ������²ź���
			//printf("\nSame Network!\n");
			}
		}
	return true;
}

/*============================================================================
Ŀ�ģ�
		��ӳ���б�m_pSourceTarget�У��޳���Ŀ������ID��������ӳ���ظ���ӳ��
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
Ŀ�ģ�
		��ȾĿ������
������
		��Ҫ�ı� m_aHostStatus m_aWormNumTimes m_nWormActive
		��Ҫ���� m_nTargetNum 
		��Ҫ�ͷ� m_pSourceTarget
============================================================================*/
bool CNetwork::InfectTarget()
{
	int WormNum=0;
	int	ID=-1;
	int i=0;
	CSourceTarget	*P=NULL;
	CSourceTarget	*L=NULL;
	//��������ֵ��ָ��
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
	//����
	m_data.m_nTargetNum=0;
	//�ͷ�
	for(L=m_data.m_pSourceTarget,P=L->pNext; P!=NULL; P=P->pNext)
		{
		L->pNext=P->pNext;
		delete P;
		P=L;
		}
	return true;
}

/*============================================================================
Ŀ�ģ�
		�ҵ�������i��·�ɽڵ㣬�����ǵ�״̬��Ϊ�����Ρ�
������
		�ı�RouteArray[P->ID].status
============================================================================*/
void CNetwork::KillRoute()
{
	int i=0;
	int KillNum=0;				//ɾ����
	CList *H=NULL;				//�ڱ�ָ��head
	CList *P=NULL;				
	CList *N=NULL;				//New

	//����K����·����
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
	//�����б��洢RouteID������RouteDegree��С���У��б�ͷ�ճ���
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
	//��RouteArray����KillNum�������ֵ
	for(i=0; i<ROUTENUM; i++)
		{
		InsertDataToList(H, i, RouteArray[i].degree);
		}
	//�о�K����·��ID
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
		RouteArray[P->ID].status=false;		//·�ɱ���ı�״̬
		}
	//����
	if(KillNum!=i)
		{
			printf("Wrong in KillRoute() 2!");
			getchar();
		}
}

/*============================================================================
Ŀ�ģ�
		�б��ֵ������һ���ڵ㣬ɾ����С��һ���ڵ�
������
		H���б�ͷָ�룬ͷλ���ǿյ�
������
		�ı�Min���б����Сֵ
============================================================================*/
void CNetwork::InsertDataToList(CList *H, const int &newID, const int &newDegree)
{
	CList *P=NULL;
	CList *N=NULL;
	for(P=H; P->next!=NULL; P=P->next)
		{
		if(newDegree > P->next->degree)	//ɾ
			{	
			//���½ڵ�
			N=new CList();			
			N->ID=newID;
			N->degree=newDegree;
			N->next=NULL;
			//�����½ڵ�
			N->next=P->next;
			P->next=N;
			//ɾ�����һ����
			for(;P->next->next!=NULL; P=P->next);
			delete P->next;
			P->next=NULL;
			break;
			}
		}
}

/*============================================================================
Ŀ�ģ�
		�жϴ�·���Ƿ�ͨ
		path[i].status�Ƿ����false
���أ�
		false��ͨ  trueͨ
============================================================================*/
bool CNetwork::IsPathOpen(CConnection *path, int SID, int TID)
{
	if(SID<0 || SID>ROUTENUM || TID<0 || TID>ROUTENUM)
		{
		printf("Wrong in IsPathOpen()!");
		getchar();
		return false;
		}
	//���path
	for(CConnection *p=path; p->pNext!=NULL; p=p->pNext)
		{
		if(!RouteArray[p->pNext->ID].status)
			{
			return false;
			}
		}
	//���SID,TID
	if(!RouteArray[SID].status || !RouteArray[TID].status)
		{
		return false;
		}
	return true;
}

/*============================================================================
Ŀ�ģ�
		�õ���ΧScope�е������������RandomNum��
˵����
		srand(**)����rand()�ĳ�ʼ����ֻ�ܳ�ʼ��һ��
============================================================================*/
void CNetwork::InitialRandomNum()
{
	//�������
	static int randomseed=0;
	int stime;
	long ltime;
	//����rand()����������ӣ��������Ŀ��
	ltime=time(NULL);
	stime=(unsigned int)ltime;
	srand(stime);
}

/*============================================================================
Ŀ�ģ�
		�õ�����ĳ���
============================================================================*/
void CNetwork::GetSTnum(int &num)
{
	num=0;
	CSourceTarget *R;
	for(R=m_data.m_pSourceTarget->pNext ; R!=NULL; R=R->pNext,num++);
}

/*============================================================================
Ŀ�ģ�
		�õ�0����Max�������
============================================================================*/
void CNetwork::GetRandomNum(const long &Max, long &RandomNum)
{
 	RandomNum=rand()*rand()%Max;
}
