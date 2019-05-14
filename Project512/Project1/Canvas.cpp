#include "global.h"
#include <omp.h>


BOOL cCanvas::Init()
{
	if (m_bInit)return TRUE;
	m_bInit = TRUE;

	for (int i = 0; i < 300;i++)
	{
		sCanvasCube& cube = m_CanvasCube[i];
		cube.m_Data.m_x = (i%20) <<5;
		cube.m_Data.m_y = (i/20) <<5;
		cube.m_Data.m_Width = 32;
		cube.m_Data.m_Height = 32;

		
		cube.m_Rect.left = cube.m_Data.m_x;
		cube.m_Rect.top = cube.m_Data.m_y;
		cube.m_Rect.right = cube.m_Data.m_x + cube.m_Data.m_Width;
		cube.m_Rect.bottom = cube.m_Data.m_y + cube.m_Data.m_Height;


		cube.PrePicNum = -1;
		cube.PicNum = 0;
		cube.NeedReDraw = TRUE;
		cube.m_Data.m_pData = new BYTE[2048];
		cube.m_bCover = FALSE;
	}
	
	return TRUE;
}

BOOL cCanvas::Add(CWFile* pPic, int showmode, BOOL bCover )
{
	if (!pPic->m_NeedShow)return TRUE;
	if (!pPic->m_NowID)return TRUE;
	BOOL bChange = pPic->NextFrame();
	if (!pPic->m_NeedShow)return TRUE;
	BOOL bMove = pPic->m_bMove;

	RECT& picRect = pPic->m_PreShowRect;
	if (bChange || bMove) //如果图像发生改变,把之前的设为无效
	{
		if (pPic->m_PreShowRect.right>0)
			SetRedraw(picRect);
	}
	pPic->GetPicRange(picRect);
	int xstart;
	int ystart;
	int xend;
	int yend;

	if (0 == showmode)
	{	
		int xof = g_pMainState->m_Map.m_pMap->GetXCenter() - 320;
		int yof = g_pMainState->m_Map.m_pMap->GetYCenter() - 240;
		picRect.left = picRect.left - xof;
		picRect.top = picRect.top - yof;
		picRect.right = picRect.right - xof;
		picRect.bottom = picRect.bottom - yof;
	}
	if (picRect.left>639)return FALSE;
	if (picRect.top >479)return FALSE;
	if (picRect.right < 0)return FALSE;
	if (picRect.bottom < 0)return FALSE;
	

	xstart = picRect.left>>5;
	ystart = picRect.top >>5;
	xend = picRect.right >>5;
	yend = picRect.bottom>>5;

	
	if (xstart < 0)xstart = 0;
	if (ystart < 0)ystart = 0;
	if (xend >19)xend = 19;
	if (yend >14)yend = 14;

	
	int index2=xstart+ystart*20;
	int index=index2;
	if (bCover)
	{
		for (int j = ystart; j <= yend; j++)
		{

			for (int i = xstart; i <= xend; i++)
			{
				if (i == xstart || i == xend || j == ystart || j == yend)
				{
					m_CanvasCube[index].Add(pPic, showmode, bMove, 0, bChange);
				}
				else
				{
					if (1 == m_CanvasCube[index].NeedReDraw)
						m_CanvasCube[index].NeedReDraw = FALSE;
					m_CanvasCube[index].Add(pPic, showmode, bMove, 1, bChange);
				}
				index += 1;
			}
			index2 += 20;
			index = index2;
		}
	}
	else
	for (int j = ystart; j <= yend; j++)
	{
		for (int i = xstart; i <= xend; i++)
		{
			m_CanvasCube[index].Add(pPic, showmode, bMove, bCover, bChange);
			index += 1;
		}
		index2 += 20;
		index = index2;
	}
	pPic->m_bMove = FALSE;
	return TRUE;
}

