#include "global.h"





cCursor::cCursor()
{
	m_Give = 0xCF1D211E;
	m_Normal= 0x535C1994;
	m_Forbid= 0x1733E33B;
	m_Attack= 0x1FBC5273;
	m_Magic = 0xB48A9B3D;
	m_Team  = 406701913;
	m_Hande = 0x5A055B13;
	m_Trade = 0xB87E0F0C;
	m_Guard = 0xB352AE45;
	m_Grab  = 0xC5750B15;
	m_No	= 0xFC03AFF1;
}

BOOL cCursor::Init()
{
	m_FollowMode = FALSE;
	m_Click.Load(0x0D98AC0A);
	m_Clickp.resize(10);
	for (int i = 0; i < 10; i++)
	{
		m_Clickp[i].Set( &m_Click);
		m_Clickp[i].m_NeedShow = FALSE;
		m_Clickp[i].m_FrameGap = 5;
	}
	Set(m_Normal);
	return TRUE;
}

BOOL cCursor::Show(long x, long y)
{
	cCanvas* pCanvas = &g_pMainState->m_Canvas;
	for (int i = 0; i < 10; i++)
	{
		if (m_Clickp[i].m_NeedShow)
		{
			pCanvas->Add(&m_Clickp[i], 0, TRUE);
			if (m_Click.GetMaxFrame() == m_Clickp[i].GetFrame() + 1)
				m_Clickp[i].m_NeedShow = FALSE; 
		}
	}
	if (g_pMainState->Mouse.GetXDelta() != 0)
	{
		m_Data.SetX(x);	
	}
	if (g_pMainState->Mouse.GetYDelta() != 0)
	{
		m_Data.SetY(y);
	}
	//if (m_Data.m_bMove)pCanvas->SetRedraw(m_Data.m_PreShowRect);
	pCanvas->Add(&m_Data, 1);
	return TRUE;
}
int cCursor::SelectObj(int UserID, int Type,cObj*& pObj)
{
	int i = -1;
	int kk = 0;
	switch (Type)
	{
	case 0:   //���Լ�ʹ��
	case 2:
	case 5:
		return UserID;
	case 1:  //���ѷ�ʹ��
	case 3:
	case 6:
	case 9:
		if (UserID < 10)
		{
			kk = 0;
		}
		else kk = 10;
		break;
	case 4:  //�Եз�ʹ��
		if (UserID < 10)
		{
			kk = 10;
		}
		else kk = 0;
		break;
	default:
		ERRBOX(Type);
		return -1;
	}
	for (i = kk; i < 10 + kk; i++)
	{
		if (g_pMainState->m_FightList[i].isLive)
		if (g_pMainState->m_FightList[i].m_pData->isPointOn(g_xMouse, g_yMouse))
		{
			if (Type == 9)
			{
				if (i == UserID)continue;
			}

			if (pObj)//����һ��ȡ������
			{
				pObj->m_TrueName.SetColor(RGB(0, 255, 0));
				pObj->SetShine(FALSE);
			}
			pObj = g_pMainState->m_FightList[i].m_pData;
			pObj->m_TrueName.SetColor(RGB(255, 0, 0));
			pObj->SetShine(TRUE);
			break;
		}
	}
	if (i == 10 + kk)//���û��ѡ���κζ���,ȡ��֮ǰ�ĸ���
	{
		if (pObj)
		{
			pObj->m_TrueName.SetColor(RGB(0, 255, 0));
			pObj->SetShine(FALSE);
			pObj = 0;
		}
	}
	else
	if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
	{
		g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
		pObj->m_TrueName.SetColor(RGB(0, 255, 0));
		pObj->SetShine(FALSE);
		pObj = 0;
		return i;
	}
	return -1;
}

BOOL cCursor::Process()
{
	
	////X������
	//if (g_pMainState->Keyboard.GetKeyState(DIK_X))
	//{
	//	g_pMainState->Keyboard.SetLock(DIK_X);
	//	Set(m_Trade);
	//}
	//T������
	if (g_pMainState->Keyboard.GetKeyState(DIK_T))
	{
		g_pMainState->Keyboard.SetLock(DIK_T);
		Set(m_Team);
	}
	//A������
	if (g_pMainState->Keyboard.GetKeyState(DIK_A))
	{
		g_pMainState->Keyboard.SetLock(DIK_A);
		g_pMainState->m_GlobalButton[18] = !g_pMainState->m_GlobalButton[18];
	}
	if (g_pMainState->Keyboard.GetKeyState(DIK_G))
	{
		g_pMainState->Keyboard.SetLock(DIK_G);
		Set(m_Give);
		return TRUE;
	}
	if(CheckRB())return TRUE;
	if (!g_pMainState->GetLockMouse())return TRUE;
	


		static int time = 0; //��¼��갴�µ�ʱ��
	
		if (g_xMouse < 0 || g_yMouse < 0)return FALSE;
		int xWorldMouse = g_xMouse + g_pMainState->m_Map.m_pMap->GetXCenter()- 320;
		int yWorldMouse = g_yMouse + g_pMainState->m_Map.m_pMap->GetYCenter() - 240;



		//�Ƿ����NPC��
		cObj*  pPc2 = g_pMainState->GetPointAt(xWorldMouse, yWorldMouse);
		g_pMainState->m_pNowSelect = pPc2;
		if (pPc2)
		{		
			if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
			{
				g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
				pPc2->SetShine(FALSE);
				if (m_Give == m_Data.m_NowID)
				{
					Set(m_Normal);
					g_pMainState->m_InterfaceGive.SetTarget(pPc2);
					return TRUE;
				}
				else
				if (m_Normal == m_Data.m_NowID)
				{
					if (!g_pMainState->m_TriggerObj.Process(g_pMainState->m_pNowSelect->m_TrueName.GetString()))
					{
						if (pPc2->m_TaskTrigger.m_pTask)
						{
							pPc2->m_TaskTrigger.m_pTask->Process(pPc2->m_TaskTrigger.m_Stage);
						}
						else
						if (pPc2->m_bHaveScript)
						{
							pPc2->m_Script.Process(pPc2);
						}
					}
					
				}
			}
			else
			{
				pPc2->SetShine(TRUE);
			}
			return TRUE;
		}
		

		if (!pPc2)
		if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON) || m_FollowMode)
		{
			time = 0;

			if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
			{
				g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
				m_FollowMode = FALSE;

				if (g_pMainState->m_Map.CheckAlong(g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->GetX(), g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->GetY(), xWorldMouse, yWorldMouse))
				{
					g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->m_time = -1;
					g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->SetTarget(xWorldMouse, yWorldMouse);
				}
				else
					g_pMainState->m_FindPt.FindPath(g_pHeroObj, xWorldMouse, yWorldMouse);


			}
			else
			{
				m_FollowTime += 1;
				//�����Ч��
				if (m_FollowTime>6)
				{
					m_FollowTime = 0;
					cObj* pUser = g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData;
					if (abs(pUser->GetX() - xWorldMouse) > 48 || abs(pUser->GetY() - yWorldMouse) > 36)
					{				
						pUser->m_time = -1;
						pUser->SetTarget(xWorldMouse, yWorldMouse);
					
					}
					else  
						return TRUE;
				}
				else return TRUE;
			}
			for (int i = 0; i < 10; i++)
			{
				if (!m_Clickp[i].m_NeedShow)
				{
					m_Clickp[i].m_NeedShow = TRUE;
					m_Clickp[i].m_x = xWorldMouse;
					m_Clickp[i].m_y = yWorldMouse;
					m_Clickp[i].SetFrame(0);
					m_Clickp[i].m_NextFrame = FALSE;
					break;
				}
			}
			return TRUE;
		}
		else if (m_FollowMode == FALSE)
		if (g_pMainState->Mouse.GetPureButtonState(MOUSE_LBUTTON))
		{
			time += 1;
			if (time == 20)
			{
				m_FollowMode = TRUE;
			}
			return TRUE;
		}
	return TRUE;
}

BOOL cCursor::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	if (m_Data.m_NowID != m_Normal)
	{
		g_pMainState->m_Canvas.SetRedrawAll();
		Set(m_Normal);
	}
	return TRUE;
}

BOOL cState::Init()
{
	if (m_bInit)return TRUE;
	m_bInit = TRUE;
	m_Addp.resize(5);
	m_Subp.resize(5);
	m_TPcDate.resize(29);
	m_PetSkill.Init();
	m_TSkillHeadSelectDate[0].SetColor(RGB(255, 255, 0));
	m_TSkillHeadSelectDate[1].SetColor(RGB(0, 255, 0));
	for (int i = 0; i < 24; i++)
		m_TPcDate[i].SetColor(RGB(0, 0, 0));
	for (int i = 24; i < 27; i++)
		m_TPcDate[i].SetColor(RGB(230, 230, 230));

	m_TSkillHeadSelectDate[0].SetColor(RGB(255, 255, 0));
	m_TSkillHeadSelectDate[1].SetColor(RGB(0, 255, 0));

	for (int i = 0; i < 9; i++)
		m_TPetDate[i].SetColor(RGB(0, 0, 0));
	m_frampre = 0;
	m_SkillPage = 0;
	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;
	m_LeveUp.Load(0x9B3AF4E5);
	m_Sub.Load(1577923263);
	m_Add.Load(3296232902);
	m_State.Load(0xA16FD45C);
	m_ButtonLvUp.Set(&g_pMainState->m_Button4);
	m_ButtonSkill.Set(&g_pMainState->m_Button4);
	m_SeeZZ.Set(&g_pMainState->m_Button4);
	m_Sub.SetStatic(TRUE);
	m_Add.SetStatic(TRUE);
	m_ButtonLvUp.m_bStatic = TRUE;
	m_ButtonSkill.m_bStatic = TRUE;
	m_State.SetStatic(TRUE);
	m_SeeZZ.m_bStatic = TRUE;
	for (int i = 0; i < 5; i++)
	{
		m_Addp[i].Set( &m_Add);
		m_Subp[i].Set( &m_Sub);
		m_Addp[i].m_bStatic = TRUE;
		m_Subp[i].m_bStatic = TRUE;
		m_Addp[i].SetFrame(3);
		m_Subp[i].SetFrame(3);
	}

	m_TPcDate[24].SetString( "����");
	m_TPcDate[25].SetString( "����");
	m_TPcDate[26].SetString( "�鿴����");

	for (int i = 0; i < 12; i++)
		m_SkillHead[i].SetStatic(TRUE);
	
	Move(384, 0);
	m_PetSkill.m_NeedShow = FALSE;
	return TRUE;
}

BOOL cState::Show(cCanvas* pCanvas)
{
	if (!m_NeedShow)return TRUE;
	if (!m_pPCData)return FALSE;
	pCanvas->Add(&m_State, 1,TRUE);
	pCanvas->Add(&m_Cancel, 1);

	for (int i = 0; i < 5; i++)
	{
		pCanvas->Add(&m_Addp[i], 1);
		pCanvas->Add(&m_Subp[i], 1);
	}
	pCanvas->Add(&m_ButtonLvUp, 1 );
	pCanvas->Add(&m_ButtonSkill, 1);
	pCanvas->Add(&m_SeeZZ, 1);

	for (int i = 0; i < 29; i++)
	{
		pCanvas->Add(&m_TPcDate[i]);
	}
	//���ʽ��滬��Ч��
	if (m_PetSkill.m_NeedShow)
	m_PetSkill.Show(pCanvas);
	return TRUE;
}

BOOL cState::SetFight(cCharacter* pc)
{
	if (!m_bInit)Init();
	m_pPCData = &pc->m_PCData;
	UpDatePcDate();
	return TRUE;
}

BOOL cState::ProcessInput()
{
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}
	if (m_bMoveMode)//�ƶ�
	{
		m_Select = 6;
		g_pMainState->SetMouseOnMap(FALSE);
		if (!CheckL())m_bMoveMode = FALSE;
		return TRUE;
	}
	ResetButton();
	if (isOn(g_xMouse, g_yMouse, m_State.m_PreShowRect))
	{
		g_pMainState->SetMouseOnMap(FALSE);
		if (CheckOn(&m_Cancel, 0))return TRUE;
		if (CheckOn(&m_ButtonLvUp, 1))return TRUE;
		if (CheckOn(&m_ButtonSkill, 2))return TRUE;
		if (CheckOn(&m_SeeZZ, 3))return TRUE;
		for (int i = 0; i < 5; i++)
		{
			if (3 == m_Addp[i].GetFrame())continue;//��ɫ�İ�ť������
			if (CheckOn(&m_Addp[i], 10 + i))return TRUE;
		}
		for (int i = 0; i < 5; i++)
		{
			if (3 == m_Subp[i].GetFrame())continue;//��ɫ�İ�ť������
			if (CheckOn(&m_Subp[i], 20 + i))return TRUE;
		}
		if (CheckRB())return TRUE;	
		m_Select = 7;
		CheckL();
	}
	//����Ƿ������ʽ�����

		m_PetSkill.ProcessInput();

	return TRUE;
}

BOOL cState::CheckL()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	m_Sleeptime = 3;
	ostringstream oss;
	switch (m_Select)
	{
	case 0://�ر�
		OnOff(FALSE);
		m_Cancel.SetFrame(1);
		return TRUE;
	case 1:  //����
		if (1)
		{
			if (g_StateType != STATE_MAIN)return TRUE;
			int limitlv = g_pMainState->GetLockLv();
			if (limitlv)
			{
				if (limitlv <= g_pCharacter->m_PCData.m_LV)
				{
					g_pMainState->m_Tags.Add("������Ҫ,��ʱ�޷�����");
					g_pMainState->m_Triggerlv.Process();
					return FALSE;
				}
			}
			if (!m_pPCData->LeveUp())
			{
				g_pMainState->m_Triggerlv.Process();
				return TRUE;
			}

			g_pCharacter->m_PCData.AddPopularity(5);//��1����5������
			g_pMainState->m_Triggerlv.Process();
			cObj* pObj = g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData;
			pObj->m_Wav.Free();
			pObj->m_Wav.LoadWav(0x02EF9247);
			UpDatePcDate();
			pObj->SetMagicEffect(&m_LeveUp,-10,2);
			m_ButtonLvUp.SetFrame(1);
			m_ButtonLvUp.m_bMove = TRUE;
			g_pMainState->m_PCHead.UpdatePcHp();
			g_pMainState->m_PCHead.UpdatePcMp();
			g_pMainState->m_PCHead.UpdatePcExp();
			m_bNeedLVUP = TRUE;
		}
		return TRUE;
	case 2: //
		if (1)
		{
			OnOff(FALSE);
			g_pMainState->m_InterfaceJiNeng.OnOff(TRUE);
			
		}
		return TRUE;
	case 3://�鿴����
		if (m_PetSkill.m_NeedShow)m_SeeZZ.SetFrame(0);
		else
		{
			m_SeeZZ.SetFrame(1);
			m_PetSkill.UpdateData(m_pPCData,FALSE);
		}
		m_PetSkill.OnOff(!m_PetSkill.m_NeedShow);
		m_frampre = 0;
		return TRUE;
	case 6:
		if (1)
		{
			g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
			int xDel = g_xMouse - m_Prex;
			int yDel = g_yMouse - m_Prey;
			m_Prex = g_xMouse;
			m_Prey = g_yMouse;
			if (xDel != 0 || yDel != 0)
			{
				Move(m_xPos + xDel, m_yPos + yDel);
			}
			m_Sleeptime = 0;
		}
		return TRUE;
	case 7://׼���ƶ�
		g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
		m_Prex = g_xMouse;
		m_Prey = g_yMouse;
		m_bMoveMode = TRUE;
		return TRUE;
		//�ӵ�
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
		if (1)
		{
			if (0 == m_pPCData->m_AddPoint.idle)return TRUE;
			switch (m_Select)
			{
			case 10:
				m_Addp[0].SetFrame( 1);
				m_pPCData->m_AddPoint.hp += 1;
				if (m_Addp[0].GetFrame() == 1)
				{
					m_Subp[0].SetFrame( 0);
					m_Subp[0].m_bMove = TRUE;
				}
				break;
			case 11:
				m_Addp[1].SetFrame(1);
				m_pPCData->m_AddPoint.mp += 1;
				if (m_Addp[1].GetFrame() == 1)
				{
					m_Subp[1].SetFrame( 0);
					m_Subp[1].m_bMove = TRUE;
				}
				break;
			case 12:
				m_Addp[2].SetFrame( 1);
				m_pPCData->m_AddPoint.atk += 1;			
				if (m_Addp[2].GetFrame() == 1)
				{
					m_Subp[2].SetFrame(0);
					m_Subp[2].m_bMove = TRUE;
				}
				break;
			case 13:
				m_Addp[3].SetFrame( 1);
				m_pPCData->m_AddPoint.def += 1;
				if (m_Addp[3].GetFrame() == 1)
				{
					m_Subp[3].SetFrame( 0);
					m_Subp[3].m_bMove = TRUE;
				}
				break;
			case 14:
				m_Addp[4].SetFrame( 1);
				m_pPCData->m_AddPoint.spd += 1;
				if (m_Addp[4].GetFrame() == 1)
				{
					m_Subp[4].SetFrame( 0);
					m_Subp[4].m_bMove = TRUE;
				}
				break;
			}
			m_pPCData->m_AddPoint.idle  -= 1;
			if (0 == m_pPCData->m_AddPoint.idle )
			{
				for (int i = 0; i < 5; i++)
				{
					m_Addp[i].SetFrame( 3);
					m_Addp[i].m_bMove = TRUE;
				}
			}
			UpdateProPerty();
		}
		return TRUE;
		//����
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
		if (1)
		{
			switch (m_Select)
			{
			case 20:
				if (0 == m_pPCData->m_AddPoint.hp )return TRUE;
				m_pPCData->m_AddPoint.hp -= 1;
				m_Subp[0].SetFrame( 1);
				if (0 == m_pPCData->m_AddPoint.hp)m_Subp[0].SetFrame( 3);
				break;
			case 21:
				if (0 == m_pPCData->m_AddPoint.mp)return TRUE;
				m_pPCData->m_AddPoint.mp -= 1;
				m_Subp[1].SetFrame( 1);
				if (0 == m_pPCData->m_AddPoint.mp)
				{
					m_Subp[1].SetFrame( 3);
					m_Subp[1].m_bMove = TRUE;
				}
				break;
			case 22:
				if (0 == m_pPCData->m_AddPoint.atk)return TRUE;
				m_pPCData->m_AddPoint.atk -= 1;
				m_Subp[2].SetFrame(1);
				if (0 == m_pPCData->m_AddPoint.atk)
				{
					m_Subp[2].SetFrame( 3);
					m_Subp[2].m_bMove = TRUE;
				}
				break;
			case 23:
				if (0 == m_pPCData->m_AddPoint.def)return TRUE;
				m_pPCData->m_AddPoint.def -= 1;
				m_Subp[3].SetFrame( 1);
				if (0 == m_pPCData->m_AddPoint.def)
				{
					m_Subp[3].SetFrame( 3);
					m_Subp[3].m_bMove = TRUE;
				}
				break;
			case 24:
				if (0 == m_pPCData->m_AddPoint.spd)return TRUE;
				m_pPCData->m_AddPoint.spd -= 1;
				m_Subp[4].SetFrame( 1);
				if (0 == m_pPCData->m_AddPoint.spd)
				{
					m_Subp[4].SetFrame( 3);
					m_Subp[4].m_bMove = TRUE;
				}
				break;
			}
			m_pPCData->m_AddPoint.idle += 1;
			if (m_pPCData->m_AddPoint.idle == 1)
			{
				for (int i = 0; i < 5; i++)
				{
					m_Addp[i].SetFrame(0);
					m_Addp[i].m_bMove = TRUE;
				}
			}
			UpdateProPerty();
		}
		return TRUE;
	}
	return TRUE;
}

BOOL cState::Move(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
	m_State.SetX( x);
	m_State.SetY( y);
	m_Cancel.SetX( x+233);	
	m_Cancel.SetY( y+5);
	for (int i = 0; i < 5; i++)  
	{
		m_Addp[i].m_x = m_xPos + 205;
		m_Addp[i].m_y = m_yPos + 255 + i * 23;
		m_Subp[i].m_x = m_xPos + 225;
		m_Subp[i].m_y = m_yPos + 255 + i * 23;
	}
	m_ButtonSkill.SetX( m_xPos+174);
	m_ButtonSkill.SetY( m_yPos+ 423);


	m_ButtonLvUp.SetX(m_xPos + 174);
	m_ButtonLvUp.SetY(m_yPos + 398);

	m_SeeZZ.SetX(m_xPos + 174);
	m_SeeZZ.SetY(m_yPos + 369);
	m_TPcDate[0].SetXY(m_xPos + 140, m_yPos + 33);
	m_TPcDate[1].SetXY(m_xPos + 56, m_yPos + 33);
	m_TPcDate[2].SetXY(m_xPos + 210, m_yPos + 57);
	m_TPcDate[3].SetXY(m_xPos + 210, m_yPos + 80);
	m_TPcDate[4].SetXY(m_xPos + 210, m_yPos + 103);

	m_TPcDate[28].SetXY(m_xPos + 56, m_yPos + 79);  //����
	m_TPcDate[27].SetXY(m_xPos + 56, m_yPos + 103);  //����
	

	m_TPcDate[5].SetXY(m_xPos + 56, m_yPos + 133);//HP
	m_TPcDate[6].SetXY(m_xPos + 56, m_yPos + 156);//MP
	m_TPcDate[7].SetXY(m_xPos + 56, m_yPos + 180);//CP
	m_TPcDate[8].SetXY(m_xPos + 56, m_yPos + 203);//����
	m_TPcDate[9].SetXY(m_xPos + 56, m_yPos + 226); //����
	m_TPcDate[10].SetXY(m_xPos + 56, m_yPos + 256); 
	m_TPcDate[11].SetXY(m_xPos + 151, m_yPos + 256);
	m_TPcDate[12].SetXY(m_xPos + 56, m_yPos + 280);
	m_TPcDate[13].SetXY(m_xPos + 151, m_yPos + 280);
	m_TPcDate[14].SetXY(m_xPos + 56, m_yPos + 302);
	m_TPcDate[15].SetXY(m_xPos + 151, m_yPos + 302);
	m_TPcDate[16].SetXY(m_xPos + 56, m_yPos + 325);
	m_TPcDate[17].SetXY(m_xPos + 151, m_yPos + 325);
	m_TPcDate[18].SetXY(m_xPos + 56, m_yPos + 348);
	m_TPcDate[19].SetXY(m_xPos + 151, m_yPos + 348);
	m_TPcDate[20].SetXY(m_xPos + 56, m_yPos + 372);
	m_TPcDate[21].SetXY(m_xPos + 141, m_yPos + 372); //
	m_TPcDate[22].SetXY(m_xPos + 86, m_yPos + 402);  //��������
	m_TPcDate[23].SetXY(m_xPos + 86, m_yPos + 426); //��þ���
	m_TPcDate[24].SetXY(m_xPos + 194, m_yPos + 402);//����
	m_TPcDate[25].SetXY(m_xPos + 194, m_yPos + 426);//����
	m_TPcDate[26].SetXY(m_xPos + 180, m_yPos + 372);	//�鿴����

	

	m_bMove = TRUE;	
	m_PetSkill.Move(x - m_PetSkill.m_back.GetWidth(), -90);
	return TRUE;
}

BOOL cState::UpDatePcDate()
{
	if (!m_pPCData)return FALSE;
	ostringstream oss;
	//����
	oss << m_pPCData->m_Name;
	m_TPcDate[0].SetString( oss.str());
	oss.str("");
	//�ȼ�
	oss << m_pPCData->m_LV;
	m_TPcDate[1].SetString( oss.str());

	
	//����
	oss.str("");
	oss << m_pPCData->m_Popularity;
	m_TPcDate[2].SetString( oss.str());



	//����
	if (-1 < m_pPCData->m_MengPaiID)
	{
		string mengpainame= g_pMainState->m_SkillManager.m_MengPaiManager.m_MengPai[m_pPCData->m_MengPaiID].m_Name;
		m_TPcDate[27].SetString(mengpainame);
		m_TPcDate[27].m_NeedShow = TRUE;
	}
	else m_TPcDate[27].m_NeedShow = FALSE;
	//����
	if (-1 < m_pPCData->m_BangPaiID)
	{
		m_TPcDate[28].SetString("��");	
	}m_TPcDate[28].SetString("��");
	m_TPcDate[28].m_NeedShow = TRUE;
	//�ﹱ
	oss.str("");
	oss << m_pPCData->m_BangPaiPoint;
	m_TPcDate[3].SetString(oss.str());

	oss.str("");
	//�ﹱ
	oss << m_pPCData->m_MengPaiPoint;
	m_TPcDate[4].SetString( oss.str());
	oss.str("");
	//��Ѫ
	UpDateHp();	
	//ħ��
	UpDateMp();	
	//��ŭ
	UpDateCp();
	//����
	oss << m_pPCData->m_Energy << '/' << m_pPCData->m_EnergyMax;
	m_TPcDate[8].SetString( oss.str());
	oss.str("");
	//����
	oss << m_pPCData->m_Physical << '/' << m_pPCData->m_PhysicalMax;
	m_TPcDate[9].SetString( oss.str());
	oss.str("");
	//����
	oss << m_pPCData->m_HIT.GetData();
	m_TPcDate[10].SetString( oss.str());
	oss.str("");
	//�˺�
	oss << m_pPCData->m_ATK.GetData();
	m_TPcDate[12].SetString( oss.str());
	oss.str("");
	
	//����
	oss << m_pPCData->m_DEF.GetData();
	m_TPcDate[14].SetString( oss.str());
	oss.str("");

	
	//�ٶ�
	oss << m_pPCData->m_SPD.GetData();
	m_TPcDate[16].SetString( oss.str());
	oss.str("");

	
	//���
	oss << m_pPCData->m_Evade.GetData();
	m_TPcDate[18].SetString( oss.str());
	oss.str("");

	
	//����
	oss << m_pPCData->m_Wakan.GetData();
	m_TPcDate[20].SetString( oss.str());
	oss.str("");
	UpdateProPerty();
	


	//��������
	oss << m_pPCData->ExpMax;
	m_TPcDate[22].SetString( oss.str());
	oss.str("");
	//��þ���
	oss << m_pPCData->EXP;
	m_TPcDate[23].SetString( oss.str());

	//��ɫ����
	oss.str("");
	oss << m_pPCData->m_ZiZhi.m_ATKZZ;
	m_TPetDate[0].SetString( oss.str());
	//��������
	oss.str("");
	oss << m_pPCData->m_ZiZhi.m_DEFZZ;
	m_TPetDate[1].SetString( oss.str());
	//��������
	oss.str("");
	oss << m_pPCData->m_ZiZhi.m_HPZZ;
	m_TPetDate[2].SetString( oss.str());
	//��������
	oss.str("");
	oss << m_pPCData->m_ZiZhi.m_MAGZZ;
	m_TPetDate[3].SetString( oss.str());
	//�ٶ�����
	oss.str("");
	oss << m_pPCData->m_ZiZhi.m_SPDZZ;
	m_TPetDate[4].SetString( oss.str());
	//��������
	oss.str("");
	oss << m_pPCData->m_ZiZhi.m_AVOIDZZ;
	m_TPetDate[5].SetString( oss.str());
	//����
	oss.str("");
	oss << "*MAX*";
	m_TPetDate[6].SetString( oss.str());
	float groving = (float)m_pPCData->m_ZiZhi.m_Growing;
	groving /= 100;
	//�ɳ���
	oss.str("");
	oss << groving;
	m_TPetDate[7].SetString( oss.str());
	oss.str("");
	oss << "*";
	m_TPetDate[8].SetString( oss.str());


	
	return TRUE;
}


void cState::UpDateHp()
{
	g_pMainState->m_Canvas.SetRedraw(m_TPcDate[5].m_PreShowRect);
	ostringstream oss;
	oss << m_pPCData->m_HP << '/' << m_pPCData->m_HPHurtMax << '/' << m_pPCData->m_HPMax.GetData();
	m_TPcDate[5].SetString( oss.str());
}
void cState::UpDateMp()
{
	g_pMainState->m_Canvas.SetRedraw(m_TPcDate[6].m_PreShowRect);
	ostringstream oss;
	oss << m_pPCData->m_MP << '/' << m_pPCData->m_MPMax.GetData();
	m_TPcDate[6].SetString( oss.str());
}
void cState::UpDateCp()
{
	g_pMainState->m_Canvas.SetRedraw(m_TPcDate[7].m_PreShowRect);
	ostringstream oss;
	oss << m_pPCData->m_CP << "/150";
	m_TPcDate[7].SetString( oss.str());
}

void cState::OnOff(BOOL NeedShow)
{
	g_pMainState->InterfaceSetShow(this, NeedShow);
	g_pMainState->m_Canvas.SetRedraw(m_State.m_PreShowRect);
	if (m_NeedShow)
	{
		Init();
		m_Sleeptime = 0;
		m_Cancel.SetFrame(0);
		UpDatePcDate();
		UpdateAddPointB();
	}
	else if (m_bNeedLVUP)
	{
		g_pMainState->m_pFightState->CheckPetExp();
		m_bNeedLVUP = FALSE;
		//��������
		for (int i = 0; i < 5; i++)
		{
			if (i == g_pMainState->m_HeroID)continue;
			if (!g_pMainState->m_FightList[i].isLive)continue;
			g_pMainState->m_FightList[i].m_pData->FollowLv();
		}
		if (g_pCharacter->m_PCData.m_LV>130)
		{
			if (!g_pCharacter->m_PCData.m_bDuJie)
			{
				cCharacter* pPc;
				for (int i = 0; i < 5; i++)
				{
					if (!g_pMainState->m_FightList[i].isLive)continue;
					pPc = (cCharacter*)g_pMainState->m_FightList[i].m_pData->m_pIndex;
					pPc->m_PCData.m_bDuJie = TRUE;
				}
				g_pMainState->m_PCHead.UpDateHead();
			}
		}
	}
	if (m_PetSkill.m_NeedShow)
		g_pMainState->m_Canvas.SetRedraw(m_PetSkill.m_back.m_PreShowRect);
}


BOOL cState::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	OnOff(FALSE);
	return TRUE;
}

void cState::SetAddPoint()
{
	m_bAddPoint = !m_bAddPoint;
	
}

void cState::UpdateProPerty()
{
	ostringstream oss;
	if (m_bAddPoint)
	{
		//���ʵ���
		oss << m_pPCData->m_AddPoint.hp;
		m_TPcDate[11].SetString(oss.str());
		oss.str("");
		//ħ������
		oss <<  m_pPCData->m_AddPoint.mp;
		m_TPcDate[13].SetString(oss.str());
		oss.str("");
		//��������
		oss << m_pPCData->m_AddPoint.atk;
		m_TPcDate[15].SetString(oss.str());
		oss.str("");
		//������
		oss <<m_pPCData->m_AddPoint.def;
		m_TPcDate[17].SetString(oss.str());
		oss.str("");
		//�ٵ���
		oss <<m_pPCData->m_AddPoint.spd;
		m_TPcDate[19].SetString(oss.str());
		oss.str("");
		//Ǳ��
		oss << m_pPCData->m_AddPoint.idle;
		m_TPcDate[21].SetString(oss.str());
		oss.str("");
	}
	else
	{
		//���ʵ���
		oss << m_pPCData->m_ProPertyHP;    
		m_TPcDate[11].SetString(oss.str());
		oss.str("");
		//ħ������
		oss << m_pPCData->m_ProPertyMag;   
		m_TPcDate[13].SetString(oss.str());
		oss.str("");
		//��������
		oss << m_pPCData->m_ProPertyAtk;   
		m_TPcDate[15].SetString(oss.str());
		oss.str("");
		//������
		oss << m_pPCData->m_ProPertyDef;   
		m_TPcDate[17].SetString(oss.str());
		oss.str("");
		//�ٵ���
		oss << m_pPCData->m_ProPertySpd;   
		m_TPcDate[19].SetString(oss.str());
		oss.str("");
		//Ǳ��
		m_TPcDate[21].SetString("0");
	}
}

void cState::UpdateAddPointB()
{
	for (int i = 0; i < 5; i++)
	{
		m_Addp[i].SetFrame(3);
	}
	for (int i = 0; i < 5; i++)
	{
		m_Subp[i].SetFrame(3);
	}
	if (m_bAddPoint)
	{
		//���ӵ��Ƿ����
		if (m_pPCData->m_AddPoint.idle)
		for (int i = 0; i < 5; i++)
		{
			m_Addp[i].SetFrame(0);
		}
		if (m_pPCData->m_AddPoint.hp)
		{
			m_Subp[0].SetFrame(0);
		}
		if (m_pPCData->m_AddPoint.mp)
		{
			m_Subp[1].SetFrame(0);
		}
		if (m_pPCData->m_AddPoint.atk)
		{
			m_Subp[2].SetFrame(0);
		}

		if (m_pPCData->m_AddPoint.def)
		{
			m_Subp[3].SetFrame(0);
		}
		if (m_pPCData->m_AddPoint.spd)
		{
			m_Subp[4].SetFrame(0);
		}
	}
}

void cShiCheng::UpdateTime()
{
	ss += 1;
	if (ss%m_fps==0)  //ǰ��һ��
	{
		s += 1;
		if (60 == s) //ǰ��һ��
		{
			s = 0;
			AddMinute(1);	
		}
	}
	srand(ss);
};

void cShiCheng::CountDay()
{
	m_week = (day + 2 * month + 3 * (month + 1) / 5 + year/4 + year / 100 + year / 400)%7+1;
}

void cShiCheng::UpdateShiCheng(int h)
{
	switch (h)
	{
	case 23:
	case 24:
	case 0:
		m_Data.Load(m_23_1);
	case 1:
	case 2:
		m_Data.Load(m_1_3);
		break;
	case 3:
	case 4:
		m_Data.Load(m_3_5);
		break;
	case 5:
	case 6:
		m_Data.Load(m_5_7);
		break;
	case 7:
	case 8:
		m_Data.Load(m_7_9);
		break;
	case 9:
	case 10:
		m_Data.Load(m_9_11);
		break;
	case 11:
	case 12:
		m_Data.Load(m_11_13);
		break;
	case 13:
	case 14:
		m_Data.Load(m_13_15);
		break;
	case 15:
	case 16:
		m_Data.Load(m_15_17);
		break;
	case 17:
	case 18:
		m_Data.Load(m_17_19);
		break;
	case 19:
	case 20:
		m_Data.Load(m_19_21);
		break;
	case 21:
	case 22:
		m_Data.Load(m_21_23);
		break;
	}
	/*if (hour > 7 && hour < 29)
	{
		m_bWhite = TRUE;

	}
	else
	{
		m_bWhite = FALSE;
	}
	g_pMainState->m_InterfaceDoor.SetWhite(m_bWhite);*/
}

void cShiCheng::Save(ofstream& File)
{
	//����ʱ��
	File << m_fps << " ";
	File << year << " ";
	File << month << " ";
	File << day << " ";
	File << minute << " ";
	File << s << " ";
	File << ss << " ";
	File << "\n";
}

void cShiCheng::Load(ifstream& File)
{
	File >>m_fps;
	File >>year;
	File >>month;
	File >>day;
	File >>minute;
	File >>s;
	File >>ss;
	g_pMainState->m_InterfaceDoor.m_TimeShiCheng.Init(
		m_fps,
		year,
		month,
		day,
		hour,
		minute,
		s
		);
}

void cShiCheng::AddMinute(int time)
{
	minute += time;
	if (minute % 5 == 0)
	{
		if (g_StateType==STATE_MAIN)
		if (g_pMainState->m_Dialog.m_NeedShow == FALSE)
		{
			g_pMainState->m_InterfaceSystem.Save(3);
		//	g_pMainState->m_Channel.AddNewString("�Զ��ڵ��ĸ�λ�ô浵");
		}
	}
	if (minute >= 60)
	{
		AddHour();
		minute -= 60;
	}
}
void cShiCheng::AddHour(int num)
{
	hour += num;
	if (hour == 24)
	{
		hour = 0;
		AddDay();
	}
	//g_pMainState->m_TriggerTime.Process();
	//UpdateShiCheng(hour);
}
void cShiCheng::AddDay()
{
	day += 1;
	int days = 31;
	switch (month)
	{
	case 4:
	case 6:
	case 9:
	case 11:
		days = 30;
		break;
	case 2:
		if (0 == year % 4)
		{
			days = 28;
		}
		else
		{
			days = 29;
		}
		break;
	}
	if (day == days)   //ǰ��һ��
	{
		day = 1;
		AddMonth();
	}
	//�ò��չ�ʽ��������
	CountDay();
	
	//���º���
	g_pMainState->m_Friend.AddDay();
}

void cShiCheng::AddMonth()
{
	month += 1;
	if (12 == month)   //ǰ��һ��
	{
		month = 1;
		AddYear();
	}
}

void cShiCheng::AddYear()
{
	year += 1;
}





void cMenu::Init()
{
	m_Text.SetColor( RGB(255, 255, 255));
	m_NeedShow = TRUE;
	m_gap = 30;
	m_Select = 0;
	m_BangPai.Load(3277532867);  //4069888541
	
	m_SkillAccelater.Load(3144573054);//3443219224
	
	m_Trade.Load(0xCAB0B8B4);//0x9874B9B8
	
	m_job.Load(0xA15292B2);//0x8F499AA3
	

	m_Give.Load(2119033822);//0x216FE0F8
	
	m_back.Load( 0x3D1FA249);
	
	m_Attack.Load(1807500026);//886189649    
	
	m_Pet.Load(250440769);//0xD5D51AD0  410697672
	
	m_Item.Load(240383749);//0x78DDA320
	
	m_Info.Load(2088395993);//0xD5DF5927
	m_Info.m_Picture.m_FrameGap = 4;
	m_System.Load(0x5116F7DF);//0x414977BD
	
	m_Team.Load(222208523);//0x3B9F2278
	
	m_BangPai.m_isStatic = TRUE;
	m_SkillAccelater.m_isStatic = TRUE;
	m_Trade.m_isStatic = TRUE;
	m_job.m_isStatic = TRUE;
	m_Give.m_isStatic = TRUE;
	m_back.m_isStatic = TRUE;
	m_Attack.m_isStatic = TRUE;
	m_Attack.m_isStatic = TRUE;
	m_Item.m_isStatic = TRUE;
	m_Info.m_isStatic = TRUE;
	m_System.m_isStatic = TRUE;
	m_Team.m_isStatic = TRUE;
	m_Pet.m_isStatic = TRUE;

	Move(660 - m_back.m_Picture.m_Width,
		480 - m_back.m_Picture.m_Height);
}

BOOL cMenu::Show(cCanvas* pCanvas)
{
	if (!m_NeedShow)return TRUE;
	pCanvas->Add(&m_back, 1); 
	pCanvas->Add(&m_Item, 1); 
	pCanvas->Add(&m_Give, 1); 
	pCanvas->Add(&m_Trade,1); 
	pCanvas->Add(&m_Team, 1); 
	pCanvas->Add(&m_Pet,  1); 
	pCanvas->Add(&m_job,  1); 
	pCanvas->Add(&m_BangPai, 1);
	pCanvas->Add(&m_SkillAccelater, 1);
	pCanvas->Add(&m_Info,   1);
	pCanvas->Add(&m_System, 1);
	pCanvas->Add(&m_Attack, 1);	
	if (m_SelectPre>-1)
		pCanvas->Add(&m_Text);
	return TRUE;
}

void cMenu::Select(int x)
{
	m_Select=(x-315) / 30 + 1;
	if (m_SelectPre != m_Select)
	{
		m_SelectPre = m_Select;
		if (m_NowSelectPic)
		{
			m_NowSelectPic->SetY( 445);
			m_NowSelectPic = 0;
		}
	}
	switch (m_Select)
	{
	case 1:
		m_Text.SetString( "����");
		m_NowSelectPic = &m_Attack;
		break;
	case 2:
		m_Text.SetString( "��Ʒ");
		m_NowSelectPic = &m_Item;
		break;
	case 3:
		m_Text.SetString( "����");
		m_NowSelectPic = &m_Give;
		break;
	case 4:
		m_Text.SetString( "����");
		m_NowSelectPic = &m_Trade;
		break;
	case 5:
		m_Text.SetString( "���");
		m_NowSelectPic = &m_Team;
		break;
	case 6:
		m_Text.SetString( "����");
		m_NowSelectPic = &m_Pet;
		break;
	case 7:
		m_Text.SetString( "����");
		m_NowSelectPic = &m_job;
		break;
	case 8:
		m_Text.SetString( "����");
		m_NowSelectPic = &m_BangPai;
		break;
	case 9:
		m_Text.SetString( "��ݼ�");
		m_NowSelectPic = &m_SkillAccelater;
		break;
	case 10:
		m_Text.SetString( "��Ϣ");
		m_NowSelectPic = &m_Info;
		break;
	case 11:
		m_Text.SetString( "ϵͳ");
		m_NowSelectPic = &m_System;
		break;
	default:
		break;
	}
	if (m_NowSelectPic)
	{
		m_NowSelectPic->SetY( 430);

	//	int x1 = m_xPos + (m_Select - 1)*m_gap;
		if (m_Select > 0 && m_Select < 12)
		{
			m_Text.SetXY((m_Select - 1)*m_gap + m_xPos, 460);
		}
	}
}

BOOL cMenu::ProcessInput()
{
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}
	if (!g_pMainState->GetLockMouse())return TRUE;

	if (m_NowSelectPic)
	{
		m_NowSelectPic->SetY(445);
		m_NowSelectPic = 0;
		m_SelectPre = -1;
	}
	if (g_yMouse < 450)return TRUE;
	if (g_xMouse < 300)return TRUE;

	g_pMainState->SetMouseOnMap(FALSE);
	Select(g_xMouse);
	if (CheckL())return TRUE;

	return TRUE;
}

BOOL cMenu::Move(int x, int y)
{
	m_xPos = x;
	m_yPos = y;


	m_back.SetXY( x,y);
	int y1 = 445;
	m_Attack.SetXY(x,y1);
	m_Item.SetXY(x + 30,y1);

	m_Give.SetX(x + 60);
	m_Give.SetY(y1);
	m_Trade.SetX(x + 90);
	m_Trade.SetY(y1);
	m_Team.SetX(x + 120);
	m_Team.SetY(y1);
	m_Pet.SetX(x + 150);
	m_Pet.SetY(y1);
	m_job.SetX(x + 180);
	m_job.SetY(y1);
	m_BangPai.SetX(x + 210);
	m_BangPai.SetY(y1);
	m_SkillAccelater.SetX(x + 240);
	m_SkillAccelater.SetY(y1);
	m_Info.SetX(x + 270);
	m_Info.SetY(y1);
	m_System.SetX(x + 300);
	m_System.SetY(y1);

	return TRUE;
}

BOOL cMenu::CheckL()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	m_Sleeptime = 3;
		switch (m_Select)
		{
		case 1:
			g_pMainState->m_Cursor.Set(g_pMainState->m_Cursor.m_Attack);
			return TRUE;
		case 2:
			g_pMainState->m_InterfaceItem.OnOff(!g_pMainState->m_InterfaceItem.m_NeedShow);
			return TRUE;
		case 3:
			g_pMainState->m_Cursor.Set(g_pMainState->m_Cursor.m_Give);
			return TRUE;
		case 4:
			g_pMainState->m_Cursor.Set(g_pMainState->m_Cursor.m_Trade);
			return TRUE;
		case 5:
			g_pMainState->m_Cursor.Set(g_pMainState->m_Cursor.m_Team);
			return TRUE;
		case 6:
			//����,��׼��д��  EB279C62 
			return TRUE;
		case 7:
			g_pMainState->m_Job.OnOff(!g_pMainState->m_Job.m_NeedShow);
			return TRUE;
		case 8://����
			
			return TRUE;
		case 9://��ݼ�
			g_pMainState->m_Accelator.OnOff(!g_pMainState->m_Accelator.m_NeedShow);
			return TRUE;
		case 10:
			g_pMainState->m_Friend.OnOff(!g_pMainState->m_Friend.m_NeedShow);
			return TRUE;
		case 11:
			g_pMainState->m_InterfaceSystem.OnOff(!g_pMainState->m_InterfaceSystem.m_NeedShow);
			return TRUE;
		default:
			return FALSE;
		}
}

void cInterfaceSystem::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;
	m_ExtGame.Set(&g_pMainState->m_Button4);
	m_SaveGame.Set(&g_pMainState->m_Button4);
	m_LoadGame.Set(&g_pMainState->m_Button4);
	m_ExtGame.m_bStatic = TRUE;
	m_SaveGame.m_bStatic = TRUE;
	m_LoadGame.m_bStatic = TRUE;
	m_tExtGame.SetColor(RGB(255,255,255));
	m_tExtGame.SetString("�˳���Ϸ");
	m_tTitle.SetColor(RGB(255,255,255));
	m_tTitle.SetString("�Ƿ��˳���Ϸ");
	m_tLoadGame.SetColor(RGB(255, 255, 255));
	m_tLoadGame.SetString("��ȡ��Ϸ");
	m_tSaveGame.SetColor(RGB(255, 255, 255));
	m_tSaveGame.SetString("������Ϸ");


	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;
	m_Back.Load(0x1DF7B97D);//  574791040);
	m_Back.SetStatic(TRUE);

//	m_tTime.SetColor(0);

	Move(320-m_Back.GetWidth()/2,240-m_Back.GetHeight()/2);
}

BOOL cInterfaceSystem::Show(cCanvas* pCanvas)
{
	if (!m_NeedShow)return TRUE;
	pCanvas->Add(&m_Back,1,TRUE);
	pCanvas->Add(&m_ExtGame, 1);
	pCanvas->Add(&m_SaveGame, 1);
	pCanvas->Add(&m_LoadGame, 1);
	pCanvas->Add(&m_tExtGame);
	pCanvas->Add(&m_tTitle);
	pCanvas->Add(&m_tSaveGame);
	pCanvas->Add(&m_tLoadGame);
	pCanvas->Add(&m_Cancel,1);
//	m_tTime.Show(pCanvas);
	return TRUE;
}

BOOL cInterfaceSystem::ProcessInput()
{
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}
	if (m_bMoveMode)//�ƶ�
	{
		g_pMainState->SetMouseOnMap(FALSE);
		if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
		{
			g_pMainState->SetMouseOnMap(FALSE);
			int xDel = g_xMouse - m_Prex;
			int yDel = g_yMouse - m_Prey;
			m_Prex = g_xMouse;
			m_Prey = g_yMouse;
			if (xDel != 0 || yDel != 0)
			{
				Move(m_xPos + xDel, m_yPos + yDel);
			}
		}
		else	m_bMoveMode = FALSE;
		return TRUE;
	}
	ResetButton();
	m_Select = -1;

	switch (m_bMode)
	{
	case 1://����
		if (1)
		{
			if (g_pMainState->m_Dialog.m_retSelect < 4)
			{
				Save(g_pMainState->m_Dialog.m_retSelect);
				g_pMainState->m_Tags.Add("����ɹ�");
			}
			m_bMode = 0;
		}
		return TRUE;
	case 2://��ȡ
		if (1)
		{
			if (g_pMainState->m_Dialog.m_retSelect < 4 && g_pMainState->m_Dialog.m_retSelect > -1)
			{
				Load(g_pMainState->m_Dialog.m_retSelect);
				OnOff(FALSE);
			}
			m_bMode = 0;
		}
		return TRUE;
	}

	if (g_pMainState->Keyboard.GetKeyState(DIK_L))
	{
		g_pMainState->Keyboard.SetLock(DIK_L);
		Load(0);
		return TRUE;
	}

	if (isOn(g_xMouse, g_yMouse, m_Back.m_PreShowRect))
	{
		g_pMainState->SetMouseOnMap(FALSE);

		if(CheckOn(&m_ExtGame, 1))return TRUE;
		if (CheckOn(&m_SaveGame, 2))return TRUE;
		if (CheckOn(&m_LoadGame, 3))return TRUE;
		if (CheckOn(&m_Cancel, 0))return TRUE;
		m_Select = 0;
		if(CheckRB())return TRUE;
		
		m_Select = 100;
		if (CheckL())return TRUE;;
		
	}
	return TRUE;
}

BOOL cInterfaceSystem::Move(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
	m_Back.SetX( x);
	m_Back.SetY( y);
	m_tTitle.SetXCenter(m_Back.GetX()+80);
	m_tTitle.SetY(m_yPos+4);
	m_tTitle.UpdateXCenter();
	/*m_ExtGame.SetXY(m_xPos + 15, m_yPos + 40);
	m_SaveGame.SetXY(m_xPos + 15, m_yPos + 65);
	m_LoadGame.SetXY(m_xPos + 15, m_yPos + 90);*/

	m_ExtGame.SetXY(m_Back.GetCenter()-m_ExtGame.GetWidth()/2, m_yPos + 40);
	m_SaveGame.SetXY(m_ExtGame.GetX(), m_yPos + 65);
	m_LoadGame.SetXY(m_ExtGame.GetX(), m_yPos + 90);


	m_tExtGame.SetXCenter(m_ExtGame.GetX() + m_ExtGame.GetWidth() / 2);
	m_tExtGame.SetY(m_ExtGame.GetY()+3);
	m_tExtGame.UpdateXCenter();
	m_Cancel.SetXY(m_xPos + m_Back.GetWidth() - 20,m_yPos + 3);
	m_tSaveGame.SetXCenter(m_SaveGame.GetCenter());
	m_tSaveGame.SetY(m_SaveGame.GetY() + 3);
	m_tSaveGame.UpdateXCenter();

	m_tLoadGame.SetXCenter(m_LoadGame.GetCenter());
	m_tLoadGame.SetY(m_LoadGame.GetY() + 3);
	m_tLoadGame.UpdateXCenter();

//	m_tTime.SetPos(x + 90, y + 40);
	return TRUE;
}

BOOL cInterfaceSystem::CheckL()
{
	if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
	{
		g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	}
	else return FALSE;
	m_Sleeptime = 3;
	switch (m_Select)
	{
	case 0:
		OnOff(FALSE);
		break;
	case 1:
		g_StateType = STATE_EXT;
		for (int i = 0; i < 300; i++)
		{
			g_pMainState->m_Canvas.m_CanvasCube[i].NeedReDraw = TRUE;
		}
		m_ExtGame.SetFrame(1);
		OnOff(FALSE);
		return TRUE;
	case 2:
	case 3:
		if (1)
		{
			vector<string> strselect;
			GetSaveTitle(strselect);
			g_pMainState->m_Dialog.SetObj(0);
			g_pMainState->m_Dialog.OnOff(FALSE);
			g_pMainState->m_Dialog.Reset();
			g_pMainState->m_Dialog.m_bRetSelect = TRUE;
			if (m_Select == 2)
			{
				m_bMode = 1;
				m_SaveGame.SetFrame(1);
				g_pMainState->m_Dialog.SetString("ѡ��浵λ��", strselect[0], strselect[1], strselect[2]);//, strselect[3]);
			}
			else
			{
				m_bMode = 2;
				m_LoadGame.SetFrame(1);
				g_pMainState->m_Dialog.SetString("ѡ���ȡλ��", strselect[0], strselect[1], strselect[2], strselect[3]);
			}
			g_pMainState->m_Dialog.OnOff(TRUE);
			return TRUE;
		}
		break;
	case 100:
		m_bMoveMode = TRUE;
		g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
		m_Prex = g_xMouse;
		m_Prey = g_yMouse;
		break;
	}
	return TRUE;
}

void cInterfaceSystem::OnOff(BOOL NeedShow)
{
	if (NeedShow == m_NeedShow)return;
	g_pMainState->InterfaceSetShow(this, NeedShow);

	if (m_NeedShow)
	{
		m_Sleeptime = 0;
		Init();
		m_Cancel.SetFrame(0);
		/*cShiCheng& shicheng = g_pMainState->m_InterfaceDoor.m_TimeShiCheng;
		ostringstream oss;
		oss << "��ǰʱ��:\n#R";
		oss << shicheng.year;
		oss << "#R��#R";
		oss << shicheng.month;
		oss << "#R��#R";
		oss << shicheng.day;
		oss << "#R��#R";
		oss << "����";
		switch (shicheng.m_week)
		{
		case 1:oss << "һ";break;
		case 2:oss << "��";break;
		case 3:oss << "��";break;
		case 4:oss << "��";break;
		case 5:oss << "��";break;
		case 6:oss << "��";break;
		case 7:oss << "��";break;
		}
		oss << "#R";
		m_tTime.SetFaceText(oss.str(), g_pMainState->m_hFont[0], 75);
		m_tTime.SetPos(m_xPos + 90, m_yPos + 40);*/
	}
	g_pMainState->m_Canvas.SetRedraw(m_Back.m_PreShowRect);
}

BOOL cInterfaceSystem::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	switch (m_Select)
	{
	case 0:
		OnOff(FALSE);
		break;
	default:
		break;
	}
	return TRUE;
}

void cInterfaceSystem::Save(int id)
{
	if (g_StateType != STATE_MAIN)return;
	if (id < 0)id = 0;
	ofstream File;
	ostringstream oss;
	oss << g_strMediaPath<<"�浵//5.txt";
	File.open(oss.str());
	File << id;
	File.close();
	oss.str("");
	oss << g_strMediaPath<< "�浵//";
	oss << id;
	oss << ".txt";

	File.open(oss.str());
	g_pMainState->m_InterfaceDoor.Save(File);
	File << "\n";
	SaveFightList(File);
	File << "\n";
	File << "���汣������" << "\n";
	SaveTrigger(File);
	SaveFriend(File);
	g_pMainState->m_FangWu.Save(File);
	g_pMainState->m_TaskZhuaGui.Save(File);
	g_pMainState->m_TaskBaoTu.Save(File);
	g_pMainState->m_TaskFB.Save(File);
	g_pMainState->m_TaskHuoDong.Save(File);
	g_pMainState->SaveGlobal(File);
	SavePos(File);
	
	g_pMainState->m_Map.Save(File);
	g_pMainState->m_taskSM.Save(File);
	File.close();
}
void cInterfaceSystem::Load(int id)
{
	g_pMainState->ClearInterfaceSetShow();
	g_pMainState->m_Map.SetLock();
	g_pMainState->m_Map.SetLock(FALSE);
	ifstream File;
	ostringstream oss;
	oss << g_strMediaPath<<"�浵//";
	oss << id;
	oss << ".txt";
	File.open(oss.str());
	g_StateType = STATE_MAIN;
	g_pMainState->m_InterfaceDoor.Load(File);

	LoadFightList(File);
	g_pMainState->m_PCHead.UpDateHead();
	string dustbin;
	File >> dustbin;
	g_pMainState->m_Canvas.m_NeedShowMap = TRUE;
	LoadTrigger(File);
	LoadFriend(File);
	g_pMainState->m_FangWu.Load(File);
	g_pMainState->m_TaskZhuaGui.Load(File);
	g_pMainState->m_TaskBaoTu.Load(File);
	g_pMainState->m_TaskFB.Load(File);
	g_pMainState->m_TaskHuoDong.Load(File);
	g_pMainState->LoadGlobal(File);
	LoadPos(File);
	
	g_pMainState->m_Map.LoadMap(File);
	g_pMainState->m_taskSM.Load(File);

	g_pMainState->m_Channel.m_ContextNum = 0;
	g_pMainState->m_Channel.m_ContextShowNum = 0;
	if (100 == g_pMainState->m_GlobalButton[0])
	{
		g_pMainState->m_pFightState->m_FightMenu.SetTeamControl(TRUE);
	}
	else g_pMainState->m_pFightState->m_FightMenu.SetTeamControl(FALSE);
	if (g_pMainState->m_GlobalButton[16])
		g_pMainState->m_Mp3.Stop();
	else
		g_pMainState->m_Mp3.Play();
	File.close();
	g_pMainState->m_Canvas.SetRedrawAll();

}
void cInterfaceSystem::SavePos( ofstream& File)
{
	//����ص�
	File << "\n\r";
	File << g_pMainState->m_Map.m_MapName<<" ";
	File << g_pHeroObj->GetX() /20<<" ";
	File << (g_pMainState->m_Map.m_pMap->m_Height - g_pHeroObj->GetY()) / 20 << " ";

}
void cInterfaceSystem::LoadPos(ifstream& File)
{
	g_pMainState->m_Map.m_MapName = "";//
	/*for (int i = 0; i < g_pMainState->m_Map.m_numberOfNpc; i++)
	{
		g_pMainState->m_Map.m_pNpc[i]->Free();
	}
	g_pMainState->m_Map.m_numberOfNpc = 0;
	g_pMainState->m_Map.m_MapNumber = -1;*/
	//��ȡ�ص�
	int xpos, ypos;
	string mapname;
	File >> mapname;
	File >> xpos;
	File >> ypos;
	if (g_pMainState->m_FangWu.GetMapName() == mapname)
		g_pMainState->m_FangWu.GoIn();
	else
	{
		g_pMainState->m_Map.LoadMap(xpos, ypos, (char*)mapname.c_str(),FALSE);
	}
}



void cInterfaceSystem::SaveFightList(ofstream& File)
{
	File << g_pMainState->m_HeroID<<" ";
	g_pMainState->m_InterfaceTeam.Save(File);
	File << "\n";
	for (int i = 0; i < 5; i++)
	{
		sFightList& list = g_pMainState->m_FightList[i];
		File << list.isLive<<" ";
		File << list.CheckDie() << " ";
		if (!list.isLive)continue;
		File << list.m_pData->m_IndexType<<" ";
		if (list.isLive)
		{
			switch (list.m_pData->m_IndexType)
			{
			case INDEXTYEP_CHARACTER:
				if (1)
				{
					cCharacter* pc = (cCharacter*)list.m_pData->m_pIndex;
					if (i != g_pMainState->m_HeroID)
					{
						//����ٻ��޽���
						int jinjienum = (g_pCharacter->m_PCData.m_LV + 10) / 60;
						for (UINT i2 = 0; i2 < pc->m_PCData.m_NumofBB; i2++)
						{
							cPetData& Pet = pc->m_PCData.m_pPetDataList[i2]->m_PetDataZZ;
							if (Pet.m_JinJieNum.m_PositionID < jinjienum)
							{
								Pet.SetJinJie(jinjienum - Pet.m_JinJieNum.m_PositionID);
								Pet.m_RanSe.Reset();
								sFriendList* plist = g_pMainState->m_Friend.FindFriend2(pc->m_PCData.m_Name);
								for (UINT i3 = 0; i3 < plist->m_BBList.size(); i3++)
								{
									if (plist->m_BBList[i3].m_name == Pet.m_Name)
									{
										plist->m_Ranse.Reset();
									}
								}
							}
						}
					}
					pc->m_PCData.Save(File);
				}
				break;
			case INDEXTYEP_PET:
				if (1)
				{
					cPet* pc = (cPet*)list.m_pData->m_pIndex;
					pc->m_PetDataZZ.Save( File);
				}
				break;
			default:
				ERRBOX(0);
				break;
			}
		}

	}
}

void cInterfaceSystem::LoadFightList(ifstream& File)
{
	int heroid;
	File >> heroid;
	g_pMainState->m_InterfaceTeam.Load(File);
	int indextype;
	for (int i = 0; i < 5; i++)
	{
		g_pMainState->FreeFightList(i);
	}
	for (int i = 0; i < 5; i++)
	{
		sFightList& list = g_pMainState->m_FightList[i];
		list.m_pData->m_TrueName.SetColor( RGB(0,255,0));
		File >> list.isLive;
		BOOL bdie;
		File >> bdie;
		list.SetDie(bdie);
		if (!list.isLive)continue;
		File >> indextype;
		list.m_pData->SetDataType(indextype);
		if (list.isLive)
		{
			switch (list.m_pData->m_IndexType)
			{
			case INDEXTYEP_CHARACTER:
				if (1)
				{
					cCharacter* pc = (cCharacter*)list.m_pData->m_pIndex;
					pc->m_PCData.Load(File);
					pc->SetData(&g_pMainState->m_pCharacterDataList[pc->m_PCData.modeid]);
					list.m_pData->m_TrueName.SetString(pc->m_PCData.m_Name);
					if (i == heroid)g_pMainState->SetCharacter(heroid);
					if (pc->m_PCData.m_FightPetID > -1)
					{
						int petid= pc->m_PCData.m_FightPetID;
						pc->m_PCData.m_FightPetID = -1;
						g_pMainState->SetFightPet(i, petid);
					}
				}
				break;
			case INDEXTYEP_PET:
				if (1)
				{
					cPet* pc = (cPet*)list.m_pData->m_pIndex;
					pc->m_PetDataZZ.Load(File);
					pc->SetData( g_pMainState->m_PetDataManager.GetPetData(pc->m_PetDataZZ.modeid));
					list.m_pData->m_TrueName.SetString(pc->m_PetDataZZ.m_Name);
				}
				break;
			default:
				ERRBOX(0);
				return;
			}
			list.m_pData->Reset();
			list.m_pData->Set(POS_STAND);
		}

	}
//	g_pMainState->SetCharacter(g_pMainState->m_HeroID);
	/*g_pHeroObj->Reset();
	g_pHeroObj->Set(POS_STAND);*/

	//g_pMainState->m_InterfaceTeam.SetFormation(normal);
	//g_pMainState->m_InterfaceTeam.SetFormation(normal, TRUE);
}


void cInterfaceSystem::SaveTrigger(ofstream& File)
{
	SaveATrigger(&g_pMainState->m_TriggerFightTurn, File);
	SaveATrigger(&g_pMainState->m_TriggerFightStart, File);
	g_pMainState->m_TriggerGive.Save(File);
	SaveATrigger(&g_pMainState->m_TriggerFightSuccess, File);
	g_pMainState->m_TriggerObj.Save(File);
	g_pMainState->m_TriggerObjMap.Save(File);
	g_pMainState->m_Triggerlv.Save(File);
	g_pMainState->m_TriggerGive.Save( File);
	g_pMainState->m_TriggerDialog.Save(File);
	//g_pMainState->m_TriggerTime.Save(File);
	g_pMainState->m_Job.Save(File);
	File << "////////////////�������";
	g_pMainState->m_ZhuXian.Save(File);
	g_pMainState->m_ZhiXian.Save(File);
	g_pMainState->m_ZhiXian2.Save(File);
	g_pMainState->m_ZhiXian3.Save(File);
	
}
void cInterfaceSystem::LoadTrigger(ifstream& File)
{
	LoadATrigger(&g_pMainState->m_TriggerFightTurn, File);
	LoadATrigger(&g_pMainState->m_TriggerFightStart, File);
	g_pMainState->m_TriggerGive.Load( File);
	LoadATrigger(&g_pMainState->m_TriggerFightSuccess, File);
	g_pMainState->m_TriggerObj.Load(File);
	g_pMainState->m_TriggerObjMap.Load(File);
	g_pMainState->m_Triggerlv.Load(File);
	g_pMainState->m_TriggerGive.Load(File);
	g_pMainState->m_TriggerDialog.Load(File);
	//g_pMainState->m_TriggerTime.Load(File);
	g_pMainState->m_Job.Load(File);
	string dustbin;
	File >> dustbin;
	g_pMainState->m_ZhuXian.Load(File);
	g_pMainState->m_ZhiXian.Load(File);
	g_pMainState->m_ZhiXian2.Load(File);
	g_pMainState->m_ZhiXian3.Load(File);
}
void cInterfaceSystem::SaveATrigger(cTrigger* trigger, ofstream& File)
{
	File << "\n";
	File << trigger->m_TaskList.size() << " ";
	for (int i = 0; i < trigger->m_TaskList.size(); i++)
	{
		File << trigger->m_TaskList[i].m_pTask->GetType()<<" ";
		File << trigger->m_TaskList[i].m_pTask->m_HuanShu<<" ";
		File << trigger->m_TaskList[i].m_Stage<<" ";
	}
	File << "\n";
}
void cInterfaceSystem::LoadATrigger(cTrigger* trigger, ifstream& File)
{
	int num;
	File >> num;
	int stage;
	int type;
	int huanshu;
	trigger->Clear();
	for (int i = 0; i < num; i++)
	{
		File >> type;
		File >> huanshu;
		File >> stage;
		cTaskFather* ptask = g_pMainState->FindTaskByID(type);
		ptask->m_HuanShu = huanshu;
		trigger->AddTask(ptask, stage);
	}
	
}

void cInterfaceSystem::SaveFriend(ofstream& File)
{
	g_pMainState->m_Friend.Save(File);
}

void cInterfaceSystem::LoadFriend(ifstream& File)
{
	g_pMainState->m_Friend.Load(File);
}




void cInterfaceSystem::GetSaveTitle(vector<string>& strselect)
{
	strselect.resize(4);
	int nowid=-1;
	for (int i = 0; i < 4; i++)
	{
		ifstream File3;
		ostringstream oss;
		oss <<g_strMediaPath <<"�浵//";
		oss << i;
		oss << ".txt";
		File3.open(oss.str());
		if (!File3)
		{
			strselect[i] = "��";
			continue;
		}
		oss.str("");
		//��ȡ�ص�
	
		int xpos,lv;
		//����ʱ��
		File3 >> xpos;
		File3 >> lv;
		if (nowid < xpos)
		{
			nowid = xpos;
		}
		oss << "��"<<xpos << "�δ浵,�ȼ�:"<<lv;
		strselect[i] = oss.str();
	}
	ifstream File;
	ostringstream oss;
	oss << g_strMediaPath<< "�浵//5.txt";
	File.open(oss.str());
	if (!File)return;
	int newid;
	File >> newid;
	File.close();
	strselect[newid] += "    ����";
}




void cInterfaceJob::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;
	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;
	m_Back.resize(2);
	for (int i = 0; i < 2; i++)
	{
		m_Back[i].Load(0xF151309F);// 788193189);
		m_Back[i].SetFrame(1);
		m_Back[i].m_NeedShow = FALSE;
		m_Back[i].m_isStatic = TRUE;
	}
	m_gap = m_Back[0].m_Picture.m_Height;

	m_Job.Load(700775208);
	m_Job.m_isStatic = TRUE;
	m_Up.Load(149082521);
	m_Up.m_isStatic = TRUE;
	m_Down.Load(153189907);
	m_Down.m_isStatic = TRUE;
	m_Select = 0;
	m_Check = 0;
	m_Context.SetColor(RGB(0, 0, 0));
	Move(150, 130);
	
}
void cInterfaceJob::UpDatePageTitle()
{
	if (m_JobList.size() - m_Page * 8> 0)
	{
		int num = m_JobList.size() - m_Page * 8;
		if (num > 8)num = 8;
		for (int i = 0; i < num; i++)
		{
			m_TitleList[i].SetString(m_JobList[i].m_Title);
		}
	}
}

void cInterfaceJob::SetSelect(int select)
{
	if (m_Check > -1)
		m_TitleList[m_Check].SetColor(0);
	if (0 > select)
	{
		m_Check = -1;
		return;
	}
	
	int trueselect = select + m_Page * 8;
	if (trueselect < m_JobList.size())
	{
		m_Check = select;
		m_TitleList[m_Check].SetColor(RGB(255, 0, 0));
		m_Context.SetFaceText(m_JobList[trueselect].m_Context, g_pMainState->m_hFont[0], 142);	
		m_Context.SetPos(m_xPos + 196, m_yPos + 69);
	}
	else
	{
		SetSelect(m_JobList.size() - 1 - m_Page * 8);
	}
}

BOOL cInterfaceJob::Show(cCanvas* pCanvas)
{
	if (!m_NeedShow)return TRUE;
	pCanvas->Add(&m_Job, 1, TRUE);
		pCanvas->Add(&m_Back[0], 1);
		pCanvas->Add(&m_Back[1], 1);
	pCanvas->Add(&m_Cancel, 1);
	pCanvas->Add(&m_Up, 1);
	pCanvas->Add(&m_Down, 1);
	int num = m_JobList.size() - m_Page * 8;
	if (num > 0)
	{
		if (num > 8)num = 8;
		for (int i = 0; i < num; i++)
		{
			pCanvas->Add(&m_TitleList[i], 1);
		}
		if (-1 < m_Check)
		{
			m_Context.Show(pCanvas);
		}
	}
	return TRUE;
}





BOOL cInterfaceJob::ProcessInput()
{
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}
	m_Select = m_Check;
	ResetButton();

	if (m_bMoveMode)//�ƶ�
	{
		m_Select = 6;
		g_pMainState->SetMouseOnMap(FALSE);
		if (!CheckL())
			m_bMoveMode = FALSE;
		return TRUE;
	}

	if (!isInRect(g_xMouse, g_yMouse, &m_Job))return TRUE;
	g_pMainState->SetMouseOnMap(FALSE);

	CheckOn(&m_Cancel, 100);
	CheckOn(&m_Up, -11);
	CheckOn(&m_Down, -12);
	m_Back[0].m_NeedShow = FALSE;
	m_Back[1].m_NeedShow = FALSE;
	int xstart = m_xPos + 15;
	int ystart = m_yPos + 60;
	if (g_xMouse > xstart && g_yMouse > ystart && g_xMouse < xstart + 150 && g_yMouse < ystart + 185)
	{
		m_Select = (g_yMouse - ystart) / m_gap ;
		if (m_Select < m_JobList.size())
		{
			int ypos = ystart + m_gap*(m_Select);
			m_Back[0].SetY(ypos);
			m_Back[1].SetY(ypos);
			m_Back[0].m_NeedShow =TRUE;
			m_Back[1].m_NeedShow =TRUE;
			if (CheckL())return TRUE;
		}
	}
	m_Select = 0;
	if(CheckRB())return TRUE;

	//׼���ƶ�
	m_Select = 7;
	CheckL();
	return TRUE;
}

BOOL cInterfaceJob::CheckL()
{
	if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
	{
		g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	}
	else return FALSE;
	m_Sleeptime = 3;
	switch (m_Select)
	{
	case -11:
		if (1)
		{
			m_Up.SetFrame(1);
			if (0 == m_Page)
			{
				m_Up.SetFrame(1);
				return TRUE;
			}

			m_Page -= 1;
			UpDatePageTitle();
			m_Check = -1;
		}
		return TRUE;
	case -12:
		if (1)
		{
			m_Down.SetFrame(1);
			if (m_JobList.size() - m_Page * 8 < 9)
			{
				return TRUE;
			}

			m_Page += 1;
			UpDatePageTitle();
			m_Check = -1;
		}
		return TRUE;
	case 6://�ƶ�
		if (1)
		{
			g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
			int xDel = g_xMouse - m_Prex;
			int yDel = g_yMouse - m_Prey;
			m_Prex = g_xMouse;
			m_Prey = g_yMouse;
			if (xDel != 0 || yDel != 0)
			{
				Move(m_xPos + xDel, m_yPos + yDel);
			}
			m_Sleeptime = 0;
		}
		return TRUE;
	case 7://׼���ƶ�
		g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
		m_Prex = g_xMouse;
		m_Prey = g_yMouse;
		m_bMoveMode = TRUE;
		return TRUE;
	case 100:
		if (1)
		{
			m_Cancel.SetFrame(1);
			OnOff(FALSE);
		}
		return TRUE;
	default:
		if (1)
		{
			SetSelect(m_Select);
		}
		return TRUE;
	}

}

BOOL cInterfaceJob::Move(int x, int y)
{
	m_xPos = x;
	m_yPos = y;

	m_Job.SetX(x);
	m_Job.SetY(y);


	m_Cancel.SetX(m_xPos + 350);
	m_Cancel.SetY(m_yPos + 5);

	int x1 = m_xPos + 165;
	m_Up.SetX(x1);
	m_Down.SetX(x1);
	m_Up.SetY(m_yPos + 40);
	m_Down.SetY(m_yPos + 240);

	m_Back[0].SetX(m_xPos + 14);
	m_Back[1].SetX(m_xPos + 68);
	//int ystart = m_yPos + 60;
	//int ypos = ystart + m_gap*(m_Check - 1);
	//m_Back[0].SetY(ypos);
	//m_Back[1].SetY(ypos);
	MoveContext(m_xPos, m_yPos);
	return TRUE;
}

void cInterfaceJob::OnOff(BOOL NeedShow)
{
	g_pMainState->InterfaceSetShow(this, NeedShow);
	g_pMainState->m_Canvas.SetRedraw(m_Job.m_PreShowRect);
	if (NeedShow)
	{
		m_Page = 0;
		UpDatePageTitle();
		SetSelect(m_Check);
	}
}

BOOL cInterfaceJob::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	switch (m_Select)
	{
	case 0:
		OnOff(FALSE);
		break;
	}
	return TRUE;
}
BOOL cInterfaceJob::ReMoveJob(cTaskFather* p)
{
	for (int i = 0; i < m_JobList.size(); i++)
	{
		if (m_JobList[i].m_pTask->GetType()== p->GetType())
		{
			m_JobList.erase(m_JobList.begin() + i);
			if (i == m_Check)
			{
				SetSelect(m_JobList.size()-1);
			}
			break;
		}
	}
	return FALSE;
}
BOOL cInterfaceJob::AddaJob(sJob& job,int huanshu)
{
	if (-1 < huanshu)
	{
		ostringstream oss;
		oss << job.m_Context;
		oss << " ��ǰ����#B";
		oss << huanshu << "#B";
		job.m_Context = oss.str();
	}
	for (int i = 0; i < m_JobList.size(); i++)
	{
		if (m_JobList[i].m_pTask->GetType() == job.m_pTask->GetType())
		{
			if (job.m_Time > 0)
			{
				m_JobList[i].m_Time = job.m_Time;
			}
			if ("" == job.m_Context)job.m_Context = "��";
			m_JobList[i].m_Context = job.m_Context;
			if ("" == job.m_Title)job.m_Title = "��";
			m_JobList[i].m_Title = job.m_Title;
			m_TitleList[i].SetString(job.m_Title);
			return TRUE;
		}
	}


	m_JobList.push_back(job);
	if (m_JobList.size() - m_Page * 8 < 9)
		m_TitleList[m_JobList.size() - m_Page * 8 - 1].SetString(job.m_Title);
	if (m_JobList.size()<9)
		SetSelect(m_JobList.size() - 1);
	else
	{
		m_Page = m_JobList.size()/8;
		UpDatePageTitle();
		SetSelect(m_JobList.size() - 1-m_Page*8);
	}
	return TRUE;
}

void cInterfaceJob::MoveContext(int x, int y)
{
	for (int i = 0; i < 8; i++)
	{
		m_TitleList[i].SetXY(x + 18, y + 65 + i * 24);
	}
	m_Context.SetPos(x + 196, y + 69);
}

BOOL cInterfaceJob::CheckHaveJob(int ID)
{
	for (int i = 0; i < m_JobList.size(); i++)
	{
		if (m_JobList[i].m_pTask->GetType() == ID)
			return TRUE;
	}
	return FALSE;
}

void cInterfaceJob::Save(ofstream& File)
{
	File << "\n";
	CheckErr();
	File << m_JobList.size() << " ";
	if (!m_JobList.size())return;
	
	for (int i = 0; i < m_JobList.size(); i++)
	{
		File << "\n";
		sJob& job =m_JobList[i];
		File << job.m_Title << "\n";
		File << job.m_Context << "\n";
		File << job.m_Time << " ";
		File << job.m_pTask->GetType() << " ";
		File << job.m_pTask->m_HuanShu << " ";
	}
	File << "0";
	File << "\n";
}

void cInterfaceJob::Load(ifstream& File)
{
	m_JobList.clear();
	int num;
	File >> num;
	if (!num)return;
	int type;
//	int huanshu;
	string dustbin;
	m_JobList.resize(num);
	for (int i = 0; i < num; i++)
	{
		sJob& data = m_JobList[i];
		getline(File, data.m_Title);
		getline(File, data.m_Title);
		getline(File, data.m_Context);
		File >> data.m_Time;
		File >> type;
		data.m_pTask = g_pMainState->FindTaskByID(type);
		File >> data.m_pTask->m_HuanShu;
		if (m_JobList.size() - m_Page * 8 < 9)
			m_TitleList[m_JobList.size() - m_Page * 8 - 1].SetString(data.m_Title);
	}
	File >> num;
	if (0 != num)
		ERRBOX;
}


void cInterfaceJob::CheckErr()
{
	for (int i = 0; i < m_JobList.size(); i++)
	{
		if (m_JobList[i].m_Context == "" || m_JobList[i].m_Title == "")
		{
			m_JobList.erase(m_JobList.begin() + i);
			return CheckErr();
		}
	}
}





void cInterfaceJiNeng::Set(DWORD ID)
{
	if (m_Data.m_NowID == ID)return;
	m_Data.Load(ID);
	if (ID == m_ZhuJiNeng)//������
	{
		cCharacter* pC = (cCharacter*)g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->m_pIndex;
		cMengPai* pMengPai= g_pMainState->m_SkillManager.m_MengPaiManager.GetMengPai(pC->m_PCData.m_MengPaiID);
		if (pMengPai)
		for (int i = 0; i < 7; i++)
		{
			m_ZhuJiNengHead[i].Load(pMengPai->m_MainSkill[i].m_BigHead);
		}
		
	}
}

BOOL cInterfaceJiNeng::Show(cCanvas* pCanvas)
{
	if (!m_NeedShow)return TRUE;
	m_bUpdateJiNeng = FALSE;
	//����
	pCanvas->Add(&m_Data,	 1,TRUE);
	pCanvas->Add(&m_Cancel, 1);
	//��ť
	pCanvas->Add(&m_SmB, 1);
	pCanvas->Add(&m_FuZhuB, 1);
	pCanvas->Add(&m_XiuLianB, 1);
	
	for (int i = 0; i < 3; i++)
		pCanvas->Add(&m_BText[i]);

	if (m_Data.m_NowID == m_ZhuJiNeng)
	{
		ostringstream oss;
		
		for (int i = 0; i < 7; i++)
		{
			pCanvas->Add(&m_ZhuJiNengHead[i], 1);	
			pCanvas->Add(&m_ZhuJiNengLv[i]);
		}
		if (m_JiNengHead>0)
		{
			for (int i = 0; i < m_JiNengHeadNum; i++)
			{
				pCanvas->Add(&m_JiNengHead[i], 1);
			}

			for (int i = 0; i < m_NumMiaoShu; i++)
				pCanvas->Add(&m_MiaoShu[i]);

			for (int i = 0; i< 6; i++)
			{
				if (m_SpecialSkill[i].m_NowID)
				pCanvas->Add(&m_SpecialSkill[i], 1);
			}
		}
	}
	else if (m_Data.m_NowID == m_XiuLian)
	{
		if (m_JiNengSelect > -1)
		{
			pCanvas->Add(&m_SelectBackXiuLian, 1);

		}
		for (int i = 0; i < 8; i++)
			pCanvas->Add(&m_TXiuLian[i]);
		
	}
	else if (m_Data.m_NowID == m_FuZhuJiNeng)
	{
		if (m_JiNengHead>0)
		{
			for (int i = 0; i < m_JiNengHeadNum; i++)
			{
				pCanvas->Add(&m_JiNengHead[i], 1);
			}
		}
		if (m_JiNengSelect>-1)
		{
			pCanvas->Add(&m_MiaoShu[0]);
			pCanvas->Add(&m_MiaoShu[1]);
		}
		for (int i = 0; i < m_JiNengHeadNum; i++)
		{
			pCanvas->Add(&m_FuZhuJiNengLv[i]);
		}
	}
	return TRUE;
}

cInterfaceJiNeng::cInterfaceJiNeng()
{
	m_JiNengSelect = -1;
	m_SkillSelect = -1;
}

void cInterfaceJiNeng::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;
	m_SelectBackXiuLian.Load(0x436592D8);
	m_SelectBackXiuLian.SetStatic(TRUE);
	m_SelectBackXiuLian.m_Picture.m_bShadow = 50;
	m_SelectBackXiuLian.SetShowWidth(380);
	m_SelectBackXiuLian.SetxOffset2(10);
	m_TXiuLian.resize(8);
	m_BText.resize(3);
	for (int i = 0; i < 8; i++)
	{
		m_TXiuLian[i].SetColor(0);
		m_TXiuLian[i].m_hFont = g_pMainState->m_hFont[0];
	}
	for (int i = 0; i < 3; i++)
	{
		m_BText[i].m_hFont = g_pMainState->m_hFont[0];
		m_BText[i].SetColor(RGB(255, 255, 255));
	}
	m_BText[0].SetString("ʦ�ż���");
	m_BText[1].SetString("��������");
	m_BText[2].SetString("��������");

	m_XiuLian = 993909921;
	m_ZhuJiNeng = 1044547630;
	m_FuZhuJiNeng = 1560928702;

	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;
	
	m_SmB.Set(&g_pMainState->m_Button4);
	m_FuZhuB.Set(&g_pMainState->m_Button4);
	m_learnSkillB.Set(&g_pMainState->m_Button4);
	m_XiuLianB.Set(&g_pMainState->m_Button4);

	Set(m_ZhuJiNeng);
	
	m_learnSkillB.m_bStatic = TRUE;
	m_FuZhuB.m_bStatic = TRUE;
	m_SmB.m_bStatic = TRUE;
	m_XiuLianB.m_bStatic = TRUE;

	m_MiaoShu.resize(4);
	for (int i = 0; i < 4; i++)
	{
		m_MiaoShu[i].m_hFont = g_pMainState->m_hFont[0];
		m_MiaoShu[i].LockWidth(132);
	}
	
	for (int i = 0; i < 7; i++)
	{
		m_ZhuJiNengLv[i].SetColor(0);
	}
	Move(0, 0);
	MoveXiuLian(m_xPos, m_yPos);
}

BOOL cInterfaceJiNeng::ProcessInput()
{
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}
	if (m_bMoveMode)//�ƶ�
	{
		if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
		{
			g_pMainState->SetMouseOnMap(FALSE);
			int xDel =g_xMouse - m_Prex;
			int yDel =g_yMouse - m_Prey;
			m_Prex = g_xMouse;
			m_Prey = g_yMouse;
			if (xDel != 0 || yDel != 0)
			{
				Move(m_xPos + xDel, m_yPos + yDel);
			}
		}
		else	m_bMoveMode = FALSE;
		return TRUE;
	}
	ResetButton();
	m_Select = -1;
	if (!isOn(g_xMouse, g_yMouse, m_Data.m_PreShowRect))return TRUE;
	
		g_pMainState->SetMouseOnMap(FALSE);
		if (CheckOn(&m_Cancel,0))
			return TRUE;
		if (1!=m_SmB.GetNowFrame())
		if (CheckOn(&m_SmB, 1))return TRUE;
		if (1 != m_FuZhuB.GetNowFrame())
		if (CheckOn(&m_FuZhuB, 2))return TRUE;
		if (3!=m_learnSkillB.GetNowFrame())
		if (CheckOn(&m_learnSkillB, 3))return TRUE;
		if (1 != m_XiuLianB.GetNowFrame())
		if (CheckOn(&m_XiuLianB, 4))return TRUE;
		//�����������״̬
		if (m_Data.m_NowID == m_ZhuJiNeng)
		{
			int i;
			for (i = 0; i < 7; i++)
			{
				if (isOn(g_xMouse, g_yMouse, m_ZhuJiNengHead[i].m_PreShowRect))
				{
					m_dustSkillSelect = i;
					m_Select = 10;
					return CheckL();
				}
			}
			for (i = 0; i < m_JiNengHeadNum; i++)
			{
				if (isOn(g_xMouse, g_yMouse, m_JiNengHead[i].m_PreShowRect))
				{
					m_dustSkillSelect = i;
					m_Select = 11;
					return CheckL();
				}
			}
			for (i = 0; i < 6; i++)//�ؼ�
			{
				if (m_SpecialSkill[i].m_NowID)
				if (isOn(g_xMouse, g_yMouse, m_SpecialSkill[i].m_PreShowRect))
				{
					m_dustSkillSelect = i;
					m_Select = 14;
					return CheckL();
				}
			}

		}
		else
		if (m_Data.m_NowID == m_FuZhuJiNeng)
		{
			for (int i = 0; i < m_JiNengHeadNum; i++)
			{
				if (isOn(g_xMouse, g_yMouse, m_JiNengHead[i].m_PreShowRect))
				{
					m_dustSkillSelect = i;
					m_Select = 12;
					return CheckL();
				}
			}
		}
		else
		if (m_Data.m_NowID == m_XiuLian)
		{
			for (int i = 0; i < 8; i++)
			{
				if (isOn(g_xMouse, g_yMouse, m_TXiuLian[i].m_PreShowRect))
				{
					m_dustSkillSelect = i;
					m_Select = 13;
					return CheckL();
				}
			}
			
		}
		if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
		{
			m_bMoveMode = TRUE;
			m_Prex = g_xMouse;
			m_Prey = g_yMouse;
		}
		else
		if (g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))
		{
			OnOff(FALSE);
		}
	
	return FALSE;
}

BOOL cInterfaceJiNeng::Move(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
	m_Data.SetXY( m_xPos, m_yPos);
	m_SmB.SetXY( m_xPos + 25, m_yPos + 30);
	m_FuZhuB.SetXY( m_xPos + 130, m_yPos + 30);
	m_XiuLianB.SetXY(m_xPos + 235,m_yPos + 30);

	m_learnSkillB.SetXY(m_xPos + 340,m_yPos + 435);
	m_Cancel.SetXY(x + 430,y + 5);
	int newpos;
	newpos = m_SmB.GetY() + 4;
	for (int i = 0; i < 3; i++)
		m_BText[i].SetY(newpos);
	for (int i = 0; i < 3; i++)
	{
		newpos = m_xPos + 63 + i * 105;
		m_BText[i].SetXCenter(newpos);
	}
	for (int i = 0; i < 3; i++)
	{
		m_BText[i].UpdateXCenter();
	}
	m_SelectBackXiuLian.SetX(x+14);
	for (int i = 0; i < 6; i++)
	{
		m_SpecialSkill[i].SetX(x+40+i*66);
		m_SpecialSkill[i].SetY(y+384);
	}

	if (m_Data.m_NowID == m_ZhuJiNeng)
	{
		 MoveMainSkill(m_xPos,m_yPos);
	}
	else
	if (m_Data.m_NowID == m_FuZhuJiNeng)
	{
		MoveFuZhuSkill(m_xPos,m_yPos);
	}
	else
	if (m_Data.m_NowID == m_XiuLian)
	{
		MoveXiuLian(m_xPos,m_yPos);
	}

	return TRUE;
}

BOOL cInterfaceJiNeng::CheckL()
{
	if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
	{
		g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	}
	else return FALSE;
	m_Sleeptime = 3;
		switch (m_Select)
		{
		case 0:
			if (1)
			{
				OnOff(FALSE);
				m_Cancel.SetFrame(1);
			}
			return TRUE;
		case 1: //SM����
			SetMode(m_ZhuJiNeng);
			m_frampre = 0;
			break;
		case 2://��������
			SetMode(m_FuZhuJiNeng);
			m_frampre = 0;
			break;
		case 3: //ѧϰ����
			if (1)
			{

			}
			break;
		case 4: //��������
			SetMode(m_XiuLian);
			MoveXiuLian(m_xPos, m_yPos);
			m_frampre = 0;
			break;
		case 10: //����������
			m_JiNengSelect = m_dustSkillSelect;
			m_SkillSelect = -1;
			m_MiaoShuID = 10;
			UpdateJiNeng();
			
			break;
		case 11:
			m_SkillSelect = m_dustSkillSelect;
			m_MiaoShuID = 11;
			UpdateJiNeng();
		
			break;
		case 12:
			m_JiNengSelect = m_dustSkillSelect;
			
			UpdateJiNeng();
			break;
		case 13:
			m_JiNengSelect = m_dustSkillSelect;
			UpdateJiNeng();
			break;
		case 14:
			m_SpeciallSkillSelect = m_dustSkillSelect;
			m_MiaoShuID = 14;
			UpdateJiNeng();
			
			break;
		default:
			ERRBOX;
			break;
		}
 return TRUE;
}


BOOL cInterfaceJiNeng::UpdateXiuLian()
{
	cCharacter* pc = (cCharacter*)g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->m_pIndex;
	ostringstream oss;
	int lv = pc->m_PCData.m_LV - 20;
	if (lv < 0)lv = 0;
	lv /= 5;//��ǰ�ȼ�����������ȼ�
	if (lv>30)lv = 30;
	sBBXiuLian* xiulian = &pc->m_PCData.m_XiuLian;
	oss.str("");
	oss << "��������  �ȼ�:" << xiulian->GetAtk() << "/" << lv << "   ";
	oss << "��������" << xiulian->GetAtkExp() << "/" << GetXiuLIanExp(xiulian->GetAtk());
	m_TXiuLian[0].SetString( oss.str());
	oss.str("");
	oss << "��������  �ȼ�:" << xiulian->GetDef() << "/" << lv << "   ";
	oss << "��������" << xiulian->GetDefExp() << "/" << GetXiuLIanExp(xiulian->GetDef());
	m_TXiuLian[1].SetString( oss.str());
	oss.str("");
	oss << "��������  �ȼ�:" << xiulian->GetMag() << "/" << lv << "   ";
	oss << "��������" << xiulian->GetMagExp() << "/" << GetXiuLIanExp(xiulian->GetMag());
	m_TXiuLian[2].SetString( oss.str());
	oss.str("");
	oss << "��������  �ȼ�:" << xiulian->GetDMag() << "/" << lv << "   ";
	oss << "��������" << xiulian->GetDMagExp() << "/" << GetXiuLIanExp(xiulian->GetDMag());
	m_TXiuLian[3].SetString( oss.str());
	oss.str("");
	xiulian = &pc->m_PCData.m_BBXiuLian;
	oss << "��������  �ȼ�:" << xiulian->GetAtk() << "/" << lv << "   ";
	oss << "��������" << xiulian->GetAtkExp() << "/" << GetXiuLIanExp(xiulian->GetAtk());
	m_TXiuLian[4].SetString(oss.str());
	oss.str("");
	oss << "��������  �ȼ�:" << xiulian->GetDef() << "/" << lv << "   ";
	oss << "��������" << xiulian->GetDefExp() << "/" << GetXiuLIanExp(xiulian->GetDef());
	m_TXiuLian[5].SetString(oss.str());
	oss.str("");
	oss << "��������  �ȼ�:" << xiulian->GetMag() << "/" << lv << "   ";
	oss << "��������" << xiulian->GetMagExp() << "/" << GetXiuLIanExp(xiulian->GetMag());
	m_TXiuLian[6].SetString(oss.str());
	oss.str("");
	oss << "��������  �ȼ�:" << xiulian->GetDMag() << "/" << lv << "   ";
	oss << "��������" << xiulian->GetDMagExp() << "/" << GetXiuLIanExp(xiulian->GetDMag());
	m_TXiuLian[7].SetString(oss.str());
	oss.str("");

	for (int i = 0; i < 8; i++)
	{
		m_TXiuLian[i].UpdateXCenter();
	}
	if (m_JiNengSelect > -1)
		m_SelectBackXiuLian.SetY(m_TXiuLian[m_JiNengSelect].m_y - 23);
	return TRUE;
}

void cInterfaceJiNeng::OnOff(BOOL NeedShow)
{
	g_pMainState->m_Canvas.SetRedraw(m_Data.m_PreShowRect);
	g_pMainState->InterfaceSetShow(this, NeedShow);
	if (NeedShow)
	{
		Init();
		UpdateJiNeng();
	}
}


BOOL cInterfaceJiNeng::UpdateMainSkill()
{
	cMengPai* pMengPai = 0;
	if (g_pCharacter->m_PCData.m_MengPaiID > -1)
		pMengPai = g_pMainState->m_SkillManager.m_MengPaiManager.GetMengPai(g_pCharacter->m_PCData.m_MengPaiID);
	else
	{
		g_pMainState->m_Tags.Add("�㻹û�м�������");
		return TRUE;
	}
	switch (m_MiaoShuID)
	{
	case 11:
		//ʦ�ż���
		{
			sSkill* pSkill = m_JiNengHeadData[m_SkillSelect];
			m_MiaoShu[0].SetString(pSkill->m_Name,RGB(255,0,0));
			//int h = m_MiaoShu[0].GetHeight();
			m_MiaoShu[1].SetString(pSkill->m_Text,0);
			string name2 = "����:";
			name2 += pSkill->m_tCost;
			m_MiaoShu[2].SetString(name2,0);
			m_NumMiaoShu = 3;
			
		}
		break;
	case 10:
		//���ɼ���
		{
			if (m_JiNengSelect == -1)
			{
				m_NumMiaoShu = 0;
				break;
			}
			
			m_MiaoShu[0].SetString(pMengPai->m_MainSkill[m_JiNengSelect].m_Name, RGB(255, 0, 0));
			m_MiaoShu[1].SetString(pMengPai->m_MainSkill[m_JiNengSelect].m_Text, 0);
			m_MiaoShu[2].SetString(pMengPai->m_MainSkill[m_JiNengSelect].m_Text2, 0);
			m_NumMiaoShu = 3;
		
			m_JiNengHeadNum = 0;
			int num = g_pCharacter->m_PCData.m_SkillEnablelist.size();
			for (int i = 0; i < num; i++)
			{
				if (g_pCharacter->m_PCData.m_SkillEnablelist[i]->m_Owner == m_JiNengSelect)
				{
					m_JiNengHeadData[m_JiNengHeadNum] = g_pCharacter->m_PCData.m_SkillEnablelist[i];
					m_JiNengHead[m_JiNengHeadNum].Load(m_JiNengHeadData[m_JiNengHeadNum]->m_BigHead);
					m_JiNengHeadNum += 1;
				}
			}
		}
		break;
	case 14:
		{
			sSkill* pSkill = g_pMainState->m_SkillManager.GetSpecialSkill(g_pCharacter->m_PCData.m_SpecialSkillList[m_SpeciallSkillSelect]);
			m_MiaoShu[0].SetString(pSkill->m_Name, RGB(255, 0, 0));
			m_MiaoShu[1].SetString(pSkill->m_Text,0);
			m_NumMiaoShu = 2;
		}
		break;
	case -1:
		break;
	default:
		ERRBOX;
		break;
	}
	//���½�ɫ�ȼ�
	ostringstream oss;
	for (int i = 0; i < 7; i++)
	{
		oss.str("");
		oss << g_pCharacter->m_PCData.m_MengPaiSkillLV[i];
		m_ZhuJiNengLv[i].SetString(oss.str());
		m_ZhuJiNengHead[i].Load(pMengPai->m_MainSkill[i].m_BigHead);
	}
	for (int i = 0; i < 6; i++)
	{
		int id = g_pCharacter->m_PCData.m_SpecialSkillList[i];
		if (id>-1)
		{
			sSkill* pSkill = g_pMainState->m_SkillManager.GetSpecialSkill(id);
			m_SpecialSkill[i].Load(pSkill->m_BigHead);
		}
		else 	m_SpecialSkill[i].Free();
	}
	MoveMainSkill(m_xPos,m_yPos);
	return TRUE;
}

void cInterfaceJiNeng::SetMode(DWORD mode)
{
	m_JiNengHeadNum = 0;
	m_JiNengSelect = -1;
	m_SkillSelect = -1;
	m_learnSkillB.SetFrame(3);
	m_learnSkillB.m_bMove = TRUE;
	if (m_pPreMode)
	{
		m_pPreMode->SetFrame(0);
		m_pPreMode->m_bMove = TRUE;
	}
	Set(mode);
	m_Data.m_bMove = TRUE;
	if (mode == m_ZhuJiNeng)
	{
		m_SmB.SetFrame(1);	
		m_pPreMode = &m_SmB;
	}
	else
	if (mode == m_FuZhuJiNeng)
	{
		m_FuZhuB.SetFrame(1);
		m_pPreMode = &m_FuZhuB;
	}
	else
	if (mode == m_XiuLian)
	{
		m_XiuLianB.SetFrame(1);
		m_pPreMode = &m_XiuLianB;		
	}
	m_pPreMode->m_bMove = TRUE;
	UpdateJiNeng();

}

BOOL cInterfaceJiNeng::UpdateJiNeng()
{
	if (m_bUpdateJiNeng)return TRUE;
	if (-1 != m_JiNengSelect)
	{
		m_learnSkillB.SetFrame(0);
		m_learnSkillB.m_bMove = TRUE;
	}
	if (m_Data.m_NowID == m_ZhuJiNeng)
	{
		return UpdateMainSkill();
	}
	else
	if (m_Data.m_NowID == m_FuZhuJiNeng)
	{
		return UpdateFuZhuJiNeng();
	}
	else
	if (m_Data.m_NowID == m_XiuLian)
	{
		return UpdateXiuLian();
	}
	
	return TRUE;
}

BOOL cInterfaceJiNeng::UpdateFuZhuJiNeng()
{
	ostringstream oss;
	cPetData* pUserData;
	 pUserData=&g_pCharacter->m_PCData;
	 int skillnum = 7;// c_NumFuZhu;
	 for (int i = 0; i < skillnum; i++)
	{
		m_JiNengHead[i].Load(g_pMainState->m_SkillManager.m_FuZhuSkillList[i].m_BigHead);
		oss.str("");
		oss << pUserData->m_FuZhuSkillLvList[i];
		m_FuZhuJiNengLv[i].SetString(oss.str());
	}
	if (-1 != m_JiNengSelect)
	{
		sSkill*  Skill = g_pMainState->m_SkillManager.GetFuZhuSkill(m_JiNengSelect);
		m_MiaoShu[0].SetString(Skill->m_Name, RGB(255, 0, 0));
	//	int h = m_MiaoShu[0].GetHeight();
		m_MiaoShu[1].SetString(Skill->m_Text,0);
	}
	m_JiNengHeadNum = skillnum;
	MoveFuZhuSkill(m_xPos, m_yPos);
	return TRUE;
}


void cInterfaceJiNeng::MoveMainSkill(int x, int y)
{
	m_ZhuJiNengHead[0].SetX(m_xPos + 105);
	m_ZhuJiNengHead[0].SetY(m_yPos + 130);

	m_ZhuJiNengHead[1].SetX(m_xPos + 105);
	m_ZhuJiNengHead[1].SetY(m_yPos + 60);

	m_ZhuJiNengHead[2].SetX(m_xPos + 105);
	m_ZhuJiNengHead[2].SetY(m_yPos + 200);

	m_ZhuJiNengHead[3].SetX(m_xPos + 45);
	m_ZhuJiNengHead[3].SetY(m_yPos + 95);

	m_ZhuJiNengHead[4].SetX(m_xPos + 45);
	m_ZhuJiNengHead[4].SetY(m_yPos + 165);

	m_ZhuJiNengHead[5].SetX(m_xPos + 165);
	m_ZhuJiNengHead[5].SetY(m_yPos + 95);

	m_ZhuJiNengHead[6].SetX(m_xPos + 165);
	m_ZhuJiNengHead[6].SetY(m_yPos + 165);

	for (int i = 0; i < 7; i++)
	{
		m_ZhuJiNengLv[i].SetY(m_ZhuJiNengHead[i].GetY() + 47);
		m_ZhuJiNengLv[i].SetXCenter(m_ZhuJiNengHead[i].GetX() + 20);
		m_ZhuJiNengLv[i].UpdateXCenter();
	}
	int h = m_yPos + 65;
	for (int i = 0; i < m_NumMiaoShu; i++)
	{
		m_MiaoShu[i].SetXY(m_xPos + 250, h);
		h += m_MiaoShu[i].GetHeight();
	}
	int i2;
	int i3;
	for (int i = 0; i < m_JiNengHeadNum; i++)
	{
		i2 = i % 7;
		i3 = i / 7;
		m_JiNengHead[i].m_Picture.m_x = m_xPos + 27 + 60 * i2;
		m_JiNengHead[i].m_Picture.m_y = m_yPos + 275 + 50 * i3;
	}

}

void cInterfaceJiNeng::MoveFuZhuSkill(int x, int y)
{
	ostringstream oss;
	//cPetData* pUserData=&g_pCharacter->m_PCData;
	int x1, y1;
	for (int i = 0; i < c_NumFuZhu; i++)
	{
		int i2 = i % 7;
		int i3 = i / 7;
		x1 = m_xPos + 14 + 60 * i2;
		y1 = m_yPos + 276 + 74 * i3;
		m_JiNengHead[i].m_Picture.SetXY(x1, y1);
		m_FuZhuJiNengLv[i].SetXCenter(x1+20);
		m_FuZhuJiNengLv[i].SetY(y1+50);
		m_FuZhuJiNengLv[i].UpdateXCenter();
	}
	if (-1 != m_JiNengSelect)
	{
		m_MiaoShu[0].SetXY(m_xPos + 250, m_yPos + 65);
		int h = m_MiaoShu[0].GetHeight();
		m_MiaoShu[1].SetXY(m_xPos + 250, m_yPos + 66 + h);
	}
}
void cInterfaceJiNeng::MoveXiuLian(int x, int y)
{
	for (int i = 0; i < 8; i++)
	{
		m_TXiuLian[i].SetXCenter(x+225);
		m_TXiuLian[i].UpdateXCenter();
	}
	for (int i = 0; i < 4; i++)
	{
		m_TXiuLian[i].SetY(m_yPos + 115+22*i);
		m_TXiuLian[i+4].SetY(m_yPos + 285+22*i);
	}
	if (m_JiNengSelect > -1)
		m_SelectBackXiuLian.SetY(m_TXiuLian[m_JiNengSelect].m_y - 23);
}
void cInterfacePet::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;
	ostringstream oss;
	oss.str("");
	oss << "��ս";
	m_TPetProPerty[19].SetString(oss.str());
	//oss.str("");
	//oss << "ѱ��";
	//m_TPetProPerty[20].SetString(oss.str());

	m_TPetProPerty[20].m_NeedShow = FALSE;
	m_TPetProPerty[21].m_NeedShow = FALSE;
	oss.str("");
	oss << "����";
	m_TPetProPerty[22].SetString(oss.str());
	oss.str("");
	oss << "�鿴����";
	m_TPetProPerty[23].SetString(oss.str());
	oss.str("");
	oss << "�ӵ�";
	m_TPetProPerty[24].SetString(oss.str());
	m_PetSkill.Init();
	m_ItemDataTime = 0;
	
	//m_Shadow.Set(&g_pMainState->m_Shadow);
	//m_Shadow.m_bStatic = TRUE;
	m_TSkillSelectData[0].SetColor(RGB(255, 255, 0));
	m_TSkillSelectData[1].SetColor(RGB(0, 255, 0));

	m_TPetProPerty[1].m_hFont = g_pMainState->m_hFont[0];

	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;
	m_UpB.Load(0x08E2D199);
	m_UpB.SetStatic(TRUE);
	m_DownB.Load(0xC013B53B);
	m_DownB.SetStatic(TRUE);
	m_SkillPage = 0;
	m_PetPage = 0;  
	
	m_back.Load(1227154258);
	m_PetObj.Init();
	m_PetObj.SetDirection(3);
	m_PetObj.m_IndexType = INDEXTYEP_PET;

	for (int i = 0; i < 4; i++)
	{
		m_TPetName[i].m_hFont = g_pMainState->m_hFont[0];
	}
	for (int i = 0; i < 2; i++)
	{
		m_PetSelectBack[i].Load(0xF151309F);
		m_PetSelectBack[i].SetStatic(TRUE);
		m_PetSelectBack[i].SetFrame(1);
		m_PetSelectBack[i].SetxOffset(30);
	}
	m_SetAttackB.Load(0x79D01E0E);
	m_SetAttackB.SetStatic(TRUE);
	for (int i = 0; i < 5; i++)
	{
		m_AddPoint[i].Load(0xC47885C6);
		m_SubPoint[i].Load(1577923263);
		m_AddPoint[i].SetStatic(TRUE);
		m_SubPoint[i].SetStatic(TRUE);
		m_AddPoint[i].SetFrame(3);
		m_SubPoint[i].SetFrame(3);
	}
	m_XunYang.Load(0x79D01E0E);
	m_XunYang.SetStatic(TRUE);

	m_SetAddPoint.Set(&m_XunYang);
	m_SetAddPoint.m_bStatic = TRUE;

	m_ChaKanZiZhi.Load(0xB15C5678);
	m_ChaKanZiZhi.SetStatic(TRUE);
	m_FreePet.Load(0x79D01E0E);
	m_FreePet.SetStatic(TRUE);
	for (int i = 0; i < 17; i++)
	{
		m_TPetProPerty[i].SetColor(RGB(0, 0, 0));
	}
	for (int i = 17; i < 25;i++)
		m_TPetProPerty[i].SetColor(RGB(250, 250, 250));
	
	Move(0, 0);
	m_PetSkill.m_NeedShow = FALSE;
}

BOOL cInterfacePet::Show(cCanvas* pCanvas)
{
	if (!m_NeedShow)return TRUE;
	pCanvas->Add(&m_back,1,TRUE);
	pCanvas->Add(&m_Cancel, 1);
	if (m_TrueSelect > -1)
	{
		pCanvas->Add(&m_PetSelectBack[0], 1);
		pCanvas->Add(&m_PetSelectBack[1], 1);
	}
	pCanvas->Add(&m_UpB, 1 	);
	pCanvas->Add(&m_DownB,1 );

	
		pCanvas->Add(&m_SetAttackB, 1);
		pCanvas->Add(&m_FreePet, 1);
	//	pCanvas->Add(&m_XunYang, 1);
		pCanvas->Add(&m_ChaKanZiZhi, 1);

		for (int i = 0; i < 5; i++)
		{
			pCanvas->Add(&m_AddPoint[i], 1);
			pCanvas->Add(&m_SubPoint[i], 1);
		}
		if (g_pCharacter->m_PCData.m_NumofBB)
		{
			//pCanvas->Add(&m_Shadow, 1);
			if (m_PetObj.m_pIndex)
			{
				pCanvas->Add(m_PetObj.GetShadow(), 1);
				m_PetObj.ShowOnScreen(pCanvas);
			}
			/*for (int i = 0; i < 4; i++)
			{
			if (m_PetPic[i].m_NowID)
			pCanvas->Add(&m_PetPic[i], 1);
			}*/
			for (int i = 0; i < 19; i++)
			{
				pCanvas->Add(&m_TPetProPerty[i]);
			}
		}
		pCanvas->Add(&m_SetAddPoint,1);
		for (int i = 19; i < 25; i++)
		{
			pCanvas->Add(&m_TPetProPerty[i]);
		}
	if (m_PetSkill.m_NeedShow)
	{
		m_PetSkill.Show(pCanvas);
	}
	int petnum = (g_pCharacter->m_PCData.m_NumofBB - m_PetPage * 4);
	if (petnum>4)petnum = 4;
	for (int i = 0; i < petnum; i++)
	{
		pCanvas->Add(&m_TPetName[i]);
	}
	return TRUE;
}

BOOL cInterfacePet::ProcessInput()
{
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}
	if (m_bMoveMode)//�ƶ�
	{
		m_Select = 6;
		g_pMainState->SetMouseOnMap(FALSE);
		if (!CheckL())
			m_bMoveMode = FALSE;
		return TRUE;
	}
	ResetButton();
	if (m_FreePetMode)
	{
		return ProcessFreePet();
	}
	//�����겻�ڽ�����,ֱ���˳�
	if (isOn(g_xMouse, g_yMouse, m_back.m_PreShowRect))
	{
		g_pMainState->SetMouseOnMap(FALSE);
		if (CheckOn(&m_Cancel, 60))return TRUE;
		if (CheckOn(&m_SetAttackB, 13))return TRUE;
		if(m_bAddPoint)
		for (int i = 0; i < 5; i++)  //
		{
			if (m_AddPoint[i].GetNowFrame()!=3)
			if (CheckOn(&m_AddPoint[i], 20 + i))return TRUE;
			if (m_SubPoint[i].GetNowFrame() != 3)
			if (CheckOn(&m_SubPoint[i], 30 + i))return TRUE;
		}
		//if (CheckOn(&m_XunYang, 14))return TRUE;
		if (CheckOn(&m_ChaKanZiZhi, 15))return TRUE;
		if (CheckOn(&m_FreePet, 17))return TRUE;
		if (CheckOn(&m_SetAddPoint, 61))return TRUE;

		if (m_UpB.GetNowFrame() != 3)
		if (CheckOn(&m_UpB, 50))return TRUE;
		if (m_DownB.GetNowFrame() != 3)
		if (CheckOn(&m_DownB, 51))return TRUE;

		//�ٻ���ѡ��
		if (g_yMouse > m_yPos + 45 && g_yMouse<m_yPos + 160 && g_xMouse>m_xPos + 15 && g_xMouse < m_xPos + 120)
		{
			m_PetSelectBack[0].m_NeedShow =TRUE;
			m_PetSelectBack[1].m_NeedShow =TRUE;
			m_Select = (g_yMouse - (m_yPos + 45)) / 28;
			if (m_Select>3)m_Select = 3;
			if (g_pCharacter->m_PCData.m_NumofBB - m_PetPage * 4 <= m_Select)
			{
				m_Select = g_pCharacter->m_PCData.m_NumofBB - m_PetPage * 4 - 1;
			}
			m_PetSelectBack[0].SetY(m_yPos + 45 + m_Select * 28);
			m_PetSelectBack[1].SetY(m_PetSelectBack[0].GetY());
			return CheckL();
		}
		else
		{
			m_PetSelectBack[0].m_NeedShow = FALSE;
			m_PetSelectBack[1].m_NeedShow = FALSE;
		}
		//�رս���
		if (g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))
		{
			OnOff(FALSE);
		}
		m_Select = 7;
		CheckL();
	}
	if (m_PetSkill.m_NeedShow)
	{
		m_PetSkill.ProcessInput();
		if (!m_PetSkill.m_NeedShow)
		{
			m_ChaKanZiZhi.SetFrame(0);
			m_ChaKanZiZhi.m_bMove = TRUE;
		}
	}
	return TRUE;
}

BOOL cInterfacePet::CheckL()
{
	if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
	{
		g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	}
	else return FALSE;
	m_Sleeptime = 3;
	ostringstream oss;
	switch (m_Select)
	{
	case -1:
		return TRUE;
	case 0:  //�ٻ���ѡ��
	case 1:
	case 2:
	case 3:
		SelectPet(m_Select + m_PetPage * 4);
		break;
	case 6://�ƶ�
		if (1)
		{
			g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
			int xDel = g_xMouse - m_Prex;
			int yDel = g_yMouse - m_Prey;
			m_Prex = g_xMouse;
			m_Prey = g_yMouse;
			if (xDel != 0 || yDel != 0)
			{
				Move(m_xPos + xDel, m_yPos + yDel);
			}
			m_Sleeptime = 0;
		}
		return TRUE;
	case 7://׼���ƶ�
		g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
		m_Prex = g_xMouse;
		m_Prey = g_yMouse;
		m_bMoveMode = TRUE;
		return TRUE;
	case 13:
		//���ò�ս
		if (2 == m_SetAttackB.GetNowFrame())
		{
			m_back.m_bMove = TRUE;
			if (g_pMainState->SetFightPet(g_pMainState->m_HeroID, m_TrueSelect))
			{
				m_SetAttackB.SetFrame(1);
				m_frampre = 0;
			}
			else
				g_pMainState->m_Tags.Add("��ɫ�ĵȼ�����");
		}
		else
		{
			m_SetAttackB.SetFrame(0);
			m_SetAttackB.m_bMove = TRUE;
			g_pMainState->SetFightPet(g_pMainState->m_HeroID, -1);
		}
		return TRUE;
	case 14:
		if (1)
		{	//ѵ��
			m_XunYang.SetFrame(1);
			if (m_TrueSelect < 0)return TRUE;
			cPet* pPetSelect = g_pCharacter->m_PCData.m_pPetDataList[m_TrueSelect];
			cPetData* pTargetData = &pPetSelect->m_PetDataZZ;
			int& lv = pTargetData->m_LV;
			AddExp(pTargetData, (pTargetData->m_LV / 3 + 1)*GetNormalFightExp(lv, lv, 5));
		}
		return TRUE;
	case 15:	//�鿴����
		if (m_ChaKanZiZhi.GetNowFrame() == 1)
		{
			m_PetSkill.m_NeedShow = FALSE;
			m_ChaKanZiZhi.SetFrame(0);
		}
		else
		{
			m_PetSkill.m_NeedShow = TRUE;
			m_ChaKanZiZhi.SetFrame(1);
			m_frampre = 0;
		}
		g_pMainState->m_Canvas.SetRedraw(m_PetSkill.m_back.m_PreShowRect);
		return TRUE;
	case 17:
		{
			m_FreePet.SetFrame(1);
			if (m_TrueSelect < 0)return TRUE;
			cPet* pPetSelect = g_pCharacter->m_PCData.m_pPetDataList[m_TrueSelect];
			//cPetData* pTargetData = &pPetSelect->m_PetDataZZ;
			//����
			m_FreePetMode = TRUE;
			g_pMainState->m_Dialog.Reset();
			g_pMainState->m_Dialog.SetPet(pPetSelect);
			g_pMainState->m_Dialog.SetString("�Ƿ�ȷ������?#24", "��", "��");
			g_pMainState->m_Dialog.OnOff(TRUE);
			g_pMainState->m_Dialog.m_bRetSelect = TRUE;
		}
		return TRUE;
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
		
		{
			if (m_TrueSelect < 0)return TRUE;
			cPet* pPetSelect = g_pCharacter->m_PCData.m_pPetDataList[m_TrueSelect];
			//cPetData* pTargetData = &pPetSelect->m_PetDataZZ;
			cPetData& petdata = pPetSelect->m_PetDataZZ;
			switch (m_Select)
			{
			case 20:  //�ӵ�
				if (petdata.m_AddPoint.idle)
				{
					petdata.m_AddPoint.hp += 1;
					petdata.m_AddPoint.idle -= 1;
					oss << petdata.m_AddPoint.hp;
					m_TPetProPerty[8].SetString(oss.str());
				}
				break;
			case 21:
				if (petdata.m_AddPoint.idle)
				{
					petdata.m_AddPoint.mp += 1;
					petdata.m_AddPoint.idle -= 1;
					oss << petdata.m_AddPoint.mp;
					m_TPetProPerty[9].SetString(oss.str());
				}
				break;
			case 22:
				if (petdata.m_AddPoint.idle)
				{
					petdata.m_AddPoint.atk += 1;
					petdata.m_AddPoint.idle -= 1;
					oss << petdata.m_AddPoint.atk;
					m_TPetProPerty[10].SetString(oss.str());
				}
				break;
			case 23:
				if (petdata.m_AddPoint.idle)
				{
					petdata.m_AddPoint.def += 1;
					petdata.m_AddPoint.idle -= 1;
					oss << petdata.m_AddPoint.def;
					m_TPetProPerty[11].SetString(oss.str());
				}
				break;
			case 24:
				if (petdata.m_AddPoint.idle)
				{
					petdata.m_AddPoint.spd += 1;
					petdata.m_AddPoint.idle -= 1;
					oss << petdata.m_AddPoint.spd;
					m_TPetProPerty[12].SetString(oss.str());
				}
				break;
			}
			UpdatePointButton();
			if (m_frampre->GetNowFrame() != 3)
			m_frampre->SetFrame(1);
			oss.str("");
			oss << petdata.m_AddPoint.idle;
			m_TPetProPerty[13].SetString(oss.str());
		}
		return TRUE;
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
		if (1)
		{
			if (m_TrueSelect < 0)return TRUE;
			cPet* pPetSelect = g_pCharacter->m_PCData.m_pPetDataList[m_TrueSelect];
		//	cPetData* pTargetData = &pPetSelect->m_PetDataZZ;
			cPetData& petdata = pPetSelect->m_PetDataZZ;
			switch (m_Select)
			{
			case 30:  //����
				if (petdata.m_AddPoint.hp)
				{
					petdata.m_AddPoint.hp -= 1;
					petdata.m_AddPoint.idle += 1;
					oss.str("");
					oss << petdata.m_AddPoint.hp;
					m_TPetProPerty[8].SetString(oss.str());
				}
				break;
			case 31:
				if (petdata.m_AddPoint.mp)
				{
					petdata.m_AddPoint.mp -= 1;
					petdata.m_AddPoint.idle += 1;
					oss.str("");
					oss << petdata.m_AddPoint.mp;
					m_TPetProPerty[9].SetString(oss.str());
				}
				break;
			case 32:
				if (petdata.m_AddPoint.atk)
				{
					petdata.m_AddPoint.atk -= 1;
					petdata.m_AddPoint.idle += 1;
					oss.str("");
					oss << petdata.m_AddPoint.atk;
					m_TPetProPerty[10].SetString(oss.str());
				}
				break;
			case 33:
				if (petdata.m_AddPoint.def)
				{
					petdata.m_AddPoint.def -= 1;
					petdata.m_AddPoint.idle += 1;
					oss.str("");
					oss << petdata.m_AddPoint.def;
					m_TPetProPerty[11].SetString(oss.str());
				}
				break;
			case 34:
				if (petdata.m_AddPoint.spd)
				{
					petdata.m_AddPoint.spd -= 1;
					petdata.m_AddPoint.idle += 1;
					oss.str("");
					oss << petdata.m_AddPoint.spd;
					m_TPetProPerty[12].SetString(oss.str());
				}
				break;
			}

			UpdatePointButton();
			if (m_frampre->GetNowFrame()!=3)
				m_frampre->SetFrame(1);
			oss.str("");
			oss << petdata.m_AddPoint.idle;
			m_TPetProPerty[13].SetString(oss.str());
		}
		return TRUE;
	case 50://�Ϸ�
		if (m_PetPage > 0)
		{
			SetPage(m_PetPage - 1);
		}
		return TRUE;
	case 51://�·�
		if (g_pCharacter->m_PCData.m_NumofBB > 4)
		{
			SetPage(m_PetPage + 1);
		}
		return TRUE;
	case 60: //ȡ��
		OnOff(FALSE);
		return TRUE;
	case 61://���üӵ�
		if (1)
		{
			m_framprep->SetFrame(1);
			if (m_TrueSelect > -1)
				m_bAddPoint = !m_bAddPoint;
			if (m_bAddPoint)
			{
				UpdatePointButton();
			}
			UpdateProPerty();
		}
		return TRUE;
	default:
		ERRBOX;
		break;
		}
	return TRUE;
}

BOOL cInterfacePet::Move(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
	m_UpB.m_Picture.m_x = x + 122;
	m_UpB.m_Picture.m_y = y + 25;
	m_DownB.m_Picture.m_x = x + 122;
	m_DownB.m_Picture.m_y = y + 140;
	m_SetAttackB.m_Picture.m_x = m_xPos + 223;
	m_SetAttackB.m_Picture.m_y = m_yPos + 144;
	for (int i = 0; i < 5; i++)
	{
		m_AddPoint[i].m_Picture.m_x = m_xPos + 230;
		m_AddPoint[i].m_Picture.m_y = m_yPos + 215 + 25 * i;
		m_SubPoint[i].m_Picture.m_x = m_xPos + 250;
		m_SubPoint[i].m_Picture.m_y = m_yPos + 215 + 25 * i;
	}
	m_XunYang.m_Picture.m_x = m_xPos + 175;
	m_XunYang.m_Picture.m_y = m_yPos + 358;

	m_SetAddPoint.m_x = m_xPos + 228;
	m_SetAddPoint.m_y = m_yPos + 335;

	m_FreePet.m_Picture.m_x = m_xPos + 16;
	m_FreePet.m_Picture.m_y = m_yPos + 430;

	m_ChaKanZiZhi.m_Picture.m_x = m_xPos + 180;
	m_ChaKanZiZhi.m_Picture.m_y = m_yPos + 425;

	m_back.SetXY(x,y);
	//if (m_PetSkill.m_NeedShow)
	{
		m_PetSkill.Move(x + m_back.GetWidth(),y);
	
	}
	m_Cancel.SetXY(m_back.GetX() + 260, m_back.GetY() + 5);
	m_PetObj.SetXY(m_xPos + 210, m_yPos + 120);
	m_PetObj.MoveShadow();
	/*for (int i = 0; i < 4; i++)
	{
		m_PetPic[i].m_Picture.m_x = m_xPos + 210;
		m_PetPic[i].m_Picture.m_y = m_yPos + 120;
	}
	m_Shadow.m_x = m_PetPic[0].m_Picture.m_x;
	m_Shadow.m_y = m_PetPic[0].m_Picture.m_y;*/
	for (int i = 0; i < 4; i++)
	{
		m_TPetName[i].SetXY(m_xPos + 15, m_yPos + 50 + i * 28);
	}
	m_PetSelectBack[0].SetX(m_xPos + 10);
	m_PetSelectBack[1].SetX(m_xPos + 55);
	m_PetSelectBack[0].m_NeedShow = FALSE;
	m_PetSelectBack[1].m_NeedShow = FALSE;
	
	MovePetProPerty();
	return TRUE;
}


BOOL cInterfacePet::SetPage(int page)
{
	if (page<0)page = 0;
	if ((page * 4) >= g_pCharacter->m_PCData.m_NumofBB)page -= 1;
	if (m_TrueSelect > -1)
		m_TPetName[m_TrueSelect - m_PetPage * 4].SetColor(RGB(0, 0, 0));
	//m_TrueSelect = -1;
	m_PetPage = page;
	UpdateList(m_PetPage);
	if (m_PetPage < 0)
	{
		m_TrueSelect = -1;
		return TRUE;
	}
	m_TrueSelect = m_PetPage * 4;
	SelectPet(m_TrueSelect);
	if (m_PetPage <1)
		m_UpB.SetFrame(3);
	else
		m_UpB.SetFrame(0);

	if (m_PetPage>-1&&m_PetPage < (g_pCharacter->m_PCData.m_NumofBB-1) / 4)
		m_DownB.SetFrame(0);
	else
		m_DownB.SetFrame(3);
	
	RECT rect;
	rect.left = m_xPos + 15;
	rect.right = m_xPos + 120;
	rect.top = m_yPos + 45;
	rect.bottom = m_yPos + 160;
	g_pMainState->m_Canvas.SetRedraw(rect);

	m_Select = 0;
	if (m_TrueSelect == g_pCharacter->m_PCData.m_FightPetID)
		m_SetAttackB.SetFrame(1);
	else  m_SetAttackB.SetFrame(0);
	return TRUE;
}

BOOL cInterfacePet::MovePetProPerty()
{
	m_TPetProPerty[24].SetXCenter(m_SetAddPoint.GetCenter());
	m_TPetProPerty[24].UpdateXCenter();
	m_TPetProPerty[24].SetY(m_SetAddPoint.GetY()+3);


	m_TPetProPerty[23].SetXY(m_xPos + 185, m_yPos + 432);
	m_TPetProPerty[22].SetXY(m_xPos + 20, m_yPos + 432);
	m_TPetProPerty[21].SetXY(m_xPos + 70, m_yPos + 432);
	m_TPetProPerty[20].SetXY(m_xPos + 180, m_yPos + 362);
	m_TPetProPerty[19].SetXY(m_xPos + 228, m_yPos + 147);
	m_TPetProPerty[18].SetXY(m_xPos + 85, m_yPos + 170);
	m_TPetProPerty[17].SetXY(m_xPos + 215, m_yPos + 170);
	m_TPetProPerty[16].SetXY(m_xPos + 50, m_yPos + 410);
	m_TPetProPerty[15].SetXY(m_xPos + 90, m_yPos + 386);
	m_TPetProPerty[14].SetXY(m_xPos + 90, m_yPos + 362);
	m_TPetProPerty[13].SetXY(m_xPos + 180, m_yPos + 338);
	m_TPetProPerty[12].SetXY(m_xPos + 180, m_yPos + 314);
	m_TPetProPerty[11].SetXY(m_xPos + 180, m_yPos + 290);
	m_TPetProPerty[10].SetXY(m_xPos + 180, m_yPos + 266);
	m_TPetProPerty[9].SetXY(m_xPos + 180, m_yPos + 242);
	m_TPetProPerty[8].SetXY(m_xPos + 180, m_yPos + 218);
	m_TPetProPerty[7].SetXY(m_xPos + 50, m_yPos + 338);
	m_TPetProPerty[6].SetXY(m_xPos + 50, m_yPos + 314);
	m_TPetProPerty[5].SetXY(m_xPos + 50, m_yPos + 290);
	m_TPetProPerty[4].SetXY(m_xPos + 50, m_yPos + 266);
	m_TPetProPerty[3].SetXY(m_xPos + 50, m_yPos + 242);
	m_TPetProPerty[0].SetXY(m_xPos + 180, m_yPos + 147);
	m_TPetProPerty[1].SetXY(m_xPos + 50, m_yPos + 194);
	m_TPetProPerty[2].SetXY(m_xPos + 50, m_yPos + 218);
	return TRUE;
}



cInterfacePet::~cInterfacePet()
{
	Free();
}

BOOL cInterfacePet::Free()
{
	m_PetObj.m_IndexType = 0;
	m_PetObj.m_pIndex = 0;
	/*for (int i = 0; i < 4; i++)
		m_PetPic[i].Free();*/
	return TRUE;
}

BOOL cInterfacePetSkill::InitDaShuMode()
{
	if (0==g_pMainState)return FALSE;
	if (INDEXTYEP_CHARACTER != g_pMainState->m_FightList[m_FightID].m_pData->GetType())return FALSE;

	cCharacter* p= (cCharacter*)g_pMainState->m_FightList[m_FightID].m_pData->m_pIndex;
	m_DaShuNum = 0;
	sItem2* pItem=0;
	for (int i = 0; i < 60; i++)//�����ߺ�����
	{
		pItem = &p->m_PCData.m_Item[i];
		if (pItem->m_Num>0)
		{
			if (40 == pItem->GetType())
			{
				if (9 == m_DaShuNum)
					return TRUE;
				m_DaShuItemId[m_DaShuNum] = i;
				m_DaShuHead[m_DaShuNum].Load(g_pMainState->m_SkillManager.GetPetSkill(pItem->GetID())->m_BigHead);
				m_DaShuNum += 1;
			}
		}
	}
	if (m_DaShuNum == 0)
		return FALSE;
	m_DaShuText[1].UpdateXCenter();
	return TRUE;
}

void cInterfacePet::OnOff(BOOL NeedShow)
{
	if (NeedShow == m_NeedShow)return;
	g_pMainState->InterfaceSetShow(this, NeedShow);
	
	if (NeedShow)
	{
		
		m_Sleeptime = 0;
		Init();
		SetPage(m_PetPage);
		SelectPet(m_TrueSelect);
	}
	else m_PetObj.m_pIndex=0;
		g_pMainState->m_Canvas.SetRedraw(m_PetSkill.m_back.m_PreShowRect);
		g_pMainState->m_Canvas.SetRedraw(m_back.m_PreShowRect);
}

BOOL cInterfacePet::ProcessFreePet()
{
	switch (g_pMainState->m_Dialog.m_retSelect)
	{
	case 0:
		m_FreePetMode = FALSE;
		if (0 == g_pMainState->m_Dialog.m_NowMouseOnSelect)
		{
			if (m_TrueSelect == g_pCharacter->m_PCData.m_FightPetID)
			{
				g_pMainState->SetFightPet(g_pMainState->m_HeroID, -1);  //���ٻ�
			}
			else
			{
				if (m_TrueSelect < g_pCharacter->m_PCData.m_FightPetID)
				{
					g_pCharacter->m_PCData.m_FightPetID -= 1;
				}
			}
			g_pCharacter->m_PCData.DeletePet(m_TrueSelect);
			g_pMainState->m_PCHead.SetPet(-1);
			g_pMainState->m_PCHead.UpdatePet();
			SetPage(m_PetPage);
			SelectPet(0);
			if (!g_pCharacter->m_PCData.m_NumofBB)
			{
				OnOff(FALSE);
			}
		}
		g_pMainState->m_Dialog.m_NeedShow = FALSE;
		break;
	case 1:
	case 5:
		m_FreePetMode = FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

void cInterfacePet::UpDateExp()
{
	ostringstream oss;
	oss << g_pCharacter->m_PCData.m_pPetDataList[m_TrueSelect]->m_PetDataZZ.EXP;
	m_TPetProPerty[14].SetString(oss.str());
}

void cInterfacePet::UpDateByLvUp()
{
	
	if (m_TrueSelect<0)return;
	cPet* pPetSelect = g_pCharacter->m_PCData.m_pPetDataList[m_TrueSelect];
	if (!pPetSelect)return;
		ostringstream oss;
		cPetData&  petzz = pPetSelect->m_PetDataZZ;
		oss << petzz.m_LV;
		m_TPetProPerty[0].SetString(oss.str());
		oss.str("");
		oss << petzz.m_Name;
		m_TPetProPerty[1].SetString(oss.str());
		oss.str("");
		oss << petzz.m_HP << "/" << pPetSelect->m_PetDataZZ.m_HPMax.GetData();
		m_TPetProPerty[2].SetString(oss.str());
		oss.str("");
		oss << petzz.m_MP << "/" << pPetSelect->m_PetDataZZ.m_MPMax.GetData();
		m_TPetProPerty[3].SetString(oss.str());
		oss.str("");
		oss << petzz.m_ATK.GetData();
		m_TPetProPerty[4].SetString(oss.str());
		oss.str("");
		oss << petzz.m_DEF.GetData();
		m_TPetProPerty[5].SetString(oss.str());
		oss.str("");
		oss << petzz.m_SPD.GetData();
		m_TPetProPerty[6].SetString(oss.str());
		oss.str("");
		oss << petzz.m_Wakan.GetData();
		m_TPetProPerty[7].SetString(oss.str());
		oss.str("");
		UpdateProPerty();
		UpDateExp();
		
		oss << petzz.ExpMax;
		m_TPetProPerty[15].SetString(oss.str());
		oss.str("");
		oss << 100;
		m_TPetProPerty[16].SetString(oss.str());
		//��ս�ȼ�
		oss.str("");
		oss << pPetSelect->GetData()->m_LVLimit;
		m_TPetProPerty[17].SetString(oss.str());
		//�ٻ�����
		oss.str("");
		oss << g_pCharacter->m_PCData.m_NumofBB << "/" << 8;
		m_TPetProPerty[18].SetString(oss.str());

	if (m_PetSkill.m_NeedShow)
	{
		m_PetSkill.UpdateData(&petzz,TRUE);
	}
}

BOOL cInterfacePet::SelectPet(int id)
{
	int pre = m_TrueSelect - m_PetPage * 4;
	if (pre > -1 && pre< 4)
		m_TPetName[pre].SetColor(RGB(0, 0, 0));
	m_TrueSelect = id;
	int pageid = id - m_PetPage * 4;
	if (pageid<0 || pageid>3)
	{
		m_TrueSelect = m_PetPage * 4;
	}
	
	if (pageid < 0)return FALSE;
	m_TPetName[pageid].SetColor(RGB(255, 255, 0));

	if (m_TrueSelect < 0)
	{
		for (int i = 0; i < 5; i++)
		{
			m_AddPoint[i].SetFrame(3);
			m_SubPoint[i].SetFrame(3);
		}
		UpDateByLvUp();
		Free();
		m_PetSkill.UpdateData(0,TRUE);
	}
	else
	{
		if (g_pCharacter->m_PCData.m_NumofBB <= m_TrueSelect)return FALSE;
		cPet* pPetSelect = g_pCharacter->m_PCData.m_pPetDataList[m_TrueSelect];
		//vector<DWORD> out;
		//pPetSelect->GetID(POS_STAND, out);
		m_PetObj.m_pIndex = pPetSelect;
		m_PetObj.m_IndexType = INDEXTYEP_PET;
		m_PetObj.Reset();
		m_PetObj.Set(POS_STAND);
		/*for (int i = 0; i < 4; i++)
		{
			m_PetPic[i].m_NeedShow = FALSE;
		}
		for (int i = 0; i < out.size(); i++)
		{
			m_PetPic[i].Load(out[i]);
			m_PetPic[i].m_Picture.m_direction = 3;
			m_PetPic[i].m_NeedShow = TRUE;
		}*/
		//���¼���
		//if (m_PetSkill.m_NeedShow)m_PetSkill.UpdateData(&pPetSelect->m_PetDataZZ,TRUE);
		//���ӵ��Ƿ����
		UpdatePointButton();
		UpDateByLvUp();
		m_PetSkill.UpdateData(&pPetSelect->m_PetDataZZ,TRUE);
	}

	if (m_TrueSelect == g_pCharacter->m_PCData.m_FightPetID)
		m_SetAttackB.SetFrame(1);
	else  m_SetAttackB.SetFrame(0);

	
	return TRUE;
}

void cInterfacePet::UpdateList(int page)
{
	if (page<0)
	{
		for (int i = 0; i < 4; i++)
			m_TPetName[i].SetString("");
		return;
	}
	int num2 = g_pCharacter->m_PCData.m_NumofBB - m_PetPage * 4;
	if (num2 > 4)num2 = 4;
	for (int i = 0; i < num2; i++)
		m_TPetName[i].SetString(g_pCharacter->m_PCData.m_pPetDataList[i + m_PetPage * 4]->m_PetDataZZ.m_Name);

}

void cInterfacePet::AddExp(cPetData* pPetSelect,INT64 num)
{//���ٻ������Ӿ���
	pPetSelect->EXP += num;
	int prelv = pPetSelect->m_LV;
	g_pMainState->m_pFightState->CheckPetExp();
	if(prelv != pPetSelect->m_LV)
	{
		UpDateByLvUp();	
	}
	else
		UpDateExp();
	g_pMainState->m_PCHead.UpdatePet();

}

void cInterfacePet::UpdatePointButton()
{
	if (m_bAddPoint)
	{
		cPet* pPetSelect = g_pCharacter->m_PCData.m_pPetDataList[m_TrueSelect];
		sAddPoint& addpoint =  pPetSelect->m_PetDataZZ.m_AddPoint;
		for (int i = 0; i < 5; i++)
		{
			if (0 == addpoint.idle)
				m_AddPoint[i].SetFrame(3);
			else
				m_AddPoint[i].SetFrame(0);
			m_SubPoint[i].SetFrame(0);
		}

		if (0 == addpoint.hp)m_SubPoint[0].SetFrame(3);
		if (0 == addpoint.mp)m_SubPoint[1].SetFrame(3);
		if (0 == addpoint.atk)m_SubPoint[2].SetFrame(3);
		if (0 == addpoint.def)m_SubPoint[3].SetFrame(3);
		if (0 == addpoint.spd)m_SubPoint[4].SetFrame(3);
	}
	else
	{
		for (int i = 0; i < 5; i++)
		{
			m_SubPoint[i].SetFrame(3);
			m_AddPoint[i].SetFrame(3);
		}
	}
}

void cInterfacePet::UpdateProPerty()
{
	cPet* pPetSelect = g_pCharacter->m_PCData.m_pPetDataList[m_TrueSelect];
	ostringstream oss;
	cPetData& petzz = pPetSelect->m_PetDataZZ;
	if (m_bAddPoint)
	{
		oss << petzz.m_AddPoint.hp;
		m_TPetProPerty[8].SetString(oss.str());
		oss.str("");
		oss << petzz.m_AddPoint.mp;
		m_TPetProPerty[9].SetString(oss.str());
		oss.str("");
		oss << petzz.m_AddPoint.atk;
		m_TPetProPerty[10].SetString(oss.str());
		oss.str("");
		oss << petzz.m_AddPoint.def;
		m_TPetProPerty[11].SetString(oss.str());
		oss.str("");
		oss << petzz.m_AddPoint.spd;
		m_TPetProPerty[12].SetString(oss.str());
		oss.str("");
		oss << petzz.m_AddPoint.idle;
		m_TPetProPerty[13].SetString(oss.str());
		oss.str("");
	}
	else
	{
		oss << petzz.m_ProPertyHP;
		m_TPetProPerty[8].SetString(oss.str());
		oss.str("");
		oss << petzz.m_ProPertyMag;
		m_TPetProPerty[9].SetString(oss.str());
		oss.str("");
		oss << petzz.m_ProPertyAtk;
		m_TPetProPerty[10].SetString(oss.str());
		oss.str("");
		oss << petzz.m_ProPertyDef;
		m_TPetProPerty[11].SetString(oss.str());
		oss.str("");
		oss << petzz.m_ProPertySpd;
		m_TPetProPerty[12].SetString(oss.str());
		oss.str("");
		m_TPetProPerty[13].SetString("0");
		oss.str("");
	}
}



void cInterfaceFB::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;
	m_Back.Load( 0x1A2DEAB9);//1259967470);
	m_Back.SetStatic(TRUE);
	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;
	m_Title.SetColor(RGB(255, 255, 255));
	m_Title.SetString("��������");

	m_TButton.SetColor(RGB(255, 255, 255));
	m_TButton.SetString("����");

	m_MouseOnBack.Load(0x8F43CE91);//0x51E340B6);
	m_MouseOnBack.m_NeedShow = FALSE;
	m_MouseOnBack.SetShowWidth(140);

	m_tFBName.resize(5);
	m_tFBName[0].SetString("�ڼ���");
	m_tFBName[1].SetString("ˮ·���");
	m_tFBName[2].SetString("���ٹ�");
	m_tFBName[3].SetString("���ž���");
	m_tFBName[4].SetString("�ļ�");
	Move(320 - m_Back.GetWidth() / 2, 240 - m_Back.GetHeight() / 2);
}

BOOL cInterfaceFB::Show(cCanvas* pCanvas)
{
	if (!m_NeedShow)return TRUE;
	pCanvas->Add(&m_Back, 1, TRUE);
	pCanvas->Add(&m_Button, 1);
	pCanvas->Add(&m_TButton);
	pCanvas->Add(&m_Title);
	pCanvas->Add(&m_Cancel, 1);
	pCanvas->Add(&m_MouseOnBack, 1);
	for (int i = 0; i < m_tFBName.size(); i++)
	{
		pCanvas->Add(&m_tFBName[i]);
	}
	return TRUE;
}

BOOL cInterfaceFB::ProcessInput()
{
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}
	m_Select = -1;
	ResetButton();
	if (!isOn(g_xMouse, g_yMouse, m_Back.m_PreShowRect))return TRUE;
	g_pMainState->SetMouseOnMap(FALSE);
	if (CheckOn(&m_Cancel, 0))return TRUE;
	if (CheckOn(&m_Button, 1))return TRUE;
	if (isOn(g_xMouse, g_yMouse, m_Rect))
	{
		m_Select = (g_yMouse - m_Rect.top) / 20 + 10;
		m_MouseOnBack.m_NeedShow = TRUE;
		m_MouseOnBack.SetXY(m_xPos+16, m_tFBName[m_Select - 10].GetY()-3);
		if (CheckL())return TRUE;
	}
	else
	{
		m_MouseOnBack.m_NeedShow = FALSE;
	}
	if (CheckRB())return TRUE;
	return TRUE;

}

BOOL cInterfaceFB::Move(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
	m_Back.m_Picture.m_x = x;
	m_Back.m_Picture.m_y = y;
	m_Cancel.SetXY(x+m_Back.GetWidth()-20,y+3);
	m_Button.Load(0x79D01E0E);
	m_Button.SetStatic(TRUE);

	m_Title.SetXCenter(m_xPos + 77);
	m_Title.SetY(m_yPos + 3);
	m_Title.UpdateXCenter();
	m_Rect.left = x + 30;
	m_Rect.top = y + 40;
	m_Rect.bottom = m_Rect.top + m_tFBName.size() * 20;
	m_Rect.right = m_Rect.left + 115;

	for (int i = 0; i < m_tFBName.size(); i++)
	{
		m_tFBName[i].SetXY(m_Rect.left, m_Rect.top + i * 20);
	}
	m_Button.SetXY(m_xPos + m_Back.GetWidth() / 2 - m_Button.GetWidth() / 2, m_yPos + 204);
	m_TButton.SetXCenter(m_Button.GetCenter());
	m_TButton.SetY(m_Button.GetY()+3);
	m_TButton.UpdateXCenter();

	m_Title.SetXCenter(m_xPos + 77);
	m_Title.SetY(m_yPos + 3);
	m_Title.UpdateXCenter();
	return TRUE;
}

BOOL cInterfaceFB::CheckL()
{
	if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
	{
		g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	}
	else return FALSE;
	m_Sleeptime = 3;
	switch (m_Select)
	{
	case 0:
		OnOff(FALSE);
		break;
	case 1:
		m_Button.SetFrame(1);
		if (m_TrueSelect > -1)
		{
			if (!g_pCharacter->m_PCData.AddMoney(-50000, 0,0,FALSE))
			{
				g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->Talk("Ǯ������......");
				return TRUE;
			}
			else
			{
				g_pMainState->m_TaskFB.Start(m_TrueSelect);
				ostringstream oss;
				oss << "�õ��˸�������,���Դ�#R";
				oss << g_pMainState->m_TaskFB.m_TextList[0];
				oss << "#R��#R";
				oss << g_pMainState->m_TaskFB.m_TextList[1];
				oss << "#R�����븱��";
				g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->Talk(oss.str());
				g_pMainState->m_TaskFB.Process(0);

				sJob job;
				job.m_Context = oss.str();
				job.m_pTask = &g_pMainState->m_TaskFB;
				job.m_Time = 100;
				job.m_Title ="����";
				g_pMainState->m_Job.AddaJob(job);
			}
		}
		break;
	default:
		if (1)
		{
			if (m_TrueSelect == m_Select-10)return TRUE;
			if (m_TrueSelect>-1)
				m_tFBName[m_TrueSelect].SetColor(0);
			m_TrueSelect = m_Select-10;
			m_tFBName[m_TrueSelect].SetColor(RGB(255,0,0));
		}
		break;
	}
	return FALSE;
}

void cInterfaceFB::OnOff(BOOL NeedShow)
{
	if (NeedShow == m_NeedShow)return;
	g_pMainState->InterfaceSetShow(this, NeedShow);
	if (m_NeedShow)
	{
		Init();
		m_TrueSelect = -1;
	}
	else
	{
		if (m_TrueSelect > 9)
			m_tFBName[m_TrueSelect - 10].SetColor(0);
		m_MouseOnBack.m_NeedShow = FALSE;
	}
	g_pMainState->m_Canvas.SetRedraw(m_Back.m_PreShowRect);
}



BOOL cInterfaceFB::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	OnOff(FALSE);
	return TRUE;
}


void cInterfaceDoor::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;
	
	m_CharacterPos.SetColor( RGB(255, 255, 255));
	m_CharacterPos.SetXCenter(56);
	m_CharacterPos.SetY(63);
	m_MapName.SetXCenter(60);
	m_MapName.SetY(12);
	m_MapName.SetColor(RGB(255, 255, 255));
	m_MapNameNeedShow = TRUE;
	m_GlimSelect = 0;


	m_WorldmapGlim.Load(3136727389);
	m_WorldmapGlim.m_isStatic = TRUE;
	m_Glim.Load(3136727389);
	m_Glim.m_isStatic = TRUE;
	m_MapGlim.Load(3136727389);
	m_MapGlim.m_isStatic = TRUE;
	m_Door.Load(0xDE3F48B7);
	m_Door.m_isStatic = TRUE;
	m_SmallPeople[0].Load(0xAC307575);//��
	m_SmallPeople[1].Load(3351166789); //վ

	m_Door.SetX(0);
	m_Door.SetY(0);
	m_WorldmapGlim.SetX( 96);
	m_WorldmapGlim.SetY( 25);

	m_Glim.SetX( 96);
	m_Glim.SetY( 40);

	m_MapGlim.SetX( 96);
	m_MapGlim.SetY( 55);
	for (int i = 0; i < 2; i++)
	{
		m_SmallPeople[1].SetX(60);
		m_SmallPeople[1].SetY(60);
	}
	m_MapName.m_hFont = g_pMainState->m_hFont[0];
	
}

BOOL cInterfaceDoor::Show(cCanvas* pCanvas)
{
	pCanvas->Add(&m_Back,		 1);
	pCanvas->Add(&m_Door,			 1);
	pCanvas->Add(&m_WorldmapGlim,	 1);
	pCanvas->Add(&m_Glim,			 1);
	pCanvas->Add(&m_MapGlim,		 1);
	pCanvas->Add(&m_SmallPeople[1],  1);
	pCanvas->Add(&m_TimeShiCheng.m_Data, 1, FALSE);//��ʾʱ��

	pCanvas->Add(&m_CharacterPos);

	if (m_MapNameNeedShow)
	{
		pCanvas->Add(&m_MapName);
	}
	return TRUE;
}

BOOL cInterfaceDoor::ProcessInput()
{
	m_TimeShiCheng.UpdateTime();
	
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}

	if (m_bRunMode <2)
	{	
		if (0 == m_TimeShiCheng.ss % 3)
		{
			if (m_bRunMode)
			{
				m_MapName.m_x -= 1;
				m_MapName.SetShowWidth(m_MapName.m_ShowWidth+1);
				if (m_MapName.m_x < 23)
				{
					m_bRunMode = 0;
				}
			}
			else
			{
				m_MapName.SetxOffset2(m_MapName.m_xOffset2 + 1);
			
				if (m_MapName.GetShowWidth() < 2)
				{
					m_MapName.SetX(21 + wid);
					m_MapName.SetxOffset2(0);
					m_MapName.SetShowWidth(1);
					m_bRunMode = 1;
				}
			}
			m_MapName.m_bMove = TRUE;
		}
	}
	if (g_StateType != STATE_MAIN)return FALSE;
	CheckFight();
	cObj* pPc1 = g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData;
	if ((pPc1->GetX() != m_xoldHeroPos)
		|| (pPc1->GetY() != m_yoldHeroPos))
	{
		m_xoldHeroPos = pPc1->GetX();
		m_yoldHeroPos = pPc1->GetY();
		UpdatePos(pPc1);
	}

	if (!g_pMainState->GetLockMouse())return TRUE;
	ResetButton();
	if (g_yMouse > m_Door.m_PreShowRect.bottom)return TRUE;
	if (g_xMouse > m_Door.m_PreShowRect.right)return TRUE;
	g_pMainState->SetMouseOnMap(FALSE);

	
	if( CheckOn(&m_MapGlim,3))return TRUE;
	if (CheckOn(&m_Glim, 2))return TRUE;
	if (CheckOn(&m_WorldmapGlim, 1))return TRUE;
	return FALSE;
}

BOOL cInterfaceDoor::CheckL()
{
	if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
	{
		g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	}
	else return FALSE;
	m_Sleeptime = 3;
	switch (m_Select)
	{
	case 1:
		g_pMainState->ClearInterfaceSetShow();
		g_pMainState->m_WorldMap.OnOff(!g_pMainState->m_WorldMap.m_NeedShow);
		return TRUE;
		break;
	case 2:

		break;
	case 3:

		break;
	default:
		break;
	}
	return FALSE;
}

void cInterfaceDoor::UpdatePos(cObj* pObj)
{
	if (!g_pMainState)return;
	ostringstream oss;
	m_HeroX = pObj->GetX() / 20;
	m_HeroY = (g_pMainState->m_Map.m_pMap->m_Height - pObj->GetY()) / 20;
	oss << "X:" << m_HeroX<< " Y:" <<m_HeroY ;  //��ʾ��ɫ����	
	m_CharacterPos.SetString(oss.str());
	m_CharacterPos.UpdateXCenter();
	m_CharacterPos.m_bMove = TRUE;
}

void cInterfaceDoor::SetName(string name)
{
	g_pMainState->m_Canvas.SetRedraw(m_MapName.m_PreShowRect);
	m_MapName.SetString(name);
	if (m_MapName.GetTrueWidth() > wid)
	{
		m_MapName.SetX(22);
		m_MapName.SetShowWidth(wid);
		m_bRunMode = 0;
	}
	else
	{
		m_MapName.UpdateXCenter();
		m_bRunMode = 2;
	}
}

void cInterfaceDoor::CheckFight()
{
	if (!m_pTask)return;
	if (m_AvoidFight == 2) return;//������
	
	switch (g_pMainState->m_Map.m_MapType)//m_stage == 3)
	{
	case 2://����
			if (g_pHeroObj->m_time != -2) //վ��ʱ������
			{
				int xpos, ypos;
				cObj* p;
				xpos = g_pHeroObj->GetX();
				ypos = g_pHeroObj->GetY();
				for (int i = 0; i < g_pMainState->m_ObjNumber; i++)
				{
					if (g_pMainState->m_pObj[i]->m_IndexType != INDEXTYEP_PET)continue;
					p = g_pMainState->m_pObj[i];
					cPet* pPet = (cPet*)p->m_pIndex;
					string name = p->m_TrueName.GetString();
					if (0 != name.compare(pPet->GetData()->m_Name))
						continue;
					if (abs(p->GetX() - xpos)>25)continue;
					if (abs(p->GetY() - ypos) > 25)continue;
					g_pMainState->m_TaskNormal.m_pNpc = g_pMainState->m_pObj[i];
					m_pTask->Process(m_stage);
					return;
				}
			}	
			if (m_TimeShiCheng.ss%20 == 1)
			{
				cObj* p;
				for (int i = rand() % g_pMainState->m_ObjNumber; i < g_pMainState->m_ObjNumber; i++)
				{
					if (g_pMainState->m_pObj[i]->m_IndexType != INDEXTYEP_PET)continue;
					p = g_pMainState->m_pObj[i];
					cPet* pPet = (cPet*)p->m_pIndex;
					string name = p->m_TrueName.GetString();
					if (0 != name.compare(pPet->GetData()->m_Name))
						continue;
					int xmove=0;
					int ymove=0;
					int gap = 30;
					m_TimeShiCheng.UpdateTime();
					switch (rand() % 8)
					{
					case 0:xmove = -gap; ymove = -gap; break;
					case 1:xmove = 0; ymove = -gap; break;
					case 2:xmove = -gap; ymove = 0; break;
					case 3:xmove = gap; ymove = -gap; break;
					case 4:xmove = -gap; ymove = gap; break;
					case 5:xmove = gap; ymove = 0; break;
					case 6:xmove = 0; ymove = gap; break;
					case 7:xmove = gap; ymove = gap; break;
					}
					p->m_time = -1;
					p->SetTarget(g_pMainState->m_pObj[i]->GetX() + xmove, g_pMainState->m_pObj[i]->GetY() + ymove);
					return;
				}
			}
		//}	
		break;
	case 1:
		if (1) //����
		{
			if (g_pHeroObj->m_time == -2)return; //վ��ʱ������
			if (m_AvoidFight == 1) //�����㲻����
			{
				if (g_pCharacter->m_PCData.m_LV > g_pMainState->m_Map.m_EnemyLv)return;
			}
			if (m_nowtimeleast > 0)
			{
				m_nowtimeleast -= 1;
			}
			else
			{
				m_pTask->Process(m_stage);
				m_nowtimeleast = m_timeleast + (rand() % 50 * m_timeleast) / 100;
			}
		}
	}
}

void cInterfaceDoor::SetTrigger(cTaskFather* pFather, int stage, int least/*=100*/)
{
	if (pFather)
	{
		m_pTask = pFather;
		m_stage = stage;
		if (m_bHalf)least /= 2;
		m_timeleast = least;
	/*	if (0==m_nowtimeleast)
			m_nowtimeleast = least;*/
		return;
	}
	else
	{
		m_nowtimeleast = 400;
		m_pTask = 0;
	}
}

void cInterfaceDoor::SetWhite(BOOL b)
{
	if (m_bWhite == b)return;
	m_bWhite = b;
	if (b)
	{		
		m_Back.Load(0x9DF6DEBC); //����	
	}
	else
	{
		m_Back.Load(2574487372);  //����
	}
	m_Back.SetStatic(TRUE);
	m_Back.SetX(20);
	m_Back.SetY(35);
	m_Back.SetShowWidth(80);
}

void cInterfaceDoor::Save(ofstream& File)
{
	File << m_SaveNum << " ";
	File << g_pCharacter->m_PCData.m_LV<<" ";
	File << m_bWhite << " ";
	File << m_nowtimeleast << " ";
	m_SaveNum += 1;
}

void cInterfaceDoor::Load(ifstream& File)
{
	File >> m_SaveNum;
	int dustbin;
	File >> dustbin;
	m_SaveNum += 1;
	BOOL bwhite;
	File >> bwhite;
	SetWhite(bwhite);
	File >> m_nowtimeleast;
}

void cInterfaceDoor::SetHalf(BOOL bTrue /*= TRUE*/)
{
	if (bTrue == -1)bTrue = !m_bHalf;
	m_bHalf = bTrue;
	if (bTrue)
	{
	m_timeleast /= 2; 
	g_pMainState->m_Channel.AddNewString("������������");
	}
	else
	{
		m_timeleast *= 2;
		g_pMainState->m_Channel.AddNewString("������������");
	}
}





BOOL cFriend::Init()
{
	if (m_bInit)return TRUE;
	m_bInit = TRUE;
	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;
	for (int i = 0; i < 4; i++)
	{
		m_Buttonp[i].m_bStatic = TRUE;
		m_Buttonp[i].Set( &g_pMainState->m_Button4);
		m_ButtonText[i].SetColor(RGB(255,255,255));
		m_ButtonText[i].m_hFont = g_pMainState->m_hFont[0];
	}
	m_ButtonText[0].SetString( "�������");
	m_ButtonText[1].SetString( "����ǿ��");

	m_ButtonText[2].SetString(  "��TAǮ");
	m_ButtonText[3].SetString( "ҪǮ");
	for (int i = 0; i < 2; i++)
	{
		m_PetSelectBack[i].Load(0xF151309F);
		m_PetSelectBack[i].SetStatic(TRUE);
		m_PetSelectBack[i].SetFrame(1);
		m_PetSelectBack[i].SetxOffset(38);
	}
	m_nowPage = 0;
	m_Back.Load(3482058907);

	Move(640 - m_Back.m_Picture.m_Width, 50);
	return TRUE;
}

BOOL cFriend::Show(cCanvas* pCanvas)
{
	if (!m_NeedShow)return TRUE;
	pCanvas->Add(&m_Back, 1, TRUE);
	pCanvas->Add(&m_Cancel, 1);
	if (m_TrueFriendSelect > -1)
	{
		pCanvas->Add(&m_PetSelectBack[0], 1);
		pCanvas->Add(&m_PetSelectBack[1], 1);
	}
	for (int i = 0; i < m_NameNum; i++)
	{
		
		pCanvas->Add(&m_NameList[i]);
		pCanvas->Add(&m_NameListHead[i], 1);
	}
	for (int i = 0; i < 4; i++)
	{
		pCanvas->Add(&m_Buttonp[i],		1);
		pCanvas->Add(&m_ButtonText[i]);
	}
	return TRUE;
}

BOOL cFriend::ProcessInput()
{
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}
	ResetButton();
	if (m_bMoveMode)//�ƶ�
	{
		m_Select = 26;
		g_pMainState->SetMouseOnMap(FALSE);
		if (!CheckL())
			m_bMoveMode = FALSE;
		return TRUE;
	}
	m_Select = -1;


		for (int i = 0; i < 2; i++)
		{
			m_PetSelectBack[i].m_NeedShow = FALSE;
		}
	
	if (!isOn(g_xMouse, g_yMouse, m_Back.m_PreShowRect))return FALSE;
	g_pMainState->SetMouseOnMap(FALSE);
	g_pMainState->m_Cursor.m_FollowMode = FALSE;
	if (CheckOn(&m_Cancel, 20))return TRUE;
	for (int i = 0; i<4; i++)
	if (CheckOn(&m_Buttonp[i], 10 + i))return TRUE;

	int yStart = m_yPos + 110;
	if (m_NameNum>0)
	if (g_yMouse >m_Rect.top && g_yMouse<m_Rect.bottom)
	{
		m_Select = (g_yMouse - yStart) / 35;
		if (m_Select>m_NameNum-1)m_Select = m_NameNum-1;
		for (int i = 0; i < 2; i++)
		{
			m_PetSelectBack[i].SetY(yStart + m_Select * 35+2);
			m_PetSelectBack[i].m_NeedShow = TRUE;
		}
		
		if(CheckL())return TRUE;
		if (CheckRB())return TRUE;
	}

	//׼���ƶ�
	m_Select = 27;
	if (CheckL())return TRUE;
	m_Select = 20;
	if (CheckRB())return TRUE;
	return TRUE;
}

BOOL cFriend::Move(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
	m_PetSelectBack[0].m_Picture.m_x = m_xPos+5;
	m_PetSelectBack[1].m_Picture.m_x = m_PetSelectBack[0].m_Picture.m_x + m_PetSelectBack[0].m_Picture.m_ShowWidth;
	for (int i = 0; i < 2; i++)
	{
		m_PetSelectBack[i].m_Picture.m_y = m_yPos + 112 + m_TrueFriendSelect * 35;;
		m_PetSelectBack[i].m_bMove = TRUE;
	}

	m_Back.SetXY( x,y);
	for (int i = 0; i < 6; i++)
	{
		m_NameListHead[i].m_Picture.SetXY( x + 10, y + 110 + i * 35);
		m_NameList[i].SetXY(x + 50, y + 118 + i * 35);
	}
	for (int i = 0; i < 4;i++)
	{
		m_Buttonp[i].m_x = m_xPos + 25;
	
	}
	m_Cancel.SetXY( x + 107,y + 4);
	m_Buttonp[0].m_y = m_yPos + 30;
	m_Buttonp[1].m_y = m_yPos + 65;
	m_Buttonp[2].m_y = m_yPos + 335;
	m_Buttonp[3].m_y = m_yPos + 370;
	for (int i = 0; i < 4; i++)
	{
		m_ButtonText[i].SetXCenter(m_Buttonp[i].m_x + m_Buttonp[i].GetShowWidth()/2);
		m_ButtonText[i].SetY(m_Buttonp[i].m_y+3);
		m_ButtonText[i].UpdateXCenter();
	}
	UpdateList(m_nowPage);

	m_Rect.left = m_xPos + 3;
	m_Rect.right = m_Rect.left + 120;
	m_Rect.top = m_yPos + 110;
	m_Rect.bottom = m_Rect.top + 210;
	return TRUE;
}

BOOL cFriend::AddFriend(string filepath)
{
	cObj* Obj=new cObj;
	g_pMainState->LoadCharacterData2(Obj, filepath);
	cCharacter* pc=(cCharacter*)Obj->m_pIndex;
	cPCData * pcdata = &pc->m_PCData;
	sFriendList lis;
	//lis.m_bOnLine = FALSE;
	lis.m_Name = pcdata->m_Name;
	lis.m_dataFileName = pcdata->m_dataFileName;
	lis.m_Feeling = 0;
	lis.m_traderLV = pc->m_PCData.m_traderLV;
	//��¼����
	lis.m_modeid=pcdata->modeid;
    lis.m_stronglv=pcdata->m_StrongLv;//���Լӳ�
	lis.m_money = 0;
	//��������
	if (pcdata->m_Equip[2].GetNum())
	{
		lis.m_weaponid = pcdata->m_Equip[2].GetID();
		lis.m_weapontype = pcdata->m_Equip[2].GetType();
	}
	else
	{
		lis.m_weapontype = -1;
		lis.m_weaponid = -1;
	}
	//Ⱦɫ
	lis.m_Ranse = pcdata->m_RanSe;
	
	lis.m_ZuoQiRanse = pcdata->m_ZuoQiRanSe;
	
	//����
	lis.m_bOnZuoQi= pcdata->m_bOnZuoQi; //�Ƿ���������
	lis.m_ZuoQiID= pcdata->zuoqiid;
	lis.m_bZhuangShi = pcdata->m_ZuoQi.m_ZhuangShi.GetNum();
	if (lis.m_bZhuangShi)
	{
		lis.m_ZhuangShiID = pcdata->m_ZuoQi.m_ZhuangShi.GetID();
	}
	lis.m_ZhuangShiRanse = pcdata->m_ZhuangShiRanSe;

	lis.m_money = 29999999;//BUG
	for (int i = 0; i < m_FriendList.size(); i++)
	{
		if (0 == m_FriendList[i].m_Name.compare(lis.m_Name))
		{
			m_FriendList.erase(m_FriendList.begin() + i);
			break;
		}
	}
	m_FriendList.push_back(lis);
	Obj->Free();
	SAFE_DELETE(Obj);
	return TRUE;
}

BOOL cFriend::UpdateList(int page/*=-1*/)
{
	if (page == -1)
		page = m_nowPage;
	else m_nowPage = page;
	m_NameNum = m_FriendList.size() - page * m_MaxNameNum;
	if (m_NameNum<0)return FALSE;
	if (m_NameNum > m_MaxNameNum)m_NameNum = m_MaxNameNum;
	for (int i = 0; i < m_NameNum; i++)
	{
		if (!m_FriendList[i + m_nowPage * m_MaxNameNum].CheckInTeam()) //���Ա�����Ľ�ɫ��������ɫ
		{
			m_NameList[i].SetColor(RGB(0, 126, 0));
		}
		else  //�����Ա�����Ľ�ɫ�����Ǻ�ɫ
		{
			m_NameList[i].SetColor(RGB(0, 0, 0));
		}
		m_NameList[i].SetString(m_FriendList[i + page * m_MaxNameNum].m_Name);
		m_NameList[i].m_NeedShow = TRUE;

			m_NameListHead[i].Load(m_FriendList[i + page*m_MaxNameNum].GetFriendHead());
	}
	m_Back.m_bMove = TRUE;
	return TRUE;
}

BOOL cFriend::CheckL()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	m_Sleeptime = 3;
	switch (m_Select)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		if (m_TrueFriendSelect > -1)
		{//��֮ǰ�ı����	
			if (!m_FriendList[m_TrueFriendSelect + m_nowPage * m_MaxNameNum].CheckInTeam()) //���Ա�����Ľ�ɫ��������ɫ
			{
				m_NameList[m_TrueFriendSelect].SetColor(RGB(0, 126, 0));
			}
			else  //�����Ա�����Ľ�ɫ�����Ǻ�ɫ
			{
				m_NameList[m_TrueFriendSelect].SetColor(RGB(0, 0, 0));
			}
		}
		m_TrueFriendSelect = m_Select + m_MaxNameNum * m_nowPage;
		m_NameList[m_TrueFriendSelect].SetColor(RGB(255, 0, 0));
		if (m_friendqianghua.m_NeedShow)
		{
			m_friendqianghua.Set(m_FriendList[m_TrueFriendSelect]);
		}
		break;
	case 10://�������
		m_Buttonp[0].SetFrame( 1);
		if (m_TrueFriendSelect < 0)return TRUE;
		if (m_FriendList.size() <= m_TrueFriendSelect)return TRUE;
		if (-1 == g_pMainState->CheckInTeam(m_FriendList[m_TrueFriendSelect].m_Name))break;
		if (m_FriendList[m_TrueFriendSelect].CheckInTeam())return TRUE;
		Invite(m_TrueFriendSelect);
		if (m_TrueFriendSelect + m_nowPage * m_MaxNameNum>-1)
		m_NameList[m_TrueFriendSelect + m_nowPage * m_MaxNameNum].SetColor(0);
		break;
	case 11:
		m_Buttonp[1].SetFrame(1);
		if (m_TrueFriendSelect < 0)return TRUE;
		m_friendqianghua.Set(m_FriendList[m_TrueFriendSelect+m_nowPage*m_MaxNameNum]);
		m_friendqianghua.OnOff(TRUE);
		break;
	case 12://��Ǯ
		
		{
			m_Buttonp[2].SetFrame(1);
			if (m_TrueFriendSelect < 0)return TRUE;
			g_pMainState->m_YingHang.SetMode(0, &m_FriendList[m_TrueFriendSelect]);
			g_pMainState->m_YingHang.OnOff(TRUE);
		/*	g_pMainState->m_YingHang.m_ShuRu.SetMaxNum(g_pCharacter->m_PCData.m_Money[0]);
			ostringstream oss;
			oss << m_FriendList[m_TrueFriendSelect + m_nowPage * m_MaxNameNum].m_money;
			g_pMainState->m_YingHang.m_CunYing.SetString(oss.str());
				*/
		}
		break;
	case 13: //ҪǮ
		
		{
			m_Buttonp[3].SetFrame(1);
			if (m_TrueFriendSelect < 0)return TRUE;
			g_pMainState->m_YingHang.SetMode(1, &m_FriendList[m_TrueFriendSelect]);
			g_pMainState->m_YingHang.OnOff(TRUE);
		}
		break;
	case 20://�ر�
		OnOff(FALSE);
		m_Cancel.SetFrame(1);
		break;

	case 26://�ƶ�
		
		{
			m_Sleeptime = 0;
			g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
			int xDel = g_xMouse - m_Prex;
			int yDel = g_yMouse - m_Prey;
			m_Prex = g_xMouse;
			m_Prey = g_yMouse;
			if (xDel != 0 || yDel != 0)
			{
				Move(m_xPos + xDel, m_yPos + yDel);
			}
		}
		return TRUE;
	case 27://׼���ƶ�
		g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
		m_Prex = g_xMouse;
		m_Prey = g_yMouse;
		m_bMoveMode = TRUE;
		return TRUE;
	}
	return TRUE;
}

BOOL cFriend::Invite(int id, int idlepos)
{
	//��û�п�λ
	if (idlepos == -1)
	{
		int i=0;
		for (; i < 5; i++)
		{
			if (!g_pMainState->m_FightList[g_pMainState->m_InterfaceTeam.GetTeamTurn(i)].isLive)
			{
				idlepos = g_pMainState->m_InterfaceTeam.GetTeamTurn(i);
				break;
			}
		}
		if (5 == i)return FALSE;
	}

	//����Ƿ��Ѿ������ڵ���˫����
	for (int i = 0; i < 20; i++)
	{
		if (g_pMainState->m_FightList[i].isLive)
		{
			cPetData* pPetdata;
			g_pMainState->m_FightList[i].m_pData->GetDateP(pPetdata);
			if (0 == pPetdata->m_Name.compare(m_FriendList[id].m_Name))
			{
				return FALSE;
			}
		}
	}
	g_pMainState->LoadCharacterData2(idlepos, m_FriendList[id].m_dataFileName);
	sFightList& list = g_pMainState->m_FightList[idlepos];
	
	cCharacter* pc = (cCharacter*)list.m_pData->m_pIndex;
	if (idlepos < 10)
	{
		pc->m_PCData.m_Controler.Set(0, 0);
		if (g_pCharacter->m_PCData.m_LV>130)pc->m_PCData.m_bDuJie = TRUE;
	}
	else pc->m_PCData.m_Controler.Set(0, 1);
	pc->m_PCData.m_IDinFightList = idlepos;
	pc->m_PCData.m_Popularity = g_pCharacter->m_PCData.m_Popularity;
	list.isLive = TRUE;
	list.SetDie(FALSE);
	list.m_pData->SetDirection(g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->GetDirection());
	
	list.m_pData->SetX(g_pHeroObj->GetX());
	list.m_pData->SetY(g_pHeroObj->GetY());
	list.m_pData->Stand();
	

	list.m_FriendListid = id;//��¼

	/*pc->m_PCData.m_RanSe = m_FriendList[id].m_Ranse;


	if (m_FriendList[id].m_bOnZuoQi)
	{
		pc->m_PCData.m_bOnZuoQi = m_FriendList[id].m_bOnZuoQi;
		g_pMainState->m_ZuoQi.GetZuoQi(pc->m_PCData, m_FriendList[id].m_ZuoQiID);
		pc->m_PCData.m_ZuoQiRanSe = m_FriendList[id].m_ZuoQiRanse;

	}*/

	sFriendList& list2 = m_FriendList[id];
	int jinjienum = (g_pCharacter->m_PCData.m_LV + 10) / 60;
	for (int i = 0; i < list2.m_BBList.size(); i++)
	{
		cPet* ppet = new cPet;
		ppet->Set(&list2.m_BBList[i],
			list2.m_stronglv
			);
		if (jinjienum)
		ppet->m_PetDataZZ.SetJinJie(jinjienum);
		g_pMainState->GivePcAPet(idlepos, ppet);
		ppet = 0;
	}
	g_pMainState->SetFightPet(idlepos, list2.m_FightBB,FALSE);

	//
	sZiZhi& zz = list2.m_zizhi;
	pc->m_PCData.AddAtkZZ(zz.m_ATKZZ);
	pc->m_PCData.AddDefZZ(zz.m_DEFZZ);
	pc->m_PCData.AddMagZZ(zz.m_MAGZZ);
	pc->m_PCData.AddHpZZ(zz.m_HPZZ);
	pc->m_PCData.AddAvoidZZ(zz.m_AVOIDZZ);
	pc->m_PCData.AddSpdZZ(zz.m_SPDZZ);
	pc->m_PCData.AddGrowing(zz.m_Growing);


	g_pMainState->FollowLv(idlepos);
	//�����������
	pc->m_PCData.m_JinJieNum = list2.m_JinJieNum;
	if (list2.m_weapontype > -1)
	{
		int id = pc->m_PCData.m_Equip[2].GetID();
		pc->m_PCData.m_Equip[2].Set(list2.m_weapontype, id);
	}
	pc->m_PCData.m_RanSe = list2.m_Ranse;


	pc->m_PCData.m_bOnZuoQi = list2.m_bOnZuoQi;
	if (list2.m_ZuoQiID > -1)
	{
		pc->m_PCData.m_ZuoQiRanSe = list2.m_ZuoQiRanse;
		g_pMainState->m_ZuoQi.GetZuoQi(pc->m_PCData, list2.m_ZuoQiID);
		if (list2.m_ZhuangShiID > -1)
		{
			pc->m_PCData.m_ZuoQi.m_ZhuangShi.Set(24, list2.m_ZhuangShiID);
			pc->m_PCData.m_ZhuangShiRanSe = list2.m_ZhuangShiRanse;
		}
	}

	list.m_pData->Reset();
	list.m_pData->Set(POS_STAND);
	list.m_pData->MoveShadow();
	g_pMainState->m_PCHead.UpDateHead();

	return TRUE;
}

BOOL cFriend::Invite(string name, int fighterid)
{
	for (int i = 0; i < m_FriendList.size(); i++)
	{
		if (name == m_FriendList[i].m_Name)
		{
			Invite(i, fighterid);
			g_pMainState->m_PCHead.UpDateHead();
			return TRUE;
		}
	}
	if (AddFriend(name))
	{
		return Invite(name);
	}
	return FALSE;
}

void cFriend::OnOff(BOOL NeedShow)
{
	return ;
	//if (m_MessageList.size())
	//{
	//	g_pMainState->m_TriggerDialog.AddTask(m_MessageList[0].messagePath, m_MessageList[0].id, m_MessageList[0].p, m_MessageList[0].taskid);
	//	m_MessageList.erase(m_MessageList.begin());
	//	if (!m_MessageList.size())
	//	{
	//		g_pMainState->m_Menu.m_Info.SetStatic(TRUE);
	//		g_pMainState->m_Menu.m_Info.SetFrame(0);
	//	}
	//	return;
	//}
	//if (NeedShow == m_NeedShow)return;
	//g_pMainState->InterfaceSetShow(this, NeedShow);
	//if (m_NeedShow)
	//{
	//	Init();
	//	UpdateList(m_nowPage);
	//	if (m_TrueFriendSelect >= m_NameNum)
	//	{
	//		if (m_TrueFriendSelect > -1)
	//		{
	//			if (!m_FriendList[m_TrueFriendSelect + m_nowPage * m_MaxNameNum].CheckInTeam()) //���Ա�����Ľ�ɫ��������ɫ
	//			{
	//				m_NameList[m_TrueFriendSelect].SetColor(RGB(0, 126, 0));
	//			}
	//			else  //�����Ա�����Ľ�ɫ�����Ǻ�ɫ
	//			{
	//				m_NameList[m_TrueFriendSelect].SetColor(RGB(0, 0, 0));
	//			}
	//		}
	//		m_TrueFriendSelect = m_NameNum - 1;
	//		if (m_TrueFriendSelect>-1)
	//		m_NameList[m_TrueFriendSelect].SetColor(RGB(255, 0, 0));
	//	}
	//	m_Sleeptime = 0;
	//}
	//g_pMainState->m_Canvas.SetRedraw(m_Back.m_PreShowRect);
}

BOOL cFriend::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	switch (m_Select)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		if (1)
		{
			g_pMainState->m_ChaKan.Set(m_FriendList[m_Select + m_nowPage * m_MaxNameNum]);
			g_pMainState->m_ChaKan.OnOff(TRUE);
		}
		break;
	case 20:
		 OnOff(FALSE);
		 break;
	default:
		break;
	}
	
	return TRUE;
}

void cFriend::JuanZeng(int num)
{
	
	g_pCharacter->m_PCData.m_Money[0] -= num;
	if (g_pCharacter->m_PCData.m_Money[0] < 0)
	{
		g_pCharacter->Talk("Ǯ����.....");
		g_pCharacter->m_PCData.m_Money[0] += num;
		return;
	}
	m_FriendList[m_TrueFriendSelect + m_nowPage * m_MaxNameNum].m_money += num;
}
void cFriend::UnJuanZeng(int num)
{
	m_FriendList[m_TrueFriendSelect + m_nowPage * m_MaxNameNum].m_money -= num;
	if (m_FriendList[m_TrueFriendSelect + m_nowPage * m_MaxNameNum].m_money  < 0)
	{
		g_pCharacter->Talk("��TA�ܾ�����....");
		m_FriendList[m_TrueFriendSelect + m_nowPage * m_MaxNameNum].m_money += num;
	}
	g_pCharacter->m_PCData.m_Money[0] += num;
}

void cFriend::Save(ofstream& File)
{//���浽�ļ�
	File << "\n";
	File << m_MessageList.size() << " ";
	for (int i = 0; i < m_MessageList.size(); i++)
	{
		m_MessageList[i].Save(File);
	}
	File << "\n";
	File << "��������б�" << "\n";
	File << m_FriendList.size() << "\n";
	for (int i = 0; i < m_FriendList.size(); i++)
	{
		m_FriendList[i].Save(File);
	}
}

void cFriend::Load(ifstream& File)
{//��ȡ�ļ�
	int nummessage;
	File >> nummessage;
	if (nummessage)
	{
		g_pMainState->m_Menu.m_Info.SetStatic(FALSE);
		m_MessageList.resize(nummessage);
		for (int i = 0; i < nummessage; i++)
		{
			m_MessageList[i].Load(File);
		}
	}
	else
	{
		m_MessageList.clear();
		g_pMainState->m_Menu.m_Info.SetStatic(TRUE);
	}
	g_pMainState->m_Menu.m_Info.SetFrame(0);
	int numfriend;
	string dustbin;
	getline(File, dustbin);
	getline(File, dustbin);
	if (dustbin != "��������б�")
		ERRBOX;
	File >> numfriend;

	m_FriendList.resize(numfriend);
	for (int i = 0; i < numfriend; i++)
	{
		m_FriendList[i].Load(File);
	}
}

BOOL cFriend::UnInvite(string name)
{
	for (int i = 0; i < 5; i++)
	{
		sFightList& list = g_pMainState->m_FightList[g_pMainState->m_InterfaceTeam.GetTeamTurn(i)];
		string targetname= list.m_pData->m_TrueName.GetString();
		if (targetname==name)
		{
			UnInvite(i);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL cFriend::UnInvite(int id)
{
	int trueid = g_pMainState->m_InterfaceTeam.GetTeamTurn(id);
	if (!g_pMainState->m_FightList[trueid].isLive)return TRUE;
	int i=id;
	for (; i < 4; )
	{
		int nexttrueid = g_pMainState->m_InterfaceTeam.GetTeamTurn(i + 1);
		if (g_pMainState->m_FightList[nexttrueid].isLive)
		{
			g_pMainState->m_InterfaceTeam.SwapTeamTurn(i, i + 1);;
		}else break;
		++i;
	}
	g_pMainState->FreeFightList(g_pMainState->m_InterfaceTeam.GetTeamTurn(i));
	g_pMainState->m_PCHead.UpDateHead();
	return TRUE;
}

void cFriend::AddMeesage(string path, int id, cTaskFather* ptask, int taskid)
{
	sMessageList list;
	list.messagePath = path;
	list.p = ptask;
	list.taskid = taskid;
	list.id = id;
	m_MessageList.push_back(list);
	g_pMainState->m_Menu.m_Info.SetStatic(FALSE);
}

int cFriend::FindFriend(string name)
{
	for (int i = 0; i < m_FriendList.size(); i++)
	{
		if (!m_FriendList[i].m_Name.compare(name))return i;
	}
	AddFriend(name);
	return FindFriend(name);
}

void cFriend::AddDay()
{
	for (int i = 0; i < m_FriendList.size(); i++)
	{
		sFriendList& l= m_FriendList[i];
		int money = (l.m_stronglv+100)*g_pCharacter->m_PCData.m_LV*300;
		if (l.m_traderLV)
		{
			money += (money*(20 + l.m_traderLV)) / 100;
		}
		if (l.m_stronglv<100)
			l.m_stronglv += 1;
	}
}

void cFriend::ReMove(string name)
{
	for (int i = 0; i < m_FriendList.size(); i++)
	{
		if (m_FriendList[i].m_Name == name)
		{
			m_FriendList.erase(m_FriendList.begin() + i);
			return;
		}
	}
}



BOOL cInterfaceFather::isInRect(long x, long y, CWFile* pPic)
{
	RECT rect;
	rect.left = pPic->m_Picture.m_x;
	rect.top =  pPic->m_Picture.m_y;
	if (pPic->m_Picture.m_ShowHeight==0)
	rect.bottom = rect.top + pPic->m_Picture.m_Height;
	else  rect.bottom = rect.top + pPic->m_Picture.m_ShowHeight;
	if (pPic->m_Picture.m_ShowWidth == 0)
	rect.right = rect.left + pPic->m_Picture.m_Width;
	else  rect.right = rect.left + pPic->m_Picture.m_ShowWidth;
	
	return isOn(x, y, rect);
}

BOOL cInterfaceFather::CheckOn(CWFile* pTarget, int id)
{
	if (!isOn(g_xMouse, g_yMouse, pTarget->m_PreShowRect))return FALSE;
	m_Select = id;
	if (pTarget->GetNowFrame() == 0)
	{
		m_frampre = pTarget;
		m_frampre->SetFrame(2);
	}
	pTarget->m_bMove = TRUE;
	CheckL();
	return TRUE;
}

BOOL cInterfaceFather::CheckOn(cWfileP* pTarget, int id)
{
	if (!isOn(g_xMouse, g_yMouse, pTarget->m_PreShowRect))return FALSE;
	m_Select = id;
	if (pTarget->GetFrame() == 0)
	{
		m_framprep = pTarget;
		m_framprep->SetFrame( 2);
	}
	pTarget->m_bMove = TRUE;
	return CheckL();
}

BOOL cInterfaceFather::CheckOn(cText* ptext, int id)
{
	if (isOn(g_xMouse, g_yMouse, ptext->m_PreShowRect))
	{
		m_Select = id;
		return CheckL();
	}
	return FALSE;
}

void cInterfaceFather::ResetButton()
{
	if (m_framprep)
	{
		if (3 == m_framprep->GetFrame())return;
		m_framprep->SetFrame( 0);
		m_framprep->m_bMove = TRUE;
		m_framprep = 0;
	}
	else
	if (m_frampre)
	{
		if (3 == m_frampre->GetNowFrame())return;
		m_frampre->SetFrame(0);
		m_frampre->m_bMove = TRUE;
		m_frampre = 0;
	}
}

void cInterfaceDialog::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;
	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;

	m_NameBack.Load(0x2EFADFA5);
	m_SelectT.resize(4);
	for (int i = 0; i < 4; i++)
	{
		m_SelectT[i].SetColor(RGB(255, 0, 0));
		m_SelectT[i].m_x = 0;
	}
	m_NowMouseOnSelect = -1;

	m_NameBack.m_Picture.m_x = 80;
	m_NameBack.m_Picture.m_y = 220;
	m_ContextList.m_Name.SetXY(80, 220);
	   
	m_DialogBack.Load(0x73D983B7);
	m_DialogBack.m_bMove = TRUE;
	m_DialogBack.m_Picture.m_x = 320 - m_DialogBack.m_Picture.m_Width / 2;
	m_DialogBack.m_Picture.m_y = 240;
	for (int i = 0; i < 10; i++)
	{
		m_ContextList.m_TextLineList[i].m_line.m_hFont = g_pMainState->m_hFont[0];
		m_ContextList.m_TextLineList[i].m_line.SetColor( RGB(255,255,255));
	}
	m_ContextList.m_Name.SetColor(RGB(240, 240, 240));
	m_ContextList.m_Name.SetXCenter(m_NameBack.GetX() + m_NameBack.GetWidth()/2);

	m_Cancel.SetXY(m_DialogBack.GetX()+m_DialogBack.GetWidth()-20,m_DialogBack.GetY()+3);
}

BOOL cInterfaceDialog::Free()
{
	m_DialogBack.Free();
	return TRUE;
}

void cInterfaceDialog::SetObj(string name)
{
	cObj* pObj = g_pMainState->FindObjByName(name);
	SetObj(pObj);
	
}
void cInterfaceDialog::SetObj(cObj* pObj)
{
	m_pObj = pObj;
	if (!pObj)
	{
		m_Head.Load(0);
		m_ContextList.m_Name.SetString("");
		return;
	}
	if (m_ContextList.m_Name.GetString() != pObj->m_TrueName.GetString())
	{
		m_ContextList.m_Name.SetString(pObj->m_TrueName.GetString());
		m_ContextList.m_Name.UpdateXCenter();
	}
	SetHead(m_pObj->GetIndex(POS_DIALOG));
}

BOOL cInterfaceDialog::Show(cCanvas* pCanvas)
{
	if (!m_NeedShow)return TRUE;
	pCanvas->Add(&m_Head, 1);
	pCanvas->Add(&m_DialogBack, 1, TRUE);

	if (m_ContextList.m_Name.GetString().size()>0)
	{
		pCanvas->Add(&m_NameBack, 1);
		pCanvas->Add(&m_ContextList.m_Name);
	}
	pCanvas->Add(&m_Cancel, 1);
	//��ʾ�Ի�	
	for (int i = 0; i < m_ContextList.m_LineNum; i++)
	{
		if (1 == m_ContextList.m_LineNum && 0 == m_ContextList.m_TextLineList[i].m_NumFace)
		{
			pCanvas->Add(&m_ContextList.m_TextLineList[i].m_line);
		}
		else
		{
			pCanvas->Add(&m_ContextList.m_TextLineList[i].m_line);
			//����б���,����ʾ����
			for (int j = 0; j < m_ContextList.m_TextLineList[i].m_NumFace; j++)
			{
				pCanvas->Add(&m_ContextList.m_TextLineList[i].m_Face[j], 1);
			}
		}
	}
	
		switch (m_NumOfSelect)
		{
		case 4:
			pCanvas->Add(&m_SelectT[3]);
		case 3:
			pCanvas->Add(&m_SelectT[2]);
		case 2:
			pCanvas->Add(&m_SelectT[1]);
		case 1:
			pCanvas->Add(&m_SelectT[0]);
			break;
		}
	
	return TRUE;
}

BOOL cInterfaceDialog::ProcessInput()
{
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}
	cInputDevice& Mouse = g_pMainState->Mouse;
	m_retSelect = -1;
	m_Select = -1;
	g_pMainState->m_Cursor.m_FollowMode = FALSE;
	ResetButton();

	if (m_NowMouseOnSelect > -1)
	{
		if (m_NowMouseOnSelect<m_SelectT.size())
			m_SelectT[m_NowMouseOnSelect].SetColor(RGB(255, 0, 0));
		m_NowMouseOnSelect = -1;
	}

	g_pMainState->SetMouseOnMap(FALSE);
	if (!isOn(g_xMouse, g_yMouse, m_DialogBack.m_PreShowRect))
	{
		Mouse.SetLock(MOUSE_LBUTTON); //�Ի�ʱ����������������
		if (m_NowMouseOnSelect > -1)//��֮ǰ���ֵ���ɫ������
		{
			if (m_NowMouseOnSelect<m_SelectT.size())
				m_SelectT[m_NowMouseOnSelect].SetColor(RGB(255, 0, 0));
			m_NowMouseOnSelect = -1;
		}
		return  TRUE;
	}
	if(CheckOn(&m_Cancel, 4))return TRUE;

	if (m_NumOfSelect)
	{
		int i;
		for ( i= 0; i < 4; i++)
		{
			if (isOn(g_xMouse, g_yMouse, m_SelectT[i].m_PreShowRect))
			{
				m_Select = 0;
				m_NowMouseOnSelect = i;
				m_SelectT[m_NowMouseOnSelect].SetColor(RGB(255, 255, 255));
				if (CheckL())return TRUE;
				break;
			}
		}
		
	}
	else
	{
		m_Select = 1;
		CheckL();
	}
	return TRUE;
}

void cInterfaceDialog::SetString(string str, string select1, string select2, string select3, string select4)
{
	if (str.size())
		m_ContextList.SetFaceText(str, g_pMainState->m_hFont[0], 470);
	
	m_NumOfSelect = 2;
	if ("" != select4)	m_NumOfSelect = 4;
	else
	if ("" != select3)	m_NumOfSelect = 3;
	else
	if ("" != select2)	m_NumOfSelect = 2;
	else
	if ("" != select1)	m_NumOfSelect = 1;
	else m_NumOfSelect = 0;
	switch (m_NumOfSelect)
	{
	case 4:
		m_SelectT[3].SetString(select4);
	case 3:
		m_SelectT[2].SetString(select3);
	case 2:
		m_SelectT[1].SetString(select2);
	case 1:
		m_SelectT[0].SetString(select1);
		break;
	}
	int xstart = m_DialogBack.GetX() + 10;
	 m_ContextList.SetPos(xstart, m_DialogBack.GetY()+25);
	 int h = m_ContextList.m_Height;
	 switch (m_NumOfSelect)
	 {
	 case 1:
		 m_SelectT[0].SetXY(xstart, m_DialogBack.GetY() + 35+h );
		 break;
	 case 2:
		 for (int i = 0; i < m_NumOfSelect; i++)
		 {
			 m_SelectT[i].SetXY(xstart, m_DialogBack.GetY() + 35 + h);
			 h += m_SelectT[i].GetHeight() + 20;
		 }
		 break;
	 case 3:
		 for (int i = 0; i < m_NumOfSelect; i++)
		 {
			 m_SelectT[i].SetXY(xstart, m_DialogBack.GetY() + 35+ h);
			 h += m_SelectT[i].GetHeight() + 15;
		 }
		 break;
	 case 4:
		 for (int i = 0; i < m_NumOfSelect; i++)
		 {
			 m_SelectT[i].SetXY(xstart, m_DialogBack.GetY() + 35 + h);
			 h += m_SelectT[i].GetHeight() + 10;
		 }
		 break;
	 }
	
	return;
}

void cInterfaceDialog::SetPet(cPet* p)
{
	DWORD idlist = p->GetID(POS_DIALOG);
	if (idlist)g_pMainState->m_Dialog.m_Head.Load(idlist);
	m_ContextList.m_Name.SetString(p->m_PetDataZZ.m_Name);
	int a = m_Head.m_Picture.GetDataID();
	m_Head.m_Picture.m_x = 320 - m_DialogBack.m_Picture.m_Width / 2 + m_Head.m_Picture.Spr.m_pDataOffsetList[a].FramX;
	m_Head.m_Picture.m_y = 240 - m_Head.m_Picture.m_Height + m_Head.m_Picture.Spr.m_pDataOffsetList[a].FramY;
}

BOOL cInterfaceDialog::CheckL()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))return FALSE;
		g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	m_Sleeptime = 3;
	if (m_bRetSelect)
	{	
		if (m_Select == 4)
			m_NowMouseOnSelect = 4;
		if (m_NowMouseOnSelect > -1)
		{
			OnOff(FALSE);
			m_bRetSelect = FALSE;
			m_retSelect = m_NowMouseOnSelect;
			
			if (m_pTask)
			{
				cTaskFather* pTaskFather = m_pTask;
				m_pTask = 0;
				if (m_retSelect > 4)m_retSelect = 4;
				pTaskFather->Process(m_retstage[m_retSelect]);
			}	
			return TRUE;
		}
	}
	switch (m_Select)
	{
	case 0://ѡ��
		if (-1 < m_NowMouseOnSelect)
		{
			if (m_NumOfSelect)
				m_pObj->m_ScriptSelect = m_NowMouseOnSelect;
			m_NumOfSelect = 0;
			OnOff(FALSE);
			if (m_pObj)
			{
				m_pObj->m_Script.Process(m_pObj);
			}			
		}
		return TRUE;
	case 1://��������λ��
		if (0 == m_NumOfSelect)
		{
			OnOff(FALSE);
			if (!g_pMainState->m_TriggerDialog.m_datalist.size())return FALSE;
			if (g_pMainState->m_TriggerDialog.m_datalist[0].m_Nowtext > -1)
			{
				return g_pMainState->m_TriggerDialog.Process();
			}
			else
			if (m_pObj)
			{
				m_pObj->m_Script.Process(m_pObj);
			}

		}
		return TRUE;
	case 4://ȡ��
		Close(); break;
	default:
		ERRBOX;
		break;
	}
	return TRUE;
	
}

void cInterfaceDialog::OnOff(BOOL bShow)
{
	if (m_NeedShow == bShow)return;
	g_pMainState->InterfaceSetShow(this, bShow);
	if (m_NeedShow)
	{
		Init();
		if (!m_Reset)ERRBOX;
		if (m_NowMouseOnSelect>-1 && m_NowMouseOnSelect<4) m_SelectT[m_NowMouseOnSelect].SetColor(RGB(255, 0, 0));
		m_NowMouseOnSelect = -1;
		m_Cancel.SetFrame(0);
	}
	else m_Reset = FALSE;
	g_pMainState->m_Canvas.SetRedraw(m_DialogBack.m_PreShowRect);
	g_pMainState->m_Canvas.SetRedraw(m_Head.m_PreShowRect);
}

void cInterfaceDialog::Reset()
{
	m_retSelect = -1;
	m_NumOfSelect = 0;
	//m_pObj = 0;
	m_pTask = 0;
	m_bRetSelect = FALSE;
	m_Reset = TRUE;
	//m_Head.Free();
	//m_ContextList.m_Name.SetString("");
}

void cInterfaceDialog::SetTrigger(cTaskFather* pTask,  int defaultret, int stage0, int stage1, int stage2, int stage3)
{
	m_pTask = pTask; 
	m_retstage[4] = defaultret;
	
	/*if (stage0 == -1)
	{
		m_retstage[0] = defaultret;
		m_retstage[1] = defaultret;
		m_retstage[2] = defaultret;
		m_retstage[3] = defaultret;
	}
	else*/
	{
		m_retstage[0] = stage0;
		m_retstage[1] = stage1;
		m_retstage[2] = stage2;
		m_retstage[3] = stage3;
	}
	m_bRetSelect = TRUE;
}

void cInterfaceDialog::SetHead(DWORD head)
{
	if (!head)
	{
		m_Head.m_NeedShow = FALSE;
		return;
	}
	m_Head.Load(head);
	m_Head.m_NeedShow = TRUE;
	m_Head.m_Picture.m_x = 320 - m_DialogBack.m_Picture.m_Width / 2+m_Head.m_Picture.Spr.m_pDataOffsetList[0].FramX;
	m_Head.m_Picture.m_y = 240 - m_Head.m_Picture.m_Height +m_Head.m_Picture.Spr.m_pDataOffsetList[0].FramY;
}

void cInterfaceDialog::Talk(string context, string name, DWORD head)
{
	Reset();
	m_ContextList.m_Name.SetString(name);
	SetString(context);
	SetHead(head);
	OnOff(TRUE);
}

void cInterfaceDialog::Talk(sTalkList* talk)
{
	Reset();
	cObj* pObj = 0;
	SetString(talk->context);
	m_ContextList.m_Name.SetString(talk->name);
	if (talk->name == "")
	{
		SetHead(0);
		OnOff(TRUE);
		return;
	}
		pObj = g_pMainState->FindObjByName(talk->name);	
		if (pObj)
		{
			SetHead(pObj->GetIndex(POS_DIALOG));
		}
		else
		{
			//����ͷ��
			for (UINT i = 0; i < 5; i++)
			{
				if (g_pMainState->m_FightList[i].isLive)
				{
					string name = g_pMainState->m_FightList[i].m_pData->m_TrueName.GetString();
					if (name == talk->name)
					{
						SetHead(g_pMainState->m_FightList[i].m_pData->GetIndex(POS_DIALOG));
						OnOff(TRUE);
						return;
					}
				}
			}
			//����ͷ��
			for (UINT i = 0; i < g_pMainState->m_Friend.m_FriendList.size(); i++)
			{
				sFriendList& list = g_pMainState->m_Friend.m_FriendList[i];
				if (list.m_Name == talk->name)
				{
					SetHead(list.GetDialogHead());
					OnOff(TRUE);
					return;
				}
			}
			//����ӵ�ͷ��
			for (UINT i = 0; i < g_pMainState->m_DialogList.size(); i++)
			{
				if (g_pMainState->m_DialogList[i].name == talk->name)
				{
					SetHead(g_pMainState->m_DialogList[i].idlist);
					OnOff(TRUE);
					return;
				}
			}
			SetHead(0);
		}
	

	OnOff(TRUE);
}



void cInterfaceDialog::Reset2()
{
	m_bRetSelect = FALSE;
	m_Reset = TRUE;
}

void cInterfaceDialog::Close()
{
	if (!m_NeedShow)return;
	if (m_pObj)
		m_pObj->m_Script.m_NowScript = 0;
	OnOff(FALSE);
	if (0 == g_pMainState->m_TriggerDialog.m_datalist.size())return;
	if (g_pMainState->m_TriggerDialog.m_datalist[0].m_Nowtext > -1)
	{
		g_pMainState->m_TriggerDialog.Process(0,TRUE);
	}
}

void cInterfaceDialog::InitQuestion()
{
	if (m_bInitQuestion)return;
	m_bInitQuestion = TRUE;

	ifstream File;
	ostringstream oss;
	oss << g_strMediaPath<<"����//���//";
	oss << "0.txt";
	File.open(oss.str());
	int num;
	File >> num;
	m_PoemList.resize(num);
	for (int i = 0; i < num; i++)
	{
		m_PoemList[i].Read(File);
	}
	File.close();
}

void cInterfaceDialog::Question()
{
	InitQuestion();
	GetQuestion(m_Question);
	switch (m_Question.select.size())
	{
	case 1:
		SetString(m_Question.title, m_Question.select[0]);
		break;
	case 2:
		SetString(m_Question.title, m_Question.select[0], m_Question.select[1]);
		break;
	case 3:
		SetString(m_Question.title, m_Question.select[0], m_Question.select[1], m_Question.select[2]);
		break;
	case 4:
		SetString(m_Question.title, m_Question.select[0], m_Question.select[1], m_Question.select[2], m_Question.select[3]);
		break;
	}
	
}

std::string cInterfaceDialog::GetTrueSelect()
{
	ostringstream oss;
	oss << "��ȷ����#R";
	oss << m_Question.select[m_Question.answer];
	oss << "#R";
	return oss.str();
}

void cInterfaceDialog::GetQuestion(sQuestion& quest,int selectnum)
{
	quest.answer = 0;
	quest.select.resize(selectnum);
	
	InitQuestion();
	int poemid = rand() % m_PoemList.size();
	sPoem& po= m_PoemList[poemid];
	sPoem* pPo;
	int type = rand() % 3;
	ostringstream oss;
	switch (type)
	{
	case 0://�����
		if (1)
		{

			int contextid = rand() % po.m_context.size();
			contextid -= 2;
			if (contextid < 0)contextid = 0;
		
			oss <<"#G"<< po.m_context[contextid] << "," << po.m_context[contextid + 1] << "#G��������ʫ?";
			quest.title = oss.str();
			quest.select[0] = po.m_title;
			if(selectnum>1)
			while (1)
			{
				AutoGetPoem(pPo);
				if (pPo->m_title != quest.select[0])
				{
					quest.select[1] = pPo->m_title;
					break;
				}
			}
			if (selectnum>2)
			while (1)
			{
				AutoGetPoem(pPo);
				if (pPo->m_title != quest.select[0])
				if (pPo->m_title != quest.select[1])
				{
					quest.select[2] = pPo->m_title;
					break;
				}
			}
			if (selectnum>3)
			while (1)
			{
				AutoGetPoem(pPo);
				if (pPo->m_title != quest.select[0])
				if (pPo->m_title != quest.select[1])
				if (pPo->m_title != quest.select[2])
				{
					quest.select[3] = pPo->m_title;
					break;
				}
			}	
		}
		break;
	case 1:
		if (1)
		{
			//if (rand() % 2)
			{
				int contextid = rand() % po.m_context.size();
				contextid -= 2;
				if (contextid < 0)contextid = 0;
				oss <<"#G"<< po.m_context[contextid] << "," << po.m_context[contextid + 1] << "#G��������˭?";
				quest.title = oss.str();
			}
			/*else
			{
				oss << po.m_title << "��������˭?";
				quest.title = oss.str();
			}*/
		
			quest.select[0] = po.m_name;
			if (selectnum>1)
			while (1)
			{
				AutoGetPoem(pPo);
				if (pPo->m_name != quest.select[0])
				{
					quest.select[1] = pPo->m_name;
					break;
				}
			}
			if (selectnum>2)
			while (1)
			{
				AutoGetPoem(pPo);
				if (pPo->m_name != quest.select[0])
				if (pPo->m_name != quest.select[1])
				{
					quest.select[2] = pPo->m_name;
					break;
				}
			}
			if (selectnum>3)
			while (1)
			{
				AutoGetPoem(pPo);
				if (pPo->m_name != quest.select[0])
				if (pPo->m_name != quest.select[1])
				if (pPo->m_name != quest.select[2])
				{
					quest.select[3] = pPo->m_name;
					break;
				}
			}
			
		}
		break;
	case 2://
		if (1)
		{
			int contextid = rand() % po.m_context.size();
			if (po.m_context.size() < 3)ERRBOX;//ʫ��������С��3
			contextid -= 2;
			if (contextid < 0)contextid = 0;

			oss <<"#G"<< po.m_context[contextid] << "," << po.m_context[contextid + 1] << "#G��һ����ʲô?";
			contextid += 2;


			quest.title = oss.str();
			quest.select[0] = po.m_context[contextid];

			if (selectnum > 1)
			{
				AutoGetPoem(pPo);
				quest.select[1] = pPo->m_context[rand() % (pPo->m_context.size())];
			}
			if (selectnum > 2)
			{
				AutoGetPoem(pPo);
				quest.select[2] = pPo->m_context[rand() % (pPo->m_context.size())];
			}
			if (selectnum > 3)
			{
				AutoGetPoem(pPo);
				quest.select[3] = pPo->m_context[rand() % (pPo->m_context.size())];
			
			}

		}
		break;
	default:
		ERRBOX;
		break;
	}
	quest.Auto();
}

void cInterfaceDialog::AutoGetPoem(sPoem*& out)
{
	out = &m_PoemList[rand() % m_PoemList.size()];
}

void cInterfaceDialog::TalkNObj(string context)
{//
	Reset();
	SetString(context);
	OnOff(TRUE);
}

void cInterfaceDialog::TalkObj(string context,cObj* p)
{//
	
	Reset();
	if (p)
	{
		SetObj(p);
	}
	SetString(context);
	OnOff(TRUE);
}
void cInterfaceFight::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;
	m_Describe.Init();
	m_TitleT.SetColor(RGB(255,255,255));
	m_SkillPreSelectT.SetColor(RGB(255, 255, 255));
	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;

	//�ٻ��ٻ���
	m_Button.Load(0x79D01E0E);
	m_Button.SetStatic(TRUE);
	m_PetBack.Load(0xE87F26E1);
	m_PetPage = 0;

	m_UpB.Load(0x08E2D199);
	m_UpB.SetStatic(TRUE);
	m_DownB.Load(0xC013B53B);
	m_DownB.SetStatic(TRUE);


	for (int i = 0; i < 2; i++)
	{
		m_PetSelectBack[i].Load(0xF151309F);
		m_PetSelectBack[i].SetStatic(TRUE);
		m_PetSelectBack[i].SetFrame(1);
		m_PetSelectBack[i].SetxOffset(30);
	}
	//����
	m_ItemSelectFrame.Load(0x6F88F494);
	m_ItemSelectFrame.m_NeedShow = FALSE;
	m_ItemBack.Load(0xE3AEF018);

	m_pFightState = g_pMainState->m_pFightState;
	m_NowFighterID =-1;
	m_Magic.Load(2208066685);
	m_Magic.SetStatic(TRUE);
	m_Item.Load(361218670);
	m_Item.SetStatic(TRUE);
	m_Defend.Load(2791494731);
	m_Defend.SetStatic(TRUE);
	m_Guard.Load(73900200);
	m_Guard.SetStatic(TRUE);
	m_Summon.Load(2732776514);
	m_Summon.SetStatic(TRUE);
	/*m_Auto.Load(4021614423);
	m_Auto.SetStatic(TRUE);*/
	m_Catch.Load(717963597);
	m_Catch.SetStatic(TRUE);
	m_Escape.Load(320881121);
	m_Escape.SetStatic(TRUE);
	m_Fabao.Load(781132167);
	m_Fabao.SetStatic(TRUE);
	m_Stunt.Load(949865717);
	m_Stunt.SetStatic(TRUE);
	m_Conduct.Load(2324308397);
	m_Conduct.SetStatic(TRUE);

	 m_BackMagic6.Load(0x2FD95E30);
	 m_BackMagic10.Load( 0x69823EE5);
	 m_BackMagic14.Load(0x0A8B7D87);
	 m_BackMagic20.Load(0x32F119A5);
	 m_BackMagic30.Load(0x4226BD41);
	 m_MagicBack.m_bStatic = TRUE;

	 m_MenuMode = FMENU_NORMAL;
	 m_TitleT.SetString("ѡ����");
	MoveMenu(550, 100);

}

BOOL cInterfaceFight::Show(cCanvas* pCanvas)
{
	if (!m_NeedShow)return TRUE;
	ShowMenu(pCanvas);
	switch (m_MenuMode)
	{
	case FMENU_NORMAL:
		break;
	case FMENU_MAGIC2:
		break;
	case FMENU_ITEM:
		ShowItem(pCanvas);
		break;
	case FMENU_GRAB:
		break;
	case FMENU_ITEM2:
		break;
	case FMENU_MAGIC:
		ShowMagic(pCanvas);
		break;
	case FMENU_PET:
		ShowPet();
		break;
	case FMENU_PROTECT:
		break;
	case FMENU_SPECIAL:
		ShowSpecial(pCanvas);
		break;
	case FMENU_SPECIAL2:
		break;
	/*case FMENU_CONDUCT:
		break;*/
	//case FMENU_FABAO:
	//	ShowItem(pCanvas);
	//	break;
	default:
		ERRBOX;
		break;
	}
	return TRUE;
}

BOOL cInterfaceFight::ProcessInput()
{
	if (m_Describe.m_NeedShow)m_Describe.OnOff(FALSE);
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}
	if (m_NowFighterID == -1)
	{
		AddTurn();
		if (m_NowFighterID == -1)
		{
			return FALSE;
		}
	}

	
	m_Select = -1;
	ResetButton();

	if (CheckA())return TRUE;
	if (CheckG())return TRUE;
	if (CheckT())return TRUE;
	if (CheckD())return TRUE;
	if (CheckQ())return TRUE;
	if (CheckW())return TRUE;
	if (CheckX())return TRUE;
	if (CheckE())return TRUE;
	if (CheckV())return TRUE;
	if (CheckS())return TRUE;
	if (!g_pMainState->GetLockMouse())return TRUE;

	g_pMainState->m_Accelator.ProcessAccelator();

	switch (m_MenuMode)
	{
	case FMENU_NORMAL:
		if (ProcessNormal())return TRUE;
		break;
	case FMENU_MAGIC2:
		if (ProcessMagic2())return TRUE;
		break;
	case FMENU_ITEM:
		if (ProcessItem())return TRUE;
		break;
	case FMENU_GRAB:
		if (ProcessGrab())return TRUE;
		break;
	case FMENU_ITEM2:
		if (ProcessItem2())return TRUE;
		break;
	case FMENU_MAGIC:
		if (ProcessMagic())return TRUE;
		break;
	case FMENU_PET:
		if (ProcessPet())return TRUE;
		break;
	case FMENU_PROTECT:
		if (ProcessProtect())return TRUE;
		break;
	case FMENU_SPECIAL:
		if (ProcessSpecial())return TRUE;
		break;
	/*case FMENU_CONDUCT:
		break;*/
		/*case FMENU_FABAO:
			if (ProcessItem())return TRUE;
			break;*/
	case FMENU_SPECIAL2:
		if (ProcessSpecial2())return TRUE;
		break;
	default:
		ERRBOX;
		break;
	}
	ProcessMenu();
	return TRUE;
}

BOOL cInterfaceFight::Move(int x, int y)
{
	m_xPos=x;
	m_yPos=y;
	if (m_NowFighterID>-1)
	UpdateMenu();
	return TRUE;
}

BOOL cInterfaceFight::UpdateMenu()
{
	m_Magic.m_NeedShow = TRUE;
	m_Stunt.m_NeedShow = TRUE;
	m_Fabao.m_NeedShow = TRUE;
	m_Conduct.m_NeedShow = TRUE;
	m_Item.m_NeedShow = TRUE;
	m_Defend.m_NeedShow = TRUE;
	m_Guard.m_NeedShow = TRUE;
	m_Summon.m_NeedShow = TRUE;
//	m_Auto.m_NeedShow = TRUE;
	m_Catch.m_NeedShow = TRUE;
	m_Escape.m_NeedShow = TRUE;
	cObj*  pTarget = g_pMainState->m_FightList[m_NowFighterID].m_pData;
	cPetData* pTargetData;
	pTarget->GetDateP(pTargetData);
	if (pTarget->GetType() == INDEXTYEP_PET)
	{
		m_Stunt.m_NeedShow = FALSE;
		m_Fabao.m_NeedShow = FALSE;
		m_Conduct.m_NeedShow = FALSE;
		m_Summon.m_NeedShow = FALSE;
	//	m_Auto.m_NeedShow = FALSE;
		m_Catch.m_NeedShow = FALSE;
	}
	//����
	if (pTargetData->m_FightProperty.cannotMagic)
	{
		m_Magic.m_NeedShow = FALSE;
	}
	//�ؼ�
	if (pTargetData->m_FightProperty.cannotSkill)
	{
		m_Stunt.m_NeedShow = FALSE;
	}
	m_Fabao.m_NeedShow = FALSE;
	MoveMenu(550, 100);
	
	return TRUE;
}

BOOL cInterfaceFight::InitMagic()
{
	m_TitleT.SetString("ѡ����");
	int skillid = g_pMainState->m_FightList[m_NowFighterID].m_SkillPreSelect.SkillID;
	if (skillid>-1)
	{
		sSkill* pSkill;
		if (skillid > 2000)
			pSkill = g_pMainState->m_SkillManager.GetPetSkill(skillid-2000);
		else
			pSkill = g_pMainState->m_SkillManager.GetSkill(skillid);
		string str2 = "Ĭ�Ϸ���";
		str2 += pSkill->m_Name;
		m_SkillPreSelectT.SetString(str2);	
	}
	else	m_SkillPreSelectT.SetString("Ĭ�Ϸ���:��");
	m_NumofSkill = 0;
	switch (g_pMainState->m_FightList[m_NowFighterID].m_pData->GetType())
	{
	case INDEXTYEP_CHARACTER:
		if (1)
		{
			cCharacter* p = (cCharacter*)g_pMainState->m_FightList[m_NowFighterID].m_pData->m_pIndex;
			m_NumofSkill = 0;
			for (int i = 0; i < p->m_PCData.m_SkillEnablelist.size(); i++)
			{
				if (p->m_PCData.m_SkillEnablelist[i]->m_SkillType>1 && p->m_PCData.m_SkillEnablelist[i]->m_SkillType<7)//�Ƿ�ս������
				{
					m_pSkillShow[m_NumofSkill] = p->m_PCData.m_SkillEnablelist[i];
					m_NumofSkill += 1;
				}
				if (m_NumofSkill==30)
					break;
			}
			for (int i = 0; i < p->m_PCData.m_NumofBBSkill; i++)
			{
				if (p->m_PCData.m_pPetSkill[i]->m_SkillType != 7)
				{
					m_pSkillShow[m_NumofSkill] = p->m_PCData.m_pPetSkill[i];
					m_NumofSkill += 1;
				}
				if (m_NumofSkill == 30)
					break;
			}
		}
		break;
	case INDEXTYEP_PET:
		if (1)
		{
			cPet* p = (cPet*)g_pMainState->m_FightList[m_NowFighterID].m_pData->m_pIndex;
			for (int i = 0; i < p->m_PetDataZZ.m_NumofBBSkill; i++)
			{
				if (p->m_PetDataZZ.m_pPetSkill[i]->m_SkillType != 7)
				{
					m_pSkillShow[m_NumofSkill] = p->m_PetDataZZ.m_pPetSkill[i];
					m_NumofSkill += 1;
				}
			}
		}
		break;
	default:
		break;
	}
	if (m_NumofSkill < 11)m_MagicBack.Set(&m_BackMagic10);
	else
	if (m_NumofSkill < 15)m_MagicBack.Set(&m_BackMagic14);
	else
	if (m_NumofSkill < 21)m_MagicBack.Set(&m_BackMagic20);
	else
	{
		m_MagicBack.Set(&m_BackMagic30);
		if (30==m_NumofSkill)
			m_NumofSkill = 30;
	}
	//���ؼ���ͷ��,�����ú�λ��
	for (int i = 0; i < m_NumofSkill; i++)
	{
		m_SkillHead[i].Load(m_pSkillShow[i]->m_BigHead);
	}
	MoveMagicBack(320 - m_MagicBack.m_pFile->GetWidth() / 2, 240 - m_MagicBack.m_pFile->GetHeight() / 2);
	return TRUE;
}

void cDescribe::Set(int xMouse, int yMouse, DWORD BigHead, int num, string* pTex, DWORD* colorlist, int yOffset)
{
	const int wid = 160;
	m_Num = num;
	//ȷ�����
	m_BigHead.Load(BigHead);
	m_Back.SetShowWidth(m_BigHead.m_Picture.m_Width + wid+3);
	//ȷ���߶�
	int a = m_BigHead.m_Picture.GetDataID();
	int& xOf = m_BigHead.m_Picture.Spr.m_pDataOffsetList[a].FramX;
	int& yOf = m_BigHead.m_Picture.Spr.m_pDataOffsetList[a].FramY;
	m_BigHead.SetXY(xOf, yOf);
	int xTextStart = m_BigHead.m_Picture.m_x - xOf + m_BigHead.m_Picture.m_Width + 2;;
	int h;
	int yTop = m_Back.m_y;

	for (int i = 0; i < m_Num; i++)
	{
		m_Text[i].LockWidth(wid);
		if(colorlist)
			m_Text[i].SetColor(colorlist[i]);
		else 
			m_Text[i].SetColor(m_Color[i]);
		m_Text[i].SetString(pTex[i]);
		m_Text[i].SetXY(xTextStart, yTop);
		h = m_Text[i].GetShowHeight();
		yTop += h + 10;
	}
	h = yTop - (m_Back.GetY());
	if (h < m_BigHead.m_Picture.m_Height + 2)
		h = m_BigHead.m_Picture.m_Height + 2;
	m_Back.SetShowHeight(h);
	//ȷ��λ��
	if (yMouse - m_Back.GetShowHeight() < yOffset && (yMouse + m_Back.GetShowHeight() + yOffset>480))
	{
		//����̫��,���Ǻ������
		{
			if (xMouse<320)
			xMouse += yOffset;
			else
				xMouse -= m_Back.GetShowWidth() + yOffset;
		}
		
	}
	else
	{
		//���ȷ����·�
		if (yMouse + m_Back.GetShowHeight() + yOffset>480)
		{
			yMouse -= m_Back.GetShowHeight() + yOffset;
			xMouse -= m_Back.GetShowWidth() / 2;
		}
		else
		{
			yMouse += yOffset;
			xMouse -= m_Back.GetShowWidth() / 2;
		}
	}
	
	if (yMouse < 0)yMouse = 0;
	if (xMouse < 0)xMouse = 0;
	if (yMouse + m_Back.GetShowHeight()>640)yMouse = 640 - m_Back.GetShowHeight();
	if (xMouse + m_Back.GetShowWidth()>640)xMouse = 640 - m_Back.GetShowWidth();
	m_Back.SetXY(xMouse,yMouse);
	m_BigHead.SetXY(m_Back.GetX()+xOf, m_Back.GetY()+yOf);
	xTextStart = m_BigHead.m_Picture.m_x - xOf + m_BigHead.m_Picture.m_Width + 2;;
	yTop = m_Back.m_y;
	for (int i = 0; i < m_Num; i++)
	{
		m_Text[i].SetXY(xTextStart, yTop);
		h = m_Text[i].GetHeight();
		yTop += h + 10;
	}
	
}

BOOL cDescribe::Show(cCanvas* pCanvas)
{
	if (!m_NeedShow)return TRUE;
	pCanvas->Add(&m_Back, 1);
	pCanvas->Add(&m_BigHead, 1);
	for (int i = 0; i < m_Num; i++)
		pCanvas->Add(&m_Text[i]);
	return TRUE;
}
CWFile cDescribe::m_DescribeBack;
void cDescribe::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;

	m_Color[10];
	m_Color[2] = RGB(250, 250, 0);
	m_Color[0] = RGB(250, 250, 0);
	m_Color[1] = RGB(220, 220, 220);
	for (int i = 3; i < 10; i++)
	{
		m_Color[i] = RGB(0, 128, 0);
	}

	if (!m_DescribeBack.m_NowID)
	{
		m_DescribeBack.Load(0x1D0C0B23);
	}
	m_Back.Set(&m_DescribeBack);
		
		;
	m_Back.SetxOffset2(2);
	m_Back.SetyOffset2(2);
	m_BigHead.SetStatic(TRUE);
	
	for (int i = 1; i < 10; i++)
		m_Text[i].m_hFont = g_pMainState->m_hFont[0];
	m_Text[0].m_hFont = g_pMainState->m_hFont[2];
}

BOOL cDescribe::CheckL()
{
	if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
	{
		g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	}
	else return FALSE;
	m_Sleeptime = 3;
	switch (m_Select)
	{
	case 0:
		break;
	}
	return TRUE;
}

void cDescribe::OnOff(BOOL bonoff)
{
	if (m_NeedShow == bonoff)
	{
		if (m_NeedShow)
		{
			g_pMainState->m_Canvas.SetRedraw(m_Back.m_PreShowRect);
		}
		return;
	}
	g_pMainState->InterfaceSetShow(this, bonoff);
	if (m_NeedShow)
	{
		Init();
	}
	else
	{
		for (int i = 0; i < 10;i++)
		m_Text[i].SetString("");
	}
	g_pMainState->m_Canvas.SetRedraw(m_Back.m_PreShowRect);
}


BOOL cInterfaceFight::AddTurn()
{
	if (m_NowFighterID>-1)
		m_pFightState->m_pList[m_NowFighterID]->m_pData->m_TrueName.SetColor(RGB(0, 255, 0));
	while (1)
	{
		switch (m_NowFighterID)
		{
		case -1:
			m_NowFighterID = 0;
			break;
		case 0:
			m_NowFighterID = 5;
			break;
		case 1:
			m_NowFighterID = 6;
			break;
		case 2:			
			m_NowFighterID = 7;
			break;
		case 3:			
			m_NowFighterID = 8;
			break;
		case 4:
			m_NowFighterID = 9;
			break;
		case 5:
			m_NowFighterID = 1;
			break;
		case 6:
			m_NowFighterID = 2;
			break;
		case 7:
			m_NowFighterID = 3;
			break;
		case 8:
			m_NowFighterID = 4;
			break;
		case 9:
			if (1)
			{
				m_pFightState->m_turn = 1;
				m_NowFighterID = -1;
				if (m_pObjSelect)
				{
					m_pObjSelect->m_TrueName.SetColor(RGB(0, 255, 0));
					m_pObjSelect->SetShine(FALSE);

				}
				return TRUE;
			}
			break;
		}
		if (CheckTurn(m_NowFighterID))
		{
			if (g_StateType == STATE_FIGHT)
			{
				UpdateMenu();
				m_pFightState->m_pList[m_NowFighterID]->m_pData->m_TrueName.SetColor(RGB(139, 0, 255));
				g_pMainState->m_PCHead.SetFighter(m_NowFighterID);
			}
			return TRUE;
		}
	}
	return TRUE;
}

BOOL cInterfaceFight::CheckTurn(int ID)
{
	if (!m_bCanControl[ID])return FALSE;
	if (m_pFightState->m_pList[ID]->isLive)//����
//	if (!m_pFightState->m_pList[ID]->m_bDie)//���� //����һ������ѡ������
	{
		cPetData* pTargetData;
		m_pFightState->m_pList[ID]->m_pData->GetDateP(pTargetData);
		//if (!pTargetData->m_Controler.CheckHero())return FALSE;
		if (m_pFightState->m_pList[ID]->m_WorkType > -1)return FALSE;

	//	m_pFightState->m_pList[ID]->m_pData->GetDateP(pTargetData);
		if (!pTargetData->m_FightProperty.chaos)
		{

			if (!pTargetData->m_FightProperty.cannotAll)//�����ж�
			//if (pTargetData->m_Controler.CheckHero())
				return TRUE;
		}
	}
	return FALSE;
}

BOOL cInterfaceFight::ProcessItem()
{
	if (m_MoveBack)
	{
		m_Select = 17;
		if (!CheckL())m_MoveBack = FALSE;
		return TRUE;
	}
	if (CheckRButton())return TRUE;
	if (CheckOn(&m_Cancel, 100))return TRUE;
	m_ItemSelect = g_pMainState->m_ItemSelect.Process(g_xMouse, g_yMouse);
	if (m_ItemSelect > -1)
	{
		m_Select = 12;
		if (CheckL())
		{
			g_pMainState->m_ItemSelect.m_Describe.OnOff(FALSE);
			g_pMainState->m_InterfaceItem.m_SelectPre = -1;
			return TRUE;
		}
	}
	m_Select = 16;
	CheckL();
	return TRUE;
	//
	//m_ItemSelect = -1;
	
	//if (!isOn(g_xMouse, g_yMouse, m_ItemBack.m_PreShowRect))return FALSE;
	//g_pMainState->SetMouseOnMap(FALSE);
	//
	//if (isOn(g_xMouse, g_yMouse, m_ItemRect))
	//{
	//	m_ItemSelect = (g_xMouse - m_ItemRect.left) / 51 + ((g_yMouse - m_ItemRect.top) / 51) * 5;
	//}
	//if (m_ItemSelect > -1 && m_ItemCanUse[m_ItemSelect])
	//{
	//	m_ItemSelect;
	//	int xpos, ypos;

	//	cObj* pObj = g_pMainState->m_pFightState->m_pList[m_NowFighterID]->m_pData;

	//	if (INDEXTYEP_PET == pObj->m_IndexType)
	//	{
	//		cPetData* pPet;
	//		pObj->GetDateP(pPet);
	//		if (pPet->m_Controler.CheckBB())
	//			pObj = g_pMainState->m_FightList[m_NowFighterID - 5].m_pData;
	//		else return FALSE;
	//	}
	//	cCharacter* pc = (cCharacter*)pObj->m_pIndex;
	//	sItem2* pItem = pc->m_PCData.GetItem(m_ItemSelect);
	//	xpos = m_ItemHead[m_ItemSelect].GetX();
	//	ypos = m_ItemHead[m_ItemSelect].GetY();
	//	if (!pItem->GetNum())return TRUE;
	//		g_pMainState->m_ItemManage.SetItemDescribe(
	//			xpos, ypos, m_Describe, pItem
	//			);

	//	


	//		if (m_Describe.m_NeedShow)
	//		{
	//			CWFile* itemfirst = &m_ItemHead[m_ItemSelect];

	//			m_ItemSelectFrame.m_NeedShow = TRUE;
	//			m_ItemSelectFrame.m_Picture.m_x = itemfirst->GetX();
	//			m_ItemSelectFrame.m_Picture.m_y = itemfirst->GetY();
	//		}

	//	m_Select = 12;
	//	if (CheckL())
	//	{
	//		m_Describe.OnOff(FALSE);
	//		g_pMainState->m_InterfaceItem.m_SelectPre = -1;
	//		return TRUE;
	//	}
	//}
	//else
	//{
	//	m_ItemSelect = -1;
	//	m_ItemSelectFrame.m_NeedShow = FALSE;
	//	g_pMainState->m_InterfaceItem.m_MouseOnMark.m_NeedShow = FALSE;
	//	g_pMainState->m_InterfaceItem.m_SelectPre = -1;
	//	if (m_Describe.m_NeedShow)
	//		m_Describe.OnOff(FALSE);
	//	
	//	
	//	
	//}
	//if(CheckRButton())return TRUE;
	//return FALSE;
	//����ڿհ״�,׼���ƶ�
	
}
//2
void cInterfaceFight::InitItem(int offset)
{	
	cObj* pObj = g_pMainState->m_FightList[m_NowFighterID].m_pData;
	if (INDEXTYEP_PET == pObj->m_IndexType)
	{
		cPetData* pPet;
		pObj->GetDateP(pPet);
		if (pPet->m_Controler.CheckBB())
			pObj = g_pMainState->m_FightList[m_NowFighterID - 5].m_pData;
		else return;
	}
	cCharacter* pc = (cCharacter*)pObj->m_pIndex;

	g_pMainState->m_ItemSelect.Update(pc);
	MoveItem(320-m_ItemBack.GetWidth()/2, 240 - m_ItemBack.GetHeight() / 2);
	//g_pMainState->m_ItemSelect.Move(m_ItemBack.GetX()+19, m_ItemBack.GetY()+34);
	//int itemoffset;
	m_TitleT.SetString("ѡ����Ʒ");
	m_ItemBack.m_bMove = TRUE;
	//cObj* pObj = g_pMainState->m_FightList[m_NowFighterID].m_pData;
	//if (INDEXTYEP_PET == pObj->m_IndexType)
	//{
	//	cPetData* pPet;
	//	pObj->GetDateP(pPet);
	//	if (pPet->m_Controler.CheckBB())
	//		pObj = g_pMainState->m_FightList[m_NowFighterID - 5].m_pData;
	//	else return;
	//}
	//cCharacter* pc = (cCharacter*)pObj->m_pIndex;
	//for (int i = 0; i < 20; i++)
	//{
	//	itemoffset = i + offset;
	//	m_ItemCanUse[i] = FALSE;
	//	sItem2& Item = pc->m_PCData.m_Item[itemoffset];
	//	if (Item.GetNum())
	//	{
	//		switch (Item.GetType())
	//		{
	//		case 28:
	//			if (Item.GetID() == 11)
	//			{
	//				m_ItemCanUse[i] = TRUE;

	//				m_ItemHead[i].Load(Item.m_pItemData->m_SmallHead);
	//			}
	//			break;
	//		case 22://ҩƷ
	//		case 25://����
	//			if (1)
	//			{
	//				m_ItemCanUse[i] = TRUE;
	//				m_ItemHead[i].Load(Item.m_pItemData->m_SmallHead);
	//			}
	//			break;
	//		}
	//	}
	//	else
	//	{
	//		m_ItemHead[i].Free();
	//	}
	//}
	//m_Item.SetFrame(1);
	//MoveItem(320 - m_ItemBack.m_Picture.m_Width / 2, 240 - m_ItemBack.m_Picture.m_Height / 2);
}

BOOL cInterfaceFight::SetNowFight(int fight)
{
	m_NowFighterID = fight;
	UpdateMenu();
	
	return TRUE;
}


void cInterfaceFight::MoveMagicBack(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
	m_MagicBack.m_x = x;
	m_MagicBack.m_y = y;
	if (m_NumofSkill < 11)
	{
		//���ü���λ��
		for (int i = 0; i < m_NumofSkill; i++)
		{
			m_SkillHead[i].SetXY(m_MagicBack.m_x + 30 + 88 * (i % 2), m_MagicBack.m_y + 33 + 42 * (i / 2));
		}
	}
	else if (m_NumofSkill < 15)
	{
		//���ü���λ��
		for (int i = 0; i < m_NumofSkill; i++)
		{
			m_SkillHead[i].SetXY(m_MagicBack.m_x + 30 + 88 * (i % 2), m_MagicBack.m_y + 33 + 42 * (i / 2));
		}
	}
	else if (m_NumofSkill < 21)
	{
		//���ü���λ��
		for (int i = 0; i < m_NumofSkill; i++)
		{
			m_SkillHead[i].SetXY(m_MagicBack.m_x + 30 + 88 * (i % 2), m_MagicBack.m_y + 30 + 42 * (i / 2));
		}
	}
	else //30
	{
		for (int i = 0; i < m_NumofSkill; i++)
		{
			m_SkillHead[i].SetXY(m_MagicBack.m_x + 25 + 50 * (i % 3), m_MagicBack.m_y + 27 + 42 * (i / 3));
		}
	}
	m_Cancel.m_x = m_MagicBack.m_x + m_MagicBack.m_pFile->GetWidth() - 20;
	m_Cancel.m_y = m_MagicBack.m_y + 5;
	int xc = x + m_MagicBack.m_Width / 2;
	m_TitleT.SetY(y + 3);
	m_TitleT.SetXCenter(xc);
	m_TitleT.UpdateXCenter();
	m_SkillPreSelectT.SetY(y + m_MagicBack.m_pFile->GetHeight()-20);
	m_SkillPreSelectT.SetXCenter(xc);
	m_SkillPreSelectT.UpdateXCenter();
}

void cInterfaceFight::MoveMenu(int x, int y)
{
	m_xMenuPos = x;
	m_yMenuPos = y;
	int yOff = 21;
	int i = 0;
	if (m_Magic.m_NeedShow)
	{
		m_Magic.SetXY(x,y+yOff*i);
		i += 1;
	}
	if (m_Stunt.m_NeedShow)
	{
		m_Stunt.SetXY(x, y + yOff*i);  //�ؼ�
		i += 1;
	}
	if (m_Fabao.m_NeedShow)
	{
		m_Fabao.SetXY(x, y + yOff*i);
		i += 1;
	}
	if (m_Conduct.m_NeedShow)
	{
		m_Conduct.SetXY(x, y + yOff*i);
		i += 1;
	}
	if (m_Item.m_NeedShow)
	{
		m_Item.SetXY(x, y + yOff*i);  //����
		i += 1;
	}
	if (m_Defend.m_NeedShow)  //��
	{
		m_Defend.SetXY(x, y + yOff*i);
		i += 1;
	}
	if (m_Guard.m_NeedShow)  //����
	{
		m_Guard.SetXY(x, y + yOff*i);
		i += 1;
	}
	if (m_Summon.m_NeedShow)  //�ٻ�
	{
		m_Summon.SetXY(x, y + yOff*i); //�ٻ�
		i += 1;
	}
	//if (m_Auto.m_NeedShow)  //�Զ�
	//{
	//	m_Auto.SetXY(x, y + yOff*i);
	//	i += 1;
	//}
	if (m_Catch.m_NeedShow)  //ץ��
	{
		m_Catch.SetXY(x, y + yOff*i);
		i += 1;
	}
	if (m_Escape.m_NeedShow)  //����
	{
		m_Escape.SetXY(x, y + yOff*i);
		i += 1;
	}
	m_NumofButtonShow = i;
}

void cInterfaceFight::SelectMode(eFightMenu eFightMenu)
{
	//�Ȱ�֮ǰ��ͼ�����
	if (eFightMenu == m_MenuMode)return;

	cCanvas&  canvas = g_pMainState->m_Canvas;
	switch (m_MenuMode)
	{
	case FMENU_NORMAL:
		
		break;
	case FMENU_MAGIC2:
	case FMENU_GRAB:
	case FMENU_ITEM2:
	case FMENU_SPECIAL2:
	case FMENU_PROTECT:
		g_pMainState->m_Cursor.Set(g_pMainState->m_Cursor.m_Normal);
		break;
//	case FMENU_FABAO:
	case FMENU_ITEM:
		g_pMainState->m_ItemSelect.m_Describe.OnOff(FALSE);
		canvas.SetRedraw(m_ItemBack.m_PreShowRect);
		break;
	case FMENU_MAGIC:
	case FMENU_SPECIAL:
		canvas.SetRedraw(m_MagicBack.m_PreShowRect);
		break;
	case FMENU_PET:
		canvas.SetRedraw(m_PetBack.m_PreShowRect);
		break;
	default:
		ERRBOX;
		break;	
	}
	m_MenuMode = eFightMenu;
	switch (eFightMenu)
	{
	case FMENU_NORMAL:
		g_pMainState->m_Cursor.Set(g_pMainState->m_Cursor.m_Normal);
		break;
	case FMENU_MAGIC2:
		InitMagic2();
		break;
	case FMENU_ITEM:	
		InitItem(0);
		break;
	case FMENU_GRAB:
		g_pMainState->m_Cursor.Set(g_pMainState->m_Cursor.m_Grab);
		break;
	case FMENU_ITEM2:
		g_pMainState->m_Cursor.Set(g_pMainState->m_Cursor.m_Magic);
		break;
	case FMENU_MAGIC:
		InitMagic();
		break;
	case FMENU_PET:
		InitPet();
		
		break;
	case FMENU_PROTECT:
		g_pMainState->m_Cursor.Set(g_pMainState->m_Cursor.m_Guard);
		break;
	case FMENU_SPECIAL:	
		InitSpecial();
		break;
	case FMENU_SPECIAL2:
		g_pMainState->m_Cursor.Set(g_pMainState->m_Cursor.m_Magic);
		break;
	//case FMENU_CONDUCT:
		
		break;
	default:
		ERRBOX;
		break;
	}
}

void cInterfaceFight::SwitchMode(eFightMenu eFightMenu)
{
	if (m_MenuMode == eFightMenu)
		SelectMode(FMENU_NORMAL);
	else
	{
		m_Cancel.SetFrame(0);
		SelectMode(eFightMenu);
	}
	if (m_Describe.m_NeedShow)
	{
		m_Describe.OnOff(FALSE);
	}
}

BOOL cInterfaceFight::CheckQ()
{
	//�Զ�����
	if (g_pMainState->Keyboard.GetKeyState(DIK_Q))
	{
		g_pMainState->Keyboard.SetLock(DIK_Q);
		sFightList* figher = m_pFightState->m_pList[m_NowFighterID];
		int id = figher->m_SkillPreSelect.SkillID;
		if (-1 == id)return TRUE;

		sSkill* skill = g_pMainState->m_SkillManager.GetSkill(id);
		int autoSelect = g_pMainState->m_AI.AutoFindTarget(m_NowFighterID, skill->m_SkillType);
		if (-1 == autoSelect)return TRUE;

		figher->m_SkillSelect.Lv = figher->m_SkillPreSelect.Lv;
		figher->m_SkillSelect.SkillID = figher->m_SkillPreSelect.SkillID;
		figher->m_SkillSelect.TargetID = autoSelect;
		figher->m_WorkType = WORKTYPE_MAGIC;
		SelectMode(FMENU_NORMAL);
		AddTurn();
		return TRUE;
	}
	return FALSE;
}
BOOL cInterfaceFight::CheckD()
{
	if (g_pMainState->Keyboard.GetKeyState(DIK_D))//��
	{
		g_pMainState->Keyboard.SetLock(DIK_D);
		Def();
		return TRUE;
	}
	return FALSE;
}
void cInterfaceFight::Def()
{
	g_pMainState->m_AI.m_AttackAI.Def(m_NowFighterID);
	SelectMode(FMENU_NORMAL);
	AddTurn();
}
BOOL cInterfaceFight::CheckT()
{
	if (g_pMainState->Keyboard.GetKeyState(DIK_T))//����
	{
		g_pMainState->Keyboard.SetLock(DIK_T);
		SwitchMode(FMENU_PROTECT);
		return TRUE;
	}
	return FALSE;
}

BOOL cInterfaceFight::CheckA()
{
	if (g_pMainState->Keyboard.GetKeyState(DIK_A))
	{
		g_pMainState->Keyboard.SetLock(DIK_A);
		int autoSelect = g_pMainState->m_AI.AutoFindTarget(m_NowFighterID, 4);
		if (-1 != autoSelect)
		{
			m_pFightState->m_pList[m_NowFighterID]->m_WorkType = WORKTYPE_ATTACK;
			m_pFightState->m_pList[m_NowFighterID]->m_SkillSelect.Lv = 0;
			m_pFightState->m_pList[m_NowFighterID]->m_SkillSelect.SkillID = 1000;
			m_pFightState->m_pList[m_NowFighterID]->targetID = autoSelect;
			m_pFightState->m_pList[m_NowFighterID]->m_SkillSelect.TargetID = autoSelect;
		}
		AddTurn();
		SelectMode(FMENU_NORMAL);
		return TRUE;
	}
	return FALSE;
}

BOOL cInterfaceFight::CheckG()
{
	if (g_pMainState->Keyboard.GetKeyState(DIK_G))//ץ��
	{
		g_pMainState->Keyboard.SetLock(DIK_G);
		SwitchMode(FMENU_GRAB);
		return TRUE;
	}
	return FALSE;
}

BOOL cInterfaceFight::CheckW()
{
	//�Ƿ���ʾ��������
	if (g_pMainState->Keyboard.GetKeyState(DIK_W))
	{
		g_pMainState->Keyboard.SetLock(DIK_W);
		SwitchMode(FMENU_MAGIC);
		return TRUE;
	}
	return FALSE;
}
BOOL cInterfaceFight::CheckX()
{
	//�Ƿ���ʾ��������
	if (g_pMainState->Keyboard.GetKeyState(DIK_X))
	{
		g_pMainState->Keyboard.SetLock(DIK_X);
		SwitchMode(FMENU_SPECIAL);
		return TRUE;
	}
	return FALSE;
}
BOOL cInterfaceFight::CheckE()
{
	//����ģʽ
	if (g_pMainState->Keyboard.GetKeyState(DIK_E))
	{
		g_pMainState->Keyboard.SetLock(DIK_E);
		SwitchMode(FMENU_ITEM);
		return TRUE;
	}
	return FALSE;
}
BOOL cInterfaceFight::CheckV()
{
	//ָ��
	if (g_pMainState->Keyboard.GetKeyState(DIK_V))
	{
		g_pMainState->Keyboard.SetLock(DIK_V);
		m_InterfaceConduct.OnOff(!m_InterfaceConduct.m_NeedShow);
	//	SwitchMode(FMENU_CONDUCT);
		return TRUE;
	}
	return FALSE;
}
BOOL cInterfaceFight::CheckRButton()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	if (m_Describe.m_NeedShow)
	{
		m_Describe.OnOff(FALSE);
		g_pMainState->m_InterfaceItem.m_SelectPre = -1;
	}
	switch (m_MenuMode)
	{
	case FMENU_NORMAL:
		break;
	case FMENU_MAGIC2:
	case FMENU_ITEM2:
	case FMENU_PROTECT:
	case FMENU_GRAB:
		SelectMode(FMENU_NORMAL);
		break;
	case FMENU_ITEM:
		if (-1==m_ItemSelect)
		{
			SelectMode(FMENU_NORMAL);
		}
		else
		{
			g_pMainState->m_FightList[m_NowFighterID].m_ItemSelectID = m_ItemSelect;
			g_pMainState->m_FightList[m_NowFighterID].m_autoItemSelectID = m_ItemSelect;
			SelectMode(FMENU_ITEM2);
		}
		break;
	/*case FMENU_FABAO:
		if (-1 == m_ItemSelect)
		{
			SelectMode(FMENU_NORMAL);
		}
		else
		{
			g_pMainState->m_FightList[m_NowFighterID].m_ItemSelectID = m_ItemSelect + 49;
			g_pMainState->m_FightList[m_NowFighterID].m_autoItemSelectID = m_ItemSelect + 49;
			SelectMode(FMENU_ITEM2);
		}
		break;*/
	case FMENU_MAGIC:
		switch (m_Select)
		{
		case 14:
			if (1)
			{
				cCharacter* pc = (cCharacter *)m_pFightState->m_pList[m_NowFighterID]->m_pData->m_pIndex;
			
				if (m_pSkillSelect->m_Owner == -1)
				{
					m_pFightState->m_pList[m_NowFighterID]->m_SkillPreSelect.SkillID = m_pSkillSelect->m_Id + 2000;
					m_pFightState->m_pList[m_NowFighterID]->m_SkillPreSelect.Lv = 0;
				}
				else
				{
					m_pFightState->m_pList[m_NowFighterID]->m_SkillPreSelect.SkillID = m_pSkillSelect->m_Id;
					m_pFightState->m_pList[m_NowFighterID]->m_SkillPreSelect.Lv = pc->m_PCData.m_MengPaiSkillLV[m_pSkillSelect->m_Owner];
				}
				InitMagic2();
			}
			break;
		case 15:
			SelectMode(FMENU_NORMAL);
			break;
		}
		break;
	case FMENU_PET:
		SelectMode(FMENU_NORMAL);
		break;
	case FMENU_SPECIAL:
		if (-1== m_Select)
		{
			SelectMode(FMENU_NORMAL);
		}
		else
		{
			m_SpecialSelect = 2000 + m_Select - 101;
			SelectMode(FMENU_SPECIAL2);
		}
		break;
	case  FMENU_SPECIAL2:
		SelectMode(FMENU_SPECIAL);
		break;
	default:
		ERRBOX;
		break;
	}
	return TRUE;
}

BOOL cInterfaceFight::ProcessMenu()
{
	if (m_MoveBack)
	{
		m_Select = 17;
		if (!CheckL())m_MoveBack = FALSE;
		return TRUE;
	}
	//����ڲ˵���
	if (!(g_xMouse > m_xMenuPos-7 && g_xMouse < m_xMenuPos + 65 && g_yMouse>m_yMenuPos&&g_yMouse < m_yMenuPos + 231))
		return FALSE;
	g_pMainState->SetMouseOnMap(FALSE);
	
	if (CheckOn(&m_Magic, 0))return TRUE;
	if (CheckOn(&m_Item, 1))return TRUE;
	if (CheckOn(&m_Defend, 2))return TRUE;
	if (CheckOn(&m_Escape, 3))return TRUE;
	if (CheckOn(&m_Guard, 4))return TRUE;
	if (m_pFightState->m_pList[m_NowFighterID]->m_pData->GetType() == INDEXTYEP_CHARACTER)
	{
		if (CheckOn(&m_Fabao, 5))return TRUE;
		if (CheckOn(&m_Stunt, 6))return TRUE;
		if (CheckOn(&m_Conduct, 7))return TRUE;
		if (CheckOn(&m_Summon, 8))return TRUE;
	//	if (CheckOn(&m_Auto, 9))return TRUE;
		if (CheckOn(&m_Catch, 10))return TRUE;
	}

	//����ڿհ״�,׼���ƶ�
	m_Select = 16;
	CheckL();
	return TRUE;
}

BOOL cInterfaceFight::CheckL()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
		return FALSE;
	m_Sleeptime = 3;
	if(m_Describe.m_NeedShow)
		m_Describe.OnOff(FALSE);
	g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	switch (m_Select)
	{
	case 0://����
		SwitchMode(FMENU_MAGIC);
		m_Magic.SetFrame(1);
		break;
	case 1://����
		SwitchMode(FMENU_ITEM);
		m_Item.SetFrame(1);
		break;
	case 2://��
		Def();
		m_Defend.SetFrame(1);
		break;
	case 3://��
		g_pMainState->m_AI.m_AttackAI.Escape(m_NowFighterID);
		m_Escape.SetFrame(1);
		AddTurn();
		break;
	case 4://����
		Guard();
		m_Guard.SetFrame(1);
		break;
	//case 5://����
	//	SwitchMode(FMENU_FABAO);
	//	m_Fabao.SetFrame(1);
	//	break;
	case 6://�ؼ�
		SwitchMode(FMENU_SPECIAL);
		m_Stunt.SetFrame(1);
		break;
	case 7://ָ��
		SwitchMode(FMENU_NORMAL);
		m_InterfaceConduct.OnOff(!m_InterfaceConduct.m_NeedShow);
		m_Conduct.SetFrame(1);
		break;
	case 8://�ٻ�
		SwitchMode(FMENU_PET);
		m_Summon.SetFrame(1);
		break;
	case 10://ץ��
		SwitchMode(FMENU_GRAB);
		m_Catch.SetFrame(1);
		break;
	case 12:
		g_pMainState->m_FightList[m_NowFighterID].m_ItemSelectID = m_ItemSelect;
		SelectMode(FMENU_ITEM2);
		break;
	case 14:
		InitMagic2();
		break;
	case 16://׼���ƶ�
		m_OldxMouse = g_xMouse;
		m_OldyMouse = g_yMouse;
		g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
		m_MoveBack = TRUE;
		break;
	case 17://�ƶ�
		if (1)
		{
			m_MagicBack.m_bMove = TRUE;
			g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
			int xn;
			int yn;
			if (m_MenuMode == FMENU_NORMAL)
			{
				xn = m_xMenuPos - m_OldxMouse + g_xMouse;
				yn = m_yMenuPos - m_OldyMouse + g_yMouse;
			}
			else
			{
				 xn = m_xPos - m_OldxMouse + g_xMouse;
				 yn = m_yPos - m_OldyMouse + g_yMouse;
			}
			switch (m_MenuMode)
			{
			case FMENU_NORMAL:
				MoveMenu(xn, yn);
				break;
			case FMENU_MAGIC2:
				break;
			case FMENU_ITEM:
				MoveItem(xn,yn);
				break;
			case FMENU_MAGIC:
				MoveMagicBack(xn, yn);
				break;
			case FMENU_PET:
				MovePet(xn,yn);
				break;
			case FMENU_PROTECT:
				break;
			case FMENU_SPECIAL:
				MoveSpecial(xn, yn);
				break;
			/*case FMENU_CONDUCT:
				break;*/
		/*	case FMENU_FABAO:
				break;*/
			default:
				ERRBOX;
				break;
			}
			m_OldxMouse = g_xMouse;
			m_OldyMouse = g_yMouse;
		}
		break;
	case 100: //�رմ���
		SelectMode(FMENU_NORMAL);
		break;
	case 101://�ؼ�ѡ��
	case 102:
	case 103:
	case 104:
	case 105:
	case 106:
		m_SpecialSelect =  m_Select-101;
		SelectMode(FMENU_SPECIAL2);
		break;
	default:
		ERRBOX;
		break;
	}
	return TRUE;
}

void cInterfaceFight::Guard()
{
	SwitchMode(FMENU_PROTECT);
}

void cInterfaceFight::MoveItem(int x,int y)
{
	m_xPos = x;
	m_yPos = y;
	m_ItemBack.SetXY(x, y);
	m_Cancel.SetXY(x + 272, y + 5);
	m_TitleT.SetXY(x+110,y+5);
	g_pMainState->m_ItemSelect.Move(x + 19, y + 34);
	/*for (int i = 0; i < 20; i++)
	{
	int xNum = (i) % 5;
	int yNum = (i) / 5;
	m_ItemHead[i].SetXY(xNum * 51 + 20 + x, yNum * 51 + 35 + y);
	}*/
	m_ItemRect.left = 20 + x;
	m_ItemRect.top = 35 + y;
	m_ItemRect.right = m_ItemRect.left + 255;
	m_ItemRect.bottom = m_ItemRect.top + 204;


}

BOOL cInterfaceFight::InitSpecial()
{
	m_TitleT.SetString("ѡ���ؼ�");
	m_MagicBack.Set( &m_BackMagic6);
	m_MagicBack.m_bMove = TRUE;
	cCharacter* pC = (cCharacter*)m_pFightState->m_pList[m_NowFighterID]->m_pData->m_pIndex;
	m_SpecialNum = 0;
	for (int i = 0; i<6;i++)
	if (pC->m_PCData.m_SpecialSkillList[i]>-1)
	{
		m_SpecialID[m_SpecialNum] = pC->m_PCData.m_SpecialSkillList[i];
		sSkill*  pSkill = g_pMainState->m_SkillManager.GetSpecialSkill(m_SpecialID[m_SpecialNum]);
		m_SpecialHead[m_SpecialNum].Load(pSkill->m_BigHead);
		m_SpecialNum += 1;
	}
	MoveSpecial(320-m_MagicBack.m_pFile->GetWidth()/2,240-m_MagicBack.m_pFile->GetHeight()/2);
	return TRUE;
}

void cInterfaceFight::InitPet()
{
	
	MovePet(320 - m_PetBack.m_Picture.m_Width / 2, 240 - m_PetBack.m_Picture.m_Height / 2);
}
void cInterfaceFight::MovePet(int x,int y)
{
	m_xPos = x;
	m_yPos = y;
	m_PetBack.m_Picture.m_x = x; 
	m_PetBack.m_Picture.m_y = y; 
	m_Button.m_Picture.m_x = x + 60;
	m_Button.m_Picture.m_y = y + 305;
	m_UpB.m_Picture.m_x = x + 130;
	m_UpB.m_Picture.m_y = y + 35;
	m_DownB.m_Picture.m_x = x + 130;
	m_DownB.m_Picture.m_y = y + 145;

	m_Cancel.m_x = x + 145;
	m_Cancel.m_y = y + 5;
	g_pMainState->m_Canvas.SetRedraw(m_PetBack.m_PreShowRect);
}
void cInterfaceFight::ShowPet()
{
	cCanvas* pCanvas = &g_pMainState->m_Canvas;


		pCanvas->Add(&m_PetBack, 1);
		pCanvas->Add(&m_Cancel, 1);

		cCharacter* pC = (cCharacter*)g_pMainState->m_FightList[m_NowFighterID].m_pData->m_pIndex;
		int kk = pC->m_PCData.m_NumofBB;
		if (!kk)return;
		if (-1 == m_PetIDSelect)
		{
			if (m_TruePetIDSelect > -1)
			{
				m_PetSelectBack[0].m_Picture.m_x = m_PetBack.m_Picture.m_x + 22;
				m_PetSelectBack[0].m_Picture.m_y = m_PetBack.m_Picture.m_y + 55 + (m_TruePetIDSelect - m_PetPage * 4) * 28;
				m_PetSelectBack[1].m_Picture.m_x = m_PetBack.m_Picture.m_x + 68;
				m_PetSelectBack[1].m_Picture.m_y = m_PetBack.m_Picture.m_y + 55 + (m_TruePetIDSelect - m_PetPage * 4) * 28;
				m_PetSelectBack[0].m_bMove = TRUE;
				m_PetSelectBack[1].m_bMove = TRUE;
				pCanvas->Add(&m_PetSelectBack[0], 1);
				pCanvas->Add(&m_PetSelectBack[1], 1);
			}
		}
		else
		{
			m_PetSelectBack[0].m_Picture.m_x = m_PetBack.m_Picture.m_x + 22;
			m_PetSelectBack[0].m_Picture.m_y = m_PetBack.m_Picture.m_y + 55 + (m_PetIDSelect - m_PetPage * 4) * 28;
			m_PetSelectBack[1].m_Picture.m_x = m_PetBack.m_Picture.m_x + 68;
			m_PetSelectBack[1].m_Picture.m_y = m_PetBack.m_Picture.m_y + 55 + (m_PetIDSelect - m_PetPage * 4) * 28;
			m_PetSelectBack[0].m_bMove = TRUE;
			m_PetSelectBack[1].m_bMove = TRUE;
			pCanvas->Add(&m_PetSelectBack[0], 1);
			pCanvas->Add(&m_PetSelectBack[1], 1);
		}
		pCanvas->Add(&m_UpB, 1);
		pCanvas->Add(&m_DownB, 1);

		if (kk > 4)
		{
			kk = 4;
		}
		for (int i = 0; i < kk; i++)
		{
			if (m_TruePetIDSelect == i)
				m_PetName[i].SetColor(RGB(255, 255, 0));
			else
				m_PetName[i].SetColor(RGB(0, 0, 0));
			m_PetName[i].SetXY(m_PetBack.m_Picture.m_x + 25, m_PetBack.m_Picture.m_y + 60 + (i)* 28);
			m_PetName[i].SetString(pC->m_PCData.m_pPetDataList[i + m_PetPage * 4]->m_PetDataZZ.m_Name);
			pCanvas->Add(&m_PetName[i]);
		}
		if (m_TruePetIDSelect>-1)
		{

			m_PetProPerty[0].SetColor(RGB(0, 0, 0));
			m_PetProPerty[0].SetXY(m_PetBack.m_Picture.m_x + 60, m_PetBack.m_Picture.m_y + 180);
			m_PetProPerty[0].SetString(pC->m_PCData.m_pPetDataList[m_TruePetIDSelect]->m_PetDataZZ.m_Name);
			pCanvas->Add(&m_PetProPerty[0]);
			ostringstream oss;
			oss << pC->m_PCData.m_pPetDataList[m_TruePetIDSelect]->m_PetDataZZ.m_LV;
			m_PetProPerty[1].SetXY(m_PetBack.m_Picture.m_x + 60, m_PetBack.m_Picture.m_y + 205);
			m_PetProPerty[1].SetString(oss.str());
			pCanvas->Add(&m_PetProPerty[1]);

			m_PetProPerty[2].SetXY(m_PetBack.m_Picture.m_x + 60, m_PetBack.m_Picture.m_y + 230);
			m_PetProPerty[2].SetString("*MAX*");
			pCanvas->Add(&m_PetProPerty[2]);

			oss.str("");
			oss << pC->m_PCData.m_pPetDataList[m_TruePetIDSelect]->m_PetDataZZ.m_HP;
			m_PetProPerty[3].SetXY(m_PetBack.m_Picture.m_x + 60, m_PetBack.m_Picture.m_y + 255);
			m_PetProPerty[3].SetString(oss.str());
			pCanvas->Add(&m_PetProPerty[3]);

			oss.str("");
			oss << pC->m_PCData.m_pPetDataList[m_TruePetIDSelect]->m_PetDataZZ.m_MP;
			m_PetProPerty[4].SetXY(m_PetBack.m_Picture.m_x + 60, m_PetBack.m_Picture.m_y + 280);
			m_PetProPerty[4].SetString(oss.str());
			pCanvas->Add(&m_PetProPerty[4]);
		}
		pCanvas->Add(&m_Button, 1);
		m_PetProPerty[5].SetColor(RGB(255, 255, 255));
		m_PetProPerty[5].SetXY(m_Button.m_Picture.m_x + 5, m_Button.m_Picture.m_y + 3);
		m_PetProPerty[5].SetString("��ս");
		pCanvas->Add(&m_PetProPerty[5]);
	
}

BOOL cInterfaceFight::ProcessPet()
{
	m_PetIDSelect = -1;
	if (m_MoveBack)
	{
		m_Select = 17;
		if (!CheckL())
			m_MoveBack = FALSE;
		g_pMainState->SetMouseOnMap(FALSE);
		return TRUE;
	}
	if (isOn(g_xMouse, g_yMouse, m_PetBack.m_PreShowRect))
	{
		g_pMainState->SetMouseOnMap(FALSE);

		if (CheckOn(&m_Cancel,100))return TRUE;
		int xB, yB, hB, wB;
		xB = m_PetBack.m_Picture.m_x;
		yB = m_PetBack.m_Picture.m_y;
		hB = m_PetBack.m_Picture.m_Height;
		wB = m_PetBack.m_Picture.m_Width;
		if (g_yMouse > yB + 22 &&
			g_yMouse< yB + 172 &&
			g_xMouse> xB + 25 &&
			g_xMouse < xB + 130)
		{
			m_PetIDSelect = (g_yMouse - (yB + 55)) / 28;
			cCharacter* pC = (cCharacter*)g_pMainState->m_FightList[m_NowFighterID].m_pData->m_pIndex;
			if (m_PetIDSelect>3)m_PetIDSelect = 3;
			if (pC->m_PCData.m_NumofBB <= m_PetIDSelect)
				m_PetIDSelect = pC->m_PCData.m_NumofBB - 1;
			if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
			{
				g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
				m_TruePetIDSelect = m_PetIDSelect;
			}
		}
		if (m_TruePetIDSelect > -1)
		{
			if (isOn(g_xMouse, g_yMouse, m_Button.m_PreShowRect))
			{
				if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
				{
					g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
					m_Button.SetFrame(1);
					g_pMainState->m_FightList[m_NowFighterID].m_SkillSelect.SkillID = 1004;
					g_pMainState->m_FightList[m_NowFighterID].m_SkillSelect.TargetID = m_NowFighterID + 5;
					g_pMainState->m_FightList[m_NowFighterID].m_SkillSelect.Lv = m_TruePetIDSelect;
					g_pMainState->m_FightList[m_NowFighterID].m_WorkType = WORKTYPE_SUMMON;
					SelectMode(FMENU_NORMAL);
					AddTurn();
					return TRUE;
				}
				else m_Button.SetFrame(2);
			}
			else m_Button.SetFrame(0);
		}
		else  m_Button.SetFrame(3);

		//����ڿհ״�,׼���ƶ�
		m_Select = 16;
		CheckL();
		if (CheckRButton())return TRUE;
	}
	return TRUE;
}



void cInterfaceFight::ShowMenu(cCanvas* pCanvas)
{
	pCanvas->Add(&m_Magic, 1);
	pCanvas->Add(&m_Item, 1);
	pCanvas->Add(&m_Defend, 1);
	pCanvas->Add(&m_Guard, 1);
	pCanvas->Add(&m_Summon, 1);
//	pCanvas->Add(&m_Auto, 1);
	pCanvas->Add(&m_Catch, 1);
	pCanvas->Add(&m_Fabao, 1);
	pCanvas->Add(&m_Escape, 1);
	pCanvas->Add(&m_Stunt, 1);
	pCanvas->Add(&m_Conduct, 1);
}

void cInterfaceFight::ShowItem(cCanvas* pCanvas)
{
	pCanvas->Add(&m_ItemBack, 1);
	pCanvas->Add(&m_TitleT);
	pCanvas->Add(&m_Cancel, 1);
	g_pMainState->m_ItemSelect.Show(pCanvas);
	/*
	for (int i = 0; i < 20; i++)
	{
		if (m_ItemCanUse[i])
		{
			pCanvas->Add(&m_ItemHead[i], 1);
		}
	}
	pCanvas->Add(&m_ItemSelectFrame, 1);*/
}

void cInterfaceFight::ShowMagic(cCanvas* pCanvas)
{
	pCanvas->Add(&m_MagicBack, 1);
	pCanvas->Add(&m_TitleT);
	pCanvas->Add(&m_SkillPreSelectT);
	pCanvas->Add(&m_Cancel, 1);
	for (int i = 0; i < m_NumofSkill; i++)
		pCanvas->Add(&m_SkillHead[i], 1);
}

BOOL cInterfaceFight::ProcessNormal()
{
	if (m_pObjSelect)
	if (g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))
	{
		g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
		g_pMainState->m_ChaKan.Set(m_pObjSelect);
		g_pMainState->m_ChaKan.OnOff(TRUE);
		return TRUE;
	}
	int Objselect;
	if (g_pMainState->Keyboard.GetKeyState(DIK_LCONTROL))
	{
		Objselect = g_pMainState->m_Cursor.SelectObj(m_NowFighterID, 9, m_pObjSelect);
	}
	else
		Objselect=g_pMainState->m_Cursor.SelectObj(m_NowFighterID, 4, m_pObjSelect);
	if (-1 == Objselect)return FALSE;

	m_pFightState->m_pList[m_NowFighterID]->targetID = Objselect;
	m_pFightState->m_pList[m_NowFighterID]->m_WorkType = WORKTYPE_ATTACK;
	m_pFightState->m_pList[m_NowFighterID]->m_SkillSelect.Lv = 0;
	m_pFightState->m_pList[m_NowFighterID]->m_SkillSelect.SkillID = 1000;
	m_pFightState->m_pList[m_NowFighterID]->m_SkillSelect.TargetID = Objselect;
	AddTurn();
	return TRUE;

}

BOOL cInterfaceFight::ProcessMagic2()
{
	if (CheckRButton())return TRUE;
	int Objselect = g_pMainState->m_Cursor.SelectObj(m_NowFighterID, m_pSkillSelect->m_SkillType,m_pObjSelect);
	if (-1 == Objselect)return FALSE;
	sFightList* pList = m_pFightState->m_pList[m_NowFighterID];
	cCharacter* pc = (cCharacter *)pList->m_pData->m_pIndex;
	pList->m_SkillSelect.Lv = pc->m_PCData.m_MengPaiSkillLV[m_pSkillSelect->m_Owner];
	if (m_pSkillSelect->m_Owner == -1)
	{
		pList->m_SkillSelect.SkillID = m_pSkillSelect->m_Id + 2000;
	}
	else
		pList->m_SkillSelect.SkillID = m_pSkillSelect->m_Id;
	
	pList->m_SkillSelect.TargetID = Objselect;
	pList->m_WorkType = WORKTYPE_MAGIC;

	m_pFightState->m_pList[Objselect]->m_pData->SetShine(FALSE);
	SelectMode(FMENU_NORMAL);
	m_pSkillSelect = 0;
	AddTurn();
	return TRUE;
}

BOOL cInterfaceFight::ProcessMagic()
{
	m_pSkillSelect = 0;

	if (m_MoveBack)
	{
		m_Select = 17;
		if (!CheckL())
			m_MoveBack = FALSE;
		g_pMainState->SetMouseOnMap(FALSE);
		return TRUE;
	}

	if (!isOn(g_xMouse, g_yMouse, m_MagicBack.m_PreShowRect))
	{
		m_MoveBack = FALSE;
		return TRUE;
	}
	g_pMainState->SetMouseOnMap(FALSE);

	if (CheckOn(&m_Cancel,100))return TRUE;

	int i=0;
	for (; i < m_NumofSkill; i++)
	{
		if (isOn(g_xMouse,g_yMouse, m_SkillHead[i].m_PreShowRect))
		{
			m_Select = 14;		
			string text[3];
			text[0] = m_pSkillShow[i]->m_Name;
			text[1] = m_pSkillShow[i]->m_Text;
			ostringstream oss;
			oss << "����:";
			oss << m_pSkillShow[i]->m_tCost;
			text[2] = oss.str();		
			m_Describe.Set(g_xMouse, g_yMouse, m_pSkillShow[i]->m_BigHead, 3, text);
			if (0==m_Describe.m_NeedShow)g_pMainState->InterfaceSetShow(&m_Describe, TRUE);
			m_pSkillSelect = m_pSkillShow[i];
			if (CheckL())return TRUE;
			if (CheckRButton())return TRUE;
			break;
		}
	}
	if (i == m_NumofSkill)
	{
		m_Select = 15;
		if (CheckRButton())return TRUE;

		if (m_Describe.m_NeedShow)
			m_Describe.OnOff(FALSE);
		//����ڿհ״�,׼���ƶ�
		m_Select = 16;
		CheckL();
	}
	return FALSE;
}

void cInterfaceFight::MoveSpecial(int x, int y)
{
	m_MagicBack.m_x = x;
	m_MagicBack.m_y = y;
	m_TitleT.SetXY(x+50,y+3);
	m_Cancel.SetXY(x + m_MagicBack.m_pFile->GetWidth() - 20, y+4);
	int xStart = x + 35;
	int yStart = y + 32;
	for (int i = 0; i < m_SpecialNum; i++)
	{
		int x1 = i % 2;
		int y1 = i / 2;
		m_SpecialHead[i].SetXY(xStart+x1*60,yStart+y1*60);
	}
}

void cInterfaceFight::ShowSpecial(cCanvas* pCanvas)
{
	pCanvas->Add(&m_MagicBack, 1, 1);
	pCanvas->Add(&m_Cancel, 1);
	pCanvas->Add(&m_TitleT);
	for (int i = 0; i < m_SpecialNum; i++)
		pCanvas->Add(&m_SpecialHead[i], 1);
}

BOOL cInterfaceFight::ProcessSpecial()
{
	if (!isOn(g_xMouse, g_yMouse, m_MagicBack.m_PreShowRect))return FALSE;
	g_pMainState->SetMouseOnMap(FALSE);
	if (CheckOn(&m_Cancel, 100))return TRUE;
	int i=0;
	for (; i < m_SpecialNum; i++)
	if (isOn(g_xMouse, g_yMouse, m_SpecialHead[i].m_PreShowRect))
	{
		sSkill* pSkill = g_pMainState->m_SkillManager.GetSpecialSkill(m_SpecialID[i]);
		string text[3];
		text[0] = pSkill->m_Name;
		text[1] = pSkill->m_Text;
		ostringstream oss;
		oss << "����:";
		oss << pSkill->m_tCost;
		text[2] = oss.str();
		m_Describe.Set(g_xMouse, g_yMouse, pSkill->m_BigHead, 3, text);
		g_pMainState->InterfaceSetShow(&m_Describe, TRUE);
		//cCharacter* pC = (cCharacter*)g_pMainState->m_pFightState->m_pList[m_NowFighterID]->m_pData->m_pIndex;
		m_pSkillSelect = g_pMainState->m_SkillManager.GetSpecialSkill(m_SpecialID[i]);// pC->m_PCData.m_SpecialSkillList[i]);
		m_Select = 101 + i;
		if(CheckL())return TRUE;
		break;
	}
	if (i == m_SpecialNum)
	{
		if (m_Describe.m_NeedShow)m_Describe.OnOff(FALSE);
	}
	CheckRButton();
	return TRUE;
}

BOOL cInterfaceFight::ProcessSpecial2()
{
	if (CheckRButton())return TRUE;
	int Objselect = g_pMainState->m_Cursor.SelectObj(m_NowFighterID, m_pSkillSelect->m_SkillType,m_pObjSelect);
	if (-1 == Objselect)return FALSE;
	cCharacter* pc = (cCharacter *)m_pFightState->m_pList[m_NowFighterID]->m_pData->m_pIndex;
	m_pFightState->m_pList[m_NowFighterID]->m_SkillSelect.Lv = 0;

	int skillid=-1;
	int num = m_SpecialSelect;
	for (int i = 0; i < 6; i++)
	{
		if (-1 != pc->m_PCData.m_SpecialSkillList[i])
		{
			num -= 1;
			if (num < 0)
			{
				skillid = pc->m_PCData.m_SpecialSkillList[i]+3000;
				break;
			}
		}
	}
	


	m_pFightState->m_pList[m_NowFighterID]->m_SkillSelect.SkillID = skillid;//pc->m_PCData.m_SpecialSkillList[m_SpecialSelect]+3000;
	m_pFightState->m_pList[m_NowFighterID]->m_SkillSelect.TargetID = Objselect;
	m_pFightState->m_pList[m_NowFighterID]->m_WorkType = WORKTYPE_MAGIC;
	m_pSkillSelect = 0;
	m_pFightState->m_pList[Objselect]->m_pData->SetShine(FALSE);
	SelectMode(FMENU_NORMAL);
	AddTurn();
	return TRUE;
}

BOOL cInterfaceFight::ProcessGrab()
{
	if (CheckRButton())return TRUE;
	int Objselect = g_pMainState->m_Cursor.SelectObj(m_NowFighterID, 4,m_pObjSelect);
	if (-1 != Objselect)
	{
		if (m_pFightState->m_pList[Objselect]->m_pData->GetType() != INDEXTYEP_PET)
			return TRUE;

		m_pFightState->m_pList[m_NowFighterID]->m_WorkType = WORKTYPE_CATCH;
		m_pFightState->m_pList[m_NowFighterID]->m_SkillSelect.Lv = 0;
		m_pFightState->m_pList[m_NowFighterID]->m_SkillSelect.SkillID = 1001;
		m_pFightState->m_pList[m_NowFighterID]->m_SkillSelect.TargetID = Objselect;
		m_pFightState->m_pList[m_NowFighterID]->targetID = Objselect;
		SelectMode(FMENU_NORMAL);
		m_pSkillSelect = 0;
		AddTurn();
		return TRUE;
	}
	else
	if (m_pObjSelect)
	{
		g_pMainState->m_Cursor.Set(g_pMainState->m_Cursor.m_Grab);
	}
	else  g_pMainState->m_Cursor.Set(g_pMainState->m_Cursor.m_No);
	return FALSE;
}

BOOL cInterfaceFight::ProcessItem2()
{
	if (CheckRButton())return TRUE;
	sItem2* pItem = g_pMainState->m_SkillManager.GetItem(m_NowFighterID);
	int selecttype;
	int  skillid;
	switch (pItem->GetType())
	{
	case 28:
		if (pItem->GetID() == 11)//Ǭ����
		{
			selecttype = 4;
			skillid = 1007;
		}
		else
		{
			selecttype = 3;
			skillid = 1003;
		}
		break;
	case 25:
		selecttype = 4;
		skillid = 1005;
		break;
	case 24: //����
		selecttype = pItem->m_pItemData->m_UseType;
		skillid = 1003;
		break;
	default:
		selecttype = 3;
		skillid = 1003;
		break;
	}
	int Objselect=g_pMainState->m_Cursor.SelectObj(m_NowFighterID, selecttype, m_pObjSelect);
	//int Objselect = SelectObj(m_NowFighterID, selecttype);
	if (-1 != Objselect)
	{
		m_pFightState->m_pList[m_NowFighterID]->m_SkillSelect.SkillID = skillid;
		m_pFightState->m_pList[m_NowFighterID]->m_SkillSelect.Lv = 0;
		m_pFightState->m_pList[m_NowFighterID]->m_SkillSelect.TargetID = Objselect;
		m_pFightState->m_pList[m_NowFighterID]->m_WorkType = WORKTYPE_MAGIC;
		SelectMode(FMENU_NORMAL);
		m_pSkillSelect = 0;
		AddTurn();
		return TRUE;
	}
	return FALSE;
}

BOOL cInterfaceFight::ProcessProtect()
{
	CheckRButton();
	int Objselect = g_pMainState->m_Cursor.SelectObj(m_NowFighterID, 9,m_pObjSelect);
	if (-1 != Objselect)
	{
		sBuff buf;
		buf.times = 1;
		g_pMainState->m_SkillManager.SetDateBuffUp(m_NowFighterID, Objselect, 1004,buf);
		SelectMode(FMENU_NORMAL);
		m_pSkillSelect = 0;
		g_pMainState->m_FightList[m_NowFighterID].m_WorkType = -2;
		AddTurn();
		return TRUE;
	}
	return FALSE;
}

BOOL cInterfaceFight::InitMagic2()
{
	g_pMainState->m_Cursor.Set(g_pMainState->m_Cursor.m_Magic);
	SelectMode(FMENU_MAGIC2);
	return TRUE;
}

void cInterfaceFight::OnOff(BOOL NeedShow)
{
	if (m_NeedShow == NeedShow)return;
	g_pMainState->InterfaceSetShow(this, NeedShow);
	if (m_NeedShow)
		Init();
	g_pMainState->m_Canvas.SetRedrawAll();
}

BOOL cInterfaceFight::CheckS()
{
	if (g_pMainState->Keyboard.GetKeyState(DIK_S))
	{
		g_pMainState->Keyboard.SetLock(DIK_S);
		sFightList* figher = m_pFightState->m_pList[m_NowFighterID];
		int id = figher->m_SkillPreSelect.SkillID;
		if (-1 == id)return TRUE;

		sSkill* skill = g_pMainState->m_SkillManager.GetSkill(id);
		m_pSkillSelect = skill;
		SelectMode(FMENU_MAGIC2);
		return TRUE;
	}
	return FALSE;
}

void cInterfaceFight::SetTeamControl(BOOL bControl)
{
	for (int i = 0; i < 10; i++)
	{
		m_bCanControl[i] = bControl;
	}
	m_bCanControl[0] = TRUE;
	m_bCanControl[5] = TRUE;
}

cInterfaceFight::cInterfaceFight()
{
	for (int i = 0; i < 20; i++)
		m_bCanControl[i] = FALSE;
}

BOOL cInterfaceFight::CheckControl(int id)
{
	return m_bCanControl[id];
}



void cInterfaceConduct::Init()
{
	if (m_bInit)return ;
	m_bInit = TRUE;
	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;

	m_Title.SetColor(RGB(255,255,255));
	m_Title.SetString("ָ��");
	m_Back.Load(0xFBEC073F);//0xCC7BC6C6);
	m_Back.SetStatic(TRUE);
	m_Buttonp.resize(7);
	m_tButtonp.resize(7);
	for (int i = 0; i < 7; i++)
	{
		m_Buttonp[i].Set(&g_pMainState->m_Button4);
		m_Buttonp[i].m_bStatic = TRUE;
		m_tButtonp[i].SetColor(RGB(255, 255, 255));
		m_tButtonp[i].SetString("ָ��Ŀ��");
	};
	m_tButtonp[6].SetString("����");
	m_tName.resize(7);
	for (int i = 0; i < 7; i++)
	{
		m_tName[i].SetColor(RGB(0,200,0));
	}
	m_tName[5].SetString("ȫ��");
	m_tName[6].SetString("ȫ��");

	m_tTargetName.resize(5);
	for (int i = 0; i < 5; i++)
	{
		m_tTargetName[i].SetColor(RGB(255,0,0));
	}
	
	Move(320 - m_Back.GetWidth() / 2, 240 - m_Back.GetHeight() / 2);
}

void cInterfaceConduct::Move(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
	m_Back.SetXY(x, y);
	m_Title.SetXCenter(m_Back.GetX() + 100);
	m_Title.SetY(m_yPos + 3);
	m_Title.UpdateXCenter();
	for (int i = 0; i < m_tName.size(); i++)
	{
		if (i < 5)
		{
			m_tName[i].SetXY(m_xPos + 15, m_yPos + 26 + (i + 2) * 30);
			m_Buttonp[i].SetXY(m_xPos + 68, m_yPos + 24 + (i + 2) * 30);
		}
		else
		{
			m_tName[i].SetXY(m_xPos + 15, m_yPos + 26 + (i -5) * 30);
			m_Buttonp[i].SetXY(m_xPos + 68, m_yPos + 24 + (i -5) * 30);
		}


		m_tButtonp[i].SetXCenter(m_Buttonp[i].GetX() + m_Buttonp[i].GetShowWidth() / 2);
		m_tButtonp[i].SetY(m_Buttonp[i].GetY() + 3);
		m_tButtonp[i].UpdateXCenter();
	}
	for (int i = 0; i < m_tTargetName.size(); i++)
	{
		m_tTargetName[i].SetXY(130+m_tName[i + 2].GetX(), m_tName[i].GetY());
	}
	m_Cancel.SetXY(m_Back.GetX() + m_Back.GetWidth() - 20, m_Back.GetY() + 3);
}

void cInterfaceConduct::OnOff(BOOL NeedShow)
{
	if (NeedShow)
	{
		Init();
		m_FighterNum = 0;
		for (int i = 0; i < 5; i++)
		{
			if (!g_pMainState->m_FightList[i].isLive)continue;
			m_FighterNum +=  1;
			m_tName[i].SetString(g_pMainState->m_FightList[i].m_pData->m_TrueName.GetString());
			if (-1 == g_pMainState->m_AI.m_AttackAI.m_FightTarget[i])
				m_Buttonp[i].SetFrame(0);
			else
				m_Buttonp[i].SetFrame(1);
		}
		if (m_FighterNum == 1)return;
		m_Buttonp[5].SetFrame(0);
		m_Buttonp[6].SetFrame(0);
		m_Mode = -1;
		Move(m_xPos, m_yPos);
	}
	g_pMainState->InterfaceSetShow(this, NeedShow);
	g_pMainState->m_Canvas.SetRedraw(m_Back.m_PreShowRect);

}

BOOL cInterfaceConduct::Show(cCanvas* pCanvas)
{
	if (m_Mode>-1)return TRUE;
	pCanvas->Add(&m_Back, 1, 1);
	pCanvas->Add(&m_Cancel, 1);
	pCanvas->Add(&m_Title);
	if (m_FighterNum > 1)
	{
		for (int i = 0; i <7; i++)
		{
			pCanvas->Add(&m_tName[i]);
			pCanvas->Add(&m_Buttonp[i],1);
			pCanvas->Add(&m_tButtonp[i]);
			if (i<5)
			if (m_Buttonp[i].GetFrame() == 1)
			{
				pCanvas->Add(&m_tTargetName[i]);
			}
		}
	}
	return TRUE;
}

BOOL cInterfaceConduct::CheckL()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	m_Sleeptime = 3;
	switch (m_Select)
	{
	case  10:
		OnOff(FALSE);
		break;
	case 16://�ƶ�
		if (1)
		{
			g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
			int xDel = g_xMouse - m_Prex;
			int yDel = g_yMouse - m_Prey;
			m_Prex = g_xMouse;
			m_Prey = g_yMouse;
			if (xDel != 0 || yDel != 0)
			{
				Move(m_xPos + xDel, m_yPos + yDel);
			}
			m_Sleeptime = 0;
		}
		return TRUE;
	case 17://׼���ƶ�
		g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
		m_Prex = g_xMouse;
		m_Prey = g_yMouse;
		m_bMoveMode = TRUE;
		return TRUE;
	default:
		if (!m_framprep)
		{//��ť�Ƿ��ڰ���״̬
			m_Buttonp[m_Select].SetFrame(0);
			switch (m_Select)
			{
			case 6://ȫ�����
				for (int i = 0; i < 5; i++)
				{
					if (i == g_pMainState->m_HeroID)continue;
					Def(i, FALSE);
				}
			case 5://ȫ��ָ��Ŀ��
				for (int i = 0; i < 5; i++)
				{	
					if (i == g_pMainState->m_HeroID)continue;
					SetTarget(i, -1);
				}
				break;
			default:
				Def(m_Select, FALSE);
				SetTarget(m_Select, -1);
				break;
			}
		}
		else
		{
			if (m_Select<5)
			if (!g_pMainState->m_FightList[m_Select].isLive)return TRUE;
			m_framprep->SetFrame(1);
			m_framprep = 0;
			switch (m_Select)
			{
			case 6://ȫ�����
				for (int i = 0; i < 5; i++)
				{
					if (i == g_pMainState->m_HeroID)continue;
					Def(i);
				}
				OnOff(FALSE);
				break;
			default:
				m_Mode = m_Select;
				g_pMainState->m_Cursor.Set(g_pMainState->m_Cursor.m_Attack);
				break;
			}
		}
	break;
	}
	return TRUE;
}

BOOL cInterfaceConduct::ProcessInput()
{
	g_pMainState->SetMouseOnMap(FALSE);
	if (m_Mode>-1)
	{
		m_Select = 1;
		if (CheckRB())return TRUE;
		int select = g_pMainState->m_Cursor.SelectObj(g_pMainState->m_HeroID, 4, m_pObj);
		if (select == -1)return TRUE;
		g_pMainState->m_Cursor.Set(g_pMainState->m_Cursor.m_Normal);
		if (m_Mode ==5)
		{
			for (int i = 0; i < 5; i++)
			{
				if (i == g_pMainState->m_HeroID)continue;
				Def(i, FALSE);
				SetTarget(i, select);
			}
			OnOff(FALSE);
		}
		else
		{
			SetTarget(m_Mode,select);
		}
		m_Mode = -1;
		return TRUE;
	}
	if (m_Sleeptime > 0)
	{
		m_Sleeptime -= 1;
		return TRUE;
	}
	if (m_bMoveMode)//�ƶ�
	{
		m_Select = 16;
		g_pMainState->SetMouseOnMap(FALSE);
		if (!CheckL())
			m_bMoveMode = FALSE;
		return TRUE;
	}
	ResetButton();
	m_Select = -1;
	if(CheckOn(&m_Cancel, 10))return TRUE;
	for (int i = 0; i < 7; i++)
	{
		if(CheckOn(&m_Buttonp[i], i))return TRUE;
	}
	m_Select = 0;
	if (CheckRB())return TRUE;

	//׼���ƶ�
	m_Select = 17;
	CheckL();
	return TRUE;
}

BOOL cInterfaceConduct::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	switch (m_Select)
	{
	case 0:
		OnOff(FALSE);
		break;
	case 1:
		/*m_tTargetName[m_Mode].SetString("");
		m_Buttonp[m_Mode].SetFrame(0);
		g_pMainState->m_AI.m_AttackAI.m_FightTarget[m_Mode] = -1;*/
		g_pMainState->m_Cursor.Set(g_pMainState->m_Cursor.m_Normal);
		m_Mode = -1;
		break;
	default:
		break;
	}
	
	return TRUE;
}

void cInterfaceConduct::SetTarget(int id, int target)
{
	g_pMainState->m_AI.m_AttackAI.m_FightTarget[id] = target;
	g_pMainState->m_AI.m_AttackAI.m_FightTarget[id+5] = target;
	if (target == -1)
	{
		m_Buttonp[id].SetFrame(0);
		m_tTargetName[id].SetString("");
	}
	else
	{
		m_Buttonp[id].SetFrame(1);
		m_tTargetName[id].SetString(g_pMainState->m_FightList[target].m_pData->m_TrueName.GetString());
	}
}

void cInterfaceConduct::Def(int id, BOOL bDef /*= TRUE*/)
{
	if (!g_pMainState->m_FightList[id].isLive)return;
	if (bDef)
	{
		g_pMainState->m_AI.m_AttackAI.Def(id);
		g_pMainState->m_AI.m_AttackAI.Def(id+5);
		m_Buttonp[id].SetFrame(1);
		m_tTargetName[id].SetString("����");
		g_pMainState->m_AI.m_AttackAI.m_FightTarget[id] = id;
		g_pMainState->m_AI.m_AttackAI.m_FightTarget[id + 5] = id+5;
	}
	else
	{
		if (g_pMainState->m_FightList[id].m_WorkType == -2)
		{
			g_pMainState->m_FightList[id].m_WorkType = -1;
			g_pMainState->m_FightList[id].m_pData->ReMoveStateID(1002);
			g_pMainState->m_FightList[id + 5].m_WorkType = -1;
			g_pMainState->m_FightList[id + 5].m_pData->ReMoveStateID(1002);
			g_pMainState->m_AI.m_AttackAI.m_FightTarget[id] = -1;
			g_pMainState->m_AI.m_AttackAI.m_FightTarget[id + 5] = -1;
		}
	}
}



BOOL cInterFaceBuy::Init()
{
	if (m_bInit)return TRUE;
	m_bInit = TRUE;
	m_NumBuy.SetMaxNum(99);
	m_Describe.Init();
	m_PriceTotal = 0;
	m_SelectMark.Load(0x10921CA7);
	m_TrueSelect = -1;
	m_Button.Load(0x35A981D3);
	m_Button.SetStatic(TRUE);
	m_MouseOnMark.Load(0x6F88F494);
	m_MouseOnMark.m_NeedShow = FALSE;
	m_Back.Load(3929029437);
	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic=TRUE;


	/*for (int i = 0; i < 4; i++)
	{
		m_Text[i].SetStyle(2);
	}*/
	m_Text[4].SetColor(RGB(240, 240, 240));
	m_Text[4].SetString("ȷ��");
	m_Text[1].SetColor(0);

	m_Text[5].SetColor(RGB(240, 240, 240));
	m_Text[5].SetString("����");

	UpdateText();
	Move(200, 30);

	m_Left.Load(0xB05126E8);
	m_Left.SetStatic(TRUE);
	m_Right.Load(0x26066130);
	m_Right.SetStatic(TRUE);

	return TRUE;
}

BOOL cInterFaceBuy::Show(cCanvas* pCanvas)
{
	if (!m_NeedShow)return TRUE;
	pCanvas->Add(&m_Back, 1,TRUE);	
	
	for (int i = 0; i < m_ItemNum; i++)
	{
		pCanvas->Add(&m_ItemHead[i], 1);
	}
	pCanvas->Add(&m_Cancel, 1);
	pCanvas->Add(&m_Button, 1);

	if (m_Select > -1)
	{
		pCanvas->Add(&m_MouseOnMark, 1);
	}

	if (m_TrueSelect>-1)
	{
		pCanvas->Add(&m_SelectMark, 1);
	}
	for (int i = 0; i < 6; i++)
	{
		if (i == 1)continue;
		pCanvas->Add(&m_Text[i]);
	}
	m_NumBuy.Show(pCanvas);
	pCanvas->Add(&m_Left, 1);
	pCanvas->Add(&m_Right, 1);
	return TRUE;
}

BOOL cInterFaceBuy::ProcessInput()
{
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}
	if (m_NumBuy.Process())
	{
		CoutTotal();
		return TRUE;
	}
	m_MouseOnMark.m_NeedShow = FALSE;
	g_pMainState->SetMouseOnMap(FALSE);
	m_Select = -1;
	ResetButton();

	if (m_Describe.m_NeedShow)
		m_Describe.OnOff(FALSE);
	if (m_bMoveMode)//�ƶ�
	{
		m_Select = 106;
		g_pMainState->SetMouseOnMap(FALSE);
		if (!CheckL())m_bMoveMode = FALSE;
		return TRUE;
	}

	if (!isInRect(g_xMouse, g_yMouse, &m_Back))return TRUE;

	if (isOn(g_xMouse, g_yMouse, m_Rect))
	{
		m_Select = (g_xMouse - m_Rect.left) / 51 + 5 * ((g_yMouse - m_Rect.top) / 51);
		if (m_Select < m_ItemNum)
		{
			g_pMainState->m_ItemManage.SetItemDescribe(g_xMouse, g_yMouse, m_Describe, &m_Item[m_Select + m_page * 20]);
			m_MouseOnMark.m_NeedShow = TRUE;
			m_MouseOnMark.SetXY(m_ItemHead[m_Select].GetX(), m_ItemHead[m_Select].GetY());
			if (CheckL())return TRUE;
		}
	}
	if (CheckOn(&m_Left, 102))return TRUE;
	if (CheckOn(&m_Right, 103))return TRUE;
	//ȡ����ť
	if (CheckOn(&m_Cancel, 100))return TRUE;
	//ȷ����ť
	if (CheckOn(&m_Button, 101))return TRUE;
	m_Select = 0;
	if(CheckRB())return TRUE;
	//׼���ƶ�
	m_Select = 107;
	CheckL();
	return TRUE;
}

BOOL cInterFaceBuy::Set(sItem2& item,int rate)
{
	m_pricerate.push_back(rate);
	m_Item.push_back(item);
	return TRUE;
}



BOOL cInterFaceBuy::Move(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
	
	m_Text[0].SetXY(m_xPos + 135, m_yPos + 287);
	RECT rect;
	rect.left = m_xPos + 135;
	rect.top = m_yPos + 310;
	rect.right = rect.left + 75;
	rect.bottom = rect.top + 18;
	m_NumBuy.Init(rect);
	m_Text[2].SetXY(m_xPos + 135, m_yPos + 333);
	m_Text[3].SetXY(m_xPos + 135, m_yPos + 356);

	m_Back.SetXY( x,y);
	m_Cancel.m_x = m_xPos + 256;
	m_Cancel.m_y = m_yPos + 10;
	m_Button.m_Picture.m_x = m_xPos + 131;
	m_Button.m_Picture.m_y = m_yPos + 392;
	m_Text[4].SetXCenter(m_Button.GetCenter());
	m_Text[4].SetY(m_Button.GetY()+3);
	m_Text[4].UpdateXCenter();
	m_Text[5].SetXCenter(m_xPos + 140);
	m_Text[5].SetY(m_yPos + 10);
	m_Text[5].UpdateXCenter();
	m_Rect.left = 15 + m_xPos;
	m_Rect.top = 40 + m_yPos;
	m_Rect.right = m_Rect.left + 255;
	m_Rect.bottom = m_Rect.top + 204;
	for (int i = 0; i < 20; i++)
	{
		int xNum = i % 5;
		int yNum = i / 5;
		m_ItemHead[i].SetXY(xNum * 51 + m_Rect.left, yNum * 51 + m_Rect.top);
	}
	if (m_TrueSelect>-1)
		m_SelectMark.m_Picture.SetXY(m_ItemHead[m_TrueSelect].GetX(), m_ItemHead[m_TrueSelect].GetY());
	
	m_Left.SetXY(m_xPos + 20, m_yPos + 252);
	m_Right.SetXY(m_xPos + 70, m_yPos + 252);
	
	return TRUE;
}

BOOL cInterFaceBuy::CheckL()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))return FALSE;
		g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	m_Sleeptime = 3;
	switch (m_Select)
	{
	case 102://��
		if (1)
		{
			m_frampre->SetFrame(1);
			SubPage();
		}
		break;
	case 103://��
		if (1)
		{
			m_frampre->SetFrame(1);
			AddPage();
		}
		break;
	case 106://�ƶ�
		if (1)
		{
			g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
			int xDel = g_xMouse - m_Prex;
			int yDel = g_yMouse - m_Prey;
			m_Prex = g_xMouse;
			m_Prey = g_yMouse;
			if (xDel != 0 || yDel != 0)
			{
				Move(m_xPos + xDel, m_yPos + yDel);
			}
			m_Sleeptime = 0;
		}
		return TRUE;
	case 107://׼���ƶ�
		g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
		m_Prex = g_xMouse;
		m_Prey = g_yMouse;
		m_bMoveMode = TRUE;
		m_Sleeptime = 0;
		return TRUE;
	case 100:
			OnOff(FALSE);
		break;
	case 101:
		if (1)
		{
			m_Button.SetFrame(1);
			if (g_pCharacter->AddMoney(-m_PriceTotal, 0) && m_NumBuy.m_num > 0)
			{
				m_Item[m_TrueSelect2].m_Num = m_NumBuy.m_num;
				g_pMainState->m_InterfaceItem.GetItem(g_pMainState->m_HeroID, &m_Item[m_TrueSelect2]);
				m_Item[m_TrueSelect2].m_Property.clear();
				UpdateText();
				ostringstream oss;
				oss << "����";
				oss << m_NumBuy.m_num;
				oss << "��";
				oss << m_Item[m_TrueSelect2].GetName();
				oss << "������";
				oss << m_PriceTotal;
				oss << "��";
				g_pMainState->m_Tags.Add(oss.str());
			}
			else
			{
				g_pMainState->m_Tags.Add("�ֽ𲻹�");
				return TRUE;
			}
		}
		break;
	default:
		if (1)
		{
			if (m_TrueSelect  >= m_ItemNum)
			{
				m_NumBuy.Reset();
				return TRUE;
			}	
			if (m_Select == m_TrueSelect)
			{
				m_NumBuy.SetNum(m_NumBuy.m_num +1);
				m_NumBuy.SetActive(TRUE);
			}
			else
			{
				m_TrueSelect = m_Select;
				m_TrueSelect2 = m_TrueSelect + m_page * 20;
				m_NumBuy.SetNum(1);
				m_NumBuy.SetActive(TRUE);
			}
			CoutTotal();
			m_SelectMark.m_Picture.SetXY(m_ItemHead[m_TrueSelect].GetX(), m_ItemHead[m_TrueSelect].GetY());
		}
		break;
	}
	return TRUE;
}

void cInterFaceBuy::OnOff(BOOL bshow)
{
	if (m_NeedShow == bshow)return ;
	g_pMainState->InterfaceSetShow(this, bshow);
	if (m_NeedShow)
	{
		Init();
		Update();
	}
	else
	{
		g_pMainState->m_Canvas.SetRedraw(m_Back.m_PreShowRect);
		m_Describe.OnOff(FALSE);
	}
}

void cInterFaceBuy::UpdateText()
{
	ostringstream  oss;
	int price;
	if (m_TrueSelect < 0)
	{
		m_NumBuy.Reset();
		m_PriceTotal = 0;
		price = 0;
	}
	else
	{
		int select = m_TrueSelect + m_page * 20;
		price = m_Item[select].m_pItemData->m_Price;
		if (m_pricerate[m_TrueSelect] != 100)
		{
			INT64 num1 = price;
			INT64 num2 = m_pricerate[m_TrueSelect];
			num1 = num2*num1 / 100;
			price = (int)num1;
		}
	}

	m_Text[0].SetColor(GetPriceColor(price));
	oss << price;
	m_Text[0].SetString(oss.str());
	

	oss.str("");
	m_Text[2].SetColor(GetPriceColor(m_PriceTotal));
	oss << m_PriceTotal;
	m_Text[2].SetString(oss.str());
	

	oss.str("");
	m_Text[3].SetColor(GetPriceColor(g_pCharacter->m_PCData.m_Money[0]));
	oss << g_pCharacter->m_PCData.m_Money[0];
	m_Text[3].SetString(oss.str());
	
}

BOOL cInterFaceBuy::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	switch (m_Select)
	{
	case 0:
		OnOff(FALSE);
		break;
	default:
		break;
	}	
	return TRUE;
}

void cInterFaceBuy::CoutTotal()
{
	if (-1 == m_TrueSelect)return;
	int select = m_TrueSelect + m_page * 20;
	int money = g_pCharacter->m_PCData.m_Money[0];
	int price = m_Item[select].m_pItemData->m_Price;
	
	if (m_pricerate[select] != 100)
	{
		INT64 num1 = price;
		INT64 num2 = m_pricerate[select];
		num1 = num2*num1/100;
		price = (int)num1;
	}
	m_PriceTotal = m_NumBuy.m_num*price;
	if (m_PriceTotal > money)
	{
		m_NumBuy.SetNum((money / price));
		m_PriceTotal = (m_NumBuy.m_num*price);
	}
	UpdateText();
}

void cInterFaceBuy::AddPage()
{
	
	if (m_Item.size() > 20 * (m_page+1))
	{
		m_page += 1;
		
		Update();	
	}
}
void cInterFaceBuy::SubPage()
{

	if (m_page>0)
	{
		m_page -= 1;
		Update();
	}
}
void cInterFaceBuy::Update()
{
	m_ItemNum = m_Item.size() - m_page * 20;
	if (m_ItemNum > 20)m_ItemNum = 20;
	int k = m_page * 20;
	m_TrueSelect = -1;
	m_TrueSelect2 = -1;
	for (int i = 0; i < m_ItemNum; i++)
	{
		m_ItemHead[i].Load(m_Item[k + i].m_pItemData->m_SmallHead);
	}
	UpdateText();
	g_pMainState->m_Canvas.SetRedraw(m_Back.m_PreShowRect);
	m_Describe.OnOff(FALSE);

}

void cInterFaceBuy::Reset()
{
	m_ItemNum = 0;
	m_Item.clear();
	m_pricerate.clear();
	m_page = 0;
}

void cInterFaceBuy::Open(int id)
{
	ostringstream oss;
	oss << g_strMediaPath<< "����\\�̵�\\";
	oss << id;
	oss << ".txt";
	ifstream File;
	File.open(oss.str());
	if (!File.is_open())ERRBOX;
	int num;
	File >> num;
	int Type, id2, pricerate;
	sItem2 item;
	item.m_Num = 1;
	Reset();
	for (int i = 0; i < num; i++)
	{
		File >> Type;
		File >> id2;
		File >> pricerate;
		item.Set(Type, id2);
		Set(item, pricerate);
	}
	OnOff(TRUE);
}

BOOL cInterfaceSmallMap::ProcessInput()
{
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}
	if (isOn(g_xMouse, g_yMouse, g_pMainState->m_Map.m_SmallMap.m_PreShowRect))
	{
		g_pMainState->SetMouseOnMap(FALSE);
	
		if (g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))
		{
			g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
			OnOff(FALSE);
			return TRUE;
		}

		if (g_pMainState->Mouse.GetXDelta() != 0 || g_pMainState->Mouse.GetYDelta() != 0)
		{
			m_MousePos.m_NeedShow = TRUE;
			m_MousePosBack.m_NeedShow = TRUE;
			m_MousePos.SetXY(g_xMouse- m_MousePosBack.m_Picture.m_Width / 2+10,g_yMouse-15);
			m_MousePosBack.SetXY(g_xMouse - m_MousePosBack.m_Picture.m_Width/2,g_yMouse-20);


			int tx = (g_xMouse - g_pMainState->m_Map.m_SmallMap.m_Picture.m_x - 18);
			int ty = (g_yMouse - g_pMainState->m_Map.m_SmallMap.m_Picture.m_y - 18);
			tx =(int )( tx*g_pMainState->m_Map.m_bili);
			ty =(int)( ty*g_pMainState->m_Map.m_bili);
			ty = g_pMainState->m_Map.m_pMap->m_Height - ty;
			tx /= 20;
			ty /= 20;
			ostringstream oss;
			oss << "X:";
			oss << tx;
			oss << " Y:";
			oss << ty;
			m_MousePos.SetString(oss.str());
			
		}
		if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
		{
			g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
		
		    
			PrePareFindPath(g_xMouse, g_yMouse);
#ifdef TESTDEBU
			g_pMainState->m_Map.LoadMap(m_xTarget / 20, (g_pMainState->m_Map.m_pMap->m_Height - m_yTarget) / 20, g_pMainState->m_Map.m_MapName.c_str(), 0);
			return TRUE;
#endif		
			g_pMainState->m_FindPt.FindPath(g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData, m_xTarget, m_yTarget);
			g_pMainState->m_Cursor.m_FollowMode = FALSE;
		}
	}
	else
	{
		m_MousePosBack.m_NeedShow = FALSE;
		m_MousePos.m_NeedShow = FALSE;
	}
	return TRUE;
}

BOOL cInterfaceSmallMap::Show(cCanvas* pCanvas)
{
	if (!m_NeedShow)return TRUE;
	pCanvas->Add(&g_pMainState->m_Map.m_SmallMap, 1, TRUE);
	int xSmallMap = g_pMainState->m_Map.m_SmallMap.m_Picture.m_x;
	int ySmallMap = g_pMainState->m_Map.m_SmallMap.m_Picture.m_y;
	float xOff =(float) g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->GetX();
	float yOff = (float)g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->GetY();
	xOff /= g_pMainState->m_Map.m_bili;
	yOff /= g_pMainState->m_Map.m_bili;
	int xPC =xSmallMap+18+ (int)xOff;
	int yPC =ySmallMap+18+ (int)yOff;
	m_PCPoint.m_Picture.m_x=xPC;
	m_PCPoint.m_Picture.m_y=yPC;
	pCanvas->Add(&m_PCPoint, 1);
	int k = 0;
	if (g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->m_time>-1)
	for (int i = 0; i <= g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->m_time;)
	{
		xOff = (float)g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->m_Path[i].sx;
		yOff = (float)g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->m_Path[i].sy;
		xOff /= g_pMainState->m_Map.m_bili;
		yOff /= g_pMainState->m_Map.m_bili;
		 xPC = xSmallMap + 18 + (int)xOff;
		 yPC = ySmallMap + 18 + (int)yOff;
		 m_PathPointp[k].m_x = xPC;
		 m_PathPointp[k].m_y = yPC;
		 pCanvas->Add(&m_PathPointp[k], 1);
		 k += 1;
		 if (g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->m_time < 300)
			 i += 10;
		 else 
		 if (g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->m_time < 600)
			 i += 20;
		 else
			 i += 40;
	}
	if (m_MousePos.m_NeedShow)
	{
		g_pMainState->m_Canvas.Add(&m_MousePosBack, 1);
		g_pMainState->m_Canvas.Add(&m_MousePos);
	}
	return TRUE;
}

void cInterfaceSmallMap::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;
	m_MousePosBack.Load(0x354CD2EB);
	m_MousePosBack.SetStatic(1);
	m_MousePos.SetColor(RGB(0, 0, 0));
	m_MousePos.m_hFont = g_pMainState->m_hFont[5];
	m_PathPoint.Load(0xF792E03C);
	m_PathPoint.SetStatic(TRUE);
	m_PCPoint.Load(0x146BB550);
	m_PCPoint.SetStatic(TRUE);
	 m_PCPoint.SetStatic(TRUE);
	 for (int i = 0; i < 30; i++)
	 {
		 m_PathPointp[i].Set( &m_PathPoint);
		 m_PathPointp[i].m_bStatic = TRUE;
	 }
}

BOOL cInterfaceSmallMap::PrePareFindPath(int xMouse, int yMouse)
{
	float x = (float)(xMouse-g_pMainState->m_Map.m_SmallMap.m_Picture.m_x-18);
	float y = (float)(yMouse-g_pMainState->m_Map.m_SmallMap.m_Picture.m_y-18);
	m_xTarget =(int)( x*g_pMainState->m_Map.m_bili);
	m_yTarget =(int)( y*g_pMainState->m_Map.m_bili);
	return TRUE;
}

BOOL cInterfaceSmallMap::CheckL()
{
	if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
	{
		g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	}
	else return FALSE;
	m_Sleeptime = 3;
	switch (m_Select)
	{
	case 0:
		break;
	}
	return TRUE;
}

void cInterfaceSmallMap::OnOff(BOOL bShow)
{
	if (!g_pMainState)return;
	if (g_pMainState->m_Map.m_bili)
	{
		g_pMainState->InterfaceSetShow(&g_pMainState->m_InterfaceSmallMap, bShow);
		if (m_NeedShow)
		{
			Init();
			m_MousePosBack.m_NeedShow = FALSE;
			m_MousePos.m_NeedShow = FALSE;
		}

		g_pMainState->m_Canvas.SetRedraw(g_pMainState->m_Map.m_SmallMap.m_PreShowRect);
	}	
}

BOOL cInterfacePetSkill::Show(cCanvas* pCanvas)
{
	pCanvas->Add(&m_back, 1, TRUE);
	pCanvas->Add(&m_Cancel, 1);
	if (m_bDaShuMode)
	{
		pCanvas->Add(&m_DaShuBack, 1, 1);
		pCanvas->Add(&m_Cancel2, 1);
		pCanvas->Add(&m_DaShuText[0]);
		if (m_DaShuText[1].m_NeedShow)
			pCanvas->Add(&m_DaShuText[1]);
		for (int i = 0; i < m_DaShuNum; i++)
		{
			pCanvas->Add(&m_DaShuHead[i], 1);
		}
	}
	else
	{
		for (int i = 0; i < 9; i++)
		{
			pCanvas->Add(&m_TPetProPerty[i]);
		}
	}

	for (int i = 0; i < m_NumSkill; i++)
	{
		pCanvas->Add(&m_SkillHead[i], 1);
	}
	pCanvas->Add(&m_NextPage, 1);

	for (int i = 0; i < 3; i++)
	{
		if (m_pEquip[i])
		{
			pCanvas->Add(&m_EquipBackp[i], 1);
			pCanvas->Add(&m_EquipHead[i], 1);
		}
	}
	m_Describe.Show(pCanvas);
	return TRUE;
}

void cInterfacePetSkill::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;
	m_EquipBack.Load(0xA393A808);
	for (int i = 0; i < 3; i++)
	{
		m_EquipBackp[i].Set(&m_EquipBack);
	}
	for (int i = 0; i < 9; i++)
		m_TPetProPerty[i].SetColor(RGB(0, 0, 0));
	m_Describe.Init();
	m_Describe.m_NeedShow = FALSE;
	m_SkillSelect = -1;
	m_back.Load(1338020705);
	m_back.SetStatic(TRUE);

	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;
	m_Cancel2.Set(&g_pMainState->m_Cancel);
	m_Cancel2.m_bStatic = TRUE;

	//����
	m_DaShuBack.Load(0x907E024C);
	m_DaShuBack.SetStatic(TRUE);
	m_bDaShuMode = FALSE;
	m_DaShuText[0].SetColor(RGB(255, 255, 255));
	m_DaShuText[1].SetColor(RGB(255, 255, 255));
	m_DaShuText[0].m_hFont = g_pMainState->m_hFont[0];
	m_DaShuText[1].m_hFont = g_pMainState->m_hFont[0];

	for (int i = 0; i < 9; i++)
		m_TPetProPerty[i].SetXY(0, 0);
	m_NextPage.Load(0xCB50AB1D);
	m_NextPage.SetStatic(TRUE);
}

BOOL cInterfacePetSkill::ProcessInput()
{
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}
	ResetButton();
	for (int i = 0; i < 3; i++)
	{
		if (m_pEquip[i])
		if (isOn(g_xMouse, g_yMouse, m_EquipBackp[i].m_PreShowRect))
		{
			g_pMainState->m_InterfaceItem.UpDateItemDate(m_Describe,m_pEquip[i]);
			g_pMainState->SetMouseOnMap(FALSE);
			return TRUE;
		}
	}
	if (!isOn(g_xMouse, g_yMouse, m_back.m_PreShowRect))
	{
		if (m_Describe.m_NeedShow)
		m_Describe.OnOff(FALSE);
		return FALSE;
	}
	g_pMainState->SetMouseOnMap(FALSE);
	if( CheckRB())return TRUE;
	//��¼����ֹ��ʱ��
	if (0 == g_pMainState->Mouse.GetXDelta() && 0 == g_pMainState->Mouse.GetYDelta())
	{
		m_ItemDataTime += 1;
	}
	else
	{
		m_ItemDataTime = 0;
		if (m_Describe.m_NeedShow)
		{
			m_Describe.m_NeedShow = FALSE;
			g_pMainState->m_Canvas.SetRedraw(m_Describe.m_Back.m_PreShowRect);
		}
	}
	if (CheckOn(&m_Cancel,61))return TRUE;
	cCharacter* pC = (cCharacter*)g_pMainState->m_FightList[m_FightID].m_pData->m_pIndex;
	
	if (m_bDaShuMode)
	{
		if (isOn(g_xMouse, g_yMouse, m_DaShuBack.m_PreShowRect))
		{
			if (CheckOn(&m_Cancel2, 62))return TRUE;
		
			for (int i = 0; i < 9; i++)
			{
				if (isOn(g_xMouse, g_yMouse, m_DaShuHead[i].m_PreShowRect))
				{
					m_DashuSkillSelect = i;

					if (m_ItemDataTime > 10)//���¼���˵��
					{
						m_Describe.m_NeedShow = TRUE;
						int skillid = pC->m_PCData.m_Item[m_DaShuItemId[m_DashuSkillSelect]].GetID();
						sSkill* pSkill = g_pMainState->m_SkillManager.GetPetSkill(skillid);
						sItem* Item = g_pMainState->m_ItemManage.GetItem(28, 0);
						string text[3];
						text[0] = Item->m_Name;
						text[1] = Item->m_Text;
						text[2] = pSkill->m_Name;
						m_Describe.Set(g_xMouse, g_yMouse, pSkill->m_BigHead, 3, text);
					}
					m_Select = 101;
					return CheckL();
				}
			}
		}
		return TRUE;
	}

		if (CheckOn(&m_NextPage, 11))return TRUE;

			if (isOn(g_xMouse, g_yMouse, m_Rect))
			{
				m_SkillSelect = (g_xMouse - m_Rect.left) / 42 + ((g_yMouse - m_Rect.top) / 42) * 4;
				if (m_ItemDataTime > 10)
				{

					if (m_SkillSelect < m_NumSkill)
					{
						m_Describe.m_NeedShow = TRUE;
						sSkill* pSkill = m_pPetData->m_pPetSkill[m_SkillSelect + m_SkillPage * 12];
						string text[3];
						int textnum = 2;
						text[0] = pSkill->m_Name;
						text[1] = pSkill->m_Text;
						if (pSkill->m_SkillType != 7)
						{
							textnum = 3;
							ostringstream oss;
							oss << "����:";
							oss << pSkill->m_tCost;
							text[2] = oss.str();
						}
						m_Describe.Set(g_xMouse, g_yMouse, pSkill->m_BigHead, textnum, text);
					}
				}
				m_Select = 100;
				return CheckL();//����
			}
		
	

	return FALSE;
}

void cInterfacePetSkill::UpdateData(cPetData* pPetData,BOOL candashu)
{
	if (!pPetData)
	{
		for (int i = 0; i < 9; i++)
			m_TPetProPerty[i].SetString("");
		m_NumSkill = 0;
		return;
	}
	m_bCanDaShu = candashu;
	m_pPetData = pPetData;
	m_SkillPage = 1;
	NextPage();
	
	for (int i = 0; i < 3; i++)
	{
		if (m_pPetData->m_PetEquip[i].GetNum())
		{
		m_pEquip[i] = &m_pPetData->m_PetEquip[i];
			m_EquipHead[i].Load(g_pMainState->m_ItemManage.GetItem(m_pEquip[i]->GetType(), m_pEquip[i]->GetID())->m_SmallHead);
		}
		else m_pEquip[i] = 0;
	}
	ostringstream oss;
	oss.str("");
	oss << pPetData->m_ZiZhi.m_ATKZZ;
	m_TPetProPerty[0].SetString(oss.str());
	oss.str("");
	oss << pPetData->m_ZiZhi.m_DEFZZ;
	m_TPetProPerty[1].SetString(oss.str());
	oss.str("");
	oss << pPetData->m_ZiZhi.m_HPZZ;
	m_TPetProPerty[2].SetString(oss.str());
	oss.str("");
	oss << pPetData->m_ZiZhi.m_MAGZZ;
	m_TPetProPerty[3].SetString(oss.str());
	oss.str("");
	oss << pPetData->m_ZiZhi.m_SPDZZ;
	m_TPetProPerty[4].SetString(oss.str());
	oss.str("");
	oss << pPetData->m_ZiZhi.m_AVOIDZZ;
	m_TPetProPerty[5].SetString(oss.str());
	oss.str("");
	oss << "*MAX*";
	m_TPetProPerty[6].SetString(oss.str());
	float groving = (float)pPetData->m_ZiZhi.m_Growing;
	groving /= 100;
	oss.str("");
	oss << groving;
	m_TPetProPerty[7].SetString(oss.str());
	oss.str("");
	oss << "*";
	m_TPetProPerty[8].SetString(oss.str());
}

BOOL cInterfacePetSkill::CheckL()
{
	if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
	{
		g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	}
	else return FALSE;
	m_Sleeptime = 3;
	cCharacter* pC = (cCharacter*)g_pMainState->m_FightList[m_FightID].m_pData->m_pIndex;
	switch (m_Select)
	{
	case 11:
		if (m_pPetData)
		{
			NextPage();
			
			m_NextPage.SetFrame(1);
			m_NextPage.m_bMove = TRUE;
		}
		break;
	
	case 61:
		if (1)
		{
			OnOff(FALSE);
			m_Cancel.SetFrame(0);
		}
		return TRUE;
	case 62: //ȡ��
		if (1)
		{
			m_bDaShuMode = FALSE;
			g_pMainState->m_Canvas.SetRedraw(m_DaShuBack.m_PreShowRect);
			m_Cancel2.SetFrame(0);
		}
		return TRUE;
	case 100://����		
		if (m_bCanDaShu)
		if (InitDaShuMode())
		{
			if (m_SkillSelect < m_NumSkill)
			{
				m_DaShuText[0].SetString("�滻����");
				m_DaShuText[1].m_NeedShow = TRUE;
				string str2 = "�滻:";
				str2 += m_pPetData->m_pPetSkill[m_SkillSelect]->m_Name;
				m_DaShuText[1].SetString(str2);
			}
			else
			{
				m_SkillSelect = m_NumSkill;
				m_DaShuText[0].SetString("���Ӽ���");
				m_DaShuText[1].m_NeedShow = FALSE;
			}
			m_DaShuBack.m_bMove = TRUE;
			m_bDaShuMode = TRUE;
		}
		return TRUE;
	case 101:
		
		{
			//ȷ����ɫ���Ȿ��
			int  itemnum = m_DaShuItemId[m_DashuSkillSelect];
			sItem2* pItem = &pC->m_PCData.m_Item[itemnum];
			if (pItem->GetNum())
			{
				if (40 == pItem->GetType())
				{
					//sItem* pit = pItem->m_pItemData;
						int itemid = pItem->GetID();
						int pos= m_SkillSelect + 12 * m_SkillPage;
						int conflict = g_pMainState->m_SkillManager.SetPetSkill(m_pPetData, itemid,pos);
						if (-1 < conflict)
						{
							//�˼�����ĳ����ì��
							m_bDaShuMode = FALSE;
							g_pMainState->m_Canvas.SetRedraw(m_DaShuBack.m_PreShowRect);
							ostringstream oss;
							oss << m_pPetData->m_pPetSkill[conflict]->m_Name;
							oss << "��˼��ܲ��ܹ���";
							g_pMainState->m_Tags.Add(oss.str());
							return TRUE;
						}
						NextPage(FALSE);
					/*	m_SkillHead[m_SkillSelect].Load(m_pPetData->m_pPetSkill[m_SkillSelect + 12 * m_SkillPage]->m_BigHead);
						m_SkillHead[m_SkillSelect].m_bMove = TRUE;
						m_NumSkill = m_pPetData->m_NumofBBSkill;*/
						pItem->m_Num -= 1;
						
				}
			}
			m_bDaShuMode = FALSE;
			m_back.m_bMove = TRUE;
		}
		return TRUE;
	}
	return TRUE;
}

void cInterfacePetSkill::Move(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
	
	int xstart = x + 20;
	int ystart = y + 313;
	MoveSkillHead(xstart,ystart);
	
	m_back.SetXY(m_xPos, m_yPos);
	m_DaShuBack.SetX(m_back.m_Picture.m_x + 10);
	m_DaShuBack.SetY(m_back.m_Picture.m_y + 90);
	m_Cancel.SetXY(m_back.GetX() + 185, m_back.GetY() + 5);
	m_Cancel2.SetXY(m_DaShuBack.GetX() + 160, m_DaShuBack.GetY() + 5);
	for (int i = 0; i < 2; i++)
	{
		m_DaShuText[i].SetXCenter(m_DaShuBack.GetX() + m_DaShuBack.GetWidth()/2);
	}
	m_DaShuText[0].UpdateXCenter();
	m_DaShuText[0].SetY(m_DaShuBack.GetY() + 3);
	m_DaShuText[1].SetY(m_DaShuBack.GetY() + 198);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int k = i * 3 + j;
			m_DaShuHead[k].SetX(m_DaShuBack.GetX() + 20 + j * 50);
			m_DaShuHead[k].SetY(m_DaShuBack.GetY() + 40 + i * 50);
		}
	}

	m_NextPage.m_Picture.m_x = m_xPos + 191;
	m_NextPage.m_Picture.m_y = m_yPos + 412;


	xstart = m_xPos + 128;
	m_TPetProPerty[8].SetXY(xstart, m_yPos + 284);
	m_TPetProPerty[7].SetXY(xstart, m_yPos + 261);
	m_TPetProPerty[6].SetXY(xstart, m_yPos + 238);
	m_TPetProPerty[5].SetXY(xstart, m_yPos + 215);
	m_TPetProPerty[4].SetXY(xstart, m_yPos + 192);
	m_TPetProPerty[3].SetXY(xstart, m_yPos + 169);
	m_TPetProPerty[2].SetXY(xstart, m_yPos + 146);
	m_TPetProPerty[1].SetXY(xstart, m_yPos + 123);
	m_TPetProPerty[0].SetXY(xstart, m_yPos + 100);
	for (int i = 0; i < 3; i++)
	{
		m_EquipBackp[i].SetXY(m_xPos+20+i*60,m_yPos+38);
		m_EquipHead[i].SetXY(m_EquipBackp[i].GetX(), m_EquipBackp[i].GetY());
	}
}

BOOL cInterfacePetSkill::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	if (m_bDaShuMode)
	{
		m_bDaShuMode = FALSE;
		g_pMainState->m_Canvas.SetRedraw(m_DaShuBack.m_PreShowRect);
	}
	else
	{
		OnOff(FALSE);
	}
	return TRUE;
}

void cInterfacePetSkill::OnOff(BOOL NeedShow)
{
	g_pMainState->m_Canvas.SetRedraw(m_back.m_PreShowRect);
	m_NeedShow = NeedShow;
	if (!NeedShow)
	{
		m_Describe.OnOff(FALSE);
	}
}

void cInterfacePetSkill::MoveSkillHead(int x, int y)
{
	int xi,yi;
	for (int i = 0; i < 12; i++)
	{
		xi = i % 4;
		yi = i / 4;
		m_SkillHead[i].SetXY( x + 42 * xi, y + 42 * yi);
	}
	m_Rect.left =x;
	m_Rect.right = m_Rect.left + 42 * 4;
	m_Rect.top = y;
	m_Rect.bottom = m_Rect.top + 42 * 3;
}

void cInterfacePetSkill::NextPage(BOOL bnext)
{
	if (bnext)
		m_SkillPage = !m_SkillPage;
	m_NumSkill = m_pPetData->m_NumofBBSkill - 12 * m_SkillPage;
	if (m_NumSkill > 12)m_NumSkill = 12;
	if (m_NumSkill < 0)m_NumSkill = 0;
	for (int i = 0; i < m_NumSkill; i++)
	{
		m_SkillHead[i].Load(m_pPetData->m_pPetSkill[i + 12 * m_SkillPage]->m_BigHead);
	}
}




BOOL cInterfaceLearnSkill::CheckL()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	m_Sleeptime = 3;
	ostringstream oss;
	switch (m_Select)
	{
	case 0://�ر�
		OnOff(FALSE);
		break;
	case 1://ѧϰ����
		if (1)
		{
			LearnSkill();
			m_Button[0].SetFrame(1);
		}
		break;
	case 2://ѧϰ����*5
		for (int i = 0; i < 5; i++)
		{
			if(!LearnSkill())break;
		}
		m_Button[1].SetFrame(1);
		break;
	case 6://�ƶ�
		if (1)
		{
			g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
			int xDel = g_xMouse - m_Prex;
			int yDel = g_yMouse - m_Prey;
			m_Prex = g_xMouse;
			m_Prey = g_yMouse;
			if (xDel != 0 || yDel != 0)
			{
				Move(m_xPos + xDel, m_yPos + yDel);
			}
			m_Sleeptime = 0;
		}
		return TRUE;
	case 7://׼���ƶ�
		g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
		m_Prex = g_xMouse;
		m_Prey = g_yMouse;
		m_bMoveMode = TRUE;
		return TRUE;
	default:
		SetSkillSelect(m_Select - 10);
		break;
	}
	return TRUE;
}

void cInterfaceLearnSkill::OnOff(BOOL NeedShow)
{
	g_pMainState->InterfaceSetShow(this, NeedShow);
	if (m_NeedShow)
	{
		m_Sleeptime = 0;
		Init();
		Update();
		SetSkillSelect(0);
	}
	g_pMainState->m_Canvas.SetRedraw(m_Back.m_PreShowRect);
}

void cInterfaceLearnSkill::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;
	for (int i = 0; i < 2; i++)
	{
		m_SkillSelectBack[i].Load(0xF151309F);
		m_SkillSelectBack[i].SetStatic(TRUE);
		m_SkillSelectBack[i].SetFrame(1);
		m_SkillSelectBack[i].SetxOffset(12);
		m_SkillSelectBack[i].m_NeedShow = FALSE;
	}
	m_Back.Load(0x9ED74AA6);
	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;
	for (int i = 0; i < 2; i++)
	{
		m_Button[i].Set(&g_pMainState->m_Button4);
		m_Button[i].m_bStatic = TRUE;
	//	m_Button[i].Load(0x35A981D3);
	//	m_Button[i].SetStatic(TRUE);
		m_tButton[i].SetColor(RGB(255, 255, 255));	
	}
	m_tButton[0].SetString("ѧϰ*1");
	m_tButton[1].SetString("ѧϰ*5");
	for (int i = 0; i < 7; i++)
	{;
		m_tMengPaiSkill[i].SetColor(0);
		m_tMengPaiSkillLv[i].SetColor(0);
	}
	m_tSkillSelectHead.LockWidth(140);
	m_tMiaoShu.LockWidth(140);
	Move(320-m_Back.GetWidth()/2,5);
}

void cInterfaceLearnSkill::Move(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
	m_Back.SetXY(m_xPos, m_yPos);
	m_Cancel.SetXY(m_xPos + m_Back.GetWidth() - 20, m_yPos + 3);
	for (int i = 0; i < 2;i++)
	m_Button[i].SetXY(m_Back.GetCenter()-80+120*i, m_yPos+434);
	m_Rect.left = m_xPos + 13;
	m_Rect.right = m_Rect.left + 167;
	m_Rect.top = m_yPos + 65;
	m_Rect.bottom = m_Rect.top + 252;


	int xStart = m_Rect.left+1;
	int yStart;
	for (int i = 0; i < 7; i++)
	{
		yStart = m_Rect.top + 1 + 36 * i;
		m_MengPaiSkillHead[i].SetXY(xStart, yStart);
		m_tMengPaiSkill[i].SetXY(xStart + 35, yStart+7);
		m_tMengPaiSkillLv[i].SetXY(xStart + 113, yStart+7);
	}
	m_tSkillSelectHead.SetXY(m_xPos+210,m_yPos+75);
	m_tMiaoShu.SetXY(m_xPos+205,m_yPos+210);

	xStart =m_xPos+ 78;
	yStart = m_yPos + 360;
	m_tHaveExp.SetXY(xStart,yStart);
	m_tHaveMoney[0].SetXY(xStart, yStart+24);
	m_tHaveMoney[1].SetXY(xStart, yStart+46);
	xStart =m_xPos+ 288;
	m_tNeedExp.SetXY(xStart, yStart);
	m_tNeedMoney.SetXY(xStart, yStart+24);
	m_tHaveMoney[2].SetXY(xStart, yStart+46);
	
	for (int i = 0; i < 2; i++)
	{
		m_tButton[i].SetXCenter(m_Button[i].GetCenter());
		m_tButton[i].SetY(m_Button[i].GetY() + 3);
		m_tButton[i].UpdateXCenter();
	}
}

BOOL cInterfaceLearnSkill::ProcessInput()
{
	if (m_Sleeptime>0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}
	g_pMainState->SetMouseOnMap(FALSE);
	ResetButton();
	if (m_bMoveMode)//�ƶ�
	{
		m_Select = 6;
		g_pMainState->SetMouseOnMap(FALSE);
		if (!CheckL())m_bMoveMode = FALSE;
		return TRUE;
	}
	if (!isOn(g_xMouse, g_yMouse, m_Back.m_PreShowRect))return TRUE;
	

	if (CheckOn(&m_Cancel, 0))return TRUE;
	for (int i = 0; i < 2;i++)
		if (CheckOn(&m_Button[i], 1+i))return TRUE;
	if (isOn(g_xMouse, g_yMouse, m_Rect))
	{
		m_Select =10+ (g_yMouse - m_Rect.top) / 36;
		int xStart = m_MengPaiSkillHead[m_Select-10].GetX() + 2;
		int yStart = m_MengPaiSkillHead[m_Select-10].GetY() + 3;
		m_SkillSelectBack[0].SetXY(xStart, yStart);
		m_SkillSelectBack[1].SetXY(xStart + 83, m_SkillSelectBack[0].GetY());
		m_SkillSelectBack[0].m_NeedShow = TRUE;
		m_SkillSelectBack[1].m_NeedShow = TRUE;
		if(CheckL())return TRUE;
	}
	else
	{
		m_SkillSelectBack[0].m_NeedShow=FALSE;
		m_SkillSelectBack[1].m_NeedShow = FALSE;
	}
	//׼���ƶ�
	m_Select = 7;
	if(CheckL())return TRUE;
	m_Select = 0;
	if(CheckRB())return TRUE;
	return TRUE;
}

BOOL cInterfaceLearnSkill::Show(cCanvas* pCanvas)
{
	if (!m_NeedShow)return TRUE;
	pCanvas->Add(&m_Back, 1, 1);
	for (int i = 0; i < 2; i++)
	{
		pCanvas->Add(&m_Button[i], 1);
		pCanvas->Add(&m_tButton[i]);
	}
	pCanvas->Add(&m_Cancel, 1);

	pCanvas->Add(&m_SkillSelectBack[0], 1);
	pCanvas->Add(&m_SkillSelectBack[1], 1);


	for (int i = 0; i < 7; i++)
	{
		pCanvas->Add(&m_MengPaiSkillHead[i], 1);
		pCanvas->Add(&m_tMengPaiSkill[i]);
		pCanvas->Add(&m_tMengPaiSkillLv[i]);
	}
	pCanvas->Add(&m_tSkillSelectHead);
	pCanvas->Add(&m_tMiaoShu);
	pCanvas->Add(&m_tHaveExp);
	pCanvas->Add(&m_tNeedExp);
	pCanvas->Add(&m_tNeedMoney);
	for (int i = 0; i < 3; i++)
	{
		pCanvas->Add(&m_tHaveMoney[i]);
	}

	
	return TRUE;
}

void cInterfaceLearnSkill::SetSkillSelect(int select)
{
	if (select < 0)return;
	if (m_SkillSelect != select)
	{
		if (m_SkillSelect>-1)
		{
			m_tMengPaiSkill[m_SkillSelect].SetColor(0);
			m_tMengPaiSkillLv[m_SkillSelect].SetColor(0);
		}

		m_SkillSelect = select;
		m_tMengPaiSkill[m_SkillSelect].SetColor(RGB(255, 0, 0));
		m_tMengPaiSkillLv[m_SkillSelect].SetColor(RGB(255, 0, 0));

		cMengPai* pMengPai = g_pMainState->m_SkillManager.m_MengPaiManager.GetMengPai(g_pCharacter->m_PCData.m_MengPaiID);
		m_tSkillSelectHead.SetString(pMengPai->m_MainSkill[select].m_Text);
		m_tMiaoShu.SetString(pMengPai->m_MainSkill[select].m_Text2);
	}
	ostringstream oss;
	oss << GetMainSkillExp(g_pCharacter->m_PCData.m_MengPaiSkillLV[select]);
	m_tNeedExp.SetString(oss.str());
	oss.str("");
	INT64 money = GetMainSkillMoney(g_pCharacter->m_PCData.m_MengPaiSkillLV[select]);
	oss << money;
	m_tNeedMoney.SetColor(GetPriceColor((int)money));
	m_tNeedMoney.SetString(oss.str());

	
}

void cInterfaceLearnSkill::Update()
{
	cCharacter* pC = (cCharacter*)g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->m_pIndex;
	cMengPai* pMengPai = g_pMainState->m_SkillManager.m_MengPaiManager.GetMengPai(pC->m_PCData.m_MengPaiID);
	for (int i = 0; i < 7; i++)
	{
		m_MengPaiSkillHead[i].Load(pMengPai->m_MainSkill[i].m_SmallHead);
		m_tMengPaiSkill[i].SetString(pMengPai->m_MainSkill[i].m_Name);
		
		ostringstream oss;
		oss << pC->m_PCData.m_MengPaiSkillLV[i];
			oss << "/180";
			m_tMengPaiSkillLv[i].SetString(oss.str());
			if (m_SkillSelect == i)
			{
				m_tMengPaiSkill[i].SetColor(RGB(255,0,0));
				m_tMengPaiSkillLv[i].SetColor(RGB(255, 0, 0));
			}
			else
			{
				m_tMengPaiSkill[i].SetColor(0);
				m_tMengPaiSkillLv[i].SetColor(0);
			}		
	}
	ostringstream oss;
	oss << pC->m_PCData.EXP;
	m_tHaveExp.SetString(oss.str());
	for (int i = 0; i < 3; i++)
	{
		oss.str("");
		oss << pC->m_PCData.m_Money[i];
		m_tHaveMoney[i].SetColor(GetPriceColor(pC->m_PCData.m_Money[i]));
		m_tHaveMoney[i].SetString(oss.str());
	}
}

BOOL cInterfaceLearnSkill::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	switch (m_Select)
	{
	case 0:
		OnOff(FALSE);
		break;
	default:
		break;
	}
	return TRUE;
}

BOOL cInterfaceLearnSkill::LearnSkill(BOOL cost)
{
	if (m_SkillSelect<-1 || m_SkillSelect>7)return FALSE;
	int& Skilllv = g_pCharacter->m_PCData.m_MengPaiSkillLV[m_SkillSelect];
	if (Skilllv > g_pCharacter->m_PCData.m_LV + 9)
	{
		g_pMainState->m_Tags.Add("�����Ѿ�������ɫ�ȼ�10��");
		return FALSE;
	}
	if (Skilllv > 179)
	{
		g_pMainState->m_Tags.Add("���ܴﵽ����");
		return FALSE;
	}
	if (cost)
	{
		INT64 needexp = GetMainSkillExp(Skilllv);
		int needmoney = (int)GetMainSkillMoney(Skilllv);
		int oldmoney[3];
		INT64 oldexp;
		for (int i = 0; i < 3; i++)
			oldmoney[i] = g_pCharacter->m_PCData.m_Money[i];
		oldexp = g_pCharacter->m_PCData.EXP;


		if (!g_pCharacter->m_PCData.AddExp(-needexp,0,FALSE))
		{

			g_pCharacter->m_PCData.EXP = oldexp;
			g_pMainState->m_Tags.Add("���鲻��");
			return FALSE;
		}
		if (!g_pCharacter->m_PCData.AddMoney(0, -needmoney,0,FALSE))
		{

			g_pCharacter->m_PCData.EXP = oldexp;
			for (int i = 0; i < 3; i++)
			{
				g_pCharacter->m_PCData.m_Money[i] = oldmoney[i];
			}
			g_pMainState->m_Tags.Add("Ǯ����");
			return FALSE;
		}

	}

	g_pMainState->m_SkillManager.lvUpMainSkill(g_pCharacter->m_PCData, 1, m_SkillSelect);
	g_pMainState->m_State.UpDatePcDate();
	Update();
	SetSkillSelect(m_SkillSelect);
	g_pMainState->m_PCHead.UpdatePcExp();
	return TRUE;
}



void cInterfaceXiuLian::Init()
{
	for (int i = 0; i < 2; i++)
	{
		m_SkillSelectBack[i].Load(0xF151309F);
		m_SkillSelectBack[i].SetStatic(TRUE);
		m_SkillSelectBack[i].SetFrame(1);
		m_SkillSelectBack[i].SetxOffset(5);
		m_SkillSelectBack[i].m_NeedShow=FALSE;
	}

	m_MiaoShu.SetString("����30000��,5��ﹱ");
	m_MiaoShu.SetColor(RGB(255,255,255));
	m_Back.Load(0xDD1E6B63);
	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;
	m_Button[0].Load(0x35A981D3);
	m_Button[1].Load(0x2BD1DEF7);
	for (int i = 0; i < 2; i++)
	{
		m_Button[i].SetStatic(TRUE);
		m_tButton[i].SetColor(RGB(255, 255, 255));
	}
	m_tButton[0].SetString("����");
	m_tButton[1].SetString("����*50");

	Move(320 - m_Back.GetWidth() / 2, 240 - m_Back.GetHeight() / 2);
}

void cInterfaceXiuLian::Move(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
	m_Back.SetXY(x, y);
	m_Cancel.SetXY(x + m_Back.GetWidth() - 20, y + 3);
	m_Rect.left = m_xPos + 18;
	m_Rect.top = m_yPos + 60;
	m_Rect.right = m_Rect.left + 175;
	m_Rect.bottom = m_Rect.top + 92;
	for (int i = 0; i < 4; i++)
	{
		m_TXiuLian[i].SetXY(m_Rect.left, m_Rect.top +5+ i * 23);
	}
	m_Button[0].SetXY(m_xPos + m_Back.GetWidth() / 3 - m_Button[0].GetWidth() / 2,
		m_yPos + 165);
	m_Button[1].SetXY(m_xPos + (m_Back.GetWidth() * 2) / 3 - m_Button[1].GetWidth() / 2,
		m_yPos + 165);
	for (int i = 0; i < 2; i++)
	{
		m_tButton[i].SetXCenter(m_Button[i].GetCenter());
		m_tButton[i].SetY(m_Button[i].GetY()+3);
		m_tButton[i].UpdateXCenter();
	}
}

void cInterfaceXiuLian::UpDate()
{
	int lv = g_pCharacter->m_PCData.m_LV - 20;
	if (lv < 0)lv = 0;
	lv /= 5;//��ǰ�ȼ�����������ȼ�
	sBBXiuLian& liulian = g_pCharacter->m_PCData.m_XiuLian;
	
	ostringstream oss;
	oss.str("");
	oss << "���� " << liulian.GetAtk() << "/" << lv << " ";
	oss << "EXP:" << liulian.GetAtkExp() << "/" << GetXiuLIanExp(liulian.GetAtk());
	m_TXiuLian[0].SetString(oss.str());
	oss.str("");
	oss << "���� " << liulian.GetDef() << "/" << lv << " ";
	oss << "EXP:" << liulian.GetDefExp() << "/" << GetXiuLIanExp(liulian.GetDef());
	m_TXiuLian[1].SetString(oss.str());
	oss.str("");
	oss << "���� " << liulian.GetMag() << "/" << lv << " ";
	oss << "EXP:" << liulian.GetMagExp() << "/" << GetXiuLIanExp(liulian.GetMag());
	m_TXiuLian[2].SetString(oss.str());
	oss.str("");
	oss << "���� " << liulian.GetDMag() << "/" << lv << " ";
	oss << "EXP:" << liulian.GetDMagExp() << "/" << GetXiuLIanExp(liulian.GetDMag());
	m_TXiuLian[3].SetString(oss.str());


}

BOOL cInterfaceXiuLian::Show(cCanvas* pCanvas)
{
	pCanvas->Add(&m_Back, 1, 1);
	pCanvas->Add(&m_Cancel, 1);

	if (m_SkillSelectBack[0].m_NeedShow)
	for (int i = 0; i < 2; i++)
	{
		pCanvas->Add(&m_SkillSelectBack[i],1);
	}
	for (int i = 0; i < 4; i++)
	{
		pCanvas->Add(&m_TXiuLian[i]);
	}
	for (int i = 0; i < 2; i++)
	{
		pCanvas->Add(&m_Button[i], 1);
		pCanvas->Add(&m_tButton[i]);
	}
	return TRUE;
}

BOOL cInterfaceXiuLian::ProcessInput()
{
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}
	ResetButton();
	m_Select = -1;
	g_pMainState->SetMouseOnMap(FALSE);
	if (m_bMoveMode)//�ƶ�
	{
		m_Select = 6;
		g_pMainState->SetMouseOnMap(FALSE);
		if (!CheckL())m_bMoveMode = FALSE;
		return TRUE;
	}
	if (!isOn(g_xMouse, g_yMouse, m_Back.m_PreShowRect))return TRUE;
	if (isOn(g_xMouse, g_yMouse, m_Rect))
	{
		m_Select = (g_yMouse - m_Rect.top-1) / 23;
		m_SkillSelectBack[0].SetXY(m_Rect.left, m_Rect.top + m_Select * 23);
		m_SkillSelectBack[1].SetXY(m_Rect.left + 85, m_Rect.top + m_Select * 23);
		m_SkillSelectBack[0].m_NeedShow = TRUE;
		m_SkillSelectBack[1].m_NeedShow = TRUE;
		m_Select += 10;
		if(CheckL())return TRUE;
	}
	else
	{
		m_SkillSelectBack[0].m_NeedShow=FALSE;
		m_SkillSelectBack[1].m_NeedShow = FALSE;
	}
	for (int i = 0; i < 2; i++)
	if (CheckOn(&m_Button[i], i + 1))return TRUE;
	if (CheckOn(&m_Cancel, 0))return TRUE;
	//׼���ƶ�
	m_Select = 7;
	if (CheckL())return TRUE;
	m_Select = 0;
	if (CheckRB())return TRUE;
	return TRUE;
}

BOOL cInterfaceXiuLian::CheckL()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	m_Sleeptime = 3;
	switch (m_Select)
	{
	case 0:
		OnOff(FALSE);
		break;
	case 1:
		m_Sleeptime = 0;
		if (!Leveupskill(1))return TRUE;
		UpDate();
		break;
	case 2:
		m_Sleeptime = 0;
		if (!Leveupskill(50))return TRUE;
		UpDate();
		break;
	case 6://�ƶ�
		if (1)
		{
			g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
			int xDel = g_xMouse - m_Prex;
			int yDel = g_yMouse - m_Prey;
			m_Prex = g_xMouse;
			m_Prey = g_yMouse;
			if (xDel != 0 || yDel != 0)
			{
				Move(m_xPos + xDel, m_yPos + yDel);
			}
			m_Sleeptime = 0;
		}
		return TRUE;
	case 7://׼���ƶ�
		g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
		m_Prex = g_xMouse;
		m_Prey = g_yMouse;
		m_bMoveMode = TRUE;
		return TRUE;
	default:
		if (m_SkillSelect>-1)
			m_TXiuLian[m_SkillSelect].SetColor(0);
		m_SkillSelect= m_Select-10;
		m_TXiuLian[m_SkillSelect].SetColor(RGB(255,0,0));
		break;
	}


	return TRUE;
}

BOOL cInterfaceXiuLian::Leveupskill(int lvup)
{
	if (m_SkillSelect < 0)return FALSE;
	//���Ѱﹱ�ͽ�Ǯ
	int oldmoney[3];//, oldbanggong;
	for (int i = 0; i < 3; i++)
		oldmoney[i] = g_pCharacter->m_PCData.m_Money[i];
	//oldbanggong = g_pCharacter->m_PCData.m_BangPaiPoint;
	/*if (!g_pCharacter->m_PCData.CostBangGong(5 * lvup))
	{
		g_pCharacter->m_PCData.m_BangPaiPoint = oldbanggong;
		g_pMainState->m_Tags.Add("�ﹱ����");
		return FALSE;
	}*/
	
	if (!g_pCharacter->m_PCData.AddMoney(0, -30000 * lvup,0,FALSE))
	{
		//g_pCharacter->m_PCData.m_BangPaiPoint = oldbanggong;
		g_pMainState->m_Tags.Add("Ǯ����");
		return FALSE;
	}
	
	int bsuccess=FALSE;
	if (m_SkillSelect < 4)
	{
		bsuccess = g_pCharacter->m_PCData.m_XiuLian.AddExp(g_pCharacter->m_PCData.m_LV, lvup * 10, (eXiuLian)m_SkillSelect);
	}
	else if (m_SkillSelect < 8)
	{
		bsuccess=g_pCharacter->m_PCData.m_BBXiuLian.AddExp(g_pCharacter->m_PCData.m_LV , lvup * 10, (eXiuLian)(m_SkillSelect - 4));
	}
	if (0 == bsuccess)
	{//�����Ѵ�����
		g_pMainState->m_Tags.Add("���������޷���������");
	//	g_pCharacter->m_PCData.m_BangPaiPoint = oldbanggong;
		for (int i = 0; i < 3; i++)
		{
			g_pCharacter->m_PCData.m_Money[i] = oldmoney[i];
		}
	}
	else if (2 == bsuccess)
	{
		string text="���";
		switch (m_SkillSelect)
		{
		case 0:text += "����"; break;
		case 1:text += "����"; break;
		case 2:text += "����"; break;
		case 3:text += "����"; break;
		case 4:text += "�ٻ��޹���"; break;
		case 5:text += "�ٻ��޷���"; break;
		case 6:text += "�ٻ��޷���"; break;
		case 7:text += "�ٻ��޷���"; break;
		}
		text += "�����ȼ�������!";
		g_pMainState->m_Tags.Add(text);
	}
	return TRUE;
}

BOOL cInterfaceXiuLian::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	switch (m_Select)
	{
	case 0:
		OnOff(FALSE);
		break;
	default:
		break;
	}
	return TRUE;
}

void cInterfaceXiuLian::OnOff(BOOL NeedShow)
{
	g_pMainState->InterfaceSetShow(this, NeedShow);
	g_pMainState->m_Canvas.SetRedraw(m_Back.m_PreShowRect);
	m_NeedShow = NeedShow;
	if (m_NeedShow)
	{
		UpDate();
		if (m_SkillSelect > -1)
		{
			m_TXiuLian[m_SkillSelect].SetColor(0);
		}
		m_SkillSelect = -1;
	}
	return;
}


void cInterfaceGive::Init()
{
	m_SelectMark.Load(0x10921CA7);
	m_SelectMark.m_NeedShow = FALSE;
	m_MouseOnMark.Load(0x6F88F494);
	m_MouseOnMark.m_NeedShow = FALSE;

	m_Back.Load(0xD6A27FFA);
	m_Back.SetStatic(TRUE);
	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;
	m_Button.Load(0x35A981D3);
	m_Button.SetStatic(TRUE);
	m_tButton.SetColor(RGB(255,255,255));
	m_tButton.SetString("����");
	m_tXIanJin.SetColor(RGB(255,255,255));
	for (int i = 0; i < 3; i++)
		m_NumItem[i].SetMaxNum(30);
	m_NumItem[0].SetHuChi(&m_NumItem[1]);
	m_NumItem[0].SetHuChi(&m_NumItem[2]);
	m_NumItem[0].SetHuChi(&m_NumShuru);

	m_NumItem[1].SetHuChi(&m_NumItem[0]);
	m_NumItem[1].SetHuChi(&m_NumItem[2]);
	m_NumItem[1].SetHuChi(&m_NumShuru);

	m_NumItem[2].SetHuChi(&m_NumItem[1]);
	m_NumItem[2].SetHuChi(&m_NumItem[0]);
	m_NumItem[2].SetHuChi(&m_NumShuru);

	m_NumShuru.SetHuChi(&m_NumItem[1]);
	m_NumShuru.SetHuChi(&m_NumItem[2]);
	m_NumShuru.SetHuChi(&m_NumItem[0]);

	for (int i = 0; i < 20; i++)
	{
		m_tItemHead[i].SetColor(RGB(255,255,255));
		m_tItemHead[i].SetStyle(1);
	}
	Move(320 - m_Back.GetWidth() / 2, 240 - m_Back.GetHeight() / 2);
}

void cInterfaceGive::Move(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
	m_Back.SetXY(m_xPos, m_yPos);
	m_Button.SetXY(m_xPos+m_Back.GetWidth()/2-m_Button.GetWidth()/2,m_yPos+ 414);
	m_tButton.SetXCenter(m_Button.GetCenter());
	m_tButton.SetY(m_Button.GetY() + 3);
	m_tButton.UpdateXCenter();
	m_Cancel.SetXY(m_xPos + m_Back.GetWidth() - 20, m_yPos + 3);
	int xstart = m_xPos + 6;
	int ystart = m_yPos + 34;
	m_Rect.left = xstart;
	m_Rect.top = ystart;
	m_Rect.right = xstart + 254;
	m_Rect.bottom = ystart + 203;

	for (int i = 0; i < 20; i++)
	{
		int xNum = i % 5;
		int yNum = i / 5;
		m_ItemHead[i].SetXY(xstart+ xNum * 51, ystart + yNum * 51);
		m_tItemHead[i].SetXY(xstart + xNum * 51 + 2, ystart + yNum * 51 + 2);
	}
	xstart = m_xPos + 33;
	ystart = m_yPos + 270;
	for (int i = 0; i < 3; i++)
	{
		m_ItemGiveHead[i].SetXY(xstart + i * 80, ystart);
	}
	m_tXIanJin.SetXY(m_xPos+42,m_yPos+366);

	RECT rect;
	rect.left = m_xPos + 186;
	rect.right = rect.left + 70;
	rect.top = m_yPos + 365;
	rect.bottom = rect.top + 20;
	m_NumShuru.Init(rect);

	rect.top = m_yPos + 329;
	rect.bottom = rect.top + 20;
	for (int i = 0; i < 3; i++)
	{
		rect.left = m_xPos + 48+i*80;
		rect.right = rect.left + 22;
		m_NumItem[i].Init(rect);
	}
}

void cInterfaceGive::OnOff(BOOL NeedShow)
{
	if (NeedShow == m_NeedShow)return;
	g_pMainState->InterfaceSetShow(this, NeedShow);
	g_pMainState->m_Canvas.SetRedraw(m_Back.m_PreShowRect);
	if (NeedShow)
	{
		UpDate();
		m_SelectMark.m_NeedShow = FALSE;
		for (int i = 0; i < 3; i++)
		{
			m_NumItem[i].m_num = 0;
		}
	}
}

BOOL cInterfaceGive::ProcessInput()
{
	if (m_NumShuru.Process())return TRUE;
	for (int i = 0; i<3;i++)
	if (m_NumItem[i].Process())
	{
		if (m_NumItem[i].m_num)
		{
			if (m_NumItem[i].m_num>g_pCharacter->m_PCData.m_Item[m_ItemGiveOwner[i]].m_Num)
			{
				m_NumItem[i].SetNum(g_pCharacter->m_PCData.m_Item[m_ItemGiveOwner[i]].m_Num);
			}
		}
		return TRUE;
	}
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}
	m_Select = -1;
	ResetButton();
	if (m_bMoveMode)//�ƶ�
	{
		m_Select = 6;
		g_pMainState->SetMouseOnMap(FALSE);
		if (!CheckL())m_bMoveMode = FALSE;
		return TRUE;
	}
	if (!isOn(g_xMouse, g_yMouse, m_Back.m_PreShowRect))return TRUE;
	g_pMainState->SetMouseOnMap(FALSE);
	
	if (isOn(g_xMouse, g_yMouse, m_Rect))
	{
		m_Select = (g_xMouse - m_Rect.left) / 51 +((g_yMouse-m_Rect.top)/51)*5;
		m_MouseOnMark.SetXY(m_ItemHead[m_Select].GetX(), m_ItemHead[m_Select].GetY());
		m_MouseOnMark.m_NeedShow=TRUE;
		m_Select += 10;
		if (CheckL())return TRUE;
	}
	else m_MouseOnMark.m_NeedShow = FALSE;
	for (int i = 0; i < 3; i++)
	{
		if (m_NumItem[i].m_num)
		{
			if (isOn(g_xMouse, g_yMouse, m_ItemGiveHead[i].m_PreShowRect))
			{
				m_Select = i+2;
				m_MouseOnMark.SetXY(m_ItemGiveHead[i].GetX(), m_ItemGiveHead[i].GetY());
				if (CheckL())return TRUE;
			}
		}
	}
	if(CheckOn(&m_Cancel, 0))return TRUE;
	if(CheckOn(&m_Button, 1))return TRUE;
	m_Select = 0;
	if (CheckRB())return TRUE;

		m_Select = 7;
		if (CheckL())return TRUE;

	return TRUE;
}

BOOL cInterfaceGive::Show(cCanvas* pCanvas)
{
	pCanvas->Add(&m_Back, 1, 1);
	for (int i = 0; i < 20; i++)
	{
		if (m_ItemNum[i])
		{
			pCanvas->Add(&m_ItemHead[i],1);
			if (m_ItemNum[i]>1)
			pCanvas->Add(&m_tItemHead[i]);
		}
	}
	for (int i = 0; i < 3; i++)
	{
		if (m_NumItem[i].m_num)
		{
			pCanvas->Add(&m_ItemGiveHead[i], 1);
			m_NumItem[i].Show(pCanvas);
		}
	}
	pCanvas->Add(&m_Cancel, 1);
	pCanvas->Add(&m_Button, 1);
	pCanvas->Add(&m_tButton);
	pCanvas->Add(&m_tXIanJin);
	pCanvas->Add(&m_MouseOnMark, 1);
	pCanvas->Add(&m_SelectMark, 1);
	m_NumShuru.Show(pCanvas);
	
	return TRUE;
}

void cInterfaceGive::UpDate()
{
	
	for (int i = 0; i < 20; i++)
	{
		sItem2& item = g_pCharacter->m_PCData.m_Item[i];
		m_ItemNum[i] = item.GetNum();
		if (m_ItemNum[i] < 1)continue;
		m_ItemHead[i].Load(item.m_pItemData->m_SmallHead);
		if (m_ItemNum[i]>1)
		{
			ostringstream oss;
			oss << item.GetNum();
			m_tItemHead[i].SetString(oss.str());
		}
	}
	for (int i = 0; i < 3; i++)
	{
		if (m_ItemGive[i].GetNum() < 1)continue;
		m_ItemGiveHead[i].Load(m_ItemGive[i].m_pItemData->m_SmallHead);
	}
	ostringstream oss;
	oss<<g_pCharacter->m_PCData.m_Money[0];
	m_tXIanJin.SetString(oss.str());
}

BOOL cInterfaceGive::CheckL()
{
	if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
	{
		g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	}
	else return FALSE;
	m_Sleeptime = 3;
	switch (m_Select)
	{
	case 0:
		OnOff(FALSE);
		break;
	case 1:
		m_Sleeptime = 0;
		m_Button.SetFrame(1);
		OnOff(FALSE);
		if (
			g_pMainState->m_TriggerGive.Process(m_pTarget->m_TrueName.GetString(), m_ItemGive[0].GetID(),
			m_ItemGive[0].GetType()))
		{
			g_pCharacter->m_PCData.m_Item[m_ItemSelect].m_Num -= m_NumItem[0].m_num;
		}
		break;
	case 2:
	case 3:
	case 4:
		m_NumItem[m_Select-2].Reset();
		break;
	case 6://�ƶ�
		if (1)
		{
			m_Sleeptime = 0;
			g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
			int xDel = g_xMouse - m_Prex;
			int yDel = g_yMouse - m_Prey;
			m_Prex = g_xMouse;
			m_Prey = g_yMouse;
			if (xDel != 0 || yDel != 0)
			{
				Move(m_xPos + xDel, m_yPos + yDel);
			}
		}
		return TRUE;
	case 7://׼���ƶ�
		g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
		m_Prex = g_xMouse;
		m_Prey = g_yMouse;
		m_bMoveMode = TRUE;
		return TRUE;
	default:
		m_ItemSelect = m_Select - 10;
		m_SelectMark.SetXY(m_ItemHead[m_ItemSelect].GetX()+3, m_ItemHead[m_ItemSelect].GetY()+1);
		m_SelectMark.m_NeedShow = TRUE;
		AddItem(m_ItemSelect);
		for (int i = 0; i < 3; i++)
		{
			if (m_ItemGiveOwner[i] == m_ItemSelect)
			{
				m_NumItem[i].SetActive(TRUE);
				break;
			}
		}
		break;
	}
	return TRUE;
}

BOOL cInterfaceGive::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	switch (m_Select)
	{
	case 0:
		OnOff(FALSE);
		break;
	default:
		break;
	}
	return TRUE;
}

void cInterfaceGive::AddItem(int select)
{
	sItem2& item = g_pCharacter->m_PCData.m_Item[select];
	if (item.GetNum() < 1)return;
//	for (int i = 0; i < 3; i++)//������Ϸ,����Ҫ�������
	int i = 0;
	{
		if (1>m_NumItem[i].m_num)
		{
			m_ItemGive[i] = item;
			m_ItemGiveOwner[i] = select;
			m_ItemGiveHead[i].Load(m_ItemGive[i].m_pItemData->m_SmallHead);
			m_NumItem[i].Reset();
			m_NumItem[i].ShuRu(1);
			return;
		}
		else
		if (m_ItemGiveOwner[i] == select)
		{
			if (m_NumItem[i].m_num)
			{
				if (m_NumItem[i].m_num > g_pCharacter->m_PCData.m_Item[m_ItemGiveOwner[i]].m_Num)
				{
					m_NumItem[i].SetNum(g_pCharacter->m_PCData.m_Item[m_ItemGiveOwner[i]].m_Num);
				}
				else if (m_NumItem[i].m_num < g_pCharacter->m_PCData.m_Item[m_ItemGiveOwner[i]].m_Num)
				{
					m_NumItem[i].SetNum(m_NumItem[i].m_num + 1);
				}
			}
			return;
		}
	}
}

void cInterfaceGive::SetTarget(cObj* pTarget)
{
	if (!m_bInit)Init(); 
	m_pTarget = pTarget; 
	OnOff(TRUE); 
	UpDate(); 
}

cObj* cInterfaceGive::GetTarget()
{
	return m_pTarget;
}

void cInterfaceGive::ReCoverOnce()
{
	g_pCharacter->m_PCData.m_Item[m_ItemSelect].m_Num += m_NumItem[0].m_num;
}

BOOL cInterfaceNumShuRu::Process()
{
	if (!m_bActive)
	{
		if (isOn(g_xMouse,g_yMouse,m_rect))
		if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
		{
			g_pMainState->SetMouseOnMap(TRUE);
			g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
			SetCaretPos(CountCaretPos(g_xMouse, g_yMouse));
			m_bActive = TRUE;
			return TRUE;
		}
		return FALSE;
	}
	
	if (m_bEraseMode)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		if (!g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
		{
			m_bEraseMode = FALSE;
		}
		else
		{	
			int endpos = CountCaretPos(g_xMouse, g_yMouse);
			if (endpos != m_EraseEnd)
			{
				m_bInEraseMode = TRUE;
				m_EraseEnd = endpos;
				m_tNum.SetTransparentColor(CountPos(m_EraseStart), CountPos(m_EraseEnd), RGB(0, 0, 150));
			}
			m_tNum.SetBackColor(0);
		}
		return TRUE;
	}
	if (m_caretpos < 10)
	{
		if (g_pMainState->Keyboard.GetKeyState(DIK_LEFT))
		{
			g_pMainState->Keyboard.SetLock(DIK_LEFT);
			SetCaretPos(m_caretpos - 1);
			return TRUE;
		}
		if (g_pMainState->Keyboard.GetKeyState(DIK_RIGHT))
		{
			g_pMainState->Keyboard.SetLock(DIK_RIGHT);
			SetCaretPos(m_caretpos + 1);
			return TRUE;
		}
		if (g_pMainState->Keyboard.GetKeyState(DIK_0))
		{
			g_pMainState->Keyboard.SetLock(DIK_0);
			if (m_caretpos == 0 && (!m_bInEraseMode))return TRUE;
			ShuRu(0);
			return TRUE;
		}
		if (g_pMainState->Keyboard.GetKeyState(DIK_1))
		{
			
			g_pMainState->Keyboard.SetLock(DIK_1);
			ShuRu(1);
			return TRUE;
		}
		if (g_pMainState->Keyboard.GetKeyState(DIK_2))
		{
			g_pMainState->Keyboard.SetLock(DIK_2);
			ShuRu(2);
			return TRUE;
		}
		if (g_pMainState->Keyboard.GetKeyState(DIK_3))
		{
			g_pMainState->Keyboard.SetLock(DIK_3);
			ShuRu(3);
			return TRUE;
		}
		if (g_pMainState->Keyboard.GetKeyState(DIK_4))
		{
			g_pMainState->Keyboard.SetLock(DIK_4);
			ShuRu(4);
			return TRUE;
		}
		if (g_pMainState->Keyboard.GetKeyState(DIK_5))
		{
			g_pMainState->Keyboard.SetLock(DIK_5);
			ShuRu(5);
			return TRUE;
		}
		if (g_pMainState->Keyboard.GetKeyState(DIK_6))
		{
			g_pMainState->Keyboard.SetLock(DIK_6);
			ShuRu(6);
			return TRUE;
		}
		if (g_pMainState->Keyboard.GetKeyState(DIK_7))
		{
			g_pMainState->Keyboard.SetLock(DIK_7);
			ShuRu(7);
			return TRUE;
		}
		if (g_pMainState->Keyboard.GetKeyState(DIK_8))
		{
			g_pMainState->Keyboard.SetLock(DIK_8);
			ShuRu(8);
			return TRUE;
		}
		if (g_pMainState->Keyboard.GetKeyState(DIK_9))
		{
			g_pMainState->Keyboard.SetLock(DIK_9);
			ShuRu(9);
			return TRUE;
		}
	}
	if (g_pMainState->Keyboard.GetKeyState(DIK_BACK))
	{
		g_pMainState->Keyboard.SetLock(DIK_BACK);
		if (m_bInEraseMode)
		{
			if (m_EraseStart > m_EraseEnd)swap(m_EraseEnd, m_EraseStart);
		//	int num = m_EraseEnd - m_EraseStart;
			for (int i = m_EraseEnd; i > m_EraseStart; i--)
			{
				Erase(i);
			}
			m_bInEraseMode = FALSE;
		}
		else
		Erase(m_caretpos);
		return TRUE;
	}
	if (isOn(g_xMouse, g_yMouse, m_rect))
	{
		if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
		{
			g_pMainState->SetMouseOnMap(FALSE);
			if (m_num > 0)
			{
				m_bEraseMode = TRUE;
				if (m_bInEraseMode)
				{
					m_tNum.UnTransparentColor(CountPos(m_EraseStart), CountPos(m_EraseEnd), RGB(0, 0, 255));
					m_bInEraseMode = FALSE;
				}
				m_EraseStart = CountCaretPos(g_xMouse, g_yMouse);
				SetCaretPos(m_EraseStart);
				m_EraseEnd = m_EraseStart;
			}
			return TRUE;
		}
	}
	else //����ڷ�Χ����, ȡ������״̬
	{
		if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
		{
			m_bActive = FALSE;
			if (m_bInEraseMode)
			{
				m_tNum.UnTransparentColor(CountPos(m_EraseStart), CountPos(m_EraseEnd), RGB(0, 0, 255));
				m_EraseStart = -1;
				m_EraseEnd = -1;
			}
		}
		return FALSE;
	}
	return FALSE;
}

void cInterfaceNumShuRu::Init(RECT rect)
{
	m_rect.left = rect.left;
	m_rect.right = rect.right;
	m_rect.top = rect.top;
	m_rect.bottom = rect.bottom;
	m_tNum.SetXY(m_rect.left, m_rect.top);
	SetCaretPos(0);
	if (!m_bInit)Init2();
}

int cInterfaceNumShuRu::CountCaretPos(int xMouse, int yMouse)
{
	int truex = xMouse - m_tNum.GetX();
	cText test;

	ostringstream oss;
	int prewidth=0;
	int width;
	int widthcube;
	for (int i = 0; i < m_numlong; i++)
	{
		oss.str("");
		for (int j = 0; j < i+1;j++)
			oss << m_numlist[j];
		test.SetString(oss.str());
		width = test.GetTrueWidth();
		widthcube = width - prewidth;
		if (truex <  widthcube)
		{
			if (truex < widthcube / 2)
				return	i;
			else
				return i + 1;
		}
		prewidth = width;
		truex -= widthcube;
	}
	return m_numlong;
}

void cInterfaceNumShuRu::Init2()
{
	for (int i = 0; i < 15; i++)
	{
		m_numlist.push_back(0);
	}
	
	m_Caret.Load(0x86785735);
	m_Caret.SetStatic(TRUE);
	m_Caret.SetShowWidth(2);
	m_Caret.SetyOffset2(4);
	m_Caret.SetShowHeight(18);
	//m_Caret.SetFrame(1);
	m_bInit = TRUE;
}

void cInterfaceNumShuRu::Show(cCanvas* pCanvas)
{
	if (m_bActive)
	{
		m_time += 1;
		m_time %= 80;
		if (m_time == 0)
			m_Caret.m_NeedShow = FALSE;
		else
		if (m_time == 40)
			m_Caret.m_NeedShow = TRUE;
		pCanvas->Add(&m_Caret, 1);
	}
	//m_Caret.m_NeedShow = !m_Caret.m_NeedShow;
	if (m_numlong)
		pCanvas->Add(&m_tNum);
}

void cInterfaceNumShuRu::UpdateCaret()
{
	int width = CountPos(m_caretpos);
	m_Caret.SetXY(m_rect.left+width,m_rect.top);
}

void cInterfaceNumShuRu::SetCaretPos(int pos)
{
	if (pos < 0)return;
	if (pos > m_numlong)pos = m_numlong;
	m_caretpos = pos;
	UpdateCaret();
}

void cInterfaceNumShuRu::UpdateNum()
{
	ostringstream oss;
	for (int i = 0; i < m_numlong; i++)
		oss << m_numlist[i];
	string text = oss.str();
	m_num= atoi(text.c_str());
	if (m_num>m_MaxNum)
	{
		Reset();
		
		SetNum(m_MaxNum);
		SetCaretPos(m_numlong);
		return;
	}
	m_tNum.SetString(text);
	DWORD color=  GetPriceColor(m_num);
	m_tNum.SetColor(color);
}

int cInterfaceNumShuRu::CountPos(int pos)
{
	if (!pos)return 0;
	cText test;
	ostringstream oss;
	for (int i = 0; i < pos; i++)
		oss << m_numlist[i];
	test.SetString(oss.str());
	return test.GetTrueWidth();
}

void cInterfaceNumShuRu::ShuRu(int num)
{
	if (m_bInEraseMode)
	{
		if (m_bInEraseMode)
		{
			if (m_EraseStart > m_EraseEnd)swap(m_EraseEnd, m_EraseStart);
			//int num = m_EraseEnd - m_EraseStart;
			for (int i = m_EraseEnd; i > m_EraseStart; i--)
			{
				Erase(i);
			}
			m_bInEraseMode = FALSE;
		}
	}
	for (int i = m_numlong-1; i >= m_caretpos; i--)
	{
		m_numlist[i] = m_numlist[i - 1];
	}
	m_numlist[m_caretpos] = num;
	m_numlong += 1;
	SetCaretPos(m_caretpos + 1);
	UpdateNum();
}

void cInterfaceNumShuRu::Erase(int pos)
{
	if (pos <= 0)return;
	m_numlong -= 1;
	for (int i = pos-1 ; i < m_numlong; i++)
	{
		m_numlist[i] = m_numlist[i + 1];
	}
	SetCaretPos(pos - 1);
	UpdateNum();
}

void cInterfaceNumShuRu::Reset()
{
	m_num = 0;
	m_numlong = 0;
	SetCaretPos(0);
}

void cInterfaceNumShuRu::SetNum(int num)
{
	if (num > m_MaxNum)num = m_MaxNum;
	m_num = num;
	m_numlong = 0;
	int numlist[15];
	while (num>0)
	{
		numlist[m_numlong] = num % 10;
		m_numlong += 1;
		num /= 10;
	}
	for (int i = 0; i < m_numlong; i++)
	{
		m_numlist[i] = numlist[m_numlong-1 - i];
	}
	ostringstream oss;
	for (int i = 0; i < m_numlong; i++)
		oss << m_numlist[i];
	string text = oss.str();
	m_tNum.SetString(text);
	DWORD color = GetPriceColor(m_num);
	m_tNum.SetColor(color);
	SetCaretPos(m_numlong);
}

void cInterfaceNumShuRu::SetActive(BOOL bActive)
{
	m_bActive = bActive;
	SetCaretPos(m_numlong);
	if (m_bActive)
	if (m_HuChiList.size())
	{
		for (int i = 0; i < m_HuChiList.size(); i++)
			m_HuChiList[i]->SetActive(FALSE);
	}
}

BOOL cInterfaceYingHang::CheckL()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	m_Sleeptime = 3;
	switch (m_Select)
	{
	case 0:
		OnOff(FALSE);
		break;
	case 1:
		if (1)
		{
			
			if (m_pTrigger)
			{
				m_pTrigger->Process(m_TriggerID);
				OnOff(FALSE);
				return TRUE;
			}
			ostringstream oss;
			if (!m_bMode)
			{
				if (m_pFriend)
				{
					m_pFriend->m_money += m_ShuRu.m_num;
				}
				else
					g_pCharacter->m_PCData.m_Money[1] += m_ShuRu.m_num;

				g_pCharacter->m_PCData.m_Money[0] -= m_ShuRu.m_num;
				oss << "�������#R";
			}
			else
			{
				g_pCharacter->m_PCData.m_Money[1] -= m_ShuRu.m_num;
				g_pCharacter->m_PCData.m_Money[0] += m_ShuRu.m_num;
				oss << "��ȡ����#R";
			}
			oss << m_ShuRu.m_num;
			oss << "#R��";
			m_ShuRu.Reset();
			OnOff(FALSE);
			g_pMainState->m_Dialog.Reset();
			if (m_pFriend)
			{
				g_pMainState->m_Dialog.Talk(oss.str(), m_pFriend->m_Name,
					m_pFriend->GetDialogHead());
			}
			else
			{
				g_pMainState->m_Dialog.SetObj("Ǯׯ�ϰ�");
				g_pMainState->m_Dialog.m_pObj = 0;
				g_pMainState->m_Dialog.SetString(oss.str());
			}
			
			g_pMainState->m_Dialog.OnOff(TRUE);
		}
		break;;
	case 6://�ƶ�
		if (1)
		{
			m_Sleeptime = 0;
			g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
			int xDel = g_xMouse - m_Prex;
			int yDel = g_yMouse - m_Prey;
			m_Prex = g_xMouse;
			m_Prey = g_yMouse;
			if (xDel != 0 || yDel != 0)
			{
				Move(m_xPos + xDel, m_yPos + yDel);
			}
		}
		return TRUE;
	case 7://׼���ƶ�
		g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
		m_Prex = g_xMouse;
		m_Prey = g_yMouse;
		m_bMoveMode = TRUE;
		return TRUE;
	}
	return TRUE;
}

void cInterfaceYingHang::OnOff(BOOL NeedShow)
{
	g_pMainState->InterfaceSetShow(this, NeedShow);
	g_pMainState->m_Canvas.SetRedraw(m_Back.m_PreShowRect);
	if (NeedShow)
	{
		
		m_ShuRu.SetActive(FALSE);
	}
	else  	m_pTrigger = FALSE;
}

void cInterfaceYingHang::SetMode(
	BOOL mode,
	sFriendList* pFriend
	)
	{//0��Ǯ 1 ȡǮ
	Init();
	m_pFriend = pFriend;
	 m_bMode = mode;

	 m_XianJin.SetString(g_pCharacter->m_PCData.m_Money[0]);
	 m_XianJin.SetColor(GetPriceColor(g_pCharacter->m_PCData.m_Money[0]));

	 if (pFriend)
	 {
		 m_CunYing.SetString(pFriend->m_money);
		 m_CunYing.SetColor(GetPriceColor(pFriend->m_money));
	 }
	 else
	 {
		 m_CunYing.SetString(g_pCharacter->m_PCData.m_Money[1]);
		 m_CunYing.SetColor(GetPriceColor(g_pCharacter->m_PCData.m_Money[1]));
	 }
	 if (!m_bMode)//���
	 {
		 m_Back.Load(0x541FA615);
		m_ShuRu.SetMaxNum(g_pCharacter->m_PCData.m_Money[0]);
	 }
	 else //ȡ��
	 {
		 m_Back.Load(0x420FF401);
		 if (m_pFriend)
		 {
			 m_ShuRu.SetMaxNum(m_pFriend->m_money);
		 }
		 else
		 m_ShuRu.SetMaxNum(g_pCharacter->m_PCData.m_Money[1]);
	 }
	 m_ShuRu.Reset();
	 Move(320 - m_Back.GetWidth() / 2, 240 - m_Back.GetHeight() / 2);
}

void cInterfaceYingHang::Move(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
	m_Back.SetXY(m_xPos, m_yPos);
	m_XianJin.SetXY(m_xPos + 100, m_yPos + 46);
	m_CunYing.SetXY(m_xPos + 100, m_yPos + 76);
	m_Cancel.SetXY(m_xPos + m_Back.GetWidth() - 20,m_yPos+3);
	m_Button.SetXY(m_xPos + m_Back.GetWidth()/2-m_Button.GetWidth()/2, m_yPos + 142);
	m_tButton.SetXCenter(m_Button.GetCenter());
	m_tButton.SetY(m_Button.GetY()+3);
	m_tButton.UpdateXCenter();
	RECT rect;
	rect.left = m_xPos + 100;
	rect.right = rect.left + 70;

	rect.top = m_yPos + 106;
	rect.bottom = rect.top + 18;
	m_ShuRu.Init(rect);
}

void cInterfaceYingHang::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;
	m_Button.Set(&g_pMainState->m_Button4);
	m_Button.m_bStatic = TRUE;
	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;
	m_tButton.SetColor(RGB(255,255,255));
	m_tButton.SetString("ȷ��");
}

BOOL cInterfaceYingHang::Show(cCanvas* pCanvas)
{
	pCanvas->Add(&m_Back, 1, 1);
	pCanvas->Add(&m_XianJin);
	pCanvas->Add(&m_CunYing);
	m_ShuRu.Show(pCanvas);
	pCanvas->Add(&m_Cancel, 1);
	pCanvas->Add(&m_Button, 1);
	pCanvas->Add(&m_tButton);
	return TRUE;
}

BOOL cInterfaceYingHang::ProcessInput()
{
	g_pMainState->SetMouseOnMap(FALSE);
	ResetButton();
	if (m_bMoveMode)//�ƶ�
	{
		m_Select = 6;
		g_pMainState->SetMouseOnMap(FALSE);
		if (!CheckL())
			m_bMoveMode = FALSE;
		return TRUE;
	}
	if(m_ShuRu.Process())return TRUE;
	if (!isOn(g_xMouse, g_yMouse, m_Back.m_PreShowRect))return TRUE;
	if(CheckOn(&m_Cancel, 0))return TRUE;
	if(CheckOn(&m_Button, 1))return TRUE;

	m_Select = 7;
	if(CheckL())return TRUE;
	m_Select = 0;
	if (CheckRB())return TRUE;
	return TRUE;
}

BOOL cInterfaceYingHang::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	switch (m_Select)
	{
	case 0:
		OnOff(FALSE);
		break;
	default:
		break;
	}
	return TRUE;
}



void cInterfaceJianDing::OnOff(BOOL NeedShow)
{
	g_pMainState->InterfaceSetShow(this, NeedShow);
	if (NeedShow)
	{
		Init();
		m_ItemSelect = -1;
		g_pMainState->m_ItemSelect.Update();
		for (int i = 0; i < 20; i++)
		{
			sItem2& item = g_pCharacter->m_PCData.m_Item[i];
			if (item.GetNum() == 0)continue;
			BOOL& needshow = g_pMainState->m_ItemSelect.m_ItemHead[i].m_NeedShow;
			if (item.GetType()>21)
			{
				switch (item.GetType())
				{
				case 32:
				case 33:
				case 34:
					needshow = TRUE;
					break;
				case 29:
					if (m_Mode == 3)
					{
						needshow = TRUE;
						break;
					}
				default:
					if(m_Mode == 2)
						needshow = TRUE;
					else
					{
						needshow = FALSE;
					}
					
					break;
				}
			}
			else needshow = TRUE;
		}
		g_pMainState->m_ItemSelect.Move(m_xPos + 20, m_yPos + 35);

	}
	else
	{
		g_pMainState->m_ItemSelect.m_Describe.OnOff(FALSE);
	}
	g_pMainState->m_Canvas.SetRedraw(m_Back.m_PreShowRect);
}

BOOL cInterfaceJianDing::CheckL()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	m_Sleeptime = 3;
	switch (m_Select)
	{
	case 0:
		OnOff(FALSE);
		break;
	case 1:
		if (1)
		{
			sItem2& item = g_pCharacter->m_PCData.m_Item[m_NowItemSelect];
			if (item.m_Property.size())
			{
				g_pMainState->m_Tags.Add("����Ѿ���������");
				return TRUE;
			}
			int moneyneed = item.m_pItemData->m_Price / 10;
			if (g_pCharacter->m_PCData.AddMoney(-moneyneed, 0,0,FALSE))
			{
				g_pMainState->m_ItemManage.AutoItemProperty(&item);
				ostringstream oss;
				oss << "�����ɹ�,������";
				oss << moneyneed;
				oss << "��";
				g_pMainState->m_Tags.Add(oss.str());
			}
			else
			{
				g_pMainState->m_Tags.Add("��Ǯ����");
			}
		}
		break;
	case 2:
		if (1)
		{
			sItem2& item = g_pCharacter->m_PCData.m_Item[m_NowItemSelect];
			
			int moneyneed=0;
			switch (m_Mode)
			{
			case 1:
				moneyneed = (item.m_Num*item.m_pItemData->m_Price) / 2;
				break;
			case 2:
				moneyneed = (item.m_Num*item.m_pItemData->m_Price) / 3;
				break;
			}
			g_pCharacter->m_PCData.m_Money[0] += moneyneed;
			item.m_Num = 0;
			g_pMainState->m_ItemSelect.m_ItemHead[m_NowItemSelect].m_NeedShow = FALSE;
			ostringstream oss;
			oss << "�õ���";
			oss << moneyneed;
			oss << "��";
			g_pMainState->m_Tags.Add(oss.str());
		}
		break;
	case 6://�ƶ�
		if (1)
		{
			m_Sleeptime = 0;
			g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
			int xDel = g_xMouse - m_Prex;
			int yDel = g_yMouse - m_Prey;
			m_Prex = g_xMouse;
			m_Prey = g_yMouse;
			if (xDel != 0 || yDel != 0)
			{
				Move(m_xPos + xDel, m_yPos + yDel);
			}
		}
		return TRUE;
	case 7://׼���ƶ�
		g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
		m_Prex = g_xMouse;
		m_Prey = g_yMouse;
		m_bMoveMode = TRUE;
		return TRUE;
	case 8:
		if (1)
		{
			sItem2& item = g_pCharacter->m_PCData.m_Item[m_NowItemSelect];
			if (item.GetType() == 29)
			{//��ʯ	
				if (m_ItemSelect == -1)return TRUE;
				sItem2& equip = g_pCharacter->m_PCData.m_Item[m_ItemSelect];
				if (0 == equip.m_Property.size())
				{
					g_pMainState->m_Tags.Add("��û����");
					return TRUE;
				}
				int nummax = g_pMainState->m_ItemManage.GetlvByID(equip.GetType(), equip.GetID()) / 10;
				if (nummax > equip.m_Property[0])
				{
					if (item.m_Num > equip.m_Property[0])
					{
						if (!g_pMainState->m_ItemManage.XiangQian(g_pCharacter->m_PCData.m_Item[m_ItemSelect], item))
						{
							g_pMainState->m_Tags.Add("�޷���Ƕ�������");
						}
						else
						{
							g_pMainState->m_Tags.Add("��Ƕ�ɹ�");
							g_pCharacter->m_PCData.m_Item[m_NowItemSelect].m_Num -= g_pCharacter->m_PCData.m_Item[m_ItemSelect].m_Property[0];
							g_pMainState->m_ItemSelect.Update();
						}
					}
					else g_pMainState->m_Tags.Add("��������");
				}
				else
				{
					g_pMainState->m_Tags.Add("�޷�����Ƕ��");
				}
			}
			else if (item.GetType() < 22 || item.GetType() == 32 || item.GetType() == 33 || item.GetType()==34)
			{
				m_ItemSelect = m_NowItemSelect;
			}
		}
	}
	return TRUE;
}

BOOL cInterfaceJianDing::ProcessInput()
{
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}
	m_NowItemSelect = -1;
	m_Select = -1;
	ResetButton();
	if (m_bMoveMode)//�ƶ�
	{
		m_Select = 6;
		g_pMainState->SetMouseOnMap(FALSE);
		if (!CheckL())m_bMoveMode = FALSE;
		return TRUE;
	}

	if (isOn(g_xMouse, g_yMouse, m_Back.m_PreShowRect))
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_NowItemSelect= g_pMainState->m_ItemSelect.Process(g_xMouse, g_yMouse);
		if (m_NowItemSelect > -1)
		{
			m_Select = 2;
			switch (m_Mode)
			{
			case 0:
					m_Select = 1;
			case 1:
			case 2:
				if (1)
				{	
					if (!g_pCharacter->m_PCData.m_Item[m_NowItemSelect].m_pItemData)return FALSE;
					int price = g_pCharacter->m_PCData.m_Item[m_NowItemSelect].m_pItemData->m_Price;
					ostringstream oss;
					oss << "�۸�";
					switch (m_Mode)
					{
					case 0:price /= 10; break;
					case 1:price /= 2; break;
					default:price /= 3; break;
					}
					oss << price;
					oss << "��";
					m_tPrice.SetString(oss.str());
					m_tPrice.UpdateXCenter();
				}
				break;
			case 3:
			{
					  int ittype = g_pCharacter->m_PCData.m_Item[m_NowItemSelect].GetType();
					  m_Select = 8;
					  if (ittype < 22 || ittype==32||ittype==33||ittype==34)
					  {
						  if (!g_pCharacter->m_PCData.m_Item[m_NowItemSelect].m_Property.size())
							  return TRUE;
						  ostringstream oss;
						  oss << g_pCharacter->m_PCData.m_Item[m_NowItemSelect].GetName() << "ǿ����Ҫ";
						  oss << g_pCharacter->m_PCData.m_Item[m_NowItemSelect].m_Property[0] + 1;
						  oss << "����ʯ";
						  m_tPrice.SetString(oss.str());
						  m_tPrice.UpdateXCenter();
					  }
			}
				break;
			default:ERRBOX;
			}
			if(CheckL())return TRUE;
			return TRUE;
		}
	
		if (CheckOn(&m_Cancel, 0))return TRUE;
		//׼���ƶ�
		m_Select = 7;
		CheckL();
		if (g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))
		{
			g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
			OnOff(FALSE);
		}
	}
	return TRUE;
}

BOOL cInterfaceJianDing::Show(cCanvas* pCanvas)
{
	pCanvas->Add(&m_Back, 1, 1);
	pCanvas->Add(&m_Cancel, 1);
	g_pMainState->m_ItemSelect.Show(pCanvas);
	pCanvas->Add(&m_tPrice);
	return TRUE;
}


void cInterfaceJianDing::Move(int xPos, int yPos)
{
	m_xPos = xPos;
	m_yPos = yPos;
	m_Back.SetXY(xPos, yPos);
	m_Cancel.SetXY(m_xPos+m_Back.GetWidth()-20,m_yPos+3);
	g_pMainState->m_ItemSelect.Move(xPos+20, yPos+35);
	
	m_tPrice.SetXCenter(m_Back.GetCenter()-10);
	m_tPrice.SetY(m_yPos + 3);
	m_tPrice.UpdateXCenter();
}

void cInterfaceJianDing::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;
	m_Back.Load(0xE3AEF018);
	m_Back.SetStatic(TRUE);
	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;
	m_tPrice.SetColor(RGB(255,255,255));
	g_pMainState->m_ItemSelect.Init();
	Move(320 - m_Back.GetWidth() / 2, 240 - m_Back.GetHeight() / 2);
}



void cItemSelect::Move(int xPos, int yPos)
{
	Init();
	m_Rect.left = xPos;
	m_Rect.top = yPos;
	m_Rect.right = m_Rect.left + 255;
	m_Rect.bottom = m_Rect.top + 204;
	int xOff, yOff;
	for (int i = 0; i < 20; i++)
	{
		xOff = xPos + (i % 5) * 51;
		yOff = yPos + (i / 5) * 51;
		m_ItemHead[i].SetXY(xOff,yOff);
		m_tItemNum[i].SetXY(xOff+2,yOff+2);
	}
}


int cItemSelect::Process(int xMouse, int yMouse)
{
	if (m_Describe.m_NeedShow)
		m_Describe.OnOff(FALSE);
	if (!isOn(xMouse, yMouse, m_Rect))
	{
		m_MouseOnMark.m_NeedShow = FALSE;
		return -1;
	}
	
	m_Select = (xMouse - m_Rect.left) / 51 + ((yMouse - m_Rect.top) / 51) * 5;

	sItem2* pItem = &g_pCharacter->m_PCData.m_Item[m_Select];
	if (!m_ItemHead[m_Select].m_NeedShow)return -1;
	g_pMainState->m_ItemManage.SetItemDescribe(
		xMouse, yMouse, m_Describe, pItem
		);

	m_MouseOnMark.m_NeedShow = TRUE;
	m_MouseOnMark.SetXY(m_ItemHead[m_Select].GetX(), m_ItemHead[m_Select].GetY());
	return m_Select;
}

void cItemSelect::Update(cCharacter* pc)
{
	Init();
	if (0 == pc)
		pc = g_pCharacter;
	m_SelectMark.m_NeedShow = FALSE;
	for (int i = 0; i < 20; i++)
	{
		m_ItemNum[i] = pc->m_PCData.m_Item[i].GetNum();
		if (m_ItemNum[i])
		{
			m_ItemHead[i].m_NeedShow=TRUE;
			m_ItemHead[i].Load(pc->m_PCData.m_Item[i].m_pItemData->m_SmallHead);
			if (m_ItemNum[i]>1)
			{
				ostringstream oss;
				oss << m_ItemNum[i];
				m_tItemNum[i].SetString(oss.str());
			}
		}
		else
			m_ItemHead[i].m_NeedShow = FALSE;
	}
}

void cItemSelect::Show(cCanvas* pCanvas)
{
	for (int i = 0; i < 20; i++)
	{
		if (m_ItemHead[i].m_NeedShow)
		{
			pCanvas->Add(&m_ItemHead[i], 1);
			if (m_ItemNum[i]>1)
			pCanvas->Add(&m_tItemNum[i]);
		}
	}
	m_Describe.Show(pCanvas);
	pCanvas->Add(&m_MouseOnMark, 1);
	pCanvas->Add(&m_SelectMark, 1);
}

void cItemSelect::Init()
{
	if (m_bInit)return;
	m_Describe.Init();
	m_bInit = TRUE;
	m_ItemHead.resize(20);
	m_tItemNum.resize(20);
	m_ItemNum.resize(20);
	m_SelectMark.Load(0x10921CA7);
	m_SelectMark.m_NeedShow = FALSE;
	m_MouseOnMark.Load(0x6F88F494);
	m_MouseOnMark.m_NeedShow = FALSE;
}

void cItemSelect::SetSelectMark(int id)
{
	if (id > -1)
	{
		m_SelectMark.m_NeedShow = TRUE;
		m_SelectMark.SetXY(m_ItemHead[m_Select].GetX(), m_ItemHead[m_Select].GetY());
	}
	else m_SelectMark.m_NeedShow = FALSE;

}

BOOL cInterfaceChaKan::CheckL()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	m_Sleeptime = 3;
	switch (m_Select)
	{
	case 0:
		OnOff(FALSE);
		break;
	case 6://�ƶ�
		if (1)
		{
			m_Sleeptime = 0;
			g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
			int xDel = g_xMouse - m_Prex;
			int yDel = g_yMouse - m_Prey;
			m_Prex = g_xMouse;
			m_Prey = g_yMouse;
			if (xDel != 0 || yDel != 0)
			{
				Move(m_xPos + xDel, m_yPos + yDel);
			}
		}
		return TRUE;
	case 7://׼���ƶ�
		g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
		m_Prex = g_xMouse;
		m_Prey = g_yMouse;
		m_bMoveMode = TRUE;
		return TRUE;
	}
	return TRUE;
}

void cInterfaceChaKan::OnOff(BOOL NeedShow)
{
	if (m_NeedShow == NeedShow)return;
	g_pMainState->InterfaceSetShow(this, NeedShow);
	if (!m_NeedShow)
	{
		m_data = 0;
		m_PetSkill.OnOff(FALSE);
	}
	else
	{
		Move(320 - m_Back.GetWidth() / 2, 240 - m_Back.GetHeight() / 2);
	}
	g_pMainState->m_Canvas.SetRedraw(m_Back.GetPicRange());
}

void cInterfaceChaKan::Move(int xPos, int yPos)
{
	m_xPos = xPos;
	m_yPos = yPos;
	m_Back.SetXY(xPos, yPos);
	m_Cancel.SetXY(xPos + m_Back.GetWidth() - 20, yPos + 3);
	m_PetSkill.m_back.m_PreShowRect.right = m_xPos+m_Back.GetWidth();
	m_PetSkill.m_back.m_PreShowRect.top = m_yPos;
	m_PetSkill.m_back.m_PreShowRect.bottom = m_yPos + m_Back.GetHeight();
	m_PetSkill.m_back.m_PreShowRect.left = m_PetSkill.m_back.m_PreShowRect.right - m_PetSkill.m_back.GetWidth() / 2;
	
	
	m_Property[14].SetXCenter(m_xPos+115);
	m_Property[14].SetY(m_yPos+180);
	m_Property[14].UpdateXCenter();

	m_Property[13].SetXY(m_xPos + 93, m_yPos + 208);
	m_Property[12].SetXY(m_xPos+218, m_yPos + 208);

	int xstart = m_xPos + 60;
	int xstart2 = xstart + 149;
	int ystart = m_yPos + 231;
	for (int i = 0; i < 6; i++)
	{
		m_Property[i].SetXY(xstart,ystart);
		m_Property[i + 6].SetXY(xstart2,ystart);
		ystart += 24;
	}
	for (int i = 0; i < 4; i++)
	{
		m_Pic[i].SetXY(114+m_xPos,154+m_yPos);
	}
	m_Shadow.SetXY(114 + m_xPos, 154 + m_yPos);
	m_PetSkill.Move(m_xPos+m_Back.GetWidth()-m_PetSkill.m_back.GetWidth() , yPos-69);
	m_PetSkill.MoveSkillHead(m_PetSkill.m_xPos + 20,m_PetSkill.m_yPos+311);
	m_PetSkill.m_back.m_PreShowRect.left = m_PetSkill.m_xPos + 20;
	m_PetSkill.m_back.m_PreShowRect.right = m_Back.GetWidth() + m_xPos;
	m_MengPai.SetXCenter(m_Shadow.GetX());
	m_MengPai.SetY(m_yPos + 35);
	m_MengPai.UpdateXCenter();
	MovePetEquip();
}

BOOL cInterfaceChaKan::ProcessInput()
{
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}
	m_Select = -1;
	ResetButton();
	if (m_bMoveMode)//�ƶ�
	{
		m_Select = 6;
		g_pMainState->SetMouseOnMap(FALSE);
		if (!CheckL())
			m_bMoveMode = FALSE;
		return TRUE;
	}

	if (!isOn(g_xMouse, g_yMouse, m_Back.m_PreShowRect))return TRUE;
	if (CheckOn(&m_Cancel, 0))return TRUE;

	if (!m_PetSkill.m_bDaShuMode)
		if (CheckRB())return TRUE;
	m_PetSkill.ProcessInput();

	g_pMainState->SetMouseOnMap(FALSE);
	
	m_Select = 7;
	if (CheckL())return TRUE;
	return TRUE;
}

BOOL cInterfaceChaKan::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	OnOff(FALSE);
	return TRUE;
}

void cInterfaceChaKan::Set(cObj* data)
{
	Init();
	cCharacter* pc;
	cPet* ppet;
	m_MengPai.m_NeedShow = FALSE;

	vector<DWORD> picid;
	sChangeColorData* pranse[4];
	data->GetIndex(POS_STAND, picid);
	switch (data->m_IndexType)
	{
	case INDEXTYEP_CHARACTER:
		if (1)
		{
			pc = (cCharacter*)data->m_pIndex;
			m_data = &pc->m_PCData;
			m_Property[13].SetString("---");
			int mengpaiid = pc->m_PCData.m_MengPaiID;
			if (mengpaiid > -1 && mengpaiid<15)
			{
				m_MengPai.SetString(g_pMainState->m_SkillManager.m_MengPaiManager.GetMengPai(mengpaiid)->m_Name);
				m_MengPai.m_NeedShow = TRUE;
				m_MengPai.UpdateXCenter();
			}
			pc->GetRanse(POS_STAND, pranse);
		}
		break;
	case INDEXTYEP_PET:
		if (1)
		{
			ppet = (cPet*)data->m_pIndex;
			m_data = &ppet->m_PetDataZZ;
			m_Property[13].SetString(ppet->GetData()->m_LVLimit);
			ppet->GetRanse(pranse);
		}
		break;
	default:
		return;
	}
	
	m_PetSkill.UpdateData(m_data,FALSE);
	MovePetEquip();
	ostringstream oss;
	m_Property[14].SetString(data->m_TrueName.GetString());
	
	m_Property[12].SetString(m_data->m_LV);

	oss.str("");
	oss << m_data->m_HP << "/" <<(m_data->m_HPMax.GetData());
	m_Property[0].SetString(oss.str());
	oss.str("");
	oss << m_data->m_MP << "/" << (m_data->m_MPMax.GetData());
	m_Property[1].SetString(oss.str());
	m_Property[2].SetString(m_data->m_ATK.GetData());
	m_Property[3].SetString(m_data->m_DEF.GetData());
	m_Property[4].SetString(m_data->m_SPD.GetData());
	m_Property[5].SetString(m_data->m_Wakan.GetData());

	m_Property[6].SetString(m_data->m_ProPertyHP);
	m_Property[7].SetString(m_data->m_ProPertyMag);
	m_Property[8].SetString(m_data->m_ProPertyAtk);
	m_Property[9].SetString(m_data->m_ProPertyDef);
	m_Property[10].SetString(m_data->m_ProPertySpd);
	m_Property[11].SetString(0);

	for (int i = 0; i < 4; i++)m_Pic[i].Free();

	for (int i = 0; i < picid.size(); i++)
		m_Pic[i].Load(picid[i], pranse[i]);
}

void cInterfaceChaKan::Set(sFriendList& list)
{
	cObj* Obj = new cObj;
	g_pMainState->LoadCharacterData2(Obj, list.m_dataFileName);
	cCharacter* pc1 = (cCharacter*)Obj->m_pIndex;
	cPCData* pc = &pc1->m_PCData;
	pc->m_StrongLv = list.m_stronglv;
	/*for (int i = 0; i < list.m_PetSkillList.size(); i++)
		g_pMainState->m_SkillManager.SetPetSkill(pc, list.m_PetSkillList[i], i);*/
	Obj->FollowLv();
	//Ⱦɫ
	for (int i = 1; i < 4; i++)
		pc->m_RanSe.m_ranse[i].partnum = 0;
	pc->m_RanSe.m_ranse[0] = list.m_Ranse.m_ranse[0];
	if (pc->m_RanSe.m_ranse[0].partnum&&pc1->GetData()->m_ColorID)
	{
		g_pMainState->m_ColorManage.Set(pc1->GetData()->m_ColorID);
		for (int i = 0; i < list.m_Ranse.m_ranse[0].partnum; i++)
		{
			g_pMainState->m_ColorManage.GetColorMatrix(
				i,
				pc->m_RanSe.m_ranse[0].GetColorChange(i),
				pc->m_RanSe.m_ranse[0].m_colormatrix[i]);
		}
	}


	//����
	pc->m_bOnZuoQi = list.m_bOnZuoQi; //�Ƿ���������
	pc->zuoqiid = list.m_ZuoQiID;
	pc->m_ZuoQiRanSe=list.m_ZuoQiRanse;
	g_pMainState->m_ZuoQi.GetZuoQi(*pc, pc->zuoqiid);
	
	//װ��Ⱦɫ
	if (list.m_bZhuangShi)
	{
		pc->m_ZuoQi.m_ZhuangShi.m_Num = 1;
		pc->m_ZuoQi.m_ZhuangShi.Set(31, list.m_ZhuangShiID);
		pc->m_ZhuangShiRanSe = list.m_ZhuangShiRanse;
	}
	if (list.m_weaponid>-1)
	pc->m_Equip[2].Set(list.m_weapontype,list.m_weaponid);

	Set(Obj);
	Obj->Free();
	SAFE_DELETE(Obj);
}

void cInterfaceChaKan::Set(sShangPinPet& list)
{
	m_Obj.AutoPetData(list.m_name, list.m_modeid, 0, &list.m_addpoint, FALSE);
	m_Obj.FollowLv();
	cPetData* petdata;
	m_Obj.GetDateP(petdata);
	for (int i = 0; i < list.m_skillid.size(); i++)
		g_pMainState->m_SkillManager.SetPetSkill(petdata, list.m_skillid[i]);
	petdata->m_RanSe = list.m_ranse;

	Set(&m_Obj);
}

void cInterfaceChaKan::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;
	m_PetSkill.Init();
	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;
	m_Back.Load(0x536A7E15);
	m_PetSkill.m_back.m_NeedShow = FALSE;
	m_PetSkill.m_Cancel.m_NeedShow = FALSE;
	for (int i = 0; i < 4; i++)
		m_Pic[i].SetDirection(3);
	m_Shadow.Set(&g_pMainState->m_Shadow);
	m_MengPai.SetColor(RGB(255,255,255));
}

BOOL cInterfaceChaKan::Show(cCanvas* pCanvas)
{
	pCanvas->Add(&m_Back, 1, 1);
	pCanvas->Add(&m_Cancel, 1);
	for (int i = 0; i < 15; i++)
	{
		pCanvas->Add(&m_Property[i]);
	}
	pCanvas->Add(&m_Shadow, 1);
	for (int i = 0; i < 4; i++)
	{
		pCanvas->Add(&m_Pic[i], 1);
	}
	m_PetSkill.Show(pCanvas);
	if (m_MengPai.m_NeedShow)
		pCanvas->Add(&m_MengPai);
	return TRUE;
}

void cInterfaceChaKan::MovePetEquip()
{
	for (int i = 0; i < 3; i++)
	{
		m_PetSkill.m_EquipBackp[i].SetXY(m_xPos+200,m_yPos+30+i*60);
		m_PetSkill.m_EquipHead[i].SetXY(m_PetSkill.m_EquipBackp[i].GetX(), m_PetSkill.m_EquipBackp[i].GetY());
	}
}



void cInterfaceFriendQiangHua::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;
	m_Back.Load(0x694F39D1);
	m_Back.SetStatic(TRUE);
	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;
	m_title.SetColor(RGB(255,255,255));
	m_title.SetString("���Ѷ���");
	m_Name.SetColor(RGB(255,255,255));
	m_Money.SetColor(RGB(255, 255, 255));

	for (int i = 0; i < 12; i++)
	{
		m_Buttonp[i].Set(&g_pMainState->m_Button4);
		m_Buttonp[i].m_bStatic = TRUE;
		m_Text[i].SetColor(RGB(255,255,255));
	}
	m_Text[0].SetString("���򱦱�");
	m_Text[1].SetString("����");
	m_Text[2].SetString("����");
	m_Text[3].SetString("����װ��");

	m_Text[4].SetString("��ɫȾɫ");
	m_Text[5].SetString("����Ⱦɫ");
	m_Text[6].SetString("װ��Ⱦɫ");
	m_Text[7].SetString("����Ⱦɫ");
	
	m_Text[8].SetString("��");//��ɫǿ��");
	m_Text[9].SetString("��ս����");
	m_Text[10].SetString("��");//����ǿ��");
	m_Text[11].SetString("����");

	Move(320 - m_Back.GetWidth() / 2, 240 - m_Back.GetHeight() / 2);
}

void cInterfaceFriendQiangHua::Move(int xPos, int yPos)
{
	m_xPos = xPos;
	m_yPos = yPos;
	m_Back.SetXY(xPos, yPos);
	m_Cancel.SetXY(m_xPos + m_Back.GetWidth() - 20, m_yPos + 3);
	m_title.SetXCenter(m_xPos+130);
	m_title.SetY(m_yPos+3);
	m_title.UpdateXCenter();
	m_Name.SetXY(m_xPos+30,m_yPos+30);
	m_Money.SetXY(m_xPos+155,m_yPos+30);
	int xstart, ystart;
	xstart = m_xPos +22;	
	ystart = m_yPos + 87;
	for (int i = 0; i < 12; i++)
	{
		m_Buttonp[i].SetXY(xstart+(i%3)*80 ,ystart+(i/3) *30);
		m_Text[i].SetXCenter(m_Buttonp[i].GetX() + m_Buttonp[i].GetShowWidth() / 2);
		m_Text[i].SetY(m_Buttonp[i].GetY()+3);
		m_Text[i].UpdateXCenter();
	}
}

BOOL cInterfaceFriendQiangHua::ProcessInput()
{
	if (m_Mode)
	{
		if (!g_pMainState->m_Dialog.m_bRetSelect)
		{
			m_Mode = FALSE;
			switch (g_pMainState->m_Dialog.m_retSelect)
			{
			case 4:
				m_pData->m_bZhuangShi = 0;
				m_pData->m_ZuoQiRanse.partnum = 0;
				m_pData->m_ZhuangShiRanse.partnum = 0;
				m_pData->m_bOnZuoQi = FALSE;
				for (int i = 0; i < 4; i++)
				{
					if (0 == m_pData->m_Name.compare(g_pMainState->m_FightList[i].m_pData->m_TrueName.GetString()))
					{
						sFightList& flist = g_pMainState->m_FightList[i];
						cCharacter* pc = (cCharacter*)flist.m_pData->m_pIndex;
						pc->m_PCData.m_bOnZuoQi = m_pData->m_bOnZuoQi;
						pc->m_PCData.m_ZuoQi.m_ZhuangShi.m_Num = 0;
						flist.m_pData->m_NowPos = -1;
						flist.m_pData->Set(POS_STAND);
					
						break;
					}
				}

				break;
			default:
				if (1)
				{
					m_pData->m_bZhuangShi = 0;
					m_pData->m_bOnZuoQi = FALSE;
					vector<int> list;
					g_pMainState->m_ZuoQi.GetShiPei(list, m_pData->m_modeid);
					
					m_pData->m_bOnZuoQi = TRUE;
					m_pData->m_ZuoQiID = list[g_pMainState->m_Dialog.m_retSelect];
					m_pData->m_ZuoQiRanse.partnum = 0;
					m_pData->m_ZhuangShiRanse.partnum = 0;
					for (int i = 0; i < 5; i++)
					{
						if (0 == m_pData->m_Name.compare(g_pMainState->m_FightList[i].m_pData->m_TrueName.GetString()))
						{
							sFightList& flist = g_pMainState->m_FightList[i];
							cCharacter* pc = (cCharacter*)flist.m_pData->m_pIndex;
							pc->m_PCData.m_bOnZuoQi = m_pData->m_bOnZuoQi;
							g_pMainState->m_ZuoQi.GetZuoQi(pc->m_PCData, m_pData->m_ZuoQiID);
							pc->m_PCData.m_ZuoQi.m_ZhuangShi.m_Num = 0;
							pc->m_PCData.m_ZuoQiRanSe = m_pData->m_ZuoQiRanse;
							if (m_pData->m_ZuoQiRanse.partnum)
							{
								DWORD color = pc->m_PCData.m_ZuoQi.m_pZuoQi->m_ColorID;			
								g_pMainState->m_ColorManage.Set(color);
								for (int i = 0; i < pc->m_PCData.m_ZuoQiRanSe.partnum; i++)//����Ⱦɫ
								{
									g_pMainState->m_ColorManage.GetColorMatrix(
										i,
										pc->m_PCData.m_ZuoQiRanSe.GetColorChange(i),
										pc->m_PCData.m_ZuoQiRanSe.m_colormatrix[i]);

								}
							}
							flist.m_pData->m_NowPos = -1;
							flist.m_pData->Set(POS_STAND);
							break;
						}
					}

				}
				break;
			}
			
		}
		return TRUE;
	}
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}

	m_Select = -1;
	ResetButton();
	if (m_bMoveMode)//�ƶ�
	{
		m_Select = 106;
		g_pMainState->SetMouseOnMap(FALSE);
		if (!CheckL())m_bMoveMode = FALSE;
		return TRUE;
	}
	if (!isOn(g_xMouse, g_yMouse, m_Back.m_PreShowRect))return TRUE;
	g_pMainState->SetMouseOnMap(FALSE);
	if(CheckOn(&m_Cancel, 100))return TRUE;
	for (int i = 0; i < 12;i++)
	if (CheckOn(&m_Buttonp[i], i))return TRUE;
	m_Select = 0;
	if (CheckRB())return TRUE;
	m_Select = 107;
	if(CheckL())return TRUE;
	return TRUE;
}

BOOL cInterfaceFriendQiangHua::CheckL()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	m_Sleeptime = 3;
	if (m_frampre)
		m_frampre->SetFrame(1);
	
	switch (m_Select)
	{
	case 0:
		g_pMainState->m_InterFaceBuyPet.Init2(m_pData, 0, 0);
		g_pMainState->m_InterFaceBuyPet.OnOff(TRUE);
		m_bUpdateTeam = FALSE;
		break;
	case 1:
		if (1)
		{
			m_bUpdateTeam = TRUE;
			int weapontype[2];
			GetWeaponType(m_pData->m_modeid, weapontype[0],weapontype[1]);
			vector<int> list;
			g_pMainState->m_ItemManage.GetIDlistByLv(list, weapontype[0], g_pCharacter->m_PCData.m_LV);
			vector<int> list2;
			g_pMainState->m_ItemManage.GetIDlistByLv(list2, weapontype[1], g_pCharacter->m_PCData.m_LV);
			sItem2 item;
			item.m_Num = 1;
			g_pMainState->m_SelectFriendItem.SetMode(m_pData, cInterSelectFriendItem::eQHװ��);
			for (int i = 0; i < list.size(); i++)
			{
				item.Set(weapontype[0], list[i]);
				g_pMainState->m_SelectFriendItem.Set(item.GetType(), item.GetID(), item.m_pItemData->m_SmallHead);
			}
			for (int i = 0; i < list2.size(); i++)
			{
				item.Set(weapontype[1], list2[i]);
				g_pMainState->m_InterFaceBuy.Set(item, 100);
				g_pMainState->m_SelectFriendItem.Set(item.GetType(), item.GetID(), item.m_pItemData->m_SmallHead);
			}
			
			g_pMainState->m_SelectFriendItem.OnOff(TRUE);
		}
		break;
	case 2:
		if (1)
		{
			m_bUpdateTeam = TRUE;
			m_Mode = 1;
			g_pMainState->m_Dialog.m_bRetSelect = TRUE;
			vector<string> list;	
			list.push_back(g_pMainState->m_ZuoQi.m_ZuoQi[0].Name);
			list.push_back(g_pMainState->m_ZuoQi.m_ZuoQi[1].Name);
			switch (m_pData->m_modeid)
			{
			case eJuSe������:
			case eJuSe��ң��:
			case eJuSe����Ů:
			case eJuSeӢŮ��:
			case eJuSe������:
				list.push_back(g_pMainState->m_ZuoQi.m_ZuoQi[2].Name);
				list.push_back(g_pMainState->m_ZuoQi.m_ZuoQi[3].Name);
				break;
			case eJuSe��ͷ��:
			case eJuSe��ħ��:
			case eJuSe�Ǿ���:
			case eJuSe������:
			case eJuSeɱ����:
				list.push_back(g_pMainState->m_ZuoQi.m_ZuoQi[4].Name);
				list.push_back(g_pMainState->m_ZuoQi.m_ZuoQi[5].Name);
				break;
			case eJuSe��̫��:
			case eJuSe�����:
			case eJuSe���ʶ�:
			case eJuSe���켧:
			case eJuSe������:
				list.push_back(g_pMainState->m_ZuoQi.m_ZuoQi[6].Name);
				list.push_back(g_pMainState->m_ZuoQi.m_ZuoQi[7].Name);
				break;
			default:OnOff(FALSE); return FALSE;
			}
			g_pMainState->m_Dialog.Reset();
			g_pMainState->m_Dialog.m_pObj = g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData;
			g_pMainState->m_Dialog.m_bRetSelect = TRUE;
			g_pMainState->m_Dialog.SetString("ѡ������", list[0], list[1], list[2], list[3]);
			g_pMainState->m_Dialog.OnOff(TRUE);

		}
		break;
	case 3:
		if (1)
		{
			if (m_pData->m_ZuoQiID < 0)return TRUE;
			m_bUpdateTeam = TRUE;
			vector<int> list;
			list.push_back(m_pData->m_ZuoQiID * 3);
			list.push_back(m_pData->m_ZuoQiID * 3+1);
			list.push_back(m_pData->m_ZuoQiID * 3+2);
			sItem2 item;
			item.m_Num = 1;
			g_pMainState->m_SelectFriendItem.SetMode(m_pData, cInterSelectFriendItem::eQH����װ��);
			for (int i = 0; i < list.size(); i++)
			{
				item.Set(31, list[i]);
				g_pMainState->m_SelectFriendItem.Set(item.GetType(), item.GetID(), item.m_pItemData->m_SmallHead);
			}

			g_pMainState->m_SelectFriendItem.OnOff(TRUE);
		}
		break;
	case 4://Ⱦɫ
		if (1)
		{
			m_bUpdateTeam = TRUE;
			g_pMainState->m_InterfaceRanSe.SetMode(0, m_pData->GetTeamID());
			g_pMainState->m_InterfaceRanSe.OnOff(TRUE);
		}
		break;
	case 5://����Ⱦɫ
		if (1)
		{
			m_bUpdateTeam = TRUE;
			g_pMainState->m_InterfaceRanSe.SetMode(1, m_pData->GetTeamID());
			g_pMainState->m_InterfaceRanSe.OnOff(TRUE);
		}
		break;
	case 6://װ��Ⱦɫ
		if (1)
		{
			m_bUpdateTeam = TRUE;
			g_pMainState->m_InterfaceRanSe.SetMode(2, m_pData->GetTeamID());
			g_pMainState->m_InterfaceRanSe.OnOff(TRUE);
		}
		break;
	case 7://�ٻ���Ⱦɫ
		if (1)
		{
			m_bUpdateTeam = FALSE;
			g_pMainState->m_InterfaceRanSe.SetMode(3, m_pData->GetTeamID());
			g_pMainState->m_InterfaceRanSe.OnOff(TRUE);
		}
		break;
	case 8://��ɫǿ��
		if (1)
		{
			m_bUpdateTeam = FALSE;
			g_pMainState->m_Tags.Add("���û�б�ʹ��,������������Ҫ��ʲô�ټ���....");
			return TRUE;
			/*g_pMainState->m_ChaKan.Set(*m_pData);
			g_pMainState->m_ChaKan.OnOff(TRUE);
			g_pMainState->m_SelectFriendItem.SetMode(m_pData, cInterSelectFriendItem::eQH����);
			for (int i = 0; i < 20; i++)
			{
				g_pMainState->m_SelectFriendItem.Set(-1, i, g_pMainState->m_SkillManager.GetPetSkill(i)->m_BigHead);
			}
			g_pMainState->m_SelectFriendItem.OnOff(TRUE);*/

		}
		break;
	case 9://��ս
		if (1)
		{
			m_bUpdateTeam = FALSE;
			g_pMainState->m_SelectFriendItem.SetMode(m_pData, cInterSelectFriendItem::eQH��ս);
			g_pMainState->m_SelectFriendItem.OnOff(TRUE);
		}
		break;
	case 10://BBǿ��
		if (1)
		{
			m_bUpdateTeam = FALSE;
			g_pMainState->m_Tags.Add("���û�б�ʹ��,������������Ҫ��ʲô�ټ���....");
			return TRUE;
			/*if (m_pData->m_FightBB == -1)
			{
				g_pMainState->m_Tags.Add("�ö���ǰû�г�ս���ٻ���");
				return TRUE;
			}
			g_pMainState->m_ChaKan.Set(m_pData->m_BBList[m_pData->m_FightBB]);
			g_pMainState->m_ChaKan.OnOff(TRUE);
			g_pMainState->m_SelectFriendItem.SetMode(m_pData, cInterSelectFriendItem::eQH����2);
			for (int i = 0; i < 20; i++)
			{
				g_pMainState->m_SelectFriendItem.Set(-1, i, g_pMainState->m_SkillManager.GetPetSkill(i)->m_BigHead);
			}
			g_pMainState->m_SelectFriendItem.OnOff(TRUE);*/
		}
		break;
	case 11://����
		if (1)
		{
			m_bUpdateTeam = FALSE;
			g_pMainState->m_SelectFriendItem.SetMode(m_pData, cInterSelectFriendItem::eQH����);
			g_pMainState->m_SelectFriendItem.OnOff(TRUE);
		}
		break;
	case 106://�ƶ�
		if (1)
		{
			m_Sleeptime = 0;
			g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
			int xDel = g_xMouse - m_Prex;
			int yDel = g_yMouse - m_Prey;
			m_Prex = g_xMouse;
			m_Prey = g_yMouse;
			if (xDel != 0 || yDel != 0)
			{
				Move(m_xPos + xDel, m_yPos + yDel);
			}
		}
		return TRUE;
	case 107://׼���ƶ�
		g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
		m_Prex = g_xMouse;
		m_Prey = g_yMouse;
		m_bMoveMode = TRUE;
		return TRUE;
	case 100:
		m_framprep->SetFrame(1);
		 OnOff(FALSE);
		 break;
	}
	return TRUE;
}

void cInterfaceFriendQiangHua::OnOff(BOOL NeedShow)
{
	g_pMainState->InterfaceSetShow(this, NeedShow);
	if (NeedShow)
	{
		Init();
		m_Cancel.SetFrame(0);
		m_bUpdateTeam = FALSE;
	}
	else
	{
		if (m_bUpdateTeam)
		g_pMainState->m_InterfaceTeam.UpDate();
		m_Mode = 0;
	}
	g_pMainState->m_Canvas.SetRedraw(m_Back.m_PreShowRect);
}

void cInterfaceFriendQiangHua::Set(sFriendList& list)
{
	m_pData = &list;
	Update();
}

BOOL cInterfaceFriendQiangHua::Show(cCanvas* pCanvas)
{
	pCanvas->Add(&m_Back, 1, 1);
	pCanvas->Add(&m_Cancel, 1);
	pCanvas->Add(&m_title);
	pCanvas->Add(&m_Name);
	//pCanvas->Add(&m_Money);
	for (int i = 0; i < 12; i++)
	{
		pCanvas->Add(&m_Buttonp[i], 1);
		pCanvas->Add(&m_Text[i]);
	}
	return TRUE;
}

BOOL cInterfaceFriendQiangHua::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	switch (m_Select)
	{
	case 0:
		OnOff(FALSE);
		break;
	case 1:
		break;
	default:
		break;
	}

	return TRUE;
}

void cInterfaceFriendQiangHua::Update()
{
	m_Name.SetString(m_pData->m_Name);
	/*ostringstream oss;
	oss << "�ֽ�:";
	oss << m_pData->m_money;
	m_Money.SetString(oss.str());*/
	Move(m_xPos, m_yPos);
}



BOOL cInterFaceBuyPet::Init()
{
	if (m_bInit)return TRUE;
	m_bInit = TRUE;
	m_NumBuy.SetMaxNum(1);
	m_PriceTotal = 0;
	m_Left.Load(0xB05126E8);
	m_Left.SetStatic(TRUE);
	m_Right.Load(0x26066130);
	m_Right.SetStatic(TRUE);

	m_SelectMark.Load(0x10921CA7);
	m_TrueSelect = -1;
	m_Button.Load(0x35A981D3);
	m_Button.SetStatic(TRUE);
	m_MouseOnMark.Load(0x6F88F494);
	m_MouseOnMark.m_NeedShow = FALSE;
	m_Back.Load(3929029437);
	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;
	m_Text[4].SetColor(RGB(240, 240, 240));
	m_Text[4].SetString("ȷ��");
	m_Text[1].SetColor(0);

	m_Text[5].SetColor(RGB(240, 240, 240));
	m_Text[5].SetString("����");

	ifstream file;
	ostringstream oss;
	oss << g_strMediaPath;
	oss << "����//�ٻ����̵�//�ٻ����б�.txt";
	file.open(oss.str());
	int lvlimit, num;
	for (int i = 0; i < 17; i++)
	{
		file >> lvlimit;
		file >> num;
		vector<sShangPinPet>&  list = m_ShangPinList[i];
		list.resize(num);
		for (int i2 = 0; i2 < num; i2++)
			list[i2].Read(file);
	}
	file.close();

	Move(200, 30);
	return TRUE;
}

BOOL cInterFaceBuyPet::Show(cCanvas* pCanvas)
{
	if (!m_NeedShow)return TRUE;
	pCanvas->Add(&m_Back, 1, TRUE);

	for (int i = 0; i < m_ItemNum; i++)
	{
		pCanvas->Add(&m_ItemHead[i], 1);
	}
	pCanvas->Add(&m_Cancel, 1);
	pCanvas->Add(&m_Button, 1);

	if (m_Select > -1)
	{
		pCanvas->Add(&m_MouseOnMark, 1);
	}

	if (m_TrueSelect>-1)
	{
		pCanvas->Add(&m_SelectMark, 1);
	}
	for (int i = 0; i < 6; i++)
	{
		if (i == 1)continue;
		pCanvas->Add(&m_Text[i]);
	}
	m_NumBuy.Show(pCanvas);
	pCanvas->Add(&m_Left, 1);
	pCanvas->Add(&m_Right, 1);
	return TRUE;
}

BOOL cInterFaceBuyPet::ProcessInput()
{
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}
	if (m_NumBuy.Process())
	{
		CoutTotal();
		return TRUE;
	}
	m_MouseOnMark.m_NeedShow = FALSE;
	g_pMainState->SetMouseOnMap(FALSE);
	m_Select = -1;
	ResetButton();

	if (m_bMoveMode)//�ƶ�
	{
		m_Select = 106;
		g_pMainState->SetMouseOnMap(FALSE);
		if (!CheckL())m_bMoveMode = FALSE;
		return TRUE;
	}

	if (!isInRect(g_xMouse, g_yMouse, &m_Back))return TRUE;
	if (CheckOn(&m_Left, 102))return TRUE;
	if (CheckOn(&m_Right, 103))return TRUE;
	if (isOn(g_xMouse, g_yMouse, m_Rect))
	{
		m_Select = (g_xMouse - m_Rect.left) / 51 + 5 * ((g_yMouse - m_Rect.top) / 51);
		if (m_Select < m_ItemNum)
		{
			m_MouseOnMark.m_NeedShow = TRUE;
			m_MouseOnMark.SetXY(m_ItemHead[m_Select].GetX(), m_ItemHead[m_Select].GetY()-7);
			if (CheckL())return TRUE;
			if (CheckRB())return TRUE;
		}
	}
	//ȡ����ť
	if (CheckOn(&m_Cancel, 100))return TRUE;
	//ȷ����ť
	if (CheckOn(&m_Button, 101))return TRUE;
	m_Select = 100;
	if (CheckRB())return TRUE;
	//׼���ƶ�
	m_Select = 107;
	CheckL();
	return TRUE;
}

BOOL cInterFaceBuyPet::Set(sShangPinPet& item, int rate)
{
	if (20 == m_ItemNum)return FALSE;
	m_pricerate[m_ItemNum] = rate;
	m_Item[m_ItemNum] = item;
	DWORD headid= g_pMainState->m_PetDataManager.m_PetList[item.m_modeid].GetID(0,POS_BIGHEAD);
	m_ItemHead[m_ItemNum].Load(headid);
	m_ItemNum += 1;
	return TRUE;
}



BOOL cInterFaceBuyPet::Move(int x, int y)
{
	m_xPos = x;
	m_yPos = y;

	m_Text[0].SetXY(m_xPos + 135, m_yPos + 287);
	RECT rect;
	rect.left = m_xPos + 135;
	rect.top = m_yPos + 310;
	rect.right = rect.left + 75;
	rect.bottom = rect.top + 18;
	m_NumBuy.Init(rect);
	m_Text[2].SetXY(m_xPos + 135, m_yPos + 333);
	m_Text[3].SetXY(m_xPos + 135, m_yPos + 356);

	m_Back.SetXY(x, y);
	m_Cancel.m_x = m_xPos + 256;
	m_Cancel.m_y = m_yPos + 10;
	m_Button.m_Picture.m_x = m_xPos + 131;
	m_Button.m_Picture.m_y = m_yPos + 392;
	m_Text[4].SetXCenter(m_Button.GetCenter());
	m_Text[4].SetY(m_Button.GetY() + 3);
	m_Text[4].UpdateXCenter();
	m_Text[5].SetXCenter(m_xPos + 140);
	m_Text[5].SetY(m_yPos + 10);
	m_Text[5].UpdateXCenter();
	m_Rect.left = 15 + m_xPos;
	m_Rect.top = 48 + m_yPos;
	m_Rect.right = m_Rect.left + 255;
	m_Rect.bottom = m_Rect.top + 204;
	for (int i = 0; i < 20; i++)
	{
		int xNum = i % 5;
		int yNum = i / 5;
		m_ItemHead[i].SetXY(xNum * 51 + m_Rect.left, yNum * 51 + m_Rect.top);
	}
	if (m_TrueSelect>-1)
		m_SelectMark.m_Picture.SetXY(m_ItemHead[m_TrueSelect].GetX(), m_ItemHead[m_TrueSelect].GetY()-7);

	m_Left.SetXY(m_xPos+ 20,m_yPos+ 252);
	m_Right.SetXY(m_xPos+ 70,m_yPos+ 252);

	return TRUE;
}

BOOL cInterFaceBuyPet::CheckL()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	m_Sleeptime = 3;
	switch (m_Select)
	{
	case 106://�ƶ�
		if (1)
		{
			g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
			int xDel = g_xMouse - m_Prex;
			int yDel = g_yMouse - m_Prey;
			m_Prex = g_xMouse;
			m_Prey = g_yMouse;
			if (xDel != 0 || yDel != 0)
			{
				Move(m_xPos + xDel, m_yPos + yDel);
			}
			m_Sleeptime = 0;
		}
		return TRUE;
	case 107://׼���ƶ�
		g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
		m_Prex = g_xMouse;
		m_Prey = g_yMouse;
		m_bMoveMode = TRUE;
		m_Sleeptime = 0;
		return TRUE;
	case 100:
		OnOff(FALSE);
		break;
	case 101:
		if (1)
		{
			m_Button.SetFrame(1);
			if (m_NumBuy.m_num == 0)return TRUE;
			int stronglv;
			
			if (m_TrueSelect < 0)return TRUE;
			if (m_plist)
			{
				if (m_plist->m_BBList.size()>7)
				{
					g_pMainState->m_Tags.Add("�޷��������ٻ���,���ȷ���");
					return TRUE;
				}
			}
			else
			{
				if (g_pCharacter->m_PCData.m_NumofBB > 7)
				{
					g_pMainState->m_Tags.Add("�޷��������ٻ���,���ȷ���");
					return TRUE;
				}
			}
			int* money;
			if (!m_plist)
			{
				money = &g_pCharacter->m_PCData.m_Money[0];
				stronglv = g_pCharacter->m_PCData.m_StrongLv;
			}
			else
			{
				money = &m_plist->m_money;
				stronglv = m_plist->m_stronglv;
			}
			*money -= m_PriceTotal;
			if (*money < 0)
			{
				
				*money += m_PriceTotal;
				g_pMainState->m_Tags.Add("�ֽ𲻹�");
				return TRUE;
			}
			sPetData* ppetdata2 = g_pMainState->m_PetDataManager.GetPetData(m_Item[m_TrueSelect].m_modeid);
			m_Item[m_TrueSelect].m_zizhi.Set(ppetdata2->m_ZiZhi, stronglv, ppetdata2->m_LVLimit);
			UpdateText();
			ostringstream oss;
			oss << "����";
			oss << m_NumBuy.m_num;
			oss << "ֻ";
			oss << m_Item[m_TrueSelect].m_name;
			oss << "������";
			oss << m_PriceTotal;
			oss << "��";
			g_pMainState->m_Tags.Add(oss.str());


			if (m_plist)
			{
				m_plist->m_BBList.push_back(m_Item[m_TrueSelect]);
				m_plist->m_FightBB = m_plist->m_BBList.size() - 1;
				g_pMainState->m_Friend.m_friendqianghua.Update();
			}


			if ((!m_plist) || m_plist->CheckInTeam())
			{
				string objname;
				if (m_plist)
				{
					objname  = m_plist->m_Name;
				}
				else
				{
					objname = g_pCharacter->m_PCData.m_Name;
				}
				UpDateTeam(objname, stronglv);
				
			}
		}
		break;
	case 102://��
		if (1)
		{
			m_frampre->SetFrame(1);
			SubPage();
		}
		break;
	case 103://��
		if (1)
		{
			m_frampre->SetFrame(1);
			AddPage();
		}
		break;
	default:
		if (1)
		{
			if (m_TrueSelect >= m_ItemNum)
			{
				m_NumBuy.Reset();
				return TRUE;
			}
			if (m_Select == m_TrueSelect)
			{
				m_NumBuy.SetNum(m_NumBuy.m_num + 1);
				m_NumBuy.SetActive(TRUE);
			}
			else
			{
				m_TrueSelect = m_Select;
				m_NumBuy.SetNum(1);
				m_NumBuy.SetActive(TRUE);
			}
			CoutTotal();
			m_SelectMark.m_Picture.SetXY(m_ItemHead[m_TrueSelect].GetX(), m_ItemHead[m_TrueSelect].GetY()-7);
		}
		break;
	}
	return TRUE;
}

void cInterFaceBuyPet::OnOff(BOOL bshow)
{
	if (m_NeedShow == bshow)return;
	g_pMainState->InterfaceSetShow(this, bshow);
	if (m_NeedShow)
	{
		Init();
		m_TrueSelect = -1;
		UpdateText();
		m_Sleeptime = 0;
	}
	g_pMainState->m_Canvas.SetRedraw(m_Back.m_PreShowRect);

}

void cInterFaceBuyPet::UpdateText()
{
	ostringstream  oss;
	int price;
	if (m_TrueSelect < 0)
	{
		m_NumBuy.Reset();
		m_PriceTotal = 0;
		price = 0;
	}
	else
	{
		price = m_Item[m_TrueSelect].m_price;
		if (m_pricerate[m_TrueSelect] != 100)
			price = price*m_pricerate[m_TrueSelect] / 100;
	}
	m_Text[0].SetString(price);
	m_Text[2].SetString(m_PriceTotal);
	if (m_plist)
	m_Text[3].SetString(m_plist->m_money);
	else  	m_Text[3].SetString(g_pCharacter->m_PCData.m_Money[0]);

}

BOOL cInterFaceBuyPet::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	switch (m_Select)
	{
	case 100:
		OnOff(FALSE);
		break;
	default:
		g_pMainState->m_ChaKan.Set(m_Item[m_Select]);
		g_pMainState->m_ChaKan.OnOff(TRUE);
		break;
	}
	return TRUE;
}

void cInterFaceBuyPet::CoutTotal()
{
	if (-1 == m_TrueSelect)ERRBOX;
	int money;
	if (m_plist)
	money	= m_plist->m_money;
	else money = g_pCharacter->m_PCData.m_Money[0];
	int price = m_Item[m_TrueSelect].m_price;
	if (m_pricerate[m_TrueSelect] != 100)
		price = price*m_pricerate[m_TrueSelect] / 100;
	m_PriceTotal = m_NumBuy.m_num*price;
	if (m_PriceTotal > money)
	{
		m_NumBuy.SetNum((money / price));
		m_PriceTotal = m_NumBuy.m_num*price;
	}
	UpdateText();
}

void cInterFaceBuyPet::Init2(sFriendList* plist, UINT page1, UINT page2)
{
	Init();
	m_ItemNum = 0;
	m_plist = plist;
	m_page = page1;
	m_page2 = page2;
	m_TrueSelect = -1;
	UINT lv = (g_pCharacter->m_PCData.m_LV-5) / 10;
	if (lv > 16)lv = 16;
	lv -= page2;
	vector<sShangPinPet>& list = m_ShangPinList[lv];
	for (int i = page1 * 20; i < list.size(); i++)
	{
		if (!Set(list[i], 100)) break;
	}
	UpdateText();
}

void cInterFaceBuyPet::AddPage()
{
	UINT lv = (g_pCharacter->m_PCData.m_LV-5) / 10;
	if (lv>16)lv = 16;
	if (lv<0)lv = 0;
	lv -= m_page2;
	vector<sShangPinPet>& list = m_ShangPinList[lv];
	if (list.size() - m_page * 20 > 20)
	{
		m_page += 1;
	}
	else
	{
		if (lv > 0)
			m_page2 += 1;
		else
			return;
	}
	Init2(m_plist, m_page, m_page2);

}
void cInterFaceBuyPet::SubPage()
{
	/*UINT lv = (g_pCharacter->m_PCData.m_LV+5) / 10;
	if (lv > 17)lv = 17;
	lv -= m_page2;
	vector<sShangPinPet>& list = m_ShangPinList[lv];	*/
	if (m_page > 0)
	{
		m_page -= 1;
	}
	else
	{
		if (m_page2 > 0)
			m_page2 -= 1;
		else
			return;
	}
	Init2(m_plist, m_page, m_page2);

}

void cInterFaceBuyPet::UpDateTeam(string name,int stronglv,int lv)
{
	for (int i = 0; i < 5; i++)
	{
		if (g_pMainState->m_FightList[i].isLive)
		{
			cObj* obj = g_pMainState->m_FightList[i].m_pData;
			cCharacter* pc = (cCharacter*)obj->m_pIndex;
			if (0 == obj->m_TrueName.GetString().compare(name))
			{
				cPet* ppet = new cPet;
				ppet->Set(&m_Item[m_TrueSelect], stronglv);
				if (i != g_pMainState->m_HeroID)
				{
					ppet->m_PetDataZZ.Followlv(lv);
					int jinjienum = (g_pCharacter->m_PCData.m_LV + 10) / 60;
					if (jinjienum)
						ppet->m_PetDataZZ.SetJinJie(jinjienum);
				}
				else
				{
					ppet->m_PetDataZZ.LeveUp(lv);
					int lvadd = ppet->m_PetDataZZ.m_LV - ppet->m_PetDataZZ.m_FuZhuSkillLvList[0];
					for (int i = 0; i < 3; i++)
						ppet->m_PetDataZZ.lvUpFuZhuSkill(lvadd, i);
					ppet->m_PetDataZZ.FillHPMP();
				}
				g_pMainState->GivePcAPet(i, ppet);
				g_pMainState->SetFightPet(i, pc->m_PCData.m_NumofBB - 1);
			}
		}
	}
}

void cInterFaceBuyPet::Init3()
{
	if (m_bInit3)return;
	m_bInit3 = TRUE;
	ifstream File;
	ostringstream oss;
	oss << g_strMediaPath;
	oss << "����//�ٻ����̵�//�����ٻ����б�.txt";
	File.open(oss.str());
	int num;
	File >> num;
	m_ShangPinList3.resize(num);
	string dustbin;
	for (int i = 0; i < num; i++)
	{
		 m_ShangPinList3[i].Read(File);
	}
	File.close();
}

sShangPinPet& cInterFaceBuyPet::GetPet3(int id)
{
	Init3();
	return m_ShangPinList3[id];
}

void sShangPinPet::Read(ifstream& File)
{
	File >> m_modeid;
	File >> m_name;
	int skillnum;
	File >> skillnum;
	if (skillnum == -1)
	{
		sPetData* p= g_pMainState->m_PetDataManager.GetPetData(m_modeid);
		skillnum = p->m_SkillNum;
		m_skillid.resize(skillnum);
		for (int i = 0; i < skillnum; i++)
		{
			m_skillid[i] = p->m_pPetSkill[i]->m_Id;
		}
	}
	else
	{
		m_skillid.resize(skillnum);
		for (int i = 0; i < skillnum; i++)
		{
			File >> m_skillid[i];
		}
	}
	File >> m_price;
	int type;
	File >> type;
	switch (type)
	{
	case 0:m_addpoint.Set(4, 0, 0, 1, 0); break;
	case 1:m_addpoint.Set(0, 0, 4, 1, 0); break;
	case 2:m_addpoint.Set(0, 0, 0, 5, 0); break;
	default:ERRBOX;
	}
	sPetData& data = g_pMainState->m_PetDataManager.m_PetList[m_modeid];
	m_zizhi.Set(data.m_ZiZhi, 50, data.m_LVLimit);
	for (int i = 0; i < 4;i++)
		m_ranse.m_ranse[i].partnum  = 0;

}
void sShangPinPet::Save(ofstream& File)
{
	File << " ";
	File << m_modeid<<" ";
	File << m_name<<" ";
	File << m_skillid.size()<<" ";
	for (int i = 0; i < m_skillid.size(); i++)
	{
		File << m_skillid[i]<<" ";
	}
	File << m_price<<" ";
	m_addpoint.Save(File);
	m_zizhi.Save(File);
	m_ranse.Save(File);
	File << "\n";
}
void sShangPinPet::Load(ifstream& File,int postion)
{
	File >> m_modeid;
	File >> m_name  ;
	int num;
	File >> num;
	m_skillid.resize(num);
	for (int i = 0; i < m_skillid.size(); i++)
	{
		File >> m_skillid[i];
	}
	File >> m_price;
	m_addpoint.Load(File);
	m_zizhi.Load(File);
	m_ranse.Read(m_modeid, File, postion);
}



void sShangPinPet::Set(int modeid, sAddPoint* addpoint, int stronglv, BOOL needskill)
{
	if (!addpoint)
	{
		m_addpoint.Set(4, 0, 0, 1, 0);
	}
	else
	m_addpoint = *addpoint;
	sPetData* ppet= g_pMainState->m_PetDataManager.GetPetData(modeid);
	m_zizhi.Set(ppet->m_ZiZhi, stronglv, ppet->m_LVLimit);
	m_modeid = modeid;
	m_name = ppet->m_Name;
	m_price = 0;
	m_ranse.Reset();
	m_skillid.clear();
	if (needskill)
	for (int i = 0; i < ppet->m_SkillNum; i++)
		m_skillid.push_back(ppet->m_pPetSkill[i]->m_Id);
}

void sShangPinPet::AddSkill(int id)
{
	m_skillid.push_back(id);
}

void sShangPinPet::LoadEasy(int id,int strong)
{
	*this = g_pMainState->m_InterFaceBuyPet.GetPet3(id);
	m_zizhi.Set(m_zizhi, strong, g_pMainState->m_PetDataManager.GetPetData(m_modeid)->m_LVLimit);
}








BOOL cInterSelectFriendItem::Init()
{
	if (m_bInit)return TRUE;
	m_bInit = TRUE;
	m_MouseOnMark.Load(0x6F88F494);
	m_MouseOnMark.m_NeedShow = FALSE;
	m_Back.Load(0xE3AEF018);
	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;
	m_TitleT.SetString("ѡ����Ʒ");
	m_TitleT.SetColor(RGB(255, 255, 255));
	Move(200, 30);
	return TRUE;
}

BOOL cInterSelectFriendItem::Show(cCanvas* pCanvas)
{
	if (!m_NeedShow)return TRUE;
	pCanvas->Add(&m_Back, 1, TRUE);
	pCanvas->Add(&m_TitleT);
	for (int i = 0; i < m_ItemNum; i++)
	{
		pCanvas->Add(&m_ItemHead[i], 1);
	}
	pCanvas->Add(&m_Cancel, 1);

	if (m_Select > -1)
	{
		pCanvas->Add(&m_MouseOnMark, 1);
	}
	return TRUE;
}

BOOL cInterSelectFriendItem::ProcessInput()
{
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}

	m_MouseOnMark.m_NeedShow = FALSE;

	m_Select = -1;
	ResetButton();

	if (m_bMoveMode)//�ƶ�
	{
		m_Select = 106;
		g_pMainState->SetMouseOnMap(FALSE);
		if (!CheckL())m_bMoveMode = FALSE;
		return TRUE;
	}

	if (!isInRect(g_xMouse, g_yMouse, &m_Back))return TRUE;
	g_pMainState->SetMouseOnMap(FALSE);
	if (isOn(g_xMouse, g_yMouse, m_Rect))
	{
		m_Select = (g_xMouse - m_Rect.left) / 51 + 5 * ((g_yMouse - m_Rect.top) / 51);
		if (m_Select < m_ItemNum)
		{
			m_MouseOnMark.m_NeedShow = TRUE;
			m_MouseOnMark.SetXY(m_ItemHead[m_Select].GetX(), m_ItemHead[m_Select].GetY());
			if (CheckL())return TRUE;
		}
	}
	//ȡ����ť
	if (CheckOn(&m_Cancel, 100))return TRUE;

	m_Select = 0;
	if (CheckRB())return TRUE;
	//׼���ƶ�
	m_Select = 107;
	CheckL();
	return TRUE;
}

BOOL cInterSelectFriendItem::Set(int itemtype, int itemid, DWORD headid)
{
	if (m_ItemNum > 19)return FALSE;
	m_ItemType[m_ItemNum] = itemtype;
	m_ItemID[m_ItemNum] = itemid;
	m_ItemHead[m_ItemNum].Load(headid);
	m_ItemNum += 1;
	return TRUE;
}

BOOL cInterSelectFriendItem::Move(int x, int y)
{

	m_xPos = x;
	m_yPos = y;
	m_Back.SetXY(x, y);
	m_Cancel.SetXY(x + 272, y + 5);
	m_TitleT.SetXY(x + 110, y + 5);

	for (int i = 0; i < 20; i++)
	{
		int xNum = (i) % 5;
		int yNum = (i) / 5;
		m_ItemHead[i].SetXY(xNum * 51 + 20 + x, yNum * 51 + 35 + y);
	}
	m_Rect.left = 20 + x;
	m_Rect.top = 35 + y;
	m_Rect.right = m_Rect.left + 255;
	m_Rect.bottom = m_Rect.top + 204;

	return TRUE;
}

BOOL cInterSelectFriendItem::CheckL()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	m_Sleeptime = 3;
	switch (m_Select)
	{
	case 106://�ƶ�
		if (1)
		{
			g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
			int xDel = g_xMouse - m_Prex;
			int yDel = g_yMouse - m_Prey;
			m_Prex = g_xMouse;
			m_Prey = g_yMouse;
			if (xDel != 0 || yDel != 0)
			{
				Move(m_xPos + xDel, m_yPos + yDel);
			}
			m_Sleeptime = 0;
		}
		return TRUE;
	case 107://׼���ƶ�
		g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
		m_Prex = g_xMouse;
		m_Prey = g_yMouse;
		m_bMoveMode = TRUE;
		m_Sleeptime = 0;
		return TRUE;
	case 100:
		OnOff(FALSE);
		break;
	default:
		if (1)
		{
			switch (m_Mode)
			{
			case cInterSelectFriendItem::eQHװ��:
				if (1)
				{
					g_pMainState->m_SelectFriendItem.GetSelect(m_pdata->m_weapontype, m_pdata->m_weaponid);
					g_pMainState->m_Tags.Add("���װ���趨");
				}
				break;
			case cInterSelectFriendItem::eQH����:
				/*if (1)
				{
					int skillid,dustbin;
					g_pMainState->m_SelectFriendItem.GetSelect(dustbin, skillid);
					sSkill* skill= g_pMainState->m_SkillManager.GetPetSkill(skillid);
					
					for (int i = 0; i < m_pdata->m_PetSkillList.size(); i++)
					{
						if (skill->m_Id == m_pdata->m_PetSkillList[i])
						{
							g_pMainState->m_Tags.Add("�Ѿ��������������");
							return TRUE;
						}
						for (int j = 0; j < skill->m_NumUnCompatible; j++)
						{
							if (skill->m_UnCompatible[j] == m_pdata->m_PetSkillList[i])
							{
								string text;
								text = "���ڲ�����,";
								text+= g_pMainState->m_SkillManager.GetPetSkill(m_pdata->m_PetSkillList[i])->m_Name;
								text += "��������";
								m_pdata->m_PetSkillList[i] = skill->m_Id;

								g_pMainState->m_Tags.Add(text);
								g_pMainState->m_ChaKan.Set(*m_pdata);
								return TRUE;
							}
						}
					}

					m_pdata->m_PetSkillList.push_back(skillid);
					g_pMainState->m_Tags.Add("�õ����µļ���");
					g_pMainState->m_ChaKan.Set(*m_pdata);
				}*/
				break;
			case cInterSelectFriendItem::eQH����2:
				if (1)
				{
					int skillid, dustbin;
					g_pMainState->m_SelectFriendItem.GetSelect(dustbin, skillid);
					sSkill* skill = g_pMainState->m_SkillManager.GetPetSkill(skillid);

					for (int i = 0; i < m_Pet->m_skillid.size(); i++)
					{
						if (skill->m_Id == m_Pet->m_skillid[i])
						{
							g_pMainState->m_Tags.Add("�Ѿ��������������");
							return TRUE;
						}
						for (int j = 0; j < skill->m_NumUnCompatible; j++)
						{
							if (skill->m_UnCompatible[j] == m_Pet->m_skillid[i])
							{
								string text;
								text = "���ڲ�����,";
								text += g_pMainState->m_SkillManager.GetPetSkill(m_Pet->m_skillid[i])->m_Name;
								text += "��������";
								m_Pet->m_skillid[i] = skill->m_Id;

								g_pMainState->m_Tags.Add(text);
								g_pMainState->m_ChaKan.Set(m_pdata->m_BBList[m_pdata->m_FightBB]);
								return TRUE;
							}
						}
					}

					m_Pet->m_skillid.push_back(skillid);
					g_pMainState->m_Tags.Add("�õ����µļ���");
					g_pMainState->m_ChaKan.Set(m_pdata->m_BBList[m_pdata->m_FightBB]);
				}
				break;

			case cInterSelectFriendItem::eQH����װ��:
				if (1)
				{
					int dustbin;
					m_pdata->m_bZhuangShi = TRUE;
					m_pdata->m_ZhuangShiRanse.partnum=0;
					g_pMainState->m_SelectFriendItem.GetSelect(dustbin, m_pdata->m_ZhuangShiID);
					g_pMainState->m_Tags.Add("���װ���趨");
				}
				break;
			case eQH��ս:
				if (1)
				{
					m_pdata->m_FightBB = m_Select;	
				}
				break;
			case eQH����:
				if (1)
				{
					string text = "������"; 
					text += m_pdata->m_BBList[m_Select].m_name;
					g_pMainState->m_Tags.Add(text);
					if (m_Select < m_pdata->m_FightBB)
						m_pdata->m_FightBB -= 1;
					else
					if (m_Select == m_pdata->m_FightBB)
					{
						m_pdata->m_FightBB = m_pdata->m_BBList.size() - 2;
					}

					for (int i = m_Select; i < m_ItemNum-1; i++)
					{
						 m_ItemID[i]=m_ItemID[i+1];
						 m_ItemHead[i].Load(m_ItemHead[i+1].m_NowID);
					}
					m_ItemNum -= 1;
					g_pMainState->m_Canvas.SetRedraw(m_Back.m_PreShowRect);
					m_pdata->m_BBList.erase(m_pdata->m_BBList.begin() + m_Select);


				}
				break;
			default:
				ERRBOX;
				break;
			}
			int teamid = m_pdata->GetTeamID();
			if (teamid > -1)
			{
				cObj* obj = g_pMainState->m_FightList[teamid].m_pData;
				cCharacter* pc = (cCharacter*)obj->m_pIndex;
				switch (m_Mode)
				{
				case cInterSelectFriendItem::eQHװ��:
					if (1)
					{
						pc->m_PCData.m_Equip[2].Set(m_pdata->m_weapontype, m_pdata->m_weaponid);
						pc->m_PCData.m_pCharacterData2 = 0;
					}
					break;
				case cInterSelectFriendItem::eQH����:
					g_pMainState->m_SkillManager.SetPetSkill(&pc->m_PCData, m_ItemID[m_Select]);
					break;
				case cInterSelectFriendItem::eQH����2:
					if (1)
					{
						cPetData* ppetdata;
						g_pMainState->m_FightList[teamid + 5].m_pData->GetDateP(ppetdata);
						g_pMainState->m_SkillManager.SetPetSkill(ppetdata, m_ItemID[m_Select]);
					}
					break;
				case cInterSelectFriendItem::eQH����װ��:
					if (1)
					{
						pc->m_PCData.m_ZuoQi.m_ZhuangShi.m_Num = 1;
						pc->m_PCData.m_ZuoQi.m_ZhuangShi.Set(31, m_pdata->m_ZhuangShiID);

					}
					break;
				case eQH��ս:
					if (1)
					{
						string text = m_pdata->m_BBList[m_pdata->m_FightBB].m_name;
						if (g_pMainState->SetFightPet(teamid, m_Select))// m_ItemID[m_Select]);
						{
							text += "���趨Ϊ��ս�ٻ���";
						}
						else
						{
							text = "ʧ��";
						}
						g_pMainState->m_Tags.Add(text);
					}
					break;
				case eQH����:
					if (1)
					{
						if (0 == obj->m_TrueName.GetString().compare(m_pdata->m_Name))
						{
							g_pMainState->SetFightPet(teamid, -1);//
							cCharacter* pc = (cCharacter*)obj->m_pIndex;
							pc->m_PCData.DeletePet(m_Select);
							g_pMainState->SetFightPet(teamid, m_pdata->m_FightBB);//
						}


					}
					break;
				default:
					break;
				}
				obj->m_NowPos = -1;
				obj->Set(POS_STAND);
			}
		}
		break;
	}
	return TRUE;
}

void cInterSelectFriendItem::OnOff(BOOL bshow)
{
	if (m_NeedShow == bshow)return;
	g_pMainState->InterfaceSetShow(this, bshow);
	if (m_NeedShow)
	{
		Init();
	}
	g_pMainState->m_Canvas.SetRedraw(m_Back.m_PreShowRect);
}

BOOL cInterSelectFriendItem::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	switch (m_Select)
	{
	case 0:
		OnOff(FALSE);
		break;
	default:
		break;
	}
	return TRUE;
}

void cInterSelectFriendItem::SetMode(sFriendList* list, eQiangHuaMode mode)
{
	m_pdata = list;
	m_Mode = mode;
	m_ItemNum = 0;
	if (mode == eQH��ս||mode==eQH����)
	{
		for (UINT i = 0; i < list->m_BBList.size(); i++)
		{
			DWORD head= g_pMainState->m_PetDataManager.GetPetData(list->m_BBList[i].m_modeid)->GetID(0,POS_BIGHEAD);
			Set(-1, i, head);
		}
	}
	if (mode == eQH����2)
	{
		m_Pet = &m_pdata->m_BBList[m_pdata->m_FightBB];
	}
}

void cInterfaceUseJiaJu::SetJiaJu(sItem2* jiaju)
{
	m_pJiaJu = jiaju;
	m_JiaJu.m_JiaJu = *jiaju;
	switch (jiaju->m_pItemData->GetLv())
	{
	case 1:
	case 2:
		m_Pic[0].m_PosType = 0;
		break;
	case 4:
	case 5:
		for (int i = 0; i < 6;i++)
			m_Pic[i].m_PosType = 1;
		break;
	default:
		m_Pic[0].m_PosType = 1;
	}
	AddDirection();
	OnOff(TRUE);
}

void cInterfaceUseJiaJu::AddDirection()
{
	sJiaJuData& list = g_pMainState->m_ItemManage.m_JiaJuData[m_JiaJu.m_JiaJu.GetType() - 35][m_JiaJu.m_JiaJu.GetID()];
	m_JiaJu.m_Direction += 1;

	m_JiaJu.m_Direction %= list.num;
	for (int i = 0; i < 6; i++)
	{
		m_Pic[i].m_NeedShow = FALSE;
	}
	switch (m_JiaJu.m_JiaJu.m_pItemData->GetLv())
	{
	case 4:
		if (1)
		{
			if (m_JiaJu.m_Direction == 2)m_JiaJu.m_Direction = 0;
			m_Pic[0].Load(list.m_data[m_JiaJu.m_Direction * 3]);
			m_Pic[1].Load(list.m_data[m_JiaJu.m_Direction * 3 + 1]);
			m_Pic[2].Load(list.m_data[m_JiaJu.m_Direction * 3 + 2]);
			for (int i = 0; i < 3; i++)
			{
				m_Pic[i].m_NeedShow = TRUE;
			}
		}
		break;
	case 5:
		if (1)
		{
			if (m_JiaJu.m_Direction == 2)m_JiaJu.m_Direction = 0;
			if (!m_JiaJu.m_Direction)
			{
				for (int i = 0; i < 6; i++)
				{
					m_Pic[i].Load(list.m_data[i]);
					m_Pic[i].m_NeedShow = TRUE;
				}
			}
			else
			{
				for (int i = 0; i < 5; i++)
				{
					m_Pic[i].Load(list.m_data[6+i]);
					m_Pic[i].m_NeedShow = TRUE;
				}
			}
		}
		break;
	default:
		m_Pic[0].Load(list.m_data[m_JiaJu.m_Direction]);
		m_Pic[0].m_NeedShow = TRUE;
		break;
	}
}

BOOL cInterfaceUseJiaJu::Show(cCanvas* pCanvas)
{
	for (int i = 0; i < 6;i++)
	pCanvas->Add(&m_Pic[i], 1);
	return TRUE;
}

BOOL cInterfaceUseJiaJu::ProcessInput()
{
	g_pMainState->SetMouseOnMap(FALSE);
	if (g_pMainState->Keyboard.GetKeyState(DIK_SPACE))
	{
		g_pMainState->Keyboard.SetLock(DIK_SPACE);
		AddDirection();
	}
	for (int i = 0; i < 6; i++)
		m_Pic[i].SetXY(g_xMouse, g_yMouse);

	if (CheckL())return TRUE;
	if (CheckRB())return TRUE;
	return TRUE;
}

void cInterfaceUseJiaJu::OnOff(BOOL NeedShow)
{
	g_pMainState->InterfaceSetShow(this, NeedShow);
	if (m_JiaJu.m_JiaJu.m_pItemData->GetLv() == 4)
	{
		for (int i = 0; i < 3;i++)
		g_pMainState->m_Canvas.SetRedraw(m_Pic[i].m_PreShowRect);
	}
	else
	g_pMainState->m_Canvas.SetRedraw(m_Pic[0].m_PreShowRect);
}

BOOL cInterfaceUseJiaJu::CheckL()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);

	int xof = g_pMainState->m_Map.m_pMap->GetXCenter() - 320;
	int yof = g_pMainState->m_Map.m_pMap->GetYCenter() - 240;

	
	m_JiaJu.m_xPos = m_Pic[0].GetX() + xof;
	m_JiaJu.m_yPos = m_Pic[0].GetY() + yof;
	if (g_pMainState->m_FangWu.AddJiaJu(m_JiaJu))
	{
		m_pJiaJu->m_Num = 0;
		g_pMainState->m_Map.AddJiaJu(&m_JiaJu);
		g_pMainState->FindObj();
	}
	OnOff(FALSE);
	return TRUE;
}

BOOL cInterfaceUseJiaJu::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	OnOff(FALSE);
	return TRUE;
}

void cFriend::sMessageList::Save(ofstream& File)
{
	File << "\n";
	File << messagePath<<"\n";
	File << id << " ";
	File << p->GetType() << " ";
	File << p->m_HuanShu << " ";
	File << taskid<<" ";
}

void cFriend::sMessageList::Load(ifstream& File)
{
	getline(File,messagePath);
	getline(File, messagePath);
	File >> id;
	int type;
	File >> type;
	p= g_pMainState->FindTaskByID(type);
	File >> p->m_HuanShu;
	File >> taskid;
}

void sFriendList::Save(ofstream& File)
{
	File << "\n";
	m_JinJieNum.Save(File);
	File << m_bDuJie << " ";
	File << m_traderLV << " ";
	File << m_Feeling << " ";
	File << m_Name << " ";
	File << m_dataFileName << " ";
	File << 0 << " ";// m_bOnLine << " ";
	File << m_stronglv << " ";
	File << m_modeid << " ";
	File << m_weapontype << " ";
	File << m_weaponid << " ";
	m_Ranse.Save(File);
	File << m_money << " ";
	File << m_bOnZuoQi << " ";
	File << m_ZuoQiID << " ";
	//����
	m_ZuoQiRanse.Save(File);
	File << m_bZhuangShi << " ";
	File << m_ZhuangShiID << " ";
	m_ZhuangShiRanse.Save(File);
	File << m_BBList.size() << " ";
	for (int i = 0; i < m_BBList.size(); i++)
	{
		m_BBList[i].Save(File);
	}
	File << m_FightBB << "\n";
	//File << 0 << " ";
	/*File << m_PetSkillList.size() << " ";
	for (int i = 0; i < m_PetSkillList.size(); i++)
	{
		File << m_PetSkillList[i] << " ";
	}*/
	//File << "\n";
}

void sFriendList::Load(ifstream& File)
{
	m_JinJieNum.Load(File);
	File >> m_bDuJie;
	File >> m_traderLV;
	File >> m_Feeling;
	File >> m_Name;
	File >> m_dataFileName;
	int dustbin;
	File >> dustbin;// m_bOnLine;
	File >> m_stronglv;
	File >> m_modeid;
	File >> m_weapontype;
	File >> m_weaponid;
	m_Ranse.Read(m_modeid+2000, File, m_JinJieNum.m_PositionID);
	File >> m_money;
	File >> m_bOnZuoQi;
	File >> m_ZuoQiID;
	//����
	m_ZuoQiRanse.Read(File, m_ZuoQiID, sChangeColorData::eZuoQi, 0);
	File >> m_bZhuangShi;
	File >> m_ZhuangShiID;
	m_ZhuangShiRanse.Read(File, m_ZhuangShiID, sChangeColorData::eZhuangShi, 0);
	int bbnum;
	File >> bbnum;
	m_BBList.resize(bbnum);
	/*for (int i = 0; i < 5; i++)
	{
		if (g_pMainState->m_FightList[i].m_pData->m_TrueName.GetString() == m_Name)
		{
			
		}
	}*/

	
	for (int i = 0; i < m_BBList.size(); i++)
	{
		m_BBList[i].Load(File,  (g_pCharacter->m_PCData.m_LV + 10) / 60);
	}
	File >> m_FightBB;
	/*int petskillnum;
	File >> petskillnum;*/
	/*m_PetSkillList.resize(petskillnum);
	for (int i = 0; i < m_PetSkillList.size(); i++)
	{
		File >> m_PetSkillList[i];
	}*/

}
DWORD sFriendList::GetFriendHead()
{
	return g_pMainState->m_pCharacterDataList[m_modeid].m_Head[m_bDuJie].m_FriendHead;
}

DWORD sFriendList::GetDialogHead()
{
	return g_pMainState->m_pCharacterDataList[m_modeid].m_Head[m_bDuJie].m_dialog;
}

BOOL sFriendList::CheckInTeam()
{
	if (GetTeamID() > -1)return TRUE;
	return FALSE;
}

int sFriendList::GetTeamID()
{
	for (int i = 0; i < 5; i++)
	{
		if (!g_pMainState->m_FightList[i].isLive)continue;
		if (g_pMainState->m_FightList[i].m_pData->m_TrueName.GetString() == m_Name)return i;
	}
	return -1;
}



void cInterfaceDaZao::Init()
{
	if (m_bInit)return;
	m_Back.Load(0xABB7D0DB);
	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;
	m_Button.Set(&g_pMainState->m_Button4);
	m_Button.m_bStatic = TRUE;
	m_tButton.SetString("ȷ��");
	m_tButton.SetColor(RGB(255, 255, 255));
	m_tMoneyHave.SetColor(0);
	m_tMoneyNeed.SetColor(0);
	m_Head[0].m_NeedShow = FALSE;
	m_Head[1].m_NeedShow = FALSE;
	Move(320 - m_Back.GetWidth() / 2, 240 - m_Back.GetHeight() / 2);
}

void cInterfaceDaZao::Move(int x, int y)
{
	m_xPos= x;
	m_yPos = y;
	m_Back.SetXY(x, y);
	m_tMoneyNeed.SetXY(x + 208,y + 38);
	m_tMoneyHave.SetXY(x + 208,y + 101);
	m_Cancel.SetXY(x + 270, y + 3);
	m_Head[0].SetXY(x + 17, y + 58);
	m_Head[1].SetXY(x + 74, y + 58);
	m_Button.SetXY(x+m_Back.GetWidth()/2-m_Button.GetWidth()/2,y+356);
	m_tButton.SetY(m_Button.GetY() + 3);
	m_tButton.SetXCenter(m_Button.GetCenter());
	m_tButton.UpdateXCenter();
	g_pMainState->m_ItemSelect.Move(x+20,130);
	if (m_NeedShow)
		g_pMainState->m_ItemSelect.Move(x, y);
}

BOOL cInterfaceDaZao::Show(cCanvas* pCanvas)
{
	if (!m_NeedShow)return TRUE;
	pCanvas->Add(&m_Back, 1,1);
	pCanvas->Add(&m_Cancel, 1);
	pCanvas->Add(&m_tMoneyHave);
	pCanvas->Add(&m_tMoneyNeed);
	for (int i = 0; i < 2;i++)
		pCanvas->Add(&m_Head[i],1);
	g_pMainState->m_ItemSelect.Show(pCanvas);
	pCanvas->Add(&m_Button, 1);
	pCanvas->Add(&m_tButton);
	return TRUE;
}

void cInterfaceDaZao::OnOff(BOOL NeedShow)
{
	g_pMainState->InterfaceSetShow(this, NeedShow);
	if (m_NeedShow)
	{
		Init();
		g_pMainState->m_ItemSelect.Update();
		g_pMainState->m_ItemSelect.Move(m_xPos + 20, m_yPos+130);
		m_Item[0] = -1;
		m_Item[1] = -1;
		m_tMoneyNeed.SetString(0);
		m_MoneyNeed = 0;
		m_tMoneyHave.SetString(g_pCharacter->m_PCData.m_Money[0]);
		;
	}
	else g_pMainState->m_ItemSelect.m_Describe.OnOff(FALSE);

	
}

BOOL cInterfaceDaZao::ProcessInput()
{
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}
	ResetButton();
	m_Select = -1;

	if (m_bMoveMode)//�ƶ�
	{
		m_Select = 6;
		g_pMainState->SetMouseOnMap(FALSE);
		if (!CheckL())m_bMoveMode = FALSE;
		return TRUE;
	}
	if (!isInRect(g_xMouse, g_yMouse, &m_Back))return TRUE;
	g_pMainState->SetMouseOnMap(FALSE);
	if(CheckOn(&m_Cancel,0))return TRUE;
	int k= g_pMainState->m_ItemSelect.Process(g_xMouse, g_yMouse);
	if (k > -1)
	{
		m_Select = k + 10;
		if(CheckL())return TRUE;
	}
	for (int i = 0; i < 2; i++)
	if (m_Head[i].m_NeedShow)
	{
		if (CheckOn(&m_Head[i], i + 1))return TRUE;
	}
	if (CheckOn(&m_Button, 5))return TRUE;
	if (m_bMoveMode)//�ƶ�
	{
		m_Select = 6;
		g_pMainState->SetMouseOnMap(FALSE);
		if (!CheckL())m_bMoveMode = FALSE;
		return TRUE;
	}
	CheckRB();
	return TRUE;
}

BOOL cInterfaceDaZao::CheckL()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
	{
		m_Select = -1;
		return FALSE;
	}
	g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	m_Sleeptime = 3;
	switch (m_Select)
	{
	case 0:
		OnOff(FALSE);
		return TRUE;
	case 1:
	case 2:
		m_Head[m_Select-1].m_NeedShow = FALSE;
		m_Item[m_Select - 1] = -1;
		if (m_MoneyNeed)
		{
			m_tMoneyNeed.SetString(0);
			m_MoneyNeed = 0;
		}
		return TRUE;
	case 5:
		if (m_MoneyNeed)
		{
			if (g_pCharacter->m_PCData.m_Money[0] < m_MoneyNeed)
			{
				g_pMainState->m_Tags.Add("Ǯ����");
				return TRUE;
			}
			int weaponid=0;
			sItem2* pItem = &g_pCharacter->m_PCData.m_Item[m_Item[weaponid]];
			sItem2* pItem2;
			if (pItem->GetType() == 41)
			{
				weaponid = 1;
				pItem = &g_pCharacter->m_PCData.m_Item[m_Item[weaponid]];
			}
			pItem2 = &g_pCharacter->m_PCData.m_Item[m_Item[!weaponid]];
			int lv = g_pMainState->m_ItemManage.GetlvByID(pItem->GetType(),pItem->GetID());
			vector<int>& prop= pItem->m_Property;
			if (0 == prop.size())
			{
				g_pMainState->m_Tags.Add("δ����");
				return TRUE;
			}
			BOOL bSuccess = FALSE;
			string text;
			if (lv < 60)
			{
				text = "60�����ϵ�װ��������Ƕ";
			}
			else	
			if (prop[2] == -1)
			{
				bSuccess = TRUE;
				prop[2] = pItem2->GetID();
			}
			else
			if (prop[3] == -1)
			{
				if (lv > 89)
				{
					if (prop[2] != pItem2->GetID())
					{
						bSuccess = TRUE;
						prop[3] = pItem2->GetID();
					}
					if (!bSuccess)text = "װ��������ͬ�ķ�ʯ";
				}
				else text = "90���µ�װ��ֻ����Ƕ1��";

			}
			else
			if (prop[4] == -1)
			{
				if (lv > 119)
				{
					if (prop[2] != pItem2->GetID())
					if (prop[3] != pItem2->GetID())
					{
						bSuccess = TRUE;
						prop[4] = pItem2->GetID();
					}
					if (!bSuccess)text = "װ��������ͬ�ķ�ʯ";
				}
				else text = "120���µ�װ��ֻ����Ƕ2��";
			}
			else
			{
				text = "����������ʯ";
			}
			if (bSuccess)
			{
				g_pMainState->m_Tags.Add("��Ƕ�ɹ�");	
				g_pCharacter->m_PCData.AddMoney(-m_MoneyNeed);
				if (g_pCharacter->m_PCData.m_Item[m_Item[0]].GetType() == 41)
				{
					g_pCharacter->m_PCData.m_Item[m_Item[0]].m_Num -= 1;
				}
				else g_pCharacter->m_PCData.m_Item[m_Item[1]].m_Num -= 1;
				for (int i = 0; i < 2; i++)
				{
					m_Head[i].m_NeedShow = FALSE;
					m_Item[i] = -1;
				}
				m_tMoneyNeed.SetString(0);
				m_MoneyNeed = 0;
			}
			else
			{
				g_pMainState->m_Tags.Add(text);
			}
		}
		break;
	case 6://�ƶ�
		if (1)
		{
			m_Sleeptime = 0;
			g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
			int xDel = g_xMouse - m_Prex;
			int yDel = g_yMouse - m_Prey;
			m_Prex = g_xMouse;
			m_Prey = g_yMouse;
			if (xDel != 0 || yDel != 0)
			{
				Move(m_xPos + xDel, m_yPos + yDel);
			}
		}
		return TRUE;
	case 7://׼���ƶ�
		g_pMainState->Mouse.UnSetLock(MOUSE_LBUTTON);
		m_Prex = g_xMouse;
		m_Prey = g_yMouse;
		m_bMoveMode = TRUE;
		return TRUE;
	default:
		for (int i = 0; i < 2; i++)
		{
			if (m_Item[i] == -1)
			{
				sItem2* item0 = &g_pCharacter->m_PCData.m_Item[m_Select-10];
				if (!item0->GetNum())return TRUE;
				BOOL  type = CheckEquip(item0->GetType());
				if (!type)
				if (item0->GetType() == 41)type = 1;
				if (type)
				{
					m_Item[i] = m_Select - 10;
					m_Head[i].Set(&g_pMainState->m_ItemSelect.m_ItemHead[m_Select - 10]);
					m_Head[i].m_NeedShow = TRUE;
					if (m_Item[0] > -1 && m_Item[1] > -1)
					{
						sItem2* pItem0 = &g_pCharacter->m_PCData.m_Item[m_Item[0]];
						sItem2* pItem1 = &g_pCharacter->m_PCData.m_Item[m_Item[1]];
						if (pItem0->GetType() == 41)
						{
							if (CheckEquip(pItem1->GetType()))
							{
								m_MoneyNeed= g_pMainState->m_ItemManage.GetlvByID(pItem1->GetType(), pItem1->GetID());
								m_MoneyNeed *= 1000;
								if (!m_MoneyNeed)m_MoneyNeed = 1;
								m_tMoneyNeed.SetString(m_MoneyNeed);
							}
						}
						else if (pItem1->GetType() == 41)
						{
							if (CheckEquip(pItem0->GetType()))
							{
								m_MoneyNeed = g_pMainState->m_ItemManage.GetlvByID(pItem0->GetType(), pItem0->GetID());
								m_MoneyNeed *= 1000;
								if (!m_MoneyNeed)m_MoneyNeed = 1;
								m_tMoneyNeed.SetString(m_MoneyNeed);
							}
						}
					}
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL cInterfaceDaZao::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	OnOff(FALSE);
	return TRUE;
}

BOOL cInterfaceDaZao::CheckEquip(int type)
{
	if (type < 22)return TRUE;
	if (type>31 && type < 35)return TRUE;
	return FALSE;
}
