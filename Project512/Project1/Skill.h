#pragma once
//����ż�����ش���
#include "global.h"
struct sBuff;
class cFightState;


//����һ������
struct sSkill
{
	string m_Name;
	int    m_Id=0;
	string m_Text;//����
	string m_tCost;//���ĵ�����
	int    m_Cost=0;
	DWORD  m_BigHead=0;
	DWORD  m_SmallHead=0;
	int   m_SkillType=0;
	int   m_EffectType=0;
	int   m_Owner=-1; //λ��
	DWORD  m_Sound=0;
	int m_NumUnCompatible=0;
	vector<int> m_UnCompatible;
	DWORD  m_MagicID=0;  //��������
	int  m_MagicPos=0; //������λ��
	int  m_TargetNum=0;//Ŀ������� �������������7 �Ǹ���������7
	int  m_TargetNumOffset=0;//���ټ�Ŀ����+1   �����������25����һ��Ŀ��
};

//struct sPetSkill
//{
//	int m_ID;
//	int m_SkillType;
//	DWORD m_BigHead;
//	DWORD m_SmallHead;
//	string m_Describe;
//	string m_Cost;
//	string m_Name;
//	int m_NumUnCompatible;
//	vector<int> m_UnCompatible;
//};


//����һ��������
class cZhuJiNeng
{
public:
	int m_ID; //������ID
	string m_Name;
	DWORD m_SmallHead;
	DWORD m_BigHead;
	string m_Text;//����  ����ʲô����
	string m_Text2;//����2   ѧϰЧ��
	//��������
	int   m_NumSkillInclude;
	int   m_Limit[12];
	int   m_SkillIncludeID[12];

};
//����һ������
class cMengPai
{
public:
	int m_ID;
	string m_Name;			//���ɵ�����
	string m_ShiFuName;    //����ʦ��������
	string m_ShiFuMap; //����ʦ�����ڵ�ͼ
	string m_XunLuoMapName;//ʦ��Ѳ�ߵĵص�
	cZhuJiNeng m_MainSkill[7];//������
};




const int c_SkillNum = 167;
const int c_NumMengPai = 12;


class cMengPaiManager
{
public:
	BOOL  Init();
	cMengPai* GetMengPai(cCharacter* pc);
	cMengPai* GetMengPai(int ID){ if (ID < 0||ID>14)return 0;  return &m_MengPai[ID]; }
	sSkill*   GetSMSkill(int ID){ return &m_SkillList[ID];}
	cMengPai  m_MengPai[c_NumMengPai];//���ɱ�
	sSkill m_SkillList[c_SkillNum];//ʦ�ż��ܱ�
	void lvUpMainSkill(cPCData& pcDate, int lv, int ID);
private:
	BOOL m_bInit;
};
const int c_NumPetSkill = 103;        //�ٻ��޼��ܵ�����

struct sFightList;
class cSkillManager
{
private:
	
	BOOL SetPetSkill1(cPetData* pPetDate, int ID, int lv = 1);
public:
	void SetMengPaiSkill(cDescribe& describe, int id);

	BOOL CheckGodLivePercent(int& dam, int target,int userid);//��������
	void lvUpFuZhuSkill(int targetID, int FuZhuSkillID, int lv);
	void lvUpMainSkill(cPCData& pcDate, int lv, int ID);

	BOOL SetMainSkill(int targetID,int MainSkillID);

	void SetSpecialSkill2(int targetID, int SkillID,  int Setup);//��װ��Ч
	void SetSpecialSkill2(cPetData* pPetdata, int SkillID, int Setup);//��װ��Ч
	int  SetPetSkill(cPetData* pTarget,int PetSkillID,int Pos=12);
	BOOL ReMoveMainSkill(int targetID, int MainSkillID);
	BOOL CheckHit(cPetData* pUserData, cPetData *pTargetData, int offset = 0);//����������Ƿ�����
	BOOL CheckCRI(cPetData* pUserData, cPetData *pTargetData, int offset = 0);//���



	BOOL SetDateBuffUp(int userID, int targetID, int stateid, sBuff& buf, int skilllv = 0);  // ��Ŀ���һ��BUFF
	BOOL SetDateBuffDown(list<sBuff>::iterator& sbuf);//int targetID, int stateid); //��Ŀ��ȥ��һ��BUFF  ������Щ�ܸı��ɫ���Ե�BUFF
	BOOL CheckFightState(int FightListID);  //ʹBUFF����Ч�� ,  �����ն�����,ÿ�غϿ�ʼҪ��Ѫ
	BOOL CheckFightStateOnLine(int FightListID,int& dam);//����ɫ�ܵ�����ʱ,BUFF��Ӱ��
	//BOOL CheckFightStateApproach(int FightListID);//����ɫ��Ҫ����ʱ��BUFF��Ӱ��;
	int GetLiveNum(BOOL our);//�õ����ŵĵ��˵�����
	int GetTargetID(int UserID, int TargetID, int Num); //�õ���UserID��Num���Ļ��ŵĶ���ID
	BOOL isIDLive(int ID, int& Num);//���һ�������Ƿ����ѡ�� ,�����Ǹ�Num�����������0,����Զ����0
	BOOL SetIndexPos(int UserID, int Index, BOOL bToAll, DWORD DataID);  //��ָ���������ñ�������״̬
	cMengPaiManager m_MengPaiManager;
	BOOL Init();
	BOOL Use(sFightSkillSelect SkillSelect, int UserID);
	BOOL UseNormal(int skillID);
	sSkill* GetPetSkill(int ID){ return &m_PetSkillList[ID]; }
	sSkill*  GetFuZhuSkill(int ID){ return &m_FuZhuSkillList[ID]; }
	sSkill*    GetSkill(int ID);
	sSkill*    GetMengPaiSkill(int ID){ return &m_MengPaiManager.m_SkillList[ID]; }
	sSkill* GetSpecialSkill(int ID){return &m_SpecialSkill[ID];}
	sSkill* GetSpecialSkill2(int ID){ return &m_SpecialSkill2[ID]; }
	sSkill m_PetSkillList[c_NumPetSkill];
	sSkill    m_FuZhuSkillList[13];
	vector<sSkill>    m_SpecialSkill;//�ؼ�
	vector<sSkill>    m_SpecialSkill2;//��Ч
	BOOL UseSwitchMap(int Skilllv, int sSkillID, int UserID, int TargetID);//���ֻ����ɷ���
	BOOL Use1(int Skilllv, int sSkillID, int UserID, int TargetID); //������
	BOOL Use7(int Skilllv, int sSkillID, int UserID, int TargetID); //�Ƹ�����
	BOOL Use26(int Skilllv, int sSkillID, int UserID, int TargetID); //������
	BOOL Use50(int Skilllv, int sSkillID, int UserID, int TargetID); //���컨��	
	BOOL Use71(int Skilllv, int sSkillID, int UserID, int TargetID); //����ն ���꽣��
	BOOL Use80(int Skilllv, int sSkillID, int UserID, int TargetID); //�������
	BOOL Use84(int Skilllv, int sSkillID, int UserID, int TargetID);//����
	BOOL Use1000(int Skilllv, int sSkillID, int UserID, int TargetID); //��ͨ����
	BOOL Use1002(int Skilllv, int sSkillID, int UserID, int TargetID); //����
	BOOL Use1003(int Skilllv, int sSkillID, int UserID, int TargetID); //ʹ�õ���
	BOOL Use1004(int Skilllv, int sSkillID, int UserID, int TargetID); //�ٻ�
	BOOL Use1005(int Skilllv, int sSkillID, int UserID, int TargetID); //ʹ�ð���
	BOOL UseSeal(int Skilllv, int sSkillID, int UserID, int TargetID);//  �Ƚϼ򵥵ķ���
	BOOL Use1006(int Skilllv, int sSkillID, int UserID, int TargetID); //����
	BOOL Use107(int Skilllv, int sSkillID, int UserID, int TargetID); //������...........
	BOOL CheckMagicHit(int Skilllv, int sSkillID, int UserID, int TargetID);
	//1017 //�Ƹ�
		//case 1050:  //���컨��
		//case 1095://����

	//���һ��������ʾ �Ƿ�����
	BOOL Cost(int& outhp, int Skilllv, int sSkillID, int UserID, int num, BOOL bCost);
	int GetSpeed(int skillid);
	void EffectSkill(int Skilllv, int sSkillID, int UserID, int TargetID);

