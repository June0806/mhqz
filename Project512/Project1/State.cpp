#include "Global.h"

BOOL CMainState::Init(void* pData)
{
	if (isInit)return TRUE;
	isInit = TRUE;
	
	Init2();
	
	Init3();
	
	isInit = TRUE;
	ResetAll();
	g_pMainState->m_InterfaceTeam.SetFormation(normal);
	g_pMainState->m_InterfaceTeam.SetFormation(normal, TRUE);
	g_pMainState->m_InterfaceDoor.m_TimeShiCheng.Init(g_FPS, 0, 1, 1, 7, 0, 0);
	m_ZhuXian.Process0(0);
	return TRUE;
}

BOOL CMainState::Frame(void* pData)
{
	
	if (!isInit)Init();	

	UpDateInput();
	ProcessInput();
	if (g_StateType != STATE_MAIN)
		return TRUE;
	if (g_pMainState->m_InterfaceDoor.m_TimeShiCheng.ss % 5 == 0)
	{
		TeamFollow();
	}
	MoveObj();
	
	ShowObj();
	if (0 == g_pMainState->m_GlobalButton[18])
	{
		ShowInterface();
	}
	ShowInterface2();
	m_Accelator.ProcessAccelator();


	if (g_pHeroObj->m_time > -2)
	{
		m_Map.CheckChuanSong();
	}
	m_Canvas.Show();
	m_Map.m_pMap->Follow(m_FightList[m_HeroID].m_pData);

	m_Mp3.CheckEnd();
	return TRUE;
}

BOOL CMainState::Free()
{
	g_StateType = STATE_GAMEOVER;
	if (m_ZhiXian3.m_bPK)
	{
		m_ZhiXian3.Process(-1);
	}
	m_ScriptTemplate.Free();
	//���ٻ�
	for (int i = 0; i < 20; i++)
	{
		if (m_FightList[i].isLive)	
		if (m_FightList[i].m_pData->GetType() == INDEXTYEP_CHARACTER)
		{
			cCharacter* pc = (cCharacter*)m_FightList[i].m_pData->m_pIndex;
			if (pc->m_PCData.m_FightPetID > -1)
			{
				SetFightPet(i, -1);
			}
		}
	}
	//�Ѵ��ڵĶ���ɾ��
	for (int i = 0; i < 20; i++)
	{
		g_pMainState->FreeFightList(i);
	}
	for (int i = 0; i < 20; i++)
	{
		SAFE_DELETE(m_FightList[i].m_pData);
	}
	

	m_Dialog.Free();
	m_Map.Free();
	
	SelectObject(g_DC, m_hOldFont);
	SelectObject(g_DC2, m_hOldFont2);
	for (int i = 0; i < 6;i++)
	DeleteObject(m_hFont[i]);

	m_InterfacePet.Free();

	//ɾ�������������Ķ���
	//g_pDirectSound->Release();
	return TRUE;
}

CMainState::~CMainState()
{
	Free();
	DeleteObject(m_blackbrush);
	SAFE_DELETE_ARRAY(m_MaskList);
}

void CMainState::LoadList()
{
	if (m_LoadList)return;
	m_LoadList = TRUE;

	
	ifstream File;
	ifstream File2;
	string strdustbin;
	int intdustbin;
	string path = g_strMediaPath;
	path += "����//��ɫ//��ɫ.txt";
	File.open(path.c_str());
	path = g_strMediaPath;
	path += "����//��ɫ//��ɫ����.txt";
	File2.open(path.c_str());
	getline(File2, strdustbin);
	int charnum2;
	File >> charnum2;
	m_pCharacterDataList.resize(charnum2);
	
	for (int i = 0; i < charnum2; i++)
	{
		m_pCharacterDataList[i].Read(File);
		//��ȡ����
		File2 >> intdustbin;
		getline(File2, strdustbin);
		for (int j = 0; j < m_pCharacterDataList[i].m_Pos.size(); j++)
			m_pCharacterDataList[i].m_Pos[j].m_Sound.Read(File2);
	}

	File.close();
	File2.close();

	ifstream File6;
	path = g_strMediaPath;
	path += "����//NPC//NPC.txt";
	File6.open(path.c_str());
	int npcnum;
	File6 >> npcnum;
	m_pNpcList.resize(npcnum);
	path = g_strMediaPath;
	path += "����//����//�����б�.txt";
	m_ZuoQi.Init(path.c_str());
	for (int i = 0; i < npcnum; i++)
	{
		File6 >> m_pNpcList[i].m_ModeId;
		File6 >> m_pNpcList[i].m_name;
		File6 >> m_pNpcList[i].m_dialog;
		File6 >> m_pNpcList[i].m_stand;
		File6 >> m_pNpcList[i].m_walk;
	}
	File6.close();


	m_ItemManage.Init();
	m_SkillManager.Init();
	m_PetDataManager.Init();

}

int CMainState::ReadInt(char*& point)
{
	char Line[256];
	for (int i = 0; i < 256; i++)
	{
		if (point[0] == '\r' || point[0] == '\n')
		{
			Line[i] = 0;
			point += 2;
			break;
		}
		Line[i] = point[0];
		point++;

	}

	return  atoi(Line);

}

BOOL CMainState::ReadName(char*& point, char*& pName)
{
	char Line[256];
	int i;
	for (i = 0; i < 256; i++)
	{
		if (point[0] == '\r' || point[0] == '\n')
		{
			Line[i] = 0;
			point += 2;
			break;
		}
		Line[i] = point[0];
		point++;

	}
	pName = new char[i+1];
	strcpy(pName, Line);
	return TRUE;
}

DWORD CMainState::ReadDWORD(char*& point)
{
	char Line[256];
	for (int i = 0; i < 256; i++)
	{
		if (point[0] == '\r' || point[0] == '\n')
		{
			Line[i] = 0;
			point += 2;
			break;
		}
		Line[i] = point[0];
		point++;
	}
	return strtoul(Line, 0, 16);

}

CMainState::CMainState()
{
	m_GlobalButton.resize(c_GlobalButtonNum);
	m_MaskList = new BOOL[1024];
	m_FightList.resize(20);
	for (int i = 0; i < c_InterfaceShowNum; i++)
	{
		m_Interface[i] = 0;
	}
	for (int i = 0; i < MAX_OBJ_NUM; i++)
	{
		m_pObj[i] = 0;
	}
	for (int i = 0; i < 20; i++)
	{
		m_FightList[i].m_pData = 0;
		m_FightList[i].isLive = FALSE;
	}
	m_ObjNumber = 0;
}



BOOL CMainState::FindObj()
{
	m_ObjNumber = 0;
	

	//��NPC����
	int xmap, ymap;
	xmap = m_Map.m_pMap->GetXCenter();
	ymap = m_Map.m_pMap->GetYCenter();
	for (int i = 0; i < m_Map.m_numberOfNpc; i++)
	{
		if (abs(m_Map.m_pNpc[i]->GetX() - xmap) < 640 && 
			abs(m_Map.m_pNpc[i]->GetY() - ymap) < 480)
		{
			if (m_Map.m_pNpc[i]->GetShow())
			{
				m_pObj[m_ObjNumber] = m_Map.m_pNpc[i];
				m_ObjNumber++;
			}
		}
	}
	for (int i = 0; i < 5; i++)
	{
		if (m_FightList[i].isLive)
		{
			m_pObj[m_ObjNumber] = m_FightList[i].m_pData;
			m_ObjNumber += 1;
		}
	}
	m_pObj[m_ObjNumber] = 0;

	m_NumShow = 0;
	for (int i = 0; i < m_ObjNumber; i++)
	{
		m_pShowTurn[m_NumShow] = m_pObj[i];
		m_ShowTurnType[m_NumShow] = 0;
		m_NumShow += 1;
	}
	int xgap, ygap;
	for (int i = 0; i < m_Map.m_numofJianZhu; i++)
	{
		CWFile* m_pFile = m_Map.m_pJianZhu[i];
		xgap = abs(m_pFile->GetX() - m_Map.m_pMap->GetXCenter());
		if (xgap>640)continue;
		ygap = abs(m_pFile->GetY() - m_Map.m_pMap->GetYCenter());
		if (ygap > 480)continue;

		m_pFile->Load(m_Map.m_JianZhuID[i]);
		m_pShowTurn[m_NumShow] = m_pFile;
		m_ShowTurnType[m_NumShow] = 1;
		m_NumShow += 1;
		if (xgap > 350 || ygap > 270)
			m_pFile->m_NeedShow = TRUE;
	}


	for (int i = 0; i < m_ObjNumber; i++)
	{
		if (-1==m_pObj[i]->m_NowPos)
			m_pObj[i]->Set(POS_STAND);
	}
	
	return TRUE;
}

BOOL CMainState::Sort()
{
	cObj* pObj;
	CWFile* pFile;
	//int charpos = m_FightList[m_HeroID].m_pData->GetX();
	for (int i = 0; i < m_NumShow; i++)
	{
		switch (m_ShowTurnType[i])
		{
		case 0:
			pObj = (cObj*)m_pShowTurn[i];
			m_PosYList[i] = pObj->m_pShowData[0]->m_PreShowRect.bottom;
			break;
		case 1:
		
			{
				pFile = (CWFile*)m_pShowTurn[i];
				switch (pFile->m_PosType)
				{
				case 1:
					m_PosYList[i] = pFile->m_PreShowRect.bottom;
					break;
				case 0:
					m_PosYList[i] = 0;//��ʾ�����·�
					break;
				case 2:
					m_PosYList[i] = 480;//��ʾ�����·�
					break;
				}
			}
			break;
		}
	}
	for (int a = 0; a < m_NumShow - 1; a++)
	for (int b = a + 1; b<m_NumShow; b++)
	{
		if (m_PosYList[a]>m_PosYList[b])//ǰ�����ں���
		{
			swap(m_PosYList[a], m_PosYList[b]);
			swap(m_pShowTurn[a], m_pShowTurn[b]);
			swap(m_ShowTurnType[a], m_ShowTurnType[b]);
		}
	}//����������ǰλ��YС����ǰ��	
	return TRUE;
}

BOOL CMainState::ProcessInput()
{
	static int time;
	m_isMouseOnMap = TRUE;
	if (m_pNowSelect) //��֮ǰ�����Ķ���ȡ������
	{
		m_pNowSelect->SetShine(FALSE);
		m_pNowSelect = 0;
	}
	if (0==g_pMainState->m_GlobalButton[18])
		ProcessInterface();
	if (g_StateType==STATE_MAIN)
		m_Menu.ProcessInput();
	

	if (Keyboard.GetKeyState(DIK_F9))
	{
		Keyboard.SetLock(DIK_F9);//�������
	
		int xianling = g_pMainState->m_GlobalButton[3];
		if (xianling > 0)
		{
			if (xianling > 5)
			{
				g_pMainState->m_GlobalButton[3] = 5;
				xianling = 5;
			}
			g_pMainState->m_InterFaceBuy.Open(xianling + 87);
		}
	}
	
	if (Keyboard.GetKeyState(DIK_Z))
	{
		Keyboard.SetLock(DIK_Z);
		m_WorldMap.OnOff(!m_WorldMap.m_NeedShow);
	}
	//S������
	if (Keyboard.GetKeyState(DIK_S))
	{
		Keyboard.SetLock(DIK_S);
		m_InterfaceSystem.OnOff(!m_InterfaceSystem.m_NeedShow);
	}

	if (Keyboard.GetKeyState(DIK_T))
	{
		Keyboard.SetLock(DIK_T);
		m_InterfaceTeam.OnOff(!m_InterfaceTeam.m_NeedShow);
	}

	//TAB  С��ͼ
	if (Keyboard.GetKeyState(DIK_TAB))
	{
		Keyboard.SetLock(DIK_TAB);
		m_InterfaceSmallMap.OnOff(!m_InterfaceSmallMap.m_NeedShow);
		return TRUE;
	}
	if (m_isMouseOnMap)
	{
		int xMove = 0;
		int yMove = 0;
		BOOL bMove = FALSE;
		if (Keyboard.GetKeyState(DIK_LEFT))
		{
			xMove = -20;
			bMove = TRUE;
			m_Cursor.m_FollowMode = FALSE;
		}
		if (Keyboard.GetKeyState(DIK_RIGHT))
		{
			xMove = 20;
			bMove = TRUE;
			m_Cursor.m_FollowMode = FALSE;
		}
		if (Keyboard.GetKeyState(DIK_UP))
		{
			yMove = -20;
			bMove = TRUE;
			m_Cursor.m_FollowMode = FALSE;
		}
		if (Keyboard.GetKeyState(DIK_DOWN))
		{
			yMove = 20;
			bMove = TRUE;
			m_Cursor.m_FollowMode = FALSE;
		}
		if (bMove)
		{
			cObj* pUser = g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData;
			pUser->m_time = -1;
			pUser->SetTarget((float)(pUser->GetX() +xMove), (float)(pUser->GetY()+yMove));
		}
	}
	
    m_Cursor.Process();
	return TRUE;
}

cObj* CMainState::GetPointAt(long x, long y) 
{
	for (int i = 0; i < MAX_OBJ_NUM; i++)
	{
		if (m_pObj[i])
		{
			if (m_pObj[i]->isPointOn(x, y))
			{
				if (m_pObj[i] == m_FightList[m_HeroID].m_pData)continue;//��������
				return m_pObj[i];
			}
		}
	}
	return 0;
}

BOOL CMainState::MoveObj()
{
	for (int i = 0; i < m_ObjNumber; i++)
	{
		if (m_pObj[i] == 0)continue;
		cObj* p = m_pObj[i];
		if (p->m_time == -2)continue;
		//Ѱ·
		if (-1 < p->m_time)
		{
			if (!p->Move())
			{
				p->m_time -= 2;
				if (-1 >= p->m_time)
				{
					p->m_time = -2;
					p->Set(POS_STAND);
					continue;
				}

				int heroXPos = p->m_Path[p->m_time].sx;
				int heroYPos = p->m_Path[p->m_time].sy;
				p->SetTarget(heroXPos, heroYPos);
				if (-1 != p->m_time)
				{
					p->Move();
				}
			}
		}
		else
			p->Move();//ֱ��
	}
	return TRUE;
}



