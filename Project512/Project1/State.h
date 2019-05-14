#pragma once
#define MAX_OBJ_NUM		70


class CGameState
{
public:
	CGameState(){ isInit = 0; };
	virtual ~CGameState(){};
	virtual BOOL Init(void* pData=0) = 0;
	virtual BOOL Frame(void* pData=0) = 0;
	virtual BOOL Free() = 0;
	BOOL isInit;
};

struct sFightList;

struct sFightList
{
	
	int m_FriendListid = -1;//������¼�ǲ�����ҵĺ���,�Ӷ����º���״̬
	BOOL Swap(sFightList& fightlist);

	//���ڼ�¼��ɫս��ǰ��λ��
	int  xOldPos;
	int  yOldPos;
	int    m_oldDirection;
	int m_WorkType;
	BOOL Free();
	//�ٻ�������ʱ����Ч��
	BOOL InitFly();
	BOOL m_bFlyInit;
	int  m_DiePoint;
	BOOL DieFly();

	int  m_rotateRate;
	BOOL Rotate(int rotaterate = 3);
	//��һ����ɫ�ȴ�  ,����˵,ת������Ŀ��ʱҪͣ����,���������ܹ�
	int m_SleepTime;
	BOOL Sleep(int Time, BOOL bStatic = TRUE);
	//�ܵ����˺�
	cWfileP m_Damage[5]; //��λ,Ҳ�����������ʾ99999���˺�
	int   m_DamageTime[5];
	int m_DamageNum;   //�˺��м�λ
	BOOL ShowDamage(CMainState* p);
	BOOL SetDamageRect(int Num); //����ÿ���ֵ�λ�� ���������ֱ��� �ڼ����ֺ�������
	//
	BOOL m_bSufferFullSkill;//�ý�ɫ�Ƿ�ȫ�巨����

	sFightSkillSelect m_SkillSelect;
	sFightSkillSelect m_SkillPreSelect;
	int               m_ItemSelectID;
	int               m_autoItemSelectID;
	sFightOffset* GetFightOffset();
	int           m_SkillIDEx;//����ʹ�÷���ʱ��ʹ��

	cObj* m_pData=0;

	BOOL NextTarget(cFightState* pFightState,int skillid);
	int  targetID = -1;
	int  m_NowTargetID;
	int  m_NowBeAtkowner = -1;
	vector<int>  targetlist;
	int  m_MaxTargetNum; //��๥��Ŀ��
	int  m_NowTarget;//��ǰĿ����Ŀ���б��е�λ��
	struct sWork
	{
		int type=-1;
		int stage=-1;
	};
	int GetNowWorkType(){ if (!m_work.size())return -1;  return m_work[m_work.size() - 1].type; }
	int GetNowWorkStage(){ if (!m_work.size())return -1; return m_work[m_work.size() - 1].stage; }
	void SetWork(int type);
	void SetWorkFront(int type);
	vector<sWork>  m_work;  //��ɫ�Ĺ���
	sWork* GetWork(){return &m_work[m_work.size() - 1];}

	BOOL   isLive=FALSE;  //��ɫ�Ƿ���� ,�����ɫ�ڱ����ɵ�ʱ��,�Ѿ�����,����������
	BOOL m_bPrePareFight=FALSE;
	
	int    m_sufferDirection=1;
	
	BOOL SetMagiSufferID(DWORD MagicID, int MagicPos, CWFile* pFile, BOOL up = 1);
	BOOL ShowOnScreen(cCanvas* pCanvas);
	sFightList();
	~sFightList();
	void RestTarget();
	void operator=(sFightList& flist);
	void SetDie(BOOL bDie){ m_bDie = bDie; }
	BOOL CheckDie(){ return m_bDie; }
private:
	BOOL   m_bDie = FALSE;  //��ɫ�Ƿ����
};



class cFightState :public CGameState
{
private:
	void FriendCure();
	void EndFight(BOOL bFriendCure=TRUE);
public:
	cFightState();
	void EscapeFight();
	void SuccessFight();
	void FalseFight();
	void DieEffect(int type);//�ɵ��ظ�,����ʧһ������Ǯ



