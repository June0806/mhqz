#pragma once
#include "global.h"
using namespace std;

const int TOTAL_MAP_WIDTH =350;
const int TOTAL_MAP_HEIGHT=350;
class cMyMap;
struct Coordinate
{
	int sx;
	int sy;
	Coordinate()
	{
		sx = 0;
		sy = 0;
	}
};

struct Node
{
	int iHeapPosition;  //此节点在堆中的位置
	int iG;
	int iH;
	int iF;
	int iColor;  //检测是否可以通过
	int isOpen; //检测是否在开启列表中
	Coordinate father;

	Node()
	{
		iHeapPosition = -1;
		iG = -1;
		iH = -1;
		iF = -1;
		iColor = -1;
		isOpen = 0;
	}
	int getF()
	{
		return iH + iG;
	}
};

class CHeap
{
public:
	CHeap();
	void removeFront(Node**g_Map); //移除堆中的第一个元素
	void push_back(Coordinate element, Node**g_Map); //往堆中添加元素
	void newHeap(int position, Node**g_Map);    //当F值改变时，更新堆
	int getSize();
	int getMinF(Node**g_Map);
	bool empty();
	void clear();
	Coordinate begin();
private:
	vector<Coordinate> v;
};

class cFindPath
{
public:
	~cFindPath();
	BOOL InitMap();
	BOOL FindPath(cObj* pObj, int Xend, int Yend);
	Node** g_Map;
	CHeap m_iOpenList; //开启列表
	void  ResetMap(cMyMap* pMap);
	void  Free();

	int m_Width=0;
	int m_Height=0;
private:
	bool m_isFind=FALSE; // 检测是否发现了正确的路径
};

