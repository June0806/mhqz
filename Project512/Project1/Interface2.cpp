#include "global.h"

void cPCHead::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;
	m_TMouseOnLine.SetColor(RGB(255, 255, 255));

	m_TeamFace.resize(5);
	m_TeamBack.resize(5);
	m_TeamPetFace.resize(5);
	
	m_PetFaceBack.Load(0x203CE1F7);
	m_PetFaceBack.SetStatic(TRUE);
	for (int i = 0; i < 5; i++)
	{
		m_TeamBack[i].Load(0x2178F38B);
		m_TeamBack[i].SetStatic(TRUE);
		m_TeamBack[i].m_NeedShow = FALSE;
		m_bPetHeadMode[i] = FALSE;
	}
	MoveTeamHead(0);



	m_CharacterHead.Load(42282575);
	m_CharacterHead.SetStatic(TRUE);
	m_PetHead.Load(1762476724);
	m_PetHead.SetStatic(TRUE);
	m_PCFace.SetX(640 - m_CharacterHead.GetWidth() + 2);
	m_PCFace.SetStatic(TRUE);
	m_PCFace.m_Picture.SetY(3);
	m_PetFace.SetStatic(TRUE);
	m_PetFace.m_NeedShow = FALSE;
	m_CharacterHead.SetX(640 - m_CharacterHead.GetWidth());
	m_CharacterHead.SetY(0);
	m_PetHead.SetX(640 - m_PetHead.GetWidth() - m_CharacterHead.GetWidth());
	m_PetHead.SetY(0);
	m_PetFace.SetX(m_PetHead.GetX() + 3);
	m_PetFace.SetY(3);
	m_PetFaceBack.SetX(m_PetHead.GetX());
	m_PetFaceBack.SetY(0);



	//����Ѫ��
	m_Heart.Load(780622062);
	m_HeartHurt.Load(0x008C2611);

	m_HPlinep.resize(2);
	m_HeartHurtp.resize(2);
	m_MPlinep.resize(2);
	m_EXPlinep.resize(2);

	int offset = m_CharacterHead.GetHeight() / 4;
	int linex = 640 - m_HeartHurt.GetWidth() - 6;  //Ѫ����xλ��
	int linex2 = m_PetFaceBack.GetX() + 50 - 1;       //�ٻ��޵�Ѫ����xλ��
	for (int i = 0; i < 2; i++)
	{
		m_HeartHurtp[i].m_bStatic = TRUE;
		m_HPlinep[i].m_bStatic = TRUE;
		m_MPlinep[i].m_bStatic = TRUE;
		m_EXPlinep[i].m_bStatic = TRUE;
	}
	m_HeartHurtp[0].Set(&m_HeartHurt);
	m_HeartHurtp[0].m_x = linex;
	m_HeartHurtp[0].m_y = 3;
	m_HeartHurtp[1].Set(&m_HeartHurt);
	m_HeartHurtp[1].m_x = linex2;
	m_HeartHurtp[1].m_y = 3;


	m_HPline.Load(0xAAD44583);
	m_HPlinep[0].Set(&m_HPline);
	m_HPlinep[0].m_x = linex;
	m_HPlinep[0].m_y = 3;
	m_HPlinep[1].Set(&m_HPline);
	m_HPlinep[1].m_x = linex2;
	m_HPlinep[1].m_y = 3;


	m_MPline.Load(0xCE4D3C2D);
	m_MPlinep[0].Set(&m_MPline);
	m_MPlinep[0].m_x = linex;
	m_MPlinep[0].m_y = offset * 1 + 2;
	m_MPlinep[1].Set(&m_MPline);
	m_MPlinep[1].m_x = linex2;
	m_MPlinep[1].m_y = offset * 1 + 2;


	m_EXPline.Load(0x7B3C08E4);
	for (int i = 0; i < 2; i++)
	{
		m_EXPlinep[i].m_bStatic = TRUE;
		m_EXPlinep[i].Set(&m_EXPline);
	}
	m_EXPlinep[0].m_x = linex;
	m_EXPlinep[0].m_y = offset * 3 + 4;
	m_EXPlinep[1].m_x = linex2;
	m_EXPlinep[1].m_y = offset * 2 + 3;

	m_CPline.Load(0xBAF8009F);
	m_CPline.SetX(linex);
	m_CPline.SetY((offset)* 2 + 3);
	m_CPline.SetStatic(TRUE);

	m_EmptyLine.Load(0x2E8758EE);
	m_EmptyLinep.resize(7);
	for (int i = 0; i < 7; i++)
	{
		m_EmptyLinep[i].Set(&m_EmptyLine);
		m_EmptyLinep[i].m_bStatic = TRUE;
	}
	int wid2 = 640 - m_EmptyLine.GetWidth();
	for (int i = 0; i < 4; i++)
	{
		m_EmptyLinep[i].m_x = wid2;
		m_EmptyLinep[i].m_y = i*offset;
	}
	for (int i = 4; i < 7; i++)
	{
		m_EmptyLinep[i].m_x = wid2 - m_Heart.GetWidth() - 50 + 1;
		m_EmptyLinep[i].m_y = (i - 4)*offset;
	}
}

BOOL cPCHead::Show(cCanvas* pCanvas)
{
	if (!g_pCharacter)return FALSE;
	if (g_pCharacter->m_PCData.m_NumofBB)
	{
		pCanvas->Add(&m_PetHead, 1, 1);

			pCanvas->Add(&m_PetFaceBack, 1);
			pCanvas->Add(&m_PetFace, 1, 1);
	}
	pCanvas->Add(&m_CharacterHead, 1, 1);
	pCanvas->Add(&m_PCFace, 1, 1);


	if (m_TeamBack[1].m_NeedShow)
	{
		for (int i = 0; i < 5; i++)//��ʾ����ͷ��
		{
			if (m_TeamBack[i].m_NeedShow)
			{
				pCanvas->Add(&m_TeamBack[i], 1);
			}
			else break;
		}
		for (int i = 0; i < 5; i++)//��ʾ����ͷ��
		{
			if (m_TeamBack[i].m_NeedShow)
			{
				if (m_bPetHeadMode[i])
					pCanvas->Add(&m_TeamPetFace[i], 1, 1);
				else
					pCanvas->Add(&m_TeamFace[i], 1, 1);
			}
			else break;
		}
	}

	//��ʾHP
	if (g_pCharacter->m_PCData.m_HP < g_pCharacter->m_PCData.m_HPMax.GetData())
	{
		pCanvas->Add(&m_EmptyLinep[0], 1);
		pCanvas->Add(&m_HeartHurtp[0], 1);
		pCanvas->Add(&m_HPlinep[0], 1);
	}
	//��ʾMP
	if (g_pCharacter->m_PCData.m_MP < g_pCharacter->m_PCData.m_MPMax.GetData())
	{
		pCanvas->Add(&m_EmptyLinep[1], 1);
		pCanvas->Add(&m_MPlinep[0], 1);
	}
	//��ʾCP
	if (g_pCharacter->m_PCData.m_CP < 150)
	{
		pCanvas->Add(&m_EmptyLinep[2], 1);
		pCanvas->Add(&m_CPline, 1);
	}
	//��ʾ���� 
	if (g_pCharacter->m_PCData.EXP<g_pCharacter->m_PCData.ExpMax)
	{
		pCanvas->Add(&m_EmptyLinep[3], 1);
		pCanvas->Add(&m_EXPlinep[0], 1);
	}
	//�������ٻ��޵�Ѫ��
	
	if (m_pPet)
	{
		//��ʾHP
		if (m_pPet->m_PetDataZZ.m_HP < m_pPet->m_PetDataZZ.m_HPMax.GetData())
		{
			pCanvas->Add(&m_EmptyLinep[4], 1);
			pCanvas->Add(&m_HeartHurtp[1], 1);
			pCanvas->Add(&m_HPlinep[1], 1);
		}
		//��ʾMP
		if (m_pPet->m_PetDataZZ.m_MP < m_pPet->m_PetDataZZ.m_MPMax.GetData())
		{
			pCanvas->Add(&m_EmptyLinep[5], 1);
			pCanvas->Add(&m_MPlinep[1], 1);
		}
		//��ʾ���� 
		if (m_pPet->m_PetDataZZ.EXP < m_pPet->m_PetDataZZ.ExpMax)
		{
			pCanvas->Add(&m_EmptyLinep[6], 1);
			pCanvas->Add(&m_EXPlinep[1], 1);
		}
	}
	if (-1 != m_MouseOnPre)pCanvas->Add(&m_TMouseOnLine);
	return TRUE;
}

BOOL cPCHead::ProcessInput()
{
	if (!g_pMainState->GetLockMouse())return TRUE;
	if (!m_pPc)return TRUE;
	m_MouseOn = -1;
	if (g_yMouse < 50)
	if (g_xMouse > m_PetHead.GetX())
	if (g_xMouse<640 && g_yMouse>0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		if (g_xMouse > 570)
		{
			m_MouseOn = g_yMouse / 12;
		}
		else
		if (g_xMouse>457 && g_xMouse<520  && g_yMouse < 36)
		{
			m_MouseOn = g_yMouse / 12 + 4;
		}

		if (g_StateType != STATE_FIGHT)//ս��ʱ��������갴����Ϣ
		{
			g_pMainState->m_Cursor.m_FollowMode = FALSE;
			if (g_xMouse > 524)
			{
				m_Select = 11;
				if (CheckL())return TRUE;
				if (g_xMouse > 585)
				{
					if (g_yMouse < 11)
					{
						m_Select = 13;
						if (CheckRB())return TRUE;
					}
					else
					if (g_yMouse < 23)
					{
						m_Select = 14;
						if (CheckRB())return TRUE;
					}
				}
			}
			else if (m_pPet)
			{
				
				if (g_xMouse > m_PetHead.GetX() && g_yMouse < 40)
				{
					m_Select = 12;
					if (CheckL())return TRUE;
					if (g_xMouse > 464)
					{
						if (g_yMouse < 11)
						{
							m_Select = 15;
							if (CheckRB())return TRUE;
						}
						else
						if (g_yMouse < 26)
						{
							m_Select = 16;
							if (CheckRB())return TRUE;
						}
					}
				}
			}
		}
	}
	
	if (m_TeamBack[0].m_NeedShow)
	{
		for (int i = 0; i<5; i++)
		{
			if (m_TeamBack[i].m_NeedShow)
			{
				if (isOn(g_xMouse, g_yMouse, m_TeamBack[i].m_PreShowRect))
				{
					g_pMainState->SetMouseOnMap(FALSE);
					m_Select = i;
					if (CheckRB())return TRUE;
					if (CheckL())return TRUE;
					
				}
			}
			else
				break;
		}
	}
	if (m_MouseOn != m_MouseOnPre)
	{
		if (m_MouseOn > 3)if (!m_pPet)return TRUE;//�����ɫû���ٻ���
		m_MouseOnPre = m_MouseOn;
		g_pMainState->m_Canvas.SetRedraw(m_TMouseOnLine.m_PreShowRect);
		ostringstream oss;
		switch (m_MouseOn)
		{
		case 0:
			if (1)
			{
				oss << "HP ";
				oss << m_pPc->m_PCData.m_HP;
				oss << "/";
				oss << m_pPc->m_PCData.m_HPHurtMax;
				oss << "/";
				oss << m_pPc->m_PCData.m_HPMax.GetData();
				SIZE size1;
				m_TMouseOnLine.SetString(oss.str());
				GetTextExtentPointA(g_DC, m_TMouseOnLine.GetString().c_str(), m_TMouseOnLine.GetString().size(), &size1);
				m_TMouseOnLine.SetXY(640 - size1.cx, 50);

			}
			break;
		case 1:
			if (1)
			{
				oss << "MP ";
				oss << m_pPc->m_PCData.m_MP;
				oss << "/";
				oss << m_pPc->m_PCData.m_MPMax.GetData();
				SIZE size1;
				m_TMouseOnLine.SetString(oss.str());
				GetTextExtentPointA(g_DC, m_TMouseOnLine.GetString().c_str(), m_TMouseOnLine.GetString().size(), &size1);
				m_TMouseOnLine.SetXY(640 - size1.cx, 50);

			}
			break;
		case 2:
			if (1)
			{
				oss << "CP ";
				oss << m_pPc->m_PCData.m_CP;
				oss << "/150";
				SIZE size1;
				m_TMouseOnLine.SetString(oss.str());
				GetTextExtentPointA(g_DC, m_TMouseOnLine.GetString().c_str(), m_TMouseOnLine.GetString().size(), &size1);
				m_TMouseOnLine.SetXY(640 - size1.cx, 50);

			}
			break;
		case 3:
			if (1)
			{
				oss << "EXP ";
				oss << m_pPc->m_PCData.EXP;
				oss << "/";
				oss << m_pPc->m_PCData.ExpMax;
				SIZE size1;
				m_TMouseOnLine.SetString(oss.str());
				GetTextExtentPointA(g_DC, m_TMouseOnLine.GetString().c_str(), m_TMouseOnLine.GetString().size(), &size1);
				m_TMouseOnLine.SetXY(640 - size1.cx, 50);
			}
			break;
		case 4:
			if (1)
			{
				oss << "HP ";
				oss << m_pPet->m_PetDataZZ.m_HP;
				oss << "/";
				oss << m_pPet->m_PetDataZZ.m_HPHurtMax;
				oss << "/";
				oss << m_pPet->m_PetDataZZ.m_HPMax.GetData();
				SIZE size1;
				m_TMouseOnLine.SetString(oss.str());
				GetTextExtentPointA(g_DC, m_TMouseOnLine.GetString().c_str(), m_TMouseOnLine.GetString().size(), &size1);
				m_TMouseOnLine.SetXY(640 - size1.cx, 50);
			}
			break;
		case 5:
			if (1)
			{
				oss << "MP ";
				oss << m_pPet->m_PetDataZZ.m_MP;
				oss << "/";
				oss << m_pPet->m_PetDataZZ.m_MPMax.GetData();
				SIZE size1;
				m_TMouseOnLine.SetString(oss.str());
				GetTextExtentPointA(g_DC, m_TMouseOnLine.GetString().c_str(), m_TMouseOnLine.GetString().size(), &size1);
				m_TMouseOnLine.SetXY(640 - size1.cx, 50);
			}
			break;
		case 6:
			if (1)
			{
				oss << "EXP ";
				oss << m_pPet->m_PetDataZZ.EXP;
				oss << "/";
				oss << m_pPet->m_PetDataZZ.ExpMax;
				SIZE size1;
				m_TMouseOnLine.SetString(oss.str());
				GetTextExtentPointA(g_DC, m_TMouseOnLine.GetString().c_str(), m_TMouseOnLine.GetString().size(), &size1);
				m_TMouseOnLine.SetXY(640 - size1.cx, 50);
			}
			break;
		default:
			g_pMainState->m_Canvas.SetRedraw(m_TMouseOnLine.m_PreShowRect);
			break;
		}
	}
	return TRUE;	
}

void cPCHead::SetPetHead(DWORD head)
{
	if (!head)
	{
		m_PetFace.m_NeedShow = FALSE;
		return;
	}
	m_PetFace.Load(head);
	m_PetFace.m_NeedShow = TRUE;
}

BOOL cPCHead::UpDateHead()
{
	
	for (int i = 0; i < 5; i++)
	{
		if (g_pMainState->m_FightList[g_pMainState->m_InterfaceTeam.GetTeamTurn(i)].isLive)
		{
			m_TeamFace[i].Load(g_pMainState->m_InterfaceTeam.GetSmallHead(i));
			m_TeamBack[i].m_NeedShow = TRUE;
			m_TeamFace[i].m_bMove = TRUE;

			m_TeamPetFace[i].Load(g_pMainState->m_InterfaceTeam.GetPetHead(i));
			m_TeamPetFace[i].m_bMove = TRUE;
		}
		else
		{
			if (m_TeamFace[i].m_NowID)
			{
				g_pMainState->m_Canvas.SetRedraw(m_TeamFace[i].m_PreShowRect);
			}
			m_TeamFace[i].Free();
			m_TeamBack[i].m_NeedShow = FALSE;
		}
		
	}
	if (1 == g_pMainState->GetLiveNum(0,10))
		m_TeamBack[0].m_NeedShow = FALSE;
	return TRUE;
}


void cPCHead::UpdatePcHp()
{
	if (!m_pPc)return;
	g_pMainState->m_Canvas.SetRedraw(m_HeartHurtp[0].GetPicRange());
	g_pMainState->m_Canvas.SetRedraw(m_HPlinep[0].GetPicRange());
	int percent;
	//HP
	if (m_pPc->m_PCData.m_HPMax.GetData() != m_pPc->m_PCData.m_HP)
	{
		//��
		if (m_pPc->m_PCData.m_HPMax.GetData() == m_pPc->m_PCData.m_HPHurtMax)
			percent = 0;
		else
			percent = 100 - m_pPc->m_PCData.m_HPHurtMax * 100 / m_pPc->m_PCData.m_HPMax.GetData();

		m_HeartHurtp[0].SetxOffset(percent*m_HeartHurt.m_Picture.m_Width / 100);

		//HP
		if (m_pPc->m_PCData.m_HPMax.GetData())
			percent = (m_pPc->m_PCData.m_HP * 100) / m_pPc->m_PCData.m_HPMax.GetData();
		else
			percent = 0;
		m_HPlinep[0].SetxOffset((98 - percent) / 2);
	}
	m_HPlinep[0].m_bMove = TRUE;
	m_HeartHurtp[0].m_bMove = TRUE;
}
void cPCHead::UpdatePcMp()
{
	if (!m_pPc)return;
	g_pMainState->m_Canvas.SetRedraw(m_MPlinep[0].GetPicRange());
	int percent;
	//MP
	if (m_pPc->m_PCData.m_MPMax.GetData() != m_pPc->m_PCData.m_MP)
	{
		if (m_pPc->m_PCData.m_MPMax.GetData())
			percent = (m_pPc->m_PCData.m_MP * 100) / m_pPc->m_PCData.m_MPMax.GetData();
		else
			percent = 0;
		m_MPlinep[0].SetxOffset((98 - percent) / 2);
	}
	m_MPlinep[0].m_bMove = TRUE;
}
void cPCHead::UpdatePcCp()
{
	if (!m_pPc)return;
	int percent = (m_pPc->m_PCData.m_CP * 100) / 150;
	m_CPline.SetxOffset((98 - percent) / 2);
	m_CPline.m_bMove = TRUE;
}
void cPCHead::UpdatePcExp()
{
	if (!m_pPc)return;
	int percent;
	if ((m_pPc->m_PCData.EXP) < m_pPc->m_PCData.ExpMax)
	{
		if (g_pCharacter->m_PCData.ExpMax)
		{
			percent = (int)((m_pPc->m_PCData.EXP * 100) / m_pPc->m_PCData.ExpMax);
		}
		else	percent = 0;
		m_EXPlinep[0].SetxOffset((98 - percent) / 2);
	}
	m_EXPlinep[0].m_bMove = TRUE;
}
void cPCHead::UpdatePetHp()
{
	if (!m_pPet)return;
	g_pMainState->m_Canvas.SetRedraw(m_HeartHurtp[1].GetPicRange());
	g_pMainState->m_Canvas.SetRedraw(m_HPlinep[1].GetPicRange());
	//	Ѫ������ʾ���ǳ�ս�ٻ��޵�Ѫ
	int percent;

	//HP
	if (m_pPet->m_PetDataZZ.m_HPMax.GetData() != m_pPet->m_PetDataZZ.m_HP)
	{
		//��
		if (m_pPet->m_PetDataZZ.m_HPMax.GetData() == m_pPet->m_PetDataZZ.m_HPHurtMax)
			percent = 0;
		else
			percent = 100 - m_pPet->m_PetDataZZ.m_HPHurtMax * 100 / (m_pPet->m_PetDataZZ.m_HPMax.GetData());
		m_HeartHurtp[1].SetxOffset(percent*m_HeartHurt.m_Picture.m_Width / 100);
		//HP
		if (g_pCharacter->m_PCData.m_HPMax.GetData())
			percent = (m_pPet->m_PetDataZZ.m_HP * 100) / (m_pPet->m_PetDataZZ.m_HPMax.GetData());
		else
			percent = 0;
		m_HPlinep[1].SetxOffset((98 - percent) / 2);
	}
	m_HeartHurtp[1].m_bMove = TRUE;
	m_HPlinep[1].m_bMove = TRUE;
}
void cPCHead::UpdatePetMp()
{
	if (!m_pPet)return;
	g_pMainState->m_Canvas.SetRedraw(m_MPlinep[1].GetPicRange());
	int percent;
	//MP
	if (m_pPet->m_PetDataZZ.m_MPMax.GetData() != m_pPet->m_PetDataZZ.m_MP)
	{
		if (g_pCharacter->m_PCData.m_MPMax.GetData())
			percent = (m_pPet->m_PetDataZZ.m_MP * 100) / m_pPet->m_PetDataZZ.m_MPMax.GetData();
		else
			percent = 0;
		m_MPlinep[1].SetxOffset((98 - percent) / 2);
	}
	m_MPlinep[1].m_bMove = TRUE;
	return;
}
void cPCHead::UpdatePetExp()
{
	if (!m_pPet)return;
	g_pMainState->m_Canvas.SetRedraw(m_EXPlinep[1].GetPicRange());
	//Ѫ������ʾ���ǳ�ս�ٻ��޵�Ѫ
	int percent;
	//int offset = m_CharacterHead.m_Picture.m_Height / 4;

	//EXP
	if ((m_pPet->m_PetDataZZ.EXP) < m_pPet->m_PetDataZZ.ExpMax)
	{
		if (m_pPet->m_PetDataZZ.ExpMax)
		{
			percent = (int)((m_pPet->m_PetDataZZ.EXP * 100) / m_pPet->m_PetDataZZ.ExpMax);
		}
		else	percent = 0;
		m_EXPlinep[1].SetxOffset((98 - percent) / 2);
	}
	m_EXPlinep[1].m_bMove = TRUE;
	return;
}

void cPCHead::UpdatePet()
{
	if (!m_pPet)return;
	g_pMainState->m_Canvas.SetRedraw(m_PetHead.m_PreShowRect);
	SetPetHead(m_pPet->GetID(POS_SMALLHEAD));
	UpdatePetHp();
	UpdatePetMp();
	UpdatePetExp();
}

void cPCHead::MoveTeamHead(BOOL mode)
{
	int xstart, ystart;
	if (!mode)
	{
		xstart = 172;
		ystart = 3;
	}
	else
	{
		xstart = 385;
		ystart = 430;
	}
	for (int i = 0; i < 5; i++)
	{
		m_TeamFace[i].SetX(xstart + (4 - i) * 50);
		m_TeamFace[i].SetY(ystart);

		m_TeamBack[i].SetX(m_TeamFace[i].GetX() - 2);
		m_TeamBack[i].SetY(m_TeamFace[i].GetY() - 3);
	

		m_TeamPetFace[i].SetX(xstart + (4 - i) * 50);
		m_TeamPetFace[i].SetY(ystart);

	}
}

BOOL cPCHead::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);

	switch (m_Select)
	{
	case 13:
		g_pMainState->m_ItemManage.FillProperty(g_pMainState->m_HeroID, enum_hphurt);
		g_pMainState->m_ItemManage.FillProperty(g_pMainState->m_HeroID, enum_hp);
		break;
	case 14:
		g_pMainState->m_ItemManage.FillProperty(g_pMainState->m_HeroID, enum_mp);
		break;
	case 15:
		g_pMainState->m_ItemManage.FillProperty(g_pMainState->m_HeroID+5, enum_hphurt);
		g_pMainState->m_ItemManage.FillProperty(g_pMainState->m_HeroID+5, enum_hp);
		break;
	case 16:
		g_pMainState->m_ItemManage.FillProperty(g_pMainState->m_HeroID+5, enum_mp);
		break;
	default:
		if (1)
		{
			
			if (m_bPetHeadMode[m_Select])
			{
				int teamselect = g_pMainState->m_InterfaceTeam.GetTeamTurn(m_Select);
				if (!g_pMainState->m_FightList[teamselect + 5].isLive)return TRUE;
				g_pMainState->m_ChaKan.Set(g_pMainState->m_FightList[teamselect + 5].m_pData);
			}
			else
			{
				int teamselect = g_pMainState->m_InterfaceTeam.GetTeamTurn(m_Select);
				g_pMainState->m_ChaKan.Set(g_pMainState->m_FightList[teamselect].m_pData);
			}
		}
		g_pMainState->m_ChaKan.OnOff(TRUE);
	}
	return TRUE;
}


