// FileOperate.cpp: implementation of the CFileOperate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WormNet.h"
#include "FileOperate.h"
#include "Network.h"
#include <fstream.h>
#include "Route.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern		CRoute	RouteArray[ROUTENUM];		//·���б�

CFileOperate::CFileOperate()
{

}

CFileOperate::~CFileOperate()
{

}

/*============================================================================
Ŀ�ģ�
		��ȡShortestPath.TXT�ļ������·�ɱ�
============================================================================*/
bool CFileOperate::FromFileShortestPath()
{
	ifstream f("ShortestPath.txt"); 
	int i=0;
	int j=0;
	int pos_left=0;
	int pos_right=0;
	int line_num=0;				//һ���е�һ������
	char line_char[ShortestPath_MaxCol];	//һ��
	char letter=' ';			//
	CString line_str;			//һ��
	CConnection *con;
	CConnection *newcon;
	int	RouteID=0;				//·��ID
	int	TableID=0;				//·�ɱ�ID
	//��ʼ��
	memset(line_char,'\0',ShortestPath_MaxCol*sizeof(char));	//line_char�������
	f.getline(line_char,ShortestPath_MaxCol-1);				//�ѵ�i�е��뵽line_char��
	line_str=line_char;
	if(line_str=="")
		{
		printf("\nShortestPath.txt�ļ���û������!\n");
		return false;
		}
	//�����ļ�
	while(line_str!="")
		{
		//�ҵ���һ������
		pos_left=2;
		pos_right=line_str.Find(letter, pos_left);
		RouteID=atoi(line_str.Mid(pos_left, pos_right-pos_left));
		//�ҵ��ڶ�������
		pos_left=pos_right+3;
		pos_right=line_str.Find(letter, pos_left);
		if(pos_right==-1)	//��ʾ����{"S:0 T:0"}״̬����ֱͨ
			{
			goto NextLine;
			}
		TableID=atoi(line_str.Mid(pos_left, pos_right-pos_left));
		con=&RouteArray[RouteID].Table[TableID];
		printf("Build Route Table! RouteID:%d, TableID:%d\r",RouteID,TableID);
		//�ҵ����������
		pos_left=pos_right+1;
		while(pos_right!=line_str.GetLength())
			{
			//�ҵ�ÿ����
			pos_left=pos_right+1;						//������ʼλ��
			pos_right=line_str.Find(letter, pos_left);	//������ֹλ��
			if(pos_right==-1)
				pos_right=line_str.GetLength();
			line_num=atoi(line_str.Mid(pos_left, pos_right-pos_left));
			//����һ��·�ɱ�ڵ�
			newcon=(CConnection *)malloc(sizeof(CConnection));
			newcon->ID=line_num;
			newcon->pNext=NULL;
			//����·������
			con->pNext=newcon;
			con=newcon;	
			}
NextLine:
		//���ļ�����һ�� 
		memset(line_char,'\0',ShortestPath_MaxCol*sizeof(char));	//line_char�������
		f.getline(line_char,ShortestPath_MaxCol-1);				//�ѵ�i�е��뵽line_char��
		line_str=line_char;
		}
	return true;
}

/*============================================================================
Ŀ�ģ�
		��·����������·����Ϣ������WormNum.TXT�ļ���
============================================================================*/
bool CFileOperate::ToFileWormNum()
{
	printf("File: WormNum.txt...");
	FILE* fout;					//���������
	int i;
	if(!(fout=fopen("WormNum.txt","w")))
		{
		printf("Fail to write into WormNum.txt\n");
		getchar();
		return false;
		}
	for(i=0;i<SIMULATETIME;i++)
		fprintf(fout,"%d  ",m_net->m_data.m_aWormNumTimes[i]);
	fclose(fout);
	printf("   Finished!\n");
	return true;
}

void CFileOperate::SetNetwork(CNetwork *net)
{
	m_net=net;
}

bool CFileOperate::FromFileRouteDegree()
{
	int i=0;
	int degree=0;
	char line_char=' ';
	ifstream f("RouteDegree.txt"); 
	printf("\nRoute and RouteDegree:\n");
	for(i=0; i<ROUTENUM; i++)
		{
		f.getline(&line_char,10);		//�ѵ�i�е��뵽line_char�У�10������к�
		RouteArray[i].degree=atoi(&line_char);
		if(RouteArray[i].degree==0)
			{
			printf("\nWrong in FromFileRouteDegree()!\n");
			return false;
			}
//		printf("\nRouteID:%d, RouteDegree:%d",i,RouteArray[i].degree);
		}
	return true;
}
