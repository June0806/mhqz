#include "global.h"



BOOL cTaskBaotu::Process(int stage)
{
	if (!m_bInit)Init();
	switch (stage)
	{
	case 0:
		
		{	
			g_pMainState->m_Dialog.Reset();
			g_pMainState->m_Dialog.SetObj("��С��");
			 m_Robber.m_name = AutoMakeName();
			m_Robber.m_mapname = m_MapNameList[rand() % m_MapNameList.size()];
			m_Robber.m_modeid = 5;
			g_pMainState->m_Map.AutoGetPos(m_Robber.m_x, m_Robber.m_y, m_Robber.m_mapname);
			ostringstream oss;
			oss.str("");
			oss << "�������Ŵ��#Rǿ��";
			oss << m_Robber.m_name;
			oss << "#R���ֽ��������²��ٰ���,ֻ�Ǵ�δ��������,��˵�����ѱ�������#R";
			oss << m_Robber.m_mapname;
			oss << "#R#Y";
			oss << m_Robber.m_x;
			oss << " ";
			oss << m_Robber.m_y;
			oss << "#Y ��,��֪���ǲ������,Ҫ�����ҵ��ͷ�����";
			string context = oss.str();
			g_pMainState->m_Dialog.SetString(context);
			g_pMainState->m_Dialog.OnOff(TRUE);
			cObj* pObj = g_pMainState->FindObjByName("��С��");
			pObj->m_AutoRun = FALSE;

			//��ΪҪ���������ɫ��ͬ,ֻ������һ��
			oss.str("");
			oss << "�������Ŵ��#Rǿ��";
			oss << m_Robber.m_name;
			oss << "#R���ֽ��������²��ٰ���,ֻ�Ǵ�δ��������,��˵�����ѱ�������#R";
			oss << m_Robber.m_mapname;
			oss << "#R#B";
			oss << m_Robber.m_x;
			oss << " ";
			oss << m_Robber.m_y;
			oss << "#B ��,��֪���ǲ������,Ҫ�����ҵ��ͷ�����";
			context = oss.str();
			sJob job;
			job.m_Context = context;
			job.m_Title = "��ͼ";
			job.m_Time = 40;
			job.m_pTask = this;
			g_pMainState->m_Job.AddaJob(job);
			g_pMainState->m_TriggerObjMap.AddTask(this, 1,m_Robber);
			return TRUE;
		}
		break;
	case 1:
	
		{
			
			sAddPoint point;
			point.Set(0, 0, 0, 5, 0);
			int id = g_pMainState->AddPetEnemy(m_Robber.m_name, m_Robber.m_modeid, g_pCharacter->m_PCData.m_LV, &point, 80);
			cPetData* pPetdata;
			g_pMainState->m_FightList[id].m_pData->GetDateP(pPetdata);
			pPetdata->m_Controler.Set();
			g_StateType = STATE_FIGHT;
			AddTriggerFightSuccess(3);
			SetMusic();
			return TRUE;
		}
		break;
	case 3:
		
		{
			if (0==rand()%10)
			{

				string nowname = m_Robber.m_name;
				m_Robber.m_name = AutoMakeName();

				m_Robber.m_mapname = m_MapNameList[rand() % m_MapNameList.size()];
				g_pMainState->m_Map.AutoGetPos(m_Robber.m_x, m_Robber.m_y, m_Robber.m_mapname);
				ostringstream oss;
				oss << "�����,��ֻ�Ǹ�Сආ�,�����ϴ���#R";
				oss << m_Robber.m_mapname;
				oss << "#R " << "#R" << m_Robber.m_x << " " << m_Robber.m_y << "#R";
				g_pMainState->FindObjByName(nowname)->Talk(oss.str());
				g_pMainState->m_Dialog.m_pObj = 0;
				g_pMainState->m_Map.RemoveNpc(nowname);
				m_Robber.m_modeid = 8;
				g_pMainState->m_TriggerObjMap.AddTask(this, 4, m_Robber);

				sJob job;
				job.m_Context = oss.str();
				job.m_Title = "��ͼ";
				job.m_Time = 40;
				job.m_pTask = this;
				g_pMainState->m_Job.AddaJob(job);
			}
			else
			{
				g_pMainState->m_Map.RemoveNpc(m_Robber.m_name);
				g_pMainState->m_TriggerObjMap.ReMove(this);
				g_pMainState->m_Job.ReMoveJob(this);
				if (rand() % 100 < 80)
				{

					//�ñ�ͼ
					sItem2 item;
					item.Set(28, 6);
					item.m_Num = 1;
					g_pMainState->m_ItemManage.AutoItemProperty(&item);
					g_pMainState->SetJiangLiItem(item, 1);
				}
				
				
			}
			sFightList& lis = g_pMainState->m_FightList[g_pMainState->m_HeroID + 5];
			INT64 petexp = 0;
			int money = g_pCharacter->m_PCData.m_LV * 10 + 2000 + rand() % 1000;
			if (lis.isLive&&!lis.CheckDie())
			{
				cPetData* petdata;
				lis.m_pData->GetDateP(petdata);
				petexp = GetNormalFightExp(g_pCharacter->m_PCData.m_LV, g_pCharacter->m_PCData.m_LV);
			}
			SetJiangLi2(0, money, 0, (int)petexp);
				return TRUE;
		}
		break;
	case 4:
	
		{
			cPetData* pPet;
			pPet= AddPCPetEnemyF(g_pCharacter->m_PCData.m_LV * 120, (ePetID)m_Robber.m_modeid, g_pCharacter->m_PCData.m_LV, eMP_���, m_Robber.m_name);
			pPet->m_Combo.d1 += 50;
			for (int i = 0; i < 5; i++)
			{
				pPet=AddPCPetEnemyF(g_pCharacter->m_PCData.m_LV * 80, eǿ��, g_pCharacter->m_PCData.m_LV);
				pPet->m_HitRate.d2 += 20;
			}
			SetMusic();
			g_StateType = STATE_FIGHT;
			AddTriggerFightSuccess(5);
			return TRUE;
		}
		break;
	case 5:
		
		{
			g_pMainState->m_Map.RemoveNpc(m_Robber.m_name);
			g_pMainState->m_TriggerObjMap.ReMove(this);
			g_pMainState->m_Job.ReMoveJob(this);
			//���屦
			SetJiangLiItem(6+rand()%5,27);
			SetJiangLi(3, TRUE, 100, 0, 100, 200);
		}
		break;
	case 6:
	{
			  int lv = g_pCharacter->m_PCData.m_LV;
			  string name="Զ��";
			  ePetID petid=(ePetID)(rand()%40);
			  name += g_pMainState->m_PetDataManager.GetPetData(petid)->m_Name;
			  int enemynum = g_pMainState->AtutoGetEnemyNum()-1;
			  cPetData* pPet = AddPCPetEnemyF(lv * 80, petid, -1, eMP_���, name);  
			  pPet->AddSkill(ePS_�߼�����);
			  for (int i = 0; i < enemynum; i++)
			  {
				  AddPCPetEnemyF(lv * 40, (ePetID)(rand()%40));
			  }
			  AddTriggerFightSuccess(7);
			  g_StateType = STATE_FIGHT;
			  SetMusic();
	}
		break;
	case 7:
	{
			  SetJiangLi(2, 1, 50, 0, 100, 200);
			  if (0 == rand() % 10)
			  {
				  SetJiangLiItem(20, 24);
			  }
	}
	default:
		break;
	}
	return FALSE;
}

void cTaskBaotu::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;
	
	//��ȡ�����б�
	ifstream File;
	ostringstream oss;
	oss << g_strMediaPath << "����//��ͼ//�ټ���.txt";
	File.open(oss.str());
	int num;
	File >> num;
	string data;
	for (int i = 0; i < num; i++)
	{
		File >> data;
		m_Firstname.push_back(data);
	}
	File >> num;
	for (int i = 0; i < num; i++)
	{
		File >> data;
		m_Secondname.push_back(data);
	}
	File.close();
	oss.str("");
	oss << g_strMediaPath << "����//��ͼ//�ص�.txt";
	File.open(oss.str());
	File >> num;
	string str;
	for (int i = 0; i < num; i++)
	{
		File >> str;
		m_MapNameList.push_back(str);
	}
	File.close();
	
	
}

std::string cTaskBaotu::AutoMakeName()
{
	string str;
	str= m_Firstname[rand() % m_Firstname.size()];
	str+= m_Secondname[rand() % m_Secondname.size()];
	return str;
}



BOOL cTaskNormal::Process(int stage)
{
	cPetData* pUserData;
	cObj* pUser = g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData;
	pUser->GetDateP(pUserData);
	switch (stage)
	{
	case -1:break;
	case 1: //��������
	case 3://��������
		g_pMainState->ReadyToFight();
		AddTriggerFightSuccess(4);
		break;
	case 4://����
		{
			if (m_pNpc)
			{
				g_pMainState->m_Map.RemoveNpc(m_pNpc,TRUE);
				m_pNpc = 0;
			}
			//���㲢��ʾ����
			int fightnum = g_pMainState->GetLiveNum(0,5);
			INT64 ExpSuccess = GetNormalFightExp(pUserData->m_LV, g_pMainState->m_Map.m_EnemyLv, fightnum);
			INT64 ExpSuccessBB = GetNormalFightExp(g_pMainState->m_Map.m_EnemyLv, g_pMainState->m_Map.m_EnemyLv, fightnum);
			int money = g_pMainState->m_Map.m_EnemyLv*(fightnum+3)*3;
			if (g_pCharacter->m_PCData.m_FightPetID == -1)ExpSuccessBB = 0;
			g_pMainState->SetJiangLi(ExpSuccess, ExpSuccessBB, money,0,FALSE);
			//m_WeaponRate���ʵõ�װ��
			if (rand() % 100 < g_pMainState->m_Map.m_WeaponRate)
			{
				int type = rand() % 22;
				if (type == 13)type = rand() % 13;
				int itemid = g_pMainState->m_ItemManage.GetIDByLv(type, g_pMainState->m_Map.m_Weaponlv*10);
				sItem2 item;
				item.m_Num = 1;
				item.Set(type, itemid);
				g_pMainState->SetJiangLiItem(item);
			}
			return TRUE;
		}
		break;
	case 5://���ɴ�����
		if (!g_pMainState->m_Dialog.m_retSelect)
		{
		
			cMyMap& map = g_pMainState->m_Map;
			switch (g_pCharacter->m_PCData.m_MengPaiID)
			{
			case 0:map.LoadMap(151, 10, "���ƹٸ�"); break;
			case 1:map.LoadMap(88, 49, "������"); break;
			case 2:map.LoadMap(20, 98, "Ů����"); break;
			case 3:map.LoadMap(104, 121, "����ɽ"); break;
			case 4:map.LoadMap(103, 16, "ʨ����"); break;
			case 5:map.LoadMap(73, 60, "ħ��կ"); break;
			case 6:map.LoadMap(175, 113, "��˿��"); break;
			case 7:map.LoadMap(37, 60, "���ܵظ�"); break;
			case 8:map.LoadMap(79, 40, "����"); break;
			case 9:map.LoadMap(28, 18, "��ׯ��"); break;
			case 10:map.LoadMap(16, 55, "����ɽ"); break;
			case 11:map.LoadMap(159, 55, "�칬"); break;
			}
		}
		break;
	case 6:
		
		{
			if (g_pMainState->m_Dialog.m_retSelect == 5)
				return TRUE;
			cMyMap& map = g_pMainState->m_Map;
			switch (g_pCharacter->m_PCData.m_race)
			{
			case 4:
			case 5:
			case 6://��				
				switch (g_pMainState->m_Dialog.m_retSelect)
				{
				case 0:map.LoadMap(151, 10, "���ƹٸ�"); break;
				case 1:map.LoadMap(88, 49, "������"); break;
				case 2:map.LoadMap(20, 98, "Ů����"); break;
				case 3:map.LoadMap(104, 121, "����ɽ"); break;
				}
				break;
			case 1:
			case 3://��
				switch (g_pMainState->m_Dialog.m_retSelect)
				{
				case 0:map.LoadMap(103, 16, "ʨ����"); break;
				case 1:map.LoadMap(73, 60, "ħ��կ"); break;
				case 2:map.LoadMap(175, 113, "��˿��"); break;
				case 3:map.LoadMap(37, 60, "���ܵظ�"); break;
				}
				break;
			case 0:
			case 2://��	
				switch (g_pMainState->m_Dialog.m_retSelect)
				{
				case 0:map.LoadMap(79, 40, "����"); break;
				case 1:map.LoadMap(28, 18, "��ׯ��"); break;
				case 2:map.LoadMap(16, 55, "����ɽ"); break;
				case 3:map.LoadMap(159, 55, "�칬"); break;
				}
				break;
			}
		}
		break;
	case 7:
		if (g_pMainState->m_Dialog.m_retSelect == 0)
		{
			g_pMainState->m_Map.LoadMap(476, 30, "������");
			return TRUE;
		}
		break;
	case 8://�ܼ�  // ��Ϣ
		g_pCharacter->m_PCData.FillHPMP();
		break;
	case 118:
	
		{

			g_pMainState->m_FangWu.GetBB();
		}
		break;
	case 119:
		
		{
			g_pMainState->m_FangWu.Move(g_pHeroObj->GetX(), g_pHeroObj->GetY(), g_pMainState->m_Dialog.m_pObj);
		}
		break;
	case 120:
		g_pMainState->m_FangWu.RemovePreJiaJu();
		break;
	case 9://��լ��
		
		{
			cObj* pObj = g_pMainState->m_pNowSelect;
			g_pMainState->m_Dialog.Reset();
			g_pMainState->m_Dialog.SetObj(pObj);
			g_pMainState->m_Dialog.SetString("����,������ʲô�Ը�?", "��Ϣ", "��������", "����(����100��)");
			g_pMainState->m_Dialog.SetTrigger(this,10, 8,10,10,10);
			g_pMainState->m_Dialog.OnOff(TRUE);
		}
		break;
	case 10:
		switch (g_pMainState->m_Dialog.m_retSelect)
		{
		case 0:
			g_pCharacter->m_PCData.FillHPMP();
			break;
		case 1:
				g_pMainState->m_FangWu.Move(g_pHeroObj->GetX(), g_pHeroObj->GetY(), g_pMainState->m_Dialog.m_pObj);
			break;
		case 2:
			
			{
				g_pCharacter->m_PCData.m_Money[0] -= 100000;
				g_pMainState->m_FangWu.RemoveBB(g_pMainState->m_Dialog.m_pObj);
				cObj* pObj = g_pMainState->m_Dialog.m_pObj;
				g_pMainState->m_Dialog.m_pObj->Talk("��������լ��");
				g_pMainState->m_Map.RemoveNpc(pObj->GetX(), pObj->GetY(),"",1);
			}
			break;
		}
		break;
	case 12://�ϼ�����
	
		{
			int lv = g_pMainState->m_GlobalButton[3];
			if (lv > 4){ g_pMainState->m_pNowSelect->Talk("���Ѿ�ûʲô���԰������"); return TRUE; }
			ostringstream oss;
			oss << "��ǰ������̵ȼ�Ϊ";
			oss << lv;
			oss << "����һ��������Ҫ";
			oss << lv+1;
			oss << "������,��Ҫ��?";
			g_pMainState->m_TriggerDialog.AddSelect(this, oss.str(), g_pMainState->m_pNowSelect->m_TrueName.GetString(), -1, "��", 13, "��", -1);
		}
		break;
	case 13:
		
		{
			int moneyneed = (g_pMainState->m_GlobalButton[3]+1) * 1000000;
			if (g_pCharacter->m_PCData.AddMoney(0, -moneyneed,0,FALSE))
			{
				g_pMainState->m_GlobalButton[3] += 1;
				ostringstream oss;
				oss << "���������̴ﵽ��" << g_pMainState->m_GlobalButton[3]<<"��!#88";

				g_pMainState->m_Dialog.TalkObj(oss.str());
			}
			else
			{
				g_pMainState->m_Dialog.TalkObj("���Ǯ����#3");
			}
		}
		break;
	case 14:
		
		{
			if (!g_pMainState->m_ItemManage.m_pNowItem)return FALSE;
			sItem2* pi = g_pMainState->m_ItemManage.m_pNowItem;
			pi->m_Property[0] -= 1;
			if (0 == pi->m_Property[0])pi->m_Num = 0;
			g_pMainState->m_Map.LoadMap(pi->m_Property[1], pi->m_Property[2], pi->m_strProperty.c_str(), 1);
		}
		break;
	case 15:
		
		{
			if (!g_pMainState->m_ItemManage.m_pNowItem)return FALSE;
			sItem2* pi = g_pMainState->m_ItemManage.m_pNowItem;
			pi->m_Property[1]=g_pMainState->m_InterfaceDoor.m_HeroX;
			pi->m_Property[2]=g_pMainState->m_InterfaceDoor.m_HeroY;
			pi->m_strProperty = g_pMainState->m_Map.m_MapName;
			g_pMainState->m_Tags.Add("�ڵ�ǰλ�ö�������");
		}
		break;
	default:
		break;
	}
	return FALSE;
}

BOOL cTrigger::AddTask(cTaskFather* ptask, int stage)
{
	sTaskFather task;
	task.m_pTask = ptask;
	task.m_Stage = stage;
	int num = m_TaskList.size();
	for (int i = 0; i < num; i++)
	{
		if (m_TaskList[i].m_pTask->GetType() == ptask->GetType())
		{
			m_TaskList[i].m_Stage = stage;
			return FALSE;
		}
	}
	m_TaskList.push_back(task);
	return TRUE;
}

void cTrigger::ReMove(cTaskFather* pTask)
{
	return ReMove(pTask->GetType());
}

void cTrigger::ReMove(int id)
{
	int num = m_TaskList.size();
	for (int i = 0; i < num; i++)
	{
		if (m_TaskList[i].m_pTask->GetType() == id)
		{
			m_TaskList.erase(m_TaskList.begin() + i);
		}
	}
}

BOOL cTrigger::Process()
{
	int num = m_TaskList.size();
	if (!num)return FALSE;
	BOOL b=FALSE;
	for (int i = 0; i < num; i++)
	{
		if (m_TaskList[i].m_pTask->Process(m_TaskList[i].m_Stage))
			b = TRUE;
	}
	return b;
}

BOOL cTrigger::ProcessReMove()
{
	int num = m_TaskList.size();
	if (!num)return FALSE;
	cTaskFather* ptask = m_TaskList[0].m_pTask;
	int stage = m_TaskList[0].m_Stage;
	Clear();
	ptask->Process(stage);
	return TRUE;
}

void cTaskZhuaGui::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;
	//��ȡ�����б�
	ifstream File;
	ostringstream oss;
	oss << g_strMediaPath << "����//ץ��//�ټ���.txt";
	File.open(oss.str());
	if (!File)ERRBOX;
	int num;
	File >> num;
	string data;
	for (int i = 0; i < num; i++)
	{
		File >> data;
		m_Firstname.push_back(data);
	}
	File >> num;
	for (int i = 0; i < num; i++)
	{
		File >> data;
		m_Secondname.push_back(data);
	}
	File >> num;
	for (int i = 0; i < num; i++)
	{
		File >> data;
		m_Thirdname.push_back(data);
	}
	File.close();
	oss.str("");
	oss << g_strMediaPath << "����//ץ��//�ص�.txt";
	File.open(oss.str());
	if (!File)ERRBOX;
	File >> num;
	string str;
	for (int i = 0; i < num; i++)
	{
		File >> str;
		m_MapNameList.push_back(str);
	}
	
	
}

BOOL cTaskZhuaGui::Process(int stage)
{
	CheckInit();
	switch (stage)
	{
	case 0:
		
		{
			cObj* pObj = g_pMainState->FindObjByName("��ظ");
			if (g_pMainState->m_Job.CheckHaveJob(m_type))
			{
				pObj->Talk("���Ѿ���������");
				return TRUE;
			}
			
			//int ournum = g_pMainState->GetPCFightNum();
			//if (ournum < 3)
			//{
			//	pObj->Talk("��������Σ��,�㻹���ٶ��ҵ��˰�(����3�����");
			//	return TRUE;//����3����
			//}

			AutoGetGost(m_Ghost, m_MapNameList[rand() % m_MapNameList.size()]);
			//�������־���λ��
			int  falsex;
			int  falsey;
			GetErrPos(m_Ghost, falsex, falsey);
			ostringstream oss;
			oss.str("");
			oss << "��һֻ#R";
			oss << m_Ghost.m_name;
			oss << "#R�ӳ��˵ظ�,���ҹ���,��Ӧ����#R";
			oss << m_Ghost.m_mapname;
			oss << "#R#Y";
			oss << falsex;
			oss << " ";
			oss << falsey;
			oss << "#Y ����,������ȥץ������";
			string context = oss.str();
			pObj->Talk(oss.str());
			g_pMainState->m_Dialog.m_pObj = 0;
			pObj->m_AutoRun = FALSE;
			UpdateZGJob();
			
		
			m_HuanShu += 1;

			g_pMainState->m_TriggerObjMap.AddTask(this, 1,m_Ghost);
			return TRUE;
		}
		break;
	case 1:
		
		{
			ReadyToFight(m_Ghost);
			AddTriggerFightSuccess(3);
			return TRUE;
		}
		break;
	case 3:
		
		{
			BOOL btrigger=FALSE;
			if (m_HuanShu == m_RetNum)btrigger = TRUE;
			m_Ghost.m_modeid = -1;
			m_Ghost.m_mapname = "";
			g_pMainState->m_Map.RemoveNpc(m_Ghost.m_name);
			g_pMainState->m_TriggerObjMap.ReMove(this);
			g_pMainState->m_Job.ReMoveJob(this);
			SetJiangLi(5, TRUE, 100, 0, 100, 100);
			//��װ��
			if (m_HuanShu==10)
			{
				int itemtype = rand() % 22;
				int lv=-1;
				if (g_pCharacter->m_PCData.m_LV > 80)
				{
					switch (rand()%4)
					{
					case 0:
						lv = 80;
						break;
					case 1:
						lv = 70;
						break;
					case 2:
						lv = 60;
						break;
					default:
						break;
					}
				}
				else
				if (g_pCharacter->m_PCData.m_LV >60)
				{
					switch (rand() % 4)
					{
					case 0:
						lv = 60;
						break;
					case 1:
						lv = 50;
						break;
					}
				}
				else
				{
					switch (rand() % 4)
					{
					case 0:
						lv = 50;
						break;
					}
				}
				if (lv > -1)
				{
					int itemid= g_pMainState->m_ItemManage.GetIDByLv(itemtype,lv);
					sItem2 item;
					item.m_Num = 1;
					item.Set(itemtype, itemid);
					g_pMainState->m_InterfaceItem.GetItem(g_pMainState->m_HeroID,&item);
					ostringstream oss;
					oss << "��õ���";
					oss << item.m_pItemData->m_Name;
					g_pMainState->m_Tags.Add(oss.str());
				}
				m_HuanShu = 0;
			}
			if (btrigger)
			{
				cTaskFather* p = m_pTask;
				m_pTask = 0;
				m_RetNum = 100;
				p->Process(m_RetStage);
			}
			return TRUE;
		}
		break;
	default:
		break;
	}
	return FALSE;
}

