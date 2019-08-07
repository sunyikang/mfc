// Data.cpp: implementation of the CData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WormNet.h"
#include "Data.h"

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
	m_nNewTargetNum=0;
	m_nNetLoad=0;
	m_nNetTP=0;
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
		NewTarget=������Ŀ*������/255/255/255/255*�ظ���ʱ��Ƭ����;
				���б���ʡ�����ʱ��Ƭ�ķ��������ظ���ʱ��Ƭ��
���أ�
		���û��1̨��Ŀ�귵��false�����ڵ���1̨��Ŀ�귵��true
============================================================================*/
bool CData::CalulateNewTargetNum(const int &RepeatTime)
{
	//�������ɨ���������˳��������������TP
	m_nNetTP=(m_nWormActive+m_nWormSleep)*SCAN_SPEED;
	//���ҵ���Ŀ����Ŀ
	m_nNewTargetNum=ROUTENUM*HOSTNUM /255/255 *m_nNetTP*RepeatTime /255/255 ;
	if(m_nNewTargetNum<1)
		return false;
	return true;
}
