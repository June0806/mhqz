#include "global.h"

//���ѡ��Ŀ�� SelectType �Ƿ���������
int cAI::AutoFindTarget(int userID, int SelectType)
{
	//�������п�ѡ��ID
	int k = 10;
	switch (SelectType)
	{
	case 2: //ֻ�ܶ��Լ�ʹ�õķ���
	case 5:
		return  userID;
	case 3: //ֻ�ܶ��ҷ�ʹ�õķ���
	case 6:
		if (userID < 10)k = 0;
		break;
	case 4: //�Ե���ʹ��
		if (userID > 9)	k = 0;
		break;
	}
	int livenum = 0;
	int idlist[10];
	sFightList* pList;
	int i2;
	for (int i = 0; i < 10; i++)
	{
		i2 = i + k;
		pList = &g_pMainState->m_FightList[i2];
		if (i2 == userID)
		{
			if (SelectType == 3 || SelectType == 6)
			{
				continue;
			}
		}
		//Ŀ���Ƿ����
		if (userID!=i2)
		if (pList->isLive)
		{
			//Ŀ���Ƿ����
			if (!pList->CheckDie())
			{
				idlist[livenum] = i2;
				livenum += 1;
			}
		}
	}
	//
	if (SelectType == 3 || SelectType == 6)
	{
		if (0 == livenum)return userID;
	}
	else
	if (0 == livenum)return -1;
	g_pMainState->m_InterfaceDoor.m_TimeShiCheng.UpdateTime();
	int select = idlist[rand() % livenum];

	return select;
}


int cAI::GetNextPos(int ID, int Num, int bLive)
{
	if (Num < 0 || Num >19)
	{
		ERRBOX(Num);
		return -1;
	}
	int start, end;
	if (ID < 10)start = 0;
	else start = 10;
	end = start + 10;
	for (int i = start; i < end; i++)
	{
		sFightList& list = g_pMainState->m_FightList[i];
		switch (bLive)
		{
		case 0://ֻ�ҿ�λ
			if (!list.isLive)return i;
			break;
		case 1://ֻ�һ���
			if ((0 == list.isLive) || list.CheckDie())
			{
			}
			else return i;
			break;
		case 2://ֻҪ���ھ���
			if (list.isLive)
			{
				return i;
			}
			break;
		}
	}
	return -1;
}
int cAI::GetNear(int ID, int Num,int bLive)
{
	if (Num < 0 || Num >19)
	{
		ERRBOX(Num);
		return -1;
	}
	int start, end;
	if (ID < 10)start = 0;
	else start = 10;
	end =start+ 10;

	int xpos = g_pMainState->m_pFightState->m_xPosList[ID];
	int ypos = g_pMainState->m_pFightState->m_yPosList[ID];
	int gaplist[10];
	int idlist[10];
	for (int i = 0; i < 10; i++)
	{
		idlist[i] = start+i;
	}
	int startgap = 0;
	for (int i = start; i < end; i++)
	{
		sFightList& list = g_pMainState->m_FightList[i];
		switch (bLive)
		{
		case 0://ֻ�ҿ�λ
			if (list.isLive)
			{
				gaplist[startgap] = 999999;;
				startgap += 1;	
				continue;
			}
			break;
		case 1://ֻ�һ���
			if ((0 == list.isLive) || list.CheckDie())
			{
				gaplist[startgap] = 999999;;
				startgap += 1;
				continue;
			}
			break;
		case 2://ֻҪ���ھ���
			if (!list.isLive)
			{
				gaplist[startgap] = 999999;;
				startgap += 1;
				continue;
			}
			break;
		}
		
		int xpos2 = g_pMainState->m_pFightState->m_xPosList[i];
		int ypos2 = g_pMainState->m_pFightState->m_yPosList[i];
		gaplist[startgap] = abs(xpos - xpos2) + ((abs(ypos - ypos2))<<1);
		startgap += 1;
	}
	int pa, pb;
	for (int a = 0; a < startgap-1; a++)
	for (int b = a + 1; b<startgap; b++)
	{
		pa = gaplist[a];
		pb = gaplist[b];
		if (pa>pb)//ǰ�����ں���
		{
			swap(gaplist[a], gaplist[b]);
			swap(idlist[a], idlist[b]);
		}
	}//����������ǰλ��YС����ǰ��
	if (999999 == gaplist[Num])return -1;
	return idlist[Num];

}



void cAttackAI::Escape(int userID)
{
//#ifdef TESTDEBU
//	 g_pMainState->m_pFightState->m_bEscape = TRUE;
//	return ;
//#endif

	sFightList& list = g_pMainState->m_FightList[userID];
	list.m_WorkType = WORKTYPE_MAGIC;
	list.m_SkillSelect.Lv = 0;
	list.m_SkillSelect.SkillID = 1006;
	list.targetID = userID;
	list.m_SkillSelect.TargetID = userID;
}

void cAttackAI::Def(int userID)
{
	if (!g_pMainState->m_FightList[userID].isLive)return;
	cPetData* petdata;
	g_pMainState->m_FightList[userID].m_pData->GetDateP(petdata);
	if (petdata->m_FightProperty.cannotAll)return;
	g_pMainState->m_AI.m_AttackAI.m_FightTarget[userID] = userID;
	sBuff buf;
	buf.times = 1;
	g_pMainState->m_SkillManager.SetDateBuffUp(userID, userID, 1002, buf);
	g_pMainState->m_FightList[userID].m_WorkType = -2;
}

BOOL cAttackAI::UseItem(eAIUseItemType mode)
{
	if (m_TargetID == -1)return FALSE;
	int ItemOwnerID=m_UserID;
	if (g_pMainState->m_FightList[ItemOwnerID].m_pData->m_IndexType == INDEXTYEP_PET)
	{
		if ((m_UserID % 10) >4)
			ItemOwnerID = m_UserID - 5;
	}
	cCharacter* pc = (cCharacter*)g_pMainState->m_FightList[ItemOwnerID].m_pData->m_pIndex;
	sFightList& list = g_pMainState->m_FightList[m_UserID];
	if (!pc->m_PCData.m_Item[mode].GetNum())
		return FALSE;
	list.m_ItemSelectID = mode;
	list.m_SkillSelect.SkillID = 1003;
	if (mode == eAIUseItemType����)
		list.m_SkillSelect.SkillID = 1005;
	list.m_SkillSelect.Lv = 0;
	list.m_SkillSelect.TargetID = m_TargetID;
	list.m_WorkType = WORKTYPE_MAGIC;


	int start, end;
	if (m_TargetID < 10)start = 0;
	else start = 10;
	end = start + 10;
	switch (mode)
	{
	case eAIUseItemTypeHP:
		m_bNeedAddHp[m_TargetID] -= 50;
		break;
	case eAIUseItemTypeHPALL:
		if (1)
		{
			for (int i = start; i < end;i++)
				m_bNeedAddHp[m_TargetID] -= 30;
		}
		break;
	case eAIUseItemTypeMP:
		m_bNeedAddMp[m_TargetID]=0;
		break;
	case eAIUseItemTypeMPALL:
		if (1)
		{
			for (int i = start; i < end; i++)
				m_bNeedAddMp[m_TargetID] = 0;
		}
		break;
	case eAIUseItemTypelive:
		m_bNeedlive[m_TargetID] = 0;
		break;
	case eAIUseItemTypeUnseal:
		m_bNeedUnSeal[m_TargetID] = 0;
		break;
	case eAIUseItemType����:
		m_bAtk[m_TargetID] += 20;
		break;
	}
	return TRUE;
	
	
}


int cAI::FindDieFight(int start /*= 0*/, int end /*= 5*/)
{


	for (int i = start; i < end; i++)
	{
		if (g_pMainState->m_FightList[i].isLive)
		if (g_pMainState->m_FightList[i].CheckDie())
		{
			return i;
		}
	}
	return -1;
}




