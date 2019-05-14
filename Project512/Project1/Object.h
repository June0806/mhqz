#pragma once
#include "global.h"
const int c_NumFuZhu =  13;
struct sSkill;
//WDF�ļ�ͷ,��¼���ļ�������(WDF),���ļ�������,���ļ��б��λ��
struct WPixel
{
	union
	{
		WORD color;								// 16bit �߲�
		struct
		{
			WORD blue : 5;						// 5bit ��ɫ
			WORD green : 6;						// 6bit ��ɫ
			WORD red : 5;							// 5bit ��ɫ
		};
		struct
		{
			BYTE l;
			BYTE h;
		};
	};
	WPixel(){};
	// 5 5 5 ��ʽת 5 6 5
	void operator =(WORD color);
	void operator =(DWORD color1);
	WPixel(int r, int g, int b) : red(r >> 3), green(g >> 2), blue(b >> 3) {}
};
enum eMengPai
{
	eMP_���ƹٸ�,
	eMP_������,
	eMP_Ů����,
	eMP_����ɽ,
	eMP_ʨ����,
	eMP_ħ��կ,
	eMP_��˿��,
	eMP_���ܵظ�,
	eMP_��������,
	eMP_��ׯ��,
	eMP_����ɽ,
	eMP_�칬,
	/*eMP_�貨��,
	eMP_�޵׶�,
	eMP_��ľ��,*/
	eMP_��,
	eMP_���
};

enum ePetID
{
	e����,
	e�󺣹�,
	e����,
	e����,
	e��ͽ,
	eǿ��,
	e��ë��,
	e������,
	eɽ��,
	eҰ��,
	e���ù�,
	e��ͷ��,
	e��󡾫,
	e�ϻ�,
	e����,
	e����,
	eţ��,
	eС��Ů,
	eҰ��,
	e��,
	eϺ��,
	eз��,
	e��ة��,
	e���ӹ�,
	e֩�뾫,
	e���ܾ�,
	e��ʬ,
	eţͷ,
	e����,
	e������,
	e��������,
	e�Ŵ�����,
	e����,
	e��ɽ����,
	e���,
	e�콫,
	e����ս��,
	e�粮,
	e���,
	e����,
	e��ʦ,
	e��������,
	eܽ������,
	eѲ������,
	e��������,
	e����,
	e��,
	e��Ѫ��,
	e��ƿŮ�,
	e�ɷ�Ů�,
	e���Ů�,
	e�������,
	e������,
	e���,
	eҹ��ɲ,
	e��ħ��,
	e���컢,
	eҰ��,
	e���ȷ�,
	e���㽫��,
	e̤����,
	e��������,
	e����,
	e����ɡ,
	e������,
	e������,
	e�����˳�,
	e����,
	e����,
	e����,
	e����,
	e��ˮҹ��,
	e��ë����,
	eǧ������,
	eϬţ������,
	eϬţ����,
	e������,
	e��«����,
	eè����,
	eè��,
	e��,
	e����,
	eЫ�Ӿ�,
	e������,
	e�����˲�����,
	e����,
	e��ü���,
	e������Գ,
	e���޿��ܹ�,
	e��������,
	e������,
	e���޿�����,
	e�����޺�,
	e����ɳ��,
	e��������,
	e��������è,
	e�������,
	e��������,
	e��������,
	e������¹,
	e����������,
	e��������,
	e�Ʋ�ͯ��,
	e�������,
	e������ţ,
	e������,
	e��������,
	e��������,
	eСëͷ,
	eСѾѾ,
	eС����,
	eС��Ů,
	eС����,
	eСħͷ,
	e�����,
	eɳ����,
	e���,
	e��˽�,
	e���꾫,
	e֪����,
	e����,
	e��̫��,
	e�ܽ���,
	e��������,
	e����,
	eС���,
	e����,
	e������,
	eת����,
	e��ͷ����,
	e����,
	e��������,
	e������,
	eţħ��,
	e��Ԫ����,
	e������ʦ,
	e�׾���,
	e��ө����,
	e�,
	e��ҧ��,
	e�ز���,
	e�������,
	e�ն���ʦ,
	e�����,
	eХ��Ȯ,
	e��ӿ����,
	e������,
	e�̻�ռ��ʦ,
	e�׿���,
	eХ��Ȯ2,
	e������ȸ,
	e�������,
	e��������,
	e��������,
	e�������,
	e��������,
	e��ɫ¹,
	e�ֹ�Ѳ��,
	e��������,
	e����⨺�,
	e����ͯ��,
	e���ӻ���
};
struct sAddPoint
{
	void Save(ofstream& File);
	void Load(ifstream& File);
	int atk=4;
	int def=0;
	int hp=1;
	int mp=0;
	int spd=0;
	int idle=0;
	void Set(int atk2, int def2, int mp2, int hp2, int spd2)
	{
		atk = atk2;
		def = def2;
		hp = hp2;
		mp = mp2;
		spd = spd2;
	}
	void operator=(sAddPoint& target)
	{
		atk = target.atk;
		def = target.def;
		hp = target.hp;
		mp = target.mp;
		spd = target.spd;
		idle = target.idle;
	}
};


struct WDataFileHeader {
	DWORD id;
	int number;
	unsigned offset;
};
//���ļ���ID λ�� ��С 
struct WDataFileIndex {
	DWORD uid;
	DWORD offset;
	DWORD size;
	DWORD space;
};

//was�ļ�ͷ
struct WASFILEHEADER{
	WORD wastitle;			//�ļ���ʾ SP
	WORD szHheard;			//�ļ�ͷ��С
	WORD nDirection;		//�����ķ�����
	WORD nFrame;			//ÿ�������֡��
	WORD nWidth;			//�����Ŀ��
	WORD nHeigh;			//�����ĸ߶�
	short xCenter;			//���������� X
	short yCenter;			//���������� Y
};
class cMap;
class cMyMap;
//����ͷ
struct DATEHEARD{
	int FramX;				//ͼƬ���� X
	int FramY;				//ͼƬ���� Y
	int Width;				//ͼƬ���
	int Height;				//ͼƬ�߶�
};
class CWFile;
class cObjFather
{
public:
	int	m_x=0;
	int	m_y=0;
	int	m_xOffset=0;
	int	m_yOffset=0;
	int	m_xOffset2=0;
	int	m_yOffset2=0;
	int	m_ShowHeight=0;
	int	m_ShowWidth=0;
	int	m_direction=1;
	int	m_bTwinkle=0;
	int m_FrameGap  =10;
	int	m_NextFrame=0;
	int	now_frame=0;
	int	m_bShadow=0;
	int	m_bShine=0;
	int m_Width=0;
	int m_Height=0;
	void SetFrame(int frame)
	{
		if (now_frame != frame)m_bMove = TRUE;
		now_frame = frame;
	}
	int  GetFrame(){ return now_frame; }


	int GetShowWidth(){ if (m_ShowWidth) return m_ShowWidth; else return m_Width; }
	void SetShowWidth(int k);
	int GetShowHeight(){ if (m_ShowHeight)return m_ShowHeight; else return m_Height; }
	void SetShowHeight(int k);

	void SetxOffset(int xOff);
	void SetyOffset(int yOff);
	void SetxOffset2(int xOff2);
	void SetyOffset2(int yOff2);
	int  GetxOffset(){ return m_xOffset; };
	int  GetyOffset(){ return m_yOffset; };
	int  GetxOffset2(){ return m_xOffset2; };
	int  GetyOffset2(){ return m_yOffset2; };

	int GetHeight(){ return m_Height; }
	int GetTrueWidth(){ return m_Width; }
	void SetHeight(int h){ m_Height = h; }
	void SetWidth(int w){ m_Width = w; }

	int GetX(){ return m_x; }
	int GetY(){ return m_y; }
	void SetXY(int x, int y){ m_x = x; m_y = y;}
	void SetX(int x){ m_x = x; }
	void SetY(int y){ m_y = y; }


	void Init(cObjFather* pObj,int alpha=100); //����һ����pObjһ�����ͼƬ,֮ǰ��ͼƬ�ᱻ�ͷŵ�
	void Reset(cObjFather* pObj, int alpha = 100);
	void Reset2(CWFile* pic);
	void Draw(cObjFather* pObj,int xpos=-1,int ypos=-1,int alpha=100);  //��ͼƬ�ϻ��µ�ͼ
	void GetRect(RECT& rect1);
	BYTE* m_pData=0;
	BYTE* m_pAlpha=0;
	BOOL m_NeedShow=TRUE;
	BOOL m_bMove=FALSE;
	RECT m_PreShowRect;
	~cObjFather();
	int m_OldDataSize=0;
};
struct sChangeColorData;
class CSprite
{
public:
	cObjFather* m_pOwner;
	int GetDataID(){ return GetOffsetID(m_Frame, m_pOwner->now_frame, m_MaxDirection, m_pOwner->m_direction); }
	int GetHeight(){ return m_Height; }
	int GetWidth(){ return m_Width; }
    int GetDirectionNum(){ return m_MaxDirection; };
	
	DATEHEARD* m_pDataOffsetList;
	HBITMAP DIBtoDDB(WORD* data, BITMAPINFO*  pBmpInfo);
	CSprite(void);
	~CSprite(void);
	BOOL Load(void* lp, long size, int type, sChangeColorData* pChangeColorData = 0);
	BOOL LoadBMP(string path);
	int GetMaxFrame(){ return m_Frame; }