std::string cTaskZhuaGui::AutoMakeName()
{
	Init();
	string str;
	str = m_Firstname[rand() % m_Firstname.size()];
	str += m_Secondname[rand() % m_Secondname.size()];
	str += m_Thirdname[rand() % m_Secondname.size()];
	return str;
}

BOOL cTaskZhuaGui::ReadyToFight(sNpc& npc)
{
	int ournum = g_pMainState->GetPCFightNum();
	
	int enemynum =  rand() % (m_HuanShu/2+1) + ournum;//������˵�����
	if (enemynum > 10)enemynum = 10;
	int Averagelevel = g_pMainState->GetAverageLv();
	//��������  18Ұ��  26��ʬ  27 ţͷ  28 ���� 10 ���ù�
	
	sZiZhi zizhi;
	zizhi = g_pMainState->m_PetDataManager.GetPetData(npc.m_modeid)->m_ZiZhi;
	cObj* pTarget = g_pMainState->m_FightList[10].m_pData;
	g_pMainState->m_FightList[10].isLive = TRUE;
	pTarget->ResetIndex();
	sAddPoint addpoint;
	pTarget->AutoPCData(npc.m_name, 0, 0, 0, (eMengPai)(rand() % 12), 0, npc.m_modeid, 50 + m_HuanShu * 5);
	g_pMainState->FollowLv(10, Averagelevel);
	cPetData* pPetdata;
	pTarget->GetDateP(pPetdata);
	switch (npc.m_modeid)
	{
	case 26://Ѫ��
		pPetdata->m_HPMax.AddDataAddPercent(2000);
		break;
	case 28://����
		pPetdata->m_DamageAddPercentMagic.d1 += 70;
		pPetdata->m_HPMax.AddDataAddPercent(1400);
		break;
	case 10://����
		pPetdata->m_HitRate.d2 += 50;
		pPetdata->m_HPMax.AddDataAddPercent(800);
		break;
	case 27://ţͷ
		pPetdata->m_DamagePercent.d2 += 70;
		pPetdata->m_HPMax.AddDataAddPercent(800);
		break;
	default: //Ұ��
		pPetdata->m_DamagePercent.d2 += 70;
		pPetdata->m_DamageAddPercentMagic.d1 += 70;
		pPetdata->m_HPMax.AddDataAddPercent(600);
		break;
	}
	pPetdata->FillHPMP();
	enemynum -= 1;

	sPetData* pPetdata2;
	string enemyname;
	int modeid;
	addpoint.Set(5, 0, 0, 0, 0);
	for (int i = 0; i < enemynum; i++)
	{
		modeid = rand() % 34;
		pPetdata2 = g_pMainState->m_PetDataManager.GetPetData(modeid);
		enemyname = pPetdata2->m_Name;
		enemyname += "���";
		int id = g_pMainState->AddPCPetEnemy((ePetID)modeid, g_pCharacter->m_PCData.m_LV - 15, &addpoint, (eMengPai)(rand() % 12), enemyname);
		g_pMainState->m_FightList[id].m_pData->GetDateP(pPetdata);
		pPetdata->FillHPMP();
	}
	g_StateType = STATE_FIGHT;
	SetMusic();
	return TRUE;
}

void cTaskZhuaGui::AutoGetGost(sNpc& npc, string mapname)
{
	int modeid;
	switch (rand() % 5)
	{
	case 0:modeid = 26; break;//Ѫ��
	case 1:modeid = 28; break;//����
	case 2:modeid = 10; break;//����
	case 3:modeid = 27; break;//ţͷ
	default: modeid = 18; break;//Ұ��
	}
	int x, y;
	g_pMainState->m_Map.AutoGetPos(x, y, mapname);
	npc.Set(AutoMakeName(), mapname, modeid, x, y,npc.m_JinJieID.GetBJinJie());
}

void cTaskZhuaGui::GetErrPos(sNpc npc, int& xout, int& yout)
{
	int direction = rand() % 9;
	if (direction == 5)direction +=rand()%4;
	//�������־���λ��
	xout = npc.m_x;
	yout = npc.m_y;
	int offset2 =  40;
	int offset =  40;
	switch (direction)
	{
	case 0:
		yout = npc.m_y - rand() % offset2;
		break;
	case 1:
		xout = npc.m_x + rand() % (offset);
		yout = npc.m_y - rand() % offset2;
		break;
	case 2:
		xout = npc.m_x + rand() % offset2;
		yout = npc.m_y - rand() % (offset);
		break;
	case 3:
		xout = npc.m_x + rand() % offset2;
		break;
	case 4:
		xout = npc.m_x + rand() % offset2;
		yout = npc.m_y + rand() % (offset);
		break;
	case 5:
		xout = npc.m_x + rand() % (offset);
		yout = npc.m_y + rand() % offset2;
		break;
	case 6:
		yout = npc.m_y + rand() % offset2;
		break;
	case 7:
		xout = npc.m_x - rand() % (offset);
		yout = npc.m_y + rand() % offset2;
		break;
	case 8:
		xout = npc.m_x - rand() % offset2;
		yout = npc.m_y + rand() % (offset);
		break;
	case 9:
		xout = npc.m_x - rand() % offset2;
		break;
	case 10:
		xout = npc.m_x - rand() % offset2;
		yout = npc.m_y - rand() % (offset);
		break;
	case 11:
		xout = npc.m_x - rand() % (offset);
		yout = npc.m_y - rand() % offset2;
		break;
	default:
		ERRBOX;
		break;
	}
	if (xout < 0)xout = 0;
	if (yout < 0)yout = 0;
	if ((xout - npc.m_x)>40)xout = npc.m_x+40;
	if ((npc.m_x - xout)>40)xout = npc.m_x - 40;
	if ((xout - npc.m_y) > 40)yout = npc.m_y + 40;
	if ((npc.m_y - xout) > 40)yout = npc.m_y - 40;
}

void cTaskZhuaGui::Save(ofstream& File)
{
	File << m_HuanShu << " ";
	//�Ƿ���ץ������
	m_Ghost.Save(File);
	//�Ƿ��о���
	if (m_pTask)
	{
		File << 1 << " ";
		File << m_pTask->GetType()<<" ";
		File << m_RetNum<<" ";
		File << m_RetStage << " ";
	}
	else File << 0 << " ";
}

void cTaskZhuaGui::Load(ifstream& File)
{
	File >> m_HuanShu;
	m_Ghost.Load(File); 
	int k;
	File >> k;
	if (k)
	{
		File >> k ;
		m_pTask = g_pMainState->FindTaskByID(k);
		File >> m_RetNum;
		File >> m_RetStage;
	}
}

void cTaskZhuaGui::SetTrigger(cTaskFather* p, int num, int stage)
{
	m_pTask = p;
	m_RetNum = num;
	m_RetStage = stage;
}

void cTaskZhuaGui::UpdateZGJob(BOOL bTrue)
{
	ostringstream oss;
	//��ΪҪ���������ɫ��ͬ,ֻ������һ��
	int falsex, falsey;
	if (bTrue)
	{
		falsex = m_Ghost.m_x;
		falsey = m_Ghost.m_y;
	}
	else
	{
		GetErrPos(m_Ghost, falsex, falsey);
	}
	oss << "��һֻ#R";
	oss << m_Ghost.m_name;
	oss << "#R�ӳ��˵ظ�,���ҹ���,��Ӧ����#R";
	oss << m_Ghost.m_mapname;
	oss << "#R#B";
	oss << falsex;
	oss << " ";
	oss << falsey;
	oss << "#B ����,������ȥץ������";


	sJob job;
	job.m_Context = oss.str();
	job.m_Title = "ץ��";
	job.m_Time = 40;
	job.m_pTask = this;
	g_pMainState->m_Job.AddaJob(job, m_HuanShu);
}


BOOL cTaskBangPai::Process(int stage)
{
	return TRUE;
}


BOOL cTaskSM::Process(int stage)
{
	switch (stage)
	{
	//case 21://ʾ��
	//	
	//	{
	//		if (g_pMainState->m_Map.m_MapName == m_Npc.m_mapname)
	//		{
	//			g_pMainState->m_Map.RemoveNpc(m_Npc.m_x, m_Npc.m_y, m_Npc.m_name, 1);
	//		}
	//		Process(20);
	//	}
	//	break;
	//case 1000://������ʧ
	//	/*g_pMainState->m_TriggerObjMap.ReMove(this);
	//	g_pMainState->m_Job.ReMoveJob(this);
	//	g_pMainState->m_TriggerObj.ReMove(this);
	//	g_pMainState->m_Job.ReMoveJob(this);*/
	//	break;
	//case 11:
	//		/*GetJiangLi();
	//		GetShiFu()->Talk("���Ĳ���,����Ŭ��");*/
	//		return TRUE;
	//case 20://����ʧ��
	//	
	//	{
	//		//Process(1000);
	//		//int money = m_HuanShu * 50;
	//		//g_pCharacter->m_PCData.AddMoney(-money, 0,0);
	//		//g_pMainState->m_Tags.GetMoneyAndExp(-money);//��ʧ
	//	}
	//	break;
	//case 4://ȡ������
	//	
	//	{
	//		/*Process(11);
	//		return TRUE;*/
	//		//
	//	/*	if (g_pMainState->m_Dialog.m_retSelect)
	//			return TRUE;
	//		m_pNowObj->Talk("��������Ѿ�ȡ����");
	//		Process(20);	*/
	//	}
	//	break;
	//case 19:
	//	
	//	//{
	//	//	sZiZhi zizhi;
	//	//	zizhi.Set(1000, 1000, 1500, 4000, 1000, 1000, 100);
	//	//	g_pMainState->AddPCEnemy(m_Npc.m_name, m_Npc.m_modeid, g_pCharacter->m_PCData.m_LV,
	//	//		0, &zizhi, (eMengPai)m_EnemyMengPai, &m_ShiWeiWeapon);

	//	//	g_pMainState->m_FightList[10].isLive = TRUE;
	//	////	cObj* pObj = g_pMainState->m_FightList[10].m_pData;
	//	//	zizhi.Set(1000, 1000, 1500, 4000, 1000, 1000, 100);
	//	//	g_pMainState->AddPC(m_YuanJiu.m_name, m_YuanJiu.m_modeid, g_pCharacter->m_PCData.m_LV,
	//	//		0, &zizhi, (eMengPai)g_pCharacter->m_PCData.m_MengPaiID);
	//	//

	//	//	g_StateType = STATE_FIGHT;
	//	//	AddTriggerFightSuccess(17);
	//	//}
	//	break;

	//case 17:
	//	
	//	/*{
	//		cObj* pObj = g_pMainState->FindObjByName(m_Npc.m_name);
	//		pObj->Talk("�Ҵ���,��ȥ������ʦ����,����Ҳ������");
	//		g_pMainState->m_Map.RemoveNpc(m_Npc.m_name);
	//		GetJiangLi();
	//	
	//	}*/
	//	break;
	//case 16://Ѳ�߹�
	//	
	//	/*{
	//		g_pMainState->m_FightList[10].isLive = TRUE;
	//		cObj* pObj = g_pMainState->m_FightList[10].m_pData;
	//		sZiZhi zizhi;
	//		zizhi.Set(1000, 1000, 1500, 4000, 1000, 1000, 100);
	//		pObj->AutoPCData(m_Npc.m_name, m_Npc.m_modeid, 0, &zizhi, (eMengPai)m_EnemyMengPai, &m_ShiWeiWeapon);
	//		g_pMainState->FollowLv(10, g_pCharacter->m_PCData.m_LV);
	//		g_pMainState->m_FightList[11].isLive = TRUE;
	//		pObj = g_pMainState->m_FightList[11].m_pData;
	//		pObj->AutoPetData("����", rand() % 70, 10, 0, g_pCharacter->m_PCData.m_LV);
	//	
	//		
	//		g_StateType = STATE_FIGHT;
	//		AddTriggerFightSuccess(17);
	//	}*/
	//	break;
	//case 15:
	//	/*if (0 == g_pMainState->m_Map.m_MapName.compare(m_Npc.m_mapname))
	//	{
	//		g_pMainState->m_Map.AddNpc(&m_Npc, this, "��", 16);
	//		cObj* target = g_pMainState->m_Map.m_pNpc[g_pMainState->m_Map.m_numberOfNpc - 1];
	//		cCharacter* pc= (cCharacter*)target->m_pIndex;
	//		pc->m_PCData.m_Equip[2] = m_ShiWeiWeapon;
	//		target->m_NowPos = -1;
	//		target->Set(POS_STAND);
	//	}*/
	//	break;
	case 0:
		if (!g_pMainState->m_pNowSelect)return TRUE;
		m_pNowObj = g_pMainState->m_pNowSelect;
		if (-1 == g_pCharacter->m_PCData.m_MengPaiID)
		{
			m_pNowObj->Talk("�Ƿ�Ҫ��ʦ?", this, 1, "��", "��");
		}
		else
		{
			if (g_pMainState->m_SkillManager.m_MengPaiManager.m_MengPai[g_pCharacter->m_PCData.m_MengPaiID].m_ShiFuName== g_pMainState->m_pNowSelect->m_TrueName.GetString())
			{
				CheckInit(g_pCharacter->m_PCData.m_MengPaiID);
			/*	ostringstream oss;
				oss << "ʦ������(��ʣ";
				oss << g_pCharacter->m_PCData.m_LV - m_HuanShu;
				oss << "��)";*/
				m_pNowObj->Talk("��ѡ��Ҫ������",this,3, "ѧϰ����");
			}
			else
			{
				m_pNowObj->Talk("�Ǳ��ŵ���,��Ҫ�ڴ˴�����");
			}
		}
		break;
	case 1://��ʦ�ɹ�
		
		{
			if (0==g_pMainState->m_Dialog.m_retSelect)
			{
				int mengpaiid=-1;
				for (int i = 0; i < 12; i++)
				{
					if (g_pMainState->m_Dialog.m_ContextList.m_Name.GetString() == g_pMainState->m_SkillManager.m_MengPaiManager.m_MengPai[i].m_ShiFuName)
					{
						mengpaiid = i;
						break;
					}
				}
				if (-1 == mengpaiid)ERRBOX;
				g_pCharacter->m_PCData.m_MengPaiID = mengpaiid;
				g_pMainState->m_Tags.Add("��ϲ��ɹ���ʦ");
				string text="������ɫ:";
				switch (mengpaiid)
				{
				case eMP_���ƹٸ�:
					g_pCharacter->m_PCData.m_Spurting.d1 += 20;
					text += "�����˺�+20%";
					break;
				case eMP_������:
					g_pCharacter->m_PCData.m_CureCriRate.d1+=20;
					text += "������20%���ʼӱ�";
					break;
				case eMP_����ɽ:
					g_pCharacter->m_PCData.ghostSpecial.d1 += 30;
					text += "�Թ��˺�+30%";
					break;
				case eMP_Ů����:
				case eMP_��ׯ��:
					g_pCharacter->m_PCData.m_SPD.AddDataAddPercent(10);
					text += "�ٶ�+10%";
					break;
				case eMP_ʨ����:
					g_pCharacter->m_PCData.m_HPMax.AddDataAddPercent(20);
					text += "��Ѫ+20%";
					break;
				case eMP_ħ��կ:
					g_pCharacter->m_PCData.m_MagicCriRate.d1 += 20;
					text += "20%���ʷ���";
					break;
				case eMP_��˿��:
					g_pCharacter->m_PCData.m_HiddenWeaponDamagePercent.d2+= 50;
					text += "��������+50%";
					break;
				case eMP_���ܵظ�:
					g_pCharacter->m_PCData.m_DamageAddPercentMagic2.d1 += 20;
					text += "�̶��˺�+20%";
					break;
				case eMP_��������:
					g_pCharacter->m_PCData.m_PierceMagic.d1 += 15;
					text += "�������ӶԷ�15%������";
					break;
				case eMP_����ɽ:
					g_pCharacter->m_PCData.m_DamageAddPercentCure.d1 += 20;
					text += "������+20%";
					break;
				case eMP_�칬:
					g_pCharacter->m_PCData.m_HitRate.d2 += 15;
					text += "�����+15%";
					break;
				}
				g_pMainState->m_Channel.AddNewString(text);
			}
		}
		break;
	case 2:
		
		//{
		//	
		//	if (g_pMainState->m_Job.CheckHaveJob(m_type))
		//	{
		//		m_pNowObj->Talk("���Ѿ���������,��Ҫȡ����?ȡ���������ʧ�ȼ�*50��Ǯ",this,4, "�Ҳ�����");
		//		return TRUE;
		//	}
		//	m_bHaveTask = TRUE;
		//	int renqi = g_pCharacter->m_PCData.m_Popularity / 200;
		//	int  tasktype;
		//	if (m_HuanShu< 40)
		//	{
		//		if (rand() % 100 < 30 - renqi)
		//			tasktype =5;//Ѳ��
		//		else  
		//		if (rand() % 100 < 20 + renqi)
		//			tasktype = 0;//����
		//		else  if (rand() % 100 < 15 + renqi)tasktype = 1;//Ҫ0��װ��
		//		else  if (rand() % 100 < 15 + renqi)tasktype = 3;//Ҫ����
		//		else  if (rand() % 100 < 15 + renqi)tasktype= 18;//Ҫ��ֽ��
		//		else  tasktype = 2; //Ҫ����ҩƷ
		//	}else
		//	if (m_HuanShu < 70)
		//	{	
		//		if (rand() % 100 < 30 - renqi)tasktype = 5;//Ѳ��
		//		else  if (rand() % 100 < 30 + renqi)tasktype = 4;//Ҫ10-30��װ��
		//		else  if (rand() % 100 < 20 + renqi)tasktype = 8;//Ҫ�ٻ���
		//		else  if (rand() % 100 < 20 + renqi)tasktype = 18;//Ҫ��ֽ��
		//		else  tasktype = 2; //Ҫ����ҩƷ
		//	}
		//	else
		//	if (m_HuanShu <100)
		//	{
		//		if (rand() % 100 < 20 - renqi)tasktype = 10;//
		//		else  if (rand() % 100 < 20 - renqi)tasktype = 19;//Ҫ40-50��
		//		else  if (rand() % 100 < 20 + renqi)tasktype = 8;//Ҫ�ٻ���
		//		else  if (rand() % 100 < 20 - renqi)tasktype = 17;//Ҫ�� ,��
		//		else  if (rand() % 100 < 20 + renqi)tasktype=11;//Ԯ��
		//		else  tasktype = 14;//Ҫ����ҩ
		//
		//	}
		//	else
		//	if (m_HuanShu < 130)
		//	{
		//		if (rand() % 100 < 20 - renqi)tasktype = 10;//
		//		else  if (rand() % 100 < 2 - renqi)tasktype = 16;//Ҫ60-70��
		//		else  if (rand() % 100 < 20 + renqi)tasktype = 8;//Ҫ�ٻ���
		//		else  if (rand() % 100 < 20 - renqi)tasktype = 17;//Ҫ�� ,��
		//		else  if (rand() % 100 < 20 + renqi)tasktype = 11;//Ԯ��
		//	//	else  if (rand() % 100 < 15 - renqi)tasktype = 7;//����
		//		else  tasktype = 14;//Ҫ����ҩ
		//	}
		//	else
		//	{
		//		if (rand() % 100 < 10 - renqi)tasktype = 10;//
		//		else  if (rand() % 100 < 5 - renqi)tasktype = 16;//Ҫ60-70��
		//		else  if (rand() % 100 < 15 + renqi)tasktype = 8;//Ҫ�ٻ���
		//		else  if (rand() % 100 < 10 - renqi)tasktype = 17;//Ҫ�� ,��
		//		else  if (rand() % 100 < 30 + renqi)tasktype = 11;//Ԯ��
		//		else  if (rand() % 100 < 10 - renqi)tasktype = 13;//Ҫ2���Ҿ�
		////		else  if (rand() % 100 < 15 - renqi)tasktype = 7;//����
		//		else  if (rand() % 100 < 10 - renqi)tasktype = 9;//Ҫ����
		//		else  tasktype = 14;//Ҫ����ҩ
		//	}
		//	AddTask(tasktype);
		//}
		break;
	case 3:
		
		{
			switch (g_pMainState->m_Dialog.m_retSelect)
			{
			case 0:
				
				/*if (g_pCharacter->m_PCData.m_LV > 19)
				{
					if (m_HuanShu < g_pCharacter->m_PCData.m_LV)
					{
						return Process(2); 
					}
					else
					{
						
						g_pMainState->m_Dialog.m_pObj->Talk("���Ѿ���Ŭ����,����ʱ��������ʦ�Ű�");
						return TRUE;
					}
				}
				else
				{
					g_pMainState->m_Dialog.m_pObj->Talk("��ĵȼ�̫��,��20������");
					return TRUE;
				}*/
			//	break;
			//case 1://ѧϰ����
				g_pMainState->m_LearnSkill.OnOff(TRUE);
				break;
			//case 2://�콱
			//	if (g_pCharacter->m_PCData.m_MengPaiPoint < 120)
			//	{
			//		m_pNowObj->Talk("������ɹ��׶�̫��,��20������");
			//	}
			//	else
			//	{
			//		g_pCharacter->m_PCData.m_MengPaiPoint -=120;
			//		SetJiangLiItem(31, 24);
			//	}
			//	break;
			default://�ر�
				break;
			}
		}
		break;
	
	//case 5: //ʦ��Ѳ��
	//
	//	{
	//			g_pMainState->m_InterfaceDoor.SetTrigger(this, 6, 600);
	//			g_pMainState->m_Map.m_MapType = 1;
	//			m_SuccessNum = 0;
	//	}
	//	return TRUE;
	//case 6:
	//	
	//	{
	//		cPetData* pPet;
	//		int petid = rand() % 50;
	//		m_enemyname = "���ҵ�";
	//		sPetData* psPet = g_pMainState->m_PetDataManager.GetPetData(petid);
	//		m_enemyname += psPet->m_Name;
	//		pPet = AddPCPetEnemyF(g_pCharacter->m_PCData.m_LV * 40, (ePetID)petid, g_pCharacter->m_PCData.m_LV-20, eMP_���, m_enemyname);
	//		m_enemyface = psPet->m_dialoghead[0];
	//		AddPetEnemyF(g_pCharacter->m_PCData.m_LV * 35, (ePetID)(rand() % 50), g_pCharacter->m_PCData.m_LV - 20, "����");
	//		SetMusic();
	//		g_StateType = STATE_FIGHT;
	//		AddTriggerFightSuccess(7);
	//	}
	//	break;
	//case 7:
	//
	//	{
	//		string text;
	//		m_SuccessNum += 1;
	//		if (m_SuccessNum == 2)
	//		{
	//			GetJiangLi();
	//			text = "��ȥ������ʦ����,����Ҳ������";
	//			g_pMainState->m_InterfaceDoor.SetTrigger(0, 0);
	//		}
	//		else
	//		{
	//			text = "����.��������ȥ";	

	//		}
	//		g_pMainState->m_Dialog.Reset();
	//		g_pMainState->m_Dialog.m_ContextList.m_Name.SetString(m_enemyname);
	//		g_pMainState->m_Dialog.SetHead(m_enemyface);
	//		g_pMainState->m_Dialog.SetString(text);
	//		g_pMainState->m_Dialog.OnOff(TRUE);
	//	}
	//	break;
	//case 8:	
	//	if (0 == g_pMainState->m_Map.m_MapName.compare(m_Npc.m_mapname))
	//	{
	//		g_pMainState->m_Map.AddNpc(&m_Npc, this, "��", 9);
	//	}
	//	break;
	//case 9:
	//
	//	{
	//		g_pMainState->AddPCEnemy(m_Npc.m_name, 0, g_pCharacter->m_PCData.m_LV, 0, 0, (eMengPai)(rand() % 12),0, m_Npc.m_modeid);
	//		
	//		g_StateType = STATE_FIGHT;
	//		m_bXiangFuSuccess = FALSE;
	//		AddTriggerFightSuccess(10);
	//	}
	//	break;
	//case 10:
	//
	//	{
	//		g_pMainState->m_Map.RemoveNpc(m_Npc.m_name);
	//		if (m_bXiangFuSuccess)
	//		{
	//			g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->Talk("ץ����,��ȥ����ʦ����");
	//			
	//			sJob job;
	//			job.m_Context = "��ȥ��Ǭ������ʦ��";
	//			job.m_Title = "����";
	//			job.m_Time = 40;
	//			job.m_pTask = this;
	//			g_pMainState->m_Job.ReMoveJob(this);
	//			g_pMainState->m_Job.AddaJob(job);
	//			g_pMainState->m_TriggerObjMap.ReMove(this);
	//			string sfname = g_pMainState->m_SkillManager.m_MengPaiManager.GetMengPai(g_pCharacter->m_PCData.m_MengPaiID)->m_Name;
	//			g_pMainState->m_TriggerGive.AddTask(sfname, 11, 28, this, 11);
	//		}
	//		else
	//		{
	//			g_pMainState->m_Map.AutoGetPos(m_Npc.m_x, m_Npc.m_y, m_Npc.m_mapname);
	//			m_Npc.m_name += g_pMainState->m_TaskBaoTu.AutoMakeName();
	//			ostringstream oss;
	//			oss << "�и�#R";
	//			oss << m_Npc.m_name;
	//			oss << "#R�ڽ�����Ϊ������,�ֱ�Ϊʦ����,��˵����#R";
	//			oss << m_Npc.m_mapname;
	//			oss << "#R#Y";
	//			oss << m_Npc.m_x / 20;
	//			oss << " ";
	//			oss << (g_pMainState->m_Map.m_BmpTest.GetHeight() - m_Npc.m_y) / 20;
	//			oss << "#Y����,��ȥ��Ǭ������������������";
	//
	//			sJob job;
	//			job.m_Title = "����";
	//			job.m_Time = 40;
	//			job.m_Context = oss.str();
	//			job.m_pTask = this;
	//			g_pMainState->m_Job.ReMoveJob(this);
	//			g_pMainState->m_Job.AddaJob(job);
	//			g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->Talk("��������,��ȥ������");
	//		}
	//	}
	//	break;

	//case 12:
	//
	//	{
	//		string shifuname = g_pMainState->m_SkillManager.m_MengPaiManager.m_MengPai[g_pCharacter->m_PCData.m_MengPaiID].m_ShiFuName;
	//		if (0==g_pMainState->m_pNowSelect->m_TrueName.GetString().compare(shifuname))
	//		{
	//			for (int i = g_pCharacter->m_PCData.m_NumofBB - 1; i >= 0; i--)
	//			{
	//				if (g_pCharacter->m_PCData.m_pPetDataList[i]->GetData()->m_ID == m_TargetBBid)
	//				{
	//					g_pCharacter->m_PCData.DeletePet(i);
	//					GetJiangLi();
	//					string targetname = g_pMainState->m_pNowSelect->m_TrueName.GetString();
	//					g_pMainState->FindObjByName(targetname)->Talk("���Ĳ���,����Ŭ��");
	//					return TRUE;
	//				}
	//				
	//			}
	//		}
	//	}
	//	return FALSE;
	case 13:
		
		{
			sPetData* pPetdata= g_pMainState->m_PetDataManager.GetPetData(m_TargetBBid);
			int moneyneed = pPetdata->m_LVLimit * 200+5500;
			cObj* pre = g_pMainState->m_Dialog.m_pObj;
			g_pMainState->m_Dialog.Reset();
			g_pMainState->m_Dialog.SetObj(pre);
			g_pMainState->m_Dialog.SetTrigger(this, 14,14,14,14,14);
			ostringstream oss;
			oss << "����#R";
			oss << pPetdata->m_Name;
			oss << "#R��Ҫ#Y";
			oss << moneyneed;
			oss << "#Y��,ȷ��Ҫ����?";
			g_pMainState->m_Dialog.SetString(oss.str(), "����");
			g_pMainState->m_Dialog.OnOff(TRUE);
		}
		break;
	case 14:
		
		{
			if (g_pMainState->m_Dialog.m_retSelect == 0)
			{
			
				sPetData* pPetdata = g_pMainState->m_PetDataManager.GetPetData(m_TargetBBid);
				int moneyneed = pPetdata->m_LVLimit * 200 + 5500;
				if (g_pCharacter->m_PCData.m_Money[0] >= moneyneed)
				{
					cPet* pet = new cPet;
					pet->AutoPetData(m_TargetBBid, 1, 0);
					pet->m_PetDataZZ.LeveUp(15, FALSE);
					g_pCharacter->m_PCData.GetPet(pet);
					pet = 0;
					ostringstream oss;
					oss << "����";
					oss << pPetdata->m_Name;
					oss << "����";
					oss << moneyneed;
					oss << "��";
					g_pMainState->m_Tags.Add(oss.str());
					g_pCharacter->m_PCData.m_Money[0] -= moneyneed;
				}
				else
				{
					g_pMainState->m_Tags.Add("Ǯ����");
				}
			}
		}
		break;
	//case 100://�͵���
	//	
	//	{
	//		string name = g_pMainState->m_InterfaceGive.GetTarget()->m_TrueName.GetString();
	//		if (0 == name.compare(m_npcname))
	//		{
	//			if (g_pMainState->m_InterfaceGive.m_NumItem[0].m_num)
	//			{
	//				sItem2& item = g_pMainState->m_InterfaceGive.m_ItemGive[0];
	//				if (item.GetType()==28)
	//				if (item.GetID() == 18)
	//				{
	//				//	g_pCharacter->m_PCData.m_Item[g_pMainState->m_InterfaceGive.m_ItemGiveOwner[0]].m_Num = 0;
	//					g_pMainState->m_InterfaceGive.GetTarget()->Talk("���Ѿ��յ���,��ȥ������ʦ����");
	//					Process(1000);
	//					AddTriggerObj(GetShiFuName(),GetShiFuMap(),11);
	//					return TRUE;
	//				}
	//			}
	//			
	//		}
	//	}
	//	return FALSE;
	//case 101:
	//	
	//	{
	//		string name = g_pMainState->m_InterfaceGive.GetTarget()->m_TrueName.GetString();
	//		if (0 == name.compare(g_pMainState->m_SkillManager.m_MengPaiManager.m_MengPai[g_pCharacter->m_PCData.m_MengPaiID].m_ShiFuName))
	//		{
	//			if (g_pMainState->m_InterfaceGive.m_NumItem[0].m_num)
	//			{
	//				sItem2& item = g_pMainState->m_InterfaceGive.m_ItemGive[0];
	//				if (item.GetType() == m_ItemNeed.GetType())
	//				if (item.GetID() == m_ItemNeed.GetID())
	//				{
	//				//	g_pCharacter->m_PCData.m_Item[g_pMainState->m_InterfaceGive.m_ItemGiveOwner[0]].m_Num -= 1;
	//					GetJiangLi();
	//					
	//					g_pMainState->m_InterfaceGive.GetTarget()->Talk("���ֵĲ���,���ǽ���");
	//					return TRUE;
	//				}
	//			}

	//		}
	//	}
	//	return FALSE;
	default:
		break;
	}
	return TRUE;
}

