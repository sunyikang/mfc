// Data.h: interface for the CData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_H__3727C29D_0A9A_4D61_A221_467A1FE86309__INCLUDED_)
#define AFX_DATA_H__3727C29D_0A9A_4D61_A221_467A1FE86309__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////
//							      ȫ�ֱ���								//
//////////////////////////////////////////////////////////////////////////
//const long	ADDRESS_SPACE=256*256*256*256;	//��ַ�ռ� 2��32�η�
const int	SCAN_SPEED=900;				//���ɨ������	 =900̨/��
const int	SIMULATETIME=1000;			//����ʱ��	ʱ����1����
const int	ROUTENUM=522;				//·��������	522	
const int	HOSTNUM=256;				//ÿ��·�����µ�©����������	256	
const int	ShortestPath_MaxCol=600;	//ShortestPath.txt�ļ�������к�
const long	NET_MAXLOAD=ROUTENUM*HOSTNUM*SCAN_SPEED;	//�����������
//////////////////////////////////////////////////////////////////////////
//							      ��������								//
//////////////////////////////////////////////////////////////////////////
typedef struct CConnection		//��������
{
	int		ID;					//�ڵ�ID
	struct CConnection* pNext;	//ָ����һ��λ��
}CConnection;

typedef struct CList			//��������
{
	int		ID;					//�ڵ�ID
	int		degree;				//�ڵ��
	struct CList* next;		//ָ����һ��λ��
}CList;

typedef struct CSourceTarget		//Դ��Ŀ���ӳ������
{
	long	source;					//Դ�ڵ�ID
	long	target;					//Ŀ�Ľڵ�ID
	struct CSourceTarget* pNext;	//ָ����һ��λ��
}CSourceTarget;

class CData  
{
public:
	//��¼���������ĸ�Ⱦ״̬
	int	m_nWormActive;					//����������	 
	int m_nWormSleep;					//���ߵ��������
	int m_aHostStatus[ROUTENUM*HOSTNUM];//��������״̬ ��0δ��Ⱦ��1��Ⱦ��2��Ⱦ���ߣ�
										//����Ⱦָ��ȷ��ΪĿ��
	//��¼Ŀ�������������Ͷ�Ӧ��ϵ
	int m_nTargetNum;				//ÿ��ʱ�����ҵ���Ŀ����������
	CSourceTarget	*m_pSourceTarget;	//Դ��Ŀ��ӳ��
	
	//��¼����ʱ�̵����
	int	m_aWormNumTimes[SIMULATETIME];	//ÿ������ʱ�̽���ʱ��Ⱦ��������
public:
	bool CalulateNewTargetNum(const int &RepeatTime);
	CData();
	virtual ~CData();
};

#endif // !defined(AFX_DATA_H__3727C29D_0A9A_4D61_A221_467A1FE86309__INCLUDED_)
