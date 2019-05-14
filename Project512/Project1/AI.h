#pragma once
enum eAIUseItemType
{
	eAIUseItemTypeHP,
	eAIUseItemTypeHPALL,
	eAIUseItemTypeMP,
	eAIUseItemTypeMPALL,
	eAIUseItemTypeHPHurt,
	eAIUseItemTypelive,
	eAIUseItemTypeUnseal,
	eAIUseItemTypeCP,
	eAIUseItemType暗器
};
class cAttackAI
{
public:
	cAttackAI();
	
	int m_ButtonSelect=-1;
	int m_ObjSelectid=-1;
	BOOL m_bPKMode = FALSE;//如果这个打开,难度将提升
	void Reset();
	void Run();
	void RunZhiHui(int start, int end, int mode, int select);
	void GetNeed(int start,int end);//需求,比如血不够了,要求加血
	void PreRun(int start, int end);//满足需求
	BOOL SaveOneself(int start, int end);//自救
	int FintTarget(int i);

	int FindAtkTarget(int rate, BOOL ATKOrMagic=TRUE);
	int FindCpMaxTarget(int num=100);//寻找敌方CP>num的对象
	int FindDefminTarget(int start, int end, eProPerty prop,BOOL maxormin=FALSE);
	int FindUnSeal(int num=10);
	int FindUnSeal(int start, int end);
	void AutoRunAi(int id);//,int targetid);
	void AutoRunEnemyAi(int id);
	BOOL CheckHaveState(int userid,int skillid);
	void NormalAtk();
	void Summon(int id);//召唤

	void UseMagic(int id);
	void NpcTryMagic(int id);//测试是否有足够的蓝用魔法,否则就物理攻击
	void Escape(int userID);//逃跑
	void Def(int userID);//防  
	BOOL UseItem(eAIUseItemType mode);
	BOOL CheckItem(eAIUseItemType mode);
	int m_HPLimit = 60;//1-99 HP低于 100-m_HPLimit时加血
	void ResetNeed();
	int m_bNeedAddHp[20];//当回合该对象是否需要加血
	BOOL m_bNeedAddHpHurt[20];
	BOOL m_bNeedUnSeal[20];//当回合该对象是否需要解封
	BOOL m_bNeedlive[20];
	int m_bNeedAddMp[20];
	BOOL m_bBeSeal[20];//是否被封
	BOOL  m_bSeal[20];//
	BOOL  m_bAtk[20];//
	//BOOL  m_bBeLive[20];//是否被复活
	//BOOL m_bCure[20];//被治疗
	//BOOL m_bCureMP[20];//被加MP
	////void CoutIdleList(int start,int end);
	
	void SetUser(int userid);
	int m_UserID = -1;
	sFightList* pUserList=0;
	cObj* m_pUser=0;
	cCharacter* m_pUserPc=0;
	cPetData* m_pUserPetData=0;

	void SetTarget(int target);
	int m_TargetID=-1;
	sFightList* m_pTargetList=0;
	cObj* m_pTarget=0;
	cPetData* m_pTargetPetData=0;
	int m_FightTarget[20];
	void ZhiHui(BOOL bOut);//
private:
	void ContentLive(int start,int end);
	void ContentLive2(int start,int end);
	void ContentHP(int start, int end);
	void ContentHP2(int start, int end);
	void ContentUnSeal(int start, int end);
	void ContentUnSeal2(int start, int end);
	BOOL CheckCanSkill(int skillid);
	void RunPet();
	void RunDT();
	void RunHS();
	void RunNR();
	void RunFC();
	void RunLG();
	void RunWZ();
	void RunPT();
	void RunTG();
	void RunDF();
	void RunPS();
	void RunMW();
	void RunST();
	void SaveOneself2(int id);
	BOOL resurgence(int user, int target);
	void resurgence2(vector<POINT>& user, vector<POINT>& target,int targetID);
	BOOL Cure(int user, int target,BOOL toall=FALSE);
	void Cure2(vector<POINT>& tlist, int start, int end);
	void UnSeal2(vector<POINT>& tlist, int start, int end);
	BOOL UnSeal(int user, int target);

	BOOL CureMP(int user, int target, BOOL toall = FALSE);
	void CureMP2(vector<POINT>& tlist, int start, int end);
public:
	enum eEnemyType
	{
		eET物理系,    //加了1力以上
		eET法系,	  //加了1蓝以上
		eET封系,	  //FC NR PS WZ TG
		eET需要蓝系, // 非DT
		eET医疗系, //HS  PT 血宠
		eET法宠,
		eET攻宠,
		eET血宠,
		eET血最少,
		eET中死亡对象,
		eET防最低,
		eET灵最低,
		eET被封物理,
		eET被封法术,
		eET被禁疗,
		eET无召唤兽,
		eET人物死亡,
		eET速度最快,
		eET加血,
		eET解封
	};
	int FindEnemy(BOOL CheckDie,BOOL bCheckWork, vector<POINT>& out, eEnemyType type, int userid, int data = -1, int data2 = -1);//寻找特定对象
	enum eEnemyCorrect
	{
		eEC道具
	};
	void CorrectEnemy(vector<POINT>& out,eEnemyCorrect typ);
};
class cAI
{
public:
	int  AutoFindTarget(int userID, int SelectType);
	int  GetNear(int ID, int Num,int bLive);
	int  GetNextPos(int ID, int Num, int bLive);
	//int  GetIdle(int ID);
	BOOL RunDT(int userID);
	BOOL RunHS(int userID);
	BOOL RunFC(int userID);
	BOOL RunNR(int userID);
	BOOL RunWZ(int userID);
	BOOL RunPT(int userID);
	BOOL RunLG(int userID);
	BOOL RunTG(int userID);
	BOOL RunMW(int userID);
	BOOL RunPS(int userID);
	BOOL RunST(int userID);
	BOOL RunDF(int userID);
	BOOL RunNULL(int userID);

	int  FindDieFight(int start = 0, int end = 5);
	
	
	cAttackAI m_AttackAI;

	

};


