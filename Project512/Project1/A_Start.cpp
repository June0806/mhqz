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
	//���Ŀ������ϰ���,���ƶ�Ŀ����λ��
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

	// 1. ����ʼ����ӵ������б�
	m_iOpenList.push_back(coordinate, g_Map);
	g_Map[Xstart][Ystart].isOpen = IS_OPEN;

	//2.�ظ����¹���
	int minF = g_Map[Xstart][Ystart].iF; //�ñ�����ʾ��СFֵ
	m_isFind = false; //�ñ������������Ŀ��λ���Ƿ���ӽ��˹ر��б�

	while (!m_iOpenList.empty()) //�������б�Ϊ��ʱ�� ѭ��ֹͣ
	{
		//a).Ѱ�ҿ����б� ��F��͵ĸ��ӣ����ǳ���Ϊ��ǰ��
		minCoord = m_iOpenList.begin();
		minF = g_Map[minCoord.sx][minCoord.sy].getF();
		if ((minCoord.sx == Xend) && (minCoord.sy == Yend)) //���Ŀ��λ�ñ���ӵ��˹ر�������ֱ������ѭ��
		{
			m_isFind = true;
			break;
		}

		//b).�ѱ����õ��ĵ�ǰ���л����ر��б�
		m_iOpenList.removeFront(g_Map); //ɾ�������б��е�Fֵ��͵ĸ���
		g_Map[minCoord.sx][minCoord.sy].isOpen = IS_CLOSE;//�Ѹø��ӵı������Ϊ���ڹر��б��С�


		int i = -1, j = -1;
		int x = 0, y = 0;
		for (i = -1; i < 2; ++i)  //������ǰ������Χ������8������
		{
			for (j = -1; j < 2; ++j)
			{
				x = minCoord.sx + i;
				y = minCoord.sy + j;

				if ((x < 0) || (y < 0) || (x > m_Width - 1) || (y > m_Height - 1)) //* ��������Ļ֮�⣬�Թ�
					continue;
				if (g_Map[x][y].iColor == MAP_WALL) //* ����Ϊǽ������ͨ�����Թ�
				{
					continue;
				}
				if (g_Map[x][y].isOpen == IS_CLOSE) //* �����Ѿ��ڹر��б����ˣ��Թ�
					continue;
				if (g_Map[x][y].isOpen == IS_UNKNOWN) //* ����ø���δ���뵽�����б�
				{
					coordinate.sx = x;
					coordinate.sy = y;
					g_Map[x][y].father = minCoord;  //��ǰ����Ϊ��һ��ĸ��ڵ�
					 xx1 = (Xend - x) * 5;
					 yy1 = (Yend - y) * 5;
					g_Map[x][y].iH =(int)sqrt( xx1*xx1 + yy1*yy1);
					//����G
					if ((i == 0) || (j == 0))
					{
						g_Map[x][y].iG = g_Map[minCoord.sx][minCoord.sy].iG + 5;
					}
					else
					{
						g_Map[x][y].iG = g_Map[minCoord.sx][minCoord.sy].iG + 7;
					}
	
					//����F
					g_Map[x][y].iF = g_Map[x][y].getF();
					//���뿪���б�
					g_Map[x][y].isOpen = IS_OPEN;
					//���뿪���б�
					m_iOpenList.push_back(coordinate, g_Map);  //�������뵽�����б���
					continue;
				}

				if (g_Map[x][y].isOpen == IS_OPEN) //* ����ø����Ѿ��ڿ����б���
				{
					int newG;
					if ((i == 0) || (j == 0))  //�����µ�Gֵ
					{
						newG = g_Map[minCoord.sx][minCoord.sy].iG + 5;
					}
					else
					{
						newG = g_Map[minCoord.sx][minCoord.sy].iG + 7;
					}
					if (newG < g_Map[x][y].iG) //����µ�Gֵ����ǰ�ĸ���
					{
						g_Map[x][y].father = minCoord; //�Ѹø�ĸ��ڵ��Ϊ��ǰ��
						int position = g_Map[x][y].iHeapPosition; //�õ��ýڵ��ڶ��е�λ��
						g_Map[x][y].iG = newG;
						g_Map[x][y].iF = g_Map[x][y].getF();
						m_iOpenList.newHeap(position, g_Map);             //����Fֵ�����˱仯�����´˽ڵ��ڶ��е�λ��
					}
				}
			}
		}
	}
	if (m_isFind) //3).����ҵ�·���ˣ�����·��
	{
		Node tempNode;
		tempNode.father = minCoord;
		pObj->m_Path.clear();//���
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
	else //û�ҵ�·���Ĵ���
	{

	}
	return FALSE;
}
//��׼����cMyMap�е�����
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
	if (!m_iOpenList.empty()) //��տ����б�
		m_iOpenList.clear();

	return TRUE;
}

