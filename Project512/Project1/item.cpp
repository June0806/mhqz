#include "global.h"


BOOL cItem::Init()
{
	if (m_bInit)return TRUE;
	m_bInit = TRUE;
	m_MagicAddHP.Load(0x77D2082D);
	m_MagicAddMP.Load(0x3E912705);
	m_MagicAddHPMP.Load(0x808CEB72);
#pragma omp parallel sections
	{
#pragma omp  section
		{
			ifstream File;
			int id,num;
			ostringstream oss;
			oss << g_strMediaPath << "����//����//����.txt";
			File.open(oss.str());
			int typenum;
			File >> typenum;
			for (int j = 0; j < typenum; j++)
			{
				File >> id;
				File >> num;
				string dustbin;
				for (int i = 0; i < 11; i++)
				{
					vector<sWeaponData2>& data = m_pWeaponList2[j][i];
					data.resize(num);
					for (int k = 0; k < num; k++)
					{
						data[k].Read(File);
						if ( k== 0)getline(File, dustbin);//��ע
					}
				}
			}
			File.close();
			oss.str("");
			oss << g_strMediaPath << "����//����//��.txt";
			LoadItem1(oss.str());
		}

#pragma omp  section
		{
		ostringstream oss;
		oss << g_strMediaPath << "����//����//��.txt";
		LoadItem1(oss.str());
		oss.str("");
		oss << g_strMediaPath << "����//����//ǹ.txt";
		LoadItem1(oss.str());
		oss.str("");
		oss << g_strMediaPath << "����//����//��.txt";
		LoadItem1(oss.str());
		oss.str("");
		oss << g_strMediaPath << "����//����//��.txt";
		LoadItem1(oss.str());
		oss.str("");
		}
#pragma omp  section
		{
			ostringstream oss;
			oss << g_strMediaPath << "����//����//��.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "����//����//צ.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "����//����//��.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "����//����//Ʈ��.txt";
			LoadItem1(oss.str());
		}
#pragma omp  section
		{
			ostringstream oss;
			oss << g_strMediaPath << "����//����//��.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "����//����//��.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "����//����//˫��.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "����//����//��.txt";
			LoadItem1(oss.str());
		}
#pragma omp  section
		{
			ostringstream oss;
			oss << g_strMediaPath << "����//����//��.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "����//����//��.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "�����б�//����װ��.txt";
			LoadZuoqiZhuangShi(oss.str());
			oss.str("");
			oss << g_strMediaPath << "����//װ��//ͷ��.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "����//װ��//����.txt";
			LoadItem1(oss.str());
			oss.str("");

		}
#pragma omp  section
		{
			ostringstream oss;
			oss << g_strMediaPath << "����//װ��//����.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "����//װ��//Ů��.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "����//װ��//����.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "����//װ��//����.txt";
			LoadItem1(oss.str());
		}
#pragma omp  section
		{
			ostringstream oss;
			oss << g_strMediaPath << "����//װ��//Ь.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "�����б�\\ҩƷ.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "�����б�\\���.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "�����б�\\����.txt";
			LoadFaBao(oss.str());
			oss.str("");
			oss << g_strMediaPath << "�����б�\\�޾�.txt";
			LoadShouJue(oss.str());
		}
#pragma omp  section
		{
			ostringstream oss;
			oss << g_strMediaPath << "�����б�\\����.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "�����б�\\����.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "�����б�\\����.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "�����б�\\���ɵ��ӵ���.txt";
			LoadItem1(oss.str());
		}
#pragma omp  section
		{
			ostringstream oss;
			oss << g_strMediaPath << "�����б�\\��.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "�����б�\\��ʯ.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "����\\װ��\\�ٻ��޻���.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "����\\װ��\\�ٻ�������.txt";
			LoadItem1(oss.str());
		}
#pragma omp  section
		{
			ostringstream oss;
			oss << g_strMediaPath << "����\\װ��\\�ٻ�����Ȧ.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "����//����//��.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "�����б�\\����.txt";
			LoadItem1(oss.str());
			oss.str("");
			oss << g_strMediaPath << "�����б�\\�Ҿ�.txt";
			LoadJiaJu(oss.str());
		}
#pragma omp  section
	{
		ifstream File;
		int num;
		ostringstream oss;
		oss << g_strMediaPath << "�����б�\\�޾�����.txt";
		File.open(oss.str());
		int ID;
		File >> num;
		for (int i = 0; i < num; i++)
		{
			File >> ID;
			m_LShouJueList.push_back(ID);
		}
		File >> num;
		for (int i = 0; i < num; i++)
		{
			File >> ID;
			m_HShouJueList.push_back(ID);
		}
		File.close();
	}
#pragma omp  section
		{
			ifstream File;
			int num,ID;
			ostringstream oss;
			oss << g_strMediaPath << "�����б�\\��Ʒ������.txt";
			File.open(oss.str());
			if (!File)ERRBOX;
			File >> num;
			string name;
			for (int i = 0; i < num; i++)
			{
				File >> ID;
				File >> name;
				m_ItemTypeName.push_back(name);
			}
			File.close();
			oss.str("");
			oss << g_strMediaPath << "�����б�\\��ʯ.txt";
			File.open(oss.str());
			if (!File)ERRBOX;
			int type;
			File >>type;
			DWORD smallhead, bighead;
			vector<sItem>& list= m_ItemList[type];
			File >> bighead;
			File >> smallhead;
			File >> name;
			File >> num;
			list.resize(num);
			for (int i = 0; i < num; i++)
			{
				File >> list[i].m_ID;
				File >> ID;//����
				File >> list[i].m_Price;
				list[i].m_Name=name;
				File >> list[i].m_Text;
				list[i].m_Type = type;
				list[i].m_BigHead = bighead;
				list[i].m_SmallHead = smallhead;
			}
			File.close();
		}
	}

	for (int i = 0; i < 22; i++)
	{
		for (int j = 0; j < m_ItemList[i].size(); j++)
		{
			m_ItemList[i][j].SetLv(GetlvByID(i, j));
		}
	}
	return TRUE;
}