//void cTaskSM::AddTask(int type)
//{
//	switch (type)
//	{
//		//
//	case 0://����
//		
//		{
//			//..........
//			string  npcname;
//			cObj pShiFu;
//			if (g_pCharacter->m_PCData.m_MengPaiID<0 || g_pCharacter->m_PCData.m_MengPaiID>11)
//				return;
//			npcname = m_NameList[g_pCharacter->m_PCData.m_MengPaiID][rand() % m_NameList[g_pCharacter->m_PCData.m_MengPaiID].size()];
//			if (npcname == "")
//				return;
//			m_npcname = npcname;
//			ostringstream oss;
//			oss << "������һ���#R";
//			oss << npcname;
//			oss << "#R����,�鷳���͹�ȥ";
//			DialogTalkNObj(oss.str());
//
//			UpdateJob(oss.str());
//			AddTriggerGive(npcname, 28, 18, 100);
//			SetJiangLiItem(18,28);
//		}
//		break;
//		case	1://Ҫ0��װ��
//		case	2://Ҫ����ҩƷ
//		case	3://Ҫ����
//		case	4://Ҫ10-30װ��
//		case	6://Ҫ����ҩ
//		case	9://Ҫ����
//		case	12://Ҫ1���Ҿ�
//		case	13://Ҫ2���Ҿ�
//		case	14://Ҫ����ҩ
//		case	15://Ҫ���
//		case	16://Ҫ60-70��
//		case	17://Ҫ�� ,��
//		case	18://Ҫ��ֽ��
//		case	19://Ҫ40-50��
//		
//			{
//					   int	itemtype = -1;
//					   int	itemid = -1;
//
//				switch (type)
//				{
//				case 1:
//					itemtype = rand() % 23;
//					if (itemtype == 13)itemtype = rand() % 13;
//					itemid = 0;
//					break;
//				case 2:
//					itemtype = 22;
//					switch (rand() % 6)
//					{
//					case 0:itemid = 1; break;
//					case 1:itemid = 2; break;
//					case 2:itemid = 19; break;
//					case 3:itemid = 20; break;
//					case 4:itemid = 26; break;
//					default:itemid = 34; break;
//					}
//					break;
//				case 3:
//					itemtype = 23;
//					itemid = 0;
//					break;
//				case 4:
//					itemtype = rand() % 23;
//					if (itemtype == 13)itemtype = rand() % 13;
//					itemid = g_pMainState->m_ItemManage.GetIDByLv(itemtype, (rand() % 3 + 1) *10);
//					break;
//				case	6://Ҫ����ҩ
//					itemtype = 22;
//					switch (rand() % 6)
//					{
//					case 0:itemid = 3; break;
//					case 1:itemid = 4; break;
//					case 2:itemid = 21; break;
//					case 3:itemid = 22; break;
//					case 4:itemid = 26; break;
//					default:itemid = 35; break;
//					}
//					break;
//				case	9://Ҫ��ʯ
//					itemtype = 29;
//					switch (rand() % 6)
//					{
//					case 0:itemid = 0; break;
//					case 1:itemid = 8; break;
//					case 2:itemid = 14; break;
//					case 3:itemid = 6; break;
//					case 4:itemid = 5; break;
//					default:itemid = 4; break;
//					}
//					break;
//				case	12://Ҫ1���Ҿ�
//					itemtype = 35;
//					itemid = rand() % 15;
//					break;
//				case	13://Ҫ2���Ҿ�
//					itemtype = 36;
//					itemid = rand() % 15;
//					break;
//				case	14://Ҫ�߼�ҩ
//					itemtype = 22;
//					switch (rand() % 6)
//					{
//					case 0:itemid = 7; break;
//					case 1:itemid = 8; break;
//					case 2:itemid = 23; break;
//					case 3:itemid = 24; break;
//					case 4:itemid = 31; break;
//					default:itemid = 36; break;
//					}
//					break;
//				case	15://Ҫ���
//					itemtype = 23;
//					switch (rand() % 6)
//					{
//					case 0:itemid = 1; break;
//					case 1:itemid = 2; break;
//					case 2:itemid = 3; break;
//					case 3:itemid = 5; break;
//					case 4:itemid = 6; break;
//					default:itemid = 7; break;
//					}
//					break;
//				case	16://Ҫ60-70��
//					itemtype = rand() % 23;
//					if (itemtype == 13)itemtype = rand() % 13;
//					itemid = g_pMainState->m_ItemManage.GetIDByLv(itemtype, (rand() % 2 + 6) * 10);
//					break;
//				case	17://Ҫ�� ,��
//					if (rand() % 2)
//					{
//						itemtype = 30;
//						itemid = rand() % 2;
//					}
//					else
//					{
//						itemtype = 39;
//						itemid = rand() % 6;
//					}
//					break;
//				case	18://Ҫ��ֽ��
//					itemtype = 27;
//					switch (rand() % 3)
//					{
//					case 0:itemid = 52; break;
//					case 1:itemid = 53; break;
//					case 2:itemid = 54; break;
//					}
//					break;
//				case	19://Ҫ40-50��
//					itemtype = rand() % 23;
//					if (itemtype == 13)itemtype = rand() % 13;
//					itemid = g_pMainState->m_ItemManage.GetIDByLv(itemtype, (rand() % 2 + 4) * 10);
//					break;
//				default:
//					return ;
//				}	
//				m_ItemNeed.Set(itemtype, itemid);
//
//				ostringstream oss;
//				oss<< "ȥ��Ϊʦ���#R";
//				oss<< m_ItemNeed.GetName();
//				oss << "#R����";
//				UpdateJob(oss.str());
//				AddTriggerGive(g_pMainState->m_Dialog.m_pObj->m_TrueName.GetString(), itemtype, itemid, 101, 1);
//				DialogTalkNObj(oss.str()); 
//			}
//			break;
//		case	5://Ѳ��
//			
//			{
//				DialogTalkNObj("������Щ����֮ͽ�����ɵ���,��������ȥ�����Ǿ͵�նɱ");
//				UpdateJob("������Ѳ��");
//				m_mapname = g_pMainState->m_SkillManager.m_MengPaiManager.m_MengPai[g_pCharacter->m_PCData.m_MengPaiID].m_XunLuoMapName;
//				AddTriggerObjMap(m_mapname,5 );
//			}
//			break;
//		case	7://����
//			
//			{
//				if (!m_bInitXiangFu)InitXiangFu();
//
//				cObj* pPreObj = g_pMainState->m_Dialog.m_pObj;
//				g_pMainState->m_Dialog.Reset();
//				g_pMainState->m_Dialog.SetObj(pPreObj);
//				g_pMainState->m_Dialog.m_pObj = 0;
//
//				m_Npc.m_name = m_XiangFuNameList[rand() % m_XiangFuNameList.size()];
//				m_Npc.m_mapname = m_XiangFuDiDianList[rand() % m_XiangFuDiDianList.size()];
//				g_pMainState->m_Map.AutoGetPos(m_Npc.m_x, m_Npc.m_y, m_Npc.m_mapname);
//
//				ostringstream oss;
//				oss << "�и�#R";
//				oss << m_Npc.m_name;
//				oss << "#R�ڽ�����Ϊ������,�ֱ�Ϊʦ����,��˵����#R";
//				oss << m_Npc.m_mapname;
//				oss << "#R#Y";
//				oss << m_Npc.m_x / 20;
//				oss << " ";
//				oss << (g_pMainState->m_Map.m_BmpTest.GetHeight() - m_Npc.m_y) / 20;
//				oss << "#Y����,��ȥ��Ǭ������������������";
//				DialogTalkNObj(oss.str());
//				UpdateJob(oss.str());
//				m_Npc.m_modeid = rand() % 40 + g_pCharacter->m_PCData.m_LV / 5;
//				AddTriggerObjMap(&m_Npc,8);
//			}
//			break;
//		case	8://Ҫ�ٻ���
//			
//			{
//				m_TargetBBid = rand() % 20 + g_pCharacter->m_PCData.m_LV / 5;
//				string bbname= g_pMainState->m_PetDataManager.GetPetData(m_TargetBBid)->m_Name;
//				ostringstream oss;
//					oss<< "��Ϊʦץֻ#R";
//				oss<< bbname;
//				oss<<"#R����";
//				DialogTalkNObj(oss.str());
//				cMengPai& mp = g_pMainState->m_SkillManager.m_MengPaiManager.m_MengPai[g_pCharacter->m_PCData.m_MengPaiID];
//				m_Npc.m_name = mp.m_ShiFuName;
//				m_Npc.m_mapname = mp.m_ShiFuMap;
//				AddTriggerObj(m_Npc.m_name, m_Npc.m_mapname,12);
//				UpdateJob(oss.str());
//			}
//			break;
//		case	10://ʾ��
//			
//			{
//				if (!m_bInitShiWei)InitShiWei();
//				m_EnemyMengPai = rand() % 12;
//				m_Npc.m_mapname = g_pMainState->m_SkillManager.m_MengPaiManager.m_MengPai[m_EnemyMengPai].m_XunLuoMapName;
//				m_Npc.m_name = m_ShiWeiNameList[m_EnemyMengPai][rand() % m_ShiWeiNameList[m_EnemyMengPai].size()];
//				m_Npc.m_modeid = m_ShiWeiIdList[m_EnemyMengPai][rand() % m_ShiWeiIdList[m_EnemyMengPai].size()];
//				g_pMainState->m_Map.AutoGetPos(m_Npc.m_x, m_Npc.m_y, m_Npc.m_mapname);
//				ostringstream oss;
//				oss << m_Npc.m_mapname;
//				oss << "��#R";
//				oss << m_Npc.m_name;
//				oss << "#R���Ϯ�����ŵ���,�ֶ���#R";
//				oss << m_Npc.m_mapname;
//				oss << m_Npc.m_x/20;
//				oss << "  ";
//				oss << (g_pMainState->m_Map.m_BmpTest.GetHeight() - m_Npc.m_y) / 20;
//				oss << "#R,��ȥ��ta����������";
//				string text = oss.str();
//				DialogTalkNObj(oss.str());
//				g_pMainState->m_TriggerObjMap.AddTask(this, 15,m_Npc);
//
//				int weapontype[2];
//				GetWeaponType(m_Npc.m_modeid, weapontype[0], weapontype[1]);
//				int typenum=2;
//				if (weapontype[1] == -1)typenum = 1;
//				int weaponid1,weapontype1;
//				weapontype1 =  weapontype[rand() % typenum];
//				weaponid1 =  g_pMainState->m_ItemManage.GetIDByLv(weapontype1, (rand() % 6 + 9) * 10);
//				m_ShiWeiWeapon.Set(weapontype1,weaponid1);
//				m_ShiWeiWeapon.m_Num = 1;
//				UpdateJob(oss.str());
//			}
//			break;
//		case	11://Ԯ��
//			
//			{
//				if (!m_bInitShiWei)InitShiWei();
//				if (!m_bInitXiangFu)InitXiangFu();
//				m_EnemyMengPai = rand() % 12;
//				m_Npc.m_mapname = m_XiangFuDiDianList[rand()%m_XiangFuDiDianList.size()];
//				m_Npc.m_name = m_ShiWeiNameList[m_EnemyMengPai][rand() % m_ShiWeiNameList[m_EnemyMengPai].size()];
//				m_Npc.m_modeid = m_ShiWeiIdList[m_EnemyMengPai][rand() % m_ShiWeiIdList[m_EnemyMengPai].size()];
//				g_pMainState->m_Map.AutoGetPos(m_Npc.m_x, m_Npc.m_y, m_Npc.m_mapname);
//				ostringstream oss;
//				oss << "#R";
//				oss << m_Npc.m_name;
//				oss << "#R����#R";
//				oss << m_Npc.m_mapname;
//				oss << m_Npc.m_x / 20;
//				oss << "  ";
//				oss << (g_pMainState->m_Map.m_BmpTest.GetHeight() - m_Npc.m_y) / 20;
//				oss << "#R,Ϯ�����ŵ���,���ȥԮ��";
//				string text = oss.str();
//				DialogTalkNObj(text);
//				m_YuanJiu.m_name = "��ս�е�ͬ��";
//				m_YuanJiu.m_x = m_Npc.m_x;
//				m_YuanJiu.m_y = m_Npc.m_y;
//				m_YuanJiu.m_direction = m_Npc.m_direction;
//				m_YuanJiu.m_modeid = rand() % 14;;
//	
//				g_pMainState->m_TriggerObjMap.AddTask(this, 19, m_YuanJiu);
//				int weapontype[2];
//				GetWeaponType(m_Npc.m_modeid, weapontype[0], weapontype[1]);
//				int typenum = 2;
//				if (weapontype[1] == -1)typenum = 1;
//				int weaponid1, weapontype1;
//				weapontype1 = weapontype[rand() % typenum];
//				weaponid1 = g_pMainState->m_ItemManage.GetIDByLv(weapontype1, (rand() % 6 + 9) * 10);
//				m_ShiWeiWeapon.Set(weapontype1, weaponid1);
//				m_ShiWeiWeapon.m_Num = 1;
//				UpdateJob(text);
//			}
//			return;
//	}
//}

