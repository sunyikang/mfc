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
	//new一个CSourceTarget给m_pSourceTarget
	m_pSourceTarget=new CSourceTarget();
	m_pSourceTarget->source=-1;
	m_pSourceTarget->target=-1;
	m_pSourceTarget->pNext=NULL;
	//初始化蠕虫单位时刻感染列表
	memset(m_aWormNumTimes, -1, SIMULATETIME*sizeof(int));
	memset(m_aHostStatus, 0, ROUTENUM*HOSTNUM*sizeof(long));
	m_nWormActive=100;			//初始蠕虫数目
	//随机选择被感染主机
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
目的：
		计算新目标数量
公式：
		NewTarget=主机数目*任务量/255/255/255/255*重复的时间片次数;
				＝中标概率×单个时间片的发包数×重复的时间片数
返回：
		如果没有1台新目标返回false，大于等于1台新目标返回true
============================================================================*/
bool CData::CalulateNewTargetNum(const int &RepeatTime)
{
	//计算蠕虫扫描包在网络顺利传播的任务量TP
	m_nNetTP=(m_nWormActive+m_nWormSleep)*SCAN_SPEED;
	//新找到的目标数目
	m_nNewTargetNum=ROUTENUM*HOSTNUM /255/255 *m_nNetTP*RepeatTime /255/255 ;
	if(m_nNewTargetNum<1)
		return false;
	return true;
}