BOOL CMainState::ProcessInterface()
{
	m_Tags.Process();

	if (Keyboard.GetKeyState(DIK_M))
	{
		Keyboard.SetLock(DIK_M);//
		g_pMainState->m_GlobalButton[16] = !g_pMainState->m_GlobalButton[16];
		if (g_pMainState->m_GlobalButton[16])
			g_pMainState->m_Mp3.Stop();
		else
			g_pMainState->m_Mp3.Play();
	}
	if (Keyboard.GetKeyState(DIK_N))
	{
		Keyboard.SetLock(DIK_N);//
		g_pMainState->m_GlobalButton[17] = !g_pMainState->m_GlobalButton[17];
	}
	
	/*if (Keyboard.GetKeyState(DIK_3))
	{
		Keyboard.SetLock(DIK_3);
		sItem2 weappon;
		weappon.m_Num = 1;
		for (int i = 0; i < 29; i++)
		{
			weappon.Set(13, i);
			m_ItemManage.AutoItemProperty(&weappon, 100,1);
			m_InterfaceItem.GetItem(g_pMainState->m_HeroID, &weappon);
		}
	
	}*/
	//if (Keyboard.GetKeyState(DIK_4))
	//{
	//	Keyboard.SetLock(DIK_4);
	//	int nextid;
	//	if (!g_pCharacter->m_PCData.m_bUseCard)
	//		nextid = 51;
	//	else
	//	{
	//		nextid = g_pCharacter->m_PCData.m_pCardPetData->GetData()->m_ID;
	//		nextid += 1;
	//		if (nextid > 217)nextid = 0;
	//	}
	//	sItem2 item2;
	//	item2.Set(26, nextid);
	//	m_ItemManage.UseItem26(&item2, m_HeroID);
	//}
	//
	////���ӷ��ݹ�ģ
	//if (Keyboard.GetKeyState(DIK_M))
	//{
	//	Keyboard.SetLock(DIK_M);
	//	m_FangWu.SetGuiMo(m_FangWu.GetGuiMo() + 1);
	//	m_FangWu.GoIn();
	//	return TRUE;
	//}

#ifdef TESTDEBU
	static int cardid = 119;
	//����
	if (Keyboard.GetKeyState(DIK_1))
	{
		Keyboard.SetLock(DIK_1);
		cardid += 1;
		g_pCharacter->m_PCData.m_JinJieNum.m_PositionID = 1;
		g_pCharacter->UseCard(cardid);
		
		m_FightList[m_HeroID].m_pData->Reset();
		m_FightList[m_HeroID].m_pData->Set(POS_STAND);
	}
	//��Ǯ
	if (Keyboard.GetKeyState(DIK_0))
	{
		Keyboard.SetLock(DIK_0);
		g_pCharacter->m_PCData.m_Money[0] += 1000000;
		return TRUE;
	}
	//
	if (Keyboard.GetKeyState(DIK_H))
	{
		Keyboard.SetLock(DIK_H);
		/*ostringstream oss;
		sPetData* pPetd;
		string path;


		for (int i = 29; i < 30; i++)
		{
		g_pCharacter->m_PCData.SetJinJie(3);
		g_pCharacter->UseCard(i);
		pPetd = m_PetDataManager.GetPetData(i);
		for (int i2 = 6; i2 < 7; i2++)
		{
		g_pHeroObj->Reset();
		g_pHeroObj->Set(i2);
		oss.str("");
		oss << "Out\\"<<i;
		oss << pPetd->m_Name;
		switch (i2)
		{
		case POS_STAND:oss << "վ"; break;
		case	POS_WALK:oss << "��"; break;
		case	POS_FIGHT_STAND:oss << "ս"; break;
		case	POS_RUN:oss << "��"; break;
		case	POS_ATTACK:oss << "��"; break;
		case	POS_MAGIC:oss << "��"; break;
		case	POS_DEF:oss << "��"; break;
		case	POS_SUFFER:oss << "��"; break;
		case	POS_DEAD:oss << "��"; break;
		case	POS_RETURN:oss << "ת"; break;
		case	POS_ATTACK2:oss << "����"; break;
		}
		path = oss.str();
		for (int i3 = 0; i3 < 1; i3++)
		{
		if (!g_pHeroObj->m_pShowData[i3]->m_NeedShow)continue;
		oss.str("");
		oss << path;
		oss << i3;
		g_pHeroObj->m_pShowData[i3]->Save(oss.str());
		}
		}
		}*/
		m_ZhiXian3.Process(0);
		/*m_TaskFB.Start(3);
		m_TaskFB.ProcessSM(11);*/
		/*g_pCharacter->m_PCData.modeid = eJuSe�����;
		g_pCharacter->SetData(&g_pMainState->m_pCharacterDataList[g_pCharacter->m_PCData.modeid]);
		g_pCharacter->m_PCData.LeveUp(160, FALSE);
		g_pCharacter->m_PCData.m_Money[0] += 100000000;*/
		return TRUE;
	}
	//ǿ��ս��ʤ��
	if (Keyboard.GetKeyState(DIK_8))
	{
		Keyboard.SetLock(DIK_8);
		m_pFightState->SuccessFight();
		return TRUE;
	}
	//�Ӿ���
	if (Keyboard.GetKeyState(DIK_9))
	{
		Keyboard.SetLock(DIK_9);
		g_pCharacter->m_PCData.EXP += g_pCharacter->m_PCData.m_LV*g_pCharacter->m_PCData.m_LV * 1000+1000;
		return TRUE;
	}
	//���Ǳ�׼��
	if (Keyboard.GetKeyState(DIK_7))//
	{
		Keyboard.SetLock(DIK_7);
		g_pCharacter->m_PCData.Followlv(g_pCharacter->m_PCData.m_LV);
		return TRUE;
	}
#endif

	if (Keyboard.GetKeyState(DIK_RETURN))// && Keyboard.GetKeyState(DIKEYBOARD_LMENU))
	{//�л�ȫ��
		Keyboard.SetLock(DIK_RETURN);
		//m_Canvas.SetRedrawAll();
		g_bFillScreen = !g_bFillScreen;
		SetFullScreen(g_bFillScreen);
		return TRUE;
	}
	if (g_StateType == STATE_MAIN)
	{
		//E  ����
		if (Keyboard.GetKeyState(DIK_E))
		{
			Keyboard.SetLock(DIK_E);
			m_InterfaceItem.OnOff(!m_InterfaceItem.m_NeedShow);
			return TRUE;
		}
		////���ѽ���
		//if (Keyboard.GetKeyState(DIK_F))
		//{
		//	Keyboard.SetLock(DIK_F);
		//	m_Friend.OnOff(!m_Friend.m_NeedShow);
		//	return TRUE;
		//}
		//  ״̬
		if (Keyboard.GetKeyState(DIK_W))
		{
			Keyboard.SetLock(DIK_W);
			m_State.OnOff(!m_State.m_NeedShow);
			return TRUE;
		}
		//O   ����
		if (Keyboard.GetKeyState(DIK_O))
		{
			Keyboard.SetLock(DIK_O);
			m_InterfacePet.OnOff(!m_InterfacePet.m_NeedShow);
			return TRUE;
		}
		else
		if (Keyboard.GetKeyState(DIK_P))
		{
			Keyboard.SetLock(DIK_P);
			m_InterfacePet.OnOff(!m_InterfacePet.m_NeedShow);
			return TRUE;
		}
		//R������
		if (Keyboard.GetKeyState(DIK_R))
		{
			Keyboard.SetLock(DIK_R);
			m_InterfaceFB.OnOff(!m_InterfaceFB.m_NeedShow);
			return TRUE;
		}

		//Q  ����
		if (Keyboard.GetKeyState(DIK_Q))
		{
			Keyboard.SetLock(DIK_Q);
			m_Job.OnOff(!m_Job.m_NeedShow);
			return TRUE;
		}
		//~  ����
		if (Keyboard.GetKeyState(DIK_GRAVE))
		{
			Keyboard.SetLock(DIK_GRAVE);
			m_InterfaceJiNeng.OnOff(!m_InterfaceJiNeng.m_NeedShow);
			return TRUE;
		}
		//C������
		if (Keyboard.GetKeyState(DIK_C))
		{
			Keyboard.SetLock(DIK_C);
			m_Accelator.OnOff(!m_Accelator.m_NeedShow);
			return TRUE;
		}
	}
	

	for (int i = c_InterfaceShowNum - 1; i >= 0; i--)
	{
		if (m_isMouseOnMap)
		if (0 != m_Interface[i])
			m_Interface[i]->ProcessInput();
	}
	m_PCHead.ProcessInput();
	if (g_StateType == STATE_MAIN)
	m_InterfaceDoor.ProcessInput();
	else m_InterfaceDoor.m_TimeShiCheng.UpdateTime();


#ifdef TESTDEBU
	if (Keyboard.GetKeyState(DIK_SPACE))
	{
		Keyboard.SetLock(DIK_SPACE);
		m_TaskNormal.Process(1);
		return TRUE;
	}
#endif

	

	return TRUE;
}

BOOL CMainState::ShowInterface()
{
	cCanvas* pCanvas = &m_Canvas;
	m_PCHead.Show(pCanvas);
	
	//��ʾ�˵�
	if (g_StateType == STATE_MAIN)
	{
		m_InterfaceDoor.Show(pCanvas);
		m_Menu.Show(&m_Canvas);
	}
	m_Channel.Show(pCanvas);
	return TRUE;
}

int CMainState::ReadTaskFightIndex(string Path)
{
	ostringstream oss;
	oss << Path << "\\index.txt";
	ifstream File;
	File.open(oss.str());
	if (!File)ERRBOX;
	int maxnum, minnum, soundnum;

	File >> minnum;
	File >> maxnum;
	File >> soundnum;
	string soundname;
	soundnum = rand() % soundnum + 1;
	for (int i = 0; i<soundnum; i++)
	{
		File >> soundname;
	}
	m_Mp3.Load(soundname);
	int num;
	num = AtutoGetEnemyNum();
	if (num < minnum)num = minnum;
	if (num>maxnum)num = maxnum;
	return num;
}



BOOL CMainState::LoadCharacterData2(int charactID, string Path)
{

	cObj* pObj = m_FightList[charactID].m_pData;
	LoadCharacterData2(pObj,Path);
	m_FightList[charactID].isLive = TRUE;
	return TRUE;
}

BOOL CMainState::LoadCharacterData2(cObj*& pObj, string Path)
{
	ostringstream oss;
	oss << g_ScriptPath;
	oss << "��ɫ//";
	pObj->LoadCharacter(Path,oss.str());
	pObj->Set(POS_STAND);
	return TRUE;
}

BOOL CMainState::FindCharacter(cObj*& data, int CharacterID)
{
	data->SetDataType(INDEXTYEP_CHARACTER);
	cCharacter* p = (cCharacter*)data->m_pIndex;
	if (!p)
	{
		p = new cCharacter;
		data->m_pIndex = p;
	}
	p->SetData(&m_pCharacterDataList[CharacterID]);
	return TRUE;
}



BOOL CMainState::FindPetData(cPet* pPet, int PetID)
{
	if (PetID < c_PetNumber)
	{
		pPet->SetData(m_PetDataManager.GetPetData(PetID));
		pPet->m_PetDataZZ.m_Name = pPet->GetData()->m_Name;
		pPet->m_PetDataZZ.modeid = PetID;
		return TRUE;
	}
	return FALSE;
}

BOOL CMainState::FindData(cObj* data, int Id)
{
	if (!data)
	{
		ERRBOX;
		return FALSE;
	}
	if (Id < 1000)
	{
		data->SetDataType(INDEXTYEP_PET);
		return FindPetData((cPet*)data->m_pIndex, Id);
	}
	if (Id < 2000)
	{
		Id -= 1000;
		data->SetDataType(INDEXTYEP_NPC);
		return FindNPCData((cNpc*&)data->m_pIndex, Id);
	}
	else
	{
		Id -= 2000;
		data->SetDataType(INDEXTYEP_CHARACTER);
		return FindCharacter(data, Id);
	}
}

BOOL CMainState::FindNPCData(cNpc*& data, int NpcID)
{
	if (!data)
	{
		data = new cNpc;
	}
	data->m_pNpcData = &m_pNpcList[NpcID];
	return TRUE;
}

BOOL CMainState::InterfaceSetShow(cInterfaceFather* p, BOOL NeedShow)
{
	if (p->m_NeedShow == NeedShow)return TRUE; //����Ѿ����� ֱ���˳�
	if (NeedShow)
	{
		for (int i = 0; i < c_InterfaceShowNum; i++)
		{
			if (m_Interface[i] == p)
			{
				p->m_NeedShow = TRUE;
				return TRUE;
			}
			if (m_Interface[i] == 0)
			{
				p->m_NeedShow = TRUE;
				m_Interface[i] = p;
				return TRUE;
			}
		}
		//����б�����,���������һ���˳��б�
		m_Interface[0]->OnOff(FALSE);
		m_Interface[c_InterfaceShowNum - 1] = p;
		m_Interface[c_InterfaceShowNum - 1]->m_NeedShow = TRUE;

	}
	else  //�˳��б�
	{
		for (int i = 0; i < c_InterfaceShowNum; i++)
		{
			if (m_Interface[i] == p)
			{
				p->m_NeedShow = FALSE;
				int k = i;
				for (; k <c_InterfaceShowNum-1; k++)
				{
					m_Interface[k] = m_Interface[k + 1];
				}
				m_Interface[k] = 0;
				return TRUE;
			}
		}
		p->m_NeedShow = FALSE;
	}
	return TRUE;

}

BOOL CMainState::Give(int CharID, sItem2* pItem)
{
	m_InterfaceItem.GetItem(CharID, pItem);
	return TRUE;
}




BOOL CMainState::GivePcAPet(int charID, cPet* pPet, BOOL setfight)
{
	if (m_FightList[charID].m_pData->GetType() != INDEXTYEP_CHARACTER)return FALSE;
	cCharacter* pPcData = (cCharacter*)m_FightList[charID].m_pData->m_pIndex;
	if (pPcData->m_PCData.m_NumofBB > 7)return FALSE;
	pPcData->m_PCData.GetPet(pPet);
	if (setfight)
	{
		SetFightPet(charID, pPcData->m_PCData.m_NumofBB - 1);
	}
	return TRUE;
}


cObj* CMainState::FindObjByNpc(sNpc* pNpc)
{
	for (int i = 0; i < m_Map.m_numberOfNpc; i++)
	{
		cObj* p = m_Map.m_pNpc[i];
		if (!p->m_TrueName.GetString().compare(pNpc->m_name))
		if (p->GetX()==pNpc->m_x*20)
		if (p->GetY()==g_pMainState->m_Map.m_pMap->m_Height- pNpc->m_y*20)
		{
			return m_Map.m_pNpc[i];
		}

	}
	return 0;
}
cObj* CMainState::FindObjByName(string Name)
{
	for (int i = 0; i < m_FightList.size(); i++)
	{
		if (m_FightList[i].isLive==FALSE)continue;
		if (!m_FightList[i].m_pData->m_TrueName.GetString().compare(Name))
		{
			return m_FightList[i].m_pData;
		}
	}
	for (int i = 0; i < m_Map.m_numberOfNpc; i++)
	{

		if (!m_Map.m_pNpc[i]->m_TrueName.GetString().compare(Name))
		{
			return m_Map.m_pNpc[i];
		}

	}
	return 0;
}

BOOL CMainState::ReadyToFight()
{
	if (m_Map.m_MapType < 1)return TRUE;
	if (0 == m_Map.m_NumOfEmemyType)return TRUE;
		for (int i = 10; i < 20; i++)
		{
			m_FightList[i].isLive = FALSE;
		}
		int num = AtutoGetEnemyNum();
		string mapname;
		switch (rand()%5)
		{
		case 0:
			mapname = "ս��1";
			break;
		case 1:
			mapname = "ս��2";
			break;
		case 2:
			mapname = "ս��3";
			break;
		case 3:
			mapname = "ս��4";
			break;
		default:
			mapname = "ս��5";
			break;
		};
		m_Mp3.Load(mapname);
		int pettype = m_Map.m_NumOfEmemyType;
		int k1 = 0;
		int BBType;  //�ٻ�������,0 ���� 1ͷ�� 2 ����
		string name2;
 		for (int i = 10; i<10+num; i++)
		{
			m_FightList[i].m_pData->SetDataType(INDEXTYEP_PET);
			cObj* pObj = m_FightList[i].m_pData;
			cPetData* pPetData;
			pObj->GetDateP(pPetData);
			k1+=1;
			m_FightList[i].isLive = TRUE;
			m_FightList[i].SetDie( FALSE);
			cPet* pPet;
			pPet = (cPet*)pObj->m_pIndex;
			int dus = rand() % 100;
			if (dus < 20)  //20%�ļ��ʳ�ͷ��
			{
				BBType = 1;
				//ֻ�е���ɫ�ĵȼ�С�ڳ����ȼ�+10ʱ,���п��ܳ�����;
				if ((g_pCharacter->m_PCData.m_LV)< m_Map.m_EnemyLv + 10)
				if (0 == dus)  //1%�ļ��ʳ�����
					BBType = 2;
			}
			else BBType = 0;
			sAddPoint addpoint;
			int bbmode = g_pMainState->m_Map.m_EnemyList[rand() % pettype];
			if (i == 10)
			{
				if (g_pMainState->m_TaskNormal.m_pNpc)
				{
					bbmode = g_pMainState->m_TaskNormal.m_pNpc->GetModeID();
				//	g_pMainState->m_TaskNormal.m_pNpc = 0;
				}
			}
			switch (BBType)
			{
			case 0:
			
				{		
					MakeAPet(pPet, bbmode, 30, m_Map.m_EnemyLv + rand() % 3, addpoint);
					pObj->m_TrueName.SetString(pObj->GetAutoName());
					pPet->m_PetDataZZ.m_HP = pPet->m_PetDataZZ.m_HPHurtMax = pPet->m_PetDataZZ.m_HPMax.GetData();
				}
				break;
			case 1:
				
				{
					
					MakeAPet(pPet, bbmode, 40, m_Map.m_EnemyLv + rand() % 3 + 2, addpoint);
					name2 = pObj->GetAutoName();
					name2 += "ͷ��";
					pObj->m_TrueName.SetString(name2);
				}
				break;
			case 2:
			
				{
					MakeAPet(pPet, bbmode, 80, 0, addpoint);
					name2 = pObj->GetAutoName();
					name2 += "����";
					pObj->m_TrueName.SetString(name2);

				}
				break;	
			}
			pPetData->m_Controler.Set(1, 1, 0, 0, 1);
		}
	g_StateType = STATE_FIGHT;
	return TRUE;
}

BOOL CMainState::TeamFollow()
{
	int fightid,preid;
	for (int i = 1; i < 5; i++)
	{
		fightid = m_InterfaceTeam.GetTeamTurn(i);
		if (!m_FightList[fightid].isLive)return TRUE;
		cObj* obj = m_FightList[fightid].m_pData;
		

		if (abs(obj->m_xPos - obj->m_xTarget) + abs(obj->m_yPos - obj->m_yTarget) < 15)
		{
			preid = m_InterfaceTeam.GetTeamTurn(i-1);
			cObj* obj2 = m_FightList[preid].m_pData;

			float dis = abs(obj->m_xPos - obj2->m_xPos) + abs(obj->m_yPos - obj2->m_yPos);
			if (dis > 100)
			{
				CoutPoint(50, obj->m_xPos, obj->m_yPos, obj2->m_xPos, obj2->m_yPos, obj->m_xTarget, obj->m_yTarget);
				m_FightList[fightid].m_pData->m_time = -3;
			}
		}


	}
	return TRUE;
}

BOOL CMainState::SwapFightOffset(int Original, int Target)
{
	m_FightList[Original].Swap(m_FightList[Target]);

	if (Original<5||Target<5)
	m_PCHead.UpDateHead();//���½�ɫͷ��

	return TRUE;
}