void cTaskSM::GetJiangLi()
{
	int lv = m_HuanShu+20;
	int exp,money;
	if (lv < 40)
	{
		exp = (1500 - lv * 12);
		money = (700 - lv * 9);
	}
	if (lv < 70)
	{
		exp = (2100 - lv * 12);
		money = (1400 - lv * 13);
	}
	if (lv < 100)
	{
		exp = (2700 - lv * 11);
		money = (1700 - lv * 10);
	}
	if (lv < 130)
	{
		exp = (3300 - lv * 11);
		money = (2000 - lv * 11);
	}
	else
	{
		exp = (3900 - lv * 8);
		money = (2300 - lv * 8);
	}
	exp = (exp*15*lv) / 100;
	money = (money *15*lv) / 100;
	SetJiangLi2(exp,money);
	m_HuanShu += 1;
	g_pCharacter->m_PCData.m_MengPaiPoint += 1;
	m_bHaveTask = FALSE;
	Process(1000);
}

void cTaskSM::CheckInit(int mengpaiid)
{
	if (!m_bInit2) Init();
	if (m_bInit[mengpaiid])return;
	//cMengPai& mengpai= g_pMainState->m_SkillManager.m_MengPaiManager.m_MengPai[mengpaiid];
	//string path;
	//path = g_strMediaPath;
	//path+="����\\ʦ��\\";
	//if (mengpaiid == 4)//BUG
	//{
	//	path += "ʨ����";
	//}
	//else
	//	path += mengpai.m_Name;
	//path += ".txt";
	//ifstream File;
	//File.open(path);
	//if (!File.is_open())ERRBOX;
	//int num;
	//File >> num;
	//string name;
	//for (int i = 0; i < num; i++)
	//{
	//	File >> name;
	//	m_NameList[mengpaiid].push_back(name);
	//}
	//File.close();
}

void cTaskSM::InitXiangFu()
{
	/*ifstream File;
	ostringstream oss;
	oss << g_strMediaPath << "����//ʦ��//ʦ�Ž���.txt";
	File.open(oss.str());
	if (!File.is_open()){ ERRBOX; return; }
	int num;
	File >> num;
	string didian;
	for (int i = 0; i < num; i++)
	{
		File >> didian;
		m_XiangFuDiDianList.push_back(didian);
	}
	File >> num;
	string name;
	for (int i = 0; i < num; i++)
	{
		File >> name;
		m_XiangFuNameList.push_back(name);
	}
	File.close();*/
}

cObj* cTaskSM::GetShiFu()
{
	return g_pMainState->FindObjByName(GetShiFuName());
}

std::string cTaskSM::GetShiFuName()
{
	if (-1 == g_pCharacter->m_PCData.m_MengPaiID)return "";
	return g_pMainState->m_SkillManager.m_MengPaiManager.GetMengPai(g_pCharacter->m_PCData.m_MengPaiID)->m_ShiFuName;
}
std::string cTaskSM::GetShiFuMap()
{
	if (-1 == g_pCharacter->m_PCData.m_MengPaiID)return "";
	return g_pMainState->m_SkillManager.m_MengPaiManager.GetMengPai(g_pCharacter->m_PCData.m_MengPaiID)->m_ShiFuMap;
}
void cTaskSM::InitShiWei()
{
	m_bInitShiWei = TRUE;
	ifstream File;
	ostringstream oss;
	oss << g_strMediaPath << "����\\ʦ��\\ʦ��ʾ��.txt";
	File.open(oss.str());
	if (!File)ERRBOX;
	string name;
	int num;
	int id;
	for (int i = 0; i < 12; i++)
	{
		File >> name;
		File >> num;
		for (int j = 0; j < num; j++)
		{
			File >> name;
			File >> id;
			m_ShiWeiIdList[i].push_back(id);
			m_ShiWeiNameList[i].push_back(name);
		}
	}
}

void cTaskSM::UpdateJob(string context)
{
	sJob job;
	job.Set("ʦ��", context,30,this,20);
	g_pMainState->m_Job.AddaJob(job,m_HuanShu);
}

void cTaskSM::Save(ofstream& File)
{
	File<< m_HuanShu<<" ";
	File << m_bHaveTask<<" ";
	if (!m_bHaveTask)return;
	File << m_bXiangFuSuccess;
	m_Npc.Save(File);
	File << m_EnemyMengPai;
	m_ShiWeiWeapon.Save(File);//ʾ��ʱNPC������
	m_YuanJiu.Save(File);
	File << m_TargetBBid;
	m_ItemNeed.Save(File);//��¼��ǰʦ��Ҫ�Ķ���
	File << m_npcname<<" "; //��¼��ǰʦ��Ҫ�ҵ���
	File << m_mapname<<" ";
	File << m_SuccessNum<<" ";
	File << m_enemyname<<" ";
	File << m_enemyface<<" ";
}

void cTaskSM::Load(ifstream& File)
{
	m_bInitShiWei = FALSE;
	for (int i = 0; i < 12; i++)
		m_bInit[i] = 0;
	File >> m_HuanShu;
	File >> m_bHaveTask;
	if (!m_bHaveTask)return;
	File >> m_bXiangFuSuccess;
	m_Npc.Load(File);
	File >> m_EnemyMengPai;
	m_ShiWeiWeapon.Load(File);//ʾ��ʱNPC������
	m_YuanJiu.Load(File);
	File >> m_TargetBBid;
	m_ItemNeed.Load(File);//��¼��ǰʦ��Ҫ�Ķ���
	File >> m_npcname ; //��¼��ǰʦ��Ҫ�ҵ���
	File >> m_mapname ;
	File >> m_SuccessNum ;
	File >> m_enemyname ;
	File >> m_enemyface ;
}


BOOL cTaskFB::Process(int stage)
{
	
	switch (m_FBID)
	{
	case 0:ProcessWj(stage);break;
	case 1:ProcessSL(stage);break;
	case 2:ProcessCC(stage); break;
	case 3:ProcessSM(stage); break;
	case 4:ProcessSJ(stage); break;
	}
	return TRUE;
}

BOOL cTaskFB::Start(int id)
{
	m_FBID = id;
	m_TextList.clear();
	ifstream File;
	ostringstream oss;
	oss <<g_strMediaPath<<"����//����//" << id << ".txt";
	File.open(oss.str());
	if (!File)ERRBOX;
	int num;
	File >> num;
	if (num)
	{
		m_TextList.resize(num);
		getline(File, m_TextList[0]);
		for (int i = 0; i < num; i++)
		{
			getline(File, m_TextList[i]);
		}
	}
	File.close();
	m_List.clear();
	m_List2.clear();
	return TRUE;
}

void cTaskFB::ProcessWj(int stage)
{
	int lv = g_pCharacter->m_PCData.m_LV;
	switch (stage)
	{
	case 0:
		
		{
			m_Stage = 0;
			g_pMainState->m_TriggerObj.AddTask(m_TextList[1], m_TextList[0], this, 1);
			g_pMainState->m_TriggerObjMap.AddTask(this, 2, m_TextList[3]);
			StageSetSameNpc(3, m_TextList[3], 3, 10, m_TextList[4]);
			UpdateJob(m_TextList[20]);
		}
		break;
	case 1:
	
		{
			g_pMainState->m_pNowSelect->Talk(m_TextList[2], this, -100, "��", "��");
		}
		break;
	case -100:
		if (g_pMainState->m_Dialog.m_retSelect)
		return ;
	case -1:
		
		{
			if (m_Stage < 10)
			{
				g_pMainState->m_Map.LoadMap(118, 90, m_TextList[3].c_str());
			}
			else
			{
				g_pMainState->m_Map.LoadMap(30, 16, m_TextList[12].c_str());
			}
		}
		break;
	case -2:
		break;
	case 2:
		
		{
			if (0 == m_Stage)
			{
				g_pMainState->m_TriggerDialog.AddTaskFB("0", 0, this, -2);
				m_Stage = 1;
			}
			AddNpc();
			ostringstream oss;
			oss << "��ʣ#R" << m_List.size() << "#R������";
			UpdateJob(oss.str(), 40);
		}
		break;
	case 3://����ս��
		
		{
			m_NowNpc = g_pMainState->m_pNowSelect;
		 	AddPCPetEnemyF(lv * 90, e����, -1, eMP_���, m_NowNpc->m_TrueName.GetString());
			int enemynum =  g_pMainState->AtutoGetEnemyNum()-1;
			for (int i = 0; i < enemynum; i++)
			{
				AddPCPetEnemyF(lv * 80, e����, -1, eMP_���, "���ľ��");
			}
			SetMusicFB();
			g_StateType = STATE_FIGHT;
			AddTriggerFightSuccess(4);
		}
		break;
	case 4://����ս��ʤ��
		
		{
			SetJiangLi(2);
			
			if (1==m_List.size())
			{
				if (1 == m_Stage)
				{
					g_pMainState->m_Map.RemoveAllNpc();
					m_List.clear();
					g_pMainState->m_TriggerDialog.AddTaskFB("0", 1, this, -5);
					m_Stage += 1;
				}
				return ;
			}
			RemoveNpc(m_NowNpc);
			ostringstream oss;
			oss << "��ʣ#R" << m_List.size() << "#R������";
			UpdateJob(oss.str());
		}
		break;
	case -5:
		
		{
			RemoveNpc(m_NowNpc);
			StageSetSameNpc(1111, m_TextList[3], 6, 15, m_TextList[6]);
			g_pMainState->m_TriggerObjMap.AddTask(this, 5, m_TextList[3]);	
			ostringstream oss;
			oss << "��ʣ#R" << m_List.size() << "#R������";
			UpdateJob(oss.str(), 40);
		}
		break;
	case 5:
		
		{
			AddNpc();
			ostringstream oss;
			oss << "��ʣ#R" << m_List.size() << "#R������";
			UpdateJob(oss.str(), 40);
		}
		break;
	case 6:
		
		{ //����һ������
			m_NowNpc = g_pMainState->m_pNowSelect;
			RemoveNpc(m_NowNpc);
			m_NowNpc = 0;
			SetJiangLi(2,300,50);
			if (!m_List.size())
			{ //������Ϊ0
				cObj obj;
				obj.SetXY(116 * 20, g_pMainState->m_Map.m_pMap->m_Height - 23 * 20);
				g_pMainState->m_FindPt.FindPath(&obj, 26 * 20, g_pMainState->m_Map.m_pMap->m_Height - 42 * 20);
				m_List.resize(15);
				int timenum = obj.m_time / m_List.size();
				for (int i = 0; i < m_List.size(); i++)
				{
					SetNpc( m_List[i],8,m_TextList[9], m_TextList[3], 15, obj.m_Path[obj.m_time].sx / 20,
						(g_pMainState->m_Map.m_pMap->m_Height - obj.m_Path[obj.m_time].sy) / 20);
					obj.m_time -= timenum;	
				}
				UpdateJob(m_TextList[10]);
				g_pMainState->m_Tags.Add(m_TextList[10]);
				//����
				m_List2.resize(1);
				SetNpc(m_List2[0],-7,m_TextList[8], m_TextList[3], 1003,18,35);
				AddTriggerObjMap(m_TextList[3],-6);
				m_Stage = 2;
				Process(-6);
				AddTriggerDialog(3,-2);
				return;
			}
			ostringstream oss;
			oss << "��ʣ#R" << m_List.size() << "#R������";
			UpdateJob(oss.str());
		}
		break;
	case -6: //��������
		
		{
			if (m_Stage == 3)
			{
				AddNpc();
			}
			else
				AddNpcOne(&m_List2[0],TRUE);
		}
		break;
	case -7://������Ի�
		
		{ 
			if (2 == m_Stage)AddTriggerDialog(2, 7);
		}
		break;
	case 7://����������
		
		{	
			m_Stage = 3;
			sJob job;
			job.Set(m_TextList[5], m_TextList[7], 40, this, 1002);
			g_pMainState->m_Job.AddaJob(job);
			Process(-6);
		}
		break;
	case 8://����ս��
	
		{ 
			m_NowNpc = g_pMainState->m_pNowSelect;
			cPetData* pet;
			pet= AddPCPetEnemyF(lv * 140, e����, lv+5, eMP_������, m_NowNpc->m_TrueName.GetString());
			pet->AddSkill(ePS_����);
			pet=AddPCPetEnemyF(lv * 140, eţ��, lv + 5, eMP_ħ��կ, "�󷨾���");
			pet->m_DamageAddPercentMagic.d2 += 50;
			AddPCPetEnemyF(lv * 140, e��ͷ��, lv + 5, eMP_���ƹٸ�, "��������");
			pet->m_DamagePercent.d2 += 50;
			SetMusicFB();
			g_StateType = STATE_FIGHT;
			AddTriggerFightSuccess(9);
		}
		break;
	case 9://��������
		
		{
			SetJiangLi(3);
			RemoveNpc(m_NowNpc);
			m_NowNpc = 0;
			if (!m_List.size())
			{
				m_List2.clear();
				m_List.clear();
				StageSetSameNpc(36, m_TextList[12], 11, 3, m_TextList[13]);
				g_pMainState->m_Channel.AddNewString(m_TextList[11]);
				m_Stage = 10;
				g_pMainState->m_TriggerObjMap.AddTask(this, 10, m_TextList[12]);
				Process(-1);
				return;
			}
			ostringstream oss;
			oss << "��ʣ#R" << m_List.size() << "#R������";
			UpdateJob(oss.str());
		}
		break;
	case 10://�������ֳ���

		{
			UpdateJob(m_TextList[11],40);
			AddNpc();
		}
		break;
	case 11://�������� ս��
	
		{
			m_NowNpc = g_pMainState->m_pNowSelect;

			m_NowNpc = g_pMainState->m_pNowSelect;
			AddPCPetEnemyF(lv * 150, e����ս��, lv+5, eMP_���, m_NowNpc->m_TrueName.GetString());
			for (int i = 0; i < 2;i++)
			AddPCPetEnemyF(lv * 90, e���ӹ�, lv+3, eMP_���, "��������");
			for (int i = 0; i < 5; i++)
			{
				AddPCPetEnemyF(lv * 100, eţͷ, -1, eMP_���, "����ʿ��");
			}
			SetMusicFB();
			g_StateType = STATE_FIGHT;
			AddTriggerFightSuccess(12);
		}
		break;
	case 12:
	
		{
			SetJiangLi(5);
			RemoveNpc(m_NowNpc);
			m_NowNpc = 0;
			if (!m_List.size())
			{ //���� ;
				vector<int>  modeid;
				modeid.resize(30);
				for (int i = 0; i < 30; i++)
					modeid[i] = i;
				StageSetSameNpc(modeid, m_TextList[12], 15, 15, m_TextList[16]);

				g_pMainState->m_TriggerObjMap.AddTask(this, 13, m_TextList[12]);
				UpdateJob(m_TextList[14], 40);
				g_pMainState->m_Channel.AddNewString(m_TextList[14]);
				return;
			}
			if (m_List.size() == 2)
			{
				UpdateJob(m_TextList[11], 5);
			}
		}
		break;
	case 13:
	
		{
			UpdateJob(m_TextList[14], 40);
			g_pMainState->m_Channel.AddNewString(m_TextList[14]);
			AddNpc();
		}
		break;
	case 15://С��ս��
	
		{
			
			m_NowNpc = g_pMainState->m_pNowSelect;

			AddPCPetEnemyF(lv * 70, (ePetID)m_NowNpc->GetModeID(), -1, eMP_���, m_NowNpc->m_TrueName.GetString());
			for (int i = 0; i < 4; i++)
			{
				AddPCPetEnemyF(lv * 60,(ePetID)(rand()%40), -1, eMP_���, m_NowNpc->m_TrueName.GetString());
			}
			SetMusicFB();
			g_StateType = STATE_FIGHT;
			AddTriggerFightSuccess(16);
		}
		break;
	case 16://С������
	
		{
			SetJiangLi(2);
			RemoveNpc(m_NowNpc);
			if (!m_List.size())
			{ //
				m_List.resize(1);
				SetNpc(m_List[0],18,m_TextList[19], m_TextList[12], 51, 64, 32, 1);
				g_pMainState->m_TriggerObjMap.AddTask(this, 17, m_TextList[12]);
				UpdateJob(m_TextList[15]);
				Process(17);
				return;
			}
			
		}
		break;
	case 17://BOSS����
	
		{
			g_pMainState->m_Tags.Add(m_TextList[15]);
			AddNpc();
		}
		break;
	case 18:
	
		{
			m_NowNpc = g_pMainState->m_pNowSelect;
			cPetData* pPet;
			AddPCPetEnemyF(lv * 160, e�������, lv+10, eMP_ʨ����, m_NowNpc->m_TrueName.GetString());
			for (int i = 0; i < 4; i++)
			{
				pPet=AddPCPetEnemyF(lv * 150, e��󡾫, lv + 5, eMP_���, "���");
				pPet->m_Combo.d1 = 60;
				pPet->comboNumUp = 6;
			}
			for (int i = 0; i < 5; i++)
			{
				pPet = AddPCPetEnemyF(lv * 140, e���꾫, lv + 5, eMP_���, "ʹŮ");
				pPet->m_Combo.d1 = 60;
				pPet->comboNumUp = 6;
			}
			SetMusicFB();
			g_StateType = STATE_FIGHT;
			AddTriggerFightSuccess(19);
		}
		break;
	case 19:

		{
			SetJiangLi(10);
			RemoveNpc(m_NowNpc);

			g_pMainState->m_Channel.AddNewString(m_TextList[17]);
			g_pMainState->m_Tags.Add(m_TextList[17]);
		}
	case 1002:

		{
			EndFB();
		}
		break;
	default:
		ERRBOX;
		break;
	}
}

void cTaskFB::Load(ifstream& File)
{//���ڵ�ͼ��ȡ��
	m_List.clear();
	m_List2.clear();
	File >> m_FBID;
	if (-1 == m_FBID)return;
	File >> m_Stage;
	File >> m_Stage2;
	File >> m_HuanShu;
	Start(m_FBID);
	int npc2num;
	File >> npc2num;
	m_List.resize(npc2num);
	for (int i = 0; i < npc2num; i++)
		m_List[i].Load(File);

	File >> npc2num;
	m_List2.resize(npc2num);
	for (int i = 0; i < npc2num; i++)
	{
		m_List2[i].Load(File);
	}
	BOOL havetrigger;
	File >> havetrigger;
	if (havetrigger)
	{
		int type;
		File >> type;
		m_pEnd = g_pMainState->FindTaskByID(type);
		File >> m_Ret;
	}
}

