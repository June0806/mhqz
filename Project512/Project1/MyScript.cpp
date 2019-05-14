#include "global.h"
BOOL cMyScript::ChangeMap(cObj* p)
{
	sAction* pAction = GetAction(p);
	p->m_Script.m_NowScript = 0;
	p->m_AutoRun = FALSE;
	return g_pMainState->m_Map.LoadMap(pAction->pEntry[1].intData, pAction->pEntry[2].intData, (char*)pAction->pEntry[0].stringData.c_str());
}
BOOL cMyScript::Process(cObj* p)
{
	if (0 == p)return FALSE;
	if (!p->m_bHaveScript)return FALSE;
	if (!p->m_bScriptLoad)
	{
		if (p->m_ScriptPath == "��")
		{
			SetGlobalButton(p->m_ScriptOneType, p->m_ScriptOneId);
			return TRUE;
		}
		else
		{
			p->m_bScriptLoad = TRUE;
			//��ȡ�ű�
			ostringstream oss;
			oss << g_strMediaPath << "script\\common\\"<<p->m_ScriptPath<<".txt";
			if (!p->m_Script.LoadScript(oss.str()))
			{
				p->m_bHaveScript = FALSE;
				return FALSE;
			}
		}
	}
	if (m_ActionList.size() == 0)return 0;
	p->m_AutoRun = TRUE;
	while (p->m_AutoRun)
	{
		if (p->m_Script.m_NowScript >= m_ActionList.size())return TRUE;
		sAction*  pAction = GetAction(p);
		switch (pAction->ID)
		{
		case 0:TalkMySelf(p);break;//����
		case 1:  NPCSelect2(p);break;
		case 2:  //2ѡ
		case 3:  //3ѡ
		case 4:  //4ѡ
			SelectNum(p, pAction->ID);break;
		case 5:EndScript(p);break;
		case 6: Case(p); break;
		case 7:Break(p);break;
		case 8:
			 Default(p);
			break;
		case 9: //����
			ChangeMap(p);
			return TRUE;
		case  10:  SetPoint(p);break;//����ָ��λ��
		case 15:Talk(p);break;//�²�
		case 16: 
			if (1)
			{
				p->m_Script.m_NowScript += 1;
				p->m_AutoRun = TRUE;
				SetGlobalButton(pAction->pEntry[0].intData, pAction->pEntry[1].intData,p);
			}
			break;//���ÿ���
		case 23:  //���
			p->m_Script.m_NowScript += 1;
			break;
		case 40:
			p->m_Script.m_NowScript += 1;
			p->m_AutoRun = FALSE;
			break;
		case 41:
			g_pMainState->m_Dialog.OnOff(FALSE);
			p->m_Script.m_NowScript += 1;
			p->m_AutoRun = FALSE;
			break;
		default:
			ERRBOX;
			return 0;
		}
		
	}
	return TRUE;
}
//�Ƿ���
BOOL cMyScript::NPCSelect2(cObj* p)
{
	g_pMainState->m_Dialog.Reset();
	sAction* pAction = GetAction(p);
	g_pMainState->m_Dialog.SetObj(p->m_TrueName.GetString());
	g_pMainState->m_Dialog.m_pObj =p;
	g_pMainState->m_Dialog.SetString(pAction->pEntry[0].stringData, "��", "��");
	g_pMainState->m_Dialog.OnOff(TRUE);
	p->m_Script.m_NowScript += 1;
	p->m_AutoRun = FALSE;
	return TRUE;
}

