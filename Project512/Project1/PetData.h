#pragma once
#include "global.h"


struct sPetSkill;

struct sPetData:public sObjFather
{
	DWORD	m_bighead[2];
	DWORD   m_smallhead[2];
	DWORD   m_dialoghead[2];
	vector<sPetDataPart> m_PartList;
	sFightOffset m_FightOffset[2];
	int m_LVLimit;//Я���ȼ�
	sZiZhi m_ZiZhi;
	int m_SkillNum=0;
	sSkill* m_pPetSkill[24];//�����24������,�����˰�- -!
	vector<int> m_posIDList[4];
	void GetID(int positionid, int Pos, vector<DWORD>& out);
	DWORD GetID(int positionid, int Pos);
	void Read2(ifstream& File);
	void ReadSound(ifstream& File);
	sPosSound m_Sound[2];
	sPetData()
	{
		for (int i = 0; i < 24; i++)
		{
			m_pPetSkill[i] = 0;
		}
	}
};
const int  c_PetNumber=162;
class cPetManager
{
public:
	BOOL Init();
	void  GetLimitList(int lv, vector<int>& idlist,int stronglv);
	sPetData* GetPetData(int ID){ if (ID < c_PetNumber)return &m_PetList[ID]; return 0; }
	sPetData m_PetList[c_PetNumber];
private:
	BOOL m_bInit = FALSE;
};

class cPet
{
public:
	sPetDataPart* GetPart(int id);
	void SetData(sPetData* date);
	sPetData* GetData(){ return m_pPetData; }

	void Set(sShangPinPet* pet, int stronglv);
	sFightOffset* GetFightOffset();
//	BOOL m_ID=0;
	cPetData  m_PetDataZZ;
	
	//ͨ���ȼ�(0-999) �� ǿ��(��Χ0-9) �������� ��ʹ���⺯��ǰ�������üӵ㷽ʽ
	void AutoPetData(
		int modeid, int stronglv,
		sAddPoint* addpoint,
		BOOL m_bNeedSkill = TRUE
		);
	void GetID(int Pos, vector<DWORD>& out);// DWORD& data1, DWORD& data2, DWORD& data3,DWORD& data4);
	DWORD GetID(int Pos);
	void GetRanse(sChangeColorData* (&pChangeColor)[4]);
private:
	sPetData*  m_pPetData = 0;

};