	void ClearState(int id);//ȥ������BUFF
	BOOL AddState(int userID, sBuff buf);//����һ��BUFF
	BOOL ReMoveStateID(int id, int userID);//ȥ��һ��IDΪuserID�� BUFF
	BOOL ReMoveSealState(int userID);//���
	BOOL ReMovePoisionState(int userID);//�ⶾ
	BOOL ReMoveBuffState(int userID);//������BUFF
	BOOL ReMoveDeBuffState(int userID);//�⸺��BUFF
	BOOL ReMoveState(eBuffType Type, int userID);
	
	void SetFightBufDown(int i);
	BOOL m_bEscape=FALSE;//���ܱ�־
	void SetDie(int ID);//���һ�������Ƿ�����,����������(�ɵ�,����)
	HDC     m_hFightBack;
	HBITMAP m_hFightBack1;
	CWFile  m_FightBack1;
	BYTE*     m_pFightBack=0;
	int m_turn;  //�ȴ���������,ս��
	int m_nowturnid;//��ǰ���ж���ID
	BOOL SortBySpeed();

	
	//����̸߳�����ط���ͼƬ,����������ʾ�Ͳ��Ῠ��0_0!
	CWFile* m_pFileToLoad[5];
	DWORD   m_IDtoLoad[5];

	BOOL   m_ThreadSleep=TRUE;
	HANDLE m_hThread;
	
	int m_HiddenWeaponsUseNum=0;
	CWFile m_HiddenWeapons;
	cWfileP m_HiddenWeaponsp[10];
	int   m_HiddenWeaponsTargetID[10];
	BOOL HiddenWeaponsFly();
	BOOL HiddenWeaponsMove(int id,int yoffset=0);
	int   m_ShowTurnType[30];
	void*  m_pShowTurn[30];
	int   m_ShowTurnUseNum;

	static unsigned int WINAPI ThreadProc(
		_In_  LPVOID lpParameter
		);

	cFullMagic m_FullSkill;

	cInterfaceFight m_FightMenu;
	cText m_Frame;  //������
	BOOL Suffer(int ID);
	float m_Sufferspd = 4;
	void Sort();
	
	BOOL MagicSuffer(int ID);
	BOOL SetWork(int ID,int Type);
	BOOL SetMagicSuffer(int ID, int direction);//��һ���������óɱ�������״̬
	BOOL SwitchMagicSuffer(int ID);  //��һ����λ���ܷ���״̬ת�ɱ���״̬
	BOOL SwitchMagicSufferAvoid(int ID);  //��һ����λ���ܷ���״̬ת�ɱ��ر�״̬
	BOOL SetSuffer(int ID, int direction,int userid);//��һ���������óɱ���״̬
	

	BOOL SetAvoid(int ID, int direction);
	void SetTarget(int ID,int Target);
	int CheckTarget(int ID, int target, int skillid);//���Ŀ���Ƿ���Թ���
	BOOL DoWork();
	BOOL isInRect(long x, long y, CWFile* pPic, cMyMap* pMap);
	BOOL Init(void* pData=0);
	BOOL Frame(void* pData=0);

	BOOL GetSelect(long x, long y);
	BOOL Free();
	BOOL ProcessInput();
	int  AutoSelect(BOOL Enemy);//���ѡ��һ������,���EnemyΪ1 ��ѡ��з�,���Ϊ0,��ѡ���ҷ�.  ����޷�ѡ������,�򷵻�-1(�����ܳ������������?)
	BOOL FallDown(int ID);//��ɫ����
//	BOOL Catch(int ID);
	BOOL Avoid(int ID);
	BOOL Guard(int ID, int xTarget, int yTarget);
	BOOL Counter(int ID);
	void SetCounter(int ID,int TargetID);
	BOOL SetDefend(int ID, int direction,int userid);
	BOOL Defend(int ID);
	BOOL Approach(int ID,int TargetID,BOOL gap=FALSE);  //����ƶ����˷�Χ��,����TRUE 
	BOOL ReturnApproach(int ID, float Speed = 20.0f);  //����ƶ����˷�Χ��,����TRUE
	int WorkAttack(int userID);//���Ŀ�겻�ڹ�����Χ��,���ȵ���Approach�ӽ� ����һ���������Ŀ�ʼ�������ж�,ʹ����˺���,��Ҫ��WaitPosEnd�ȴ���������
	BOOL WaitPosEnd(int userID);//�˺��������ǵȴ�������ɶ���.
	vector<sFightList*>  m_pList;
	int   m_SpeedListPos[20];
	int m_xPosList[20];
	int m_yPosList[20];
	cText	m_MiaoShu;
	void CheckPetExp();
	BOOL PrePareToFight(int i);
	BOOL BackFromFight(int i,BOOL bdelete=TRUE);
private:
	

};
const int c_InterfaceShowNum = 3;  //���ͬʱ��ʾ��������,���˿�������........
const int c_GlobalButtonNum = 256;
class cAI;
class cEndState;
class cMenuState;