BOOL cMyScript::LoadScript(string name)
{
	ifstream  File;
	File.open(name);
	if (!File)return FALSE;
	//CMainState* pMainState = g_pMainState;
	m_pActionTemplateList = &g_pMainState->m_ScriptTemplate.m_ActionTemplateList;
	m_ActionList.clear();
	while (true)
	{
		sAction  Action;
		File >> Action.ID;	
		File >> Action.NumOfEntry;
		
		if (Action.ID == 25)//��ȡ�Ի���
		{
			int entrytype;
			File >> entrytype;

			string dialogscriptname;
			File >> dialogscriptname;
			ifstream File2;
			ostringstream oss;
			oss << g_ScriptPath;
			oss << dialogscriptname;
			oss << ".txt";
			File >> entrytype;

			int id;
			File >> id;
			File2.open(oss.str());
			//Ѱ�ҶԻ�ID
			int id2;
			File2 >> id2;
			while (id2!=id)
			{
				int k;
				File2 >> k;
				string dustbinstr;
				for (int i = -1; i < k; i++)
				{
					getline(File2, dustbinstr);
				}
				if (File.eof())
				{
					return FALSE;
				}
				File2 >> id2;
			}
			//
			int k;
			File2 >> k;
			int chane;  //Ƶ��
			File2 >> chane;
			for (int i = 0; i < k; i++)
			{
				sAction  Action1;
				Action1.ID = 15;
				Action1.NumOfEntry = 3;
				Action1.pEntry = new sEntry[Action1.NumOfEntry];
				Action1.pEntry[0].Type = 1;
				Action1.pEntry[1].Type = 1;
				Action1.pEntry[2].Type = 2;
				File2>>Action1.pEntry[0].stringData;
				File2 >> Action1.pEntry[1].stringData;
				Action1.pEntry[2].intData =chane ;
				m_ActionList.push_back(Action1);
			}
			continue;
		}

		if (File.eof())
		{
			return TRUE;
		}
		Action.pEntry = new sEntry[Action.NumOfEntry];
		for (int k = 0; k < Action.NumOfEntry; k++)
		{
			File >> Action.pEntry[k].Type;
			switch (Action.pEntry[k].Type)
			{
			case SENTRYTYPE_INT:
				File >> Action.pEntry[k].intData;
				break;
			case SENTRYTYPE_FLOAT:
				File >> Action.pEntry[k].floatData;
				break;
			case SENTRYTYPE_BOOL:
				File >> Action.pEntry[k].intData;
				break;
			case SENTRYTYPE_STRING:
				if (1)
				{
					getline(File, Action.pEntry[k].stringData);
					if (Action.pEntry[k].stringData.size() < 3)
					{
						Action.pEntry[k].stringData = "";
						getline(File, Action.pEntry[k].stringData);
					}
				}
				break;
			default:
				MessageBoxA(0, "δ֪����", "��ȡ", MB_OK);
				break;
			}
		}
		m_ActionList.push_back(Action);
	
	}
}

BOOL cMyScript::Case(cObj* p)
{
	p->m_AutoRun = TRUE;
	sAction* pAction = GetAction(p);
	//�������ȷ��ѡ��
	if (p->m_ScriptSelect == pAction->pEntry[0].intData)
	{
		p->m_Script.m_NowScript+=1;
		return TRUE;
	}
	else  //���������ȷ��ѡ��,��Ѱ��
	{

		p->m_Script.m_NowScript+=1;
		pAction = GetAction(p);
		//�ҵ���һ��CASE �� DEFAULT
		while ((6 != pAction->ID) && (8 != pAction->ID))
		{
			p->m_Script.m_NowScript += 1;
			pAction = GetAction(p);
			if (0 == pAction)
			{
				p->m_AutoRun = FALSE;
				m_NowScript = 0;
				return TRUE;
			}
		}
        return p->m_Script.Process(p);
	}
}

BOOL cMyScript::Default(cObj* p)
{
	p->m_AutoRun = TRUE;
	p->m_Script.m_NowScript+=1;
	return	TRUE;
}

BOOL cMyScript::Break(cObj* p)
{
	p->m_Script.m_NowScript += 1;
	sAction* pAction = GetAction(p);
	//�ҵ���һ��DEFAULT
	while (8 != pAction->ID)
	{
		p->m_Script.m_NowScript += 1;	
	}
	//�ҵ���һ��break;
	pAction = GetAction(p);
	while(7 != pAction->ID)
	{
		p->m_Script.m_NowScript += 1;
	}

	p->m_AutoRun = FALSE;
	p->m_Script.m_NowScript += 1;
	return TRUE;
}

cMyScript::cMyScript()
{
	//m_bHaveScript = FALSE;
	m_NowScript = 0;
}

