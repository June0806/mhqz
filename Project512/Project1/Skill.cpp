#include "global.h"
BOOL cSkillManager::Use(sFightSkillSelect SkillSelect, int UserID)
{
	
	int  Skilllv = SkillSelect.Lv;
	int  TargetID = SkillSelect.TargetID;
	int sSkillID = SkillSelect.SkillID;
	int& stage = m_pFightState->m_pList[UserID]->GetWork()->stage;
	if (0 == stage)
	{
		if (m_pFightState->m_pList[UserID]->m_pData->m_NowPos == POS_FIGHT_STAND)
		{
			cPetData* petdata;
			cObj* pUser = m_pFightState->m_pList[UserID]->m_pData;
			if (UserID < 10)
				pUser->SetDirection(7);
			else 
				pUser->SetDirection(3);
			pUser->AutoFightDirection(UserID);
			pUser->GetDateP(petdata);
			pUser->SetStatic(FALSE);//�п��ܸղ��ڱ���,���ھ�ֹ״̬ʱ�ֵ�����
			g_pMainState->m_FightList[UserID].targetID = SkillSelect.TargetID;
			g_pMainState->m_FightList[UserID].m_NowTargetID = SkillSelect.TargetID;
			BOOL bSeal=FALSE;
			if (petdata->m_FightProperty.cannotAll)
				bSeal = TRUE;
			else
			if (sSkillID < 3000)//3000�������ؼ������ڷ���
			{
				if (sSkillID<2000 && sSkillID>1000)
				{
				}
				else
				if (1000 == sSkillID)
				{
					if (petdata->m_FightProperty.cannotAttack)bSeal = TRUE;	
				}
				else
				if (petdata->m_FightProperty.cannotMagic)bSeal = TRUE;
			}
			else
			{//�ؼ�
				if (petdata->m_FightProperty.cannotSkill)bSeal = TRUE;
			}
			if (bSeal)
			{
				if (UserID == g_pMainState->m_HeroID || UserID == g_pMainState->m_HeroID+5)
				{
					g_pMainState->m_Channel.AddNewString("�����޷�ʹ��");
				}
				g_pMainState->m_FightList[UserID].m_work.clear();
				return TRUE;
			}
			int hpout;
			if (!Cost(hpout, Skilllv, sSkillID, UserID, 1, FALSE))
			{
				if (UserID==g_pMainState->m_HeroID)
					g_pMainState->m_Channel.AddNewString("�����޷�ʹ��");
				m_pFightState->m_pList[UserID]->m_work.pop_back();
				return TRUE;
			}
			BOOL m_bmiaoshushow=TRUE;
			string actionname;
			sSkill* pskill = g_pMainState->m_SkillManager.GetSkill(m_pFightState->m_pList[UserID]->m_SkillSelect.SkillID);
			if (pskill)
			{
				actionname = pskill->m_Name;
			}
			else
				switch (m_pFightState->m_pList[UserID]->m_SkillSelect.SkillID)
			{
				case 1003:actionname = "��Ѫ"; break;
				case 1004:actionname = "�ٻ�"; break;
				case 1005:actionname = "����"; break;
				default:m_bmiaoshushow = FALSE; break;
			}
			if (m_bmiaoshushow)
			{
				ostringstream oss;
				oss << m_pFightState->m_pList[UserID]->m_pData->m_TrueName.GetString();
				oss << "ʹ����";
				oss << actionname;
				m_pFightState->m_MiaoShu.SetString(oss.str());
				m_pFightState->m_MiaoShu.SetTime(150);
				m_pFightState->m_MiaoShu.UpdateXCenter();
			}


			//���ڼ��
			int truetarget = m_pFightState->CheckTarget(UserID, TargetID,sSkillID);
			m_pFightState->m_pList[UserID]->m_NowTargetID = truetarget;
			m_pFightState->m_pList[UserID]->targetID = truetarget;
			if (truetarget == -1)
			{
				m_pFightState->m_pList[UserID]->m_work.pop_back();
				return TRUE;
			}
			if (truetarget == UserID)
			{
				cObj* pUser = m_pFightState->m_pList[UserID]->m_pData;
				cPetData* pUserData;
				pUser->GetDateP(pUserData);
				if (pUserData->m_FightProperty.chaos)
				{
					if (sSkillID == 1000)
					{
						m_pFightState->m_pList[UserID]->m_work.pop_back();
						return TRUE;
					}
				}
			}
			if (truetarget != TargetID)
			{
				m_pFightState->m_pList[UserID]->m_SkillSelect.TargetID = truetarget;			
				TargetID = truetarget;

			}
		}

	}

	switch (sSkillID)
	{
	case 1:
		if (Use1(Skilllv, sSkillID, UserID, TargetID))
			return TRUE;
		return FALSE;
		/*case 1010:
			if (Use1Ex(Skilllv, sSkillID, UserID, TargetID))
			return TRUE;
			return FALSE;*/
	case 162://���޵���
	case 107://������
	case 2087://̩ɽѹ��
		if (Use107(Skilllv, sSkillID, UserID, TargetID))
			return TRUE;
		return FALSE;
	
		/*if (Use50(Skilllv, sSkillID, UserID, TargetID))
			return TRUE;
		return FALSE;*/
	case 1050:  //���컨��
	case 1095://����
	case 0: //��ɨǧ��
	case 1010:
	case 1017://�Ƹ��ڶ���
	case 71:  //����ն
	case 97://���꽣��
	case 141://����
	case 142:// ӥ��
	case 143:// ʨ��
	case 148://������
	case 2092://����һ��
	case 2094://�����ٻ�
	case 2098://�ƶ��б�
	case 2099://���ݻ���
	case 2100://���ŭ��
	case 3034://��Ѫ
	case 3035://����
	case 3040://����
	case 3042://��Ѫ2
	case 3043://����EX
	case 3044://��ѪEX
		if (Use71(Skilllv, sSkillID, UserID, TargetID))
			return TRUE;
		return FALSE;
	case 80://�������
	case 131:// ��ɰ��ʯ
		if (Use80(Skilllv, sSkillID, UserID, TargetID))
			return TRUE;
		return FALSE;
	case 84:	return Use84(Skilllv, sSkillID, UserID, TargetID);//����
	case 1000:	return Use1000(Skilllv, sSkillID, UserID, TargetID);
	case 1003:	return Use1003(Skilllv, sSkillID, UserID, TargetID);
	case 1004:	return Use1004(Skilllv, sSkillID, UserID, TargetID);		
	case 1005:	return Use1005(Skilllv, sSkillID, UserID, TargetID);
	case 1006:  return Use1006(Skilllv, sSkillID, UserID, TargetID);
	default:
		if (UseSeal(Skilllv, sSkillID, UserID, TargetID))
			return TRUE;
		return FALSE;
	}
}

BOOL cSkillManager::Init()
{
	m_MengPaiManager.Init();
	if (m_bInit)return TRUE;
	m_bInit = TRUE;
	ifstream File;
	ostringstream oss;
	oss << g_strMediaPath << "����//����//�ٻ��޼���.txt";
	File.open(oss.str());
	if (!File)ERRBOX;
	int dustbin;
	string dustbin2str;
	File >> dustbin;
	if (c_NumPetSkill != dustbin)ERRBOX;
	for (int i = 0; i < c_NumPetSkill; i++)
	{
		sSkill& skill = m_PetSkillList[i];
		File >> skill.m_Id;
		File >> skill.m_Name;
		File >> skill.m_BigHead;
		File >> skill.m_SmallHead;
		File >> skill.m_SkillType;
		if (skill.m_SkillType == 7)
		{

			File >> skill.m_NumUnCompatible;
			for (int j = 0; j < skill.m_NumUnCompatible; j++)
			{
				int uncompatibleid;
				File >> uncompatibleid;
				skill.m_UnCompatible.push_back(uncompatibleid);
			}
		}
		else
		{
			skill.m_NumUnCompatible = 0;
			File >> skill.m_Sound;
			File >> skill.m_EffectType;
			File >> skill.m_MagicID;
			File >> skill.m_MagicPos;
			File >> skill.m_TargetNum;
			File >> skill.m_TargetNumOffset;
		}
		File >> skill.m_Text;
		if (7 != skill.m_SkillType)
			File >> skill.m_tCost;
		//��ȡ�ٻ��޼�������,�����׻�,̩ɽѹ����
		
		
	}
	File.close();

	oss.str("");
	oss << g_strMediaPath << "����//����//��������.txt";
	File.open(oss.str());
	if (!File)ERRBOX;
	File >> dustbin;
	if (c_NumFuZhu != dustbin)
	{
		ERRBOX;
	}
	for (int i = 0; i < c_NumFuZhu; i++)
	{
		sSkill& skill = m_FuZhuSkillList[i];
		File >> skill.m_Id;
		File >> skill.m_Name;
		File >> skill.m_BigHead;
		File >> skill.m_SmallHead;
		getline(File, skill.m_Text);
		getline(File, skill.m_Text);
	}
	File.close();

	oss.str("");
	oss << g_strMediaPath << "����//����//�ؼ�.txt";
	File.open(oss.str());
	if (!File)ERRBOX;

	File >> dustbin;
	m_SpecialSkill.resize(dustbin);
	for (int i = 0; i < dustbin; i++)
	{
		sSkill& skill = m_SpecialSkill[i];
		File >> skill.m_Id;
		File >> skill.m_Name;
		File >> skill.m_BigHead;
		File >> skill.m_SmallHead;
		File >> skill.m_Cost;
		oss.str("");
		oss << skill.m_Cost;
		skill.m_tCost = oss.str();
		File >> skill.m_SkillType;
		File >> skill.m_MagicID;
		File >> skill.m_MagicPos;
		File >> skill.m_TargetNum;
		skill.m_TargetNumOffset = 0;
		getline(File, skill.m_Text);
	}
	File.close();
	oss.str("");
	oss << g_strMediaPath << "�����б�\\��ʯ.txt";
	File.open(oss.str());
	if (!File)ERRBOX;

	File >> dustbin;
	string name;
	DWORD smallhead, bighead;
	int num;
	File >> bighead;
	File >> smallhead;
	File >> name;
	File >> num;
	m_SpecialSkill2.resize(num);
	for (int i = 0; i < num; i++)
	{
		sSkill& skill = m_SpecialSkill2[i];
		File>> skill.m_Id;
		File >> skill.m_Cost;
		File >> dustbin;
		 skill.m_Name=name;
		File >> skill.m_Text;
		skill.m_BigHead = bighead;
		skill.m_SmallHead = smallhead;
		skill.m_SkillType = 7;
	}
	File.close();
	oss.str("");
	oss << g_strMediaPath << "����\\����\\�ؼ�����.txt";
	File.open(oss.str());
	if (!File)ERRBOX;
	int soundnum;
	File >> soundnum;
	int soundid;
	string soundname;
	for (int i = 0; i < soundnum; i++)
	{
		File >> soundid;
		File >> soundname;
		File >> m_SpecialSkill[i].m_Sound;
	}
	File.close();

	return TRUE;
}


int cSkillManager::GetLiveNum(BOOL our)
{
	int k = 0;
	if (our)
	{
		for (int i = 10; i < 20; i++)
		{
			if (m_pFightState->m_pList[i]->isLive)
				k += 1;
		}
	}
	else
	{
		for (int i = 0; i < 10; i++)
		{
			if (m_pFightState->m_pList[i]->isLive)
				k += 1;
		}
	}
	return k;
}


int cSkillManager::GetTargetID(int UserID, int TargetID, int Num)
{
	return g_pMainState->m_AI.GetNear(TargetID, Num, TRUE);
}

BOOL cSkillManager::Use1000(int Skilllv, int sSkillID, int UserID, int TargetID)
{
	sFightList* pUserList = &g_pMainState->m_FightList[UserID];
	cObj* pUser = pUserList->m_pData;
	cPetData* pUserData;
	pUser->GetDateP(pUserData);
	pUserList->targetID = TargetID;
	cObj* pTarget = g_pMainState->m_FightList[TargetID].m_pData;
	cPetData* pTargetData;
	pTarget->GetDateP(pTargetData);
	int& stage = pUserList->GetWork()->stage;
	switch (stage)
	{
	case 0:
		if (1)
		{
			sFightOffset* pOffse = pUserList->GetFightOffset();
			if (pOffse->m_xOff > 0)
				stage = 22;
			else
			{
				stage = 21;
			}
		}
		return TRUE;
	case 22:
		if (m_pFightState->Approach(UserID, TargetID))
			stage = 21;
		return TRUE;
	case 21:
		if (1)
		{
			//�������
			CheckHit(pUserData, pTargetData);
			pUser->Set(POS_FIGHT_STAND);
			if (pTargetData->m_FightProperty.m_bHit)
			{
				CountParry(pUserData, pTargetData);//����
				CountCri(pUserData, pTargetData);//��ɱ
				//������˱���,�ȴ������߾�λ
				if (pTargetData->m_FightProperty.m_guardID.size())
				{
					stage = 101;
					return TRUE;
				}
			}
			stage = 20;
		}
		return TRUE;
	case 20:
		StageWaitSleep(4, UserID, 120);
		return TRUE;
	case 120:
		
		{
			int pos;
			if (rand()%2)
				pos = POS_ATTACK2;
			else
				pos = POS_ATTACK;
			pUser->Set(pos);
			pUser->SetSound(pos);
			stage = 1;
		}
	case 1:
		
		{
			int atkp = m_pFightState->WorkAttack(UserID);
			switch (atkp)
			{
			case 0:
				return TRUE;
			default:
				
				{	
					//�ǲ��ǹ���
					sFightOffset* pOffse = pUserList->GetFightOffset();
					if (pOffse->m_xOff== 0)
					{
						m_pFightState->m_HiddenWeapons.Load(0xE2BC8127);
						m_pFightState->m_HiddenWeaponsUseNum = 1;
						m_pFightState->m_HiddenWeaponsp[0].m_NeedShow = TRUE;
						m_pFightState->m_HiddenWeaponsp[0].m_x = pUser->GetX();
						m_pFightState->m_HiddenWeaponsp[0].m_y = pUser->GetY()+20;
						m_pFightState->m_HiddenWeaponsTargetID[0] = TargetID;
						stage = 23;
						return TRUE;
					}
					if (!PhysicalAtkHitStage(UserID, TargetID, sSkillID))
					{
						stage = 3;
						return TRUE;
					}
					if (atkp != 1)
					{
						stage += 1;
						return TRUE;
					}
				}
				break;
			}
		}
		return TRUE;
	case 23://����
	
		{
			if (m_pFightState->WaitPosEnd(UserID))
			{
				pUser->Set(POS_FIGHT_STAND);
			}
			if (m_pFightState->HiddenWeaponsMove(0, 50))
			{
				if (!PhysicalAtkHitStage(UserID, TargetID, sSkillID))
				{
					stage = 3;
					return TRUE;
				}
				stage = 2;
			}
		}
		return TRUE;
	case 2:  //�����ж�
	
		{
				 if (m_pFightState->WaitPosEnd(UserID))
				 {
					 pUser->Set(POS_FIGHT_STAND);
				 }
			stage = 3;
			int dam=0;
			CountphysicalDamage(dam, UserID, TargetID);
			if (pUserData->m_Spurting.d1)
			{
				int jianshetarget;
				for (int i = 0; i < 2; i++)
				{
					if (g_pMainState->m_FightList[TargetID].CheckDie())
						jianshetarget = g_pMainState->m_AI.GetNear(TargetID, i, TRUE);
					else
					 jianshetarget= g_pMainState->m_AI.GetNear(TargetID, i + 1, TRUE);
					 if (jianshetarget>-1)
					 {
						 cPetData* jianshepet;
						 g_pMainState->m_FightList[jianshetarget].m_pData->GetDateP(jianshepet);
						 int rate = pUserData->m_Spurting.d1 - jianshepet->m_Spurting.d2;
						 if (rate > 0)
						 {
							int   dam2 = (dam*rate)/100;
							 AddHp(jianshetarget,-dam2);
							 SetDamage(dam2, jianshetarget);
							 if (1 > jianshepet->m_HP)//��������
							 {
								 if (UserID < 10)
									 g_pMainState->m_FightList[jianshetarget].m_sufferDirection = 7;
								 else g_pMainState->m_FightList[jianshetarget].m_sufferDirection = 3;
								 g_pMainState->m_pFightState->SetDie(jianshetarget);
						//		 return TRUE;
							 }
						 }
					 }
				}
			}
		}
		return TRUE;
	case 3:
		if (m_pFightState->WaitPosEnd(UserID)||(pUser->m_NowPos==POS_FIGHT_STAND))
		{
			
			//pUser->SetStatic(FALSE);
			g_pMainState->m_FightList[TargetID].SetMagiSufferID(0, 1, &m_pFightState->m_FullSkill.m_SufferMagic[TargetID]);
			if (g_pMainState->m_FightList[TargetID].CheckDie())
			{
				stage = 5;
				return TRUE;
			}
			stage = 4;
		}
		return TRUE;
	case 4:
	
		{
			//�Ƿ񷴻�
			int rat = pTargetData->m_CounterRate.d1 - pUserData->m_CounterRate.d2;
			if ((rand() % 100) < rat)
			{
				pUser->Set(POS_FIGHT_STAND);
				stage = 14;
			}
			else 
				stage = 7;
		}
		return TRUE;
	case 14:
		
		{
			//�ȵȴ�Ӳֱ����
			if (g_pMainState->m_FightList[TargetID].m_work.size())return TRUE;
			stage = 6;
			//����
			pUser->Set(POS_FIGHT_STAND);
			pTarget->SetStatic(FALSE);
			m_pFightState->SetCounter(TargetID, UserID);
			
		}
		return TRUE;
	case 5:
		return StageWaitReturn(UserID, -1);
	case 6://�ȴ����ַ�����
		return StageWaitFightBack(UserID,TargetID,7);
	case 7://�Ƿ�����
		if (!g_pMainState->m_FightList[TargetID].CheckDie())
		{
			if (CoutCombo(pUserData, pTargetData))
			{
				g_pMainState->m_FightList[UserID].GetWork()->stage = 21;
			}
			else
				stage = 5;
		}
		else
			stage = 5;
		return TRUE;
	case 101: //�ȴ������߾�λ
		return StageWaitProtect(TargetID, 1);
	}
	return FALSE;
}

BOOL cSkillManager::SetDamage(int HP, int Who)
{
	if (0 == HP)return TRUE;
	if (99999<HP)HP = 99999;
	else if (-99999>HP)HP = -99999;
	sFightList* pUserList =&g_pMainState->m_FightList[Who];
	int color = 0;
	if (HP > 0)
	{
		color = 1;
	}
	else
	{
		HP = -HP;
	}
	for (int i = 0; i < 5; i++)
	{
		if (color == 1)
			pUserList->m_Damage[i].Set(&g_pMainState->m_FNumRed);
		else
		pUserList->m_Damage[i].Set(&g_pMainState->m_FNumGreen);
	}
	ostringstream oss;
	int num = 0;
	oss.str("");
	oss << HP;
	string str2 = oss.str();
	pUserList->m_DamageNum = str2.length();
	num = HP / 10000;
	pUserList->m_Damage[4].SetFrame(num);
	HP -= num * 1000;
	num = HP / 1000;
	pUserList->m_Damage[3].SetFrame(num);
	HP -= num * 1000;
	num = HP / 100;
	pUserList->m_Damage[2].SetFrame(num);
	HP -= num * 100;
	num= HP / 10;
	pUserList->m_Damage[1].SetFrame(num);
	HP -= num * 10;
	num = HP;
	pUserList->m_Damage[0].SetFrame(num);

	int width = 0;
	for (int i = 0; i < pUserList->m_DamageNum; i++)
	{
		width += pUserList->m_Damage[i].m_Width;
	}
	//����ȡ��Ӱ������,����ĳЩ��ɫ(����������- -!!!)����ʱ�������,���ֻ�͹ֵ����ֽ���
	int xpos = pUserList->m_pData->GetShadow()->GetX() - width / 2;

	int ypos = (pUserList->m_pData->GetY() + pUserList->m_pData->GetTopY()+10)/2;

	for (int i = pUserList->m_DamageNum-1; i >-1; i--)
	{
		pUserList->m_DamageTime[i] = 20 - i * 4;
		pUserList->m_Damage[i].SetShowHeight(1);
		pUserList->m_Damage[i].SetXY(xpos, ypos);
		pUserList->m_Damage[i].m_NeedShow = TRUE;
		xpos += pUserList->m_Damage[i].m_Width;
	}

	g_pMainState->m_FightList[Who].m_pData->UpdateHpLine();
	return TRUE;
}
//������
BOOL cSkillManager::Use1(int Skilllv, int sSkillID, int UserID, int TargetID)
{
	sFightList* pUserList = m_pFightState->m_pList[UserID];
	cObj* pUser = pUserList->m_pData;
	int& stage = pUserList->GetWork()->stage;
	switch (stage)
	{
	case 0:
		pUser->Set(POS_MAGIC);
		pUser->SetSound(POS_MAGIC);
		stage = 1;
		return TRUE;
	case 1:
		if (WaitForMagicPoint(UserID))
		{
			pUser->SetMagicSound(sSkillID);
			pUserList->SetMagiSufferID(968268166, -10, &m_pFightState->m_FullSkill.m_SufferMagic[UserID]);
			stage += 1;
		}
		return TRUE;
	case 2:
		if (m_pFightState->WaitPosEnd(UserID))
		{
			pUser->Set(POS_FIGHT_STAND);
		}
		if (!pUser->m_MagicSuffer.m_NeedShow)
		{
			stage += 1;
			EffectSkill(Skilllv, sSkillID, UserID, TargetID);
			pUser->Set(POS_FIGHT_STAND);		
		}
		return TRUE;
	case 3:
		if (m_pFightState->WaitPosEnd(UserID) || (pUser->m_NowPos == POS_FIGHT_STAND))
		{
			pUserList->m_work.pop_back();
		}
		return TRUE;
	}
	return TRUE;
}


BOOL cSkillManager::Use7(int Skilllv, int sSkillID, int UserID, int TargetID)
{//�Ƹ�����
	sFightList* pUserList = m_pFightState->m_pList[UserID];
	cObj* pUser = pUserList->m_pData;
	int& stage = pUserList->GetWork()->stage;
	switch (stage)
	{
	case 0:
		//���ڼ��
		pUser->Set(POS_MAGIC);
		pUser->SetSound(POS_MAGIC);
		stage = 1;
		return TRUE;
	case 1:
		if (WaitForMagicPoint(UserID))
		{
			pUserList->m_MaxTargetNum = 0;//��๥��3��Ŀ��
			cPetData* pPet;
			pUser->GetDateP(pPet);
			int num = 3 + pPet->m_MagicNumAdd;
			for (int i = 0; i < num; i++)
			{
				pUserList->targetlist[i] = GetTargetID(UserID, TargetID, i);
				int id = pUserList->targetlist[i];
				if (UserID == id)continue;
				if (id == -1)break;
				pUserList->m_MaxTargetNum += 1;
			}
			int nowtarget;
			for (int i = 0; i < pUserList->m_MaxTargetNum; i++)
			{
				nowtarget = pUserList->targetlist[i];
				m_pFightState->m_pList[nowtarget]->SetMagiSufferID(0x1EE9406C, 0, &m_pFightState->m_FullSkill.m_SufferMagic[TargetID]);
				m_pFightState->m_pList[nowtarget]->m_pData->m_MagicSuffer.m_FrameGap = GetSpeed(sSkillID);
				m_pFightState->SetMagicSuffer(nowtarget, pUser->GetDirection());
			}
			pUserList->m_NowTarget = -1;
			stage = 21;
		}
		return TRUE;
	case 21://�˺��ж�
		if (m_pFightState->m_pList[TargetID]->m_pData->m_MagicSuffer.GetFrame() == m_pFightState->m_pList[TargetID]->m_pData->m_MagicSuffer.m_pFile->GetMaxFrame() - 15)
		{
			int nowtarget;
			for (int i = 0; i < pUserList->m_MaxTargetNum; i++)
			{
				nowtarget = pUserList->targetlist[i];
				m_pFightState->SetSuffer(nowtarget, pUser->GetDirection(),UserID);
				EffectSkill(Skilllv, sSkillID, UserID, nowtarget);

				m_pFightState->SwitchMagicSuffer(nowtarget);
				m_pFightState->m_pList[nowtarget]->SetMagiSufferID(0, 1, 0);
			}
			pUserList->NextTarget(m_pFightState,sSkillID);
			stage = 2;
		}
		if (m_pFightState->WaitPosEnd(UserID))
		{
			pUser->Set(POS_FIGHT_STAND);
		}
		return TRUE;
	case 2:
		if (m_pFightState->WaitPosEnd(UserID) || (pUser->m_NowPos == POS_FIGHT_STAND))
		{
			pUser->Set(POS_FIGHT_STAND);
			
			if (g_pMainState->m_FightList[UserID].m_NowTargetID>-1)
			{
				
			}
		}
		return TRUE;
	case 20:
		return	StageWaitSleep(20, UserID, 3);
	case 3:
		if (g_pMainState->m_FightList[pUserList->m_NowTargetID].CheckDie())
		{		
			stage = 7;
			int hpout;

			sBuff buf;
			buf.ID = 1;
			SetDateBuffUp(UserID, UserID, sSkillID, buf, Skilllv);

			Cost(hpout, Skilllv, sSkillID, UserID, 1, TRUE);
			return TRUE;
		}
		if (m_pFightState->Approach(UserID, pUserList->m_NowTargetID))
		{
			stage += 1;
		}
		return TRUE;


	case 4:
		
		{
			int pos;
			if (rand() % 2)
				pos = POS_ATTACK2;
			else
				pos = POS_ATTACK;
			pUser->Set(pos);
			pUser->SetSound(pos);
			stage = 120;
		}
	case 120:
		if (m_pFightState->WorkAttack(UserID))
		{
			g_pMainState->m_FightList[TargetID].m_work.clear();
			stage = 5;
		}
		return TRUE;
	case 5:  //�����ж�
		
		{
			
			m_pFightState->SetSuffer(pUserList->m_NowTargetID, pUser->GetDirection(),UserID);
			//150%���˺�
			EffectSkill(Skilllv, sSkillID, UserID, pUserList->m_NowTargetID);
			m_pFightState->m_pList[pUserList->m_NowTargetID]->SetMagiSufferID(0x1D3FF13C, 1, &m_pFightState->m_FullSkill.m_SufferMagic[TargetID]);
			
			
			if (pUserList->NextTarget(m_pFightState, sSkillID))
			{
				stage = 20;
			}
			else
			{
				sBuff buf;
				buf.ID = 1;
				SetDateBuffUp(UserID, UserID, sSkillID, buf, Skilllv);
				int outhp;
				Cost(outhp, Skilllv, sSkillID, UserID, 3, TRUE);
				stage = 7;
			}
			
			
			stage =2;
		}
		return  TRUE;
	case 7:
		return StageWaitReturn(UserID, -1);
	}
	return TRUE;
}

BOOL cSkillManager::isIDLive(int ID, int& Num)
{
	if (m_pFightState->m_pList[ID]->isLive)
	{
		if (Num == 0)return TRUE;
		Num -= 1;
	}
	return FALSE;
}


