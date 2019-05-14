#pragma once
#include "global.h"
class cCanvas;

class cAccelator:public cInterfaceFather
{
public:
	BOOL CheckRB();
	void Init();
	BOOL ProcessAccelator();
	virtual BOOL ProcessInput();
	virtual BOOL Show(cCanvas* pCanvas);
	virtual BOOL CheckL();
	void OnOff(BOOL NeedShow);
private:
	BOOL UseSkill(int id);
	int m_AccelatorSelect=-1;
	cDescribe m_Describe;
	int m_Mode=0;
	BOOL m_bInit=FALSE;
	CWFile m_Accelatorback; //快捷键的背景框
	cWfileP m_Accelatorbackp[8];
	CWFile  m_AccelatorSkill[8];
	cText   m_tAccelatorSkill[8];
	int		m_AccelatorSkillid[8];

	RECT m_redrawrect;
	int m_NumofSkill=0;
	cWfileP m_MagicBack;
	CWFile m_SkillHead[30];
	sSkill* m_pSkillShow[30];
	void MoveMagicBack(int x,int y);
	cText m_TitleT;
	int m_SkillSelect;

};

class cPCHead :public cInterfaceFather
{
public:

	BOOL CheckL();

	int m_MouseOn;
	int m_MouseOnPre = -1;
	//记录上一次的角色血条,如果不一样,再计算
	//总比每帧都计算乘法除法好

	void UpdatePcHp();
	void UpdatePcMp();
	void UpdatePcCp();
	void UpdatePcExp();
	void UpdatePetHp();
	void UpdatePetMp();
	void UpdatePetExp();

	BOOL UpDateHead();
	CWFile m_PetFaceBack; //召唤兽头像背景
	CWFile m_PCFace;
	vector<CWFile> m_TeamFace;
	vector<CWFile> m_TeamBack;
	vector<CWFile> m_TeamPetFace;
	CWFile m_PetFace;
	
	
	void SetPetHead(DWORD head);
	void UpdatePC();
	void UpdatePet();
	void Init();
	void MoveTeamHead(BOOL mode);
	
	BOOL Show(cCanvas* pCanvas);
	
	BOOL ProcessInput();
	virtual void OnOff(BOOL NeedShow){};
	BOOL CheckRB();
	
	void SetFighter(int id);
	void SetPet(int id);
	vector<cWfileP> m_EmptyLinep;
	CWFile m_EmptyLine;


	vector<cWfileP> m_HPlinep;
	vector<cWfileP> m_HeartHurtp;
	vector<cWfileP> m_MPlinep;
	vector<cWfileP> m_EXPlinep;

	CWFile m_MPline;
	CWFile m_CPline;
	CWFile m_HeartHurt;
	CWFile m_EXPline;
	CWFile m_HPline;

	CWFile m_Heart;
	CWFile m_PetHead;
	CWFile m_CharacterHead;
	cText m_TMouseOnLine;
private:
	cPet*   m_pPet = 0;      //在选择时的召唤兽
	cCharacter* m_pPc=0;
	BOOL m_bPetHeadMode[5];
	BOOL m_bInit = FALSE;
};
class cFullMagic
{
public:
	BOOL SetMagicUp(int dataid,DWORD id,int x,int y);
	vector<CWFile> m_FullSkilUP;
	vector<CWFile> m_SufferMagic;
	cFullMagic();
};


enum eChannel
{
	ec当前, //0
	ec队伍,    //1
	ec世界,
	ec系统,
	ec私聊
};
class cInterfaceChannel :public cInterfaceFather
{
public:
	void OnOff(BOOL NeedShow){};
	CWFile m_MoveIcon;
	CWFile* m_pCurrentChannel;
	CWFile m_Channel[5];
	CWFile m_Face[129];
	sFaceText m_ContextList[30];
	int     m_ContextStart;  //第1条的位置
	int     m_ContextNum;//总共有多少条
	int     m_ContextShowStart;//从哪条开始显示
	int    m_ContextShowNum;
	BOOL AddNewString( string str,string name="",int channel=3);
	void UpDateShow();
	void SetPos();
	int m_WidthChannel;
	int m_HeightChannel;

	int m_HeightCube;//行距
	CWFile m_Back;
	cWfileP m_BackP[4];
	BOOL Init();
	DWORD m_FaceId[129];
	virtual BOOL ProcessInput();
	virtual BOOL CheckL();
	BOOL Show(cCanvas* pCanvas);
	BOOL Move(int x,int y);
};
struct sMapselect
{
	DWORD m_SmallMap;
	string m_Name;
	vector<int> m_x, m_y;
	vector<int> m_xorg, m_yorg;
	int m_num;
	int m_Height;
	float m_bili;
};
class cCanvas;
class cInterfaceItem :public cInterfaceFather
{
public:
	