//���ó�ս�ٻ���
BOOL CMainState::SetFightPet(int ID, int PetID,BOOL blimit)
{
	if ((ID + 5) % 10 < 5)return FALSE;
	cObj* pPcObj = m_FightList[ID].m_pData;
	cCharacter* pc = (cCharacter*)pPcObj->m_pIndex;
	cPet* pet=0;
	cPetData* pPcData;
	pPcObj->GetDateP(pPcData);
	cObj* pPetObj = m_FightList[ID + 5].m_pData;
	if (pc->m_PCData.m_FightPetID == PetID)
		return TRUE;
	if (pPetObj->GetType() == INDEXTYEP_CHARACTER)
	{ //λ�ñ�����ռ����,�޷��ٻ�
		return FALSE;
	}
	if (m_FightList[ID].m_FriendListid > -1)
	{//ͬ��
		int flistid = m_FightList[ID].m_FriendListid;
		if (flistid<g_pMainState->m_Friend.m_FriendList.size())
		g_pMainState->m_Friend.m_FriendList[flistid].m_FightBB = PetID;
	}

	if (PetID == -1)//��IDΪ-1ʱ��ʱ �ٻ�
	{
		//������� ,��ΪBB��սʱ�����Ի�������Ӱ��
		if (pPetObj->GetType()==INDEXTYEP_PET)
		{
			pet = (cPet*)pPetObj->m_pIndex;
			if (!pet->m_PetDataZZ.m_Controler.CheckBB())
			{
				return FALSE;//λ�ñ�����ռ����,�޷��ٻ�
			}
			if (g_StateType == STATE_FIGHT)
			{
				g_pMainState->m_pFightState->BackFromFight(ID + 5);
			}
			pet->m_PetDataZZ.m_XiuLian.Reset();
		}
		pPetObj->m_pIndex = 0;
		pPetObj->m_IndexType = 0;
		pPetObj->Free();
		m_FightList[ID + 5].isLive = FALSE;
		pc->m_PCData.m_FightPetID = -1;	
		pc->m_PCData.m_Controler.SetHaveBB(FALSE);
	}
	else     //���ò�ս
	{
		if (PetID<0)return FALSE;
		if (PetID>7)return FALSE;
		pet = pc->m_PCData.m_pPetDataList[PetID];
		
		if (blimit)
		{
			if (pet->GetData()->m_LVLimit > pc->m_PCData.m_LV)return FALSE;
			if (pet->m_PetDataZZ.m_LV > pc->m_PCData.m_LV + 10)return FALSE;
		}
		//���ٻ�
		if (pc->m_PCData.m_FightPetID > -1)
			SetFightPet(ID, -1);
	
		//��������
		pet->m_PetDataZZ.m_XiuLian = pc->m_PCData.m_BBXiuLian;
		if (g_StateType == STATE_FIGHT)
		{
			g_pMainState->m_pFightState->PrePareToFight(ID + 5);
		}

		pPetObj->m_pIndex = pet;
		pPetObj->m_IndexType=INDEXTYEP_PET;


		if (!pc->m_PCData.m_pPetDataList[PetID]->GetData())
			pc->m_PCData.m_pPetDataList[PetID]->SetData(g_pMainState->m_PetDataManager.GetPetData(pc->m_PCData.m_pPetDataList[PetID]->m_PetDataZZ.modeid));
		m_FightList[ID + 5].isLive = TRUE;
		pPetObj->m_TrueName.m_hFont = m_hFont[0];
		pPetObj->m_TrueName.SetString(pc->m_PCData.m_pPetDataList[PetID]->m_PetDataZZ.m_Name);
		pc->m_PCData.m_FightPetID = PetID;

		cPetData* pPetData;
		pPetObj->GetDateP(pPetData);
		
		//�������Ĺ��� ,����ϵͳ��֪����ս��ʱ�����û��Ȩ��������
		pPetData->m_Controler.Set(1, 0, 1, 0);	
		pc->m_PCData.m_Controler.SetHaveBB(TRUE);
		g_pMainState->m_FightList[ID + 5].m_SkillPreSelect.SkillID = -1;
	}
	g_pMainState->m_PCHead.Init();
	//֪ͨ�ı�ͷ��
	if (ID == m_HeroID)
	{
		if (-1 == PetID)
		{
			m_PCHead.SetPet(-1);
		}
		else
		{
			m_PCHead.SetPet(ID+5);
		}
	}
	m_PCHead.UpDateHead();
	return TRUE;
}

BOOL CMainState::ShowInterface2()
{
	if (m_Cursor.m_Data.m_NowID==m_Cursor.m_Normal)
	for (int i = 0; i <c_InterfaceShowNum; i++)
	{
		if (m_Interface[i])
			m_Interface[i]->Show(&m_Canvas);
	}	
	m_Tags.Show(&m_Canvas);
	
	
	if (g_xMouse < 0 || g_yMouse < 0 || g_xMouse>640 || g_yMouse>480)
	{
		if (ShowCursor(TRUE) > 1)
			ShowCursor(FALSE);
	}
	else
	{
		if (ShowCursor(FALSE) < -1)
			ShowCursor(TRUE);
		m_Cursor.Show(g_xMouse, g_yMouse);
	}
	return TRUE;
}



void CMainState::ClearInterfaceSetShow()
{
	for (int i = 0; i < c_InterfaceShowNum; i++)
	{
		if (m_Interface[i])
		{
			m_Interface[i]->m_NeedShow = FALSE;
			m_Interface[i] = 0;
		}
	}
}

int CMainState::GetAverageLv()
{
	cPetData* pPetData;
	int Averagelevel = 0;
	for (int i = 0; i < 5; i++)
	{
		if (m_FightList[i].isLive)
		{
			m_FightList[i].m_pData->GetDateP(pPetData);
			Averagelevel += pPetData->m_LV;
		}
		else break;
	}
	Averagelevel /= g_pMainState->GetPCFightNum();
	return Averagelevel;
}

void CMainState::SetMouseOnMap(BOOL lock)
{
	m_isMouseOnMap = lock;
	m_Cursor.m_FollowMode = FALSE;
}

BOOL CMainState::MakeAPet(cPet*& pPet, int ID, int StrongLv, int LV, sAddPoint addpoint)
{
	if (!pPet)pPet = new cPet;
	pPet->m_PetDataZZ.Reset();
	pPet->m_PetDataZZ.m_LV = 0;
	pPet->AutoPetData(ID, StrongLv,&addpoint);
	pPet->m_PetDataZZ.LeveUp(LV,FALSE);
	
	return TRUE;
}

int CMainState::AddPCEnemy(
	string name,
	int modeid,
	int lv,
	sAddPoint* addpoint ,
	sZiZhi* zizhi ,
	eMengPai mengpaid ,
	sItem2* pWeapon ,
	int petid ,
	int stronglv ,
	int pos,
	BOOL our)
{
	//sFightList* plist = 0;
	int id;
	if (-1 == pos)
	{
		int start;
		if (our)
			start = 0;
		else
			start = 10;
		id = m_AI.GetNextPos(start, 0, 0); //GetIdle(start);
	}
	else id = pos;
	if (id == -1)return -1;
	m_FightList[id].isLive = TRUE;
	m_FightList[id].m_pData->AutoPCData(name, modeid, addpoint, zizhi, mengpaid, 0, petid, stronglv);
	
	FollowLv(id,lv);
	if (pWeapon)
	{
		m_FightList[id].m_pData->SetWeapon(pWeapon->GetType(),pWeapon->GetID());
	}
	return id;
}
int CMainState::AddPCEnemy(sMapObjTrigger* pdata, int lv, sAddPoint* addpoint, sZiZhi* zizhi, eMengPai mengpaid, int stronglv /*= 50*/, int pos /*= -1*/, BOOL out /*= FALSE*/)
{
	sFightList* plist = 0;
	int id;
	if (-1 == pos)
	{
		int start;
		if (out)
			start = 0;
		else
			start = 10;
		id = m_AI.GetNextPos(start, 0, 0); //GetIdle(start);
	}
	else id = pos;
	if (id == -1)return FALSE;
	m_FightList[id].isLive = TRUE;
	plist = &m_FightList[id];
	sItem2* pWeapon = 0;
	if (pdata->m_Npc.m_modeid >= 2000)
	{
		if (pdata->m_Npc.weapontype > -1)
		{//����
			sItem2 weapon;
			weapon.m_Num = 1;
			weapon.Set(pdata->m_Npc.weapontype, pdata->m_Npc.weaponid);
			pWeapon = &weapon;
		}
		plist->m_pData->AutoPCData(pdata->m_Npc.m_name, pdata->m_Npc.m_modeid-2000,  addpoint, zizhi, mengpaid, pWeapon, -1, stronglv);
	}
	else
	{
		plist->m_pData->AutoPCData(pdata->m_Npc.m_name,0 , addpoint, zizhi, mengpaid, 0, pdata->m_Npc.m_modeid, stronglv);
	}
	g_pMainState->FollowLv(id,lv);
	return id;
}


int CMainState::AddPetEnemy(string name, int modeid, int lv, sAddPoint* addpoint, int stronglv, int pos /*= -1*/,  BOOL our)
{
	sFightList* plist = 0;
	int start;
	if (-1 == pos)
	{
		if (our)
		start = 0;
		else
		start = 10;
	}
	else start = pos;
	int id=-1;
	for (int i = start; i < start + 10; i++)
	{
		if (!m_FightList[i].isLive)
		{
			id = i;
			break;
		}
	}
	if (id == -1)return id;
	m_FightList[id].isLive = TRUE;
	plist = &m_FightList[id];
	plist->m_pData->AutoPetData(name, modeid, stronglv, addpoint, TRUE);
	plist->m_pData->FollowLv(lv);
	return id;
}

int CMainState::AddPC(
	string name,
	int modeid,
	int lv,
	sAddPoint* addpoint,
	sZiZhi* zizhi,
	eMengPai mengpaid,
	sItem2* pWeapon,
	int petid,
	int stronglv ,
	int pos )
{
	return AddPCEnemy( name,  modeid,  lv,  addpoint,  zizhi,  mengpaid /*= eMP_��*/,  pWeapon /*= 0*/ ,petid,stronglv, pos, TRUE);
}

int CMainState::AddPet(string name, int modeid, int lv, sAddPoint* addpoint, int stronglv, int Pos /*= -1*/)
{
	return AddPetEnemy(name, modeid, lv, addpoint, stronglv, Pos, TRUE);
}

void CMainState::Init2()
{
	if (m_bInit2)return;
	m_bInit2 = TRUE;
	// ����IDirectSound8�ӿ�
	DirectSoundCreate8(NULL, &g_pDirectSound, NULL);
	g_pDirectSound->SetCooperativeLevel(g_hWnd, DSSCL_NORMAL);
	g_redbrush = CreateSolidBrush(RGB(255, 0, 0));
	g_blackbrush = CreateSolidBrush(RGB(0, 0, 0));
	g_whitebrush = CreateSolidBrush(RGB(255, 255, 255));
	g_bluebrush = CreateSolidBrush(RGB(0, 0, 255));
	

	m_hFont.resize(6);
	//����
	m_hFont[0] = EasyCreateFont(14,  L"Arial");
	m_hFont[1] = EasyCreateFont(15,  L"������Ů����W5");
	m_hFont[2] = EasyCreateFont(28,  L"�����кڼ�",700);
	m_hFont[3] = EasyCreateFont(24,  L"���ǰ������");
	m_hFont[4] = EasyCreateFont(24,  L"������ͷ�����",700);
	m_hFont[5] = EasyCreateFont(12,  L"����κ����");
	m_hOldFont2 = (HFONT)SelectObject(g_DC2, m_hFont[0]);

	m_Button4.Load(0x86D66B9A);
	m_Cancel.Load(0xF11233BB);
	m_HpLine.Load(0x4FD9FFF3);
	m_HpEmptyLine.Load(0x4D0A334C);
	m_Shadow.Load(3705976162);
	m_FNumRed.Load(551);
	m_FNumGreen.Load(552);
	m_FaceTextBack.Load(0xA4120EA9);
	Input.Init();
	Keyboard.Create(&Input, KEYBOARD);
	Mouse.Create(&Input, MOUSE);
	
	m_Canvas.Init();
	m_Cursor.Init();
	m_InterfaceTeam.Init();

}

void CMainState::Init3()
{
	if (m_bInit3)return;
	
	////��ʼ��ȫ�ֱ���
	for (int i = 0; i < c_GlobalButtonNum; i++)
	{
		m_GlobalButton[i] = 0;
	}
	m_GlobalButton[0] = 80; //�Ѷ�
	m_GlobalButton[1] = 1000;//��Ч


	for (int i = 0; i < 20; i++)
	{
		m_FightList[i].m_pData = new cObj();
		m_FightList[i].m_pData->m_IDInFightlist = i;
		for (int j = 0; j < 5; j++)
			m_FightList[i].m_Damage[j].m_bStatic = TRUE;
	}

	//������Ϊģ��
	string path;
	path = g_strMediaPath;
	path += "��Ϊģ��.txt";
	m_ScriptTemplate.LoadActionTemplate(path.c_str());
	m_ColorManage.Init();
	m_Tags.Init();	
	m_FaceTextBack.SetStatic(TRUE);
	m_FaceTextBack.m_Picture.m_bShadow = 50;
	m_FaceTextBackp.resize(2);
	m_FaceTextBackp[0].Set(&m_FaceTextBack);
	m_FaceTextBackp[1].Set(&m_FaceTextBack);
	m_Channel.Init();
	m_Map.Init();

	
	LoadList();
	


	m_Job.Init();
	m_InterfaceDoor.Init();
	for (int i = 0; i < 20; i++)
	{
		m_FightList[i].m_pData->Init();
		m_FightList[i].m_pData->m_TrueName.m_bShowOnScreen = FALSE;
		m_FightList[i].m_pData->m_TrueName.SetBackColor(RGB(0, 0, 0));
	}
	
	m_Menu.Init();
	//��ɫͷ��
	m_PCHead.Init();
	
	m_Dialog.Init();
	//����ʱ��
	m_InterfaceDoor.m_TimeShiCheng.SetFPS(g_FPS);

	m_bInit3 = TRUE;

	//ifstream File;
	//File.open("2.txt");
	//int dustbin;
	//File >> dustbin;
	//DWORD data[2];
	//for (int i = 0; i < 2; i++)
	//	File >> data[i];
	//m_pTestpic.Load(data[0]);
	//m_pTestpic2.Load(data[1]);
	//for (int i = 0; i < 4; i++)
	//{
	//	m_pTestpicp[i].Set(&m_pTestpic);
	//	m_pTestpicp2[i].Set(&m_pTestpic2);
	//
	//}
	//m_pTestpicp[2].m_direction = 1;
	//m_pTestpicp2[2].m_direction=1;
	//m_pTestpicp[3].m_direction = 3;
	//m_pTestpicp2[3].m_direction = 3;
	//m_pTestpicp[0].m_direction = 7;
	//m_pTestpicp2[0].m_direction = 7;
	//m_pTestpicp[1].m_direction = 9;
	//m_pTestpicp2[1].m_direction = 9;

	//m_pTestpicp[0].SetXY(220, 200);
	//m_pTestpicp2[0].SetXY(220, 200);
	//m_pTestpicp[1].SetXY(420, 200);
	//m_pTestpicp2[1].SetXY(420, 200);
	//m_pTestpicp[2].SetXY(220, 300);
	//m_pTestpicp2[2].SetXY(220, 300);
	//m_pTestpicp[3].SetXY(420, 300);
	//m_pTestpicp2[3].SetXY(420, 300);
}

cTaskFather* CMainState::FindTaskByID(int ID)
{
	switch (ID)
	{
	case 0:return &m_TaskNormal;
	case 30:return &m_TaskBaoTu;
	case 35:return &m_TaskHuoDong;
	case 40:return &m_TaskZhuaGui;
	case 50:return &m_TaskFB;
	case 1003:return &m_taskSM;
	case 2007:return &m_ZhuXian;
	case 2008:return &m_ZhiXian;
	case 2009:return &m_ZhiXian2;
	case 2010:return &m_ZhiXian3;
	default:
		ERRBOX;
		break;
	}
	return 0;
}

void CMainState::SetCharacter(int ID)
{
	m_HeroID = ID;
	m_FightList[m_HeroID].m_pData->m_bCheckWall = TRUE;//������·Ҫ������ײ���
	g_pHeroObj = m_FightList[ID].m_pData;
	g_pCharacter = (cCharacter*)g_pHeroObj->m_pIndex;
	m_State.SetFight(g_pCharacter);
	m_PCHead.SetFighter(m_HeroID);
	cObj* pHero = m_FightList[m_HeroID].m_pData;
	cPetData* pPetData;
	pHero->GetDateP(pPetData);
	for (int i = 0; i < 20; i++)
		m_pFightState->m_FightMenu.SetControl(i,FALSE);
	m_pFightState->m_FightMenu.SetControl(ID, TRUE);
	m_pFightState->m_FightMenu.SetControl(ID+5, TRUE);
}

HFONT CMainState::EasyCreateFont(int h,  wstring name, int weight)
{
	return CreateFont(
		h, 0,       //����ĸ�  ��
		0, 0,       //������ʾ�ĽǶ�   ����ĽǶ�
		weight,    //����İ���   700����   
		FALSE,     //б��
		false,     //�»���
		false,     //ɾ����
		GB2312_CHARSET,  //�ַ���
		OUT_CHARACTER_PRECIS,  //�������
		CLIP_STROKE_PRECIS,  //�ü�����
		DEFAULT_QUALITY,  //�߼�����������豸��ʵ������֮��ľ���
		FF_MODERN,      //������
		name.c_str());//L"����κ����");//L"�����ֻ������");//L"�������Ƽ���");         //������
}




void CMainState::UpDateInput()
{
	Keyboard.Acquire(TRUE);   // Read keyboard
	Keyboard.Read();
	Mouse.Acquire(TRUE);   // Read keyboard
	Mouse.Read();
	g_xMouse = Mouse.GetXPos();
	g_yMouse = Mouse.GetYPos();
}

int CMainState::AtutoGetEnemyNum()
{
	int our = 0;
	for (int i = 0; i<5; i++)
	{
		if (m_FightList[i].isLive)our += 1;
	}
	int EnemyNum = our + rand() % (our+2);
	if (EnemyNum > 10)EnemyNum = 10; 
	return EnemyNum; 
}



int CMainState::GetLiveNum(int start, int end)
{
	int num = 0;
	for (int i = start; i < end; i++)
	{
		if (m_FightList[i].isLive)
		{
			if (i%10!=0)
			if(m_FightList[i].m_pData->CheckHaveState(89))
			continue;

			if (!m_FightList[i].CheckDie())
				num += 1;
		}
	}
	return num;
}