BOOL cSkillManager::UseSeal(int Skilllv, int sSkillID, int UserID, int TargetID)
{
	sFightList* pUserList = &g_pMainState->m_FightList[UserID];
	cObj* pUser = pUserList->m_pData;
	cPetData* pUserData;
	cPetData* pTargetData;
	pUser->GetDateP(pUserData);	
	cObj* pTarget = g_pMainState->m_FightList[TargetID].m_pData;
	pTarget->GetDateP(pTargetData);
	int& stage = pUserList->GetWork()->stage;
	switch (stage)
	{
	case 0:
		
		{
			int hpout=0;
			Cost(hpout, Skilllv, sSkillID, UserID, 1, TRUE);
			if (hpout)
			{
				SetDamage(hpout, UserID);
			}
			pUser->Set(POS_MAGIC);
			pUser->SetSound(POS_MAGIC);
			stage = 1;
		}
		return TRUE;
	case 1:
		if (WaitForMagicPoint(UserID) || pUser->m_pShowData[0]->m_isStatic)
		{
			pUser->SetStatic(FALSE);
			pUser->SetMagicSound(sSkillID);//����

			DWORD MagicID;
			int MagicPos;
			BOOL MagicUporDown;
			BOOL  toAll;
			BOOL bAtk;
			int maxnum;
			GetMagicAndNum(Skilllv, sSkillID, UserID, TargetID, MagicID, maxnum, MagicPos, MagicUporDown, toAll);
			int magicspeed = GetSpeed(sSkillID);
			bAtk = GetSkillAtk(sSkillID);
			pUserList->m_MaxTargetNum = 0;
			if (toAll)	m_pFightState->m_FullSkill.SetMagicUp(0,MagicID,0,0);

			if (maxnum < 2)
			{
				pUserList->targetlist[0] = TargetID;
				pUserList->m_MaxTargetNum = 1;
			}
			else
			switch (sSkillID)
			{
			case 33:
				
				{
					vector<POINT> tList;
					g_pMainState->m_AI.m_AttackAI.FindEnemy(TRUE, FALSE, tList, cAttackAI::eET����ϵ, UserID,33);
					if (tList.size() < maxnum)maxnum = tList.size();
					int i = 0; 
					for (; i < maxnum; i++)
					{
						if (tList[i].x == TargetID)break;
					}
					for (int i = 0; i < maxnum; i++)
					{
						pUserList->targetlist[i] = tList[i].x;
					}
					pUserList->m_MaxTargetNum = maxnum;
				}
				break;
			case 38:
			case 39:
			case 40:
			case 41:
			case 42:
			case 45:
			case 91:
				
				{
					vector<POINT> tList;
					g_pMainState->m_AI.m_AttackAI.FindEnemy(FALSE,FALSE,tList, cAttackAI::eETѪ����, UserID);
					if (tList.size() < maxnum)maxnum = tList.size();
					int i = 0;
					for (; i < maxnum; i++)
					{
						if (tList[i].x == TargetID)break;
					}						
					if (i == maxnum)tList[maxnum - 1].x = TargetID;
					for (i = 0; i < maxnum; i++)
					{
						pUserList->targetlist[i] = tList[i].x;
					}
					pUserList->m_MaxTargetNum = maxnum;
				}
				break;	
			case 112:
				
				{
					vector<POINT> tList;
					g_pMainState->m_AI.m_AttackAI.FindEnemy(TRUE, FALSE, tList, cAttackAI::eET��ϵ, UserID, 112);
					if (tList.size() < maxnum)maxnum = tList.size();
					int i = 0;
					for (; i < maxnum; i++)
					{
						if (tList[i].x == TargetID)break;
					}
					for (int i = 0; i < maxnum; i++)
					{
						pUserList->targetlist[i] = tList[i].x;
					}
					pUserList->m_MaxTargetNum = maxnum;
				}
				break;
			case 37:
			case 104:
			case 125:
			case 3008:
			case 3009:
			case 3048:
				pUserList->m_MaxTargetNum = 0;
				for (int i = 0; i < maxnum; i++)
				{
					pUserList->targetlist[pUserList->m_MaxTargetNum] = g_pMainState->m_AI.GetNear(TargetID, i, 2);

					if (bAtk)
					if (UserID == pUserList->targetlist[pUserList->m_MaxTargetNum])
					{//��ֹ���󹥻��Լ�
						continue;
					}

					if (-1 == pUserList->targetlist[pUserList->m_MaxTargetNum])
					{
						break;
					}
					pUserList->m_MaxTargetNum += 1;
				}
				break;
			default:
				for (int i = 0; i < maxnum; i++)
				{
					pUserList->targetlist[pUserList->m_MaxTargetNum] = g_pMainState->m_AI.GetNear(TargetID, i, 1);
						//GetTargetID(UserID, TargetID, i);

					if (bAtk)
					if (UserID == pUserList->targetlist[pUserList->m_MaxTargetNum])
					{//��ֹ���󹥻��Լ�
						continue;
					}

					if (-1 == pUserList->targetlist[pUserList->m_MaxTargetNum])
					{
						break;
					}
					pUserList->m_MaxTargetNum += 1;
				}
				break;
			}
			for (int i = 0; i < pUserList->m_MaxTargetNum; i++)
			{
				pUserList->m_NowTargetID = pUserList->targetlist[i];
				if (bAtk)m_pFightState->SetMagicSuffer(pUserList->m_NowTargetID, pUser->GetDirection());

				if (toAll)
				{
					m_pFightState->m_pList[pUserList->m_NowTargetID]->m_bSufferFullSkill = TRUE;
					m_pFightState->m_FullSkill.m_FullSkilUP[0].m_Picture.m_FrameGap = magicspeed;
				}
				else
				{
					m_pFightState->m_pList[pUserList->m_NowTargetID]->m_bSufferFullSkill = FALSE;
					m_pFightState->m_pList[pUserList->m_NowTargetID]->SetMagiSufferID(MagicID, MagicPos, &m_pFightState->m_FullSkill.m_SufferMagic[TargetID], MagicUporDown);
					m_pFightState->m_pList[pUserList->m_NowTargetID]->m_pData->m_MagicSuffer.m_FrameGap = magicspeed;
				}
			}
			stage += 1;
		}
		return TRUE;
	case 2:
		if (m_pFightState->WaitPosEnd(UserID) || pUser->m_pShowData[0]->m_isStatic || (pUser->m_NowPos==POS_FIGHT_STAND))
		{
			pUser->Set(POS_FIGHT_STAND);
			pUser->SetStatic(FALSE);
			stage += 1;
		}
	case 3:  //����BUFF
		
		{
			DWORD MagicID;
			BOOL toAll=GetSkillToAll(sSkillID);
			int LateTime; //�˺�������ʱ
			BOOL bAtk;
			GetDelayAnd(sSkillID, LateTime, MagicID);
			bAtk = GetSkillAtk(sSkillID);
			BOOL bover=FALSE;
			if (toAll)
			{
				if (m_pFightState->m_FullSkill.m_FullSkilUP[0].GetNowFrame() == m_pFightState->m_FullSkill.m_FullSkilUP[0].GetMaxFrame() - LateTime)
				{
					bover = TRUE;
				}
			}
			else
			{
				if (!pTarget->m_MagicSuffer.m_NeedShow)bover = TRUE;
				else
				{
					if (LateTime >= pTarget->m_MagicSuffer.m_pFile->GetMaxFrame())LateTime = pTarget->m_MagicSuffer.m_pFile->GetMaxFrame() - 1;
					if (pTarget->m_MagicSuffer.GetFrame() == pTarget->m_MagicSuffer.m_pFile->GetMaxFrame() - LateTime)
					{
						bover = TRUE;
					}
				}
			}
			if (bover)
			{
				stage = 4;
				pUserList->m_NowTarget = -1;
				for (int i = 0; i < pUserList->m_MaxTargetNum; i++)
				{
					pUserList->m_NowTargetID = pUserList->targetlist[i];
					if (bAtk)
					{
						m_pFightState->SwitchMagicSuffer(pUserList->m_NowTargetID);
						m_pFightState->m_pList[pUserList->m_NowTargetID]->m_pData->SetSound(POS_SUFFER);//����

					}
					
					if (CheckMagicHit(Skilllv, sSkillID, UserID, pUserList->m_NowTargetID))
					{
						EffectSkill(Skilllv, sSkillID, UserID, pUserList->m_NowTargetID);
					}
				}
				cPetData* pPet;
				pUser->GetDateP(pPet);
				if (1 > pPet->m_HP)
				{
					if (UserID < 10)
						pUserList->m_sufferDirection = 3;
					else pUserList->m_sufferDirection = 7;
					g_pMainState->m_pFightState->SetDie(UserID);
					return TRUE;
				}
			}
		}
		return TRUE;
	case 4://�ȴ����Ƕ������
		if (m_pFightState->WaitPosEnd(UserID)||pUser->m_pShowData[0]->m_isStatic)
		{		
			switch (sSkillID)
			{
			case 7:
				
				{
					pUserList->m_SkillSelect.SkillID = 1017;
					stage  = 0;
				}
					return TRUE;
			case 50:
			
				{
					pUserList->m_SkillSelect.SkillID = 1050;
					stage = 0;
				}
				return TRUE;
			case 2095:
			
				{
					pUserList->m_SkillSelect.SkillID = 1095;
					stage = 0;
				}
				return TRUE;
			}
			pUser->Set(POS_FIGHT_STAND);
			pUser->SetStatic(FALSE);
			stage = 5;
		}
		return TRUE;
	case 5:StageWaitSleep(10, UserID, 6); break;
	case 6:
		
		{
			pUser->SetMagicSound(-1);//����
			pUserList->m_work.pop_back();
			
			if (g_pMainState->m_FightList[TargetID].isLive && (!g_pMainState->m_FightList[TargetID].CheckDie()))
				CoutComboMagic(pUserData, UserID, pTargetData);
		}
		break;
		
	}
	return TRUE;
}
BOOL cSkillManager::Cost(int& outhp, int Skilllv, int sSkillID, int UserID, int num, BOOL bCost)
{
	//return TRUE;//bug
	cObj* pObj= g_pMainState->m_FightList[UserID].m_pData;
	cPetData* pUserData;
	pObj->GetDateP(pUserData);

	int hp = pUserData->m_HP;
	int hpmax = pUserData->m_HPMax.GetData();
	int mp = pUserData->m_MP;
//	int mpmax = pUserData->m_MPMax.GetData();
	int cp = pUserData->m_CP;
	int hpsub = 0; 
	int mpsub = 0;
	int cpsub=0;

	if (!bCost)
	{
		switch (sSkillID)
		{
		case  0://��ɨ
			
			{
				if (hp > (hpmax >> 1))return TRUE;
				else return FALSE;
			}
			break;
		case 97: // ����
		
			{
				if (hp > ((hpmax * 70) / 100))return TRUE;
				else return FALSE;
			}
			break;
		case 141:  //ʨ�յļ���
		case 142:
		case 143:
		case 148:
			
			{
				if (!pObj->CheckHaveState(145))
					return FALSE;
			}
			break;
		}
	}
	if (sSkillID >= 3000)
	{
		cpsub=GetSpecialSkill(sSkillID-3000)->m_Cost;	
	}
	else
	switch (sSkillID)
	{
	case 0:hpsub = hp / 10; break;
	case 1:
	case 1010:
		hpsub = hp / 20; break;
	case 3:hpsub = hp / 20; break;
	case 6:mpsub = 50; break;
	case 1017:hpsub = hp / 2; break;
	case 11:mpsub = 30; break;
	case 13:mpsub = 30; break;
	case 14:mpsub = 40; break;
	case 15:mpsub = 40; break;
	case 16:mpsub = 40; break;
	case 17:mpsub = 60; break;
	case 18:mpsub = 60; break;
	case 19:mpsub = 40; break;
	case 20:mpsub = 60; break;
	case 21:mpsub = 70; break;
	case 22:mpsub = 1; break;
	case 23:hpsub = 1; break;
	case 25:mpsub = hp / 20; break;
	case 26:mpsub = 50;	break;
	case 27:mpsub = 100; break;
	case 28:mpsub = 30 * num; break;
	case 31:mpsub = 20 * num; break;
	case 33:mpsub = 30 * num; break;
	case 34:mpsub = 30 * num; break;
	case 35:mpsub = 30 * num; break;
	case 36:mpsub = 30 * num; break;
	case 37:mpsub = 150; break;
	case 38:mpsub = 50; break;
	case 39:mpsub = 40; break;
	case 40:mpsub = 30 * num; break;
	case 41:mpsub = 40; break;
	case 42:mpsub = 50 * num; break;
	case 43:mpsub = 30; break;
	case 44:mpsub = hp / 20; break;
	case 45:mpsub = 300; hp -= 1000; break;
	case 46:mpsub = 150; break;
	case 49:mpsub = 100; break;
	case 50:mpsub = 50; break;
	case 54:mpsub = 50; break;
	case 58:mpsub = hp / 20; break;
	case 59:mpsub = 60 * num; break;
	case 63:mpsub = 50; break;
	case 67:mpsub = 50; break;
	case 68:mpsub = 50; break;
	case 69:mpsub = 50; break;
	case 70:mpsub = hp / 20; break;
	case 71:mpsub = 30 * num; break;
	case 72:mpsub = 50; break;
	case 74:mpsub = 30 * num; break;
	case 76:mpsub = 50; break;
	case 77:mpsub = 80; break;
	case 79:mpsub = 50; break;
	case 80:mpsub = 25 * num; break;
	case 83:mpsub = hp / 20; break;
	case 84:mpsub = 50; break;
	case 88:mpsub = 100; break;
	case 89:mpsub = 50; break;
	case 90:mpsub = 50; break;
	case 91:mpsub = 30 * num; break;
	case 93:hpsub = hp / 20; break;
	case 94:hpsub = 100; break;
	case 97:mpsub = 45; break;
	case 102:mpsub = 50; break;
	case 103:mpsub = 50; break;
	case 104:mpsub = 150; break;
	case 105://�������з���
	case 106:
	case 107:
	case 108:
	case 109:mpsub = 50; break;
	case 110:hpsub = hp / 20; break;
	case 112:mpsub = 80 * num; break;
	case 115:mpsub = 10; break;
	case 116:mpsub = 25 * num; break;
	case 117:mpsub = 50; break;
	case 120:mpsub = 50 * num; break;
	case 122:mpsub = 40; break;
	case 123:mpsub = 150; break;
	case 124:hpsub = hp * 5 / 100; break;
	case 125:mpsub = 150; break;
	case 126:mpsub = 80 * num; break;
	case 127:mpsub = 80; break;
	case 129:mpsub = 50; break;
	case 131:mpsub = 30 * num; break;
	case 132:mpsub = 60; break;
	case 135:mpsub = 50; break;
	case 136:mpsub = 100; break;
	case 141:mpsub = 30; break;
	case 142:mpsub = 30 * num; break;
	case 143:mpsub = 30; break;
	case 144:mpsub = 40; break;
	case 145:mpsub = 20; break;
	case 148:mpsub = 80; break;
	case 149:hpsub = hp / 20; break;
	case 150:mpsub = 30; break;
	case 152:mpsub = 50; break;
	case 154:mpsub = 40; break;
	case 155:mpsub = 40; break;
	case 156:mpsub = 35; break;
	case 159:mpsub = 40; break;
	case 160:mpsub = 50; break;
	case 161:mpsub = 60; break;
	case 162:mpsub = 30 * num; break;
	case 163:mpsub = hp / 20; break;
	case 164:mpsub = 80; break;
	case 166:mpsub = 80; break;
	case 167:mpsub = 50; break;
		//�ķ�
	case 2084: mpsub = 30 * num; break;
	case 2085: mpsub = 30 * num; break;
	case 2086: mpsub = 30 * num; break;
	case 2087: mpsub = 30 * num; break;
	case 2088: mpsub = 30 * num; break;
	case 2089: mpsub = 30 * num; break;
	case 2090: mpsub = 30 * num; break;
	case 2091: mpsub = 30 * num; break;
		//
	case 2092: mpsub = 30; break;
	case 2093: mpsub = 80; break;
	case 2094: mpsub = 50; break;
	case 2095: mpsub = 50; break;
	case 2098: mpsub = 50; break;
	case 2099: mpsub = 50; break;
	case 2100: mpsub = 100; break;
	case 2101: mpsub = 300; break;
	default:
		return TRUE;
	}
	if (bCost)
	{
		if (hp < hpsub)return FALSE;
		if (mp < mpsub)return FALSE;
		if (cp < cpsub)
			return FALSE;
		if (hpsub)
		{
			hpsub = hpsub*(100 + pUserData->mpsavepercent) / 100;
			if (hpsub < 1)hpsub = 1;
			AddHp(UserID,-hpsub);
			SetDamage(hpsub, UserID);
		}
		if (mpsub)
		{
			if (pUserData->mpsavepercent>0)
			{
				mpsub = mpsub*(100 + pUserData->mpsavepercent) / 100;
				if (mpsub < 1)mpsub = 1;
			}
			AddMp(UserID, -mpsub);
		}
		if (cpsub)
		{
			AddCp(UserID, -cpsub);
		}
	}
	else
	{
		if (hp<hpsub)return FALSE;
		if (mp<mpsub)return FALSE;
		if (cp<cpsub)return FALSE;
	}
	return TRUE;
}

BOOL cSkillManager::Use50(int Skilllv, int sSkillID, int UserID, int TargetID)
{
	sFightList* pUserList = m_pFightState->m_pList[UserID];
	cObj* pUser = pUserList->m_pData;
	cObj* pTarget = m_pFightState->m_pList[TargetID]->m_pData;
	cPetData* pUserData;
	cPetData* pTargetData;
	pUser->GetDateP(pUserData);
	pTarget->GetDateP(pTargetData);
	int& stage = m_pFightState->m_pList[UserID]->GetWork()->stage;
	switch (stage)
	{
	case 0:
		pUser->Set(POS_MAGIC);
		pUser->SetSound(POS_MAGIC);
		stage = 1;
		return TRUE;
	case 1:
		if (WaitForMagicPoint(UserID))
		{
			DWORD magicid=0;
			switch (sSkillID)
			{
			case 50:magicid = 0x9D5F7277; break;
			case 2095:magicid = 0xFF33B0EE; break;
			default:break;
			}	
			m_pFightState->m_pList[TargetID]->SetMagiSufferID(magicid, -10, &m_pFightState->m_FullSkill.m_SufferMagic[TargetID], TRUE);
			pUser->SetMagicSound(sSkillID);//����
			stage += 1;
			break;
		}
		return TRUE;
	case 2:
		if (m_pFightState->WaitPosEnd(UserID))
		{
			pUser->Set(POS_FIGHT_STAND);
		}
		if (!pTarget->m_MagicSuffer.m_NeedShow)
		{
			stage += 1;
		}
		return TRUE;
	case 3:
		if (m_pFightState->Approach(UserID, TargetID))
		{
			pUser->Set(POS_FIGHT_STAND);

			//�������
			pTargetData->m_FightProperty.m_bHit = TRUE;
			CountParry(pUserData, pTargetData);//����
			CountCri(pUserData, pTargetData);//��ɱ
			//������˱���,�ȴ������߾�λ
			if (pTargetData->m_FightProperty.m_guardID.size())
			{
				stage = 101;
				return TRUE;
			}
			stage = 20;
		}
		return TRUE;
	case 20:
		StageWaitSleep(10, UserID, 4);
		return TRUE;
	case 4:
		
		{
			int pos;
			if (rand() % 2)
				pos = POS_ATTACK2;
			else
				pos = POS_ATTACK;
			pUser->Set(pos);
			pUser->SetSound(pos);
			stage = 120;
		}
	case 120:

		if (m_pFightState->WorkAttack(UserID))
		{
			stage = 5;
		}
		return FALSE;
	case 5:  //�����ж�
		
		{
			stage += 1;
			int dam;
			int magic2= 0xACA4A54A;
			if (CountCri(pUserData,pTargetData))
			{
				magic2 = 0xECD0E003;
			}

			switch (sSkillID)
			{
			case 50:
				CountphysicalDamage(dam, UserID, TargetID, 30);
				break;
			case 2095://����
				CountphysicalDamage(dam, UserID, TargetID, 30);
				break;
			}
			
			int MagicPos;
			//��	 
			if (pTargetData->m_FightProperty.m_bDefend)
			{
				dam /= 2;
				magic2 = 0x3622636F;
				MagicPos = 0;
				m_pFightState->SetDefend(TargetID, pUser->GetDirection(),UserID);
			}
			else
			{
				MagicPos = 0;
				m_pFightState->SetSuffer(TargetID, pUser->GetDirection(),UserID);
			}
			m_pFightState->m_pList[TargetID]->SetMagiSufferID(magic2, MagicPos, &m_pFightState->m_FullSkill.m_SufferMagic[TargetID]);
			CountProtect(dam, pUser, pTarget);//�����˺�����
			AddHp(TargetID, -dam);
			SetDamage(dam, TargetID);
			if (sSkillID==50)
			if (rand() % 2)
			{
				sBuff buf;
				buf.times = 1 + rand() % 3;
				SetDateBuffUp(UserID, TargetID, sSkillID, buf, Skilllv); //50%���ж���
			}
			
		}
		return  FALSE;
	case 6:
		if (m_pFightState->WaitPosEnd(UserID) || (pUser->m_NowPos == POS_FIGHT_STAND))
		{
			pUser->Set(POS_FIGHT_STAND);
			stage += 1;
		}
		return TRUE;
	case 7:  //����
		if (m_pFightState->ReturnApproach(UserID))
		{
			
			pUserList->m_work.pop_back();
			return TRUE;
		}
		return FALSE;
	case 101: //�ȴ������߾�λ
		return StageWaitProtect(TargetID, 20);

	}
	return TRUE;
}

BOOL cSkillManager::Use71(int Skilllv, int sSkillID, int UserID, int TargetID)
{
	sFightList* pUserList = m_pFightState->m_pList[UserID];
	cObj* pUser = pUserList->m_pData;
	cObj* pTarget = m_pFightState->m_pList[TargetID]->m_pData;
	cPetData* pUserData;
	cPetData* pTargetData;
	pUserList->m_pData->GetDateP(pUserData);
	pTarget->GetDateP(pTargetData);
	int& stage = pUserList->GetWork()->stage;
	switch (stage)
	{
	case 0:
		
		{
			switch (sSkillID)
			{
			case 1050:  //���컨��
			case 1095://����
			case 0://��ɨǧ��
			case 97://���꽣��
			case 141://����
			case 143:// ʨ��
			case 148://������
			case 1010://��
			case 2092://����һ��
			case 2094://�����ٻ�
			case 2098://�ƶ��б�
			case 2099://���ݻ���
			case 2100://���ŭ��
			case 3034://��Ѫ
			case 3035://����
			case 3040://����
			case 3042://��Ѫ2
			case 3043://����EX
			case 3044://��ѪEX
				
				{
					switch (sSkillID)
					{
					case 0:pUserList->m_MaxTargetNum = 3; break;  //��ɨǧ��
					case 97:pUserList->m_MaxTargetNum = 2; break;//���꽣��
					case 148:
						
						{
							if (Skilllv < 30)pUserList->m_MaxTargetNum = 1;
							else
							if (Skilllv < 50)pUserList->m_MaxTargetNum = 2;
							else
							if (Skilllv < 70)pUserList->m_MaxTargetNum = 3;
							else
							if (Skilllv < 105)pUserList->m_MaxTargetNum = 4;
							else
							if (Skilllv < 140)pUserList->m_MaxTargetNum = 5;
							else
								pUserList->m_MaxTargetNum = 6;
						}
						break;
					case 3034:pUserList->m_MaxTargetNum = 2; break;//��Ѫ
					case 1050:  //���컨��
					case 1095://����
					case 1010://��
					case 2092://����һ��
					case 2094://�����ٻ�
					case 2098://�ƶ��б�
					case 2099://���ݻ���
					case 2100://���ŭ��
					case 3040://����
					case 3043://����EX
					case 3035:
						pUserList->m_MaxTargetNum = 1; break;//����		
					case 3042:pUserList->m_MaxTargetNum = 3; break;//��Ѫ2
					case 3044:pUserList->m_MaxTargetNum = 4; break;//��ѪEX
					default:pUserList->m_MaxTargetNum = 1;
						break;
					}
					if (TargetID == UserID)
					{
						pUserList->m_work.clear();
						return TRUE;
					}
					pUserList->m_MaxTargetNum += pUserData->m_MagicNumAdd;
					for (int i = 0; i < pUserList->m_MaxTargetNum; i++)
					{
						pUserList->targetlist[i] = TargetID;
					}
				}
				break;
			case 1017://�Ƹ�
			case 142://ӥ��
			case 71://����ն
				
				{
					int maxnum = 0;
					switch (sSkillID)
					{
					case 1017:
						maxnum = 3;
						break;
					case 142://ӥ��
					
						{
							 maxnum = Skilllv / 30 + 1;
							if (maxnum > 6)maxnum = 6;
						}
						break;
					case 71://����ն
					
						{
							if (g_pMainState->m_AI.m_AttackAI.m_bPKMode || TargetID<10)
							{
								maxnum = 1;
							}
							else
							{
								if (Skilllv > 59)maxnum = 3;
								else 	if (Skilllv > 29)maxnum = 2;
								else  	maxnum = 1;
							}
						}
						break;
					}
					maxnum += pUserData->m_MagicNumAdd;
					pUserList->m_MaxTargetNum = 0;
					for (int i = 0; i < maxnum; i++)
					{
						pUserList->targetlist[pUserList->m_MaxTargetNum] = GetTargetID(UserID, TargetID, i);
						int id = pUserList->targetlist[pUserList->m_MaxTargetNum];
						if (id == UserID)continue;
						if (-1 == id)break;
						pUserList->m_MaxTargetNum += 1;
					}
					
				}
				break;
			default:
				ERRBOX;
				break;
			}
		}
		pUserList->m_NowTarget = -1;
		if (!pUserList->NextTarget(m_pFightState, sSkillID))stage = 7;
		else
		stage = 1;
		return TRUE;
	case 1:
		if (pUserList->m_NowTarget != -1)
		{
		
			if (pUserList->m_NowTarget == 0)
			{
				pUserList->targetID = pUserList->targetlist[pUserList->m_NowTarget];
				pUserList->m_pData->SetMagicSound(sSkillID);
			}
			else
			{
				if (pUserList->targetlist[0] != pUserList->targetlist[1])
				{
					pUserList->targetID = pUserList->targetlist[pUserList->m_NowTarget];
					pUserList->m_pData->SetMagicSound(sSkillID);
				}
			}
			stage = 22;
		}
		else stage = 7;
		return TRUE;
	case 22:	
		
		{
			sFightOffset* pOffse = g_pMainState->m_FightList[UserID].GetFightOffset();
			if (pOffse->m_xOff > 0)
			{
				if (pUserList->m_NowTarget > 0)
				{
					if (pUserList->m_NowTargetID == pUserList->targetlist[pUserList->m_NowTarget - 1])
					{
						stage = 23;//Ŀ����ͬ�����ƶ�
						return TRUE;
					}
				}
				stage = 2;
			}
			else
			{
				stage = 23;
			}
		}
		return TRUE;	
	case 2:
		if (m_pFightState->Approach(UserID, pUserList->m_NowTargetID))
		{
			stage = 23;
		}
		return TRUE;
	case 23:
		
		{
			m_pFightState->m_pList[pUserList->m_NowTargetID]->m_pData->GetDateP(pTargetData);
			//�������
			CheckHit(pUserData, pTargetData,20);
			pUserList->m_pData->Set(POS_FIGHT_STAND);
			CountParry(pUserData, pTargetData);//��
			CountCri(pUserData, pTargetData);//��ɱ

			//������˱���,�ȴ������߾�λ
			if (pTargetData->m_FightProperty.m_guardID.size())
			{
				stage = 101;
				return TRUE;
			}
			stage = 20;
		}
		return TRUE;
	case 20: return StageWaitSleep(10, UserID, 3);//�ȴ�
	case 3:
		
		{
			int pos;
			if (rand() % 2)
				pos = POS_ATTACK2;
			else
				pos = POS_ATTACK;
			pUserList->m_pData->Set(pos);
			pUserList->m_pData->SetSound(pos);
			stage = 120;
		}
	case 120:
		
		{
			int atkp = m_pFightState->WorkAttack(UserID);
			switch (atkp)
			{
			case 0:return TRUE;
			default:
				
				{

					//�ǲ��ǹ���
					sFightOffset* pOffse = g_pMainState->m_FightList[UserID].GetFightOffset();
					if (pOffse->m_xOff == 0)
					{
						m_pFightState->m_HiddenWeapons.Load(0xE2BC8127);
						m_pFightState->m_HiddenWeaponsUseNum = 1;
						m_pFightState->m_HiddenWeaponsp[0].m_NeedShow = TRUE;
						m_pFightState->m_HiddenWeaponsp[0].m_x = pUser->GetX();
						m_pFightState->m_HiddenWeaponsp[0].m_y = pUser->GetY() + 20;
						m_pFightState->m_HiddenWeaponsTargetID[0] = pUserList->m_NowTargetID;
						stage = 24;
						return TRUE;
					}
					if (!PhysicalAtkHitStage(UserID, pUserList->m_NowTargetID, sSkillID))
					{
						stage = 4;
						return TRUE;
					}
					if (atkp != 1)
					{
						stage = 4;
					}
				}
				break;
			}
		}
		return TRUE;
	case 24://����
		
		{
			if (m_pFightState->WaitPosEnd(UserID))
			{
				pUser->Set(POS_FIGHT_STAND);
			}
			if (m_pFightState->HiddenWeaponsMove(0, 50))
			{
				PhysicalAtkHitStage(UserID, pUserList->m_NowTargetID, sSkillID);
				//if (PhysicalAtkHitStage(UserID, pUserList->m_NowTargetID, sSkillID))
					/*{
						stage = 4;
						}
						else
						{
						stage = 5;
						}*/
				stage = 4;
			}
		}
		return TRUE;
	case 4:  //�����ж�

	{
				 if (m_pFightState->WaitPosEnd(UserID))
				 {
					 pUser->Set(POS_FIGHT_STAND);
				 }
				 stage = 5;//����÷���effectskill֮ǰ,��Ϊ�����������,stage����ʧЧ
				 cPetData* pPet;
				 g_pMainState->m_FightList[pUserList->m_NowTargetID].m_pData->GetDateP(pPet);
				 if (pPet->m_FightProperty.m_bHit)
					 EffectSkill(Skilllv, sSkillID, UserID, pUserList->m_NowTargetID);

				 g_pMainState->m_FightList[pUserList->m_NowTargetID].m_pData->GetDateP(pTargetData);
				 if (pTargetData->m_HP < 1)
				 {
					 pUserList->m_pData->SetSpeed(15);
				 }

		
			if (!pUserList->NextTarget(m_pFightState, sSkillID))
			{
				int hpout;
				Cost(hpout, Skilllv, sSkillID, UserID, pUserList->m_MaxTargetNum, TRUE);
				switch (sSkillID)
				{
				case 142://ӥ��	
					{
						sBuff buf;
						buf.times = 1;
						SetDateBuffUp(UserID, UserID, sSkillID, buf);
					}
					break;
				case 0: //��ɨ 
				case 1017:
				{
						  sBuff buf;
						  buf.times = 1;
						  SetDateBuffUp(UserID, UserID, 0, buf);
				}
					break;
				}
			}
		}
		return  TRUE;
	case 5:
		if (m_pFightState->WaitPosEnd(UserID) || pUser->m_NowPos == POS_FIGHT_STAND)
		{

			if (pUserList->m_NowTargetID == -1)
			{
				pUserList->m_pData->Set(POS_FIGHT_STAND);
				stage = 7;
				return TRUE;
			}
			if (sSkillID == 142)//ӥ��Ҫ�ܻ�ȥ
				stage = 8;
			else
				stage = 1;

		}
		return TRUE;
	case 7:	return StageWaitReturn(UserID, -1);
	case 8:	return StageWaitReturn(UserID, 1);
	case 101:  return StageWaitProtect(TargetID, 3);
	}
	return TRUE;
}