	int m_kx;				// �ؼ��� X
	int m_ky;				// �ؼ��� Y
	unsigned char** lpAlpha;	// alpha����ָ��
	unsigned char** lpDate;		// ͼ������ָ��
	void FreeDatelp(void);
private:
	int m_Frame;			// ÿ����֡��
	inline BOOL CountCross(int xPos, int yPos, RECT destrect, RECT& picrectout, RECT& nowrectout, int& xOffPic, int& yOffPic, int& xOffDes, int& yOffDes);
	int pitch;					// ɨ���п��


	int m_Width;			// ���
	int m_Height;			// �߶�
	int m_MaxDirection;		// ������

	

public:
	BOOL ShowOnScreen(BYTE* Pic, RECT rect);
	BOOL Show(BYTE* Pic, RECT rect, cMyMap* pMap);
	
	int  m_Bottom = -1;
	BOOL isInMask(int x, int y, int w, int h, int xMask, int yMask, int wMask, int hMask);
	BOOL CheckMask(int xStart, int yStart, int Width, int Height,int xOff2,int yOff2, cMap* pmap, int a,BYTE* pPic);


	BOOL ShowPart(int xOffset, int yOffset,int x, int y, int Width, int Height, int direction, BOOL bStatic = 0);
	BOOL m_bLoad=FALSE;
private:
	void mread(void* Dst, unsigned int size, unsigned int n);
	BYTE* flp;
	void mcreat(void* ilp, unsigned int size);
	void mseek(long offset);
	BYTE* fstr;
public:
	void mclose(void);
private:
	unsigned int fsize;
};

class CPicture:public cObjFather
{
public:
	//ͼƬ�ϴε���������
	int m_xold = 0;
	int m_yold = 0;
	int GetDataID(){ return GetOffsetID(Spr.GetMaxFrame(), now_frame, Spr.GetDirectionNum(), m_direction); }
	//DATEHEARD* GetDataID2(){ Spr.m_pDataOffsetList[GetDataID()] };
	CPicture(void);
	~CPicture(void);
	// ��������
	void Load(void* pdate, int size, int type, sChangeColorData* pChangeColorData = 0);
	void Free();	
	CSprite Spr;

	BOOL Show(BYTE* Pic, RECT rect, cMyMap* pMap)
	{
		if (!Spr.m_bLoad)return TRUE;
		Spr.m_pOwner = this;
		return Spr.Show(Pic, rect, pMap);
	}
	void ShowOnScreen(BYTE* pPic, RECT rect)
	{
		if (!Spr.m_bLoad)return;
		Spr.m_pOwner = this;
		Spr.ShowOnScreen(pPic, rect);
	}

};
class CWdf
{
public:
	CWdf(){ m_pIndex = 0; }
	~CWdf(){ Free(); }
	BOOL Free();
	BOOL Open(const CHAR* FileName);
	CHAR m_Name[256];
	WDataFileIndex* SearchFile(DWORD id);
	ifstream* GetFileHandle() { return &m_File; }
	WDataFileIndex *m_pIndex;
private:
	UINT m_Number;
	ifstream m_File;
};

class CWFile
{
public:
	void Save(string name);
	BYTE *lp=0;
	INT m_lpSize=0;
	int m_PosType=1;
	RECT m_PreShowRect;
	

	void SetShine(BOOL needshow){/*if(needshow!=m_Picture.m_bShine)*/ m_bMove = TRUE; m_Picture.m_bShine = needshow; }
	int GetCenter(){ return GetX() + (GetWidth() >>1); }
	int GetX(){ return m_Picture.GetX(); }
	int GetY(){ return m_Picture.GetY(); }
	void SetXY(int x, int y){ m_Picture.SetXY(x, y); m_bMove = TRUE; }
	void SetX(int x){ if(m_Picture.GetX() != x)m_bMove = TRUE;  m_Picture.SetX(x); }
	void SetY(int y){ if (m_Picture.GetY() != y)m_bMove = TRUE; m_Picture.SetY(y); }
	int GetWidth(){ return m_Picture.GetTrueWidth(); }
	int GetHeight(){ return m_Picture.GetHeight(); }
	int GetShowWidth(){ return m_Picture.GetShowWidth(); }
	void SetShowWidth(int k){ m_Picture.SetShowWidth(k); }
	int GetShowHeight(){ return m_Picture.GetShowHeight(); }
	void SetShowHeight(int k){ m_Picture.SetShowHeight(k); }
	int GetDirectionNum(){ return m_Picture.Spr.GetDirectionNum(); }
	void SetxOffset(int xOff){ m_Picture.SetxOffset(xOff); }
	void SetyOffset(int yOff){ m_Picture.SetyOffset(yOff); }
	void SetxOffset2(int xOff2){ m_Picture.SetxOffset2(xOff2); }
	void SetyOffset2(int yOff2){ m_Picture.SetyOffset2(yOff2); }
	int  GetxOffset(){ return m_Picture.m_xOffset; };
	int  GetyOffset(){ return m_Picture.m_yOffset; };
	int  GetxOffset2(){ return m_Picture.m_xOffset2; };
	int  GetyOffset2(){ return m_Picture.m_yOffset2; };
	BOOL m_bMove;
	DWORD m_NowID=0;
	
	BOOL m_isStatic;
	CWFile();
	~CWFile();
	void GetPicRange(RECT& rect);//�õ�ͼƬ�ķ�Χ
	RECT GetPicRange();//�õ�ͼƬ�ķ�Χ
	BOOL isSelectOn(int x, int y);
	void SetStatic(BOOL bIsStatic){ m_isStatic = bIsStatic; }
	BOOL m_NeedShow=TRUE;
	
	int m_Type;
	BOOL LoadBMP(string path);
	BOOL Load(DWORD uid, sChangeColorData* pChangeColorData=0);
	void LoadWav(DWORD uid, int position=0,BOOL bPlay=TRUE);
	void PlayWav();
	void GetFileP(DWORD uid,ifstream*& f);
	BOOL Show(BYTE* Pic, RECT rect, cMyMap* pMap)
	{
		if (!m_NeedShow)return TRUE;
		return m_Picture.Show(Pic, rect, pMap);
	}
	
	void ShowOnScreen(BYTE* Pic, RECT rect){ m_Picture.ShowOnScreen(Pic, rect); };
	void SubFrame(){ m_Picture.m_NextFrame = 0; (m_Picture.GetFrame() > 0) ? (m_Picture.SetFrame(m_Picture.GetFrame() - 1)) : m_Picture.SetFrame(0); }
	void SetFrame(int frame);
	int  GetNowFrame(){ return m_Picture.GetFrame(); }
	int  GetMaxFrame(){ return m_Picture.Spr.GetMaxFrame(); }
	BOOL  GetWillNextFrame();
	BOOL Free();
	BOOL CloseWDF();
	static	BOOL Open(const CHAR* FileName);
	static  CWdf* m_pWdf[64];

	CPicture m_Picture;
	cMyWav m_Wav;
	//BOOL m_bStatic;
	BOOL NextFrame();
	BOOL m_bCircle=TRUE;
	BOOL PreFrame();
	void SetDirection(int dir);
	BOOL m_ShowOnce = FALSE;//ֻ��ʾһ��
private:
	////�ļ����Ͷ���
	//struct ExsDate
	//{
	//	WORD  hdw;  //ͷ�����ֽ�
	//	DWORD sst;  //���߸��ֽڿ�ʼ���ĸ��ֽ�
	//	DWORD nst;  //�ھŸ��ֽڿ�ʼ���ĸ��ֽ�
	//	DWORD dss;  //���������ֽڿ�ʼ���ĸ��ֽ�
	//	WORD sss;	//������ֽڿ�ʼ�Ķ����ֽ�
	//	BYTE dsg[3];//���������ֽ�
	//};
	//int setfiletype(ExsDate esdate);
	
	int  m_Direction;
	DWORD m_Offset;
	DWORD m_Size;
};
#define tile_num(x,y) ((y)*map_w+(x))  //�� x,y ����ת��Ϊ��ͼ�Ͽ�ı��
#define tile_x(n) ((n)%map_w)          //�ɿ��ŵó� x,y ����
#define tile_y(n) ((n)/map_w)
enum eBuffType
{
	eBuffType��ӡ,
	eBuffType����,
	eBuffType����,
	eBuffType��,
	eBuffType����
};
struct sBuff
{
	int ID;   //BUFF��ID
	DWORD MagicID;
	//int Type; 
	int times;//ʣ��ʱ��
	//buf�ķ�����ʾЧ��
	int bufPos;  //λ��
	CWFile data;
	BOOL bBuffup; //�����滹������
	int userID;
	int targetID;
	eBuffType EffectType;//���BUFF������, 0 ����  1 �쳣 2 �� 3 �����ٻ�  
	int damage;
	int damage2;
	int skilllv = -1;
};

class cWfileP:public cObjFather
{
public:
	int GetNowFrame(){ return now_frame; }
	int GetWidth(){ return m_pFile->GetWidth(); };
	int GetCenter();
	vector<int> m_oldmask;
	//ͼƬ�ϴε���������
	int m_xold = 0;
	int m_yold = 0;
	int GetDataID(){return GetOffsetID(m_pFile->m_Picture.Spr.GetMaxFrame(), now_frame, m_pFile->m_Picture.Spr.GetDirectionNum(),m_direction);}
	RECT m_PreShowRect;
	void operator=(cWfileP& file);
	