void cAttackAI::RunDT()
{
	if (m_FightTarget[m_UserID] > -1)
	{
		SetTarget(m_FightTarget[m_UserID]);
	}
	else SetTarget(-1);
	//���ȿ����ؼ�
	if (m_pUserPetData->m_AiLv > 70)
	{
		if (m_bPKMode)
		if (!(CheckINT(m_pUserPetData->m_FightProperty.m_AtkIntensify,1)))
		{
			if (CheckCanSkill(3018))
			{
				SetTarget(m_UserID);
				return UseMagic(3018);
			}
		}
		if (!m_pUserPetData->m_FightProperty.cannotSkill)
		{
			if (m_pUserPetData->m_StrongLv > 80)
			{
				if (CheckCanSkill(3034))
				{
					if (m_FightTarget[m_UserID] == -1)
						SetTarget(FindAtkTarget(200));
					return UseMagic(3034);
				}
			}
		}
		
	}
	if (!m_pUserPetData->m_FightProperty.cannotMagic)
	{
		//�����
		if (m_pUserPetData->m_LV > 129)
		{//���淨ϵ��ʱ��
			if (!m_pUser->CheckHaveState(8))
			{
				int magicnum = 0;
				int start, end;
				if (m_UserID < 10)start = 0; else start = 10;
				end = start + 10;
				cPetData* pPetdata;
				for (int i = start; i < end; i++)
				{
					g_pMainState->m_FightList[i].m_pData->GetDateP(pPetdata);
					if (pPetdata->m_AddPoint.mp>2)
						magicnum += 1;
				}
				if (magicnum>3)
				{
					SetTarget(m_UserID);
					return UseMagic(8);
				}
			}
		}


		if (m_FightTarget[m_UserID] == -1)
		{
			SetTarget(FindAtkTarget(150));
		}

		if (m_pUserPetData->m_LV > 29)
		{
			if (m_pUserPetData->m_HP > (m_pUserPetData->m_HPMax.GetData() >> 1))
			{//��ɨ
				if (m_FightTarget[m_UserID] == -1)
				{
					int targetid = FindAtkTarget(330);
					SetTarget(targetid);
				}
				if (!m_pUserPetData->m_FightProperty.cannotAttack)
				if (m_pTargetPetData->m_HP < m_pUserPetData->m_ATK.GetData() - m_pTargetPetData->m_DEF.GetData())
					return NormalAtk();
				return UseMagic(0);
			}
			else
			{
				if (m_FightTarget[m_UserID] == -1)
				{
					SetTarget(FindAtkTarget(150));
				}
				if (m_pUserPetData->m_LV > 129)
					return UseMagic(7);
				//��
				if (m_FightTarget[m_UserID] == -1)
				SetTarget(FindAtkTarget(150));
				return UseMagic(1);
			}
		}
		
	}
	if (!m_pUserPetData->m_FightProperty.cannotAttack)
	{
		if (m_FightTarget[m_UserID] == -1)
		SetTarget(FindAtkTarget(100));
		return NormalAtk();
	}
	return Def(m_UserID);
}
void cAttackAI::RunNR()
{
	BOOL bConduct = FALSE;
	if (m_FightTarget[m_UserID] > -1)
	{
		SetTarget(m_FightTarget[m_UserID]);
		bConduct = TRUE;
	}
	else SetTarget(-1);

	if (!m_pUserPetData->m_FightProperty.cannotMagic)
	{
		if (bConduct)
		{//���ָ��Ŀ�������²���Ŀ��û�б��� ���Է�ӡĿ��
			if (!m_bSeal[m_TargetID])
			if (!m_pTargetPetData->m_FightProperty.cannotMagic)
			{
				m_bSeal[m_TargetID] = TRUE;
				if (m_pUserPetData->m_LV > 129)
				{
					return UseMagic(59);
				}
				return UseMagic(54);
			}
		}
	}
	if (!m_pUserPetData->m_FightProperty.cannotUseItem)
	{//ʹ�ð���
		if (m_FightTarget[m_UserID] == -1)
		{
			SetTarget(g_pMainState->m_AI.AutoFindTarget(m_UserID, 4));
		}
		UseItem(eAIUseItemType����);
		return;
	}
	return	Def(m_UserID);
}
void cAttackAI::RunFC()
{
	if (m_FightTarget[m_UserID] > -1)
	{
		SetTarget(m_FightTarget[m_UserID]);
	}
	else SetTarget(-1);
	if (m_pUserPetData->m_AiLv > 80)
	if (!m_pUserPetData->m_FightProperty.cannotSkill)
	{
		if (m_pUserPetData->m_StrongLv > 70)
		{//Ц��

			if (m_FightTarget[m_UserID] == -1)
			{
				int target = FindCpMaxTarget();
				if (target > -1)
					SetTarget(target);
			}
			if (m_pTarget->m_IndexType == INDEXTYEP_CHARACTER)
			{
				cCharacter* pc = (cCharacter*)m_pTarget->m_pIndex;
				if (pc->m_PCData.m_CP > 100)
				{

					if (CheckCanSkill(3016))return UseMagic(3016);//Ц��
				}
			}
		}
	}

	if (!m_pUserPetData->m_FightProperty.cannotMagic)
	{
		if (m_FightTarget[m_UserID] ==-1)
		{
			int start, end;
			if (m_UserID < 10)start = 10;
			else start = 0;
			end = start + 10;
			int target2 = FindUnSeal(start, end);
			if (target2 == -1)
			{//�з�ȫ��������.........
				if (m_bPKMode)
				{
					if (m_pUserPetData->m_LV>129)
					{
						if (!m_pUser->CheckHaveState(26))
							return UseMagic(26);
						if (!m_pTarget->CheckHaveState(27))
							return UseMagic(27);
						return Def(m_UserID);
					}
				}
				else
					return UseMagic(28);
			}
			SetTarget(target2);
		}
	
		if (m_FightTarget[m_UserID] == -1)
		{
			if (!m_pTarget->CheckHaveState(17))
				return UseMagic(17);
		}
		else
		{
			//����ҷ��������͵Ķ໹�Ƿ����͵Ķ�
			int ouratknum = 0;
			int start, end;
			if (m_UserID < 10)start = 0;
			else start = 10;
			end = start + 10;
			cPetData* pPetdata;
			for (int i = start; i < end; i++)
			{
				g_pMainState->m_FightList[i].m_pData->GetDateP(pPetdata);
				if (pPetdata->m_AddPoint.atk> 2)
				{
					ouratknum += 1;
					continue;
				}
				if (pPetdata->m_AddPoint.mp > 2)
				{
					ouratknum -= 1;
					continue;
				}
			}
			if (ouratknum > 1)
			{
				return UseMagic(20);
			}
			else return UseMagic(14);
		}
		if (!m_pUserPetData->m_FightProperty.cannotUseItem)
		{
			if (CheckItem(eAIUseItemTypeMP)) { UseItem(eAIUseItemTypeMP); return; }
			if (CheckItem(eAIUseItemTypeMPALL)) { UseItem(eAIUseItemTypeMPALL); return; }
		}
	}else
	if (!m_pUserPetData->m_FightProperty.cannotUseItem)
	{
		if (CheckItem(eAIUseItemTypeUnseal)){ UseItem(eAIUseItemTypeUnseal); return; }
	}
	return Def(m_UserID);
}
void cAttackAI::RunHS()
{
	/*if (m_FightTarget[m_UserID] > -1)
	{
		SetTarget(m_FightTarget[m_UserID]);
	}
	else SetTarget(-1);*/
	//ʹ�÷���
	vector<POINT> tList;
	int start, end;
	if (m_UserID < 10)start = 0;
	else start = 10;
	end = start + 10;
	//����
	int num = FindEnemy(TRUE,FALSE,tList, eETѪ����, m_UserID);
	if (num)
	{
		SetTarget(tList[0].x);
		if (m_pTargetPetData->m_HPHurtMax < m_pTargetPetData->m_HPMax.GetData())
		{
			if (num>1)
				return UseMagic(42);
			else 	return UseMagic(38);
		}
		else
		{
			if (num > 1)
				return UseMagic(40);
			else 	return UseMagic(39);
		}
	}
	//��˫ר��
	if (g_pMainState->m_TriggerFightSuccess.m_TaskList[0].m_pTask->GetType() == 0)
	{
		SetTarget(g_pMainState->m_AI.AutoFindTarget(m_UserID, 4));
		return UseMagic(31);	
	}
	//��ջ���
	FindEnemy(TRUE,FALSE,tList, eET����ϵ, m_UserID, 33);
	if (tList.size()>2)
	{
		SetTarget(tList[0].x);
		return UseMagic(33);
	}
	//��ջ���
	FindEnemy(TRUE,FALSE,tList, eET�����, m_UserID, 35);
	if (tList.size()>2)
	{
		SetTarget(tList[0].x);
		return UseMagic(35);
	}
	//Τ�ӻ���
	FindEnemy(TRUE, FALSE, tList, eET�����, m_UserID, 34);
	if (tList.size() > 2)
	{
		SetTarget(tList[0].x);
		return UseMagic(34);
	}
	return Def(m_UserID);
}
void cAttackAI::RunLG()
{
	if (m_FightTarget[m_UserID] > -1)
	{
		SetTarget(m_FightTarget[m_UserID]);
	}
	else
	{
		SetTarget(g_pMainState->m_AI.AutoFindTarget(m_UserID, 4));
	}
	int start, end;
	if (m_UserID<10)start = 10;
	else start = 0;
	end = start + 10;
	switch (g_pMainState->GetLiveNum(start, end))
	{
	case 1://����
		if (m_pUserPetData->m_AiLv>90)
		if (m_pUserPetData->CheckHavePetSkill(2101))
		{
			return UseMagic(2101);
		}
		return UseMagic(79);
		break;
	case 2://����
		if (m_pUserPetData->m_LV > 129)
			return UseMagic(88);
	default://����
		return UseMagic(80);
		break;
	}
}
void cAttackAI::RunTG()
{
	if (m_FightTarget[m_UserID] > -1)
	{
		SetTarget(m_FightTarget[m_UserID]);
	}
	else SetTarget(-1);
	if (!m_pUserPetData->m_FightProperty.cannotSkill)
	{
	}
	if (!m_pUserPetData->m_FightProperty.cannotMagic)
	{
		if (g_pMainState->m_FightList[10].isLive)
		{
			cPetData* ppetdata;
			g_pMainState->m_FightList[10].m_pData->GetDateP(ppetdata);
			if (ppetdata->m_HP > m_pUserPetData->m_LV * 200)
			{
				SetTarget(10);
				if (!CheckCanSkill(72))
				{
					SetTarget(m_UserID);
					if (CheckItem(eAIUseItemTypeMP)) { UseItem(eAIUseItemTypeMP); return; }
					if (CheckItem(eAIUseItemTypeMPALL)){ UseItem(eAIUseItemTypeMPALL); return; }

					if (m_pUserPetData->m_AddPoint.atk > 2)//���칬
						return NormalAtk();
					return Def(m_UserID);
				}
				return UseMagic(72);
			}
		}
		if (m_pUserPetData->m_AddPoint.atk > 2)//���칬
		{
			if (m_FightTarget[m_UserID] == -1)
				SetTarget(FindAtkTarget(130));
			return  UseMagic(71);
		}
		else
		{
			if (m_pUserPetData->m_LV > 129)
			{
				if (m_FightTarget[m_UserID] == -1)
					SetTarget(FindAtkTarget(110,FALSE));
				return UseMagic(74);
			}
			else
			{
				if (m_FightTarget[m_UserID] == -1)
				{
					int start, end;
					if (m_UserID < 10)start = 10;
					else start = 0;
					end = start + 10;
					SetTarget(FindDefminTarget(start,end,eProPerty��Ѫ,TRUE));
				}
				if (m_pTargetPetData->m_HP > m_pUserPetData->m_LV * 200)
					return UseMagic(71);
				else
				{
					m_bSeal[m_TargetID] = TRUE;
					return UseMagic(68);
				}
			}
		}
	}
	if (!m_pUserPetData->m_FightProperty.cannotAttack)
	{
		if (m_pUserPetData->m_AddPoint.atk > 2)//���칬
		{
			if (m_FightTarget[m_UserID] == -1)
				SetTarget(FindAtkTarget(100));
			return NormalAtk();
		}
	}
	Def(m_UserID);
}
void cAttackAI::RunWZ()
{	
	if (m_FightTarget[m_UserID] > -1)
	{
		SetTarget(m_FightTarget[m_UserID]);
	}
	else SetTarget(-1);
	if (!m_pUserPetData->m_FightProperty.cannotMagic)
	{
		if (m_pUserPetData->m_AddPoint.atk > 2)//�ǲ��ǽ���ׯ
		{
			if (m_pUserPetData->m_HP > m_pUserPetData->m_HPMax.GetData() * 70 / 100)
			{//����
				if (m_TargetID == -1)SetTarget(FindAtkTarget(200));
				if (!m_pUserPetData->m_FightProperty.cannotAttack)
				if (m_pTargetPetData->m_HP < m_pUserPetData->m_ATK.GetData() - m_pTargetPetData->m_DEF.GetData())
					return NormalAtk();
				return UseMagic(97);
			}
			else
			{
				//����֮Ȫ
				if (m_pUser->CheckHaveState(91))
				{
					SetTarget(m_UserID);
					return UseMagic(91);
				}
				else return NormalAtk();
			}
		}
		if (m_TargetID == -1)
		{
			int start, end;
			if (m_UserID < 10)start = 11;
			else start = 1;
			end = start + 9;
			if (m_bPKMode)//�չ�Ǭ���ڷ�PKʱ�޷�������
			{
				start -= 1;
				end -= 5;
			}
			int target2= FindUnSeal(start, end);
			if (target2 == -1)
			{
				SetTarget(m_UserID);
				if (m_pUser->CheckHaveState(91))return UseMagic(91);
				SetTarget(FindAtkTarget(200));
				if (!m_pUserPetData->m_FightProperty.cannotAttack)
				if (m_pTargetPetData->m_HP < m_pUserPetData->m_ATK.GetData() - m_pTargetPetData->m_DEF.GetData())
					return NormalAtk();
				return UseMagic(97);
			}
			SetTarget(target2);
		}
		m_bSeal[m_TargetID] = TRUE;
		return UseMagic(89);//����
	}
	
	if (!m_pUserPetData->m_FightProperty.cannotAttack)
	{
		if (m_pUserPetData->m_AddPoint.atk > 2)
		{
			if (m_TargetID == -1)SetTarget(FindAtkTarget(100));
			return NormalAtk();
		}
	}

	if (!m_pUserPetData->m_FightProperty.cannotUseItem)
	{
		if (CheckItem(eAIUseItemTypeHP)){ UseItem(eAIUseItemTypeHP); return; }
	}
	return Def(m_UserID);
}
void cAttackAI::RunPT()
{
	if (m_FightTarget[m_UserID] > -1)
	{
		SetTarget(m_FightTarget[m_UserID]);
	}
	else SetTarget(-1);
	if (!m_pUserPetData->m_FightProperty.cannotAttack)
	{
	}
	if (!m_pUserPetData->m_FightProperty.cannotSkill)
	{
	}
	if (!m_pUserPetData->m_FightProperty.cannotMagic)
	{
		if (m_FightTarget[m_UserID] == -1)
		{
			SetTarget(g_pMainState->m_AI.AutoFindTarget(m_UserID, 4));
		}
		return UseMagic(105 + rand() % 5);
	}
	if (!m_pUserPetData->m_FightProperty.cannotUseItem)
	{
		
	}
	return Def(m_UserID);
}
void cAttackAI::RunDF()
{
	if (m_FightTarget[m_UserID] > -1)
	{
		SetTarget(m_FightTarget[m_UserID]);
	}
	else SetTarget(-1);
	if (!m_pUserPetData->m_FightProperty.cannotSkill)
	{
		if ((!m_pUserPetData->m_FightProperty.cannotSkill) && m_pUserPetData->m_AiLv > 90)
		{
			if (m_pUserPetData->m_AddPoint.atk > 2)//���ظ�
			{
				if (CheckCanSkill(3034))
				{
					if (m_FightTarget[m_UserID] == -1)
						SetTarget(FindAtkTarget(200));
					return UseMagic(3034);
				}
			}
		}
	}
	if (!m_pUserPetData->m_FightProperty.cannotAttack)
	{
		if (m_pUserPetData->m_AddPoint.atk > 2)//���ظ�
		{
			if (m_FightTarget[m_UserID] == -1)
				SetTarget(FindAtkTarget(150));
			return NormalAtk();
		}
	}
	
	if (!m_pUserPetData->m_FightProperty.cannotMagic)
	{
		int start, end;
		if (m_TargetID<10)start = 0;
		else start = 10;
		end = start + 10;
		if (g_pMainState->GetLiveNum(start, end) > 1)
		{
			SetTarget(g_pMainState->m_AI.AutoFindTarget(m_UserID, 4));
			return UseMagic(116);
		}
		else
		{
			SetTarget(g_pMainState->m_AI.AutoFindTarget(m_UserID, 4));
			UseMagic(117);
		}
	}
	return Def(m_UserID);
}
void cAttackAI::RunPS()
{
	BOOL bConduct = FALSE;
	if (m_FightTarget[m_UserID] > -1)
	{
		SetTarget(m_FightTarget[m_UserID]);
		bConduct = TRUE;
	}
	else SetTarget(-1);
	BOOL bCanMagic, bCanAtk,bCanSkill;
	if (!m_pUserPetData->m_FightProperty.cannotMagic)bCanMagic = TRUE;
	else bCanMagic = FALSE;
	if (!m_pUserPetData->m_FightProperty.cannotAttack)bCanAtk = TRUE;
	else bCanAtk = FALSE;
	if (!m_pUserPetData->m_FightProperty.cannotSkill)bCanSkill = TRUE;
	else bCanSkill = FALSE;
	if (m_pUserPetData->m_AddPoint.atk > 2)//����˿
	{
		if (m_FightTarget[m_UserID] == -1)
			SetTarget(FindAtkTarget(150));
		if (bCanAtk)
		{
			if(!bCanMagic)return NormalAtk();
			if (m_pUserPetData->m_ATK.GetData() - m_pTargetPetData->m_DEF.GetData() > m_pUserPetData->m_LV * 10)
				return NormalAtk();
			else
				return UseMagic(162); //���޵���
		}
		else
		{
			if (bCanMagic)UseMagic(162); //���޵���
		}
		
	}

	if (bCanMagic)
	{//����ʹ��ħ���������
		if (bConduct)
		{//���ָ��Ŀ�������²���Ŀ��û�б��� ���Է�ӡĿ��
			if (!m_bSeal[m_TargetID])
			{
				if (!m_pTargetPetData->m_FightProperty.cannotMagic)
				{
					m_bSeal[m_TargetID] = TRUE;
					return UseMagic(159);
				}

			}
			else
			{
				if (!m_bPKMode)
					return UseMagic(162); //���޵���
				else
					return UseMagic(155);//����
			}
		}
		if (!m_bPKMode)
		{
			if (m_FightTarget[m_UserID] == -1)
				SetTarget(g_pMainState->m_AI.AutoFindTarget(m_UserID, 4));
			return UseMagic(162); //���޵���
		}
		//PK
		if (m_FightTarget[m_UserID] == -1)
		{
			int unsealtarget = FindUnSeal(5);
			if (unsealtarget == -1)
			{//���еĵ��˶�������?
				SetTarget(g_pMainState->m_AI.AutoFindTarget(m_UserID, 4));
			}
			else
			{
				SetTarget(unsealtarget);
				return UseMagic(159);
			}
		}
	}
	return Def(m_UserID);
}
void cAttackAI::RunMW()
{
	if (m_FightTarget[m_UserID] > -1)
	{
		SetTarget(m_FightTarget[m_UserID]);
	}
	else SetTarget(g_pMainState->m_AI.AutoFindTarget(m_UserID, 4));


	int start, end;
	if (m_UserID<10)start = 10;
	else start = 0;
	end = start + 10;
	if (g_pMainState->GetLiveNum(start, end) > 1)
	{//��ɰ��ʯ	
		return UseMagic(131);
	}
	else
	{//��ζ���
		return UseMagic(132);
	}
}
void cAttackAI::RunST()
{
	BOOL bConduct = FALSE;
	if (m_FightTarget[m_UserID] > -1)
	{
		SetTarget(m_FightTarget[m_UserID]);
		bConduct = TRUE;
	}
	else SetTarget(-1);

	//���ȿ����ؼ�
	if (m_pUserPetData->m_AiLv > 70)
	{
		if (!m_pUserPetData->m_FightProperty.cannotSkill)
		{
			if (m_pUserPetData->m_StrongLv > 80)
			{
				if (CheckCanSkill(3034))
				{
					if (m_FightTarget[m_UserID] == -1)
						SetTarget(FindAtkTarget(200));
					return UseMagic(3034);
				}
			}
		}

	}
	if (!m_pUserPetData->m_FightProperty.cannotMagic)
	{
		if (m_pUser->CheckHaveState(145))
		{
			if (bConduct)
			{
				return UseMagic(148);
			}
			if (g_pMainState->GetLiveNum(10, 20) > 1)
			{
				SetTarget(FindAtkTarget(110));
				return UseMagic(142);
			}
			else
			{
				SetTarget(FindAtkTarget(330));
				return UseMagic(148);
			}
		}
		else
		{
			if (bConduct)
			if (!m_pUserPetData->m_FightProperty.cannotAttack)
				return NormalAtk();
			SetTarget(m_UserID);
			return UseMagic(145);
		}
	}
	if (!m_pUserPetData->m_FightProperty.cannotAttack)
	{
		if (bConduct)
			return NormalAtk();
		if (m_FightTarget[m_UserID] == -1)
			SetTarget(FindAtkTarget(100));
		return NormalAtk();
	}
	
	return Def(m_UserID);
}





