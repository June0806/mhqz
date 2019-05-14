#include "global.h"

#define POSITIVE(x,y) ((((x) - (y))>0)? ((x)-(y)):((y)-(x)))
const int IS_UNKNOWN = 0;
const int IS_OPEN = 1;
const int IS_CLOSE = 2;
const int MAP_ROAD = 0;
const int MAP_WALL = 1;
const int MAP_CANT = 2;


BOOL cFindPath::FindPath(cObj* pObj, int Xend, int Yend)
{
	if (Xend < 0||Yend<0)return FALSE;
	InitMap();

	int Xstart = pObj->GetX()/20;
	int Ystart = pObj->GetY()/20;
	Xend /= 20;
	Yend /= 20;

	if (Xend>m_Width - 1 || Yend>m_Height - 1)return FALSE;
	//如果目标点在障碍内,则移动目标点的位置
	while(g_Map[Xend][Yend].iColor==MAP_WALL)
	{
		if (Xend > Xstart)
			Xend -= 1;
		else
			Xend += 1;
		if (Yend > Ystart)
			Yend -= 1;
		else
			Yend += 1;
		if (abs(Xend - Xstart)<10&&abs(Yend == Ystart)<10)
			return TRUE;
	}
	Coordinate coordinate;
	Coordinate minCoord;
	coordinate.sx = Xstart;
	coordinate.sy = Ystart;
	int xx1 = (Xstart - Xend)*5;
	int yy1 = (Ystart - Yend)*5;
	g_Map[Xstart][Ystart].iH = (int)sqrt(xx1*xx1 + yy1*yy1);;
	g_Map[Xstart][Ystart].iG = 0;
	g_Map[Xstart][Ystart].iF = g_Map[Xstart][Ystart].getF();

	// 1. 把起始格添加到开启列表
	m_iOpenList.push_back(coordinate, g_Map);
	g_Map[Xstart][Ystart].isOpen = IS_OPEN;

	//2.重复如下工作
	int minF = g_Map[Xstart][Ystart].iF; //该变量表示最小F值
	m_isFind = false; //该变量，用来检测目标位置是否被添加进了关闭列表

	while (!m_iOpenList.empty()) //当开启列表为空时， 循环停止
	{
		//a).寻找开启列表 中F最低的格子，我们称它为当前格
		minCoord = m_iOpenList.begin();
		minF = g_Map[minCoord.sx][minCoord.sy].getF();
		if ((minCoord.sx == Xend) && (minCoord.sy == Yend)) //如果目标位置被添加到了关闭容器，直接跳出循环
		{
			m_isFind = true;
			break;
		}

		//b).把遍历得到的当前格切换到关闭列表
		m_iOpenList.removeFront(g_Map); //删除开启列表中的F值最低的格子
		g_Map[minCoord.sx][minCoord.sy].isOpen = IS_CLOSE;//把该格子的标记设置为“在关闭列表中”


		int i = -1, j = -1;
		int x = 0, y = 0;
		for (i = -1; i < 2; ++i)  //遍历当前格子周围的其他8个格子
		{
			for (j = -1; j < 2; ++j)
			{
				x = minCoord.sx + i;
				y = minCoord.sy + j;

				if ((x < 0) || (y < 0) || (x > m_Width - 1) || (y > m_Height - 1)) //* 坐标在屏幕之外，略过
					continue;
				if (g_Map[x][y].iColor == MAP_WALL) //* 格子为墙，不能通过，略过
				{
					continue;
				}
				if (g_Map[x][y].isOpen == IS_CLOSE) //* 格子已经在关闭列表中了，略过
					continue;
				if (g_Map[x][y].isOpen == IS_UNKNOWN) //* 如果该格子未加入到开启列表，
				{
					coordinate.sx = x;
					coordinate.sy = y;
					g_Map[x][y].father = minCoord;  //当前格作为这一格的父节点
					 xx1 = (Xend - x) * 5;
					 yy1 = (Yend - y) * 5;
					g_Map[x][y].iH =(int)sqrt( xx1*xx1 + yy1*yy1);
					//设置G
					if ((i == 0) || (j == 0))
					{
						g_Map[x][y].iG = g_Map[minCoord.sx][minCoord.sy].iG + 5;
					}
					else
					{
						g_Map[x][y].iG = g_Map[minCoord.sx][minCoord.sy].iG + 7;
					}
	
					//设置F
					g_Map[x][y].iF = g_Map[x][y].getF();
					//加入开吂列表
					g_Map[x][y].isOpen = IS_OPEN;
					//放入开启列表
					m_iOpenList.push_back(coordinate, g_Map);  //把他加入到开启列表中
					continue;
				}

				if (g_Map[x][y].isOpen == IS_OPEN) //* 如果该格子已经在开启列表中
				{
					int newG;
					if ((i == 0) || (j == 0))  //计算新的G值
					{
						newG = g_Map[minCoord.sx][minCoord.sy].iG + 5;
					}
					else
					{
						newG = g_Map[minCoord.sx][minCoord.sy].iG + 7;
					}
					if (newG < g_Map[x][y].iG) //如果新的G值比先前的更好
					{
						g_Map[x][y].father = minCoord; //把该格的父节点改为当前格
						int position = g_Map[x][y].iHeapPosition; //得到该节点在堆中的位置
						g_Map[x][y].iG = newG;
						g_Map[x][y].iF = g_Map[x][y].getF();
						m_iOpenList.newHeap(position, g_Map);             //由于F值发生了变化，更新此节点在堆中的位置
					}
				}
			}
		}
	}
	if (m_isFind) //3).如果找到路径了，保存路径
	{
		Node tempNode;
		tempNode.father = minCoord;
		pObj->m_Path.clear();//清空
		pObj->m_time = -1;
		int x1, y1;
		while (true)
		{
			if ((tempNode.father.sx == -1) && (tempNode.father.sy == -1)){
				break;
			}
			x1= tempNode.father.sx ;
			y1= tempNode.father.sy ;
			tempNode.father.sx*=20;
			tempNode.father.sy*=20;
			pObj->m_Path.push_back(tempNode.father);
			pObj->m_time += 1;
			tempNode.father = g_Map[x1][y1].father;
		}
		pObj->m_time -= 1;
		if (pObj->m_time < 0)return FALSE;
		pObj->SetTarget(pObj->m_Path[pObj->m_time].sx, pObj->m_Path[pObj->m_time].sy);
		pObj->m_time -= 1;
	}
	else //没找到路径的处理
	{

	}
	return FALSE;
}
//我准备在cMyMap中调用它
void cFindPath::ResetMap(cMyMap* pMap)
{
	Free();
	m_Height = pMap->m_pMap->m_Height/20;
	m_Width = pMap->m_pMap->m_Width/20;
	if (!m_Height)return;
	if (!m_Width)return;
	g_Map = new Node*[m_Width];
	for (int i = 0; i < m_Width;i++)
	{
		g_Map[i] = new Node[m_Height];
	}
	for (int i = 0; i < m_Width;i++)
	for (int j = 0; j < m_Height; j++)
	{
		if (pMap->m_pBmp->isOK(i * 20, j * 20))
		{
			g_Map[i][j].iColor = MAP_WALL;
		}
		else
			g_Map[i][j].iColor = MAP_ROAD;
	}

}