BOOL cItem::UseItem22(sItem2* pItem, int ObjId)
{
	BOOL success=FALSE;
	m_bAddHp = FALSE;
	m_bAddMp = FALSE;
	m_bAddCp = FALSE;
	m_bToAll = FALSE;
	switch (pItem->GetID())
	{
	case 0:	//ͬ���⸬  
		success = AddAll(&cItem::AddHp, 100, ObjId, 10000);
		break;
	case 1://��Ҷ��   
		success = AddHp(150, ObjId);
		break;
	case 2://����ذ�
		success = AddHp(400, ObjId);
		break;
	case 3://�˲�
		success = AddHp(600, ObjId);
		break;
	case 4://Ѫɫ�軨
		success = AddHp(800, ObjId);
		break;
	case 5://�ܵ�
		success = AddHp(1000, ObjId);
		break;
	case 6://��ǲ�
		success = AddHp(1500, ObjId);
		break;
	case 7://Ѫɺ��
		success = AddHp(2000, ObjId);
		break;
	case 8://������֥
		success = AddHp(3000, ObjId);
		break;
	case 9://��֮��м
		success = AddHp(5000, ObjId);
		break;
	case 10://��ҩ
		success = AddHp(7000, ObjId);
		break;
	case 11://��ѩɢ
		success = AddHp(10000, ObjId);
		break;
	case 12://�˽���Ҷ
		success = AddAll(&cItem::AddHp, 100, ObjId);
		break;
	case 13://��Ҷ��
		success = AddAll(&cItem::AddHp, 150, ObjId);
		break;
	case 14://¹��
		success = AddAll(&cItem::AddHp, 300, ObjId);
		break;
	case 15://���β
		success = AddAll(&cItem::AddHp, 600, ObjId);
		break;
	case 16://���֮��
		success = AddAll(&cItem::AddHp, 1000, ObjId);
		break;
	case 17://С����
		success = AddAll(&cItem::AddHp, 2000, ObjId);
		break;
	case 18://������
		success = AddAll(&cItem::AddHp, 5000, ObjId);
		break;
	case 19://����
		success = AddMp(20, ObjId);
		break;
	case 20://������
		success = AddMp(50, ObjId);
		break;
	case 21://��Ҷ
		success = AddMp(100, ObjId);
		break;
	case 22://��٢�޻�
		success = AddMp(300, ObjId);
		break;
	case 23://����ˮ
		success = (600, ObjId);
		break;
	case 24://�ͷ���¶
		success = AddMp(1000, ObjId);
		break;
	case 25://��ˮ��Ԫ��
		success = AddMp(3000, ObjId);
		break;
	case 26://�ϵ���
		success = AddAll(&cItem::AddMp, 20, ObjId);
		break;
	case 27://�����
		success = AddAll(&cItem::AddMp, 50, ObjId);
		break;
	case 28://�����ͷ
		success = AddAll(&cItem::AddMp, 100, ObjId);
		break;
	case 29://����ˮ
		success = AddAll(&cItem::AddMp, 200, ObjId);
		break;
	case 30://������
		success = AddAll(&cItem::AddMp, 300, ObjId);
		break;
	case 31://����
		success = AddAll(&cItem::AddMp, 400, ObjId);
		break;
	case 32://�ɺ���
		success = AddAll(&cItem::AddMp, 600, ObjId);
		break;
	case 33://������
		success = AddAll(&cItem::AddMp, 1000, ObjId);
		break;
	case 34://�ݹ�
		success = AddHpHurt(50, ObjId);
		break;
	case 35://�����
		success = AddHpHurt(100, ObjId);
		break;
	case 36://ˮ����
		success = AddHpHurt(200, ObjId);
		break;
	case 37://ɽҩ
		success = AddHpHurt(800, ObjId);
		break;
	case 38://��֬
		success = AddAll(&cItem::AddHpHurt, 100, ObjId);
		break;
	case 39://��ʯӢ
		success = AddAll(&cItem::AddHpHurt, 500, ObjId);
		break;
	case 40://�¼���
		success = AddHpHurt(50, ObjId);
		AddHp(100, ObjId);
		break;
	case 41://��¶Ϊ˪
		success = AddHpHurt(200, ObjId);
		AddHp(500, ObjId);
		break;
	case 42://��ȸ��
		success = AddHpHurt(400, ObjId);
		AddHp(1000, ObjId);
		break;
	case 43://��Ы����
		success = AddHpHurt(1000, ObjId);
		AddHp(3000,ObjId);
		break;
	case 44://���������
		success = Resurgence(1,ObjId);
		if (success)
			AddHpHurt(99, ObjId);
		break;
	case 45://��ת�ػ굤
		success = Resurgence(30, ObjId, 10000);
		break;
	case 46://��ɫ��
		success = Detoxify(eBuffType��, ObjId,TRUE);
		break;
	case 47://�����ֻ�
		success = AddHp(30, ObjId, 10000);
		break;
	case 48://ǧ�걣�ĵ�
		success = AddHp(60, ObjId, 10000);
		break;
	case 49://������
		
		{
			success = TRUE;
			Detoxify(eBuffType��ӡ, ObjId, TRUE);
			Detoxify(eBuffType����, ObjId, TRUE);
			AddHp(10, ObjId, 10000);
		}
		break;
	case 50://����ɳ
		success = TRUE;
		AddAll(&cItem::Detoxify, eBuffType��, ObjId,TRUE);//ȫ��ⶾ
		AddAll(&cItem::Detoxify, eBuffType��ӡ, ObjId,TRUE);//ȫ����
		break;
	case 51://��ζ¶
		success = TRUE;
		
		AddAll(&cItem::Resurgence, 10000, ObjId,FALSE);//ȫ�帴��
		AddAll(&cItem::Detoxify, eBuffType��, ObjId, TRUE);//ȫ��ⶾ
		AddAll(&cItem::AddMp, 2000, ObjId, FALSE);
		break;
	case 52://������
		
		{
			success = TRUE;
			Resurgence(1, ObjId);
			AddHpHurt(100, ObjId, 10000);
			AddHp(100, ObjId, 10000);
			AddMp(100, ObjId, 10000);
			AddCp(100, ObjId, 10000);
			Detoxify(eBuffType��, ObjId, TRUE);
			Detoxify(eBuffType��ӡ, ObjId, TRUE);
			Detoxify(eBuffType����, ObjId, TRUE);
			sBuff buf;
			buf.times = 10;
		
			g_pMainState->m_SkillManager.SetDateBuffUp(ObjId, ObjId, 2093,buf );
		}
		break;
	case 53://�����⸬
		success = AddHp(100, ObjId, 10000);
		break;
	case 54://��¶��
		success = AddCp(40, ObjId, FALSE);
		break;
	case 55://��ζ��
		success = AddCp(50, ObjId, FALSE);
		break;
	case 56://�ߵ���
		success = AddCp(60, ObjId, FALSE);
		break;
	case 57://��������
		success = AddCp(70, ObjId, FALSE);
		break;
	case 58://��¶��
		success = TRUE;
		AddHpHurt(100, ObjId, 10000);
		AddHp(100, ObjId, 10000);
		AddMp(100, ObjId, 10000);
		break;
	case 59://������Һ
		success = TRUE;
		AddAll(&cItem::AddHpHurt, 50, ObjId, 10000);
		AddAll(&cItem::AddHp, 50, ObjId, 10000);
		AddAll(&cItem::AddMp, 50, ObjId, 10000);
		break;
	case 60://ǧ��¶
		success = TRUE;
		AddAll(&cItem::Resurgence, 1000, ObjId, FALSE);//ȫ�帴��
		break;
	default:
		ERRBOX;
		break;
	}
	
	if (0 == success)return FALSE;
	pItem->m_Num -= 1;
	
	if (g_StateType==STATE_MAIN)
	if (ObjId%5==g_pMainState->m_HeroID)
	{
		g_pHeroObj->m_Wav.Free();
		g_pHeroObj->m_Wav.Load(0xAFE0E635);
	}
	return TRUE;
}
BOOL cItem::UseItem23(sItem2* pItem, int ObjId)
{
	if (g_StateType == STATE_FIGHT)return TRUE;
	BOOL success;
	m_bAddHp = FALSE;
	m_bAddMp = FALSE;
	m_bAddCp = FALSE;
	switch (pItem->GetID())
	{
	case 0:	//���� 
		success = AddHp(300, ObjId);
		break;
	case 1://��Ѽ   
		success = AddHp(1000, ObjId);
		break;
	case 2://��ի
		success = AddHp(3000, ObjId);
		break;
	case 3://������
		success = AddHp(5000, ObjId);
		break;
	case 4://Ů����
		success = AddHpHurt(200, ObjId);
		break;
	case 5://���Ǿ�
		success = AddHpHurt(500, ObjId);
		break;
	case 6://÷����
		success = AddHpHurt(1000, ObjId);
		break;
	case 7://����ǽ
		success = AddMp(500, ObjId);
		break;
	case 8://��䶹��
		success = AddMp(2000, ObjId);
		break;
	case 9://��֮��м
		success = AddHp(3500, ObjId);
		break;
	default:
		return FALSE;
		break;
	}
	if (0 == success)return FALSE;
	pItem->m_Num -= 1;
	
	if (g_StateType == STATE_MAIN)
	if (ObjId % 5 == g_pMainState->m_HeroID)
	{
		g_pHeroObj->m_Wav.Free();
		g_pHeroObj->m_Wav.Load(0xAFE0E635);
	}
	return TRUE;
}
BOOL cItem::UseItem24(sItem2* pItem, int ObjId)
{
	if (g_StateType == STATE_FIGHT)return TRUE;
	cObj* pTarget = g_pMainState->m_FightList[ObjId].m_pData;
	cPetData* pTargetData;
	pTarget->GetDateP(pTargetData);
	g_pMainState->m_InterfaceItem.m_Describe.OnOff(FALSE);
	switch (pItem->GetID())
	{
	case 4://����
		if (1)
		{
			g_pMainState->m_InterfaceDoor.SetHalf();
		}
		break;
	case 11:
		g_pMainState->m_InterfaceItem.InitMode(6);
		break;
	case 12://ȥCA
	case 13://ȥAL
	case 14://ȥCS
	case 15://ȥZZ

		g_pMainState->m_InterfaceItem.InitMode(pItem->GetID());
		break;
	case 16://����
		if (1)
		{
			if (2 == g_pMainState->m_InterfaceDoor.m_AvoidFight)
			{

				g_pMainState->m_InterfaceDoor.m_AvoidFight = 0;
				g_pMainState->m_Tags.Add("�����������״̬");
			}
			else
			{

				g_pMainState->m_InterfaceDoor.m_AvoidFight = 2;
				g_pMainState->m_Tags.Add("�����������״̬");
			}
			
		}
		break;
	default:
		break;
	}


	return TRUE;
}
BOOL cItem::UseItem26(sItem2* pCard, int ObjId)
{
	if (ObjId < 0)return FALSE;
	cObj* pObj = g_pMainState->m_FightList[ObjId].m_pData;
	cCharacter*  pc = (cCharacter*)pObj->m_pIndex;
	pc->UseCard(pCard->GetID());
	if (pObj->GetDirection() % 2 == 0)
		pObj->SetDirection(pObj->GetDirection() + 1);
	pObj->Reset();
	pObj->Set(POS_STAND);
	g_pMainState->m_PCHead.UpDateHead();
	pCard->m_Num -= 1;
	return TRUE;
}
BOOL cItem::UseItem27(sItem2* pItem, int ObjId,  int* num)
{
	if (g_StateType == STATE_FIGHT)return TRUE;
	switch (pItem->GetID())
	{
	case 0://���з�
		g_pMainState->m_InterfaceItem.InitMode(1);
		g_pMainState->m_InterfaceItem.pItemnum = num;
		break;
	case 1://������
		g_pMainState->m_InterfaceDoor.m_AvoidFight=TRUE;
		(*num) -= 1;
		return TRUE;
	case 2://��ڤ��
		g_pMainState->m_InterfaceDoor.m_AvoidFight = FALSE;
		(*num) -= 1;
		return TRUE;
	case 3://66
	case 4:
	case 5:
		
		{
			if (-1 == g_pCharacter->m_PCData.m_FightPetID)return TRUE;
			int limit;
			switch (pItem->GetID())
			{
			case 3:limit = 45;
			case 4:limit = 105;
			default:limit = 999;
			}
			cPet* pPet1 = (cPet*)g_pMainState->m_FightList[g_pMainState->m_HeroID + 5].m_pData->m_pIndex;
			if (pPet1->GetData()->m_LVLimit > limit)return TRUE;
			if (pPet1->m_PetDataZZ.m_AddPoint.idle)
			{
				g_pMainState->m_Tags.Add("δ�������Ե�");
				return TRUE;
			}
			pPet1->m_PetDataZZ.ResetAddPoint();
			(*num) -= 1;
		}
		return TRUE;
	case 36://��ź
	
		{
			if (!g_pMainState->m_FightList[g_pMainState->m_HeroID + 5].isLive)return TRUE;
			(*num) -= 1;
			cPetData* data;
			g_pMainState->m_FightList[g_pMainState->m_HeroID + 5].m_pData->GetDateP(data);
			data->EXP += data->ExpMax;
		}
		return TRUE;
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
		if (0==(g_pMainState->m_GlobalButton[200] & (1 << (pItem->GetID() - 42))))
		{
			g_pMainState->m_GlobalButton[200] |= (1 << (pItem->GetID() - 42));
			(*num) -= 1;
		}
		return TRUE;
	case 55://����BUG
		if (g_pMainState->m_Job.CheckHaveJob(40))
		{
			g_pMainState->m_TaskZhuaGui.UpdateZGJob(TRUE);
			*num -= 1;
			return TRUE;
		}
		else if (g_pMainState->m_Job.CheckHaveJob(50))
		{
			if (1==g_pMainState->m_TaskFB.m_List2.size())
			{
			ostringstream oss;
			oss << "�ҵ�,����:"  << g_pMainState->m_TaskFB.m_List2[0].m_Npc.m_name;
			oss << " ����X:";
			oss <<g_pMainState->m_TaskFB.m_List2[0].m_Npc.m_x<<" Y:";
			oss << g_pMainState->m_TaskFB.m_List2[0].m_Npc.m_y;
			g_pMainState->m_Channel.AddNewString(oss.str());
			}	
			if (g_pMainState->m_TaskFB.m_List.size()<3)
			{
				if (g_pMainState->m_TriggerObj.m_datalist.size())
				{
					ostringstream oss;
					oss << "�ҵ�,����:" << g_pMainState->m_TaskFB.m_List[0].m_Npc.m_name;
					oss << " ����X:";
					oss << g_pMainState->m_TaskFB.m_List[0].m_Npc.m_x << " Y:";
					oss << g_pMainState->m_TaskFB.m_List[0].m_Npc.m_y;
					g_pMainState->m_Channel.AddNewString(oss.str());
				}
			}
			*num -= 1;
			return TRUE;
		}
		else if (g_pMainState->m_TriggerObj.m_datalist.size()<3)
		{
			if (!g_pMainState->m_TriggerObj.m_datalist.size())return TRUE;
			ostringstream oss;
			oss << "�ҵ�,����:" << g_pMainState->m_TriggerObj.m_datalist[0].m_name;
			oss << " ���ڵ�ͼ:" << g_pMainState->m_TriggerObj.m_datalist[0].m_mapname;
			g_pMainState->m_Channel.AddNewString(oss.str());
			*num -= 1;
			return TRUE;
		}
		break;
	case 59:
		(*num) -= 1;
		g_pMainState->m_InterfaceJianDing.SetMode(0);
		g_pMainState->m_InterfaceJianDing.OnOff(TRUE);
		return TRUE;
	case 60:
	case 61:
	case 62:
	case 63:
		if (1)
		{
			(*num) -= 1;
			eXiuLian xl;
			switch (pItem->GetID())
			{
			case 60:xl = eXLATK; break;
			case 61:xl = eXLDEF; break;
			case 62:xl = eXLMAG; break;
			default:xl = eXLDMAG; break;
			}
			g_pCharacter->m_PCData.m_BBXiuLian.AddExp(g_pCharacter->m_PCData.m_LV, 150, xl);
			if (g_pMainState->m_FightList[g_pMainState->m_HeroID + 5].isLive)
			{
				cPetData* pPet;
				g_pMainState->m_FightList[g_pMainState->m_HeroID + 5].m_pData->GetDateP(pPet);	
				pPet->m_XiuLian= g_pCharacter->m_PCData.m_BBXiuLian;
			}
		}
		return TRUE;
	case 64:
	case 65:
	case 66:
	case 67:
		if (1)
		{
			(*num) -= 1;
			eXiuLian xl;
			switch (pItem->GetID())
			{
			case 64:xl = eXLATK; break;
			case 65:xl = eXLDEF; break;
			case 66:xl = eXLMAG; break;
			default:xl = eXLDMAG; break;
			}
			g_pCharacter->m_PCData.m_XiuLian.AddExp(g_pCharacter->m_PCData.m_LV, 100, xl);
		}
		return TRUE;
	case 68:
	case 69:
	case 70:
	case 71:
	case 72:
	case 73:
	case 74:
		if (1)
		{
			if (!g_pMainState->m_FightList[g_pMainState->m_HeroID + 5].isLive)return TRUE;
			cPetData* pPet;
			g_pMainState->m_FightList[g_pMainState->m_HeroID + 5].m_pData->GetDateP(pPet);
			int add;
			switch (pItem->GetID())
			{
			case 68:
				if (2000>pPet->m_ZiZhi.m_ATKZZ)
				{
					add = (2000 - pPet->m_ZiZhi.m_ATKZZ)/30;
					if (add > 40)add = 40;
					if (add < 5)add = 5;
					pPet->AddAtkZZ(add);
					(*num) -= 1;
				}
				break;
			case 69:
				if (2000>pPet->m_ZiZhi.m_DEFZZ)
				{
					add = (2000 - pPet->m_ZiZhi.m_DEFZZ) / 30;
					if (add > 40)add = 40;
					if (add < 5)add = 5;
					pPet->AddDefZZ(add);
					(*num) -= 1;
				}
				break;
			case 70:
				if (8000>pPet->m_ZiZhi.m_HPZZ)
				{
					add = (8000 - pPet->m_ZiZhi.m_HPZZ) / 120;
					if (add > 160)add = 160;
					if (add < 20)add = 20;
					pPet->AddHpZZ(add);
					(*num) -= 1;
				}
				break;
			case 71:
				if (4000>pPet->m_ZiZhi.m_MAGZZ)
				{
					add = (4000 - pPet->m_ZiZhi.m_MAGZZ) / 60;
					if (add > 80)add = 80;
					if (add < 10)add = 10;
					pPet->AddMagZZ(add);
					(*num) -= 1;
				}
				break;
			case 72:
				if (2000>pPet->m_ZiZhi.m_SPDZZ)
				{
					add = (2000 - pPet->m_ZiZhi.m_SPDZZ) / 30;
					if (add > 40)add = 40;
					if (add < 5)add = 5;
					pPet->AddSpdZZ(add);
					(*num) -= 1;
				}
				break;
			case 73:
				if (2000>pPet->m_ZiZhi.m_AVOIDZZ)
				{
					add = (2000 - pPet->m_ZiZhi.m_AVOIDZZ) / 30;
					if (add > 40)add = 40;
					if (add < 5)add = 5;
					pPet->AddAvoidZZ(add);
					(*num) -= 1;
				}
				break;
			case 74:
				if (150>pPet->m_ZiZhi.m_Growing)
				{
					add = (150 - pPet->m_ZiZhi.m_Growing) / 10;
					if (add > 3)add = 3;
					if (add < 1)add = 1;
					pPet->AddGrowing(add);
					(*num) -= 1;
				}
				break;
			}
			
		}
		return TRUE;
	}
	return FALSE;
}
BOOL cItem::FillProperty(int CharID, ItemProPerty type)
{
	int * dada1 = 0;
	int * dada2 = 0;

	cObj* pUser = g_pMainState->m_FightList[CharID].m_pData;
	cPetData* pPetData;
	pUser->GetDateP(pPetData);
	switch (type)
	{
	case enum_hp:
		dada1 = &pPetData->m_HP;   //hp
		dada2 = &pPetData->m_HPHurtMax; //��
		break;
	case enum_hphurt:
		dada1 = &pPetData->m_HPHurtMax;         //hp
		dada2 = pPetData->m_HPMax.GetDatap(); 
		break;
	case enum_mp:
		dada1 = &pPetData->m_MP;
		dada2 = pPetData->m_MPMax.GetDatap();
		break;
	default:
		ERRBOX;
		break;
	}
	
	cCharacter* pCharOwner;
	int charid2=CharID%5;

	pCharOwner = (cCharacter*)g_pMainState->m_FightList[CharID%5].m_pData->m_pIndex;
	int dustbin = 0;
	int* numItem=&dustbin;
	sItem2* Item = 0;
	while (*dada1 < *dada2)
	{
		if (!(*numItem))
		{
			int id3 = GetCheapestItem(charid2, type);
			if (id3==-1)return TRUE;
			Item =&pCharOwner->m_PCData.m_Item[id3];
			numItem = &Item->m_Num;
			
		}
		if (Item)
		{		
			UseItem(Item, CharID, 0);
		}
	}

	return TRUE;
}