class CMainState :public CGameState
{
public:
	void SetRanse(int i, int d0, int d1=0, int d2=0, int d3=0,  int part = 0);
	void SetJiangLiItem(sItem2& item, BOOL m_bNeedTag = FALSE);
	void SetJiangLi(INT64 exp, INT64 expbb, INT64 money, INT64 chubeimoney = 0, BOOL m_bNeedTag = FALSE);

	void FollowLv(int id,int lv=-1);
	void FreeFightList(int i);
	
	cTaskHuoDong m_TaskHuoDong;
	cInterfaceUseJiaJu m_UseJiaJu;
	cFangWu m_FangWu;
	void ResetAll();
	cEndState* m_pEndState;
	cMenuState* m_pMenuState;
	
	int  m_FightType;
	BOOL* m_MaskList;
	cInterfaceChaKan m_ChaKan;
	cTaskZhuXian m_ZhuXian;
	cTaskZhiXian m_ZhiXian;
	cTaskZhiXian2 m_ZhiXian2;
	cTaskZhiXian3 m_ZhiXian3;
	cTaskFB  m_TaskFB;
	void SetCharacter(int ID);
	int AddPCEnemy(
		sMapObjTrigger* pdata,
		int lv,
		sAddPoint* addpoint,
		sZiZhi* zizhi,
		eMengPai mengpaid,
		int stronglv = -1,
		int pos = -1,
		BOOL our = FALSE);

	int AddPCEnemy(
		string name,
		int modeid,
		int lv,
		sAddPoint* addpoint,
		sZiZhi* zizhi,
		eMengPai mengpaid ,
		sItem2* pWeapon = 0,
		int petid = -1,
		int stronglv = -1,
		int pos =-1,
		BOOL our = FALSE);
	int AddPCPetEnemy(
		ePetID modeid,
		int lv=-1,
		sAddPoint* addpoint=0,
		eMengPai mengpaid = eMP_���,
		string name = "",
		int stronglv = -1,
		BOOL bOut = FALSE,
		int pos = -1);
	int AddPCPet(
		ePetID modeid,
		int lv,
		sAddPoint* addpoint,
		eMengPai mengpaid,
		string name = "",
		int stronglv = -1,
		BOOL bOur = FALSE,
		int pos = -1);
	int AddPetEnemy(
		string name,
		int modeid,
		int lv,
		sAddPoint* addpoint,
		int stronglv=-1,
		int Pos = -1,
		BOOL out = FALSE);
	int AddPetEnemy2(
		int modeid,
		int lv=-1,
		sAddPoint* addpoint=0,
		int stronglv=-1,
		int Pos = -1,
		BOOL our = FALSE);
	int AddPC(
		string name,
		int modeid,
		int lv,
		sAddPoint* addpoint,
		sZiZhi* zizhi,
		eMengPai mengpaid,
		sItem2* pWeapon = 0,
		int petid = -1,
		int stronglv = -1,
		int pos = -1
		);

	int AddPet(
		string name,
		int modeid,
		int lv,
		sAddPoint* addpoint,
		int stronglv=-1,
		int Pos = -1);
	cInterfaceJianDing m_InterfaceJianDing;
	cInterfaceGive m_InterfaceGive;
	cInterfaceYingHang m_YingHang;
	cInterfaceXiuLian m_LearnXiuLian;
	cBangPaiJiNengXueXi m_LearnBangPaiSkill;
	cInterfaceLearnSkill m_LearnSkill;
	cTaskSM m_taskSM;
	int GetAverageLv();//�������ƽ���ȼ�
	cInterfaceRanSe m_InterfaceRanSe;
	cChangeColor m_ColorManage;
	cItemSelect m_ItemSelect;
	cTriggerGive m_TriggerGive;
	//cTriggerTime m_TriggerTime;
	cTrigger m_TriggerFightSuccess;
	cTrigger m_TriggerFightFalse;
	cTrigger m_TriggerFightEscape;
	cDialogTrigger m_TriggerDialog;
	cTrigger m_TriggerFightStart;
	cMapObjTrigger m_TriggerObjMap;
	cObjTrigger m_TriggerObj;
	cLvTrigger m_Triggerlv;
	cTaskNormal m_TaskNormal;
	cTaskBaotu m_TaskBaoTu;
	cTaskZhuaGui m_TaskZhuaGui;
	cTaskBangPai m_TaskBangPai;
	cTrigger  m_TriggerFightTurn;//����ս��
	//cText m_sFPS;
	string m_MusicName;
	cInterSelectFriendItem m_SelectFriendItem;
	cMySound  m_Mp3;
	//
	cWorldMap m_WorldMap;

	
	cTags m_Tags;
	cInterfaceTeam m_InterfaceTeam;
	