BOOL cPCHead::CheckL()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
		return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	switch (m_Select)
	{
	case 11:
		g_pMainState->m_State.OnOff(!g_pMainState->m_State.m_NeedShow);
		break;
	case 12:
		g_pMainState->m_InterfacePet.OnOff(!g_pMainState->m_InterfacePet.m_NeedShow);
		break;
	case 13:
		if (1)
		{
			g_pMainState->m_ItemManage.FillProperty(g_pMainState->m_HeroID, enum_hphurt);
			g_pMainState->m_ItemManage.FillProperty(g_pMainState->m_HeroID, enum_hp);
		}
		break;
	case 14:
		if (1)
		{
			g_pMainState->m_ItemManage.FillProperty(g_pMainState->m_HeroID, enum_mp);
		}
		break;
	case 15:
		if (1)
		{
			g_pMainState->m_ItemManage.FillProperty(g_pMainState->m_HeroID+5, enum_hphurt);
			g_pMainState->m_ItemManage.FillProperty(g_pMainState->m_HeroID+5, enum_hp);
		}
		break;
	case 16:
		if (1)
		{
			g_pMainState->m_ItemManage.FillProperty(g_pMainState->m_HeroID+5, enum_mp);
		}
		break;

	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		if (1)
		{
			m_bPetHeadMode[m_Select] = !m_bPetHeadMode[m_Select];
			g_pMainState->m_Canvas.SetRedraw(m_TeamBack[m_Select].m_PreShowRect);
		}
	}
	return TRUE;
}

void cPCHead::UpdatePC()
{
	if (!m_pPc)return;
	m_PCFace.Load(m_pPc->GetID(POS_SMALLHEAD));
	UpdatePcHp();
	UpdatePcMp();
	UpdatePcCp();
	UpdatePcExp();
}

void cPCHead::SetFighter(int id)
{
	if (g_pMainState->m_FightList[id].m_pData->m_IndexType != INDEXTYEP_CHARACTER)return;
	m_pPc = (cCharacter*)g_pMainState->m_FightList[id].m_pData->m_pIndex;
	UpdatePC();
	if (id % 10 > 4)return;
	SetPet(id + 5);
}

void cPCHead::SetPet(int id)
{
	if (id == -1)
	{
		m_pPet = 0;
		return;
	}
	if (g_pMainState->m_FightList[id].m_pData->m_IndexType != INDEXTYEP_PET)return;
	m_pPet = (cPet*)g_pMainState->m_FightList[id].m_pData->m_pIndex;
	UpdatePet();
}




void cInterfaceItem::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;

	 m_EquipBackP.resize(6);
	 m_PetEquipBackP.resize(3);
	m_ItemHead.resize(20);
	m_EquipHead.resize(6);//��ɫ���ٻ���װ��
	m_PetEquipHead.resize(3);

	m_Obj.Init();
	m_Obj.m_IndexType = INDEXTYEP_PET;
	m_MousePosBack.Load(0x354CD2EB);
	m_MousePosBack.SetStatic(1);
	m_MousePos.SetColor(RGB(0, 0, 0));
	m_MousePos.SetXY(700, 500);
	m_MousePos.m_hFont = g_pMainState->m_hFont[5];
	//
	m_PathPoint.Load(0x146BB550);
	for (int i = 0; i < 30; i++)
	{
		m_PathPointp[i].Set(&m_PathPoint);
		m_PathPointp[i].m_bStatic = TRUE;
	}
	ifstream File;
	ostringstream oss;
	oss << g_strMediaPath;
	oss << "�����б�\\����������.txt";
	File.open(oss.str());
	if (!File)ERRBOX;
	int x2,y2;
	float fx2, fy2;
	for (int i = 0; i < 4; i++)
	{
		File >> m_sMapSelect[i].m_Name;
		File >> m_sMapSelect[i].m_Height;
		File >> m_sMapSelect[i].m_SmallMap;
		m_MapBack.Load(m_sMapSelect[i].m_SmallMap);
		m_sMapSelect[i].m_bili = ((float)m_sMapSelect[i].m_Height) / ((float)m_MapBack.GetHeight() - 36);
		File >> m_sMapSelect[i].m_num;
		for (int j = 0; j < m_sMapSelect[i].m_num; j++)
		{
			File >> x2;
			File >> y2;
			m_sMapSelect[i].m_xorg.push_back(x2);
			m_sMapSelect[i].m_yorg.push_back(y2);
			x2 *= 20;
			y2 *= 20;
			y2 = m_sMapSelect[i].m_Height - y2;
			fx2 = (float)x2;
			fy2 = (float)y2;
			fx2 /= m_sMapSelect[i].m_bili;
			fy2 /= m_sMapSelect[i].m_bili;
			x2 = (int)fx2;
			y2 = (int)fy2;
			x2 += 18;
			y2 += 18;
			m_sMapSelect[i].m_x.push_back(x2);
			m_sMapSelect[i].m_y.push_back(y2);
		}
	}
	File.close();
	//���з���ͼ
	m_Describe.Init();
	m_WorldMap.Load(0x516749F4);
	m_WorldMap.SetStatic(TRUE);
	m_WorldMap.m_Picture.m_x = 320 - m_WorldMap.m_Picture.m_Width / 2;
	m_WorldMap.m_Picture.m_y = 240 - m_WorldMap.m_Picture.m_Height / 2;
	m_MapSelect.SetStatic(TRUE);
	m_MapID[0] = 0xC5C5FE46;//����
	m_MapID[1] = 0x3D561594;//����
	m_MapID[2] = 0xDB61AD29;//����
	m_MapID[3] = 0x6BD8F1C8;//AL
	m_MapID[4] = 0x29622DDB;//����Ů��
	m_MapID[5] = 0x491E6B4C;//����
	m_MapID[6] = 0x549B2B83;//���Ϲ�
	//CS
	m_MapRect[0].left = 209;
	m_MapRect[0].top = 18;
	m_MapRect[0].right = 272;
	m_MapRect[0].bottom = 65;
	//JY
	m_MapRect[1].left = 424;
	m_MapRect[1].top = 190;
	m_MapRect[1].right = 476;
	m_MapRect[1].bottom = 238;
	//CA
	m_MapRect[2].left = 377;
	m_MapRect[2].top = 84;
	m_MapRect[2].right = 487;
	m_MapRect[2].bottom = 174;
	//AL
	m_MapRect[3].left = 574;
	m_MapRect[3].top = 245;
	m_MapRect[3].right = 621;
	m_MapRect[3].bottom = 290;
	//XL
	m_MapRect[4].left = 204;
	m_MapRect[4].top = 87;
	m_MapRect[4].right = 236;
	m_MapRect[4].bottom = 120;
	//BX
	m_MapRect[5].left = 187;
	m_MapRect[5].top = 161;
	m_MapRect[5].right = 257;
	m_MapRect[5].bottom = 205;

	//ZZ
	m_MapRect[6].left = 251;
	m_MapRect[6].top = 225;
	m_MapRect[6].right = 314;
	m_MapRect[6].bottom = 270;


	m_NumMapSelect = -1;

	m_EquipBack.Load(0xA393A808);
	m_EquipBack.SetStatic(TRUE);
	for (int i = 0; i < 6; i++)
	{
		m_EquipBackP[i].Set(&m_EquipBack);
	}
	for (int i = 0; i < 3; i++)
	{
		m_PetEquipBackP[i].Set(&m_EquipBack);
	}
	m_bXianJin = TRUE;
	m_XianJin.Load(0x79D01E0E);
	m_XianJin.SetStatic(TRUE);
	m_SwapMode = FALSE;

	//�ٻ���װ��
	m_PetHeadback.Load(0x9B1DB10D);
	m_ButtonPetMode.Set(&m_XianJin);
	m_ButtonPetMode.m_bStatic = TRUE;
	m_tButtonPetMode.SetColor(RGB(255,255,255));
	m_tButtonPetMode.SetString("BB");
	m_Obj.m_TrueName.SetColor(RGB(255,255,255));
	//

	m_Select = 1;
	m_Item.Load(2198359531);
	m_PetID = 0x9B1DB10D;
	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;
	m_ZuoQiB.Load(0x79D01E0E);
	m_ZuoQiB.SetStatic(TRUE);
	m_ZuoQiZhuangShiB.Load(0x79D01E0E);
	m_ZuoQiZhuangShiB.SetStatic(TRUE);
	m_ItemButton.Load(0x63CC8A23);
	m_ItemButton2.Load(0x00D13BBF);
	m_FabaoButton.Load(0x00D13BBF);//0x18C30C41);
	m_FabaoButton.SetStatic(TRUE);
	m_ItemButton.SetStatic(TRUE);
	m_ItemButton2.SetStatic(TRUE);
	m_FabaoID = 0x323E2BCD;
	m_BackID = 1261107812;
	m_ZuoQiID = 0xA7CE2F61;
	m_Headback.Load(m_BackID);
	m_BigHead.m_NeedShow = TRUE;
	

	m_SelectMark.Load(0x10921CA7);
	m_SelectMark.m_NeedShow = FALSE;
	m_MouseOnMark.Load(0x6F88F494);
	m_MouseOnMark.m_NeedShow = FALSE;
	
	for (int i = 0; i < 20; i++)
	{
		m_TItemNum[i].SetColor(RGB(255, 255, 255));
		m_TItemNum[i].SetStyle(1);
		m_TItemNum[i].SetBackColor(RGB(0, 0, 0));
	}

	m_TZhuangShi.SetString("װ��");
	m_TZhuangShi.SetColor(RGB(240, 240, 240));
	m_TZuoQi.SetColor(RGB(240, 240, 240));
	m_TZuoQi.SetString( "����");
	

	m_TCunYingN.SetColor(RGB(0, 0, 0));
	m_TXianJinN.SetColor(RGB(0, 0, 0));
	m_TXianJin.SetColor(RGB(240, 240, 240));

	m_ItemButton.SetFrame(1);
	Move(0, 0);
}

BOOL cInterfaceItem::Show(cCanvas* pCanvas)
{

	switch (m_Mode)
	{
	case 0:
	case 2:
		break;
	case 1:
		return ShowSelectMap();
	case 6:
		return TRUE;
	default:
		return ShowSelectMap2();
	}
	pCanvas->Add(&m_Item, 1, TRUE);
	pCanvas->Add(&m_Cancel, 1);
	
	if (m_Mode == 0)
	{
		pCanvas->Add(&m_Headback, 1,1);
		pCanvas->Add(&m_XianJin, 1);
		pCanvas->Add(&m_TXianJin);
		pCanvas->Add(&m_TCunYingN);
		pCanvas->Add(&m_TXianJinN);
		pCanvas->Add(&m_BigBigHead, 1);
	}
	else
	{
		pCanvas->Add(&m_PetHeadback, 1,1);
		pCanvas->Add(m_Obj.GetShadow(), 1);
		m_Obj.ShowOnScreen(pCanvas);
		g_pMainState->m_Canvas.Add(&m_PetName);
	}
	if (m_ButtonPetMode.m_NeedShow)
	{
		pCanvas->Add(&m_ButtonPetMode, 1);
		pCanvas->Add(&m_tButtonPetMode);
	}
	pCanvas->Add(&m_ItemButton,	 1);
	pCanvas->Add(&m_ItemButton2, 1);
	pCanvas->Add(&m_FabaoButton, 1);
	

	if (m_ZuoQiB.m_NeedShow)
	{
		pCanvas->Add(&m_ZuoQiB, 1);
		pCanvas->Add(&m_TZuoQi);

		if (m_ZuoQiZhuangShiB.m_NeedShow)
		{
			pCanvas->Add(&m_ZuoQiZhuangShiB, 1);
			pCanvas->Add(&m_TZhuangShi);
		}
	}
	cCharacter* p = (cCharacter*)g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->m_pIndex;
		ostringstream oss;
		int trueid;


		if (m_SwapMode)
		{
			if (m_Mode == 2)
			{
				for (int i = 0; i < 3; i++)
				{
					if (i == m_SwapID - 12000)continue;
					if (pPet->m_PetDataZZ.m_PetEquip[i].GetNum())
					{
						pCanvas->Add(&m_PetEquipBackP[i], 1);
						pCanvas->Add(&m_PetEquipHead[i], 1);
					}
				}
			}
			else
			for (int i = 0; i < 6; i++)
			{
				if (i == m_SwapID-10000)continue;
				if (p->m_PCData.m_Equip[i].GetNum())
				{
					pCanvas->Add(&m_EquipBackP[i], 1);
					pCanvas->Add(&m_EquipHead[i], 1);
				}
			}
			for (int i = 0; i < 20; i++)
			{
				trueid = i  + m_ItemPage * 20;
				if (trueid == m_SwapID)
				{
					continue;
				}
				if (p->m_PCData.m_Item[trueid].GetNum()>0)
				{
					pCanvas->Add(&m_ItemHead[i], 1);
				}
				if (p->m_PCData.m_Item[trueid].GetNum()>1)
				{
					pCanvas->Add(&m_TItemNum[i]);
				}
			}
		}
		else
		{
			if (m_Mode == 2)
			{
				for (int i = 0; i < 3; i++)
				{
					if (pPet->m_PetDataZZ.m_PetEquip[i].GetNum())
					{
						pCanvas->Add(&m_PetEquipBackP[i], 1);
						pCanvas->Add(&m_PetEquipHead[i], 1);
					}
				}
			}
			else
			for (int i = 0; i < 6; i++)
			{
				if (p->m_PCData.m_Equip[i].GetNum())
				{
					pCanvas->Add(&m_EquipBackP[i], 1);
					pCanvas->Add(&m_EquipHead[i], 1);
				}
			}
			for (int i = 0; i < 20; i++)
			{
				trueid = i + m_ItemPage * 20;
				if (p->m_PCData.m_Item[trueid].GetNum())
				{
					pCanvas->Add(&m_ItemHead[i], 1);
				}
				if (p->m_PCData.m_Item[trueid].GetNum()>1)
				{
					pCanvas->Add(&m_TItemNum[i]);
				}
			}
		}
		//}
		pCanvas->Add(&m_MouseOnMark, 1);

		if (m_SwapMode)
		{
			pCanvas->Add(&m_SelectMark, 1);
			pCanvas->Add(&m_SwapItemSelect, 1);
		}
		
		return TRUE;

}

BOOL cInterfaceItem::ProcessInput()
{
	if (m_Sleeptime > 0)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_Sleeptime -= 1;
		return TRUE;
	}
	ResetButton();
	m_Select = -1;
	m_NowSelect = -1;
	if (m_Describe.m_NeedShow)
	{
		m_Describe.OnOff(FALSE);
	}
	cObj* pUser =g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData;
	cCharacter* pc = (cCharacter*)pUser->m_pIndex;
	switch (m_Mode) //���з�
	{
	case 0:
		break;
	case 2:
		break;
	case 1:
		return ProcessSelectMap();
	case 6:
		if (1)
		{
		
			int i;//Ѱ���¹ⱦ�е�λ��
			sItem2* pItem=0;
			for (i = 0; i < 60; i++)
			{
				if (pc->m_PCData.m_Item[i].GetType() == 24)
				{
					pItem = &pc->m_PCData.m_Item[i];
					if (11 == pItem->m_pItemData->m_ID)
					{
						break;
					}
				}
			}
			if (i == 60)return FALSE;
			switch (g_pMainState->m_Dialog.m_retSelect)
			{
			case 0:
				if (1)
				{
					InitMode(0);
					g_pMainState->m_Map.LoadMap(pItem->m_Property[1],
						pItem->m_Property[2],
						(char*)pItem->m_strProperty.c_str(),TRUE);
					OnOff(FALSE);
				}
				break;
			case 1:
				if (1)
				{	
					pItem->m_strProperty = g_pMainState->m_Map.m_MapName;
					int xpos = pUser->GetX() / 20;
					int ypos = (g_pMainState->m_Map.m_pMap->m_Height - pUser->GetY()) / 20;
					if (pItem->m_Property.size() == 1)
					{
						pItem->m_Property.push_back(xpos);
						pItem->m_Property.push_back(ypos);
					}
					else
					{
						pItem->m_Property[1]=(xpos);
						pItem->m_Property[2]=(ypos);
					}
					g_pMainState->m_Channel.AddNewString("���ڵ�ǰλ�ö�������");
					InitMode(0);
				}
				break;
			default:
				m_Mode = 0;
				OnOff(FALSE);
				break;
			}
		}
		return TRUE;
	default:
		return ProcessSelectMap2()
			;
	}

	if (m_bMoveMode)//�ƶ�
	{	
		m_Select = 6;
		g_pMainState->SetMouseOnMap(FALSE);
		if (!CheckL())m_bMoveMode = FALSE;
		return TRUE;
	}
	if (m_SwapMode)
	{
		g_pMainState->SetMouseOnMap(FALSE);
		m_SwapItemSelect.SetXY(g_xMouse - 25, g_yMouse - 25);
	}
	
	if (isInRect(g_xMouse, g_yMouse, &m_Item))
	{
		g_pMainState->SetMouseOnMap(FALSE);
		
		
		if(CheckOn(&m_Cancel,1))return TRUE;
		if (CheckOn(&m_XianJin, 2))return TRUE;

		
		if (0 == m_FabaoButton.GetNowFrame())
		if (CheckOn(&m_FabaoButton, 3))return TRUE;
	
		if (0 == m_ItemButton.GetNowFrame())
		if (CheckOn(&m_ItemButton, 4))return TRUE;
		
		if (m_ZuoQiB.m_NeedShow)	
		if (CheckOn(&m_ZuoQiB, 11))return TRUE;
		if(m_ButtonPetMode.m_NeedShow)
		if (CheckOn(&m_ButtonPetMode, 14))return TRUE;
		if (m_ZuoQiZhuangShiB.m_NeedShow)
		if (CheckOn(&m_ZuoQiZhuangShiB, 12))return TRUE;


		if (0==m_ItemButton2.GetNowFrame())
		if (CheckOn(&m_ItemButton2, 5))return TRUE;
		
		if (m_Mode == 2)
		{
			if (isOn(g_xMouse, g_yMouse, m_rectPetEquip))
			{
				m_NowSelect =  (g_xMouse - m_rectPetEquip.left) / 55 + ((g_yMouse - m_rectPetEquip.top) / 55) * 2;
				m_MouseOnMark.SetXY(m_PetEquipHead[m_NowSelect].GetX(), m_PetEquipHead[m_NowSelect].GetY());
					m_NowSelect += 12000;
			}
		}
		else
		if (isOn(g_xMouse, g_yMouse, m_rectEquip))
		{
				m_NowSelect =  (g_xMouse - m_rectEquip.left) / 55 + ((g_yMouse - m_rectEquip.top) / 55) * 2;

					m_MouseOnMark.SetXY(m_EquipHead[m_NowSelect].GetX(), m_EquipHead[m_NowSelect].GetY());
					m_NowSelect += 10000;
	
		}

		if (isOn(g_xMouse, g_yMouse, m_rectItem))
		{
			m_NowSelect = (g_xMouse - m_rectItem.left) / 51 + ((g_yMouse - m_rectItem.top) / 51) * 5;
			m_MouseOnMark.SetXY(m_ItemHead[m_NowSelect].GetX(), m_ItemHead[m_NowSelect].GetY());

			m_NowSelect += m_ItemPage * 20;
		}
	
		if (m_NowSelect>-1)
		{
			sItem2* pItem = pc->m_PCData.GetItem(m_NowSelect);
			m_MouseOnMark.m_NeedShow = TRUE;
			
			if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
			{
				g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
				cCharacter* pc = (cCharacter*)g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->m_pIndex;

				if (!m_SwapMode)
				{
					if (pItem->GetNum())
					{
						m_SelectMark.m_NeedShow = TRUE;
						m_SwapItemSelect.Load(pItem->m_pItemData->m_SmallHead);

						if (m_NowSelect < 10000)
						{
							m_SwapItemSelect.SetXY(g_xMouse - 25, g_yMouse - 25);
							m_SelectMark.SetXY(m_ItemHead[m_NowSelect - m_ItemPage * 20].GetX(),
								m_ItemHead[m_NowSelect - m_ItemPage * 20].GetY());
							
						}
						else  if (m_NowSelect < 12000)
						{
							m_SelectMark.SetXY(m_EquipHead[m_NowSelect - 10000].GetX(),
								m_EquipHead[m_NowSelect - 10000].GetY());
						
						}
						else
						{
							m_SelectMark.SetXY(m_PetEquipHead[m_NowSelect - 12000].GetX(),
								m_PetEquipHead[m_NowSelect - 12000].GetY());
						}
						m_SwapID = m_NowSelect;
						m_SwapMode = TRUE;
						m_MouseOnMark.m_NeedShow = FALSE;
					}

				}
				else
				{
					if (m_SwapID == m_NowSelect)
					{
						m_SwapID = -1;
						m_SwapMode = FALSE;
						m_SelectMark.m_NeedShow = FALSE;
						return TRUE;
					}
					else
					{
						if (m_NowSelect >= 12000)
						{
							if (m_SwapID >= 10000)
							{
								m_SwapID = -1;
								m_SwapMode = FALSE;
								m_SelectMark.m_NeedShow = FALSE;
								return TRUE;
							}
							else
							if (UseItem(m_SwapID, g_pMainState->m_HeroID))
							{
								m_SwapID = -1;
								m_SwapMode = FALSE;
								m_SelectMark.m_NeedShow = FALSE;
								return TRUE;
							}
						}
						else
						if (m_NowSelect >=10000)//װ��
						{
							if (m_SwapID >=10000)
							{
								m_SwapID = -1;
								m_SwapMode = FALSE;
								m_SelectMark.m_NeedShow = FALSE;
								return TRUE;
							}
							else
							if (pc->m_PCData.m_Item[m_SwapID].GetType() < 22) //װ��
							{
								if (UseItem(m_SwapID, g_pMainState->m_HeroID))
								{
									m_SwapID = -1;
									m_SwapMode = FALSE;
									m_SelectMark.m_NeedShow = FALSE;
									return TRUE;
								}
							}
						}
						else
						{
							if (!SwapItem(m_SwapID, m_NowSelect))return TRUE;
							if (m_SwapID >10000)
							{
								g_pMainState->m_State.UpDatePcDate();
							}
							m_SwapID = -1;
							m_SwapMode = FALSE;
							m_SelectMark.m_NeedShow = FALSE;
						}
						
					}
					return TRUE;
				}
			}
			//������Ʒ��Ϣ
			if (m_SwapMode == FALSE)
			{
				UpDateItemDate(m_Describe);
			}
		}
		else
		{
			m_MouseOnMark.m_NeedShow = FALSE;
			//׼���ƶ�
			m_Select = 7;
			if(CheckL())return TRUE;
			if (g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))
			{
				g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
				OnOff(FALSE);
			}
		}
		//ʹ�õ���
		m_Select = 0;
		CheckRB();
	}
	else
	{
		if (m_SwapMode)
		{
			if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
			{
				//cCharacter* pc = (cCharacter*)g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->m_pIndex;
				g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
				if (m_Select == -1)
				{
					SwapItem(m_SwapID, m_Select);
					m_SwapMode = FALSE;
					m_SwapID = -1;
					m_SelectMark.m_NeedShow = FALSE;
				}

			}
		}
		else
		{
			if (m_Describe.m_NeedShow)m_Describe.OnOff(FALSE);
		}
	}
	return TRUE;
}

BOOL cInterfaceItem::Add(int Pos, cPCData& pcData, sItem2* item)
{
	Init();
	sItem2* pItem;
	if (item->GetType() == 24)//����
	{	
		for (int i = 0; i < 60; i++)
		{
			pItem = pcData.GetItem(i);
			if ((*pItem) == (*item))
				return FALSE;
		}
		g_pMainState->m_ItemManage.SetFabaoEffect(pcData, 1, (eFaBao)item->GetID());
	}
	
	pItem = pcData.GetItem(Pos);
	(*pItem) = (*item);

	if (pcData.m_IDinFightList == g_pMainState->m_HeroID)
	{
		int headpos = Pos - m_ItemPage * 20;
		if (headpos <  20 && headpos >-1)
		{
			m_ItemHead[headpos].Load(pItem->m_pItemData->m_SmallHead);
		}
	}
	return TRUE;
}
//�ڿհ״��Ӷ���
BOOL cInterfaceItem::Add(int Pos, int CharID, sItem2* item)
{
	cCharacter* p = (cCharacter*)g_pMainState->m_FightList[CharID].m_pData->m_pIndex;
	return Add(Pos, p->m_PCData, item);
}



int  cInterfaceItem::FindIdle(int StartPos, int EndPos, int PcID)
{
	cCharacter* p = (cCharacter*)g_pMainState->m_FightList[PcID].m_pData->m_pIndex;
	return FindIdle(StartPos, EndPos, p->m_PCData);
}

int cInterfaceItem::FindIdle(int StartPos /*= 0*/, int EndPos /*= 60*/, cPCData& pcdata)
{
	for (int i = StartPos; i < EndPos; i++)
	{
		if (!pcdata.m_Item[i].GetNum())
			return i;
	}
	return -1;
}