	cWfileP();
	void Set(CWFile* p);
	BOOL ShowOnScreen(BYTE* pPic, RECT rect)
	{
		if (!m_NeedShow)return TRUE;
		if (!m_pFile)return FALSE;
		CSprite& spr = m_pFile->m_Picture.Spr;
		if (!spr.m_bLoad)return TRUE;
		spr.m_pOwner = this;
		spr.ShowOnScreen(pPic, rect);
		return TRUE;
	}
	BOOL Show(BYTE* pPic, RECT rect, cMyMap* pMap)
	{
		if (!m_NeedShow)return TRUE;
		if (!m_pFile)return FALSE;
		CSprite& spr = m_pFile->m_Picture.Spr;
		if (!spr.m_bLoad)return TRUE;
		spr.m_pOwner = this;
		return spr.Show(pPic, rect, pMap);
	}
	BOOL m_NeedShow;
	
	BOOL m_bStatic;

	BOOL NextFrame();//���֡���ı�,����true���򷵻�false
	RECT GetPicRange();
	CWFile* m_pFile;
};
class cCanvas;
class cText :public cObjFather
{
public:
	HPEN m_hPen;
	HFONT m_hFont;
	void SetColor(DWORD color);
	void SetString(char* str);
	void SetString(string str);
	void SetString(string str, DWORD color);
	void SetString(int str);
	void SetString(INT64 str);
	void UpdateData();
	//0 ����,1 ���� 2 ���
	void SetStyle(int style){ m_Style = style; }
	string GetString(){ return m_Text; }
	BOOL DrawAlphaEx(BYTE* Pic, RECT rect, cMap* pMap);
	void Show(cCanvas* pCanvas);
	cText();
	~cText();
	void SetBackColor(DWORD color);
	void LockWidth(int Width){ m_MaxWidth = Width; }
	void CountSize(SIZE& size);
	void SetXCenter(int xCenter){ m_xCenter = xCenter; };//�������ֵ�����
	BOOL m_bShowOnScreen = TRUE;
	void UpdateXCenter(){ m_x = m_xCenter - m_Width / 2; };
	BOOL m_bBold = FALSE;
	void SetTagColor(int starttag, int endtag, DWORD color);
	void SetTransparentColor(int startpos, int endpos, DWORD color);
	void UnTransparentColor(int startpos, int endpos, DWORD transparentcolor);
	//ͼƬ��ʾҪ8�ֽڶ���,����m_Width�����ֵ�Ǵ��ڵ�����ʾ��ȵ�.m_tuewidth�������ǰ�Ŀ��
	int  GetTrueWidth(){return m_tuewidth;} 
	void SetTime(int time){ m_time = time; m_NeedShow = TRUE; }
private:
	int m_tuewidth;
	DWORD m_color=0;
	DWORD m_BacColor=0;
	string m_Text;
	int m_Style=0;
	BYTE* m_pData2 = 0;
	int m_Size=0;
	int m_MemSize2 = 0;;
	int m_MemSize=0;
	int m_MaxWidth = 0;
	int m_xCenter=0;
	int m_time=0;
	void ChangeColor(DWORD originalcolor, DWORD outcolor);
};
struct sNpcData
{
	int  m_ModeId;
	string  m_name;
	DWORD	m_stand;
	DWORD	m_walk;
	DWORD	m_dialog;
};
struct sItemData
{
	string m_name;
	DWORD	m_smallhead;
};
struct sFabaoData
{
	string m_name;
	DWORD	m_smallhead;
};


struct sSkillData
{
	string m_name;
	DWORD	m_smallhead;
	DWORD	m_bighead;
	DWORD	m_majic;
	DWORD	m_defend;
};


#define WORKTYPE_ATTACK    1    //����
#define WORKTYPE_MAGIC     3    //ʩ��
#define WORKTYPE_APPROACH  4    //�ӽ�
#define WORKTYPE_DEFEND    5    //��
#define WORKTYPE_SUFFER    6    //�ܵ�������
#define WORKTYPE_RETURN    7    //����
#define WORKTYPE_MAGICSUFFER  8  //�ܵ�ħ������
#define WORKTYPE_MAGICATTACK  9   //����������������
#define WORKTYPE_FLY       10     //�ٻ��ޱ�����
#define WORKTYPE_FALLDOWN  11  //��ɫ����
#define WORKTYPE_CATCH  12  // ץ��
#define WORKTYPE_AVOID  13  // �ر�
#define WORKTYPE_MAGIC2   14    //ʹ�����ⷨ�� �������,����
#define WORKTYPE_ITEM     15    //�õ���
#define WORKTYPE_SUMMON   16    //�ٻ�
#define WORKTYPE_GUARD   17    //����
#define WORKTYPE_COUNTER 18  //����

#define INDEXTYEP_CHARACTER 100
#define INDEXTYEP_PET       101
#define INDEXTYEP_NPC       102

enum PropertyType
{
	typeAtkZZ,
	typeDefZZ,
	typeHPZZ,
	typeMagZZ,
	typeSpdZZ,
	typeAvoidZZ
};

struct sCharacterData;
class cMyScript;
struct sFightOffset;

class cInterfaceChannel;
class cCanvas;
struct sTextLine   //һ��
{
	cWfileP m_Face[10];
	int     m_xFace[10];
	int m_NumFace;
	cText m_line;
	int m_Height;
};
struct sColorTag
{
	int m_linenum; //��������
	int m_start;  //��������
	DWORD m_Color; //��ɫ
};
struct sFaceText //һ�� �����������
{
	vector<sColorTag> m_ColorTag;
	int m_Height=0;
	int m_Gap = 0;
	int m_FirstAdd;//��������
	void Show(cCanvas* pCanvas);
	void SetPos(int x, int y);
	cWfileP m_Channel; //Ƶ��
	cText m_Name;  //����
	int m_LineNum;  //����
	void SetColor(DWORD color){ for (int i = 0; i < 7; i++)m_TextLineList[i].m_line.SetColor(color); }
	vector<sTextLine> m_TextLineList; //һ�����9��
	//                  
	BOOL SetFaceText(CWFile* pChannel, string name, string tex, HFONT hFont, int LineWidth);
	BOOL SetFaceText(string tex, HFONT hFont, int LineWidth);
	sFaceText()
	{
		m_TextLineList.resize(10);
		m_Channel.m_bStatic = TRUE; 
		for (int i = 0; i < 10; i++)
		{
			m_TextLineList[i].m_line.SetColor(RGB(255,255,255));
		}
	}
	int m_ShowStyle=0;  //0��ʾ����ʾƵ�������� 1��ʾ����ʾ����  2��ʾȫ����ʾ  
	void SetTagColor(sColorTag& starttag, sColorTag& endtag);
};
class cJinJieNum
{
public:
	int m_PositionID = 0;
	int m_JinJieNum = 0;
	BOOL GetBJinJie()
	{
		switch (m_PositionID)
		{
		case 0:
		case 1:
			return 0;
		default:
			return 1;
		}
	}
	void operator=(cJinJieNum& data)
	{
		m_PositionID = data.m_PositionID;
		m_JinJieNum = data.m_JinJieNum;
	}
	void Save(ofstream& File)
	{
		File << " ";
		File << m_PositionID << " ";
		File << m_JinJieNum << " ";
	}
	void Load(ifstream& File)
	{
		File >> m_PositionID;
		File >> m_JinJieNum;
	}
	void Reset()
	{
		m_PositionID=0;
		m_JinJieNum=0;
	}
};
struct sChangeColorData
{
	void Reset()
	{
		partnum = 0;
		for (int i = 0; i < 4; i++)
		{
			m_colorchange[i] = 0;
		}
	}
	enum eRanseType
	{
		ePc,
		ePet,
		eZuoQi,
		eZhuangShi
	};
	//Ⱦɫ
	int partnum = 0;
	int GetColorChange(int id){ return m_colorchange[id]; }
	BOOL SetColorChange(int id, int num);
	BOOL AddColorChange(int id);
	sChangeColorMatrix m_colormatrix[4];
	sChangeColorData()
	{
		for (int i = 0; i < 4; i++)
		{
			m_colorchange[i] = 0;
		}
	}
	void operator=(sChangeColorData& data);
	
	void Set(int d0,int d1,int d2,int d3);
	void Save(ofstream& File);
	void Read(ifstream& File, int modeid, eRanseType k, int partid);
private:
	int  m_colorchange[4];
};
struct sChangeColorData2
{
	sChangeColorData m_ranse[4];
	void Read(int modeid, ifstream& File, int posid=0);
	void Save(ofstream& File);
	void operator=(sChangeColorData2& data)
	{
		for (int i = 0; i < 4; i++)
		{
			m_ranse[i] = data.m_ranse[i];
		}
	}
	void Reset()
	{
		for (int i = 0; i < 4; i++)
			m_ranse[i].partnum = 0;
	}
};
struct sNpc
{
	cJinJieNum m_JinJieID;
	int weaponid = -1;
	int weapontype = -1;
	sChangeColorData2 m_Ranse;
	int m_modeid = -1;
	string m_name;
	string m_mapname="";//����λ��
	int m_x;
	int m_y;
	int  m_direction;
	void Reset();
	void SetRanse(int id0, int id1 = 0, int id2 = 0, int id3 = 0, int part = 0);
	void AutoDirection4(){ m_direction = Direction8To4(rand() % 10); }
	void operator =(sNpc& data)
	{
		m_x = data.m_x;
		m_y = data.m_y;
		m_name = data.m_name;
		m_mapname = data.m_mapname;
		m_modeid = data.m_modeid;
		m_direction = data.m_direction;

		weapontype = data.weapontype;
		weaponid = data.weaponid;
		m_JinJieID=  data.m_JinJieID;
		
	}
	void Set(int x1, int y1){ m_x = x1; m_y = y1; }
	void Set(string name, string mapname, int modeid, int x, int y, int direction = -1, int jinjie = 0);
	
