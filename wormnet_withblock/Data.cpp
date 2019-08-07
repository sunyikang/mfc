// Data.cpp: implementation of the CData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WormNet.h"
#include "Data.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CData::CData()
{
	m_nWormSleep=0;
	m_nTargetNum=0;

	//newһ��CSourceTarget��m_pSourceTarget
	m_pSourceTarget=new CSourceTarget();
	m_pSourceTarget->source=-1;
	m_pSourceTarget->target=-1;
	m_pSourceTarget->pNext=NULL;
	//��ʼ����浥λʱ�̸�Ⱦ�б�
	memset(m_aWormNumTimes, -1, SIMULATETIME*sizeof(int));
	memset(m_aHostStatus, 0, ROUTENUM*HOSTNUM*sizeof(long));
	m_nWormActive=100;			//��ʼ�����Ŀ
	//���ѡ�񱻸�Ⱦ����
	for(int i=0; i<m_nWormActive; i++)
		{
		m_aHostStatus[HOSTNUM*(rand()%ROUTENUM)+rand()%HOSTNUM]=1;
		}
}

CData::~CData()
{
	delete	m_pSourceTarget;
	m_pSourceTarget=NULL;
}

/*============================================================================
Ŀ�ģ�
		������Ŀ������
��ʽ��
		N2=N1*[1-pow(N1/M,8)];				(1)
		M=NET_MAXLOAD
		N1=m_nWormActive*SCAN_SPEED
		N2=m_nNetTP
		TargetNum=N2*RepeatTime*BiLi;		(2)
		Bili=ROUTENUM*HOSTNUM /(255*255*255*255)
���أ�
		���û��1̨��Ŀ�귵��false�����ڵ���1̨��Ŀ�귵��true
============================================================================*/
bool CData::CalulateNewTargetNum(const int &RepeatTime)
{
	double N1=0.0;
	double N2=0.0;
	double temp=0.0;
	//�������ɨ���������˳��������������m_nNetTP
	N1=m_nWormActive*SCAN_SPEED;
	temp=N1/(double)(NET_MAXLOAD);
	N2=N1*(1-pow(temp,8));
	//���ҵ���Ŀ����Ŀm_nTargetNum
	m_nTargetNum=ROUTENUM*HOSTNUM /255/255 *N2*RepeatTime /255/255 ;
	if(m_nTargetNum<1)
		return false;
	return true;
}