int cItem::GetCheapestItem(int CharID, ItemProPerty Type)
{
	cCharacter* pChar = (cCharacter*)g_pMainState->m_FightList[CharID].m_pData->m_pIndex;
	sItem* pItem;
	int cost=100000;
	int retId=-1;
	switch (Type)
	{
	case enum_hp:
		if (1)
		{
			for (int i = 0; i < 20; i++)
			{
				if (0==pChar->m_PCData.m_Item[i].GetNum())continue;
				if (pChar->m_PCData.m_Item[i].GetType() == 23)
				{
					pItem = pChar->m_PCData.m_Item[i].m_pItemData;
					switch (pItem->m_ID)
					{
					case 0:
						return i;
					case 1:
						if (cost>10)
						{
							cost = 1;
							retId = i;
						}
						break;
					case 2:
						if (cost > 20)
						{
							cost = 2;
							retId = i;
						}
						break;
					case 3:
						if (cost > 30)
						{
							cost = 1;
							retId = 3;
						}
						break;
					}
				}
				else
				if (pChar->m_PCData.m_Item[i].GetType() == 22)
				{
					pItem = pChar->m_PCData.m_Item[i].m_pItemData;
					switch (pItem->m_ID)
					{
					case 1://��Ҷ��
						if (cost > 1)
						{
							cost = 1;
							retId = i;
						}
						break;
					case 2:
						if (cost > 2)
						{
							cost = 2;
							retId = i;
						}
						break;
					case 3:
						if (cost > 3)
						{
							cost = 3;
							retId = i;
						}
						break;
					case 4:
						if (cost > 11)
						{
							cost = 11;
							retId = i;
						}
						break;
					case 5:
						if (cost > 12)
						{
							cost = 12;
							retId = i;
						}
						break;
					case 6:
						if (cost > 11)
						{
							cost = 11;
							retId = i;
						}
						break;
					case 7:
						if (cost > 11)
						{
							cost = 11;
							retId = i;
						}
						break;
					case 8:
						if (cost > 21)
						{
							cost = 21;
							retId = i;
						}
						break;
					case 9:
						if (cost > 11)
						{
							cost = 11;
							retId = i;
						}
						break;
					case 10:
						if (cost > 11)
						{
							cost = 11;
							retId = i;
						}
						break;
					case 11:
						if (cost > 11)
						{
							cost = 11;
							retId = i;
						}
						break;
					case 12:
						if (cost > 11)
						{
							cost = 11;
							retId = i;
						}
						break;
					case 13:
						if (cost > 11)
						{
							cost = 11;
							retId = i;
						}
						break;
					case 15:
						if (cost > 11)
						{
							cost = 11;
							retId = i;
						}
						break;
					case 40://�¼���
						if (cost > 15)
						{
							cost = 15;
							retId = i;
						}
						break;
					case 41:
						if (cost > 16)
						{
							cost = 16;
							retId = i;
						}
						break;
					case 42:
						if (cost > 17)
						{
							cost = 17;
							retId = i;
						}
						break;
					case 47: //�����ֻ� 30%
						if (cost > 30)
						{
							cost = 30;
							retId = i;
						}
						break;
					}
				}
			}
		}
		break;
	case enum_hphurt:
		if (1)
		{
			for (int i = 0; i < 20; i++)
			{
				if (0 == pChar->m_PCData.m_Item[i].GetNum())continue;
				if (pChar->m_PCData.m_Item[i].GetType() == 23)
				{
					pItem = pChar->m_PCData.m_Item[i].m_pItemData;
					switch (pItem->m_ID)
					{
					case 4:   //Ů����
						return i;
						break;
					case 5:   //���Ǿ�
						if (cost > 2)
						{
							cost = 2;
							retId = i;
						}
						break;
					case 6:   //÷����
						if (cost > 16)
						{
							cost = 16;
							retId = i;
						}
						break;
					default:
						break;
					}
				}
				else
				if (pChar->m_PCData.m_Item[i].GetType() == 23)
				{
					pItem = pChar->m_PCData.m_Item[i].m_pItemData;
					switch (pItem->m_ID)
					{
					case 34:   //�ݹ�
						if (cost > 1)
						{
							cost = 1;
							retId = i;
						}
						break;
					case 35:   //�����
						if (cost > 2)
						{
							cost = 2;
							retId = i;
						}
						break;
					case 36:   //�ݹ�
						if (cost > 3)
						{
							cost = 3;
							retId = i;
						}
						break;
					case 37:   //ɽҩ
						if (cost > 41)
						{
							cost = 11;
							retId = i;
						}
						break;
					case 38:   //��֬ ȫ��
						if (cost > 5)
						{
							cost = 5;
							retId = i;
						}
						break;
					case 39:   //��ʯӢ
						if (cost > 31)
						{
							cost = 31;
							retId = i;
						}
						break;
					case 40:   //�¼���
						if (cost > 4)
						{
							cost = 4;
							retId = i;
						}
						break;
					case 41:   //÷����
						if (cost > 11)
						{
							cost = 11;
							retId = i;
						}
						break;
					}
				}
			}
		}
		break;
	case enum_mp:
		for (int i = 0; i < 20; i++)
		{
			if (0 == pChar->m_PCData.m_Item[i].GetNum())continue;
			if (pChar->m_PCData.m_Item[i].GetType() == 23)
			{
				pItem = pChar->m_PCData.m_Item[i].m_pItemData;
				switch (pItem->m_ID)
				{		
				case 7:   //����ǽ
					if (cost > 2)
					{
						cost = 2;
						retId = i;
					}
					break;
				case 8:   //��䶹��
					if (cost > 10)
					{
						cost = 10;
						retId = i;
					}
					break;		
				default:
					break;
				}
			}
			else
			if (pChar->m_PCData.m_Item[i].GetType() == 22)
			{
				pItem = pChar->m_PCData.m_Item[i].m_pItemData;
				switch (pItem->m_ID)
				{
				case 19:   //����
					return i;
					break;
				case 20:   //������
					if (cost > 1)
					{
						cost = 1;
						retId = i;
					}
					break;
				case 21:   //��Ҷ
					if (cost > 2)
					{
						cost = 2;
						retId = i;
					}
					break;
				case 22:   //��٢�޻�
					if (cost > 3)
					{
						cost = 3;
						retId = i;
					}
					break;
				case 23:   //����ˮ
					if (cost > 11)
					{
						cost = 11;
						retId = i;
					}
					break;
				case 24:   //�ͷ���¶
					if (cost > 12)
					{
						cost = 12;
						retId = i;
					}
					break;
				case 26:   //�ϵ��� ȫ��
					if (cost > 5)
					{
						cost = 30;
						retId = i;
					}
					break;
				case 27:   //����
					if (cost > 6)
					{
						cost = 6;
						retId = i;
					}
					break;
				case 28:   //�����ͷ
					if (cost > 16)
					{
						cost = 16;
						retId = i;
					}
					break;
				case 29:   //����ˮ
					if (cost > 17)
					{
						cost = 17;
						retId = i;
					}
					break;
				case 30:   //������
					if (cost > 26)
					{
						cost = 26;
						retId = i;
					}
					break;
				case 31:   //����
					if (cost > 27)
					{
						cost = 27;
						retId = i;
					}
					break;
				case 32:   //�ɺ���
					if (cost > 28)
					{
						cost = 28;
						retId = i;
					}
					break;
				default:
					break;
				}
			}
		}
		break;
	default:
		MessageBoxA(0,"δ����Ѱ������","GetCheapestItem",MB_OK);
		break;
	}
	return retId;
}
//��ȡ����װ��
BOOL cItem::LoadZuoqiZhuangShi(string path)
{
	ifstream File;
	File.open(path);
	if (!File)return FALSE;
	int num;
	string dustbin;
	File >> num;
	sItem item;
	item.m_Type = 31;
	if (num != 24)return FALSE;
	for (int i = 0; i < num; i++)
	{
		item.m_Price = 1000000;
		File >> item.m_ID;
		File >> item.m_Name;
		File >> item.m_SmallHead;
		File >> item.m_BigHead;
		m_ItemList[31].push_back(item);
		File >> m_ZuoQiZhuangShiList[i].NodeStand;
		File >> m_ZuoQiZhuangShiList[i].NodeWalk;
		File >> m_ZuoQiZhuangShiList[i].m_ColorID;
		for (int j = 0; j < 15; j++)
		{
			File >> dustbin;
			File >> m_ZuoQiZhuangShiList[i].Stand[j];
			File >> m_ZuoQiZhuangShiList[i].Walk[j];
		}
	}
	File.close();
	return TRUE;
}


