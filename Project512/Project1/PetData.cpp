#include "global.h"
BOOL cPetManager::Init()
{
	if (m_bInit)return TRUE;
	m_bInit = TRUE;
	int dustbin;
	int dustbin2;
	ifstream File;
	ostringstream oss;
	oss << g_strMediaPath << "数据//召唤兽//召唤兽新.txt";
	File.open(oss.str());
	File >> dustbin;

	if (c_PetNumber != dustbin)
	{
		MessageBoxA(0, "加载召唤兽列表错误", "cPetManager:Init", MB_OK);
		return FALSE;
	}
	for (int i = 0; i < c_PetNumber; i++)
	{
		m_PetList[i].Read2(File);
	}
	File.close();
	

	ifstream File2;
	oss.str("");
	oss << g_strMediaPath << "数据//召唤兽//召唤兽战斗偏移表.txt";
	File2.open(oss.str());
	oss.str("");
	oss << g_strMediaPath << "数据//召唤兽//召唤兽.txt";
	File.open(oss.str());
	File >> dustbin;
	File2 >> dustbin2;
	if (dustbin2 != dustbin)
	{
		ERRBOX;
		return FALSE;
	}
	string dustbinstring;
	if (c_PetNumber != dustbin)return FALSE;
	BOOL offsetnum;
	for (int i = 0; i < c_PetNumber; i++)
	{
		
		m_PetList[i].m_FightOffset[0].ReadPet(File2);
		File2 >> offsetnum;
		getline(File2,dustbinstring);
		if (offsetnum)
		{
			m_PetList[i].m_FightOffset[1].ReadPet(File2);
		}
		else
		{
			m_PetList[i].m_FightOffset[1] = m_PetList[i].m_FightOffset[0];
		}

		File >> m_PetList[i].m_ID;
		File >> m_PetList[i].m_Name;
		File >> m_PetList[i].m_Autorace;
		File >> m_PetList[i].m_bSex;
		File >> m_PetList[i].m_LVLimit; //携带等级
		
		File >> m_PetList[i].m_ZiZhi.m_ATKZZ;
		File >> m_PetList[i].m_ZiZhi.m_DEFZZ;
		File >> m_PetList[i].m_ZiZhi.m_HPZZ;
		File >> m_PetList[i].m_ZiZhi.m_MAGZZ;
		File >> m_PetList[i].m_ZiZhi.m_SPDZZ;
		File >> m_PetList[i].m_ZiZhi.m_AVOIDZZ;
		File >> m_PetList[i].m_ZiZhi.m_Growing;
		File >> m_PetList[i].m_SkillNum;
		for (int j = 0; j < m_PetList[i].m_SkillNum; j++)
		{
			File >> dustbin;
			m_PetList[i].m_pPetSkill[j] = g_pMainState->m_SkillManager.GetPetSkill(dustbin);
		}
	}
	File.close();
	File2.close();

	oss.str("");
	oss << g_strMediaPath << "数据//召唤兽//召唤兽声音.txt";
	File.open(oss.str());

	int numsound;
	File >> numsound;
	for (int i = 0; i < numsound; i++)
	{
		m_PetList[i].ReadSound(File);
	}
	File.close();
	return TRUE;
}

void cPetManager::GetLimitList(int lv, vector<int>& idlist, int stronglv)
{
	if (lv < 5)return  idlist.push_back(1);//0级就大海龟 其它都是稀有召唤兽
	idlist.clear();
	int maxn, minn;
	maxn = lv+1;
	minn = lv - 50+stronglv/5;
	if (minn < 1)minn = 1;
	for (int i = 0; i < c_PetNumber; i++)
	{
		if (m_PetList[i].m_LVLimit < maxn&&m_PetList[i].m_LVLimit>minn)
		{
			if (m_PetList[i].m_ID==92)
				continue;//金身罗汉太难看,不要
			idlist.push_back(i);
		}
	}
	
}

DWORD cPet::GetID(int Pos)
{
	vector<DWORD> out;
	GetID(Pos,out);
	return out[0];
}
void cPet::GetID(int Pos , vector<DWORD>& out)
{
	if (!m_pPetData)return;
	m_pPetData->GetID(m_PetDataZZ.m_JinJieNum.m_PositionID, Pos, out);
}



void cPet::AutoPetData(int modeid, int stronglv, sAddPoint* addpoint,BOOL m_bNeedSkill)
{
	if (addpoint)
	m_PetDataZZ.m_AddPoint = *addpoint;
	else
	{
		sAddPoint addpoint2;
		m_PetDataZZ.m_AddPoint = addpoint2;
	}
	m_PetDataZZ.modeid = modeid;
	if (stronglv == -1)stronglv = g_pMainState->m_GlobalButton[0];
	m_PetDataZZ.m_StrongLv = stronglv;
	m_pPetData = g_pMainState->m_PetDataManager.GetPetData(modeid);
	m_PetDataZZ.AutoPetData(m_bNeedSkill);
	
	m_PetDataZZ.m_race = m_pPetData->m_Autorace;
}

sFightOffset* cPet::GetFightOffset()
{
	return &m_pPetData->m_FightOffset[m_PetDataZZ.m_JinJieNum.GetBJinJie()];
}

void cPet::GetRanse(sChangeColorData* (&pChangeColor)[4])
{

	for (int i = 0; i < 4; i++)
	{
		if (m_PetDataZZ.m_RanSe.m_ranse[i].partnum)
		{
			pChangeColor[i] = &m_PetDataZZ.m_RanSe.m_ranse[i];
		}
		else pChangeColor[i] = 0;
	}
}

