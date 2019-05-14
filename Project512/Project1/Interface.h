#pragma once
#include <vector>
#include <string>
class cMyMap;
class CWFile;


class cInterfaceFather
{
public:
	int m_Sleeptime = 0;
	virtual BOOL ProcessInput() = 0;
	virtual BOOL Show(cCanvas* pCanvas) = 0;
	virtual BOOL CheckL() = 0;
	virtual void OnOff(BOOL NeedShow) = 0;
	BOOL m_NeedShow=FALSE;
	int m_xPos=0;
	int m_yPos=0;
	int  m_Prex=0;
	int  m_Prey=0;
	BOOL m_bMoveMode=FALSE;
	BOOL m_bMove=FALSE;
	BOOL isInRect(long x, long y, CWFile* pPic);
	cWfileP m_Cancel;
	
	BOOL CheckOn(CWFile* pTarget, int id);
	BOOL CheckOn(cWfileP* pTarget, int id);
	BOOL CheckOn(cText* ptext, int id);
	
	int m_Select=-1;
	int m_SelectPre=-1;
	CWFile* m_frampre=0;
	cWfileP* m_framprep=0;
	void ResetButton();

};


class cInterfaceNumShuRu
{
public:
	int CountPos(int pos);
	int CountCaretPos(int xMouse, int yMouse);
	void SetCaretPos(int pos);
	void UpdateCaret();
	BOOL Process();
	void Show(cCanvas* pCanvas);
	int m_num=0;
	void Init(RECT rect);
	void SetMaxNum(int num){ m_MaxNum = num; }
	cText m_tNum;
	
	void UpdateNum();
	void SetActive(BOOL bActive);
	void ShuRu(int num);
	void Erase(int pos);
	void Reset();
	void SetNum(int num);
	void SetHuChi(cInterfaceNumShuRu* huchi){ m_HuChiList.push_back(huchi); }
private:
	vector<cInterfaceNumShuRu*>  m_HuChiList;
	void Init2();
	int m_MaxNum=0;
	BOOL m_bActive = FALSE;
	int m_time=0;
	BOOL m_bInit = FALSE;
	CWFile m_Caret;
	vector<int>  m_numlist;
	int  m_numlong=0;
	int m_caretpos = -1;
	RECT m_rect;
	BOOL m_bEraseMode = FALSE;
	BOOL m_bInEraseMode = FALSE;
	int m_EraseStart;
	int m_EraseEnd;
};

class cDescribe :public cInterfaceFather
{
public:
	void OnOff(BOOL bonoff);
	void Set(int xMouse, int yMouse, DWORD BigHead, int num, string* pTex,DWORD* colorlist=0, int yOffset = 25);
	void Init();

	virtual BOOL ProcessInput(){ return TRUE; }
	virtual BOOL CheckL();
	virtual BOOL Show(cCanvas* pCanvas);
	cWfileP m_Back;
	static CWFile m_DescribeBack;
private:
	DWORD m_Color[10];
	
	BOOL m_bInit=FALSE;
	CWFile m_BigHead;
	cText  m_Text[10];
	int m_Num;
};
class cItemSelect
{
public:
	vector<CWFile> m_ItemHead;
	vector<cText>  m_tItemNum;
	vector<int> m_ItemNum;
	void Init();
	int Process(int xMouse,int yMouse);
	void Show(cCanvas* pCanvas);
	void Move(int xPos, int yPos);
	void Update(cCharacter* pc=0);
	cDescribe m_Describe;
	void SetSelectMark(int id);
private:
	BOOL m_bInit=FALSE;
	int m_Select;
	int m_xPos;
	int m_yPos;
	RECT m_Rect;
	CWFile m_SelectMark;
	CWFile m_MouseOnMark;
	
};
class  cInterFaceBuy: public cInterfaceFather
{
public:
	
	void UpdateText();
	RECT m_Rect;
	CWFile m_MouseOnMark;
	
	CWFile m_SelectMark;
	int m_TrueSelect;
	int m_TrueSelect2;
	virtual BOOL ProcessInput();
	virtual BOOL CheckL();
	BOOL CheckRB();
	BOOL Init();
	void Open(int id);
	BOOL Set(sItem2& item,int rate);
	BOOL Move(int x, int y);
	BOOL Show(cCanvas* pCanvas);
	cText m_Text[6];
	
	vector<sItem2> m_Item;
	vector<int> m_pricerate;
	CWFile m_ItemHead[20];
	void OnOff(BOOL bshow);
	cDescribe m_Describe;
	void Reset();
private:
	int m_ItemNum = 0;
	int m_page = 0;
	void AddPage();
	void SubPage();
	void Update();
	BOOL m_bInit=FALSE;
	int m_PriceTotal;
	void CoutTotal();
	CWFile m_Back;
	
	CWFile m_Button;
	cInterfaceNumShuRu m_NumBuy;

	CWFile m_Left;
	CWFile m_Right;
};
struct sFriendList;

struct sShangPinPet
{
	int m_modeid;
	string m_name;
	vector<int>  m_skillid;
	int m_price;
	sAddPoint m_addpoint; //�ӵ�
	sZiZhi m_zizhi;//����
	sChangeColorData2 m_ranse;
	void operator=(sShangPinPet pet)
	{
		m_modeid=pet.m_modeid;
		m_name=pet.m_name;
		m_skillid.swap(pet.m_skillid);
		m_price=pet.m_price;
		m_addpoint=pet.m_addpoint; //�ӵ�
		m_zizhi=pet.m_zizhi;//����
		m_ranse=pet.m_ranse;
	}
	void Set(int modeid, sAddPoint* addpoint = 0, int stronglv=100, BOOL needskill = FALSE);
	void AddSkill(int id);
	void Save(ofstream& File);
	void Read(ifstream& File);//���̵��ȡ�򻯵��ٻ���

	void Load(ifstream& File,int positon);//�Ӵ浵��ȡ
	void LoadEasy(int id,int strong = -1);
};


class  cInterFaceBuyPet : public cInterfaceFather
{
public:
	void UpdateText();
	virtual BOOL ProcessInput();
	virtual BOOL CheckL();
	BOOL CheckRB();
	BOOL Init();
	BOOL Set(sShangPinPet& item, int rate);
	BOOL Move(int x, int y);
	BOOL Show(cCanvas* pCanvas);
	void OnOff(BOOL bshow);
	void Init2(sFriendList* plist, UINT page1, UINT page2);
	void Init3();
	void AddPage();
	void SubPage();
	void UpDateTeam(string name,int stronglv,int lv=-1);
	sShangPinPet& GetPet3(int id);
private:
	cText m_Text[6];
	int m_ItemNum = 0;
	sShangPinPet m_Item[20];
	int m_pricerate[20];
	CWFile m_ItemHead[20];

	CWFile m_MouseOnMark;
	CWFile m_SelectMark;
	int m_TrueSelect = -1;

	RECT m_Rect;
	CWFile m_Left;
	CWFile m_Right;
	vector<sShangPinPet> m_ShangPinList[17];
	vector<sShangPinPet> m_ShangPinList3;
	BOOL m_bInit3=FALSE;
	int m_page=0;
	int m_page2=0;//����0���ٻ���,�����������20,��m_page2��������ҳ
	sFriendList* m_plist=0;
	BOOL m_bInit = FALSE;
	int m_PriceTotal;
	void CoutTotal();
	CWFile m_Back;
	
	CWFile m_Button;
	cInterfaceNumShuRu m_NumBuy;
};
enum eFightMenu
{
	FMENU_NORMAL,  //��ͨ
	FMENU_MAGIC2,   //����Ŀ��ѡ��
	FMENU_ITEM,  //��Ʒѡ��
	FMENU_GRAB,   //ץ
	FMENU_ITEM2,  //��ƷĿ��ѡ��
	FMENU_MAGIC, //����Ŀ��
	FMENU_PET,   //�ٻ�
	FMENU_PROTECT, //����
	FMENU_SPECIAL,  //�ؼ�
//	FMENU_CONDUCT,   //ָ��
//	FMENU_FABAO,  //����
	FMENU_SPECIAL2
};
class cInterfaceConduct :public cInterfaceFather
{
public:
	void PrePare();
	virtual BOOL ProcessInput();
	virtual BOOL Show(cCanvas* pCanvas);
	virtual BOOL CheckL();
	virtual void OnOff(BOOL NeedShow);
	BOOL CheckRB();
	void SetID(int id);
	void  Init();
	void Move(int x, int y);
	void Def(int id, BOOL bDef = TRUE);
	void SetTarget(int id,int target);
private:
	cObj* m_pObj = 0;
	int m_Mode=-1;
	int m_FighterNum;

	CWFile m_Back;//ָ�ӽ���  0xCC7BC6C6
	BOOL m_bInit = FALSE;
	cText m_Title;
	int  m_IdSelect;
	vector<cWfileP> m_Buttonp;
	vector<cText> m_tButtonp;
	vector<cText> m_tName;
	vector<cText> m_tTargetName;
};


class cInterfaceFight:public cInterfaceFather
{
public:
	cInterfaceFight();
	void SetTeamControl(BOOL bC);
	cInterfaceConduct m_InterfaceConduct;
	void OnOff(BOOL NeedShow);
	RECT m_ItemRect;
	cDescribe m_Describe;
	void InitPet();
	void ShowPet();
	void MovePet(int x, int y);

	CWFile m_Button;
	cText m_PetName[4];
	cText m_PetProPerty[6];
	cText m_Text;
	CWFile	m_UpB;
	CWFile	m_DownB;
	int  m_PetPage;
	int m_PetIDSelect;
	int m_TruePetIDSelect;
	CWFile m_PetBack;
	CWFile m_ItemDataBack;
	CWFile m_BackMagic6;//��������  0x2FD95E30
	CWFile m_BackMagic10;//�������� 0x69823EE5
	CWFile m_BackMagic14;//��������  0A8B7D87
	CWFile m_BackMagic20;//��������  0x32F119A5
	CWFile m_BackMagic30;// 0x4226BD41
	CWFile m_BackPet; // 0xE87F26E1
	CWFile m_BackItem;//  0xE3AEF018
	BOOL AddTurn();
	BOOL CheckTurn(int ID);
	
	BOOL UpdateMenu();
	void SwitchMode(eFightMenu eFightMenu);
	void SelectMode(eFightMenu eFightMenu);
	BOOL Move(int x, int y);
	void MoveMagicBack(int x,int y);
	void MoveMenu(int x, int y);
	void MoveItem(int x,int y);
	int  m_NumofButtonShow;  //��Ҫ��ʾ�İ�ť��


	CWFile m_Magic;
	CWFile m_Stunt;  //�ؼ�
	CWFile m_Conduct; //ָ��
	CWFile m_Item;
	CWFile m_Defend;
	CWFile m_Guard;
	CWFile m_Summon; //�ٻ�
//	CWFile m_Auto;
	CWFile m_Catch; //ץ
	CWFile m_Fabao;
	CWFile m_Escape;  
	eFightMenu    m_MenuMode;
	void   InitItem(int offset);
	//BOOL   SetSpecialMode(BOOL b);
	CWFile m_ItemSelectFrame;
	int    m_NumofSkill;
//	CWFile m_ItemHead[20];

	sSkill* m_pSkillShow[30];
	
	sSkill* m_pSkillSelect=0;
	CWFile m_SkillHead[30];

	BOOL   InitMagic2();
	BOOL   InitMagic();
	BOOL   InitSpecial();
	void   MoveSpecial(int x, int y);
	void   ShowSpecial(cCanvas* pCanvas);
	BOOL   ProcessSpecial();
	BOOL   ProcessSpecial2();
	BOOL  ProcessGrab();
	BOOL  ProcessItem2();
	