BOOL cSkillManager::Use80(int Skilllv, int sSkillID, int UserID, int TargetID)
{
	sFightList* pUserList = m_pFightState->m_pList[UserID];
	cObj* pUser = pUserList->m_pData;
	cPetData* pUserData;
	pUser->GetDateP(pUserData);
	int& stage = pUserList->GetWork()->stage;
	switch (stage)
	{
	case 0:
		pUser->Set(POS_MAGIC);
		pUser->SetSound(POS_MAGIC);
		while (!m_pFightState->m_ThreadSleep)
		{
			Sleep(0);  //�ȴ��߳�
		}
		switch (sSkillID)
		{
		case 80:
			m_pFightState->m_IDtoLoad[0] = 0xA19EAB31;  //��
			m_pFightState->m_IDtoLoad[1] = 0xF1F26922;  //��
			m_pFightState->m_IDtoLoad[2] = 0xD61E2519;  //����
			break;
		case 131://��ɳ��ʯ
			m_pFightState->m_IDtoLoad[0] =  0x4DB67680;
			m_pFightState->m_IDtoLoad[1] = 0x49CBD108;
			break;
		default:
			ERRBOX;
			break;
		}
		m_pFightState->m_pFileToLoad[0] = &m_pFightState->m_FullSkill.m_FullSkilUP[0];
		m_pFightState->m_pFileToLoad[1] = &m_pFightState->m_FullSkill.m_FullSkilUP[1];
		m_pFightState->m_pFileToLoad[2] = &m_pFightState->m_FullSkill.m_FullSkilUP[2];
		ResumeThread(m_pFightState->m_hThread);
		stage = 1;
		return TRUE;
	case 1:
		if (WaitForMagicPoint(UserID))
		{
			pUser->SetMagicSound(sSkillID);

			while (!m_pFightState->m_ThreadSleep)
			{
				Sleep(1);  //�ȴ��߳�
			}
			int speed = GetSpeed(sSkillID);
			for (int i = 0; i < 3; i++)
				m_pFightState->m_FullSkill.m_FullSkilUP[i].m_Picture.m_FrameGap = speed;

			//DWORD FullMagicID = 0;
			//DWORD FullMagicID2 = 0;
			//int Position = 1;
			int pos;
			int maxtarget;
			DWORD magicid;
			BOOL upordown;
			BOOL toall;
			GetMagicAndNum(Skilllv, sSkillID, UserID, TargetID, magicid, maxtarget, pos, upordown, toall);
			
			for (int i = 0; i < 2; i++)
				m_pFightState->m_FullSkill.m_FullSkilUP[i].m_Picture.m_FrameGap =  GetSpeed(sSkillID);
			pUserList->m_MaxTargetNum = 0;
			int id;
			for (int i = 0; i < maxtarget; i++)
			{
				pUserList->targetlist[pUserList->m_MaxTargetNum] = GetTargetID(UserID, TargetID, i);
				id= pUserList->targetlist[pUserList->m_MaxTargetNum];
				if (id == UserID)continue;
				if (id != -1)
				{
					pUserList->m_NowTargetID = pUserList->targetlist[pUserList->m_MaxTargetNum];
					m_pFightState->SetMagicSuffer(id, pUser->GetDirection());
					m_pFightState->m_pList[id]->m_bSufferFullSkill = TRUE;
					pUserList->m_MaxTargetNum += 1;
				}
				else break;
			}
			stage += 1;
		}
		return TRUE;
	case 2:
		
		{	
			DWORD id[3];
			int x1, y1;
			x1 = m_pFightState->m_pList[TargetID]->m_pData->GetX();
			y1 = m_pFightState->m_pList[TargetID]->m_pData->GetY();
			if (m_pFightState->m_IDtoLoad[1] != 0)Sleep(0);//�ȴ��߳�
			switch (sSkillID)
			{

			case 80:
				id[0] = 0xA19EAB31;  //�� 
				id[1] = 0xD61E2519;  //����
				id[2] = 0xF1F26922;  //��
				m_pFightState->m_FullSkill.SetMagicUp(0, id[0], x1, y1);
				m_pFightState->m_FullSkill.SetMagicUp(1, id[1], x1, y1);
				if (m_pFightState->m_IDtoLoad[2] != 0)Sleep(0);//�ȴ��߳�
				m_pFightState->m_FullSkill.SetMagicUp(2, id[2], x1, y1);
				break;
			case 131://��ɳ��ʯ
				id[0] =  0x4DB67680;
				id[1] = 0x49CBD108;
				m_pFightState->m_FullSkill.SetMagicUp(1, id[1], x1,y1);
				m_pFightState->m_FullSkill.SetMagicUp(0, id[0],x1-100,y1-120);
				break;
			default: ERRBOX;
			}
			stage += 1;
		}
		return TRUE;
	case 3:
		
		{
			if (m_pFightState->WaitPosEnd(UserID))
			{	
				pUser->Set(POS_FIGHT_STAND);
			}
			DWORD MagicID = 0;
		//	BOOL toAll = 0;
			int LateTime;
			GetDelayAnd(sSkillID, LateTime, MagicID);
		
		//	int nowframe = m_pFightState->m_FullSkill.m_FullSkilUP[0].GetNowFrame();
//			int maxframe = m_pFightState->m_FullSkill.m_FullSkilUP[0].GetMaxFrame();
			if (m_pFightState->m_FullSkill.m_FullSkilUP[0].GetNowFrame() == m_pFightState->m_FullSkill.m_FullSkilUP[0].GetMaxFrame()-LateTime)
			{
				//�˺��ж�
				for (int i = 0; i < pUserList->m_MaxTargetNum; i++)
				{
					int id = pUserList->targetlist[i];
					if (id == -1)
					{
						stage += 1;
						return TRUE;
					}
					if (CheckMagicHit(Skilllv, sSkillID, UserID, id))
					{
						EffectSkill(Skilllv, sSkillID, UserID, id);
						
					}
					m_pFightState->SwitchMagicSuffer(id);
				}
				int dustbin;
				Cost(dustbin, Skilllv, sSkillID, UserID, pUserList->m_MaxTargetNum, TRUE);
				stage += 1;
			}

		}
		return TRUE;
	case 4:
		if (m_pFightState->WaitPosEnd(UserID) || (pUser->m_NowPos == POS_FIGHT_STAND))
		{
			pUser->Set(POS_FIGHT_STAND);
			pUserList->m_work.pop_back();
			for (int i = 0; i < 3; i++)
				m_pFightState->m_FullSkill.m_FullSkilUP[i].m_NeedShow = FALSE;

			
			if (1 > pUserData->m_HP)
			{
				if (UserID < 10)
					pUserList->m_sufferDirection = 3;
				else pUserList->m_sufferDirection = 7;
				g_pMainState->m_pFightState->SetDie(UserID);
				return TRUE;
			}

			cPetData* pTargetdata;
			m_pFightState->m_pList[TargetID]->m_pData->GetDateP(pTargetdata);
			CoutComboMagic(pUserData, UserID, pTargetdata);
		}
		return TRUE;
	}
	return TRUE;

}

BOOL cSkillManager::Use84(int Skilllv, int sSkillID, int UserID, int TargetID)
{//����
	sFightList* pUserList = m_pFightState->m_pList[UserID];
	cObj* pUser = pUserList->m_pData;
	cObj* pTarget = m_pFightState->m_pList[TargetID]->m_pData;
	int& stage = pUserList->GetWork()->stage;
	switch (stage)
	{
	case 0:
		
		{
			int hpout = 0;
			Cost(hpout, Skilllv, sSkillID, UserID, 1, TRUE);
			if (hpout)
			{
				SetDamage(hpout, UserID);
			}

			pUser->Set(POS_MAGIC);
			pUser->SetSound(POS_MAGIC);
			while (!m_pFightState->m_ThreadSleep)
			{
				Sleep(1);  //�ȴ��߳�
			}

			m_pFightState->m_pFileToLoad[0] = &m_pFightState->m_FullSkill.m_SufferMagic[TargetID];
			m_pFightState->m_IDtoLoad[0] = 0x5EC2D5BB;
			m_pFightState->m_pFileToLoad[1] = &m_pFightState->m_FullSkill.m_FullSkilUP[0];
			m_pFightState->m_IDtoLoad[1] = 0x9FE48849;
			m_pFightState->m_pFileToLoad[2] = &m_pFightState->m_FullSkill.m_FullSkilUP[1];
			m_pFightState->m_IDtoLoad[2] = 0x63605C8D;
			for (int i = 0; i < 3; i++)
				m_pFightState->m_pFileToLoad[i]->m_Picture.m_FrameGap = 2;
			ResumeThread(m_pFightState->m_hThread);
			stage = 1;
		}
		return TRUE;
	case 1:
			if (WaitForMagicPoint(UserID))
			{
				pUser->SetMagicSound(sSkillID);
				if (m_pFightState->m_IDtoLoad[0] != 0)Sleep(1);//�ȴ��߳�
				int speed = GetSpeed(sSkillID);
				for (int i = 0; i < 3; i++)
					m_pFightState->m_FullSkill.m_FullSkilUP[i].m_Picture.m_FrameGap = speed;

				pUserList->m_MaxTargetNum = 0;
				int id;
				for (int i = 0; i < 10; i++)
				{
					id = GetTargetID(UserID, TargetID, i);
					if (UserID == id)continue;
					if (-1 == id)break;
					pUserList->targetlist[pUserList->m_MaxTargetNum] = id;
					m_pFightState->SetMagicSuffer(id, pUser->GetDirection());
					m_pFightState->m_pList[id]->SetMagiSufferID(0x5EC2D5BB, 2, &m_pFightState->m_FullSkill.m_SufferMagic[TargetID]);
					m_pFightState->m_pList[id]->m_bSufferFullSkill = TRUE;
					pUserList->m_MaxTargetNum += 1;
				}
				pUserList->m_NowTarget = -1;
				for (int i = 0; i < 3; i++)
					m_pFightState->m_FullSkill.m_FullSkilUP[i].m_NeedShow = FALSE;
				stage += 1;
			}
		return TRUE;
	case 2:
		if (pTarget->m_MagicSuffer.GetFrame()>=4)
		{
			if (TargetID>9)
			{
				m_pFightState->m_FullSkill.SetMagicUp(0, 0x63605C8D, 300, 150);
				m_pFightState->m_FullSkill.SetMagicUp(1, 0x9FE48849, 230, 200);
			}
			else
			{
				m_pFightState->m_FullSkill.SetMagicUp(0, 0x63605C8D, 400, 250);
				m_pFightState->m_FullSkill.SetMagicUp(1, 0x9FE48849, 430, 300);
			}
			stage += 1;
		}
		return TRUE;
	case 3:
		if (m_pFightState->WaitPosEnd(UserID))
		{
			pUser->Set(POS_FIGHT_STAND);
		}
		if (m_pFightState->m_FullSkill.m_FullSkilUP[0].GetNowFrame() == 3)
		{
			cPetData* data;
			pUser->GetDateP(data);
		//	int dam=Skilllv+data->m_LV/2;
			//�˺��ж�
			while (pUserList->NextTarget(m_pFightState, sSkillID))
			{
				EffectSkill(Skilllv, sSkillID, UserID,pUserList->m_NowTargetID );
				m_pFightState->m_pList[pUserList->m_NowTargetID]->m_pData->m_MagicSuffer.m_NeedShow = FALSE;
				m_pFightState->SwitchMagicSuffer(pUserList->m_NowTargetID);
			}
			stage += 1;
		}
		return TRUE;
	case 4:
		if (m_pFightState->WaitPosEnd(UserID) || (pUser->m_NowPos == POS_FIGHT_STAND))
		{
			pUser->Set(POS_FIGHT_STAND);
			pUserList->m_work.pop_back();
			cPetData* pPet;
			pUserList->m_pData->GetDateP(pPet);
			if (1 > pPet->m_HP)
			{
				if (UserID < 10)
					pUserList->m_sufferDirection = 3;
				else pUserList->m_sufferDirection = 7;
				g_pMainState->m_pFightState->SetDie(UserID);
				return TRUE;
			}
		}
		return TRUE;
	}
	return TRUE;
}


BOOL cSkillManager::CheckFightState(int FightListID)
{
	sFightList* pTargetList = &g_pMainState->m_FightList[FightListID];
	cObj* pTarget = m_pFightState->m_pList[FightListID]->m_pData;
	cPetData* pTargetData;
	pTarget->GetDateP(pTargetData);
	if (!pTargetList->m_pData->m_StateList.size())return TRUE;
	sBuff buf;
	for (list<sBuff>::iterator Iterator = pTarget->m_StateList.begin(); Iterator != pTarget->m_StateList.end();)
	{
		Iterator->times -= 1;
		if (Iterator->times < 0)
		{
			list<sBuff>::iterator it2 = Iterator;
			Iterator++;
			g_pMainState->m_SkillManager.SetDateBuffDown(it2);
			it2->data.Free();
			pTargetList->m_pData->m_StateList.erase(it2);
			continue;
		}
		if (pTargetList->CheckDie())
			return TRUE;
		switch (Iterator->ID)
		{
		case 1://��
			
			{
				pTargetList->m_SkillSelect.Lv = pTarget->GetSkillLv(1);
				pTargetList->m_SkillSelect.SkillID = 1010;
				pTargetList->m_SkillSelect.TargetID = Iterator->userID;
				pTargetList->targetID = Iterator->userID;
				pTargetList->m_WorkType = WORKTYPE_MAGIC;
			}
			break;
		case 6: //�����
			
			{
				int autoSelect = g_pMainState->m_AI.AutoFindTarget(FightListID, 3);
				if (-1 != autoSelect)
				{
					m_pFightState->m_pList[FightListID]->m_WorkType = WORKTYPE_ATTACK;
					m_pFightState->m_pList[FightListID]->m_SkillSelect.Lv = 0;
					m_pFightState->m_pList[FightListID]->m_SkillSelect.SkillID = 1000;
					m_pFightState->m_pList[FightListID]->targetID = autoSelect;
					m_pFightState->m_pList[FightListID]->m_SkillSelect.TargetID = autoSelect;
				}
			}
			break;
		case 50://��
			
			{
				cPetData* petdata;
				g_pMainState->m_FightList[FightListID].m_pData->GetDateP(petdata);
				int dam = petdata->m_HP / 20;
				if (dam > (petdata->m_LV + 10) << 4)
					dam = (petdata->m_LV + 10) << 4;
				if (Iterator->damage)
					dam += (dam*Iterator->damage) / 100;
				SetDamage(-AddHp(FightListID, -dam), FightListID);
				if (petdata->m_HP<1)
					g_pMainState->m_pFightState->SetDie(FightListID);
			}
			break;
		case 122://ʬ����
			
			{
				cPetData* petdata;
				g_pMainState->m_FightList[FightListID].m_pData->GetDateP(petdata);
				int dam = petdata->m_HP / 20;
				if (dam >(petdata->m_LV + 10) << 4)
					dam = (petdata->m_LV + 10) << 4;
				dam += (dam*Iterator->damage) / 100;
				SetDamage(-AddHp(FightListID, -dam), FightListID);
				AddHpHurt(FightListID, (dam>>3));
				if (petdata->m_HP < 1)
					g_pMainState->m_pFightState->SetDie(FightListID);
			}
			break;
		}	
		Iterator++;
	}
	return TRUE;
}

BOOL cSkillManager::SetDateBuffUp(int userID, int targetID, int stateid, sBuff& buf,int skilllv)
{
	if (g_StateType != STATE_FIGHT)return TRUE;
	if (buf.times < 1)buf.times = 1;
	int MagicPos = 0;
	DWORD MagicID = 0; //BUFF ��ID
	BOOL MagicUpOrDown = FALSE; //����BUFF ��������ʾ����ʾ
//	int  magicType = 1; //Ĭ�Ϸ������� ��ӡ
	cObj* pTarget = m_pFightState->m_pList[targetID]->m_pData;
	cPetData* pTargetData;
	pTarget->GetDateP(pTargetData);
	sFightData& TargetFightP = pTargetData->m_FightProperty;
	//eBuffType EffectType = eBuffType��ӡ;
	

	for (list<sBuff>::iterator it = pTarget->m_StateList.begin(); it != pTarget->m_StateList.end(); it++)
	{
		if (it->ID == stateid)
		{
			it->times = buf.times;
			return TRUE;
		}
	}
	switch (stateid)
	{
	case 0: //��ɨǧ��
	case 7: //�Ƹ�����
		MagicID = 0xD9463A0C;
		TargetFightP.cannotAll += 1;
		TargetFightP.cannotAttack += 1;
		TargetFightP.cannotMagic += 1;
		TargetFightP.cannotSkill += 1;
		TargetFightP.cannotUseItem += 1;
		MagicPos = -10;
		break;
	case 1://������
		MagicID = 0x2A2A0663;
		pTargetData->m_SPD.AddDataOriginal(10000);
		pTargetData->m_Pierce.d1 += 20;
		pTargetData->m_CriRate.d1 += 10;
		MagicPos = -10;
		break;	
	case 4://ɱ����
		MagicUpOrDown = TRUE;
		MagicID = 0x62EDD1CB;
		buf.damage = skilllv * 3;
		pTargetData->m_HIT.AddDataOriginal(buf.damage);
		pTargetData->m_ATK.AddDataOriginal(buf.damage/4);
		break;
	case 6://�����
		m_pFightState->m_pList[targetID]->m_pData->SetShake(TRUE);
		TargetFightP.chaos += 1;
		break;
	case 8://�����
		MagicPos = -10;
		MagicUpOrDown = TRUE;
		MagicID = 3860707508;
		pTargetData->m_DamageAddPercentMagic.d2 += 30;
		break;
	case 14://����
		MagicPos = -10;
		TargetFightP.cannotAll += 1;
		TargetFightP.cannotAttack += 1;
		TargetFightP.cannotMagic += 1;
		TargetFightP.cannotSkill += 1;
		TargetFightP.cannotUseItem += 1;
		MagicID = 3398426285;
		break;
	case 15://ʧ��
		MagicPos = -10;
		TargetFightP.cannotMagic += 1;
		MagicID = 3635978625;
		break;
	case 16://����
		MagicPos = -10;
		TargetFightP.cannotMagic += 1;
		MagicID = 0x8B43833D;
		break;
	case 17://ʧ��
		MagicPos = -10;
		TargetFightP.cannotAttack += 1;
		TargetFightP.cannotMagic += 1;
		TargetFightP.cannotSkill += 1;
		MagicID = 3946646209;
		break;
	case 18://׷��
		MagicPos = -10;
		pTargetData->m_DEF.AddDataAddPercent(-20);
		TargetFightP.cannotAttack += 1;
		MagicID = 3655438388;
		break;
	case 19://���
		MagicPos = -10;
		TargetFightP.cannotMagic += 1;
		MagicID = 1214058823;
		break;
	case 20://ʧ��   
		MagicPos = -10;
		TargetFightP.cannotMagic += 1;
		MagicID = 2336457533;
		break;
	case 21://���� 
		MagicPos = -10;
		TargetFightP.cannotAll += 1;
		TargetFightP.cannotAttack += 1;
		TargetFightP.cannotMagic += 1;
		TargetFightP.cannotSkill += 1;
		MagicID = 2461182746;
		break;
	case 26:
		pTarget->SetShadow(50);
		break;
	case 27: //��׷�
		pTargetData->m_DEF.AddDataAddPercent(-20);
		MagicID = 0x5525C5FB;
		MagicUpOrDown = TRUE;
		MagicPos = 10;
		break;
	case 33://��ջ���
		pTarget->SetTwinkle(TRUE); 
		pTargetData->m_ATK.AddDataAddPercent(20);
		break;
	case 34: //Τ�ӻ���  
		pTargetData->m_DamageAddPercentMagic.d2 += 20;
		MagicID = 0xF184BD59;
		MagicPos = -10;
		MagicUpOrDown = TRUE;
		break;
	case 35:  //��ջ���
		pTargetData->m_DEF.AddDataAddPercent(20);
		MagicID = 916220457;
		MagicUpOrDown = TRUE;
		break;
	case 36://һέ�ɽ�
		MagicID = 382287583;
		MagicPos = -10;
		pTargetData->m_SPD.AddDataAddPercent(20);
		break;
	case 46:    //���ޱ�
		pTargetData->m_MagicComboRate.d1 += 30;
		MagicID = 3930943562;
		MagicPos = -10;
		MagicUpOrDown = TRUE;
		break;
	case 49:    //��������
		MagicID = 0x72AD6DBA;
		MagicPos = -10;
		MagicUpOrDown = TRUE;
		break;
	case 50: //���컨���ж�
		MagicID = 0xDBCEE32C;
		MagicPos = -10;
		MagicUpOrDown = 1;
		break;
	case 54://��������
		TargetFightP.cannotAttack += 1;
		TargetFightP.cannotMagic += 1;
		MagicID = 0x59D77EF9;
		MagicPos = -10;
		break;
	case 59://һЦ���
		TargetFightP.cannotMagic += 1;
		MagicID = 0x1E1AF8D9;
		MagicPos = -10;
		break;
	case 63://���񻤷�
		break;
	case 67:	//����
		pTargetData->m_ATK.AddDataAddPercent(-30);
		MagicID = 0x98753F94;
		MagicPos = -10;
		break;
	case 68:	//����
		pTargetData->m_HIT.AddDataAddPercent(-20);
		TargetFightP.cannotMagic += 1;
		MagicID = 0x2F78C46E;
		MagicUpOrDown = TRUE;
		MagicPos = -10;
		break;
	case 69:	//�������
		TargetFightP.cannotAttack += 1;
		MagicID = 0xC48ABA7B;
		MagicPos = -10;
		break;
	case 81:// ����
		pTarget->SetTwinkle(TRUE);
		buf.damage = skilllv * 2;
		pTargetData->m_ATK.SetDataAdd(buf.damage);
		break;
	case 89:  //����Ǭ��
		TargetFightP.cannotAttack += 1;
		TargetFightP.cannotMagic += 1;
		TargetFightP.cannotSkill += 1;
		MagicID = 0x313BCC79;
		MagicUpOrDown = 1;
		MagicPos = -10;
		break;
	case 90:   //��������
		MagicID = 0x719D767F;
		MagicUpOrDown = 1;
		MagicPos = 10;
		buf.damage = skilllv+10;
		pTargetData->m_FightProperty.m_TurnMPAdd += buf.damage;
		AddMp(targetID, buf.damage);
		break;
	case 91:    //����֮Ȫ
		MagicID = 0xF8EE853A;
		MagicUpOrDown = 1;
		MagicPos = 10;
		buf.damage = skilllv *3+ 10;
		pTargetData->m_FightProperty.m_TurnHPAdd += buf.damage;
		AddHp(targetID, buf.damage);
		break;
	case 102: //�ն�����
		MagicID = 0xD8857128;
		MagicUpOrDown = 1;
		MagicPos = 10;
		buf.damage = skilllv*4+15;
		buf.damage2 = buf.damage;
		pTargetData->m_FightProperty.m_TurnHPAdd += buf.damage; 
		pTargetData->m_FightProperty.m_TurnHPHurtAdd += buf.damage2;
		break;
	case 103:// ������
		MagicID = 0xEFE73EB9;
		MagicUpOrDown = 1;
		MagicPos = 10;
		pTargetData->m_FightProperty.m_TurnHPHurtAdd -= buf.damage;
		break;
	case 112:// �鶯����
		MagicID = 0x95FF4460;
		MagicUpOrDown = 1;
		MagicPos = 0;
		pTargetData->m_Wakan.AddDataAddPercent(10);
		break;
	case 113:// �ߵ�����
		MagicID = 0x4DDF5690;
		MagicUpOrDown = 1;
		MagicPos = 0;
		pTargetData->m_Wakan.AddDataAddPercent(10);
		pTargetData->m_ATK.AddDataAddPercent(10);
		pTargetData->m_DEF.AddDataAddPercent(10);
		pTargetData->m_SPD.AddDataAddPercent(10);
		pTargetData->m_HIT.AddDataAddPercent(10);
		break;
	case 120://��ڤ����
		MagicID = 0x21435203;
		MagicUpOrDown = 1;
		MagicPos = 0;
		break;
	case 122:// ʬ����
		MagicID = 0x839CC29A;
		MagicUpOrDown = 1;
		MagicPos = -10;

		break;
	case 123://��������
		
		{
			pTarget->SetShadow(50);
			pTargetData->m_HitRate.d1 += 10;
			pTargetData->m_HitRate.d2 += 50;
			pTargetData->m_MagicHitRate.d2 += 20;
		}
		break;
	case 125:
		pTargetData->m_ATK.AddDataAddPercent(15);
		break;
	case 126:// ��Ȫ֮Ϣ
		MagicID = 0x5ECEBEDD;
		pTargetData->m_SPD.AddDataAddPercent(-10);
		MagicUpOrDown = 1;
		MagicPos = -10;
		break;
	case 129:// ������
		MagicID = 0x5C0073EE;
		MagicUpOrDown = 1;
		MagicPos = 1;
		break;
	case 133://ţ��
		MagicID = 0x62EDD1CB;
		buf.damage = skilllv;
		MagicUpOrDown = 1;
		pTargetData->m_Wakan.AddDataOriginal(buf.damage);
		break;
	case 141://����
		TargetFightP.cannotAttack += 1;
		TargetFightP.cannotMagic += 1;
		MagicID = 0x3646A4D9;
		MagicPos = -10;
		break;
	case 148://������
	case 142://ӥ��
		stateid = 142;
		TargetFightP.cannotMagic += 1;
		MagicID = 0x58628406;
		MagicPos = -10;
		break;
	case	144:// ����
		MagicID=0x20FCCE94;
		MagicPos = -10;
		pTargetData->m_ATK.AddDataAddPercent(-30);
		pTargetData->m_DEF.AddDataAddPercent(-20);
		pTargetData->m_Wakan.AddDataAddPercent(-10);
		break;
	case 145:// ����
		MagicID = 0x85C9AF08;
		pTargetData->m_ATK.AddDataAddPercent(5);
		MagicUpOrDown = 1;
		break;
	case 147://������
		MagicID = 755485932;
		pTargetData->m_Wakan.AddDataAddPercent(10);
		MagicUpOrDown = 1;
		break;
	case 150://���ȷ��
		MagicID = 0xBBEA83F4;
		pTargetData->m_CounterRate.d1+=30;
		MagicUpOrDown = 1;
		break;
	case 152://��ħ����
		MagicID = 0x3D3FA597;
		pTargetData->m_HPMax.AddDataAddPercent(30);
		
		{
			int dam=AddHp(targetID, 5, 10000);
			SetDamage(-dam, targetID);
		}
		MagicUpOrDown = 1;
		break;
	case 156://ħ�����
		MagicID=0xAF07277F;
		MagicPos = -10;
		TargetFightP.cannotAddHP += 1;
		break;
	case 159://��������
		TargetFightP.cannotAttack += 1;
		TargetFightP.cannotMagic += 1;
		MagicID = 0x8103AFD2;//0xC4AA8E18;
		MagicPos = -10;
		break;
	case 160:// ��˿��
		buf.damage = skilllv * 5;
		pTargetData->m_DEF.AddDataAdd(buf.damage);
		MagicID = 0x369C6A29;
		MagicUpOrDown = TRUE;
		break;
	case 161: break;//����	
	case 162:  //���޵���
		MagicPos = -10;
		MagicID = 0x8F3E73D5;
		TargetFightP.cannotAttack += 1;
		break;
	case 164:break;//����ͬ��	
	case 166://�þ���
		MagicID = 0x0B788E5A;
		MagicUpOrDown = TRUE;
		MagicPos = -10;
		pTargetData->m_StabRateMagic.d1 += 50;    //���������˺�
		pTargetData->m_StabDamageRateMagic.d1 += 50; //����������
		break;
	case 1002: //1�غϵĸ� BUFF
		pTargetData->m_ParryRate.d1 += 100;
		break;
	case 1004://����
		break;
	case 2092://����һ��
		pTargetData->m_ATK.AddDataAddPercent(-20);
		break;
	case 2093://ҹ�����
		pTargetData->m_ATK.AddDataAddPercent(10);   //�˺�
		pTargetData->m_HIT.AddDataAddPercent(10);   //����
		pTargetData->m_DEF.AddDataAddPercent(10);   //����
		pTargetData->m_Wakan.AddDataAddPercent(10); //����
		pTargetData->m_SPD.AddDataAddPercent(10);   //�ٶ�
		pTargetData->m_Evade.AddDataAddPercent(10); //����
		break;
	case 2094://�����ٻ�
		MagicID= 0x19182964;
		MagicPos = -10;
		pTargetData->m_FightProperty.cannotLive += 1;
		break;
	case 2097:// ��������
		MagicID = 0xDEDEE983;
		MagicUpOrDown = TRUE;
		pTargetData->m_DamageAddPercentMagic.d2 += 50;
		break;
	case 3024://̫������ 
	case 3025://�޺�
		MagicPos = -10;
		MagicID = 0xAF07277F;// 0x5CC81980;
		pTargetData->m_DamageAddPercentMagic.d2 += 50;
		break;
	case 3026://������
		pTargetData->m_StabRateMagic.d1 += 100;
		pTargetData->m_StabDamageRateMagic.d1 += 30;
		break;
	case 3027://�����޷�
		pTargetData->m_MagicHitRate.d2 += 50;
		break;
	case 3047://��������
		pTargetData->m_DamageAddPercentMagic.d2 += 50;//���������ӳ�
		pTargetData->m_DamagePercent.d2 += 50;//��������ӳ�
		pTarget->ReMoveState(eBuffType��ӡ);
		pTarget->ReMoveState(eBuffType��);
		pTarget->ReMoveState(eBuffType����);
		pTarget->ReMoveState(eBuffType����);
		break;
	default: 
		ERRBOX2(stateid);
		return TRUE;
	}
	sSkill* pskill = g_pMainState->m_SkillManager.GetSkill(stateid);
	if (pskill)
	{
		buf.EffectType = (eBuffType)pskill->m_EffectType;
	}
	else buf.EffectType = eBuffType����;
	buf.skilllv = skilllv;
	buf.userID = userID;
	buf.targetID = targetID;
	buf.MagicID = MagicID;
	buf.bufPos = MagicPos;
	buf.bBuffup = MagicUpOrDown;
	buf.ID = stateid; //����BUFF  ID
	m_pFightState->AddState(targetID, buf);
	return TRUE;
}