void cAttackAI::UseMagic(int id)
{
	if (m_TargetID == -1)return;
	sFightList& list = g_pMainState->m_FightList[m_UserID];
	list.m_WorkType = WORKTYPE_MAGIC;
	list.m_SkillSelect.Lv = m_pUserPetData->m_LV + m_pUserPetData->m_StrongLv/5-10;
	if (list.m_SkillSelect.Lv < 0)list.m_SkillSelect.Lv = 0;
	if (list.m_SkillSelect.Lv>180)list.m_SkillSelect.Lv = 180;
	list.m_SkillSelect.SkillID = id;
	list.m_SkillSelect.TargetID = m_TargetID;
}

void cAttackAI::NormalAtk()
{
	sFightList& list = g_pMainState->m_FightList[m_UserID];
	list.m_WorkType = WORKTYPE_ATTACK;
	list.m_SkillSelect.Lv = 0;
	list.m_SkillSelect.SkillID = 1000;
	list.m_SkillSelect.TargetID = m_TargetID;
}

void cAttackAI::ResetNeed()
{
	for (int i = 0; i < 20; i++)
	{
		m_bNeedUnSeal[i] = 0;
		m_bNeedAddHp[i] = 0;
		m_bNeedAddMp[i] = 0;
		m_bNeedlive[i] = 0;
		m_bNeedAddHpHurt[i] = 0;
	}
}

BOOL cAttackAI::SaveOneself(int start,int end)
{
	for (int i = start; i < end; i++)
	{
		if (i == g_pMainState->m_HeroID)continue;
		SaveOneself2(i);
	}
	return FALSE;
}

void cAttackAI::GetNeed(int start, int end)
{
	for (int i = start; i < end; i++)
	{
		if (!g_pMainState->m_FightList[i].isLive)continue;
		SetUser(i);
		//����
		if (m_pUserPetData->m_HP == 0)
		{
			if (!m_pUserPetData->m_FightProperty.cannotLive)
			{
				m_bNeedlive[i] = TRUE;
			}
		}
		//Ѫ����
		if (m_pUserPetData->m_HP < (m_pUserPetData->m_HPMax.GetData()* m_HPLimit) / 100)
		{
			//Ѫ��,Խ��Ҫ��Ѫ
			int k = (m_pUserPetData->m_HP * 100) / m_pUserPetData->m_HPMax.GetData() + 1;
			k = 100 - k;
			if (m_pUserPetData->m_HPHurtMax < m_pUserPetData->m_HPMax.GetData())

				m_bNeedAddHpHurt[i] = TRUE;

			m_bNeedAddHp[i] = k;
	
			if (m_pUserPetData->m_FightProperty.cannotAddHP)//����Ҳ���
			{
				m_bNeedUnSeal[i] = TRUE;
			}
		}
		//������
		if (m_pUserPc)
		{
			if (m_pUserPc->m_PCData.m_MengPaiID != 0)//���Ʋ���Ҫ��
			{
				m_bNeedAddMp[i] = (m_pUserPetData->m_MP * 100) / m_pUserPetData->m_MPMax.GetData();
				m_bNeedAddMp[i] = 100 - m_bNeedAddMp[i];
			}
		}
		if (m_pUser->CheckHaveState(6))	
		{//���˷���
			m_bNeedUnSeal[i] = TRUE;
		}
		if (m_pUserPetData->m_FightProperty.cannotMagic)//����ʹ��ħ��
		{
			if (!m_pUser->m_StateList.size())continue;
			if (m_pUser->m_StateList.back().times < 3)
				continue;//3�غ����µ�BUFF���ù�
			if (i%10>4)
			if (m_pUserPetData->m_AddPoint.hp > 3)continue;//Ѫ�費�þ�
			m_bNeedUnSeal[i] = TRUE;
		}
		
		if (m_pUserPetData->m_FightProperty.cannotAttack)//���ܹ���
		{
			if (i % 10>4)
			if (m_pUserPetData->m_AddPoint.atk > 3)
				m_bNeedUnSeal[i] = TRUE;
		}
	}
}

void cAttackAI::SetTarget(int target)
{
	if (m_TargetID == target)return;
	m_TargetID = target;
	if (-1 == target)
	{
		m_pTarget = 0;
		m_pTargetPetData = 0;
		return ;
	}
	m_pTargetList = &g_pMainState->m_FightList[m_TargetID];
	m_pTarget= g_pMainState->m_FightList[m_TargetID].m_pData;
	m_pTarget->GetDateP(m_pTargetPetData);
	if (0 == m_pTargetPetData)
	{
		ERRBOX(target);
	}
}

void cAttackAI::SetUser(int userid)
{
	if (m_UserID == userid)return;
	m_UserID = userid;
	pUserList = &g_pMainState->m_FightList[userid];
	m_pUser = g_pMainState->m_FightList[userid].m_pData;
	if (m_pUser->m_IndexType==INDEXTYEP_CHARACTER)
		m_pUserPc=(cCharacter*)m_pUser->m_pIndex;
	else m_pUserPc = 0;
	m_pUser->GetDateP(m_pUserPetData);
}

void cAttackAI::Run()
{
	Reset();
	GetNeed(0, 20);
	for (int i = 0; i < 10; i++)
	{
		if ((i%5)==g_pMainState->m_HeroID)
		{
			if (g_pMainState->m_FightList[i].isLive)
			{
				SetUser(i);
				if (m_pUserPc)
				if (pUserList->m_WorkType == WORKTYPE_MAGIC)
				{//������� ��ӡ
					if (pUserList->m_SkillSelect.TargetID > 9)
					{
						m_bNeedUnSeal[pUserList->m_SkillSelect.TargetID] = TRUE;
					}
					else
					{
						m_bNeedAddHp[pUserList->m_SkillSelect.TargetID] -= 50;
					}
				}
				if (pUserList->m_WorkType == WORKTYPE_ITEM)
				{//������Ǽ�Ѫ
					if (pUserList->m_SkillSelect.TargetID < 10)
					{
					//	sItem2* pItem = &m_pUserPc->m_PCData.m_Item[pUserList->m_ItemSelectID];
						m_bNeedAddHp[pUserList->m_SkillSelect.TargetID] -= 50;
					}
				}
			}
		}
	}
	ZhiHui(TRUE);
	//PreRun(0,10);
	if (m_bPKMode)
	{
		ZhiHui(FALSE);
	}
	//
	for (int i = 0; i < 10; i++)
	{
		if (g_pMainState->m_pFightState->m_FightMenu.CheckControl(i))continue;
		g_pMainState->m_InterfaceDoor.m_TimeShiCheng.UpdateTime();
		if (g_pMainState->m_FightList[i].isLive)
		if (g_pMainState->m_FightList[i].m_WorkType == -1)
		{
			g_pMainState->m_InterfaceDoor.m_TimeShiCheng.UpdateTime();
			AutoRunAi(i);
		}
	}
	if (m_bPKMode)
	{
		for (int i = 10; i < 20; i++)
		{
			if (g_pMainState->m_pFightState->m_FightMenu.CheckControl(i))continue;
			g_pMainState->m_InterfaceDoor.m_TimeShiCheng.UpdateTime();
			if (g_pMainState->m_FightList[i].isLive)
			if (g_pMainState->m_FightList[i].m_WorkType == -1)
			{
				g_pMainState->m_InterfaceDoor.m_TimeShiCheng.UpdateTime();
				AutoRunAi(i);
			}
		}
	}
	else
	{
		for (int i = 10; i < 20; i++)
		{
			g_pMainState->m_InterfaceDoor.m_TimeShiCheng.UpdateTime();
			if (g_pMainState->m_FightList[i].isLive)
			if (g_pMainState->m_FightList[i].m_WorkType == -1)
			{
				g_pMainState->m_InterfaceDoor.m_TimeShiCheng.UpdateTime();
				AutoRunEnemyAi(i);
			}
		}
	}
	for (int i = 0; i < 20; i++)
	{
		m_FightTarget[i] = -1;
		m_bSeal[i] = FALSE;
		m_bAtk[i] = 0;
	}
}

void cAttackAI::PreRun(int start, int end)
{
	//CoutIdleList(start,end);
	GetNeed(start, end);
	ContentLive(start,end);
	ContentHP(start, end);
	ContentUnSeal(start, end);
	SaveOneself(start, end);
}

void cAttackAI::AutoRunAi(int i)
{
	SetUser(i);	
	if (!m_pUserPc)//�ٻ���
	{
		return RunPet();
	}
	if (m_pUserPetData->m_LV < 25)
	{
		if (m_FightTarget[m_UserID] > -1)
		{
			SetTarget(m_FightTarget[m_UserID]);
		}
		else
			SetTarget(g_pMainState->m_AI.AutoFindTarget(m_UserID, 4));
		return	NormalAtk();
	}
	

	eMengPai emngpaiid = (eMengPai)m_pUserPc->m_PCData.m_MengPaiID;
	switch (emngpaiid)
	{
	case eMP_���ƹٸ�:return RunDT();
	case eMP_����ɽ:return RunFC();
	case eMP_������:return RunHS();	
	case eMP_Ů����:return RunNR();	
	case eMP_�칬:return RunTG();
	case eMP_��������:return RunLG();
	case eMP_��ׯ��:return RunWZ();
	case eMP_����ɽ:return RunPT();
	case eMP_���ܵظ�:return RunDF();
	case eMP_ħ��կ:return RunMW();
	case eMP_ʨ����:return RunST();
	case eMP_��˿��:return RunPS();
	default:
		if (m_FightTarget[m_UserID] > -1)
		{
			SetTarget(m_FightTarget[m_UserID]);
		}
		else
			SetTarget(g_pMainState->m_AI.AutoFindTarget(m_UserID, 4));
		return NormalAtk();
		break;
	}
}