	BOOL  ProcessProtect();
	BOOL ProcessPet();
	cText  m_TitleT;
	cText  m_SkillPreSelectT;
	cWfileP m_MagicBack;
	int    m_SpecialNum=0;
	int	   m_SpecialID[6];
	CWFile m_SpecialHead[6];
	int    m_SpecialSelect;

	virtual BOOL ProcessInput();
	BOOL ProcessMenu();
	//��Ʒ
	CWFile m_PetSelectBack[2];
	CWFile m_ItemBack;
	
	int  m_ItemSelect;
	
	void Init();
	BOOL m_bInit=FALSE;
	BOOL SetNowFight(int fight);
	BOOL Show(cCanvas* pCanvas);
	BOOL   m_ItemCanUse[20];


	int  m_NowFighterID;
	int  m_PreFighterID;

	
	cFightState* m_pFightState;
	BOOL CheckL();
	//�ƶ�
	int m_xMenuPos;
	int m_yMenuPos;
	BOOL m_MoveBack = FALSE;
	int m_OldxMouse;
	int m_OldyMouse;
	void SetControl(int id, BOOL cancontrol){ m_bCanControl[id] = cancontrol; }
	BOOL CheckControl(int id);
private:
	BOOL m_bCanControl[20];
	cObj* m_pObjSelect=0;
	//int SelectObj(int Userid,int type);
	inline  BOOL ProcessItem();
	inline  BOOL ProcessNormal();
	inline  BOOL ProcessMagic2();
	inline  BOOL ProcessMagic();
	
	inline	BOOL CheckA();
	inline	BOOL CheckQ();
	inline	BOOL CheckD();
	inline	BOOL CheckT();
	inline	BOOL CheckG();
	inline	BOOL CheckW();
	inline	BOOL CheckX();
	inline	BOOL CheckS();
	inline	BOOL CheckE();
	inline	BOOL CheckV();

	inline	BOOL CheckRButton();
	inline	void Def();
	inline	void Guard();
	inline	void ShowMenu(cCanvas* pCanvas);
	inline	void ShowItem(cCanvas* pCanvas);
	inline	void ShowMagic(cCanvas* pCanvas);
};
struct sTalkList;

struct sPoem
{//һ��ʫ
	string m_title;
	string m_name;
	vector<string> m_context;
	void Read(ifstream& File)
	{
		File >> m_name;
		File >> m_title;
		int num;
		File >> num;
		m_context.resize(num);
		for (int i = 0; i < num; i++)
			File >> m_context[i];
	}
};
struct	sQuestion
{
	string title;
	vector<string> select;
	int  answer; //��ȷ��

	void Auto()//�ı���Ŀ��ѡ���λ��
	{
		int a = rand() % select.size();
		if (a != answer)
		{
			swap(select[answer], select[a]);
			answer = a;
		}
		a = (a + 1) % select.size();
		int b = rand() % select.size();
		if (a != b)
		{
			swap(select[a], select[b]);
		}
		if (b == answer)answer = a;
	}
};
class cInterfaceDialog :public cInterfaceFather
{
private:
	vector<sPoem>  m_PoemList;
	void AutoGetPoem(sPoem*& out);//����õ�����,������Ϊtitle;
public:
	string GetTrueSelect();
	void GetQuestion(sQuestion& quest, int selectnum = 4);
	
	void InitQuestion();

	BOOL m_bInitQuestion = FALSE;
	int m_retSelect=-1;

	void Talk(sTalkList* talk);
	void Talk(string context ,string name,DWORD head);
	void TalkNObj(string context);
	void TalkObj(string context,cObj* p=0);
	sFaceText m_ContextList;
	CWFile m_DialogBack;
	int    m_NumOfSelect;
	vector<cText>  m_SelectT;
	int    m_NowMouseOnSelect=-1;
	void Reset();
	void Reset2();
	CWFile m_NameBack;
	void OnOff(BOOL bShow);
	
	void Init();
	void SetObj(string name);
	void SetObj(cObj* pObj);
	void SetHead(DWORD head);
	void SetPet(cPet* p);
	
	BOOL  ProcessInput();
	BOOL  CheckL();
	void Close();
	BOOL  Show(cCanvas* pCanvas);
	BOOL  Free();
	void  SetString(string str="", string select1="", string select2="", string select3="",string select4="");
	void  Question();
	cObj* m_pObj;
	void SetTrigger(cTaskFather* pTask,int defaultret ,int stage0=-1, int stage1=-1, int stage2=-1, int stage3=-1);
	BOOL m_bRetSelect = FALSE;
	sQuestion m_Question ;
private:
	
	cTaskFather* m_pTask = 0;
	int m_retstage[5];
	BOOL m_Reset=FALSE;
	CWFile m_Head;
	BOOL m_bInit = FALSE;
};


struct sFriendList
{
	int GetTeamID();
	BOOL CheckInTeam();
	//BOOL m_bOnLine;
	int m_traderLV=0;
	int m_Feeling=0;//�øж�
	string m_Name;//����
	string m_dataFileName;//�����ļ�������
	int  m_stronglv;//��ɫ��ǿ�Ȼ�ı�
	int  m_modeid;
	BOOL m_bDuJie=0;//�Ƿ�ɽ�
	//��������
	int m_weapontype=-1;
	int m_weaponid=-1;
	cJinJieNum m_JinJieNum;
	sZiZhi m_zizhi;//����
	//Ⱦɫ
	sChangeColorData2 m_Ranse;
	//Ǯ
	int  m_money;
	//����
	BOOL  m_bOnZuoQi=0;
	int   m_ZuoQiID=-1;
	sChangeColorData m_ZuoQiRanse;
	int   m_bZhuangShi=0;
	int   m_ZhuangShiID=-1;
	sChangeColorData m_ZhuangShiRanse;

