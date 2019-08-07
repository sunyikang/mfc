// CRoute.cpp: implementation of the CRoute class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Route.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRoute::CRoute()
{
	RouteID=-1;				//-1直通  -2不通
	degree=0;
	status=true;
	for(int i=0; i<ROUTENUM; i++)
		{
		Table[i].ID=-1;
		Table[i].pNext=NULL;
		}
}

CRoute::~CRoute()
{
	CConnection *con;
	CConnection *nextcon;
	for(int i=0; i<ROUTENUM; i++)
		{
		if(Table[i].pNext==NULL)
			continue;
		con=Table[i].pNext;
		while(con!=NULL)
			{
			nextcon=con->pNext;
			free(con);
			con=nextcon;
			}
		}
	con=NULL;
	nextcon=NULL;
}