void cAttackAI::AutoRunEnemyAi(int i)
{
	vector<POINT> tlist;
	SetUser(i);
	if (m_pUserPetData->m_FightProperty.cannotAll)return;
	if (m_FightTarget[i] > -1)
	{
		SetTarget(m_FightTarget[i]);
	}
	else
		SetTarget(g_pMainState->m_AI.AutoFindTarget(i, 4));
	
//	int ailv = m_pUserPetData->m_AiLv;
	if (m_pUserPetData->m_FightProperty.cannotMagic)//���ⷨ��ʱ������
	{
		return NormalAtk();
	}
	if (!m_pUserPc)//�ٻ���
	{
		vector<int> skilllist;
		//Ѱ���ٻ��޵���������
		for (int j = 0; j < m_pUserPetData->m_NumofBBSkill; j++)
		{
			if (m_pUserPetData->m_pPetSkill[j]->m_SkillType == 4)
			{
				skilllist.push_back(m_pUserPetData->m_pPetSkill[j]->m_Id);
			}
		}
		if (skilllist.size())
		{//�����
			int skilluse = 2000 + skilllist[rand() % skilllist.size()];
			if (CheckCanSkill(skilluse))
				return UseMagic(skilluse);
		}
		return	NormalAtk();
	}
	eMengPai emngpaiid = (eMengPai)m_pUserPc->m_PCData.m_MengPaiID;
	if (m_pUserPetData->m_FightProperty.cannotMagic)
	{
		switch (emngpaiid)
		{
		case eMP_Ů����:
			if (!m_pUserPetData->m_FightProperty.cannotUseItem)
			 UseItem(eAIUseItemType����);
			else NormalAtk();
			return;
		default:
			NormalAtk();
			return;
		}
	}
	switch (emngpaiid)
	{
	case eMP_���ƹٸ�:
			switch (rand() % 7)
			{
			case 0:return NormalAtk();
			case 1:
				if (m_pUserPetData->m_HP<5000)
				{
					if (m_pUserPetData->m_LV > 129)
					{
						return NpcTryMagic(7);
					}
				}
				if (m_pUserPetData->m_FightProperty.cannotAttack)return NpcTryMagic(0);
				return NormalAtk();
				break;
			case 2:return NpcTryMagic(1);
			case 3:
				if (!CheckHaveState(i, 4))
				{
					if (CheckCanSkill(4))
					{
						SetTarget(i);
						return UseMagic(4);
					}
					else
						return NormalAtk();
				}
				else
				{
					if (m_pUserPetData->m_FightProperty.cannotAttack)return NpcTryMagic(0);
					return NormalAtk();
				}
			case 4:return NpcTryMagic(6);
			default:return NpcTryMagic(0);
			}
			break;
	case eMP_����ɽ:
		switch (rand() % 4)
		{
		case 0:
			if (m_pUserPetData->m_FightProperty.cannotAttack)
			{
				return NpcTryMagic(11);
			}
			else
			return NormalAtk();
		case 1:
			if (1)
			{
				int start, end;
				if (m_UserID < 10)start = 10;
				else start = 0;
				end = start + 10;
				int target2 = FindUnSeal(start, end);
				if (target2 == -1)return NormalAtk();
				SetTarget(target2);
				m_bSeal[m_TargetID] = TRUE;
				switch (rand() % 5)
				{
				case 1:return NpcTryMagic(14);
				case 2:return NpcTryMagic(15);
				case 3:return NpcTryMagic(19);
				case 4:return NpcTryMagic(20);
				default:return NpcTryMagic(21);
				}
			}
		default:return NpcTryMagic(11);
		}
		break;
	case eMP_������:
			switch (rand() % 7)
			{
			case 0:
				return UseMagic(31);
			case 1:
				if (m_pUserPetData->m_FightProperty.cannotAttack)
				{
					return NpcTryMagic(40);
				}
				else
				return NormalAtk();
			case 2:
				if (1)
				{
					SetTarget(g_pMainState->m_AI.AutoFindTarget(m_UserID, 3));
					switch (rand() % 5)
					{
					case 0: return NpcTryMagic(42);
					case 1:return NpcTryMagic(33);
					case 2: return NpcTryMagic(35);
					case 3: return NpcTryMagic(39);
					case 4: return NpcTryMagic(40);
					}
				}
				break;
			default:
				SetTarget(g_pMainState->m_AI.AutoFindTarget(m_UserID, 3));
				if (rand() % 2)return NpcTryMagic(40);
				else  return NpcTryMagic(42);
			}
			break;
	case eMP_Ů����:
			switch (rand() % 4)
			{
			case 0:
				if (m_pUserPetData->m_FightProperty.cannotAttack)
				{
					 UseItem(eAIUseItemType����);
					 return;
				}
				else
				return NormalAtk();
			case 1:
				if (1)
				{
					int start, end;
					if (m_UserID < 10)start = 10;
					else start = 0;
					end = start + 10;
					int target2 = FindUnSeal(start, end);
					if (target2 == -1)return NormalAtk();
					SetTarget(target2);
					m_bSeal[m_TargetID] = TRUE;
					if (m_pUserPetData->m_LV > 129)return NpcTryMagic(54);
					return NpcTryMagic(54);
				}
			default:
				UseItem(eAIUseItemType����);
				return;
			
			}
		break;
	case eMP_�칬:
			switch (rand() % 10)
			{
			case 1:
			case 2:
				if (1)
				{
					int start, end;
					if (m_UserID < 10)start = 10;
					else start = 0;
					end = start + 10;
					int target2 = FindUnSeal(start, end);
					if (target2 == -1)return NormalAtk();
					SetTarget(target2);
					m_bSeal[m_TargetID] = TRUE;
					if (rand() % 2)
					{
						return NpcTryMagic(68);
					}
					else return NpcTryMagic(69);
				}
				return;
			case 3:
				if (m_pUserPetData->m_FightProperty.cannotAttack)
				{
					return NpcTryMagic(71);
				}
				else
				return NormalAtk();
			case 4:return NpcTryMagic(72); 
			case 5: 
				if (m_pUserPetData->m_LV > 129)return NpcTryMagic(74);
			default:return NpcTryMagic(71);
			}
		break;
	case eMP_��������:
			switch (rand()%7)
			{
			case 0:return NpcTryMagic(84);
			case 1:return NpcTryMagic(79);
			case 2:
				if (m_pUserPetData->m_LV > 129)return NpcTryMagic(88);
				else 
				if (m_pUserPetData->m_FightProperty.cannotAttack)
				{
					return NpcTryMagic(80);
				}
				else
					return NormalAtk();
			case 3:
				if (!CheckHaveState(m_UserID,147))
				{
					if (CheckCanSkill(147))
					{
						SetTarget(m_UserID);
						return UseMagic(147);
					}
				}
				return NpcTryMagic(80);
			default:return NpcTryMagic(80);
			}
		break;
	case eMP_��ׯ��:
			switch (rand() % 7)
			{
			case 0: 
				if (m_pUserPetData->m_FightProperty.cannotAttack)
				{
					return NpcTryMagic(97);
				}
				else
				return NormalAtk();
			case 2: 
				if (1)
				{
					int start, end;
					if (m_UserID < 10)start = 11;
					else start = 1;
					end = start + 9;
					if (m_bPKMode)//�չ�Ǭ���ڷ�PKʱ�޷�������
					{
						start -= 1;
						end -= 5;
					}
					int target2 = FindUnSeal(start, end);
					if (target2 == -1)return NormalAtk();
					SetTarget(target2);
					m_bSeal[m_TargetID] = TRUE;
					return NpcTryMagic(89);
				}
			case 3: 
					SetTarget(g_pMainState->m_AI.AutoFindTarget(m_UserID, 3));
					return NpcTryMagic(91);
			default: 
				return NpcTryMagic(97);
			}
		break;
	case eMP_����ɽ:
		if (rand() % 3)
		{
			return NpcTryMagic(105 + rand() % 5);
		}
		else
		{
			int targetid = g_pMainState->m_AI.AutoFindTarget(m_UserID, 3);
			switch (rand() % 5)
			{
			case 0: 
				if (CheckCanSkill(113))
				{//�ߵ�����
					SetTarget(targetid);
					if (m_pTarget->CheckHaveState(113))
					{
						SetTarget(g_pMainState->m_AI.AutoFindTarget(i, 4));
						return NpcTryMagic(105 + rand() % 5);
					}
					return  NpcTryMagic(113);
				}
				return NpcTryMagic(105 + rand() % 5);
			case 1:
				if (CheckCanSkill(102))//�ն�
				{
					SetTarget(g_pMainState->m_AI.AutoFindTarget(m_UserID, 3));
					if (tlist.size())
					{
						SetTarget(tlist[0].x);
						return NpcTryMagic(102);
					}
				}
				SetTarget(g_pMainState->m_AI.AutoFindTarget(i, 4));
				return NpcTryMagic(105 + rand() % 5);
			case 2: 
				if (CheckCanSkill(103))
				return UseMagic(103);
				return NormalAtk();
			case 3: 
				if (CheckCanSkill(112))
				{
					SetTarget(targetid);
					return UseMagic(112);
				}
				return NpcTryMagic(105 + rand() % 5);
			case 4:
				if (m_pUserPetData->m_FightProperty.cannotAttack)
				{
					return NpcTryMagic(105 + rand() % 5);
				}
				else
				return NormalAtk();
			}
		}
		break;
	case eMP_���ܵظ�:
		if (1)
		{
			
			switch (rand() % 6)
			{
			case 0: 
				if (m_pUserPetData->m_FightProperty.cannotAttack)
				{
					return NpcTryMagic(117);
				}
				else
				return NormalAtk();
			case 1:return NpcTryMagic(117);
			case 3: return NpcTryMagic(122);
			case 4: 
				if (CheckCanSkill(123))
				{
					SetTarget(g_pMainState->m_AI.AutoFindTarget(m_UserID, 3));
					return NpcTryMagic(123);
				}
				else return NpcTryMagic(116);
			default: return NpcTryMagic(116);
			}
		}
		break;
	case eMP_ħ��կ:
		if (1)
		{
			switch (rand() % 6)
			{
			case 0: 
				if (m_pUserPetData->m_FightProperty.cannotAttack)
				{
					return NpcTryMagic(131);
				}
				else
				return NormalAtk();
			case 1:return NpcTryMagic(132);
			case 2:
				if (!m_pUser->CheckHaveState(133))
				{
					if (CheckCanSkill(133))
					{
						SetTarget(m_UserID);
						return NpcTryMagic(133);
					}
				}
				return NpcTryMagic(131);
			default:  return NpcTryMagic(131);
			}
		}
		break;
	case eMP_ʨ����:
		if (1)
		{
			if (m_pUser->CheckHaveState(145))
			{
				switch (rand() % 5)
				{
				case 0:return NpcTryMagic(143);
				case 1:return NpcTryMagic(148);
				case 2:return NpcTryMagic(141);
				default: return NpcTryMagic(142);
				}
			}
			else
			{
				switch (rand() % 3)
				{
				case 0:
					if (m_pUserPetData->m_FightProperty.cannotAttack)
					{
						SetTarget(m_UserID);
						return NpcTryMagic(145);
					}
					else
					return NormalAtk();
				default:
					SetTarget(m_UserID);
					return NpcTryMagic(145);
				}
			}
		}
		break;
	case eMP_��˿��:
		switch (rand() % 6)
		{
		case 0:
			if(rand()%2)
			return NpcTryMagic(154);
			return NpcTryMagic(155);
		case 1:return NpcTryMagic(156);
		case 2:
			if (1)
			{
				int start, end;
				if (m_UserID < 10)start = 10;
				else start = 0;
				end = start + 10;
				int target2 = FindUnSeal(start, end);
				if (target2 == -1)return NormalAtk();
				SetTarget(target2);
				m_bSeal[m_TargetID] = TRUE;
				return NpcTryMagic(159);
			}
		case 3:
			if (!CheckHaveState(m_UserID, 160))
			{
				if (CheckCanSkill(160))
				{
					SetTarget(m_UserID);
					return UseMagic(160);
				}
				
			}
			if (m_pUserPetData->m_FightProperty.cannotAttack)
			{
				return NpcTryMagic(162);
			}
			else
			return NormalAtk();
		case 4:return NpcTryMagic(164);
		default:return NpcTryMagic(162);
		}
		break;
	 default:
		 ERRBOX;
		break;
	}
}
void cAttackAI::ContentHP(int start, int end)
{
	
	for (int i = start; i < end; i++)
	{
		if (!m_bNeedlive[i])//ȷ��Ӧ�û���
		if (m_bNeedAddHp[i]>0 || m_bNeedAddHpHurt[i]>0)
		{
			SetTarget(i);
			ContentHP2(start, end);
			if (m_bNeedAddHp[i]<0)
			m_bNeedAddHp[i] = 0;
			if (m_bNeedAddHpHurt[i]<0)
			m_bNeedAddHpHurt[i] = 0;
		}
	}
}
void cAttackAI::ContentHP2(int start, int end)
{
	vector<int> list;
	int id2 = -1;
	int id;
	int firstpos = -1;
	for (int i = 19; i >-1; i--)
	{
		id = g_pMainState->m_pFightState->m_SpeedListPos[i];
		if (id < start || id >= end)continue;
		if (!g_pMainState->m_FightList[id].isLive)continue;
		if (g_pMainState->m_FightList[id].CheckDie())continue;
		if (g_pMainState->m_FightList[id].m_WorkType != -1)continue;

		if (-1 == id2)
		if (g_pMainState->m_FightList[id].m_pData->m_IndexType == INDEXTYEP_CHARACTER)
		{//
			cCharacter* pc = (cCharacter*)g_pMainState->m_FightList[id].m_pData->m_pIndex;
			if (!pc->m_PCData.m_FightProperty.cannotMagic)
			{
				switch (pc->m_PCData.m_MengPaiID)
				{
				case eMP_����ɽ:
				case eMP_������:
				case eMP_��ׯ��:
					id2 = id;
					firstpos = list.size();
					break;
				}
			}
		}
		cPetData* pPet;
		g_pMainState->m_FightList[id].m_pData->GetDateP(pPet);
		if (!pPet->m_FightProperty.cannotUseItem)
			list.push_back(id);
	}
	if (!list.size())return;
	if (-1 == id2)
	{
		id2 = list[0];
		firstpos = 0;
	}
	SetUser(id2);
	//BOOL bUse = FALSE;

		int num = 0;//��Ҫ���ƵĶ�����
		for (int i2 = start; i2 < end; i2++)
		if (m_bNeedAddHp[i2]>0 || m_bNeedAddHpHurt[i2]>0)
			num += 1;
		if (m_pUserPc)
		{
			eMengPai mengpai = (eMengPai)m_pUserPc->m_PCData.m_MengPaiID;
			switch (mengpai)
			{
			case eMP_������:
				if (1)
				{
					if (num == 1)
					{
						switch (m_pUserPetData->m_StrongLv >> 5)
						{
						case 3:
							if (CheckCanSkill(39))
							{
								m_bNeedAddHpHurt[m_TargetID] = -20;
								m_bNeedAddHp[m_TargetID] = -20;
								return UseMagic(39);
							}
						case 2:
							if (CheckCanSkill(3002))
							{
								m_bNeedAddHpHurt[m_TargetID] = -15;
								m_bNeedAddHp[m_TargetID] = -15;
								return UseMagic(3002);
							}
						case 1:
							if (CheckCanSkill(3001))
							{
								m_bNeedAddHpHurt[m_TargetID] = -10;
								m_bNeedAddHp[m_TargetID] = -10;
								return UseMagic(3001);
							}
							break;
						}
					}
					//����
					if (!m_pUserPetData->m_FightProperty.cannotMagic)
					{
						int skillid;
						if (num > 1)
						{
							if (m_bNeedAddHpHurt>0)skillid = 42;
							else skillid = 40;
						}
						else
						{
							
							if (m_bNeedAddHpHurt>0)skillid = 38;
							else
							{
								if (m_pUserPetData->m_LV > 129)
									skillid = 45;
								else
									skillid = 39;
							}

						}
						if (CheckCanSkill(skillid))
						{
							m_bNeedAddHpHurt[m_TargetID] = -10;
							m_bNeedAddHp[m_TargetID] = -10;
							return UseMagic(skillid);
						}
					}
				}
				break;
			case eMP_���ܵظ�:
				if (num > 1) //�ĺ�
				{
					if (m_pUserPetData->m_StrongLv > 90)
					{
						if (CheckCanSkill(7))
						{
							m_bNeedAddHpHurt[m_TargetID] = -20;
							m_bNeedAddHp[m_TargetID] = -20;
							return UseMagic(39);
						}
					}
				}
				break;
			case eMP_����ɽ:
				if (1)
				{
					if (num > 1) //�ĺ�
					{
						if (m_pUserPetData->m_StrongLv >90)
						{
							if (CheckCanSkill(7))
							{
								m_bNeedAddHpHurt[m_TargetID] = -20;
								m_bNeedAddHp[m_TargetID] = -20;
								return UseMagic(39);
							}
						}
					}
					//����
					if (!m_pUserPetData->m_FightProperty.cannotMagic)
					{
						if (CheckCanSkill(102))
						{
							m_bNeedAddHpHurt[m_TargetID] = -30;
							m_bNeedAddHp[m_TargetID] = -30;
							return UseMagic(102);
						}
					}
				}
				break;
			}
		}
		//����

		if (m_bNeedAddHp[m_TargetID]>0)
		{
			if (num > 3)
			{
				if (CheckItem(eAIUseItemTypeHPALL))
				{
					for (int i2 = start; i2 < end;i2++)
						m_bNeedAddHp[i2] =0;
					  UseItem(eAIUseItemTypeHPALL);
					  return;
				}
			}
			if (CheckItem(eAIUseItemTypeHP))
			{
				m_bNeedAddHp[m_TargetID] = 0;
				  UseItem(eAIUseItemTypeHP);
				  return;
			}
		}
		if (CheckItem(eAIUseItemTypeHPHurt))
		{
			m_bNeedAddHpHurt[m_TargetID] = 0;
			  UseItem(eAIUseItemTypeHPHurt);
			  return;
		}
	
}
void cAttackAI::ContentLive(int start, int end)
{
	for (int i = start; i < end; i++)
	{
		if (m_bNeedlive[i])
		{
			SetTarget(i);
			ContentLive2(start, end);
		}
	}
}