int cInterfaceItem::FindSame(int itemID, int itemType, int StartPos /*= 9*/, int EndPos /*= 28*/, int PcID /*= 2*/)
{
	cCharacter* p = (cCharacter*)g_pMainState->m_FightList[PcID].m_pData->m_pIndex;
	for (int i = StartPos; i < EndPos; i++)
	{
		if (p->m_PCData.m_Item[i].m_Num)
		switch (p->m_PCData.m_Item[i].GetType())
		{
		case 22:
		case 23:
		case 25:
		case 27:
			if (1)  //ֻ�е��߿��Ե���
			{
				sItem2* p2 = &p->m_PCData.m_Item[i];
				if (p2->GetID() == itemID)
				if (p2->GetType() == itemType)
				if (p2->GetNum() < 30)
				{
					return i;
				}
			}
		}
	}
	return -1;
}
BOOL cInterfaceItem::GetItem(int CharID, sItem2* pItem)
{
	if (!pItem)return TRUE;
	if ((!pItem->m_Property.size()) && pItem->GetType()>21 && pItem->GetType()<32)
		g_pMainState->m_ItemManage.AutoItemProperty(pItem);
	int i;
	int itemType = pItem->GetType();
	int itemID = pItem->GetID();
//	int num = pItem->GetNum();
	switch (itemType)
	{
		//�ɵ��ӵ���
	case 22:
	case 23:
	case 25:
	case 27: 
	case 29:
	case 40:
	case 41:
		if (1)
		{
			i = FindSame(itemID, itemType, 0, 60, CharID);
			if (i > -1)
			{
				Overlay(i, CharID,pItem);
				return TRUE;
			}
			else
			{
				i = FindIdle(0, 60, CharID);
				if (i > -1)
				{
					Add(i,  CharID, pItem);
				}
				return TRUE;
			}
		}
		break;
	case 24://����
		if (1)
		{
			i = FindSame(itemID, itemType, 0, 60, CharID);
			if (i > -1)
			{
				g_pMainState->m_Tags.Add("��������Ѿ�����");
				return TRUE;
			}
			else
			{
				i = FindIdle(0, 60, CharID);
				if (i > -1)
				{
					Add(i, CharID, pItem);
				}
				return TRUE;
			}
		}
		break;
		break;
	default:
		if (1) //���ɵ���
		{
			int num = pItem->GetNum();
			pItem->m_Num = 1;
			for (int i = 0; i<num; i++)
			{
				int idle = FindIdle(0, 60, CharID);
				if (idle > -1)
				{
					Add(idle, CharID, pItem);
				}
				else
				{
					g_pMainState->m_Tags.Add("�Ų�����");
					break;
				}
			}
		}
		break;
	}

	return TRUE;
}

BOOL cInterfaceItem::Overlay(int Pos, int CharID, sItem2* item)
{
	cCharacter* pc = (cCharacter*)g_pMainState->m_FightList[CharID].m_pData->m_pIndex;
	sItem2* pitem2 = pc->m_PCData.GetItem(Pos);
	pitem2->m_Num += item->GetNum();
	if (pitem2->GetNum() > 29)
	{
		int k = pitem2->GetNum() - 30;
		pitem2->m_Num = 30;
		int newPos = FindIdle(0, 60, CharID);
		int prenum = item->m_Num;
			item->m_Num = k;
		if (newPos > 0)
		{
			Add(newPos, CharID, item);
		}
		item->m_Num = prenum;
	}

	int trueid = Pos - m_ItemPage * 20;
	if (trueid<0 || trueid>19)return TRUE;
	sItem2* pItem = &pc->m_PCData.m_Item[Pos];
	if (pItem->GetNum() > 1)
	{
		ostringstream oss;
		oss.str("");
		oss << pItem->GetNum();
		m_TItemNum[trueid].SetString(oss.str());
	}
	return TRUE;
}

BOOL cInterfaceItem::SwapItem(const int ID, const int IDTarget, cPCData* pcdata)
{
	if (ID == IDTarget)return TRUE;
	if (ID > 60 && IDTarget > 60)return TRUE;//��ֹ"���װ��"����
	if (!pcdata)
	{
		pcdata = &g_pCharacter->m_PCData;
	}
	if (-1 == IDTarget)  //ɾ����Ʒ
	{
		if (ID >= 12000)
		{
			sItem2& Item = pcdata->m_pPetDataList[pcdata->m_FightPetID]->m_PetDataZZ.m_PetEquip[ID - 12000];
			pcdata->m_pPetDataList[pcdata->m_FightPetID]->m_PetDataZZ.UnPetEquip(ID - 12000);
			sItem2 dustbin;
			Item.Swap(dustbin);
		}
		else
		if (ID == 11000)
		{
			sItem2& Item = pcdata->m_ZuoQi.m_ZhuangShi;
			sItem2 dustbin;
			Item.Swap(dustbin);
		}
		else
		if (ID >= 10000)//ֱ����װ��
		{
			sItem2& Item = pcdata->m_Equip[ID - 10000];
			pcdata->UnEquip(ID - 10000);
			sItem2 dustbin;
			Item.Swap(dustbin);
		}
		else
		{
			if (pcdata->m_Item[ID].GetType() == 24)
			{
				g_pMainState->m_ItemManage.SetFabaoEffect(*pcdata, -1, (eFaBao)pcdata->m_Item[ID].GetID());
			}
			sItem2& Item = pcdata->m_Item[ID];
			sItem2 dustbin;
			Item.Swap(dustbin);
		}
		return TRUE;
	}

	if (ID >= 12000)
	{//����װ��
		sItem2& Item = pcdata->m_pPetDataList[pcdata->m_FightPetID]->m_PetDataZZ.m_PetEquip[ID - 12000];
			pcdata->m_pPetDataList[pcdata->m_FightPetID]->m_PetDataZZ.UnPetEquip(ID - 12000);
			Item.Swap(pcdata->m_Item[IDTarget]);
			pcdata->m_pPetDataList[pcdata->m_FightPetID]->m_PetDataZZ.PetEquip(ID - 12000);
			
		
	}
	else
	if (ID == 11000)
	{//����װ��
		pcdata->m_ZuoQi.m_ZhuangShi.Swap(pcdata->m_Item[IDTarget]);
	}
	else
	if (ID >= 10000)
	{//����װ��
		if (IDTarget >= 10000)return TRUE;
		sItem2& itemtarget = pcdata->m_Item[IDTarget];
		pcdata->UnEquip(ID);
		pcdata->Equip(itemtarget);
		pcdata->m_Equip[ID - 10000].Swap(pcdata->m_Item[IDTarget]);
		
		
	}
	else //ʹ�õ���
	if (IDTarget >= 12000)
	{
		if (-1 == pcdata->m_FightPetID)
		{
			if (!pcdata)
			g_pMainState->m_Tags.Add("��ǰû�г�ս�ٻ���");
			return TRUE;
		}
		cPetData* pdata = &pcdata->m_pPetDataList[pcdata->m_FightPetID]->m_PetDataZZ;
		sItem2& Item = pdata->m_PetEquip[IDTarget - 12000];
		pdata->UnPetEquip(IDTarget - 12000);
		Item.Swap(pcdata->m_Item[ID]);
		pdata->PetEquip(IDTarget - 12000);
	}
	else
	if (IDTarget == 11000)
	{
		pcdata->m_ZuoQi.m_ZhuangShi.Swap(pcdata->m_Item[ID]);
	}
	else
	if (IDTarget >= 10000)
	{
		sItem2 itemtarget = pcdata->m_Item[ID];
		if (!itemtarget.m_Property.size())
		{
			g_pMainState->m_Tags.Add("δ����");
			return FALSE;
		}
		pcdata->UnEquip(IDTarget - 10000);	
		if (pcdata->Equip(itemtarget))
			pcdata->m_Equip[IDTarget - 10000].Swap(pcdata->m_Item[ID]);
	
	}
	else
	{
		sItem2* pItem = pcdata->GetItem(ID);
		sItem2* pItemtarget = pcdata->GetItem(IDTarget);
		int& numtarget = pItemtarget->m_Num;
		{
			int& numuser = pcdata->m_Item[ID].m_Num;
			//���ӵ���
			if (pItemtarget->GetNum())
			if (pItem->GetID() == pItemtarget->GetID() && pItem->GetType() == pItemtarget->GetType())
			{
				switch (pItemtarget->GetType())
				{
				case 22:
				case 23:
				case 25:
				case 27:
				case 29:
				case 30:
					if (1)
					{
						if (numtarget< 29)
						{
							numtarget += numuser;
							if (numtarget>29)
							{
								numuser = numtarget - 30;
								numtarget = 30;
							}
							else numuser = 0;
						}
					}
					UpdateItemNum(ID);
					UpdateItemNum(IDTarget);

					return TRUE;
				}
			}
	
				pcdata->m_Item[ID].Swap(pcdata->m_Item[IDTarget]);
		}
		
	}
	if (pcdata->m_Name != g_pCharacter->m_PCData.m_Name)return TRUE;
	LoadSmallMap(ID);
	LoadSmallMap(IDTarget);
	UpdateItemNum(ID);
	UpdateItemNum(IDTarget);

	return TRUE;
}

BOOL cInterfaceItem::LoadSmallMap(int ID)
{
	if (ID<1000)
	if (ID - m_ItemPage * 20 > 19 || ID - m_ItemPage * 20<0)return TRUE;
	cCharacter* p = g_pCharacter;
	sItem2* pItem = p->m_PCData.GetItem(ID);
	if (0 == pItem->GetNum())return TRUE;
	if (ID >= 12000)
	{
		m_PetEquipHead[ID-12000].Load(g_pMainState->m_ItemManage.GetItem(pItem->GetType(), pItem->GetID())->m_SmallHead);
		return TRUE;
	}
	if (ID == 11000)return TRUE;
	if (ID < 10000)
	{
		if (ID<m_ItemPage * 20)return TRUE;
		if (ID>m_ItemPage * 20 + 20)return TRUE;
		if (pItem->m_Num)
			m_ItemHead[ID-m_ItemPage * 20].Load(pItem->m_pItemData->m_SmallHead);
	
	}
	else
	{
		ID -= 10000;
		if (pItem->m_Num)
			m_EquipHead[ID].Load(pItem->m_pItemData->m_SmallHead);
	}
	
	return TRUE;
}
BOOL cInterfaceItem::UseItem(int ID, cPCData& pcData)
{
	return UseItem(ID, pcData.m_IDinFightList);
}
BOOL cInterfaceItem::UseItem(int ID,int UserID)
{
	if (UserID < 0)return FALSE;
	cCharacter* p;
	if (g_pMainState->m_FightList[UserID].m_pData->m_IndexType == INDEXTYEP_CHARACTER)
	{
		p = (cCharacter*)g_pMainState->m_FightList[UserID].m_pData->m_pIndex;
	}
	else
		return TRUE;
	
	sItem2* item;
	item = p->m_PCData.GetItem(ID);
	if (item->GetNum() < 1)return FALSE;
	if (item->GetType() < 0)return FALSE;
	switch (item->GetType())
	{
	case 22:
		if (1)
		{
			g_pMainState->m_ItemManage.UseItem22(item, UserID);
			break;
		}
	case 23:
		if (1)
		{
			g_pMainState->m_ItemManage.UseItem23(item, UserID);
		
			break;
		}
	case 24:
		if (1)
		{
			g_pMainState->m_ItemManage.UseItem24(item, UserID);
			break; 
		}
	case 25:return FALSE;//�����޷�ʹ��
	case 26://����
		if (1)
		{
			g_pMainState->m_ItemManage.UseItem26(item, UserID);
			break;
		}
	case 27:
		if (1)
		{
			 g_pMainState->m_ItemManage.UseItem27(item, UserID, &p->m_PCData.m_Item[ID].m_Num);
		}
		break;
	case 28://���ɵ��ӵ���
		if (1)
		{
			g_pMainState->m_ItemManage.UseItem28(item, UserID);
		}
		break;
	case 31://����װ��
		if (1)
		{
		
			if (ID ==11000)
			{

				SwapItem(ID, FindIdle(0, 60, UserID), &p->m_PCData);
				SetEquipZuoQiZhuangShi(FALSE);
			}
			else
			{
				sItem2& m_Item = p->m_PCData.m_Item[ID];
				if (p->m_PCData.m_ZuoQi.m_pZuoQi->ID != m_Item.GetID() / 3)return FALSE;//�޷�װ������	
				p->m_PCData.m_ZuoQi.m_ZhuangShi.Swap(m_Item);
				SetEquipZuoQiZhuangShi(TRUE);
			}
			g_pMainState->m_FightList[p->m_PCData.m_IDinFightList].m_pData->Reset();
			g_pMainState->m_FightList[p->m_PCData.m_IDinFightList].m_pData->Set(POS_STAND);
		}
		break;
		//����
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
			if (!item->m_Property.size())
			{
				if (p->m_PCData.m_MengPaiID == eMP_���ƹٸ�)
				{
					if (p->m_PCData.m_MengPaiSkillLV[2] >= g_pMainState->m_ItemManage.GetlvByID(item->GetType(), item->GetID()))
					{
						g_pMainState->m_ItemManage.AutoItemProperty(item, p);
					}
					else
					{
						g_pMainState->m_Tags.Add("���ܵȼ�����");
						return TRUE;
					}
				}
				else
				{
					g_pMainState->m_Tags.Add("δ����");
					return TRUE;
				}
			}
			if (ID == 10002)
			{
				int targetid = FindIdle(0, 60, UserID);
				if (targetid == -1)return FALSE;
				SwapItem(ID, targetid,&p->m_PCData);
			}
			else
			{
				if (!p->m_PCData.CanEquip(p->m_PCData.m_Item[ID]))return TRUE;//
				SwapItem(ID, 10002, & p->m_PCData);
			}
			g_pMainState->m_FightList[p->m_PCData.m_IDinFightList].m_pData->Reset();
			g_pMainState->m_FightList[p->m_PCData.m_IDinFightList].m_pData->Set(POS_STAND);
			g_pMainState->m_PCHead.UpdatePcHp();
			g_pMainState->m_PCHead.UpdatePcMp();
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
		if (1)
		{
			if (!item->m_Property.size())
			{
				if (p->m_PCData.m_MengPaiID == eMP_���ƹٸ�)
				{
					if (p->m_PCData.m_MengPaiSkillLV[2] >= g_pMainState->m_ItemManage.GetlvByID(item->GetType(), item->GetID()))
					{
						g_pMainState->m_ItemManage.AutoItemProperty(item, p);
					}
					else
					{
						g_pMainState->m_Tags.Add("���ܵȼ�����");
						return TRUE;
					}
				}
				else
				{
					g_pMainState->m_Tags.Add("δ����");
					return TRUE;
				}
			}
			int targetid=0;
			if (ID >= 10000)
			{
				targetid = FindIdle(0, 60, UserID);
				if (targetid == -1)return FALSE;
			}
			else
			{
				if (p->m_PCData.m_LV < g_pMainState->m_ItemManage.GetlvByID(p->m_PCData.m_Item[ID].GetType(), p->m_PCData.m_Item[ID].GetID()))
				{
					g_pMainState->m_Tags.Add("��ɫ�ȼ�����");
					return TRUE;
				}
				switch (p->m_PCData.m_Item[ID].GetType())
				{
				case 15:
				case 16:
					targetid = 0;
					break;
				case 17:
				case 18:
					targetid = 3;
					break;
				case 19:
					targetid = 1;
					break;
				case 20:
					targetid = 4;
					break;
				case 21:
					targetid = 5;
					break;
				default:
					ERRBOX;
					break;
				}
				targetid += 10000;
			}
			SwapItem(ID, targetid, &p->m_PCData);
			g_pMainState->m_PCHead.UpdatePcHp();
			g_pMainState->m_PCHead.UpdatePcMp();
		}
		break;
	case 32:
	case 33:
	case 34:
		if (1)
		{
			
			if (!item->m_Property.size())
			{
				if (p->m_PCData.m_MengPaiID == eMP_���ƹٸ�)
				{
					if (p->m_PCData.m_MengPaiSkillLV[2] >= g_pMainState->m_ItemManage.GetlvByID(item->GetType(), item->GetID()))
					{
						g_pMainState->m_ItemManage.AutoItemProperty(item, p);
					}
					else
					{
						g_pMainState->m_Tags.Add("���ܵȼ�����");
						return TRUE;
					}
				}
				else
				{
					g_pMainState->m_Tags.Add("δ����");
					return TRUE;
				}
			}
			if (!g_pMainState->m_FightList[UserID + 5].isLive)return TRUE;
			int targetid=0;
			if (ID >= 12000)
			{//ȡ��
				targetid = FindIdle(0, 60, UserID);
				if (targetid == -1)return FALSE;
			}
			else
			{//װ��
				cPetData* Pet;
				g_pMainState->m_FightList[UserID + 5].m_pData->GetDateP(Pet);
				if (Pet->m_LV < p->m_PCData.m_Item[ID].GetID() * 10)
				{
					g_pMainState->m_Tags.Add("�ٻ��޵ȼ�����");
					return TRUE;
				}
				if (!p->m_PCData.m_Item[ID].m_Property.size())return TRUE;
				switch (p->m_PCData.m_Item[ID].GetType())
				{
				case 32:targetid = 0; break;
				case 33:targetid = 1; break;
				default:targetid = 2; break;
				}
				targetid += 12000;
			}
			SwapItem(ID, targetid, &p->m_PCData);
			g_pMainState->m_PCHead.UpdatePet();
		}
		break;
	case 35:
	case 36:
	case 37:
	case 38:
		g_pMainState->m_ItemManage.UseJiaJu(item);
		break;
	default:
		break;
	}


	int trueid;
	sItem2* pItem=0;
	if (ID<80)
	{
		trueid = ID - m_ItemPage * 20;
		pItem = &p->m_PCData.m_Item[ID];
	}
	else
	if (ID == 11000)
	{
		pItem = &p->m_PCData.m_ZuoQi.m_ZhuangShi;	
	}
	else if (ID<11000)
	{
		trueid = ID - 10000;
		pItem = &p->m_PCData.m_Equip[trueid];
	}
	else if (ID>=12000)
	{
		trueid = ID - 12000;
		pItem = &p->m_PCData.m_pPetDataList[p->m_PCData.m_FightPetID]->m_PetDataZZ.m_PetEquip[ID - 12000];
	}
	else ERRBOX;

	if (pItem->GetNum() > 1)
	{
		UpdateItemNum(ID);
	/*	ostringstream oss;
		oss.str("");
		oss << pItem->GetNum();
		m_TItemNum[trueid].SetString(oss.str());*/
	}
	return TRUE;
}



BOOL cInterfaceItem::UpDateItemDate(cDescribe& Describe)
{
	sItem2* pItem = g_pCharacter->m_PCData.GetItem(m_NowSelect);
	return UpDateItemDate(Describe, pItem);
}

BOOL cInterfaceItem::UpDateItemDate(cDescribe& Describe, sItem2* pItem)
{
	 return g_pMainState->m_ItemManage.SetItemDescribe(
		g_xMouse, g_yMouse, Describe, pItem);
}

BOOL cInterfaceItem::CheckL()
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
	case 1:
		OnOff(FALSE);
		return TRUE;
	case 2:
		
		{
		//	cCharacter* pC = (cCharacter*)g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->m_pIndex;
			m_XianJin.SetFrame(1);
			m_bXianJin = !m_bXianJin;
			SetXianJin(m_bXianJin);
		}
		return TRUE;
	case 3://����
		SetItemPage(2);
		m_frampre = 0;
		return TRUE;
	case 4://����
		SetItemPage(0);
		m_frampre = 0;
		return TRUE;
	case 5://����
		SetItemPage(1);
		m_frampre = 0;
		return TRUE;
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
	case 11: //����
		if (1)
		{
			int k = m_ZuoQiB.GetNowFrame();
			cCharacter* pc = (cCharacter*)g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->m_pIndex;
			if (k == 2)//����
			{
				m_ZuoQiB.SetFrame(1);				
				pc->m_PCData.m_bOnZuoQi = TRUE;
				
			}
			else //����
			{
				pc->m_PCData.m_bOnZuoQi = FALSE;
				m_ZuoQiB.SetFrame(0);
			}
			m_frampre = 0;
			g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->Reset();
			g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->Set(POS_STAND);
		}
		return TRUE;
	case 12: //װ��
		if (m_ZuoQiZhuangShiB.GetNowFrame() == 2)//����
		{
			SetEquipZuoQiZhuangShi(FALSE);
			UseItem(11000, g_pMainState->m_HeroID);
		}
		return TRUE;
	case 14:
		if (m_Mode == 2)
		{
			m_Mode = 0;
			m_ButtonPetMode.SetFrame(0);
		}
		else
		{
			m_Mode = 2;
			m_ButtonPetMode.SetFrame(1);
			m_framprep = 0;
		}
		UpdateData();
		break;
	default:
		break;
	}
	
	return TRUE;
}

BOOL cInterfaceItem::SetItemPage(int page)
{
	if (m_ItemPage == page)return FALSE;
	m_ItemPage = page;
	cCharacter* p = (cCharacter*)g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->m_pIndex;
	ostringstream oss;
	int trueid;
	for (int i = 0; i < 20; i++)
	{
		trueid = i + page * 20;
		sItem2* pItem = &p->m_PCData.m_Item[trueid];
		if (pItem->GetNum()>0)
		{
			m_ItemHead[i].Load(pItem->m_pItemData->m_SmallHead);
			if (pItem->GetNum()> 1)
			{
				oss.str("");
				oss << pItem->GetNum();
				m_TItemNum[i].SetString(oss.str());
				m_TItemNum[i].m_bMove = TRUE;
			}
		}
	}
	m_ItemButton.SetFrame(0);
	m_FabaoButton.SetFrame(0);
	m_ItemButton2.SetFrame(0);
	switch (page)
	{
	case 0:
		m_ItemButton.SetFrame(1);
		break;
	case 1:
		m_ItemButton2.SetFrame(1);
		break;
	case 2:
		m_FabaoButton.SetFrame(1);
		break;
	default:
		ERRBOX
		break;
	}
	m_FabaoButton.m_bMove = TRUE;
	m_ItemButton.m_bMove = TRUE;
	m_ItemButton2.m_bMove = TRUE;

	g_pMainState->m_Canvas.SetRedraw(m_Item.m_PreShowRect);
	return TRUE;
}

BOOL cInterfaceItem::Move(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
	m_Item.m_bMove = TRUE;
	m_Item.m_Picture.SetXY(x, y);

	m_TCunYingN.SetXY(m_xPos + 65, m_yPos + 178);
	//���ð�ť��λ��
	m_ItemButton.m_Picture.m_x = x+40;
	m_ItemButton.m_Picture.m_y = y + 420;
	m_ItemButton2.m_Picture.m_x = x+90;
	m_ItemButton2.m_Picture.m_y = y + 420;
	m_FabaoButton.m_Picture.m_x = x+140;
	m_FabaoButton.m_Picture.m_y = y + 420;

	m_Cancel.m_x = x+290;
	m_Cancel.m_y = y+4;
	m_Headback.m_Picture.m_x = x-1;
	m_Headback.m_Picture.m_y = y+25;
	m_PetHeadback.SetXY(x, y + 25);

	m_XianJin.m_Picture.m_x = x+7;
	m_XianJin.m_Picture.m_y = y+150;
	
	m_ButtonPetMode.SetXY(x,y+80);
	m_tButtonPetMode.SetXCenter(m_ButtonPetMode.GetX() + m_ButtonPetMode.GetShowWidth() / 2);
	m_tButtonPetMode.SetY(m_ButtonPetMode.GetY() + 3);
	m_tButtonPetMode.UpdateXCenter();

	m_ZuoQiB.SetXY(2+x,30+y);
	m_ZuoQiZhuangShiB.SetXY(2+m_xPos,55+m_yPos);
	//������Ʒ��λ��
	for (int i = 0; i < 6; i++)
	{
		int xNum = i % 2;
		int yNum = i / 2;
		m_EquipHead[i].SetXY(xNum * 54 + 201 + m_xPos, yNum * 54 + 29 + m_yPos);
		m_EquipBackP[i].m_x = m_EquipHead[i].GetX();
		m_EquipBackP[i].m_y = m_EquipHead[i].GetY();
		
	}
	for (int i = 0; i < 3; i++)
	{
		m_PetEquipHead[i].SetXY(m_xPos+ i * 54 +10 , m_yPos+148);
		m_PetEquipBackP[i].m_x = m_PetEquipHead[i].GetX();
		m_PetEquipBackP[i].m_y = m_PetEquipHead[i].GetY();
	}
	m_rectPetEquip.left = m_PetEquipHead[0].GetX();
	m_rectPetEquip.right = m_PetEquipHead[2].GetX()+54;
	m_rectPetEquip.top = m_PetEquipHead[0].GetY();
	m_rectPetEquip.bottom = m_PetEquipHead[0].GetY()+54;
	for (int i = 0; i < 20; i++)
	{
		int xNum = i % 5;
		int yNum = i /5;
		m_ItemHead[i].SetXY(m_xPos + xNum * 51 + 29, m_yPos+yNum * 51 + 200);
		m_TItemNum[i].SetXY(m_xPos + xNum * 51 + 31, m_yPos + yNum * 51 + 202);
	}
	//��������λ��
	m_TXianJinN.SetXY(m_xPos + 65, m_yPos + 155);
	m_TXianJin.SetXY(m_xPos + 12, m_yPos + 152);
	m_TZuoQi.SetXCenter(m_ZuoQiB.GetCenter());
	m_TZuoQi.SetY(m_ZuoQiB.m_Picture.m_y + 2);
	m_TZuoQi.UpdateXCenter();

	m_TZhuangShi.SetXCenter(m_ZuoQiZhuangShiB.GetCenter());
	m_TZhuangShi.SetY(m_ZuoQiZhuangShiB.m_Picture.m_y + 2);
	m_TZhuangShi.UpdateXCenter();

	m_BigBigHead.m_Picture.m_x = m_xPos + 38;
	m_BigBigHead.m_Picture.m_y = m_yPos + 25;

	m_Obj.SetXY(
		m_xPos + 97,
		m_yPos + 118);
	m_Obj.MoveShadow();
	m_PetName.SetXY(
		m_xPos + 185,
		m_yPos + 53);
	
	m_rectEquip.left = m_xPos + 200;
	m_rectEquip.top = m_yPos + 30;
	m_rectEquip.right = m_rectEquip.left + 102;
	m_rectEquip.bottom = m_rectEquip.top + 152;
	
	m_rectItem.left = m_xPos + 25;
	m_rectItem.top = m_yPos + 200;
	m_rectItem.right = m_rectItem.left + 255;
	m_rectItem.bottom = m_rectItem.top + 203;
	return TRUE;
}