BOOL cSkillManager::SetDateBuffDown(list<sBuff>::iterator& sbuf)//int targetID, int stateid)
{
	int targetID = sbuf->targetID;
	int stateid = sbuf->ID;
	cObj* pTarget = m_pFightState->m_pList[targetID]->m_pData;
	cPetData* pTargetData;
	pTarget->GetDateP(pTargetData);
	sFightData&  targetProperty = pTargetData->m_FightProperty;
	

	switch (stateid)
	{
	case 0://��ɨǧ��
	case 7://�Ƹ�����
		targetProperty.cannotAll -= 1;
		targetProperty.cannotAttack -= 1;
		targetProperty.cannotMagic -= 1;
		targetProperty.cannotSkill -= 1;
		targetProperty.cannotUseItem -= 1;
		break;
	case 1: //������
		pTargetData->m_SPD.AddDataOriginal(-10000);
		pTargetData->m_Pierce.d1 -= 20;
		pTargetData->m_CriRate.d1 -= 10;
		break;
	case 4:
		pTargetData->m_HIT.AddDataOriginal(-sbuf->damage);
		pTargetData->m_ATK.AddDataOriginal(-sbuf->damage / 4);
		break;
	case 6:
		pTarget->SetShake(FALSE);
		targetProperty.chaos -= 1;
		g_pMainState->m_FightList[pTarget->m_IDInFightlist].m_WorkType = -1;
		break;
	case 8://�����
		pTargetData->m_DamageAddPercentMagic.d2 -= 30;
		break;
	case 14://���߷�
		targetProperty.cannotAll -= 1;
		targetProperty.cannotAttack -= 1;
		targetProperty.cannotMagic -= 1;
		targetProperty.cannotSkill -= 1;
		targetProperty.cannotUseItem -= 1;
		break;
	case 15://ʧ��
		targetProperty.cannotMagic -= 1;
		break;
	case 16://����
		targetProperty.cannotMagic -= 1;
		break;
	case 17://ʧ��
		targetProperty.cannotAttack -= 1;
		targetProperty.cannotMagic -= 1;
		targetProperty.cannotSkill -= 1;
		break;
	case 18://׷��
		targetProperty.cannotAttack -= 1;
		pTargetData->m_DEF.AddDataAddPercent(20);
		break;
	case 19://���
		targetProperty.cannotMagic -= 1;
		break;
	case 20://ʧ��   
		targetProperty.cannotMagic -= 1;
		break;
	case 21://���� 
		targetProperty.cannotAll -= 1;
		targetProperty.cannotAttack -= 1;
		targetProperty.cannotMagic -= 1;
		targetProperty.cannotSkill -= 1;
		break;
	case 26:
		pTarget->SetShadow(FALSE);
		break;
	case 27: //��׷�
		pTargetData->m_DEF.AddDataAddPercent(20);
		break;
	case 33://��ջ���
		pTarget->SetTwinkle(FALSE);
		pTargetData->m_ATK.AddDataAddPercent(-20);
		break;
	case 34:   //Τ�ӻ���  
		pTargetData->m_DamageAddPercentMagic.d2 -= 20;
		break;
	case 35:  //��ջ���
		pTargetData->m_DEF.AddDataAddPercent(-20);
		break;
	case 36:  //һέ�ɽ�
		pTargetData->m_SPD.AddDataAddPercent(-20);
		break;
	case 46:    //���ޱ�
		pTargetData->m_MagicComboRate.d1 -= 30;
		break;
	case 49:    //��������
		
		break;
	case 50: //���컨���ж�
		
		break;
	case 141://����
	case 54://��������
		targetProperty.cannotAttack -= 1;
		targetProperty.cannotMagic -= 1;
		break;
	case 59://һЦ���
		targetProperty.cannotMagic -= 1;
		break;
	case 63://���񻤷�

		break;
	case 67:	//����
		pTargetData->m_ATK.AddDataAddPercent(30);
		break;
	case 68:	//����
		pTargetData->m_HIT.AddDataAddPercent(20);
		targetProperty.cannotMagic -= 1;
		break;
	case 69:	//�������
		targetProperty.cannotAttack -= 1;
		break;
	case 81:// ����
		pTarget->SetTwinkle(FALSE);
		pTargetData->m_ATK.SetDataAdd(-sbuf->damage);
		break;
	case 89:  //����Ǭ��
		targetProperty.cannotAttack -= 1;
		targetProperty.cannotMagic -= 1;
		targetProperty.cannotSkill -= 1;
		break;
	case 90:   //��������
		pTargetData->m_FightProperty.m_TurnMPAdd -= sbuf->damage;
		break;
	case 91:    //����֮Ȫ
		pTargetData->m_FightProperty.m_TurnHPAdd -= sbuf->damage;
		break;
	case 102: //�ն�����
		pTargetData->m_FightProperty.m_TurnHPAdd -= sbuf->damage;
		pTargetData->m_FightProperty.m_TurnHPHurtAdd -= sbuf->damage2;
		break;
	case 103://������
		pTargetData->m_FightProperty.m_TurnHPHurtAdd += sbuf->damage;
		break;
	case 112:// �鶯����
		pTargetData->m_Wakan.AddDataAddPercent(-10);
		break;
	case 113:// �ߵ����� 
		pTargetData->m_Wakan.AddDataAddPercent(-10);
		pTargetData->m_ATK.AddDataAddPercent(-10);
		pTargetData->m_DEF.AddDataAddPercent(-10);
		pTargetData->m_SPD.AddDataAddPercent(-10);
		pTargetData->m_HIT.AddDataAddPercent(-10);
		break;
	case 122:
		break;
	case 123://��������
		
		{
			pTarget->SetShadow(FALSE);
			pTargetData->m_HitRate.d1 -= 10;
			pTargetData->m_HitRate.d2 -= 50;
			pTargetData->m_MagicHitRate.d2 -= 20;
		}
		break;
	case 125:
		pTargetData->m_ATK.AddDataAddPercent(-15);
		break;
	case 126:// ��Ȫ֮Ϣ
		pTargetData->m_SPD.AddDataAddPercent(10);
		break;
	case 129:// ������
		targetProperty.cannotLive -= 1;
		break;
	case 133://ţ��
		pTargetData->m_Wakan.AddDataOriginal(-sbuf->damage);
		break;
	case 142://ӥ��
		targetProperty.cannotMagic -= 1;
		break;
	case	144:// ����
		pTargetData->m_ATK.AddDataAddPercent(30);
		pTargetData->m_DEF.AddDataAddPercent(20);
		pTargetData->m_Wakan.AddDataAddPercent(10);
		break;
	case 145:// ����
		pTargetData->m_ATK.AddDataAddPercent(-5);
		break;
	case 147://����
		pTargetData->m_Wakan.AddDataAddPercent(-10);
		break;
	case 150://���ȷ��
		pTargetData->m_CounterRate.d1 -= 30;
		break;
	case 152://��ħ����
		pTargetData->m_HPMax.AddDataAddPercent(-30);
		
		{
			int dam = AddHp(targetID, -5, 10000);
			SetDamage(-dam, targetID);
		}
		break;
	case 156://ħ�����
		pTargetData->m_FightProperty.cannotAddHP -= 1;
		break;
	case 159://��������
		targetProperty.cannotAttack -= 1;
		targetProperty.cannotMagic -= 1;
		break;
	
	case 160:// ��˿��	
		pTargetData->m_DEF.AddDataAdd(-sbuf->damage);
		break;
	
	case 162:  //���޵���
		targetProperty.cannotAttack -= 1;
		break;
	case 166://�þ���
		pTargetData->m_StabRateMagic.d1 -= 50;    //���������˺�
		pTargetData->m_StabDamageRateMagic.d1 -= 50; //����������
		break;
	case 1002:
		pTargetData->m_ParryRate.d1 -= 100;
		break;
	case 2092://����һ��
		pTargetData->m_ATK.AddDataAddPercent(20);
		break;
	case 2093://ҹ�����
		pTargetData->m_ATK.AddDataAddPercent(-10);   //�˺�
		pTargetData->m_HIT.AddDataAddPercent(-10);   //����
		pTargetData->m_DEF.AddDataAddPercent(-10);   //����
		pTargetData->m_Wakan.AddDataAddPercent(-10); //����
		pTargetData->m_SPD.AddDataAddPercent(-10);   //�ٶ�
		pTargetData->m_Evade.AddDataAddPercent(-10); //����
		break;
	case 2094://�����ٻ�
		pTargetData->m_FightProperty.cannotLive -= 1;
		break;
	case 2097:// ��������
		pTargetData->m_DamageAddPercentMagic.d2 -= 50;
		break;
	case 3024://̫������ �޺�
	case 3025:
		pTargetData->m_DamageAddPercentMagic.d2 -= 50;
		break;
	case 3026://������
		pTargetData->m_StabRateMagic.d1 -= 100;
		pTargetData->m_StabDamageRateMagic.d1 -= 30;
		break;
	case 3027://�����޷�
		pTargetData->m_MagicHitRate.d2 -= 50;
		break;
	case 3047://��������
		pTargetData->m_DamageAddPercentMagic.d2 -= 50;//���������ӳ�
		pTargetData->m_DamagePercent.d2 -= 50;//��������ӳ�
		break;
	default:
		ERRBOX2(stateid);
		return FALSE;
		break;
	}
	return TRUE;
}

BOOL cSkillManager::Use1002(int Skilllv, int sSkillID, int UserID, int TargetID)
{
	sBuff buf;
	buf.times = 1;
	SetDateBuffUp(UserID, UserID, Skilllv, buf, Skilllv);
	m_pFightState->m_pList[UserID]->m_work.pop_back();
	return TRUE;
}
//����
BOOL cSkillManager::Use1003(int Skilllv, int sSkillID, int UserID, int TargetID)
{
	sFightList* pUserList = m_pFightState->m_pList[UserID];
	int& stage = pUserList->GetWork()->stage;
	switch (stage)
	{
	case 0:
	
		{
			pUserList->m_pData->Set(POS_MAGIC);
			pUserList->m_pData->SetSound(POS_MAGIC);
			stage += 1;
		}
		return TRUE;
	case 1:
		if (WaitForMagicPoint(UserID))
		{
			stage += 1;
			if (g_pMainState->m_ItemManage.UseItem(GetItem(UserID), TargetID))
			{
				if (g_pMainState->m_ItemManage.m_bToAll)
				{
					int startid;
					if (TargetID < 10)startid = 0;
					else startid = 10;
					for (int i = startid; i < startid + 10; i++)
						SetDamage(-g_pMainState->m_ItemManage.m_hp, TargetID);
				}
				else
					SetDamage(-g_pMainState->m_ItemManage.m_hp, TargetID);
			}
		}
		return TRUE;
	case 2:
		if (m_pFightState->WaitPosEnd(UserID) || (pUserList->m_pData->m_NowPos == POS_FIGHT_STAND))
		{
			pUserList->m_pData->Set(POS_FIGHT_STAND);
			pUserList->m_work.pop_back();
		}
		return TRUE;
	}
	return TRUE;
}
BOOL cSkillManager::Use1005(int Skilllv, int sSkillID, int UserID, int TargetID)
{
	sFightList* pUserList = m_pFightState->m_pList[UserID];
	cObj* pUser = pUserList->m_pData;
	cPetData* pPet;
	pUser->GetDateP(pPet);
	int lv = pPet->m_LV;
	int& stage = pUserList->GetWork()->stage;
	switch (stage)
	{
	case 0:
		
		{	
			if (pUser->m_IndexType == INDEXTYEP_PET)
			{
				pUserList->m_work.pop_back();
				stage = -1;
				g_pMainState->m_Channel.AddNewString("�����޷�ʹ�ð���");
				return TRUE;
			}
			sItem2* pItem = GetItem(UserID);
			if (pItem->GetNum() < 1)
			{
				pUserList->m_work.pop_back();
				stage = -1;
				g_pMainState->m_Channel.AddNewString("û�е���");
				return TRUE;
			}
			if (lv / 15 < pItem->GetID())
			{
				pUserList->m_work.pop_back();
				stage = -1;
				g_pMainState->m_Channel.AddNewString("��ɫ�ȼ�̫��,�޷�ʹ��");
				return TRUE;
			}
			pUser->Set(POS_MAGIC);
			pUser->SetSound(POS_MAGIC);
			stage += 1;
		}
		return TRUE;
	case 1:
		
		{

			if (WaitForMagicPoint(UserID))
			{
				stage += 1;
				sItem2* pItem = GetItem(UserID);
				
				DWORD dateid = 0;
					int num = 0;
					switch (pItem->GetID())
					{
					case 0://�ɵ�
						dateid = 0x44BE65A3;
						num = 1;
						break;
					case 1://�ɻ�ʯ
						dateid = 0x44BE65A3;
						num = 1+lv/30;
						break;
					case 2://����޼
						dateid = 0x44BE65A3;
						num = 1 + lv / 30;
						break;
					case 3://��Ӱ����
						dateid = 0x44BE65A3;
						num = 1+lv / 30;
						break;
					case 4://��ȸ��
						dateid = 0x44BE65A3;
						num = 1 + lv / 30;
						break;
					case 5://��ɳ��Ӱ
						dateid = 0x44BE65A3;
						num = 1+lv / 30;
						break;
					case 6://��������
						dateid = 0x44BE65A3;
						num = 1 + lv / 30;
						break;
					case 7://��������
						dateid = 0x44BE65A3;
						num = 1+lv / 30;
						break;
					case 8://ħ����
						dateid = 0x44BE65A3;
						num = 1 + lv / 30;
						break;
					case 9://˳������
						dateid = 0x44BE65A3;
						num = 1+lv / 30;
						break;
					default:
						return FALSE;
					}
					m_pFightState->m_HiddenWeapons.Load(dateid);
					int i=0;
					for (; i < num; i++)
					{				
						m_pFightState->m_HiddenWeaponsTargetID[i] = g_pMainState->m_AI.GetNear(TargetID, i,TRUE);
						if (-1==m_pFightState->m_HiddenWeaponsTargetID[i])
							break;
						m_pFightState->m_HiddenWeaponsp[i].m_NeedShow = TRUE;
						m_pFightState->m_HiddenWeaponsp[i].m_x = pUser->GetX();
						m_pFightState->m_HiddenWeaponsp[i].m_y = pUser->GetY() - 30;
					}			
					m_pFightState->m_HiddenWeaponsUseNum = i;
			}
		}
		return TRUE;
	case 2:
		if (m_pFightState->WaitPosEnd(UserID) || (pUser->m_NowPos == POS_FIGHT_STAND))
		{
			pUser->Set(POS_FIGHT_STAND);
			stage  += 1;
		}
	case 3:
		
		{
			int targetid;
			sItem2* pItem = GetItem(UserID);
			for (int i = 0; i < 10; i++)
			{
				if (TRUE == m_pFightState->m_HiddenWeaponsp[i].m_NeedShow)
				{
					if (m_pFightState->HiddenWeaponsMove(i))
					{
						targetid = m_pFightState->m_HiddenWeaponsTargetID[i];					
						m_pFightState->m_HiddenWeaponsUseNum -= 1;
						m_pFightState->m_HiddenWeaponsp[i].m_NeedShow = FALSE;
						if (!g_pMainState->m_FightList[targetid].isLive)continue;						
						cPetData* pTargetData;
						g_pMainState->m_FightList[targetid].m_pData->GetDateP(pTargetData);
						int damageup = pPet->m_HiddenWeaponDamagePercent.d1 - pTargetData->m_HiddenWeaponDamagePercent.d2;						
						if (rand() % 100 >= 100 + pPet->m_HitRate.d1 - pTargetData->m_HitRate.d2)
						{
							m_pFightState->SetAvoid(targetid, pUser->GetDirection());
						}
						else
						{
							int dam, damh;
							switch (pItem->GetID())
							{
							case 0://�ɵ�
								dam = 100;
								damh = 10;
								break;
							case 1://�ɻ�ʯ
								dam = 120;
								damh = 20;
								break;
							case 2://����޼
								dam = 200;
								damh = 50;
								break;
							case 3://��Ӱ����
								dam = 300;
								damh = 80;
								break;
							case 4://��ȸ��
								dam = 400;
								damh = 100;
								break;
							case 5://��ɳ��Ӱ
								dam = 500;
								damh = 150;
								break;
							case 6://��������
								dam = 800;
								damh = 200;
								break;
							case 7://��������
								dam = 1000;
								damh = 250;
								break;
							case 8://ħ����
								dam = 1500;
								damh = 300;
								break;
							default://˳������
								dam = 2000;
								damh = 400;
								break;
							}


							dam = (dam*(100 + damageup)) / 100;
							if (dam > 0)
							{
								CheckPoision(UserID, targetid);//�ж�����
								if ((TargetID < 10) || g_pMainState->m_AI.m_AttackAI.m_bPKMode)
								{//��ͨ����ʱ�˺���ʵ��2������
									dam = dam >> 1;
									damh = damh >> 1;
								}
								m_pFightState->SetSuffer(targetid, m_pFightState->m_HiddenWeaponsp[i].m_direction, UserID);
								AddHp(targetid, -dam);
								AddHpHurt(targetid, -damh);
								SetDamage(dam, targetid);
							}
							else 	m_pFightState->SetAvoid(targetid, m_pFightState->m_HiddenWeaponsp[i].m_direction);
						}
					}
				}
			}
			if (0 == m_pFightState->m_HiddenWeaponsUseNum)
			{
				if (3 == stage)
				{
					pUserList->m_work.pop_back();
					stage = -1;
				}
			}
		}
		break;
	}
	return TRUE;
}
//�ٻ�
BOOL cSkillManager::Use1004(int Skilllv, int sSkillID, int UserID, int TargetID)
{
	sFightList*& pUserList = m_pFightState->m_pList[UserID];
	//int& stage = pUserList->GetWork()->stage;
	//switch (stage)
	//{
	//case 0:
	/*	pUserList->m_pData->Set(POS_MAGIC);
		pUserList->m_pData->SetSound(POS_MAGIC);
		stage += 1;
		return TRUE;
	case 1:
		if (WaitForMagicPoint(UserID))
		{*/
			//�Ȱ�֮ǰ���ٻ���ȥ��ս��Ч��
			if (g_pMainState->m_FightList[UserID + 5].isLive)
			{
				g_pMainState->m_pFightState->SetFightBufDown(UserID + 5);
			}

			g_pMainState->SetFightPet(UserID, pUserList->m_SkillSelect.Lv);
			m_pFightState->PrePareToFight(UserID + 5);
			int k = m_pFightState->m_SpeedListPos[UserID];
			m_pFightState->SortBySpeed();
			if (m_pFightState->m_SpeedListPos[UserID] != k)
				m_pFightState->m_nowturnid += 1;
		//	stage += 1;
			g_pMainState->FindObj();
	/*	}
		return TRUE;
	case 2:
		if (m_pFightState->WaitPosEnd(UserID) || (pUserList->m_pData->m_NowPos == POS_FIGHT_STAND))
		{
			pUserList->m_pData->Set(POS_FIGHT_STAND);*/
			pUserList->m_work.pop_back();
	/*	}
		return TRUE;
	}*/
	return TRUE;
}

BOOL cSkillManager::CheckFightStateOnLine(int FightListID,int& dam)
{
	cObj* pUser = g_pMainState->m_FightList[FightListID].m_pData;
	cPetData* pPetdata;
	pUser->GetDateP(pPetdata);
	if (pUser->CheckHaveState(14))
	{//���߷� ����1%�ͱ�����
		if (pPetdata->m_HPMax.GetData() / dam > 1)
			pUser->ReMoveStateID(14);
	}
	return TRUE;
}


BOOL cSkillManager::SetPetSkill1(cPetData* pPetDate, int ID, int lv)
{
	switch (ID)
	{
	case 0://����
		pPetDate->m_CounterRate.d1 += lv*30;
		break;
	case 1://�߼�����
		pPetDate->m_CounterRate.d1 += lv*45;
		break;
	case 2://����
		pPetDate->m_StabRate.d1 += lv*25;
		pPetDate->m_StabDamageRate.d1 += lv*30;
		break;
	case 3://�߼�����
		pPetDate->m_StabRate.d1 += lv*30;
		pPetDate->m_StabDamageRate.d1 += lv*40;
		break;
	case 4://��Ѫ
		pPetDate->m_SuckBloodRate.d1 += lv*20;
		break;
	case 5://�߼���Ѫ
		pPetDate->m_SuckBloodRate.d1 += lv*30;
		break;
	case 6://����
		pPetDate->m_Combo.d1 += lv*20;
		break;
	case 7://�߼�����
		pPetDate->m_Combo.d1 += lv*30;
		break;
	case 8://����
		pPetDate->m_HitRate.d1  += lv*10;
		pPetDate->m_HitRate.d2    += lv*10;
		pPetDate->m_HiddenWeaponDamagePercent.d2 -= lv * 30;
		break;
	case 9://�߼�����
		pPetDate->m_HitRate.d1 += lv * 15;
		pPetDate->m_HitRate.d2 += lv * 15;
		pPetDate->m_HiddenWeaponDamagePercent.d2 -= lv * 10;
		break;
	case 10://ҹս
		pPetDate->m_Spurting.d2 += 20;
		pPetDate->nightLv += lv*10;
		break;
	case 11://�߼�ҹս
		pPetDate->m_Spurting.d2 += 30;
		pPetDate->nightLv += lv*20;
		break;
	case 12://����
		pPetDate->m_HitRate.d2 += lv*15;
		break;
	case 13://�߼�����
		pPetDate->m_HitRate.d2 += lv*25;
		break;
	case 14://��֪
		pPetDate->m_HitRate.d1 += lv*15;
		break;
	case 15://�߼���֪
		pPetDate->m_HitRate.d1 += lv*25;
		break;
	case 16://����
		pPetDate->m_LiveHpLv += lv*10;
		break;
	case 17://�߼�����
		pPetDate->m_LiveHpLv += lv*20;
		break;
	case 18://ڤ˼
		pPetDate->m_LiveMpLv += lv*10;
		break;
	case 19://�߼�ڤ˼
		pPetDate->m_LiveMpLv += lv*20;
		break;
	case 20://�۸�
		pPetDate->mpsavepercent += lv*30;
		break;
	case 21://�߼��۸�
		pPetDate->mpsavepercent += lv*50;
		break;
	case 22://����
		pPetDate->m_CriRate.d2   -= lv*10;
		pPetDate->m_CriRate.d2 -= lv * 10;
		break;
	case 23://��ɱ
		pPetDate->m_CriRate.d1 += lv*10;
		break;
	case 24://�߼���ɱ
		pPetDate->m_CriRate.d1 += lv*20;
		break;
	case 25://����
		pPetDate->m_CriRate.d2 -= lv*10;
		pPetDate->m_MagicCriRate.d2 -= lv * 10;
		break;
	case 26://�߼�����
		pPetDate->m_CriRate.d2 -= lv*20;
		pPetDate->m_MagicCriRate.d2 -= lv * 20;
		break;
	case 27://��
		pPetDate->restoreRate += lv*10;
		break;
	case 28://�߼���
		pPetDate->restoreRate += lv*20;
		break;
	case 29://�м�
		pPetDate->m_ParryRate.d1 += lv*20;
		break;
	case 30://�߼��м�
		pPetDate->m_ParryRate.d1 += lv*30;
		break;
	case 31://����
		pPetDate->buffAdd += lv*2;
		break;
	case 32://�߼�����
		pPetDate->buffAdd += lv*4;
		break;
	case 33://����
		pPetDate->m_SPD.AddDataAddPercent(lv*10);
		break;
	case 34://�߼�����
		pPetDate->m_SPD.AddDataAddPercent(lv * 20);
		break;
	case 35://ǿ��
		pPetDate->m_ATK.AddDataAddPercent(lv*5);
		break;
	case 36://�߼�ǿ��
		pPetDate->m_ATK.AddDataAddPercent(lv * 10);
		break;
	case 37://����
		pPetDate->m_DEF.AddDataAddPercent(lv * 5);
		break;
	case 38://�߼�����
		pPetDate->m_DEF.AddDataAddPercent(lv * 10);
		break;
	case 39://͵Ϯ  
		pPetDate->m_StabRate.d2 += lv * 20;//����������
		pPetDate->m_CounterRate.d2 += lv * 20;//����������
		pPetDate->m_ParryRate.d2 += lv * 10;//����
		break;
	case 40://�߼�͵Ϯ
		pPetDate->m_StabRate.d2 += lv * 30;//����������
		pPetDate->m_CounterRate.d2 += lv * 30;
		pPetDate->m_ParryRate.d2 += lv * 20;
		break;
	case 41://��
		pPetDate->poisonRate.d1 += lv*10;
		break;
	case 42://�߼���
		pPetDate->poisonRate.d1 += lv*20;
		break;
	case 43://����
		pPetDate->ghostSpecial.d1 += lv*30;
		break;
	case 44://�߼�����
		pPetDate->ghostSpecial.d1 += lv*40;
		break;
	case 45://�����
		pPetDate->m_DamagePercent.d2 += lv*20;
		if (lv > 0)
		{
			pPetDate->m_race += 10;
		}
		else
		{
			pPetDate->m_race -= 10;
		}
		break;
	case 46://�߼������
		pPetDate->m_DamagePercent.d2 += lv*30;
		if (lv > 0)
		{
			pPetDate->m_race += 10;
		}
		else
		{
			pPetDate->m_race -= 10;
		}
		break;
	case 47://ħ֮��
		pPetDate->m_DamageAddPercentMagic.d1 += lv*10;
		break;
	case 48://�߼�ħ֮��
		pPetDate->m_DamageAddPercentMagic.d1 += lv*20;
		break;
	case 49://����
		pPetDate->GoldLivePercent.d1 += lv*20;
		break;
	case 50://�߼�����
		pPetDate->GoldLivePercent.d1 += lv*30;
		break;
	case 51://������
		pPetDate->abnormalResist += lv*10;
		break;
	case 52://�߼�������
		pPetDate->abnormalResist += lv*20;
		break;
	case 53://������
		pPetDate->GoldLivePercent.d2 += lv*20;
		break;
	case 54://�߼�������
		pPetDate->GoldLivePercent.d2 += lv*30;
		break;
	case 55://��������
		pPetDate->m_MagicComboRate.d1 += lv*20;
		break;
	case 56://�߼���������
		pPetDate->m_MagicComboRate.d1 += lv*30;
		break;
	case 57://��������
		pPetDate->m_MagicCriRate.d1 += lv*10;
		break;
	case 58://�߼���������
		pPetDate->m_MagicCriRate.d1 += lv*15;
		break;
	case 59://��������
		pPetDate->magicWavePercent += lv*30;
		break;
	case 60://�߼���������
		pPetDate->magicWavePercent += lv*40;
		break;
	case 61://�ٶ�
		pPetDate->m_DEF.AddDataAddPercent(lv*20);
		pPetDate->m_SPD.AddDataAddPercent(-lv * 40);
		pPetDate->m_HitRate.d2 -= lv*20;
		pPetDate->m_ParryRate.d1 -= lv*40;
		break;
	case 62://�����
		pPetDate->m_MagicFire.d2 -= lv*30; //��	
		break;
	case 63://����ˮ
		pPetDate->m_MagicWater.d2 -= lv*30;  //ˮ
		break;
	case 64://������
		pPetDate->m_MagicSoil.d2 -= lv*30; //��
		break;
	case 65://������
		pPetDate->m_MagicThunder.d2 -= lv*30;//��
		break;
	case 66://����������
		pPetDate->m_MagicThunder.d2 += lv*20;//��
		break;
	case 67://�߼�����������
		pPetDate->m_MagicThunder.d2 += lv*30;//��
		break;
	case 68://����������
		pPetDate->m_MagicSoil.d2 += lv*20; //��
		break;
	case 69://�߼�����������
		pPetDate->m_MagicSoil.d2 += lv*30; //��
		break;
	case 70://����������
		pPetDate->m_MagicFire.d2 += lv*20; //��
		break;
	case 71://�߼�����������
		pPetDate->m_MagicFire.d2 += lv*30; //��
		break;
	case 72://ˮ��������
		pPetDate->m_MagicWater.d2 += lv*20;  //ˮ
		break;
	case 73://�߼�ˮ��������
		pPetDate->m_MagicWater.d2 += lv*30;  //ˮ
		break;
	case 74://�ƻ���ľ 
		pPetDate->m_HiddenWeaponDamagePercent.d2 += lv * 30;
		break;
	case 75://����
		pPetDate->m_Spurting.d1 += lv * 20;
		break;
	case 76://��Ѫ׷��
		pPetDate->ShiXueAdd += lv*5;
		break;
	case 77://���۷���
		pPetDate->m_DamageAddMagicLv.d2 += lv * 2;//�����˺����
		pPetDate->m_DamageAddMagic.d2	+= lv * 2;  //�����˺����
		pPetDate->m_DamageAddPercentMagic2.d2 += 10;
		pPetDate->m_MagicWater.d2 += lv*20;  //ˮ
		break;
	case 78://�����ػ�
		pPetDate->m_DamageAddMagicLv.d2 += lv * 3;//�����˺����
		pPetDate->m_DamageAddLv.d2 += lv * 3;  //�����˺����
		pPetDate->m_DamageAddPercentMagic2.d2 += 15;
		pPetDate->m_MagicWater.d2 += lv * 30;  //ˮ
		break;
	case 79://�����ֿ�
		pPetDate->m_DamageAddPercentMagic.d2 += lv * 10;
		break;
	case 80://�߼������ֿ�
		pPetDate->m_DamageAddPercentMagic.d2 += lv * 15;
		break;
	case 81://ֱ��
		pPetDate->m_Pierce.d1+=30;//���ӷ�����
		pPetDate->m_PierceMagic.d1+=30; //����������
		break;
	case 82://�绪��ʥ
		pPetDate->comboNumUp += lv*3;
		pPetDate->m_Combo.d1 += lv*25;
		break;
	case 83://��������
		pPetDate->m_StabRateMagic.d1 += lv*20;//����������
		pPetDate->m_StabDamageRateMagic.d1 += lv*30;//���������˺���
		break;
	}
	return TRUE;
}