	cDescribe m_Describe;
	void  SetEquipSpecial(sItem2& ProPerty, int ID);
	
	void  SetEquipSpecial2(sItem2& ProPerty, int ID);//设置特效
	void  SetHaveZuoQi(BOOL b);
	void  SetEquipZuoQiZhuangShi(BOOL b);
	
	BOOL Move(int x, int y);
	int   m_ItemPage=0;

	BOOL   CheckZuoQi();
	CWFile m_EquipBack;  //当装备被装上时的背景
	

	CWFile m_BigHead;
	BOOL   m_bXianJin;
	CWFile m_XianJin;   //现金按钮
	
	BOOL   UseItem(int ID, cPCData& pcData);
	BOOL   UseItem(int ID,int TargetID);
	RECT m_rectEquip;
	RECT m_rectPetEquip;
	RECT m_rectItem;

	BOOL   m_SwapMode;
	CWFile m_SelectMark;
	CWFile m_MouseOnMark;
	vector<cWfileP> m_EquipBackP;
	vector<cWfileP> m_PetEquipBackP;
	vector<CWFile> m_ItemHead;
	vector<CWFile> m_EquipHead;//角色和召唤兽装备
	vector<CWFile> m_PetEquipHead;
	CWFile m_SwapItemSelect;
	//角色装备模式
	CWFile m_BigBigHead;
	CWFile m_Headback;
	//召唤兽装备模式
	CWFile m_PetHeadback;
	cWfileP m_ButtonPetMode;
	cText m_tButtonPetMode;
	cPet* pPet;
	cText m_PetName;
	//
	DWORD m_PetID;
	CWFile m_Item;
	CWFile m_ItemButton;
	CWFile m_ItemButton2;
	CWFile m_ZuoQiB;
	CWFile m_ZuoQiZhuangShiB;
	DWORD m_BackID;
	DWORD m_FabaoID;
	CWFile m_FabaoButton;
	DWORD m_ZuoQiID;
	int  m_SwapID;
	BOOL SetItemPage(int page);
	BOOL SwapItem(const int ID, const int IDTarget, cPCData* pcdata = 0);
	BOOL GetItem(int CharID, sItem2* pItem);  //得到物品
	void OnOff(BOOL NeedShow=FALSE);
	BOOL Overlay(int Pos, int CharID, sItem2* item);//叠加物品
	int  FindSame(int itemID, int itemType, int StartPos = 9, int EndPos = 28, int PcID = 2); //寻找可叠加物品
	int  FindIdle(int StartPos, int EndPos, cPCData& pcdata);//寻找空格 
	int  FindIdle(int StartPos = 0, int EndPos = 60, int PcID = 0);//寻找空格 
	BOOL Add(int Pos,int CharID,sItem2* item);//在空位置加物品
	BOOL Add(int Pos, cPCData& pcData, sItem2* item);//在空位置加物品
	void Set(int Num){ m_Select = Num; };
	BOOL ProcessInput();
	BOOL CheckL();
	BOOL CheckRB();
	void UpdateData();
	BOOL UpDateItemDate(cDescribe& Describe);//更新物品信息
	BOOL UpDateItemDate(cDescribe& Describe,sItem2* pItem);
	void Init();
	BOOL Show(cCanvas* pCanvas);
	BOOL LoadSmallMap(int ID);
	BOOL SetXianJin(BOOL b);
	~cInterfaceItem();
	cText m_TCunYingN;
	cText m_TXianJin;
	cText m_TXianJinN;
	cText m_TZuoQi;
	cText m_TZhuangShi;
	cText m_TItemNum[20];

	int m_NowSelect;
	//飞行符
	CWFile m_WorldMap;
	CWFile m_MapSelect;
	int    m_NumMapSelect;
	int    m_Mode;
	RECT   m_MapRect[7];
	DWORD  m_MapID[7];
	