void CMainState::ResetAll()
{
	Init2();
	Init3();
	m_TriggerFightStart.Clear();
	m_TriggerGive.Clear();
	m_TriggerFightSuccess.Clear();
	m_TriggerObj.m_datalist.clear();
	m_TriggerObjMap.m_datalist.clear();
	m_Triggerlv.Clear();
	m_TriggerGive.Clear();
	m_Job.Reset();

	for (int i = 0; i < 20; i++)
	{
		m_FightList[i].Free();
	}
	ClearInterfaceSetShow();
	m_Canvas.m_NeedShowMap = TRUE;
}

void CMainState::ShowObj()
{
	Sort();
	for (int i = 0; i < m_NumShow; i++)
	{
		switch (m_ShowTurnType[i])
		{
		case 0:
			
			{
				cObj* p = (cObj*)m_pShowTurn[i];
				m_Canvas.Add(p->GetShadow(), 0);
				p->Show(&m_Canvas);
				
			}
			break;
		case 1:
			
			{
				CWFile* p = (CWFile*)m_pShowTurn[i];
				m_Canvas.Add(p,0);
			}
			break;
		}
	}
}

void CMainState::FreeFightList(int i)
{
	if (!m_FightList[i].isLive)return;
	cPetData* pPet;
	m_FightList[i].m_pData->GetDateP(pPet);
	if (pPet->m_Controler.CheckHaveBB())
	{
		m_FightList[i + 5].m_pData->m_pIndex = 0;
		m_FightList[i + 5].m_pData->m_IndexType = 0;
		m_FightList[i + 5].Free();
	}
	else
	if (pPet->m_Controler.CheckBB())
	{
		m_FightList[i].m_pData->m_pIndex = 0;
		m_FightList[i].m_pData->m_IndexType = 0;
	}
	m_FightList[i].Free();
	if (g_StateType!=STATE_GAMEOVER)
		FindObj();

}

void CMainState::SaveGlobal(ofstream& File)
{
	File << "\nȫ�ֱ���\n";
	for (int i = 0; i < c_GlobalButtonNum; i++)
	{
		File << m_GlobalButton[i] << " ";
		if (i == 30)File << "\n";
	}
}

void CMainState::LoadGlobal(ifstream& File)
{
	string dustbin;
	File >> dustbin;
	if (dustbin.compare("ȫ�ֱ���"))ERRBOX;
	m_GlobalButton.resize(c_GlobalButtonNum);
	for (int i = 0; i < c_GlobalButtonNum; i++)
	{
		File >> m_GlobalButton[i];
	}
}

int CMainState::AddPetEnemy2(int modeid, int lv/*=-1*/, sAddPoint* addpoint/*=0*/, int stronglv/*=-1*/, int Pos /*= -1*/, BOOL out /*= FALSE*/)
{
	if (-1 == lv)lv = g_pCharacter->m_PCData.m_LV;
	return AddPetEnemy(g_pMainState->m_PetDataManager.GetPetData(modeid)->m_Name, modeid, lv, addpoint,stronglv, Pos, out);
}

void CMainState::FollowLv(int id,int lv/*=-1*/)
{
	m_FightList[id].m_pData->FollowLv(lv);
}






unsigned int WINAPI cFightState::ThreadProc(_In_ LPVOID lpParameter)
{
	cFightState*  pFightState = (cFightState*)lpParameter;
	for (;;)
	{
		
		pFightState->m_ThreadSleep = FALSE;
		for (int i = 0; i < 5; i++)
		{
			if (pFightState->m_IDtoLoad[i])
			{
				pFightState->m_pFileToLoad[i]->Load(pFightState->m_IDtoLoad[i]);
				pFightState->m_IDtoLoad[i] = 0;
			}
		}
		
		pFightState->m_ThreadSleep = TRUE;
		//�������˯��
		SuspendThread(pFightState->m_hThread);
		

		
		
	}
}




BOOL cFightState::Init(void* pData)
{
	if (isInit)
	{
		return TRUE;
	}
	//�����߳�
	if (!m_hThread)
	{
		m_MiaoShu.SetXCenter(320);
		m_MiaoShu.SetY(400);
		m_MiaoShu.SetColor(RGB(255,255,255));
		for (int i = 0; i < 10; i++)
		{
			m_HiddenWeaponsp[i].Set( &m_HiddenWeapons);
			m_HiddenWeaponsp[i].m_NeedShow = FALSE;
		}
		m_FightBack1.Load(0xE3B87E0F);
		//���õ�ͼƬ��800*600��,����ʾ����640*480��
		m_FightBack1.SetxOffset2(80);
		m_FightBack1.SetyOffset2(60);
		m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, this, CREATE_SUSPENDED, NULL);
		//CreateThread(0, 0, ThreadProc, this, CREATE_SUSPENDED, 0);

		m_FightMenu.Init();
		m_pFightBack = new BYTE[75 << 13];

		for (int i = 0; i < 20; i++)
			m_pList[i] = &g_pMainState->m_FightList[i];
	}

	isInit = TRUE;
	m_bEscape = FALSE;
	m_nowturnid = 0;
	g_pMainState->m_Cursor.m_FollowMode = FALSE;
	g_pMainState->m_PCHead.MoveTeamHead(1);
	if (g_pMainState->m_pNowSelect)
	{
		g_pMainState->m_pNowSelect->SetShine(FALSE);
		g_pMainState->m_pNowSelect = 0;
	}
	//����Ƿ���5��
	for (int i = 0; i < 5; i++)
	{
		if (!g_pMainState->m_FightList[i].isLive)
		{
			g_pMainState->m_InterfaceTeam.SetFormation(normal,0,TRUE);
		}
	}
	if (g_pMainState->m_GlobalButton[0] == 100)
	{
		g_pMainState->m_AI.m_AttackAI.m_bPKMode = TRUE;
	}
	else g_pMainState->m_AI.m_AttackAI.m_bPKMode = FALSE;
	for (int i = 0; i < 20; i++)
	{
		m_SpeedListPos[i] = i;
		PrePareToFight(i);
	}
	SortBySpeed();

	for (int i = 0; i < 5; i++)
	{
		m_IDtoLoad[i] = 0;
	}

	//���±���

	int xOf1 = g_pMainState->m_Map.m_pMap->m_xCenterOffset;
	int yOf1 = g_pMainState->m_Map.m_pMap->m_yCenterOffset;
	for (int i = 0; i < 3; i++)
	for (int j = 0; j < 3; j++)
	{
		int k = g_pMainState->m_Map.m_pMap->m_NowMapNum + (i - 1) + (j - 1) *g_pMainState->m_Map.m_pMap->m_WidthNum;
		if (k < 0)continue;
		if (k>=g_pMainState->m_Map.m_pMap->m_NumBlock)continue;
		int xstart;
		int ystart;
		int xOf;
		int yOf;
		int W;
		int H;
		switch (i)
		{
		case 0:
			xOf = 0;
			xstart = xOf1;
			W = 320 - xOf1;
			break;
		case 1:
			W = 320;
			xOf = 319-xOf1;
			xstart = 0;
			break;
		default:
			xstart = 0;
			W = xOf1;
			xOf = 639-xOf1;
			break;
		}
		switch (j)
		{
		case 0:
			yOf = 0;
			ystart = yOf1;
			H = 240 - yOf1;
			break;
		case 1:
			ystart = 0;
			yOf = 239-yOf1;
			H = 240;
			break;
		default:
			ystart = 0;
			yOf = 479-yOf1;
			H = yOf1;
			break;
		}
		BYTE* P1 = &m_pFightBack[(xOf + yOf * 640) << 1];
		BYTE* P2 = &g_pMainState->m_Map.m_pMap->m_pBitmap[k][((xstart) + (ystart) * 320) << 1];
		int w2 =W << 1;
		for (int y = 0; y <H; y++)
		{
			memcpy_s(P1, 1280, P2, w2);
			P1 += 1280;
			P2 += 640;
		}	
	}
	int index, index1, index2;
	WPixel pix1, pix2;
	BYTE* lpSrc = m_FightBack1.m_Picture.Spr.lpDate[0];

	int index11 = (m_FightBack1.m_Picture.m_yOffset2) * 800 + m_FightBack1.m_Picture.m_xOffset2;
	index1 = index11;
	index = index1 << 1;
	index2 = 0;
	for (int i = 0; i < 480; i++)
	{
		for (int j = 0; j < 640; j++)
		{
			pix1.l = lpSrc[index];
			pix1.h = lpSrc[index + 1];
			pix2.l = m_pFightBack[index2];
			pix2.h = m_pFightBack[index2 + 1];

			pix2.red = (pix1.red + pix2.red) >> 1;
			pix2.green = (pix1.green + pix2.green) >> 1;
			pix2.blue = (pix1.blue + pix2.blue) >> 1;
			m_pFightBack[index2] = pix2.l;
			m_pFightBack[index2 + 1] = pix2.h;

	
			index1 += 1;
			index += 2;
			index2 += 2;
		}
		index11 += 800;
		index1 = index11;
		index = index11 << 1;
	}
	

	for (int i = 0; i < 300; i++)
	{
		if (!g_pMainState->m_Canvas.m_CanvasCube[i].m_bCover)
			g_pMainState->m_Canvas.m_CanvasCube[i].PrePicNum = -1;
	}
	m_ThreadSleep = TRUE;

	m_Frame.SetXY(200, 200);
	m_Frame.SetColor(RGB(200, 0, 0));
	g_pMainState->m_Map.m_pMap->m_bMove = FALSE;

	m_turn = 0;
	g_pMainState->m_TriggerFightStart.Process();
	return TRUE;
}

BOOL cFightState::Frame(void* pData/*=0*/)
{
	if (!isInit)Init();
	g_pMainState->SetMouseOnMap(TRUE);
	g_pMainState->UpDateInput();
	g_pMainState->ProcessInterface();
	if (g_StateType != STATE_FIGHT)return TRUE;
	DoWork();
	if(g_StateType != STATE_FIGHT)return TRUE;

	//����
	if (m_bEscape)
	{
		EscapeFight();
		return TRUE;
	}
	//��ʾ��ɫ
	Sort();

	for (int i = 0; i < m_ShowTurnUseNum; i++)
	{
		switch (m_ShowTurnType[i])
		{
		case 1:
			
			{
				sFightList* pShowList = (sFightList*)m_pShowTurn[i];
				cObj* pObj = pShowList->m_pData;
				g_pMainState->m_Canvas.Add(pShowList->m_pData->GetShadow(), 1);
				pShowList->ShowOnScreen(&g_pMainState->m_Canvas);
				if (pObj->m_IDInFightlist<10||(0==g_pMainState->m_AI.m_AttackAI.m_bPKMode))
				{
					//��ʾѪ��
					if ((0 == pShowList->m_work.size()) && (pShowList->m_pData->m_NowPos == POS_FIGHT_STAND))
					{
						g_pMainState->m_Canvas.Add(&pObj->m_HpEmptyLinep, 1);
						g_pMainState->m_Canvas.Add(&pObj->m_HpLinep, 1);
					}
				}
			}
			break;
		case 2:
			
			{
				cWfileP* pWfilep = (cWfileP*)m_pShowTurn[i];
				g_pMainState->m_Canvas.Add(pWfilep, 1);
			}
			break;
		}

	}
	for (int i = 0; i < 3; i++)
	{
		if (m_FullSkill.m_FullSkilUP[i].m_NeedShow)
		{
			g_pMainState->m_Canvas.Add(&m_FullSkill.m_FullSkilUP[i], 1);
			if (m_FullSkill.m_FullSkilUP[i].GetMaxFrame()>1)
			if (m_FullSkill.m_FullSkilUP[i].GetNowFrame() == m_FullSkill.m_FullSkilUP[i].GetMaxFrame() - 1)
			{
				m_FullSkill.m_FullSkilUP[i].SetFrame(0);
				m_FullSkill.m_FullSkilUP[i].m_NeedShow = FALSE;
			}
		}
	}
	//��ʾ�˺�
	for (int i = 0; i < 20; i++)
	{
		if (m_pList[i]->isLive)
		{
			if (m_pList[i]->m_DamageTime[0] < 71)
				m_pList[i]->ShowDamage(g_pMainState);
		}
	}
	if (m_MiaoShu.m_NeedShow)
		m_MiaoShu.Show(&g_pMainState->m_Canvas);
	g_pMainState->ShowInterface();
	if (m_FightMenu.m_NeedShow)
	m_FightMenu.Show(&g_pMainState->m_Canvas);
	g_pMainState->ShowInterface2();


	g_pMainState->m_Canvas.Show();
	g_pMainState->m_Mp3.CheckEnd();
	return TRUE;
}

BOOL cFightState::Free()
{
	SAFE_DELETE_ARRAY(m_pFightBack);
	return TRUE;
}

BOOL cFightState::ProcessInput()
{

	g_pMainState->ProcessInterface();

	return TRUE;
}



int cFightState::GetSelect(long x, long y)
{
	for (int i = 0; i < 20; i++)
	{
		if (m_pList[i]->isLive)
		{
			if (m_pList[i]->m_pData->m_pShowData[0]->isSelectOn(x, y))
				return i;
			if (m_pList[i]->m_pData->m_pShowData[1]->m_NowID)
			if (m_pList[i]->m_pData->m_pShowData[1]->isSelectOn(x, y))
				return i;
			if (m_pList[i]->m_pData->m_pShowData[2]->m_NowID)
			if (m_pList[i]->m_pData->m_pShowData[2]->isSelectOn(x, y))
				return i;
		}
	}


	return -1;
}