BOOL cItem::AddHp(int Num, int TargetID, BOOL Percent)
{
	cPetData* pPetData;
	g_pMainState->m_FightList[TargetID].m_pData->GetDateP(pPetData);
	return AddHp(Num,pPetData,Percent);
}
BOOL cItem::AddHp(int Num, cPetData* pdata, BOOL Percent)
{
	m_bAddHp = TRUE;
	if (g_StateType==STATE_FIGHT)
	if (0 == pdata->m_HP)return FALSE;//����ʱ���ܼ�Ѫ
	int& Hp = pdata->m_HP;
	int HpHurtMax = pdata->m_HPHurtMax;
	int HpMax = pdata->m_HPMax.GetData();
	if (Hp == HpMax)return FALSE;//��ֹ��Ѫ
	int add = Num;
	if (Percent)
	{
		add = HpMax*add/100;
		if (add > Num*Percent)
			add = Num*Percent;
	}
	m_hp = add;
	Hp += add;
	//��Ѫ���ܳ��������Ѫ
	if (Hp<0)Hp = 0;
	else if (Hp > HpHurtMax)Hp = HpHurtMax;
	return TRUE;
}
BOOL cItem::AddHpHurt(int Num, int TargetID, BOOL Percent)
{
	cPetData* pPetData;
	g_pMainState->m_FightList[TargetID].m_pData->GetDateP(pPetData);
	return AddHpHurt(Num, pPetData, Percent);
}
BOOL cItem::AddHpHurt(int Num, cPetData* pPetData, BOOL Percent)
{
	if (0 == pPetData->m_HP)return FALSE;//����ʱ���ܼ�Ѫ
	int& HpHurtMax = pPetData->m_HPHurtMax;
	int HpMax = pPetData->m_HPMax.GetData();
	if (HpHurtMax == HpMax)return FALSE;
	int add = Num;
	if (Percent)
	{
		add = HpMax*add/100;
		if (add > Num*Percent)
			add = Num*Percent;
	}
	HpHurtMax += add;
	if (HpHurtMax<0)HpHurtMax = 0;
	else if (HpHurtMax > HpMax)HpHurtMax = HpMax;
	return TRUE;
}
BOOL cItem::AddMp(int Num, int TargetID, BOOL Percent)
{
	m_bAddMp = TRUE;
	cObj* pObj = g_pMainState->m_FightList[TargetID].m_pData;
	cPetData* pPetData;
	pObj->GetDateP(pPetData);
	if (g_StateType!=STATE_MAIN)
	if (0 == pPetData->m_HP)return FALSE;//����ʱ���ܼ�Ѫ
	int& Mp = pPetData->m_MP;
	int MpMax =pPetData->m_MPMax.GetData();
	if (Mp == MpMax)return FALSE;
	int add= Num;
	if (Percent)
	{
		add = MpMax*add/100;
		if (add > Num*Percent)
			add = Num*Percent;
	}
	Mp += add;
	if (Mp<0)Mp = 0;
	else if (Mp > MpMax)Mp = MpMax;
	return TRUE;
}
BOOL cItem::AddCp(int Num,int TargetID, BOOL Percent)
{
	cObj* pObj = g_pMainState->m_FightList[TargetID].m_pData;
	cPetData* pPetData;
	pObj->GetDateP(pPetData);
	if (0 == pPetData->m_HP)return FALSE;//����ʱ���ܼ�Ѫ
	int& Cp = pPetData->m_CP;
	int add = Num;
	if (Percent)add =150;
	Cp += add;
	if (Cp<0)Cp = 0;
	else if (Cp > 150)Cp = 150;
	return  TRUE;
}



BOOL cItem::AddAll(BOOL(cItem::*AddSomething)(int, int, BOOL), int Num, int TargetID, BOOL Percent)
{
	m_bToAll = TRUE;
	BOOL sucess = FALSE;
	if (TargetID < 10)
	{
		for (int i = 0; i < 10; i++)
		{
			if (g_pMainState->m_FightList[i].isLive)
			if ((this->*AddSomething)(Num, i, Percent)){ sucess = TRUE; };
		}
	}
	else
	{
		for (int i = 10; i < 20; i++)
		{
			if (g_pMainState->m_FightList[i].isLive)
			if ((this->*AddSomething)(Num, i, Percent)){ sucess = TRUE; };
		}
	}
	return sucess;
}




BOOL cItem::UseItem(sItem2* pItem, int ObjId,  int* num)
{
	if (pItem->GetNum() < 1)
		return FALSE;
	switch (pItem->GetType())
	{
	case 22:
		if(!UseItem22(pItem, ObjId))return FALSE;
		break;
	case 23:
		if(!UseItem23(pItem, ObjId))return FALSE;
		break;
	case 24:
		return UseItem24(pItem, ObjId);
	case 25:
		return FALSE;
	case 27:
		return UseItem27(pItem, ObjId, num);
	default:
		return FALSE;
		break;
	}
	cObj* pObj;
	if (ObjId == g_pMainState->m_HeroID)
	{
		if (m_bAddHp)
		{
			g_pMainState->m_PCHead.UpdatePcHp();
			g_pMainState->m_State.UpDateHp();
		}
		if (m_bAddMp)
		{
			g_pMainState->m_PCHead.UpdatePcMp();
			g_pMainState->m_State.UpDateMp();
		}
		if (m_bAddCp)
		{
			g_pMainState->m_PCHead.UpdatePcCp();
			g_pMainState->m_State.UpDateCp();
		}
	}
	else
	if (ObjId == g_pMainState->m_HeroID + 5)
	{
		if (m_bAddHp)
		{
			g_pMainState->m_PCHead.UpdatePetHp();
		}
		if (m_bAddMp)
		{
			g_pMainState->m_PCHead.UpdatePetMp();
		}
	}
	if (g_StateType == STATE_FIGHT)
	if (m_bToAll)
	{
		int start, end;
		if (ObjId < 10)
		{
			start = 0;
		}
		else start = 10;
		end = start + 10;
		for (int i = start; i < end; i++)
		{
			if (!g_pMainState->m_FightList[i].isLive)continue;
			pObj = g_pMainState->m_FightList[i].m_pData;
			if (m_bAddHp&&m_bAddMp)
			{
				pObj->SetMagicEffect(&m_MagicAddHPMP, -10);
				g_pMainState->m_SkillManager.SetDamage(-g_pMainState->m_ItemManage.m_hp, i);
			}
			else
			if (m_bAddHp)
			{
				pObj->SetMagicEffect(&m_MagicAddHP, -10);
				g_pMainState->m_SkillManager.SetDamage(-g_pMainState->m_ItemManage.m_hp, i);
			}
			else
			if (m_bAddMp)
			{
				pObj->SetMagicEffect(&m_MagicAddMP, -10);
			}
			else return TRUE;
		}
		return TRUE;
	}
	if (g_StateType == STATE_MAIN)
		ObjId = ObjId % 5;
	pObj = g_pMainState->m_FightList[ObjId].m_pData;
	if (m_bAddHp&&m_bAddMp)
	{
		pObj->SetMagicEffect(&m_MagicAddHPMP, -10);
		if (g_StateType == STATE_FIGHT)
			g_pMainState->m_SkillManager.SetDamage(-g_pMainState->m_ItemManage.m_hp, ObjId);
	}
	else
	if (m_bAddHp)
	{
		pObj->SetMagicEffect(&m_MagicAddHP, -10);
		if (g_StateType == STATE_FIGHT)
			g_pMainState->m_SkillManager.SetDamage(-g_pMainState->m_ItemManage.m_hp, ObjId);
	}
	else
	if (m_bAddMp)
	{
		pObj->SetMagicEffect(&m_MagicAddMP, -10);
	}


	return TRUE;
}

BOOL cItem::Resurgence(int Num, int TargetID, BOOL Percent)
{
	cObj* pObj = g_pMainState->m_FightList[TargetID].m_pData;
	pObj->SetStatic(FALSE);
	pObj->Set(POS_FIGHT_STAND);
	g_pMainState->m_FightList[TargetID].SetDie( FALSE);
	cPetData* pPet;
	pObj->GetDateP(pPet);
	pPet->m_HP = 1;
	AddHpHurt(Num, TargetID, Percent);
	AddHp(Num, TargetID, Percent);
	pObj->MoveShadow();
	return TRUE;
}

BOOL cItem::Detoxify(int Type, int TargetID, BOOL ReMoveAll)
{ 
	BOOL success=FALSE;
	if (ReMoveAll)
	{
		success= Detoxify1(Type, TargetID);
	}
	else
	{
		while (Detoxify1(Type, TargetID))
		{
			success = TRUE;
		}
		return TRUE;
	}
	return success;
}
BOOL cItem::Detoxify1(int Type, int TargetID)
{
	cObj* p1 = g_pMainState->m_FightList[TargetID].m_pData;
	sBuff buf;
	if (p1->m_StateList.size())
	for (list<sBuff>::iterator Iterator = p1->m_StateList.begin(); Iterator != p1->m_StateList.end();)
	{
		if (Type == (int)Iterator->EffectType)
		{
			list<sBuff>::iterator it2 = Iterator;
			Iterator++;
			g_pMainState->m_SkillManager.SetDateBuffDown(it2);//it2->targetID, it2->ID);
			it2->data.Free();
			p1->m_StateList.erase(it2);
			return TRUE;
		}
		else
			Iterator++;
	}
	return FALSE;
}

BOOL cItem::LoadItem1(string path)
{
	ifstream File;
	File.open(path);
	int type;
	File >> type;
	
	int num;
	File >> num;
	vector<sItem>& item = m_ItemList[type];
	item.resize(num);

	string tex;
	for (int i = 0; i < num; i++)
	{
		item[i].m_Type = type;
		File >> item[i].m_ID;
		File >> item[i].m_Name;
		File >> item[i].m_SmallHead;
		File >> item[i].m_BigHead;
		File >> item[i].m_Price;
		getline(File, item[i].m_Text);
		getline(File, item[i].m_Text);
	}
	File.close();
	return TRUE;
}