	void Save(ofstream& File);
	void Load(ifstream& File);
};
struct sZiZhi
{
	//����
	int m_ATKZZ = 0;
	int m_DEFZZ = 0;
	int m_MAGZZ = 0;
	int m_HPZZ = 0;
	int m_SPDZZ = 0;
	int m_AVOIDZZ = 0;
	int m_Growing = 0;
	void Save(ofstream& File);
	void Load(ifstream& File);
	void Set(int atk, int def, int mag, int hp, int spd, int avoid, int grow)
	{
		m_ATKZZ = atk;
		m_DEFZZ = def;
		m_HPZZ = hp;
		m_MAGZZ = mag;
		m_AVOIDZZ = avoid;
		m_SPDZZ = spd;
		m_Growing = grow;
	}
	//�ɳ�
	void Set(sZiZhi& zishi, int strong, int limit)
	{
		int ZiZhiOffset = 100 + (limit*strong) / 100;

		m_ATKZZ = zishi.m_ATKZZ + rand() % (ZiZhiOffset);
		m_DEFZZ = zishi.m_DEFZZ + rand() % (ZiZhiOffset);
		m_MAGZZ = zishi.m_MAGZZ + rand() % (2 * (ZiZhiOffset));
		m_HPZZ = zishi.m_HPZZ + rand() % (4 * (ZiZhiOffset));
		m_SPDZZ = zishi.m_SPDZZ + rand() % (ZiZhiOffset);
		m_AVOIDZZ = zishi.m_AVOIDZZ + rand() % (ZiZhiOffset);
		m_Growing = zishi.m_Growing + rand() % ((ZiZhiOffset) >> 4);
	}
	void operator=(sZiZhi& zizhi)
	{
		//����
		m_ATKZZ = zizhi.m_ATKZZ;
		m_DEFZZ = zizhi.m_DEFZZ;
		m_MAGZZ = zizhi.m_MAGZZ;
		m_HPZZ = zizhi.m_HPZZ;
		m_SPDZZ = zizhi.m_SPDZZ;
		m_AVOIDZZ = zizhi.m_AVOIDZZ;
		m_Growing = zizhi.m_Growing;
	}
};

struct sMapObjTrigger;
struct sShangPinPet;
class cPetData;
class cTaskFather
{
public:
	cPetData* AddPetEnemyF(int hp, ePetID id, int lv=-1, string name="", BOOL bOur=0, int pos=-1);
	cPetData* AddPCPetEnemyF(int hp, ePetID id, int lv=-1, eMengPai mp = eMP_���, string name = "", BOOL bOur = 0, int pos = -1);
	cPetData* AddPCEnemyF(int hp, int id, sZiZhi* zz, int lv=-1, eMengPai mp = eMP_���, string name = "", int weapontype = -1, int weaponid = -1, BOOL bOur = 0, int pos = -1);

	void AutoGetPet(string name, int id,int id1=-1,int id2=-1,int id3=-1);
	void SetMusic(string name = "");
	BOOL CheckHeroMoney(int money,BOOL bTag=TRUE);
	void DialogTalkObj(string text, cObj* p);
	void DialogTalkNObj(string text);
	void NowNpcTalk(string text);
	void UnInviteAll();//��ɢ����
	void UpdateJob(string context, string title, int time, int ret);
	void UpdateJob(string context, string title);
	void SetNpc(sMapObjTrigger& data, int ret, string name, string map, int modeid, int x, int y, int direction = -1);
	void AddTriggerFightTurn(int ret);
	
	void RemoveTriggerObj();
	void RemoveTriggerGive();
	void RemoveTriggerObjMap(int ret=-1);
	void RemoveTriggerObjMap(int x,int y);
	void RemoveTriggerObjMap2(cObj* pObj);
	void RemoveTriggerSuccess();
	void RemoveTriggerFalse();
	void RemoveTriggerEscape();
	void AddChannel(string text,string name="",int channel=3);
	void AddTriggerTalkList(string path, int id, int ret);
	void AddTriggerGive(string name, int type, int id, int ret,BOOL bHuChi=FALSE);
	void AddTriggerFightStart(int ret);
	void AddTriggerFightSuccess(int ret);
	void AddTriggerFightFalse(int ret);
	void AddTriggerFightEscape(int ret);
	void AddTriggerLv(int ret,int lv);
	virtual void AddTriggerDialog(string path, int id, int ret);
	void AddTriggerObjMap(string mapname, int ret, BOOL bHuChi = TRUE);
	void AddTriggerObjMap(sNpc* npc, int ret,BOOL bHuChi=TRUE);
	void AddTriggerObjMap(sMapObjTrigger* data);
	void AddTriggerObj(sNpc* npc, int ret);
	void AddTriggerObj(string name,string map, int ret,BOOL bHuChi=TRUE);
	void AddNpcOne(sMapObjTrigger* data, BOOL needfind=FALSE);
	virtual void AddTag(string text, int time = 500);
	BOOL CheckNpc(string mapname, string npcname);//�������Ƿ����ĳNPC��
	int m_HuanShu = 0;
	BOOL m_bInit = FALSE;
	virtual void Init()=0;
	void CheckInit(){if (!m_bInit)Init(); m_bInit = TRUE;}
	virtual BOOL Process(int stage) = 0;
	
	void SetJiangLi(int time, BOOL needtag = FALSE, int money = 100, int chubei = 0, int exp = 100, int expbb = 0);
	void SetJiangLi2(int exp,int money=0,int chubei=0,int petexp=0,BOOL needtag=TRUE);
	void SetJiangLiItem(int id, int type, int num = 1);
	void FollowLv(int id, int lv);
	void GetPet(string name, sShangPinPet* pet,int stronglv);
	void GetPet2(sShangPinPet* pet, int stronglv);
	int GetType(){ return m_type; }
protected:
	int m_type;
private:

};

struct sTaskFather
{
	cTaskFather* m_pTask=0;
	int m_Stage=-1;
};

struct sObjFather;
class cPetData;
class cPCData;
struct sItem2;
class cObj
{
public:
	void LoadCharacter(string name, string path);
	void FollowLv(int lv=-1);
	sObjFather* GetObjFather();
	BOOL GetSex();
	void Talk(string context, cTaskFather* pTask, int stage,
		string select1 = "", string select2 = "", string select3 = "", string select4 = "");
	void Answer(cTaskFather* pTask, int stage);
	void Talk(string context,BOOL needret=FALSE);
	void ResetIndex();
	void SetWeapon(int type, int id);
	void AutoPCData(
		string name,
		int modeid,
		sAddPoint* addpoint,
		sZiZhi* zizhi,
		eMengPai mengpaid = eMP_��,
		sItem2* pWeapon = 0,
		int petid = -1,
		int stonglv=-1
		);
	void AutoPCPetData(
		string name,
		int  petid,
		sAddPoint* addpoint,
		eMengPai mengpaid = eMP_��,
		int stonglv = -1
		);
	void AutoPetData(
		string name, int modeid, int strongid
		, sAddPoint* addpoint,BOOL Needskill=TRUE
		);

	sTaskFather m_TaskTrigger;
	int GetSkillLv(int skillid);
	void MoveMagicSuffer();
	void ClearState();
	BOOL ReMoveState(eBuffType Type);
	BOOL ReMoveStateID(int id);
	BOOL AddState(sBuff buf);
	BOOL CheckHaveState(int skillid);
	BOOL SetMagiSufferID(DWORD MagicID, int MagicPos, CWFile* pFile, BOOL up);
	void MoveMagicSufferID();
	//����ʱ�ķ���
	cWfileP m_MagicSuffer; //�����ʱ����ʾ�ķ���
	BOOL    m_bMagicSufferUp; //����Ч����λ��,1��ʾ��,0��ʾ��
	int  m_MagicSufferPos;
	//�������ڼ�¼��ɫ��״̬,�����ж�,���ߵȵ�
	list<sBuff> m_StateList;

	CWFile m_Wav;
	CWFile m_WavSuffer;
	CWFile m_WavAtk;
	CWFile m_WavAtk2;
	CWFile m_WavMagic;
	CWFile m_WavDef;
	//
	CWFile m_MagicWav;
	cWfileP m_HpLinep;
	cWfileP m_HpEmptyLinep;
	vector<cWfileP> m_FaceTextBackp; //�Ի�����
	void UpdateHpLine();
	void  MoveDialog();
	void  MoveName();
	void  MoveHpLine();
	void Move2();
	BOOL m_bNeedShowName = TRUE; //�Ƿ���ʾ����

	void GetDateP(cPetData* &p);//�õ�ָ���ٻ��޲������ݵ�ָ��
	inline int GetTopY();//�õ���ɫ������λ��
	int GetTopY2();
	BOOL m_bCheckWall;//�Ƿ�Ҫ��ײ���
	