BOOL cFightState::DoWork()
{
	cObj* pUser;
	cPetData* pUserData;

	switch (m_turn)
	{
	case 0: //�ȴ�
		m_FightMenu.m_NeedShow = TRUE;
		m_FightMenu.ProcessInput();
		break;
	case 1:
		SortBySpeed();
		g_pMainState->m_TriggerFightTurn.Process();
		if (g_StateType != STATE_FIGHT)return TRUE;
		g_pMainState->m_AI.m_AttackAI.Run();
		m_FightMenu.m_NeedShow = FALSE;
		m_nowturnid = 0;
		m_turn += 1;
	case 2:
		while (true)
		{
			if (m_nowturnid > 19)
			{
				m_turn = 4;
				break;
			}
			sFightList*& list = m_pList[m_SpeedListPos[m_nowturnid]];
			if (list->m_work.size())
				break;
			if (list->m_WorkType > -1)
			{
				if (!g_pMainState->GetLiveNum(10, 20))
				{
					//սʤ��
					SuccessFight();
					return TRUE;
				}
				if (!g_pMainState->GetLiveNum(0, 10))
				{
					//ս����
					FalseFight();
				
					return TRUE;
				}
				if (list->CheckDie())
				{
					m_nowturnid += 1;
					continue;
				}
				SetWork(m_SpeedListPos[m_nowturnid], list->m_WorkType);
				
				list->m_WorkType = -1;

				m_turn += 1;
				break;
			}
			else
			{
				m_nowturnid += 1;
				if (m_nowturnid == 20)
				{
					m_turn = 4;
					break;
				}
			}
		}
		break;
	case 3://�ȴ�1����ɫ�ж����
		
		{
			int i3 = 0;
			for (; i3 < 20; i3++)
			{
				if (m_pList[i3]->isLive)
				{
					if (m_pList[i3]->m_work.size())
						break;
				}
			}
			if (i3 == 20)
			{
				m_nowturnid += 1;
				m_turn = 2;
				if (m_nowturnid == 20) //һ�غϽ���
				{
					m_turn = 4;
				}
			}
		}
		break;
	case 4://׼����һ�غ�
		
		{//ս���������
			
			if (!g_pMainState->GetLiveNum(10, 20))
			{
				//սʤ��
				SuccessFight();
				return TRUE;
			}
			if (!g_pMainState->GetLiveNum(0, 10))
			{
				//ս����
				FalseFight();
				
				return TRUE;
			}

			for (int i = 0; i < 20; i++)
			{
				//m_pList[i]->targetID = -1;
				if (m_pList[i]->isLive)
				{
					m_pList[i]->m_pData->GetDateP(pUserData);
					pUserData->m_FightProperty.AttackNum = 0;
					pUserData->m_FightProperty.SufferNum = 0;
					m_pList[i]->RestTarget();
					m_pList[i]->m_work.clear();
					m_pList[i]->m_WorkType = -1;
				}
			}
			
			for (int i = 0; i < 20; i++)
			{
				if (!m_pList[i]->isLive)continue;
				//���½�ɫBUFF
				g_pMainState->m_SkillManager.CheckFightState(i);
				pUser = g_pMainState->m_FightList[i].m_pData;
				pUser->GetDateP(pUserData);
				//״̬�����
				if (pUserData->restoreRate>0)
				{
					if (rand() % 100<pUserData->restoreRate)
					{
						pUser->ReMoveState(eBuffType��ӡ);
						pUser->ReMoveState(eBuffType��);
						pUser->ReMoveState(eBuffType����);
					}
				}
				

				if (pUserData->m_FightProperty.m_TurnHPHurtAdd)
					g_pMainState->m_SkillManager.AddHpHurt(i, pUserData->m_FightProperty.m_TurnHPHurtAdd);
				if (pUserData->m_FightProperty.m_TurnHPAdd)
					g_pMainState->m_SkillManager.AddHp(i, pUserData->m_FightProperty.m_TurnHPAdd);
				if (pUserData->m_FightProperty.m_TurnMPAdd)
					g_pMainState->m_SkillManager.AddMp(i, pUserData->m_FightProperty.m_TurnMPAdd);
				if (pUserData->m_LiveCp)
					g_pMainState->m_SkillManager.AddCp(i, pUserData->m_LiveCp);

			}
		}
		m_turn = 0;
		return TRUE;
	}
	for (int i = 0; i < 20; i++)
	{
		if (m_pList[i]->isLive)
		{
			if (m_pList[i]->m_work.empty())
				continue;
			for (int j = 0; j < m_pList[i]->m_work.size(); j++)
			{
				pUser = g_pMainState->m_FightList[i].m_pData;
				pUser->GetDateP(pUserData);
				switch (m_pList[i]->GetWork()->type)
				{
				case WORKTYPE_MAGICSUFFER:
					MagicSuffer(i);
					break;
				case WORKTYPE_SUFFER:  //����,ʲôʱ������ɹ����߾���
					Suffer(i);
					break;
				case WORKTYPE_ATTACK:
					//�Ƿ񱻷�
				
					{
						if (pUserData->m_FightProperty.cannotAttack)
						{
							m_pList[i]->m_work.clear();
							if (i == g_pMainState->m_HeroID)
								g_pMainState->m_Channel.AddNewString("�޷�ʹ��������");
							break;
						}
						if (g_pMainState->m_SkillManager.Use(m_pList[i]->m_SkillSelect, i))
						{
						}
					}
					break;
				case WORKTYPE_MAGIC2:
					g_pMainState->m_SkillManager.Use(m_pList[i]->m_SkillSelect, i);
					
					
					break;
				case WORKTYPE_MAGIC:
					//�Ƿ񱻷�
					
					g_pMainState->m_SkillManager.Use(m_pList[i]->m_SkillSelect, i);
						
						
					break;
				case WORKTYPE_ITEM:
					//ʹ�õ���
					
					
						if (pUserData->m_FightProperty.cannotAll)
						{
							m_pList[i]->m_work.clear();
							if (i == g_pMainState->m_HeroID)
								g_pMainState->m_Channel.AddNewString("�޷�ʹ�õ���");
							break;
						}
						g_pMainState->m_SkillManager.Use(m_pList[i]->m_SkillSelect, i);
						
						
					
					break;
				case WORKTYPE_SUMMON:
					if (pUserData->m_FightProperty.cannotAll)
					{
						m_pList[i]->m_work.clear();
						if (i == g_pMainState->m_HeroID)
							g_pMainState->m_Channel.AddNewString("�޷��ٻ�");
						break;
					}
					if (g_pMainState->m_SkillManager.Use(m_pList[i]->m_SkillSelect, i))
					{

					}
					break;
				case WORKTYPE_DEFEND:
					Defend(i);
					break;
				case WORKTYPE_FLY:
					if (!m_pList[i]->DieFly())
					{
						BackFromFight(i);
					}
					break;
				case WORKTYPE_FALLDOWN:
					FallDown(i);
					break;
				case  WORKTYPE_CATCH:
					if (g_pMainState->m_SkillManager.Use(m_pList[i]->m_SkillSelect, i))
					{
					}
					break;
				case WORKTYPE_AVOID:
					Avoid(i);
					break;
				case WORKTYPE_RETURN:
					if (ReturnApproach(i))
					{
						m_pList[i]->m_work.pop_back();
						return TRUE;
					}
					break;
				case WORKTYPE_COUNTER:
					if (Counter(i))
					{

					}
					break;
				default:
					ERRBOX;
					break;
				}
			}
		}
	}
	return FALSE;
}
BOOL cFightState::Guard(int ID,int xTarget, int yTarget)
{	
//	sFightList* pFigh = m_pList[ID];
	float Speed = 30.0f;
	int x = m_pList[ID]->m_pData->GetX();
	int y = m_pList[ID]->m_pData->GetY();
	if (x == xTarget&&y == yTarget)return TRUE;
	//�õ���ȷ��Ŀ��λ��
	//int direction1 = GetDirection4(x, y, xTarget, yTarget);
	if (x != xTarget&&y != yTarget)
	{
		m_pList[ID]->m_pData->SetDirection(GetDirection4(x, y, (int)xTarget, (int)yTarget));
	}
	if (abs(x - xTarget) < 20 && abs(y - yTarget) < 20)
	{
		m_pList[ID]->m_pData->SetPos(xTarget, yTarget);
		m_pList[ID]->m_pData->MoveHpLine();
		m_pList[ID]->m_pData->MoveDialog();
		return TRUE;
	}
	float radian = atan2(
		(float)(yTarget - y),
		(float)(xTarget - x)
		);
	float yOffset = Speed * sin(radian);
	float xOffset = Speed * cos(radian);
	m_pList[ID]->m_pData->SetPos(x + xOffset, y + yOffset);
	m_pList[ID]->m_pData->MoveHpLine();
	m_pList[ID]->m_pData->MoveDialog();
	return FALSE;
}
BOOL cFightState::Approach(int ID, int TargetID,BOOL gap)
{
	int xTarget =m_pList[TargetID]->m_pData->GetX();
	int yTarget =m_pList[TargetID]->m_pData->GetY();
	m_pList[ID]->m_pData->Set(POS_RUN);
	m_pList[ID]->m_pData->SetStatic(FALSE);
//	sFightList* pFigh = m_pList[ID];
	sFightOffset* pOffse = m_pList[ID]->GetFightOffset();
	int xDistance = pOffse->m_xOff;
	int yDistace = pOffse->m_yOff;
	if (gap)
	{
		xDistance = 0;
		yDistace = 0;
	}
	float Speed = 30.0f;
	int x = m_pList[ID]->m_pData->GetX();
	int y = m_pList[ID]->m_pData->GetY();
	if (x == xTarget&&y == yTarget)
	{
		return TRUE;
	}
	int xTargetold = (int)xTarget;
	int yTargetold = (int)yTarget;
	
	//�õ���ȷ��Ŀ��λ��
	int direction1 = GetDirection4(x, y, xTargetold, yTargetold);

	if (xDistance||yDistace)
	{
		switch (direction1)
		{
		case 9:
			xTarget -= xDistance;
			yTarget += yDistace;
			break;
		case 7:
			xTarget += xDistance;
			yTarget += yDistace;
			break;
		case 3:
			xTarget -= xDistance;
			yTarget -= yDistace;
			break;
		case 1:
			xTarget += xDistance;
			yTarget -= yDistace;
			break;
		}
	}
	//ȷ������
	int  direction2;
	if (x!=xTarget&&y!=yTarget)
		  direction2 = GetDirection4(x, y, (int)xTarget, (int)yTarget);
	else   direction2 = m_pList[ID]->m_pData->GetDirection();
	if (direction2 != m_pList[ID]->m_pData->GetDirection())
	{
		m_pList[ID]->m_pData->SetDirection(direction2);
	}
	int k = (x - xTarget)*(x - xTarget) + (y - yTarget)*(y - yTarget);
	if (k<=Speed*Speed)  //�����Ŀ��ֻ��һ���
	{
		m_pList[ID]->m_pData->Set(POS_RUN);
		m_pList[ID]->m_pData->SetPos(xTarget, yTarget);
		m_pList[ID]->m_pData->SetDirection(GetDirection4(x, y, xTargetold, yTargetold));
		m_pList[ID]->m_pData->Move2();
		return  TRUE;
	}
	float radian= atan2(
		(float)(yTarget - y),
		(float)(xTarget - x)
		);
	float yOffset = Speed * sin(radian);
	float xOffset = Speed * cos(radian);

	m_pList[ID]->m_pData->Set(POS_RUN);
	m_pList[ID]->m_pData->SetPos(x + xOffset, y + yOffset);
	m_pList[ID]->m_pData->Move2();
	return FALSE;
}

void cFightState::Sort()
{
	m_ShowTurnUseNum = 0;
	int yList[30];
	int xoffsetList[30];
	int dustbin;
	void* pdustbin;

	for (int i = 0; i < 20; i++)
	{
		if (m_pList[i]->isLive == TRUE)
		{
			m_pShowTurn[m_ShowTurnUseNum] = m_pList[i];
			m_ShowTurnType[m_ShowTurnUseNum] = 1;
			yList[m_ShowTurnUseNum] = m_pList[i]->m_pData->GetShadow()->m_y;
			xoffsetList[m_ShowTurnUseNum] = m_pList[i]->m_pData->GetShadow()->m_x;
			m_ShowTurnUseNum += 1;

		}
	}
	//����
	for (int i = 0; i < m_HiddenWeaponsUseNum; i++)
	{
		m_pShowTurn[m_ShowTurnUseNum] = &m_HiddenWeaponsp[i];
		yList[m_ShowTurnUseNum] = m_HiddenWeaponsp[i].m_y;// *100 / (m_HiddenWeaponsp[i].m_x);
		xoffsetList[m_ShowTurnUseNum] = m_HiddenWeaponsp[i].m_x;
		m_ShowTurnType[m_ShowTurnUseNum] = 2;
		m_ShowTurnUseNum += 1;
	}

	for (int i = 0; i < m_ShowTurnUseNum; i++)
	for (int j = i + 1; j < m_ShowTurnUseNum; j++)
	{
		if (yList[i] >= yList[j])
		{
			if (yList[i] == yList[j])
			{
				if (xoffsetList[i] < xoffsetList[j])continue;
			}
			dustbin = yList[i];
			yList[i] = yList[j];
			yList[j] = dustbin;

			dustbin = m_ShowTurnType[i];
			m_ShowTurnType[i] = m_ShowTurnType[j];
			m_ShowTurnType[j] = dustbin;

			pdustbin = m_pShowTurn[i];
			m_pShowTurn[i] = m_pShowTurn[j];
			m_pShowTurn[j] = pdustbin;
		}
	}

}

void cFightState::SetTarget(int ID, int Target)
{
	m_pList[ID]->targetID = Target;
}

BOOL cFightState::ReturnApproach(int ID,  float Speed /*= 20.0f*/)
{
	int xTarget = m_xPosList[ID];
	int yTarget = m_yPosList[ID];
	int x =(int)m_pList[ID]->m_pData->GetX();
	int y =(int)m_pList[ID]->m_pData->GetY();
	cObj* pObj = m_pList[ID]->m_pData;
	//�Ƿ��Ѿ���Ŀ�ĵ�
	if (abs(xTarget- x)<2 && abs(yTarget - y)<2)
	{
		if (ID < 10)
		{
			pObj->SetDirection(7);
		}
		else
		{
			pObj->SetDirection(3);
		}
		pObj->SetPos(xTarget, yTarget);
		pObj->Set(POS_FIGHT_STAND);
		pObj->Move2();
		return TRUE;
	}	
	int direction = GetDirection4(x, y, xTarget, yTarget);
	m_pList[ID]->m_pData->Set(POS_RETURN);
	CWFile* pdata = pObj->m_pShowData[0];
	if (direction != pObj->GetDirection())  //���Ҫ�ƶ��ķ����ԭ���ķ���ͬ,��ת��
	{
		int nowframe1 = pdata->GetNowFrame();
		if (nowframe1 != pdata->GetMaxFrame() - 1 && pdata->GetWillNextFrame())
		{
			pdata->SetFrame(0);
			pObj->Move2();
			pObj->SetDirection(direction);
			return FALSE;
		}
	}
	int k =(int)( (x - xTarget)*(x - xTarget) + (y - yTarget)*(y - yTarget));
	if (k>0&&k <= Speed*Speed)  //�����Ŀ��ֻ��һ���
	{	
		pdata->SetFrame(pdata->GetMaxFrame() - 1);
		pObj->SetPos(xTarget, yTarget);
		pObj->Move2();
		return FALSE;
	}
	float radian = atan2(
		(float)(yTarget - y),
		(float)(xTarget - x)
		);
	float yOffset = Speed * sin(radian);
	float xOffset = Speed * cos(radian);
	pObj->SetPos((int)(x + xOffset), (int)(y + yOffset));
	//��ֹ�������һ֡
	if (pdata->GetNowFrame() == pdata->GetMaxFrame() - 2)
		pdata->SetFrame(0);
	pObj->SetDirection(direction);
	pObj->Move2();
	return FALSE;
}

BOOL cFightState::Suffer(int ID)
{
	int& time=m_pList[ID]->GetWork()->stage;
	cObj* pTarget = m_pList[ID]->m_pData;
	
	for (int i = 0; i<3; i++)
	{
		pTarget->m_pShowData[i]->m_bMove = TRUE;
	}
	if (time == 30)
	{
		pTarget->SetDirection(GetReverseDirection(m_pList[ID]->m_sufferDirection));
		pTarget->Set(POS_SUFFER);
		pTarget->SetStatic(TRUE);
		
		pTarget->SetFrame(1);
		
			m_Sufferspd =2;
			time -= 4;
			cPetData* pet;
			pTarget->GetDateP(pet);
			if (pet->m_HP<1)
			{
				if (m_pList[ID]->m_NowBeAtkowner>-1)
				{
					m_pList[m_pList[ID]->m_NowBeAtkowner]->m_pData->SetSpeed(10);
				}
			}
	}
	else
	if (time>10)
	{
	//	int maxf = pTarget->m_pShowData[0]->GetNowFrame();
		time -= 1;
		pTarget->SufferBack(m_Sufferspd, m_pList[ID]->m_sufferDirection);
				
	}
	else
	if (time > 0)
	{
		cPetData* pTargetData;
		pTarget->GetDateP(pTargetData);
		if (1 > pTargetData->m_HP)
		{
			time -= 1;
			return TRUE;
		}
		else
		{
			time = 0;
			return TRUE;
		}
	}
	else
	if (time == 0)
	{
		cPetData* pTargetData;
		pTarget->GetDateP(pTargetData);
		if (pTargetData->m_Controler.GetFly())
		{
			
			if (1 > pTargetData->m_HP)
			{
				time = -100;
				return TRUE;
			}
		}
		time -= 1;
		//pTarget->m_pShowData[0]->SetFrame(1);
	}
	else
	if (time>-100)
	{
		if (abs(pTarget->m_xPos - pTarget->m_oldx)<6.0f )
		{
			if (abs(pTarget->m_yPos - pTarget->m_oldy) < 6.0f)
			{
				pTarget->SetX(pTarget->m_oldx);
				pTarget->SetY(pTarget->m_oldy);
				time = -100;
				return TRUE;
			}
		}
		time -= 1;
			int direction = GetReverseDirection(m_pList[ID]->m_sufferDirection);
			pTarget->SetDirection(direction);
		pTarget->SufferBack(m_Sufferspd, direction);
		pTarget->Set(POS_SUFFER);
		

	}
	else  if (-100 == time)
	{
		cPetData* pTargetData;
		pTarget->GetDateP(pTargetData);
		if (1 > pTargetData->m_HP)
		{
			SetDie(ID);
			return TRUE;
		}
		time -= 1;
	}
	else 
	{
		pTarget->Set(POS_FIGHT_STAND);
		pTarget->SetFrame(pTarget->m_pShowData[0]->GetMaxFrame()-2);//���ٵȴ�ʱ��
		pTarget->Move2();
		m_pList[ID]->m_work.pop_back();
		pTarget->SetStatic(FALSE);
		return TRUE;
	}
	
	return FALSE;
}

BOOL cFightState::MagicSuffer(int ID)
{
	cObj* pobj = m_pList[ID]->m_pData;
	for (int i = 0; i < 4; i++)
	{
		pobj->m_pShowData[i]->m_bMove = TRUE;
	}
	return TRUE;
}

int cFightState::AutoSelect(BOOL Enemy)
{
	int startNum = rand() % 10;
	int offset = 0;
	if (!Enemy)
	{
		offset = 10;
	}
	int i = 0;
	for (; i < 10; i++)
	{
		if (m_pList[startNum + offset]->isLive)
			break;
		else
			startNum = (startNum + 1) % 10;
	}
	if (i == 10)return -1;
	return startNum + offset;
}

int cFightState::WorkAttack(int i)
{
	CWFile* ShowData = m_pList[i]->m_pData->m_pShowData[0];
	int point0;
	int point1;
	
	if (m_pList[i]->m_pData->m_NowPos == POS_ATTACK)
	{
		point0 = m_pList[i]->GetFightOffset()->m_Point[0];
		point1 = m_pList[i]->GetFightOffset()->m_Point[1];
	}
	else
	{
		point0 = m_pList[i]->GetFightOffset()->m_Point2[0];
		point1 = m_pList[i]->GetFightOffset()->m_Point2[1];
	}
	////�Ƿ񵽹�����
	int nowfrme = ShowData->GetNowFrame();
	if (ShowData->GetMaxFrame() <= point0)point0 = ShowData->GetMaxFrame() - 1;
	if ( nowfrme== point0 && ShowData->GetWillNextFrame())
	{
		if (point0 == point1)
			return 3; //��һ��+�ڶ���
		return 1;//��һ��
	}
	//�Ƿ��˺��� �ڶ���
	if (ShowData->GetMaxFrame() <= point1)point1 = ShowData->GetMaxFrame() - 1;
	if (ShowData->GetNowFrame() == point1 && ShowData->GetWillNextFrame())
		return 2;
	return 0;
}

BOOL cFightState::WaitPosEnd(int i)
{
	CWFile* data = m_pList[i]->m_pData->m_pShowData[0];
	if (data->GetMaxFrame() - data->GetNowFrame() == 1)
	{
		if (data->GetWillNextFrame())
			return TRUE;
	}
	return FALSE;
}

BOOL cFightState::SetSuffer(int ID, int direction, int userid)
{
	cPetData* petdata;
	m_pList[ID]->m_pData->GetDateP(petdata);

	m_pList[ID]->m_NowBeAtkowner = userid;

	
	m_pList[ID]->m_pData->SetSound(POS_SUFFER,TRUE);
	
	m_pList[ID]->m_sufferDirection = direction;

	if (m_pList[ID]->m_work.size())
	{
		if (WORKTYPE_SUFFER == m_pList[ID]->GetWork()->type)
		{
			m_pList[ID]->GetWork()->stage = 30;
			return TRUE;
		}
	}
	m_pList[ID]->SetWork(WORKTYPE_SUFFER);
	m_pList[ID]->GetWork()->stage = 30;
	m_pList[ID]->m_pData->m_oldx = m_pList[ID]->m_pData->m_xPos;
	m_pList[ID]->m_pData->m_oldy = m_pList[ID]->m_pData->m_yPos;
	return TRUE;
}