cFindPath::~cFindPath()
{
	Free();
}





//���캯��
CHeap::CHeap()
{
}

//�ж϶�����Ƿ�Ϊ��
bool CHeap::empty()
{
	if (v.size() == 0)
		return true;
	else
		return false;
}

//��ն����
void CHeap::clear()
{
	if (v.size() != 0)
		v.clear();
}

//���ض�����еĵ�һ��Ԫ�أ���Fֵ��͵�Ԫ�أ�
Coordinate CHeap::begin()
{
	return v[0];
}

//ɾ������ѵĶ���Ԫ�أ���Fֵ��͵�Ԫ�أ�
void CHeap::removeFront(Node**g_Map)
{
	if (v.size() == 0)
		return;

	g_Map[v[v.size() - 1].sx][v[v.size() - 1].sy].iHeapPosition = 0;
	g_Map[v[0].sx][v[0].sy].iHeapPosition = -1;

	v[0] = v[v.size() - 1];      //�����һ��Ԫ�ذѵ�һ��Ԫ�ظ��ǵ�����Ϊɾ��
	v.pop_back();                //ɾ������β��Ԫ��



	int currentIndex = 0;
	while (currentIndex < v.size()) //���µĶ���Ԫ�ط��ڶ����ʵ���λ��
	{
		int leftChildIndex = 2 * currentIndex + 1;
		int rightChildIndex = 2 * currentIndex + 2;

		//�Ѿ�����ײ㣬����
		if (rightChildIndex > v.size())
			break;
		int minIndex = leftChildIndex;

		//���������ӣ��ҳ��������ӽڵ���Fֵ��͵�Ԫ��
		if (rightChildIndex<v.size() && (g_Map[v[minIndex].sx][v[minIndex].sy].getF()  > g_Map[v[rightChildIndex].sx][v[rightChildIndex].sy].getF()))
		{
			minIndex = rightChildIndex;
		}

		//�����ǰ�ڵ��Fֵ ���� �����ӽڵ��Fֵ���򽻻�
		if (g_Map[v[currentIndex].sx][v[currentIndex].sy].getF() > g_Map[v[minIndex].sx][v[minIndex].sy].getF())
		{
			Coordinate temp = v[minIndex];
			v[minIndex] = v[currentIndex];
			v[currentIndex] = temp;

			//ͬ�������ͼ�и������ڶ��е�����λ��
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

//���ض��е���СFֵ
int CHeap::getMinF(Node**g_Map)
{
	if (v.size() > 0)
		return g_Map[v[0].sx][v[0].sx].getF();
	else
		return 0;
}


//����������µ�Ԫ�أ��ڵ㣩
void CHeap::push_back(Coordinate element, Node**g_Map)
{
	v.push_back(element);//���½ڵ���ӵ��ѵ�ĩβ
	int currentIndex = v.size() - 1;
	g_Map[v[currentIndex].sx][v[currentIndex].sy].iHeapPosition = currentIndex; //����������ڶ��е�λ��

	while (currentIndex > 0) //���ϵ������ĸ��ڵ�Ƚϣ�ֱ�����½ڵ��Fֵ�������ĸ��ڵ��FֵΪֹ ���� ���½ڵ㵽�˶���
	{
		int parentIndex = (currentIndex - 1) / 2;
		if (g_Map[v[currentIndex].sx][v[currentIndex].sy].getF() < g_Map[v[parentIndex].sx][v[parentIndex].sy].getF())
		{
			Coordinate temp = v[currentIndex];
			v[currentIndex] = v[parentIndex];
			v[parentIndex] = temp;

			//ͬ�������ͼ�и������ڶ��е�����λ��
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

//������ĳԪ�ص�Fֵ�����ı�ʱ�����¸�Ԫ���ڶ��е�λ��
void CHeap::newHeap(int position, Node**g_Map)
{
	int currentIndex = position;
	int parentIndex;
	//while (currentIndex > 0) //�����Ԫ���µ�Fֵ�����ĸ��ڵ��FֵС������
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
	//��������ѭ��û��ִ�У����޷��жϸýڵ������Fֵ����Դ�С��
	//���ԣ���ʱ��Ҫ�Ѹýڵ��ƶ�������ɾ������Ȼ�����ڶ�ĩβ��Ӹýڵ㣬�������ٰѸýڵ��ƶ��������ʵ���λ�ô�
	if (currentIndex == position)
	{
		while (currentIndex > 0) //�Ѹýڵ��ƶ�������
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
	removeFront(g_Map); //ɾ���ýڵ�
	push_back(temp, g_Map);//�����ڶ��в���ýڵ�
}

//���ض���Ԫ�صĸ���
int CHeap::getSize()
{
	return v.size();
}