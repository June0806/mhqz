#pragma once
class cBmp    //障碍的地图
{
public:
	cBmp(){ m_pBmp = 0;};
	~cBmp();
	BOOL Load(CHAR* name);
	inline BOOL isOK(int x, int y);
	BOOL Free();
	BYTE* m_pBmp;
	int GetHeight(){ return m_Height; }
	int GetWidth(){ return m_Width; }
	string oldmap = "";
private:
	int m_Width;
	int m_Height;
	int m_capacity=0;
};

struct SMaskInfo
{
	int		startX;
	int		startY;
	int		width;
	int		height;
	int		mask_size;	// mask数据大小
};
//mask的区域
struct sMASKRect
{
	long x;
	long y;
	long w;
	long h;
	//long  Index;
};
class cMap
{
public:
	//
	BYTE* m_pTempBuffer = 0;
	int m_TempBufferNum = 0;
	BYTE* m_pMapJpgData = 0;
	int   m_MapJpgDataSize = 0;
	BYTE* m_pUnitNData = 0;
	int  m_UnitNDataSize = 0;
	vector<int>  m_MaskIDList;
	void CountMaskList();//计算屏幕中的mask
	void xMove(int x);
	void yMove(int y);
	int m_MapID = -1;
	int m_AddSpeed = 24;
	int m_xPreSpeed = 0;
	int m_yPreSpeed = 0;
	//锁定地图
	int m_LockMap;
	BOOL m_ReturnMap;
	int m_xLock;
	int m_yLock;

	BITMAPINFO * m_pBmpinfo;
	int  m_NowMapNum;
	BOOL SetCenter(float xHero, float yHero);
	BOOL Follow(cObj* pObj);
	//void FollowAlong(int speed,int xtarget,int ytarget);
	//void FollowStop(int xHero, int yHero);

	BOOL m_bMove;  //画面是否移动
	ifstream m_File;
	ifstream m_File2;//这个在主线程中用,另一个在子线程中用
	BYTE**  m_pBitmap;
	vector<int>    m_BitmapID;

	WORD*	m_pTexture;//[320*240];	// ARGB1555格式像素

	HBITMAP DIBtoDDB(WORD* data, HDC dc);
	void  ScreenToWorld(int x, int y, int& xOut, int& yOut);
	void  WorldToScreen(int x, int y, int& xOut, int& yOut);
	BOOL  Load(CHAR* FullPath, int mapid);
	BOOL Show(BYTE* pPic, RECT rect);
	inline	void CountAlpha(WPixel& color1, WPixel& color2, BYTE* pPic, int index2, int time);
	BOOL  MoveMap();
	
	void GetPoint();
	cMap();
	~cMap();
	struct SMap_Header
	{
		int				flag;		// 1296126040,即"XPAM"
		unsigned int	map_width;
		unsigned int	map_height;
	};
	struct SMaskHead
	{
		unsigned int	unknown;
		int				mask_num;
	};
	struct SBlockInfo
	{
		unsigned int	UnitN;	// 未知数据大小
		unsigned int	flag;
		unsigned int	size;
	};

	static unsigned int WINAPI ThreadProc(
		_In_  LPVOID lpParameter
		);
	int	  m_MaskNum;
	BYTE** m_pMaskDataDec;
	int*    m_align_width;



	HANDLE m_hThread;
	BOOL m_bLoadReady;
	BOOL m_bLoadReady2 = TRUE;
	//地图的偏移数据
	UINT* m_pBlockOffset;
	//mask的偏移数据
	UINT* m_pMaskDataOffset;
	//


	//地图块数
	int   m_NumBlock;
	int   m_HeightNum;
	int   m_WidthNum;
	//地图大小
	int   m_Width;
	int	  m_Height;

	sMASKRect* m_pMaskRect;

	//判断两个矩形是否相交
	BOOL isInRect(int x1, int y1, int w1, int h1, int x, int y, int w, int h);
	int		DecompressMask(void* in, void* out);

	//记录地图中心的位置
	float m_xCenter;
	float m_yCenter;
	//当前中心图块
	int m_xCenterNum = -1;
	int m_yCenterNum = -1;
	int m_xCenterOffset;
	int m_yCenterOffset;
	//
	int GetXCenter(){ return (int)m_xCenter; }
	int GetYCenter(){ return (int)m_yCenter; }
	void Free();
private:

	void WriteMapPixel(WORD* pBmp16, int width, int height, int pitch);
	//读取地图块
	void ReadBlock(int block_offset, WORD* map);
	void ReadMask(int offset, int mask_index);
	void Mask(int x, int y, int Width, int Height, int offset, int mask_index, BYTE* lpback);

	WORD*			m_pMapBmp;				// ARGB1555格式像素

	//JPEG数据处理器
	BYTE*	JpgHandler(BYTE* Buffer, int inSize, UINT* outSize);

	//解压JPEG
	void ByteSwap(WORD& value)
	{
		WORD tempvalue = value >> 8;
		value = (value << 8) | tempvalue;
	}
};
class CMainState;
struct sRangeTr
{
	string m_name="";
	int m_x;
	int m_y;
	int m_range;
	BOOL Check(int x, int y);
};
struct sTransmit:public sRangeTr
{
	//目的地名字
	string name; 
	//目的地坐标
	int m_xTarget;
	int m_yTarget;
};
struct sTriggerRange:public sRangeTr
{
	cTaskFather* m_p = 0;
	int m_ret;
	void Save(ofstream& File);
	void Load(ifstream& File);
	void Set(string name,int x, int y, int range, cTaskFather* p, int ret);
};
struct sNpc;
class cMyMap
{
public:	
	void CheckChuanSong();
	//寻找某张地图上的随机空位
	cBmp m_BmpTest;
	void AutoGetPos(int& xout, int& yout, string mapname="");
	//该场景的物品等级和几率
	int m_Weaponlv;
	int m_WeaponRate;

	void AddNpcSmall(string name, int id, int x, int y, int direction, cTaskFather* p = 0, string scriptname = "无", int stage = 0, BOOL NeedFind = TRUE);

	void ShadowNpc(string name, BOOL NeedShow, BOOL NeedFind = TRUE);
	void RemoveNpc(string name,BOOL NeedFind=TRUE);
private:
	void RemoveNpc(int id, BOOL NeedFind);
public:
	void RemoveNpc(int xPos, int yPos, string name , BOOL NeedFind);
	void RemoveNpc(cObj* pObj,BOOL Needfind);
	void RemoveAllNpc();
	int  m_MapNumber=-1;
	int   m_MapType; //场景类型 
	int   m_NumOfEmemyType;
	int   m_EnemyLv;
	int   m_EnemyList[10];
	string m_MapName;
	CWFile m_SmallMap;
	float  m_bili;
	//CWFile m_Transmit;
	cMyMap();
	~cMyMap();
	BOOL CheckAlong(int xPos, int yPos, int xTarget, int yTarget);
	BOOL Free();
	BOOL LoadMap(long x,long y,const CHAR* Name,BOOL bFly=TRUE);
	BOOL Show(BYTE* pPic, RECT rect){return m_pMap->Show(pPic, rect);}
	
	cMap* m_pMap;
	cBmp* m_pBmp;
	vector<cObj*>  m_pNpc;
	vector<CWFile*> m_pJianZhu;
	vector<DWORD> m_JianZhuID;
	void AddJiaJu(sJiaJu* jiaju);
	void RemoveJiaJu(sJiaJu* jiaju);
	BOOL Init();
	int m_numberOfNpc;
	sTransmit* m_pTransmit;
	int m_numberOfTransmit;
	int m_numofJianZhu;
	void SetLock(string mapname = "");
	void SetLock(BOOL b);
	void Save(ofstream& File);
	void LoadMap(ifstream& File);
	void AddTriggerRange(sTriggerRange& data);

	void AddNpc(sMapObjTrigger* data, BOOL needfind = FALSE);
	void AddNpc(sNpc* npc, cTaskFather* p = 0, string scriptname = "无", int stage = 0, BOOL NeedFind = TRUE);
	
private:
	void AddNpc(string name, int id, int x, int y, int direction, cTaskFather* p = 0, string scriptname = "无",
		int stage = 0, BOOL NeedFind = TRUE, int weapontype = -1, int weaponid = -1, sChangeColorData2* ranse = 0, int positionID = 0);

	vector<string> m_LockList;
	BOOL m_bInit = FALSE;
	BOOL m_bLock = FALSE;//禁止转换地图
	vector<sTriggerRange> m_RangeTriggerList;
};

