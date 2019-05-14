#pragma  once;

class cTaskJuQing :public cTaskFather 
{
public:
	string SetUpPathJuQing(string str);
	void Save(ofstream& File);
	void Load(ifstream& File);
	void Init();
	virtual BOOL Process(int stage){ return TRUE; }

	void SetChapter(int chapter);
	int GetChapter(){ return m_Chapter; }



protected:
	//BOOL m_bInit2 = FALSE;
	string m_AutoPath;
	vector<string> m_Textlist;
	int m_Chapter=-1;
	string m_DialogPath;
	sAddPoint m_AddPoint;
	int AddPetEnemy(ePetID id, int lv, int name = -1, BOOL bOur = FALSE, int pos = -1);
	int AddPCPetEnemy(ePetID id, int lv, eMengPai mp=eMP_���, int name = -1, BOOL bOur = FALSE, int pos = -1);
	int AddPCEnemy(int id, sZiZhi* zz, int lv, eMengPai mp, int name = -1, int weapontype = -1, int weaponid = -1, BOOL bOur = FALSE, int pos = -1);

	cPetData* AddPetEnemy2(int hp, ePetID id, int lv, int name = -1, BOOL bOur = FALSE, int pos = -1);
	cPetData* AddPCPetEnemy2(int hp, ePetID id, int lv, eMengPai mp=eMP_���, int name = -1, BOOL bOur = FALSE,int pos=-1);
	cPetData* AddPCEnemy2(int hp, int id, sZiZhi* zz, int lv, eMengPai mp, int name = -1, int weapontype = -1, int weaponid = -1, BOOL bOur = FALSE, int pos = -1);
	sItem2* FindItem(int Type, int id);
	
	void AddTag(int str,int time=500);
	void LockMap(int name=-1);
	void TalkList(int textid, int nextname, int nextmap, int jobtext, int ret, int dialogret = -1);
	void SetDialogList(int id);
	void AddRangeMap(int mapname, int x, int y, int range, int ret);
	void UpdateJob2(string text);
	void UpdateJob2(int id);
	void AddChannel2(int id);
	void Invite(string name,int fighterid=-1);
	void UnInvite(string name);
	void AddFriend(int i);
	void AddTriggerDialog2(int id, int ret = -1);
	void AddTriggerObj2(int name, int mapname, int ret, BOOL bHuChi = TRUE);
	void RemoveNpc(int id, BOOL needfind = FALSE);
	void FollowLv(int id, int lv);
	void Select(int title, int name, int cancelret, int select0, int ret0, int select1, int ret1=-1, int select2 = -1, int ret2 = -1, int select3 = -1, int ret3 = -1);
	cPetData* GetPetData(int id);
	int GetCharacterID(string name);
	void AddTriggerObjMap2(int name, int mapname, int modeid, int x, int y, int direction = -1, int ret = -1, int jinjie = 0);
};

class cTaskZhiXian :public cTaskJuQing
{
public:
	void GetBaoXiangID(int type,int& id, int& index);
	cTaskZhiXian(){ m_type = 2008; m_AutoPath = "֧��"; }
	virtual BOOL Process(int stage);
	BOOL Process0(int stage);
	BOOL Process1(int stage);//���ֶ԰�
	BOOL Process2(int stage);
	BOOL Process3(int stage);//���ֱ���
	BOOL Process4(int stage);
	BOOL Process6(int stage);//֧��2
	//1015 ����/Ů����
	void ProcessSelect(int stage);
private:
	cObj* m_pObj=0;
	/*
	


	BOOL Process5(int stage);
	
	BOOL Process7(int stage);
	BOOL Process8(int stage);
	BOOL Process9(int stage);
	BOOL Process10(int stage);*/
};
class cTaskZhiXian2 :public cTaskJuQing
{
public:
	cTaskZhiXian2(){ m_type = 2009; m_AutoPath = "����2"; }
	virtual BOOL Process(int stage);
};
class cTaskZhiXian3 :public cTaskJuQing
{
public:
	cTaskZhiXian3(){ 
		m_type = 2010; 
		m_AutoPath = "����3"; 
		for (int i = 0; i < 10;i++)
			m_pFighter[i]=0;
	}
	~cTaskZhiXian3();
	void Init2();
	virtual BOOL Process(int stage);
	BOOL LoadTeam(int id, string path,BOOL bEnemy);
	BOOL m_bPK=FALSE;
private:
	
	BOOL m_bInit2;
	cObj* m_pFighter[10];
	cObj* m_PreFighter[10];
	BOOL m_islive[10];
	eFormation m_eOurFormation;
	int m_prehard=100;
};
class cTaskZhuXian :public cTaskJuQing
{
public:
	cTaskZhuXian(){ m_type = 2007; m_AutoPath = "����"; }
	virtual BOOL Process(int stage);
	BOOL Process0(int stage);//���˵Ĺ��
	BOOL Process1(int stage);//����
	BOOL Process2(int stage);//������
	BOOL Process3(int stage);//С����
	BOOL Process4(int stage);//�˽�
	BOOL Process5(int stage);//������
	BOOL Process6(int stage);//����׹Ǿ�
	BOOL Process7(int stage);//��ȡ�Ž���
	BOOL Process8(int stage);//����Ů��
	BOOL Process9(int stage);//��������

private:
	cObj* m_pObj=0;
};