void cInterfaceItem::SetHaveZuoQi(BOOL b)
{
	m_bCharHaveZuoQi = b;
	m_ZuoQiB.m_NeedShow = b;
}

void cInterfaceItem::SetEquipZuoQiZhuangShi(BOOL b)
{
	m_ZuoQiZhuangShiB.m_NeedShow = b;
}

BOOL cInterfaceItem::CheckZuoQi()
{
	cCharacter* p = (cCharacter*)g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->m_pIndex;
	if (p->m_PCData.m_ZuoQi.m_pZuoQi)
	{
		SetHaveZuoQi(TRUE);	
		//�Ƿ���װ��
		if (p->m_PCData.m_ZuoQi.m_ZhuangShi.GetNum())
			SetEquipZuoQiZhuangShi(TRUE);
		else
			SetEquipZuoQiZhuangShi(FALSE);
	}
	else
		SetHaveZuoQi(FALSE);
	return TRUE;
}

BOOL cInterfaceItem::SetXianJin(BOOL b)
{
	cCharacter* p = (cCharacter*)g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->m_pIndex;
	ostringstream oss;
	m_bXianJin = b;
	if (m_bXianJin)
	{
		oss << p->m_PCData.m_Money[0];
		g_pMainState->m_Canvas.SetRedraw(m_TXianJinN.m_PreShowRect);
		m_TXianJinN.SetString( oss.str());
		m_TXianJin.SetString( "�ֽ�");
		
	}
	else
	{
		oss << p->m_PCData.m_Money[2];
		g_pMainState->m_Canvas.SetRedraw(m_TXianJinN.m_PreShowRect);
		m_TXianJinN.SetString( oss.str());
		m_TXianJin.SetString( "����");
	}
	return TRUE;
}

BOOL cInterfaceItem::ProcessSelectMap()
{
	//0����
	//1����
	//2����
	//3AL
	//4����Ů��
	//5����
	//6���Ϲ�
	m_Select = 2;
	if(CheckRB())return TRUE;
	if (!isInRect(g_xMouse, g_yMouse, &m_WorldMap))
		return TRUE;
	m_NumMapSelect = -1;
	int xMouse= g_xMouse - m_WorldMap.GetX();
	int yMouse= g_yMouse - m_WorldMap.GetY();
	for (int i = 0; i < 7; i++)
	{
		if (isOn(xMouse, yMouse, m_MapRect[i]))
		{
			m_NumMapSelect = i;
			m_MapSelect.Load(m_MapID[i]);
			m_MapSelect.m_Picture.m_x = m_MapRect[i].left + m_WorldMap.m_Picture.m_x;
			m_MapSelect.m_Picture.m_y = m_MapRect[i].top + m_WorldMap.m_Picture.m_y;
			m_MapSelect.m_bMove = TRUE;
			break;
		}
	}
	if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
	{
		g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
		if (m_NumMapSelect == -1)return TRUE;
		
			m_Mode = FALSE;
			g_pMainState->m_Canvas.SetRedraw(m_WorldMap.GetPicRange());
		
		g_pMainState->InterfaceSetShow(this, FALSE);
		(*pItemnum)-=1;
		switch (m_NumMapSelect)
		{
		case 0:
			return	g_pMainState->m_Map.LoadMap(107,60, "���ٴ�");
		case 1:
			return	g_pMainState->m_Map.LoadMap(60, 27, "������");
		case 2:
			return	g_pMainState->m_Map.LoadMap(355, 247, "������");
		case 3:
			return	g_pMainState->m_Map.LoadMap(127, 92, "������");
		case 4:
			return	g_pMainState->m_Map.LoadMap(93, 31, "����Ů��");
		case 5:
			return	g_pMainState->m_Map.LoadMap(115, 72, "�����");
		case 6:
			return	g_pMainState->m_Map.LoadMap(143, 95, "���Ϲ�");
		}
	
	}
	return TRUE;
}

BOOL cInterfaceItem::ShowSelectMap()
{
	g_pMainState->m_Canvas.Add(&m_WorldMap, 1,TRUE);
	if (m_NumMapSelect>-1)
		g_pMainState->m_Canvas.Add(&m_MapSelect, 1);
	
	return TRUE;
}

void cInterfaceItem::OnOff(BOOL NeedShow)
{
	g_pMainState->InterfaceSetShow(this, NeedShow);
	g_pMainState->m_Canvas.SetRedraw(m_Item.m_PreShowRect);
	if (m_Describe.m_NeedShow)m_Describe.OnOff(FALSE);
	if (m_NeedShow)
	{
		Init();
		m_Cancel.SetFrame(0);
		for (int i = 0; i < 20; i++)
		{
			int id = i  + m_ItemPage * 20;
			LoadSmallMap(id);
			UpdateItemNum(id);
		}
		UpdateData();
		if (g_pCharacter->m_PCData.m_FightPetID < 0)
			m_ButtonPetMode.m_NeedShow=FALSE;
		else m_ButtonPetMode.m_NeedShow = TRUE;
		CheckZuoQi();
	}

}

void cInterfaceItem::SetEquipSpecial2(sItem2& ProPerty, int ID)
{
	if (ID > 49)
	{
		if (ProPerty.m_Property.size()>6)
		ProPerty.m_Property[6] = ID;
		else ERRBOX;
	}
}

void cInterfaceItem::UpdateItemNum(int ID)
{
	if (ID >1000)return ;
	ostringstream oss;
	
	cCharacter* p = (cCharacter*)g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->m_pIndex;
	sItem2* pItem;
	int trueid = ID - m_ItemPage * 20;
	pItem = &p->m_PCData.m_Item[ID];
	if (trueid >= 20)return;
	if (trueid < 0)return;

	if (!pItem->GetNum())return;
	oss << pItem->GetNum();
	m_TItemNum[trueid].SetString(oss.str());
}

BOOL cInterfaceItem::ProcessSelectMap2()
{
	g_pMainState->SetMouseOnMap(FALSE);
	m_Select = 2;
	CheckRB();
	if (g_pMainState->Mouse.GetXDelta() != 0 || g_pMainState->Mouse.GetYDelta() != 0)
	{
		UpdateMousePosBack();
	}
	//������
	for (int i = 0; i < m_pMapSelect->m_num; i++)
	{
		if (isOn(g_xMouse, g_yMouse, m_PathPointp[i].m_PreShowRect))
		{
			if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
			{
				g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
				g_pMainState->m_Map.LoadMap(m_pMapSelect->m_xorg[i], m_pMapSelect->m_yorg[i], (CHAR*)m_pMapSelect->m_Name.c_str());
				m_Mode = 0;
				g_pMainState->m_Canvas.SetRedraw(m_MapSelect.m_PreShowRect);
				OnOff(FALSE);
				return TRUE;
			}
			
		}
	}
	return TRUE;
}

BOOL cInterfaceItem::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	m_Describe.OnOff(FALSE);
	switch (m_Select)
	{
	case 0:
		if (1)
		{
			if (m_SwapMode)
			{
				m_SwapMode = FALSE;
			}
			else
			{
				cCharacter* pc = (cCharacter*)g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->m_pIndex;
				if (m_NowSelect > -1)
				{
					sItem2* pItem = pc->m_PCData.GetItem(m_NowSelect);
					if (pItem->GetNum())
					{
						if (m_Mode==2)
						{
							UseItem(m_NowSelect, g_pMainState->m_HeroID);

							for (int i = 0; i < 3; i++)
							{
								sItem2& item = pPet->m_PetDataZZ.m_PetEquip[i];
								if (item.GetNum())
								{
									m_PetEquipHead[i].Load(g_pMainState->m_ItemManage.GetItem(item.GetType(), item.GetID())->m_SmallHead);
								}
							}

							
						}
						else
						{
							UseItem(m_NowSelect, g_pMainState->m_HeroID);
							if (g_pMainState->m_State.m_NeedShow)
								g_pMainState->m_State.UpDatePcDate();
						}
					}
				}
				else
				{
					OnOff(FALSE);
				}
			}
			m_SwapID = -1;
		}
		break;
	case 1:
		g_pMainState->m_Canvas.SetRedrawAll();
		break;
	case 2:
	case 3:
	case 4:
	case 5:
		g_pMainState->m_Canvas.SetRedraw(m_MapBack.m_PreShowRect);
		g_pMainState->m_Canvas.SetRedraw(m_Item.m_PreShowRect);
		m_Mode = 0;	
		break;
	default:
		ERRBOX;
		break;
	}
	return TRUE;
}

BOOL cInterfaceItem::ShowSelectMap2()
{
	g_pMainState->m_Canvas.Add(&m_MapBack,1,1);
	for (int i = 0; i < m_pMapSelect->m_num; i++)
	{
		g_pMainState->m_Canvas.Add(&m_PathPointp[i], 1);
	}
	if (m_MousePos.m_NeedShow)
	{
		g_pMainState->m_Canvas.Add(&m_MousePosBack, 1);
		g_pMainState->m_Canvas.Add(&m_MousePos);
	}
	return TRUE;
}

void cInterfaceItem::InitMode(int id)
{
	m_Mode = id;
	g_pMainState->m_Canvas.SetRedraw(g_pMainState->m_InterfaceItem.m_Item.m_PreShowRect);
	switch (m_Mode)
	{
	case 0:
		return;
	case 1:
		g_pMainState->m_Canvas.SetRedrawAll();return;
	case 12:
	case 13:
	case 14:
	case 15:
		m_pMapSelect = &m_sMapSelect[m_Mode - 12];
		m_MapBack.Load(m_pMapSelect->m_SmallMap);	
		UpdateMousePosBack();
		break;
	case 6://�¹�
		if (1)
		{
			sItem2* pItem=0;
			int i;//Ѱ���¹ⱦ�е�λ��
			for (i = 0; i <60; i++)
			{
				pItem = &g_pCharacter->m_PCData.m_Item[i];
				if (pItem->GetType()== 24)
				{
					if (11 == pItem->GetID())
					{
						break;
					}
				}
			}
			if (0 == pItem)ERRBOX;
			string str;
			if (pItem->m_Property.size()==3)
			{
				ostringstream oss;
				oss << "�Ƿ�Ҫ���͵�#Y";
				oss << pItem->m_strProperty;
				oss << "#Y#R ";
				oss << pItem->m_Property[1];
				oss << " ";
				oss << pItem->m_Property[2];
				oss << "#R ?";
				str = oss.str();
				g_pMainState->m_Dialog.Reset();
				g_pMainState->m_Dialog.SetObj(g_pCharacter->m_PCData.m_Name);
				g_pMainState->m_Dialog.SetString(str, "����", "�����ﶨ����");
				g_pMainState->m_Dialog.OnOff(TRUE);
				g_pMainState->m_Dialog.m_bRetSelect = TRUE;
			}
			else
			{
				cObj* pUser= g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData;
				pItem->m_strProperty = g_pMainState->m_Map.m_MapName;
				int xpos = pUser->GetX()/20;
				int ypos = (g_pMainState->m_Map.m_pMap->m_Height-  pUser->GetY())/20;
				pItem->m_Property.push_back(xpos);
				pItem->m_Property.push_back(ypos);
				g_pMainState->m_Channel.AddNewString("���ڵ�ǰλ�ö�������");
				InitMode(0);
			}
			return;
		}
		break;
	default:
		ERRBOX;
		break;
	}
	m_MapBack.SetXY(320 - m_MapBack.GetWidth() / 2, 240 - m_MapBack.GetHeight() / 2);
	int num = m_pMapSelect->m_x.size();
	for (int i = 0; i < num; i++)
	{
		m_PathPointp[i].SetXY(m_MapBack.GetX() + m_pMapSelect->m_x[i], m_MapBack.GetY()+m_pMapSelect->m_y[i]);
	}
}

void cInterfaceItem::UpdateMousePosBack()
{
	m_MousePos.SetXY(g_xMouse - m_MousePosBack.m_Picture.m_Width / 2 + 10, g_yMouse - 15);
	m_MousePosBack.m_Picture.m_x = g_xMouse - m_MousePosBack.m_Picture.m_Width / 2;
	m_MousePosBack.m_Picture.m_y = g_yMouse - 20;
	int tx = (g_xMouse - m_MapBack.m_Picture.m_x - 18);
	int ty = (g_yMouse - m_MapBack.m_Picture.m_y - 18);
	tx = (int)(tx*m_pMapSelect->m_bili);
	ty = (int)(ty*m_pMapSelect->m_bili);
	ty = m_pMapSelect->m_Height - ty;
	tx /= 20;
	ty /= 20;
	ostringstream oss;
	oss << "X:";
	oss << tx;
	oss << " Y:";
	oss << ty;
	m_MousePos.SetString(oss.str());
}

void cInterfaceItem::SetEquipSpecial(sItem2& ProPerty, int ID)
{
	if (ID < 50)
	{
		if (ProPerty.m_Property.size()>5)
		ProPerty.m_Property[5] = ID; //�����ؼ�
		else ERRBOX;
	}
}

void cInterfaceItem::UpdateData()
{
	if (m_Mode == 2)
	{
		if (g_pCharacter->m_PCData.m_FightPetID > -1)
		{
			pPet = g_pCharacter->m_PCData.m_pPetDataList[g_pCharacter->m_PCData.m_FightPetID];
			m_Obj.m_pIndex = pPet;
			m_Obj.Reset();
			m_Obj.Set(POS_STAND);
			m_PetName.SetString(pPet->m_PetDataZZ.m_Name);
			for (int i = 0; i < 3; i++)
			{
				sItem2& item = pPet->m_PetDataZZ.m_PetEquip[i];
				if (item.GetNum())
				{
					m_PetEquipHead[i].Load(g_pMainState->m_ItemManage.GetItem(item.GetType(), item.GetID())->m_SmallHead);
				}
			}
		}
	}
	else
	{
		CheckZuoQi();
		for (int i = 10000; i < 10006; i++)
		{
			LoadSmallMap(i);
		}
		cCharacter* pc = (cCharacter*)g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData->m_pIndex;
		ostringstream oss;
		oss << pc->m_PCData.m_Money[1];//����
		m_TCunYingN.SetString(oss.str());
		SetXianJin(m_bXianJin);
		m_BigBigHead.Load(pc->GetID(POS_BIGHEAD));
	}
	g_pMainState->m_Canvas.SetRedraw(m_Headback.m_PreShowRect);
}

cInterfaceItem::~cInterfaceItem()
{
	m_Obj.m_pIndex = 0;
	m_Obj.m_IndexType = 0;
}



BOOL cFullMagic::SetMagicUp(int dataid,DWORD id, int x, int y)
{
	m_FullSkilUP[dataid].Load(id);
	m_FullSkilUP[dataid].SetFrame(0);
	m_FullSkilUP[dataid].m_NeedShow = TRUE;
	m_FullSkilUP[dataid].m_Picture.m_x = x;
	m_FullSkilUP[dataid].m_Picture.m_y = y;
	return TRUE;
}

cFullMagic::cFullMagic()
{
	m_FullSkilUP.resize(3);
	for (int i = 0; i < 3; i++)
	{
		m_FullSkilUP[i].m_NeedShow = FALSE;
		m_FullSkilUP[i].m_Picture.m_x = 0;
		m_FullSkilUP[i].m_Picture.m_y = 0;
	}
	m_SufferMagic.resize(20);
}

BOOL cInterfaceChannel::Init()
{
	for (int i = 0; i < 30; i++)
	{
		m_ContextList[i].m_ShowStyle = 2;
		m_ContextList[i].m_Name.m_hFont = g_pMainState->m_hFont[0];
		m_ContextList[i].m_Name.SetColor(RGB(0, 255, 0));
		for (int k = 0; k < 6; k++)
		{
			m_ContextList[i].m_TextLineList[k].m_line.SetColor(RGB(255, 255, 255));
			m_ContextList[i].m_TextLineList[k].m_line.m_hFont = g_pMainState->m_hFont[0];
		}
	}
	m_Channel[0].Load(0x65C5B7EE);
	m_Channel[1].Load(0xF9858C95);
	m_Channel[2].Load(0x1B1DCE56);
	m_Channel[3].Load(0x43700E25);
	m_Channel[4].Load(0xF9ADC3DA);
	ifstream File;
	ostringstream oss;
	oss << g_strMediaPath << "����//����//����.txt";
	File.open(oss.str());
	
	string dustbin;
	for (int i = 0; i < 120;i++)
	{
		File >> m_FaceId[i];
		getline(File, dustbin);
	}
	File.close();

	m_HeightChannel = 120;
	m_WidthChannel = 310;
	m_NeedShow = TRUE;
	m_Back.Load(0xA0120D38);
	for (int i = 0; i < 4; i++)
	{
		m_BackP[i].Set(&m_Back);
		m_BackP[i].m_bStatic = TRUE;
		m_BackP[i].m_bShadow = 20;
	}
	m_BackP[0].SetxOffset(m_Back.m_Picture.m_Width - m_WidthChannel / 2);
	m_BackP[2].SetxOffset(m_BackP[0].GetxOffset());
	m_BackP[0].SetyOffset(m_Back.m_Picture.m_Height - m_HeightChannel / 2);
	m_BackP[1].SetyOffset(m_BackP[0].GetyOffset());

	m_BackP[1].SetxOffset2(m_Back.m_Picture.m_Width - m_WidthChannel / 2);
	m_BackP[3].SetxOffset2(m_BackP[1].GetxOffset2());
	m_BackP[1].SetShowWidth(m_WidthChannel / 2);
	m_BackP[3].SetShowWidth(m_BackP[1].GetShowWidth());

	m_BackP[2].SetyOffset2(m_Back.m_Picture.m_Height - m_HeightChannel / 2);
	m_BackP[2].SetShowHeight(m_HeightChannel / 2);

	m_BackP[3].SetyOffset2(m_BackP[2].GetyOffset2());
	m_BackP[3].SetShowHeight(m_BackP[2].GetShowHeight());


	TEXTMETRIC tx;
	GetTextMetrics(g_DC, &tx);
	m_HeightCube = tx.tmHeight+3;
	m_MoveIcon.Load(0x32BE710D);
	m_MoveIcon.m_NeedShow = TRUE;
	m_MoveIcon.m_Picture.m_x = m_xPos ;
	m_MoveIcon.m_Picture.m_y = m_yPos + m_HeightChannel - 20;
	Move(0, 480 - m_HeightChannel);
	m_ContextStart=-1;  //��1����λ��
	return TRUE;
}

BOOL cInterfaceChannel::Show(cCanvas* pCanvas)
{
	for (int i = 0; i < 4; i++)
		pCanvas->Add(&m_BackP[i], 1);

	int linestart = m_ContextShowStart;
	//��������ʾ����
	for (int i = 0; i < m_ContextShowNum; i++)
	{

		m_ContextList[linestart].Show(pCanvas);
		linestart -= 1;
		if (linestart < 0)linestart = 29;
	}
	return TRUE;
}

BOOL cInterfaceChannel::ProcessInput()
{
	return TRUE;
}

BOOL cInterfaceChannel::AddNewString(string str, string name, int channel)
{
	

	if (channel < 2)//����ǵ�ǰ�����
	{
		int i = 0;
		for (; i < 5; i++)
		{
			if (g_pMainState->m_FightList[i].isLive)
			if (0 == g_pMainState->m_FightList[i].m_pData->m_TrueName.GetString().compare(name))
			{
				g_pMainState->m_FightList[i].m_pData->m_ContextList.SetFaceText(str, g_pMainState->m_hFont[0], 114);
				g_pMainState->m_FightList[i].m_pData->m_ShowTime = 210;
				g_pMainState->m_FightList[i].m_pData->MoveDialog();
				break;
			}
		}
		if (i == 5)
		{
			for (i=10; i < 15; i++)
			{
				if (g_pMainState->m_FightList[i].isLive)
				if (0 == g_pMainState->m_FightList[i].m_pData->m_TrueName.GetString().compare(name))
				{
					g_pMainState->m_FightList[i].m_pData->m_ContextList.SetFaceText(str, g_pMainState->m_hFont[0], 114);
					g_pMainState->m_FightList[i].m_pData->m_ShowTime = 210;
					g_pMainState->m_FightList[i].m_pData->MoveDialog();
					break;
				}
			}
		}
		if (i == 15)
		{
			g_pMainState->m_TriggerDialog.Process(1, TRUE);
			return FALSE;
		}
	}
	
	m_ContextStart += 1;
	if (m_ContextStart > 29)
		m_ContextStart = 0;
	if (m_ContextNum < 30)
		m_ContextNum += 1;
	m_ContextList[m_ContextStart].SetFaceText(&m_Channel[channel], name, str, g_pMainState->m_hFont[0], m_WidthChannel - 10);
	m_ContextShowStart = m_ContextStart;


	for (int i = 0; i < 4;i++)
	g_pMainState->m_Canvas.SetRedraw(m_BackP[i].m_PreShowRect);
	UpDateShow();
	return TRUE;
}

BOOL cInterfaceChannel::Move(int x, int y)
{
	m_xPos =x;
	m_yPos =y;
	m_BackP[0].m_x = x;
	m_BackP[0].m_y = y;
	m_BackP[1].m_x = x+m_WidthChannel/2;
	m_BackP[1].m_y = y;
	m_BackP[2].m_x = x;
	m_BackP[2].m_y = y + m_HeightChannel / 2;
	m_BackP[3].m_x = m_BackP[1].m_x;
	m_BackP[3].m_y = m_BackP[2].m_y;
	return TRUE;
}

BOOL cInterfaceChannel::CheckL()
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
	return FALSE;
}

void cInterfaceChannel::UpDateShow()
{
	if (!m_ContextNum)return;
	//const int gap = 10;
	//��������ʾ����
	int linestart = m_ContextShowStart;
	int xStart=m_xPos+2;
	//���㴰�������ɵĶԻ���
	int yStart = 5;
	m_ContextShowNum = 0;
	while (true)
	{
		yStart += m_ContextList[linestart].m_Height;
		if (yStart>m_HeightChannel)
			break;
		if (m_ContextShowNum==m_ContextNum)
			break;
		linestart -= 1;
		if (linestart < 0)linestart = 29;
		m_ContextShowNum += 1;
	}
	linestart += 1;
	if (linestart>29)linestart = 0;
	yStart = m_yPos+5;
	//���öԻ���λ��
	for (int i = 0; i < m_ContextShowNum;i++)
	{
		m_ContextList[linestart].SetPos(xStart, yStart);
		yStart += m_ContextList[linestart].m_Height;
		linestart += 1;
		if (linestart > 29)linestart = 0;
	}
}