void cMyScript::Free()
{
	//m_bHaveScript = FALSE;

	m_NowScript = 0;
	for (int i = 0; i < m_ActionList.size(); i++)
	{
		SAFE_DELETE_ARRAY(m_ActionList[i].pEntry);
	}
	vector<sAction> dustbin;
	m_ActionList.swap(dustbin);
}

BOOL cMyScript::SetPoint(cObj* p)
{
	sAction* pAction = GetAction(p);
	int k = pAction->pEntry[0].intData;
	while (true)
	{
		if (pAction->pEntry[1].intData)
		{//����
			p->m_Script.m_NowScript -= 1;
		}
		else
		{//����
			p->m_Script.m_NowScript += 1;
		}

		if (p->m_Script.m_NowScript < 0)
		{
			p->m_Script.m_NowScript = 0;
			return FALSE;
		}

		sAction* pAction1 = GetAction(p);
		if (pAction1->ID == 23)
		{
			if (pAction1->pEntry[0].intData == k)
			{
				p->m_Script.m_NowScript += 1;
				p->m_AutoRun = FALSE;
				return TRUE;
			}
		}
	
	}
	return TRUE;
}

sAction* cMyScript::GetAction(cObj* p)
{
	if (p->m_Script.m_NowScript > p->m_Script.m_ActionList.size() - 1 || p->m_Script.m_NowScript < 0)
	{
		ERRBOX;
		return 0;
	}
	return &p->m_Script.m_ActionList[p->m_Script.m_NowScript];
}