BOOL cItem::LoadFaBao(string path)
{
	ifstream File;
	File.open(path);
	int type;
	File >> type;
	int num;
	File >> num;
	sItem item;
	for (int i = 0; i < num; i++)
	{
		item.m_Type = type;
		File >> item.m_ID;
		File >> item.m_Name;
		//File >> item.m_lv;
		File >> item.m_SmallHead;
		File >> item.m_BigHead;
		File >> item.m_UseType;
		File >> item.m_Price;
		getline(File, item.m_Text);
		getline(File, item.m_Text);
		m_ItemList[type].push_back(item);
	}
	File.close();
	return TRUE;
}
BOOL cItem::LoadShouJue(string path)
{
	ifstream File;
	File.open(path);
	string name = "ħ��Ҫ��";
	int type;
	File >> type;
	int num;
	File >> num;
	DWORD head[2];
	string text;
	File >> head[0];
	File >> head[1];
	File >> text;
	text += "(��������:";
	sItem item;
	for (int i = 0; i < num; i++)
	{
		item.m_Type = type;
		File >> item.m_ID;
		File >> item.m_Name;
		//File >> item.m_lv;
		item.m_SmallHead=head[0];
		item.m_BigHead=head[1];
		File >> item.m_Price;
		item.m_Text = text;
		item.m_Text += item.m_Name;
		item.m_Text += ")";
		item.m_Name = name;
		m_ItemList[type].push_back(item);
	}
	File.close();
	return TRUE;
}

void cItem::SetFabaoEffect(cPCData& data, int leveup, eFaBao ID)
{
	int add=leveup;
	switch (ID)
	{
	case eFaBao0������:
		data.m_DamageAddPercentPoision.d1 += 20 * add;
		break;
	case eFaBao1�ɽ�Īа:
		data.m_AtkDownHPHurt.d1+=1*add;
		break;
	case eFaBao2�޺���:
		data.mpsavepercent += 20*add;
		break;
	case eFaBao3����:
		data.m_LiveMpLv += 5*add;
		break;
	case eFaBao4����:
		break;
	case eFaBao5������:
		data.ghostSpecial.d1 += 20 * add;
		break;
	case eFaBao6��Ԫɡ:
		data.m_StabRateMagic.d1 += 10*add;    //���������˺�
		data.m_StabDamageRateMagic.d1 += 5*add; //����������
		break;
	case eFaBao7�޻����:
		data.m_DEF.AddDataAddPercent(5*add);
		break;
	case eFaBao8Ǭ��������:
		data.m_LiveCp += 5*add;
		break;
	case eFaBao9�ն�:
		data.m_MagicNumAdd += add;
		break;
	case eFaBao10���س�:
		g_pMainState->m_GlobalButton[13] += 50*add;
		break;
	case eFaBao11�¹ⱦ��:
		break;
	case eFaBao12��ɫ������:
		break;
	case eFaBao13��ɫ������:
		break;
	case eFaBao14��ɫ������:
		break;
	case eFaBao15��ɫ������:
		break;
	case eFaBao16����:
		break;
	case eFaBao17�ܽ����ְ�:
		data.m_Popularity += 100 * add;
		break;
	case eFaBao18ʱ֮��:
		data.m_SPD.AddDataAdd(1000 * add);
		data.GoldLivePercent.d1 += 100*add;
		break;
	case eFaBao19ͨ�鱦��:
		g_pMainState->m_GlobalButton[14] += 20 * add;
		break;
	case eFaBao20�۱���:
		g_pMainState->m_GlobalButton[15] += 20 * add;
		break;
	case eFaBao21�����«:
		data.m_DamageAddPercentCure.d1 += 20 * add;
		break;
	case eFaBao22��ľ����:
		data.m_DamageAddPercentCure.d2 += 20 * add;
		break;
	case eFaBao23���뾵:
		data.m_StabRate.d1 += 10 * add;
		data.m_StabDamageRate.d1 += 30 * add;
		break;
	case eFaBao24��Ѫ�:
		data.comboNumUp += add;
		data.m_Combo.d1 += 5 * add;
		break;
	case eFaBao25�������:
		data.m_JinJiaRate.d1 += 10 * add;
		break;
	case eFaBao26��ħ����:
		data.m_XiangMoRate.d1 += 10 * add;
		break;
	case eFaBao27����:
		data.m_DamageAddPercentCure.d1 = add;
		break;
	case eFaBao28������ë:
		data.GoldLivePercent.d1 += 5 * add;
		break;
	case eFaBao29����:
		data.m_PierceMagic.d1 += 5 * add;
		break;
	case eFaBao30�������ӡ:
		data.m_MagicCriRate.d1 += 5 * add;
		break;
	case eFaBao31����:
		data.m_AtkDownMP.d1 += 1 * add;
		break;
	case eFaBao32�ȱ�:
		data.m_CureCriRate.d1 += 10 * add;
		break;
	case eFaBao33�ε�:
		data.m_Spurting.d2 += 15*add;
		data.nightLv += 10 * add;
		break;
	default:
		ERRBOX;
		break;
	}
}


int cItem::GetIDByLv(int type, int lv)
{
	if (lv > 160)lv = 160;
	lv /= 10;
	if (type < 15) //����
	{
		if (lv < 9)return lv;
		if (lv < 12)return 9 + (rand() % 3) *3;
		if (lv < 15)return 18 + (rand() % 3)*3;
		if (lv == 15)return 27;
		return 28;
	}
	if (type ==19||type==20)//װ��
	{
		if (lv == 3)return 3+rand()%2;
		if (lv == 4)return 5+rand()%2;
		if (lv == 5)return 7+rand()%2;
		if (lv == 6)return 9+rand()%2;
		if (lv == 7)return 11+rand()%2;
		if (lv == 8)return 13+rand()%2;
		if(lv>8) return lv + 6;
	}
	return lv;
 }
int cItem::GetlvByID(int type, int id)
{
	if (type < 15) //����
	{
		if (id < 9)return id*10;
		if (id < 18)return (9 + id % 3)*10 ;
		if (id < 27)return (12 + id % 3) * 10;
		if (id == 27)return 150;
		else return 160;
	}
	if (type == 19 || type == 20)//װ��
	{
		if (id < 3)return id * 10;
		else if (id < 15)return ((id - 3) / 2 + 3) * 10;
		else return (id - 6) * 10;
	}
	return id*10;
}
BOOL cItem::UseItem28(sItem2* pItem2, int ObjId)
{
	sItem* pItem = pItem2->m_pItemData;
	switch (pItem->m_ID)
	{
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
		if (1)
		{
			m_pNowItem = pItem2;
			if (!pItem2->m_Property.size())
			{
				AutoItemProperty(pItem2);
			}
			if (1==pItem2->m_Property.size())
			{
				pItem2->m_Property.push_back(g_pMainState->m_InterfaceDoor.m_HeroX);
				pItem2->m_Property.push_back(g_pMainState->m_InterfaceDoor.m_HeroY);
				pItem2->m_strProperty = g_pMainState->m_Map.m_MapName;
				g_pMainState->m_Tags.Add("�ڵ�ǰλ�ö�������");
			}
			else
			{
				ostringstream oss;
				oss << "Ҫȥ" << pItem2->m_strProperty << "X:"<<pItem2->m_Property[0] <<" Y:"<< pItem2->m_Property[1] << " ��?";
				oss << "��ʣ" << pItem2->m_Property[0] << "��";
				g_pMainState->m_TriggerDialog.AddSelect(&g_pMainState->m_TaskNormal, oss.str(), g_pCharacter->m_PCData.m_Name, -1, "����", 14, "����", 15);
			}
		}
		return TRUE;
	case 6://��ͼ
		if (1)
		{
			cObj* pUser = g_pHeroObj;
			cCharacter* pc = (cCharacter*)pUser->m_pIndex;
			ostringstream oss;
			if (0 == g_pMainState->m_Map.m_MapName.compare(pItem2->m_strProperty))
			if (abs(pUser->GetX() - pItem2->m_Property[3])<100)
			if (abs(pUser->GetY() - pItem2->m_Property[4])<100)
			{
				int rat = rand() % 100;		
				int itemrat[10];
				itemrat[0] = 15;//��Ǯ��
				itemrat[1] = 10;//������
				itemrat[2] = 10;//������
				itemrat[3] = 10;//��ʯ�� 
				itemrat[4] = 5;//�޾���
				itemrat[5] = 10;//�û���
				itemrat[6] = 3;//�屦��
				itemrat[7] = 10;//66
				itemrat[8] = 10;//������
				itemrat[9] = 10;//��ʯ

				pItem2->m_Num -= 1;
				//������� 
				if (rat < itemrat[0])//10%���ʵ�Ǯ
				{
					int money = pc->m_PCData.m_LV * 100 + 5000;
					oss.str("");
					oss << "��õ���";
					oss << money;
					oss << "Ǯ";
					pc->AddMoney(money,0);
					g_pMainState->m_Tags.Add(oss.str());
					g_pMainState->m_InterfaceItem.SetXianJin(g_pMainState->m_InterfaceItem.m_bXianJin);
					return TRUE;
				}
				rat -= itemrat[0];
				if (rat < itemrat[1])// 10% ����
				{
					/*	g_pMainState->m_Tags.Add("�����������ֵļ�");
						oss.str("");
						oss << pUser->m_TrueName.GetString();
						oss << "�ڱ�ʱ��С�����������ֵķ���,һȺ�������ڸ�������#15";
						g_pMainState->m_Channel.AddNewString(oss.str());*/
					g_pMainState->m_Channel.AddNewString("���˰���ʲô��û�ڵ�..."); 
					return TRUE;
				}
				rat-= itemrat[1];
				if (rat < itemrat[2])// 10% ������
				{
					g_pMainState->m_Tags.Add("���ڳ���һ������,�����������ܳ�������");

					g_pMainState->m_TaskBaoTu.Process(6);
					return TRUE;
				}
				rat -= itemrat[2];

				sItem2 item;
				item.m_Num = 1;
				if (rat < itemrat[3])//��ʯ
				{	
						item.Set(41, rand()%78);
						g_pMainState->m_Tags.Add("��õ��˷�ʯ");
					g_pMainState->m_InterfaceItem.GetItem(g_pMainState->m_HeroID, &item);
					return TRUE;
				}
				rat -= itemrat[3];
				if (rat < itemrat[4]) //3%���ʵ��޾�
				{
					item.Set(40, g_pMainState->m_ItemManage.m_LShouJueList[rand() % g_pMainState->m_ItemManage.m_LShouJueList.size()]);
					g_pMainState->m_InterfaceItem.GetItem(g_pMainState->m_HeroID, &item);
					g_pMainState->m_Tags.Add("��õ������������鼮");
					return TRUE;
				}
				rat -= itemrat[4];
				if (rat < itemrat[5])//7%���ʵû�
				{
					int type = rand() % 22;
					if (type == 13)type = rand() % 13;
					int lv = (5 + rand() % 3 )*10;  //50-70װ��
					int id = GetIDByLv(type, lv);
					item.Set(type, id);
					g_pMainState->m_InterfaceItem.GetItem(g_pMainState->m_HeroID, &item);
					g_pMainState->m_Tags.Add("��õ������������ı���");
					return TRUE;
				}
				rat -= itemrat[5];
				if (rat < itemrat[6])//5%���ʳ��屦
				{
					g_pMainState->m_Tags.Add("�����õ��˵�ʲô");
					item.Set(27, 6 + rand() % 5);
					g_pMainState->m_InterfaceItem.GetItem(g_pMainState->m_HeroID, &item);
					return TRUE;
				}
				rat -= itemrat[6];
				if (rat < itemrat[7])//��66
				{
					g_pMainState->m_Tags.Add("��õ��˴�˵�еĽ���¶");
					item.Set(27, 3+rand()%3);
					g_pMainState->m_InterfaceItem.GetItem(g_pMainState->m_HeroID, &item);
					return TRUE;
				}
				rat -= itemrat[7];
				if (rat < itemrat[8])//��
				{
					g_pMainState->m_Tags.Add("���ڳ�������,���ڱ���״̬");
					pc->m_PCData.m_HP = 1;
					pc->m_PCData.m_MP = 1;
					g_pMainState->m_PCHead.UpdatePcHp();
					g_pMainState->m_PCHead.UpdatePcMp();
					return TRUE;
				}
				rat -= itemrat[8];
				if (rat < itemrat[9])//��ʯ
				{
					item.Set(29, rand()%17);
					g_pMainState->m_InterfaceItem.GetItem(g_pMainState->m_HeroID, &item);
					g_pMainState->m_Tags.Add("���ڵ��˱�ʯ");
					return TRUE;
				}
				else  //ʲô��û�ڵ�
				{
					g_pMainState->m_Tags.Add("���˰���ʲô��û�ڵ�");		
		
					return TRUE;
				}
			}
		}
		break;
		case 19:
			if (1)
			{
				ERRBOX;
			}
			break;
		case 20:
			ERRBOX;
			break;
		case 21://�����ڱ�
			if (1)
			{
				g_pMainState->m_TaskFB.ProcessSM(8);
				return FALSE;
			}
	default:
		break;
	}
	return FALSE;
}