BOOL cSkillManager::CheckHit(cPetData* pUserData, cPetData *pTargetData, int offset/*=0*/)
{
	int hit = pUserData->m_HIT.GetData();
	int avoid = pTargetData->m_Evade.GetData()+hit;
	if (avoid < 1)avoid = 1;
	int hitrate = offset+10 + pUserData->m_HitRate.d1 - pTargetData->m_HitRate.d2 + (100 * hit) / avoid;
	if (rand() % 100 < hitrate)  //��������
	{
		pTargetData->m_FightProperty.m_bHit =  TRUE;
		return  TRUE;
	}
	pTargetData->m_FightProperty.m_bHit = FALSE;
	return FALSE;
}

BOOL cSkillManager::CheckCRI(cPetData* pUserData, cPetData *pTargetData, int offset /*= 0*/)
{
	int cri = pUserData->m_CriRate.d1;
	int uncri = pTargetData->m_CriRate.d2;
	int truecri = cri - uncri + offset;
	if (rand() % 101 < truecri)
		return TRUE;
	return FALSE;
}



BOOL cSkillManager::WaitForMagicPoint(int Waiter)
{
	CWFile* file= m_pFightState->m_pList[Waiter]->m_pData->m_pShowData[0];
	sFightOffset* p = m_pFightState->m_pList[Waiter]->m_pData->GetFightOffset();
	if (file->GetNowFrame() == p->m_MagicPoint
		&&file->GetWillNextFrame())
	{
		return TRUE;
	}
	return FALSE;
}

sItem2* cSkillManager::GetItem(int UserID)
{
	int k = UserID;
	if (g_pMainState->m_FightList[k].m_pData->m_IndexType != INDEXTYEP_CHARACTER)
	{
		k -= 5;
		if (g_pMainState->m_FightList[k].m_pData->m_IndexType != INDEXTYEP_CHARACTER)
			return 0;
	}
	cCharacter* pc = (cCharacter*)m_pFightState->m_pList[k]->m_pData->m_pIndex;
	return &pc->m_PCData.m_Item[m_pFightState->m_pList[UserID]->m_ItemSelectID];
}
BOOL  cSkillManager::CountMagicDamage(int& dam, int UserID, int TargetID, int Damageup,eMagicWuXing wuxing)//���㷨���˺�
{
	cPetData*  pUserData;
	cPetData*  pTargetData;
	cObj* pUser = m_pFightState->m_pList[UserID]->m_pData;
	cObj* pTarget = m_pFightState->m_pList[TargetID]->m_pData;
	pUser->GetDateP(pUserData);
	pTarget->GetDateP(pTargetData);
	
	if (pTargetData->m_FightProperty.m_WallNum > 0)
	{//���֮ǽ
		pTargetData->m_FightProperty.m_WallNum -= 1;
		dam = 0;
		return TRUE;
	}
	int hit = 100+pUserData->m_MagicHitRate.d1 - pTargetData->m_MagicHitRate.d2;
	if (rand() % 100 >= hit)//�����ر�
	{
		dam = 0;
		return TRUE;
	}

	int wakanUser = pUserData->m_Wakan.GetData();    //��������
	int  wakanTarget = pTargetData->m_Wakan.GetData(); //Ŀ������
	wakanTarget -= pUserData->m_LV<<1;
	int& atknum = pTargetData->m_FightProperty.SufferNum;
	int pierce = pUserData->m_PierceMagic.d1+atknum * 3;
	if (pierce)
	{
		if (pierce > 100)pierce = 100;
		wakanTarget -= (wakanTarget* pierce) / 100; //��������
	}
	int DamageAddPercent = pUserData->m_DamageAddPercentMagic.d1; //��������������
	int DamageDefPercent = pTargetData->m_DamageAddPercentMagic.d2;   //Ŀ�귨����������
	dam = (wakanUser)-(wakanTarget)+pUserData->m_LV;
	BOOL bCri = FALSE;
	if (dam > 1)
	{
		Damageup += CorrectWuXing(pUserData, pTargetData, wuxing);//��������	
		Damageup += CountSpecial(pUserData, pTargetData, pTarget->GetSex());//�ع�����
		dam = (dam*(100 + DamageAddPercent - DamageDefPercent + Damageup)) / 100;
		bCri = CountCriMagic(pUserData, pTargetData, dam);//��������

		if (pUserData->magicWavePercent)
		{//��������
			dam = dam * (100 + rand() % pUserData->magicWavePercent) / 100;
		}
		dam += (pUserData->m_DamageAddMagicLv.d1 - pTargetData->m_DamageAddMagicLv.d2) *pUserData->m_LV;
		if (rand() % 100 < pUserData->m_XiangMoRate.d1 - pUserData->m_XiangMoRate.d2)dam /= 2;
		if (Damageup > -1)if (dam < 1)dam = 1;
	}
	else dam = 1;
	CheckFightStateOnLine(TargetID, dam);
	if (pTargetData->m_StabRateMagic.d1)
	{
		//int rate= pTargetData->m_StabRateMagic.d1 - pUserData->m_StabRateMagic.d2;
		if (rand() % 100 < dam)
		{
			int rate = pTargetData->m_StabDamageRateMagic.d1 - pUserData->m_StabDamageRateMagic.d2;
			int dam2 = dam*rate / 100;
			CheckGodLivePercent(dam2, UserID, TargetID); //����
			AddHp(UserID, -dam2);
			SetDamage(dam2, UserID);
		}
	}

	if (pTargetData->m_FightProperty.m_bDefend)dam /= 2;//����Ƿ����	

	//����
	int rate = pUserData->m_MagDownHPHurt.d1 - pTargetData->m_MagDownHPHurt.d2;
	if (rate>0)AddHpHurt(TargetID, -(dam*rate) / 100);

	CheckGodLivePercent(dam, TargetID, UserID); //����
	AddHp(TargetID, -dam);
	SetDamage(dam, TargetID);
	pTargetData->m_FightProperty.SufferNum+=1;
	return bCri;
}
void cSkillManager::CountphysicalDamage(int& dam,int UserID, int TargetID, int Damageup/*=0*/)
{
	cObj* pUser;
	cObj* pTarget;
	cPetData* UserData;
	cPetData* TargetData;
	pUser = m_pFightState->m_pList[UserID]->m_pData;
	pUser->GetDateP(UserData);
	pTarget = m_pFightState->m_pList[TargetID]->m_pData;
	pTarget->GetDateP(TargetData);
	//sFightData& userFightData = UserData->m_FightProperty;
	sFightData& targetFightData =TargetData->m_FightProperty;
	if (TargetData->m_FightProperty.m_WallNum>0)
	{
		TargetData->m_FightProperty.m_WallNum -= 1;
		dam = 0;
		return;
	}
	int userAtk = UserData->m_ATK.GetData();    //������
	int SufferDef = TargetData->m_DEF.GetData(); //Ŀ�����
	int& atknum = targetFightData.SufferNum;

	int picrcerate = UserData->m_Pierce.d1 - TargetData->m_Pierce.d2 + atknum*3;
	if (picrcerate > 0)
	{
		SufferDef -= (SufferDef*picrcerate ) / 100;//���ӷ���
	}
	dam = (userAtk)-(SufferDef);
	//�������˺�����
	Damageup += UserData->m_DamagePercent.d1 - TargetData->m_DamagePercent.d2; //����
	Damageup+= CountSpecial(UserData, TargetData, pTarget->GetSex());//�ع�����
	Damageup += UserData->ShiXueAdd*atknum;//����������
	int& userlv = UserData->m_LV;  //����ȼ�
	dam = (dam*(100 +  + Damageup)) / 100;
	//��ɱ����
	CountCriDamage(UserData, TargetData, dam);
	if (rand() % 100 < UserData->m_JinJiaRate.d1 - UserData->m_JinJiaRate.d2)dam /= 2;
	//
	//�������
	dam += UserData->m_DamageAdd.d1 - TargetData->m_DamageAdd.d2;
	dam += (UserData->m_DamageAddLv.d1 - TargetData->m_DamageAddLv.d2) *userlv;
	
	if (dam < 1)dam = 1;
	CheckFightStateOnLine(TargetID,dam);	
	CheckPoision(UserID, TargetID);//�ж�����
	if (TargetData->m_FightProperty.m_bDefend)dam /= 2;//�����˺�����
	CountProtect(dam, pUser, pTarget);//�����˺�����

	SuckBlood(dam, UserID, TargetData);//��Ѫ
	CheckFanZheng(UserID, TargetID, dam);//����
	//����
	int rate= UserData->m_AtkDownHPHurt.d1 - TargetData->m_AtkDownHPHurt.d2;
	if (rate>0)AddHpHurt(TargetID, -(dam*rate) / 100);
	//����
	rate=UserData->m_AtkDownMP.d1 - TargetData->m_AtkDownMP.d2;
	if (rate > 0)AddMp(TargetID, -(dam*rate) / 100);
	//����
	if (TargetData->m_AtkDownAtk.d1)
	{
		if (TargetData->m_ATK.GetData() > 1)
		{
			rate = UserData->m_AtkDownAtk.d1 - TargetData->m_AtkDownAtk.d2;
			if (rate > 0)
			{
				if (TargetData->m_ATK.GetData() <= rate)rate = TargetData->m_ATK.GetData() - 1;
				TargetData->m_ATK.AddDataAdd(-rate);
			}
		}
	}
	//����
	if (TargetData->m_AtkDownDef.d1)
	{
		if (TargetData->m_DEF.GetData() > 1)
		{
			rate = UserData->m_AtkDownDef.d1 - TargetData->m_AtkDownDef.d2;
			if (rate > 0)
			{
				if (TargetData->m_DEF.GetData() <= rate)rate = TargetData->m_DEF.GetData() - 1;
				TargetData->m_DEF.AddDataAdd(-rate);
			}
		}
	}
	//������
	if (TargetData->m_AtkDownHit.d1)
	{
		if (TargetData->m_HIT.GetData() > 1)
		{
			rate = UserData->m_AtkDownHit.d1 - TargetData->m_AtkDownHit.d2;
			if (rate > 0)
			{
				if (TargetData->m_HIT.GetData() <= rate)rate = TargetData->m_HIT.GetData() - 1;
				TargetData->m_HIT.AddDataAdd(-rate);
			}
		}
	}
	//����
	if (TargetData->m_AtkDownWakan.d1)
	{
		if (TargetData->m_Wakan.GetData() > 1)
		{
			rate = UserData->m_AtkDownWakan.d1 - TargetData->m_AtkDownWakan.d2;
			if (rate > 0)
			{
				if (TargetData->m_Wakan.GetData() <= rate)rate = TargetData->m_Wakan.GetData() - 1;
				TargetData->m_Wakan.AddDataAdd(-rate);
			}
		}
	}
	//����
	if (TargetData->m_AtkDownSpd.d1)
	{
		if (TargetData->m_SPD.GetData() > 1)
		{
			rate = UserData->m_AtkDownSpd.d1 - TargetData->m_AtkDownSpd.d2;
			if (rate > 0)
			{
				if (TargetData->m_SPD.GetData() <= rate)rate = TargetData->m_SPD.GetData() - 1;
				TargetData->m_SPD.AddDataAdd(-rate);
			}
		}
	}
	
	//��CP
	if (TargetData->m_AtkDownCP.d1)
	{
		rate = UserData->m_AtkDownCP.d1 - UserData->m_AtkDownCP.d2;
		if (rate > 0)UserData->m_CP -= rate;
		if (UserData->m_CP < 0)UserData->m_CP = 0;
	}


	CheckGodLivePercent(dam, TargetID,UserID); //����
	AddHp(TargetID, -dam);
	SetDamage(dam, TargetID);
	atknum += 1;
}

//��ɱ����
void cSkillManager::CountCriDamage(cPetData* user, cPetData* target,int& nowdam)
{
	if (user->m_FightProperty.m_bCri)
		nowdam += (nowdam*(100 + user->m_CriAddPercent.d1 - target->m_CriAddPercent.d2)) / 100;
}
BOOL cSkillManager::CountCri(cPetData* user, cPetData* target)
{
	int cri = user->m_CriRate.d1 - target->m_CriRate.d2;
	if ((rand() % 100) < cri)
	{
		user->m_FightProperty.m_bCri = TRUE;
	}
	else
	{
		user->m_FightProperty.m_bCri =  FALSE;
	}
	return user->m_FightProperty.m_bCri;
}
//��������
BOOL cSkillManager::CountCriMagic(cPetData* user, cPetData* target, int& nowdam)
{
	BOOL bCri = FALSE;
	int cri = user->m_MagicCriRate.d1 - target->m_MagicCriRate.d2;
	if ((rand() % 100) < cri)
	{
		bCri = TRUE;
		nowdam += nowdam;
	}
	return bCri;
}

BOOL cSkillManager::CountParry( cPetData* pUserData,cPetData* pTargetData)
{
	int Parry = pTargetData->m_ParryRate.d1 - pUserData->m_ParryRate.d2;
	Parry -= pTargetData->m_FightProperty.SufferNum * 5;
	if ( rand() % 100 < Parry)
	{
		pTargetData->m_FightProperty.m_bDefend = TRUE;
		return TRUE;
	}
	else
	pTargetData->m_FightProperty.m_bDefend = FALSE;
	return FALSE;
}

BOOL cSkillManager::CountProtect(int& dam, cObj* pUser, cObj* pTarget)
{
	cPetData* pUserData;
	cPetData* pTargetData;
	pUser->GetDateP(pUserData);
	pTarget->GetDateP(pTargetData);
	int guardlistsize = pTargetData->m_FightProperty.m_guardID.size();
	if (guardlistsize)
	{
		int target2 = pTargetData->m_FightProperty.m_guardID[0];
		cObj* pTarget2 = m_pFightState->m_pList[target2]->m_pData;
		cPetData* pTarget2Data;
		pTarget2->GetDateP(pTarget2Data);
		//int& target2hp = pTarget2Data->m_HP;
		int dam3 = dam / 10;
		int dam2= dam3 * 7;
		dam = dam3 * 2;
		AddHp(target2, -dam2);
		SetDamage(dam2, target2);
		m_pFightState->SetSuffer(target2, pUser->GetDirection(),-1);
		return TRUE;
	}
	return FALSE;
}



BOOL cSkillManager::StageWaitProtect(int UserID,int retStage)
{
	cObj* pTarget = m_pFightState->m_pList[UserID]->m_pData;
	cPetData* pTargetData;
	pTarget->GetDateP(pTargetData);
	int targetID = pTargetData->m_FightProperty.m_guardID[0];

	if (0 == m_pFightState->m_pList[targetID]->m_work.size())
	{
		m_pFightState->m_pList[UserID]->GetWork()->stage	= retStage;
	}
	return TRUE;
}

BOOL cSkillManager::StageWaitFightBack(int UserID, int TargetID, int retStage)
{
	if (0 == m_pFightState->m_pList[TargetID]->m_work.size())
	{
		m_pFightState->m_pList[UserID]->GetWork()->stage = retStage;
	}
	return TRUE;
}

BOOL cSkillManager::StageWaitReturn(int UserID,  int retStage)
{
	if (m_pFightState->ReturnApproach(UserID))
	{
		if (-1 == retStage)
		{
			m_pFightState->m_pList[UserID]->m_work.pop_back();
			return TRUE;
		}
		m_pFightState->m_pList[UserID]->GetWork()->stage = retStage;
	}
	return TRUE;
}

BOOL cSkillManager::CheckFanZheng( int UserID, int TargetID, int& dam)
{
	cObj* pUser = m_pFightState->m_pList[UserID]->m_pData;
	cObj* pTarget = m_pFightState->m_pList[TargetID]->m_pData;
	cPetData* pUserData;
	cPetData* pTargetData;
	pUser->GetDateP(pUserData);
	pTarget->GetDateP(pTargetData);
	if (rand() % 100 < pTargetData->m_StabRate.d1-pUserData->m_StabRate.d2)
	{
		//���㷴���˺�
		int dam3 = (dam *(pTargetData->m_StabDamageRate.d1-pUserData->m_StabDamageRate.d2)) / 100;
		if (dam3 < 1)dam3 = 1;
		AddHp(UserID, -dam3);
		SetDamage(dam3, UserID);
		m_pFightState->SetSuffer(UserID, pTarget->GetDirection(), UserID);
		return TRUE;
	}
	return FALSE;
}

void cSkillManager::SuckBlood(int dam, int UserID, cPetData* target)
{
	if (target->m_race == 6 || target->m_race == 5)return;//��ͻ�е���ᱻ��Ѫ
	cObj* pUser= m_pFightState->m_pList[UserID]->m_pData;
	cPetData* pUserData;
	pUser->GetDateP(pUserData);
	int suckrate = pUserData->m_SuckBloodRate.d1 - target->m_SuckBloodRate.d2;
	if (suckrate > 0)
	{
	//	int userhp = pUserData->m_HP;
		int suckblooddam = (dam*suckrate) / 100;
		if (!suckblooddam)return;
		SetDamage(-suckblooddam, UserID);
		AddHp(UserID, suckblooddam);
	}
}

void cSkillManager::CheckPoision(int UserID, int TargetID)
{
	cObj* pUser = m_pFightState->m_pList[UserID]->m_pData;
	cPetData* pUserData;
	pUser->GetDateP(pUserData);
	cObj* pTarget = m_pFightState->m_pList[TargetID]->m_pData;
	cPetData* pTargetData;
	pTarget->GetDateP(pTargetData);
	if (pTargetData->m_race == 5 || pTargetData->m_race == 6)return;//��е�͹����ж�
	int Poision = pUserData->poisonRate.d1 - pUserData->poisonRate.d2; //�ж���
	if (rand() % 100 < Poision) 
	{
		sBuff buf;
		buf.damage = pUserData->m_DamageAddPercentPoision.d1 - pTargetData->m_DamageAddPercentPoision.d2;//�ж��˺�����
		buf.times = 1 + rand() % 3;
		g_pMainState->m_SkillManager.SetDateBuffUp(UserID, TargetID, 50,buf);
	}
}
BOOL cSkillManager::ReMoveMainSkill(int targetID, int MainSkillID)
{
	if (INDEXTYEP_CHARACTER != g_pMainState->m_FightList[targetID].m_pData->m_IndexType)return FALSE;
	cCharacter* pc = (cCharacter*)g_pMainState->m_FightList[targetID].m_pData->m_pIndex;
	//cPCData& pcDate = pc->m_PCData;
	sSkill* pSkill = &m_MengPaiManager.m_SkillList[MainSkillID];
	//int num = pc->m_PCData.m_SkillEnablelist.size();

	for (vector<sSkill*>::iterator Iterator = pc->m_PCData.m_SkillEnablelist.begin(); Iterator != pc->m_PCData.m_SkillEnablelist.end();)
	{
		if (pSkill->m_Id == (*Iterator)->m_Id)
		{
			pc->m_PCData.m_SkillEnablelist.erase(Iterator);
			return TRUE;
		}
		Iterator++;
	}
	return TRUE;
}
BOOL cSkillManager::SetMainSkill(int targetID, int MainSkillID)
{
	if (INDEXTYEP_CHARACTER != g_pMainState->m_FightList[targetID].m_pData->m_IndexType)return FALSE;
	cCharacter* pc= (cCharacter*)g_pMainState->m_FightList[targetID].m_pData->m_pIndex;
	//cPCData& pcDate = pc->m_PCData;
	sSkill* pSkill = &m_MengPaiManager.m_SkillList[MainSkillID];
	//�Ƿ��Ѿ�����
	int num = pc->m_PCData.m_SkillEnablelist.size();
	for (int i = 0; i < num; i++)
	{
		if (pSkill->m_Id == pc->m_PCData.m_SkillEnablelist[i]->m_Id)
			return TRUE;
	}

	pc->m_PCData.m_SkillEnablelist.push_back(pSkill);

	return TRUE;
}

//                                   ��ɫID        ����ID		����λ��
int cSkillManager::SetPetSkill(cPetData* pTarget, int PetSkillID, int Pos)
{
	if (!pTarget)return -1;
	//��ȷ�����ظ�
	for (int i = 0; i < pTarget->m_NumofBBSkill; i++)
	{
		if (PetSkillID == pTarget->m_pPetSkill[i]->m_Id)return i;
	}
	sSkill* pPetSkill = GetPetSkill(PetSkillID);
	for (int k = 0; k < pPetSkill->m_NumUnCompatible; k++)//Ҫ������м��������ݵļ���
	{
		for (int i = 0; i < pTarget->m_NumofBBSkill; i++)
		{//ͬ���ļ��ܲ��ܴ���,���ܹ���ļ���Ҳ���ܴ���
			if ((pPetSkill->m_UnCompatible[k] == pTarget->m_pPetSkill[i]->m_Id))
			{
				if (Pos == i)continue;
				return i;
			}
		}
	}
	//�滻��������
	if (Pos < pTarget->m_NumofBBSkill)
	{
		SetPetSkill1(pTarget, pTarget->m_pPetSkill[Pos]->m_Id, -1);
		pTarget->m_pPetSkill[Pos] = pPetSkill;
	}
	else
	{
		pTarget->m_pPetSkill[pTarget->m_NumofBBSkill] = pPetSkill;
		pTarget->m_NumofBBSkill += 1;
	}
	SetPetSkill1(pTarget, PetSkillID);
	return -1;
}

void cSkillManager::lvUpFuZhuSkill(int targetID, int FuZhuSkillID, int lv)
{
	cPetData* petDate;
	g_pMainState->m_FightList[targetID].m_pData->GetDateP(petDate);
	petDate->lvUpFuZhuSkill(lv, FuZhuSkillID);
}