	int m_LineNum;
	sFaceText m_ContextList;
	int m_ShowTime;
	BOOL ShowFaceText(cCanvas* pCanvas);
	BOOL m_bGhost;
	cWfileP m_Ghost[4][2];
	BOOL SetGhost(BOOL b);//��Ӱ
	//
	BOOL SetTwinkle(BOOL b);//��ҫ
		
	BOOL Shake();//�ö������Ҷ���  ս��
	sFightOffset* GetFightOffset();
	//int* GetProperty(PropertyType Type);
	INT64* GetExp();
	INT64* GetExpMax();
	string GetAutoName();
	int   GetModeID();
	BOOL SetMagicEffect(CWFile* MagicEffect,int Pos=0,int Speed=4);
	cWfileP m_MagicEffect;  //�Լ����ϵķ���Ч��  ��������
	BOOL m_AutoRun;
	BOOL isPointOn(int x,int y);  //����1���Ƿ���ͼƬ��
	int  m_ScriptSelect;  //�ɶԻ��򷵻� 
	string m_ScriptPath;
	BOOL m_bScriptLoad;
	BOOL m_bHaveScript;
	int m_ScriptOneType;
	int m_ScriptOneId;
	void InitScript(string path)
	{
		m_ScriptPath = path;
		m_bHaveScript = TRUE;
		m_bScriptLoad = FALSE;
	}
	cMyScript  m_Script;
	BOOL Move();
	int  m_NowPos;  //��ǰ��������	
	int m_PrePos;
	void SetStatic(BOOL isStatic)
	{
		for (int i = 0; i < 6;i++)
			m_data[i].m_isStatic = isStatic;
	}
	void SetSpeed(int k);
	void SufferBack(float speed,int sufferDirection);
	void Reset();
    BOOL Set(int POS_X,BOOL bCircle=TRUE);
	void SetSound(int POS_X,BOOL bPlay=TRUE);
	void PlayWav(int POS_X);
	void SetMagicSound(int SkillID,int position=0);
	void GetIndex(int POS_X, vector<DWORD>& out);
	DWORD GetIndex(int POS_X);
	void  Free();
	void  FreeIndex();
	void Stand();
	void SetTarget(float x, float y){ m_xTarget = x; m_yTarget = y; }
	void SetTarget(int x, int y){ m_xTarget = (float)x; m_yTarget = (float)y; }
	virtual BOOL Show(cCanvas* pCanvas);
	virtual BOOL ShowOnScreen(cCanvas* pCanvas);
	void AutoFightDirection(int id);
	int m_time;  //-2��ʾ��ֹ, -1��ʾֱ�� ,����-1��ʾѰ·
	
	vector<Coordinate> m_Path;
	float m_xTarget;
	float m_yTarget;
	float   m_oldx;
	float   m_oldy;
	cText m_TrueName;
	void SetTrueName(string Name){ m_TrueName.SetString( (char*)Name.c_str()); };

	
	void Init();
	void SetShine(BOOL needshine);//������ʾ
	
	cObj();
	~cObj();

	int m_speed;    //��ɫ���ƶ��ٶ�
	void SetFrame(int fram);

	BOOL m_NowShowData=0;
	CWFile* m_pShowData[4];
	CWFile m_data[8];
	void SetDataType(int Type);
	int  GetType(){ return m_IndexType; }
	void*  m_pIndex;
	BOOL CanEquip(int Equiptype, int ID, int Pos);
	BOOL SetShadow(int iShadow);
	

	BOOL AddZZ(int num, PropertyType typpe);
	int GetX(){ return (int)m_xPos; };
	int GetY(){ return (int)m_yPos; };
	void SetPos(int x, int y){ SetPos((float)x, (float)y); }
	void SetPos(float x, float y);
	void SetX(int x);
	void SetY(int y);
	void SetX(float x){ SetX((int)x); m_xPos = x; }
	void SetY(float y){ SetY((int)y); m_yPos = y; }
	void SetXY(int x, int y);
	void SetXY(float x, float y){ SetXY((int)x, (int)y); m_yPos = y; m_xPos = x; };
	void SetDirection(int direction);
	int  GetDirection(){ return m_direction; }
	void SetShake(BOOL shake);
	void MoveShadow();
	cWfileP* GetShadow(){ return &m_Shadow; }
	float m_xPos;
	float m_yPos;
	int    m_IndexType = 0;
	int m_IDInFightlist=-1;//����ID,�����sFightList���Ӷ���,���������������
	void SetShow(BOOL Needshow);
	BOOL GetShow();
	DWORD GetSound(int POS);
private:
	BOOL m_NeedShow2 = TRUE;
	cWfileP m_Shadow;
	int m_direction = -1;
	
	int    m_bShaketime; //��Shake���õ�
	BOOL m_bShake; //�Ƿ񶶶�
	
};
enum eXiuLian
{
	eXLATK,
	eXLDEF,
	eXLMAG,
	eXLDMAG,
};
struct sBBXiuLian
{
	void Reset()
	{
		m_Atk = 0;
		m_Def = 0;
		m_Mag = 0;
		m_DMag = 0;
		m_AtkExp = 0;
		m_DefExp = 0;
		m_MagExp = 0;
		m_DMagExp = 0;
	}
	int& GetAtk(){ return m_Atk; }
	int& GetDef(){ return m_Def; }
	int& GetMag(){ return m_Mag; }
	int& GetDMag(){ return m_DMag; }
	int& GetAtkExp(){ return m_AtkExp; }
	int& GetDefExp(){ return m_DefExp; }
	int& GetMagExp(){ return m_MagExp; }
	int& GetDMagExp(){ return m_DMagExp; }
	void AddXiuLianLv(int num, eXiuLian type);
	int AddExp(int lv, int num, eXiuLian type);//0 ����û������ 1 ���������� 2 �ȼ�������
	void Set(int atk, int def, int mag, int dmag)
	{
		m_Atk = atk;
		m_Def = def;
		m_Mag = mag;
		m_DMag = dmag;
	}
	void Save(ofstream& File);
	void Load(ifstream& File);
	void operator=(sBBXiuLian& data)
	{
		m_Atk = data.m_Atk;
		m_Def = data.m_Def;
		m_Mag = data.m_Mag;
		m_DMag = data.m_DMag;
		m_AtkExp = data.m_AtkExp;
		m_DefExp = data.m_DefExp;
		m_MagExp = data.m_MagExp;
		m_DMagExp = data.m_DMagExp;
	}
private:
	int  m_Atk;
	int  m_Def;
	int  m_Mag;
	int  m_DMag;
	//��������
	int  m_AtkExp;
	int  m_DefExp;
	int  m_MagExp;
	int  m_DMagExp;
};


struct spInt
{
	int d1 = 0;
	int d2 = 0;
	void Save(ofstream& File)
	{
		File << d1 << " ";
		File << d2 << " ";
	}
	void Load(ifstream& File)
	{
		File >> d1;
		File >> d2;
	}
	void Reset()
	{
		d1 = 0;
		d2 = 0;
	}
};
struct sInt
{
	void Save(ofstream& File);
	void Load(ifstream& File);
	void Reset()
	{
		m_Data = 0;
		m_DataAdd = 0;
		m_DataAddPercent = 0;
		m_DataOriginal = 0;
	}
	void InitAll(int DataOriginal, int Percent, int Add)
	{
		m_DataOriginal = DataOriginal;
		m_DataAdd = Add;
		m_DataAddPercent = Percent;
		Update();
	}
	void SetDataAdd(int num)
	{
		m_DataAdd = num;
		Update();
	};
	void AddDataAdd(int num)
	{
		if (!num)return;
		m_DataAdd += num;
		Update();
	}
	void SetDataOriginal(int num)
	{
		m_DataOriginal = num;
		Update();
	};
	void AddDataOriginal(int num)
	{
		m_DataOriginal += num;
		Update();
	}
	void SetDataAddPercent(int num)
	{
		if (!num)return;
		m_DataAddPercent = num;
		Update();
	};
	void AddDataAddPercent(int num)
	{
		if (!num)return;
		m_DataAddPercent += num;
		Update();
	}
	int* GetDatap(){ return &m_Data; }
	int* GetPercentp(){ return &m_DataAddPercent; }
	int* GetOriginalp(){ return &m_DataOriginal; }
	int* GetAddp(){ return &m_DataAdd; }