BOOL cItem::LoadWeapon(string path)
{
	ifstream File;
	File.open(path);
	int type;
	File >> type;
	int num;
	File >> num;
	sItem item;
	int weaponlv;
	for (int i = 0; i < num; i++)
	{
		item.m_Type = type;
		File >> item.m_ID;
		File >> item.m_Name;

		File >> weaponlv;
		item.SetLv(weaponlv);
		File >> item.m_SmallHead;
		File >> item.m_BigHead;
		getline(File, item.m_Text);
		getline(File, item.m_Text);

		//int k = 0;
		//if (i >= 9)k = i - 6;    //
		//else if (i < 3)k = 0;  //0-20������������һ��
		//else if (i < 6)k = 1;  //30-50������������һ��
		//else if (i < 9)k = 2;  //60-80
		////int WeaponOffset = m_pWeaponList[0].Type * 11 + k;
		//m_pWeaponList[type].data[i].m_pData[0] = &m_pWeaponList2[k + type * 11];
		//m_pWeaponList[type].data[i].m_pData[1] = &m_pWeaponList2[k + type * 11 + 165];
	}
	File.close();
	return TRUE;
}

sWeaponData2* cItem::GetWeaponList(int charid, int weaponid, int type)
{
	int trueid;
	//0-20 30-50 60-80 �ȵ���һ������
	if (weaponid<27)trueid = weaponid / 3;
	else trueid = weaponid - 18;

	vector<sWeaponData2>& list=	 g_pMainState->m_ItemManage.m_pWeaponList2[type][trueid];
	for (int i = 0; i < list.size(); i++)
	{
		if (charid == list[i].Master)
			return  &list[i];
	}
	return 0;
}

BOOL cItem::SetItemDescribe(int x,int y,cDescribe& pDescribe, sItem2* pItem)
{
	if (!pItem
		|| !pItem->GetNum()
		)
	{
		if (pDescribe.m_NeedShow)pDescribe.OnOff(FALSE);
		return FALSE;
	}	

	m_Color[2] = RGB(250, 250, 0);
	m_Color[0] = RGB(250, 250, 0);
	m_Color[1] = RGB(220, 220, 220);
	for (int i = 3; i < 10; i++)
	{
		m_Color[i] = RGB(0, 255, 0);
	}
	
	ostringstream oss;
	m_text[0] = pItem->GetName();
	m_text[1] = pItem->GetText();
	 m_numtext = 2;
	int itemtype = pItem->GetType();
	switch (itemtype)
	{
	case 22:
	case 23:
	case 24:
	case 25:
	case 27:
		break;
	case 28:
		if (1)
		{
			switch (pItem->GetID())
			{
			case 0://
				ERRBOX;
				break;
			case 6://�ر�ͼ
				if (1)
				{
					if ("" == pItem->m_strProperty)
					{

						AutoItemProperty(pItem, g_pCharacter);
					}
					g_pMainState->m_TaskBaoTu.Init();
					m_numtext += 1;
					oss.str("");
					oss << "�����д�ŵص�:\n";
					oss << pItem->m_strProperty << " ";

					
					oss << pItem->m_Property[1] << " ";
					oss << pItem->m_Property[2];
					m_text[2] = oss.str();
				}
				break;
			case 7:
				m_numtext += 1;
				oss.str("");
				oss << "�ȼ�:";
				oss << pItem->m_Property[0];
				m_text[2] = oss.str();
				break;
			case 8:
				m_numtext += 2;
				oss.str("");
				oss << "����:";
				oss << g_pMainState->m_ItemManage.m_ItemTypeName[pItem->m_Property[0]];
				m_text[2] = oss.str();
				oss.str("");
				oss << "�ȼ�:";
				oss << pItem->m_Property[1];
				m_text[3] = oss.str();
				break;
			case 9://���Ŀǰû�б�ʹ��
				if (1)
				{
					m_numtext += 2;
					oss.str("");
					oss << "������";
					oss << pItem->m_Property[0];
					oss << "Ǯ";
					m_text[2] = oss.str();
					oss.str("");
					oss.str("");
					oss << "Ŀ��";
					oss << pItem->m_Property[1];
					oss << "Ǯ";
					m_text[3] = oss.str();
				}
			default:
				break;
			}

		}
		break;
	case 31:
		if (1)
		{
			oss.str("");
			oss << "���Ը�";
			oss << g_pMainState->m_ZuoQi.m_ZuoQi[pItem->GetID() / 3].Name;
			oss << "װ��";
			m_text[1] = oss.str();
		}
		break;
	case 32:
	case 33:
	case 34:
		if (1)
		{
			oss.str("");
			oss << "�ȼ� ";
			oss << pItem->m_pItemData->m_ID*10+5;
			m_text[m_numtext] = oss.str();
			oss.str("");
			m_numtext += 1;
			if (pItem->m_Property.size() == 0)
			{
				oss.str("");
				oss << "δ����";
				m_text[m_numtext] = oss.str();
				m_numtext += 1;
			}
			else
			{
				MakePropString2(pItem);
			}
			
		}
		break;
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
		if (1)
		{
			oss.str("");
			oss << "�ȼ� ";
			oss << pItem->m_pItemData->GetLv();
			m_text[m_numtext] = oss.str();
			oss.str("");
			m_numtext += 1;
			if (pItem->m_Property.size() == 0)
			{
				oss.str("");
				oss << "δ����";
				m_text[m_numtext] = oss.str();
				m_numtext += 1;
			}
			else
			{
				MakePropString2(pItem);
			}
		}
		break;
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
		if (1)
		{
			oss.str("");
			oss << "�ȼ� ";
			oss << pItem->m_pItemData->GetLv();
			m_text[m_numtext] = oss.str();
			oss.str("");
			m_numtext += 1;
			if (pItem->m_Property.size() == 0)
			{
				oss.str("");
				oss << "δ����";
				m_text[m_numtext] = oss.str();
				m_numtext += 1;
			}
			else
			{
				MakePropString2(pItem);
			}
		}
		break;
	default:
		break;
	}
	pDescribe.Set(x + 25, y + 25, pItem->m_pItemData->m_BigHead, m_numtext, m_text,m_Color);
	pDescribe.OnOff(TRUE);
	return TRUE;
}
void cItem::AutoItemProperty(sItem2* item, cCharacter* pc)
{
	return AutoItemProperty(item,&pc->m_PCData);
}
void cItem::AutoItemProperty(sItem2* item, cPCData* pc)
{
	int Luck;
	if (!pc)
	{
		cCharacter* pc2 = g_pCharacter;
		Luck = pc2->m_PCData.m_Popularity;
	}
	else
		Luck = pc->m_Popularity;
	AutoItemProperty(item, Luck,1);
	return;
}
void cItem::AutoItemProperty(sItem2* item, int Luck,int dustbin)
{
	g_pMainState->m_InterfaceDoor.m_TimeShiCheng.UpdateTime();
	if (Luck < 10)Luck = 10;
	if (Luck>1000)Luck = 1000;
	item->m_Property.clear();
	item->m_strProperty = "";
	vector<int>& property = item->m_Property;
	property.clear();
	switch (item->GetType())
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
		if (1)
		{
			g_pMainState->m_InterfaceDoor.m_TimeShiCheng.UpdateTime();
			int lv = item->m_pItemData->GetLv();
			int Offset = lv / 10;
		
			
			property.push_back(0);//ǿ���ȼ�
			if (0==rand()%10)
				property.push_back(rand()%36);
			else 	property.push_back(-1);
			property.push_back(-1);
			property.push_back(-1);
			property.push_back(-1);
			int extranum = rand() % 10;
			if (extranum > 2)extranum = 0;

			property.push_back(extranum+2);
			property.push_back(eProPerty�˺�);
			property.push_back((15 + Offset * 40)+(40*rand()%(1000+Luck))/2000 );
			property.push_back(eProPerty����);
			property.push_back((10 + Offset * 35) + (35 * rand() % (1000 + Luck)) / 2000);
			AutoItemProperty2(property, extranum, Luck, Offset);

		}
		break;
		//װ��
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	
		{
			g_pMainState->m_InterfaceDoor.m_TimeShiCheng.UpdateTime();
			int lv;
			lv = item->m_pItemData->GetLv();//  GetlvByID(item->m_pItemData->m_Type, item->m_pItemData->m_ID);

			int Offset = lv / 10;
			property.push_back(0);//ǿ���ȼ�

			//// �õ��ؼ��ļ���
			//int special, special2;
			//if (rand() % (15 - Luck / 100) == 0)
			//	special = rand() % 50;
			//else special = -1;
			//if (rand() % (15 - Luck / 100) == 0)
			//	special2 = 50 + rand() % 8;
			//else  special2 = -1;
			//property.push_back(special);
			//property.push_back(special2);

			if (0 == rand() % 10)
				property.push_back(rand() % 36);
			else 	property.push_back(-1);
			property.push_back(-1);
			property.push_back(-1);
			property.push_back(-1);
			int extranum=0;
			if (18 == item->GetType())
			{
				int extranum = rand() % 10;
				if (extranum > 2)extranum = 0;
			}

			switch (item->GetType())
			{
			case 15:  //ͷ
			case 16:
				property.push_back(extranum+2);
				property.push_back(eProPerty����);
				property.push_back((5 + Offset * 5) + (5 * (rand() % (100 + Luck / 10))) / 100);
				property.push_back(eProPertyħ��);
				property.push_back((5 + Offset * 10) + (10 * (rand() % (100 + Luck / 10))) / 100);
				break;
			case 17:   //��
			case 18:
				property.push_back(extranum+1);
				property.push_back(eProPerty����);
				property.push_back((10 + Offset * 15) + (15 * (rand() % (100 + Luck / 10))) / 100);
				break;
			case 19:  //����
				property.push_back(extranum+1);
				property.push_back(eProPerty����);
				property.push_back((5 + Offset * 12) + (12 * (rand() % (100 + Luck / 10))) / 100);
				break;
			case 20: //����
				property.push_back(extranum+2);
				property.push_back(eProPerty����);
				property.push_back((5 + Offset * 5) + (5 * (rand() % (100 + Luck / 10))) / 100);
				property.push_back(eProPerty��Ѫ);
				property.push_back((10 + Offset * 20) + (20 * (rand() % (100 + Luck / 10))) / 100);
				break;
			case 21: //Ь
				property.push_back(extranum+2);
				property.push_back(eProPerty����);
				property.push_back((5 + Offset * 5) + (5 * (rand() % (100 + Luck / 10))) / 100);
				property.push_back(eProPerty�ٶ�);
				property.push_back((5 + Offset * 3) + (3 * (rand() % (100 + Luck / 10))) / 100);
				break;
			}
			AutoItemProperty2(property, extranum, Luck, Offset);
		
		}
		break;
	case 32:
	case 33:
	case 34:
		{
			g_pMainState->m_InterfaceDoor.m_TimeShiCheng.UpdateTime();
			int Offset = item->m_pItemData->m_ID+1;
			property.push_back(0);//ǿ���ȼ�

			//// �õ���Ч�ļ���
			//int special2;
			//if (rand() % (15 - Luck / 100) == 0)
			//	special2 = 50 + rand() % 8;
			//else  special2 = -1;
			//property.push_back(-1);
			//property.push_back(special2);
			property.push_back(-1);
			property.push_back(-1);
			property.push_back(-1);
			property.push_back(-1);
			int extranum = 0;
			if (18 == item->GetType())
			{
				int extranum = rand() % 10;
				if (extranum > 2)extranum = 0;
			}
			property.push_back(extranum + 2);
			switch (item->GetType())
			{
			case 32://���� ����
				property.push_back(eProPerty����);
				property.push_back(15 + Offset * 50 + (50 * (rand() % (100 + Luck / 10))) / 100);

				property.push_back(eProPerty�˺�);
				property.push_back(5 + Offset * 20 + (20 * (rand() % (100 + Luck/10))) / 100);
				break;
			case 33://��Ȧ �ٶ�	
				property.push_back(eProPerty�ٶ�);
				property.push_back(3 + Offset * 4 + (4 * (rand() % (100 + Luck / 10))) / 100);

				property.push_back(eProPerty����);
				property.push_back(2 + Offset * 3 + (3 * (rand() % (100 + Luck / 10))) / 100);
				break;
			case 34://���� ��
				property.push_back(eProPerty����);
				property.push_back(10 + Offset * 15 + (15 * (rand() % (100 + Luck / 10))) / 100);

				property.push_back(eProPerty��Ѫ);
				property.push_back(30 + Offset * 50 + (50 * (rand() % (100 + Luck / 10))) / 100);
				break;
			}
			AutoItemProperty2(property, extranum, Luck, Offset);
		}
		break;
	case 24://����
			property.push_back(0);
		break;
	case 27:
		break;
	case 28: //���ɵ��ӵ���
		switch (item->GetID())
		{
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
			property.push_back(20);
			break;
		case 6:
			
			{//��ͼ
				int xbaotu, ybaotu;
				g_pMainState->m_TaskBaoTu.Init();
				string mapname = g_pMainState->m_TaskBaoTu.m_MapNameList[rand() % g_pMainState->m_TaskBaoTu.m_MapNameList.size()];
			//	property.push_back(mapid);;
				g_pMainState->m_Map.AutoGetPos(xbaotu, ybaotu, mapname);
				item->m_strProperty = mapname;
				property.push_back(-1);
				property.push_back(xbaotu);
				property.push_back(ybaotu);
				property.push_back(xbaotu * 20);
				property.push_back(g_pMainState->m_Map.m_BmpTest.GetHeight() - ybaotu * 20);

			}
			break;
		case 8:  //������
			if (1)
			{
				int equiptype = rand() % 22;
				if (equiptype == 13)equiptype = rand() % 13;
				int equipid = (rand() % 8) * 10;
				property.push_back(equiptype);
				property.push_back(equipid);
			}
			break;
		default:
			break;
		}
		break;
	case 31:
		if (1)
		{
			property.push_back(0);//����װ��Ⱦɫ
		}
		break;
	default:
		return;
		break;
	}
}