BOOL cInterfaceTeam::Init()
{
	if (m_bInit)return TRUE;
	m_bInit = TRUE;
	m_xPosList.resize(20);
	m_yPosList.resize(20);
	m_tMengPai.resize(5);
	m_TeamTurn.resize(20);
	m_lineupSelectBP.resize(11);
	m_lineupSelectBPT.resize(11);
	m_Describe.resize(2);
	m_lineupPeoplepp.resize(5);
	m_lineupPeoplep.resize(5);
	m_lineupSelectB.Set(&g_pMainState->m_Button4);
	m_lineupSelectB.m_bStatic = TRUE;
	m_SetFriend.Set(&g_pMainState->m_Button4);
	m_SetFriend.m_bStatic = TRUE;

	m_Fighter.resize(5);
	m_tName.resize(5);
	m_tLv.resize(5);
	for (int i = 0; i < m_TeamTurn.size(); i++)
	{
		m_TeamTurn[i] = i;
	}
	m_lineupB.Load(0x79D01E0E);
	m_lineupB.SetStatic(TRUE);
	m_lineupBT.SetColor(RGB(255, 255, 255));
	m_lineupBT.SetString("����");

	//m_ControlB.Set(&g_pMainState->m_Button4);
	//m_ControlB.m_bStatic=TRUE;
	//m_ControlBT.SetColor(RGB(255, 255, 255));
	//m_ControlBT.SetString("���ƶ���");


	m_tSetFriend.SetColor(RGB(255, 255, 255));
	m_tSetFriend.SetString("���ö���");
	m_lineupSelectBack.Load(0x8072710D);
	m_lineupSelectBack.SetStatic(TRUE);
	m_lineupPeople.Load(0x0E43E0DF);
	
	for (int i = 0; i < 11; i++)
	{
		m_lineupSelectBP[i].Set(&g_pMainState->m_Button4);
		m_lineupSelectBP[i].m_bStatic = TRUE;
		m_lineupSelectBPT[i].SetXY(0, 0);
		m_lineupSelectBPT[i].SetColor(RGB(255,255,255));
		m_lineupSelectBPT[i].m_hFont = g_pMainState->m_hFont[0];
	}
	m_Describe[0].SetColor(RGB(255,255,0));
	m_Describe[1].SetColor(RGB(0, 255, 0));
	m_Describe[1].LockWidth(211);
	m_lineupSelectBT.SetColor(RGB(255, 255, 255));
	m_lineupSelectBT.m_hFont = g_pMainState->m_hFont[0];
	m_lineupSelectBT.SetXY(0, 0);


	m_lineupSelectBPT[0].SetString("��ͨ��");
	m_lineupSelectBPT[1].SetString("������");
	m_lineupSelectBPT[2].SetString("������");
	m_lineupSelectBPT[3].SetString("������");
	m_lineupSelectBPT[4].SetString("������");
	m_lineupSelectBPT[5].SetString("������");
	m_lineupSelectBPT[6].SetString("�����");
	m_lineupSelectBPT[7].SetString("�츲��");
	m_lineupSelectBPT[8].SetString("�ƴ���");
	m_lineupSelectBPT[9].SetString("�׾���");
	m_lineupSelectBPT[10].SetString("ӥХ��");
	m_lineupSelectBT.SetString("ѡ������");

	m_lineupPeoplepT.resize(5);
	for (int i = 0; i < 5; i++)
	{
		m_lineupPeoplep[i].Set(&m_lineupPeople);
		m_lineupPeoplep[i].m_bStatic = TRUE;
		m_lineupPeoplepT[i].SetColor(RGB(255, 255, 255));
		m_lineupPeoplepT[i].m_hFont = g_pMainState->m_hFont[0];
	}
	
	m_lineupPeoplepT[0].SetString("1");
	m_lineupPeoplepT[1].SetString("2");
	m_lineupPeoplepT[2].SetString("3");
	m_lineupPeoplepT[3].SetString("4");
	m_lineupPeoplepT[4].SetString("5");

	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;
	m_lineupCancelB.Set( &g_pMainState->m_Cancel);
	m_lineupCancelB.m_bStatic = TRUE;
	
	m_Back.Load(0xF5B8E062);
	m_Back.SetStatic(TRUE);
	m_FighterSelectid = -1;
	m_FighterSelect.Load(0xC540D7A7);
	m_FighterSelect.SetStatic(TRUE);
	for (int i = 0; i < 5;i++)
	{
		m_Fighter[i].Init();
	}

	m_lineupName.m_hFont = g_pMainState->m_hFont[0];
	m_lineupName.SetString(m_lineupSelectBPT[0].GetString());
	m_lineupSelectBP[0].SetFrame(1);

	m_tSmall.SetString("0");
	m_tBig.SetString("175");
	m_pFightState = g_pMainState->m_pFightState;


	Move(320 - m_Back.m_Picture.m_Width / 2, 240 - m_Back.m_Picture.m_Height / 2);
	MoveLineSelect(320 - m_lineupSelectBack.m_Picture.m_Width / 2, 240 - m_lineupSelectBack.m_Picture.m_Height / 2);


	return TRUE;
}

void cInterfaceTeam::Move(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
	m_Back.m_Picture.SetXY(x, y);
	m_lineupName.SetXY(x+100,y+42);
	m_lineupB.SetX(x+20);
	m_lineupB.SetY(y+38);
	m_lineupBT.SetXY(x + 24,y+40);

	//m_ControlB.SetX(x + 380);
	//m_ControlB.SetY(y + 40);
	//m_ControlBT.SetXY(x + 384, y + 42);


	m_SetFriend.SetXY(x+300,y+40);
	m_tSetFriend.SetXCenter(m_SetFriend.GetCenter());
	m_tSetFriend.UpdateXCenter();
	m_tSetFriend.SetY(y + 43);
	m_tSmall.SetXY(x + 214, y + 42);
	m_tBig.SetXY(x + 259, y + 42);
	for (int i = 0; i < 5; i++)
	{
		m_Fighter[i].SetXY(x + 65 + i * 120, y +190);
		m_Fighter[i].MoveShadow();
		m_tName[i].SetXCenter(x + 65 + i * 120);
		m_tName[i].SetY( y + 220);
		m_tName[i].UpdateXCenter();
		m_tMengPai[i].SetXCenter(x+65+i*120);
		m_tMengPai[i].SetY( y + 243);
		m_tMengPai[i].UpdateXCenter();
		m_tLv[i].SetXY(x+16+i*119 ,y + 266);
	}
	m_Cancel.SetXY(x+593,y+4);

	m_FightRect.left = m_xPos + 10;
	m_FightRect.right = m_xPos + 600;
	m_FightRect.top = m_yPos + 72;
	m_FightRect.bottom = m_yPos + 211;
	m_FighterSelect.m_Picture.m_y = m_FightRect.top;
	m_FighterSelect.m_Picture.m_x = m_FightRect.left + 120 * m_FighterSelectid;
}

BOOL cInterfaceTeam::ProcessInput()
{
	ResetButton();
	m_Select = -1;	

	if (m_bMoveMode)//�ƶ�
	{
		m_Select = 6;
		g_pMainState->SetMouseOnMap(FALSE);
		if (!CheckL())
			m_bMoveMode = FALSE;
		return TRUE;
	}
	if (m_bLiineupSelect)
		return ProcessSelectLineup(g_xMouse,g_yMouse);
	if (isOn(g_xMouse, g_yMouse, m_Back.m_PreShowRect))
	{
		g_pMainState->SetMouseOnMap(FALSE);
		g_pMainState->m_Cursor.m_FollowMode = FALSE;
		//����ѡ��
		if (CheckOn(&m_lineupB,21))return TRUE;
	//	if (CheckOn(&m_ControlB, 24))return TRUE;
		if (CheckOn(&m_Cancel, 20))return TRUE;


		if (isOn(g_xMouse, g_yMouse, m_FightRect))
		{
			m_Select = (g_xMouse - m_FightRect.left) / 120;

			if (g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))
			{
				g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
				if (m_FighterSelectid > -1)
				{
					if (g_pMainState->m_FightList[m_TeamTurn[m_Select]].isLive)
					if (m_FighterSelectid != m_Select)
					{
						g_pMainState->m_InterfaceTeam.SwapTeamTurn(m_FighterSelectid, m_Select);
						UpDate();
						g_pMainState->m_PCHead.UpDateHead();
					}
				}
				return TRUE;
			}
			return CheckL();
		}
		if (CheckOn(&m_SetFriend, 8))return TRUE;
	
		m_Select = 0;
		if (CheckRB())return TRUE;
		m_Select = 7;
		if(CheckL())return TRUE;
	}
	return TRUE;
}

BOOL cInterfaceTeam::Show(cCanvas* pCanvas)
{
	if (!m_NeedShow)return TRUE;
	if (m_bLiineupSelect)
		return ShowSelectLineup(pCanvas);
	pCanvas->Add(&m_Back,1,1);
	pCanvas->Add(&m_lineupName);
	pCanvas->Add(&m_lineupB, 1);
	pCanvas->Add(&m_lineupBT);

	/*pCanvas->Add(&m_ControlB, 1);
	pCanvas->Add(&m_ControlBT);*/

	pCanvas->Add(&m_SetFriend, 1);
	pCanvas->Add(&m_tSetFriend);

	pCanvas->Add(&m_tSmall);
	pCanvas->Add(&m_tBig);
	if (m_FighterSelectid > -1)
	{
		pCanvas->Add(&m_FighterSelect, 1);
	}

	int fightnum = g_pMainState->GetLiveNum(0,5);
	for (int k = 0; k < fightnum; k++)
	{
		if (m_Fighter[k].m_pIndex)
		{
		pCanvas->Add(&m_tName[k]);
		pCanvas->Add(&m_tMengPai[k]);
		pCanvas->Add(&m_tLv[k]);
		pCanvas->Add(m_Fighter[k].GetShadow(), 1);
		m_Fighter[k].ShowOnScreen(pCanvas);
		}
		
	}
	pCanvas->Add(&m_Cancel, 1);
	return TRUE;
}

BOOL cInterfaceTeam::UpDate()
{
	int fightnum = g_pMainState->GetLiveNum(0,5);
	
	cPetData* pPetData;
	cObj* pPet;
	ostringstream oss;
	for (int i = 0; i < 5; i++)
	{
		FreeFighter(i);
	}
	BOOL bErr=FALSE;
	for (int i = 0; i < fightnum; i++)
	{
		if (!g_pMainState->m_FightList[m_TeamTurn[i]].isLive)
		{
			bErr = TRUE;
			if (!AutoSwap(i))return FALSE;
		}
		pPet= g_pMainState->m_FightList[m_TeamTurn[i]].m_pData;
		m_Fighter[i].m_pIndex = pPet->m_pIndex;
		m_Fighter[i].m_IndexType = pPet->m_IndexType;
		m_Fighter[i].Set(POS_STAND);
		pPet->GetDateP(pPetData);
		
		m_tName[i].SetString(pPetData->m_Name);
		if (m_Fighter[i].m_IndexType==INDEXTYEP_CHARACTER)
		{
			cMengPai* pMengPai = g_pMainState->m_SkillManager.m_MengPaiManager.GetMengPai((cCharacter*)pPet->m_pIndex);
			if (pMengPai)
			m_tMengPai[i].SetString(pMengPai->m_Name);
			else m_tMengPai[i].SetString("��");
		}
		else  	m_tMengPai[i].SetString("");
		oss.str("");
		oss << pPetData->m_LV<<"��";
		m_tLv[i].SetString(oss.str());
		m_tName[i].UpdateXCenter();
		m_tMengPai[i].UpdateXCenter();
	}
	if (bErr)
		g_pMainState->m_PCHead.UpDateHead();
	return TRUE;
}

BOOL cInterfaceTeam::CheckL()
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
		m_FighterSelectid = m_Select;
		m_FighterSelect.m_Picture.m_x = m_FightRect.left + 120 * m_FighterSelectid;
		m_FighterSelect.m_bMove = TRUE;
		break;
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
				g_pMainState->m_Canvas.SetRedraw(m_Back.m_PreShowRect);
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
		{
			if (m_FighterSelectid < 0)return TRUE;
			int id = m_TeamTurn[m_FighterSelectid];
			if (id == g_pMainState->m_HeroID)return TRUE;
			if (!g_pMainState->m_FightList[id].isLive)return TRUE;
			cPetData* ppetdata;
			g_pMainState->m_FightList[id].m_pData->GetDateP(ppetdata);
			id= g_pMainState->m_Friend.FindFriend(ppetdata->m_Name);
			g_pMainState->m_Friend.m_friendqianghua.Set(g_pMainState->m_Friend.m_FriendList[id]);
			g_pMainState->m_Friend.m_friendqianghua.OnOff(TRUE);
		}
		break;
	case 20:
		OnOff(FALSE);
		m_Cancel.SetFrame(1);
		break;
	case 21:
		if (1)
		{
			m_bLiineupSelect = TRUE;
			g_pMainState->m_Canvas.SetRedraw(m_lineupSelectBack.m_PreShowRect);
			m_Back.m_bMove = TRUE;
			SetlineupPeople(m_eOurFormation);
			int havet = g_pMainState->m_GlobalButton[200];
			m_lineupSelectBP[0].SetFrame(0);
			for (int i = 1; i < 11; i++)
			{
				if (havet & (1<<(i-1)))
				{
					m_lineupSelectBP[i].SetFrame(0);
				}
				else m_lineupSelectBP[i].SetFrame(3);
			}
			m_plineupButton = 0;
			m_plineupButton = &m_lineupSelectBP[m_eOurFormation];
			m_plineupButton->SetFrame(1);
			m_plineupButton->m_bMove = TRUE;
			m_lineupName.SetString(m_lineupSelectBPT[m_eOurFormation].GetString());
		}
		break;
	case 22:
		m_bLiineupSelect = FALSE;
		break;
	case 23://������
		SetFormation(m_eLineupSelect,0,TRUE);
		break;
		/*case 24:
			m_ControlB.SetFrame(1);
			g_pMainState->m_pFightState->m_FightMenu.SetTeamControl();
			break;*/
		//��
	case 100:
	case 101:
	case 102:
	case 103:
	case 104:
	case 105:
	case 106:
	case 107:
	case 108:
	case 109:
	case 110:
		if (1)
		{
			if (m_plineupButton)
			{
				m_plineupButton->SetFrame(0);
				m_plineupButton->m_bMove = TRUE;
				m_plineupButton = 0;
			}
			m_plineupButton = m_framprep;
			m_framprep->SetFrame(1);
			m_framprep = 0;
			m_eLineupSelect = (eFormation)(m_Select - 100);
			SetlineupPeople(m_eLineupSelect);
		}
		break;
	}
	return TRUE;
}

BOOL cInterfaceTeam::ProcessSelectLineup(int xMouse,int yMouse)
{
	if (!isOn(xMouse, yMouse, m_lineupSelectBack.m_PreShowRect))return FALSE;
	g_pMainState->SetMouseOnMap(FALSE);
	for (int i = 0; i < 11; i++)
	{
		if (3 == m_lineupSelectBP[i].GetFrame())continue;
		if (1 == m_lineupSelectBP[i].GetFrame())continue;
		if (CheckOn(&m_lineupSelectBP[i], 100 + i))return TRUE;
	}
	//ȡ��
	if (CheckOn(&m_lineupCancelB, 22))return TRUE;
	//ѡ����
	if (CheckOn(&m_lineupSelectB, 23))return TRUE;
	m_Select = 1;
	if (CheckRB())return TRUE;
	return TRUE;
}

void cInterfaceTeam::MoveLineSelect(int x, int y)
{
	m_lineupSelectBack.SetX(x);
	m_lineupSelectBack.SetY(y);
	m_lineupCancelB.m_x = x + m_lineupSelectBack.GetWidth()-22;
	m_lineupCancelB.m_y = y+4;
	m_Describe[0].SetXY(x+270,y+24);
	m_Describe[1].SetXY(x+270,y+44);
	for (int i = 0; i < 11; i++)
	{
		int xo = i % 3;
		int yo = i / 3;
		m_lineupSelectBP[i].SetXY(x + 23 + xo * 80, y + 38 + yo * 30);
		m_lineupSelectBPT[i].SetXCenter(m_lineupSelectBP[i].GetX() + m_lineupSelectBP[i].GetShowWidth()/2);
		m_lineupSelectBPT[i].SetY(m_lineupSelectBP[i].GetY()+3);
		m_lineupSelectBPT[i].UpdateXCenter();
	}
	m_lineupSelectB.SetXY(x + 25, y + 280);
	m_lineupSelectBT.SetXCenter(m_lineupSelectB.GetCenter());
	m_lineupSelectBT.SetY(y + 283);
	m_lineupSelectBT.UpdateXCenter();
}

BOOL cInterfaceTeam::ShowSelectLineup(cCanvas* pCanvas)
{
	pCanvas->Add(&m_lineupSelectBack, 1,1);
	pCanvas->Add(&m_lineupCancelB, 1);
	pCanvas->Add(&m_Describe[0]);
	pCanvas->Add(&m_Describe[1]);
	for (int i = 0; i < 5; i++)pCanvas->Add(m_lineupPeoplepp[i], 1);
	for (int i = 0; i < 5; i++)pCanvas->Add(&m_lineupPeoplepT[i]);

	for (int i = 0; i < 11; i++)
	{
		pCanvas->Add(&m_lineupSelectBP[i], 1);
		pCanvas->Add(&m_lineupSelectBPT[i]);
	}
	pCanvas->Add(&m_lineupSelectB, 1);
	pCanvas->Add(&m_lineupSelectBT);
	return TRUE;
}

void cInterfaceTeam::SetlineupPeople(eFormation select)
{
	//ͼƬ�� 52  54 ��ƫ��
	int xPos = m_xPos + 52;
	int yPos = m_yPos - 54;
	int xOf1 = 38;
	int yOf1 = 19;
	m_Describe[0].SetString(m_lineupSelectBPT[select].GetString());
	string str;
	switch (select)
	{
	case normal:
		if (1)
		{
			str = "û���κ�Ч��";
			m_Describe[1].SetString(str);
			int xstart = xPos + 145;
			int ystart = yPos + 240;
			m_lineupPeoplep[0].m_x = xstart + xOf1 * 2;
			m_lineupPeoplep[0].m_y = ystart - yOf1 * 2;
			m_lineupPeoplep[1].m_x = xstart + xOf1 * 3;
			m_lineupPeoplep[1].m_y = ystart - yOf1 * 3;
			m_lineupPeoplep[2].m_x = xstart + xOf1 * 1;
			m_lineupPeoplep[2].m_y = ystart - yOf1 * 1;
			m_lineupPeoplep[3].m_x = xstart + xOf1 * 4;
			m_lineupPeoplep[3].m_y = ystart - yOf1 * 4;
			m_lineupPeoplep[4].m_x = xstart;
			m_lineupPeoplep[4].m_y = ystart;
		}
		break;
	case land://����
		if (1)
		{
			str = "1,3,4��λ�﷨����+15%, 2��λ�﷨�˺�+15%, 5��λ�ٶ�+10%,����λ�﷨�˺�+10%,";
			m_Describe[1].SetString(str);
			int xstart = xPos + 145;
			int ystart = yPos + 240;
			m_lineupPeoplep[0].m_x = xstart + xOf1 * 2;
			m_lineupPeoplep[0].m_y = ystart - yOf1 * 2;
			m_lineupPeoplep[2].m_x = xstart + xOf1 * 3;
			m_lineupPeoplep[2].m_y = ystart - yOf1 * 3;
			m_lineupPeoplep[3].m_x = xstart + xOf1 * 1;
			m_lineupPeoplep[3].m_y = ystart - yOf1 * 1;

			m_lineupPeoplep[1].m_x = xstart + xOf1 * 1;
			m_lineupPeoplep[1].m_y = ystart - yOf1 * 3;

			m_lineupPeoplep[4].m_x = xstart + xOf1 * 3;
			m_lineupPeoplep[4].m_y = ystart - yOf1 * 1;
		}
		break;
	case wind:
		if (1)
		{
			str = "1��λ�﷨�˺�+20%,  4,5��λ�ٶ�+10%,����λ�﷨�˺�+10%";
			m_Describe[1].SetString(str);
			int xstart = xPos + 145;
			int ystart = yPos + 240;
			m_lineupPeoplep[0].m_x = xstart + xOf1 * 2;
			m_lineupPeoplep[0].m_y = ystart - yOf1 * 2;
			m_lineupPeoplep[1].m_x = xstart + xOf1 * 3;
			m_lineupPeoplep[1].m_y = ystart - yOf1 * 3;
			m_lineupPeoplep[2].m_x = xstart + xOf1 * 1;
			m_lineupPeoplep[2].m_y = ystart - yOf1 * 1;
			m_lineupPeoplep[3].m_x = xstart + xOf1 * 2;
			m_lineupPeoplep[3].m_y = ystart - yOf1 * 4;
			m_lineupPeoplep[4].m_x = xstart;
			m_lineupPeoplep[4].m_y = ystart-yOf1*2;
		}
		break;
	case tiger:
		if (1)
		{
			str = "1��λ�﷨�˺�+25%, 2,3��λ�﷨����+10%, ����λ�﷨�˺�+10%";
			m_Describe[1].SetString(str);
			int xstart = xPos + 145;
			int ystart = yPos + 240;
			m_lineupPeoplep[0].m_x = xstart + xOf1 * 3;
			m_lineupPeoplep[0].m_y = ystart - yOf1 * 1;
			m_lineupPeoplep[1].m_x = xstart + xOf1 * 3;
			m_lineupPeoplep[1].m_y = ystart - yOf1 * 3;
			m_lineupPeoplep[2].m_x = xstart + xOf1 * 1;
			m_lineupPeoplep[2].m_y = ystart - yOf1 * 1;
			m_lineupPeoplep[3].m_x = xstart + xOf1 * 3;
			m_lineupPeoplep[3].m_y = ystart - yOf1 * 5;
			m_lineupPeoplep[4].m_x = xstart-xOf1;
			m_lineupPeoplep[4].m_y = ystart-yOf1;
		}
		break;
	case dragon:
		if (1)
		{
			str = "1��λ��������+20%, 2��λ�������+10%, 3��λ�����˺�+30%,�ٶ�-30% 4��λ�ٶ�+15%,5��λ�﷨�˺�+20%,����λ�﷨�˺�+10%,";
			m_Describe[1].SetString(str);
			int xstart = xPos + 145;
			int ystart = yPos + 240;
			m_lineupPeoplep[0].m_x = xstart + xOf1 * 2;
			m_lineupPeoplep[0].m_y = ystart - yOf1 * 2;
			m_lineupPeoplep[1].m_x = xstart + xOf1 * 3;
			m_lineupPeoplep[1].m_y = ystart - yOf1;
			m_lineupPeoplep[2].m_x = xstart + xOf1*3;
			m_lineupPeoplep[2].m_y = ystart - yOf1*5;
			m_lineupPeoplep[3].m_x = xstart;
			m_lineupPeoplep[3].m_y = ystart - yOf1 * 2;
			m_lineupPeoplep[4].m_x = xstart+xOf1;
			m_lineupPeoplep[4].m_y = ystart-yOf1*3;
		}
		break;
	case bird:
		if (1)
		{
			str = "1-5λ�ٶ�+20%����λ�﷨�˺�+10%";
			m_Describe[1].SetString(str);
			int xstart = xPos + 145;
			int ystart = yPos + 240;
			m_lineupPeoplep[0].m_x = xstart + xOf1 * 2;
			m_lineupPeoplep[0].m_y = ystart - yOf1 * 2;

			m_lineupPeoplep[1].m_x = xstart + xOf1 * 4;
			m_lineupPeoplep[1].m_y = ystart - yOf1 * 2;
			m_lineupPeoplep[2].m_x = xstart + xOf1 * 2;
			m_lineupPeoplep[2].m_y = ystart ;

			m_lineupPeoplep[3].m_x = xstart + xOf1 * 4;
			m_lineupPeoplep[3].m_y = ystart - yOf1 * 4;
			m_lineupPeoplep[4].m_x = xstart;
			m_lineupPeoplep[4].m_y = ystart;
		}
		break;
	case snake:
		if (1)
		{
			str = "1,2,3��λ���������+15%, ����λ�﷨�˺�+10%";
			m_Describe[1].SetString(str);
			int xstart = xPos + 145;
			int ystart = yPos + 240;
			m_lineupPeoplep[0].m_x = xstart + xOf1 * 2;
			m_lineupPeoplep[0].m_y = ystart - yOf1 * 2;
			m_lineupPeoplep[2].m_x = xstart + xOf1 * 3;
			m_lineupPeoplep[2].m_y = ystart - yOf1 * 3;
			m_lineupPeoplep[1].m_x = xstart + xOf1 * 1;
			m_lineupPeoplep[1].m_y = ystart - yOf1 * 1;

			m_lineupPeoplep[3].m_x = xstart;
			m_lineupPeoplep[3].m_y = ystart - yOf1 * 2;
			m_lineupPeoplep[4].m_x = xstart+xOf1*3;
			m_lineupPeoplep[4].m_y = ystart-yOf1;
		}
		break;
	case sky:
		if (1)
		{
			str = "1-5λ�﷨�˺�+20%,�ٶ�-10%,����λ�﷨�˺�+10%";
			m_Describe[1].SetString(str);
			int xstart = xPos + 145;
			int ystart = yPos + 240;
			m_lineupPeoplep[0].m_x = xstart + xOf1 * 3;
			m_lineupPeoplep[0].m_y = ystart - yOf1 * 1;
			m_lineupPeoplep[1].m_x = xstart + xOf1 * 3;
			m_lineupPeoplep[1].m_y = ystart - yOf1 * 3;
			m_lineupPeoplep[2].m_x = xstart + xOf1 * 1;
			m_lineupPeoplep[2].m_y = ystart - yOf1 * 1;
			m_lineupPeoplep[3].m_x = xstart + xOf1 * 4;
			m_lineupPeoplep[3].m_y = ystart - yOf1 * 4;
			m_lineupPeoplep[4].m_x = xstart;
			m_lineupPeoplep[4].m_y = ystart;
		}
		break;
	case cloud:
		if (1)
		{
			str = "1��λ�﷨����+40%�ٶ�-30%, 2��λ�﷨����+10%, 4,5��λ�ٶ�+15%,����λ�﷨�˺�+10%";
			m_Describe[1].SetString(str);
			int xstart = xPos + 145;
			int ystart = yPos + 240;
			m_lineupPeoplep[0].m_x = xstart + xOf1 * 4;
			m_lineupPeoplep[0].m_y = ystart;
			m_lineupPeoplep[1].m_x = xstart + xOf1 * 3;
			m_lineupPeoplep[1].m_y = ystart - yOf1 * 3;
			m_lineupPeoplep[2].m_x = xstart + xOf1 * 1;
			m_lineupPeoplep[2].m_y = ystart - yOf1 * 1;
			m_lineupPeoplep[3].m_x = xstart + xOf1 * 4;
			m_lineupPeoplep[3].m_y = ystart - yOf1 * 4;
			m_lineupPeoplep[4].m_x = xstart;
			m_lineupPeoplep[4].m_y = ystart;
		}
		break;
	case thunder:
		if (1)
		{
			str = "1,2,3��λ�̶��˺�Ч��+20%, 4,5��λ�﷨�˺�+10%,����λ�﷨�˺�+10%";
			m_Describe[1].SetString(str);
			int xstart = xPos + 145;
			int ystart = yPos + 240;
			m_lineupPeoplep[0].m_x = xstart + xOf1 * 4;
			m_lineupPeoplep[0].m_y = ystart;
			m_lineupPeoplep[4].m_x = xstart + xOf1 * 3;
			m_lineupPeoplep[4].m_y = ystart - yOf1 * 3;
			m_lineupPeoplep[3].m_x = xstart + xOf1 * 1;
			m_lineupPeoplep[3].m_y = ystart - yOf1 * 1;

			m_lineupPeoplep[1].m_x = xstart + xOf1 * 4;
			m_lineupPeoplep[1].m_y = ystart - yOf1 * 2;
			m_lineupPeoplep[2].m_x = xstart + xOf1 * 2;
			m_lineupPeoplep[2].m_y = ystart;
		}
		break;
	case eagle:
		if (1)
		{
			str = "1��λ�﷨����+10%, 2,3��λ�ٶ�+15%, 4��λ�﷨�˺�+15%, ����λ�﷨�˺�+10%";
			m_Describe[1].SetString(str);
		
			int xstart = xPos + 145;
			int ystart = yPos + 240;
			m_lineupPeoplep[4].m_x = xstart + xOf1 * 4;
			m_lineupPeoplep[4].m_y = ystart;
			m_lineupPeoplep[1].m_x = xstart + xOf1 * 3;
			m_lineupPeoplep[1].m_y = ystart - yOf1 * 3;
			m_lineupPeoplep[2].m_x = xstart + xOf1 * 1;
			m_lineupPeoplep[2].m_y = ystart - yOf1 * 1;


			m_lineupPeoplep[0].m_x = xstart + xOf1 * 3;
			m_lineupPeoplep[0].m_y = ystart-yOf1;
			m_lineupPeoplep[3].m_x = xstart + xOf1 * 2;
			m_lineupPeoplep[3].m_y = ystart-yOf1*2;
		}
		break;
	default:
		ERRBOX;
		break;
	}
	for (int i = 0; i < 5; i++)
	{
		m_lineupPeoplep[i].m_bMove = TRUE;
		m_lineupPeoplepT[i].SetXY(m_lineupPeoplep[i].m_x+3, m_lineupPeoplep[i].m_y+20);
	}
	for (int i = 0; i < 2;i++)
		m_Describe[i].m_bMove = TRUE;
	SortLineupPeople();
}