void cFindPath::Free()
{
	if (!g_Map)return;
	for (int i = 0; i < m_Width; i++)
	{
		delete[] g_Map[i];
	}
	SAFE_DELETE_ARRAY(g_Map);
	m_Width = 0;
	m_Height = 0;
}

BOOL cFindPath::InitMap()
{
	if (!m_Height)return FALSE;
	if (!m_Width)return FALSE;
	for (int x = 0; x < m_Width; x++)
	{
		for (int y = 0; y < m_Height; y++)
		{
			g_Map[x][y].iHeapPosition = -1;
			g_Map[x][y].iH = -1;
			g_Map[x][y].iG = -1;
			g_Map[x][y].iF = -1;
			g_Map[x][y].isOpen = IS_UNKNOWN;
			g_Map[x][y].father.sx = -1;
			g_Map[x][y].father.sy = -1;
		}
	}
	if (!m_iOpenList.empty()) //清空开启列表
		m_iOpenList.clear();

	return TRUE;
}

cFindPath::~cFindPath()
{
	Free();
}





//构造函数
CHeap::CHeap()
{
}

//判断二叉堆是否为空
bool CHeap::empty()
{
	if (v.size() == 0)
		return true;
	else
		return false;
}

//清空二叉堆
void CHeap::clear()
{
	if (v.size() != 0)
		v.clear();
}

//返回二叉堆中的第一个元素（即F值最低的元素）
Coordinate CHeap::begin()
{
	return v[0];
}