	int  m_FightBB=-1;//�׷��ٻ���
	vector<sShangPinPet>  m_BBList;
	//vector<int>  m_PetSkillList;
	void Save(ofstream& File);
	void Load(ifstream& File);
	DWORD GetFriendHead();
	DWORD GetDialogHead();
};
class cInterfaceFriendQiangHua :public cInterfaceFather
{
public:
	void Set(sFriendList& list);
	void Update();
	void Init();
	BOOL CheckRB();
	virtual BOOL ProcessInput();
	virtual BOOL Show(cCanvas* pCanvas);
	virtual BOOL CheckL();
	virtual void OnOff(BOOL NeedShow);
	void Move(int xPos, int yPos);

private:
	BOOL m_bUpdateTeam;
	int m_Mode=0;
	sFriendList* m_pData;
	cWfileP m_Buttonp[12];
	cText m_Name;
	cText m_Money;
	cText m_title;
	BOOL m_bInit = FALSE;
	CWFile m_Back;
	
	cText m_Text[12];
};
class cFriend:public cInterfaceFather
{
public:
	cInterfaceFriendQiangHua m_friendqianghua;
	const int m_MaxNameNum=6;
	cText m_ButtonText[4];
	cWfileP m_Buttonp[4];
	
	CWFile m_PetSelectBack[2];
	int  m_TrueFriendSelect=-1;

	cText m_NameList[6];//
	CWFile m_NameListHead[6];
	BOOL UpdateList(int page=-1);
	CWFile m_Back;
	BOOL Move(int x, int y);
	BOOL Init();
	struct sMessageList
	{
		string	messagePath;
		int id;
		cTaskFather* p;
		int taskid;
		void Save(ofstream& File);
		void Load(ifstream& File);
	};
	void AddMeesage(string path, int id, cTaskFather* ptask, int taskid);
	vector<sMessageList> m_MessageList;

	BOOL ProcessInput();
	BOOL CheckL();
	void JuanZeng(int num);
	void UnJuanZeng(int num);
	BOOL CheckRB();
	BOOL Show(cCanvas* pCanvas);
	
	void OnOff(BOOL NeedShow);
	
	BOOL Invite(string name, int fighterid = -1);
	BOOL UnInvite(string name);
	BOOL UnInvite(int id);
	vector<sFriendList> m_FriendList;
	void Save(ofstream& File);
	void Load(ifstream&File);
	BOOL AddFriend(string filepath);
	int FindFriend(string name);
	sFriendList* FindFriend2(string name){ return &m_FriendList[FindFriend(name)]; }
	void ReMove(string name);
	void AddDay();
private:
	void operator=(cFriend& list){}
	BOOL Invite(int id, int fighterid = -1);//����   �����Ƿ�����ɹ�
	RECT m_Rect;
	BOOL m_bInit = FALSE;
	int m_nowPage=0;
	int m_NameNum=0;
};

class cInterfaceSmallMap:public cInterfaceFather
{
public:
	cInterfaceSmallMap(){ m_NeedShow = FALSE; }
	void Init();
	BOOL ProcessInput();
	BOOL CheckL();
	CWFile m_PathPoint;
	cWfileP m_PathPointp[30];
	CWFile m_PCPoint;
	BOOL Show(cCanvas* pCanvas);
	int m_xTarget;
	int m_yTarget;
	void OnOff(BOOL bShow);
	cText m_MousePos;
	CWFile m_MousePosBack;
private:
	
	BOOL m_bInit = FALSE;
	BOOL PrePareFindPath(int xMouse, int yMouse);
};
class cShiCheng
{
public:
	void Save(ofstream& File);
	void Load(ifstream& File);

	DWORD  m_23_1;  //��
	DWORD  m_1_3;   //��
	DWORD  m_3_5;   //��
	DWORD  m_5_7;   //î
	DWORD  m_7_9;   //��
	DWORD  m_9_11;  //��  
	DWORD  m_11_13;  //��
	DWORD  m_13_15;  //δ
	DWORD  m_15_17; //��
	DWORD  m_17_19; //��
	DWORD  m_19_21;  //��
	DWORD  m_21_23;  //��

	CWFile m_Data;
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int s;
	int ss;
	int m_fps;
	int m_week; //����
	cShiCheng()
	{
		m_23_1 = 0x361FA820;
		m_1_3 = 0xC0A66903;
		m_3_5 = 0xD1D11294;
		m_5_7 = 0xAA7DEB05;
		m_7_9 = 0x21274A87;
		m_9_11 = 0x9C4978D;
		m_11_13 = 0xC9E2F072;
		m_13_15 = 0x2ACB36B2;
		m_15_17 = 0xC26BF189;
		m_17_19 = 0x1AA170AE;
		m_19_21 = 0x7921D3A3;
		m_21_23 = 0xEA7CAB84;
		m_Data.SetX(0);
		m_Data.SetY(25);
		m_Data.SetStatic(TRUE);
	}
	void SetFPS(int fps){ fps = m_fps; }
	void UpdateTime(); //ǰ��һ����
	void AddMinute(int add);
	void AddHour(int num=1);
	void AddDay();
	void AddMonth();
	void AddYear();
	void Init(int fps, int y, int m, int d,int h, int mi, int secend, int millisecond = 0)
	{
		m_fps = fps;
		year = y;
		month = m;
		day = d;
		hour = h;
		minute = mi;
		s = secend;
		ss = millisecond;
		CountDay();
		UpdateShiCheng(hour);
	}
	void CountDay();
	void UpdateShiCheng(int h);
	BOOL CheckWhite(){ return m_bWhite; }
private:
	BOOL m_bWhite;
};