BOOL cCanvas::Add(cText* pTex)
{
	int xtrue=pTex->GetX();
	int ytrue=pTex->GetY();
	if (!pTex->m_bShowOnScreen)
	{
		xtrue -= g_pMainState->m_Map.m_pMap->GetXCenter() - 320;
		ytrue -= g_pMainState->m_Map.m_pMap->GetYCenter() - 240;
	}
	if (!pTex->m_NeedShow)return TRUE;
	if (xtrue+pTex->GetShowWidth() < 0)return FALSE;
	if (ytrue+pTex->GetShowHeight() < 0)return FALSE;
	if (ytrue >479)return FALSE;
	if (xtrue >639)return FALSE;
	BOOL bMove = pTex->m_bMove;
	if (bMove)
	{
		if (pTex->m_PreShowRect.right > 0)
			SetRedraw(pTex->m_PreShowRect);
	}
	pTex->m_PreShowRect.left = xtrue;
	pTex->m_PreShowRect.right = xtrue + pTex->GetShowWidth();
	pTex->m_PreShowRect.top = ytrue;
	pTex->m_PreShowRect.bottom = ytrue + pTex->GetShowHeight();

	int xstart = pTex->m_PreShowRect.left >>5;
	int ystart = pTex->m_PreShowRect.top >>5;
	int xend = pTex->m_PreShowRect.right >>5;
	int yend = pTex->m_PreShowRect.bottom >>5;


	if (xstart < 0)xstart = 0;
	if (ystart < 0)ystart = 0;
	if (xend >19)xend = 19;
	if (yend >14)yend = 14;

	int index2 = xstart+20*ystart;
	int index = index2;
	for (int j = ystart; j <= yend; j++)
	{
		for (int i = xstart; i <= xend; i++)
		{
			m_CanvasCube[index].Add(pTex, bMove); 
			index += 1;
		}
		index2+=20;
		index = index2;
	}
	pTex->m_bMove = FALSE;
	return TRUE;
}

BOOL cCanvas::Add(cWfileP* p, int showmode, BOOL bCover/*=FALSE*/)
{
	if (!p->m_NeedShow)return TRUE;
	BOOL bChange = p->NextFrame();
	BOOL bMove = p->m_bMove;
	if (bChange || bMove) //如果图像发生改变,把之前的设为无效
	{
		if (p->m_PreShowRect.right > 0)
			SetRedraw(p->m_PreShowRect);
	}
	p->m_PreShowRect = p->GetPicRange();
	if (showmode == 0)
	{
		p->m_PreShowRect.left = p->m_PreShowRect.left - g_pMainState->m_Map.m_pMap->GetXCenter() + 320;
		p->m_PreShowRect.right = p->m_PreShowRect.right - g_pMainState->m_Map.m_pMap->GetXCenter() + 320;
		p->m_PreShowRect.top = p->m_PreShowRect.top - g_pMainState->m_Map.m_pMap->GetYCenter() + 240;
		p->m_PreShowRect.bottom = p->m_PreShowRect.bottom - g_pMainState->m_Map.m_pMap->GetYCenter() + 240;
	}
	if (p->m_PreShowRect.bottom<0)return FALSE;
	if (p->m_PreShowRect.right<0)return FALSE;
	if (p->m_PreShowRect.top > 479)return FALSE;
	if (p->m_PreShowRect.left > 639)return FALSE;
	if (p->m_PreShowRect.left < 0)p->m_PreShowRect.left = 0;
	if (p->m_PreShowRect.top < 0)p->m_PreShowRect.top = 0;

	int xstart = p->m_PreShowRect.left >>5;
	int ystart = p->m_PreShowRect.top >>5;
	int xend = p->m_PreShowRect.right >>5;
	int yend = p->m_PreShowRect.bottom >>5;

	if (xend>19)xend = 19;
	if (yend>14)yend = 14;

	int index2 = xstart + ystart * 20;
	int index = index2;
	if (bCover)
	{
		for (int j = ystart; j <= yend; j++)
		{
			for (int i = xstart; i <= xend; i++)
			{
				if (i == xstart || i == xend || j == ystart || j == yend)
					m_CanvasCube[index].Add(p, showmode, bMove, 0, bChange);
				else
				{
					if (1 == m_CanvasCube[index].NeedReDraw)
						m_CanvasCube[index].NeedReDraw = FALSE;
					m_CanvasCube[index].Add(p, showmode, bMove, 1, bChange);
				}
				index += 1;
			}
			index2 += 20;
			index = index2;
		}
	}
	else
	for (int j = ystart; j <= yend; j++)
	{

		for (int i = xstart; i <= xend; i++)
		{
			m_CanvasCube[index].Add(p, showmode, bMove, bCover, bChange);
			index += 1;
		}
		index2 += 20;
		index = index2;
	}
		p->m_bMove = FALSE;
	return TRUE;
}