	const int GetData()const{ return m_Data; }
	const int GetPercent()const{ return m_DataAddPercent; }
	const int GetOriginal()const{ return m_DataOriginal; }
	const int GetAdd()const{ return m_DataAdd; }
	void SetData(int k){  m_Data=k; }
	void SetPercent(int k){  m_DataAddPercent=k; }
	void SetOriginal(int k){  m_DataOriginal=k; }
	void SetAdd(int k){  m_DataAdd=k; }
	void operator=(sInt& k)
	{
		m_Data = k.GetData();
		m_DataAdd = k.GetAdd();
		m_DataAddPercent = k.GetPercent();
	}
private:
	void Update(){ m_Data = (m_DataOriginal*(100 + m_DataAddPercent)) / 100 + m_DataAdd; }
	int m_Data=0;
	int m_DataAddPercent=0;
	int m_DataOriginal=0;
	int m_DataAdd=0;
};
struct sPetSkill;
class cPet;
struct sFightData  //��ʾ��ɫս��ʱ������ (ս��ʱ���Ի���õ���BUFF���ı�.���ǵ�Ȼ����ֱ���ý�ɫ��������������)
{
	sInt  m_oldATK;   //�˺�
	sInt  m_oldHIT;   //����
	sInt  m_oldDEF;   //����
	sInt  m_oldWakan; //����
	sInt  m_oldSPD;   //�ٶ�
	sInt  m_oldEvade; //����
	spInt  m_oldGoldLive;//����
	//һ����ɫ��״̬�б�
	int SufferNum;	//��¼���غϱ���Ĵ���
	int AttackNum;	//��¼���غϵĹ�����
	int cannotAttack; //�Ƿ����ʹ��������   
	int cannotMagic;  //�Ƿ����ʹ��ħ��
	int cannotSkill;  //�Ƿ����ʹ���ؼ�
	int cannotUseItem;//�Ƿ����ʹ�õ���
	int cannotAddHP;  //�Ƿ���Լ�Ѫ
	int cannotAll;  //�Ƿ���Զ�  ���߷���Ч���������,������Ҳ�����
	int chaos;		//�Ƿ����
	int cannotLive;  //�Ƿ���Ա�����
	BOOL m_bHit; //��¼�Ƿ�����  �����ֵ���ж��Ƿ���Ҫ����	
	BOOL m_bDefend;//�Ƿ����
	BOOL m_bCri;//�Ƿ񱻱�ɱ
	vector<int>  m_guardID;//��¼�Ƿ񱻳ɹ����� �� �������Ľ�ɫ��ID

	int m_MaxDef;
	int m_MaxMagicDef;
	//��¼ǿ��״̬  ��0λ+5%(����ħ��֮ӡ) ��1λ+10%(����Ұ��) ��2λ -5% ��3λ-10% (�������)  �����λ�������Ժ���չ
	int m_AtkIntensify;
	int m_DefIntensify;
	int m_SpdIntensify;
	int m_WallNum; //
	void Reset();
	void Clear()
	{
		m_guardID.clear();
		m_AtkIntensify=0;
		m_DefIntensify=0;
		m_SpdIntensify=0;
		m_WallNum=0; //
		Reset();
	}
	BOOL Init();
	int m_TurnHPAdd;
	int m_TurnHPHurtAdd;
	int m_TurnMPAdd;
};

struct sItem;
struct sItem2
{
	void Save(ofstream& File);
	void Load(ifstream& File);
	void Set(int type, int id,BOOL needclear=TRUE);
	int GetType();
	int GetID();
	string GetName();
	string GetText();

	int GetNum(){ return m_Num; }
	sItem* m_pItemData = 0;
	int m_Num=0;
	vector<int>  m_Property; //����  
	string m_strProperty="";
	void Reset()
	{
		m_Num = 0;
		m_Property.clear();
		m_pItemData = 0;
		m_strProperty="";
	}
	void Swap(sItem2& item2)
	{
		swap(m_Num, item2.m_Num);
		swap(m_pItemData, item2.m_pItemData);
		m_strProperty.swap(item2.m_strProperty);
		m_Property.swap(item2.m_Property);
	}
	void operator=(sItem2& item)
	{
		m_pItemData = item.m_pItemData;
		m_Num = item.m_Num;
		m_Property.clear();
		for (UINT i = 0; i < item.m_Property.size(); i++)
			m_Property.push_back(item.m_Property[i]);
		m_strProperty = item.m_strProperty;
	}
	BOOL operator==(sItem2& item)
	{
		if (item.GetType()==GetType())
		if (item.GetID() == GetID())
			return TRUE;
		return FALSE;
	}
};
struct sShangPinPet;
struct sControler
{
	void Set(int bfly=1, int bdel=1, int bBb=0, int bHavebb=0,int bCanGrab=0)
	{
		m_bBB = bBb;
		m_bDelete = bdel;
		m_HaveBB = bHavebb;
		m_bFly = bfly;
		m_CanGrab = bCanGrab;
	}
	void Save(ofstream& File);
	void Load(ifstream& File);
//	int GetOwner(){ return m_Hero; };

	int  GetFly(){return m_bFly; }
	int GetDel(){return m_bDelete; }
	int CheckBB(){ return m_bBB; }
	int CheckHaveBB(){ return m_HaveBB; }
	void SetHaveBB(int k){ m_HaveBB = k; }
	BOOL CheckGrab(){ return m_CanGrab; }
private:
	int m_CanGrab = 0; //�Ƿ����ץ һ�㶼�ǲ���ץ��
	int m_bFly = 1;//�Ƿ�ᱻ����
	int m_bDelete = 1; //ս�����Ƿ�ɾ��
	int m_bBB = 0;//�Ƿ���ĳ�˵��ٻ���;
	int m_HaveBB = 0;//�Ƿ���BB
};
enum ePetSkill
{
	ePS_����,
	ePS_�߼�����,
	ePS_����,
	ePS_�߼�����,
	ePS_��Ѫ,
	ePS_�߼���Ѫ,
	ePS_����,
	ePS_�߼�����,
	ePS_����,
	ePS_�߼�����,
	ePS_ҹս,
	ePS_�߼�ҹս,
	ePS_����,
	ePS_�߼�����,
	ePS_��֪,
	ePS_�߼���֪,
	ePS_����,
	ePS_�߼�����,
	ePS_ڤ˼,
	ePS_�߼�ڤ˼,
	ePS_�۸�,
	ePS_�߼��۸�,
	ePS_����,
	ePS_��ɱ,
	ePS_�߼���ɱ,
	ePS_����,
	ePS_�߼�����,
	ePS_��,
	ePS_�߼���,
	ePS_�м�,
	ePS_�߼��м�,
	ePS_����,
	ePS_�߼�����,
	ePS_����,
	ePS_�߼�����,
	ePS_ǿ��,
	ePS_�߼�ǿ��,
	ePS_����,
	ePS_�߼�����,
	ePS_͵Ϯ,
	ePS_�߼�͵Ϯ,
	ePS_��,
	ePS_�߼���,
	ePS_����,
	ePS_�߼�����,
	ePS_�����,
	ePS_�߼������,
	ePS_ħ֮��,
	ePS_�߼�ħ֮��,
	ePS_���Ӹ���,
	ePS_�߼����Ӹ���,
	ePS_������,
	ePS_�߼�������,
	ePS_������,
	ePS_�߼�������,
	ePS_��������,
	ePS_�߼���������,
	ePS_��������,
	ePS_�߼���������,
	ePS_��������,
	ePS_�߼���������,
	ePS_�ٶ�,
	ePS_�����,
	ePS_����ˮ,
	ePS_������,
	ePS_������,
	ePS_����������,
	ePS_�߼�������,
	ePS_����������,
	ePS_�߼�������,
	ePS_����������,
	ePS_�߼�������,
	ePS_ˮ��������,
	ePS_�߼�ˮ����,
	ePS_�ƻ���ľ,
	ePS_����,
	ePS_��Ѫ׷��,
	ePS_���۷���,
	ePS_�����ػ�,
	ePS_�����ֿ�,
	ePS_�߼������ֿ�,
	ePS_ֱ��,
	ePS_�绪��ʥ,
	ePS_�����ݾ�,
	ePS_�׻�,
	ePS_������,
	ePS_����,
	ePS_̩ɽѹ��,
	ePS_ˮ��,
	ePS_ˮ����ɽ,
	ePS_�һ�,
	ePS_�����һ�,
	ePS_����һ��,
	ePS_ҹ�����,
	ePS_�����ٻ�,
	ePS_������ɽ,
	ePS_��������,
	ePS_��������,
	ePS_�ƶ��б�,
	ePS_���ݻ���,
	ePS_���ŭ��,
	ePS_����ռ���,
	ePS_�����ƻ�����
};

class cCharacter;
class cPCData;
class cPetData   //һ��BBӦ���е�����
{
private:
	BOOL m_bSetupXiuLian = FALSE;
public:
	void ReadPet(string name, string path);
	void SetXLEffect(BOOL bSetup);
	void ResetAddPoint();
	void AutoGetSkill(int id);
	void AddSkill(ePetSkill id);
	//Ⱦɫ
	sChangeColorData2 m_RanSe;
	void SetJinJie(int num);
	cJinJieNum m_JinJieNum;//����
	void lvUpFuZhuSkill(int lv,int FuZhuSkillID);
	virtual void Followlv(int lv = -1);
	void FillHPMP();
	void EquipExtra(vector<int>& prop, int& startid,BOOL m_bEquip);
	void PetEquip(int Pos,int bequip=1);
	void UnPetEquip(int Pos);
	
	void Set(sShangPinPet* pet);
	BOOL CheckHavePetSkill(int SkillID);
	void AutoPetData(BOOL m_bNeedSkill=TRUE);
	BOOL LeveUp(int lv = 1, BOOL bCost = TRUE);
	void AddProPertyAtk(int num);
	void AddProPertyDef(int num);
	void AddProPertyMag(int num);
	void AddProPertyHp(int num);
	void AddProPertySpd(int num);
	void AddHpZZ(int num);
	void AddMagZZ(int num);
	void AddAtkZZ(int num);
	void AddAvoidZZ(int num);
	void AddSpdZZ(int num);
	void AddDefZZ(int num);
	void AddGrowing(int num);
	int CountATK();//�����˺�
	int CountDEF();//�����
	int CountHP();//����HP
	int CountWakan();//��������
	int CountMP(); //����MP
	int CountSpd(); //�����ٶ�
	int CountEvade(); //��������
	int CountHit();//����������
	void Reset();
//
	void AddPopularity(int num);
	int   m_Popularity = 0;  //����
	//
	sBBXiuLian m_XiuLian;
	