	//导标旗
	BOOL ProcessSelectMap();
	BOOL ProcessSelectMap2();
	BOOL ProcessSelectMapMoon();
	CWFile m_MapBack;
	cText m_MousePos;
	CWFile m_MousePosBack;
	void UpdateMousePosBack();
	//string  m_WorldName;
	CWFile m_PathPoint;
	cWfileP m_PathPointp[30];//最多30个
	sMapselect*  m_pMapSelect = 0;
	sMapselect m_sMapSelect[4];
	BOOL ShowSelectMap();
	BOOL ShowSelectMap2();
	void InitMode(int id);
	int* pItemnum;
private:
	cObj m_Obj;
	BOOL m_bInit = FALSE;
	BOOL  m_bCharHaveZuoQi;
	void  UpdateItemNum(int ID);
};
enum eFormation  //阵型
{
	normal,//普通阵
	land,//地载阵
	wind,//风扬阵
	tiger,//虎翼阵
	dragon,//龙飞阵
	bird,//鸟翔阵
	snake,//蛇蟠阵
	sky,//天覆阵
	cloud,//云垂阵
	thunder,//雷绝阵
	eagle//鹰啸阵
};
class cInterfaceTeam :public cInterfaceFather
{
public:

	~cInterfaceTeam();
	void FreeFighter(int id);
	cText m_lineupName;
	int   m_ilineupSelect;
	cWfileP m_lineupSelectB;
	vector<cWfileP> m_lineupSelectBP;//一共有11个阵
	cWfileP m_SetFriend;
	cText m_tSetFriend;
	vector<cText>   m_lineupSelectBPT;
	cText m_lineupSelectBT;
	vector<cText> m_Describe;
	CWFile m_lineupB;
	/*cWfileP m_ControlB;
	cText  m_ControlBT;*/
	CWFile m_lineupSelectBack;
	BOOL   m_bLiineupSelect;
	cWfileP m_lineupCancelB;
	CWFile m_lineupPeople;
	vector<cWfileP*>  m_lineupPeoplepp;
	void  SortLineupPeople();
	vector<cWfileP> m_lineupPeoplep;
	vector<cText> m_lineupPeoplepT;
	void  SetlineupPeople(eFormation select);
	cText  m_lineupBT;
	cText  m_tSmall;
	cText  m_tBig;
	eFormation  m_eLineupSelect;
	CWFile m_FighterSelect;
	int  m_FighterSelectid;
	CWFile m_Back;
	RECT m_FightRect;
	void Move(int x, int y);
	void OnOff(BOOL NeedShow);
	void MoveLineSelect(int x, int y);
	BOOL Init();
	
	vector<cObj> m_Fighter;
	vector<cText> m_tName;
	vector<cText> m_tLv;
	vector<cText> m_tMengPai;
	
	BOOL ProcessInput();
	BOOL ProcessSelectLineup(int xMouse, int yMouse);
	BOOL CheckL();
	cWfileP* m_plineupButton=0;
	BOOL CheckRB();
	DWORD GetSmallHead(int id);
	DWORD GetPetHead(int id);
	BOOL Show(cCanvas* pCanvas);
	BOOL ShowSelectLineup(cCanvas* pCanvas);
	BOOL UpDate();
	void SetupOneFormationEffect(int Pos, cPetData* pPetData, BOOL UporDown);
	void SetFighterPos(BOOL enemy);
	vector<int> m_xPosList;
	vector<int> m_yPosList;
	inline int GetXPos(int id);
	inline int GetYPos(int id);
	int  GetReTramTrun(int id);
	int GetTeamTurn(int id){ return m_TeamTurn[id]; }
	void SetTeamTurn(int id,int turn){ m_TeamTurn[id]=turn; }
	void SwapTeamTurn(int id1, int id2);
	void SetFormation(eFormation format, BOOL enemy = FALSE,BOOL updatetext=FALSE);
	
	void UpdatePos();
	void Save(ofstream& File);

	void Load(ifstream& File);
	eFormation m_eEnemyFormation;
	eFormation m_eOurFormation;
private:
	
	BOOL AutoSwap(int i);
	BOOL m_bInit = FALSE;
	vector<int> m_TeamTurn;//用来调整队伍位置
	cFightState* m_pFightState;
	void SetFighterPos2(int** px,int** py,int xOf1,int yOf1 ,BOOL enemy);
};
struct sTags
{
	cObjFather m_Back;
	int m_time;
};
class cTags
{
public:
	void GetMoneyAndExp(int money, INT64 exp = 0, int chubei = 0,INT64 expbb=0,BOOL needtag=TRUE);
	void GetItem(string itemname);
	int m_Sleeptime;
	CWFile m_Back;
	BOOL Init();
	BOOL Add(string str,int time=500);
	BOOL Show(cCanvas* pCanvas);
	void Process();
	int  m_TagNum;
	sTags m_tags[10];//最多显示5条
	cText m_Text;
	~cTags();
private:
	BOOL m_bInit = FALSE;
};

