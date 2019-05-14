#pragma once
//画布单位
struct sCanvasCube  
{
	sCanvasCube()
	{
		for (int i = 0; i < 20; i++)
		{
			pPicList[i] = 0;
			PicType[i] = 0;  //0 CWFile  1 cObj 2 cText 3 cwfilep 4 objfather
			ShowMode[i] = 0;
		}
	}
//	RECT rect;//位置
//	BYTE* m_ShowData;
	cObjFather m_Data;
	//在画布中的单位, 最多有20个.多于20个的话...再说
	void* pPicList[20];
	int  PicType[20];  //0 CWFile  1 cObj 2 cText 3 cwfilep 4 objfather

	int  ShowMode[20];
	int  PicNum=0;   //当前帧的图片数
	int  PrePicNum=0;//前一帧的图片数
	int  PreCover=FALSE;//前一帧是否有覆盖
	int  NeedReDraw=FALSE;//是否需要重画
	BOOL Add(CWFile* p, int showmode, BOOL bMove, BOOL bCover, BOOL bChange);
	BOOL Add(cObjFather* p, int showmode, BOOL bMove, BOOL bCover);
	BOOL Add(cText* p, BOOL bMove);
	BOOL Add(cWfileP* p, int showmode, BOOL bMove, BOOL bCover, BOOL bChange);
	BOOL CheckNum(BOOL bCover);
	BOOL m_bCover;
	RECT m_Rect;
};
//画布
class cCanvas
{
public:
	int  m_Time=0;
	BOOL Init();
	BOOL Add(CWFile* pPic, int showmode, BOOL bCover = FALSE);
	BOOL Add(cText* pTex);
	BOOL Add(cObjFather* pObj, int showmode, BOOL bCover = FALSE);
	BOOL Add(cWfileP* p, int showmode  ,  BOOL bCover=FALSE);
	BOOL Free();
	//HDC m_MemDC; //显示文字时用于过渡
	sCanvasCube  m_CanvasCube[300];
	BOOL Show();
	void SwapBuf();
	HPEN hPen;
	//HDC m_hdc;
	~cCanvas();
	BOOL SetRedraw(RECT rect);
	void SetRedrawAll(BOOL ball=TRUE);
	BOOL m_NeedShowMap = FALSE;
private:
	BOOL m_bInit = FALSE;
	
};