BOOL cMyScript::SetGlobalButton(int stype, int sid, cObj* p)
{
	
	switch (stype)
	{
	case 62://�ϼ�����
		if (1)
		{
			p->m_AutoRun = FALSE;
			g_pMainState->m_TaskNormal.Process(12);
		}
		break;
	case 1010:
	case 1011:
	case 1012:
	case 1015:
	case 1016:
		g_pMainState->m_ZhiXian.SetChapter(stype-1010);
		g_pMainState->m_ZhiXian.Process(sid);
		break;
	case 1014://ѡ��
		g_pMainState->m_ZhiXian.SetChapter(stype - 1010);
		g_pMainState->m_ZhiXian.ProcessSelect(sid);
		break;
	case 1013://����
		g_pMainState->m_ZhiXian.Process3(sid);
		break;
	case 2001:
		if (1)
		{
			cPCData* pdata = &g_pCharacter->m_PCData;
			vector<int>  idlist;
			int money=0;
			int id;
			for (int i = pdata->m_NumofBB-1; i >=0; i--)
			{
				id = pdata->m_pPetDataList[i]->GetData()->m_ID;
				if (pdata->m_FightPetID == i)continue;
				if ( id< 2)
				{	
					if (id == 0)money += 350;
					else money += 250;
					idlist.push_back(i);
				}
			}
			if (money)
			{
				for (int i = 0; i < idlist.size(); i++)
				{
					pdata->DeletePet(idlist[i]);
				}

				pdata->m_Money[0] += money;
				g_pMainState->m_Tags.GetMoneyAndExp(money);
			}
		}
		break;
	case 1000://�ܼ�
		if (1)
		{
			cObj* pObj= g_pMainState->m_pNowSelect;
			int huanjin = g_pMainState->m_FangWu.GetHuanJin();
			ostringstream oss;
			oss << "����,��ǰ������:#R";
			oss << huanjin;
			oss << "#R��ģ:#R";
			oss << g_pMainState->m_FangWu.GetGuiMo();
			oss << "#R������ʲô�Ը�?";
			string context;
			context = oss.str();

			g_pMainState->m_Dialog.Reset();
			g_pMainState->m_Dialog.SetObj(pObj);

			g_pMainState->m_Dialog.SetString(context, "��Ϣ", "����լ��", "��������", "�����һ���Ҿ�");
			g_pMainState->m_Dialog.SetTrigger(&g_pMainState->m_TaskNormal, -1,8,118,119,120);
			g_pMainState->m_Dialog.OnOff(TRUE);
		}
		break;
	case 30://��ͼ
		if (1)
		{
			g_pMainState->m_TaskBaoTu.CheckInit();
			g_pMainState->m_TaskBaoTu.Process(sid);
		
		}
		break;
	case 11:
		if (1)
		{
			g_pMainState->m_TaskBangPai.CheckInit();
			g_pMainState->m_TaskBangPai.Process(sid);
		}
		break;
	case 12:
		g_pMainState->m_LearnBangPaiSkill.OnOff(TRUE);
		break;
	case 13:
		break;
	case 14:
		g_pMainState->m_LearnXiuLian.CheckInit();
		g_pMainState->m_LearnXiuLian.OnOff(TRUE);
		break;
	case 16:
		g_pMainState->m_YingHang.SetMode(sid);
		g_pMainState->m_YingHang.OnOff(TRUE);
		break;
	case 1040://ץ��
		g_pMainState->m_TaskZhuaGui.Process(0);
		break;
	case 1001://Ⱦɫ
		if (1)
		{
			g_pMainState->m_InterfaceRanSe.SetMode(p->m_ScriptSelect,g_pMainState->m_HeroID);
			g_pMainState->m_InterfaceRanSe.OnOff(TRUE);
		}
		break;
	case 1002:
		if (1)
		{
			g_pMainState->m_InterFaceBuy.Open(sid);
		}
		break;
	case 1003:
		g_pMainState->m_taskSM.Process(sid); break;
		break;
	case 1004://���ܵ���
		switch (sid)
		{
		case 0://����װ��
		case 1://�չ�װ��
		case 2://��̨����
		case 3:
			if (1)
			{
				if (g_pMainState->m_Map.m_MapName == "�������")
				{
					g_pMainState->m_InterfaceJianDing.SetMode(3);
				}
				else
				{
					g_pMainState->m_InterfaceJianDing.SetMode(sid);
				}
				g_pMainState->m_InterfaceJianDing.OnOff(TRUE);
			}
			break;
		
		default:
			break;
		}
		break;
	case 1005:
		if (1)
		{
			int price = 0;
			for (int i = 0; i < g_pCharacter->m_PCData.m_NumofBB; i++)
			{
				cPetData& petdata = g_pCharacter->m_PCData.m_pPetDataList[i]->m_PetDataZZ;
				if (petdata.m_HP == petdata.m_HPMax.GetData() && petdata.m_MP == petdata.m_MPMax.GetData())continue; //û���˵��ٻ��޲�����
				price += petdata.m_LV*((petdata.m_NumofBBSkill) * 5+petdata.m_ZiZhi.m_Growing+petdata.m_ZiZhi.m_HPZZ/200) + 1;
			}

			for (int i = 0; i < g_pCharacter->m_PCData.m_NumofBB; i++)
			{
				g_pCharacter->m_PCData.m_pPetDataList[i]->m_PetDataZZ.FillHPMP();
			}
			if (g_pCharacter->m_PCData.AddMoney(-price))
			{
				ostringstream oss;
				oss << "������#R";
				oss << price;
				oss << "#R��,����ٻ����Ѿ��ָ�����";
				g_pMainState->m_Dialog.m_pObj->Talk(oss.str());
			}
			else
			{
				g_pMainState->m_Dialog.m_pObj->Talk("��ξ����յ��,����Ҫ�ƴ�����ٻ��ް�");
			}
		}
		break;
	case 1006:
		if (1)
		{	
			g_pCharacter->m_PCData.FillHPMP();
			switch (sid)
			{
			case 0://�Ƶ��ϰ�
				if (g_pCharacter->m_PCData.AddMoney(500, 0,0,0))
				{
					g_pMainState->m_Dialog.m_pObj->Talk("��ӭ�´�����");
				}
				else
				{
					g_pCharacter->m_PCData.m_Money[0] += 500;
					g_pMainState->m_Dialog.m_pObj->Talk("����,��#R500$R��������,�������");
				}
				break;
			case 1://�³���
				g_pMainState->m_Dialog.m_pObj->Talk("���Ѿ��ָ�����");
				break;
			default:
				ERRBOX;
				break;
			}
			
		}
		break;
	case 1007:
		if (1)
		{
			g_pMainState->m_InterFaceBuyPet.Init2(0, 0, 0);
			g_pMainState->m_InterFaceBuyPet.OnOff(TRUE);
		}
		break;
	case 1008:
		if (1)
		{
			if (!sid)
			{
				cObj* obj = g_pMainState->FindObjByName("���ɴ�����");
				g_pMainState->m_Dialog.Reset();
				if (g_pCharacter->m_PCData.m_MengPaiID > -1)
				{

					if (g_pCharacter->m_PCData.m_LV < 40)
					{
						g_pMainState->m_Dialog.SetObj(obj);
						g_pMainState->m_Dialog.SetString("Ҫ��������?", "��", "��");
						g_pMainState->m_Dialog.SetTrigger(&g_pMainState->m_TaskNormal,-1, 5);
						g_pMainState->m_Dialog.OnOff(TRUE);
					}
					else obj->Talk("�޷�����40�����ϵ���,���Լ�ʹ�ü��ܻ�ȥ");
				}
				else
				{
					g_pMainState->m_Dialog.SetObj(obj);
					cMengPai* pMengpai = g_pMainState->m_SkillManager.m_MengPaiManager.m_MengPai;
					switch (g_pCharacter->GetRace())
					{
					case 0:
					case 2://��					
						g_pMainState->m_Dialog.SetString("��ѡ��Ҫȥ������?", pMengpai[8].m_Name, pMengpai[9].m_Name, pMengpai[10].m_Name, pMengpai[11].m_Name);
						break;
					case 1:
					case 3://��
						g_pMainState->m_Dialog.SetString("��ѡ��Ҫȥ������?", pMengpai[4].m_Name, pMengpai[5].m_Name, pMengpai[6].m_Name, pMengpai[7].m_Name);
						break;
					case 4:
					case 5:
					case 6://��
						g_pMainState->m_Dialog.SetString("��ѡ��Ҫȥ������?", pMengpai[0].m_Name, pMengpai[1].m_Name, pMengpai[2].m_Name, pMengpai[3].m_Name);
						break;
					}
					g_pMainState->m_Dialog.SetTrigger(&g_pMainState->m_TaskNormal,-1, 6,6,6,6);
					g_pMainState->m_Dialog.OnOff(TRUE);
				}
			}
			else
			{
				if (g_pCharacter->m_PCData.m_MengPaiID >-1)
				{
					string mapname = g_pMainState->m_Map.m_MapName;
					string mapname2 = g_pMainState->m_SkillManager.m_MengPaiManager.GetMengPai(g_pCharacter->m_PCData.m_MengPaiID)->m_XunLuoMapName;
					if (0 == mapname2.compare(mapname))
					{
						g_pMainState->m_Dialog.Reset();
						g_pMainState->m_Dialog.SetObj(g_pMainState->m_pNowSelect);
						g_pMainState->m_Dialog.SetString("Ҫ�س�����","��","��");
						g_pMainState->m_Dialog.SetTrigger(&g_pMainState->m_TaskNormal,-1, 7);
						g_pMainState->m_Dialog.OnOff(TRUE);
						return TRUE;
					}
					g_pMainState->m_pNowSelect->Talk("�����ͷǱ��ŵ���");
					return TRUE;
				}
				
				g_pMainState->m_Dialog.Reset();
				g_pMainState->m_Dialog.SetObj(g_pMainState->m_pNowSelect);
				g_pMainState->m_Dialog.SetString("Ҫ�س�����", "��", "��");
				g_pMainState->m_Dialog.SetTrigger(&g_pMainState->m_TaskNormal,-1, 7);
				g_pMainState->m_Dialog.OnOff(TRUE);
				return TRUE;
			}
		}
		break;
	case 1009:
		if (g_pMainState->m_FangWu.GetGuiMo() < 0)
		{
			g_pMainState->m_Dialog.m_pObj->Talk("����,���ס��.......");
			return TRUE;
		}
		g_pMainState->m_FangWu.GoIn();
		return TRUE;
	default:
		return TRUE;
		break;
	}
	return TRUE;
}