class cWorldMap:public cInterfaceFather
{
public:
	enum  eMap
	{
		eMapWorld,	
		eMapWorldEast,
		eMapWorldWest,
		eMapWorldSouth,
		eMapWorldNorth,
		eMapWorldLG,
		eMapWorldTG,
		eMapWorldYG,
		eMapWorldPTY,
		eMapSouth,
		eMapNorth,
		eMapEast,
		eMapWest,
		eMapFenChao, //北俱芦洲 13-16
		eMapLongKu,
		eMapNvWa,
		eMapAL,		//东胜神洲 16-20
		eMapHGS,
		eMapNR,
		eMapPengLai,
		eMapCA,		//南 20-38
		eMapDT,
		eMapDF,
		eMapGLZ,
		eMapHZ,
		eMapHS,
		eMapJY,
		eMapJZ,
		eMapLG,
		eMapMW,
		eMapPS,
		eMapPT,
		eMapSML,
		eMapST,
		eMapD5,
		eMapWZ,
		eMapXXT,	
		eMapMJC,
		eMapBX,		//西  38-45
		eMapBQ,
		eMapCS,
		eMapFC,
		eMapZZ,
		eMapXL,
		eMapQLS
	};
	BOOL CheckRB();
	int m_xPre=0;
	int m_yPre=0;
	cText m_text;
	void OnOff(BOOL bShow);
	BOOL  MoveTest(CWFile& Target);
	virtual BOOL ProcessInput();
	BOOL ProcessInput0(); //世界
	BOOL ProcessInput1(); //东
	BOOL ProcessInput2(); //西
	BOOL ProcessInput3(); //南
	BOOL ProcessInput4(); //北
	virtual BOOL Show(cCanvas* pCanvas);
	BOOL	Show0(cCanvas* pCanvas);
	BOOL	Show1(cCanvas* pCanvas);
	BOOL	Show2(cCanvas* pCanvas);
	BOOL	Show3(cCanvas* pCanvas);
	BOOL	Show4(cCanvas* pCanvas);
	virtual BOOL CheckL();
	int m_Mode;
	CWFile m_Data[45];
	string m_MapName[45];
	void Init();
	void InitPos();
	CWFile* m_pShow = 0;
private:
	BOOL m_bInit = FALSE;
};

class cInterfaceRanSe:public cInterfaceFather
{
public:
	BOOL CheckRB();
	void UpdateColor();
	BOOL CheckL();
	void OnOff(BOOL NeedShow);
	void Init();
	BOOL Show(cCanvas* pCanvas);
	void Move(int x, int y);
	void SetMode(int mode, int Characterid);//sFriendList* plist=0);//0 角色染色  1坐骑染色  2 坐骑装饰染色 3 召唤兽染色
	void SetPet(int part, cCharacter* pc);
	BOOL ProcessInput();
	int m_Mode;
	CWFile m_Back;
	cText m_Title;
	cText m_tBuy;
	CWFile m_btBuy;
	cText m_Cost;
	CWFile m_Target;
	CWFile m_btSelect[4];
	cText  m_tSelect[4];
	DWORD  m_ColorID;
	int m_NowPart=0;
	sChangeColorData m_ChangeColorData;
	
private:
	CWFile m_NextPage;
	cObj m_Obj;
	sFriendList* m_plist = 0;
	BOOL m_bInit = FALSE;
};


class cBangPaiJiNengXueXi:public cInterfaceFather
{
public:
	BOOL Leveupskill(int lvup);
	void Init();
	void OnOff(BOOL NeedShow);
	virtual BOOL ProcessInput();
	virtual BOOL Show(cCanvas* pCanvas);
	virtual BOOL CheckL();
	BOOL CheckRB();
	void Move(int x, int y);
	void UpDate();
	void SetSkillSelect(int select);
	CWFile m_Back;


	CWFile m_SkillHead[14];
	cText m_tSkillHead[14];
	CWFile m_Button[2];
	cText m_tButton[2];
	cText m_tExp;
	cText m_tNeedBangGong;
	cText m_tNeedExp;
	cText m_tNeedMoney;
	cText m_tXueFei;
	cText m_tChuBei;
	cText m_tMiaoShu;
	CWFile m_SelectMark;

	int m_SkillSelect;
	
	INT64 m_NeedExp;
	INT64 m_NeedMoney;
	int m_NeedBangGong;

private:
	int m_BangpaiSkilllv=180;
	BOOL m_bInit = FALSE;
};