void cAttackAI::ContentLive2(int start, int end)
{
	vector<int> list;
	int id2=-1;
	int id;
	int firstpos=-1;
	for (int i = 0; i < 20; i++)
	{
		id= g_pMainState->m_pFightState->m_SpeedListPos[i];
		if (id < start || id >= end)continue;
		if (!g_pMainState->m_FightList[id].isLive)continue;
		if (g_pMainState->m_FightList[id].CheckDie())continue;
		if (g_pMainState->m_FightList[id].m_WorkType != -1)continue;

		if (-1==id2)
		if (g_pMainState->m_FightList[id].m_pData->m_IndexType == INDEXTYEP_CHARACTER)
		{//
			cCharacter* pc = (cCharacter*)g_pMainState->m_FightList[id].m_pData->m_pIndex;
			if (!pc->m_PCData.m_FightProperty.cannotMagic)
			{
				switch (pc->m_PCData.m_MengPaiID)
				{
				case eMP_����ɽ:
				case eMP_������:
					id2 = id;
					firstpos = list.size();
					break;
				case eMP_���ܵظ�:
					if (pc->m_PCData.m_LV > 129)
					{
						id2 = id;
						firstpos = list.size();
					}
				}
			}
		}
		cPetData* pPet;
		g_pMainState->m_FightList[id].m_pData->GetDateP(pPet);
		if (!pPet->m_FightProperty.cannotUseItem)
			list.push_back(id);
	}
	if (!list.size())return;
	if (-1 == id2)
	{
		id2 = list[0];
		firstpos =0;
	}
	SetUser(id2);
	BOOL bUse = FALSE;
	if (m_pUserPc)
	{
		eMengPai mengpai = (eMengPai)m_pUserPc->m_PCData.m_MengPaiID;
		switch (mengpai)
		{
		case eMP_����ɽ:
			if (1)
			{
				//�ؼ�   �����дȺ�
				if (m_pUserPetData->m_StrongLv > 95)
				{
					int num = 0;
					for (int i2 = start; i2<end; i2++)
					if (m_bNeedlive[i2])num += 1;
					if (num > 2)
					{
						if (CheckCanSkill(3048))
						{
							for (int i2 = start; i2 < end; i2++)
								m_bNeedlive[i2] = FALSE;
							UseMagic(3048);
							bUse = TRUE;
						}
					}
				}
				if (!bUse)
				//����
				if (!m_pUserPetData->m_FightProperty.cannotMagic)
				{
					if (CheckCanSkill(102))
					{
						m_bNeedlive[id2] = FALSE;
						 UseMagic(102);
						 bUse = TRUE;
					}
				}
			}
			break;
		case eMP_������:
			//����
			if (!m_pUserPetData->m_FightProperty.cannotMagic)
			{
				if (CheckCanSkill(37))
				{
					m_bNeedlive[id2] = FALSE;
					UseMagic(37);
					bUse = TRUE;
				}
			}
			break;
		case eMP_���ܵظ�:
			//����
			if (m_pUserPetData->m_LV > 129)
			if (!m_pUserPetData->m_FightProperty.cannotMagic)
			{
				if (CheckCanSkill(37))
				{
					m_bNeedlive[id2] = FALSE;
					UseMagic(37);
					bUse = TRUE;
				}
			}
			break;
		}
	}
	if (!bUse)
	//����
	if (!m_pUserPetData->m_FightProperty.cannotUseItem)
	if (CheckItem(eAIUseItemTypelive))
	{
		m_bNeedlive[m_TargetID] = FALSE;
		 UseItem(eAIUseItemTypelive);
		 bUse = TRUE;
	}
	if (!bUse)return;
	for (int i = firstpos + 1; i < list.size(); i++)
	{
		SetUser(list[firstpos + 1]);
		//����
		if (!m_pUserPetData->m_FightProperty.cannotUseItem)
		if (CheckItem(eAIUseItemTypelive))
		{
			m_bNeedlive[m_TargetID] = FALSE;
			m_bNeedAddHp[m_TargetID] = FALSE;
			m_bNeedAddHpHurt[m_TargetID] = FALSE;
			 UseItem(eAIUseItemTypeHPHurt);
			 return;
		}

	}
}

BOOL cAttackAI::CheckCanSkill(int skillid)
{//���һ�������Ƿ����,���編��,MP����,�򷵻�0
	int dubstbin;
	int skilllv=0;
	if (skillid < 1000)
	{
		sSkill*  skill = g_pMainState->m_SkillManager.GetMengPaiSkill(skillid);
		skilllv = m_pUserPc->m_PCData.m_MengPaiSkillLV[skill->m_Owner];
	}
	return g_pMainState->m_SkillManager.Cost(dubstbin, skilllv, skillid, m_UserID, 5, FALSE);
}

//void cAttackAI::CoutIdleList(int start, int end)
//{
//	int needlist[10];
//	for (int i = start; i < end; i++)
//	{
//		m_IdleList[i] = i;
//	}
//	for (int i = 0; i < 10; i++)
//	{
//		SetUser(i);
//		if (!m_pUserPc)
//		{
//			if (m_pUserPetData->m_AddPoint.hp>1 || m_pUserPetData->m_AddPoint.spd>1)
//			{
//				needlist[i] = 80;
//			}
//			else
//				needlist[i] = 10;
//		}
//		else
//		{
//			eMengPai mp = (eMengPai)m_pUserPc->m_PCData.m_MengPaiID;
//			switch (mp)
//			{
//			case eMP_���ƹٸ�:
//				needlist[i] = 1;
//				break;
//			case eMP_������:
//				needlist[i] = 99;
//				break;
//			case eMP_Ů����:
//				needlist[i] = 10;
//				break;
//			case eMP_����ɽ:
//				needlist[i] = 10;
//				break;
//			case eMP_ʨ����:
//				needlist[i] = 1;
//				break;
//			case eMP_ħ��կ:
//				needlist[i] = 1;
//				break;
//			case eMP_��˿��:
//				needlist[i] = 10;
//				break;
//			case eMP_���ܵظ�:
//				if (m_pUserPetData->m_AddPoint.atk > 2)
//				{
//					needlist[i] = 10;
//				}
//				else
//				{
//					needlist[i] = 98;
//				}
//				break;
//			case eMP_��������:
//				needlist[i] = 1;
//				break;
//			case eMP_��ׯ��:
//				needlist[i] = 90;
//				break;
//			case eMP_����ɽ:
//				needlist[i] = 95;
//				break;
//			case eMP_�칬:
//				needlist[i] = 80;
//				break;
//			default:
//				needlist[i] = 0;
//				break;
//			}
//		}
//		if (m_pUserPetData->m_FightProperty.cannotMagic)
//		{
//			needlist[i] += 10;
//		}
//		if (m_pUserPetData->m_FightProperty.chaos)
//		{
//			needlist[i] -= 100;
//			continue;
//		}
//		if (g_pMainState->m_FightList[i].m_bDie)
//		{
//			needlist[i] -= 100;
//			continue;
//		}
//	}
//	for (int a = 0; a < 9; a++)
//	for (int b = a + 1; b<10; b++)
//	{
//		if (needlist[a]<needlist[b])//ǰ�����ں���
//		{
//			swap(needlist[a], needlist[b]);
//			swap(m_IdleList[a+start], m_IdleList[b+start]);
//		}
//	}//����	
//}

void cAttackAI::Reset()
{
	cPetData* pPetdata;
	for (int i = 0; i < 20; i++)
	{

		m_bNeedAddHp[i] = 0;//���غϸö����Ƿ���Ҫ��Ѫ
		m_bNeedAddHpHurt[i] = 0;
		m_bNeedUnSeal[i] = 0;//���غϸö����Ƿ���Ҫ���
		m_bNeedlive[i] = 0;
		m_bNeedAddMp[i] = 0;
		//����ɫ�Ƿ񱻷�
		m_bBeSeal[i] = FALSE;
		if (g_pMainState->m_FightList[i].isLive)
		{
			g_pMainState->m_FightList[i].m_pData->GetDateP(pPetdata);
			if (pPetdata->m_FightProperty.cannotMagic)
				m_bBeSeal[i] = TRUE;
		}
		
	}
}

BOOL cAttackAI::CheckItem(eAIUseItemType mode)
{
	if (m_UserID % 10 <5)
	{
		if (m_pUserPc)
		if (m_pUserPc->m_PCData.m_Item[mode].GetNum())
			return TRUE;
	}
	else
	{
		if (INDEXTYEP_CHARACTER == g_pMainState->m_FightList[m_UserID - 5].m_pData->m_IndexType)
		{
			cCharacter* pc = (cCharacter*)g_pMainState->m_FightList[m_UserID - 5].m_pData->m_pIndex;
			if (pc->m_PCData.m_Item[mode].GetNum())
				return TRUE;
		}
	}
	return FALSE;
}

void cAttackAI::ContentUnSeal(int start, int end)
{
	for (int i = start; i < end; i++)
	{
		if (m_bNeedUnSeal[i])
		{
			SetTarget(i);
			ContentUnSeal2(start, end);
			m_bNeedUnSeal[i] = FALSE;
		}
	}
}

