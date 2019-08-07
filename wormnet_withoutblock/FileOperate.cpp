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

extern		CRoute	RouteArray[ROUTENUM];		//路由列表

CFileOperate::CFileOperate()
{

}

CFileOperate::~CFileOperate()
{

}

/*============================================================================
目的：
		读取ShortestPath.TXT文件，填充路由表
============================================================================*/
bool CFileOperate::FromFileShortestPath()
{
	ifstream f("ShortestPath.txt"); 
	int i=0;
	int j=0;
	int pos_left=0;
	int pos_right=0;
	int line_num=0;				//一行中的一个数字
	char line_char[ShortestPath_MaxCol];	//一行
	char letter=' ';			//
	CString line_str;			//一行
	CConnection *con;
	CConnection *newcon;
	int	RouteID=0;				//路由ID
	int	TableID=0;				//路由表ID
	//初始化
	memset(line_char,'\0',ShortestPath_MaxCol*sizeof(char));	//line_char数组清空
	f.getline(line_char,ShortestPath_MaxCol-1);				//把第i行导入到line_char中
	line_str=line_char;
	if(line_str=="")
		{
		printf("\nShortestPath.txt文件中没有数据!\n");
		return false;
		}
	//遍历文件
	while(line_str!="")
		{
		//找到第一个数字
		pos_left=2;
		pos_right=line_str.Find(letter, pos_left);
		RouteID=atoi(line_str.Mid(pos_left, pos_right-pos_left));
		//找到第二个数字
		pos_left=pos_right+3;
		pos_right=line_str.Find(letter, pos_left);
		if(pos_right==-1)	//表示进入{"S:0 T:0"}状态——直通
			{
			goto NextLine;
			}
		TableID=atoi(line_str.Mid(pos_left, pos_right-pos_left));
		con=&RouteArray[RouteID].Table[TableID];
		printf("Build Route Table! RouteID:%d, TableID:%d\r",RouteID,TableID);
		//找到后面的数字
		pos_left=pos_right+1;
		while(pos_right!=line_str.GetLength())
			{
			//找到每个数
			pos_left=pos_right+1;						//数字起始位置
			pos_right=line_str.Find(letter, pos_left);	//数字中止位置
			if(pos_right==-1)
				pos_right=line_str.GetLength();
			line_num=atoi(line_str.Mid(pos_left, pos_right-pos_left));
			//创建一个路由表节点
			newcon=(CConnection *)malloc(sizeof(CConnection));
			newcon->ID=line_num;
			newcon->pNext=NULL;
			//加入路由链表
			con->pNext=newcon;
			con=newcon;	
			}
NextLine:
		//找文件的下一行 
		memset(line_char,'\0',ShortestPath_MaxCol*sizeof(char));	//line_char数组清空
		f.getline(line_char,ShortestPath_MaxCol-1);				//把第i行导入到line_char中
		line_str=line_char;
		}
	return true;
}

/*============================================================================
目的：
		把路由网络的最短路径信息导出到WormNum.TXT文件中
============================================================================*/
bool CFileOperate::ToFileWormNum()
{
	printf("File: WormNum.txt...");
	FILE* fout;					//保存仿真结果
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
		f.getline(&line_char,10);		//把第i行导入到line_char中，10是最大列号
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