class cInterfaceDoor:public cInterfaceFather
{
public:
	int m_SaveNum=0;
	void OnOff(BOOL NeedShow){};
	cText m_CharacterPos;
	char   m_MapNameBuf[50];
	cText  m_MapName; 
	BOOL  m_MapNameNeedShow;//�Ƿ���ʾ��ͼ������
	cShiCheng m_TimeShiCheng;//ʱ��
	CWFile m_SmallMap;
	int    m_time;  
	CWFile m_Back;
	CWFile m_Door;
	CWFile m_WorldmapGlim; //����
	CWFile m_Glim;
	CWFile m_MapGlim;
	int   m_GlimSelect;
	CWFile m_SmallPeople[2];
	BOOL ProcessInput();
	void UpdatePos(cObj* pObj);
	BOOL CheckL();
	void Init();
	BOOL Show(cCanvas* pCanvas);
	void SetName(string name);
	cInterfaceDoor(){ m_time = 0; }
	int m_xoldHeroPos;
	int m_yoldHeroPos;
	int wid = 70;
	BOOL m_bRunMode=0;
	void SetTrigger(cTaskFather* pFather=0,int stage=-1,int least=1024);
	void CheckFight();
	int m_AvoidFight = 0;
	void SetWhite(BOOL b);
	BOOL CheckWhite(){ return m_bWhite; }
	int m_HeroX;
	int m_HeroY;
	void Save(ofstream& File);
	void Load(ifstream& File);
	void SetHalf(BOOL bTrue = -1);
	cTaskFather* m_pTask;
	int m_stage;
private:
	BOOL m_bHalf = FALSE;
	BOOL m_bWhite=2;
	BOOL m_bInit = FALSE;

	int m_timeleast;  //�������ֵ����ʱ����
	int  m_rate;//������
	int m_nowtimeleast=200;
	

};

class cInterfaceFB:public cInterfaceFather
{
public:
	void OnOff(BOOL NeedShow);
	BOOL CheckRB();
	void Init();
	BOOL ProcessInput();
	BOOL Show(cCanvas* pCanvas);
	BOOL CheckL();
	BOOL Move(int x, int y);
	//void Reset();//���ø���״̬
	vector<cText> m_tFBName;
private:
	int m_TrueSelect;
	RECT m_Rect;
	CWFile m_MouseOnBack;
	cText m_TButton;
	cText m_Title;
	CWFile m_Button;
	CWFile m_Back;
	BOOL m_bInit = FALSE;
};
class cInterfacePetSkill :public cInterfaceFather
{
public:
	int m_ItemDataTime = 0;
	void Move(int x, int y);
	void MoveSkillHead(int x,int y);
	void UpdateData(cPetData* pPetData, BOOL candashu);
	void NextPage(BOOL bnext=TRUE);
	void Init();
	virtual BOOL ProcessInput();
	virtual BOOL CheckL();
	void OnOff(BOOL NeedShow);
	BOOL CheckRB();
	virtual BOOL Show(cCanvas* pCanvas);
	int m_FightID = 0;
	CWFile m_back;
	cWfileP m_Cancel2;
	RECT m_Rect;
	BOOL m_bDaShuMode = 0;
	cWfileP m_EquipBackp[3];
	CWFile m_EquipHead[3];
	
private:
	BOOL m_bCanDaShu=FALSE;
	BOOL m_bInit=FALSE;
	int m_SkillSelect;

	cText m_DaShuText[2];
	int m_DashuSkillSelect;
	BOOL  InitDaShuMode();//�����ɫ�ĵ�������û�м�����,����False;
	CWFile m_DaShuHead[9];
	int    m_DaShuItemId[9];//��¼��ɫ�������м�����
	cPetData* m_pPetData;
	cText m_TPetProPerty[9];
	int m_SkillPage = 0;
	int m_NumSkill;
	CWFile m_SkillHead[12];
	sItem2* m_pEquip[3];
	CWFile  m_EquipBack;

	cWfileP m_SkillSelectHead;
	int m_DaShuNum;
	CWFile m_DaShuBack;
	CWFile m_NextPage;
	
	
	cDescribe m_Describe;
};

class cInterfaceChaKan :public cInterfaceFather
{
public:
	void Init();
	void Set(cObj* data);
	void Set(sFriendList& list);
	void Set(sShangPinPet& list);
	virtual BOOL ProcessInput();
	virtual BOOL Show(cCanvas* pCanvas);
	virtual BOOL CheckL();
	virtual void OnOff(BOOL NeedShow);
	void Move(int xPos, int yPos);
	BOOL CheckRB();
	void MovePetEquip();
private:
	cText m_MengPai;

	cText m_Property[15];
	cPetData* m_data;
	CWFile m_Back;
	BOOL m_bInit = FALSE;
	cInterfacePetSkill m_PetSkill;
	CWFile m_Pic[4];
	cWfileP m_Shadow;
	cObj m_Obj;
};

class cInterfacePet:public cInterfaceFather
{
public:
	void AddExp(cPetData* pPetSelect, INT64 num);
	cInterfacePetSkill m_PetSkill;
	void OnOff(BOOL NeedShow);
	//CWFile* m_frampre;
	//cWfileP* m_framprep;


	BOOL   Move(int x, int y);//�ƶ�����
	CWFile m_UpB;
	CWFile m_DownB;
	//CWFile m_ItemDataBack;
	
	CWFile m_SetAttackB;
	CWFile m_AddPoint[5];
	CWFile m_SubPoint[5];
	CWFile m_XunYang;
	CWFile m_ChaKanZiZhi;
	CWFile m_FreePet;//����
	cWfileP m_SetAddPoint;//���üӵ�
	BOOL  m_bAddPoint=FALSE;
	void   FreePet();
	BOOL   m_FreePetMode;
	//CWFile m_DaShu;
	int m_SkillPage;//�������������12��,��Ҫ��ҳ
	int m_PetPage=0;//���BB������4��,��Ҫҳ
	