BOOL cCanvas::Add(cObjFather* pTex, int showmode, BOOL bCover /*= FALSE*/)
{
	if (!pTex->m_NeedShow)return TRUE;
	int xtrue = pTex->GetX();
	int ytrue = pTex->GetY();
	if (!showmode)
	{
		xtrue -= g_pMainState->m_Map.m_pMap->GetXCenter() - 320;
		ytrue -= g_pMainState->m_Map.m_pMap->GetYCenter() - 240;
	}
	
	if (xtrue + pTex->GetShowWidth() < 0)return FALSE;
	if (ytrue + pTex->GetShowHeight() < 0)return FALSE;
	if (ytrue >479)return FALSE;
	if (xtrue >639)return FALSE;
	
	pTex->m_PreShowRect.left = xtrue;
	pTex->m_PreShowRect.right = xtrue + pTex->GetShowWidth();
	pTex->m_PreShowRect.top = ytrue;
	pTex->m_PreShowRect.bottom = ytrue + pTex->GetShowHeight();

	int xstart = pTex->m_PreShowRect.left >>5;
	int ystart = pTex->m_PreShowRect.top >>5;
	int xend = pTex->m_PreShowRect.right >>5;
	int yend = pTex->m_PreShowRect.bottom>>5;

	BOOL bMove = pTex->m_bMove;
	if (bMove)
	{
		SetRedraw(pTex->m_PreShowRect);
	}
	if (xstart < 0)xstart = 0;
	if (ystart < 0)ystart = 0;
	if (xend >19)xend = 19;
	if (yend >14)yend = 14;

	int index2=xstart+ystart*20;
	int index = index2;
	if (bCover)
	{
		for (int j = ystart; j <= yend; j++)
		for (int i = xstart; i <= xend; i++)
		{
			if (i == xstart || i == xend || j == ystart || j == yend)
				m_CanvasCube[index].Add(pTex, showmode, bMove, 0);
			else
			{
				if (1 == m_CanvasCube[index].NeedReDraw)
					m_CanvasCube[index].NeedReDraw = FALSE;
				m_CanvasCube[index].Add(pTex, showmode, bMove, 1);
			}
			index += 1;
		}
		index2 += 20;
		index = index2;
	}
	else
	for (int j = ystart; j <= yend; j++)
	{

		for (int i = xstart; i <= xend; i++)
		{
			m_CanvasCube[index].Add(pTex, showmode, bMove, 0);
			index += 1;
		}
		index2 += 20;
		index = index2;
	}
	pTex->m_bMove = FALSE;
	return TRUE;
}