void cSkillManager::SetSpecialSkill2(int targetID, int SkillID, int Setup)
{
	cPetData* pcDate;
	g_pMainState->m_FightList[targetID].m_pData->GetDateP(pcDate);
	SetSpecialSkill2(pcDate, SkillID,Setup);
}
void cSkillManager::SetSpecialSkill2(cPetData* p, int SkillID,int Setup)
{//��ʯЧ�� 
	if (SkillID == -1)return;
	int k = 1;
	int num = m_SpecialSkill2[SkillID].m_Cost;
	if (!(1==Setup))
	{
		k = -1;
		num = -num;
	}
	switch (SkillID)
	{
	case 0:p->m_HPMax.AddDataAddPercent(num); 
		if (g_pCharacter)
		if (p->m_Name == g_pCharacter->m_PCData.m_Name)g_pMainState->m_PCHead.UpdatePcHp(); return;
	case 1:p->m_MPMax.AddDataAddPercent(num); 
		if (g_pCharacter)
		if (p->m_Name == g_pCharacter->m_PCData.m_Name)g_pMainState->m_PCHead.UpdatePcMp(); return;
	case 2:p->m_LiveHpLv += num; return;
	case 3:p->m_LiveMpLv += num; return;
	case 4:p->m_LiveCp += num; return;
	case 5:
		p->m_Spurting.d2 +=num;
		p->nightLv += num; return;
	case 6:p->restoreRate += num; return;
	case 7:p->abnormalResist += num; return;
	case 8:p->magicWavePercent += num; return;
	case 9:p->comboNumDown += k; p->m_DEF.AddDataAddPercent(-num); return;
	case 10:p->ShiXueAdd += num; return;
	case 11:p->buffAdd += num; return;
	case 12:p->m_ATK.AddDataAddPercent(num); return;
	case 13:p->m_HIT.AddDataAddPercent(num); return;
	case 14:p->m_DEF.AddDataAddPercent(num); return;
	case 15:p->m_Wakan.AddDataAddPercent(num); return;
	case 16:p->m_SPD.AddDataAddPercent(num); return;
	case 17:p->m_Evade.AddDataAddPercent(num); return;
	case 18:p->GoldLivePercent.d1+=num; return;//���
	case 19:p->GoldLivePercent.d2+=num; return;
	case 20:p->m_CriRate.d1 += num; return;
	case 21:p->m_CriRate.d2 += num; return;
	case 22:p->m_MagicCriRate.d1 += num; return;
	case 23:p->m_MagicCriRate.d2 += num; return;
	case 24:p->m_CureCriRate.d1 += num; return;
	case 25:p->m_CureCriRate.d2 += num; return;
	case 26:p->m_CriAddPercent.d1 += num; return;
	case 27:p->m_Combo.d1 += num; return;
	case 28:p->m_MagicComboRate.d1 += num; return;
	case 29:p->m_Pierce.d1 += num; return;
	case 30:p->m_PierceMagic.d1 += num; return;
	case 31:p->m_HitRate.d1 += num; return;		
	case 32:p->m_HitRate.d2 += num; return;
	case 33:p->m_MagicHitRate.d1 += num; return;
	case 34:p->m_MagicHitRate.d2 += num; return;
	case 35:p->m_SealRate.d1 += num; return;
	case 36:p->m_SealRate.d2 += num; return;
	case 37:p->poisonRate.d1 += num; return;
	case 38:p->poisonRate.d2 += num; return;
	case 39:p->m_DamageAddPercentMagic2.d1 += num; return;
	case 40:p->m_DamageAddPercentMagic2.d2 += num; return;
	case 41:p->m_DamageAddPercentMagic.d1 += num; return;
	case 42:p->m_DamageAddPercentMagic.d2 += num; return;
	case 43:p->m_DamagePercent.d1 += num; return;
	case 44:p->m_DamagePercent.d2 += num; return;
	case 45:p->m_HiddenWeaponDamagePercent.d1 += num; return;
	case 46:p->m_HiddenWeaponDamagePercent.d2 += num; return;
	case 47:p->m_DamageAddPercentPoision.d1 += num; return;
	case 48:p->m_DamageAddPercentPoision.d2 += num; return;
	case 49:p->m_StabRate.d1 += num; return;
	case 50:p->m_StabRate.d2 += num; return;
	case 51:p->m_StabDamageRate.d1 += num; return;
	case 52:p->m_StabDamageRate.d2 += num; return;
	case 53:p->m_StabRateMagic.d1 += num; return;
	case 54:p->m_StabRateMagic.d2 += num; return;
	case 55:p->m_StabDamageRateMagic.d1 += num; return;
	case 56:p->m_StabDamageRateMagic.d2 += num; return;
	case 57:p->m_SuckBloodRate.d1 += num; return;
	case 58:p->m_SuckBloodRate.d2 += num; return;
	case 59:p->m_MagicFire.d1 += num; return;
	case 60:p->m_MagicFire.d2 += num; return;
	case 61:p->m_MagicSoil.d1 += num; return;
	case 62:p->m_MagicSoil.d2 += num; return;
	case 63:p->m_MagicWater.d1 += num; return;
	case 64:p->m_MagicWater.d2 += num; return;
	case 65:p->m_MagicThunder.d1 += num; return;
	case 66:p->m_MagicThunder.d2 += num; return;
	case 67:p->ghostSpecial.d1 += num; return;
	case 68:p->PeopleSpecial.d1 += num; return;
	case 69:p->MonsterSpecial.d1 += num; return;
	case 70:p->ImmortalSpecial.d1 += num; return;
	case 71:p->machineSpecial.d1 += num; return;
	case 72:p->m_AtkDownHPHurt.d1 += num; return;
	case 73:p->m_AtkDownHPHurt.d2 += num; return;
	case 74:p->m_MagDownHPHurt.d1 += num; return;
	case 75:p->m_MagDownHPHurt.d2 += num; return;
	case 76:p->m_AtkDownMP.d1 += num; return;
	case 77:p->m_AtkDownMP.d2 += num; return;
	case 78:p->m_AtkDownAtk.d1 += num; return;
	case 79:p->m_AtkDownDef.d1 += num; return;
	case 80:p->m_AtkDownSpd.d1 += num; return;
	case 81:p->m_AtkDownWakan.d1 += num; return;
	case 82:p->m_AtkDownHit.d1 += num; return;
	case 83:p->m_Spurting.d1 += num; return;
	case 84:p->m_AtkDownCP.d1 += num; return;
	case 85:p->m_AtkDownCP.d2 += num; return;
	case 86:if (num == 1) p->m_StartMagic.d1 = 145; else 0; return;
	case 87:if (num == 1) p->m_StartMagic.d1 = 81; else 0; return;
	case 88:if (num == 1) p->m_StartMagic.d1 = 147; else 0; return;
	}	
}
void cSkillManager::EffectSkill(int Skilllv, int sSkillID, int UserID, int TargetID)
{
	if (TargetID == -1)return;
	sFightList* pUserList = m_pFightState->m_pList[UserID];
	cObj* pTarget = m_pFightState->m_pList[TargetID]->m_pData;
	cObj* pUser = m_pFightState->m_pList[UserID]->m_pData;
	cPetData* pTargetData;
	cPetData* pUserData;
	sBuff buf;
	buf.times = -999;

	pTarget->GetDateP(pTargetData);
	pUser->GetDateP(pUserData);
	int& userlv = pUserData->m_LV;
	int& targetlv = pTargetData->m_LV;
	int dam = 0;

	switch (sSkillID)
	{
	case 50:return;
	case 2095:return;
	case 1050:
		CountphysicalDamage(dam, UserID, TargetID, 30);
		if (rand() % 2)
		{
			buf.times = 2+rand()%3;
			SetDateBuffUp(UserID, TargetID, 50, buf, Skilllv);
			buf.times = -999;
		}
		break;
	case 1095://����

	{
				  int atkadd = pTargetData->m_DEF.GetData() - pTargetData->m_ATK.GetData();
				  if (atkadd < 0)atkadd = 0;
				  pUserData->m_ATK.AddDataAdd(atkadd);
				  CountphysicalDamage(dam, UserID, TargetID, 30);
				  pUserData->m_ATK.AddDataAdd(-atkadd);
	}
		break;
	case 2101://�ռ���
		
		{
			pUserData->m_PierceMagic.d1 +=100;
			CountMagicDamage(dam, UserID, TargetID, 300,eMWX��);
			pUserData->m_PierceMagic.d1 -= 100;
		}
		break;
	case 3040://����
		CountphysicalDamage(dam, UserID, TargetID, 0);
		AddMp(TargetID,-dam);
		break;
	case 3035://����
		pUserData->m_Pierce.d1 += 50;
		CountphysicalDamage(dam, UserID, TargetID, 0);
		pUserData->m_Pierce.d1 -= 50;
		break;
	case 3043://����EX
		pUserData->m_Pierce.d1 += 80;
		CountphysicalDamage(dam, UserID, TargetID, 0);
		pUserData->m_Pierce.d1 -= 80;
		break;
	case 3034://��Ѫ
	case 3042://��Ѫ2
	case 3044://��ѪEX
		CountphysicalDamage(dam, UserID, TargetID, 10);
		break;
	case 147://����
		buf.times = 5;
		break;
	case 1007://Ǭ����
	
		{
			cObj* target = g_pMainState->m_FightList[TargetID].m_pData;
			string targetname = target->m_TrueName.GetString();
			if (0 == targetname.compare(g_pMainState->m_taskSM.m_Npc.m_name))
			{
				cPetData* pData;
				target->GetDateP(pData);
				if (pData->m_HP < (pData->m_HPMax.GetData()) / 2)
				{
					if (rand() % 3 < 2)
					{
						dam = pData->m_HP;
						AddHp(TargetID, -dam);
						SetDamage(dam, TargetID);
						g_pMainState->m_taskSM.m_bXiangFuSuccess = TRUE;
					}
				}

			}
		}
		break;
	case 0://��ɨǧ��
		
		{
			int damageup;
			switch (pUserList->m_NowTarget)
			{
			case 0:
				damageup = 0;
				break;
			case 1:// �����߹���-Ŀ����� 120%���˺�
				damageup = 20;
				break;
			case 2:// �����߹���-Ŀ�����  ����10%�Ĳ���  150%���˺�
				damageup = 50;
				break;
			default:
				damageup = 70;
				break;
			}
			CountphysicalDamage(dam, UserID, TargetID, damageup);
		}
		break;
	case 1:
		SetDateBuffUp(TargetID, UserID, sSkillID, buf, Skilllv);
		buf.times = -999;
		break;
	case 7:
	
		{
			CountphysicalDamage(dam, UserID, TargetID, -50);		
		}
		break;
	case 1017:
		CountphysicalDamage(dam, UserID, TargetID, 50);
		break;
	case 8://�����
		buf.times = 15;
		break;
	case 1010:
		
		{
			CountphysicalDamage(dam, UserID, TargetID, 120);
			pUser->ReMoveStateID(1);
		}
		break;
	case 4://ɱ����
		buf.times = 5;
		break;
	case 11://������
	case 28://���׷�
	
		{
			pUserData->ghostSpecial.d1 += 100;
			CountMagicDamage(dam, UserID, TargetID, 20 + Skilllv / 5 - (m_pFightState->m_pList[UserID]->m_MaxTargetNum - 1) * 5,eMWX��);
			pUserData->ghostSpecial.d1 -= 100;
		}
		break;
	case 13:
		
		{
			m_pFightState->m_pList[UserID]->isLive = FALSE;
			if (UserID == g_pMainState->m_HeroID)
				m_pFightState->m_bEscape = TRUE;
		}
		break;
	case 6: //�����
	case 14://���߷�
	case 15://ʧ�ķ�
	case 16://����
	case 17://ʧ��
	case 18://׷��
	case 19://���
	case 20://ʧ��
	case 21://����
		buf.times = 4;
		buf.times += (Skilllv - targetlv) / 5;
		break;
	case 22://��Ԫ��
		dam = -Skilllv * 8;
		CorrectCureHP(dam, UserID, UserID);//��������
		AddHp(TargetID, -dam);
		SetDamage(dam, TargetID);
		break;
	case 23://������
		AddMp(TargetID, (Skilllv * 2));
		break;
	case 26://������
		buf.times = 5;
		buf.times += (Skilllv - targetlv) / 5;
		break;
	case 27://��׷�
		buf.times = 5;
		buf.times += (Skilllv - targetlv) / 5;
		break;
	case 31://��������
	
		{
			pUserData->m_Wakan.AddDataOriginal(pUserData->m_DEF.GetData()>>3);
			CountMagicDamage(dam, UserID, TargetID, 15 + Skilllv / 5 - (m_pFightState->m_pList[UserID]->m_MaxTargetNum - 1) * 4);
			pUserData->m_Wakan.AddDataOriginal(-pUserData->m_DEF.GetData()>>3);
		}
		break;
	case 33:  //��ջ���
	
		{
			buf.times = 5;
			buf.times += (Skilllv - targetlv) / 5;
		}
		break;
	case 34:    //Τ�ӻ���  
	case 35:  //��ջ���
	case 36:     //һέ�ɽ�
		
		{
			buf.times = 5;
			buf.times += (Skilllv - targetlv) / 5;
			break;
		}
	case 37:     //�ҷ�ȱ�
		Resurgence(TargetID);
		AddHpHurt(TargetID, (Skilllv * 5));
		dam = Skilllv * 3;
		CorrectCureHP(dam, UserID, TargetID);//��������
		break;
	case 38:     //����
		dam = Skilllv * 10;
		CorrectCureHP(dam, UserID, TargetID);//��������
		AddHpHurt(TargetID, dam);
		break;
	case 39:    //��Ѫ
		dam = Skilllv * 12;
		CorrectCureHP(dam, UserID, TargetID);//��������
		
		break;
	case 40:    //��������
		dam = Skilllv * 7;
		CorrectCureHP(dam, UserID, TargetID);//��������
		break;
	case 41:    //���ֻش�
		AddHpHurt(TargetID, (Skilllv * 7));
		break;
	case 42:    //��������
		AddHpHurt(TargetID, (Skilllv * 6));
		dam = Skilllv * 6;
		CorrectCureHP(dam, UserID, TargetID);//��������
		break;
	case 43://�ⶾ
		pTarget->ReMoveState(eBuffType��);
		break;
	case 45://����
		AddHpHurt(TargetID, 100, 10000);
		dam = -AddHp(TargetID, 100, 10000);
		SetDamage(dam, TargetID);
		break;
	case 46:    //���ޱ�
		
		{
			buf.times = 5;
			buf.times += (Skilllv - targetlv) / 5;
		}
		break;
	case 49:    //��������
		
		{
			buf.times = 5;
			buf.times += (Skilllv - targetlv) / 5;
		}
		break;
	case 54:    //��������
		
		{
			buf.times = 5;
			buf.times += (Skilllv - targetlv) / 5;
		}
		break;
	case 59:    //һЦ���
		
		{
			buf.times = 3;
			buf.times += (Skilllv - targetlv) / 5;
		}
		break;
	case 63:    //���񻤷�
		
		{
			buf.times = 8;
			buf.times += (Skilllv - targetlv) / 5;
		}
		break;
	case 66:	//����
		pUser->ReMoveState(eBuffType����);
		break;
	case 67:    //���� 
		
		{
			buf.times = 6;
			buf.times += (Skilllv - targetlv) / 5;
		}
		break;
	case 68:    //����
	case 69:    //�������
		
		{
			buf.times = 6;
			buf.times += (Skilllv - targetlv) / 5;
		}
		break;
	case 71://����ն
		CountphysicalDamage(dam, UserID, TargetID, 30);
		break;
	case 72://���׺䶥
		
		{
			int rate = (pUserData->m_XiuLian.GetMag() - pTargetData->m_XiuLian.GetDMag())*2;
			int offset = Skilllv - pTargetData->m_LV;
			rate = 70 + rate + offset;
			if (rand() % 100 < rate)
				dam = pTargetData->m_HP / 4;
			else dam = pTargetData->m_HP / 20;
			if (dam > (pUserData->m_LV * 50))dam = pUserData->m_LV * 50;
			AddHp(TargetID, -dam);
			SetDamage(dam, TargetID);
		}
		break;
	case 74://�������
		CountMagicDamage(dam, UserID, TargetID, 20 + Skilllv / 5 - (m_pFightState->m_pList[UserID]->m_MaxTargetNum - 1) * 8,eMWX��);
		break;
	case 119://������
	case 77: //���
		
		{
			m_pFightState->ReMoveSealState(TargetID);
		}
		break;
	case 79://����
		CountMagicDamage(dam, UserID, TargetID, 40 + Skilllv / 5,eMWXˮ);
		break;
	case 80://�������
		CountMagicDamage(dam, UserID, TargetID, 25 + Skilllv / 5 - (m_pFightState->m_pList[UserID]->m_MaxTargetNum - 1) * 4,eMWX��ˮ);
		break;
	case 81://����
		buf.times = 7;
		break;
	case 84: //����
		dam = Skilllv + 1;
		CountMagic2Damage(dam, UserID, TargetID, eMWXˮ);
		AddMp(TargetID, -dam);
		break;
	case 88://����
		CountMagicDamage(dam, UserID, TargetID, 30 + Skilllv / 5 - (m_pFightState->m_pList[UserID]->m_MaxTargetNum - 1) * 4,eMWXˮ);
		break;
	case 89://����Ǭ��
		
		{
			buf.times = 4;
			buf.times += (Skilllv - targetlv) / 5;
		}
		break;
	case 90: //��������
		
		{
			buf.times = 5;
			buf.times += (Skilllv - targetlv) / 5;
			dam = Skilllv * 4;
			CorrectCureHP(dam, UserID, TargetID);
			buf.damage = dam;
		}
		break;
	case 91://����֮Ȫ
		
		{
			buf.times = 5;
			buf.times += (Skilllv - targetlv) / 5;
			dam = Skilllv * 5;
			CorrectCureHP(dam, UserID, TargetID);//��������
			buf.damage = dam;
		}
		break;
	case 94:   //�����۶�
		dam = (Skilllv * 8 + 10);
		CorrectCureHP(dam, UserID, TargetID);
		break;
	case 97: //����
		CountphysicalDamage(dam, UserID, TargetID, 0);
		break;
	case 102://�ն�����
		buf.times = 5;
		buf.times += (Skilllv - targetlv) / 5;
		dam = Skilllv * 7;
		CorrectCureHP(dam, UserID, TargetID);//��������
		buf.damage = dam;
		buf.damage2 = dam;
		AddHpHurt(TargetID, -dam);
		break;
	case 103://������
		buf.times = 7;
		buf.times += (Skilllv - targetlv) / 5;
		buf.damage = ((Skilllv * 2 + 1)* (100 + pUserData->m_DamageAddPercentMagic.d1)) / 100;
		break;
	case 104://������¶
		Resurgence(TargetID);
		AddHpHurt(TargetID, Skilllv * 10);
		dam = Skilllv * 12;
		CorrectCureHP(dam, UserID, TargetID);//��������
		break;
	case	105:// �չ⻪
		dam = Skilllv * 9 + 30;
		CountMagic2Damage(dam, UserID, TargetID);
		break;
	case	106:// ��׺�
		dam = Skilllv * 7 + 30;
		CountMagic2Damage(dam, UserID, TargetID);
		if (rand() % 20 == 0)
		{
			m_pFightState->m_pList[TargetID]->m_WorkType = -1;
		}
		break;
	case	107:// ������
		dam = Skilllv * 7 + 30;
		CountMagic2Damage(dam, UserID, TargetID);
		if (rand() % 8 == 0)
		{
			AddCp(TargetID, -20);
		}
		break;
	case	108:// ��ã��
		dam = Skilllv * 7 + 30;
		CountMagic2Damage(dam, UserID, TargetID);
		if (rand() % 10 == 0)
		{
			AddMp(UserID, -dam / 10);
		}
		break;
	case	109:// ���ѻ�
		dam = Skilllv * 7 + 30;
		CountMagic2Damage(dam, UserID, TargetID);
		if (rand() % 20 == 0)
		{
			AddHpHurt(TargetID, -dam / 10);
		}
		break;
	case 112://�鶯
		buf.times = 5;
		buf.times += (Skilllv - targetlv) / 5;
		break;
	case 113://���еߵ�
		buf.times = 5;
		buf.times += (Skilllv - targetlv) / 5;
		break;
	case 115://�����ķ�
		
		{
			if (pTarget->m_StateList.size())
			for (list<sBuff>::iterator Iterator = pTarget->m_StateList.begin(); Iterator != pTarget->m_StateList.end();)
			{
				if (Iterator->ID == 102 || Iterator->ID == 91)
				{
					list<sBuff>::iterator it2 = Iterator;
					Iterator++;
					if (Iterator->ID == 91)
					{
						int hpadd = it2->damage2*it2->times;
						AddHpHurt(TargetID, hpadd);
					}
					dam = -it2->damage*it2->times;
					AddHp(TargetID, -dam);
					g_pMainState->m_SkillManager.SetDateBuffDown(it2);// it2->targetID, it2->ID);
					it2->data.Free();
					pTarget->m_StateList.erase(it2);
					break;
				}
				else 	if (Iterator->ID == 90)
				{
					list<sBuff>::iterator it2 = Iterator;
					Iterator++;
					int mpadd = it2->damage*it2->times;
					AddMp(TargetID, mpadd);
					g_pMainState->m_SkillManager.SetDateBuffDown(it2);// it2->targetID, it2->ID);
					it2->data.Free();
					pTarget->m_StateList.erase(it2);
					break;
				}
			}

		}
		break;
	case 116://������
		dam = Skilllv * 7 + 30;
		CountMagic2Damage(dam, UserID, TargetID);
		AddHpHurt(TargetID, -dam / 10);
		break;
	case 117://�й���
		dam = Skilllv * 10 + 50;
		CountMagic2Damage(dam, UserID, TargetID);
		AddMp(TargetID, -dam / 10);
		break;
	case 120://����
		buf.times = 5;
		buf.times += (Skilllv - targetlv) / 25;
		break;
	case 122://ʬ����
		
		{
			if (pTargetData->m_race == 5 || pTargetData->m_race == 6)return ;//��е�͹����ж�
			int Poision = pUserData->poisonRate.d1 - pUserData->poisonRate .d2+ 80; //�ж���
			if (rand() % 100 < Poision)
			{
				sBuff buf;
				buf.damage = pUserData->m_DamageAddPercentPoision.d1 - pTargetData->m_DamageAddPercentPoision.d2;//�ж��˺�����
				buf.times += (Skilllv - targetlv) / 25;
				g_pMainState->m_SkillManager.SetDateBuffUp(UserID, TargetID, 122, buf);
				dam = Skilllv * 4;
				AddHp(TargetID, -dam);
			}	
		}
		break;
	case 123:   //��������
		buf.times = 5;
		buf.times += (Skilllv - targetlv) / 25;
		break;
	case 125:
		Resurgence(TargetID);
		AddHpHurt(TargetID, Skilllv * 5);
		dam = -Skilllv * 5;
		AddHp(TargetID, dam);
		buf.times = 5;
		buf.times += (Skilllv - targetlv) / 25;
		break;
	case 126:
		buf.times = 5;
		buf.times += (Skilllv - targetlv) / 25;
		break;
	case 127:// �����ɢ
		pTarget->ReMoveState(eBuffType����);
		break;
	case 131://��ɰ��ʯ
		CountMagicDamage(dam, UserID, TargetID, 25 + Skilllv / 4 - (m_pFightState->m_pList[UserID]->m_MaxTargetNum - 1) * 4);
		break;
	case	132:// �������
		CountMagicDamage(dam, UserID, TargetID, 70 + Skilllv / 4);
		break;
	case 133://ţ��
		buf.times = 10;
		break;
	case 141://����
		CountphysicalDamage(dam, UserID, TargetID, 15);
		pUser->ReMoveStateID(145);//ȡ������
		buf.times = 1;
		SetDateBuffUp(UserID, TargetID, sSkillID, buf);
		buf.times = -999;
		break;
	case 142://ӥ�� 
		CountphysicalDamage(dam, UserID, TargetID, 10);
		if (pUserList->m_NowTarget == 0)
		{
			buf.times = 1;
			SetDateBuffUp(UserID, UserID, sSkillID, buf);
		}
		buf.times = -999;
		break;
	case	143:// ʨ��
		CountphysicalDamage(dam, UserID, TargetID, 20);
		break;
	case	144:// ����
		buf.times = 5;
		buf.times += (Skilllv - targetlv) / 25;
		break;
	case	145://����
		buf.times = 5;
		buf.times += (Skilllv - targetlv) / 3;
		break;
	case 148:// ������
		CountphysicalDamage(dam, UserID, TargetID, 10 - pUserList->m_NowTarget * 5);
		if (pUserList->m_NowTarget == 0)
		{
			buf.times = 1;
			SetDateBuffUp(UserID, UserID, sSkillID, buf);
			buf.times = -999;
		}
		break;
	case 150://���ȷ��
		buf.times = 5;
		buf.times += (Skilllv - targetlv) / 25;
		break;
	case 152:
		buf.times = 5;
		buf.times += (Skilllv - targetlv) / 25;
		break;
	case 154://����
		dam = -AddHp(TargetID, -5, userlv * 200);
		dam += -AddHp(TargetID, -Skilllv * 4);
		SetDamage(dam,TargetID);
		dam /= 2;
		CorrectCureHP(dam, UserID, UserID);//��������
		break;
	case 155://����
		dam = -AddMp(TargetID, -5, userlv * 100);
		dam += -AddMp(TargetID, -Skilllv * 2);
		dam /= 2;
		CorrectCureMP(dam, UserID, UserID);//��������
		dam = 0;
		break;
	case 156://ħ�����
		buf.times = 5;
		buf.times += (Skilllv - targetlv) / 25;
		break;
	case 159://��������
		buf.times = 5;
		buf.times += (Skilllv - targetlv) / 25;
		break;
	case 160:// ��˿��
		buf.times = 5;
		buf.times += (Skilllv - targetlv) / 25;
		break;
	case 161: //����
		m_pFightState->ReMoveSealState(UserID);
		m_pFightState->ReMoveDeBuffState(UserID);
		m_pFightState->ReMovePoisionState(UserID);
		break;
	case 162:  //���޵���
		dam = Skilllv * 9 + (pUserData->m_ATK.GetData()/4);
		CountMagic2Damage(dam, UserID, TargetID);
		if (rand() % 2 == 0)
		{
			buf.times = rand() % 3 + 1;
		}
		break;
	case 164://����ͬ��
		dam = AddMp(TargetID, -10, userlv * 100);
		dam += AddMp(TargetID, Skilllv * 5);
		dam = 0;
		break;
	case 166://�þ���
		buf.times = 1;
		buf.times += (Skilllv - userlv) / 5;
		break;
	case 1001://ץ��
		
		{
			if (!pTargetData->m_Controler.CheckGrab())
			{
				g_pMainState->m_Channel.AddNewString("�ö����޷�ץ��");
				return ;
			}
			cCharacter* pc1 = (cCharacter*)pUser->m_pIndex;
			if (pc1->m_PCData.m_NumofBB > 7)
			{
				g_pMainState->m_Tags.Add("�ٻ��������ﵽ����");
				break;
			}
			if ((rand() % 100) > 50)
			{
				
				m_pFightState->BackFromFight(TargetID,FALSE);
				cPet* pPet = (cPet*)pTarget->m_pIndex;
				pc1->m_PCData.GetPet(pPet);
				pTarget->m_pIndex = 0;
				pTarget->m_IndexType = 0;
			}
		}
		break;
	case 2084:  //�׻�
		CountMagicDamage(dam, UserID, TargetID, 30,eMWX��);
		break;
	case 2085://������
		CountMagicDamage(dam, UserID, TargetID, 35 - pUserList->m_NowTarget * 5,eMWX��);
		break;
	case 2086: //����
		CountMagicDamage(dam, UserID, TargetID, 25,eMWX��);
		if (rand() % 5 == 0)
		{
			AddCp(TargetID, -20);
		}
		break;
	case 2087: //̩ɽѹ��
		CountMagicDamage(dam, UserID, TargetID, 30 - pUserList->m_NowTarget * 5,eMWX��);
		if (rand() % 10 == 0)
		{
			AddCp(TargetID, -20);
		}
		break;
	case 2088: //ˮ��
		CountMagicDamage(dam, UserID, TargetID, 25,eMWXˮ);
		if (rand() % 15 == 0)
		{
			m_pFightState->m_pList[TargetID]->m_WorkType = -1;
		}
		break;
	case 2089: //ˮ����ɽ
		CountMagicDamage(dam, UserID, TargetID, 30 - pUserList->m_NowTarget * 5,eMWXˮ);
		if (rand() % 30 == 0)
		{
			m_pFightState->m_pList[TargetID]->m_WorkType = -1;
		}
		break;
	case 2090: //�һ�
		CountMagicDamage(dam, UserID, TargetID, 25,eMWX��);
		if (rand() % 10 == 0)
		{
			AddHpHurt(TargetID, -dam / 10);
		}
		break;
	case 2091: //�����һ�
		CountMagicDamage(dam, UserID, TargetID, 30 - pUserList->m_NowTarget * 5,eMWX��);
		if (rand() % 20 == 0)
		{
			AddHpHurt(TargetID, -dam / 10);
		}
		break;
	case 2092://����һ��
		CountphysicalDamage(dam, UserID, TargetID, -10);
		buf.times = 1;
		break;
	case 2094://�����ٻ�
		CountphysicalDamage(dam, UserID, TargetID, 0);
		if (rand() % 100<15)
			buf.times = 15;
		break;
	case 2093://ҹ�����
		buf.times = 10;
		break;
		//case 2095:break;//������ɽ
	case 2097:// ��������
		buf.times = 6;
		break;
	case 2098://�ƶ��б�
		
		{
			int rate;
			if (rand() % 100 > 10+ pTargetData->m_Popularity/100 )
				rate = 100;
			else
				rate = -150;
			CountphysicalDamage(dam, UserID, TargetID, rate);
		}
		break;
	case 2099://���ݻ���
		CountphysicalDamage(dam, UserID, TargetID, 10);
		break;
	case 2100://���ŭ��
		CountphysicalDamage(dam, UserID, TargetID, 300);
		break;
	case 3000: //����
	
		{
			dam = CountHPPercent(pTargetData, 3, userlv);
			dam += 200;
			CorrectCureHP(dam, UserID, TargetID);//��������
		}
		break;
	case 3001: //����
		
		{
			dam = CountHPPercent(pTargetData, 6, userlv);
			dam += 400;
			CorrectCureHP(dam, UserID, TargetID);//��������
		}
		break;
	case 3002: //����
	
		{
			dam = CountHPPercent(pTargetData, 9, userlv);
			dam += 600;
			CorrectCureHP(dam, UserID, TargetID);//��������
		}
		break;
	case 3003: //����
		
		{
			dam = CountMPPercent(pTargetData, 10, userlv);
			dam += 240;
			CorrectCureMP(dam, UserID, TargetID);//��������
		}
		break;
	case 3004:  //����
	
		{
			dam = CountMPPercent(pTargetData, 15, userlv);
			dam += 240;
			CorrectCureMP(dam, UserID, TargetID);//��������
		}
		break;
	case 3006: //����
		dam = CountHPPercent(pTargetData, 50, userlv);
		CorrectCureHP(dam, UserID, TargetID);//��������
		break;
	case 3005:  //����
	case 3007: //�ĺ���ƽ
		dam = CountHPPercent(pTargetData, 25, userlv);
		CorrectCureHP(dam, UserID, TargetID);//��������
		break;
	case 3008://�ػ���
		Resurgence(TargetID);
		dam = CountHPPercent(pTargetData, 20, userlv);
		CorrectCureHP(dam, UserID, TargetID);//��������
		AddHpHurt(TargetID, dam);
		break;
	case 3009: //��������
		Resurgence(TargetID);
		dam = CountHPPercent(pTargetData, 50, userlv);
		CorrectCureHP(dam, UserID, TargetID);//��������
		AddHpHurt(TargetID, dam);
		break;
	case 3012://����
	case 3010://ˮ��
		pTarget->ReMoveState(eBuffType��ӡ);
		pTarget->ReMoveState(eBuffType��);
		pTarget->ReMoveState(eBuffType����);
		break;
	case 3011://����
	case 3013://����
		dam = CountHPPercent(pTargetData, 25, userlv);
		CorrectCureHP(dam, UserID, TargetID);//��������
		pTarget->ReMoveState(eBuffType��ӡ);
		pTarget->ReMoveState(eBuffType��);
		pTarget->ReMoveState(eBuffType����);
		break;
	case 3016://Ц��ص�
		AddCp(TargetID, -70);
		break;
	case 3017://����ħ��
		AddCp(TargetID, -30);
		break;
	case 3018://Ұ��	
		if (!CheckINT(pTargetData->m_FightProperty.m_AtkIntensify, 1))
		{
			SetINT(pTargetData->m_FightProperty.m_AtkIntensify, 1);
			pTargetData->m_ATK.AddDataAddPercent(10);
		}
		break;
	case 3019://ħ��֮ӡ
		if (!CheckINT(pTargetData->m_FightProperty.m_AtkIntensify, 0))
		{
			SetINT(pTargetData->m_FightProperty.m_AtkIntensify, 0);
			pTargetData->m_ATK.AddDataAddPercent(5);
		}
		break;
	case 3020://���
		if (!CheckINT(pTargetData->m_FightProperty.m_DefIntensify, 1))
		{
			SetINT(pTargetData->m_FightProperty.m_DefIntensify, 1);
			pTargetData->m_DEF.AddDataAddPercent(10);
		}
		break;
	case 3021://ʥ��
		if (!CheckINT(pTargetData->m_FightProperty.m_DefIntensify, 0))
		{
			SetINT(pTargetData->m_FightProperty.m_DefIntensify, 0);
			pTargetData->m_DEF.AddDataAddPercent(5);
		}
		break;
	case 3022://����
		if (!CheckINT(pTargetData->m_FightProperty.m_SpdIntensify, 1))
		{
			SetINT(pTargetData->m_FightProperty.m_SpdIntensify, 1);
			pTargetData->m_SPD.AddDataAddPercent(10);
		}
		break;
	case 3023://Х��
		if (!CheckINT(pTargetData->m_FightProperty.m_SpdIntensify, 0))
		{
			SetINT(pTargetData->m_FightProperty.m_SpdIntensify, 0);
			pTargetData->m_SPD.AddDataAddPercent(5);
		}
		break;
	case 3024://̫������
		buf.times = 4;
		break;
	case 3025://�޺�����
		buf.times = 4;
		break;
	case 3026://������
		buf.times = 4;
		break;
	case 3027://�����޷�
		buf.times = 4;
		break;
	case 3028://��������
		if (!CheckINT(pTargetData->m_FightProperty.m_AtkIntensify, 3))
		{
			SetINT(pTargetData->m_FightProperty.m_AtkIntensify, 3);
			pTargetData->m_ATK.AddDataAddPercent(-10);
		}
		break;
	case 3029://��ɽѹ��
		if (!CheckINT(pTargetData->m_FightProperty.m_AtkIntensify, 2))
		{
			SetINT(pTargetData->m_FightProperty.m_AtkIntensify, 2);
			pTargetData->m_ATK.AddDataAddPercent(-5);
		}
		break;
	case 3030://�Ƽ���
		if (!CheckINT(pTargetData->m_FightProperty.m_DefIntensify, 3))
		{
			SetINT(pTargetData->m_FightProperty.m_DefIntensify, 3);
			pTargetData->m_DEF.AddDataAddPercent(-10);
		}
		break;
	case 3031://�����
		if (!CheckINT(pTargetData->m_FightProperty.m_DefIntensify, 2))
		{
			SetINT(pTargetData->m_FightProperty.m_DefIntensify, 2);
			pTargetData->m_DEF.AddDataAddPercent(-5);
		}
		break;
	case 3032://������
		if (!CheckINT(pTargetData->m_FightProperty.m_SpdIntensify, 3))
		{
			SetINT(pTargetData->m_FightProperty.m_SpdIntensify, 3);
			pTargetData->m_SPD.AddDataAddPercent(-10);
		}
		break;
	case 3033://ͣ����
		if (!CheckINT(pTargetData->m_FightProperty.m_SpdIntensify, 2))
		{
			SetINT(pTargetData->m_FightProperty.m_SpdIntensify, 2);
			pTargetData->m_SPD.AddDataAddPercent(-5);
		}
		break;
	case 3046://���֮ǽ
		pTargetData->m_FightProperty.m_WallNum = 1;
		break;
	case 3038://�Ǳ�ʥ��
		pTargetData->m_FightProperty.m_WallNum = 2;
		break;
	case 3039://������EX
		{
			dam= CountHPPercent(pTargetData, 50, userlv);
			dam += 1000;
			CorrectCureHP(dam, UserID, TargetID);//��������
		}
		break;
	case 3047://��������
		buf.times = 5;
		break;
	case 3048://�Ⱥ��ն�
		Resurgence(TargetID);
		AddHpHurt(TargetID, 100, 10000);
		dam = CountHPPercent(pTargetData, 50, userlv<<3);
		CorrectCureHP(dam, UserID, TargetID);//��������
		break;
	case 3049://Ѹ�����
		buf.times = 10;
		break;
	case 2102:
		CountphysicalDamage(dam, UserID, TargetID, 250);
		break;
	default:
		ERRBOX2(sSkillID);
		return;
	}
	if (buf.times != -999)
	{
		if (buf.times < 1)buf.times = 1;
		if (buf.times>1)
		{
			if (UserID<10&&TargetID<10)
				buf.times += pTargetData->buffAdd;
			else
			if (TargetID>9&&UserID>10)
				buf.times += pTargetData->buffAdd;
		}
		SetDateBuffUp(UserID, TargetID, sSkillID, buf, Skilllv);
	}
}