BOOL cFightState::SetWork(int ID, int Type)
{
	m_pList[ID]->SetWork(Type);
	return TRUE;
}

BOOL cFightState::SetMagicSuffer(int ID, int direction)
{
	//m_pList[ID]->m_work.clear();
	m_pList[ID]->SetWork(WORKTYPE_MAGICSUFFER);
	cObj* pObj = m_pList[ID]->m_pData;
	pObj->SetShake(TRUE);
	pObj->Set(POS_SUFFER);
	pObj->SetStatic(TRUE);
	m_pList[ID]->m_sufferDirection = direction;
	pObj->SetDirection(GetReverseDirection(direction));
	for (int i = 0; i < 3; i++)
	{
		if (pObj->m_pShowData[i]->m_NeedShow)
		{
			pObj->m_pShowData[i]->SetFrame(1);
			pObj->m_pShowData[i]->m_bMove = TRUE;
		}
	}

	return TRUE;
}



BOOL cFightState::SwitchMagicSuffer(int ID)
{
	sFightList* p = m_pList[ID];
	if (p->m_work.size())
	{
		p->m_work.pop_back();
	}
	p->m_pData->SetShake(FALSE);
	p->m_pData->SetStatic(FALSE);
	SetSuffer(ID, p->m_sufferDirection, -1);
	return TRUE;
}
BOOL cFightState::SwitchMagicSufferAvoid(int ID)
{
	sFightList* p = m_pList[ID];
	if (p->m_work.size())
	{
		p->m_work.pop_back();
	}
	p->m_pData->SetShake(FALSE);
	p->m_pData->SetStatic(FALSE);
	SetAvoid(ID, p->m_sufferDirection);
	return TRUE;
}



BOOL cFightState::SortBySpeed()
{
	cObj* pObj[2];
	cPetData* pPetData[2];
	int* spd1,* spd2;
	int posid;
	for (int i = 0; i < 20; i++)
	for (int j = i + 1; j < 20; j++)
	{
		if (m_pList[m_SpeedListPos[i]]->isLive && m_pList[m_SpeedListPos[j]]->isLive)
		{
			pObj[0] = m_pList[m_SpeedListPos[i]]->m_pData;
			pObj[1] = m_pList[m_SpeedListPos[j]]->m_pData;
			pObj[0]->GetDateP(pPetData[0]);
			pObj[1]->GetDateP(pPetData[1]);


			spd1 = pPetData[0]->m_SPD.GetDatap();
			spd2 = pPetData[1]->m_SPD.GetDatap();
			if (*spd1 < *spd2)
			{
				posid = m_SpeedListPos[i];
				m_SpeedListPos[i] = m_SpeedListPos[j];
				m_SpeedListPos[j] = posid;
			}
		}
	}
	return TRUE;
}

int cFightState::CheckTarget(int ID, int target,int skillid)
{
	if (m_pList[target]->isLive)
	{
		if (g_pMainState->m_FightList[target].CheckDie())
		{
			switch (skillid)
			{
			case 37:
			case 104:
			case 125:
			case 3008:
			case 3009:
			case 3048:
				return target;
				break;
			case 1003:
				if (1)
				{
					sItem2* item = g_pMainState->m_SkillManager.GetItem(ID);
					if (item->GetType() == 22)
					{
						switch (item->GetID())
						{
						case 44:
						case 45:
						case 51:
						case 52:
						case 60:
							return target;
							break;
						}
					}
				}
				break;
			default:
				break;
			}
		}
		else return target;
	}
	

	if (ID < 10 && target < 10)
	{
		return g_pMainState->m_AI.AutoFindTarget(ID, 3);
	}
	else
	if (ID > 9 && target > 9)
	{
		return g_pMainState->m_AI.AutoFindTarget(ID, 3);
	}
	else
	{
		return g_pMainState->m_AI.AutoFindTarget(ID, 4);
	}
}

void cFightState::SetDie(int ID)
{
	m_pList[ID]->m_work.clear();
	m_pList[ID]->m_WorkType = -1;
	m_pList[ID]->m_pData->m_MagicSuffer.m_NeedShow = FALSE;
	cPetData* petdata;
	m_pList[ID]->m_pData->GetDateP(petdata);
	m_pList[ID]->m_DiePoint = 2;
	if(!petdata->m_Controler.GetFly())
	{
	//	m_pList[ID]->m_pData->SetSound(POS_DEAD);
		SetWork(ID, WORKTYPE_FALLDOWN);	
	}
	else
	{
		m_pList[ID]->m_pData->m_Wav.LoadWav(1382520422);
		SetWork(ID, WORKTYPE_FLY);
	}
}



BOOL sFightList::SetMagiSufferID(DWORD MagicID, int MagicPos, CWFile* pFile,BOOL up)
{
	return  m_pData->SetMagiSufferID( MagicID,  MagicPos,  pFile,  up);
}

BOOL sFightList::ShowOnScreen(cCanvas* pCanvas)
{
	if (!isLive)return TRUE;
	m_pData->ShowOnScreen(pCanvas);
	
	return TRUE;
}

BOOL sFightList::ShowDamage(CMainState* p)//����
{
	if (m_DamageTime[0] > 70)
	{
		return TRUE;
	}
	for (int i = 0; i <m_DamageNum; i++)
	{
		SetDamageRect(i);	
		m_Damage[i].m_bMove = TRUE;
		p->m_Canvas.Add(&m_Damage[i],1); 
		m_DamageTime[i] += 1;
	}
	return TRUE;
}

BOOL sFightList::SetDamageRect(int Num)
{
	int& time =m_DamageTime[Num];
	int speed = 4;
	if (time < 36)
	{
		if (time>29)
		{
			m_Damage[Num].m_y += speed;
		}
		else
		if (time > 25)
		{
			m_Damage[Num].m_y -= speed;
		}
		else
		if (time > 20)
		{
			m_Damage[Num].m_y -= speed;
			m_Damage[Num].SetShowHeight(m_Damage[Num].GetShowHeight()+speed);
		}
	}
	//int woff = 16;
	m_Damage[Num].m_bMove=TRUE;

	/*m_Damage[Num].SetXY(pData->GetX() - Num*woff + (m_DamageNum/ 2) * woff - woff / 2
		,(pData->GetY() + pData->GetTopY())/2 + m_DamageYOffset[Num]+10);*/
	return TRUE;
}

sFightList::sFightList()
{
	targetlist.resize(10);
	m_SleepTime = 0;
	m_DamageNum = 0;
	isLive = 0;
	m_pData = 0;
	RestTarget();
	
	m_SkillPreSelect.SkillID = -1;
	for (int i = 0; i < 5; i++)
	{
		//m_Damage[i].SetStyle(2);
		//m_Damage[i].m_bBold = TRUE;
		//m_Damage[i].SetBackColor(RGB(255,255,255));
		m_DamageTime[i] = 100;
	}
}


BOOL sFightList::Sleep(int Time, BOOL bStatic)
{
	if (bStatic)m_pData->SetStatic(TRUE);

	if (m_SleepTime == Time)
	{
		if (bStatic)m_pData->SetStatic(FALSE);
		m_SleepTime = 0;
		return TRUE;
	}
	m_SleepTime += 1;
	return FALSE;
}


BOOL sFightList::NextTarget(cFightState* pFightState, int sSkillID)
{
	BOOL bCheckDie=TRUE;
	switch (sSkillID)
	{
	case 37:
	case 104:
	case 125:
	case 3008:
	case 3009:
	case 3048:
		bCheckDie = FALSE;
	}


	m_NowTarget += 1;
	if (m_NowTarget == m_MaxTargetNum)
	{
		m_NowTargetID = -1;
		return FALSE;
	}
	m_NowTargetID = targetlist[m_NowTarget];
	if (-1 == m_NowTargetID)return FALSE;
	if (pFightState->m_pList[m_NowTargetID]->isLive)
	{
		if (bCheckDie)
		{
			cPetData* data;
			pFightState->m_pList[m_NowTargetID]->m_pData->GetDateP(data);
			if (data->m_HP > 0)
				return TRUE;
		}
		else
		return TRUE;
	}
	return NextTarget(pFightState, sSkillID);
}

BOOL sFightList::DieFly()
{
	if (!m_bFlyInit)
		InitFly();
	int& stage = GetWork()->stage;
	if (stage == 0)
	{
		if (Sleep(5))
		{
			m_pData->Set(POS_SUFFER);
			m_pData->SetStatic(TRUE);
			stage += 1;
		}
		else
			return TRUE;
	}
	else
	if (stage == 1)
	{
		if (m_pData->GetX() < 0)
		{
			if (m_DiePoint >0)
			{
				m_DiePoint -= 1;
				if (m_sufferDirection == 1)
				{
					m_sufferDirection = 3;
				}
				else
				if (m_sufferDirection == 7)
				{
					m_sufferDirection = 9;
				}
			}
		}
		else
		if (m_pData->GetX() > 630)
		{
			if (m_DiePoint > 0)
			{
				m_DiePoint -= 1;
				if (m_sufferDirection == 3)
				{
					m_sufferDirection = 1;
				}
				else
				if (m_sufferDirection == 9)
				{
					m_sufferDirection = 7;
				}
			}
		}
		else
		if (m_pData->GetY() < 0)
		{
			if (m_DiePoint >0)
			{
				m_DiePoint -= 1;
				if (m_sufferDirection == 9)
				{
					m_sufferDirection = 3;
				}
				else
				if (m_sufferDirection == 7)
				{
					m_sufferDirection = 1;
				}
			}
		}
		else
		if (m_pData->GetY() > 470)
		{
			if (m_DiePoint > 0)
			{
				m_DiePoint -= 1;
				if (m_sufferDirection == 1)
				{
					m_sufferDirection = 7;
				}
				else
				if (m_sufferDirection == 3)
				{
					m_sufferDirection = 9;
				}

			}
		}
		int speed = 14;
		int speed2 = 7;
		switch (m_sufferDirection)
		{
		case 1:
			m_pData->SetX(m_pData->GetX() - speed);
			m_pData->SetY(m_pData->GetY() + speed2);
			break;
		case 3:
			m_pData->SetX(m_pData->GetX() + speed);
			m_pData->SetY(m_pData->GetY() + speed2);
			break;
		case 7:
			m_pData->SetX(m_pData->GetX() - speed);
			m_pData->SetY(m_pData->GetY() - speed2);
			break;
		case 9:
			m_pData->SetX(m_pData->GetX() + speed);
			m_pData->SetY(m_pData->GetY() - speed2);
			break;
		}
		Rotate();
		
		if (0==m_DiePoint)
		{
			m_bFlyInit = FALSE;
			m_pData->SetStatic(FALSE);
			m_work.clear();
			return FALSE;
		}
	}
	m_pData->MoveHpLine();
	m_pData->MoveShadow();
	return TRUE;
}

BOOL sFightList::Rotate(int rotaterate)
{
	m_rotateRate += 1;
	m_rotateRate = m_rotateRate % rotaterate;
	if (m_rotateRate != 0)
		return TRUE;
	/*BOOL bPet=TRUE;
	if (pData->m_IndexType == INDEXTYEP_CHARACTER)
	{
	cCharacter* pc = (cCharacter*)pData->m_pIndex;
	if (!pc->m_PCData.m_bUseCard)
	{
	bPet = FALSE;
	}
	}*/

	m_pData->SetDirection(GetRotateDirection(m_pData->GetDirection(), TRUE));
	return TRUE;
}

BOOL sFightList::InitFly()
{
	m_DiePoint = 3;
	m_rotateRate = 0;
	m_bFlyInit = TRUE;
	return TRUE;
}

BOOL sFightList::Free()
{
	
	m_work.clear();
	m_WorkType = -1;
	if (m_pData)
	{
		m_pData->Free();
	}
	isLive = 0;
	m_bDie = FALSE;
	m_bFlyInit = FALSE;
	m_DiePoint = 0;
	return TRUE;
}


sFightOffset* sFightList::GetFightOffset()
{
	switch (m_pData->m_IndexType)
	{
	case INDEXTYEP_PET:
		if (1)
		{
			cPet* pe= (cPet*)m_pData->m_pIndex;
			return pe->GetFightOffset();
		}
		break;
	case INDEXTYEP_CHARACTER:
		if (1)
		{
			cCharacter* pc = (cCharacter*)m_pData->m_pIndex;
			return pc->GetFightOffset();
		}
	}
	return 0;
}

sFightList::~sFightList()
{
	Free();
	SAFE_DELETE(m_pData);
}

BOOL sFightList::Swap(sFightList& fightlist)
{
	/*if (!isLive)
	{
		fightlist.m_id

		return TRUE;
	}*/
	//if (!fightlist.isLive)return FALSE;
	/*cPetData* pPetdata1;
	cPetData* pPetdata2;
	m_pData->GetDateP(pPetdata1);
	fightlist.m_pData->GetDateP(pPetdata2);*/
	//swap(pPetdata1->m_IDinFightList, pPetdata2->m_IDinFightList);
	swap(m_pData, fightlist.m_pData);
	swap(isLive, fightlist.isLive);
	swap(m_FriendListid, fightlist.m_FriendListid);
	return TRUE;
}


void sFightList::RestTarget()
{
	m_NowTarget = -1;
	m_NowTargetID = -1;
	for (int i = 0; i < 7; i++)
		targetlist[i] = -1;
}

void sFightList::operator=(sFightList& flist)
{
	m_pData = flist.m_pData;
}

void sFightList::SetWork(int type)
{
	sWork work;
	work.type = type;
	work.stage = 0;
	//m_work.clear();
	m_work.push_back(work);
}
void sFightList::SetWorkFront(int type)
{
	sWork work;
	work.type = type;
	work.stage = 0;
	m_work.insert(m_work.begin(), work);
}









//����ɫ��һ��BUF
BOOL cFightState::AddState(int userID, sBuff buf)
{
	return m_pList[userID]->m_pData->AddState(buf);
}




BOOL cFightState::FallDown(int ID)
{
	int& stage = m_pList[ID]->GetWork()->stage;
	switch (stage)
	{
	case 0:
		m_pList[ID]->m_pData->Set(POS_DEAD);
		m_pList[ID]->m_pData->SetSound(POS_DEAD);
		m_pList[ID]->m_pData->SetStatic(FALSE);
		stage = 1;
		break;
	case 1:
		if (WaitPosEnd(ID))
		{
			m_pList[ID]->m_pData->SetStatic(TRUE);
			m_pList[ID]->m_work.clear();
		}
		break;
	}
	
	return TRUE;
}


BOOL cFightState::Avoid(int ID)
{
	sFightList* p = m_pList[ID];
	cObj* pObj = p->m_pData;
	int& time = p->GetWork()->stage;
	time -= 1;
	if (time % 2 != 0)return TRUE;
	for (int i = 0; i<3; i++)
	{
		pObj->m_pShowData[i]->m_bMove = TRUE;
	}
	if (14 == time)
	{
		pObj->m_oldx = (float)m_xPosList[ID];// pObj->m_xPos;
		pObj->m_oldy = (float)m_yPosList[ID];// pObj->m_yPos;
	}
	else
	if (time > 0)
	{
		pObj->SetDirection(GetReverseDirection(m_pList[ID]->m_sufferDirection));
		pObj->SufferBack(8.0f, p->m_sufferDirection);
	}
	else
	if (time > -3)
	{
	}
	else
	{
	//	int xp1 = m_pList[ID]->m_pData->GetX();
	//	int xp2 = (int)m_xPosList[ID];
	//	int yp1 = m_pList[ID]->m_pData->GetY();
		//int yp2 = (int)m_yPosList[ID];
		if (abs(p->m_pData->GetX() - m_xPosList[ID]) < 9 && abs(p->m_pData->GetY() - m_yPosList[ID]) < 9)
		{
			pObj->SetXY(pObj->m_oldx, pObj->m_oldy);
			p->m_work.pop_back();
		//	pObj->SetGhost(FALSE);
			return FALSE;
		}
		int direction = GetReverseDirection(p->m_sufferDirection);
		pObj->SetDirection(direction);
		pObj->SufferBack(8.0f, direction);
	}
	return FALSE;
}