void cInterfaceTeam::SortLineupPeople()
{
	for (int i = 0; i < 5; i++)
	{
		m_lineupPeoplepp[i] = &m_lineupPeoplep[i];
	}
	for (int i = 0; i < 4; i++)
	{
		if (m_lineupPeoplepp[i]->m_y>m_lineupPeoplepp[i+1]->m_y)
		{
			cWfileP* p = m_lineupPeoplepp[i];
			m_lineupPeoplepp[i] = m_lineupPeoplepp[i + 1];
			m_lineupPeoplepp[i + 1] = p;
		}
	}
}


void cInterfaceTeam::SetFighterPos2(int** px, int** py, int xOf1, int yOf1 , BOOL enemy)
{
	eFormation Formation;
	if (enemy)
		Formation = m_eEnemyFormation;
	else
		Formation = m_eOurFormation;
	int xstart = 340;
	int ystart = 390;
	switch (Formation)
	{
		case normal:
			if (1)
			{
				*px[0] = xstart + xOf1 * 2;
				*py[0] = ystart - yOf1 * 2;
				*px[1] = xstart + xOf1 * 3;
				*py[1] = ystart - yOf1 * 3;
				*px[2] = xstart + xOf1 * 1;
				*py[2] = ystart - yOf1 * 1;
				*px[3] = xstart + xOf1 * 4;
				*py[3] = ystart - yOf1 * 4;
				*px[4] = xstart;
				*py[4] = ystart;
			}
			break;
		case land://����
			if (1)
			{
				*px[0] = xstart + xOf1 * 3;
				*py[0] = ystart - yOf1;
				*px[1] = xstart + xOf1 * 2;
				*py[1] = ystart - yOf1 * 2;
				*px[2] = xstart + xOf1 * 4;
				*py[2] = ystart - yOf1 * 2;
				*px[3] = xstart + xOf1 * 2;
				*py[3] = ystart;
				*px[4] = xstart + xOf1 * 4;
				*py[4] = ystart;
			}
			break;
		case wind:
			if (1)
			{
				*px[0] = xstart + xOf1 * 3;
				*py[0] = ystart - yOf1 ;
				*px[1] = (*px[0]) + xOf1;
				*py[1] = (*py[0]) - yOf1;

				*px[2] = (*px[0]) - xOf1;
				*py[2] = (*py[0]) + yOf1;
				*px[3] = (*px[1]) - xOf1;
				*py[3] = (*py[1]) - yOf1;
				*px[4] = (*px[2]) - xOf1;
				*py[4] = (*py[2]) - yOf1;
			}
			break;
		case tiger:
			if (1)
			{
				*px[0]= xstart + xOf1 * 4;
				*py[0]= ystart ;
				*px[1]= xstart + xOf1 * 4;
				*py[1]= ystart - yOf1 * 2;
				*px[2]= xstart + xOf1 * 2;
				*py[2]= ystart ;
				*px[3]= xstart + xOf1 * 4;
				*py[3]= ystart - yOf1 * 4;
				*px[4]= xstart;
				*py[4]= ystart;
			}
			break;
		case dragon:
			if (1)
			{
				*px[0] = xstart + xOf1 * 3;
				*py[0] = ystart - yOf1 ;
				*px[1] = xstart + xOf1 * 4;
				*py[1] = ystart ;
				*px[2] = xstart + xOf1 * 4;
				*py[2] = ystart - yOf1 * 4;
				*px[3] = xstart+xOf1;
				*py[3] = ystart - yOf1 ;
				*px[4] = xstart + xOf1*2;
				*py[4] = ystart - yOf1 * 2;
			}
			break;
		case bird:
			if (1)
			{
				*px[0]= xstart + xOf1 * 2;
				*py[0]= ystart - yOf1 * 2;
				*px[1]= xstart + xOf1 * 4;
				*py[1]= ystart - yOf1 * 2;
				*px[2]= xstart + xOf1 * 2;
				*py[2]= ystart;
				*px[3]= xstart + xOf1 * 4;
				*py[3]= ystart - yOf1 * 4;
				*px[4]= xstart;
				*py[4]= ystart;
			}
			break;
		case snake:
			if (1)
			{
				*px[0] = xstart + xOf1 *3;
				*py[0] = ystart - yOf1 ;
				*px[2] = xstart + xOf1 * 4;
				*py[2] = ystart - yOf1 * 2;
				*px[1] = xstart + xOf1 * 2;
				*py[1] = ystart ;
				*px[3] = xstart+xOf1;
				*py[3] = ystart - yOf1;
				*px[4] = xstart + xOf1 * 4;
				*py[4] = ystart;
			}
			break;
		case sky:
			if (1)
			{
				*px[0] = xstart + xOf1 * 3;
				*py[0] = ystart - yOf1 * 1;
				*px[1] = xstart + xOf1 * 3;
				*py[1] = ystart - yOf1 * 3;
				*px[2] = xstart + xOf1 * 1;
				*py[2] = ystart - yOf1 * 1;
				*px[3] = xstart + xOf1 * 4;
				*py[3] = ystart - yOf1 * 4;
				*px[4] = xstart;
				*py[4] = ystart;
			}
			break;
		case cloud:
			if (1)
			{
				*px[0]= xstart + xOf1 * 4;
				*py[0]= ystart;
				*px[1]= xstart + xOf1 * 3;
				*py[1]= ystart - yOf1 * 3;
				*px[2] = xstart + xOf1 * 1;
				*py[2] = ystart - yOf1 * 1;
				*px[3] = xstart + xOf1 * 4;
				*py[3] = ystart - yOf1 * 4;
				*px[4] = xstart;
				*py[4] = ystart;
			}
			break;
		case thunder:
			if (1)
			{
				*px[0] = xstart + xOf1 * 4;
				*py[0] = ystart;
				*px[4] = xstart + xOf1 * 3;
				*py[4] = ystart - yOf1 * 3;
				*px[3] = xstart + xOf1 * 1;
				*py[3] = ystart - yOf1 * 1;
				*px[1] = xstart + xOf1 * 4;
				*py[1] = ystart - yOf1 * 2;
				*px[2] = xstart + xOf1 * 2;
				*py[2] = ystart;
			}
			break;
		case eagle:
			if (1)
			{
				*px[4] = xstart + xOf1 * 4;
				*py[4] = ystart;
				*px[1] = xstart + xOf1 * 3;
				*py[1] = ystart - yOf1 * 3;
				*px[2] = xstart + xOf1 * 1;
				*py[2] = ystart - yOf1 * 1;
				*px[0] = xstart + xOf1 * 3;
				*py[0] = ystart - yOf1;
				*px[3] = xstart + xOf1 * 2;
				*py[3] = ystart - yOf1 * 2;
			}
			break;
		default:
			ERRBOX;
			break;
	}

	*px[5] = xstart + xOf1;
	*py[5] = ystart - yOf1 * 3;
	*px[6] = xstart + xOf1 * 2;
	*py[6] = ystart - yOf1 * 4;
	*px[7] = xstart;
	*py[7] = ystart - yOf1 * 2;
	*px[8] = xstart + xOf1 * 3;
	*py[8] = ystart - yOf1 * 5;
	*px[9] = xstart - xOf1;
	*py[9] = ystart - yOf1;
	if (enemy)
	for (int i = 5; i < 10; i++)
	{
		*px[i] -= 10;
	}
	else
	for (int i = 5; i < 10; i++)
	{
		*py[i] -= 10;
	}
	if (enemy)
	{
		for (int i = 0; i < 10; i++)
		{
			*px[i] = 640 - *px[i];
			*py[i] = 480 - *py[i];
		}
	
	}
	UpdatePos();
}

void cInterfaceTeam::SetFighterPos(BOOL enemy /*= TRUE*/)
{
	int* px[10];
	int* py[10];
	for (int i = 0; i < 10; i++)
	{
		px[i] = &m_xPosList[i + enemy * 10];
		py[i] = &m_yPosList[i + enemy * 10];
	}
	SetFighterPos2(px, py,60,30, enemy);
}
void cInterfaceTeam::SetupOneFormationEffect(int Pos, cPetData* pPetData, BOOL UporDown)
{
	if (!pPetData)return;
	//Pos = m_TeamTurn[Pos];//
	int setup;
	if (UporDown)
		setup = 1;
	else  setup = -1;
	eFormation formation;
	if (Pos<10)
	{
		formation = m_eOurFormation;
	}
	else
	{
		formation = m_eEnemyFormation;
	}
	Pos %= 10;
	if (Pos>4)
	{
		pPetData->m_DamageAddPercentMagic.d1 += 10 * setup;
		pPetData->m_DamagePercent.d1 += 10 * setup;
		return;
	}
	switch (formation)
	{
	case normal:
		return;
	case land:
		// 1 3 4 λ �﷨��������15%
		switch (Pos)
		{
		case 0:
		case 2:
		case 3:
			pPetData->m_DamageAddPercentMagic.d1 += 15*setup; 
			pPetData->m_DamagePercent.d2 += 15*setup;
			break;
		case 1://2��λ�﷨�˺�����
			pPetData->m_DamageAddPercentMagic.d1 += 15*setup;
			pPetData->m_DamagePercent.d1 += 15*setup;
			break;
		case 4://5��λ��10%��
			pPetData->m_SPD.AddDataAddPercent(10*setup);
			break;
		}
		return;
	case wind:
		switch (Pos)
		{
		case 0:
			pPetData->m_DamageAddPercentMagic.d1 += 20 * setup;
			pPetData->m_DamagePercent.d1 += 20 * setup;
			break;
		case 1:
		case 2:
			pPetData->m_DamageAddPercentMagic.d1 += 10 * setup;
			pPetData->m_DamagePercent.d1 += 10 * setup;
			break;
		case 3://
		case 4://4,5��λ��10%��
			pPetData->m_SPD.AddDataAddPercent(10 * setup);
			break;
		}
		return;
	case tiger:
		switch (Pos)
		{
		case 0:  //1 ��25 ��
			pPetData->m_DamageAddPercentMagic.d1 += 25 * setup;
			pPetData->m_DamagePercent.d1 += 25 * setup;
			break;
		case 1:   //2 3λ��10%��
		case 2:
			pPetData->m_DamageAddPercentMagic.d1 += 10 * setup;
			pPetData->m_DamagePercent.d2 += 10 * setup;
			break;
		case 3://
		case 4://4,5��λ��10��
			pPetData->m_DamageAddPercentMagic.d1 += 10 * setup;
			pPetData->m_DamagePercent.d1 += 10 * setup;
			break;
		}
		return;
	case dragon:
		switch (Pos)
		{
		case 0:  //1��λ ��10����
			pPetData->m_DamageAddPercentMagic.d1 += 20 * setup;
			break;
		case 1:   //2��λ ��20���
			pPetData->m_DamagePercent.d2 += 10 * setup;
			break;
		case 2: //3��λ  ��30����
			pPetData->m_DamageAddPercentMagic.d1 += 30 * setup;	
			break;
		case 3://4�ż�15 ��
			pPetData->m_SPD.AddDataAddPercent(15 * setup);
			break;
		case 4://5��λ��20��
			pPetData->m_DamageAddPercentMagic.d1 += 20 * setup;
			pPetData->m_DamagePercent.d1 += 20 * setup;;
			break;
		}
		return;
	case bird:
			pPetData->m_SPD.AddDataAddPercent(20*setup);
		return;
	case snake:
		switch (Pos)
		{
		case 0:  //1-3��λ ��15 �������
		case 1:  
		case 2:
			pPetData->m_DamageAddPercentMagic.d1 += 10 * setup;
			pPetData->m_DamagePercent.d2 += 10 * setup;
			break;
		case 3://
		case 4://4,5��λ��10��
			pPetData->m_DamageAddPercentMagic.d1 += 10 * setup;
			pPetData->m_DamagePercent.d1 += 10 * setup;
			break;
		}
		return;
	case sky: // 1-5�� 20�˺�,��10%��
			pPetData->m_DamageAddPercentMagic.d1 += 20 * setup;
			pPetData->m_DamagePercent.d1 += 20 * setup;
			pPetData->m_SPD.AddDataAddPercent(-10 * setup);
		return;
	case cloud:
		switch (Pos)
		{
		case 0:  //1��λ ��40��
			pPetData->m_DamagePercent.d2 += 40 * setup;
			pPetData->m_DamageAddPercentMagic.d1 += 40 * setup;
			pPetData->m_SPD.AddDataAddPercent(-30 * setup);
			break;
		case 1:   //2��λ ��10��
			pPetData->m_DamagePercent.d2 += 10 * setup;
			pPetData->m_DamageAddPercentMagic.d1 += 10 * setup;
			break;
		case 2: //3��λ  ��20��
			pPetData->m_DamageAddPercentMagic.d1 += 20 * setup;
			pPetData->m_DamagePercent.d1 += 20 * setup;
			break;
		case 3://4,5�ż�15 ��
		case 4://
			pPetData->m_SPD.AddDataAddPercent(15 * setup);
			break;
		}
		return;
	case thunder:
		switch (Pos)
		{
		case 0:  //1��λ 
		case 1:   //2��λ
		case 2: //3��λ  ��20%�̶��˺�
			pPetData->m_DamageAddPercentMagic2.d1 += 20 * setup;
			break;
		default:
			pPetData->m_DamageAddPercentMagic.d1 += 10 * setup;
			pPetData->m_DamagePercent.d1 += 10 * setup;
			break;
		}
		return;
	case eagle:
		switch (Pos)
		{
		case 0:  //1��λ ��10���
			pPetData->m_DamagePercent.d2 += 10 * setup;
			break;
		case 1:   //2 3��λ ��15��
		case 2:
			pPetData->m_SPD.AddDataAddPercent(15 * setup);
			break;
		case 3://4�ż�15 ��
			pPetData->m_DamageAddPercentMagic.d1 += 15 * setup;
			pPetData->m_DamagePercent.d1 += 15 * setup;
			break;
		case 4://5�ż�10 ��
			pPetData->m_DamageAddPercentMagic.d1 += 10 * setup;
			pPetData->m_DamagePercent.d1 += 10 * setup;
			break;
		}
		return;
	default:
		ERRBOX;
		return;
	}
}



void cInterfaceTeam::SwapTeamTurn(int id1, int id2)
{
	swap(m_TeamTurn[id1], m_TeamTurn[id2]);
	swap(m_TeamTurn[id1+5], m_TeamTurn[id2+5]);
	UpdatePos();
}

void cInterfaceTeam::OnOff(BOOL NeedShow)
{
	if (NeedShow)g_pMainState->ClearInterfaceSetShow();
	g_pMainState->InterfaceSetShow(this, NeedShow);
	if (NeedShow)
	{
		Init();
		UpDate();
	}
	
	g_pMainState->m_Canvas.SetRedraw(m_Back.GetPicRange());
}

BOOL cInterfaceTeam::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	switch (m_Select)
	{
	case 0:
		OnOff(FALSE);
		break;
	case 1:
		m_bLiineupSelect = FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

void cInterfaceTeam::SetFormation(eFormation format, BOOL enemy /*= FALSE*/,BOOL bUpdate)
{
	if (enemy)
	{
		m_eEnemyFormation = format;
	}
	else
	{
		m_eOurFormation = format;
	}
	SetFighterPos(enemy);
	m_bLiineupSelect = FALSE;
	if (bUpdate)
	m_lineupName.SetString(m_lineupSelectBPT[m_eOurFormation].GetString());
}

DWORD cInterfaceTeam::GetSmallHead(int id)
{
	return g_pMainState->m_FightList[m_TeamTurn[id]].m_pData->GetIndex(POS_SMALLHEAD);
}
DWORD cInterfaceTeam::GetPetHead(int id)
{
	return g_pMainState->m_FightList[m_TeamTurn[id + 5]].m_pData->GetIndex(POS_BIGHEAD);
}

void cInterfaceTeam::UpdatePos()
{
	for (int i = 0; i < 20; i++)
	{
		g_pMainState->m_pFightState->m_xPosList[i] = g_pMainState->m_InterfaceTeam.GetXPos(i);
		g_pMainState->m_pFightState->m_yPosList[i] = g_pMainState->m_InterfaceTeam.GetYPos(i);
	}
}

void cInterfaceTeam::Save(ofstream& File)
{
	File << m_eOurFormation << " ";
	File << m_eEnemyFormation << " ";
	for (int i = 0; i < 20; i++)
	{
		File << m_TeamTurn[i] << " ";
	}
}

void cInterfaceTeam::Load(ifstream& File)
{
	int k;
	File >> k;
	g_pMainState->m_InterfaceTeam.SetFormation((eFormation)k, FALSE, TRUE);
	File >> k;
	g_pMainState->m_InterfaceTeam.SetFormation((eFormation)k, TRUE, TRUE);
	for (int i = 0; i < 20; i++)
	{
		File >> m_TeamTurn[i];
	}
	UpdatePos();
}

int cInterfaceTeam::GetXPos(int id)
{
	return m_xPosList[GetReTramTrun(id)];
}

int cInterfaceTeam::GetYPos(int id)
{
	return m_yPosList[GetReTramTrun(id)];
}

cInterfaceTeam::~cInterfaceTeam()
{
	for (int i = 0; i < 5; i++)
	{
		FreeFighter(i);
		
	}
}

void cInterfaceTeam::FreeFighter(int id)
{
	m_Fighter[id].m_pIndex = 0;
	m_Fighter[id].m_IndexType = 0;
	m_Fighter[id].Free();
}

int cInterfaceTeam::GetReTramTrun(int id)
{
	for (int i = 0; i < 20; i++)
	if (m_TeamTurn[i] == id)
	{
		return i;
	}
	return -1;
}

BOOL cInterfaceTeam::AutoSwap(int k)
{
	int start, end;
	if (k < 10)start = 0;
	else start = 10;
	end = start + 5;
	for (int i = k+1; i < end; i++)
	{
		if (!g_pMainState->m_FightList[GetTeamTurn(i)].isLive)continue;
			SwapTeamTurn(k, i);
		return TRUE;
	}
	return FALSE;
}



BOOL cTags::Init()
{
	if (m_bInit)return TRUE;
	m_bInit = TRUE;
	m_Back.Load(0x4B82870A);//014FF746);//EDCED445);//436592D8);//0xAB7B8559);
	/*m_Back.m_Picture.m_pData = m_Back.m_Picture.Spr.lpDate[0];
	m_Back.m_Picture.m_pAlpha = m_Back.m_Picture.Spr.lpAlpha[0];*/

	m_TagNum = 0;
	for (int i = 0; i < 10; i++)
	{
		m_tags[i].m_Back.Init(&m_Back.m_Picture);
		m_tags[i].m_Back.m_y = 150+i*20;
	}
	m_Text.m_hFont = 0;
	m_Text.SetColor(RGB(255, 255, 255));
	m_Text.SetXCenter(320);
	return TRUE;
}

BOOL cTags::Show(cCanvas* pCanvas)
{
	if (0 == m_TagNum)return TRUE;
	
	for (int i = 0; i < 10; i++)
	{
		if (m_tags[i].m_time>0)
		{
			pCanvas->Add(&m_tags[i].m_Back, 1);
			m_tags[i].m_time -= 1;
			if (m_tags[i].m_time < 1)
			{
				m_TagNum -= 1;
			}
		}
	}
	return TRUE;
}

BOOL cTags::Add(string str, int time)
{
	if (m_TagNum < 10)
	{
		m_TagNum += 1;
		for (int i = 0; i < 10; i++)
		{
			if (m_tags[i].m_time < 1)
			{
				m_Text.SetString( str);
				m_Text.SetY(m_tags[i].m_Back.m_y + 2);
				m_Text.UpdateXCenter();
				m_tags[i].m_Back.m_x = m_Text.m_x - 5;
				m_Back.SetXY(m_tags[i].m_Back.m_x, m_tags[i].m_Back.m_y);
				m_tags[i].m_Back.SetShowWidth(m_Text.m_Width + 10);
				m_tags[i].m_Back.Reset2(0);// &m_Back);
				m_tags[i].m_time = time;
				m_tags[i].m_Back.Draw(&m_Text);
				RECT rect;
				m_tags[i].m_Back.GetRect(rect);
				g_pMainState->m_Canvas.SetRedraw(rect);
				return TRUE;
			}
		}
	}
	return FALSE;
}

void cTags::Process()
{
	if (0 == m_TagNum)return;
	if (m_Sleeptime > 0)
	{
		m_Sleeptime -= 1;
	}
	if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
	{
		m_Sleeptime = 0;
		m_TagNum = 0;
		for (int i = 0; i < 10; i++)
		{
			m_tags[i].m_time = 0;
		}
		
	}
}

cTags::~cTags()
{
	m_Back.m_Picture.m_pData = 0;
	m_Back.m_Picture.m_pAlpha =0;
}

void cTags::GetMoneyAndExp(int money, INT64 exp /*= 0*/, int chubei, INT64 expbb,BOOL needtag)
{
	ostringstream oss;
	if (0!=money)
	{
		if (money > 0)
		{
			oss << "��õ���";
			oss << money;
		}
		else
		{
			oss << "��ʧȥ��";
			oss << -money;
		}
		oss << "Ǯ";
		if (needtag)
		Add(oss.str());
		g_pMainState->m_Channel.AddNewString(oss.str());
		oss.str("");
	}
	
	if (0 != chubei)
	{
		if (chubei > 0)
		{
			oss << "��õ���";
			oss << chubei;
		}
		else
		{
			oss << "��ʧȥ��";
			oss << chubei;
		}
		oss << "������";
		if (needtag)
		Add(oss.str());
		g_pMainState->m_Channel.AddNewString(oss.str());
		oss.str("");
	}
	if (0 != exp)
	{
		if (exp > 0)
		{
			oss << "��õ���";
			oss << exp;
		}
		else
		{
			oss << "��ʧȥ��";
			oss << exp;
		}
		oss << "����";
		if (needtag)
			Add(oss.str());
		g_pMainState->m_Channel.AddNewString(oss.str());
		oss.str("");
	}
	if (0 != expbb)
	{
		if (expbb > 0)
		{
			oss << "����ٻ��޵õ���";
			oss << expbb;
		}
		else
		{
			oss << "����ٻ���ʧȥ��";
			oss << expbb;
		}
		oss << "����";
		if (needtag)
			Add(oss.str());
		g_pMainState->m_Channel.AddNewString(oss.str());
		oss.str("");
	}
}

void cTags::GetItem(string itemname)
{
	ostringstream oss;
	oss << "��õ���";
	oss << itemname;
	Add(oss.str());
	g_pMainState->m_Channel.AddNewString(oss.str());
}






BOOL cAccelator::ProcessInput()
{
	if (m_Describe.m_NeedShow)//g_pMainState->InterfaceSetShow(&m_Describe, FALSE);
		m_Describe.OnOff(FALSE);

	if (m_Mode == 0)
	{
		if (isOn(g_xMouse, g_yMouse, m_redrawrect))
		{
			m_Select = (g_xMouse - m_redrawrect.left) / m_Accelatorback.GetWidth();
			if (CheckL())return TRUE;
			if (m_AccelatorSkillid[m_Select] > -1)
			{
				g_pMainState->m_SkillManager.SetMengPaiSkill(m_Describe, m_AccelatorSkillid[m_Select]);
				if (0 == m_Describe.m_NeedShow)//g_pMainState->InterfaceSetShow(&m_Describe, TRUE);
				m_Describe.OnOff(TRUE);
			}
			if (CheckRB())return TRUE;
		}
	}
	else
	{
		ResetButton();
		g_pMainState->SetMouseOnMap(FALSE);
		m_SkillSelect = -1;

		if (!isOn(g_xMouse, g_yMouse, m_MagicBack.m_PreShowRect))
		{
			return TRUE;
		}
		if (CheckOn(&m_Cancel, 100))return TRUE;


		int i = 0;
		for (; i < m_NumofSkill; i++)
		{
			if (isOn(g_xMouse, g_yMouse, m_SkillHead[i].m_PreShowRect))
			{
				m_Select = 14;
				m_SkillSelect = m_pSkillShow[i]->m_Id;
				g_pMainState->m_SkillManager.SetMengPaiSkill(m_Describe, m_SkillSelect);
				if (0 == m_Describe.m_NeedShow)g_pMainState->InterfaceSetShow(&m_Describe, TRUE);
				if (CheckL())return TRUE;
				break;
			}
		}
		m_Select = 15;
		if (CheckRB())return TRUE;
	}
	return TRUE;
}

BOOL cAccelator::Show(cCanvas* pCanvas)
{
	if (!m_NeedShow)return TRUE;
	for (int i = 0; i < 8; i++)
	{
		pCanvas->Add(&m_Accelatorbackp[i], 1);
		pCanvas->Add(&m_AccelatorSkill[i], 1);
		pCanvas->Add(&m_tAccelatorSkill[i]);
	}
	if (m_Mode)
	{
		pCanvas->Add(&m_MagicBack, 1);
		pCanvas->Add(&m_TitleT);
		pCanvas->Add(&m_Cancel, 1);
		for (int i = 0; i < m_NumofSkill; i++)
			pCanvas->Add(&m_SkillHead[i], 1);
	}
	return TRUE;
}

void cAccelator::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;
	m_Describe.Init();
	//��ݼ�����
	m_Accelatorback.Load(76806695);
	ostringstream oss;
	for (int i = 0; i < 8; i++)
	{
		m_Accelatorbackp[i].Set( &m_Accelatorback);
		m_Accelatorbackp[i].SetXY(340 + i*m_Accelatorback.GetWidth(), 420);
		m_AccelatorSkill[i].SetXY(344 + i*m_Accelatorback.GetWidth(),424);
		oss.str("");
		oss << "F";
		oss << i+1;
		m_tAccelatorSkill[i].SetColor(RGB(255, 255, 255));
		m_tAccelatorSkill[i].SetStyle(2);
		m_tAccelatorSkill[i].SetString(oss.str());
		m_tAccelatorSkill[i].SetXY(350 + i*m_Accelatorback.GetWidth(), 405);
		
	}
	



	m_redrawrect.left = m_Accelatorbackp[0].m_x;
	m_redrawrect.top = m_Accelatorbackp[0].m_y;
	m_redrawrect.right = m_Accelatorbackp[7].m_x + m_Accelatorback.GetWidth();
	m_redrawrect.bottom = m_Accelatorbackp[0].m_y + m_Accelatorback.GetHeight();
	m_TitleT.SetString("����");
	m_TitleT.SetColor(RGB(255,255,255));
	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;
}