BOOL cMyScript::Talk(cObj* p)
{
	sAction* pAction = GetAction(p);
	m_DialogueStyle = pAction->pEntry[2].intData;
	//����Ի�
	if (m_DialogueStyle == -1)
	{
		g_pMainState->m_Dialog.Reset();
		g_pMainState->m_Dialog.SetObj(pAction->pEntry[0].stringData);
		g_pMainState->m_Dialog.m_pObj = p;
		g_pMainState->m_Dialog.m_ContextList.SetFaceText(pAction->pEntry[1].stringData,g_pMainState->m_hFont[0],470);
		g_pMainState->InterfaceSetShow(&g_pMainState->m_Dialog, TRUE);

		//����λ��
		int  y = g_pMainState->m_Dialog.m_DialogBack.m_Picture.m_y + 20;
		int xStart = g_pMainState->m_Dialog.m_DialogBack.m_Picture.m_x + 5;
		for (int i = 0; i < g_pMainState->m_Dialog.m_ContextList.m_LineNum; i++)
		{
			y += g_pMainState->m_Dialog.m_ContextList.m_TextLineList[i].m_Height + 2;
			cText& text = g_pMainState->m_Dialog.m_ContextList.m_TextLineList[i].m_line;
			text.SetXY(xStart, y - 18);
			if (1 == g_pMainState->m_Dialog.m_ContextList.m_LineNum && 0 == g_pMainState->m_Dialog.m_ContextList.m_TextLineList[i].m_NumFace)
			{
			}
			else
			{
				//����б���,����ʾ����
				for (int j = 0; j < g_pMainState->m_Dialog.m_ContextList.m_TextLineList[i].m_NumFace; j++)
				{
					g_pMainState->m_Dialog.m_ContextList.m_TextLineList[i].m_Face[j].m_x = xStart + g_pMainState->m_Dialog.m_ContextList.m_TextLineList[i].m_xFace[j];
					g_pMainState->m_Dialog.m_ContextList.m_TextLineList[i].m_Face[j].m_y = y + 2;
				}
			}
		}

	}

	//
	if (m_DialogueStyle > -1)
	{
		g_pMainState->m_Channel.m_pCurrentChannel = &g_pMainState->m_Channel.m_Channel[m_DialogueStyle];
		m_SleepTime = 210;
		g_pMainState->m_Channel.AddNewString(pAction->pEntry[1].stringData, pAction->pEntry[0].stringData,pAction->pEntry[2].intData);
		//Sleep(60);
	}

	p->m_Script.m_NowScript += 1;
	p->m_AutoRun = FALSE;
	return TRUE;
}