	sItem2 m_PetEquip[3];
	//�ӵ㷽ʽ
	sAddPoint m_AddPoint;
	//����
	sZiZhi m_ZiZhi;
	//BB����
	int  m_NumofBBSkill=0;
	vector<sSkill*> m_pPetSkill;
	//����
	INT64 EXP = 0;
	INT64 ExpMax = 0;
	

	//ս�����Ա�
	sFightData   m_FightProperty;
	string m_Name;
	string m_dataFileName;
	sControler  m_Controler;

	//��������
	int   m_FuZhuSkillLvList[c_NumFuZhu];
	//
	
	int  modeid = -1;
	int  m_AiLv = 0;//AI�ȼ�  ������ɫ��ս��ʱ����Ϊ 0-100
	int  m_StrongLv = 0; //ǿ��  ������ɫ������ǿ��
	int  m_LV = 0;
	int  m_HP = 0;
	int  m_HPHurtMax = 0;
	int  m_MP = 0;
	int  m_CP = 0;      //��ŭ
	int  m_CPMax = 150;
	int m_LiveHpLv = 0;//��������
	int m_LiveMpLv = 0;//ڤ˼
	int m_LiveCp = 0; //ÿ�غ�CP�ظ�

	int   m_Physical=0;    //����
	int   m_PhysicalMax=0; //��������
	int   m_Energy=0;      //����
	int   m_EnergyMax=0;  //��������
	//���Ե�����
	int  m_ProPertyAtk =0;
	int  m_ProPertyDef =0;
	int  m_ProPertyMag =0;
	int  m_ProPertyHP = 0;
	int  m_ProPertySpd = 0;
	
	int nightLv = 0;//�ȼ�ҹս����
	int mpsavepercent = 0; //�۸�	
	int restoreRate = 0; //״̬�ָ���
	int  abnormalResist = 0; //������
	int  magicWavePercent=0;  //��������

	int buffAdd = 0;//BUFFʱ��
	int m_MagicNumAdd = 0;//�������ö�������

	
	sInt  m_HPMax;
	sInt  m_MPMax;
	sInt  m_ATK;   //�˺�
	sInt  m_HIT;   //����
	sInt  m_DEF;   //����
	sInt  m_Wakan; //����
	sInt  m_SPD;   //�ٶ�
	sInt  m_Evade; //����


	spInt GoldLivePercent;//���
	spInt  m_ParryRate; //����  
	//��ɱ��
	spInt  m_CriRate; //����
	spInt  m_MagicCriRate; //����
	spInt  m_CureCriRate;//����

	spInt  m_CriAddPercent; //��ɱ�˺�����
	//����
	int comboNumDown = 1;//��������
	int comboNumUp = 2;//��������
	int ShiXueAdd = 0; //�����˺�����
	spInt  m_Combo;  //������
	spInt  m_MagicComboRate; //������

	spInt  m_Pierce;//���ӷ�����
	spInt  m_PierceMagic; //����������

	//������  �����
	spInt m_HitRate;  //����
	spInt  m_MagicHitRate;//����
	spInt  m_SealRate;//��ӡ
	spInt poisonRate; //��
	//�˺��ӳ�  �����ӳ�
	spInt  m_DamageAddPercentMagic2;//�̶�
	spInt  m_DamageAddPercentMagic;//����
	spInt  m_DamagePercent;//����
	spInt  m_HiddenWeaponDamagePercent; //����
	spInt  m_DamageAddPercentPoision; //��
	spInt  m_DamageAddPercentCure;//����  ������
	//�ٻ����˺��ӳ�
	spInt  m_PetDamagePercentMagic;//�����˺�
	spInt  m_PetDamagePercent;//�����˺�
	//�˺�����
	spInt  m_DamageAddMagic;//�˺��������
	spInt  m_DamageAddMagicLv;//�����˺��������lv
	spInt  m_DamageAdd;//�����˺��������
	spInt  m_DamageAddLv;  //�����˺����
	spInt  m_DamageAddCure;//����Ч������
	//����
	spInt m_CounterRate;
	//����
	spInt m_StabRate;
	spInt m_StabRateMagic;    //���������˺�
	spInt m_StabDamageRate;
	spInt m_StabDamageRateMagic; //����������
	//
	spInt m_SuckBloodRate;//��Ѫ��
	//
	
	//����������,��������������
	spInt m_AtkDownCP;//������CP
	spInt m_StartMagic;//��������
	spInt m_MagDownHPHurt;
	spInt m_AtkDownHPHurt;
	spInt m_AtkDownMP;
	spInt m_AtkDownAtk;
	spInt m_AtkDownHit;
	spInt m_AtkDownDef;
	spInt m_AtkDownWakan;
	spInt m_AtkDownSpd;
	spInt  m_Spurting;//����;
	//
	//������ºͽ�ħ����Ч��
	spInt m_JinJiaRate;
	spInt m_XiangMoRate;
	//���������˺�,0-100
	spInt  m_MagicFire; //��
	spInt  m_MagicSoil; //��
	spInt  m_MagicWater;  //ˮ
	spInt  m_MagicThunder;//��
	//�ع� 
	spInt BoySpecial ;  //��
	spInt GirlSpecial;   //Ů
	spInt GoldSpecial;     //�����ع�
	spInt MagSpecial ;    //ħ���ع�
	spInt ImmortalSpecial; //�����ع�
	spInt MonsterSpecial;//�����ع�
	spInt PeopleSpecial ; //��
	spInt ghostSpecial;  //���ع�
	spInt machineSpecial ;//��е�ع�

	cPetData();
	BOOL Save(ofstream& File, BOOL bPet = TRUE);
	BOOL Load(ifstream& File,BOOL bPet=TRUE);
	int m_IDinFightList=-1;//��fightlist�б��е�λ��
	int m_race = -1;//����
};

class cMengPai;
struct sSkill;

struct sFightSkillSelect
{
	int Lv=0;
	int SkillID=-1;
	int TargetID=-1;
};
struct sZuoQiZhuangShi
{
	DWORD m_ColorID;
	DWORD  NodeStand;
	DWORD  NodeWalk;
	DWORD  Stand[15];
	DWORD  Walk[15];
};
struct sZuoQi
{
	DWORD m_ColorID;
	int ID;
	string Name;
	DWORD  nodeStand;
	DWORD  nodeWalk;
	DWORD  CharStand[15];
	DWORD  CharWalk[15];
};
struct sZuoQi2
{
	int m_ownerID;
	sZuoQi* m_pZuoQi = 0;
	sItem2  m_ZhuangShi;
	DWORD GetID(int Pos){ vector<DWORD> out; GetID(Pos, out); return out[0];}
	void GetID(int Pos, vector<DWORD>& out, BOOL haveChar = TRUE);
};
enum eCharacterEquip
{
	eChaEquipWuqi,
	eChaEquipTouKui,
	eChaEquipYaoDai,
	eChaEquipXieZi,
	eChaEquipXiangLian,
	eChaEquipYiFu
};

struct sWeaponData2;
struct sCharacterData3;
class cPCData :public cPetData  //һ������Ӧ���е�����
{
public:
	void Read(string name,string path);
	BOOL AddMoney(int xianjin, int chubei = 0, int cunyin = 0,BOOL cost=TRUE);//�����ֽ�,����,���� ���С��0,�Ƿ�۳�
	BOOL AddExp(INT64 exp, INT64 expbb = 0, BOOL bCost=TRUE);
	void Followlv(int lv);
	int FindItem(int type, int id);
	void Save(ofstream& File);
	void Load(ifstream& File);
	//���˵ȼ�
	int m_traderLV=0;
	INT64 m_traderExp=0;
	INT64 m_traderExpMax=0;
	BOOL AddTraderExp(int num);
	//
	int m_Accelarate[8];//��ݼ�
	BOOL CostBangGong(int num);
	sWeaponData2*    m_pWeaponData2 = 0;
	sCharacterData3* m_pCharacterData2 = 0;
	sItem2* GetItem(int ID);
	void Reset();
	void AutoPCData(
		string name,
		int modeid,
		sAddPoint* addpoint=0,
		sZiZhi* zizhi=0,
		eMengPai mengpaid = eMP_��,
		int  petid = -1
		);
	//����
	BOOL m_bUseCard=FALSE;
	void SetCard(int id);
	int&  GetCard();
	int m_CardPetID = -1;
	int    m_bDuJie=0;//�Ƿ�ɽ�
	int		m_face = 0;//��
public:
	//sChangeColorData2 m_CardRanSe;//��һ�����ʱ�ɱ����ٻ���
	cPet*  m_pCardPetData=0;
	//����
	BOOL m_bOnZuoQi=FALSE; //�Ƿ���������
	int zuoqiid=-1;  //����ID
	sChangeColorData m_ZuoQiRanSe;//����Ⱦɫ
	sChangeColorData m_ZhuangShiRanSe;//װ��Ⱦɫ
	sZuoQi2 m_ZuoQi;

	void operator=(cPCData& target);
	cPCData();
	//���е��ٻ�����
	int  m_NumofBB=0;
	vector<cPet*> m_pPetDataList;
	int m_FightPetID=-1;
	BOOL DeletePet(int ID); //����ɫ��ȥһ��BB
	BOOL GetPet(cPet* pPet);//����ɫ����һ��BB
	void GetShangPinPet(int ShangPinID);
	void GetShangPinPet2(sShangPinPet* ShangPin);
	vector<sSkill*> m_SkillEnablelist;
	int m_MengPaiPoint;
	int  m_SpecialSkillList[6];  //װ���ؼ�
	//����
	int  m_MengPaiID=-1;
	int  m_MengPaiSkillLV[7];
	