BOOL cFightState::SetAvoid(int ID, int direction)
{
	m_pList[ID]->SetWork(WORKTYPE_AVOID);
	m_pList[ID]->GetWork()->stage = 15;
	m_pList[ID]->m_sufferDirection = direction;
//	m_pList[ID]->m_pData->SetGhost(TRUE);
	return TRUE;
}
BOOL cFightState::Defend(int ID)
{
	int& time = m_pList[ID]->GetWork()->stage;
	cObj* pTarget = m_pList[ID]->m_pData;

	for (int i = 0; i < 3; i++)
	{
		pTarget->m_pShowData[i]->m_bMove = TRUE;
	}
	if (time == 30)
	{
		pTarget->Set(POS_DEF);
	
		pTarget->SetStatic(TRUE);
		m_Sufferspd = 2;
		time -= 4;
		cPetData* pet;
		pTarget->GetDateP(pet);
		if (pet->m_HP<1)
		{
			if (m_pList[ID]->m_NowBeAtkowner>-1)
			{
				m_pList[m_pList[ID]->m_NowBeAtkowner]->m_pData->SetSpeed(10);
			}
		}
	}
	else
	if (time > 10)
	{
		time -= 1;
		pTarget->SufferBack(3, m_pList[ID]->m_sufferDirection);
	}
	else
	if (time > 0)
	{
		cPetData* pTargetData;
		pTarget->GetDateP(pTargetData);
		if (1 > pTargetData->m_HP)
		{
			time -= 1;
			return TRUE;
		}
		else
		{
			time = 0;
			return TRUE;
		}
	}
	else
	if (time == 0)
	{
		cPetData* pTargetData;
		pTarget->GetDateP(pTargetData);
		if (pTargetData->m_Controler.GetFly())
		{

			if (1 > pTargetData->m_HP)
			{
				time = -100;
				return TRUE;
			}
		}
		time -= 1;
	}
	else
	if (time > -100)
	{
		if (abs(pTarget->m_xPos - pTarget->m_oldx) < 6.0f)
		{
			if (abs(pTarget->m_yPos - pTarget->m_oldy) < 6.0f)
			{
				pTarget->SetX(pTarget->m_oldx);
				pTarget->SetY(pTarget->m_oldy);
				time = -100;
				return TRUE;
			}
		}
		time -= 1;
		pTarget->SufferBack(m_Sufferspd, GetReverseDirection(m_pList[ID]->m_sufferDirection));
	


	}
	else  if (-100 == time)
	{
		cPetData* pTargetData;
		pTarget->GetDateP(pTargetData);
		if (1 > pTargetData->m_HP)
		{
			SetDie(ID);
			return TRUE;
		}
		time -= 1;
	}
	else
	{
		pTarget->Set(POS_FIGHT_STAND);
		pTarget->SetFrame(pTarget->m_pShowData[0]->GetMaxFrame() - 1);
		
		pTarget->Move2();
		m_pList[ID]->m_work.pop_back();
		pTarget->SetStatic(FALSE);
		for (int i = 0; i < 3; i++)
		{
			pTarget->m_pShowData[i]->m_bMove = TRUE;
		}
		return TRUE;
	}

	return FALSE;
}

BOOL cFightState::SetDefend(int ID, int direction, int userid)
{
	cPetData* pet;
	m_pList[ID]->m_pData->GetDateP(pet);
	if (pet->m_HP < 1)
	{
		SetSuffer(ID, direction, userid);
		return TRUE;
	}
	m_pList[ID]->m_pData->Set(POS_DEF);
	m_pList[ID]->m_pData->SetSound(POS_DEF);
	m_pList[ID]->m_sufferDirection = direction;

	if (m_pList[ID]->m_work.size())
	{
		if (WORKTYPE_DEFEND == m_pList[ID]->GetWork()->type)
		{
			m_pList[ID]->GetWork()->stage = 30;
			return TRUE;
		}
	}
	m_pList[ID]->SetWork(WORKTYPE_DEFEND);
	m_pList[ID]->GetWork()->stage = 30;
	m_pList[ID]->m_pData->m_oldx = m_pList[ID]->m_pData->m_xPos;
	m_pList[ID]->m_pData->m_oldy = m_pList[ID]->m_pData->m_yPos;

	return TRUE;
}
BOOL cFightState::BackFromFight(int i, BOOL bdelete)
{
	if (!m_pList[i]->isLive)return TRUE;
	if (!m_pList[i]->m_bPrePareFight)return TRUE;
	m_pList[i]->m_bPrePareFight = FALSE;

	m_pList[i]->m_work.clear();
	m_pList[i]->m_WorkType = -1;
	m_pList[i]->m_pData->m_MagicSuffer.m_NeedShow = FALSE;
	m_pList[i]->m_DamageTime[0] = 100;
	m_pList[i]->m_pData->SetStatic(FALSE);
	m_pList[i]->m_pData->m_TrueName.m_bShowOnScreen = FALSE;

	cObj* pTarget = m_pList[i]->m_pData;
	pTarget->Reset();
	pTarget->SetShake(FALSE);
	cPetData* pTargetData;
	if (!pTarget->m_IndexType)
	{
		m_pList[i]->isLive = FALSE;
		return TRUE;
	}
	pTarget->GetDateP(pTargetData);
	
	SetFightBufDown(i);	
	if (!pTargetData->m_Controler.GetDel())
	{
		pTargetData->SetXLEffect(FALSE);
		//��ԭ��ɫ��λ��,����,��ʹ֮վ��
		pTarget->SetXY(m_pList[i]->xOldPos, m_pList[i]->yOldPos);
		pTarget->SetDirection(m_pList[i]->m_oldDirection);
		pTarget->m_TrueName.SetColor(RGB(0, 255, 0));
		pTarget->Stand();
		pTarget->MoveShadow();
		pTarget->MoveDialog();
		//ȥ��Ч��
		pTargetData->m_ATK = pTargetData->m_FightProperty.m_oldATK;      //�˺�
		pTargetData->m_HIT = pTargetData->m_FightProperty.m_oldHIT;   //����
		pTargetData->m_DEF = pTargetData->m_FightProperty.m_oldDEF;   //����
		pTargetData->m_Wakan = pTargetData->m_FightProperty.m_oldWakan; //����
		pTargetData->m_SPD = pTargetData->m_FightProperty.m_oldSPD;   //�ٶ�
		pTargetData->m_Evade = pTargetData->m_FightProperty.m_oldEvade; //����
		pTargetData->GoldLivePercent = pTargetData->m_FightProperty.m_oldGoldLive;
		pTargetData->m_FightProperty.Clear();
	}
	else if(bdelete)
	{
		g_pMainState->FreeFightList(i);
	}
	m_pList[i]->isLive = FALSE;
	return TRUE;
}
BOOL cFightState::PrePareToFight(int i)
{
	if (!m_pList[i]->isLive)return TRUE;
	if (m_pList[i]->m_bPrePareFight)return TRUE;
	m_pList[i]->m_bPrePareFight = TRUE;
	cObj* pTarget = m_pList[i]->m_pData;
	pTarget->SetSound(POS_SOUNDSUFFER, FALSE);
	cPetData* pTargetData;
	pTarget->GetDateP(pTargetData);
	if (1 > pTargetData->m_HP)pTargetData->m_HP = 1;
	pTargetData->SetXLEffect(TRUE);
	//��ʼ����ɫ״̬
	pTargetData->m_FightProperty.Init();
	pTarget->m_TrueName.SetColor(RGB(0, 255, 0));
	m_pList[i]->SetDie(FALSE);
	pTarget->m_TrueName.m_bShowOnScreen = TRUE;
	m_pList[i]->m_work.clear();
	m_pList[i]->m_WorkType = -1;
	//
	if (!pTargetData->m_Controler.GetDel())
	{
		//��¼��ɫ�ĵ�λ��,����ͻ�������
		m_pList[i]->xOldPos = pTarget->GetX();
		m_pList[i]->yOldPos = pTarget->GetY();
		m_pList[i]->m_oldDirection = m_pList[i]->m_pData->GetDirection();

		pTargetData->m_FightProperty.m_oldATK = pTargetData->m_ATK;      //�˺�
		pTargetData->m_FightProperty.m_oldHIT = pTargetData->m_HIT;   //����
		pTargetData->m_FightProperty.m_oldDEF = pTargetData->m_DEF;   //����
		pTargetData->m_FightProperty.m_oldWakan = pTargetData->m_Wakan; //����
		pTargetData->m_FightProperty.m_oldSPD = pTargetData->m_SPD;   //�ٶ�
		pTargetData->m_FightProperty.m_oldEvade = pTargetData->m_Evade; //����
		pTargetData->m_FightProperty.m_oldGoldLive = pTargetData->GoldLivePercent;
		pTargetData->m_FightProperty.m_TurnHPAdd = pTargetData->m_LiveHpLv*pTargetData->m_LV / 10;
		pTargetData->m_FightProperty.m_TurnHPHurtAdd = 0;
		pTargetData->m_FightProperty.m_TurnMPAdd = pTargetData->m_LiveMpLv*pTargetData->m_LV / 10;
	}

	//������Ч��
	g_pMainState->m_InterfaceTeam.SetupOneFormationEffect(g_pMainState->m_InterfaceTeam.GetReTramTrun(i), pTargetData, 1);
	//���ý�ɫ�ĳ�ʼ����, ����,λ��
	pTarget->Set(POS_FIGHT_STAND);
	if (i < 10)
		pTarget->SetDirection(DIRECTION_LEFT_UP);
	else
		pTarget->SetDirection(DIRECTION_RIGHT_DOWN);

	pTarget->SetXY(m_xPosList[i], m_yPosList[i]);
	pTarget->MoveShadow();
	pTarget->MoveDialog();
	pTarget->MoveHpLine();
	pTarget->UpdateHpLine();
	pTarget->MoveName();

	if (pTargetData->m_StartMagic.d1)
	{//������״̬
		g_pMainState->m_SkillManager.EffectSkill(pTargetData->m_LV, pTargetData->m_StartMagic.d1, i, i);
	}
	return TRUE;
}


BOOL cFightState::Counter(int ID)
{
	int TargetID = m_pList[ID]->targetID;
	int& time = m_pList[ID]->GetWork()->stage;

	cObj* pUser =m_pList[ID]->m_pData;
	cObj* pTarget = m_pList[TargetID]->m_pData;



	cPetData* pUserData;
	cPetData* pTargetData;
	pUser->GetDateP(pUserData);
	pTarget->GetDateP(pTargetData);
	switch (time)
	{
	case 0:
		if (1)
		{
			int pos;
			if (rand() % 2)
				pos = POS_ATTACK2;
			else
				pos = POS_ATTACK;
			pUser->Set(pos);
			pUser->SetSound(pos);

			time = 120;
		}
	case 120:
		if (1)
		{
			int atkp = WorkAttack(ID);
			switch (atkp)
			{
			case 0:
				return TRUE;
			default:
				if (1)
				{
					if (atkp != 2)
						time = 1;
					DWORD magic2 = 0x1D3FF13C;
					int MagicPos = 1;
					//��ɱ
					if (g_pMainState->m_SkillManager.CountCri(pUserData, pTargetData))
					{
						magic2 = 0xECD0E003;
					}
					m_pList[TargetID]->SetMagiSufferID(magic2, MagicPos, &m_FullSkill.m_SufferMagic[TargetID]);
					SetSuffer(TargetID, pUser->GetDirection(),ID);
				
				}
				break;
			}
		}
		return TRUE;
	case 1:
		if (1)
		{
			time += 1;
			int dam;
			g_pMainState->m_SkillManager.CountphysicalDamage(dam, ID, TargetID);
			
		}
		return  TRUE;
	case 2:
		if (WaitPosEnd(ID))
		{
			pUser->Set(POS_FIGHT_STAND);
			m_pList[TargetID]->SetMagiSufferID(0, 1, &m_FullSkill.m_SufferMagic[TargetID]);
			m_pList[ID]->m_work.pop_back();
		}
		return TRUE;
	}
	return TRUE;
}

BOOL cFightState::HiddenWeaponsMove(int id,int yoffset)
{
	float xTarget = (float)m_pList[m_HiddenWeaponsTargetID[id]]->m_pData->GetX();
	float yTarget = (float)(m_pList[m_HiddenWeaponsTargetID[id]]->m_pData->GetY()+yoffset);
	int x = m_HiddenWeaponsp[id].m_x;
	int y = m_HiddenWeaponsp[id].m_y;
	int xDistance =30;
	int yDistace = 0;
	float Speed = 18.0f;
	

	if (x == xTarget&&y == yTarget)
	{
		m_HiddenWeaponsp[id].m_NeedShow = FALSE;
		return TRUE;
	}
	int xTargetold = (int)xTarget;
	int yTargetold = (int)yTarget;

	//�õ���ȷ��Ŀ��λ��
	int direction1 = GetDirection4(x, y, xTargetold, yTargetold);

	if (xDistance || yDistace)
	{
		switch (direction1)
		{
		case 9:
			xTarget -= xDistance;
			yTarget += yDistace;
			break;
		case 7:
			xTarget += xDistance;
			yTarget += yDistace;
			break;
		case 3:
			xTarget -= xDistance;
			yTarget -= yDistace;
			break;
		case 1:
			xTarget += xDistance;
			yTarget -= yDistace;
			break;
		}
	}
	//ȷ������
	int  direction2;
	if (x != xTarget&&y != yTarget)
		direction2 = GetDirection4(x, y, (int)xTarget, (int)yTarget);
	else   direction2 = m_HiddenWeaponsp[id].m_direction;

	m_HiddenWeaponsp[id].m_direction = direction2;

	int targetx2 = (int)xTarget;
	int targety2 = (int)yTarget;
	int k = (x - targetx2)*(x - targetx2) + (y - targety2)*(y - targety2);
	if (k <= Speed*Speed)  //�����Ŀ��ֻ��һ���
	{
		m_HiddenWeaponsp[id].m_NeedShow = FALSE;
		return  TRUE;
	}
	float radian = atan2(
		yTarget - y,
		xTarget - x
		);
	int yOffset2 =(int)(Speed * sin(radian));
	int xOffset =(int)(Speed * cos(radian));
	m_HiddenWeaponsp[id].m_x = x + xOffset;
	m_HiddenWeaponsp[id].m_y = y + yOffset2;
	m_HiddenWeaponsp[id].m_bMove = TRUE;
	return FALSE;
}

BOOL cFightState::ReMoveSealState(int userID)
{
	return ReMoveState(eBuffType��ӡ, userID);
}

BOOL cFightState::ReMoveBuffState(int userID)
{
	return ReMoveState(eBuffType����, userID);
}

BOOL cFightState::ReMoveDeBuffState(int userID)
{
	return ReMoveState(eBuffType����, userID);
}

BOOL cFightState::ReMovePoisionState(int userID)
{
	return ReMoveState(eBuffType��, userID);
}

BOOL cFightState::ReMoveStateID(int id, int userID)
{
	return m_pList[userID]->m_pData->ReMoveStateID(id);
}


BOOL cFightState::ReMoveState(eBuffType Type, int userID)
{
	return m_pList[userID]->m_pData->ReMoveState(Type);
}

void cFightState::ClearState(int id)
{
	return m_pList[id]->m_pData->ClearState();
}
void cFightState::SuccessFight()
{
//	sFightList* pObjList = &g_pMainState->m_FightList[g_pMainState->m_HeroID];
	EndFight();
	g_pMainState->m_TriggerFightFalse.Clear();
	g_pMainState->m_TriggerFightEscape.Clear();
	g_pMainState->m_TriggerFightTurn.Clear();
	g_pMainState->m_TriggerFightStart.Clear();
	g_pMainState->m_TriggerFightSuccess.ProcessReMove();//���㽱��
	g_pMainState->m_PCHead.UpdatePC();
	if (g_StateType != STATE_FIGHT)
	{
		FriendCure();
	}
}

void cFightState::EndFight(BOOL bFriendCure)
{
	if (g_StateType != STATE_FIGHT)return;
	m_turn = 0;
	isInit = FALSE;
	g_StateType = STATE_MAIN;
	g_pMainState->m_PCHead.MoveTeamHead(0);
	g_pMainState->m_Mp3.Load(g_pMainState->m_MusicName);

	for (int i = 0; i < 20; i++)
	{
		if (g_pMainState->m_FightList[i].isLive)
			BackFromFight(i);
	}
	for (int i = 0; i < 3; i++)
		m_FullSkill.m_FullSkilUP[i].m_NeedShow = FALSE;
	m_FightMenu.m_NowFighterID = -1;
	for (int i = 0; i<10; i++)
	{
		if (m_pList[i]->m_pData->m_IndexType)
		{
			m_pList[i]->isLive = TRUE;
			m_pList[i]->SetDie(FALSE);
		}
	}
	if (g_pCharacter->m_PCData.m_FightPetID>-1)
	{//����ٻ�������
		CheckPetExp();
		
	}
	if (g_pMainState->m_InterfaceTeam.m_eEnemyFormation!=normal)
	g_pMainState->m_InterfaceTeam.SetFormation(normal, TRUE);
	g_pMainState->FindObj();
	//�ػ�
	g_pMainState->m_Canvas.SetRedrawAll();

}

void cFightState::SetCounter(int ID, int TargetID)
{
	m_pList[ID]->SetWork(WORKTYPE_COUNTER);
	m_pList[ID]->targetID = TargetID;
}



void cFightState::SetFightBufDown(int i)
{
	cPetData* pTargetData;
	m_pList[i]->m_pData->GetDateP(pTargetData);
	//ȥ����ɫ��״̬,��ԭ��ɫ���� 
	m_pList[i]->m_pData->m_MagicSuffer.m_NeedShow = FALSE;
	m_pList[i]->m_DamageTime[0] = 100;
	//ȥ����Ч��
	g_pMainState->m_InterfaceTeam.SetupOneFormationEffect(g_pMainState->m_InterfaceTeam.GetReTramTrun(i), pTargetData, 0);
	ClearState(i);
}

void CMainState::SetJiangLiItem(sItem2& item, BOOL m_bNeedTag)
{
	g_pMainState->m_InterfaceItem.GetItem(g_pMainState->m_HeroID, &item);
	ostringstream oss;
	oss << "��õ���";
	oss << item.GetName();
	g_pMainState->m_Channel.AddNewString(oss.str());
	if (m_bNeedTag)g_pMainState->m_Tags.Add(oss.str());
}