	int AddHp(int target, int num, int percent = 0); //��percent��Ϊ0, numΪ�ٷֱ�,�ı����ֵΪpercent*num;
	int AddMp(int target, int num, int percent = 0);
	void AddHpHurt(int target, int num, int percent = 0);
	void AddCp(int target, int num, int percent = 0);
	int m_time;
inline	void Resurgence(int TargetID);
	//3000�������ؼ�
	sItem2* GetItem(int UserID);
	BOOL CoutCombo(cPetData* user, cPetData* target);
	BOOL CountCri(cPetData* user, cPetData* target);//��ɱ����,�����Ƿ��ɱ
	void CountCriDamage(cPetData* user, cPetData* target, int& dam);
	int CountSpecial(cPetData* user, cPetData* target, BOOL targetSex);//�ع�����
	BOOL CountCriMagic(cPetData* user, cPetData* target, int& nowdam);//��������
	BOOL CoutComboMagic(cPetData* user, int userid, cPetData* ptarget);
	
	void  CountphysicalDamage(int& dam, int UserID, int TargetID, int Damageup = 0);
	enum eMagicWuXing
	{
		eMWX��,
		eMWX��,
		eMWX��,
		eMWXˮ,
		eMWX��,
		eMWX��ˮ,
		eMWX��

	};
	BOOL  CountMagicDamage(int& dam, int UserID, int TargetID, int Damageup = 0,eMagicWuXing wuxing=eMWX��);//���㷨���˺�
	void  CountMagic2Damage(int& dam, int UserID, int TargetID, eMagicWuXing wuxing = eMWX��);//�̶��˺�����
	BOOL  CountParry( cPetData* pUserData,cPetData* pTargetData);//��������
	BOOL  CountProtect(int& dam, cObj* pUser, cObj* pTarget);//��������
	void CorrectCureHP(int& dam,int UserID,int TargetID);//������������
	void CorrectCureMP(int& dam, int UserID, int TargetID);//������������
	int  CountHPPercent(cPetData* ptarget, UINT num, UINT bili)
	{ 
		UINT dam = (ptarget->m_HPMax.GetData() *num) / 100;
		if (dam > num*bili)dam = num*bili;
		return (int)dam;
	}
	int  CountMPPercent(cPetData* ptarget, UINT num, UINT bili)
	{
		UINT dam = (ptarget->m_MPMax.GetData() *num) / 100;
		if (dam > num*bili)dam = num*bili;
		return (int)dam;
	}
	cFightState* m_pFightState;
	BOOL SetDamage(int HP, int Who); //��ʾ���˶���Ѫ,����Ǹ���,��ʾ�ָ�����Ѫ, �˺��������ڱ�����ʾ��Щ����,��Ϊ��ʱ����������10000Ѫ,�������ʾ9999 
	BOOL CheckFanZheng(int UserID,int TargetID,int& dam);//������
	void SuckBlood(int dam, int UserID,cPetData* target);
	void CheckPoision(int UserID, int TargetID);

	BOOL StageWaitReturn(int UserID, int retStage);//�ȴ�����
	BOOL StageWaitFightBack(int UserID, int TargetID, int retStage);//�ȴ����ַ���
	BOOL StageWaitProtect(int UserID, int retStage);//�ȴ������߾�λ
	inline	BOOL StageWaitSleep(int time, int UserID, int retStage);
	BOOL GetSkillAtk(int ID);
	BOOL GetSkillToAll(int ID);
private:
	int CorrectWuXing(cPetData* pUser, cPetData* pTarget, eMagicWuXing MagicWuXing);
	BOOL WaitForMagicPoint(int Waiter);
	void GetDelayAnd(int skillID, int& delay, DWORD& magicID);
	void GetMagicAndNum(int Skilllv, int sSkillID, int UserID, int TargetID, DWORD& MagicID, int& NumMax, int& MagicPos,  BOOL& MagicUporDown, BOOL&  toAll);
	BOOL PhysicalAtkHitStage(int UserID, int targetid, int sSkillID);//�����Ƿ�����
	BOOL m_bInit = FALSE;
};