int cSkillManager::AddHp(int target, int num, int percent)
{
	
	cPetData* pData;
	cObj* pObj = g_pMainState->m_FightList[target].m_pData;
	pObj->GetDateP(pData);


	if (g_StateType == STATE_FIGHT)
	{
		if (pData->m_FightProperty.cannotAddHP)return 0;
	}

	int& hp = pData->m_HP;
	if (0 == hp)return 0;
	int& hphurt = pData->m_HPHurtMax;
	int hpadd = num;
	int hpmax= pData->m_HPMax.GetData();
	if (percent)
	{
		hpadd *= hpmax;
		if (abs(hpadd) > abs(percent*num))hpadd = percent*num;
		hpadd /= 100;
	}
	hp += hpadd;
	if (hp > hphurt)hp = hphurt;
	else if (hp < 0)hp = 0;
	

	if (target ==g_pMainState->m_HeroID)
	{
		g_pMainState->m_PCHead.UpdatePcHp();
	}
	else
	if (target == g_pMainState->m_HeroID + 5)
	{
		g_pMainState->m_PCHead.UpdatePetHp();
	}
	if (g_StateType == STATE_MAIN)return hpadd;

	if (hpadd < 0)
	{
		if (pData->m_HP<1)
		{
			g_pMainState->m_FightList[target].SetDie( TRUE);
		}

		int cpadd = abs((100* hpadd) / hpmax);
		pData->m_CP+=cpadd;
		if (pData->m_CP>pData->m_CPMax)pData->m_CP = pData->m_CPMax;
		if (target == g_pMainState->m_HeroID)
		{
			g_pMainState->m_PCHead.UpdatePcCp();
		}
	}

	pObj->UpdateHpLine();
	return hpadd;
}
int cSkillManager::AddMp(int target, int num, int percent)
{
	cPetData* pData;
	cObj* pObj = g_pMainState->m_FightList[target].m_pData;
	pObj->GetDateP(pData);
	if (g_StateType == STATE_FIGHT)
	{
		if (pData->m_FightProperty.cannotAddHP)return 0;
	}
	int& mp = pData->m_MP;
	int  mpmax = pData->m_MPMax.GetData();
	int mpadd = num;
	if (percent)
	{
		mpadd *= mpmax;
		if (abs(mpadd)>abs(percent*num))mpadd = percent*num;
		mpadd /= 100;
	}
	mp += mpadd;
	if (mp > mpmax)mp = mpmax;
	else if (mp < 0)mp = 0;

	if (target ==g_pMainState->m_HeroID)
	{
		g_pMainState->m_PCHead.UpdatePcMp();
	}
	else
	if (target == g_pMainState->m_HeroID + 5)
	{
		g_pMainState->m_PCHead.UpdatePetMp();
	}
	return mpadd;
}

void cSkillManager::AddHpHurt(int target, int num, int percent)
{
	
	cPetData* pData;
	cObj* pObj = g_pMainState->m_FightList[target].m_pData;
	pObj->GetDateP(pData);
	if (g_StateType == STATE_FIGHT)
	{
		if (pData->m_FightProperty.cannotAddHP)return ;
	}
	int& hp = pData->m_HP;
	if (0 == hp)return;
	int hphurt = pData->m_HPHurtMax;
	int hpmax = pData->m_HPMax.GetData();
	int dam = num;
	if (percent)
	{
		dam *= hpmax;
		if (abs(dam) > abs(num*percent))dam = num*percent;
		dam /= 100;
	}
	hphurt += dam;
	if (hphurt > hpmax)hphurt = hpmax;
	else if (hphurt < 0)hphurt = 0;
	if (target ==g_pMainState->m_HeroID)
	{
		g_pMainState->m_PCHead.UpdatePcHp();
	}
	else
	if (target == g_pMainState->m_HeroID + 5)
	{
		g_pMainState->m_PCHead.UpdatePetHp();
	}
}

void cSkillManager::AddCp(int target, int num, int percent)
{
	cPetData* pData;
	cObj* pObj = g_pMainState->m_FightList[target].m_pData;
	pObj->GetDateP(pData);
	int& cp = pData->m_CP;
	int dam = num;
	if (percent)
	{
		cp *= 150;
		cp /= 100;
	}
	cp +=dam;
	if (cp>150)cp = 150;
	else if (cp < 0)cp = 0;

	if (target == g_pMainState->m_HeroID)
	{
		g_pMainState->m_PCHead.UpdatePcCp();
	}
}
//����
void cSkillManager::Resurgence(int TargetID)
{
	g_pMainState->m_ItemManage.Resurgence(1, TargetID);
}

BOOL cSkillManager::UseSwitchMap(int Skilllv, int sSkillID, int UserID, int TargetID)
{
	if (STATE_MAIN == g_StateType)
	{
		int hpout;
		if (!Cost(hpout, Skilllv, sSkillID, UserID, 1, FALSE))
		{
			g_pMainState->m_Channel.AddNewString("�����޷�ʹ��", 0,3 );
			return FALSE;
		}
		Cost(hpout, Skilllv, sSkillID, UserID, 1, TRUE);

		cMyMap& map = g_pMainState->m_Map;
		switch (sSkillID)
		{
		case 3:	map.LoadMap(151, 10, "���ƹٸ�"); break;
		case 25:map.LoadMap(104, 121, "����ɽ"); break;
		case 44:map.LoadMap(88, 49, "������"); break;
		case 58:map.LoadMap(20, 98, "Ů����"); break;
		case 70:map.LoadMap(159, 55, "�칬"); break;
		case 83:map.LoadMap(79, 40, "����"); break;
		case 93:map.LoadMap(28, 18, "��ׯ��"); break;
		case 110:map.LoadMap(16, 55, "����ɽ"); break;
		case 124:map.LoadMap(37, 60, "���ܵظ�"); break;
		case 138:map.LoadMap(73, 60, "ħ��կ"); break;
		case 149:map.LoadMap(103, 16, "ʨ����"); break;
		case 163:map.LoadMap(175, 113, "��˿��"); break;
		case 180:ERRBOX; break; //�貨��		
		case 192:ERRBOX; break;//�޵׶�
		case 201:ERRBOX; break;//��ľ��
		}
	}
	return TRUE;
}

void cSkillManager::GetDelayAnd(int sSkillID, int& LateTime, DWORD& magicID)
{
	magicID = 0;
	LateTime = 3; //�˺�������ʱ
	switch (sSkillID)
	{
	case 11:
		LateTime = 7;
		break;
	case 28://���׷�
		LateTime = 5;
		break;
	case 88:// ����Ϸ��
		LateTime = 20;
		break;
	case 72://����
		LateTime = 10;
		break;
	case 74:
		LateTime = 10;
		break;
	case 79://����
		LateTime = 11;
		break;
	case  80:
		LateTime = 8;
		break;
	case 105://�չ⻪
		LateTime = 10;
		break;
	case 106://��׺�
		LateTime = 6;
		break;
	case 107://������
		LateTime = 20;
		break;
	case 108://��ã��
		LateTime = 6;
		break;
	case 109://���ѻ�
		LateTime = 8;
		break;
	case 116:  //������
		LateTime = 7;
		break;
	case 117:  //�й���
		LateTime = 2;
		break;
	case 131:// ��ɰ��ʯ
		LateTime = 10;
		break;
	case 132:// �������
		LateTime = 2;
		break;
	case 160:
		LateTime = 4;
		break;
	case  162:
		magicID = 0x8F3E73D5;
		break;
	case 2088:
		LateTime = 10;
		break;
	case 2089:
		LateTime = 20;
		break;
	case 2091:
		LateTime = 10;
		break;
	case 2101:
		LateTime = 10;
		break;
	
	}
}

void cSkillManager::GetMagicAndNum(int Skilllv, int sSkillID, int UserID, int TargetID, DWORD& MagicID, int& NumMax, int& MagicPos, BOOL& MagicUporDown, BOOL&  toAll1)
{
	toAll1 = GetSkillToAll(sSkillID);
	MagicUporDown = 1;
	 MagicID = 0;
	 NumMax = 1;
	 MagicPos = 0;
	 cObj* pUserobj= g_pMainState->m_FightList[UserID].m_pData;
	 cPetData* pUserData;
	 pUserobj->GetDateP(pUserData);
	 if (sSkillID < 1000||sSkillID>1999)
	 {
		 sSkill* pskill= g_pMainState->m_SkillManager.GetSkill(sSkillID);
		 MagicID= pskill->m_MagicID;
		 MagicPos= pskill->m_MagicPos;
		 if (pskill->m_TargetNumOffset>0)
		 {
			 if (sSkillID>2000)
				 NumMax = pUserData->m_LV / pskill->m_TargetNumOffset + 1;
			 else
				NumMax = Skilllv / pskill->m_TargetNumOffset + 1;
			 if (NumMax>pskill->m_TargetNum)NumMax = pskill->m_TargetNum;
		 }
		 else NumMax = pskill->m_TargetNum;
		 
	 }
	switch (sSkillID)
	{
	case 1007:
		MagicID = 0x78246FE4;
		break;
	case 116:  //������
		MagicUporDown = 0;
		break;
	case 1001: //ץ��
		MagicID = 0xBE7DDF16;
		break;
	}
	NumMax += pUserData->m_MagicNumAdd;
	if (NumMax > 10)NumMax = 10;
	
}

BOOL cSkillManager::CheckGodLivePercent(int& dam, int target, int userid)
{
	cPetData* pTargetData;
	cPetData* pUserData;
	cObj* pObj = m_pFightState->m_pList[target]->m_pData;
	pObj->GetDateP(pTargetData);
	pObj = m_pFightState->m_pList[userid]->m_pData;
	pObj->GetDateP(pUserData);
	if (pTargetData->m_HP-dam<1)
	{
		int godlive = pTargetData->GoldLivePercent.d1 - pUserData->GoldLivePercent.d2;
		if (rand() % 100 < godlive)
		{
			dam = -pTargetData->m_HPMax.GetData();
			pTargetData->GoldLivePercent.d1 -= 5;
			return TRUE;
		}			
	}
	return FALSE;
}

BOOL cSkillManager::StageWaitSleep(int time,int UserID, int retStage)
{
	if (m_pFightState->m_pList[UserID]->Sleep(time, FALSE))
	{
		m_pFightState->m_pList[UserID]->GetWork()->stage = retStage;
	}
	return TRUE;
}
//����
BOOL cSkillManager::Use1006(int Skilllv, int sSkillID, int UserID, int TargetID)
{
	sFightList* pUserList = m_pFightState->m_pList[UserID];
	cObj* pUser = pUserList->m_pData;
	cObj* pTarget = pUserList->m_pData;
	cPetData* pUserData;
	cPetData* pTargetData;
	pUser->GetDateP(pUserData);
	pTarget->GetDateP(pTargetData);

	////ERR ���ܱسɹ�
	//pUserList->work.clear();
	//pUserList->m_stage = 0;
	//m_pFightState->m_bEscape = TRUE;
	//return TRUE;
	////ERR
	int& stage = pUserList->GetWork()->stage;
	switch (stage)
	{
	case 0:
	
		{
			int direct;
			if (UserID < 10)direct = 3;
			else  direct = 7;
			pUser->SetDirection(direct);
			pUser->Set(POS_WALK);
			stage = 1;
			m_time = 30;
		}
		break;
	case 1:
		StageWaitSleep(20, UserID, 2);
		break;
	case 2:
		if (rand() % 100 < 70 + g_pMainState->m_GlobalButton[13]+ pUserData->m_FuZhuSkillLvList[9] - pTargetData->m_FuZhuSkillLvList[10])
		{
			stage = 3;
			pUser->m_Wav.Free();
			pUser->m_Wav.Load(35379719);
		}
		else
		{
			stage = 4;
			pUser->m_Wav.Free();
			pUser->m_Wav.Load(2673692967);	 
		}
		break;
	case 3:
	
		{
			int k;
			if (UserID < 10)k=3;
			else  k=-3;
			int x2 = pUser->GetX() + 2 * k;
			int y2 = pUser->GetY() + 1 * k;
			pUser->SetPos(x2, y2);
			pUser->MoveHpLine();
	

			//�ƶ��Ի�
			if (pUser->m_ShowTime > 0)
			{
				pUser->MoveDialog();
			}
			//�ƶ�����
			if (pUser->m_bNeedShowName)
			{
				pUser->MoveName();
			}
			if (UserID%10<5)
			if (pUserData->m_Controler.CheckHaveBB())
			{//�ٻ���һ����
				if (m_pFightState->m_pList[UserID + 5]->isLive && (!m_pFightState->m_pList[UserID + 5]->CheckDie()))
				{
					cObj* pPet = m_pFightState->m_pList[UserID + 5]->m_pData;
					int x3 = pPet->GetX() + 2 * k;
					int y3 = pPet->GetY() + 1 * k;
					if (pPet->m_NowPos != POS_WALK)
					{
						pPet->Set(POS_WALK);
						pPet->SetDirection(pUser->GetDirection());
					}
					pPet->SetPos(x3, y3);
					pPet->MoveHpLine();
					//�ƶ��Ի�
					if (pPet->m_ShowTime > 0)
					{
						pPet->MoveDialog();
					}
					//�ƶ�����
					if (pPet->m_bNeedShowName)
					{
						pPet->MoveName();
					}
				}
			}
			if (y2>480)
			{
				
				g_pMainState->m_pFightState->BackFromFight(UserID);
				
				if (pUserData->m_Controler.CheckHaveBB())
				{
						g_pMainState->m_pFightState->BackFromFight(UserID);
				}
		
				if (g_pMainState->m_HeroID == UserID)
				{
					m_pFightState->m_bEscape = TRUE;
				}
			}
		}
		break;
	case 4:
		{
			m_time -= 1;
			pUserList->Rotate(1);
			if (m_time < 0)
			{
				int direct;
				if (UserID < 10)direct = 7;
				else  direct = 3;
				pUser->SetDirection(direct);
				pUser->Set(POS_FIGHT_STAND);
				pUserList->m_work.clear();
			}
		}
		break;
	}
	return TRUE;
}

BOOL cSkillManager::GetSkillAtk(int ID)
{
	switch (ID)
	{
	case 0:
	case 1:
	case 7:
	case 11:
	case 28:
	case 31:
	case 71:
	case 72:
	case 74:
	case 79:
	case 80:
	case 84:
	case 88:
	case 97:
	case 105:
	case 106:
	case 107:
	case 108:
	case 109:
	case 116:
	case 117:
	case 122:
	case 131:
	case 132:
	case 141:
	case 142:
	case 143:
	case 148:
	case 154:
	case 155:
	case 162:
	case 167:
	case 178:
	case 169:
	case 171:
	case 172:
	case 184:
	case 186:
	case 193:
	case 194:
	case 195:
	case 196:
	case 197:
	case 198:
	case 1007:
	case 2084: 
	case 2085: 
	case 2086: 
	case 2087: 
	case 2088: 
	case 2089: 
	case 2090: 
	case 2091: 
	case 2101:
	case 2102:
		return TRUE;
	default:
		return FALSE;
	}
}

int cSkillManager::GetSpeed(int skillid)
{
	switch (skillid)
	{
	case 7:return 3;
	case 79:return 3;
	case 74:return 5;
	case 80:return 3;
	case 88:return 4;// ����Ϸ��
	case 107:return 3;
	case 131:return 3;
	case 142:return 1;
	case 2089:return 3;
	case 2091:return 3;
	case 2092:return 3;
	case 2093:return 3;
	case 2098:return 3;
	case 2101:return 2;
		
	default:
		return 5;
	}
}

BOOL cSkillManager::Use107(int Skilllv, int sSkillID, int UserID, int TargetID)
{
	sFightList* pUserList = m_pFightState->m_pList[UserID];
	cObj* pUser = pUserList->m_pData;
	cObj* pTarget = m_pFightState->m_pList[TargetID]->m_pData;
	int& stage = pUserList->GetWork()->stage;
	switch (stage)
	{
	case 0:
		if (1)
		{
			int hpout = 0;
			Cost(hpout, Skilllv, sSkillID, UserID, 1, TRUE);
			if (hpout)
			{
				SetDamage(hpout, UserID);
			}
			pUser->Set(POS_MAGIC);
			pUser->SetSound(POS_MAGIC);
			stage = 1;
		}
		return TRUE;
	case 1:
		if (WaitForMagicPoint(UserID))
		{
		
			if (sSkillID != 107)	
				pUser->SetMagicSound(sSkillID);//����
			else
				pUser->SetMagicSound(sSkillID,18);//�����Ƶ�ǰ���������Ҫ��
			DWORD MagicID;
			int MagicPos;
			BOOL MagicUporDown;
			BOOL  toAll;
			BOOL bAtk;
			int maxnum;
			GetMagicAndNum(Skilllv, sSkillID, UserID, TargetID, MagicID, maxnum, MagicPos, MagicUporDown, toAll);

			int magicspeed = GetSpeed(sSkillID);
			bAtk = GetSkillAtk(sSkillID);
			pUserList->m_MaxTargetNum = 0;
		
			for (int i = 0; i < maxnum; i++)
			{
				pUserList->targetlist[pUserList->m_MaxTargetNum] = GetTargetID(UserID, TargetID, i);
				int id = pUserList->targetlist[pUserList->m_MaxTargetNum];
				if (-1==id)break;
				if (UserID == id)continue;
				pUserList->m_MaxTargetNum += 1;
			}
			if (toAll)
			{
				for (int i = 0; i < pUserList->m_MaxTargetNum; i++)
				{
					pUserList->m_NowTargetID = pUserList->targetlist[i];
				
						m_pFightState->SetMagicSuffer(pUserList->m_NowTargetID, pUser->GetDirection());
						m_pFightState->m_pList[pUserList->m_NowTargetID]->m_bSufferFullSkill = toAll;

				}
				m_pFightState->m_FullSkill.SetMagicUp(0, MagicID, pTarget->GetX(), pTarget->GetY()-200);
			m_pFightState->m_FullSkill.m_FullSkilUP[0].m_Picture.m_FrameGap = magicspeed;
		}
			else
			{
				for (int i = 0; i < pUserList->m_MaxTargetNum; i++)
				{
					pUserList->m_NowTargetID = pUserList->targetlist[i];
					m_pFightState->SetMagicSuffer(pUserList->m_NowTargetID, pUser->GetDirection());

					m_pFightState->m_pList[pUserList->m_NowTargetID]->m_bSufferFullSkill = toAll;
					//0x565A6FE1
					m_pFightState->m_pList[pUserList->m_NowTargetID]->SetMagiSufferID(MagicID, -10, &m_pFightState->m_FullSkill.m_SufferMagic[TargetID], MagicUporDown);
					m_pFightState->m_pList[pUserList->m_NowTargetID]->m_pData->m_MagicSuffer.m_y -= 140;
					m_pFightState->m_pList[pUserList->m_NowTargetID]->m_pData->m_MagicSuffer.m_FrameGap = magicspeed;


				}
			}
			pUserList->m_NowTarget = -1;
			stage = 19;
		}
		return TRUE;
	case 19:
		if (m_pFightState->WaitPosEnd(UserID))
		{
			pUserList->m_pData->Set(POS_FIGHT_STAND);
		}
		StageWaitSleep(15, UserID, 20);
		break;
	case 20:
		if (m_pFightState->WaitPosEnd(UserID))
		{
			pUserList->m_pData->Set(POS_FIGHT_STAND);
		}
		switch (sSkillID)
		{
		case 162:
		case 107:
			pUserList->m_NowTarget = -1;
			if (m_pFightState->m_pList[pUserList->m_NowTargetID]->m_pData->m_MagicSuffer.m_y == m_pFightState->m_pList[pUserList->m_NowTargetID]->m_pData->GetY())
			{
				if (sSkillID==107)
				for (int i = 0; i < pUserList->m_MaxTargetNum; i++)
				{
					pUserList->m_NowTargetID = pUserList->targetlist[i];
					m_pFightState->m_pList[pUserList->m_NowTargetID]->SetMagiSufferID(0xDF4C3BE3, -10, &m_pFightState->m_FullSkill.m_SufferMagic[TargetID], TRUE);
				}
				pUserList->m_NowTarget = -1;
				stage = 3;
			}
			else
			{
				for (int i = 0; i < pUserList->m_MaxTargetNum; i++)
				{
					pUserList->m_NowTargetID = pUserList->targetlist[i];
					m_pFightState->m_pList[pUserList->m_NowTargetID]->m_pData->m_MagicSuffer.SetY(m_pFightState->m_pList[pUserList->m_NowTargetID]->m_pData->m_MagicSuffer.GetY()+20);
				}
				pUserList->m_NowTarget = -1;
			}
			break;
		
		case 2087:
			m_pFightState->m_FullSkill.m_FullSkilUP[0].SetY(m_pFightState->m_FullSkill.m_FullSkilUP[0].GetY() + 40);
			if (m_pFightState->m_FullSkill.m_FullSkilUP[0].GetY() == pTarget->GetY())
			{
				pUserList->m_NowTarget = -1;
				stage = 3;
			}
			break;
		default:
			return FALSE;
		}
		break;	
	case 3:  //����BUFF
		
		{
			if (m_pFightState->WaitPosEnd(UserID))
			{
				pUserList->m_pData->Set(POS_FIGHT_STAND);
			}

			DWORD MagicID;
			//BOOL toAll=GetSkillToAll(sSkillID);
			int LateTime; //�˺�������ʱ
		
			GetDelayAnd(sSkillID, LateTime, MagicID);
		
			for (int i = 0; i < pUserList->m_MaxTargetNum; i++)
			{
				pUserList->m_NowTargetID = pUserList->targetlist[i];

					m_pFightState->m_pList[pUserList->m_NowTargetID]->m_pData->SetSound(POS_SUFFER);//����
					m_pFightState->SwitchMagicSuffer(pUserList->m_NowTargetID);
					EffectSkill(Skilllv, sSkillID, UserID, pUserList->m_NowTargetID);

			}
			if (sSkillID==107) //̩ɽѹ����ʱ�°�
				stage = 4;
			else
				stage = 5;
		}
		return TRUE;
	case 4://�ȴ����Ƕ������
		if (m_pFightState->WaitPosEnd(UserID) || (pUser->m_NowPos == POS_FIGHT_STAND))
		{
			if (sSkillID == 162)
			{
				pUser->SetMagicSound(-1);//����
			}
			pUserList->m_pData->Set(POS_FIGHT_STAND);
			pUserList->m_work.pop_back();
			m_pFightState->m_FullSkill.m_FullSkilUP[0].m_NeedShow = FALSE;

			cPetData* pPet;
			pUser->GetDateP(pPet);
			if (1 > pPet->m_HP)
			{
				if (UserID < 10)
					pUserList->m_sufferDirection = 3;
				else pUserList->m_sufferDirection = 7;
				g_pMainState->m_pFightState->SetDie(UserID);
				return TRUE;
			}


			cPetData* userdata;
			cPetData* targetdata;
			pUser->GetDateP(userdata);
			pTarget->GetDateP(targetdata);
			CoutComboMagic(userdata, UserID, targetdata);
		}
		return TRUE;
	case 5://�ȴ����Ƕ������
		if (m_pFightState->WaitPosEnd(UserID) || (pUser->m_NowPos == POS_FIGHT_STAND))
		{
			pUserList->m_pData->Set(POS_FIGHT_STAND);
		}
		StageWaitSleep(10, UserID, 4);
		return TRUE;
	}
	return TRUE;
}

void cSkillManager::CountMagic2Damage(int& dam, int UserID, int TargetID, eMagicWuXing wuxing)
{//����̶��˺�
	cPetData*  pUserData;
	cPetData*  pTargetData;
	cObj* pUser = m_pFightState->m_pList[UserID]->m_pData;
	cObj* pTarget = m_pFightState->m_pList[TargetID]->m_pData;
	pUser->GetDateP(pUserData);
	pTarget->GetDateP(pTargetData);
	
	if (pTargetData->m_FightProperty.m_WallNum > 0)
	{//���֮ǽ
		pTargetData->m_FightProperty.m_WallNum -= 1;
		dam = 0;
		return;
	}
	int hit = 100+pUserData->m_MagicHitRate.d1 - pTargetData->m_MagicHitRate.d2;
	if (rand() % 100 >= hit)//�����ر�
	{
		dam = 0;
		return;
	}

	int damageup = CorrectWuXing(pUserData, pTargetData, wuxing);//��������
	//�ع�����
	damageup+= CountSpecial(pUserData, pTargetData,  pTarget->GetSex());
	damageup+= pUserData->m_DamageAddPercentMagic2.d1 - pTargetData->m_DamageAddPercentMagic2.d2;

	dam = (dam*(100 + damageup)) / 100;
	if ((TargetID < 10) || g_pMainState->m_AI.m_AttackAI.m_bPKMode)
	{//��ͨ����ʱ�˺���ʵ��2������
		dam= dam >>1;
	}
	if (dam < 1)dam = 1;
	//����
	int rate = pUserData->m_MagDownHPHurt.d1 - pTargetData->m_MagDownHPHurt.d2;
	if (rate>0)AddHpHurt(TargetID, -(dam*rate) / 100);

	if (pTargetData->m_StabRateMagic.d1)
	{
		//int rate = pTargetData->m_StabRateMagic.d1 - pUserData->m_StabRateMagic.d2;
		if (rand() % 100 < dam)
		{
			int rate = pTargetData->m_StabDamageRateMagic.d1 - pUserData->m_StabDamageRateMagic.d2;
			int dam2 = dam*rate / 100;
			CheckGodLivePercent(dam2, UserID, TargetID); //����
			AddHp(UserID, -dam2);
			SetDamage(dam2, UserID);
		}
	}

	CheckFightStateOnLine(TargetID, dam);
	CheckGodLivePercent(dam, TargetID, UserID); //����
	AddHp(TargetID, -dam);
	SetDamage(dam, TargetID);
	pTargetData->m_FightProperty.SufferNum += 1;
}