void cAttackAI::ContentUnSeal2(int start, int end)
{
	vector<int> list;
	int id2 = -1;
	int id;
	int firstpos = -1;
	for (int i = 19; i > -1; i--)
	{
		id = g_pMainState->m_pFightState->m_SpeedListPos[i];
		if (id < start || id >= end)continue;
		if (!g_pMainState->m_FightList[id].isLive)continue;
		if (g_pMainState->m_FightList[id].CheckDie())continue;
		if (g_pMainState->m_FightList[id].m_WorkType != -1)continue;
		cPetData* pPet;
		g_pMainState->m_FightList[id].m_pData->GetDateP(pPet);
		if (!pPet->m_FightProperty.cannotUseItem)
			list.push_back(id);
	}
	if (!list.size())return;
	if (-1 == id2)
	{
		id2 = list[0];
		firstpos = 0;
	}
	SetUser(id2);
//	BOOL bUse = FALSE;
	
	int num = 0;
	for (int i2 = start; i2 < end; i2++)
	if (m_bNeedUnSeal[i2])num += 1;
		
		if (m_pUserPc)
		{
			eMengPai mengpai = (eMengPai)m_pUserPc->m_PCData.m_MengPaiID;
			switch (mengpai)
			{
			case eMP_����ɽ:
			case eMP_��˿��:
			case eMP_���ܵظ�:
			case eMP_��ׯ��:
			case eMP_�칬:
			case eMP_Ů����:
				if (1)
				{
					if (num > 2)
					{
						switch (m_pUserPetData->m_StrongLv >> 6)
						{
						case 1:
							if (CheckCanSkill(3013))
							{
								for (int i2 = start; i2 < end; i2++)
								{
									m_bNeedUnSeal[i2] = FALSE;
									if (m_bNeedAddHp[i2]>0)
									{
										m_bNeedAddHp[i2] -= 20;
									}
								}
								return UseMagic(3013);
							}
						case 0:
							if (CheckCanSkill(3012))
							{
								for (int i2 = start; i2 < end; i2++)
									m_bNeedUnSeal[i2] = FALSE;
								return UseMagic(3012);
							}
							break;
						}
					}
					else
					//�ؼ�   
					switch (m_pUserPetData->m_StrongLv >> 6)
					{
					case 1:
						if (CheckCanSkill(3011))
						{
							m_bNeedUnSeal[m_TargetID] = FALSE;
							if (m_bNeedAddHp[m_TargetID]>0)
							m_bNeedAddHp[m_TargetID] -= 20;
							return UseMagic(3013);
						}
					case 0:
						if (CheckCanSkill(3010))
						{
								m_bNeedUnSeal[m_TargetID] = FALSE;
							return UseMagic(3012);
						}
						break;
					}
				}
				break;
			
			}
		}
		//����
		if (!m_pUserPetData->m_FightProperty.cannotUseItem)
		if (CheckItem(eAIUseItemTypeUnseal))
		{
			m_bNeedUnSeal[m_TargetID] = FALSE;
			  UseItem(eAIUseItemTypeUnseal);
			  return;
		}
}

cAttackAI::cAttackAI()
{
	for (int i = 0; i < 20; i++)
	{
		m_FightTarget[i] = -1;
		m_bSeal[i] = FALSE;
		m_bNeedAddHp[i]=0;//���غϸö����Ƿ���Ҫ��Ѫ
		 m_bNeedAddHpHurt[i]=0;
		 m_bNeedUnSeal[i]=0;//���غϸö����Ƿ���Ҫ���
		 m_bNeedlive[i]=0;
		m_bNeedAddMp[i]=0;
		 m_bBeSeal[i]=0;//�Ƿ񱻷�
		  m_bSeal[i]=0;//
		  m_bAtk[i]=0;//
	}
}

//int cAttackAI::FintTarget(int id)
//{
//	int targetid;
//	int start, end;
//	if (id < 10)start = 10;
//	else start = 0;
//	end = start + 10;
//
//	if (-1 == m_FightTarget[id])
//	{
//		SetUser(id);
//		if (m_pUserPc)
//		{
//
//			eMengPai mengpai = (eMengPai)m_pUserPc->m_PCData.m_MengPaiID;
//			switch (mengpai)
//			{
//			case eMP_��ׯ��:
//				if (m_pUserPetData->m_AddPoint.atk < 3)
//				{
//					return FindUnSeal(start, end);
//				}
//			case eMP_���ƹٸ�:
//			case eMP_ʨ����:
//			case eMP_�칬:
//				switch (rand() % 3)
//				{
//				case 0:
//					return FindDefminTarget(start, end, eProPerty����);
//				case 1:
//					return FindDefminTarget(start, end, eProPerty�˺�, 1);
//				case 2:
//					return FindDefminTarget(start, end, eProPerty����, 1);
//				}
//				break;
//			case eMP_������:
//				if (id < 10)start = 0;
//				else start = 10;
//				end = start + 10;
//				return FindDefminTarget(start, end, eProPerty����);
//				break;
//			case eMP_����ɽ:
//					return FindUnSeal(start, end);
//				break;
//			case eMP_ħ��կ:
//			case eMP_��������:
//					return FindDefminTarget(start, end, eProPerty����);
//				break;
//			case eMP_��˿��:
//				if (m_pUserPetData->m_AddPoint.atk > 2)
//					return FindDefminTarget(start, end, eProPerty����);
//				else
//					return FindUnSeal(start, end);
//			case eMP_���ܵظ�:
//				if (m_pUserPetData->m_AddPoint.atk>2)
//					return FindDefminTarget(start, end, eProPerty����);
//			case eMP_����ɽ:
//				switch (rand() % 3)
//				{
//				case 0:
//					return FindDefminTarget(start, end, eProPerty����, 1);
//				case 1:
//					return FindDefminTarget(start, end, eProPerty�˺�, 1);
//				case 2:
//					return FindDefminTarget(start, end, eProPerty����, 1);
//				}
//				break;
//			case eMP_Ů����:
//					return FindUnSeal(start, end);
//			}
//		}
//		else
//		{
//			if (m_pUserPetData->m_AddPoint.atk>2)
//				return FindDefminTarget(start, end, eProPerty����);
//			else
//				return FindDefminTarget(start, end, eProPerty����);
//		}
//	}
//	else
//	{
//		if (!g_pMainState->m_FightList[m_FightTarget[id]].isLive)
//		{
//			return FintTarget(id);
//		}
//		else targetid = m_FightTarget[id];
//	}
//	if (targetid == -1)return -1;
//	SetTarget(targetid);
//	return targetid;
//}

int cAttackAI::FindDefminTarget(int start, int end, eProPerty prop,BOOL maxormin)
{
	int id=-1;
	for (int i = start; i < end; i++)
	{
		if (g_pMainState->m_FightList[i].isLive)
		{
			id = i;
			break;
		}
	}
	if (-1 == id)return id;
	cPetData * pP1;
	cPetData* pP2;
	for (int i = id + 1; i < end; i++)
	{

		g_pMainState->m_FightList[id].m_pData->GetDateP(pP1);
		if (!g_pMainState->m_FightList[i].isLive)continue;
		g_pMainState->m_FightList[i].m_pData->GetDateP(pP2);
		if (maxormin)
		switch (prop)
		{
		case eProPerty�˺�:if (pP1->m_ATK.GetData() < pP2->m_ATK.GetData())id = i; break;
		case eProPerty����:if (pP1->m_DEF.GetData() < pP2->m_DEF.GetData())id = i; break;
		case eProPerty����:if (pP1->m_Wakan.GetData() < pP2->m_Wakan.GetData())id = i; break;
		case eProPerty�ٶ�:if (pP1->m_SPD.GetData() < pP2->m_SPD.GetData())id = i; break;
		case eProPerty��Ѫ:	if (pP1->m_HP < pP2->m_HP)id = i;	break;
		}
		else
			switch (prop)
		{
			case eProPerty�˺�:if (pP1->m_ATK.GetData() > pP2->m_ATK.GetData())id = i;break;
			case eProPerty����:if (pP1->m_DEF.GetData() > pP2->m_DEF.GetData())id = i;break;
			case eProPerty����:if (pP1->m_Wakan.GetData() > pP2->m_Wakan.GetData())id = i;break;
			case eProPerty�ٶ�:if (pP1->m_SPD.GetData() > pP2->m_SPD.GetData())id = i;break;
			case eProPerty��Ѫ:	if (pP1->m_HP > pP2->m_HP)id = i;	break;
		}
		
	}
	return id;
}

int cAttackAI::FindUnSeal(int start, int end)
{
	int num = end - start;
	int id=rand()%num+start;
	for (int i = 0; i < num+1; i++)
	{
		if (g_pMainState->m_FightList[id].isLive)
		if ((!m_bBeSeal[id]) && (!m_bSeal[id]))
		{
			m_bSeal[id] = TRUE;
			return id;
		}
		id += 1;
		if (id == end)
			id = start;
	}

	return -1;
}

int cAttackAI::FindUnSeal(int num/*=10*/)
{
	int start, end;
	if (m_UserID < 10)start = 10;
	else start = 0;
	end = start + num;
	return FindUnSeal(start, end);
}

void cAttackAI::SaveOneself2(int i)
{
	if (!g_pMainState->m_FightList[i].isLive)return;
	if (g_pMainState->m_FightList[i].CheckDie())return;
	if (g_pMainState->m_FightList[i].m_WorkType != -1)return;
	SetUser(i);
	SetTarget(i);
	if (m_pUserPc)
	if (!m_pUserPetData->m_FightProperty.cannotSkill)
	{
		if (m_pUserPetData->m_StrongLv>90)
		{
			if (m_bNeedAddHp[i] > 0 || m_bNeedAddHpHurt[i] > 0)
			{
				if (CheckCanSkill(3006))
				{
					m_bNeedAddHp[i] = FALSE;
					m_bNeedAddHpHurt[i] = FALSE;
					return UseMagic(3006);//����
				}
			}
			if (m_bNeedAddMp[i])
			{
				if (CheckCanSkill(3004))
				{
					m_bNeedAddMp[i] = FALSE;
					return UseMagic(3004);//�����  
				}
			}
		}
		else
		if (m_pUserPetData->m_StrongLv > 50)
		{
			if (m_bNeedAddHp[i] || m_bNeedAddHpHurt[i])
			{
				if (CheckCanSkill(3005))
				{
					m_bNeedAddHp[i] = FALSE;
					m_bNeedAddHpHurt[i] = FALSE;
					return UseMagic(3005);//����
					
				}
			}
			if (m_bNeedAddMp[i])
			{
				if (CheckCanSkill(3003))
				{
					m_bNeedAddMp[i] = FALSE;
					return UseMagic(3003);//������
				}
			}
		}
	}
	if (m_pUserPetData->m_FightProperty.cannotUseItem)return;
	if (m_bNeedAddHpHurt[i])
	{
		if (CheckItem(eAIUseItemTypeHPHurt))
		{
			UseItem(eAIUseItemTypeHPHurt);
		}
		return;
	}
	if (m_bNeedAddHp[i])
	{
		if (CheckItem(eAIUseItemTypeHP))
		{
			UseItem(eAIUseItemTypeHP);
		}
		return;
	}
	if (m_bNeedUnSeal[i])
	{
		if (CheckItem(eAIUseItemTypeUnseal))
			 UseItem(eAIUseItemTypeUnseal);//���
		return;
	}
	if (m_bNeedAddMp[i])
	{
		if (CheckItem(eAIUseItemTypeMP))
			 UseItem(eAIUseItemTypeMP);
		return;
	}
}



int cAttackAI::FindAtkTarget(int rate, BOOL ATKOrMagic)
{ //Ѱ����������������м�ֵ�Ķ���
	int start, end;
	if (m_UserID < 10)start = 10;
	else start = 0;
	end = start + 10;
	int num = end - start;
	int id = rand() % num+start;
	int idret=-1;
	int damage=0;
	int damage2;
	int DamageAddPercent;
	int DamageDefAddPercent;
	for (int i = 0; i < num+1; i++)
	{
		if (g_pMainState->m_FightList[id].isLive)
		if (!g_pMainState->m_FightList[id].CheckDie())
		{
			SetTarget(id);
			int targethp = m_pTargetPetData->m_HP - m_bAtk[id];
			if (ATKOrMagic)
			{
				DamageAddPercent = m_pUserPetData->m_DamagePercent.d1; //����������
				DamageDefAddPercent = m_pTargetPetData->m_DamagePercent.d2;   //Ŀ���������
				damage2 = ((m_pUserPetData->m_ATK.GetData() - m_pTargetPetData->m_DEF.GetData())*rate) / 100;
			}
			else
			{
				DamageAddPercent = m_pUserPetData->m_DamageAddPercentMagic.d1; //��������������
				DamageDefAddPercent = m_pTargetPetData->m_DamageAddPercentMagic.d2;   //Ŀ�귨����������
				damage2 = ((m_pUserPetData->m_Wakan.GetData() - m_pTargetPetData->m_Wakan.GetData())*rate) / 100;	
			}
			damage2 = damage2*(100 + DamageAddPercent - DamageDefAddPercent) / 100;//��������
			if (damage2>targethp)
			{
				//���ֵ�HPԽ��Խ��    �ҷ����˺�Խ��Խ��
				damage2 = (targethp <<2) - damage2;
			}
			
			if (-1 == idret)
			{
				idret = id;
				damage = damage2;
			}
			else
			{
				if (m_pTargetPetData->m_HP - m_bAtk[id] < 1)
				{
					continue;
				}
				if (damage2 > damage)
				{
					idret = id;
					damage = damage2;
				}
			}
		}
		id += 1;
		if (id >= end)
			id = start;
	}
	if (idret>-1)
		m_bAtk[idret] += damage; //��¼���󵥻غ��ܵ����˺���Ԥ��ֵ
	return idret;
}