void cTaskFB::Save(ofstream& File)
{
	File << "\n";
	File << m_FBID << " ";
	if (-1 == m_FBID)return;
	File << m_Stage<<" ";
	File << m_Stage2 << " ";
	File << m_HuanShu << " ";
	File << m_List.size() << " ";
	for (int i = 0; i < m_List.size(); i++)
	{
		m_List[i].Save(File);
	}
	File << m_List2.size()<<" ";
	for (int i = 0; i < m_List2.size(); i++)
	{
		m_List2[i].Save(File);
	}
	if (m_pEnd)
	{
		File << 1 << " ";
		File << m_pEnd->GetType()<<" ";
		File << m_Ret<<" ";
	}
	else
	{
		File << 0 << " ";
	}
}
void cTaskFB::RemoveNpc(sNpc* p, BOOL bRemoveList )
{
	cObj* po = g_pMainState->FindObjByNpc(p);
	RemoveNpc(po,bRemoveList);
}
void cTaskFB::RemoveNpc(cObj*& p, BOOL bRemoveList )
{
	if (!p)return;
	if (bRemoveList)
	{
		UINT i;
		for (i = 0; i < m_List.size(); i++)
		{
			if (m_List[i].m_Npc.m_x == p->GetX() / 20)
			if (m_List[i].m_Npc.m_y == (g_pMainState->m_Map.m_pMap->m_Height - p->GetY()) / 20)
			if (m_List[i].m_Npc.m_name == p->m_TrueName.GetString())
			{
				m_List.erase(m_List.begin() + i);
				break;
			}
		}
		if (i==m_List.size())
		for (i = 0; i < m_List2.size(); i++)
		{
			if (m_List2[i].m_Npc.m_x == p->GetX() / 20)
			if (m_List2[i].m_Npc.m_y == (g_pMainState->m_Map.m_pMap->m_Height - p->GetY()) / 20)
			if (m_List2[i].m_Npc.m_name == p->m_TrueName.GetString())
			{
				m_List2.erase(m_List2.begin() + i);
				break;
			}
		}
	}
	g_pMainState->m_Map.RemoveNpc(p, TRUE);
	p = 0;
}

void cTaskFB::AddTriggerDialog(string path, int id, int ret)
{
	g_pMainState->m_TriggerDialog.AddTaskFB(path, id, this, ret);
}

void cTaskFB::AddNpc()
{
	for (int i = 0; i < m_List.size(); i++)
	{
		g_pMainState->m_Map.AddNpc(&m_List[i]);
	}
	for (int i = 0; i < m_List2.size(); i++)
	{
		g_pMainState->m_Map.AddNpc(&m_List2[i]);
	}
	g_pMainState->FindObj();
}

void cTaskFB::UpdateJob(string context, int time/*=-1*/, int falret /*= 1002*/, string title /*= ""*/)
{ //
	sJob job;
	if (title == "")
		title = "����";
	job.Set(title, context, time, this, falret);
	g_pMainState->m_Job.AddaJob(job);
}



void cTaskFB::ProcessSL(int stage)
{
	switch (stage)
	{
	case 0:
		
		{
			m_Stage = 0;
			g_pMainState->m_TriggerObj.AddTask(m_TextList[1], m_TextList[0], this, 1);
			g_pMainState->m_TriggerObjMap.AddTask(this, 2, m_TextList[2]);
			m_List.resize(2);
			SetNpc(m_List[0],3,m_TextList[3], m_TextList[2],1117,35,37,3);
			SetNpc(m_List[1],6, m_TextList[4], m_TextList[2], 1118, 93, 80, 1);
			UpdateJob(m_TextList[26]);
		}
		break;
	case 1:g_pMainState->m_pNowSelect->Talk(m_TextList[5], this, -1, "��", "��");
		break;
	case -1:
		if (!g_pMainState->m_Dialog.m_retSelect)
		{
			UpdateJob(m_TextList[8], 120);
			g_pMainState->m_Map.LoadMap(16, 14, m_TextList[2].c_str());
		}
		break;
	case 2:
		
		{
			if (!m_List.size())return ;
			if (m_Stage==3)
				AddNpcOne(&m_List[1]);
			AddNpcOne(&m_List[0], TRUE);
		}
		break;
	case 3:
		m_NowNpc = g_pMainState->m_pNowSelect;
		g_pMainState->m_pNowSelect->Talk(m_TextList[13], this, -3, "��", "��");
		break;
	case -3:
			switch (m_Stage)
			{
			case 0://����
				
				{
					m_NowNpc->Talk(m_TextList[7]);
					g_pMainState->m_TriggerObjMap.AddTask(this, 4, m_TextList[9]);
					UpdateJob(m_TextList[7]);
					m_Stage = 2;
					
				}
				break;
			case 1: //�ռ�ľ��
				
				{
					m_NowNpc->Talk(m_TextList[6]);
					g_pMainState->m_TriggerObj.AddTask(m_TextList[4], m_TextList[2], this, 6);
					UpdateJob(m_TextList[6]);
					m_Stage = 3;
					AddNpcOne(&m_List[1]);

				}
				break;
			default:
				break;
			}
		break;
	case 4://����NPC
		
		{
			cMyMap* p = &g_pMainState->m_Map;
			int num = p->m_numberOfNpc;
			for (int i = 0; i < num; i++)
			{
				p->m_pNpc[i]->m_TaskTrigger.m_pTask = this;
				p->m_pNpc[i]->m_TaskTrigger.m_Stage = 5;
			}
		}
		break;
	case 5://����NPC
		
		{
			g_pMainState->m_pNowSelect->Talk(m_TextList[10]);
			g_pMainState->m_Dialog.m_pObj = 0;
			g_pMainState->m_Map.RemoveNpc(g_pMainState->m_pNowSelect,TRUE);
			if (g_pMainState->m_Map.m_numberOfNpc<3)
			{
				g_pMainState->m_Map.RemoveAllNpc();
				UpdateJob(m_TextList[8]);
				SetJiangLi(10,TRUE);
				g_pMainState->m_TriggerObjMap.ReMove(this,m_TextList[9]);
				g_pHeroObj->Talk(m_TextList[14]);
				m_Stage = 1;

				g_pMainState->m_TriggerObjMap.AddTask(this, 2, m_TextList[2]);
			
				m_HuanShu = 0;
			}
		}
		break;
	case 6:
		{
			if (3 != m_Stage)return;
			int lv = g_pCharacter->m_PCData.m_LV;
			m_NowNpc = g_pMainState->m_pNowSelect;
			int select = rand() % 10;
			if (select == 0)
			{
				Process(7);
			}
			else
			{
				g_pMainState->m_Tags.Add(m_TextList[11]);
				for (int i = 0; i < 8; i++)
				{
					AddPCPetEnemyF(lv*60, (ePetID)(rand() % 10), -1, eMP_���, m_TextList[12]);
				}
				SetMusicFB();
				g_StateType = STATE_FIGHT;
				AddTriggerFightSuccess(7);
			}
		}
		break;
	case 7://��������
		{
			SetJiangLi(2);
			m_HuanShu += 1;
			if (m_HuanShu == 5)
			{
				RemoveNpc(m_NowNpc);
				m_NowNpc = g_pMainState->FindObjByName(m_TextList[3]);
				RemoveNpc(m_NowNpc);
				g_pMainState->m_TriggerObj.ReMove(this);
				AddTriggerDialog(0, 8);

				return;
			}
			UpdateJob(m_TextList[8], 120);
			m_Stage = 1;
		}
		break;
	case 8:
		{
			sMapObjTrigger npc;
			SetNpc(npc,9,m_TextList[15], m_TextList[2], 139, 49, 28,1);
		
			UpdateJob(m_TextList[20]);
			g_pMainState->m_TriggerObjMap.AddTask(npc);
			AddNpcOne(&npc,FALSE);
		}
		break;
	case 9:
		m_NowNpc = g_pMainState->m_pNowSelect;
		AddTriggerDialog(1, 10); break;
	case 10:
		{
			m_List.resize(2);
			SetNpc(m_List[0],12,m_TextList[17], m_TextList[2], 56, 116, 83, 1);//����
			SetNpc(m_List[1],13,m_TextList[18], m_TextList[2], 49, 122, 80, 1);//��
			AddTriggerObjMap(m_TextList[2], 11);
			g_pMainState->m_Map.RemoveNpc(m_NowNpc,TRUE);
			UpdateJob(m_TextList[16]);
			Process(11);
		}
		break;
	case 11:AddNpc(); break;
	case 12:
	case 13:
		{
			int lv = g_pCharacter->m_PCData.m_LV;
			m_NowNpc = g_pMainState->m_pNowSelect;
		//	sNpc* pNpc = (sNpc*)m_NowNpc->m_pIndex;
			if (stage == 12)
			{	
				AddPCPetEnemyF(lv * 120, (ePetID)m_List[0].m_Npc.m_modeid, lv + 5, eMP_���, m_TextList[17]);
			}
			else
			{
				AddPCPetEnemyF(lv * 120, (ePetID)m_List[m_List.size()-1].m_Npc.m_modeid, lv + 5, eMP_���, m_TextList[18]);
			}
			for (int i = 0; i < 7; i++)
			{
				AddPCPetEnemyF(lv * 80, (ePetID)(rand() % 40), -1, eMP_���, m_TextList[19]);
			}
			AddTriggerFightSuccess(14);
			SetMusicFB();
			g_StateType = STATE_FIGHT;
		}
		break;
	case 14:
		{
			SetJiangLi(4, 1);
			RemoveNpc(m_NowNpc);
			if (!m_List.size())
			{
				m_List.resize(1);
				SetNpc( m_List[0],15,m_TextList[21], m_TextList[2], 1066, 120, 81, 1);
				AddTriggerObjMap(&m_List[0]);
				AddNpc();
			}
		}
		break;
	case 15:
			g_pMainState->m_pNowSelect->Talk(m_TextList[22], this, 16, "��", "��");
		break;
	case 16:
	{
			g_pMainState->m_TriggerObj.AddTask(m_TextList[1], m_TextList[22], this, 18);
			g_pMainState->m_Map.LoadMap(54, 4, m_TextList[23].c_str());
			AddTriggerObjMap(m_TextList[23], 11);
			m_List.resize(20);
			cObj obj;
			obj.SetXY(28 * 20, g_pMainState->m_Map.m_pMap->m_Height - 14 * 20);
			g_pMainState->m_FindPt.FindPath(&obj, 20 * 20, g_pMainState->m_Map.m_pMap->m_Height - 81 * 20);
			int timenum = obj.m_time / m_List.size();
			for (int i = 0; i < m_List.size(); i++)
			{
				SetNpc(m_List[i],22,m_TextList[19], m_TextList[23], rand() % 40, obj.m_Path[obj.m_time].sx / 20, (g_pMainState->m_Map.m_pMap->m_Height - obj.m_Path[obj.m_time].sy) / 20);
				obj.m_time -= timenum;
			}
			m_List2.resize(1);
			for (int i = 0; i < m_List2.size(); i++)
			{
				SetNpc(m_List2[i],21,m_TextList[24], m_TextList[23], 121, 43, 78, 1);
			}
			Process(11);
		}
		break;
	case 18:g_pMainState->m_pNowSelect->Talk(m_TextList[5], this, 19, "��", "��");
		break;
	case 19:
		g_pMainState->m_Map.LoadMap(16, 14, m_TextList[2].c_str());
		break;
	case 22: //С��ս��
		
		{
			m_NowNpc = g_pMainState->m_pNowSelect;
			int lv = g_pCharacter->m_PCData.m_LV;
			AddPCPetEnemyF(lv * 80, (ePetID)(m_NowNpc->GetModeID()));
			for (int i = 0; i < 7; i++)
			{
				AddPCPetEnemyF(lv * 70, (ePetID)(m_NowNpc->GetModeID()));
			}
			SetMusicFB();
			g_StateType = STATE_FIGHT;
			AddTriggerFightSuccess(20);
		}
		break;
	case 20://С������
		
		{
			SetJiangLi(2);
			RemoveNpc(m_NowNpc);
		}
		break;
	case 21://����ս��
		
		{
			m_NowNpc = g_pMainState->m_pNowSelect;
			int lv = g_pCharacter->m_PCData.m_LV;
			AddPCPetEnemyF(lv * 150, (ePetID)m_NowNpc->GetModeID(), lv + 10, eMP_���, m_NowNpc->m_TrueName.GetString());
			for (int i = 0; i < 9; i++)
			{
				AddPCPetEnemyF(lv * 100,(ePetID)(rand()%50));
			}
			SetMusicFB();
			g_StateType = STATE_FIGHT;

			AddTriggerFightSuccess(23);
		}
		break;
	case 23://BOSS���� 
		
		{
			SetJiangLi(10,TRUE);
			RemoveNpc(m_NowNpc);
			g_pMainState->m_Channel.AddNewString(m_TextList[25]);
			g_pMainState->m_Tags.Add(m_TextList[25]);
		}
	case 1002:
		EndFB();
		break;
	default:
		break;
	}
}




void cTaskFB::ProcessCC(int stage)
{
	int lv = g_pCharacter->m_PCData.m_LV;
	switch (stage)
	{
	case 0:
		
		{
			m_Stage =0;
			AddTriggerObj(m_TextList[1], m_TextList[0], 1);
			AddTriggerObjMap(m_TextList[3], 3);
			m_List.resize(1);
			SetNpc(m_List[0],4,m_TextList[5], m_TextList[3], 1075,53,32,1);
			UpdateJob(m_TextList[22]);	
		}
		break;
	case 1:
		g_pMainState->m_pNowSelect->Talk(m_TextList[2], this, -1, "��", "��");
		break;
	case -1:
		if (g_pMainState->m_Dialog.m_retSelect)return;
	case 2:
		
		{
			if (!m_Stage)
			{
				g_pMainState->m_Map.LoadMap(8, 11, m_TextList[3].c_str());
			}
			else
			{
				g_pMainState->m_Map.LoadMap(37, 20, m_TextList[14].c_str());
			}
		}
		break;
	case 3:AddNpc();
		break;
	case 4:
		m_NowNpc = g_pMainState->m_pNowSelect;
		AddTriggerDialog(0, 6); 
		UpdateJob("�ش��ʿ������,���ռ�����ľͷ��������", -1, -1, "����");
		break;
	case 6://�����ռ�ľͷ
		
		{
			m_Stage2 = 20;
			RemoveNpc(m_NowNpc);
			m_List2.resize(1);
			SetNpc(m_List2[0],8,m_TextList[4], m_TextList[3], 1119, 87, 116, 1);
			StageSetSameNpc(1077, m_TextList[3], 9, 40, m_TextList[6]);
			
		}
		break;
	case 8:
	
		{
			m_NowNpc = g_pMainState->m_pNowSelect;
			int num=0;
			for (int i = 0; i < 60; i++)
			{
				sItem2& item= g_pCharacter->m_PCData.m_Item[i];
				if (!item.m_Num)continue;
				if (item.GetID() == 56)
				if (item.GetType() == 27)
				{
					num += item.GetNum();
					item.m_Num = 0;
				}
			}
			if (num)
			{
				if (num > m_Stage2)
					num = m_Stage2;
				m_Stage2 -= num;
				for (int i = 0; i < num; i++)
				{
					SetJiangLi(1);
					if (!(rand() % 20))GetItem(27, rand() % 4 + 38, TRUE);
				}
				if (m_Stage2)
				{
					ostringstream oss;
					oss << "����Ҫ";
					oss << m_Stage2;
					oss << "��ľͷ";
					m_NowNpc->Talk(oss.str());
				}
				else
				{
					g_pHeroObj->Talk(m_TextList[7]);
					UpdateJob(m_TextList[7]);
					ClearNpc();
					StageSetSameNpc(120,m_TextList[3],12,10,m_TextList[8]);
				}
			}
		}
		break;
	case 9://��ʿ
		
		{
			m_NowNpc = g_pMainState->m_pNowSelect;
			m_NowNpc->Answer(this, 10);
		}
		break;
	case 10:
		
		{
			if (g_pMainState->m_Dialog.m_retSelect == g_pMainState->m_Dialog.m_Question.answer )
			{
				GetItem(27, 56);
			}
			else
			{
				m_NowNpc->Talk(g_pMainState->m_Dialog.GetTrueSelect());
			}
			//��ʿ���ط�
			int x, y;
			sMapObjTrigger* pNpc= FindNpcByObj(m_NowNpc);
			g_pMainState->m_Map.AutoGetPos(x, y, m_TextList[3]);
			m_NowNpc->SetXY(x * 20, g_pMainState->m_Map.m_pMap->m_Height - y * 20);
			if (!m_NowNpc)return;
			pNpc->m_Npc.m_x = x;
			pNpc->m_Npc.m_y = y;
			g_pMainState->FindObj();
		}
		break;
	case 12:
		
		{
			m_NowNpc = g_pMainState->m_pNowSelect;
			int enemynum =g_pMainState->AtutoGetEnemyNum();
			sAddPoint addpoint;
			for (int i = 0; i < enemynum; i++)
			{
				AddPetEnemyF(lv * 80, e����, lv, m_TextList[8]);
			}
			SetMusicFB();
			g_StateType = STATE_FIGHT;
			AddTriggerFightSuccess(13);
		}
		break;
	case 13:
		
		{
			SetJiangLi(2,0,200,100);
			if (!(rand() % 10))GetItem(27, rand() % 4 + 38, TRUE);
			RemoveNpc(m_NowNpc);
			if (!m_List.size())
			{
				AddTriggerDialog(1, 14);
			}
		}
		break;
	case 14:
		
		{
			UpdateJob(m_TextList[12]);
			m_List.resize(3);
			SetNpc(m_List[0],16,m_TextList[9],m_TextList[3],51,62,99,3);
			SetNpc(m_List[1],16,m_TextList[10], m_TextList[3], 51, 72, 103, 3);
			SetNpc(m_List[2],16,m_TextList[11], m_TextList[3], 51, 84, 109, 3);
			Process(3);
		}
		break;
	case 16:
		
		{
			m_NowNpc = g_pMainState->m_pNowSelect;
			AddPCPetEnemyF(lv * 150, e�������, -1, eMP_���, m_NowNpc->m_TrueName.GetString());
			for (int i = 0; i < 7; i++)
			{
				AddPCPetEnemyF(lv * 100, (ePetID)(rand() % 30));
			}
			g_StateType = STATE_FIGHT;
			SetMusicFB();
			AddTriggerFightSuccess(17);
		}
		break;
	case 17:
		
		{
			SetJiangLi(5);
			if (!(rand() % 20))
			{
				GetItem(27, 5,TRUE);
			}
			RemoveNpc(m_NowNpc);
			int x, y;
			g_pMainState->m_Map.AutoGetPos(x,y,m_TextList[14]);
			if (!m_List.size())
			{
				m_Stage = 1;
				AddTriggerObjMap(m_TextList[14], 3);
				//͵�� ֻ������״򲻶�
				m_List.resize(2);
				g_pMainState->m_Map.AutoGetPos(x, y);
				SetNpc(m_List[0], 18, m_TextList[15], m_TextList[14],56,x,y);
				g_pMainState->m_Map.AutoGetPos(x, y);
				SetNpc(m_List[1], 19, m_TextList[16], m_TextList[14], 48, x, y);
				Process(2);
				UpdateJob(m_TextList[13]);
			}
		}
		break;
	case 18://�㲻��
		
		{
			m_NowNpc = g_pMainState->m_pNowSelect;
	
			AddPCPetEnemyF(lv * 120, e�ɷ�Ů�, -1, eMP_���, m_NowNpc->m_TrueName.GetString());
			sAddPoint addpoint;
			for (int i = 0; i < 7; i++)
			{
				AddPCPetEnemyF(lv * 90, (ePetID)(rand() % 50));
			}
			SetMusicFB();
			g_StateType = STATE_FIGHT;
			AddTriggerFightSuccess(20);
		}
		break;
	case 19://�Ҳ���
		
		{
			m_NowNpc = g_pMainState->m_pNowSelect;
			AddPCPetEnemyF(lv * 160, e��ƿŮ�, lv+9, eMP_ħ��կ, m_NowNpc->m_TrueName.GetString());
			for (int i = 0; i < 4;i++)
			AddPCPetEnemyF(lv * 160, e��ƿŮ�, -1, eMP_ħ��կ);
			for (int i = 0; i < 5; i++)
				AddPCPetEnemyF(lv * 160, e�������);
			g_StateType = STATE_FIGHT;
			SetMusicFB();
			AddTriggerFightSuccess(20);
		}
		break;
	case 20:
		
		{
			RemoveNpc(m_NowNpc);
			SetJiangLi(5);
			if (!m_List.size())
			{
				m_NowNpc->Talk(m_TextList[17]);
				m_List.resize(1);
				int x, y;
				g_pMainState->m_Map.AutoGetPos(x, y, m_TextList[14]);
				SetNpc(m_List[0], 21, m_TextList[18],m_TextList[14],56,20,128,3);
				Process(3);
				UpdateJob(m_TextList[17]);
			}
		}
		break;
	case 21://����BOSS
		
		{
			m_NowNpc = g_pMainState->m_pNowSelect;
			AddPCPetEnemyF(lv * 180, e������, lv + 10, eMP_ʨ����, "��������");
			AddPCPetEnemyF(lv * 180, e��������, lv + 9, eMP_ħ��կ, "��������");
			AddPCPetEnemyF(lv * 180, e������¹, lv + 9, eMP_������, "¹������");
			sAddPoint addpoint;
			ePetID petid;
			for (int i = 0; i < 5; i++)
			{
				switch (rand()%3)
				{
				case 0:petid = e��ħ��; break;
				case 1:petid = e���; break;
				default:petid = e��;
					break;
				}
				AddPCPetEnemyF(lv * 120, petid);
			}
			SetMusicFB();
			g_StateType = STATE_FIGHT;
			AddTriggerFightSuccess(22);
		}
		break;
	case 22:
		
		{
			RemoveNpc(m_NowNpc);
			SetJiangLi(10);
			//�������
			StageSetSameNpc(1125, m_TextList[14], 23, 20);
			UpdateJob(m_TextList[21], 2);
		}
		break;
	case 23:
		
		{
			vector<int> rate;
			rate.resize(5);
			rate[0] = 5;
			rate[1] = 30;
			rate[2] = 25;
			rate[3] = 20;
			rate[4] = 20;
			int rat = rand() % 100;		
			for (int i = 0; i < rate.size(); i++)
			{
				if (rat < rate[i])
				{
					switch (i)
					{
					case 0: 
						switch (rand() % 3)
						{
						case 0:
							SetJiangLiItem(g_pMainState->m_ItemManage.m_LShouJueList[rand() % g_pMainState->m_ItemManage.m_LShouJueList.size()], 40); //�޾�
							break;
						case 1:SetJiangLiItem(33, 27);
							break;
						case 2:
							SetJiangLiItem(rand()%6+5, 27);
							break;
						}
						break;
					case 1:SetJiangLiItem(rand() % 17, 29); //��ʯ
						break;
					case 2:SetJiangLi(5, TRUE, 100, 0, 0);//Ǯ
						break;
					case 3:SetJiangLi(5, TRUE, 0, 0, 100);//����
						break;
					case 4:SetJiangLiItem(rand()%89,41);//��ʯ
						break;
					}
					break;
				}
				rat -= rate[i];
			}
			RemoveNpc(g_pMainState->m_pNowSelect);
			if (!m_List.size())
			{
				ProcessCC(1002);
			}
		}
		break;
	case 1002:
		EndFB();
		break;
	}
}