	CWFile m_back;
	
	CWFile m_PetSelectBack[2];

	cText m_TPetName[4];
	void Init();

	BOOL MovePetProPerty();
	BOOL CheckL();
	BOOL ProcessFreePet();
	BOOL ProcessInput();
	BOOL UpdateDescript(CWFile* pHead,sPetSkill* pPet,int xMouse,int yMouse);
	BOOL Show(cCanvas* pCanvas);
	BOOL SetPage(int page);
	void UpdateList(int page);
	//BOOL InitPage();
	void UpDateExp();
	void UpDateByLvUp();  
	int m_TrueSelect;
	

	int m_ItemDataTime;
	
	cText m_TSkillSelectData[2];
	//CWFile* m_PetPic[4]; //һ���ٻ��޿�����4���������,���⻹�и���
	cObj m_PetObj;
	//cWfileP m_Shadow;
	int m_PetSelectID=0;
	
	cText m_TPetProPerty[25];
	~cInterfacePet();
	BOOL Free();
	void UpdatePointButton();
	void UpdateProPerty();
private:
	
	BOOL m_bInit=FALSE;
	cObj  m_FreePetObj;
	BOOL SelectPet(int id);
	/*BOOL CheckOn(CWFile* pTarget, int id);
	BOOL CheckOn(cWfileP* pTarget, int id);*/
	/*int m_xMouse;
	int m_yMouse;*/
};
class cInterfaceJiNeng:public cInterfaceFather
{
private:
	
	int m_MiaoShuID=-1;
public:
	CWFile m_SelectBackXiuLian;
	cWfileP* m_pPreMode=0;
	void SetMode(DWORD mode);
	void OnOff(BOOL NeedShow);
	BOOL CheckL();
	CWFile m_Data;
	vector<cText> m_BText;
	cText m_ZhuJiNengLv[7];
	cText m_FuZhuJiNengLv[14];
	cInterfaceJiNeng();
	
	DWORD m_ZhuJiNeng;
	DWORD m_XiuLian;
	DWORD m_FuZhuJiNeng;

	
	cWfileP m_SmB;
	cWfileP m_FuZhuB;
	cWfileP m_learnSkillB;
	cWfileP m_XiuLianB;
	void Init();
	BOOL ProcessInput();
	void Set(DWORD ID);
	BOOL Show(cCanvas* pCanvas);
	BOOL Move(int x,int y);
	void MoveMainSkill(int x,int y);
	void MoveFuZhuSkill(int x, int y);
	void MoveXiuLian(int x, int y);
	CWFile m_ZhuJiNengHead[7];
	CWFile m_SpecialSkill[6];
	int    m_JiNengHeadNum;

	sSkill* m_JiNengHeadData[14];
	CWFile m_JiNengHead[14];
	CWFile m_TeJiHead[6];
	int    m_NumMiaoShu=0;
	vector<cText>  m_MiaoShu;
	int    m_SpeciallSkillSelect;
	int    m_JiNengSelect;
	int    m_SkillSelect;
	int    m_dustSkillSelect;
	vector<cText>  m_TXiuLian;
	BOOL   UpdateJiNeng();
	BOOL   m_bUpdateJiNeng=0;
	BOOL   UpdateXiuLian();
	BOOL   UpdateMainSkill();
	BOOL   UpdateFuZhuJiNeng();
private:
	BOOL m_bInit = FALSE;
};


class cTaskFather;
struct sJob  //��ʾһ������
{
	cTaskFather* m_pTask=0;
	int m_Stage=-1;
	string m_Title;
	string m_Context;
	int m_Time=0;
	void Set(string title, string context, int time, cTaskFather* task, int Stage)
	{
		m_Stage = Stage;
		m_Time = time;
		m_Context = context;
		m_Title = title;
		m_pTask = task;
	}
	void operator=(sJob& target)
	{
		m_Stage = target.m_Stage;
		m_Context = target.m_Context;
		m_pTask = target.m_pTask;
		m_Title = target.m_Title;
		m_Time = target.m_Time;
	}
};
struct sJobCharcter
{
	string characterName;
	int xPos;
	int yPos;
	string MapName;
	int direction;
	int TeamPos;
	string taskName;
};

class cCanvas;
class cInterfaceJob:public cInterfaceFather
{
public:
	void CheckErr();
	BOOL CheckHaveJob(int ID);//����Ƿ����ĳ������
	void SetSelect(int select);
	void UpDatePageTitle();
	int m_Page = 0;
	BOOL AddaJob(sJob& job,int huanshu=-1);
	BOOL ReMoveJob(cTaskFather* p);
	void Reset(){ m_JobList.clear(); }
	
	cText m_TitleList[8];
	sFaceText m_Context;
	void OnOff(BOOL NeedShow);
	//CWFile* m_pPreButton;
	
	vector<CWFile> m_Back;
	int  m_gap;
	CWFile m_Up;
	CWFile m_Down;
	int m_Check;
	BOOL Move(int x, int y);
	sJob m_List[10];
	CWFile m_Job;
	void Init();
	BOOL ProcessInput();
	BOOL CheckL();
	BOOL CheckRB();
	BOOL Show(cCanvas* pCanvas);
	void MoveContext(int x, int y);
	void Save(ofstream& File);
	void Load(ifstream& File);
	
private:
	vector<sJob> m_JobList;
	BOOL m_bInit=FALSE;
};


