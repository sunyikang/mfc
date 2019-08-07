// FileOperate.h: interface for the CFileOperate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEOPERATE_H__2CF6EB3B_BC6E_4A5B_9E58_A62BE1650678__INCLUDED_)
#define AFX_FILEOPERATE_H__2CF6EB3B_BC6E_4A5B_9E58_A62BE1650678__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class	CNetwork;

class CFileOperate  
{
public:
	CNetwork	*m_net;
public:
	bool FromFileRouteDegree();
	void SetNetwork(CNetwork *net);
	bool ToFileWormNum();
	CFileOperate();
	virtual ~CFileOperate();
	bool	FromFileShortestPath();
};

#endif // !defined(AFX_FILEOPERATE_H__2CF6EB3B_BC6E_4A5B_9E58_A62BE1650678__INCLUDED_)