BOOL cSkillManager::GetSkillToAll(int ID)
{
	switch (ID)
	{
	case 80:
	case 131:
	//case 162:
	case 2087:
		return TRUE;
	}
	return FALSE;
}

void cSkillManager::lvUpMainSkill(cPCData& pcDate, int lv, int ID)
{
	if (!lv)return;
	m_MengPaiManager.lvUpMainSkill(pcDate, lv, ID);
}



sSkill* cSkillManager::GetSkill(int ID)
{
	if (ID<1000)
		return &m_MengPaiManager.m_SkillList[ID]; 
	if (ID < 2000)//1000-2000֮��Ĳ��Ǽ���
		return 0;
	if (ID < 3000)
		return GetPetSkill(ID - 2000);
	return GetSpecialSkill(ID - 3000);
}

void cSkillManager::SetMengPaiSkill(cDescribe& describe, int id)
{
	if (id < 0)return;
	sSkill& Skill = m_MengPaiManager.m_SkillList[id];
	string text[3];
	text[0] = Skill.m_Name;
	text[1] = Skill.m_Text;
	ostringstream oss;
	oss << "����:";
	oss << Skill.m_tCost;
	text[2] = oss.str();
	describe.Set(g_xMouse, g_yMouse, Skill.m_BigHead, 3, text);
}

BOOL cSkillManager::UseNormal(int skillID)
{//ƽʱʹ�õķ���,��������ɷ���
	int targetid = g_pMainState->m_HeroID;
	sSkill* pSkill= g_pMainState->m_SkillManager.GetMengPaiSkill(skillID);
	int skilllv=g_pCharacter->m_PCData.m_MengPaiSkillLV[pSkill->m_Owner];

	int hpout;
	if (!Cost(hpout, skilllv, skillID, targetid, 1, FALSE))
	{
		g_pMainState->m_Channel.AddNewString("�����޷�ʹ��");
		return TRUE;
	}
	
	if (!CheckMagicHit(skilllv, skillID, targetid, targetid))
	{
		Cost(hpout, skilllv, skillID, targetid, 1, TRUE);
		g_pMainState->m_Channel.AddNewString("����ʹ��ʧ��");
		return TRUE;
	}
	switch (skillID)
	{
	case 3:   //����   ���ֻ�����
	case 25://����ɽ
	case 44: //������
	case 58: //Ů����
	case 70: //�칬
	case 83://����
	case 93: //��ׯ��
	case 110: //����
	case 124://�ظ�
	case 138: //ħ��կ
	case 149: //ʨ��
	case 163: //��˿��
	case 180: //�貨��
	case 192://�޵׶�
	case 201://Ҷ��
		if (UseSwitchMap(skilllv, skillID, targetid, targetid))
			return TRUE;
		return FALSE;
	}
	return TRUE;
}

int cSkillManager::CountSpecial(cPetData* user, cPetData* target,  BOOL targetSex)
{
	int damageup = 0;
	switch (target->m_race%10)
	{
	case 0:
		damageup = user->GoldSpecial.d1-target->ghostSpecial.d2;     //�����ع�
		break;
	case 1:
		damageup = user->MagSpecial.d1 - target->MagSpecial.d2;    //ħ���ع�
		break;
	case 2:
		damageup = user->ImmortalSpecial.d1 - target->ImmortalSpecial.d2; //�����ع�
		break;
	case 3:
		damageup = user->MonsterSpecial.d1 - target->MonsterSpecial.d2;//�����ع�
		break;
	case 4:
		damageup = user->PeopleSpecial.d1 - target->PeopleSpecial.d2; //��
		break;
	case 5:	//�����޷����ع�
		break;
	case 6:
		damageup = user->machineSpecial.d1 - target->machineSpecial.d2;//��е�ع�
		break;
	default:
	
		break;
	}
	if (target->m_race > 11)
	{
		damageup += user->ghostSpecial.d1 - target->ghostSpecial.d2; //���ع�
	}
	if (targetSex)
	{
		damageup += user->BoySpecial.d1 - target->BoySpecial.d2;  //��	
	}
	else damageup += user->GirlSpecial.d1 - target->GirlSpecial.d2;//Ů

	return damageup;
}

BOOL cSkillManager::CoutComboMagic(cPetData* user, int userid, cPetData* ptarget)
{
	user->m_FightProperty.AttackNum += 1;
	if (user->m_FightProperty.AttackNum<2)
	if ((rand() % 100) <  (user->m_MagicComboRate.d1 - ptarget->m_MagicComboRate.d2))
	{	
		g_pMainState->m_FightList[userid].SetWorkFront(WORKTYPE_MAGIC);
		return TRUE; 
	}
	return FALSE;
}

BOOL cSkillManager::CoutCombo(cPetData* user, cPetData* target)
{
	user->m_FightProperty.AttackNum += 1;
	if (user->m_FightProperty.AttackNum < user->comboNumUp)
	{
		if (user->m_FightProperty.AttackNum < user->comboNumDown)
		{
			return TRUE;
		}
		int rate = user->m_Combo.d1 - target->m_Combo.d2;
		if (rand() % 100 < rate)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void cSkillManager::CorrectCureHP(int& dam, int UserID, int TargetID)
{
	cPetData* pUser;
	cPetData* pTarget;
	g_pMainState->m_FightList[UserID].m_pData->GetDateP(pUser);
	g_pMainState->m_FightList[TargetID].m_pData->GetDateP(pTarget);
	dam += (dam* (pUser->m_DamageAddPercentCure.d1 + pTarget->m_DamageAddPercentCure.d2)) / 100;
	int crirate = pUser->m_CureCriRate.d1 + pUser->m_CureCriRate.d2;
	if (rand() % 100 < crirate)
	{
		dam = dam * 2;
	}
	dam += pUser->m_DamageAddCure.d1 + pUser->m_DamageAddCure.d2;
	AddHp(TargetID, dam);
	SetDamage(-dam, TargetID);
}
void cSkillManager::CorrectCureMP(int& dam, int UserID, int TargetID)
{
	cPetData* pUser;
	cPetData* pTarget;
	g_pMainState->m_FightList[UserID].m_pData->GetDateP(pUser);
	g_pMainState->m_FightList[TargetID].m_pData->GetDateP(pTarget);
	dam += (dam* (pUser->m_DamageAddPercentCure.d1 + pTarget->m_DamageAddPercentCure.d2)) / 100;
	AddMp(TargetID, dam);
	SetDamage(-dam, TargetID);
}

BOOL cSkillManager::CheckMagicHit(int Skilllv, int sSkillID, int UserID, int TargetID)
{//����������
	cPetData* pUserData;
	cPetData* pTargetData;
	g_pMainState->m_FightList[UserID].m_pData->GetDateP(pUserData);
	g_pMainState->m_FightList[TargetID].m_pData->GetDateP(pTargetData);
	//��ӡ��������
	int rate = pUserData->m_SealRate.d1 - pTargetData->m_SealRate.d2 + ((Skilllv - pTargetData->m_LV) << 1);
	switch (sSkillID)
	{
		//����
	case 6:rate += 40;break;
		//����
	case 14://����
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
		rate += 50; break;
		//Ů��
	case 54:rate += 45; break;//��������
	case 59:rate += 30; break;
		//�칬
	case 67:rate += 45; break;
	case 68:rate += 45; break;
	case 69:rate += 45; break;
		//����
	case 77:
	case 80:
	case 88:
		rate = 110; 
		break;
		//�ٻ��޷���
	case 2084:
	case 2085:
	case 2086:
	case 2087:
	case 2088:
	case 2089:
	case 2090:
	case 2091:
		rate = 100; break;
		//�������η���
	case 105:
	case 106:
	case 107:
	case 108:
	case 109:
		rate = 100; break;
		//ħ��
	case 131:rate = 100; break;//��ɳ
		//��ׯ
	case 89:rate += 40; break;
		//�ظ�
	case 122:rate = 80; break;
	case 127:rate += 80; break;
	case 129:rate += 40; break;
		//ST
	case 144:rate += 40; break;
		//PS
	case 154:
	case 155:
	case 156://����
	case 159:
	case 164:
		if (1)
		{
			switch (sSkillID)
			{
			case 154:rate += 40; break;
			case 155:rate += 40; break;
			case 156:rate += 50; break;
			case 159:rate += 40; break;
			case 164:rate += 40; break;
			}	
			rate += (pUserData->m_Popularity - pTargetData->m_Popularity) / 50;
		}
		break;
	case 162:rate = 100; break;
	case 165:rate += 40; break;
	case 3:   //����   ���ֻ�����
	case 25://����ɽ
	case 44: //������
	case 58: //Ů����
	case 70: //�칬
	case 83://����
	case 93: //��ׯ��
	case 110: //����
	case 124://�ظ�
	case 138: //ħ��
	case 149: //ʨ��
	case 163: //��˿��
	case 180: //�貨��
	case 192://�޵׶�
	case 201://Ҷ��
			rate=60 + (Skilllv>>1); break;//;������
	default:
		return TRUE;
	}
	//����������
	rate += pUserData->m_MagicHitRate.d1 - pTargetData->m_MagicHitRate.d2;
	if ((rand() % 100) < rate)
		return TRUE;
	return FALSE;
}

int cSkillManager::CorrectWuXing(cPetData* pUserData, cPetData* pTargetData, eMagicWuXing wuxing)
{//�����˺�����
	int offwx = 0;
	switch (wuxing)
	{
	case cSkillManager::eMWX��:
		break;
	case cSkillManager::eMWX��:
		offwx = pUserData->m_MagicThunder.d1 - pTargetData->m_MagicThunder.d2;
		break;
	case cSkillManager::eMWX��:
		offwx = pUserData->m_MagicFire.d1 - pTargetData->m_MagicFire.d2;
		break;
	case cSkillManager::eMWXˮ:
		offwx = pUserData->m_MagicWater.d1 - pTargetData->m_MagicWater.d2;
		break;
	case cSkillManager::eMWX��:
		offwx = pUserData->m_MagicSoil.d1 - pTargetData->m_MagicSoil.d2;
		break;
	case cSkillManager::eMWX��ˮ:
		offwx = (pUserData->m_MagicThunder.d1 - pTargetData->m_MagicThunder.d2/2);
		offwx += (pUserData->m_MagicWater.d1 - pTargetData->m_MagicWater.d2/2);
		break;
	case cSkillManager::eMWX��:
		offwx = pUserData->m_MagicThunder.d1 - pTargetData->m_MagicThunder.d2 / 3;
		offwx += pUserData->m_MagicWater.d1 - pTargetData->m_MagicWater.d2 / 3;
		offwx += pUserData->m_MagicFire.d1 - pTargetData->m_MagicFire.d2 / 3;
		offwx += pUserData->m_MagicSoil.d1 - pTargetData->m_MagicSoil.d2 / 3;
		break;
	default:
		break;
	}
	return offwx;
}

BOOL cSkillManager::PhysicalAtkHitStage(int UserID, int targetid, int sSkillID)
{
	sFightList* pUserList = &g_pMainState->m_FightList[UserID];
	cObj* pUser = pUserList->m_pData;
	cPetData* pUserData;
	pUser->GetDateP(pUserData);
	int pos = 0;
	DWORD id = 0;
	switch (sSkillID)
	{
	case 0://��ɨ
		id = 0xACA4A54A;
		break;
	case 71://����ն
		id = 0x4C117B7C;
		break;
	case 97://���꽣��0xD06119B3   
		id = 0xC29A0737;
		break;
	case 142:
		id = 2194212682;//0xE1E5EA57;
		break;
	case 143:
		id = 0x718754A2;
		break;
	case 2092://����һ��
		id = 0x3BA4B993;
		pUser->SetMagicSound(sSkillID, 50);
		pos = -10;
		break;
	case 2094://�����ٻ�
		pUser->SetMagicSound(sSkillID, 30);
		break;
	case 2098://�ƶ��б�
		pUser->SetMagicSound(sSkillID);
		id = 0xDBFCCCBA;
		break;
	case 2099://���ݻ���
		pUser->SetMagicSound(sSkillID);
		id = 0x6B59951D;
		break;
	case 2100://���ŭ��
		pUser->SetMagicSound(sSkillID);
		id = 0xB0B36693;
		break;
	default:
		id = 0x1D3FF13C;
		break;
	}
	cObj* pTarget = m_pFightState->m_pList[targetid]->m_pData;
	cPetData* pTargetData;
	pTarget->GetDateP(pTargetData);

	if (FALSE == pTargetData->m_FightProperty.m_bHit)
	{
		if (g_pMainState->m_FightList[targetid].m_work.size())
		if (g_pMainState->m_FightList[targetid].GetWork()->type == WORKTYPE_AVOID)
			return FALSE;
		m_pFightState->SetAvoid(targetid, pUser->GetDirection());
		return FALSE;
	}
	if (pUserData->m_FightProperty.m_bCri)
	{
		m_pFightState->SetSuffer(targetid, pUser->GetDirection(), UserID);
		pTarget->SetSound(POS_SUFFER);
		id = 0xECD0E003;//��ɱ
		pos = 0;
	}
	else
	if (pTargetData->m_FightProperty.m_bDefend)
	{
		id = 0x3622636F;
		pos = 0;
		m_pFightState->SetDefend(targetid, pUser->GetDirection(), UserID);
		pTarget->SetSound(POS_DEF);
	}
	else
	{
		m_pFightState->SetSuffer(targetid, pUser->GetDirection(), UserID);
		pTarget->SetSound(POS_SUFFER);
	}
	m_pFightState->m_pList[targetid]->SetMagiSufferID(id, pos, &m_pFightState->m_FullSkill.m_SufferMagic[targetid]);
	pTarget->m_MagicSuffer.m_FrameGap = GetSpeed(sSkillID);
	return TRUE;
}


BOOL cMengPaiManager::Init()
{
	if (m_bInit)return TRUE;
	m_bInit = TRUE;
	ifstream File;
	ostringstream oss;
	oss << g_strMediaPath << "����//����//ʦ�ż���.txt";
	File.open(oss.str());
	int dustbin;
	File >> dustbin;
	if (dustbin != c_SkillNum)ERRBOX;
	for (int i = 0; i < c_SkillNum; i++)
	{
		sSkill& skill = m_SkillList[i];
		File >> skill.m_Id;
		File >> skill.m_Name;
		File >> skill.m_BigHead;
		File >> skill.m_SmallHead;
		File >> skill.m_SkillType;
		File >> skill.m_EffectType;
		File >> skill.m_Owner;

		File >> skill.m_TargetNum;
		File >> skill.m_TargetNumOffset;
		File >> skill.m_MagicID;
		File >> skill.m_MagicPos;
		File >> skill.m_Text;
		File >> skill.m_tCost;
	/*	getline(File, skill.m_Text);
		getline(File, skill.m_Text);
		if (7 != skill.m_SkillType)
			getline(File, skill.m_tCost);*/
	}
	File.close();
	oss.str("");
	oss << g_strMediaPath<<"����//����//ʦ�ż�������.txt";
	File.open(oss.str());
	if (!File)
	{
		ERRBOX;
		File.close();
	}
	int soundnum;
	File >> soundnum;
	int soundid;
	string soundname;
	for (int i = 0; i < soundnum; i++)
	{
		File >> soundid;
		File >> soundname;
		File >>  m_SkillList[i].m_Sound;
	}
	File.close();
	oss.str("");
	oss << g_strMediaPath << "����//����//����.txt";
	File.open(oss.str());
	File >> dustbin;
	if (dustbin != c_NumMengPai)
	{
		ERRBOX;
		return FALSE;
	}
	for (int i = 0; i < c_NumMengPai; i++)
	{
		File >> m_MengPai[i].m_ID;
		File >> m_MengPai[i].m_Name; //������
		File >> m_MengPai[i].m_ShiFuName;
		File >> m_MengPai[i].m_XunLuoMapName;
		File >> m_MengPai[i].m_ShiFuMap;
		for (int j = 0; j < 7; j++) //������
		{
			cZhuJiNeng&  zskill = m_MengPai[i].m_MainSkill[j];
			File >> zskill.m_ID;
			File >> zskill.m_Name;
			File >> zskill.m_BigHead;
			File >> zskill.m_SmallHead;

			if (0 == zskill.m_BigHead)zskill.m_BigHead = 3416022732;
			if (0 == zskill.m_SmallHead)zskill.m_SmallHead = 3416022732;
			File >> zskill.m_NumSkillInclude;
			
			for (int k = 0; k < zskill.m_NumSkillInclude; k++)//��������
			{
				File >> zskill.m_SkillIncludeID[k];
				File >> zskill.m_Limit[k];
			}
			getline(File, zskill.m_Text);
			getline(File, zskill.m_Text);
			getline(File, zskill.m_Text2);
		}
	}
	File.close();
	return TRUE;
}

cMengPai* cMengPaiManager::GetMengPai(cCharacter* pc)
{
	return GetMengPai(pc->m_PCData.m_MengPaiID);
}

void cMengPaiManager::lvUpMainSkill(cPCData& pcDate, int lv, int ID)
{
	if (ID > 7 || ID < 0)return;
	if (pcDate.m_MengPaiID < 0 || pcDate.m_MengPaiID>11)return;
	cZhuJiNeng* pZhuJiNeng= &m_MengPai[pcDate.m_MengPaiID].m_MainSkill[ID];
	int& skilllv = pcDate.m_MengPaiSkillLV[ID];
	int prelv = skilllv;
	int nextlv = prelv + lv;
	switch (pcDate.m_MengPaiID)
	{
	case 0://����
		switch (ID)
		{
		case 0:pcDate.AddAtkZZ(2 * lv); break;
		case 1:pcDate.AddSpdZZ(lv);break;
		case 2:pcDate.AddHpZZ(7 * lv); break;
		case 3:pcDate.AddAvoidZZ(lv); break;
		case 4:
			pcDate.AddGrowing(-prelv/10);
			pcDate.AddGrowing(nextlv /10); 
			break;
		case 5:pcDate.AddDefZZ(3 * lv); break;
		case 6:pcDate.AddMagZZ(2 * lv); break;
		}
		break;
	case 1://����
		switch (ID)
		{
		case 0:pcDate.AddSpdZZ( lv); break;
		case 1:pcDate.AddAtkZZ( lv); break;
		case 2:pcDate.AddMagZZ(3 * lv); break;
		case 3:
			pcDate.AddGrowing(-prelv /10);
			pcDate.AddGrowing(nextlv /10);
			break;
		case 4:pcDate.AddDefZZ(3 * lv); break;
		case 5:pcDate.AddHpZZ(10 * lv); break;
		case 6:pcDate.AddAvoidZZ( lv); break;
		}
		break;
	case 2://Ů��
		switch (ID)
		{
		case 0:
			pcDate.AddGrowing(-prelv / 10);
			pcDate.AddGrowing(nextlv / 10);
			break;
		case 1:pcDate.AddAvoidZZ(3 * lv); break;
		case 2:pcDate.AddDefZZ(2 * lv); break;
		case 3:pcDate.AddAtkZZ(2 * lv); break;
		case 4:
			pcDate.AddHpZZ(5 * lv);
			pcDate.m_HiddenWeaponDamagePercent.d1 -= prelv/5;
			pcDate.m_HiddenWeaponDamagePercent.d1 += nextlv/5;
			break;
		case 5:pcDate.AddMagZZ(2 * lv); break;
		case 6:pcDate.AddSpdZZ(3 * lv); break;
		}
		break;
	case 3://����
		switch (ID)
		{
		case 0:pcDate.AddDefZZ(lv); break;
		case 1:
			pcDate.AddAtkZZ(lv); 
			pcDate.ghostSpecial.d1 -= prelv/8;
			pcDate.ghostSpecial.d1 += nextlv/8;
			break;
		case 2:
			pcDate.AddMagZZ(lv); 
			pcDate.MonsterSpecial.d1 -= prelv/8;
			pcDate.MonsterSpecial.d1 += nextlv/8;
			break;
		case 3:
			pcDate.AddGrowing(-prelv / 10);
			pcDate.AddGrowing(nextlv / 10);
			break;
		case 4:pcDate.AddHpZZ(7 * lv); break;
		case 5:pcDate.AddSpdZZ(2 * lv); break;
		case 6:pcDate.AddAvoidZZ(2 * lv); break;
		}
		break;
	case 4:  //ʨ��
		switch (ID)
		{
		case 0:pcDate.AddDefZZ(lv); break;
		case 1:pcDate.AddAtkZZ(2 * lv); break;
		case 2:pcDate.AddHpZZ(10 * lv); break;
		case 3:pcDate.AddMagZZ(3 * lv); break;
		case 4:
			pcDate.AddGrowing(-prelv / 10);
			pcDate.AddGrowing(nextlv / 10);
			break;
		case 5:pcDate.AddAvoidZZ(lv); break;
		case 6:pcDate.AddSpdZZ(lv); break;
		}
		break;
	case 5://ħ��
		switch (ID)
		{
		case 0:
			pcDate.AddGrowing(-prelv / 10);
			pcDate.AddGrowing(nextlv / 10);
			pcDate.m_MagicCriRate.d1 -= prelv / 15;
			pcDate.m_MagicCriRate.d1 += nextlv / 15;	
			break;
		case 1:pcDate.AddMagZZ(5 * lv); break;
		case 2:
			pcDate.AddSpdZZ(-prelv / 2);
			pcDate.AddSpdZZ(nextlv/2); 
			pcDate.m_MagicFire.d2 -= prelv/10;
			pcDate.m_MagicFire.d2 += nextlv/10;
			break;
		case 3:
			pcDate.AddAtkZZ(2 * lv); 
			pcDate.m_MagicFire.d1 -= prelv / 10;
			pcDate.m_MagicFire.d1 += nextlv / 10;
			break;
		case 4:
			pcDate.AddHpZZ(9 * lv);
			break;
		case 5:
			pcDate.AddDefZZ(lv);
			pcDate.m_CounterRate.d1 -= prelv / 10;
			pcDate.m_CounterRate.d1 += nextlv / 10;
			break;
		case 6:
			pcDate.AddAvoidZZ(-prelv / 2);
			pcDate.AddAvoidZZ(nextlv/2); 
			break;
		}
		break;
	case 6://��˿
			switch (ID)
			{
			case 0:
				pcDate.AddDefZZ(2* lv); 
				pcDate.m_HiddenWeaponDamagePercent.d2 -= prelv/2;
				pcDate.m_HiddenWeaponDamagePercent.d2 += nextlv/2;
				break;
			case 1: pcDate.AddMagZZ(4 * lv); break;
			case 2:pcDate.AddHpZZ(7 * lv); break;
			case 3:
				pcDate.AddGrowing(-prelv / 10);
				pcDate.AddGrowing(nextlv / 10);
				break;
			case 4:
				pcDate.AddAtkZZ(3 * lv); break;	
				break;
			case 5:pcDate.AddAvoidZZ(3 * lv); break;
			case 6:pcDate.AddSpdZZ(2* lv); break;
			}
			break;
	case 7://�ظ�
		switch (ID)
		{
		case 0:
			pcDate.AddGrowing(-prelv / 10);
			pcDate.AddGrowing(nextlv / 10);
			pcDate.m_SealRate.d2 -= prelv / 10;
			pcDate.m_SealRate.d2 += nextlv / 10;
			break;
		case 1:pcDate.AddAtkZZ(3 * lv); break;
		case 2:pcDate.AddSpdZZ(1 * lv); break;
		case 3:
			pcDate.AddHpZZ(10 * lv); 
			pcDate.nightLv -= prelv / 10;
			pcDate.nightLv += nextlv / 10;
			break;
		case 4:pcDate.AddMagZZ(3 * lv); break;
		case 5:
			pcDate.AddDefZZ( lv);
			pcDate.poisonRate.d1 -= prelv / 10;//����
			pcDate.poisonRate.d1 += nextlv/10;//����
			break;
		case 6:pcDate.AddAvoidZZ(lv); break;
		}
		break;
	case 8://����
		switch (ID)
		{
		case 0:
			pcDate.AddGrowing(-prelv / 10);
			pcDate.AddGrowing(nextlv / 10);
			break;
		case 1:
			pcDate.AddMagZZ(5 * lv); 
			pcDate.m_MagicWater.d2 -= prelv / 10;
			pcDate.m_MagicWater.d2 += nextlv / 10;
			break;
		case 2:pcDate.AddHpZZ(5* lv); break;
		case 3:
			pcDate.AddAtkZZ(lv); 
			pcDate.m_MagicWater.d1 -= prelv / 10;
			pcDate.m_MagicWater.d1 += nextlv / 10;
			break;		
		case 4:pcDate.AddDefZZ(3 * lv); break;
		case 5:pcDate.AddAvoidZZ(-prelv/2); 
			pcDate.AddAvoidZZ(nextlv / 2); break;
		case 6:pcDate.AddSpdZZ(-prelv/2); 
			pcDate.AddSpdZZ(nextlv/ 2); break;
		}
		break;
	case 9://��ׯ
		switch (ID)
		{
		case 0:
			pcDate.AddGrowing(-prelv / 10);
			pcDate.AddGrowing(nextlv / 10);
			break;
		case 1:pcDate.AddAtkZZ(2 * lv); break;
		case 2:pcDate.AddHpZZ(6 * lv); break;
		case 3:pcDate.AddMagZZ(2 * lv); break;
		case 4:pcDate.AddDefZZ(2 * lv); break;
		case 5:pcDate.AddSpdZZ(2 * lv); break;
		case 6:pcDate.AddAvoidZZ(2 * lv); break;
		}
		break;
	case 10://����
		switch (ID)
		{
		case 0:pcDate.AddMagZZ(3 * lv); break;
		case 1:pcDate.AddHpZZ(6 * lv); break;
		case 2:pcDate.AddSpdZZ(lv); break;
		case 3:
			pcDate.AddGrowing(-prelv / 10);
			pcDate.AddGrowing(nextlv / 10);
			break;
		case 4:pcDate.AddDefZZ(3 * lv); break;
		case 5:
			if (1)
			{
				pcDate.AddAtkZZ(lv);
				int data = prelv / 20;
				pcDate.m_MagicFire.d2 -= data; //��
				pcDate.m_MagicSoil.d2 -= data; //��
				pcDate.m_MagicWater.d2 -= data;  //ˮ
				pcDate.m_MagicThunder.d2 -= data;//��
				data = nextlv / 20;
				pcDate.m_MagicFire.d2 += data; //��
				pcDate.m_MagicSoil.d2 += data; //��
				pcDate.m_MagicWater.d2 += data;  //ˮ
				pcDate.m_MagicThunder.d2 += data;//��
			}
			break;
		case 6:pcDate.AddAvoidZZ(lv); break;
		}
		break;
	case 11://�칬
		switch (ID)
		{
		case 0:
			pcDate.AddAtkZZ(lv); 
			pcDate.MonsterSpecial.d1 -= prelv / 5;
			pcDate.MonsterSpecial.d1 += nextlv / 5;
			break;
		case 1:pcDate.AddHpZZ(6 * lv); break;
		case 2:
			pcDate.AddGrowing(-prelv / 10);
			pcDate.AddGrowing(nextlv / 10);
			break;
		case 3:pcDate.AddDefZZ(2 * lv); break;
		case 4:pcDate.AddMagZZ(2 * lv); break;
		case 5:pcDate.AddSpdZZ(2 * lv); break;
		case 6:pcDate.AddAvoidZZ(2 * lv); break;
		}
		break;
	default:
		ERRBOX;
		return;
	}
	skilllv += lv;
	if (lv>0)
	for (int i = 0; i < pZhuJiNeng->m_NumSkillInclude; i++)
	{
		if (prelv < pZhuJiNeng->m_Limit[i])
		if (skilllv >= pZhuJiNeng->m_Limit[i])
		{
			//���ڼ��
			for (int j = 0; j < pcDate.m_SkillEnablelist.size(); j++)
			{
				if (pcDate.m_SkillEnablelist[j]->m_Id == pZhuJiNeng->m_SkillIncludeID[i])
					return;
			}
			sSkill* pSkill = &m_SkillList[pZhuJiNeng->m_SkillIncludeID[i]];
			pcDate.m_SkillEnablelist.push_back(pSkill);

		}
	}
}