void CMainState::SetJiangLi(INT64 exp, INT64 expbb, INT64 money, INT64 chubeimoney, BOOL bNeedTag)
{
	if (exp)//�ӳ�
	{
		INT64 exprate = m_GlobalButton[14];
		exp += exprate*exp /100;
	}
	if (expbb)
	{
		INT64 exprate = m_GlobalButton[14];
		expbb += expbb*exprate / 100;
	}
	if (money)//�ӳ�
	{
		INT64 moneyrate = m_GlobalButton[15];
		money += money*moneyrate / 100;
	}
	cPCData& data = g_pCharacter->m_PCData;
	data.AddMoney((int)money, (int)chubeimoney);
	data.AddExp(exp, expbb);
	m_Tags.GetMoneyAndExp((int)money, exp, (int)chubeimoney, expbb, bNeedTag);
	
	ostringstream oss;
	//���㲢��ʾ����
	if (exp)
	{
		g_pMainState->m_PCHead.UpdatePcExp();
	}
	if (expbb)
	if (data.m_FightPetID > -1)
	{
		g_pMainState->m_PCHead.UpdatePetExp();
	}
}

void CMainState::SetDialogList(string name, DWORD id)
{
	for (int i = 0; i < m_DialogList.size(); i++)
	{
		if (0 == m_DialogList[i].name.compare(name))
		{
			return;
		}
	}
	sDialogList list;
	list.name = name;
	list.idlist = id;
	m_DialogList.push_back(list);
}

void CMainState::SetDialogList(cObj* pObj)
{
	sDialogList list;
	list.name = pObj->m_TrueName.GetString();
	for (int i = 0; i < m_DialogList.size(); i++)
	{
		if (0 == m_DialogList[i].name.compare(list.name))
		{
			return;
		}
	}
	list.idlist = pObj->GetIndex(POS_DIALOG);
	m_DialogList.push_back(list);
}

int CMainState::AddPCPetEnemy(ePetID modeid, int lv, sAddPoint* addpoint, eMengPai mengpaid, string name, int stronglv /*= -1*/,BOOL bOur, int pos /*= -1*/)
{//�ٻ������͵������ɵ���
	sZiZhi m_ZiZhi;
	sPetData* pData= g_pMainState->m_PetDataManager.GetPetData(modeid);
	m_ZiZhi.Set(pData->m_ZiZhi, stronglv, pData->m_LVLimit);
	if (name == "")name = pData->m_Name;
	int id= AddPCEnemy(name, 0, lv, addpoint, &m_ZiZhi, mengpaid, 0, modeid, stronglv,pos,bOur);
	if (id == -1)return -1;
	cObj* pObj = g_pMainState->m_FightList[id].m_pData;
	cPetData* pPetdata;
	pObj->GetDateP(pPetdata);
	pPetdata->AutoGetSkill(modeid);
	return id;
}
int CMainState::AddPCPet(ePetID modeid, int lv, sAddPoint* addpoint, eMengPai mengpaid, string name, int stronglv /*= -1*/, BOOL bOur, int pos /*= -1*/)
{//�ٻ������͵��������Ѿ�
	sZiZhi m_ZiZhi;
	sPetData* pData = g_pMainState->m_PetDataManager.GetPetData(modeid);
	m_ZiZhi.Set(pData->m_ZiZhi, stronglv, pData->m_LVLimit);
	if (name == "")name = pData->m_Name;
	return AddPCEnemy(name, 0, lv, addpoint, &m_ZiZhi, mengpaid, 0, modeid, stronglv,pos, bOur);
}

void CMainState::LockLv(int b /*= 0*/)
{
	m_GlobalButton[2] = b; 
}

void CMainState::SetDialogListPet(string name, ePetID petid, BOOL bJinJie)
{
	DWORD id = m_PetDataManager.GetPetData(petid)->m_dialoghead[bJinJie];
	SetDialogList(name, id);
}

int CMainState::CheckInTeam(string name)
{
	for (int i = 0; i < 5; i++)
	{
		if (!m_FightList[i].isLive)continue;
		if (m_FightList[i].m_pData->m_TrueName.GetString() == name)return i;
	}
	return -1;
}

void CMainState::SetRanse(int i, int d0, int d1, int d2, int d3,  int part/*=0*/)
{
	cPetData* pData;
	m_FightList[i].m_pData->GetDateP(pData);
	pData->m_RanSe.m_ranse[part].Set(d0, d1, d2, d3);
	switch (m_FightList[i].m_pData->m_IndexType)
	{
	case INDEXTYEP_CHARACTER:
			m_ColorManage.Give2(&pData->m_RanSe.m_ranse[part], pData->modeid + 2000);
			return;
	case INDEXTYEP_PET:
			m_ColorManage.Give2(&pData->m_RanSe.m_ranse[part], pData->modeid);
			return;
	default:
		return;
	};
}

void CMainState::SetDialogListNpc(string name, int npcid)
{
	if (npcid > 999)npcid -= 1000;
	sDialogList list;
	list.name = name;
	list.idlist = m_pNpcList[npcid].m_dialog;
	m_DialogList.push_back(list);  
}

void CMainState::SetDialogListPC(string name, int pcid)
{
	if (pcid > 1999)pcid -= 2000;
	sDialogList list;
	list.name = name;
	list.idlist = m_pCharacterDataList[pcid].m_Head[0].m_dialog;
	m_DialogList.push_back(list);
}



void cFightState::FalseFight()
{
	EndFight();
	//
	g_pMainState->m_TriggerFightSuccess.Clear();
	g_pMainState->m_TriggerFightEscape.Clear();
	g_pMainState->m_TriggerFightTurn.Clear();
	g_pMainState->m_TriggerFightStart.Clear();
	if (!g_pMainState->m_TriggerFightFalse.ProcessReMove())
	{
		DieEffect(1);
		g_pMainState->m_pEndState->Set("GAME OVER", "����ȫ����..�볢�Ե㼼��,����,����װ������");
		g_StateType = STATE_GAMEOVER;
	}
	
}

void cFightState::EscapeFight()
{//��ս��������
	
	EndFight();
	//
	g_pMainState->m_TriggerFightSuccess.Clear();
	g_pMainState->m_TriggerFightFalse.Clear();
	g_pMainState->m_TriggerFightTurn.Clear();
	g_pMainState->m_TriggerFightStart.Clear();
	g_pMainState->m_TriggerFightEscape.ProcessReMove();
	
	FriendCure();
	
}

void cFightState::DieEffect(int type)
{
	switch (type)
	{
	case 0://
		if (1)
		{
			g_pMainState->m_pEndState->Set("GAME OVER", "ʤ���˱��ҳ���,��������������");
			g_StateType = STATE_GAMEOVER;
		}
		break;
	case 1:
		if (1)
		{
			int moneysub= g_pCharacter->m_PCData.m_Money[0] / 10;
			INT64 expsub =  g_pCharacter->m_PCData.EXP / 10;
			g_pCharacter->m_PCData.m_Money[0]-=moneysub;
			g_pCharacter->m_PCData.EXP-=expsub;
			ostringstream oss;
			oss << "ս��ʧ����,��ʧ��#R";
			oss << moneysub << "#RǮ #R" << expsub << "#R����";
			g_pMainState->m_Channel.AddNewString(oss.str());
		}
		break;
	}
}

void cFightState::FriendCure()
{
	//�����Զ���Ѫ����
	for (int i = 0; i < 5; i++)
	{
		if (!g_pMainState->m_FightList[i].isLive)continue;
		if (i == g_pMainState->m_HeroID)continue;
		cPetData* pdata;
		g_pMainState->m_FightList[i].m_pData->GetDateP(pdata);
		pdata->FillHPMP();
		if (INDEXTYEP_CHARACTER == g_pMainState->m_FightList[i].m_pData->m_IndexType)
		{
			cCharacter* pc = (cCharacter*)g_pMainState->m_FightList[i].m_pData->m_pIndex;
			for (int j = 0; j < pc->m_PCData.m_NumofBB; j++)
			{
				pc->m_PCData.m_pPetDataList[j]->m_PetDataZZ.FillHPMP();
			}
			pc->m_PCData.m_Item[0].m_Num = 30;
			pc->m_PCData.m_Item[1].m_Num = 30;
			pc->m_PCData.m_Item[2].m_Num = 30;
			pc->m_PCData.m_Item[3].m_Num = 30;
			pc->m_PCData.m_Item[4].m_Num = 30;
			pc->m_PCData.m_Item[5].m_Num = 30;
			pc->m_PCData.m_Item[6].m_Num = 30;
			pc->m_PCData.m_Item[7].m_Num = 30;
			pc->m_PCData.m_Item[8].m_Num = 900;
		}
	}
}

cFightState::cFightState()
{
	m_pList.resize(20);
	for (int i = 0; i < 20; i++)
	{
		m_pList[i] = 0;
	}
	for (int i = 0; i < 5; i++)
	{
		m_pFileToLoad[i]=0;
		m_IDtoLoad[i] = 0;
	}
	for (int i = 0; i < 30; i++)
	{
		m_pShowTurn[i] = 0;
	}
}

void cFightState::CheckPetExp()
{
	if (!g_pMainState->m_FightList[g_pMainState->m_HeroID + 5].isLive)return;
	cPetData* pPetdata;
	g_pMainState->m_FightList[g_pMainState->m_HeroID + 5].m_pData->GetDateP(pPetdata);
	if (pPetdata->EXP >= pPetdata->ExpMax)
	{
		if (pPetdata->m_LV < g_pCharacter->m_PCData.m_LV + 10)
		{
			while (pPetdata->EXP >= pPetdata->ExpMax)
			{
				if (!pPetdata->LeveUp())break;
				if (pPetdata->m_LV > g_pCharacter->m_PCData.m_LV + 9)
					break;
			}
			ostringstream oss;
			oss.str("");
			oss << "���";
			oss << pPetdata->m_Name;
			oss << "������";
			oss << pPetdata->m_LV;
			oss << "��";
			g_pMainState->m_Channel.AddNewString(oss.str(),"",3);
			int lvadd = pPetdata->m_LV - pPetdata->m_FuZhuSkillLvList[0];
			for (int i = 0; i < 3; i++)
				pPetdata->lvUpFuZhuSkill(lvadd, i);
			g_pMainState->m_PCHead.UpdatePetHp();
			g_pMainState->m_PCHead.UpdatePetMp();
			g_pMainState->m_PCHead.UpdatePetExp();

		}
	}
	
}

BOOL cMenuState::CheckL()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	m_Sleeptime = 3;
	switch (m_Select)
	{
	case 0:
		if (1)
		{
			m_Button[0].SetFrame(1);
			Show(&g_pMainState->m_Canvas);
			g_pMainState->m_Canvas.Show();
			if (!g_pMainState->m_bInit3)
			{
				g_pMainState->Init3();
			}
			g_StateType = STATE_MAIN;
			g_pMainState->isInit = TRUE;
			g_pMainState->ResetAll();
			g_pMainState->m_ZhuXian.SetChapter(0);
			g_pMainState->m_ZhuXian.Process(0);
			
		}
		break;
	case 1:
		if (1)
		{
			m_Button[1].SetFrame(1);
			vector<string> strselect;
			g_pMainState->m_InterfaceSystem.GetSaveTitle(strselect);
			g_pMainState->m_Dialog.OnOff(FALSE);
			g_pMainState->m_Dialog.Reset();
			g_pMainState->m_Dialog.m_bRetSelect = TRUE;
			
			m_bMode = 2;
			g_pMainState->m_Dialog.SetString("ѡ���ȡλ��", strselect[0], strselect[1], strselect[2], strselect[3]);

			g_pMainState->m_Dialog.OnOff(TRUE);
			return TRUE;
		}
		break;
	case 2:
		if (1)
		{
			g_StateType = STATE_EXT;
			return TRUE;
		}
		break;
	}
	return TRUE;
}





BOOL cMenuState::Init(void* pData /*= 0*/)
{
	if (m_bInit)return TRUE;
	m_bInit = TRUE;

	m_tButton.resize(3);
	m_Button.resize(3);

	g_pMainState->m_Canvas.m_NeedShowMap = FALSE;
	g_pMainState->Init2();
	g_pMainState->UpDateInput();
	g_pMainState->m_Cursor.m_Data.SetXY(g_xMouse, g_yMouse);
	string path;
	path = g_strMediaPath;
	path += "����\\��ʼ����\\0.BMP";
	m_Back.LoadBMP(path.c_str());

	m_tButton[0].SetString("��ʼ��Ϸ");
	m_tButton[1].SetString("��ȡ����");
	m_tButton[2].SetString("�˳���Ϸ");
	for (int i = 0; i < 3; i++)
	{
		m_Button[i].Set(&g_pMainState->m_Button4);
		m_Button[i].m_bStatic=TRUE;
		m_tButton[i].SetColor(RGB(255,255,255));
	}
	Move(0, 0);
	
	////�����߳�
	if (!m_hThread)
	{
		m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, this, CREATE_SUSPENDED, NULL);
	//	m_hThread = CreateThread(0, 0, ThreadProc, this, 0, 0);
	}
	return TRUE;
}

void cMenuState::Move(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
	m_Back.SetXY(x, y);
	for (int i = 0; i < 3; i++)
	{
		m_Button[i].SetXY(120+200*(i)-m_Button[i].GetWidth()/2, 400);
		m_tButton[i].SetXCenter(m_Button[i].GetCenter());
		m_tButton[i].SetY(m_Button[i].GetY()+3);
		m_tButton[i].UpdateXCenter();
	}
}

BOOL cMenuState::Frame(void* pData /*= 0*/)
{
	if (!m_bInit)Init();
	g_pMainState->UpDateInput();
	if (m_pPre)
	{
		m_pPre->SetFrame(0);
		m_pPre = 0;
	}
	if (m_bMode == 2)
	{
		if (g_pMainState->m_Dialog.m_retSelect > -1)
		{
			if (g_pMainState->m_Dialog.m_retSelect < 4)
			{
				g_pMainState->ResetAll();
				Show(&g_pMainState->m_Canvas);
				g_pMainState->m_Canvas.Show();
				if (!g_pMainState->m_bInit3)
				{
					g_pMainState->Init3();
				}
				g_pMainState->isInit = TRUE;
				g_pMainState->m_InterfaceSystem.Load(g_pMainState->m_Dialog.m_retSelect);
			}
			m_bMode = 0;
			return TRUE;
		}

	}

	if (g_pMainState->m_Dialog.m_NeedShow)
		g_pMainState->m_Dialog.ProcessInput();
	if (g_pMainState->Keyboard.GetKeyState(DIK_1))
	{
		g_pMainState->Keyboard.SetLock(DIK_1);
		m_ID += 1;
		m_ID %= 15;
		ostringstream oss;
		oss << g_strMediaPath<< "����\\��ʼ����\\";
		oss << m_ID;
		oss << ".BMP";
		m_Back.LoadBMP(oss.str());//0x01408ACC);
		g_pMainState->m_Canvas.SetRedrawAll();
	}
	for (int i = 0; i < 3;i++)
	if (isOn(g_xMouse, g_yMouse, m_Button[i].m_PreShowRect))
	{
		m_Select = i;
		m_Button[i].SetFrame(2);
		m_pPre = &m_Button[i];
		m_pPre->m_bMove = TRUE;
		if (CheckL())return TRUE;
	}
	Show(&g_pMainState->m_Canvas);
	g_pMainState->ShowInterface2();
	g_pMainState->m_Canvas.Show();
	return TRUE;
}

void cMenuState::Show(cCanvas* pCanvas)
{
	pCanvas->Add(&m_Back, 1, 1);
	for (int i = 0; i < 3; i++)
	{
		pCanvas->Add(&m_Button[i], 1);
		pCanvas->Add(&m_tButton[i]);
	}
	if (g_pMainState->m_Dialog.m_NeedShow)
		g_pMainState->m_Dialog.Show(pCanvas);
}

unsigned int WINAPI cMenuState::ThreadProc(_In_ LPVOID lpParameter)
{
	cMenuState* pState = (cMenuState*) lpParameter;
	g_pMainState->Init3();
	CloseHandle(pState->m_hThread);
	ExitThread(3);
}

BOOL cEndState::Init(void* pData /*= 0*/)
{
	if (m_bInit)return TRUE;
	m_bInit = TRUE;
	string path = g_strMediaPath;
	path += "����\\��ʼ����\\end.BMP";
	m_Back.LoadBMP(path);
	m_Title.SetColor(RGB(255,0,0));
	m_Title.SetXCenter(320);
	m_Title.SetY(200);
	m_Title.m_hFont = g_pMainState->m_hFont[2];
	m_Context.SetColor(RGB(255,255,255));
	m_Context.SetXCenter(320);
	return TRUE;
}

void cEndState::Set(string title, string context)
{
	Init();
	for (int i = 0; i < 10; i++)
	{
		g_pMainState->FreeFightList(i);
	}
	g_pMainState->m_Canvas.m_NeedShowMap = FALSE;
	g_pMainState->ResetAll();
	
	g_pMainState->m_Canvas.SetRedrawAll();
	m_Title.SetString(title);
	m_Context.SetString(context);
	m_Title.UpdateXCenter();
	
	m_Context.SetY(m_Title.GetY() + m_Title.m_Height + 5);
	m_Context.UpdateXCenter();
}

BOOL cEndState::Frame(void* pData /*= 0*/)
{
	g_pMainState->m_Canvas.Add(&m_Back, 1, 1);
	g_pMainState->m_Canvas.Add(&m_Title);
	g_pMainState->m_Canvas.Add(&m_Context);
	g_pMainState->UpDateInput();
	if(CheckL())return TRUE;
	g_pMainState->m_Cursor.Show(g_xMouse, g_yMouse);
	g_pMainState->m_Canvas.Show();
	return TRUE;
}

BOOL cEndState::CheckL()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	g_pMainState->m_pMenuState->Init();

	for (int i = 0; i < 300; i++)
	{
		g_pMainState->m_Canvas.m_CanvasCube[i].PrePicNum = -1;
		
	}
	
	g_StateType = STATE_MENU;
	return TRUE;
}


