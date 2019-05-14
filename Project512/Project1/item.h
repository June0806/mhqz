#pragma once
#include "global.h"
using namespace std;
class CMainState;
struct sWeaponData2:public sPetDataPart //һ��������һ��״̬
{
	int     Master;
	int     MasterPos; //������������Ľ�ɫ��ID
	void Read(ifstream& File)
	{
		File >> m_attack;
		m_attack2 = m_attack;
		File >> m_defend;
		File >> m_dead;
		File >> m_fstand;
		File >> m_majic;
		File >> m_return;
		File >> m_run;
		File >> m_stand;
		File >> m_suffer;
		File >> m_walk;
		File >> Master;
		File >> MasterPos;
	}
};

enum eFaBao
{
	eFaBao0������,
	eFaBao1�ɽ�Īа,
	eFaBao2�޺���,
	eFaBao3����,
	eFaBao4����,
	eFaBao5������,
	eFaBao6��Ԫɡ,
	eFaBao7�޻����,
	eFaBao8Ǭ��������,
	eFaBao9�ն�,
	eFaBao10���س�,
	eFaBao11�¹ⱦ��,
	eFaBao12��ɫ������,
	eFaBao13��ɫ������,
	eFaBao14��ɫ������,
	eFaBao15��ɫ������,
	eFaBao16����,
	eFaBao17�ܽ����ְ�,
	eFaBao18ʱ֮��,
	eFaBao19ͨ�鱦��,
	eFaBao20�۱���,
	eFaBao21�����«,
	eFaBao22��ľ����,
	eFaBao23���뾵,
	eFaBao24��Ѫ�,
	eFaBao25�������,
	eFaBao26��ħ����,
	eFaBao27����,
	eFaBao28������ë,
	eFaBao29����,
	eFaBao30�������ӡ,
	eFaBao31����,
	eFaBao32�ȱ�,
	eFaBao33�ε�
};
struct sItem //��ʾһ������
{
	int m_Type;
	int m_ID;
	string m_Name;
	DWORD m_SmallHead; //ͷ��
	DWORD m_BigHead;
	int m_Price=0;//�۸�
	
	string m_Text; //����
	int m_UseType;
	void SetLv(int lv){ m_lv = lv; }
	int GetLv(){ return m_lv; }
private:
	int m_lv;
};
enum eProPerty
{
	eProPerty�˺�,
	eProPerty����,
	eProPerty����,
	eProPertyħ��,
	eProPerty����,
	eProPerty�ٶ�,
	eProPerty��Ѫ,
	eProPerty����,
	eProPerty����,
	eProPerty����,
	eProPertyħ��,
	eProPerty����,
	eProPertyˮ��,
	eProPerty��,
	eProPerty����,
	eProPerty�׿�,
	eProPerty���
};

enum ItemProPerty
{
	enum_hp,
	enum_hphurt,
	enum_mp
};


class cZuoQi
{
public:
	BOOL GetZuoQi(int charID, int ZuoQiID);
	BOOL GetZuoQi(cPCData& pcData, int ZuoQiID);
	BOOL Init(string Path);
	sZuoQi m_ZuoQi[8];
	void GetShiPei(vector<int>& list,int pcData);
};
class cFightState;
class cDescribe;
struct sJiaJuData
{
	int num;
	vector<DWORD> m_data;
};
class cItem //��Ʒ������
{
public:
	void GetIDlistByLv(vector<int>& outlist,int type, int lv);
	int GetIDByLv(int type, int lv);//ͨ���ȼ�����õ���Ʒ��ID
	int GetlvByID(int type, int id);//ͨ��ID�õ���Ʒ�ȼ�
	BOOL m_bAddHp=0;
	BOOL m_bAddMp=0;
	BOOL m_bAddCp=0;
	BOOL m_bToAll=0;
	BOOL AddHp(int Num, int TargetID, BOOL Percent = FALSE);
	BOOL AddHpHurt(int Num, int TargetID, BOOL Percent = FALSE);
	BOOL AddHp(int Num, cPetData* pdata, BOOL Percent = FALSE);
	BOOL AddHpHurt(int Num, cPetData* pdata, BOOL Percent = FALSE);
	BOOL Detoxify(int Type, int TargetID,BOOL ReMoveAll);//��ĳ�־����״̬   ,���һ��������ʾ�Ƿ��Ƴ�����ͬ��״̬
	
	BOOL AddMp(int Num, int TargetID, BOOL Percent = FALSE);
	BOOL AddCp(int Num, int TargetID, BOOL Percent = FALSE);
	BOOL Resurgence(int Num,int TargetID,BOOL Percent=FALSE);//����

	BOOL AddAll(BOOL(cItem::*AddSomething)(int, int, BOOL), int Num, int TargetID, BOOL Percent = FALSE);
	int  m_hp;//������¼�ϴ�ʹ����Ʒ�ӵ�HP
	//leveup��ʾ�����ټ�
	void  SetFabaoEffect(cPCData& data, int leveup, eFaBao ID);

	sItem* GetItem(int type, int id){ return &m_ItemList[type][id]; }
	vector<sItem> m_ItemList[42]; 
	
	sZuoQiZhuangShi m_ZuoQiZhuangShiList[24];
	//sEquipData m_EquipList[7];  //װ���б�
	BOOL Init();//�����б�

	vector<sJiaJuData> m_JiaJuData[4];

	//���޾����з���,���㽱��
	vector<int> m_LShouJueList;
	vector<int> m_HShouJueList;
	//
	BOOL UseItem22(sItem2* pItem,int ObjId);
	BOOL UseItem23(sItem2* pItem, int ObjId);
	BOOL UseItem24(sItem2* pItem, int ObjId);
	BOOL UseItem26(sItem2* pCard, int ObjId);
	BOOL UseItem27(sItem2* pItem, int ObjId, int* num=0);
	BOOL UseItem28(sItem2* pItem, int ObjId);
	BOOL UseJiaJu(sItem2* pItem);
	BOOL UseItem(sItem2* pItem, int ObjId,int* num=0);
	
	

	BOOL FillProperty(int CharID, ItemProPerty type);

	int  GetCheapestItem(int CharID, ItemProPerty Type);//Ѱ������˵���Ʒ
	CWFile m_MagicAddHP;
	CWFile m_MagicAddMP;
	CWFile m_MagicAddHPMP;
	vector<string> m_ItemTypeName;

	vector<sWeaponData2> m_pWeaponList2[15][11];//15������
	sWeaponData2* GetWeaponList(int charid, int weaponid, int type);
	BOOL SetItemDescribe(int x, int y,cDescribe& pDescribe, sItem2* pItem);
	void AutoItemProperty(sItem2* item, cPCData* pc);
	void AutoFuShi(sItem2* item, int lv, eMengPai mengpaiid);
	void AutoItemProperty(sItem2* item,cCharacter* pc=0); //�Զ�����Ʒ�������, ��������������
	void AutoItemProperty(sItem2* item, int Luck, int dustbin);
	void AutoItemProperty2(vector<int>& prop,  int num, int Luck, int Offset);
	BOOL XiangQian(sItem2& equip, sItem2& gem);
	void SetUpQiangHua(vector<int>& prop,int protype, int num);
	void SetUpBaoShi(vector<int>& pro, int baoshiid, int m_bSetup=1);
	sItem2* m_pNowItem = 0;
private:
	
	string m_text[10];
	int m_numtext = 0;
	DWORD m_Color[10];

	string MakePropString(vector<int>& prop, int pos);
	void  MakePropString2(sItem2* item);
	BOOL m_bInit = FALSE;
	BOOL Detoxify1(int Type, int TargetID);
	BOOL LoadItem1(string path);  //  
	BOOL LoadFaBao(string path);
	BOOL LoadShouJue(string path);
	BOOL LoadWeapon(string path);
	BOOL LoadZuoqiZhuangShi(string path);
	BOOL LoadJiaJu(string path);
	void LoadJiaJuCube(ifstream& File,int type);
};
class cJiangLi:public cTaskFather
{
public:
	BOOL Process(int stage){ return TRUE; }
	void Init(){}
	void AutoGetJiangLi();
	void AddLShouJue(int rate){ Add(0, 40, rate, 5); }
	void AddHShouJue(int rate){ Add(0, 40, rate, 10); }
	void Reset(){ m_List.clear(); }
	void Add(int id, int type, int rate,int ran=1);//���ӽ�����
	void SetBaoShi(int rate){ Add(0, 29, rate, 17); }
	void SetQiangHua(int rate);
	int m_MoenyRate;
	int m_ExpRate;
	int m_Moneytime;
	int m_Exptime;
	void SetMoeny(int rate, int time){ m_MoenyRate = rate; m_Moneytime = time; }
	void SetExp(int rate, int time){ m_ExpRate = rate; m_Exptime = time; }
	struct sJiangLiList
	{
		int m_Type;
		int m_Id;
		int m_Rate;
		int m_rand = 1;
	};
	vector<sJiangLiList> m_List;

};