//删除二叉堆的堆首元素（即F值最低的元素）
void CHeap::removeFront(Node**g_Map)
{
	if (v.size() == 0)
		return;

	g_Map[v[v.size() - 1].sx][v[v.size() - 1].sy].iHeapPosition = 0;
	g_Map[v[0].sx][v[0].sy].iHeapPosition = -1;

	v[0] = v[v.size() - 1];      //用最后一个元素把第一个元素覆盖掉，即为删除
	v.pop_back();                //删除容器尾巴元素



	int currentIndex = 0;
	while (currentIndex < v.size()) //把新的堆首元素放在堆中适当的位置
	{
		int leftChildIndex = 2 * currentIndex + 1;
		int rightChildIndex = 2 * currentIndex + 2;

		//已经到最底层，结束
		if (rightChildIndex > v.size())
			break;
		int minIndex = leftChildIndex;

		//有两个孩子，找出两个孩子节点中F值最低的元素
		if (rightChildIndex<v.size() && (g_Map[v[minIndex].sx][v[minIndex].sy].getF()  > g_Map[v[rightChildIndex].sx][v[rightChildIndex].sy].getF()))
		{
			minIndex = rightChildIndex;
		}

		//如果当前节点的F值 大于 他孩子节点的F值，则交换
		if (g_Map[v[currentIndex].sx][v[currentIndex].sy].getF() > g_Map[v[minIndex].sx][v[minIndex].sy].getF())
		{
			Coordinate temp = v[minIndex];
			v[minIndex] = v[currentIndex];
			v[currentIndex] = temp;

			//同步保存地图中该坐标在堆中的最新位置
			g_Map[v[currentIndex].sx][v[currentIndex].sy].iHeapPosition = currentIndex;
			g_Map[v[minIndex].sx][v[minIndex].sy].iHeapPosition = minIndex;
			currentIndex = minIndex;
		}
		else
		{
			break;
		}
	}
}

//返回堆中的最小F值
int CHeap::getMinF(Node**g_Map)
{
	if (v.size() > 0)
		return g_Map[v[0].sx][v[0].sx].getF();
	else
		return 0;
}


//往堆中添加新的元素（节点）
void CHeap::push_back(Coordinate element, Node**g_Map)
{
	v.push_back(element);//把新节点添加到堆的末尾
	int currentIndex = v.size() - 1;
	g_Map[v[currentIndex].sx][v[currentIndex].sy].iHeapPosition = currentIndex; //保存该坐标在堆中的位置

	while (currentIndex > 0) //不断的与他的父节点比较，直到该新节点的F值大于他的父节点的F值为止 或者 该新节点到了堆首
	{
		int parentIndex = (currentIndex - 1) / 2;
		if (g_Map[v[currentIndex].sx][v[currentIndex].sy].getF() < g_Map[v[parentIndex].sx][v[parentIndex].sy].getF())
		{
			Coordinate temp = v[currentIndex];
			v[currentIndex] = v[parentIndex];
			v[parentIndex] = temp;

			//同步保存地图中该坐标在堆中的最新位置
			g_Map[v[currentIndex].sx][v[currentIndex].sy].iHeapPosition = currentIndex;
			g_Map[v[parentIndex].sx][v[parentIndex].sy].iHeapPosition = parentIndex;
			currentIndex = parentIndex;
			continue;
		}
		else
		{
			break;
		}
	}
}

//当堆中某元素的F值发生改变时，更新该元素在堆中的位置
void CHeap::newHeap(int position, Node**g_Map)
{
	int currentIndex = position;
	int parentIndex;
	//while (currentIndex > 0) //如果该元素新的F值比他的父节点的F值小，交换
	//{
	//	parentIndex = (currentIndex - 1) / 2;
	//	if (g_Map[v[currentIndex].sx][v[currentIndex].sy].getF()  < g_Map[v[parentIndex].sx][v[parentIndex].sy].getF())
	//	{
	//		Coordinate temp = v[currentIndex];
	//		v[currentIndex] = v[parentIndex];
	//		v[parentIndex] = temp;
	//		g_Map[v[currentIndex].sx][v[currentIndex].sy].iHeapPosition = parentIndex;
	//		g_Map[v[parentIndex].sx][v[parentIndex].sy].iHeapPosition = currentIndex;
	//		currentIndex = parentIndex;
	//		continue;
	//	}
	//	else
	//	{
	//		break;
	//	}
	//}
	Coordinate temp;
	//如果上面的循环没有执行，则无法判断该节点的最新F值的相对大小，
	//所以，此时需要把该节点移动到堆首删除掉，然后再在堆末尾添加该节点，最后程序再把该节点移动到堆中适当的位置处
	if (currentIndex == position)
	{
		while (currentIndex > 0) //把该节点移动到堆首
		{
			parentIndex = (currentIndex - 1) / 2;
			temp = v[currentIndex];
			v[currentIndex] = v[parentIndex];
			v[parentIndex] = temp;
			g_Map[v[currentIndex].sx][v[currentIndex].sy].iHeapPosition = currentIndex;
			g_Map[v[parentIndex].sx][v[parentIndex].sy].iHeapPosition = parentIndex;
			currentIndex = parentIndex;
		}
	}
	temp = v[0];
	removeFront(g_Map); //删除该节点
	push_back(temp, g_Map);//重新在堆中插入该节点
}

//返回堆中元素的个数
int CHeap::getSize()
{
	return v.size();
}