	int m_HeroID;
	cCanvas m_Canvas;
	cZuoQi m_ZuoQi;
	int  AtutoGetEnemyNum();
	int GetPCFightNum(){ return GetLiveNum(0, 5); }

	//int  GetEnemyNum(int startid=10,int endid=20);
	int GetLiveNum(int start, int end);

	BOOL SwapFightOffset(int Original, int Target);
	BOOL ReadyToFight();
	int ReadTaskFightIndex(string name);
	cInterfaceChannel m_Channel;
	
	void LockLv(int b = 0);//�趨�ȼ�����
	int GetLockLv(){ return m_GlobalButton[2]; }
	vector<int> m_GlobalButton;
	void SaveGlobal(ofstream& File);
	void LoadGlobal(ifstream& File);
	 RECT m_FullRect;
	 HBRUSH  m_blackbrush;
	
	BOOL SetEnemy(int EnemyType, int EnemyID, int lv, int AI);
	//��ָ����ɫ�õ���Ʒ  ������ǿɵ�����Ʒ���һ��������Ϊ�ȼ�ʹ��
	BOOL Give(int CharID, sItem2* pItem);
	CWFile m_PathPoint;
	cItem m_ItemManage;
	
	cInterfaceSmallMap m_InterfaceSmallMap; //������ʾС��ͼ
	//���ļ���ȡ
	BOOL LoadCharacterData2(int charactID, string Path);
	BOOL LoadCharacterData2(cObj*& data, string Path);
	cObj*  m_pNowSelect;

	BOOL  FindAttackPoint(cObj* data,char* name);
	//Ѱ�ҽ�ɫ�б�
	BOOL  FindCharacter(cObj*& data, int CharacterID);
	BOOL SetFightPet(int ID,int PetID,BOOL blimit=TRUE);

	//����һ���ٻ���(ע��,���ص���Դ�ڶ���,�������������������ɾ��)
	BOOL MakeAPet(cPet*& pPet, int ID, int StrongLv, int LV,sAddPoint addpoint);
	//BOOL SetUpOffset(sFightList* p);
	BOOL  GivePcAPet(int charID, cPet* pPet,BOOL setfight=TRUE);
	int CheckInTeam(string name);

	BOOL  FindNPCData(cNpc*& data, int NpcID);
	BOOL  FindData(cObj* data, int Id);
	//Ѱ���ٻ����б�
	BOOL  FindPetData(cPet* pPet, int PetID);
	cPetManager m_PetDataManager;
	cSkillManager m_SkillManager;
	//ս����ɫ��
	vector<sFightList> m_FightList;

	CWFile m_Shadow;
	CWFile m_FNumRed;
	CWFile m_FNumGreen;
	CWFile m_FaceTextBack;
	vector<cWfileP> m_FaceTextBackp;
	cAI     m_AI;
	cFriend m_Friend;
	cInterfaceDoor m_InterfaceDoor;
	cInterfaceItem m_InterfaceItem;
	cInterfaceFB   m_InterfaceFB;
	cInterfacePet  m_InterfacePet;
	cInterfaceJiNeng m_InterfaceJiNeng;
	cInterfaceJob  m_Job;
	cInterfaceSystem m_InterfaceSystem;

	cMenu   m_Menu;
	cState  m_State;
	cCursor m_Cursor;
	vector<HFONT> m_hFont;
	HFONT m_hOldFont;
	HFONT m_hOldFont2;
	cPCHead m_PCHead;