class cTrigger;
class cObjTrigger;
class cMapObjTrigger;
class cLvTrigger;
class cInterfaceSystem:public cInterfaceFather
{
public:
	void GetSaveTitle(vector<string>& list);
	void Save(int id);
	void Load(int id);
	cWfileP m_ExtGame;
	cWfileP m_SaveGame;
	cWfileP m_LoadGame;
	cInterfaceSystem(){ m_xPos = 100; m_yPos = 100; }
	BOOL ProcessInput();
	BOOL CheckL();
	BOOL CheckRB();
	CWFile m_Back;
	cText m_tExtGame;
	cText m_tSaveGame;
	cText m_tLoadGame;
	cText m_tTitle;
	//sFaceText m_tTime;
	void Init();
	BOOL Move(int x, int y);
    BOOL Show(cCanvas* pCanvas);
	void OnOff(BOOL needshow);

private:
	int m_bMode=0;
	BOOL m_bInit=FALSE;
	void SavePos(ofstream& File);
	void SaveFriend(ofstream& File);

	void SaveFightList(ofstream& File);
	void SaveTrigger(ofstream& File);
	void SaveATrigger(cTrigger* trigger, ofstream& File);
	void SaveObjTrigger(cObjTrigger* trigger, ofstream& File);
	void LoadObjTrigger(cObjTrigger* trigger, ifstream& File);




	void LoadPos(ifstream& File);
	
	void LoadFriend(ifstream& File);
	
	void LoadFightList(ifstream& File);
	void LoadTrigger(ifstream& File);
	void LoadATrigger(cTrigger* trigger, ifstream& File);
	
};
class cMenu:public cInterfaceFather
{
public:
	void OnOff(BOOL NeedShow){};
	CWFile* m_NowSelectPic;
	BOOL Move(int x, int y);
	
	cText m_Text;
	int m_gap;
	int m_SelectPre;
	CWFile m_Trade;
	CWFile m_back;
	CWFile m_Attack;
	CWFile m_Team;
	CWFile m_Pet;
	CWFile m_Item;
	CWFile m_SkillAccelater;
	CWFile m_Info;
	CWFile m_Give;
	CWFile m_BangPai;
	CWFile m_System;
	CWFile m_job;
	void Init();
	BOOL Show(cCanvas* pCanvas);
	void Select(int x);
	virtual BOOL ProcessInput();
	virtual BOOL CheckL();
};
class cCursor
{
public:
	int SelectObj(int UserID, int Type, cObj*& pObj);
	int m_FollowTime=0;
	BOOL   m_FollowMode;
	DWORD   m_Give;
	DWORD   m_Normal;
	DWORD   m_Forbid;
	DWORD   m_Trade;
	DWORD   m_Hande;
	DWORD   m_Attack;
	DWORD   m_Magic;
	DWORD   m_Team;
	DWORD   m_Guard;
	DWORD   m_Grab;
	DWORD   m_No;
	CWFile  m_Data;
	CWFile  m_Click;
	vector<cWfileP>  m_Clickp;
	cCursor();
	void Set(DWORD  Type)
	{
		if (m_Data.m_NowID == Type)return;
		m_Data.Load(Type); 
	}
	BOOL Init();
	BOOL Process();
	BOOL CheckRB();
	BOOL Show(long x, long y);
private:

};

class cState :public cInterfaceFather
{
public:

	void OnOff(BOOL NeedShow);
	cText m_TPetDate[9];
	vector<cText> m_TPcDate; //27 ����  28 ����

	CWFile m_LeveUp;
	cText m_Text;
	cWfileP m_ButtonLvUp;   //����
	cWfileP m_ButtonSkill;  //����
	int m_SkillPage;//�������������12��,��Ҫ��ҳ
	CWFile m_SkillHead[12];
	int m_SkillSelect;
	cWfileP m_SkillHeadSelect;
	////�鿴����

	cText m_TSkillHeadSelectDate[2];
	cWfileP m_SeeZZ; 

	cInterfacePetSkill m_PetSkill;
	BOOL UpDatePcDate();
	void UpdateAddPointB();
	void UpdateProPerty();
	void UpDateHp();
	void UpDateMp();
	void UpDateCp();
	vector<cWfileP> m_Addp;
	vector<cWfileP> m_Subp;
	CWFile m_Add;
	
	CWFile m_Sub;
	CWFile* m_pShow;
	BOOL Init();
	BOOL ProcessInput();
	BOOL CheckL();
	BOOL CheckRB();
	BOOL SetFight(cCharacter* pc);
	cPCData* m_pPCData;
	BOOL Show(cCanvas* pCanvas);
	BOOL Move(int x, int y);
	int m_NowFighter;
	CWFile m_State;
	void SetAddPoint();
	BOOL CheckAddPoint(){ return m_bAddPoint; }
private:
	BOOL m_bAddPoint;
	BOOL m_bNeedLVUP=FALSE;
	BOOL m_bInit = FALSE;
};
class cInterfaceLearnSkill :public cInterfaceFather
{
public:
	BOOL CostSkill(INT64 needexp, INT64 needmoney);
	CWFile m_SkillSelectBack[2];
	virtual BOOL ProcessInput();
	virtual BOOL Show(cCanvas* pCanvas);
	virtual BOOL CheckL();
	BOOL LearnSkill(BOOL cost=TRUE);
	void OnOff(BOOL needshow);
	void Move(int x, int y);
	void Init();
	BOOL CheckRB();
	CWFile m_Back;
	
	cWfileP m_Button[2];
	cText m_tButton[2];

	RECT m_Rect;
	int m_SkillSelect;
	CWFile m_MengPaiSkillHead[7];
	cText m_tMengPaiSkill[7];
	cText m_tMengPaiSkillLv[7];
	cText m_tSkillSelectHead;
	cText m_tMiaoShu;
	cText m_tNeedExp;
	cText m_tHaveExp;
	cText m_tNeedMoney;
	cText m_tHaveMoney[3];
	void Update();
	void SetSkillSelect(int select);
private:
	BOOL m_bInit = FALSE;
};
class cInterfaceXiuLian:public cInterfaceFather
{
public:
	void CheckInit(){ if (!m_bInit)Init(); };
	BOOL Leveupskill(int lvup);