void cTaskFB::GetItem(int type,int id, BOOL needtag,int num)
{
	sItem2 item;
	item.m_Num = num;
	item.Set(type, id);
	g_pMainState->m_InterfaceItem.GetItem(g_pMainState->m_HeroID, &item);
	if (needtag)
	{
		ostringstream oss;
		oss << "��õ���";
		oss << item.m_pItemData->m_Name;
		g_pMainState->m_Tags.Add(oss.str());
	}
}

void cTaskFB::ProcessSJ(int stage)
{
	int lv = g_pCharacter->m_PCData.m_LV;
	switch (stage)
	{
	case 0:
		{
			Init();
			m_HuanShu = 0;
			m_Stage = 0;
			g_pMainState->m_TriggerObj.AddTask(m_TextList[1], m_TextList[0], this, 1);
			AddTriggerObjMap(m_TextList[3], 3);
			m_List.resize(1);
			SetNpc(m_List[0],4,m_TextList[5], m_TextList[3], 1050, 59, 47, 1);
			UpdateJob(m_TextList[23]);

		}
		break;
	case 1:
		{
			g_pMainState->m_pNowSelect->Talk(m_TextList[2], this, -2, "��", "��");
		}
		break;
	case -2:
		if (g_pMainState->m_Dialog.m_retSelect)return;
	case 2:
		switch (m_Stage)
		{
		case 0:g_pMainState->m_Map.LoadMap(16, 30, m_TextList[3].c_str()); break;
		case 1:g_pMainState->m_Map.LoadMap(120, 57, m_TextList[7].c_str()); break;
		case 2:g_pMainState->m_Map.LoadMap(69, 51, m_TextList[9].c_str()); break;
		case 3:g_pMainState->m_Map.LoadMap(69, 51, m_TextList[19].c_str()); break;
		}
		break;
	case 3:AddNpc();
		break;
	case 4:
		m_NowNpc = g_pMainState->m_pNowSelect;
		g_pMainState->m_pNowSelect->Talk(m_TextList[6], this, 5, "��", "��");
		break;
	case 5://�õ�ץ������
		if (!g_pMainState->m_Dialog.m_retSelect)
		{
			//..
			m_List2.resize(1);
			g_pMainState->m_TaskZhuaGui.AutoGetGost(m_List2[0].m_Npc, m_TextList[3]);
			m_List2[0].m_pTask = this;
			m_List2[0].m_Stage = 8;
			int xerr, yerr;
			g_pMainState->m_TaskZhuaGui.GetErrPos(m_List2[0].m_Npc, xerr, yerr);
			ostringstream oss;
			oss << "ȥ#R" << xerr << " " << yerr << "#R������#R" << m_List2[0].m_Npc.m_name << "#Rץ����";
			m_NowNpc->Talk(oss.str());
			UpdateJob(oss.str());
			AddTriggerObjMap(m_TextList[3], 6);
			AddNpcOne(&m_List2[0], TRUE);
		}
		break;
	case 6:
		AddNpc();
		break;
	case 8:
		{
			m_NowNpc = g_pMainState->m_pNowSelect;
			g_pMainState->m_TaskZhuaGui.ReadyToFight(m_List2[0].m_Npc);
		
			AddTriggerFightSuccess(9);
		}
		break;
	case 9:
		RemoveNpc(m_NowNpc);
		SetJiangLi(3);
		m_HuanShu += 1;
		if (m_HuanShu == 6)
		{
			m_Stage = 1;

			ProcessSJ(2);
			g_pMainState->m_TaskHuoDong.Initmp();

			m_List.resize(12);
			for (int i = 0; i < m_List.size(); i++)
			{
				m_List[i] = g_pMainState->m_TaskHuoDong.m_List[i];
				m_List[i].m_pTask = this;
				m_List[i].m_Stage = 10;
				g_pMainState->m_Map.AutoGetPos(m_List[i].m_Npc.m_x, m_List[i].m_Npc.m_y, m_TextList[7]);
			}
			AddTriggerObjMap(m_TextList[7], 6);
			ProcessSJ(6);
			g_pMainState->m_Tags.Add(m_TextList[15]);
			UpdateJob(m_TextList[15]);
			return ;
		}
		UpdateJob(m_TextList[16]);
		break;
	case 10:
		{
			m_NowNpc = g_pMainState->m_pNowSelect;
			int id = m_NowNpc->GetModeID()-2000;
			sMapObjTrigger* plist= FindNpcByObj(m_NowNpc);
			g_pMainState->m_TaskHuoDong.ReadytoFightmp(*plist, (eMengPai)id);
			AddTriggerFightSuccess(11);
		}
		break;
	case 11:
		{
			SetJiangLi(8,TRUE,100,30,150);
			RemoveNpc(m_NowNpc);
			if (!m_List.size())
			{
				
				ProcessSJ(-11);
			}
		}
		break;
	case -11:
	{
				m_Stage = 2;
				ProcessSJ(2);
				g_pMainState->m_Tags.Add(m_TextList[8]);
				UpdateJob(m_TextList[8]);
				m_List.resize(1);
				string name;
				int modeid;
				if (-1 == g_pCharacter->m_PCData.m_MengPaiID)g_pCharacter->m_PCData.m_MengPaiID = 0;
				name = g_pMainState->m_SkillManager.m_MengPaiManager.GetMengPai(g_pCharacter->m_PCData.m_MengPaiID)->m_ShiFuName;
				switch (g_pCharacter->m_PCData.m_MengPaiID)
				{
				case eMP_���ƹٸ�:modeid = 139; break;
				case eMP_������:modeid = 142; break;
				case eMP_Ů����:modeid = 132; break;
				case eMP_����ɽ:modeid = 135; break;
				case eMP_ʨ����:modeid = 143; break;
				case eMP_ħ��կ:modeid = 133; break;
				case eMP_��˿��:modeid = 136; break;
				case eMP_���ܵظ�:modeid = 140; break;
				case eMP_��������:modeid = 131; break;
				case eMP_��ׯ��:modeid = 134; break;
				case eMP_����ɽ:modeid = 141; break;
				default:  modeid = 138; break;
				}
				m_HuanShu = 0;
				//int x, y;
				
				SetNpc(m_List[0], 12, name, m_TextList[9], modeid, 102, 62, 3);
				AddTriggerObjMap(m_TextList[9], 3);//&m_List[0]);
				AddNpcOne(&m_List[0]);
				m_Stage2 = 0;
	}
		break;
	case 12:
		{
			if (m_Stage2)return;
			m_NowNpc = g_pMainState->m_pNowSelect;
			m_NowNpc->Talk(m_TextList[10], this, 13, "��", "��");
			
		}
		break;
	case 13:
		{
			if (0 != g_pMainState->m_Dialog.m_retSelect)return;
			m_Stage2 = 1;
			int rate = rand() % 100;
			if (rate < 30)
			{//Ҫ����
				m_NowNpc->Talk(m_TextList[11]);
				g_pMainState->m_TriggerGive.AddTask(m_NowNpc->m_TrueName.GetString(),0,23,this, 14);
				return;
			}
			else
			{//ս��
				int x, y;
				g_pMainState->m_Map.AutoGetPos(x, y, m_TextList[9]);
				ostringstream oss;
				oss << m_TextList[12];
				oss << x << " " << y;
				oss << m_TextList[13];
				m_NowNpc->Talk(oss.str());
				UpdateJob(oss.str());
				m_List2.resize(1);
				SetNpc(m_List2[0], 15, m_TextList[14], m_TextList[9], rand() % 12 + 2000, x, y);
				AddNpcOne(&m_List2[0],TRUE);
			}
		}
		break;
	case 14:
		{
			SetJiangLi(2, TRUE, 50);
			m_Stage2 = 0;
		}
		break;
	case 15:
		{
			m_NowNpc = g_pMainState->m_pNowSelect;
			cPetData* pPet;
			pPet=AddPCEnemyF(lv *120, m_NowNpc->GetModeID(), 0, -1, eMP_���, m_NowNpc->m_TrueName.GetString());
			pPet->m_DamagePercent.d2 = 50;
			int enemynum =  g_pMainState->AtutoGetEnemyNum()-1;
			for (int i = 0; i < enemynum; i++)
			{
				switch (rand() % 4)
				{
				case 0:
					pPet = AddPCPetEnemyF(-lv * 10, e��󡾫);
					pPet->m_DamagePercent.d2 = 90;
					pPet->m_DamageAddPercentMagic.d2 = 90;
					break;
				case 1:
					pPet = AddPCPetEnemyF(-lv*10, e��󡾫);
					pPet->m_DamagePercent.d2 = 90;
					pPet->m_DamageAddPercentMagic.d2 = 90;
					break;
				case 2:
					pPet = AddPCPetEnemyF(lv*100, e����);
					pPet->m_DamagePercent.d2 = 80;
					break;
				case 3:
					pPet = AddPCPetEnemyF(lv*100, e��������);
					pPet->m_DamagePercent.d2 = 80;
					break;
				}
			}
			g_StateType = STATE_FIGHT;
			SetMusicFB();
			AddTriggerFightSuccess(16);
		}
		break;
	case 16:
		{
			   UpdateJob(m_TextList[8]);
			RemoveNpc(m_NowNpc);
			SetJiangLi(3, TRUE);
			m_Stage2 = 0;
			m_HuanShu += 1;
			if (8 == m_HuanShu)
			{
				m_Stage = 3;
				m_HuanShu = 0;
				AddTriggerObjMap(m_TextList[19], 3);
				
				g_pMainState->m_Tags.Add(m_TextList[17]);
				UpdateJob(m_TextList[18]);

				m_List2.resize(1);
				SetNpc(m_List2[0], 18, m_TextList[20], m_TextList[19], 1120, 63, 113);

				vector<int> modelist;
				modelist.resize(4);
				for (int i = 0; i < 4; i++)
					modelist[i] = i + 63;
				StageSetSameNpc(modelist, m_TextList[19], 17, 20);


			
				AddTriggerGive(m_TextList[20], 27, 57, 20,FALSE);
				AddTriggerGive(m_TextList[20], 27, 58, 21,FALSE);
				ProcessSJ(2);
				m_Stage2 = 50;
			}
		}
		break;
	case 17://�� ս��
		{
			m_NowNpc = g_pMainState->m_pNowSelect;
			AddPCPetEnemyF(lv * 140,(ePetID)m_NowNpc->GetModeID(),-1, eMP_���, m_NowNpc->m_TrueName.GetString());
			int enemynum =  g_pMainState->AtutoGetEnemyNum()-1;
			ePetID ptid;
			for (int i = 0; i < enemynum; i++)
			{
				switch (rand()%4)
				{
				case 0:ptid = e������;
				case 1:ptid = e������;
				case 2:ptid = e����ɡ;
					default:ptid = e��󡾫;
				}
				AddPCPetEnemyF(lv * 100, ptid);
			}
			g_StateType = STATE_FIGHT;

			AddTriggerFightSuccess(19);
		}
		break;
	case 18:
		{
			m_NowNpc = g_pMainState->m_pNowSelect;
			ostringstream oss;
			oss << "��ǰ����#R";
			oss << m_HuanShu <<"/"<< m_Stage2;
			oss << "#R";
			g_pMainState->m_pNowSelect->Talk(oss.str());
		}
		break;
	case 19:
		{
			GetItem(27, 57 + rand() % 2, TRUE); 
			RemoveNpc(m_NowNpc);
		}
		break;
	case 20:
	case 21:
		{
			if (20==stage)
			m_HuanShu += 20;
			else m_HuanShu += 30;
			SetJiangLi(3, TRUE);
		
			
			int mode2;
			switch (m_HuanShu / 10)
			{
			case 0:mode2 = 1120; break;
			case 1:mode2 = 1121; break;
			case 2:mode2 = 1122; break;
			case 3:mode2 = 1123; break;
			default: mode2 = 1124; break;
			}
			if (mode2 != m_List2[0].m_Npc.m_modeid)
			{
				m_List2[0].m_Npc.m_modeid = mode2;
				RemoveNpc(&m_List2[0].m_Npc, FALSE);
				g_pMainState->m_Map.AddNpc(&m_List2[0],TRUE);
			}
			if (m_HuanShu < 50)
			{
				AddTriggerGive(m_TextList[20], 27, 57, 20, FALSE);
				AddTriggerGive(m_TextList[20], 27, 58, 21, FALSE);
			}
			else
			{
				m_NowNpc = 0;
				m_NowNpc->Talk(m_TextList[21]);
				ClearNpc();

				StageSetSameNpc(1125, m_TextList[19],22,25);

				UpdateJob(m_TextList[22]);

			}
		}
		break;
	case 22:
		{
			int rat = rand() % 100;
			if (rat < 5)
			{
				SetJiangLiItem(33, 27);
			}
			else
			if (rat < 15)
			{
				SetJiangLiItem(38 + rand() % 4, 27);
			}
			if (rat < 25)
			{
				SetJiangLiItem(rand()%17,29);
			}
			else
			{
				if(rand()%2)
				SetJiangLi(5, TRUE,100,0,0);
				else SetJiangLi(5, TRUE, 0, 0);
			}
			RemoveNpc(g_pMainState->m_pNowSelect);
			if (m_List.size()<2)
			{
				ProcessSJ(1002);
			}
		}
		break;
	case 1002:
		EndFB();
		break;
	}
}