	cInterfaceDialog m_Dialog;
	cInterFaceBuy   m_InterFaceBuy;
	cInterFaceBuyPet   m_InterFaceBuyPet;
	cInterfaceDaZao m_Dazao;
	BOOL MoveObj();
	cFindPath m_FindPt;
	cObj* GetPointAt(long x, long y);
	cScript m_ScriptTemplate;

	cObj* FindObjByName(string Name);
	cObj* FindObjByNpc(sNpc* pNpc);
	cAccelator m_Accelator;


	BOOL ProcessInput();
	BOOL ProcessInterface();

	cInterfaceFather* m_Interface[c_InterfaceShowNum];
	BOOL InterfaceSetShow(cInterfaceFather* p,BOOL NeedShow);
	void ClearInterfaceSetShow();
	cInput    Input;
	cInputDevice Mouse;
	cInputDevice Keyboard;
	BOOL FindObj();
	BOOL Sort();

	//�ö��Ѹ�������
	BOOL TeamFollow();

	inline void UpDateInput();
	CMainState();
	cObj*  m_pObj[MAX_OBJ_NUM];
	int m_ObjNumber;
	~CMainState();
	BOOL SetWork(int ID,int Target,int Pos);
	BOOL Init(void* pData=0);
	void Init2();
	void Init3();
	
	HFONT EasyCreateFont(int h, wstring name, int weight=400);
	cTaskFather* FindTaskByID(int ID);
	int FindPetWeapon(string name);
	BOOL Frame(void* pData=0);
	BOOL ShowInterface();
	BOOL ShowInterface2();
	BOOL Free();
	cMyMap m_Map;
	

	vector<sNpcData> m_pNpcList;

	
	long m_EquipNumber;

	
	int  m_AttackPointNumber;
	void LoadList();
	BOOL LoadWeaponList(string Path);
	int  ReadInt(char*& point);
	BOOL  ReadName(char*& point, char*& pName);
	DWORD ReadDWORD(char*& point);

	vector<sCharacterData> m_pCharacterDataList;
	
	cFightState* m_pFightState=0;
	void SetMouseOnMap(BOOL lock);
	BOOL GetLockMouse(){ return m_isMouseOnMap; }
	BOOL m_bInit3 = FALSE;

	//ȫ�ֱ���
	CWFile m_Cancel;
	CWFile m_HpLine;
	CWFile m_HpEmptyLine;
	CWFile m_Button4;//���ְ�ť
	void ShowObj();
	struct sDialogList
	{
		string name;
		DWORD  idlist;
	};
	void SetDialogList(string name, DWORD id);
	void SetDialogListPet(string name, ePetID petid,BOOL bJinJie=FALSE);
	void SetDialogListNpc(string name, int npcid);
	void SetDialogListPC(string name, int pcid);
	void SetDialogList(cObj* pObj);
	void ClearDialogList(){ m_DialogList.clear(); }
	vector<sDialogList>  m_DialogList;//ͷ���б�,���ڶԻ�ʱû���ֵĽ�ɫ��ͷ��
private:
	BOOL m_bInit2=FALSE;
	BOOL m_LoadList = FALSE;
	BOOL m_isMouseOnMap;
	
	int m_NumShow=0;
	int   m_ShowTurnType[70];
	void*  m_pShowTurn[70];
	int m_PosYList[70];
};

class cMenuState:public CGameState
{
public:
	cWfileP* m_pPre=0;
	BOOL CheckL();
	virtual BOOL Init(void* pData = 0) ;
	virtual BOOL Frame(void* pData = 0);
	void Show(cCanvas* pCanvas);
	virtual BOOL Free(){ return TRUE; }
	vector<cWfileP> m_Button;
	vector<cText> m_tButton;
	static unsigned int WINAPI ThreadProc(
		_In_  LPVOID lpParameter
		);
private:
	int m_bMode=0;
	HANDLE m_hThread=0;
	int m_Sleeptime;
	int m_Select;
	int m_ID=0;
	CWFile m_Back;
	BOOL m_bInit = FALSE;
	void Move(int x, int y);
	int m_xPos;
	int m_yPos;
};
class cEndState :public CGameState
{
public:
	BOOL CheckL();
	virtual BOOL Init(void* pData = 0);
	virtual BOOL Frame(void* pData = 0);
	void Show(cCanvas* pCanvas);
	void Set(string title, string context);
	virtual BOOL Free(){ return TRUE; }
private:
	CWFile m_Back;
	BOOL m_bInit = FALSE;
	cText m_Title;
	cText m_Context;
};
