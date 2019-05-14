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
	int iHeapPosition;  //�˽ڵ��ڶ��е�λ��
	int iG;
	int iH;
	int iF;
	int iColor;  //����Ƿ����ͨ��
	int isOpen; //����Ƿ��ڿ����б���
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
	void removeFront(Node**g_Map); //�Ƴ����еĵ�һ��Ԫ��
	void push_back(Coordinate element, Node**g_Map); //���������Ԫ��
	void newHeap(int position, Node**g_Map);    //��Fֵ�ı�ʱ�����¶�
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
	CHeap m_iOpenList; //�����б�
	void  ResetMap(cMyMap* pMap);
	void  Free();

	int m_Width=0;
	int m_Height=0;
private:
	bool m_isFind=FALSE; // ����Ƿ�������ȷ��·��
};