BOOL cMyScript::SelectNum(cObj* p,int num)
{
	sAction* pAction = GetAction(p);
	g_pMainState->m_Dialog.Reset();
	g_pMainState->m_Dialog.SetObj(pAction->pEntry[0].stringData);
	g_pMainState->m_Dialog.m_pObj = p;  //???????????
	switch (num)
	{
	case 2:
		g_pMainState->m_Dialog.SetString(pAction->pEntry[1].stringData, pAction->pEntry[2].stringData, pAction->pEntry[3].stringData);
		break;
	case 3:
		g_pMainState->m_Dialog.SetString(pAction->pEntry[1].stringData, pAction->pEntry[2].stringData, pAction->pEntry[3].stringData,  pAction->pEntry[4].stringData);
		break;
	case 4:
		g_pMainState->m_Dialog.SetString(pAction->pEntry[1].stringData, pAction->pEntry[2].stringData, pAction->pEntry[3].stringData, pAction->pEntry[4].stringData, pAction->pEntry[5].stringData);
		break;
	default:
		ERRBOX;
		break;
	}
	g_pMainState->m_Dialog.OnOff(TRUE);
	p->m_Script.m_NowScript += 1;
	p->m_AutoRun = FALSE;
	return TRUE;
}

BOOL cMyScript::EndScript(cObj* p)
{
	p->m_Script.Free();
	p->m_AutoRun = FALSE;
	return TRUE;
}

BOOL cMyScript::TalkMySelf(cObj* p)
{
	sAction* pAction = GetAction(p);
	g_pMainState->m_Dialog.Reset();
	g_pMainState->m_Dialog.SetObj(p->m_TrueName.GetString());
	g_pMainState->m_Dialog.SetString(pAction->pEntry[0].stringData);
	g_pMainState->m_Dialog.OnOff(TRUE);
	p->m_Script.m_NowScript += 1;
	p->m_AutoRun = FALSE;
	return TRUE;
}