BOOL cItem::LoadJiaJu(string path)
{
	ifstream File;
	File.open(path);
	LoadJiaJuCube(File, 35);
	LoadJiaJuCube(File, 36);
	LoadJiaJuCube(File, 37);
	LoadJiaJuCube(File, 38);
	File.close();
	return TRUE;
}

void cItem::LoadJiaJuCube(ifstream& File, int type)
{
	int lv;
	int num;
	sItem item;

	File >> lv;
	File >> num;
	int jiajulv;
	for (int i = 0; i < num; i++)
	{
		item.m_Type = type;
		item.m_ID = i;
		File >> item.m_Name;
		File >> item.m_SmallHead;
		File >> item.m_BigHead;
		File >> jiajulv;
		item.SetLv(jiajulv);//��¼�Ҿߵ�����
		File >> item.m_Price;

		switch (lv)
		{
		case 1:
			item.m_Text = "һ���Ҿ�"; break;
		case 2:
			item.m_Text = "�����Ҿ�"; break;
		case 3:
			item.m_Text = "�����Ҿ�"; break;
		case 4:
			item.m_Text = "�ļ��Ҿ�"; break;
		default:
			ERRBOX;
			break;
		}
	
		m_ItemList[type].push_back(item);
		//�����Ǽ�԰����Դ,��ʱ����
		sJiaJuData data;
		DWORD PicID;
		File >> data.num;

		for (int j = 0; j < data.num; j++)
		{
			File >> PicID;
			data.m_data.push_back(PicID);
		}
		m_JiaJuData[lv-1].push_back(data);
	}
}

void cItem::GetIDlistByLv(vector<int>& outlist, int type, int lv)
{
	if (lv>160)lv = 160;
	lv /= 10;
	if (type < 15) //����
	{
		if (lv < 9)
		{
			outlist.push_back(lv);
			return;
		}
		if (lv < 12)
		{
			outlist.push_back(lv);
			outlist.push_back(lv+3);
			outlist.push_back(lv+6);
			return;
		}
		if (lv < 15)
		{
			outlist.push_back(lv+6);
			outlist.push_back(lv + 9);
			outlist.push_back(lv + 12);
			return;
		}
		if (lv == 15)
		{
			outlist.push_back(27);
			return;
		}
		outlist.push_back(28);
		return;
	}
	if (type == 19 || type == 20)//װ��
	{
		if (lv == 3)
		{
			outlist.push_back(3);
			outlist.push_back(4);
			return;
		}
		if (lv == 4)
		{
			outlist.push_back(5);
			outlist.push_back(6);
			return; 
		}
		if (lv == 5)
		{
			outlist.push_back(7);
			outlist.push_back(8);
			return;
		}
		if (lv == 6)
		{
			outlist.push_back(9);
			outlist.push_back(10);
			return;
		}
		if (lv == 7)
		{
			outlist.push_back(11);
			outlist.push_back(12);
			return;
		}
		if (lv == 8)
		{
			outlist.push_back(13);
			outlist.push_back(14);
			return;
		}
		if (lv > 8)
		{
			outlist.push_back(lv+6);
			return;
		}
	}
	outlist.push_back(lv);
	return;
}

std::string cItem::MakePropString(vector<int>& prop, int pos)
{
	string str;
	switch (prop[pos])
	{
	case eProPerty�˺�:str = "�˺� ";break;
	case eProPerty����:str = "���� "; break;
	case eProPerty����:str = "���� "; break;
	case eProPertyħ��:str = "ħ�� "; break;
	case eProPerty����:str = "���� "; break;
	case eProPerty�ٶ�:str = "�ٶ� "; break;
	case eProPerty��Ѫ:str = "��Ѫ "; break;
	case eProPerty����:str = "���� "; break;
	case eProPerty����:str = "���� "; break;
	case eProPerty����:str = "���� "; break;
	case eProPertyħ��:str = "ħ�� "; break;
	case eProPerty����:str = "���� "; break;
	case eProPerty��:str = "�� "; break;
	case eProPerty�׿�:str = "�׿� "; break;
	case eProPerty����:str = "���� "; break;
	case eProPertyˮ��:str = "ˮ�� "; break;
	default:
		return str;
	}
	ostringstream oss;
	if (prop[pos + 1] > 0)
	{
		oss << "+";
	}
	oss << prop[pos + 1];
	str += oss.str();
	return str;
}

void cItem::MakePropString2(sItem2* item)
{
	int pos = 5;
	vector<int>& prop = item->m_Property;
	ostringstream oss;

	//��������
	m_Color[m_numtext] = RGB(255, 255, 0);
	int num;
	switch (item->GetType())
	{
	case 17://��1����������
	case 18:
	case 19:
		if (1)
		{		
			m_text[m_numtext] = MakePropString(prop, pos + 1);
			
			m_numtext += 1;
			num = prop[pos] - 1;
			pos += 3;
		}
		break;
	default: //��2����������
		m_text[m_numtext] = MakePropString(prop, pos +1);
		m_text[m_numtext] += "   ";
		m_text[m_numtext] += MakePropString(prop, pos +3);
		m_numtext += 1;
		num = prop[pos] - 2;
		pos += 5;
		break;
	}
	

	//�ؼ�
	if (item->m_Property[1] > -1)
	{
		m_Color[m_numtext] = RGB(126, 255, 126);
		oss.str("");
		oss << "�ؼ�:";
		oss << g_pMainState->m_SkillManager.GetSpecialSkill(item->m_Property[1])->m_Name;
		m_text[m_numtext] = oss.str();
		m_numtext += 1;
	}
	if (item->m_Property[2] > -1)
	{
		m_Color[m_numtext] = RGB(126, 126, 255);
		oss.str("");
		oss << "��Ч:";
		oss << g_pMainState->m_SkillManager.GetSpecialSkill2(item->m_Property[2])->m_Text;
		m_text[m_numtext] = oss.str();
		m_numtext += 1;
	}
	if (item->m_Property[3] > -1)
	{
		m_Color[m_numtext] = RGB(126, 126, 255);
		oss.str("");
		oss << "��Ч:";
		oss << g_pMainState->m_SkillManager.GetSpecialSkill2(item->m_Property[3])->m_Text;
		m_text[m_numtext] = oss.str();
		m_numtext += 1;
	}
	if (item->m_Property[4] > -1)
	{
		m_Color[m_numtext] = RGB(126, 126, 255);
		oss.str("");
		oss << "��Ч:";
		oss << g_pMainState->m_SkillManager.GetSpecialSkill2(item->m_Property[4])->m_Text;
		m_text[m_numtext] = oss.str();
		m_numtext += 1;
	}

	//ǿ���ȼ�
	oss.str("");
	oss << "�����ȼ�: ";
	oss << item->m_Property[0];
	m_Color[m_numtext] = RGB(255, 255, 0);
	m_text[m_numtext] = oss.str();
	m_numtext += 1;


	//��������
	m_Color[m_numtext] = RGB(0, 255, 0);
	for (int i = 0; i < num; i++)
	{
		if (i % 2 == 0)
		{
			m_text[m_numtext] = MakePropString(prop, pos + i * 2);
		}
		else
		{
			m_text[m_numtext] += "   ";
			m_text[m_numtext] += MakePropString(prop, pos + i * 2);
			m_numtext += 1;
		}
	}
	if (num%2 == 1)
	{
		m_numtext += 1;
	}

}