void cAccelator::OnOff(BOOL NeedShow)
{
	if (m_NeedShow == NeedShow)return;
	g_pMainState->InterfaceSetShow(this, NeedShow);
	if (m_NeedShow)
	{
		Init();
		for (int i = 0; i < 8; i++)
		{
			m_AccelatorSkillid[i] = g_pCharacter->m_PCData.m_Accelarate[i];
			m_AccelatorSkill[i].Load(g_pMainState->m_SkillManager.GetMengPaiSkill(m_AccelatorSkillid[i])->m_SmallHead);
		}

	}
	m_Describe.OnOff(FALSE);
	g_pMainState->m_Canvas.SetRedraw(m_redrawrect);
}

void cAccelator::MoveMagicBack(int x, int y)
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
}

BOOL cAccelator::CheckL()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))return FALSE;
		g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);

	m_Sleeptime = 3;
	switch (m_Select)
	{
	case 14:
		if (1)
		{
			g_pCharacter->m_PCData.m_Accelarate[m_AccelatorSelect] = m_SkillSelect;

			m_AccelatorSkillid[m_AccelatorSelect] = g_pCharacter->m_PCData.m_Accelarate[m_AccelatorSelect];
			m_AccelatorSkill[m_AccelatorSelect].Load(g_pMainState->m_SkillManager.GetMengPaiSkill(m_AccelatorSkillid[m_AccelatorSelect])->m_SmallHead);

		}
	case 100:
		m_Mode = 0;
		g_pMainState->m_Canvas.SetRedraw(m_MagicBack.m_PreShowRect);
		break;
	default:
		if (1)
		{
			if (g_StateType == STATE_MAIN)
			{
				//���ÿ�ݼ�
				m_Cancel.SetFrame(0);
				m_Mode = 1;
				m_AccelatorSelect = m_Select;
				m_NumofSkill = 0;
				for (int i = 0; i < g_pCharacter->m_PCData.m_SkillEnablelist.size(); i++)
				{
					if (m_NumofSkill == 30)
						break;
					if (g_pCharacter->m_PCData.m_SkillEnablelist[i]->m_SkillType != 7)//�Ƿ�ս������
					{
						m_pSkillShow[m_NumofSkill] = g_pCharacter->m_PCData.m_SkillEnablelist[i];
						m_NumofSkill += 1;
					}

				}
				g_pMainState->m_pFightState->m_FightMenu.Init();
				if (m_NumofSkill < 11)m_MagicBack.Set(&g_pMainState->m_pFightState->m_FightMenu.m_BackMagic10);
				else
				if (m_NumofSkill < 15)m_MagicBack.Set(&g_pMainState->m_pFightState->m_FightMenu.m_BackMagic14);
				else
				if (m_NumofSkill < 21)m_MagicBack.Set(&g_pMainState->m_pFightState->m_FightMenu.m_BackMagic20);
				else
				{
					m_MagicBack.Set(&g_pMainState->m_pFightState->m_FightMenu.m_BackMagic30);
					if (30 < m_NumofSkill)
						m_NumofSkill = 30;
				}
				//���ؼ���ͷ��,�����ú�λ��
				for (int i = 0; i < m_NumofSkill; i++)
				{
					m_SkillHead[i].Load(m_pSkillShow[i]->m_BigHead);
				}
				MoveMagicBack(320 - m_MagicBack.m_pFile->GetWidth() / 2, 240 - m_MagicBack.m_pFile->GetHeight() / 2);

			}
			else
			{
				sSkill* pskill = g_pMainState->m_SkillManager.m_MengPaiManager.GetSMSkill(m_AccelatorSkillid[m_Select]);

				if (pskill->m_EffectType != 0)
				{
					sFightList& list = g_pMainState->m_FightList[g_pMainState->m_pFightState->m_FightMenu.m_NowFighterID];
					list.m_SkillPreSelect.SkillID = pskill->m_Id;

					cCharacter* pc = (cCharacter*)list.m_pData->m_pIndex;

					list.m_SkillPreSelect.Lv = pc->m_PCData.m_MengPaiSkillLV[pskill->m_Owner];

					g_pMainState->m_pFightState->m_FightMenu.InitMagic2();
				}
				return TRUE;
			}
		}
		break;
	}
	return TRUE;
}

BOOL cAccelator::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	switch (m_Select)
	{
	case 15:
		if (1)
		{
			m_Mode = 0;
			g_pMainState->m_Canvas.SetRedraw(m_MagicBack.m_PreShowRect);
		}
	default:
		if (1)
		{
			if (m_AccelatorSkillid[m_Select] < 0)return TRUE;
			sSkill* pskill = g_pMainState->m_SkillManager.m_MengPaiManager.GetSMSkill(m_AccelatorSkillid[m_Select]);
			switch (g_StateType)
			{
			case STATE_MAIN:
				if (pskill->m_EffectType == 0)
					g_pMainState->m_SkillManager.GetSkill(m_AccelatorSkillid[m_Select]);
				return TRUE;
			case STATE_FIGHT:
				if (pskill->m_EffectType != 0)
				{
					sFightList& list = g_pMainState->m_FightList[g_pMainState->m_pFightState->m_FightMenu.m_NowFighterID];
					list.m_SkillPreSelect.SkillID = pskill->m_Id;

					cCharacter* pc = (cCharacter*)list.m_pData->m_pIndex;

					list.m_SkillPreSelect.Lv = pc->m_PCData.m_MengPaiSkillLV[pskill->m_Owner];

					g_pMainState->m_pFightState->m_FightMenu.InitMagic2();
				}
				return TRUE;
			}
		}
		/*if (m_AccelatorSkillid[m_Select]>-1)
		return UseSkill(m_AccelatorSkillid[m_Select]);*/
		break;
	}
	
	return TRUE;
}

BOOL cAccelator::ProcessAccelator()
{
	if (!g_pCharacter)return TRUE;
	int* acc = g_pCharacter->m_PCData.m_Accelarate;
	for (int i = 0; i<8; i++)
	{
		if (acc[i] > -1)
		{
			if (g_pMainState->Keyboard.GetKeyState(59+i))
			{
				g_pMainState->Keyboard.SetLock(59 + i);
				return UseSkill(acc[i]);
			}
			
		}
	}
	
	
	return FALSE;
}

BOOL cAccelator::UseSkill(int id)
{
	sSkill* pskill = g_pMainState->m_SkillManager.GetMengPaiSkill(id);
	if (g_StateType == STATE_MAIN)
	{
		if (pskill->m_SkillType == 0)
		{
			g_pMainState->m_SkillManager.UseNormal(id);
			return TRUE;
		}
	}
	else
	{
		if (pskill->m_SkillType != 0)
		{
			if (g_pMainState->m_pFightState->m_FightMenu.m_NowFighterID == g_pMainState->m_HeroID)
			{
				g_pMainState->m_pFightState->m_FightMenu.m_pSkillSelect = pskill;
				g_pMainState->m_pFightState->m_FightMenu.InitMagic2();
			}
			return TRUE;
		}
	}
	return FALSE;
}

void cWorldMap::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;
	m_Mode = 0;
	m_Data[eMapWorld].Load(0x93E77F54);
	m_Data[eMapSouth].Load(0x0B19A5DC);
	m_Data[eMapEast].Load(0x26E89B1F);
	m_Data[eMapNorth].Load(0xE9EC7A25);
	m_Data[eMapWest].Load(0x11A18FBF);
	m_Data[eMapFenChao].Load(0x13659C02);
	m_Data[eMapLongKu].Load(0xA0B51368);
	m_Data[eMapNvWa].Load(0xEDEEF9AF);
	m_Data[eMapAL].Load(0xCCB7A7C3);
	m_Data[eMapHGS].Load(0x6B27F57F);
	m_Data[eMapNR].Load(0x6BF13E64);
	m_Data[eMapPengLai].Load(0xA9BC7D93);
	m_Data[eMapCA].Load(0x71DF41E1);
	m_Data[eMapDT].Load(0xE0C66A6F);
	m_Data[eMapDF].Load(0x9C992236);
	m_Data[eMapGLZ].Load(0xFF1D4A14);
	m_Data[eMapHZ].Load(0x6EA52AE1);
	m_Data[eMapHS].Load(0x47FE6497);
	m_Data[eMapJY].Load(0x6AB41A9A);
	m_Data[eMapJZ].Load(0x991B588D);
	m_Data[eMapLG].Load(0xAD1DC28A);
	m_Data[eMapMW].Load(0x150B5122);
	m_Data[eMapPS].Load(0x301A4FE7);
	m_Data[eMapPT].Load(0xEC9EBCA8);
	//m_Data[eMapSML].Load(0xCAF25E94);
	m_Data[eMapST].Load(0xBF78C1EF);
	m_Data[eMapD5].Load(0x5B96A2DD);
	m_Data[eMapWZ].Load(0xCA26DE5D);
	m_Data[eMapXXT].Load(0xEA60ABA4);
	m_Data[eMapBX].Load(0x0D906C8D);
	m_Data[eMapBQ].Load(0x63566DA9);
	m_Data[eMapCS].Load(0x637B3C48);
	m_Data[eMapFC].Load(0x6A154DDE);
	m_Data[eMapZZ].Load(0x689EB676);
	m_Data[eMapXL].Load(0xE329AD6A);
	m_Data[eMapQLS].Load(0x048DA00D);
	m_Data[eMapWorldNorth].Load(0xEA1F50E2);
	m_Data[eMapWorldLG].Load(0xCE11152D);
	m_Data[eMapWorldEast].Load(0xCC1742A6);
	m_Data[eMapWorldSouth].Load(0x7FFAB47A);
	m_Data[eMapWorldWest].Load(0x0AF24A29);
	m_Data[eMapWorldTG].Load(0x5ABFF907);
	m_Data[eMapWorldYG].Load(0xF63C410E);
	m_Data[eMapWorldPTY].Load(0xF8A67A7A);
	m_Data[eMapMJC].Load(0xF617F5A9);
			
	//CWFile	m_LBC;		  m_LBC.Load(0xBDAEEA53);
	//CWFile	m_WDD;		  m_WDD.Load(0xBFF5620B);
	InitPos();
}

void cWorldMap::InitPos()
{
	 m_text.SetXY(100, 20);
	 m_Data[eMapWorld].SetXY(0, 0);
	 m_Data[eMapSouth].SetXY(0, 0);
	 m_Data[eMapNorth].SetXY(0, 0);
	 m_Data[eMapEast].SetXY(0, 0);
	 m_Data[eMapWest].SetXY(0, 0);
	 m_Data[eMapFenChao].SetXY(327, 145);
	 m_Data[eMapLongKu].SetXY(52, 167);
	 m_Data[eMapNvWa].SetXY(161, 20);

	 m_Data[eMapAL].SetXY(343, 273);
	 m_Data[eMapHGS].SetXY(263, 11);
	 m_Data[eMapNR].SetXY(212, 162);
	 m_Data[eMapPengLai].SetXY(149, 313);


	 m_Data[eMapCA].SetXY(368, 141);
	 m_Data[eMapDT].SetXY(450, 102);
	 m_Data[eMapDF].SetXY(331, 34);
	 m_Data[eMapGLZ].SetXY(105, 230);
	 m_Data[eMapHZ].SetXY(295, 130);
	 m_Data[eMapHS].SetXY(477, 158);
	 m_Data[eMapJY].SetXY(468, 232);
	 m_Data[eMapJZ].SetXY(350, 113);
	 m_Data[eMapLG].SetXY(469, 374);
	 m_Data[eMapMW].SetXY(191, 94);
	 m_Data[eMapPS].SetXY(213, 194);
	 m_Data[eMapPT].SetXY(333, 410);
	 m_Data[eMapSML].SetXY(0, 0);
	 m_Data[eMapST].SetXY(4, 185);
	 m_Data[eMapD5].SetXY(273, 18);
	 m_Data[eMapWZ].SetXY(232, 271);
	 m_Data[eMapXXT].SetXY(50, 69);
	 m_Data[eMapBX].SetXY(488, 274);
	 m_Data[eMapBQ].SetXY(329, 101);
	 m_Data[eMapCS].SetXY(414,117);
	 m_Data[eMapFC].SetXY(543, 8);
	 m_Data[eMapZZ].SetXY(469, 330);
	 m_Data[eMapXL].SetXY(457, 200);
	 m_Data[eMapQLS].SetXY(387, 317);
	 m_Data[eMapMJC].SetXY(155, 17);
	 m_Data[eMapWorldNorth].SetXY(271, 10);
	 m_Data[eMapWorldLG].SetXY(388, 376);
	 m_Data[eMapWorldEast].SetXY(499, 160);
	 m_Data[eMapWorldSouth].SetXY(85, 104);
	 m_Data[eMapWorldWest].SetXY(0, 58);
	 m_Data[eMapWorldTG].SetXY(166, 23);
	 m_Data[eMapWorldYG].SetXY(75, 4);
	 m_Data[eMapWorldPTY].SetXY(199, 0);
}

BOOL cWorldMap::ProcessInput()
{
	g_pMainState->SetMouseOnMap(FALSE);
	ResetButton();

	switch (m_Mode)
	{
	case 0:
		ProcessInput0();
		break;
	case 1:
		ProcessInput1();
		break;
	case 2:
		ProcessInput2();
		break;
	case 3:
		ProcessInput3();
		break;
	case 4:
		ProcessInput4();
		break;
	}
	m_Select = 0;
	CheckRB();
	return TRUE;
}

BOOL cWorldMap::Show(cCanvas* pCanvas)
{
	switch (m_Mode)
	{
	case 0:
		Show0(pCanvas);
		break;
	case 1:
		Show1(pCanvas);
		break;
	case 2:
		Show2(pCanvas);
		break;
	case 3:
		Show3(pCanvas);
		break;
	case 4:
		Show4(pCanvas);
		break;
	}
	pCanvas->Add(&m_text);
	return TRUE;
}
//�����ͼ
BOOL cWorldMap::ProcessInput0()
{
	CWFile* pNowSelect=0;
	int i = 1;

	for (; i < 9; i++)
	{
		if (m_Data[i].isSelectOn(g_xMouse, g_yMouse))
		{
			pNowSelect = &m_Data[i];
			break;
		}
	}

	if (pNowSelect)
	{	
		m_pShow = pNowSelect;
		m_Select = i;
		m_pShow->SetStatic(FALSE);
		m_pShow->m_bMove = TRUE;
		return CheckL();
	}
	if ((9 == i)&&m_pShow)
	{
		m_pShow->SetFrame(0);
		m_pShow->SetStatic(TRUE);
		m_pShow->m_bMove = TRUE;
		m_pShow = 0;
	}

	return TRUE;
}
//��ʤ����
BOOL cWorldMap::ProcessInput1()
{
	if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
	{
		for (int i = 16; i < 20; i++)
		{
			if (CheckOn(&m_Data[i], i))return TRUE;
		}
	}
	return TRUE;
}
BOOL cWorldMap::ProcessInput2()
{
	if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
	{
		for (int i = 38; i < 45; i++)
		{
			if (CheckOn(&m_Data[i], i))return TRUE;
		}
	}
	return TRUE;
}
BOOL cWorldMap::ProcessInput3()
{
	if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
	{
		for (int i = 20; i < 38; i++)
		{
			if (CheckOn(&m_Data[i], i))return TRUE;
		}
	}
	return TRUE;
}
BOOL cWorldMap::ProcessInput4()
{
	if (g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))
	{
		for (int i = 13; i < 16; i++)
		{
			if (CheckOn(&m_Data[i], i))return TRUE;
		}
	}
	return TRUE;
}

BOOL cWorldMap::Show0(cCanvas* pCanvas)
{
	pCanvas->Add(&m_Data[eMapWorld], 1, 1);
	if (m_pShow)
		pCanvas->Add(m_pShow, 1, 0);
	return TRUE;
}
//��
BOOL cWorldMap::Show1(cCanvas* pCanvas)
{
	pCanvas->Add(&m_Data[eMapEast], 1, 1);
	for (int i = 16; i < 20; i++)
		pCanvas->Add(&m_Data[i], 1, 0);
	return TRUE;
}
//��
BOOL cWorldMap::Show2(cCanvas* pCanvas)
{
	pCanvas->Add(&m_Data[eMapWest], 1, 1);
	for (int i = 38; i < 45; i++)
		pCanvas->Add(&m_Data[i], 1, 0);
	return TRUE;
}
//��
BOOL cWorldMap::Show3(cCanvas* pCanvas)
{
	pCanvas->Add(&m_Data[eMapSouth], 1, 1);
	for (int i = 20; i < 38;i++)
		pCanvas->Add(&m_Data[i], 1, 0);
	return TRUE;
}
//��
BOOL cWorldMap::Show4(cCanvas* pCanvas)
{
	pCanvas->Add(&m_Data[eMapNorth], 1, 1);
	for (int i = 13; i < 16; i++)
		pCanvas->Add(&m_Data[i], 1, 0);
	return TRUE;
}

void cWorldMap::OnOff(BOOL bShow)
{
#ifndef TESTDEBU
	return;
#endif
	
	if (m_NeedShow == bShow)return;
	g_pMainState->InterfaceSetShow(this, bShow);
	
	g_pMainState->m_Canvas.SetRedrawAll();
	if (m_NeedShow)
	{
		if (!m_bInit)Init();
	}
}

BOOL cWorldMap::CheckL()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_LBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_LBUTTON);
	m_Sleeptime = 3;
	cMyMap& map= g_pMainState->m_Map;

	eMap mapid = (eMap)m_Select;
	switch (mapid)
	{
	case cWorldMap::eMapWorld:
	case cWorldMap::eMapWorldEast:
	case cWorldMap::eMapWorldWest:
	case cWorldMap::eMapWorldSouth:
	case cWorldMap::eMapWorldNorth:
		g_pMainState->m_Canvas.SetRedrawAll();
		m_Mode = m_Select;
		break;
	case cWorldMap::eMapSouth:
	case cWorldMap::eMapNorth:
	case cWorldMap::eMapEast:
	case cWorldMap::eMapWest:
		g_pMainState->m_Canvas.SetRedrawAll();
		m_Mode = m_Select;
		break;
	default:
		if (1)
		{
			OnOff(FALSE);
			BOOL bfly = FALSE;
			switch (mapid)
			{
			case cWorldMap::eMapFenChao:map.LoadMap(64, 59, "�ﳲһ��", bfly); break;
			case cWorldMap::eMapLongKu:map.LoadMap(148, 15, "����һ��", bfly); break;
			case cWorldMap::eMapNvWa:map.LoadMap(51, 38, "Ů���",bfly); break;
			case cWorldMap::eMapAL:map.LoadMap(127, 92, "������",bfly); break;
			case cWorldMap::eMapHGS:map.LoadMap(35, 98, "����ɽ",bfly); break;
			case cWorldMap::eMapNR:map.LoadMap(69, 63, "Ů����",bfly); break;
			case cWorldMap::eMapPengLai:map.LoadMap(83, 34, "�����ɵ�",bfly); break;
			case cWorldMap::eMapCA:map.LoadMap(200, 160, "������", bfly); break;
			case cWorldMap::eMapDT:map.LoadMap(99, 35, "���ƹٸ�", bfly); break;
			case cWorldMap::eMapDF:map.LoadMap(44, 54, "���ܵظ�", bfly); break;
			case cWorldMap::eMapGLZ:map.LoadMap(394, 56, "���ƾ���", bfly); break;
			case cWorldMap::eMapHZ:map.LoadMap(41, 36, "���ƹ���", bfly); break;
			case cWorldMap::eMapHS:map.LoadMap(35, 20, "������", bfly); break;
			case cWorldMap::eMapJY:map.LoadMap(266, 33, "������", bfly); break;
			case cWorldMap::eMapJZ:map.LoadMap(204, 234, "���ƹ���");break;
			case cWorldMap::eMapLG:map.LoadMap(91, 46, "����", bfly); break;
			case cWorldMap::eMapWorldLG:map.LoadMap(91, 46, "����", bfly); break;
			case cWorldMap::eMapMW:map.LoadMap(39, 24, "ħ��կ", bfly); break;
			case cWorldMap::eMapPS:map.LoadMap(179, 120, "��˿��", bfly); break;
			case cWorldMap::eMapPT:map.LoadMap(69, 18, "����ɽ", bfly); break;
			case cWorldMap::eMapSML://��ľ��break;
			case cWorldMap::eMapST:map.LoadMap(114, 83, "ʨ����", bfly); break;
			case cWorldMap::eMapD5:map.LoadMap(41, 31, "�������", bfly); break;
			case cWorldMap::eMapWZ:map.LoadMap(43, 38, "��ׯ��", bfly); break;
			case cWorldMap::eMapXXT:map.LoadMap(37, 208, "С����", bfly); break;
			case cWorldMap::eMapBX:map.LoadMap(115, 72, "�����", bfly); break;
			case cWorldMap::eMapBQ:map.LoadMap(58, 79, "�����", bfly); break;
			case cWorldMap::eMapCS:map.LoadMap(107, 58, "���ٴ�", bfly); break;
			case cWorldMap::eMapZZ:map.LoadMap(12, 7, "���Ϲ�", bfly); break;
			case cWorldMap::eMapXL:map.LoadMap(84, 62, "����Ů��", bfly); break;
			case cWorldMap::eMapMJC:map.LoadMap(42, 155, "ī�Ҵ�", bfly); break;
			case cWorldMap::eMapQLS:map.LoadMap(31, 117, "����ɽ", bfly); break;
			case cWorldMap::eMapFC:map.LoadMap(126, 42, "����ɽ", bfly); break;
			case cWorldMap::eMapWorldTG:map.LoadMap(202, 33, "�칬", bfly); break;
			case cWorldMap::eMapWorldYG:map.LoadMap(98, 60, "�¹�", bfly); break;
			case cWorldMap::eMapWorldPTY:map.LoadMap(132, 35, "���԰", bfly); break;
			default:ERRBOX;break;
			}
		}
	}
	return TRUE;
}