BOOL cCanvas::Show()
{
	while (!g_pMainState->m_Map.m_pMap->m_bLoadReady)
	{
		Sleep(1);
	}
	if (m_Time < 300)m_Time += 30;
	for (int i = 0; i < m_Time; i++)
	{
		sCanvasCube& cube = m_CanvasCube[i];
		if (!cube.NeedReDraw)
		{
			if (cube.PrePicNum != cube.PicNum) cube.NeedReDraw = TRUE;
			else if (cube.PreCover != cube.m_bCover)cube.NeedReDraw = TRUE;
			else continue;
		}


		if (g_StateType == STATE_FIGHT)
		{
			if (!g_pMainState->m_pFightState)return TRUE;
	
			BYTE* p1 = cube.m_Data.m_pData;
			BYTE* p2 = &g_pMainState->m_pFightState->m_pFightBack[(((i / 20) << 5)) * 1280 + ((i % 20) << 6)];
			for (int i2 = 0; i2 < 32; i2++) 
			{
				memcpy_s(p1, 64, p2, 64);
				p1 += 64;
				p2 += 1280;
			}
		}
		else
		{
			if (m_NeedShowMap)
			{
				if (!cube.m_bCover)
				{
					g_pMainState->m_Map.Show(cube.m_Data.m_pData, cube.m_Rect);
				}
				
			}
		}
		for (int k = 0; k < cube.PicNum; k++)
		{
			switch (cube.PicType[k])
			{
			case 0:
				if (1)
				{
					CWFile* p = (CWFile*)cube.pPicList[k];
					if (cube.ShowMode[k])
					{
						p->ShowOnScreen(cube.m_Data.m_pData, cube.m_Rect);
					}
					else 	p->Show(cube.m_Data.m_pData, cube.m_Rect, &g_pMainState->m_Map);
				}
				break;
			case 2:
				if (1)
				{
					cText* p = (cText*)cube.pPicList[k];
					p->DrawAlphaEx(cube.m_Data.m_pData, cube.m_Rect, g_pMainState->m_Map.m_pMap);
				}
				break;
			case 3:
				if (1)
				{
					cWfileP* p = (cWfileP*)cube.pPicList[k];

					switch (cube.ShowMode[k])
					{
					case 0:
						p->Show(cube.m_Data.m_pData, cube.m_Rect, &g_pMainState->m_Map);
						break;
					case 1:
						p->ShowOnScreen(cube.m_Data.m_pData, cube.m_Rect);
						break;
					}
				}
				break;
			case 4:
				if (1)
				{
					cObjFather* p = (cObjFather*)cube.pPicList[k];
					switch (cube.ShowMode[k])
					{
					case 0:
						if (1)
						{
							int xpos = p->m_x - g_pMainState->m_Map.m_pMap->GetXCenter() + 320;
							int ypos = p->m_y - g_pMainState->m_Map.m_pMap->GetYCenter() + 240;
							cube.m_Data.Draw(p, xpos, ypos);
						}
						break;
					case 1:
						cube.m_Data.Draw(p);
						break;
					}
				}
				break;
			}
		}
	}
	

	SwapBuf();

	for (int i = 0; i < m_Time; i++)
	{
		m_CanvasCube[i].PrePicNum = m_CanvasCube[i].PicNum;
		m_CanvasCube[i].PicNum = 0;
		m_CanvasCube[i].NeedReDraw = FALSE;
		m_CanvasCube[i].PreCover = m_CanvasCube[i].m_bCover;
		m_CanvasCube[i].m_bCover = FALSE;
	}
	return TRUE;
}

BOOL cCanvas::Free()
{
	for (int i = 0; i < 300; i++)
	{
		SAFE_DELETE_ARRAY(m_CanvasCube[i].m_Data.m_pData);
	}
	DeleteObject(hPen);
	return TRUE;
}

cCanvas::~cCanvas()
{
	Free();
}

BOOL cCanvas::SetRedraw(RECT rect)
{
	int	xstart = rect.left>>5;
	int	ystart = rect.top >>5;
	int	xend = rect.right>>5;
	int	yend = (rect.bottom)>>5;

	if (xstart < 0)xstart = 0;
	if (ystart < 0)ystart = 0;
	if (xend >19)xend = 19;
	if (yend >14)yend = 14;
	int k;
	for (int i = xstart; i <= xend; i++)
	for (int j = ystart; j <= yend; j++)
	{
		k = i + j * 20;
		m_CanvasCube[k].PrePicNum = -1;
	}
	return TRUE;
}