void cTaskFB::ProcessSM(int stage)
{
	int lv = g_pCharacter->m_PCData.m_LV;
	switch (stage)
	{
	case 0:
		{
			m_Stage = 0;
			AddTriggerObj(m_TextList[1], m_TextList[0], 1);
			AddTriggerObjMap(m_TextList[3], 4);
			StageSetSameNpc(2001, m_TextList[3], 5, 12,m_TextList[8]);
			StageSetSameNpc2(1125, m_TextList[3], -5, 15, m_TextList[14]);
		}
		break;
	case 1:
		{
			g_pMainState->m_pNowSelect->Talk(m_TextList[2], this, 2, "��", "��");
		}
		break;
	case 2:
		if (g_pMainState->m_Dialog.m_retSelect)return;
	case 3:
		switch (m_Stage)
		{
		case 0:
			g_pMainState->m_Map.LoadMap(24, 31, m_TextList[3].c_str());
			UpdateJob(m_TextList[9]);
			break;
		case 1:
			g_pMainState->m_Map.LoadMap(45, 25, m_TextList[4].c_str());
			UpdateJob(m_TextList[12]);
			break;
		case 2:
			g_pMainState->m_Map.LoadMap(8, 11, m_TextList[5].c_str());
			break;
		case 3:
			g_pMainState->m_Map.LoadMap(65, 56, m_TextList[6].c_str());
			break;
		case 4:
			g_pMainState->m_Map.LoadMap(8, 11, m_TextList[7].c_str());
			break;
		}
		break;
	case 4:AddNpc(); break;
	case -5:
		RemoveNpc(g_pMainState->m_pNowSelect);
		switch (rand()%4)
		{
		case 0:SetJiangLi(1, TRUE, 50, 0, 0); break;
		case 1:SetJiangLi(1, TRUE, 0, 0, 50); break;
		case 2:
			{ 
				g_pCharacter->m_PCData.m_HP /= 2;
				g_pCharacter->m_PCData.m_MP /= 2;
				g_pMainState->m_PCHead.UpdatePcHp();
				g_pMainState->m_PCHead.UpdatePcMp();
				AddTag(m_TextList[15]);
			}
			break;
		default://ս��
			{
				m_NowNpc = g_pMainState->m_pNowSelect;			
				int enemynum =  g_pMainState->AtutoGetEnemyNum();
				for (int i = 0; i < enemynum; i++)
				{
					AddPCPetEnemyF(lv * 65, (ePetID)(rand() % 40));
				}
				g_StateType = STATE_FIGHT;
				SetMusicFB();
				AddTriggerFightSuccess(-6);
			}
			return;
		}
		if (!(m_List.size()) && (!m_List2.size()))
		{
			m_Stage = 1;
			AddTriggerObjMap(m_TextList[4], 4);
			m_List2.resize(0);
			m_List.resize(1);
			SetNpc(m_List[0], 7, m_TextList[10], m_TextList[4], 1084, 48, 31);
			m_HuanShu = 1;
			ProcessSM(3);
		}
		break;
	case -6://����ս��ʤ
		{
			RemoveNpc(m_NowNpc);
			SetJiangLi(1);
			if (!(m_List.size()) && (!m_List2.size()))
			{
				m_Stage = 1;
				AddTriggerObjMap(m_TextList[4], 4);
				m_List2.resize(0);
				m_List.resize(1);
				SetNpc(m_List[0], 7, m_TextList[10], m_TextList[4], 1084, 48, 31);
				m_HuanShu = 1;
				ProcessSM(3);
			}
		}
		break;
	case 5://��������
		m_NowNpc = g_pMainState->m_pNowSelect;
		m_NowNpc->Answer(this, 6);
		break;
	case 6:

		{
			if (g_pMainState->m_Dialog.m_retSelect == g_pMainState->m_Dialog.m_Question.answer)
			{
				RemoveNpc(m_NowNpc);
				SetJiangLi(2,TRUE,0);
			}
			else
			{
				m_NowNpc->Talk(g_pMainState->m_Dialog.GetTrueSelect());
			}
			if (!(m_List.size())&& (!m_List2.size()))
			{
				m_Stage = 1;
				m_Stage2 = 0;
				AddTriggerObjMap(m_TextList[4], 4);
				m_List2.resize(0);
				m_List.resize(1);
				SetNpc(m_List[0], 7, m_TextList[10], m_TextList[4], 1084,48,31);
				m_HuanShu = 1;
				ProcessSM(3);
			}
		}
		break;
	case 7:
	
		{
			if (m_Stage2>0)
			{
				g_pMainState->m_pNowSelect->Talk(m_TextList[24]); 
				return;
			}
			g_pMainState->m_pNowSelect->Talk(m_TextList[11]);
			m_Stage2 = 1;
			sItem2 item;
			item.m_Num = 1;
			item.Set(28, 21);
			int x, y;
			g_pMainState->m_Map.AutoGetPos(x, y, m_TextList[4]);
			item.m_Property.resize(2);
			item.m_Property[0]=x;
			item.m_Property[1]=y;
			g_pMainState->m_InterfaceItem.GetItem(g_pMainState->m_HeroID, &item);
		}
		break;
	case 8://�ڱ�
	
		{
			if (g_pMainState->m_Map.m_MapName != m_TextList[4])
			{

				return;
			}
			sItem2* pItem = g_pCharacter->m_PCData.GetItem(g_pMainState->m_InterfaceItem.m_NowSelect);
			if (abs(g_pMainState->m_InterfaceDoor.m_HeroX -pItem->m_Property[0])<3)
			if (abs(g_pMainState->m_InterfaceDoor.m_HeroY - pItem->m_Property[1])<3)
			{
				m_Stage2 = 0;
				int rat = rand() % 100;
				if (rat < 20)
				{
					SetJiangLi(2,TRUE);
					m_HuanShu += 1;
					if (m_HuanShu >7)
					{
						m_HuanShu = 0;
						AddTriggerObjMap(m_TextList[5], 4);
						m_Stage = 2;
						m_List.resize(8);
						int x, y;
						g_pMainState->m_Map.AutoGetPos(x, y, m_TextList[5]);
						for (int i = 0; i < 8; i++)
						{
							g_pMainState->m_Map.AutoGetPos(x, y);
							SetNpc(m_List[i], 12, m_TextList[16 + i], m_TextList[5], 41, x, y);
						}
						UpdateJob(m_TextList[25]);
						Process(3);
						AddTag(m_TextList[25]);
					}
				}
				else
				{
					Process(9);	
					UpdateJob(m_TextList[12]);
				}
				pItem->m_Num = 0;
				return;
			}
			ostringstream oss;
			oss << "�ص�:" << pItem->m_Property[0] << " " << pItem->m_Property[1];
			AddTag(oss.str());
		}
		break;
	case 9: //�ڱ�ս��
	
		{
			m_NowNpc = g_pMainState->m_pNowSelect;

			AddPCPetEnemyF(lv * 10000, e��ʦ, -1, eMP_���, m_TextList[13]);
			for (int i = 0; i < 5; i++)
			{
				AddPCPetEnemyF(lv * 65, (ePetID)(rand() % 40));
			}
			g_StateType = STATE_FIGHT;
			SetMusicFB();
			AddTriggerFightTurn(10);
			AddTriggerFightSuccess(11);
		}
		break;
	case 10:
	
		{
			if (!g_pMainState->m_FightList[10].isLive)g_pMainState->m_pFightState->EscapeFight();
			int num=  g_pMainState->GetLiveNum(10, 20);
			cAttackAI& ai= g_pMainState->m_AI.m_AttackAI;
			if (1==num)
			{
				g_pMainState->m_pFightState->SuccessFight();
			}
			else
			{
				ai.Def(10);
			}
		}
		break;
	case 11:
	
		{
			SetJiangLi(3,TRUE);
			m_HuanShu += 1;
			if (m_HuanShu >7)
			{
				m_HuanShu = 0;
				AddTriggerObjMap(m_TextList[5], 4);
				m_Stage = 2;
				
				int x, y;
				g_pMainState->m_Map.AutoGetPos(x, y, m_TextList[5]);
				m_List.resize(4);
				for (int i = 0; i < 4; i++)
				{
					g_pMainState->m_Map.AutoGetPos(x, y);
					SetNpc(m_List[i], 12, m_TextList[16 + i], m_TextList[5], 41, x, y);
				}
				m_List2.resize(4);
				for (int i = 0; i < 4; i++)
				{
					g_pMainState->m_Map.AutoGetPos(x, y);
					SetNpc(m_List2[i], 12, m_TextList[20 + i], m_TextList[5], 41, x, y);
				}
				UpdateJob(m_TextList[25]);
				Process(3);
				AddTag(m_TextList[25]);
			}
		}
		break;
	case 12:
	
		{
			m_NowNpc = g_pMainState->m_pNowSelect;

			AddPCPetEnemyF(lv * 100, (ePetID)(m_NowNpc->GetModeID()), -1, eMP_���, m_NowNpc->m_TrueName.GetString());
			cPetData* Pet;
			for (int i = 0; i < 2; i++)
			{
				Pet = AddPCPetEnemyF(0, e��������, -1, eMP_���, m_NowNpc->m_TrueName.GetString());
				Pet->m_DamageAdd.d2 = 80;
				Pet->m_DamageAddMagic.d2 = 80;
			}
			for (int i = 0; i < 5; i++)
			{
				AddPCPetEnemyF(lv * 60, e��������);
			}
			SetMusicFB();
			g_StateType = STATE_FIGHT;
			AddTriggerFightSuccess(13);
		}
		break;
	case 13:

	{
			   BOOL find = FALSE;
			   string name = m_NowNpc->m_TrueName.GetString();
			   for (int i = 0; i < m_List.size(); i++)
			   {
				   if (name == m_List[i].m_Npc.m_name)
				   {
					   if (i == 0)
					   {
						   RemoveNpc(m_NowNpc);
						   find = TRUE;
						   break;
					   }
				   }
			   }
			   if (!find)
			   for (int i = 0; i < m_List2.size(); i++)
			   {
				   if (name == m_List2[i].m_Npc.m_name)
				   {
					   if (i == 0)
					   {
						   RemoveNpc(m_NowNpc);
						   find = TRUE;
						   break;
					   }
				   }
			   }
			   if (find)
			   if ((!m_List.size()) && (!m_List2.size()))
			   {
				   ProcessSM(-13);
			   }
			   return;
	}
		
	case -13:
			
			{//�׻��Ž���
				m_Stage = 3;
				AddTriggerObjMap(m_TextList[6], 4);
				m_List.resize(23);
				m_List2.resize(4);
				int x, y;
				g_pMainState->m_Map.AutoGetPos(x, y, m_TextList[6]);
				for (int i = 0; i < 4; i++)
				{
					g_pMainState->m_Map.AutoGetPos(x, y);
					SetNpc(m_List2[i],14,m_TextList[3+i] ,m_TextList[6],49,x,y);
				}
				for (int i = 0; i < 23; i++)
				{
					g_pMainState->m_Map.AutoGetPos(x, y);
					if (i==14)//BUG ����������
						SetNpc(m_List[i], 14, m_TextList[26 + 15], m_TextList[6], 49, x, y);
					else
						SetNpc(m_List[i], 14, m_TextList[26 + i], m_TextList[6], 49, x, y);
				}
				UpdateJob(m_TextList[53]);
				g_pMainState->m_Tags.Add(m_TextList[53]);
				Process(3);
			}
		break;
	case 14:
	
		{
			m_NowNpc = g_pMainState->m_pNowSelect;
			AddPCPetEnemyF(lv * 100, (ePetID)(m_NowNpc->GetModeID()), lv+5, eMP_���, m_NowNpc->m_TrueName.GetString());
			cPetData* Pet;
			for (int i = 0; i < 2; i++)
			{
				Pet = AddPCPetEnemyF(lv*60, e��������);
				Pet->m_DamageAdd.d2 = 30;
				Pet->m_DamageAddMagic.d2 = 30;
			}
			sAddPoint addpoint;
			for (int i = 0; i < 5; i++)
			{
				AddPCPetEnemyF(lv*70, e���);
			}
			g_StateType = STATE_FIGHT;
			SetMusicFB();
			AddTriggerFightSuccess(15);
		}
		break;
	case 15:
		
		{
			RemoveNpc(m_NowNpc);
			SetJiangLi(3);
			if (!m_List2.size())
			{
				ClearNpc();
				m_List.resize(1);
				SetNpc(m_List[0],16,m_TextList[50],m_TextList[6],2000,65,57,1);
				m_List[0].m_Npc.SetRanse(0,4,3);
				m_List[0].m_Npc.weapontype = 0;
				m_List[0].m_Npc.weaponid = 24;
				AddNpc();
				UpdateJob(m_TextList[51]);
				AddTag(m_TextList[51]);
			}
		}
		break;
	case 16://���ܽ���  0 4 3 0
		
		{
			m_NowNpc = g_pMainState->m_pNowSelect;
			
			cPetData* Pet;
			Pet= AddPCEnemyF(lv * 200, 0, 0, lv + 10, eMP_���ƹٸ�, m_NowNpc->m_TrueName.GetString(), m_List[0].m_Npc.weapontype, m_List[0].m_Npc.weaponid);
			Pet->m_RanSe.m_ranse[0].Set(2, 3, 3, 0);
			g_pMainState->m_ColorManage.Give2(&Pet->m_RanSe.m_ranse[0], 2000);
			
			for (int i = 0; i < 7; i++)
			{
				Pet = AddPCEnemyF(lv * 120, 0, 0, lv + 10, eMP_���ƹٸ�, "����Ӱ��", m_List[0].m_Npc.weapontype, m_List[0].m_Npc.weaponid);
				Pet->m_RanSe.m_ranse[0].Set(2, 3, 3, 0);
				g_pMainState->m_ColorManage.Give2(&Pet->m_RanSe.m_ranse[0], 2000);
			}
			g_StateType = STATE_FIGHT;
			SetMusicFB();
			AddTriggerFightSuccess(17);
		}
		break;
	case 17://
		
		{
			m_Stage = 4;
			m_List[0].m_Npc.weaponid = -1;
			m_List[0].m_Npc.weapontype = -1;
			m_List.resize(50);
			StageSetSameNpc(1125, m_TextList[7], 18, 40, m_TextList[14]);
			AddTriggerObjMap(m_TextList[7],4);
			Process(3);
			SetJiangLi(10);
			AddTag(m_TextList[52]);
			UpdateJob(m_TextList[52]);
		}
		break;
	case 18:
		
		{
			m_NowNpc = g_pMainState->m_pNowSelect;
			cJiangLi jiangli;
			jiangli.SetBaoShi(20);//��ʯ
			jiangli.Add(3,27,5,3); //66
			jiangli.Add(33, 27, 4);//�ʹ�
			jiangli.AddLShouJue(2);//���޾�
			jiangli.Add(38, 27,10, 4);//ǿ��ʯ
			jiangli.SetExp(30,8);
			jiangli.SetMoeny(25,8);
			jiangli.AutoGetJiangLi();
			RemoveNpc(m_NowNpc);
			if (m_List.size()<2)
				Process(1002);
		}
		break;
	case 1002:
		EndFB();
		
		break;
	}
}

void cTaskFB::AddTriggerDialog(int id, int ret)
{
	ostringstream oss;
	oss << m_FBID;
	g_pMainState->m_TriggerDialog.AddTaskFB(oss.str(), id, this, ret);
}

void cTaskFather::SetNpc(sMapObjTrigger& data, int ret, string name, string map, int modeid, int x, int y, int direction/*=-1*/)
{
	data.m_Npc.Reset();
	data.m_Npc.Set(name, map, modeid, x, y, direction);
	data.m_Stage = ret;
	data.m_pTask = this;
}

sMapObjTrigger* cTaskFB::FindNpcByObj(cObj* p)
{
	int x = p->GetX()/20;
	int y = (g_pMainState->m_Map.m_pMap->m_Height - p->GetY()) / 20;
	int modeid = p->GetModeID();

	for (int i = 0; i < m_List.size(); i++)
	{
		if (m_List[i].m_Npc.m_x==x)
		if (m_List[i].m_Npc.m_y == y)
		if (m_List[i].m_Npc.m_modeid == modeid)
		{
			return &m_List[i];
		}
	}
	for (int i = 0; i < m_List2.size(); i++)
	{
		if (m_List2[i].m_Npc.m_x == x)
		if (m_List2[i].m_Npc.m_y == y)
		if (m_List2[i].m_Npc.m_modeid == modeid)
		{
			return &m_List2[i];
		}
	}
	return 0;
}

void cTaskFB::ClearNpc()
{
	g_pMainState->m_TaskNormal.m_pNpc = 0;
	if (g_pMainState->m_pNowSelect)
	{
		g_pMainState->m_pNowSelect->SetShine(FALSE);
		g_pMainState->m_pNowSelect = 0;
	}
	g_pMainState->m_Map.RemoveAllNpc();
	m_List2.clear();
	m_List.clear();
	g_pMainState->FindObj();
}
void cTaskFB::StageSetSameNpc(vector<int> modid, string mapname, int ret, int num, string name)
{
	m_List.resize(num);
	int x, y;
	g_pMainState->m_Map.AutoGetPos(x, y, mapname);
	for (int i = 0; i < m_List.size(); i++)
	{
		g_pMainState->m_Map.AutoGetPos(x, y);
		SetNpc(m_List[i], ret, name, mapname, modid[rand() % modid.size()], x, y);
	}
	if (g_pMainState->m_Map.m_MapName == mapname)
	{
		AddNpc();
	}
}
void cTaskFB::StageSetSameNpc(int modid, string mapname, int ret, int num, string name)
{
	vector<int>  mod2;
	mod2.push_back(modid);
	StageSetSameNpc(mod2,mapname,ret,num,name);
}
void cTaskFB::StageSetSameNpc2(vector<int> modid, string mapname, int ret, int num, string name)
{
	m_List2.resize(num);
	int x, y;
	g_pMainState->m_Map.AutoGetPos(x, y, mapname);
	for (int i = 0; i < m_List2.size(); i++)
	{
		g_pMainState->m_Map.AutoGetPos(x, y);
		SetNpc(m_List2[i], ret, name, mapname, modid[rand() % modid.size()], x, y);
	}
	if (g_pMainState->m_Map.m_MapName == mapname)
	{
		AddNpc();
	}
}
void cTaskFB::StageSetSameNpc2(int modid, string mapname, int ret, int num, string name)
{
	vector<int>  mod2;
	mod2.push_back(modid);
	StageSetSameNpc2(mod2, mapname, ret, num, name);
}

void cTaskFB::EndFB()
{
	g_pMainState->m_Tags.Add("����������");
	g_pMainState->m_Map.LoadMap(485, 144, "������");
	g_pMainState->m_Job.ReMoveJob(this);
	RemoveTriggerObjMap();
	RemoveTriggerObj();
	m_List.clear();
	m_List2.clear();
	m_FBID = -1;
	if (m_pEnd)
	{
		cTaskFather* p = m_pEnd;
		m_pEnd = 0;
		p->Process(m_Ret);
	}

}

void cTaskFB::SetTask(cTaskFather* p, int ret)
{
	m_pEnd = p; 
	m_Ret = ret; 
}

void cTaskFB::SetMusicFB()
{
	string name;
	switch (rand()%2)
	{
	case 0:
		name="ս��_����";
		break;
	default:
		name = "ս��_����2";
		break;
	}
	SetMusic(name);
}



BOOL cTaskFriend::Process(int stage)
{
	switch (stage)
	{
	case 0:
	
		{
			g_pMainState->m_Friend.JuanZeng(g_pMainState->m_YingHang.m_ShuRu.m_num);
		}
		break;
	case 1:
		
		{
			g_pMainState->m_Friend.UnJuanZeng(g_pMainState->m_YingHang.m_ShuRu.m_num);
		}
	default:
		break;
	}
	return TRUE;
}