	//����
	int  m_BangPaiID=-1;
	int   m_BangPaiPoint=0;
	//�ٻ�������
	sBBXiuLian m_BBXiuLian;

	int   m_Money[3];       //Ǯ  ���� ������  
	
	sItem2 m_Item[60];  // ������� 


	~cPCData();
	BOOL Equip(sItem2& item,int m_bEquip=TRUE);
	//void EquipSpecial(vector<int>& prop,int& proppos, int Pos, int m_bEquip = 1);
	BOOL UnEquip(int id){ if (id >= 10000)id -= 10000; return Equip(m_Equip[id], -1); }
	BOOL CanEquip(sItem2& item);
	BOOL CanEquip(int type);

	sItem2 m_Equip[6];//   ����װ��
};

class cNpc
{
public:
	sNpcData* m_pNpcData;
	void GetID(int POS_X,vector<DWORD>& out);
private:

};

struct sAttackPoint
{
	string name;
	int xOffset;
	int yOffset;
	int point;
};

struct sFightOffset
{
	int  m_xOff;
	int  m_yOff;
	int  m_Point[2];
	int  m_Point2[2];
	int  m_MagicPoint;
	void ReadChar(ifstream& File)
	{
		File >> m_xOff;
		m_yOff = m_xOff >> 1;
		File >> m_Point[0];
		File >> m_Point[1];
		//��ɫֻ��һ�ֹ���
		m_Point2[0] = m_Point[0];
		m_Point2[1] = m_Point[1];
		File>>m_MagicPoint;
	}
	void ReadPet(ifstream& File)
	{
		File >> m_xOff;
		m_yOff = m_xOff >> 1;
		File >> m_Point[0];
		File >> m_Point[1];
		File>> m_Point2[0];
		File>>m_Point2[1];
		File >> m_MagicPoint;
	}
	void operator=(sFightOffset& target)
	{
		m_xOff = target.m_xOff;
		m_yOff = target.m_yOff;
		m_Point[0] = target.m_Point[0];
		m_Point[1] = target.m_Point[1];
		m_Point2[0] = target.m_Point2[0];
		m_Point2[1] = target.m_Point2[1];
		m_MagicPoint = target.m_MagicPoint;
	}
};
struct sObjFather
{
	int m_ID;
	string m_Name;
	BOOL m_bSex;
	int m_Autorace;
};
struct sPosSound
{
	//����
	DWORD	m_soundattack;
	DWORD	m_soundattack2;
	DWORD	m_soundmajic;
	DWORD	m_sounddefend;
	DWORD	m_soundsuffer;
	DWORD   m_sounddead;
	DWORD GetSoundID(int Pos);
};
struct sCharacterSound :public sPosSound
{
	void Read(ifstream& File)
	{
		File >> m_sounddead;
		File >> m_soundsuffer;
		File >> m_soundmajic;
		File >> m_soundattack;
		m_soundattack2 = m_soundattack;
		
		File >> m_sounddefend;
		
		
	}
};
struct sPetDataPart
{
public:
	DWORD m_ColorID;
	DWORD m_stand;
	DWORD m_walk;
	DWORD m_fstand;
	DWORD m_run;
	DWORD m_attack;
	DWORD m_attack2;
	DWORD m_majic;
	DWORD m_defend;
	DWORD m_suffer;
	DWORD m_dead;
	DWORD m_return;
	virtual DWORD GetID(int posid);
	virtual void Read(ifstream& File)
	{
		File >> m_ColorID;
		File >> m_attack;
		File >> m_attack2;
		File >> m_dead;
		File >> m_defend;
		File >> m_stand;
		File >> m_fstand;
		File >> m_majic;
		File >> m_return;
		File >> m_run;
		File >> m_suffer;
		File >> m_walk;
	}
};

struct sCharacterData3 :public sPetDataPart
{
	
	sCharacterSound m_Sound;
	sFightOffset m_Offset;
	void Read(ifstream& File)
	{		
		File >> m_attack;
		m_attack2 = m_attack;
		File >> m_dead;
		File >> m_defend;
		File >> m_fstand;
		File >> m_majic;
		File >> m_return;
		File >> m_run;
		File >> m_stand;
		File >> m_suffer;
		File >> m_walk;
		m_Offset.ReadChar(File);
		string dustbin;
		getline(File, dustbin);
	}
};
struct sCharacterHead2
{
	DWORD	m_smallhead;
	DWORD	m_bighead;
};
struct sCharacterHead
{
	sCharacterHead2 m_Head[4];
	DWORD	m_dialog;
	DWORD   m_FriendHead;
	void Read(ifstream& File)
	{
		File >> m_dialog;
		File >> m_FriendHead;
		for (int i = 0; i < 4; i++)
		{
			File >> m_Head[i].m_smallhead; //���Ͻǵ�ͷ��
			File >> m_Head[i].m_bighead; //�������е�ͷ��
		}
	}
};



//һ�ֽ�ɫ
struct sCharacterData:public sObjFather
{
	DWORD m_ColorID;
	vector<sCharacterData3> m_Pos; //���� ���� ��ƫ��
	sCharacterHead m_Head[2]; //ͷ��
	//����
	DWORD	m_posgreet;
	DWORD	m_posbow;
	DWORD	m_posdance;
	DWORD	m_poscry;
	DWORD   m_posangry;
	DWORD   m_possit;
	void Read(ifstream& File)
	{
		File >> m_ID;
		int num;
		File >> num;
		m_Pos.resize(num);
		File >> m_ColorID;
		File >> m_Name;
		File >> m_Autorace;
		File >> m_bSex;
		File >> m_posbow;
		File >> m_poscry;
		File >> m_posdance;
		File >> m_posgreet;
		File >> m_possit;
		File >> m_posangry;
		for (int i = 0; i < num; i++)
		{
			m_Pos[i].Read(File);
		}
		m_Head[0].Read(File);
		m_Head[1].Read(File);
	}
	void ReadSound(int i, ifstream& File)
	{
		m_Pos[i].m_Sound.Read(File);
	}
	BOOL CanEquip(int EquipType);
	
	
};

class cPCData;
class CMainState;



#define TYPE_EQUIP 1
#define TYPE_ITEM  2
struct sZuoQi;
class cSkillManager;

class cCharacter
{
public:
	void UseCard(int id);
	int GetRace();
	void Free();
	void Talk(string str);
	void AutoPCData(
		string name,
		int modeid,
		sAddPoint* addpoint=0,
		sZiZhi* zizhi=0,
		eMengPai mengpaid=eMP_��,
		int petid = -1
		);


	int GetSkillLv(int id, cSkillManager* p);
	//int m_Modeid;
	BOOL AddMoney(int xianjin, int chubei=0,int cunyin=0);//�����ֽ�,����,����
	
	//��������
	cPCData m_PCData;	
	
	DWORD GetID(int Pos)
	{ 
		vector<DWORD> out; 
		GetID(Pos, out); 	
		if (out.size())
			return out[0];
		return 0;
	}
	void GetID(int Pos, vector<DWORD>& out);
	void GetRanse(int Pos,  sChangeColorData* (&pChangeColor)[4]);
	sFightOffset* GetFightOffset();
	string GetName(){ return m_PCData.m_Name; }//�õ���ɫ����
	BOOL CanEquip(int EquipType,int EquipLv,int Pos=-1);
	void SetData(sCharacterData* data);
	sCharacterData* GetData(){ return m_pCharacterData; }
private:
	//ͼ������
	sCharacterData* m_pCharacterData;
};


struct sJiaJu
{
	sItem2 m_JiaJu;
	int m_xPos;
	int m_yPos;
	int m_Direction=0;
	void Save(ofstream& File)
	{
		File << " ";
		File << m_xPos<<" ";
		File << m_yPos << " ";
		File << m_Direction << " ";
		m_JiaJu.Save(File);
	}
	void Load(ifstream& File)
	{
		File >> m_xPos ;
		File >> m_yPos ;
		File >> m_Direction;
		m_JiaJu.Load(File);
	}
};

class cFangWu
{
public:
	void GetBB();
	void RemoveBB(cObj* yahuan);
	void RemovePreJiaJu();
	cFangWu();
	void Init();
	int GetHuanJin();
	void GoIn();
	void CheckHuanJin();
	BOOL AddJiaJu(sJiaJu& jiaju);
	string GetMapName();
	void Move(int x, int y, cObj* p);

	int GetGuiMo(){ return m_GuiMo; }
	void SetGuiMo(int num);
	void Save(ofstream& File);
	void Load(ifstream& File);

	vector<sNpc> m_List; //��լ����  Ӱ�컷���� =5�㻷����
	
	sNpc m_GuanJia;//�ܼ� 
	sItem2 m_DiBan;//�ذ�
	vector<sJiaJu> m_JiaJu;//�Ҿ�
	vector<int>  m_YaHuangIDList;
private:
	int m_PreHuanJin=0;
	BOOL m_bInit=FALSE;
	int m_GuiMo=-1;//��ģ ��ģԽ��,�ܰڷŵļҾ�Խ��
	int m_HuanJinDu = 0;//������  
};