int cAttackAI::FindCpMaxTarget(int num/*=100*/)
{
	int start, end;
	if (m_UserID < 10)start = 10;
	else start = 5;
	end = start + 5;
	cPetData* petdata;
	int id = rand() % 5 + start;
	for (int i = 0; i < 5; i++)
	{
		if (!g_pMainState->m_FightList[id].isLive)continue;
		g_pMainState->m_FightList[id].m_pData->GetDateP(petdata);
		if (petdata->m_CP>num)
		{
			return id;
		}
		id += 1;
		if (id >= end)id = start;
	}
	return -1;
}

BOOL cAttackAI::CheckHaveState(int userid, int skillid)
{
	return g_pMainState->m_FightList[userid].m_pData->CheckHaveState(skillid);
}

void cAttackAI::NpcTryMagic(int id)
{
	if (CheckCanSkill(id))
	{
		return UseMagic(id);
	}
	else
	{
		if (m_UserID<10 && m_TargetID<10)
			SetTarget(g_pMainState->m_AI.AutoFindTarget(m_UserID, 4));
		else
		if (m_UserID > 9 && m_TargetID > 9)
			SetTarget(g_pMainState->m_AI.AutoFindTarget(m_UserID, 4));
		return NormalAtk();
	}
}

void cAttackAI::RunPet()
{
	if (!g_pMainState->m_FightList[m_UserID].isLive)return;
	//Ѱ��Ŀ��
	if (m_FightTarget[m_UserID] > -1)
	{
		SetTarget(m_FightTarget[m_UserID]);
	}
	else
	{
		int targetid = g_pMainState->m_AI.AutoFindTarget(m_UserID, 4);
		if (targetid == -1)
		{
			return Def(m_UserID);
		}
		SetTarget(targetid);
	}
	//Ѱ�Ҽ���
	vector<sSkill*> skilllist;
	BOOL bHaveBuff=FALSE;//�Ƿ��״̬����
	BOOL bHaveMagic=FALSE;//�Ƿ�ᵥ��
	BOOL bHaveMagic2=FALSE;//�Ƿ��Ⱥ��
	for (int j = 0; j < m_pUserPetData->m_NumofBBSkill; j++)
	{
		switch (m_pUserPetData->m_pPetSkill[j]->m_SkillType)
		{
		case 4:
			if (1)
			{
				if (1<g_pMainState->m_SkillManager.GetPetSkill(m_pUserPetData->m_pPetSkill[j]->m_Id)->m_TargetNum)
				{
					bHaveMagic2 = TRUE;
				}
				else bHaveMagic = TRUE;
			}
			break;
		case 7:continue;
		default:
			bHaveBuff = TRUE;
			break;
		}
		skilllist.push_back(m_pUserPetData->m_pPetSkill[j]);
	}
	//�������������
	if (skilllist.size())
	{
		if (bHaveBuff)
		{
			if (rand() % 3 == 1)
			{
				//״̬�෨��
				for (int i = 0; i < skilllist.size(); i++)
				{
					if (skilllist[i]->m_SkillType == 2)
					{
						int skillid = skilllist[i]->m_Id + 2000;
						if (!m_pUser->CheckHaveState(skillid))
						{
							if (CheckCanSkill(skillid))
							{
								SetTarget(m_UserID);
								return UseMagic(skillid);
							}
						}
					}
				}
			}
		}
		if (bHaveMagic && (0 == bHaveMagic2))
		{
			for (int i = 0; i < skilllist.size(); i++)
			{//����
				if (g_pMainState->m_SkillManager.m_PetSkillList[skilllist[i]->m_Id].m_EffectType == 0)
				{
					return NpcTryMagic(2000 + skilllist[i]->m_Id);
				}
			}
		}
		else
		if ((0==bHaveMagic) && bHaveMagic2)
		{
			for (int i = 0; i < skilllist.size(); i++)
			{//��Ⱥ��
				if (g_pMainState->m_SkillManager.m_PetSkillList[skilllist[i]->m_Id].m_EffectType == -3)
				{
					return NpcTryMagic(2000 + skilllist[i]->m_Id);
				}
			}
		}
		else
		if (bHaveMagic&& bHaveMagic2)
		{
			//ʹ�õ�����Ⱥ��
			int start, end;
			if (m_UserID<10)start = 10;
			else start = 0;
			end = start + 10;
			if (g_pMainState->GetLiveNum(start, end) > 1)
			for (int i = 0; i < skilllist.size(); i++)
			{//��Ⱥ��
				if (g_pMainState->m_SkillManager.m_PetSkillList[skilllist[i]->m_Id].m_EffectType == -3)
				{
					return NpcTryMagic(2000 + skilllist[i]->m_Id);
				}
			}

			for (int i = 0; i < skilllist.size(); i++)
			{//����
				if (g_pMainState->m_SkillManager.m_PetSkillList[skilllist[i]->m_Id].m_EffectType == 0)
				{
					return NpcTryMagic(2000 + skilllist[i]->m_Id);
				}
			}
		}
		
	}
	if (m_pUserPetData->m_AddPoint.atk < 2)
		return Def(m_UserID);
	return	NormalAtk();
}

void cAttackAI::Summon(int id)
{
	g_pMainState->m_FightList[m_UserID].m_SkillSelect.SkillID = 1004;
	g_pMainState->m_FightList[m_UserID].m_SkillSelect.TargetID = g_pMainState->m_FightList[m_UserID].m_FriendListid + 5;
	g_pMainState->m_FightList[m_UserID].m_SkillSelect.Lv = id;
	g_pMainState->m_FightList[m_UserID].m_WorkType = WORKTYPE_SUMMON;
}

int cAttackAI::FindEnemy(BOOL CheckDie,BOOL bCheckWork,vector<POINT>& out, eEnemyType type, int userid, int data, int data2)
{
	out.clear();
	int ret=0;
	vector<POINT> list;
	cPetData* pPet;
	POINT pt;
	cCharacter* pc;
	int randstart, randend;
	if (userid<10)randstart = 0; else randstart = 10;
	randend = randstart + 10;
	for (int i = randstart; i < randend; i++)
	{
		sFightList& list = g_pMainState->m_FightList[i];

		if (list.m_pData->m_IndexType == INDEXTYEP_CHARACTER)pc = (cCharacter*)list.m_pData->m_pIndex;
		else pc = 0;
		if (!list.isLive)continue;
		list.m_pData->GetDateP(pPet);
		if (bCheckWork)if (list.m_WorkType!=-1)continue;
		if (CheckDie)if (pPet->m_HP < 1)continue;
		
		if (-1 < data)
		if (list.m_pData->CheckHaveState(data))
			continue;
		if (-1 < data2)
		if (list.m_pData->CheckHaveState(data))
			continue;
			switch (type)
			{
			case cAttackAI::eET����:
				if (pc)continue;
			case cAttackAI::eET����ϵ:
				if (pPet->m_AddPoint.atk > 1)//����1�����ϵĶ���Ϊ����ϵ
				{
				
					pt.x = i;
					pt.y = pPet->m_ATK.GetData();
					out.push_back(pt);
					ret += 1;
				}
				break;
			case cAttackAI::eET����:
				if (pc)continue;
			case cAttackAI::eET��ϵ:
				if (pPet->m_AddPoint.mp > 1)//����1ħ���ϵĶ���Ϊ��ϵ
				{
	
					pt.x = i;
					pt.y = pPet->m_Wakan.GetData();
					out.push_back(pt);
					ret += 1;
				}
				break;
			case cAttackAI::eET��ϵ:
				if (!pc)continue;
				switch (pc->m_PCData.m_MengPaiID)
				{
				case eMP_Ů����:
				case eMP_����ɽ:
				case eMP_��˿��:
				case eMP_��ׯ��:
				case eMP_�칬:
					pt.x = i;
					pt.y = 1;
					out.push_back(pt);
					ret += 1;
					break;
				}
				break;
			case cAttackAI::eETѪ����:
				if (pPet->m_HP < 1)
					pt.y = 100;
				else
				{
					pt.y = (100 * pPet->m_HP) / pPet->m_HPMax.GetData();
					if (pt.y < m_HPLimit)ret += 1;
					pt.y = -pt.y;
				}
				pt.x = i;
				out.push_back(pt);
				break;
			case cAttackAI::eET��Ҫ��ϵ:
				if (pc)
				{
					switch (pc->m_PCData.m_MengPaiID)
					{
					case eMP_���ƹٸ�:continue;
					default:
						pt.y = -(100 * pc->m_PCData.m_MP) / pc->m_PCData.m_MPMax.GetData();
						pt.x = i;
						out.push_back(pt);
						break;
					}
				}
				break;
			case cAttackAI::eETҽ��ϵ:
				if (g_pMainState->m_pFightState->m_FightMenu.CheckControl(i))continue;
				if (!pc)continue;
				if (pc->m_PCData.m_HP < 1)continue;
				switch (pc->m_PCData.m_MengPaiID)
				{
				case eMP_��ׯ��:
				case eMP_������:
				case eMP_����ɽ:
					pt.y = pPet->m_SPD.GetData();
					pt.x = i;
					out.push_back(pt);
					ret += 1;
					break;	
				}
				break;
			case cAttackAI::eETѪ��:
				if (g_pMainState->m_pFightState->m_FightMenu.CheckControl(i))continue;
				if (pc)continue;		
				if (pPet->m_AddPoint.hp > 1)
				{
					pt.x = i;
					pt.y = pPet->m_SPD.GetData();
					out.push_back(pt);
					ret += 1;
				}
				break;
			case cAttackAI::eET����������:
				if (!pc)continue;
				if (list.m_pData->CheckHaveState(2094) || list.m_pData->CheckHaveState(129))
				{
					pt.x = i;
					pt.y = 1;
					out.push_back(pt);
					ret += 1;
				}	
				break;
			case cAttackAI::eET�����:
				pt.x = i;
				pt.y = -pPet->m_DEF.GetData();
				out.push_back(pt);
				ret += 1;
				break;
			case cAttackAI::eET�����:
				pt.x = i;
				pt.y = -pPet->m_Wakan.GetData();
				out.push_back(pt);
				ret += 1;
				break;
			case cAttackAI::eET��������:
				if (!pc)continue;
				if (pc->m_PCData.m_FightProperty.cannotAttack)
				{
					pt.x = i;
					pt.y = 1;
					out.push_back(pt);
					ret += 1;
				}
				break;
			case cAttackAI::eET���ⷨ��:
				if (!pc)continue;
				if (pc->m_PCData.m_FightProperty.cannotMagic)
				{
					pt.x = i;
					pt.y = 1;
					out.push_back(pt);
					ret += 1;
				}
				break;
			case cAttackAI::eET������:
				if (!pc)continue;
				if (pc->m_PCData.m_FightProperty.cannotAddHP)
				{
					pt.x = i;
					pt.y = 1;
					out.push_back(pt);
					ret += 1;
				}
				break;
			case cAttackAI::eET���ٻ���:
				if (g_pMainState->m_pFightState->m_FightMenu.CheckControl(i))continue;
				if (i > 14)continue;
				if (!pc)continue;
				if (!g_pMainState->m_FightList[i + 5].isLive)
				{
					for (int i2 = 0; i2 < pc->m_PCData.m_NumofBB; i2++)
					if (pc->m_PCData.m_pPetDataList[i2]->m_PetDataZZ.m_HP>10)
					{
						pt.x = i;
						pt.y = 1;
						out.push_back(pt);
						ret += 1;
					}
				}
				break;
			case cAttackAI::eET��������:
				if (!pc)continue;
				if (0 == pPet->m_HP)
				{
					if (list.m_pData->CheckHaveState(2094))continue;//����
					if (list.m_pData->CheckHaveState(129))continue;//������
					pt.x = i;
					pt.y = 1;
					out.push_back(pt);
					ret += 1;
				}
				break;
			case cAttackAI::eET�ٶ����:
				if (g_pMainState->m_pFightState->m_FightMenu.CheckControl(i))continue;
				pt.x = i;
				pt.y = pPet->m_SPD.GetData();
				out.push_back(pt);
				ret += 1;
				break;
			case eET��Ѫ:
				if (list.CheckDie())break;
				if (pPet->m_FightProperty.cannotAll)break;
				if (pc)
				{
					switch (pc->m_PCData.m_MengPaiID)
					{
					case eMP_��ׯ��:
					case eMP_������:
					case eMP_����ɽ:
					case eMP_���ܵظ�:
						pt.y = 10;			
						break;
					case eMP_Ů����:
					case eMP_����ɽ:
					case eMP_��˿��:
					case eMP_�칬:
						pt.y = 6;
						break;
					default:
						pt.y = 9 - pPet->m_AddPoint.atk;
						break;

					}
				}
				else
				{
					if (pPet->m_AddPoint.mp > pPet->m_AddPoint.atk)
					{
						pt.y = 14 -pPet->m_AddPoint.mp;
					}
					else
					{
						pt.y = 14 - pPet->m_AddPoint.atk;
					}
				}
				pt.x = i;
				out.push_back(pt);
				ret += 1;
				break;
			case eET���:
				if (list.CheckDie())break;
				if (pPet->m_FightProperty.cannotAll)break;
				if (pc)
				{
					pt.y = 1;
				}
				else
				{
					if (pPet->m_AddPoint.mp > 1 || pPet->m_AddPoint.atk > 1)
					{
						if (pPet->m_AddPoint.mp > 3 || pPet->m_AddPoint.atk > 3)
							pt.y = 5;
						else
							pt.y = 3;
						out.push_back(pt);
					}
					else
					{
						pt.y = 2;
					}
					break;
				}
				pt.x = i;
				out.push_back(pt);
				ret += 1;
				break;
			default:ERRBOX(0);
				return -1;
				}
		
	}
	if (out.size()>1)
	{//���������ŵ���ǰ��
		for (int i = 0; i < out.size()-1;i++)
		for (int j = i+1; j < out.size(); j++)
		{
			if (out[i].y < out[j].y)
			{
				swap(out[i].x, out[j].x);
				swap(out[i].y, out[j].y);
			}
		}
	}
	return ret;
}