BOOL cObjTrigger::Process(string name)
{
	int num = m_datalist.size();
	if (!num)return FALSE;
	for (int i = 0; i < num; i++)
	{
		if (0 == m_datalist[i].m_mapname.compare(g_pMainState->m_Map.m_MapName))
		{
			if (0==m_datalist[i].m_name.compare(name))
			{
				m_datalist[i].m_pTask->Process(m_datalist[i].m_Stage);
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL cObjTrigger::AddTask(string name, string mapname, cTaskFather* ptask, int stage, BOOL bHuChi)
{
	if (bHuChi)
	{
		int num = m_datalist.size();
		for (int i = 0; i < num; i++)
		{
			if (m_datalist[i].m_pTask->GetType() == ptask->GetType())
			{
				m_datalist[i].m_Stage = stage;
				m_datalist[i].m_name = name;
				m_datalist[i].m_mapname = mapname;
				return FALSE;
			}
		}
	}
	sObjTrigger task;
	task.m_pTask = ptask;
	task.m_Stage = stage;
	task.m_name = name;
	task.m_mapname = mapname;
	m_datalist.push_back(task);
	return TRUE;
}

void cObjTrigger::ReMove(int i)
{
	m_datalist.erase(m_datalist.begin() + i);
}

void cObjTrigger::ReMove(cTaskFather* pTask)
{
	for (int i = 0; i < m_datalist.size(); i++)
	{
		if (pTask->GetType() == m_datalist[i].m_pTask->GetType())
		{
			ReMove(i);
			return ReMove(pTask);
		}
	}
}

void cObjTrigger::Save(ofstream& File)
{
	File << "\n";
	File << m_datalist.size() << " ";
	for (int i = 0; i < m_datalist.size(); i++)
	{
		File << m_datalist[i].m_pTask->GetType() << " ";
		File << m_datalist[i].m_pTask->m_HuanShu << " ";
		File << m_datalist[i].m_Stage << "\n";
		File << m_datalist[i].m_name << "\n";
		File << m_datalist[i].m_mapname << "\n";
	}
	File << "\n";
}

void cObjTrigger::Load(ifstream& File)
{
	m_datalist.clear();
	int num;
	File >> num;
	if (!num)return;
	int type;
	int huanshu;
	m_datalist.resize(num);
	
	for (int i = 0; i < num; i++)
	{
		cObjTrigger::sObjTrigger& data=m_datalist[i];
		File >> type;
		File >> huanshu;
		File >> data.m_Stage;
		data.m_pTask = g_pMainState->FindTaskByID(type);
		data.m_pTask->m_HuanShu = huanshu;
		getline(File, data.m_name);
		getline(File, data.m_name);
		getline(File, data.m_mapname);
	}
}

BOOL cMapObjTrigger::AddTask(cTaskFather* ptask, int stage, sNpc Npc ,BOOL bHuChi)
{	
	int num = m_datalist.size();
	if (bHuChi)
	for (int i = 0; i < num; i++)
	{
		if (m_datalist[i].m_pTask)
		if (m_datalist[i].m_pTask->GetType() == ptask->GetType())
		{
			if (m_datalist[i].m_Npc.m_mapname == Npc.m_mapname)
			{
				m_datalist[i].m_Npc = Npc;
				m_datalist[i].m_Stage = stage;
				return FALSE;
			}
		}
	}
	sMapObjTrigger data;
	data.m_pTask = ptask;
	data.m_Stage = stage;
	data.m_Npc = Npc;
	m_datalist.push_back(data);
	return TRUE;
}

BOOL cMapObjTrigger::AddTask(cTaskFather* ptask, int stage, string mapname,BOOL bHuChi)
{
	sNpc npc;
	npc.m_mapname = mapname;
	return AddTask(ptask, stage, npc,bHuChi);
}

void cMapObjTrigger::AddTask(sMapObjTrigger& target)
{
	for (int i = 0; i < m_datalist.size(); i++)
	{
		if(m_datalist[i].m_Npc.m_x==target.m_Npc.m_x)
		if (m_datalist[i].m_Npc.m_y == target.m_Npc.m_y)
		if (m_datalist[i].m_Npc.m_name == target.m_Npc.m_name)
		if (m_datalist[i].m_Npc.m_mapname == target.m_Npc.m_mapname)
		{
			return;
		}
	}
	m_datalist.push_back(target);
	if (g_pMainState->m_Map.m_MapName == target.m_Npc.m_mapname)
	{
		g_pMainState->m_Map.AddNpc(&target, TRUE);
	}

}

BOOL cMapObjTrigger::Process()
{
	int num = m_datalist.size();
	if (!num)return FALSE;
	BOOL bTrigger;
	for (int i = 0; i < num; i++)
	{
		if (i < m_datalist.size())
		{
			bTrigger = FALSE;
			if (m_datalist[i].m_Npc.m_mapname == "��")bTrigger = TRUE;
			if (m_datalist[i].m_Npc.m_mapname == g_pMainState->m_Map.m_MapName)bTrigger = TRUE;
			if (bTrigger)
			{
				if (m_datalist[i].m_Npc.m_modeid>-1)
				{
					g_pMainState->m_Map.AddNpc(&m_datalist[i]);
				}
				else
				{
					m_datalist[i].m_pTask->Process(m_datalist[i].m_Stage);
				}
			}
		}
		else return TRUE;
	}
	return TRUE;
}

void cMapObjTrigger::ReMove(int i)
{
	if (m_datalist.size())
	m_datalist.erase(m_datalist.begin() + i);
}
void cMapObjTrigger::ReMove(cTaskFather* pTask,int ret)
{
	for (int i = 0; i < m_datalist.size(); i++)
	if (m_datalist[i].m_pTask == pTask)
	{
		if (ret == -1)
		{
			ReMove(i);
			return ReMove(pTask,ret);
		}
		else
		{
			if (m_datalist[i].m_Stage == ret)
			{
				ReMove(i);
				return;
			}
		}
	}
}

void cMapObjTrigger::ReMove(cTaskFather* pTask, string mapname)
{
	for (int i = 0; i < m_datalist.size(); i++)
	if (m_datalist[i].m_pTask == pTask&&m_datalist[i].m_Npc.m_mapname==mapname)
	{
		ReMove(i);
		return;
	}
}

void cMapObjTrigger::ReMove(cTaskFather* pTask, int x, int y)
{
	for (int i = 0; i < m_datalist.size(); i++)
	if (m_datalist[i].m_pTask == pTask)
	{
		if (m_datalist[i].m_Npc.m_x == x&&m_datalist[i].m_Npc.m_y == y)
		{
			ReMove(i);
			return;
		}
	}
}

void cMapObjTrigger::Save(ofstream& File)
{
	File << "\n";
	File << "��ͼtrigger"<<" ";
	File << m_datalist.size() << " ";;
	for (int i = 0; i < m_datalist.size(); i++)
	{
		sMapObjTrigger& li = m_datalist[i];
		li.Save(File);
	}
	File << "\n";
}

void cMapObjTrigger::Load(ifstream& File)
{
	m_datalist.clear();
	string dustbin;
	int num;
	File >> dustbin ;
	while (dustbin != "��ͼtrigger")
		ERRBOX;
	File >> num;
	if (!num)return;
	m_datalist.resize(num);
	for (int i = 0; i < m_datalist.size(); i++)
	{
		sMapObjTrigger& li = m_datalist[i];
		li.Load(File);
	}
}

sMapObjTrigger* cMapObjTrigger::FindByName(string name)
{
	int num = m_datalist.size();
	for (int i = 0; i < num; i++)
	{
		if (0 == (m_datalist[i].m_Npc.m_name).compare(name))
		{
			return &m_datalist[i];
		}
	}
	return 0;
}



BOOL cLvTrigger::AddTask(cTaskFather* ptask, int stage, int lv)
{
	int num = m_datalist.size();
	for (int i = 0; i < num; i++)
	{
		if (m_datalist[i].m_pTask->GetType() == ptask->GetType())
		{
			m_datalist[i].lv = lv;
			m_datalist[i].m_Stage = stage;
			return TRUE;
		}
	}
	sLvTrigger data;
	data.lv = lv;
	data.m_pTask = ptask;
	data.m_Stage = stage;
	m_datalist.push_back(data);
	return TRUE;
}

BOOL cLvTrigger::Process()
{
	int num = m_datalist.size();
	int i = 0;
	for (; i < num; i++)
	{
		if (g_pCharacter->m_PCData.m_LV >= m_datalist[i].lv)
		{
			int stage = m_datalist[i].m_Stage;
			cTaskFather* ptask = m_datalist[i].m_pTask;
			m_datalist.erase(m_datalist.begin()+i);
			ptask->Process(stage);
			break;
		}
	}
	if (i < num)
		return Process();
	return TRUE;
}

void cLvTrigger::ReMove(int i)
{
	if (m_datalist.size())
		m_datalist.erase(m_datalist.begin() + i);
}

void cLvTrigger::ReMove(cTaskFather* pTask)
{
	for (int i = 0; i < m_datalist.size(); i++)
		if (m_datalist[i].m_pTask == pTask)ReMove(i);
}

void cLvTrigger::Save(ofstream& File)
{
	File << "\n";
	File << m_datalist.size() << " ";;
	for (int i = 0; i < m_datalist.size(); i++)
	{
		File <<m_datalist[i].m_pTask->GetType() << " ";
		File <<m_datalist[i].m_pTask->m_HuanShu << " ";
		File <<m_datalist[i].m_Stage << " ";
		File <<m_datalist[i].lv << " ";
	}
	File << "\n";
}

void cLvTrigger::Load(ifstream& File)
{
	int num;
	File >> num;
	m_datalist.resize(num);
	int type;
	int huanshu;
	for (int i = 0; i < num; i++)
	{
		File >> type;
		File >> huanshu;
		File >> m_datalist[i].m_Stage;
		m_datalist[i].m_pTask = g_pMainState->FindTaskByID(type);
		m_datalist[i].m_pTask->m_HuanShu = huanshu;
		File >> m_datalist[i].lv;
	}
}

BOOL cDialogTrigger::AddTaskFB(string path, int id, cTaskFather* ptask, int stage)
{
	string path1 = g_strMediaPath;
	path1+="����//����//";
	path1 += path;
	path1 += "�Ի�.txt";
	AddTask(path1, id, ptask, stage);
	return TRUE;
}
BOOL cDialogTrigger::sDialogTrigger::AddTaskOriginal(int mode, cTaskFather* ptask, int stage,ifstream& File)
{
	m_Mode = mode;
	int textnum;
	File >> textnum;
	if (0 == textnum)return FALSE;
	m_DialogList.resize(textnum);
	string dustbin;
	getline(File, dustbin);
	for (int i = 0; i < textnum; i++)
	{
		getline(File, m_DialogList[i].name);
		if (m_DialogList[i].name == "����")
			m_DialogList[i].name = g_pCharacter->m_PCData.m_Name;
		getline(File, m_DialogList[i].context);
	}
	m_pTask = ptask;
	m_Stage = stage;
	m_Nowtext = 0;
	return TRUE;
}
BOOL cDialogTrigger::AddTask(string path,int id, cTaskFather* ptask, int stage)
{
	ifstream File;
	File.open(path);
	int tag;
	string name;
	string text;
	int textnum;
	File >> tag;
	while (tag != id)
	{
		File >> m_Mode;
		File >> textnum;
		
		getline(File, text);
		for (int i = 0; i < textnum; i++)
		{
			getline(File, name);
			getline(File, text);
		}
		File >> tag;
	}
	File >> m_Mode;
	switch (m_Mode)
	{
	case 1:
		m_datalist[0].AddTaskOriginal(m_Mode, ptask, stage, File);
		return Process(0);
	case 2:
		m_datalist[1].AddTaskOriginal(m_Mode, ptask, stage, File);
		return Process(1);
	default:
		ERRBOX;
		break;
	}
	return FALSE;
}

BOOL cDialogTrigger::sDialogTrigger::Process(BOOL skip /*= FALSE*/)
{
	if (!m_pTask)
		return TRUE;
	if (!skip)
	{
		if (m_Nowtext < m_DialogList.size())
		{
			if (1 == m_Mode) //1�������Ի�
				g_pMainState->m_Dialog.Talk(&m_DialogList[m_Nowtext]);
			else
			if (2 ==m_Mode)//2������ͨ�Ի�
			{
				g_pMainState->m_Channel.AddNewString(m_DialogList[m_Nowtext].context,m_DialogList[m_Nowtext].name, 1);
			}
			m_Nowtext += 1;
			return TRUE;
		}
	}
	m_Nowtext = -1;
	cTaskFather* p = m_pTask;
	m_pTask = 0;
	m_DialogList.clear();
	p->Process(m_Stage);
	return FALSE;
}
BOOL cDialogTrigger::Process(int id,BOOL skip)
{
	return m_datalist[id].Process(skip);
}

void cDialogTrigger::ReMove()
{
	m_datalist[0].m_pTask = 0;
	m_datalist[1].m_pTask = 0;
}

void cDialogTrigger::Save(ofstream& File)
{
	if (m_datalist[0].m_Nowtext>-1)
		g_pMainState->m_Dialog.Close();
	m_datalist[1].Save(File);
}
void cDialogTrigger::Load(ifstream& File)
{
	m_datalist[0].Reset();
	m_datalist[1].Load(File);
}

void cDialogTrigger::AddSelect(cTaskFather* p, string text, string name, int cancelret, string select0, int ret0, string select1, int ret1, string select2, int ret2, string select3, int ret3)
{
	cInterfaceDialog& d=g_pMainState->m_Dialog;
	d.Reset();
	d.SetObj(name);
	d.SetString(text, select0, select1, select2, select3);
	d.SetTrigger(p,cancelret, ret0, ret1, ret2, ret3);
	d.OnOff(TRUE);
}

cDialogTrigger::cDialogTrigger()
{
	m_datalist.resize(2);
}



void cTriggerTime::Process()
{
	cShiCheng& sc = g_pMainState->m_InterfaceDoor.m_TimeShiCheng;
	int y = sc.year;
	int m = sc.month;
	int d = sc.day;
	int h = sc.hour;
	for (int i = 0; i<m_list.size();i++)
	{
		sTimeTrigger& t = m_list[i];
		if (t.m_y > y)continue;;
		if (t.m_y - y > t.m_y2)
		{
			m_list.erase(m_list.begin() + i);
			return Process();
		}
		if (t.m_m > m)continue;
		if (t.m_m - m > t.m_m2)
		{
			m_list.erase(m_list.begin() + i);
			return Process();
		}
		if (t.m_d > d)continue;
		if (t.m_d - d > t.m_d2)
		{
			m_list.erase(m_list.begin() + i);
			return Process();
		}
		if (t.m_h > h)continue;
		if (t.m_h - h > t.m_d2)
		{
			m_list.erase(m_list.begin() + i);
			return Process();
		}
		cTaskFather* task = m_list[i].m_pTask;
		int stage = m_list[i].m_Stage;
		m_list.erase(m_list.begin() + i);
		task->Process(stage);
		return Process();
	}
}

void cTriggerTime::Save(ofstream& File)
{
	File << " ";
	File << m_list.size()<<" ";
	for (int i = 0; i < m_list.size(); i++)
	{
		File << m_list[i].m_h << " ";
		File << m_list[i].m_d << " ";
		File << m_list[i].m_m << " ";
		File << m_list[i].m_y << " ";
		File << m_list[i].m_h2 << " ";
		File << m_list[i].m_d2 << " ";
		File << m_list[i].m_m2 << " ";
		File << m_list[i].m_y2 << " ";
	}
}

void cTriggerTime::Load(ifstream& File)
{
	int num;
	File >> num;
	m_list.resize(num);
	for (int i = 0; i < num; i++)
	{
		File >> m_list[i].m_h ;
		File >> m_list[i].m_d ;
		File >> m_list[i].m_m ;
		File >> m_list[i].m_y ;
		File >> m_list[i].m_h2 ;
		File >> m_list[i].m_d2 ;
		File >> m_list[i].m_m2 ;
		File >> m_list[i].m_y2 ;

	}
}

void cTriggerTime::Add(cTaskFather* task, int stage, int h /*= -1*/, int d /*= -1*/, int m /*= -1*/, int y /*= -1*/, int y2 /*= 0*/, int m2 /*= 0*/, int d2 /*= 0*/, int h2 /*= 0*/)
{
	cShiCheng& t = g_pMainState->m_InterfaceDoor.m_TimeShiCheng;
	if (y == -1)y = t.year;
	if (m == -1)m = t.month;
	if (d == -1)d = t.day;
	for (int i = 0; i < m_list.size(); i++)
	{
		if (m_list[i].m_pTask == task)
		{
			m_list[i].m_y = y;
			m_list[i].m_d = d;
			m_list[i].m_m = m;
			m_list[i].m_h = h;
			m_list[i].m_y2 = y2;
			m_list[i].m_d2 = d2;
			m_list[i].m_m2 = m2;
			m_list[i].m_h2 = h2;
			m_list[i].m_Stage = stage;
		}
	}
	sTimeTrigger lis;
	lis.m_y = y;
	lis.m_d = d;
	lis.m_m = m;
	lis.m_h = h;
	lis.m_y2 = y2;
	lis.m_d2 = d2;
	lis.m_m2 = m2;
	lis.m_h2 = h2;
	lis.m_Stage = stage;
	lis.m_pTask = task;
	m_list.push_back(lis);
}

void cTriggerTime::Remove(cTaskFather* task)
{
	for (int i = 0; i < m_list.size(); i++)
	{
		if (m_list[i].m_pTask == task)
		{
			m_list.erase(m_list.begin() + i);
			return;
		}
	}
}

void cDialogTrigger::sDialogTrigger::Save(ofstream& File)
{
		if (!m_pTask)
		{
			File <<-1<<" ";
			return;
		}
		else
		{
			File << m_pTask->GetType() << " ";
			File << m_Mode << " ";
			File << m_DialogList.size() << " ";
			for (int i = 0; i < m_DialogList.size(); i++)
			{
				m_DialogList[i].Save(File);
			}
		}
}

void cDialogTrigger::sDialogTrigger::Load(ifstream& File)
{
	Reset();
	int id;
	File >> id;
	if (-1 == id)return;
	m_pTask = g_pMainState->FindTaskByID(id);
	File >> m_Mode;
	int size;
	File >> size;
	if (!size)return;

	m_DialogList.resize(size);
	for (int i = 0; i < size; i++)
	{
		m_DialogList[i].Load(File);
	}
}





BOOL cTaskHuoDong::Process(int stage)
{
	if (m_HuoDongID == -1)return TRUE;
	switch (m_HuoDongID)
	{
	case ���ɴ���:
		Processmp(stage);
		break;
	case �ƾ�:
		break;
	default:
		ERRBOX;
		break;
	}
	return TRUE;
}

BOOL cTaskHuoDong::Processmp(int stage)
{
	
	switch (stage)
	{
	case -1://�ʱ�䵽
		break;
	case 0:
		
		{	
			Init();
			for (int i = 0; i < 13; i++)
			{
				AddTriggerObjMap(&m_List[i]);
			}
			m_HuanShu = 0;
			UpdateJob("ȥ����107,200�μ�12����");
		}
		break;
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
	
		{
			if (stage != m_Stage+1)
			{
				return TRUE;
			}
			m_NowNpc = g_pMainState->m_pNowSelect;
			ReadytoFightmp(m_List[m_Stage], (eMengPai)m_MengPaiId[m_Stage]);
			AddTriggerFightSuccess(15);
		}
		break;
	case 13:
	
		{
			m_NowNpc = g_pMainState->m_pNowSelect;
			m_NowNpc->Talk("Ҫ��ȡ������?", this, 14, "��", "��");
		}
		break;
	case 14:
	{
			   if (0 != g_pMainState->m_Dialog.m_retSelect)return TRUE;
			   if (m_Stage > -1)return TRUE;
	}
	case -14:
	
		{//��������
			if (!m_Idlist.size())
			{
				m_Idlist.resize(12);
				for (int i = 0; i < 12; i++)
					m_Idlist[i] = i;
			}
			int id = rand() % m_Idlist.size();
			m_Stage = m_Idlist[id];
			m_Idlist.erase(m_Idlist.begin() + id);
			string text = GetText();
			m_NowNpc->Talk(text);
			UpdateJob(text,m_HuanShu);
		}
		break;
	case 15:
		
		{
			SetJiangLi(8, TRUE, 120, 30);
			
			cJiangLi jiangli;
			jiangli.SetBaoShi(25);//��ʯ
			jiangli.Add(3, 27, 20, 3); //66
			jiangli.Add(33, 27, 20);//�ʹ�
			jiangli.AddLShouJue(10);//���޾�
			jiangli.AddHShouJue(5);//���޾�
			jiangli.Add(38, 27, 20, 4);//ǿ��ʯ
			jiangli.AutoGetJiangLi();
			m_HuanShu += 1;
			if (m_pTask)
			if (m_HuanShu == m_TriggerHuanShu)
			{
				cTaskFather* p = m_pTask;
				m_pTask = 0;
				RemoveTriggerObjMap();
				g_pMainState->m_Job.ReMoveJob(this);
				m_HuoDongID = -1;
				p->Process(m_TaskRet);	
				return TRUE;
			}
			Process(-14);
		}
		break;
	}
	return TRUE;
}



std::string cTaskHuoDong::GetText()
{
	ostringstream oss;
	cMengPai* pmp = g_pMainState->m_SkillManager.m_MengPaiManager.GetMengPai(m_Stage);
	oss << "��һ��Ϊ#R";
	oss << pmp->m_Name;
	oss << "#R";
	return oss.str();
}

void cTaskHuoDong::Start(eHuoDong id)
{
	m_HuoDongID = id;
	Init();
	Process(0);
}

void cTaskHuoDong::Initmp()
{
	if (m_bInit12)return;
	m_bInit12 = TRUE;
	ifstream File;
	ostringstream oss;
	oss << g_strMediaPath;
	oss << "����//12����.txt";
	File.open(oss.str());
	if (!File)ERRBOX;
	for (int i = 0; i < 13; i++)
	{
		File >> m_MengPaiId[i];
		m_List[i].m_Npc.Load(File);
		m_List[i].m_pTask = this;
		m_List[i].m_Stage = i + 1;
	}
	m_Idlist.resize(12);
	for (int i = 0; i < 12; i++)
		m_Idlist[i] = i;
	m_Stage = -1;
	File.close();
}

void cTaskHuoDong::Init()
{
	switch (m_HuoDongID)
	{
	case ���ɴ���:
		Initmp();
		break;
	case �ƾ�:
		break;
	default:
		ERRBOX;
		break;
	}
}

void cTaskHuoDong::ReadytoFightmp(sMapObjTrigger& list,eMengPai mp)
{
	int enemynum =  g_pMainState->AtutoGetEnemyNum();
	if (enemynum > 9)enemynum = 9;
	int lv = g_pCharacter->m_PCData.m_LV;
	sZiZhi zizhi;
	zizhi.Set(1200 + m_HuanShu * 3, 800 + m_HuanShu * 3, 2000 + m_HuanShu * 4, 4000 + m_HuanShu * 10, 1000 + m_HuanShu * 2, 1000 + m_HuanShu * 2, 100 + (m_HuanShu >> 2));	
	cPetData* pPet= AddPCEnemyF(lv * 200, list.m_Npc.m_modeid, &zizhi, -1, mp, list.m_Npc.m_name, list.m_Npc.weapontype, list.m_Npc.weaponid);
	pPet->AddSkill(ePS_����);
	int petrand;
	for (int i = 0; i < enemynum; i++)
	{
		petrand = 10 + m_HuanShu;
	 	petrand= rand() % petrand+(m_HuanShu>>1);
		while (petrand>93)
		{
			petrand -= rand()%20;
		}
		AddPCPetEnemyF(lv*120,(ePetID)petrand);
	}
	SetMusic();
	g_StateType = STATE_FIGHT;
}

void cTaskHuoDong::UpdateJob(string text,int huanshu)
{
	sJob job;
	string title;
	switch (m_HuoDongID)
	{
	case ���ɴ���:title = "���ɴ���";
		break;
	case �ƾ�:title = "�ƾ�";
		break;
	default:
		break;
	}
	job.Set( title,text, -1, this,0 );
	g_pMainState->m_Job.AddaJob(job, huanshu);
}

void cTaskHuoDong::Load(ifstream& File)
{
	File >> m_HuoDongID;
	if (m_HuoDongID == -1)return;
	m_bInit12 = FALSE;
	Init();
	string str;
	File >> str;
	int num;
	File >> num;
	m_Idlist.resize(num);
	for (UINT i = 0; i < m_Idlist.size(); i++)
		File >> m_Idlist[i];
	File >> m_Stage;
	File >> m_HuanShu; 
	int k;
	File >> k;
	if (k)
	{
		File >> k;
		m_pTask = g_pMainState->FindTaskByID(k);
		File >> m_TaskRet;
		File >> m_TriggerHuanShu;
	}
	
}

void cTaskHuoDong::Save(ofstream& File)
{
	File << "\n";
	File << m_HuoDongID;
	if (m_HuoDongID == -1)return;
	File << " � ";
	File << m_Idlist.size() << " ";
	for (UINT i = 0; i < m_Idlist.size(); i++)
		File << m_Idlist[i] << " ";
	File << m_Stage << " ";
	File << m_HuanShu << " ";
	//�Ƿ��о���
	if (m_pTask)
	{
		File << 1 << " ";
		File << m_pTask->GetType() << " ";
		File << m_TaskRet << " ";
		File << m_TriggerHuanShu << " ";
	}
	else File << 0 << " ";
}



void sMapObjTrigger::Save(ofstream& File)
{
	m_Npc.Save(File);
	if (m_pTask)
	{
		File << 1 << " ";
		File << m_Stage << " ";
		File << m_pTask->GetType() << " ";
	}
	else File << 0 << " ";
}

void sMapObjTrigger::Load(ifstream& File)
{
	m_Npc.Load(File);
	BOOL havetrigger;
	File >> havetrigger;
	if (havetrigger)
	{
		File >> m_Stage;
		int id;
		File >> id;
		m_pTask = g_pMainState->FindTaskByID(id);
	}
	else
	{
		m_pTask = 0;
	}
}

void sMapObjTrigger::operator=(sFriendList& data)
{
	m_Npc.m_name = data.m_Name;
	if (data.m_modeid<1000)
		m_Npc.m_modeid = 2000+data.m_modeid;
	else
		m_Npc.m_modeid =  data.m_modeid;
	//��������
	m_Npc.weapontype = data.m_weapontype;
	m_Npc.weaponid = data.m_weaponid;
	m_Npc.m_JinJieID = data.m_JinJieNum;
	//Ⱦɫ
	m_Npc.m_Ranse = data.m_Ranse;
	//Ŀǰ��д����,����Ҫ����
}

void cTriggerGive::ReMove(cTaskFather* ptask)
{
	for (int i = 0; i < m_List.size(); i++)
	{
		if (m_List[i].m_ptask->GetType() == ptask->GetType())
		{
			m_List.erase(m_List.begin() + i);
			return ReMove(ptask);
		}
	}
}

void cTriggerGive::ReMove(int id)
{
	m_List.erase(m_List.begin() + id);
}

void cTriggerGive::AddTask(string name, int itemid, int itemtype, cTaskFather* ptask, int ret, BOOL bHuChi)
{
	if (bHuChi)ReMove(ptask);
	sTriggerGive data;
	data.Set(name, itemid, itemtype, ptask, ret);
	m_List.push_back(data);
}

BOOL cTriggerGive::Process(string name, int id, int type)
{
	int num = m_List.size();
	if (!num)return FALSE;
	for (int i = 0; i < num; i++)
	{
		if (m_List[i].m_name == name)
		if (m_List[i].m_itemtype==type)
		if (m_List[i].m_itemid == id)
		{
			cTaskFather* p = m_List[i].m_ptask;
			int stage = m_List[i].m_ret;
			ReMove(i);
			p->Process(stage);
			return TRUE;
		}
	}
	return FALSE;
}

void cTriggerGive::Save(ofstream& File)
{
	File << "\n";
	File << m_List.size()<<" ";
	for (int i = 0; i < m_List.size(); i++)
		m_List[i].Save(File);
	File << "\n";
}
void cTriggerGive::Load(ifstream& File)
{
	int num;
	File >>num ;
	m_List.resize(num);
	for (int i = 0; i < num; i++)
		m_List[i].Load(File);
}
void cTriggerGive::sTriggerGive::Save(ofstream& File)
{
	File << " ";
	if (m_name == "")
	{
		ERRBOX;
	}
	else
		File << m_name << " ";
	File << m_itemid << " ";
	File << m_itemtype << " ";
	File << m_ptask->GetType() << " ";
	File << m_ret << " ";
}
void cTriggerGive::sTriggerGive::Load(ifstream& File)
{
	File >> m_name;
	File >> m_itemid;
	File >> m_itemtype;
	int id;
	File >> id;
	m_ptask = g_pMainState->FindTaskByID(id);
	File >> m_ret;
}