	void Init();
	void OnOff(BOOL NeedShow);
	virtual BOOL ProcessInput();
	virtual BOOL Show(cCanvas* pCanvas);
	virtual BOOL CheckL();
	BOOL CheckRB();
	void Move(int x, int y);
	void UpDate();
	
	CWFile m_SkillSelectBack[2];
	CWFile m_Back;
	CWFile m_Button[2];
	cText m_tButton[2];
	cText m_MiaoShu;
	cText m_TXiuLian[4];
	RECT m_Rect;
	
private:
	int m_SkillSelect;
	BOOL m_bInit=FALSE;
};
class  cInterfaceGive:public cInterfaceFather
{
public:
	void AddItem(int select);
	virtual BOOL ProcessInput() ;
	virtual BOOL Show(cCanvas* pCanvas) ;
	virtual BOOL CheckL() ;
	void ReCoverOnce();//�յ��Ļ���������Ҫ��,�˻�
	BOOL CheckRB();
	void OnOff(BOOL NeedShow);
	void Init();
	void Move(int x, int y);
	BOOL m_bInit = FALSE;
	void SetTarget(cObj* pTarget);
	cObj* GetTarget();
	sItem2 m_ItemGive[3];
	cInterfaceNumShuRu m_NumItem[3];
	int    m_ItemGiveOwner[3];
	cObj* m_pTarget;
private:
	
	cInterfaceNumShuRu m_NumShuru;
	CWFile m_SelectMark;
	CWFile m_MouseOnMark;
	int m_ItemSelect;
	void UpDate();

	CWFile m_Back;
	CWFile m_Button;
	cText  m_tButton;
	
	int m_ItemNum[20];
	CWFile m_ItemHead[20];
	cText m_tItemHead[20];
	CWFile m_ItemGiveHead[3];
	
	
	cText m_tXIanJin;
	RECT m_Rect;
};

class cInterfaceYingHang :public cInterfaceFather
{
public:
	void Init();
	virtual BOOL ProcessInput() ;
	virtual BOOL Show(cCanvas* pCanvas) ;
	virtual BOOL CheckL() ;
	BOOL CheckRB();
	void SetMode(
		BOOL mode,
		sFriendList* pFriend = 0
		);
	void OnOff(BOOL NeedShow);
	void Move(int x, int y);
	cText m_XianJin;
	cText m_CunYing;
	cInterfaceNumShuRu m_ShuRu;
	cTaskFather* m_pTrigger;
	int m_TriggerID;
private:
	sFriendList* m_pFriend;
	BOOL m_bInit=FALSE;
	BOOL m_bMode;
	CWFile m_Back;

	cWfileP m_Button;
	cText  m_tButton;
};

class cInterfaceJianDing :public cInterfaceFather
{
public:
	void SetMode(int id){ m_Mode = id; }
	void Init();
	virtual BOOL ProcessInput() ;
	virtual BOOL Show(cCanvas* pCanvas) ;
	virtual BOOL CheckL() ;
	virtual void OnOff(BOOL NeedShow) ;
	void Move(int xPos, int yPos);
private:
	int m_ItemSelect;
	int m_NowItemSelect;
	BOOL m_Mode=0;//0��ʾ���� 1��ʾ�չ� 2�չ�һ�� 3��ʾ��Ƕ��ʯ
	BOOL m_bInit=FALSE;
	CWFile m_Back;
	cText m_tPrice;
};




class  cInterSelectFriendItem : public cInterfaceFather
{
public:
	enum eQiangHuaMode
	{
		eQHװ��,
		eQH����,
		eQH����2,
		eQH����װ��,
		eQH��ս,
		eQH����
	};

	RECT m_Rect;
	CWFile m_MouseOnMark;
	
	virtual BOOL ProcessInput();
	virtual BOOL CheckL();
	BOOL CheckRB();
	BOOL Init();
	BOOL Set(int itemtype, int itemid,DWORD headid);
	BOOL Move(int x, int y);
	BOOL Show(cCanvas* pCanvas);
	void SetMode(sFriendList* list, eQiangHuaMode mode);
	int m_ItemType[20];
	int m_ItemID[20];
	CWFile m_ItemHead[20];
	void OnOff(BOOL bshow);

	void GetSelect(int& outtype, int& outid)
	{
		outid = m_ItemID[m_Select];
		outtype = m_ItemType[m_Select];
	}
private:
	sShangPinPet* m_Pet;
	cText m_TitleT;
	sFriendList* m_pdata;
	eQiangHuaMode m_Mode;
	BOOL m_bInit = FALSE;
	int m_ItemNum=0;
	CWFile m_Back;

};
class cInterfaceUseJiaJu:public cInterfaceFather
{
public:
	cInterfaceUseJiaJu(){ 
		for (int i = 0; i < 6;i++)
		m_Pic[i].m_Picture.m_bShadow = 50; }
	virtual BOOL ProcessInput();
	virtual BOOL Show(cCanvas* pCanvas);
	virtual BOOL CheckL();
	BOOL CheckRB();
	virtual void OnOff(BOOL NeedShow);
	void SetJiaJu(sItem2* jiaju);
	void AddDirection();
private:
	sItem2* m_pJiaJu;
	CWFile m_Pic[6];
	sJiaJu m_JiaJu;
	

};
class cInterfaceDaZao:public cInterfaceFather
{
public:
	void Init();
	virtual BOOL ProcessInput();
	virtual BOOL Show(cCanvas* pCanvas);
	virtual BOOL CheckL();
	BOOL CheckRB();
	virtual void OnOff(BOOL NeedShow);
	void Move(int x, int y);
private:
	BOOL CheckEquip(int type);
	BOOL m_bInit = FALSE;
	CWFile m_Back;
	cText m_tMoneyNeed;
	cText m_tMoneyHave;
	cWfileP m_Head[2];
	int m_Item[2];
	cWfileP m_Button;
	cText m_tButton;
	int m_MoneyNeed;
};