void cAttackAI::ZhiHui(BOOL bOur)
{
	int start = 10;
	int end;
	if (bOur)start = 0;
	end = start + 10;
	vector<POINT> tList;
	vector<POINT> tList2;
	//�ٻ�������,������
	int num2;
	int num= FindEnemy(TRUE,TRUE,tList, eET���ٻ���, start);
	for (int i = 0; i < num; i++)
	{	
		if (g_pMainState->m_pFightState->m_FightMenu.CheckControl(i))continue;
		SetUser(tList[i].x);
		for (int j = 0; j < m_pUserPc->m_PCData.m_NumofBB; j++)
		{
			if (m_pUserPc->m_PCData.m_pPetDataList[j]->m_PetDataZZ.m_HP>10)
			{
				sPetData* pPetdata = m_pUserPc->m_PCData.m_pPetDataList[j]->GetData();
				if (!pPetdata)
				{
					m_pUserPc->m_PCData.m_pPetDataList[j]->SetData(g_pMainState->m_PetDataManager.GetPetData(m_pUserPc->m_PCData.m_pPetDataList[j]->m_PetDataZZ.modeid));
				}
				if (m_pUserPc->m_PCData.m_pPetDataList[j]->GetData()->m_LVLimit <= m_pUserPc->m_PCData.m_LV)
				{
					Summon(j);
				}
			}
		}
	}
	//��������,����
	num = FindEnemy(FALSE, FALSE, tList2, eET��������, start);
	if (num)
	{
		num2 = FindEnemy(TRUE, TRUE, tList, eET�ٶ����, start,6);
		if (!num2)
		{
			num2 = FindEnemy(TRUE, TRUE, tList, eETҽ��ϵ, start,6);
		}

		if (num2)
		{
			resurgence2(tList, tList2, 0);
		}
	}
	//�����Ѫ
	if (FindEnemy(FALSE,TRUE,tList, eET��Ѫ, start))
		Cure2(tList, start, end);
	//���
	FindEnemy(FALSE, TRUE, tList, eET�ٶ����, start,6);
	UnSeal2(tList,start,end );
	CureMP2(tList,start,end);
}

BOOL cAttackAI::resurgence(int user, int target)
{
	
	SetUser(user);
	if (pUserList->m_WorkType > -1)return FALSE;
	SetTarget(target);
	if (INDEXTYEP_CHARACTER == m_pUser->m_IndexType)
	{
		if (95 < m_pUserPetData->m_StrongLv)
		{
			if (CheckCanSkill(3009))
			{
				UseMagic(3009);
				return TRUE;
			}
		}
		if (!m_pUserPetData->m_FightProperty.cannotMagic)
		{
			switch (m_pUserPc->m_PCData.m_MengPaiID)
			{
			case eMP_����ɽ:
				if (CheckCanSkill(104))
				{
					UseMagic(104);
					return TRUE;
				}
				break;
			case eMP_������:
				if (CheckCanSkill(37))
				{
					UseMagic(37);
					return TRUE;
				}
				break;
			case eMP_���ܵظ�:
				if (m_pUserPc->m_PCData.m_LV > 128)
				{
					if (CheckCanSkill(125))
					{
						UseMagic(125);
						return TRUE;
					}
				}
				break;
			}
		}
	}
	if (UseItem(eAIUseItemTypelive))return TRUE;

	return FALSE;

}

void cAttackAI::resurgence2(vector<POINT>& user, vector<POINT>& target, int targetID)
{
	if (target.size() <= targetID)return;
	BOOL blist=TRUE;//�Ƿ�A��B B��C
	int i;
	for (i = 0; i < user.size(); i++)
	{
		if (target[targetID].x == user[i].x)
		{//������ٶ�̫��
			blist = FALSE;
			continue;
		}
		if (resurgence(user[i].x, target[targetID].x))
			break;
	}
	if (!blist)return resurgence2(user,target,targetID+1);
	//A��B B��C C��D
	if (i == g_pMainState->m_HeroID)return;
	if (i == g_pMainState->m_HeroID+5)return;
	i = targetID;
	for (; i < target.size() - 1; i++)
	{
		if (!resurgence(target[i].x, target[i+1].x))break;
	}
	//D��BB
	if (!g_pMainState->m_FightList[target[i].x + 5].isLive)
	{
		SetUser(target[i].x);
		int j = 0;
		for (; j < m_pUserPc->m_PCData.m_NumofBB; j++)
		{
			if (m_pUserPc->m_PCData.m_pPetDataList[j]->m_PetDataZZ.m_HP>10)
			if (m_pUserPc->m_PCData.m_pPetDataList[j]->GetData()->m_LVLimit <= m_pUserPc->m_PCData.m_LV)
			{
				Summon(j);
			}
		}
		//û��BB,�Լ���ҩ
		if (j == m_pUserPc->m_PCData.m_NumofBB)
		{
			SetTarget(target[i].x);
			if (!UseItem(eAIUseItemTypeHPHurt))
			{
				//�����κδ���
			}
		}
	}
}

void cAttackAI::CorrectEnemy(vector<POINT>& out, eEnemyCorrect typ)
{
	BOOL bFind=FALSE;
	int i;
	for (i = 0; i < out.size(); i++)
	{
		SetTarget(out[i].x);
		switch (typ)
		{
		case cAttackAI::eEC����:
			if (!m_pTargetList->isLive)bFind = TRUE;
			if (m_pTargetList->CheckDie() && (!m_bNeedlive[m_TargetID]))bFind = TRUE;
			if (m_pTargetPetData->m_FightProperty.cannotUseItem)bFind = TRUE;
			break;
		default:
			break;
		}
		if (bFind)
		{
			out.erase(out.begin() + i);
			return CorrectEnemy(out, typ);
		}
	}
	return;
}

BOOL cAttackAI::Cure(int user, int target,BOOL toall)
{
	SetUser(user);
	if (pUserList->m_WorkType > -1)return FALSE;
	SetTarget(target);
	BOOL bHurt = FALSE;
	if (m_pTargetPetData->m_HPHurtMax < m_pTargetPetData->m_HPMax.GetData())
		bHurt = TRUE;
	int start = 10;
	int end;
	if (target<10)start = 0;
	end = start + 10;

	if (m_pUserPc)
	{
		if (!m_pUserPetData->m_FightProperty.cannotMagic)
		switch (m_pUserPc->m_PCData.m_MengPaiID)
		{
		case eMP_������:
			if (bHurt)
			{
				if (toall)
				{
					UseMagic(42); 
					for (int i = start; i < end; i++)
						m_bNeedAddHp[i] -= 20;
					return TRUE;
				}
				else
				{
					UseMagic(38);
					m_bNeedAddHp[target] -= 40;
					return TRUE;
				}
			}
			else
			{
				if (toall)
				{
					UseMagic(40);
					for (int i = start; i < end; i++)
						m_bNeedAddHp[i] -= 30;
					return TRUE;
				}
				else
				{
					UseMagic(39);
					m_bNeedAddHp[target] -= 60;
					return TRUE;
				}
			}
			break;
		case eMP_��ׯ��:
			if (!m_pTarget->CheckHaveState(91))
			{
				UseMagic(91);
				for (int i = start; i < end; i++)
					m_bNeedAddHp[i] -= 20;
				return TRUE;
			}
			break;
		case eMP_����ɽ:
			if (!m_pTarget->CheckHaveState(102))
			{
				UseMagic(102);
				m_bNeedAddHp[target] -= 30;
				return TRUE;
			}
			break;
		}
	}
	if (!m_pUserPetData->m_FightProperty.cannotUseItem)
	{
		BOOL haveall=FALSE;
		if (m_pUserPc)
		{
			if (m_pUserPc->m_PCData.m_Item[1].GetNum())haveall = TRUE;
		}
		else
		{
			if (!m_pUserPetData->m_Controler.CheckBB())return FALSE;
			if (user - 5 < 0){
				ERRBOX(user - 5);
				return FALSE;
			}
			cCharacter* pc = (cCharacter*)g_pMainState->m_FightList[user-5].m_pData->m_pIndex;
			if (pc->m_PCData.m_Item[1].GetNum())haveall = TRUE;
		}
		if (toall&&haveall)
		{	
			UseItem(eAIUseItemTypeHPALL);
			for (int i = start; i < end; i++)
				m_bNeedAddHp[i] -= 40;
			return TRUE;
		}
		else
		{
			if (bHurt)
			{
				if (!UseItem(eAIUseItemTypeHPHurt))
					UseItem(eAIUseItemTypeHP);
				m_bNeedAddHp[target] -= 50;
			}
			else
			{
				if (!UseItem(eAIUseItemTypeHP))
					UseItem(eAIUseItemTypeHPHurt);
				m_bNeedAddHp[target] -= 60;
			}
			return TRUE;
		}
	}
	return FALSE;
}


BOOL cAttackAI::CureMP(int user, int target, BOOL toall /*= FALSE*/)
{
	SetUser(user);
	SetTarget(target);
	if (m_pUserPc)
	{
		if (m_pUserPetData->m_FightProperty.cannotMagic)
			switch (m_pUserPc->m_PCData.m_MengPaiID)
		{
			case eMP_��ׯ��:
				if (!m_pTarget->CheckHaveState(90))
				{
					UseMagic(90);
					return TRUE;
				}
				else
				{
					if (target == user)
					{
						UseMagic(94);
						return TRUE;
					}
				}
				break;
			case eMP_����ɽ:
				if (target == user)
				{
					if (m_pUserPetData->m_HP > 1)
					{
						UseMagic(23);
						return TRUE;
					}
				}
				break;
		}
	}
	if (!m_pUserPetData->m_FightProperty.cannotUseItem)
	{
		if (toall)
		{
			UseItem(eAIUseItemTypeMPALL);
			return TRUE;
		}
		else
		{
			UseItem(eAIUseItemTypeMP);
			return TRUE;
		}
	}
	return FALSE;
}

void cAttackAI::UnSeal2(vector<POINT>& tlist, int start, int end)
{
	UINT start1 = 0;
	for (int i = start; i < end; i++)
	{
		if (m_bNeedUnSeal[i])
		{
			for (int j = start1; j < tlist.size(); j++)
			{
				if (UnSeal(tlist[j].x, i))
				{
					start1 += 1;
					if (start1 == tlist.size())return;
					break;
				}
			}

		}
	}
}

void cAttackAI::Cure2(vector<POINT>& tlist, int start, int end)
{
	BOOL toall = TRUE;
	int start1 = 0;
	for (int i = start; i < end; i++)
	{
		if (m_bNeedAddHp[i]>m_HPLimit)
		{
			for (int j = start1; j < tlist.size(); j++)
			{
				if (toall)
				{
					int num = 0;
					int limit2 = m_HPLimit / 2;
					for (int k = start; k < end; k++)
					{
						if (m_bNeedAddHp[k] >limit2)
							num += 1;
					}
					if (num < 2)toall = FALSE;
				}
				if (Cure(tlist[j].x, i, toall))
				{
					start1 += 1;
					if (start1 == tlist.size())return;
					break;
				}
			}

		}
	}
}

BOOL cAttackAI::UnSeal(int user, int target)
{
	SetUser(user);
	if (!pUserList->isLive)return FALSE;
	if (pUserList->CheckDie())return FALSE;
	if (m_pUserPetData->m_FightProperty.cannotUseItem)return FALSE;
	SetTarget(target);
	UseItem(eAIUseItemTypeUnseal);
	return TRUE;
}

void cAttackAI::CureMP2(vector<POINT>& tlist, int start, int end)
{
	BOOL toall = TRUE;
	int start1 = 0;
	for (int i = start; i < end; i++)
	{
		if (m_bNeedAddMp[i]>80)
		{
			for (int j = start1; j < tlist.size(); j++)
			{
				if (toall)
				{
					int num = 0;
					for (int k = start; k < end; k++)
					{
						if (m_bNeedAddMp[k] >60)
							num += 1;
					}
					if (num < 3)toall = FALSE;
				}
				if (CureMP(tlist[j].x, i, toall))
				{
					start1 += 1;
					if (start1 == tlist.size())return;
					break;
				}
			}

		}
	}
}