BOOL cWorldMap::MoveTest(CWFile& Target)
{
	if (isOn(g_xMouse, g_yMouse, Target.m_PreShowRect))
	{
		if (-1 != m_Prex)
		{
			ostringstream oss;
			int x = Target.GetX();
			int y = Target.GetY();
			oss << "x:" << x << "  ";
			oss << "y:" << y;
			m_text.SetString(oss.str());
			Target.SetXY(x + g_xMouse - m_Prex, y + g_yMouse - m_Prey);
		}
		m_Prex = g_xMouse;
		m_Prey = g_yMouse;
		return TRUE;
	}
	return FALSE;
}

BOOL cWorldMap::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	switch (m_Mode)
	{
	case 0:
		OnOff(FALSE);
		break;
	default:
		g_pMainState->m_Canvas.SetRedrawAll();
		m_Mode = 0;
		break;
	}
	return TRUE;
}



void cInterfaceRanSe::Init()
{
	m_NextPage.Load(0xCB50AB1D);
	m_NextPage.SetStatic(TRUE);
	m_Back.Load(0x1A2DEAB9);
	m_Back.SetStatic(TRUE);
	m_btBuy.Load(0x79D01E0E);
	m_btBuy.SetStatic(TRUE);
	m_tBuy.SetColor(RGB(255,255,255));
	m_tBuy.SetString("ȷ��");
	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;
	m_Title.SetColor(RGB(255,255,255));
	m_Title.SetString("Ⱦɫ");
	for (int i = 0; i < 4; i++)
	{
		m_btSelect[i].Load(0xF9494E3A);
		m_btSelect[i].SetStatic(TRUE);
		m_tSelect[i].SetColor(RGB(255, 255, 255));
		m_tSelect[i].SetString("��");
	}
	Move(320 - m_Back.GetWidth() / 2, 240 - m_Back.GetHeight()/2);
	m_bInit = TRUE;
}

void cInterfaceRanSe::Move(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
	m_Back.SetXY(m_xPos, m_yPos);
	m_Title.SetXCenter(m_xPos+77);
	m_Title.SetY(m_yPos+3);
	m_Title.UpdateXCenter();

	m_btBuy.SetXY(m_xPos+m_Back.GetWidth()/2-m_btBuy.GetWidth()/2,m_yPos+238);

	m_tBuy.SetXCenter(m_btBuy.GetX() + m_btBuy.GetWidth() / 2);
	m_tBuy.SetY(m_btBuy.GetY()+2);
	m_tBuy.UpdateXCenter();

	m_Cancel.SetXY(m_xPos+m_Back.GetWidth()-20,m_yPos+3);
	int cube = (m_Back.GetWidth() - 20) / 4;
	int xstart = m_xPos + 10 + m_btSelect->GetWidth() / 2;
	for (int i = 0; i < 4; i++)
	{
		m_btSelect[i].SetXY(xstart + cube*i, m_yPos + 195);

		m_tSelect[i].SetY(m_btSelect[i].GetY()+3);
		m_tSelect[i].SetXCenter(m_btSelect[i].GetX() + m_btSelect[i].GetWidth()/2);
		m_tSelect[i].UpdateXCenter();
	}
	m_Target.SetXY(m_xPos+m_Back.GetWidth() / 2, m_yPos+m_Back.GetHeight()-95);
	m_NextPage.SetXY(m_xPos+129, m_yPos+35);
}

BOOL cInterfaceRanSe::ProcessInput()
{
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
	if (CheckOn(&m_Cancel, 5))return TRUE;
	if (CheckOn(&m_btBuy, 4))return TRUE;
	if (CheckOn(&m_NextPage, 8))return TRUE;
	for (int i = 0; i < 4; i++)
	{
		if (CheckOn(&m_btSelect[i], i))return TRUE;
	}
	m_Select = 0;
	if(CheckRB())return TRUE;

	//׼���ƶ�
	m_Select = 7;
	if (CheckL())return TRUE;
	return TRUE;
}

void cInterfaceRanSe::OnOff(BOOL NeedShow)
{
	g_pMainState->InterfaceSetShow(this, NeedShow);
	g_pMainState->m_Canvas.SetRedraw(m_Back.m_PreShowRect);
	if (m_NeedShow)
	{
		if (!m_bInit)Init();
	}
	else m_plist = 0;
}

BOOL cInterfaceRanSe::CheckL()
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
	case 1:
	case 2:
	case 3:
		m_frampre->SetFrame(1);
		m_ChangeColorData.AddColorChange(m_Select);
		UpdateColor();
		break;
	case 5://��
		OnOff(FALSE);
		break;
	case 4://ȷ��
		if (1)
		{		
			cObj* pobj=0;
			cCharacter* pc;
			if (!m_plist)
			{
				 pobj = g_pMainState->m_FightList[g_pMainState->m_HeroID].m_pData;
				 pc = g_pCharacter;
			}
			else
			{
				pobj = g_pMainState->m_FightList[m_plist->GetTeamID()].m_pData;
				pc = (cCharacter*)pobj->m_pIndex;
			}
			if (m_plist)
			{//���º�������
				switch (m_Mode)
				{
				case 0:
					m_plist->m_Ranse.m_ranse[0] = m_ChangeColorData;
					break;
				case 1:
					m_plist->m_ZuoQiRanse = m_ChangeColorData;
					break;
				case 2:
					m_plist->m_ZhuangShiRanse = m_ChangeColorData;
					break;
				case 3:
					if (-1 == m_plist->m_FightBB)return FALSE;
					m_plist->m_BBList[m_plist->m_FightBB].m_ranse.m_ranse[m_NowPart] = m_ChangeColorData;
					pobj = g_pMainState->m_FightList[m_plist->GetTeamID()+5].m_pData;
					break;
				default:
					ERRBOX;
					break;
				}
			}
			//���º��ѵ�ǰ
			switch (m_Mode)
			{
			case 0:
				pc->m_PCData.m_RanSe.m_ranse[0] = m_ChangeColorData;
				break;
			case 1:
				pc->m_PCData.m_ZuoQiRanSe = m_ChangeColorData;
				break;
			case 2:
				pc->m_PCData.m_ZhuangShiRanSe = m_ChangeColorData;
				break;
			case 3:
				pc->m_PCData.m_pPetDataList[pc->m_PCData.m_FightPetID]->m_PetDataZZ.m_RanSe.m_ranse[m_NowPart] = m_ChangeColorData;	
				if (!m_plist)
				{
					pobj = g_pHeroObj;
				}
				break;
			default:
				ERRBOX;
				break;
			}
			if (pobj)
			{

				pobj->Reset();
				pobj->Set(POS_STAND);
			}
			OnOff(FALSE);
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
			if (m_Mode != 3)return TRUE;
			if (m_plist)
			{
				cCharacter* pc;
				pc=(cCharacter*)g_pMainState->m_FightList[m_plist->GetTeamID()].m_pData->m_pIndex;
				SetPet(m_NowPart + 1, pc);
			}
			else
				SetPet(m_NowPart + 1,g_pCharacter);
			return TRUE;
	}
	return TRUE;
}

BOOL cInterfaceRanSe::Show(cCanvas* pCanvas)
{
	if (!m_NeedShow)return TRUE;
	pCanvas->Add(&m_Back, 1, 1);
	pCanvas->Add(&m_Cancel, 1);
	pCanvas->Add(&m_btBuy, 1);
	for (int i = 0; i < m_ChangeColorData.partnum; i++)
	{
		pCanvas->Add(&m_btSelect[i], 1);
		pCanvas->Add(&m_tSelect[i], 1);
	}
	pCanvas->Add(&m_tBuy, 1);
	pCanvas->Add(&m_Title, 1);
	pCanvas->Add(&m_Target, 1);
	pCanvas->Add(&m_NextPage, 1);
	return TRUE;
}

void cInterfaceRanSe::SetMode(int mode, int Characterid)//sFriendList* plist)
{
	m_Mode = mode;
	m_Target.Free();
	cCharacter* pc;
	cObj* pObj= g_pMainState->m_FightList[Characterid].m_pData;
	pc = (cCharacter*)pObj->m_pIndex;
	if (Characterid!=g_pMainState->m_HeroID)
	{
		m_plist = g_pMainState->m_Friend.FindFriend2(pc->m_PCData.m_Name);
		if (m_Mode == 1 || m_Mode == 2)
		{
			if (m_plist->m_bOnZuoQi&&m_plist->m_ZuoQiID>-1)
			{
	
			}
			else
			{
				
				OnOff(FALSE);
				return;
			}
		}
	}

	//0 ��ɫȾɫ  1����Ⱦɫ  2 ����װ��Ⱦɫ 3 �ٻ���Ⱦɫ
	DWORD data=0;
	m_ChangeColorData.Reset();
	switch (mode)
	{
	case 0:
		if (1)
		{
			if (pc->m_PCData.m_bUseCard)
			{
				g_pMainState->m_Tags.Add("����״̬���޷�Ⱦɫ");
				OnOff(FALSE);
				return;
			}
			if (pc->m_PCData.m_bOnZuoQi)
			if (pc->m_PCData.m_ZuoQi.m_pZuoQi)
			{
				pc->m_PCData.m_bOnZuoQi = FALSE;
			}

			//data=pc->GetID(POS_STAND);
			m_ColorID = g_pMainState->m_pCharacterDataList[pc->m_PCData.modeid].m_ColorID;
			g_pMainState->m_ColorManage.Set(m_ColorID);
			m_ChangeColorData.partnum = g_pMainState->m_ColorManage.GetPartNum();
			if (pc->m_PCData.m_RanSe.m_ranse[0].partnum)
			{
				m_ChangeColorData = pc->m_PCData.m_RanSe.m_ranse[0];
			}
			data= pc->GetID(POS_STAND);
		}
		break;
	case 1:
		if (1)
		{
			if (!pc->m_PCData.m_ZuoQi.m_pZuoQi)
			{
				OnOff(FALSE);
				return;
			}
			data=pc->m_PCData.m_ZuoQi.GetID(POS_STAND);
			m_ColorID = pc->m_PCData.m_ZuoQi.m_pZuoQi->m_ColorID;
			g_pMainState->m_ColorManage.Set(m_ColorID);
			m_ChangeColorData.partnum = g_pMainState->m_ColorManage.GetPartNum();

			if (pc->m_PCData.m_ZuoQiRanSe.partnum)
			{
				m_ChangeColorData = pc->m_PCData.m_ZuoQiRanSe;
			}
		}
		break;
	case 2:
		if (1)
		{
			if (!pc->m_PCData.m_ZuoQi.m_ZhuangShi.GetNum())
			{
				break;
			}
			sZuoQiZhuangShi* pZhuangShi = &g_pMainState->m_ItemManage.m_ZuoQiZhuangShiList[pc->m_PCData.m_ZuoQi.m_ZhuangShi.GetID()];
			data = pZhuangShi->NodeStand;
			m_ColorID = pZhuangShi->m_ColorID;
			g_pMainState->m_ColorManage.Set(m_ColorID);
			m_ChangeColorData.partnum = g_pMainState->m_ColorManage.GetPartNum();
			if (pc->m_PCData.m_ZhuangShiRanSe.partnum)
			{
				m_ChangeColorData = pc->m_PCData.m_ZhuangShiRanSe;
			}

		}
		break;
	case 3:SetPet(0, pc);
		return;
	default:
		break;
	}
	
	m_Target.Load(data);
	UpdateColor();
}

void cInterfaceRanSe::UpdateColor()
{
	DWORD id=  m_Target.m_NowID;
	if (!id)return;
	int fram = m_Target.GetNowFrame();
	m_Target.Free();
	g_pMainState->m_ColorManage.Set(m_ColorID);
	for (int i = 0; i < m_ChangeColorData.partnum; i++)
	{
		//��ֹԽ��
		if (m_ChangeColorData.GetColorChange(i) >= g_pMainState->m_ColorManage.m_CharacterColor.m_stagesplit[i].m_ColorMatrix.size())
			m_ChangeColorData.SetColorChange(i,0);
		g_pMainState->m_ColorManage.GetColorMatrix(i, m_ChangeColorData.GetColorChange(i), m_ChangeColorData.m_colormatrix[i]);
	}
	m_Target.Load(id, &m_ChangeColorData);
	m_Target.SetFrame(fram);
}

BOOL cInterfaceRanSe::CheckRB()
{
	if (!g_pMainState->Mouse.GetButtonState(MOUSE_RBUTTON))return FALSE;
	g_pMainState->Mouse.SetLock(MOUSE_RBUTTON);
	OnOff(FALSE);
	return TRUE;
}

void cInterfaceRanSe::SetPet(int part, cCharacter* pc)
{
	if (pc->m_PCData.m_FightPetID < 0)return;
	cPetData* pPetData = &pc->m_PCData.m_pPetDataList[pc->m_PCData.m_FightPetID]->m_PetDataZZ;
	int modeid = pPetData->modeid;
	sPetData& spetdata = g_pMainState->m_PetDataManager.m_PetList[modeid];
	vector<int>& partid = spetdata.m_posIDList[pPetData->m_JinJieNum.m_PositionID];
	if (part >= partid.size())part = 0;
	vector<DWORD> data;
	pc->m_PCData.m_pPetDataList[pc->m_PCData.m_FightPetID]->GetID(POS_STAND,data);
	m_NowPart = part;
	m_ColorID = spetdata.m_PartList[partid[m_NowPart]].m_ColorID;
	if (m_ColorID == 0)
	{
		/*g_pMainState->m_Tags.Add("���ź�,���ڱ�����,�����ٻ����޷�Ⱦɫ");

		return;*/
		m_ColorID =0x1657;
	}
	g_pMainState->m_ColorManage.Set(m_ColorID);
	m_ChangeColorData.partnum = g_pMainState->m_ColorManage.GetPartNum();

	/*	if (pc->m_PCData.m_ZuoQi.m_ZhuangShi.m_Property.size())
	{
	m_ChangeColorData = pPetData->m_RanSe.m_ranse[0];
	}*/
	m_Target.Load(data[m_NowPart]);
	UpdateColor();
}

void cBangPaiJiNengXueXi::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;
	m_NeedShow = FALSE;
	m_Back.Load(0x254E7029);

	m_Cancel.Set(&g_pMainState->m_Cancel);
	m_Cancel.m_bStatic = TRUE;
	m_Button[0].Load(0x35A981D3);
	m_Button[1].Load(0x2BD1DEF7);
	for (int i = 0; i < 2; i++)
	{
		
		m_Button[i].SetStatic(TRUE);
		m_tButton[i].SetColor(RGB(255, 255, 255));
	}
	m_tButton[0].SetString("ѧϰ");
	m_tButton[1].SetString("ѧϰ*10");
	m_tXueFei.SetString("100");
	for (int i = 0; i < c_NumFuZhu; i++)
	{
		m_SkillHead[i].Load(g_pMainState->m_SkillManager.m_FuZhuSkillList[i].m_BigHead);
	}
	m_SelectMark.Load(0x10921CA7);
	m_SelectMark.m_NeedShow = FALSE;
	
	Move(320 - m_Back.GetWidth() / 2, 240 - m_Back.GetHeight() / 2);
}

void cBangPaiJiNengXueXi::Move(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
	m_Back.SetXY(x, y);
	m_Cancel.SetXY(x + m_Back.GetWidth() - 20, y + 3);
	int xstart, ystart;
	xstart = 147 + x;
	ystart = 331  + y;

	m_tExp.SetXY(xstart, ystart);
	m_tNeedBangGong.SetXY(xstart, ystart+25);
	m_tXueFei.SetXY(xstart, ystart+50);
	m_Button[0].SetXY(xstart,ystart+75);
	m_tButton[0].SetXCenter(m_Button[0].GetCenter());
	m_tButton[0].SetY(ystart + 78);
	m_tButton[0].UpdateXCenter();

	xstart += 234;

	m_tNeedExp.SetXY(xstart, ystart);
	m_tNeedMoney.SetXY(xstart, ystart+25);
	m_tChuBei.SetXY(xstart, ystart+50);
	m_Button[1].SetXY(xstart, ystart+75);
	m_tButton[1].SetXCenter(m_Button[1].GetCenter());
	m_tButton[1].SetY(ystart + 78);
	m_tButton[1].UpdateXCenter();

	xstart =m_xPos+ 24;
	ystart =m_yPos+ 38;
	for (int i = 0; i < c_NumFuZhu; i++)
	{
		m_SkillHead[i].SetXY(xstart+ (i%7) *69,ystart+(i/7)* 80);
		m_tSkillHead[i].SetXCenter(m_SkillHead[i].GetCenter());
		m_tSkillHead[i].SetY(m_SkillHead[i].GetY()+50);
		m_tSkillHead[i].UpdateXCenter();
	}
	m_tMiaoShu.SetXY(m_xPos+30,m_yPos+207);
	m_tMiaoShu.LockWidth(440);

	if (m_SelectMark.m_NeedShow)
	{
		m_SelectMark.SetXY(m_SkillHead[m_SkillSelect].GetX() - 3, m_SkillHead[m_SkillSelect].GetY() - 5);
	}
}

BOOL cBangPaiJiNengXueXi::Show(cCanvas* pCanvas)
{
	pCanvas->Add(&m_Back, 1, 1);
	for (int i = 0; i < 7; i++)//c_NumFuZhu; i++)
	{
		pCanvas->Add(&m_SkillHead[i], 1);
		pCanvas->Add(&m_tSkillHead[i]);
	}
	pCanvas->Add(&m_Cancel,1);
	pCanvas->Add(&m_tExp);
	pCanvas->Add(&m_tNeedBangGong);
	pCanvas->Add(&m_tNeedExp);
	pCanvas->Add(&m_tNeedMoney);
	pCanvas->Add(&m_tXueFei);
	pCanvas->Add(&m_tChuBei);
	pCanvas->Add(&m_tMiaoShu);
	pCanvas->Add(&m_SelectMark, 1);
	for (int i = 0; i < 2; i++)
	{
		pCanvas->Add(&m_Button[i],1);
		pCanvas->Add(&m_tButton[i]);
	}
	return TRUE;
}

BOOL cBangPaiJiNengXueXi::CheckL()
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
		for (int i = 0; i < 10; i++)
		{
			if(!Leveupskill(1))break;
		}
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
		SetSkillSelect(m_Select - 10);
		break;
	}


	return TRUE;
}

void cBangPaiJiNengXueXi::OnOff(BOOL NeedShow)
{
	if (NeedShow)
	{
		Init();
		UpDate();
		SetSkillSelect(0);
	}
	g_pMainState->InterfaceSetShow(this, NeedShow);
	g_pMainState->m_Canvas.SetRedraw(m_Back.m_PreShowRect);
	return ;
}

void cBangPaiJiNengXueXi::UpDate()
{
	ostringstream oss;
	oss << "/";
	oss << m_BangpaiSkilllv;
	string maxskilllv = oss.str();
	string text;
	for (int i = 0; i < c_NumFuZhu; i++)
	{
		ostringstream oss2;
		oss2 << g_pCharacter->m_PCData.m_FuZhuSkillLvList[i];
		text = oss2.str();
		text += maxskilllv;
		m_tSkillHead[i].SetString(text);
		m_tSkillHead[i].UpdateXCenter();
	}
	oss.str("");
	oss << g_pCharacter->m_PCData.EXP;
	m_tExp.SetString(oss.str());

	oss.str("");
	oss << g_pCharacter->m_PCData.m_Money[2];
	m_tChuBei.SetString(oss.str());

	oss.str("");
	oss << g_pCharacter->m_PCData.EXP;
	m_tExp.SetString(oss.str());

}

void cBangPaiJiNengXueXi::SetSkillSelect(int select)
{
	if (select < 0)
	{
		m_SelectMark.m_NeedShow = FALSE;
		m_SkillSelect = -1;
		return;
	}
	m_SelectMark.m_NeedShow = TRUE;
	m_SelectMark.SetXY(m_SkillHead[select].GetX()-3, m_SkillHead[select].GetY()-5);
	m_SkillSelect = select;
	sSkill* pSkill = g_pMainState->m_SkillManager.GetFuZhuSkill(select);
	m_tMiaoShu.SetString(pSkill->m_Text);
	ostringstream oss;
	m_NeedExp = GetMainSkillExp(g_pCharacter->m_PCData.m_FuZhuSkillLvList[select]);
	oss << m_NeedExp;
	m_tNeedExp.SetString(oss.str());
	oss.str("");
	m_NeedMoney = GetMainSkillMoney(g_pCharacter->m_PCData.m_FuZhuSkillLvList[select]);
	oss << m_NeedMoney;
	m_tNeedMoney.SetColor(GetPriceColor((int)m_NeedMoney));
	m_tNeedMoney.SetString(oss.str());


	m_NeedBangGong = 0;// g_pCharacter->m_PCData.m_FuZhuSkillLvList[select];
	oss.str("");
	oss << m_NeedBangGong;
	m_tNeedBangGong.SetString(oss.str());
}

BOOL cBangPaiJiNengXueXi::ProcessInput()
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
	for (int i = 0; i <7;i++)// c_NumFuZhu; i++)
	{
		if (isOn(g_xMouse, g_yMouse, m_SkillHead[i].m_PreShowRect))
		{
			m_Select = i + 10;
			return CheckL();
		}
	}
	for (int i = 0; i < 2;i++)
		if(CheckOn(&m_Button[i], i+1))return TRUE;
	if(CheckOn(&m_Cancel, 0))return TRUE;
	//׼���ƶ�
	m_Select = 7;
	if (CheckL())return TRUE;
	m_Select = 0;
	if (CheckRB())return TRUE;
	return TRUE;
}
BOOL cBangPaiJiNengXueXi::CheckRB()
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


BOOL cBangPaiJiNengXueXi::Leveupskill(int lvup)
{
	if (m_SkillSelect < 0)return FALSE;
	int& Skilllv = g_pCharacter->m_PCData.m_FuZhuSkillLvList[m_SkillSelect];
	if (Skilllv >= m_BangpaiSkilllv)
	{
		if (m_BangpaiSkilllv == 180)
			g_pMainState->m_Tags.Add("������Ѿ��ﵽ����");
		else
			g_pMainState->m_Tags.Add("�ѴﵽĿǰ�ȼ���ѧϰ������");
		return FALSE;
	}
	int oldmoney[3],oldbanggong;
	INT64 oldexp;
	for (int i = 0; i < 3; i++)
		oldmoney[i] = g_pCharacter->m_PCData.m_Money[i];
	oldexp = g_pCharacter->m_PCData.EXP;
	oldbanggong = g_pCharacter->m_PCData.m_BangPaiPoint;

	/*if (!g_pCharacter->m_PCData.CostBangGong(m_NeedBangGong))
	{
		g_pCharacter->m_PCData.m_BangPaiPoint=oldbanggong;
		g_pMainState->m_Tags.Add("�ﹱ����");
		return FALSE;
	}*/
	if (!g_pCharacter->m_PCData.AddExp(-m_NeedExp,0,FALSE))
	{
		g_pCharacter->m_PCData.m_BangPaiPoint = oldbanggong;
		g_pCharacter->m_PCData.EXP = oldexp;
		g_pMainState->m_Tags.Add("���鲻��");
		return FALSE;
	}
	if (!g_pCharacter->m_PCData.AddMoney(0, (int)(-m_NeedMoney), 0, FALSE))
	{
		g_pCharacter->m_PCData.m_BangPaiPoint = oldbanggong;
		g_pCharacter->m_PCData.EXP = oldexp;
		for (int i = 0; i < 3; i++)
		{
			g_pCharacter->m_PCData.m_Money[i] = oldmoney[i];
		}
		g_pMainState->m_Tags.Add("Ǯ����");
		return FALSE;
	}
	g_pMainState->m_SkillManager.lvUpFuZhuSkill(g_pMainState->m_HeroID, g_pMainState->m_SkillManager.GetFuZhuSkill(m_SkillSelect)->m_Id, lvup);
	Skilllv += lvup;
	g_pMainState->m_State.UpDatePcDate();
	SetSkillSelect(m_SkillSelect);
	return TRUE;
}