void cItem::AutoItemProperty2(vector<int>& property, int extranum, int Luck, int Offset)
{
	if (!extranum)return;
	int extraprop[2];
	int extrapropnum[2];
	for (int i = 0; i < extranum; i++)
	{
		extrapropnum[i] = rand() % (Offset + Luck / 200 + 1) - Offset / 3;
		if (extrapropnum[i] == 0)extrapropnum[i] = 1;
		switch (rand() % 5)
		{
		case 0:extraprop[i] = eProPerty����; break;
		case 1:extraprop[i] = eProPertyħ��; break;
		case 2:extraprop[i] = eProPerty����; break;
		case 3:extraprop[i] = eProPerty����; break;
		case 4:extraprop[i] = eProPerty����; break;
		default:{extraprop[i] = -1; extrapropnum[i] = 0; }
		}
	}
	for (int i = 0; i < extranum; i++)
	{
		property.push_back(extraprop[i]);
		property.push_back(extrapropnum[i]);
	}
}

BOOL cItem::XiangQian(sItem2& equip, sItem2& gem)
{
	if (gem.GetType() != 29)return FALSE;
	switch (equip.GetType())
	{
	case 15://ͷ��
	case 16:
		switch (gem.GetID())
		{
		case 10://����
		case 9://̫��
		case 2://���
			break;
		default:return FALSE;
		}
		break;
	case 17:
	case 18:
		switch (gem.GetID())
		{
		case 10://����
		case 0://��â
		case 7://����
			break;
		default:return FALSE;
		}
		break;
	case 19://����
		switch (gem.GetID())
		{
		case 3://�챦ʯ
		case 4://�Ʊ�ʯ
		case 5://����ʯ
		case 6://�̱�ʯ
		case 7://����
			break;
		default:return FALSE;
		}
		break;
	case 20://����
		switch (gem.GetID())
		{
		case 1://�ڱ�ʯ
		case 0://��â
			break;
		default:return FALSE;
		}
		break;
	case 21://Ь
		switch (gem.GetID())
		{
		case 3://�챦ʯ
		case 4://�Ʊ�ʯ
		case 5://����ʯ
		case 6://�̱�ʯ
		case 8://����
		case 1://�ڱ�ʯ
			break;
		default:return FALSE;
		}
		break;
	case 32://����
		switch (gem.GetID())
		{
		case 12:
		case 15:
			break;
		default:return FALSE;
		}
		break;
	case 33://��Ȧ
		switch (gem.GetID())
		{
		case 11:
		case 14:
			break;
		default:return FALSE;
		}
		break;
	case 34://����
		switch (gem.GetID())
		{
		case 13:
		case 16:
			break;
		default:return FALSE;
		}
		break;
	default:
		if (equip.GetType() < 22)
		{
			switch (gem.GetID())
			{
			case 9:
			case 8:
			case 2:
				break;
			default:return FALSE;
			}
		}
		else return FALSE;
		break;
	}
	
	SetUpBaoShi(equip.m_Property, gem.GetID());
	return TRUE;
}

void cItem::SetUpQiangHua(vector<int>& prop,int protype, int num)
{ //

	int i = 0;
	int startpos = 5;
	for (; i < prop[startpos]; i++)
	{
		if (protype == prop[startpos + 1 + i * 2])
		{
			prop[startpos + 2 + i * 2] += num;
			break;
		}
	}
	if (i == prop[startpos])
	{
		prop.push_back(protype);
		prop.push_back(num);
		prop[startpos] += 1;
	}
	
}

void cItem::SetUpBaoShi(vector<int>& pro, int baoshiid,int m_bSetup)
{//��ʯЧ��
	pro[0] += 1;
	switch (baoshiid)
	{
	case 0:SetUpQiangHua(pro, eProPerty��Ѫ, 40*m_bSetup); return;
	case 1:SetUpQiangHua(pro, eProPerty�ٶ�, 8 * m_bSetup); return;
	case 2:
		SetUpQiangHua(pro, eProPerty����, 25 * m_bSetup);
		SetUpQiangHua(pro, eProPerty�˺�, 7 * m_bSetup);
		return;
	case 3:SetUpQiangHua(pro, eProPerty��, 2 * m_bSetup); return;
	case 4:SetUpQiangHua(pro, eProPerty�׿�, 2 * m_bSetup); return;
	case 5:SetUpQiangHua(pro, eProPertyˮ��, 2 * m_bSetup); return;
	case 6:SetUpQiangHua(pro, eProPerty����, 2 * m_bSetup); return;
	case 7:SetUpQiangHua(pro, eProPerty����, 6 * m_bSetup); return;
	case 8:SetUpQiangHua(pro, eProPerty���, 40 * m_bSetup); return;
	case 9:SetUpQiangHua(pro, eProPerty�˺�, 8 * m_bSetup); return;
	case 10:SetUpQiangHua(pro, eProPerty����, 12 * m_bSetup); return;
	case 11:SetUpQiangHua(pro, eProPerty�ٶ�, 6 * m_bSetup); return;
	case 12:SetUpQiangHua(pro, eProPerty����, 4 * m_bSetup); return;
	case 13:SetUpQiangHua(pro, eProPerty����, 8 * m_bSetup); return;
	case 14:SetUpQiangHua(pro, eProPerty���, 30 * m_bSetup); return;
	case 15:SetUpQiangHua(pro, eProPerty�˺�, 10 * m_bSetup); return;
	case 16:SetUpQiangHua(pro, eProPerty��Ѫ, 30 * m_bSetup); return;
	}
}

BOOL cItem::UseJiaJu(sItem2* pItem)
{
	string mapname = g_pMainState->m_Map.m_MapName;
	string mapnameneed = g_pMainState->m_FangWu.GetMapName();
	if (0 != mapname.compare(mapnameneed))return TRUE;

	if (pItem->m_pItemData->GetLv() == 3)
	{
		g_pMainState->m_FangWu.m_DiBan = *pItem;
		cObj* obj = g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData;
		int xold = obj->GetX();
		int yold = obj->GetY();

		g_pMainState->m_FangWu.GoIn();
		for (int i = 0; i < 5; i++)
		{
			if (g_pMainState->m_FightList[i].isLive)
			g_pMainState->m_FightList[i].m_pData->SetXY(xold, yold);
		}
		pItem->m_Num = 0;
		g_pMainState->m_FangWu.CheckHuanJin();
		return TRUE;
	}

	g_pMainState->m_UseJiaJu.SetJiaJu(pItem);
	return TRUE;
}

void cItem::AutoFuShi(sItem2* item, int lv, eMengPai mengpaiid)
{
	if (lv < 60)return;
	if (item->m_Property.size() < 5)return;
	else if (lv > 59)
	{
		if (item->GetType() < 15)
		{
			switch (mengpaiid)
			{
			case eMP_ʨ����:
				item->m_Property[2] = 86;
				break;
			default:item->m_Property[2] = 14;
				break;
			}
		}
		else
		{
			item->m_Property[2] = 14;
		}
		
	}
	if (lv > 89)
	{
		item->m_Property[3] = 0;
	}
	if (lv>119)
	{
		item->m_Property[4] = 16;
	}
}







BOOL cZuoQi::GetZuoQi(int charID, int ZuoQiID)
{
	cCharacter* p = (cCharacter*)g_pMainState->m_FightList[charID].m_pData->m_pIndex;
	return GetZuoQi(p->m_PCData, ZuoQiID);
}
BOOL cZuoQi::GetZuoQi(cPCData& data, int ZuoQiID)
{
	if (ZuoQiID < 0)return TRUE;
	data.zuoqiid = ZuoQiID;
	data.m_ZuoQi.m_pZuoQi = &m_ZuoQi[ZuoQiID];
	data.m_ZuoQi.m_ownerID = data.modeid;
	data.m_ZuoQiRanSe.partnum = 0;
	data.m_ZhuangShiRanSe.partnum = 0;
	g_pMainState->m_InterfaceItem.CheckZuoQi();
	return TRUE;
}
BOOL cZuoQi::Init(string Path)
{
	ifstream File;
	File.open(Path);
	if (!File)return FALSE;
	int num;
	File >> num;
	string dustbin;
	if (8 != num)return FALSE;
	for (int i = 0; i < 8; i++)
	{
	//	m_ZuoQi[i].ID = i;
		File >> m_ZuoQi[i].ID; //m_ZuoQi[i].ownerID;
		File >> m_ZuoQi[i].Name;
		File >> m_ZuoQi[i].nodeStand;
		File >> m_ZuoQi[i].nodeWalk;
		File >> m_ZuoQi[i].m_ColorID;
		for (int j = 0; j < 15; j++)
		{
			File >> dustbin;
			File >> m_ZuoQi[i].CharStand[j];
			File >> m_ZuoQi[i].CharWalk[j];
		}
	}
	return TRUE;
}

void cZuoQi::GetShiPei(vector<int>& list, int modeid)
{
	list.clear();
	list.push_back(0);
	list.push_back(1);
	switch (modeid)
	{
	case eJuSe������:
	case eJuSe��ң��:
	case eJuSe����Ů:
	case eJuSeӢŮ��:
	case eJuSe������:
		list.push_back(2);
		list.push_back(3);
		break;
	case eJuSe��ͷ��:
	case eJuSe��ħ��:
	case eJuSe�Ǿ���:
	case eJuSe������:
	case eJuSeɱ����:
		list.push_back(4);
		list.push_back(5);
		break;
	case eJuSe��̫��:
	case eJuSe�����:
	case eJuSe���ʶ�:
	case eJuSe���켧:
	case eJuSe������:
		list.push_back(6);
		list.push_back(7);
		break;
	}
}


void cJiangLi::AutoGetJiangLi()
{
	if (!m_List.size())return;
	int rat = rand() % 100;
	if (rat < m_MoenyRate)
	{
		SetJiangLi(m_Moneytime, TRUE, 100, 0, 0, 0);
		return;
	}
	rat -= m_MoenyRate;
	if (rat < m_ExpRate)
	{
		SetJiangLi(m_Exptime, TRUE, 0,0,100);
		return;
	}
	rat -= m_ExpRate;
	for (int i = 0; i < m_List.size(); i++)
	{
		if (rat < m_List[i].m_Rate)
		{
			if (m_List[i].m_rand>1)
			{
				switch (m_List[i].m_Type)
				{
				case 40://���޾�
					if (m_List[i].m_rand == 5)
					{
						SetJiangLiItem(g_pMainState->m_ItemManage.m_LShouJueList[rand()%43], m_List[i].m_Type);
					}
					else //���޾�
					{
						SetJiangLiItem(g_pMainState->m_ItemManage.m_HShouJueList[rand() % 43], m_List[i].m_Type);
					}
					break;
				default:
					SetJiangLiItem(m_List[i].m_Id + rand() % m_List[i].m_rand, m_List[i].m_Type);
					break;
				}
				
			}
			else
			SetJiangLiItem(m_List[i].m_Id, m_List[i].m_Type);
			return;
		}
		rat -= m_List[i].m_Rate;
	}

}

void cJiangLi::Add(int id, int type, int rate, int ran/*=1*/)
{
	sJiangLiList list;
	list.m_Id = id;
	list.m_rand = ran;
	list.m_Rate = rate;
	list.m_Type = type;
	m_List.push_back(list);
}