void cPet::Set(sShangPinPet* pet,int stronglv)
{	
	m_PetDataZZ.m_AddPoint = pet->m_addpoint;
	m_PetDataZZ.modeid = pet->m_modeid;
	m_PetDataZZ.m_StrongLv = stronglv;
	m_pPetData = g_pMainState->m_PetDataManager.GetPetData(m_PetDataZZ.modeid);
	m_PetDataZZ.Set(pet);
	m_PetDataZZ.LeveUp(g_pCharacter->m_PCData.m_LV + 10,FALSE);
	m_PetDataZZ.FillHPMP();
	m_PetDataZZ.m_RanSe=pet->m_ranse;
	m_PetDataZZ.m_race = m_pPetData->m_Autorace;
}

void cPet::SetData(sPetData* date)
{
	m_pPetData = date;
	m_PetDataZZ.m_race = m_pPetData->m_Autorace;
}

sPetDataPart* cPet::GetPart(int id)
{
	vector<int>& idlist= m_pPetData->m_posIDList[m_PetDataZZ.m_JinJieNum.m_PositionID];
	if (idlist.size() <= id)return 0;
	return	&m_pPetData->m_PartList[idlist[id]];
}




void sPetData::Read2(ifstream& File)
{
	File >> m_ID;
	string dustbin;
	File >> dustbin;
	File >> m_smallhead[0];
	File >> m_bighead[0];
	File >> m_dialoghead[0];
	File >> m_smallhead[1];
	if (0 == m_smallhead[1])
	{
		m_smallhead[1] = m_smallhead[0];
		m_bighead[1] = m_bighead[0];
		m_dialoghead[1] = m_dialoghead[0];
	}
	else
	{
		File >> m_bighead[1];
		File >> m_dialoghead[1];
	}
	int num;
	for (int i = 0; i < 4; i++)
	{
		File >> num;
		m_posIDList[i].resize(num);
		for (int j = 0; j < m_posIDList[i].size(); j++)
			File >> m_posIDList[i][j];
	}
	File >> num;
	m_PartList.resize(num);
	for (int i = 0; i < num; i++)
	{
		m_PartList[i].Read(File);
	}
}

void sPetData::GetID(int positionid, int Pos, vector<DWORD>& out)
{
	DWORD testid;
	BOOL bdujie = FALSE;
	if (positionid>1)bdujie = TRUE;

	testid = m_Sound[bdujie].GetSoundID(Pos);

	if (testid)
	{
		out.resize(1);
		out[0] = testid;
		return;
	}
	switch (Pos)
	{
	case POS_DIALOG:
	case POS_SMALLHEAD:
	case POS_BIGHEAD:
		if (1)
		{
			out.resize(1);
			switch (Pos)
			{
			case POS_DIALOG:		out[0] = m_dialoghead[bdujie];	return;
			case POS_SMALLHEAD:out[0] = m_smallhead[bdujie];	return;
			case POS_BIGHEAD:out[0] = m_bighead[bdujie];	return;
			}
			return;
		}
		break;
	}
	//得到数量
	int num = m_posIDList[positionid].size();
	out.resize(num);
	for (int i = 0; i < num; i++)
	{
		sPetDataPart& lit= m_PartList[m_posIDList[positionid][i]];
		out[i] = lit.GetID(Pos);
	}
}

DWORD sPetData::GetID(int positionid, int Pos)
{
	vector<DWORD> out;
	GetID(positionid, Pos, out);
	return out[0];
}

void sPetData::ReadSound(ifstream& File)
{
	BOOL haveJinJie;
	string dustbin;
	File >> haveJinJie;
	File >> m_Sound[0].m_soundsuffer;
	File >> m_Sound[0].m_soundmajic;
	File >> m_Sound[0].m_soundattack;
	File >> m_Sound[0].m_sounddefend;
	File >> m_Sound[0].m_soundattack2;
	File >> m_Sound[0].m_sounddead;
	getline(File, dustbin);
	if (haveJinJie)
	{
		File >> m_Sound[1].m_soundsuffer;
		File >> m_Sound[1].m_soundmajic;
		File >> m_Sound[1].m_soundattack;
		File >> m_Sound[1].m_sounddefend;
		File >> m_Sound[1].m_soundattack2;
		File >> m_Sound[1].m_sounddead;
	}
	else
	{
		m_Sound[1].m_soundsuffer = m_Sound[0].m_soundsuffer;
		m_Sound[1].m_soundmajic =  m_Sound[0].m_soundmajic;
		m_Sound[1].m_soundattack = m_Sound[0].m_soundattack;
		m_Sound[1].m_sounddefend = m_Sound[0].m_sounddefend;
		m_Sound[1].m_soundattack2 =m_Sound[0].m_soundattack2;
		m_Sound[1].m_sounddead =   m_Sound[0].m_sounddead;
	}
}

DWORD sPetDataPart::GetID(int posid)
{
	switch (posid)
	{
	case POS_STAND:return m_stand;
	case POS_WALK:return m_walk;
	case POS_FIGHT_STAND:return m_fstand;
	case POS_RUN:return m_run;
	case POS_ATTACK:return m_attack;
	case POS_ATTACK2:return m_attack2;
	case POS_MAGIC:return m_majic;
	case POS_DEF:return m_defend;
	case POS_SUFFER:return m_suffer;
	case POS_DEAD:return m_dead;
	case POS_RETURN:return m_return;
	}
	return 0;
}

DWORD sPosSound::GetSoundID(int Pos)
{
	switch (Pos)
	{
	case  POS_SOUNDATTACK:	return m_soundattack;
	case  POS_SOUNDMAGIC:	return m_soundmajic;
	case  POS_SOUNDDEF:		return m_sounddefend;
	case  POS_SOUNDSUFFER:	return m_soundsuffer;
	case  POS_SOUNDDEAD:	return m_sounddead;
	case  POS_SOUNDATTACK2:	return m_soundattack2;
	}
	return 0;
}