void cCanvas::SwapBuf()
{
	g_pBmpinfo->bmiHeader.biHeight = -32;
	g_pBmpinfo->bmiHeader.biWidth = 32;
	g_pBmpinfo->bmiHeader.biCompression = 3;
	g_pBmpinfo->bmiHeader.biSizeImage = 2048;
	for (int i = 0; i < 300; i++)
	{
		if (1 == m_CanvasCube[i].NeedReDraw)
		{
			//////画出来
			SetDIBitsToDevice(
				g_DC,
				m_CanvasCube[i].m_Data.m_x,
				m_CanvasCube[i].m_Data.m_y,
				32,
				32,
				0, 0,
				0,
				32,
				m_CanvasCube[i].m_Data.m_pData,
				g_pBmpinfo,
				DIB_RGB_COLORS
				);
		}
	}
	BitBlt(g_ShowDC, 0, 0, 640, 480, g_DC, 0, 0, SRCCOPY);
}

void cCanvas::SetRedrawAll(BOOL ball)
{
	if (ball)
	{
		for (int i = 0; i < 300; i++)
		{
			m_CanvasCube[i].PrePicNum = -1;
		}
	}
	else
	for (int i = 0; i < 300; i++)
	{
		if (m_CanvasCube[i].m_bCover)
			continue;
		m_CanvasCube[i].PrePicNum = -1;
	}
}

BOOL sCanvasCube::Add(CWFile* p, int OnScreen, BOOL bMove, BOOL bCover,BOOL bChange)
{
	CheckNum(bCover);
	ShowMode[PicNum] = OnScreen;
	pPicList[PicNum] = p;
	PicType[PicNum] = 0;
	PicNum += 1;
	if (NeedReDraw)return TRUE;
	if (bChange || bMove)
	{
			NeedReDraw = TRUE;
	}
	return TRUE;
}
BOOL sCanvasCube::Add(cWfileP* p, int OnScreen, BOOL bMove, BOOL bCover,BOOL bChange)
{
	
	CheckNum(bCover);
	ShowMode[PicNum] = OnScreen;
	pPicList[PicNum] = p;
	PicType[PicNum] = 3;
	PicNum += 1;
	if (NeedReDraw)return TRUE;
	if (bChange || bMove)
	{
		NeedReDraw = TRUE;
	}
	return TRUE;
}

BOOL sCanvasCube::Add(cText* p, BOOL bMove)
{
	CheckNum(0);
	ShowMode[PicNum] = 0;
	pPicList[PicNum] = p;
	PicType[PicNum] = 2;
	PicNum += 1;

	if (!NeedReDraw)
	{
		//是否移动
		if (bMove)
			NeedReDraw = TRUE;
	}
	return TRUE;
}

BOOL sCanvasCube::Add(cObjFather* p, int showmode, BOOL bMove, BOOL bCover)
{
	CheckNum(bCover);
	ShowMode[PicNum] = showmode;
	pPicList[PicNum] = p;
	PicType[PicNum] = 4;
	PicNum += 1;
	//是否移动
	if (bMove)
		NeedReDraw = TRUE;
	return TRUE;
}

BOOL sCanvasCube::CheckNum(BOOL bCover)
{
	if (bCover)
	{
		PicNum = 0;
		m_bCover = bCover;
		return TRUE;
	}
	if (19 < PicNum)
	{
		for (int i = 0; i < 19; i++)
		{
			pPicList[i] = pPicList[i + 1];
			PicType[i] = PicType[i + 1];
			ShowMode[i] = ShowMode[i+1];
		}
		PicNum = 19;
	}
	return TRUE;
}





