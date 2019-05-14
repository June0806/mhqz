#include "global.h"






#pragma pack (push)
#pragma pack (1)
struct TGAFILEHEADER{
	BYTE bIDLength;						//������Ϣ����
	BYTE bPalType;						//��ɫ����Ϣ(��֧��)
	BYTE bImageType;					//ͼ������(ֻ֧�� 2,10)
	WORD wPalIndex;						//��ɫ���һ������ֵ
	WORD wPalLength;					//��ɫ��������
	BYTE bPalBits;						//һ����ɫ�嵥λλ��(15,16,24,32)
	WORD wLeft;							//ͼ���������(��������)
	WORD wBottom;						//ͼ��׶�����(��������)
	WORD wWidth;						//ͼ����
	WORD wHeight;						//ͼ��߶�
	BYTE bBits;							//һ������λ��
	BYTE bAlphaBits : 4;					//Alphaλ��
	BYTE bReserved : 1;					//����,����Ϊ0
	BYTE bTopDown : 1;					//Ϊ1��ʾ���ϵ���(ֻ֧�ִ��µ���)
	BYTE bInterleaving : 2;				//����(һ��Ϊ0)
};




#pragma pack (pop)
typedef BYTE* lpBYTE;
CSprite::CSprite(void)
: m_MaxDirection(0)
, m_Frame(0)
, m_Width(0)
, m_Height(0)
, lpAlpha(NULL)
, lpDate(NULL)
, pitch(NULL)
, m_kx(0)
, m_ky(0)
, flp(NULL)
, fstr(NULL)
, fsize(0)
{
}
CSprite::~CSprite(void)
{
	
	FreeDatelp();
	
	
}
BOOL CSprite::Load(void* lp, long size, int type, sChangeColorData* pChangeColorData)
{
	FreeDatelp();
	int i, j;

	bool isgh;	//�Ƿ����
	isgh = 1;

	WASFILEHEADER head;		// �ļ�ͷ��Ϣ
	WPixel Pal[256];		// ��ɫ��
	DWORD* FrameList;		// ֡ƫ�Ʊ�


	DWORD* LineList;		// ��ƫ�Ʊ�

	mcreat(lp, size);
	//		mseek(0);
	mread(&head, sizeof(WASFILEHEADER), 1);
	if (head.wastitle != 'PS')
	{
		FreeDatelp();
		ERRBOX;
	}

	// ȡ��ɫ��
	mseek(head.szHheard + 4);
	mread(Pal, 256 * sizeof(WPixel), 1);

	// ��ȡSprite�ļ���Ϣ
	m_MaxDirection = head.nDirection;

	m_Frame = head.nFrame;

	m_Width = head.nWidth;
	m_Height = head.nHeigh;

	m_kx = head.xCenter;
	m_ky = head.yCenter;

	int numoffset = m_MaxDirection*m_Frame;
	// ����֡ƫ�Ʊ�����
	FrameList = new DWORD[numoffset];

	m_pDataOffsetList = new DATEHEARD[numoffset];
	// ��ȡ֡ƫ�Ʊ�
	for (i = 0; i < numoffset; i++)
		mread(&(FrameList[i]), 4, 1);

	lpDate = new lpBYTE[numoffset];// ��ȡͼƬ����ָ���ָ��
	lpAlpha = new lpBYTE[numoffset];// ��ȡalpha����ָ���ָ��


	// �õ����ͼƬ�Ĵ�С
	for (i = 0; i < numoffset; i++)
	{
		DATEHEARD& datehead = m_pDataOffsetList[i];		// ֡����ͷ
		// ��ȡ֡����ͷ
		if (FrameList[i] == 0) continue;
		mseek(FrameList[i] + 4 + head.szHheard);
		mread(&datehead, sizeof(DATEHEARD), 1);
		if (datehead.Height>m_Height)m_Height = datehead.Height;
		if (datehead.Width > m_Width)m_Width = datehead.Width;
	}
	pitch = ((m_Width << 1) + 7) & 0xfffffff8;
	int sizedata = pitch*m_Height + 7;
	int sizealpha = m_Width*m_Height;
	for (i = 0; i < numoffset; i++)			// �����ڴ�ռ�
	{
		if ((lpDate[i] = new BYTE[sizedata]) == 0)
		{
			ERRBOX;
		}

		if ((lpAlpha[i] = new BYTE[sizealpha]) == 0)
		{
			ERRBOX;
		}
		memset(lpAlpha[i], 0, sizealpha);
	}

	//��ʱ���ݻ���
	BYTE ckdate;	//�����ֽ�
	BYTE wdate;		//��д���ֽ�����
	BYTE walpha;	//��д��alphaֵ
	unsigned int ran;//�ظ�����

	//	int fx, fy;		// ֡������
	int r, g, b;

	//Ⱦɫ
	if (pChangeColorData)
	{
		for (int i = 0; i < pChangeColorData->partnum; i++)
		{
			sChangeColorMatrix*  matrix = &pChangeColorData->m_colormatrix[i];

			for (int j = matrix->m_start; j < matrix->m_end; j++)
			{
				WPixel& pal = Pal[j];
				r = pal.red*matrix->x1 + pal.green*matrix->x2 + pal.blue*matrix->x3;
				g = pal.red*matrix->y1 + pal.green*matrix->y2 + pal.blue*matrix->y3;
				b = pal.red*matrix->z1 + pal.green*matrix->z2 + pal.blue*matrix->z3;
				r = r >> 8;
				g = g >> 8;
				b = b >> 8;
				if (r>31)    r = 31;
				if (g > 63) g = 63;
				if (b > 31)	 b = 31;
				pal.red = r;
				pal.green = g;
				pal.blue = b;
			}
		}
	}


	WPixel PIX;
	PIX = RGB(255, 255, 255);

	
	WORD* pD;
	BYTE* pA;
	//BYTE* pD1;
	// ��ȡ֡����
	for (i = 0; i < m_MaxDirection*m_Frame; i++)
	{
		// ��ȡ֡����ͷ
		if (FrameList[i] == 0) continue;
		DATEHEARD& datehead = m_pDataOffsetList[i];		// ֡����ͷ
		mseek(FrameList[i] + 4 + head.szHheard);
		mread(&datehead, sizeof(DATEHEARD), 1);


		// ������ƫ�Ʊ�����
		LineList = new DWORD[datehead.Height];
		// ȡ��ƫ�Ʊ�
		for (j = 0; j < datehead.Height; j++)
			mread(&(LineList[j]), 4, 1);

		for (int iy = 0; iy < datehead.Height; iy++)
		{
			pD = (WORD*)&lpDate[i][iy*pitch];
			pA = &lpAlpha[i][iy*m_Width];
			mseek(4 + head.szHheard + FrameList[i] + LineList[iy]);
			for (int ix = 0; ix < datehead.Width;)
			{	
				mread(&ckdate, 1, 1);
				if (ckdate == 0)					//�����н���  
				{
					break;
				}
				else if ((ckdate & 0xC0) == 0)		// ��Bit7-8=00 
				{
					if (isgh)
					{
						if (iy % 2)
							isgh = 0;
					}
					if (ckdate & 0x20)				//��Bit6=1
					{
						walpha = (ckdate & 0x1f);	//Bit1-5
						mread(&wdate, 1, 1);		//��ȡ��һ�ֽ�
						if (walpha)
						{
							*pD = Pal[wdate].color;
							*pA = (walpha << 3) - 7;
						}
						++ix;
						++pD;
						++pA;
					}
					else
					{
						ran = (ckdate & 0x1f);
						mread(&walpha, 1, 1);
						mread(&wdate, 1, 1);

						if (walpha)
						{
							walpha = (walpha << 3) - 7;							
							while (ran)
							{
								if (ix < m_Width)
								{
									*pD = Pal[wdate].color;
									*pA = walpha;
								}
								++ix;
								++pD;
								++pA;
								--ran;
							}
						}
					
					}
				}
				else if ((ckdate & 0xC0) == 0x40)	// ��Bit7-8=01 01 00 00 00
				{
					if (isgh)
					{
						if (iy % 2)
							isgh = 0;
					}
					ran = (ckdate & 0x3f);
					walpha = 255;			
					while (ran)
					{
						mread(&wdate, 1, 1);
						if (ix < m_Width)
						{
							*pD = Pal[wdate].color;
							*pA = walpha;
						}
						++ix;
						++pD;
						++pA;
						--ran;
					}
				}
				else if ((ckdate & 0xC0) == 0x80)	// ��Bit7-8=10  10 00 00 00
				{
					if (isgh)
					{
						if (iy % 2)
							isgh = 0;
					}
					ran = (ckdate & 0x3f);
					walpha = 255;
					mread(&wdate, 1, 1);

					
					while (ran)
					{
						if (ix < m_Width)
						{
							*pD = Pal[wdate].color;
							*pA = walpha;
						}
						++ix;
						++pD;
						++pA;
						ran--;
					}
				}
				else							// ��Bit7-8=11
				{
					ran = (ckdate & 0x3f); //6λ
					if (0 == ran)
					{

						flp -= 1;
						*flp = 0x40;
						flp += 1;
						*flp = 0x40;
						flp += 1;
						*flp = 0x40;
						flp -= 2;
						//��������
						
					}
					else
					{
						ix += ran;
						pD += ran;
						pA += ran;
					}
				}
			}
		}
		if (isgh)
		{
			for (int iy = 0; iy < m_Height; iy += 2)
			{
				if (iy + 1 < m_Height)
				{
					memcpy(&lpDate[i][(iy + 1)*pitch], &lpDate[i][iy*pitch], m_Width << 1);
					memcpy(&lpAlpha[i][(iy + 1)*m_Width], &lpAlpha[i][iy*m_Width], m_Width);
				}
			}
		}
		delete[] LineList;
	}

	delete[] FrameList;
	mclose();

	m_bLoad = TRUE;
	return TRUE;
}

void CSprite::FreeDatelp(void)
{
	if (!m_bLoad)return;
	m_bLoad = FALSE;
	SAFE_DELETE_ARRAY(m_pDataOffsetList);
	int i;
	if (lpAlpha)
	{
		for (i = 0; i<m_MaxDirection*m_Frame; i++)
		{
			SAFE_DELETE_ARRAY(lpAlpha[i]);
		}
		SAFE_DELETE_ARRAY(lpAlpha);
		
	}
	if (lpDate)
	{
		for (i = 0; i<m_MaxDirection*m_Frame; i++)
		{
				SAFE_DELETE_ARRAY(lpDate[i]);
		}
		SAFE_DELETE_ARRAY( lpDate);
	}
	m_MaxDirection = 0;
	m_Frame = 0;
	m_Height = 0;
	m_Width = 0;
	m_pOwner = 0;
}

HBITMAP CSprite::DIBtoDDB(WORD* data, BITMAPINFO*  pBmpInfo)
{


	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)pBmpInfo;
	HBITMAP hbm;
	HPALETTE Pal = 0;
	HPALETTE OldPal = 0;
	HDC dc = GetDC(g_hWnd);

	int nColors = lpbi->biClrUsed ? lpbi->biClrUsed : 1 >> lpbi->biBitCount; //��ɫ����ĸ���

	BITMAPINFO &bmInfo = *(LPBITMAPINFO)g_pBmpinfo;

	if (nColors <= 256 && GetDeviceCaps(dc, RASTERCAPS) & RC_PALETTE)
	{
		UINT nSize = sizeof(LOGPALETTE)+(sizeof(PALETTEENTRY)* nColors);
		LOGPALETTE *plp = (LOGPALETTE *) new BYTE[nSize];
		plp->palVersion = 0x300;
		plp->palNumEntries = nColors;

		for (int i = 0; i < nColors; i++)
		{
			plp->palPalEntry[i].peRed = bmInfo.bmiColors[i].rgbRed;
			plp->palPalEntry[i].peGreen = bmInfo.bmiColors[i].rgbGreen;
			plp->palPalEntry[i].peBlue = bmInfo.bmiColors[i].rgbBlue;
			plp->palPalEntry[i].peFlags = 0;
		}

		Pal = CreatePalette(plp);

		delete[] plp; //Ҫ��ʱ����

		// select and realize the palette
		OldPal = SelectPalette(dc, Pal, false);
		RealizePalette(dc);
	}


	hbm = CreateDIBitmap(
		dc, //�豸�����ĵľ��
		(LPBITMAPINFOHEADER)lpbi, //λͼ��Ϣͷָ��
		(long)CBM_INIT, //��ʼ����־
		(void*)data,//lpDIBits, //��ʼ������ָ��
		(LPBITMAPINFO)lpbi, //λͼ��Ϣָ��
		DIB_RGB_COLORS
		); //��ɫ���ݵ�ʹ�÷�ʽ

	if (Pal)
	{
		SelectPalette(dc, OldPal, false);
	}

	return hbm;
}


void CSprite::mread(void* Dst, unsigned int size, unsigned int n)
{
	unsigned int ran = size*n;
	if (flp + ran - fstr>fsize)
	{
		flp = fstr + fsize;
		memset(Dst, 0, ran);
	}
	else
	{
		memcpy(Dst, flp, ran);
		flp += ran;
	}
}

void CSprite::mcreat(void* ilp, unsigned int size)
{
	flp = (BYTE*)ilp;
	fstr = flp;
	fsize = size;
}

void CSprite::mseek(long offset)
{
	flp = fstr + offset;
}

void CSprite::mclose(void)
{
	flp = NULL;
	fstr = NULL;
}


BOOL CSprite::isInMask(int x, int y, int w, int h, int xMask, int yMask, int wMask, int hMask)
{
	if (y+h >yMask + hMask)return FALSE;
	if (yMask > y + h)      return FALSE;
	if (xMask>x + w)        return FALSE;
	if (xMask + wMask<x)    return FALSE;
	
	return TRUE;
}

BOOL CSprite::ShowOnScreen(BYTE* Pic, RECT rect)
{
	if (0 == lpAlpha)return FALSE;
	//ͼ�����Ͻ��������Ļ��λ��
	int a = GetDataID();
	int xScreenPos = m_pOwner->m_x - m_pDataOffsetList[a].FramX;
	int yScreenPos = m_pOwner->m_y - m_pDataOffsetList[a].FramY;
	RECT  picrect;  //ͼƬ����Ĵ�С
	RECT nowrect;  //ͼƬ������ʾ�Ĵ�С
	int xOffset;
	int yOffset;
	int xOffset3;
	int yOffset3;
	if (!CountCross(xScreenPos, yScreenPos, rect, picrect, nowrect,
		xOffset, yOffset, xOffset3, yOffset3))
		return FALSE;
	//ʵ������Ĵ�С
	int Height = nowrect.bottom - nowrect.top;
	int Width = nowrect.right - nowrect.left;
	//ʵ���������ͼƬ�����ƫ��
	WPixel pix1, pix2;

	BYTE*  lpSrc = lpDate[a];
	BYTE*  pAlpha = lpAlpha[a];

	int alpha;
	int yof2 = yOffset + m_pOwner->m_yOffset2;
	int xof2 = xOffset + m_pOwner->m_xOffset2;
	int dealpha;

	BYTE* p1 = &Pic[(yOffset3 << 6) + (xOffset3 << 1)];
	BYTE* p2 = &lpSrc[yof2*pitch + ((xof2) << 1)];
	BYTE* pA = &pAlpha[yof2*m_Width + xof2];
	int j2;
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			j2 = j << 1;
			alpha = pA[j];

			if (!alpha)continue;

			pix1.l = p2[j2];
			pix1.h = p2[j2 + 1];

			////��͸��
			if (m_pOwner->m_bShadow)
			{
				alpha = (alpha*(100 - m_pOwner->m_bShadow)) / 100;
			}
			if (m_pOwner->m_bTwinkle) //��˸  (��ջ���)
			{
				if (m_pOwner->now_frame % 3 == 0)
				{
					if (pix1.red > 24)
						pix1.red = 31;
					else
						pix1.red += 7;
					if (pix1.green > 49)
						pix1.green = 63;
					else
						pix1.green += 14;
					if (pix1.blue < 10)
						pix1.blue = 0;
					else
						pix1.blue -= 10;
				}
			}
			else
			if (m_pOwner->m_bShine)  //����  (��ɫ��ѡ��ʱ�����)
			{
				if (pix1.red>24)
					pix1.red = 31;
				else
					pix1.red += 7;
				if (pix1.green > 49)
					pix1.green = 63;
				else
					pix1.green += 14;
				if (pix1.blue > 24)
					pix1.blue = 31;
				else
					pix1.blue += 7;
			}
			if (alpha < 255)
			{
				pix2.l = p1[j2];
				pix2.h = p1[j2 + 1];
				dealpha = 255 - alpha;
				pix2.red = (pix1.red *alpha + pix2.red *dealpha) >> 8;
				pix2.green = (pix1.green *alpha + pix2.green * dealpha) >> 8;
				pix2.blue = (pix1.blue *alpha + pix2.blue * dealpha) >> 8;
				p1[j2] = pix2.l;
				p1[j2 + 1] = pix2.h;
			}
			else
			{
				p1[j2] = pix1.l;
				p1[j2 + 1] = pix1.h;
			}
		}
		p1 += 64;
		p2 += pitch;
		pA += m_Width;
	}
	return TRUE;
}
struct sMASKRect;
BOOL CSprite::Show(BYTE* Pic, RECT rect, cMyMap* pMap)
{
	if (0 == lpAlpha)return FALSE;
	cMap*& pMap2 = pMap->m_pMap;
	int xMapCenter = pMap2->GetXCenter();
	int yMapCenter = pMap2->GetYCenter();
	int a = GetDataID();
	//ͼ�����Ͻ��������Ļ��λ��
	int xScreenPos = m_pOwner->m_x - m_pDataOffsetList[a].FramX - xMapCenter + 320;
	int yScreenPos = m_pOwner->m_y - m_pDataOffsetList[a].FramY - yMapCenter + 240 - m_pOwner->m_yOffset;

	RECT  picrect;  //ͼƬ����Ĵ�С
	RECT nowrect;  //ͼƬ������ʾ�Ĵ�С
	int xOffset;
	int yOffset;
	int xOffset3;
	int yOffset3;
	if (!CountCross(xScreenPos, yScreenPos, rect, picrect, nowrect,
		xOffset, yOffset, xOffset3, yOffset3))
		return FALSE;
	//ʵ������Ĵ�С
	int Height = nowrect.bottom - nowrect.top;
	int Width = nowrect.right - nowrect.left;


	//	//����Ƿ��ڽ�����
	BOOL  bCross = 0;
	int InH;
	int InW;
	int  xOffmask;
	int  yOffmask;
	int  xOffPic;
	int  yOffPic;

	int xnowrectoffset, ynowrectoffset;

	RECT rectmask; //�����Ĵ�С
	RECT rectcross; //ͼƬ��ʾ���ͽ����ص��Ĵ�С
	int nummask;
	//�õ��ཻ�ľ���
	if (g_StateType == STATE_MAIN)
	{
		sMASKRect* pMaskRect;
		memset(g_pMainState->m_MaskList, 0, sizeof(int)* 1024);
		int i = 0;
		nummask = pMap2->m_MaskIDList.size();
		for (int j = 0; j < nummask; j++)
		{
			i = pMap2->m_MaskIDList[j];
			pMaskRect = &pMap2->m_pMaskRect[i];
			rectmask.left = pMaskRect->x - pMap2->GetXCenter() + 320;
			rectmask.top = pMaskRect->y - pMap2->GetYCenter() + 240;
			rectmask.right = rectmask.left + pMaskRect->w;
			rectmask.bottom = rectmask.top + pMaskRect->h;

			if (m_Bottom != -1)
			{
				if (pMaskRect->y + pMaskRect->h <m_Bottom)
					continue;
			}
			if (rectmask.left>640)continue;
			if (rectmask.right<0)continue;
			if (rectmask.top>480)continue;
			if (rectmask.bottom < 0)continue;

			if (!CheckCross(picrect, rectmask, rectcross))continue;
			bCross = TRUE;

			xnowrectoffset = rectcross.left - rect.left;
			ynowrectoffset = rectcross.top - rect.top;
			InH = rectcross.bottom - rectcross.top;
			InW = rectcross.right - rectcross.left;
			xOffmask = rectcross.left - rectmask.left;
			yOffmask = rectcross.top - rectmask.top;
			xOffPic = rectcross.left - picrect.left;
			yOffPic = rectcross.top - picrect.top;
			// ��mask��Ϣ

			BYTE*& pMaskDataDec = pMap2->m_pMaskDataDec[i];

			int& align_width = pMap2->m_align_width[i];
			// ��mask����
			if (0 == align_width)
			{
				SMaskInfo maskInfo = { 0 };
				pMap2->m_File2.seekg(pMap2->m_pMaskDataOffset[i], ios::beg);
				pMap2->m_File2.read((char*)&maskInfo, sizeof(SMaskInfo));
				align_width = ((maskInfo.width >> 2) + (maskInfo.width % 4 != 0)) << 2;	// ��4����Ŀ��
				BYTE* pMaskData = 0;
				pMaskData = new BYTE[maskInfo.mask_size];
				pMap2->m_File2.read((char*)pMaskData, maskInfo.mask_size);
				// ����mask����
				pMaskDataDec = new BYTE[(align_width * maskInfo.height) >> 2];		// 1���ֽ�4�����أ���Ҫ����4
				pMap2->DecompressMask(pMaskData, pMaskDataDec);
				SAFE_DELETE_ARRAY(pMaskData);
			}
			//
			int mask_index;
			BYTE mask_value;
			for (int i2 = 0; i2 < InH; i2++)
			for (int j2 = 0; j2 < InW; j2++)
			{
				mask_index = ((i2 + yOffmask) * align_width + j2 + xOffmask) << 1;		// ��λ:λ
				if (mask_index < 0)
					ERRBOX;
				mask_value = pMaskDataDec[mask_index >> 3];	// ��λ���ֽ�
				mask_value = mask_value >> (mask_index % 8);	// ��λ��λ
				if ((mask_value & 3) == 3)
				{
					g_pMainState->m_MaskList[j2 + xnowrectoffset + ((i2 + ynowrectoffset) << 5)] = TRUE;
				}
			}
		}
	}
	WPixel pix1, pix2;
	int index, index2, index11, index22;
	BYTE*  lpSrc = lpDate[a];
	BYTE*  pAlpha = lpAlpha[a];

	int alpha;

	int indexx, indexy, unalpha;
	indexy = yOffset + m_pOwner->m_yOffset2;
	indexx = xOffset + m_pOwner->m_xOffset2;

	index11 = indexy*pitch + (indexx << 1);
	index = index11;
	index22 = (yOffset3 << 6) + (xOffset3 << 1);
	index2 = index22;

	BYTE* pA = &pAlpha[indexy*m_Width + indexx];
	BYTE* P1 = &lpSrc[indexy*pitch + (indexx << 1)];
	BYTE* P2 = &Pic[(yOffset3 << 6) + (xOffset3 << 1)];
	int j2;
	for (int i2 = 0; i2 < Height; i2++)
	{
		for (int j = 0; j < Width; j++)
		{
			j2 = j << 1;
			alpha = pA[j];
			if (!alpha)continue;

			pix1.l = P1[j2];
			pix1.h = P1[j2 + 1];

			if (m_pOwner->m_bTwinkle) //��˸  (��ջ���)
			{
				if (m_pOwner->now_frame % 3 == 0)
				{
					if (pix1.red > 24)
						pix1.red = 31;
					else
						pix1.red += 7;
					if (pix1.green > 49)
						pix1.green = 63;
					else
						pix1.green += 14;
					if (pix1.blue < 10)
						pix1.blue = 0;
					else
						pix1.blue -= 10;
				}
			}
			else
			if (m_pOwner->m_bShine)  //����  (��ɫ��ѡ��ʱ�����)
			{
				if (pix1.red>24)
					pix1.red = 31;
				else
					pix1.red += 7;
				if (pix1.green > 49)
					pix1.green = 63;
				else
					pix1.green += 14;
				if (pix1.blue > 24)
					pix1.blue = 31;
				else
					pix1.blue += 7;
			}
			if (m_pOwner->m_bShadow)
			{
				alpha = (alpha*(100 - m_pOwner->m_bShadow)) / 100; //��͸��
			}
			if (bCross)
			{
				if (g_pMainState->m_MaskList[j + xOffset3 + ((i2 + yOffset3) << 5)])
				{
					if (8 == m_MaxDirection)
					{
						alpha = alpha >> 1;
					}
					else  alpha = 0;
				}
			}
			if (alpha < 255)
			{
				pix2.l = P2[j2];
				pix2.h = P2[j2 + 1];

				unalpha = 255 - alpha;
				pix2.red = (pix1.red *alpha + unalpha* pix2.red) >> 8;
				pix2.green = (pix1.green *alpha + unalpha*pix2.green) >> 8;
				pix2.blue = (pix1.blue *alpha + unalpha*pix2.blue) >> 8;
				P2[j2] = pix2.l;
				P2[j2 + 1] = pix2.h;
			}
			else
			{
				P2[j2] = pix1.l;
				P2[j2 + 1] = pix1.h;
			}
		}
		pA += m_Width;
		P2 += 64;
		P1 += pitch;
	}
	return TRUE;
}



BOOL CSprite::CountCross(int xPos, int yPos, RECT destrect, RECT& picrectout, RECT& nowrectout, int& xOffPic, int& yOffPic, int& xOffDes, int& yOffDes)
{
	picrectout.left = (xPos > destrect.left ? xPos : destrect.left);
	picrectout.top = (yPos > destrect.top ? yPos : destrect.top);
	if (m_pOwner->m_ShowWidth)picrectout.right = xPos + m_pOwner->m_ShowWidth;
	else  picrectout.right = xPos + m_Width;
	if (m_pOwner->m_ShowHeight)picrectout.bottom = yPos + m_pOwner->m_ShowHeight;
	else  picrectout.bottom = yPos + m_Height;
	if (destrect.right < picrectout.right)picrectout.right = destrect.right;
	if (destrect.bottom < picrectout.bottom)picrectout.bottom = destrect.bottom;

	if (!CheckCross(picrectout, destrect, nowrectout))return FALSE;
	xOffPic = nowrectout.left - xPos;
	yOffPic = nowrectout.top - yPos;
	//ʵ���������Ŀ�������ƫ��
	xOffDes = nowrectout.left - destrect.left;
	yOffDes = nowrectout.top - destrect.top;
	return TRUE;
}

BOOL CSprite::LoadBMP(string path)
{
	FreeDatelp();
	m_MaxDirection = 1;
	m_Frame = 1;
	m_kx = 0;
	m_ky = 0;
	
	ifstream File;
	File.open(path, ios::binary);
	if (!File)
		ERRBOX;
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER m_BmpInfoHead;
	int m_nBitCount;

	File.read((char*)&bmfh, sizeof(BITMAPFILEHEADER));
	File.seekg(sizeof(BITMAPFILEHEADER));
	File.read((char*)&m_BmpInfoHead, sizeof(BITMAPINFOHEADER));
	m_Width = m_BmpInfoHead.biWidth;
	m_Height = m_BmpInfoHead.biHeight;
	m_nBitCount = m_BmpInfoHead.biBitCount;

	
	if (m_nBitCount <16)ERRBOX;
	pitch = ((m_Width <<1) + 7) & 0xfffffff8;
	int size = (m_Width*m_Height*m_nBitCount) >> 3;
	BYTE* data = new BYTE[size+m_Width];
	lpDate = new unsigned char*;
	lpAlpha = new unsigned char*;
	(lpDate[0]) = new BYTE[pitch*m_Height + 7];
	(lpAlpha[0]) = new BYTE[m_Width*m_Height];


	File.seekg(sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER));
	File.read((char*)data, size);
	
	memset(lpAlpha[0], 255, m_Width*m_Height);


	m_pDataOffsetList = new DATEHEARD;
	m_pDataOffsetList->FramX =0;
	m_pDataOffsetList->FramY= 0;
	m_pDataOffsetList->Height = m_Height;
	m_pDataOffsetList->Width = m_Width;
	WPixel pix;
	switch (m_nBitCount)
	{
	case 16:
		ERRBOX;
		break;
	case 24:
		if (1)
		{
			int index = 0;
			int index2 = (m_Height - 1)*pitch;
			for (int h = 0; h < m_Height; h++)
			{
				for (int w = 0; w < m_Width; w++)
				{
					pix.red = (data[index + 2] >> 3) & 0x1f;
					pix.green = ((data[index + 1] >> 3) & 0x1f) <<1;
					pix.blue = (data[index] >> 3) & 0x1f;
					(lpDate[0])[index2] = pix.l;
					(lpDate[0])[index2 + 1] = pix.h;
					index += 3;
					index2 += 2;
				}
				index2 -= pitch<< 1;
			}
		}
		break;
	default:
		ERRBOX;
		break;
	}
	SAFE_DELETE_ARRAY(data);
	m_bLoad = TRUE;
	return TRUE;
}






CPicture::CPicture(void)
{
	Spr.m_pOwner = this;
}

CPicture::~CPicture(void)
{
	Free();
}
// ��������
void CPicture::Load(void* pdate, int size, int type, sChangeColorData* pChangeColorData)
{
	Spr.Load(pdate, size, type, pChangeColorData);
	now_frame = 0;
	m_NextFrame = 0;
	m_bMove = TRUE;
	m_Width = Spr.GetWidth();
	m_Height = Spr.GetHeight();
	return;

}





void CPicture::Free()
{
	Spr.FreeDatelp();
	m_Height = 0;
	m_Width = 0;
};

CWdf* CWFile::m_pWdf[64];
BOOL CWdf::Open(const CHAR* FileName)
{
	m_File.open(FileName, ios::binary);
	if (!m_File)
	{
		MessageBoxA(g_hWnd,"û���ҵ��ļ�","",MB_OK);
		assert(0);
		return FALSE;
	}
	//��ȡ�ļ�ͷ
	WDataFileHeader header;
	m_File.read((char*)&header, sizeof(header));

	//ȷ����WDF�ļ�
	if (header.id != 'WDFP') {
		m_File.close();
		return 0;
	}
	//�������ļ��б�
	m_pIndex = new WDataFileIndex[header.number];
	m_File.seekg(header.offset, ios_base::beg);
	int size = sizeof(DWORD);
	for (int i = 0; i < header.number;i++)
	{
		m_File.read((char*)&m_pIndex[i].uid, size);
		m_File.read((char*)&m_pIndex[i].offset, size);
		m_File.read((char*)&m_pIndex[i].size, size);
		m_File.read((char*)&m_pIndex[i].space, size);
	}
	m_Number = header.number;
	return TRUE;
}

WDataFileIndex* CWdf::SearchFile(DWORD id)
{
	int begin, end, middle;
	begin = 0, end = m_Number - 1;
	//�����ҷ�,��̫������
	while (begin <= end)
	{
		middle = (begin + end)>>1;
		if (m_pIndex[middle].uid == id) return &m_pIndex[middle];
		else if (m_pIndex[middle].uid < id) begin = middle + 1;
		else end = middle - 1;
	}
	return 0;
}

BOOL CWdf::Free()
{
	SAFE_DELETE_ARRAY(m_pIndex);
	return TRUE;
}




BOOL CWFile::Open(const CHAR* FileName)
{
	if (m_pWdf[64] != 0)
	for (int i = 0; i < 64; i++)
		m_pWdf[i] = 0;
	int i;
	for (i = 0; i < 64; i++)
	if (m_pWdf[i] == 0)
		break;
	if (i == 64)
		return 0;

	m_pWdf[i] = new CWdf();
	m_pWdf[i]->Open(FileName);
	strcpy_s(m_pWdf[i]->m_Name, 256, FileName);

	return TRUE;
}
void CWFile::PlayWav()
{
	if (0 == g_pMainState->m_GlobalButton[17])
		m_Wav.m_pPrimaryBuffer->Play(0, 0, 0);
}
void CWFile::LoadWav(DWORD uid, int position,BOOL bPlay)
{
	if (g_pMainState->m_GlobalButton[17])return;
	//��ֹ�ظ�����
	if (!uid)
	{
		Free();
		return ;
	}
	
	if (m_NowID == uid)
	{
		if (m_Wav.m_pPrimaryBuffer)
		{
			m_Wav.m_pPrimaryBuffer->SetCurrentPosition(0);
			if (bPlay)m_Wav.m_pPrimaryBuffer->Play(0, 0, 0);
			return;
		}
	}
	m_NowID = uid;
	
	ifstream* f = 0;
	GetFileP(uid, f);

	//�����ļ�
	if (m_lpSize < m_Size)
	{
		SAFE_DELETE_ARRAY(lp);
		lp = new BYTE[m_Size];
		m_lpSize = m_Size;
	}
	f->read((char*)lp, m_Size);
	m_Wav.Load(lp, m_Size,position,bPlay);
}
BOOL CWFile::Load(DWORD uid, sChangeColorData* pChangeColorData)
{
	//��ֹ�ظ�����
	if (m_NowID == uid)return TRUE;
	m_NowID = uid;
	if (!uid)
	{
		Free();
		return TRUE;
	}
	ifstream* f=0;
	GetFileP(uid, f);
	//�����ļ�
	if(m_lpSize<m_Size)
	{
		SAFE_DELETE_ARRAY(lp);
		m_lpSize = m_Size;
		lp = new BYTE[m_Size];
	}
	f->read((char*)lp,m_Size);
	switch (m_Type)
	{
	case 1://was
		m_Picture.Load(lp, m_Size, m_Type, pChangeColorData);
		break;
	default:
		LoadWav(uid, 0);
		break;
	}
	
	return TRUE;
}





BOOL CWFile::Free()
{
	if (!m_NowID)return TRUE;
	m_NowID = 0;
	switch (m_Type)
	{
	case 1:
		m_Picture.Free();
		break;
	default:
		m_Wav.Free();
		break;
	}
	m_NeedShow = TRUE;
	return TRUE;
};

CWFile::CWFile()
{
	lp = 0;
	m_PreShowRect.right = -1;
	m_isStatic = FALSE;
	m_NowID = 0;
	m_bMove = FALSE;
}



BOOL CWFile::isSelectOn(int x, int y)
{
	int a = m_Picture.GetDataID();
	RECT  rect;
	GetPicRange(rect) ;
	if (isOn(x, y,rect))
	{
		int x1 = x - rect.left;
		int y1 = y - rect.top;
		//?
		if (0 != m_Picture.Spr.lpAlpha[a][x1 + y1*m_Picture.m_Width])
			return TRUE;
	}
	return FALSE;
}

BOOL CWFile::CloseWDF()
{
	for (int i = 0; i < 64; i++)
	{
		if (CWFile::m_pWdf[i])
		{
			CWFile::m_pWdf[i]->Free();
			SAFE_DELETE(CWFile::m_pWdf[i]);
		}
	}
	return TRUE;
}


void CWFile::GetPicRange(RECT& rect)
{
	if (!m_Picture.Spr.m_bLoad)
	{
		rect.right = -1;
		return;
	}
	int a = m_Picture.GetDataID();
	DATEHEARD& datehead = m_Picture.Spr.m_pDataOffsetList[a];
	rect.left = m_Picture.m_x - datehead.FramX;
	rect.top = m_Picture.m_y - datehead.FramY;

	if (0 == m_Picture.m_ShowWidth)
		rect.right = rect.left + datehead.Width;
	else
		rect.right = rect.left + GetShowWidth();
	if (0 == m_Picture.m_ShowHeight)
		rect.bottom = rect.top + datehead.Height;
	else
		rect.bottom = rect.top +   GetShowHeight();
}
RECT CWFile::GetPicRange()
{
	RECT rect;
	int a = m_Picture.GetDataID();
	rect.left = m_Picture.m_x - m_Picture.Spr.m_pDataOffsetList[a].FramX;
	rect.top = m_Picture.m_y - m_Picture.Spr.m_pDataOffsetList[a].FramY;

	if (0 == GetShowWidth())
		rect.right = rect.left + m_Picture.m_Width;
	else
		rect.right = rect.left + GetShowWidth();

	if (0 == GetShowHeight())
		rect.bottom = rect.top + m_Picture.m_Height;
	else
		rect.bottom = rect.top + GetShowHeight();
	return rect;
}
BOOL CWFile::NextFrame()
{
	if (m_isStatic)return FALSE;
	if (1 == m_Picture.m_FrameGap)return FALSE;
	if (m_Picture.m_FrameGap <= m_Picture.m_NextFrame)
	{
		m_Picture.m_NextFrame = 0;
		m_Picture.now_frame += 1;
		if (m_Picture.now_frame == m_Picture.Spr.GetMaxFrame())
		{
			if (m_bCircle)
			{
				m_Picture.now_frame = 0;
				if (m_ShowOnce)
					m_NeedShow = FALSE;
			}
			else
			{
				m_Picture.now_frame = (m_Picture.Spr.GetMaxFrame() - 1);
			}
		}
		return TRUE;
	}
	else
		m_Picture.m_NextFrame += 1;

	return FALSE;
}
BOOL CWFile::PreFrame()
{
	if (m_isStatic)return FALSE;
	if (0 == m_Picture.m_NextFrame)
	{
		m_Picture.m_NextFrame = m_Picture.m_FrameGap-1;
		m_Picture.now_frame -= 1;
		if (-1 == m_Picture.now_frame)
			m_Picture.now_frame = m_Picture.Spr.GetMaxFrame() - 1;
		return TRUE;
	}
	else
		m_Picture.m_NextFrame = 0;

	return FALSE;
}
void CWFile::SetDirection(int dir)
{
	if (m_Direction == dir)return;
	m_Direction = dir;
	m_Picture.m_direction = dir;
	m_bMove = TRUE;

}

BOOL CWFile::LoadBMP(string path)
{
	m_NowID = 100;
	m_Type = 6;
	if (m_Picture.Spr.LoadBMP(path))
	{
		m_Picture.m_Width = m_Picture.Spr.GetWidth();
		m_Picture.m_Height = m_Picture.Spr.GetHeight();
	}
	return TRUE;
}

BOOL CWFile::GetWillNextFrame()
{
	if (m_Picture.m_NextFrame == m_Picture.m_FrameGap - 1)
		return TRUE;
	return FALSE;
}

void CWFile::SetFrame(int frame)
{
	if (!GetMaxFrame())ERRBOX;

	if (m_Picture.GetFrame() != frame)
	{
		m_bMove = TRUE;
		if (m_Picture.Spr.GetMaxFrame() > frame)
		{
			m_Picture.m_NextFrame = 0;
			m_Picture.SetFrame(frame);
		}
		else
		m_Picture.SetFrame(0);
	}


}

void CWFile::GetFileP(DWORD uid, ifstream*& f)
{
	//���ҵ���Ӧ���ļ�
	WDataFileIndex * pf = 0;
	int i;
	for (i = 0; i < 64; i++)
	{
		if (m_pWdf[i] == 0)continue;
		pf = m_pWdf[i]->SearchFile(uid);
		if (pf)
		{
			f = m_pWdf[i]->GetFileHandle();
			m_Offset = pf->offset;
			m_Size = pf->size;
			WORD hdw;
			f->seekg(m_Offset, ios_base::beg);
			int dwsize = sizeof(WORD);
			f->read((char*)&hdw, dwsize);
			if (hdw == 'PS')m_Type = 1;
			else m_Type = 0;
			f->seekg(m_Offset, ios_base::beg);
			m_bMove = TRUE;
			return;
		}
	}
	assert(0);
}

void CWFile::Save(string name)
{
	name += ".was";
	ofstream oss;
	oss.open(name, ios::binary);
	if (!oss)
		assert(0);
	oss.write((char*)lp, m_Size);
	oss.close();
}

CWFile::~CWFile()
{
	SAFE_DELETE_ARRAY(lp);
}

//int CWFile::setfiletype(ExsDate esdate)
//{
//	if (esdate.hdw == 'PS') return 1;                             //was/tcp�ļ�
//	else if (esdate.hdw == 'MB') return 6;						   //bmp�ļ�
//	else if (esdate.sst == 'FIFJ') return 3;                      //jpg�ļ�
//	else if (esdate.dss == 'ELIF') return 4;                      //tga�ļ�
//	else if (esdate.hdw == 'IR' && esdate.nst == 'EVAW') return 5;  //wav�ļ�
//	else if ((esdate.dsg[0] == 0x11) && (esdate.dsg[1] == 0) && (esdate.dsg[2] == 0) && ((esdate.sss == 0x1000) || (esdate.sss == 0x0f00))) return 7;
//	else if ((esdate.hdw & 0x00ff) == 0x00ff) return 2;			   //mp3�ļ�
//	else return 0;
//	return 0;
//}



BOOL cObj::AddState(sBuff buf)
{
	//�ظ�ʱ����״̬
	if (m_StateList.size())
	for (list<sBuff>::iterator it = m_StateList.begin(); it != m_StateList.end(); it++)
	{
		if (it->ID == buf.ID)
		{
			it->times = buf.times;
			return TRUE;
		}
	}
	//
	if (m_StateList.size() == 5)
	{
		list<sBuff>::iterator it = m_StateList.begin();
		g_pMainState->m_SkillManager.SetDateBuffDown(it);// it->targetID, it->ID);
		m_StateList.pop_front();
	}
	m_StateList.push_back(buf);
	list<sBuff>::iterator it;
	if (m_StateList.size() == 1)
	{
		it = m_StateList.begin();
	}
	else
	{
		it = m_StateList.end();
		it--;
	}
	it->data.Load(buf.MagicID);
	it->data.m_NeedShow = TRUE;
	it->data.m_Picture.m_FrameGap = 5;
	MoveMagicSuffer();
	return TRUE;
}
BOOL cObj::ReMoveStateID(int id)
{
	for (list<sBuff>::iterator Iterator = m_StateList.begin(); Iterator != m_StateList.end();)
	{
		if (Iterator->ID == id)
		{
			list<sBuff>::iterator it2 = Iterator;
			Iterator++;
			g_pMainState->m_SkillManager.SetDateBuffDown(it2);// it2->targetID, it2->ID);
			it2->data.Free();
			m_StateList.erase(it2);
			return TRUE;
		}
		else Iterator++;
	}
	return FALSE;
}
BOOL cObj::ReMoveState(eBuffType Type)
{
	for (list<sBuff>::iterator Iterator = m_StateList.begin(); Iterator != m_StateList.end();)
	{
		if (Iterator->EffectType == Type)
		{
			list<sBuff>::iterator it2 = Iterator;
			Iterator++;
			g_pMainState->m_SkillManager.SetDateBuffDown(it2);//it2->targetID, it2->ID);
			it2->data.Free();
			m_StateList.erase(it2);
			continue;
		}
		else
		Iterator++;
	}
	return TRUE;
}
void cObj::ClearState()
{
	//BUFF��ʾ
	for (list<sBuff>::iterator it = m_StateList.begin(); it != m_StateList.end(); it++)
	{
		g_pMainState->m_SkillManager.SetDateBuffDown(it);// id, it->ID);
		it->data.Free();
	}
	m_StateList.clear();
}
cObj::cObj()
{
	m_MagicSuffer.m_FrameGap = 6;
	m_MagicSuffer.m_NeedShow = FALSE;
	m_bCheckWall = FALSE;
	m_Shadow.m_bStatic = TRUE;
	m_ShowTime = 0;
	m_bShake = FALSE;
	m_AutoRun = FALSE;
	m_pIndex = 0;
	m_IndexType = 0;
	m_NowPos = -1;
	m_PrePos = -1;
	m_time = -2;
	m_speed = 2;//3;
	for (int i = 0; i < 6; i++)
	{
		m_ContextList.m_TextLineList[i].m_line.SetColor(RGB(255, 255, 255));
		m_ContextList.m_TextLineList[i].m_line.m_hFont = 0;
	}
	for (int i = 0; i < 8; i++)
	{
		m_data[i].m_Picture.m_NextFrame = 0;
		m_data[i].m_Picture.now_frame = 0;
		m_data[i].m_NeedShow = FALSE;
	}
	m_TrueName.SetStyle(1);
	m_TrueName.m_bShowOnScreen = FALSE;
	for (int i = 0; i < 4; i++)
	{
		m_pShowData[i] = &m_data[i];
	}
}



void cObj::Free()
{
	for (int i = 0; i < 8; i++)
	{
		m_data[i].Free();
	}
	m_PrePos = -1;
	m_NowPos = -1;
	FreeIndex();
	m_Script.Free();
	m_Path.clear();
	m_ScriptSelect = 0;
	SetShake(FALSE);//
}

cObj::~cObj()
{
	Free();
}

BOOL cObj::Show(cCanvas* pCanvas)
{
	if (!m_pIndex)return TRUE;
	for (int i = 1; i < 4; i++)
	{
		m_pShowData[i]->m_Picture.now_frame = m_pShowData[0]->m_Picture.now_frame;
		m_pShowData[i]->m_Picture.m_NextFrame = m_pShowData[0]->m_Picture.m_NextFrame;
		m_pShowData[i]->m_bMove = m_pShowData[0]->m_Picture.m_bMove;
	}
	for (int i = 0; i < 4; i++)
	{
		if (m_pShowData[i]->m_NeedShow)
		if (m_pShowData[i]->m_NowID)
		{
			m_pShowData[i]->m_Picture.Spr.m_Bottom = GetY(); 
			pCanvas->Add(m_pShowData[i], 0);
		}

	}
	if (m_MagicEffect.m_pFile)
	{
		pCanvas->Add(&m_MagicEffect, 0);	
		if (m_MagicEffect.now_frame == m_MagicEffect.m_pFile->GetMaxFrame() - 1)
		{
			m_MagicEffect.m_pFile = 0;
		}
	}
	//��ʾ����			
	if (m_bNeedShowName)
	{
		g_pMainState->m_Canvas.Add(&m_TrueName);
	}
	//��ʾ�Ի�
	if (m_ShowTime > 0)
	{
		if (g_pMainState->m_Map.m_pMap->m_bMove)
		{
			MoveDialog();
		}
		ShowFaceText(pCanvas);
	}
	return TRUE;
}
BOOL cObj::ShowFaceText(cCanvas* pCanvas)
{
	//ʱ������
	if (1 > m_ShowTime)
		return TRUE;
	m_ShowTime -= 1;
	if (1 > m_ShowTime)
	{
		if (!g_pMainState->GetLockMouse())
		{
			m_ShowTime += 10;
		}
		else
		{
			if (g_pMainState->m_TriggerDialog.m_datalist[1].m_Nowtext > -1)
				g_pMainState->m_TriggerDialog.Process(1);
			return TRUE;
		}
	}
	////��ʾ����
	for (int i = 0; i < 2; i++)
		pCanvas->Add(&m_FaceTextBackp[i], 1);
	//��ʾ�ı�	
	m_ContextList.Show(pCanvas);

	return TRUE;
}


BOOL cObj::ShowOnScreen(cCanvas* pCanvas)
{
	if (!m_pIndex)return TRUE;

	for (list<sBuff>::iterator it = m_StateList.begin(); it != m_StateList.end(); it++)
	{
		if (it->MagicID)
		if (it->bBuffup == FALSE)
		{
			pCanvas->Add(&it->data, 1);
		}
	}
	if (!m_bMagicSufferUp)
	if (m_MagicSuffer.m_NeedShow)
	{
		pCanvas->Add(&m_MagicSuffer, 1);
		if (m_MagicSuffer.now_frame == m_MagicSuffer.m_pFile->GetMaxFrame() - 2)
		{
			m_MagicSuffer.m_NeedShow = FALSE;
		}
	}

	Shake();
	for (int i = 1; i < 4; i++)
	{	
		m_pShowData[i]->m_Picture.now_frame =   m_pShowData[0]->m_Picture.now_frame;
		m_pShowData[i]->m_Picture.m_NextFrame = m_pShowData[0]->m_Picture.m_NextFrame;
		m_pShowData[i]->m_Picture.m_bMove = m_pShowData[0]->m_Picture.m_bMove;
	}
	for (int i = 0; i <4 ; i++)
	{
		if (m_pShowData[i]->m_NeedShow)
		if (m_pShowData[i]->m_NowID)
		{
			if (m_bGhost)
			{
				int time1 = g_pMainState->m_InterfaceDoor.m_TimeShiCheng.ss;
				time1 = time1 % 8;
				if (0 == time1)
				{
					for (int i1 = 0; i1 < 4; i1++)
					{
						m_Ghost[i1][1].SetXY(GetX(), GetY());
					}
				}
				else
				if (4 == time1)
				{
					for (int i1 = 0; i1 < 4; i1++)
					{
						m_Ghost[i1][0].SetXY(GetX(), GetY());
					}
				}

				for (int j = 0; j < 2; j++)
				{
					pCanvas->Add(&m_Ghost[i][j], 1);
				}
			}
			pCanvas->Add(m_pShowData[i], 1);
		}

	}
	
	if (m_MagicEffect.m_pFile)
	{
		pCanvas->Add(&m_MagicEffect, 1);
		if (m_MagicEffect.now_frame == m_MagicEffect.m_pFile->m_Picture.Spr.GetMaxFrame() - 1)
			m_MagicEffect.m_pFile = 0;
	}
	//��ʾ����			
	if (m_bNeedShowName)
	{
		g_pMainState->m_Canvas.Add(&m_TrueName);
	}


	//BUFF��ʾ
	if (m_StateList.size())
	for (list<sBuff>::iterator it = m_StateList.begin(); it != m_StateList.end(); it++)
	{
		if (it->MagicID)
		if (it->bBuffup == TRUE)
		{
			pCanvas->Add(&it->data, 1);
		}
	}


	if (m_bMagicSufferUp)
	if (m_MagicSuffer.m_NeedShow)
	{
		pCanvas->Add(&m_MagicSuffer, 1);
		if (m_MagicSuffer.GetFrame() == m_MagicSuffer.m_pFile->GetMaxFrame() - 2)
		{
			m_MagicSuffer.m_NeedShow = FALSE;
		}
	}
	//��ʾ�Ի�
	if (m_ShowTime > 0)
	{
		ShowFaceText(pCanvas);
	}
	return TRUE;
}

void cObj::SufferBack(float speed ,int sufferDirection)
{
//	float jj = speed / 2;
	if ( sufferDirection== 1)
	{
		m_yPos += speed/2;
		m_xPos -= speed;
	}
	else
	if (sufferDirection == 3)
	{
		m_yPos += speed/2;
		m_xPos += speed;
	}
	else
	if (sufferDirection == 7)
	{
		m_yPos -= speed/2;
		m_xPos -= speed;
	}
	else
	if (sufferDirection == 9)
	{
		m_yPos -= speed/2;
		m_xPos += speed;
	}
	else
	{
		speed = (speed)*1.5f;
		if (sufferDirection == 8)
			m_yPos -= speed;
		else
		if (sufferDirection == 2)
			m_yPos += speed;
		else
		if (sufferDirection == 4)
			m_xPos -= speed;
		else
		if (sufferDirection == 6)
			m_xPos += speed;
	}
	if (m_bNeedShowName)
	{
		MoveName();
	}
	m_MagicEffect.SetXY(GetX(), GetY());
	m_MagicEffect.m_bMove = TRUE;
	for (int i = 0; i < 3; i++)
	{
		m_pShowData[i]->SetXY((int)m_xPos,(int)m_yPos);
	}
	m_Shadow.SetXY((int)m_xPos, m_Shadow.GetY() + ((int)m_xPos - m_Shadow.GetX()) / 2);
}

BOOL cObj::Set(int POS_X, BOOL bCircle)
{

	if (m_NowPos == POS_X)return TRUE;

	m_NowShowData = !m_NowShowData;
	for (int i = 0; i < 4; i++)
	{
		m_pShowData[i] = &m_data[4 * m_NowShowData + i];
		m_pShowData[i]-> SetXY((int)m_xPos, (int)m_yPos);
		m_pShowData[i]->m_bCircle = bCircle;
	}
	if (m_PrePos == POS_X)
	{
		swap(m_NowPos,m_PrePos);
	}
	else
	{
		m_PrePos = m_NowPos;
		m_NowPos = POS_X;
	}
	vector<DWORD> data;
	GetIndex(POS_X,data);
	
	sChangeColorData* pranse[4];
	for (int i = 0; i < 4; i++)
		pranse[i] = 0;
	switch (m_IndexType)
	{
	case  INDEXTYEP_CHARACTER:
		if (1)
		{
			cCharacter* pc = (cCharacter*)m_pIndex;
			pc->GetRanse(POS_X, pranse);
		}
		break;
	case INDEXTYEP_PET:
		if (1)
		{
			cPet* pc = (cPet*)m_pIndex;
			pc->GetRanse(pranse);
		}
		break;
	default:
		break;
	}
	for (int i = 0; i < 8; i++)
	{
		m_data[i].m_NeedShow = FALSE;
	}
	for (int i = 0; i < data.size(); i++)
	{
		if (0 == data[i])continue;
		m_pShowData[i]->m_NeedShow = TRUE;
		m_pShowData[i]->Load(data[i], pranse[i]);
		if (m_pShowData[0]->GetDirectionNum() == 4)
		{
			if ((m_direction % 2) == 0)
				SetDirection(Direction8To4(m_direction));
		}
	}
	//����ԽС,Խ��
	switch (POS_X)
	{
	case  POS_WALK:
		if (m_pShowData[0]->GetDirectionNum() == 8)
		{
			SetSpeed(4);
		}
		else
			SetSpeed(6);
		break;
	case POS_ATTACK:
	case POS_ATTACK2:
		SetSpeed(5);
		break;
	case POS_MAGIC:
		SetSpeed(4);
		break;
	case POS_FIGHT_STAND:
		SetSpeed(6);
		break;
	case POS_RETURN:
	case POS_RUN:
		SetSpeed(2);
		break;
	default:
		SetSpeed(8);
		break;
	}
	if (m_bGhost)
	for (int i = 0; i < 4; i++)
	{
		m_Ghost[i][0].Set(m_pShowData[i]);
		m_Ghost[i][1].Set(m_pShowData[i]);
	}
	Move2();
	return TRUE;
}

BOOL cObj::Move()
{
	if (-2==m_time)return FALSE;
	float   oldx = m_xPos;
	float   oldy =m_yPos;
	int xDis = abs((int)(m_xPos - m_xTarget));
	int yDis = abs((int)(m_yPos - m_yTarget));
	//�����������Ŀ��,ֹͣ
	if (xDis < m_speed&&yDis < m_speed)
	{
		 if (m_time==-1)
		 {
			 Stand();
		 }
		 else
		 if (m_time == -3)
		 {
			 Stand();
		 }
		return FALSE;   //�Ѿ���Ŀ�ĵ���
	}
	int Direction=-1;
	if (4 == m_pShowData[0]->GetDirectionNum())
		Direction = GetDirection4((int)m_xPos, (int)m_yPos, (int)m_xTarget, (int)m_yTarget);
	else
		Direction = GetDirection8Ex(m_direction,(int)m_xPos, (int)m_yPos, (int)m_xTarget, (int)m_yTarget);
	SetDirection(Direction);

	CoutPoint((float)m_speed, m_xPos, m_yPos, m_xTarget, m_yTarget, m_xPos, m_yPos);

	//���ǰ�����ϰ�,�ж�
	if (m_bCheckWall)
	if (m_time<0)
	if (g_pMainState->m_Map.m_pBmp->isOK((int)m_xPos, (int)m_yPos))
	{
		Set(POS_STAND);
		m_time = -2;
		m_xPos = oldx;
		m_yPos = oldy;
		return FALSE;
	}
	Set(POS_WALK);
	for (int i = 0; i < 4; i++)
	{
		m_pShowData[i]->m_Picture.m_x = (int)m_xPos;
		m_pShowData[i]->m_Picture.m_y = (int)m_yPos;
		m_pShowData[i]->m_bMove = TRUE;
	}
	MoveShadow();
	if (m_MagicEffect.m_NeedShow)
	{
		m_MagicEffect.m_x =(int)m_xPos;
		m_MagicEffect.m_y = (int)m_yPos;
		m_MagicEffect.m_bMove = TRUE;
	}
	//�ƶ��Ի�
	if (m_ShowTime > 0)
	{
		MoveDialog();
	}

	//�ƶ�����
	if (m_bNeedShowName)
	{
		MoveName();
	}
	MoveMagicSuffer();
	
	if (m_MagicSuffer.m_NeedShow)
	{
		MoveMagicSufferID();
	}
	return TRUE;
}

void cObj::Stand()
{
	m_xTarget = m_xPos;
	m_yTarget = m_yPos;
	Set(POS_STAND);
	m_time = -2;
}

void cObj::SetShine(BOOL needshine)
{
	for (int i = 0; i < 8; i++)
		m_data[i].SetShine(needshine);
}

BOOL cObj::isPointOn(int x, int y)
{
	if (!m_pIndex)return FALSE;

	RECT rect;
	for (int i = 0; i < 4; i++)
	{
		if (m_pShowData[i]->m_NowID)
		{
			CPicture& pic = m_pShowData[i]->m_Picture;
			int a = pic.GetDataID();
			rect.left = pic.m_x - pic.Spr.m_pDataOffsetList[a].FramX;
			rect.top = pic.m_y - pic.Spr.m_pDataOffsetList[a].FramY;
			rect.bottom = rect.top + pic.m_Height;
			rect.right = rect.left + pic.m_Width;
			if (isOn(x, y, rect))
			{
				int xOffset = x - rect.left;
				int yOffset = y - rect.top;
				if (0 != pic.Spr.lpAlpha[a][xOffset + yOffset*pic.m_Width])
					return TRUE;
			}
		}
	}
	return FALSE;
}

void cObj::GetIndex(int POS_X, vector<DWORD>& out)
{
	if (!m_pIndex)return;
	if (m_IndexType == INDEXTYEP_CHARACTER)
	{
		cCharacter* p = (cCharacter*)m_pIndex;
		p->GetID(POS_X, out);
		return;
	}
	if (m_IndexType == INDEXTYEP_NPC)
	{
		cNpc* p = (cNpc*)m_pIndex;
		p->GetID(POS_X, out);
		return;
	}
	if (m_IndexType == INDEXTYEP_PET)
	{
		cPet* p = (cPet*)m_pIndex;
		p->GetID(POS_X, out);
		return;
	}
	
}

DWORD cObj::GetIndex(int POS_X)
{
	vector<DWORD> out;
	GetIndex( POS_X, out);
	if (out.size())
	return out[0];
	return 0;
}

void cObj::SetDataType(int Type)
{	
	FreeIndex();
	m_IndexType = Type;
	switch (Type)
	{
	case 100:
		if (1)
		{
			cCharacter* p = new cCharacter;
			m_pIndex = p;
			p->m_PCData.m_IDinFightList = m_IDInFightlist;
		}
		break;
	case 101:
		if (1)
		{
			cPet* p = new cPet;
			m_pIndex = p;
			p->m_PetDataZZ.m_IDinFightList = m_IDInFightlist;
		}
		break;
	case 102:
		if (1)
		{
			cNpc* p = new cNpc;
			m_pIndex = p;
		}
		break;
	default:
		ERRBOX;
		break;
	}
}

void cObj::FreeIndex()
{
	switch (m_IndexType)
	{
	case INDEXTYEP_CHARACTER:
		if (1)
		{
			cCharacter* p = (cCharacter*)m_pIndex;
			if (!p)return;
			p->Free();
			SAFE_DELETE(p);
			m_pIndex = 0;
		}
		break;
	case INDEXTYEP_PET:
		if (1)
		{
			cPet* p = (cPet*)m_pIndex;
			SAFE_DELETE(p);
			m_pIndex = 0;
		}
		break;
	case INDEXTYEP_NPC:
		if (1)
		{
			cNpc* p = (cNpc*)m_pIndex;
			SAFE_DELETE(p);
			m_pIndex = 0;
		}
		break;
	default:
		break;
	}
	m_IndexType = 0;
}



BOOL cObj::SetMagicEffect(CWFile* MagicEffect, int Pos, int Speed)
{
	if (!MagicEffect)return FALSE;
	m_MagicEffect.Set(MagicEffect);
	m_MagicEffect.m_NeedShow = TRUE;

	m_MagicEffect.m_FrameGap = Speed;


	m_MagicEffect.SetX(GetX());
	if (Pos == 10)
	{
		m_MagicEffect.SetY(GetTopY());
	}
	else
	if (Pos == -10)
	{
		m_MagicEffect.SetY(GetY());
	}
	else
	{
		m_MagicEffect.SetY((GetY() + GetTopY()) / 2 - 30 * m_MagicSufferPos);
	}

	return TRUE;
}



BOOL cObj::CanEquip(int Equiptype, int lv, int Pos)
{
	
	if (m_IndexType == INDEXTYEP_CHARACTER)
	{
		cCharacter* pC = (cCharacter*)m_pIndex;
		return pC->CanEquip(Equiptype, lv,Pos);
	}
	else
	{
		//��ûд
		ERRBOX;
	}
	return FALSE;
}

sFightOffset* cObj::GetFightOffset()
{
	switch (m_IndexType)
	{
	case INDEXTYEP_CHARACTER:
		if (1)
		{
			cCharacter* p = (cCharacter*)m_pIndex;
			return p->GetFightOffset();
		}
	case INDEXTYEP_PET:
		if (1)
		{
			cPet* p = (cPet*)m_pIndex;
			return p->GetFightOffset();
		}
	default:
		break;
	}
	return 0;
}

BOOL cObj::Shake()
{
	if (!m_bShake)return TRUE;

	if (m_bShaketime<6)
		SetX(GetX() - 1);
	else
		SetX(GetX() + 1);
	
	m_bShaketime += 1;
	m_bShaketime %= 12;
	for (int i = 0; i < 8; i++)
	{
		m_data[i].m_bMove = TRUE;
	}
	return TRUE;
}

void cObj::SetDirection(int direction)
{
	m_direction = direction;
		for (int i = 0; i < 8; i++)
			m_data[i].SetDirection(direction);
	
}

BOOL cObj::SetShadow(int iShadow)
{
	for (int i = 0; i < 8; i++)
	{
		m_data[i].m_Picture.m_bShadow = iShadow;
	}
	return TRUE;
}

BOOL cObj::SetTwinkle(BOOL b)
{
	for (int i = 0; i < 8; i++)
	{
		m_data[i].m_Picture.m_bTwinkle = b;
	}
	return TRUE;
}

INT64* cObj::GetExp()
{
	cPetData* p;
	GetDateP(p);
	if (p)return &p->EXP;
	ERRBOX;
	return 0;
}

INT64* cObj::GetExpMax()
{
	cPetData* p;
	GetDateP(p);
	if (p)return &p->ExpMax;
	ERRBOX;
	return 0;
}

BOOL cObj::SetGhost(BOOL b)
{
	m_bGhost = b;
	if (b)
	{
		for (int i = 0; i < 3; i++)
		{
			m_Ghost[i][0].Set(m_pShowData[i]);
			m_Ghost[i][1].Set(m_pShowData[i]);
		}

		for (int j = 0; j < 3;j++)
		for (int i = 0; i < 2; i++)
		{
			m_Ghost[j][i].m_x =(int)m_xPos;
			m_Ghost[j][i].m_y = (int)m_yPos;
			m_Ghost[j][i].m_direction = m_direction;
		}
	}
	return TRUE;
}

std::string cObj::GetAutoName()
{
	switch (m_IndexType)
	{
	case INDEXTYEP_PET:
		if (1)
		{
			cPet* p = (cPet*)m_pIndex;
			return p->GetData()->m_Name;
		}
		break;
	case INDEXTYEP_CHARACTER:
		if (1)
		{
			cCharacter* p = (cCharacter*)m_pIndex;
			return p->GetData()->m_Name;
		}
		break;
	default:
		ERRBOX;
		break;
	}
	return 0;
}
int cObj::GetModeID()
{
	switch (m_IndexType)
	{
	case INDEXTYEP_PET:
		if (1)
		{
			cPet* p = (cPet*)m_pIndex;
			return p->GetData()->m_ID;
		}
		break;
	case INDEXTYEP_CHARACTER:
		if (1)
		{
			cCharacter* p = (cCharacter*)m_pIndex;
			return p->GetData()->m_ID+2000;
		}
		break;
	case INDEXTYEP_NPC:
		if (1)
		{
			cNpc* p = (cNpc*)m_pIndex;
			return p->m_pNpcData->m_ModeId;
		}
		break;
	default:
		ERRBOX;
		break;
	}
	return 0;
}

BOOL cObj::AddZZ(int num, PropertyType typpe)
{
	cPetData* pData=0;

	switch (m_IndexType)
	{
	case INDEXTYEP_CHARACTER:
		if (1)
		{
			cCharacter* p = (cCharacter*)m_pIndex;
			pData = &p->m_PCData;
		}
		break;
	case INDEXTYEP_PET:
		if (1)
		{
			cPet* p = (cPet*)m_pIndex;
			pData = &p->m_PetDataZZ;
		}
		break;
	default:
		ERRBOX;
		return FALSE;
	}
	switch (typpe)
	{
	case	typeAtkZZ:pData->AddAtkZZ(num); break;
	case	typeDefZZ:pData->AddDefZZ(num); break;
	case	typeHPZZ:pData->AddHpZZ(num); break;
	case	typeMagZZ:pData->AddMagZZ(num); break;
	case	typeSpdZZ:pData->AddSpdZZ(num); break;
	case	typeAvoidZZ:pData->AddAvoidZZ(num); break;
	}
	return TRUE;
}

void cObj::SetX(int x)
{
	if (((int)m_xPos) == x)return;
	m_xPos = (float)x;
	for (int i = 0; i < 8; i++)
	{
		m_data[i].m_Picture.m_x = x;
		m_data[i].m_bMove = TRUE;
	}
	m_MagicEffect.m_x = (int)m_xPos;
	m_MagicEffect.m_bMove = TRUE;
	

	if (m_bNeedShowName)
	{
		MoveName();
	}

}
void cObj::SetY(int y)
{
	if ((int)m_yPos == y)return;
	if (!g_pMainState)
	{
		ERRBOX;
		return;
	}
	m_yPos = (float)y;
	for (int i = 0; i < 8; i++)
	{
		m_data[i].m_Picture.m_y = y;
		m_data[i].m_bMove = TRUE;
	}
	
	m_MagicEffect.m_y =  (int)m_yPos;
	m_MagicEffect.m_bMove = TRUE;
	if (m_bNeedShowName)
	{
		//��ʾ����			
		MoveName();
	}
}

void cObj::SetPos(float x, float y)
{	
	if (!g_pMainState)
	{
		MessageBoxA(0, "ERR", "SetX", MB_OK);
		return;
	}
	if (m_xPos == x&&m_yPos == y)return;
	m_xPos = x;
	m_yPos = y;
	for (int i = 0; i < 8; i++)
	{
		m_data[i].m_Picture.m_x = (int)x;
		m_data[i].m_Picture.m_y = (int)y;
		m_data[i].m_bMove = TRUE;
	}
	m_MagicEffect.m_x = (int)m_xPos;
	m_MagicEffect.m_y = (int)m_yPos;
	m_MagicEffect.m_bMove = TRUE;
	MoveShadow();
	if (m_bNeedShowName)
	{
		//��ʾ����			
		MoveName();
	}

}

int cObj::GetTopY()
{
	int a = m_pShowData[0]->m_Picture.GetDataID();
	return  GetY() - m_pShowData[0]->m_Picture.Spr.m_pDataOffsetList[a].FramY;
	/*int y = GetY();
	int y1;
	for (int i = 0; i < 4; i++)
	{
		if (0 == m_pShowData[i]->m_NeedShow)continue;
		int a = m_pShowData[i]->m_Picture.GetDataID();
		y1 = GetY() - m_pShowData[i]->m_Picture.Spr.m_pDataOffsetList[a].FramY;
		if (y>y1)
			y = y1;
	}
	return y;*/
}
int cObj::GetTopY2()
{
	return GetY() - 80;
}
void cObj::Init()
{
 m_TaskTrigger.m_Stage = -1;
 m_Shadow.Set(&g_pMainState->m_Shadow); 
 m_FaceTextBackp.resize(2);
 for (int i = 0; i < 2;i++)
 {
	 m_FaceTextBackp[i].Set(&g_pMainState->m_FaceTextBack);
	 m_FaceTextBackp[i].m_bShadow = 60;
 };
 m_HpLinep.Set(&g_pMainState->m_HpLine);
 m_HpEmptyLinep.Set(&g_pMainState->m_HpEmptyLine);
 m_HpLinep.m_bStatic = TRUE;
 m_HpEmptyLinep.m_bStatic = TRUE;
 m_TrueName.m_hFont = g_pMainState->m_hFont[0];
 m_bGhost = FALSE;
 for (int i = 0; i < 3; i++)
 {
	 m_Ghost[i][0].m_bStatic = TRUE;
	 m_Ghost[i][1].m_bStatic = TRUE;
	 m_Ghost[i][0].m_bShadow=60;
	 m_Ghost[i][1].m_bShadow = 60;
 }
}

void cObj::MoveDialog()
{
	if (0 == m_ShowTime)return;
	int xscr = m_Shadow.GetX() - g_pMainState->m_FaceTextBack.GetWidth() / 2;
	int top = GetTopY2();
	int ystart =GetTopY2() - m_ContextList.m_Height;
	if (g_StateType == STATE_MAIN)
	{
		xscr -= g_pMainState->m_Map.m_pMap->GetXCenter()-320;
		ystart -= g_pMainState->m_Map.m_pMap->GetYCenter() - 240;
	}
	for (int i = 0; i < 2; i++)
		m_FaceTextBackp[i].SetX(xscr);


	int maxheitht = m_FaceTextBackp[0].m_pFile->GetHeight();
	m_ContextList.SetPos(xscr+3, ystart);
	int ystartback = ystart; //��ߵ�
	int HeightContext = m_ContextList.m_Height + 5;

	m_FaceTextBackp[0].SetyOffset(maxheitht - HeightContext/2);
	if (m_FaceTextBackp[0].GetyOffset() < 30)m_FaceTextBackp[0].SetyOffset(30);
	m_FaceTextBackp[0].SetY(ystartback);

	m_FaceTextBackp[1].SetyOffset2(maxheitht-( HeightContext - m_FaceTextBackp[0].GetShowHeight()));
	m_FaceTextBackp[1].SetyOffset(m_FaceTextBackp[1].GetyOffset2());

	m_FaceTextBackp[1].SetY(ystartback + m_FaceTextBackp[0].GetShowHeight());
	m_FaceTextBackp[0].m_bMove = TRUE;
	m_FaceTextBackp[1].m_bMove = TRUE;
}

void cObj::MoveName()
{
	//��ʾ����			
	int l = GetX() - m_TrueName.m_Width / 2;
	int t = GetY() + 15;
	if (l == m_TrueName.GetX() && t == m_TrueName.GetY())return;
	m_TrueName.SetXY(l, t);
	m_TrueName.m_bMove = TRUE;
}

void cObj::MoveHpLine()
{
	if (!m_HpEmptyLinep.m_pFile)return;
	m_HpEmptyLinep.m_x = GetX() - m_HpEmptyLinep.m_pFile->GetWidth() / 2;//m_HpEmptyLinep.m_pFile->GetWidth()/2;
	m_HpEmptyLinep.m_y = GetTopY()-8;
	m_HpLinep.m_x = m_HpEmptyLinep.m_x+1;
	if (m_HpEmptyLinep.m_y < 0)	m_HpEmptyLinep.m_y = 0;
	m_HpLinep.m_y = m_HpEmptyLinep.m_y+1;

	m_HpEmptyLinep.m_bMove = TRUE;
}

void cObj::UpdateHpLine()
{
	cPetData* pTargetdata;
	GetDateP(pTargetdata);
	int percent;
	int hpMax = pTargetdata->m_HPMax.GetData();
	int& hpnow = pTargetdata->m_HP;
	if (hpMax)
		percent = (hpnow * 36) / hpMax;
	else
		percent = 0;
	if (m_HpLinep.GetxOffset() != 36 - percent)
	{
		if (0 == percent)percent = 1;
		m_HpLinep.SetxOffset(36 - percent);
		m_HpLinep.m_bMove = TRUE;
	}
}

void cObj::GetDateP(cPetData*& p)
{
	switch (m_IndexType)
	{
	case INDEXTYEP_CHARACTER:
		if (1)
		{
			cCharacter* ppc = (cCharacter*)m_pIndex;
			p = &ppc->m_PCData;
		}
		break;
	case INDEXTYEP_PET:
		if (1)
		{
			cPet* ppet = (cPet*)m_pIndex;
			p = &ppet->m_PetDataZZ;
		}
		break;
	default:
		p = 0;
		break;
	}
}



void cObj::SetSound(int POS_X,BOOL bPlay)
{
	DWORD id = GetSound(POS_X);
	if (!id)return;
	switch (POS_X)
	{
	case POS_SUFFER:
	case POS_SOUNDSUFFER:
		m_WavSuffer.LoadWav(id, 0, bPlay);
		break;
	case POS_ATTACK:
	case POS_SOUNDATTACK:
		m_WavAtk.LoadWav(id, 0, bPlay);
		break;
	case POS_ATTACK2:
	case POS_SOUNDATTACK2:
		m_WavAtk2.LoadWav(id, 0, bPlay);
		break;
	case POS_DEF:
	case POS_SOUNDDEF:
		m_WavDef.LoadWav(id, 0, bPlay);
		break;
	default:
		m_Wav.LoadWav(id,0,bPlay);
		break;
	}
	

}
void cObj::PlayWav(int POS_X)
{
	switch (POS_X)
	{
	case POS_SUFFER:
		m_WavSuffer.PlayWav();
		break;
	case POS_ATTACK:
		m_WavAtk.PlayWav();
		break;
	case POS_ATTACK2:
		m_WavAtk2.PlayWav();
		break;
	case POS_DEF:
		m_WavDef.PlayWav();
		break;
	default:
		m_Wav.PlayWav();
		break;
	}
}

void cObj::SetMagicSound(int SkillID, int position)
{
	if (SkillID == -1)
	{
		m_MagicWav.Free();
		return;
	}
	sSkill* pSkill = g_pMainState->m_SkillManager.GetSkill(SkillID);
	if (!pSkill)return;
	DWORD id= pSkill->m_Sound;
	if (id)
	{
		m_MagicWav.LoadWav(id,position);
	}
}

DWORD cObj::GetSound(int POS)
{
	int  soundpos=POS;
	switch (POS)
	{
	case POS_ATTACK:soundpos = POS_SOUNDATTACK; break;
	case POS_DEAD:	soundpos = POS_SOUNDDEAD;		break;
	case POS_DEF:	soundpos = POS_SOUNDDEF;			break;
	case POS_ATTACK2:soundpos = POS_SOUNDATTACK2;	break;
	case POS_MAGIC:	soundpos = POS_SOUNDMAGIC;		break;
	case POS_SUFFER:soundpos = POS_SOUNDSUFFER; break;
	case POS_SOUNDATTACK:
	case POS_SOUNDDEAD:
	case POS_SOUNDDEF:
	case POS_SOUNDATTACK2:
	case POS_SOUNDMAGIC:
	case POS_SOUNDSUFFER:
		break;
	default:	
		return 0;
	}
	return GetIndex(soundpos);
}

void cObj::SetSpeed(int k)
{
	for (int i = 0; i < 6; i++)
		m_data[i].m_Picture.m_FrameGap = k;
}

BOOL cObj::SetMagiSufferID(DWORD MagicID, int MagicPos, CWFile* pFile, BOOL up)
{
	if (!MagicID)
	{
		m_MagicSuffer.m_NeedShow = FALSE;
		return TRUE;
	}
	m_MagicSufferPos = MagicPos;
	m_bMagicSufferUp = up;
	if (!pFile)ERRBOX;
	if (pFile->m_NowID == MagicID)
	{
		//�����ͬһͼ��
		pFile->m_Picture.SetFrame(0);
	}
	pFile->Load(MagicID);
	m_MagicSuffer.Set(pFile);
	m_MagicSuffer.m_NeedShow = TRUE;
	MoveMagicSufferID();
	return TRUE;
}

void cObj::MoveMagicSufferID()
{
	if (!m_MagicSuffer.m_NeedShow)return;
	m_MagicSuffer.SetX(GetX());
	if (m_MagicSufferPos == 10)
	{
		m_MagicSuffer.SetY(GetTopY());
	}
	else
	if (m_MagicSufferPos == -10)
	{
		m_MagicSuffer.SetY(GetY());
	}
	else
	{
		m_MagicSuffer.SetY((GetY() + GetTopY()) / 2 - 30 * m_MagicSufferPos);
	}
}

void cObj::MoveMagicSuffer()
{
	for (list<sBuff>::iterator it = m_StateList.begin(); it != m_StateList.end(); it++)
	{
		if (it->MagicID)
		{
			it->data.SetX(GetX());
			if (it->bufPos == 10)
			{
				it->data.SetY(GetTopY());
			}
			else
			if (it->bufPos == -10)
			{
				it->data.SetY(GetY());
			}
			else
			{
				it->data.SetY((GetY() + GetTopY()) / 2 - 30 * (it->bufPos));
			}
		}
	}
}

int cObj::GetSkillLv(int skillid)
{
	if (m_IndexType == INDEXTYEP_CHARACTER)
	{
		cCharacter* p = (cCharacter*)m_pIndex;
		return p->GetSkillLv(skillid, &g_pMainState->m_SkillManager);
	}
	return 0;
}

void cObj::SetXY(int x, int y)
{
	if (GetX() == x&&GetY() == y)return;
	m_xPos = (float)x;
	m_yPos = (float)y;
	for (int i = 0; i < 8; i++)
	{
		m_data[i].m_Picture.SetXY(x, y);
		m_data[i].m_bMove = TRUE;
	}
	m_MagicEffect.SetXY(GetX(),GetY());
	m_MagicEffect.m_bMove = TRUE;

	if (m_bNeedShowName)
	{
		MoveName();
	}
}

void cObj::AutoPCData(
	string name, int modeid,
	sAddPoint* addpoint,
	sZiZhi* zizhi, eMengPai mengpaid, sItem2* pWeapon, int petid, int stronglv)
{
	if (stronglv == -1)stronglv = g_pMainState->m_GlobalButton[0];
	SetDataType(INDEXTYEP_CHARACTER);
	cCharacter* pc = (cCharacter*)m_pIndex;
	pc->m_PCData.m_StrongLv = stronglv;
	pc->AutoPCData(name, modeid, addpoint, zizhi, mengpaid, petid);
	if (name == "")
	{
		if (petid > -1)
		{
			name = pc->m_PCData.m_pCardPetData->GetData()->m_Name;
		}
	}
	m_TrueName.SetString(name);
	if (pWeapon)
	{
		pc->m_PCData.m_Equip[2].Set(pWeapon->GetType(),pWeapon->GetID(),FALSE);
		pc->m_PCData.m_pCharacterData2 = 0;
	}
	pc->m_PCData.m_Controler.Set();
}

void cObj::AutoPetData(string name, int modeid, int stronglv, sAddPoint* addpoint, BOOL Needskill)
{
	SetDataType(INDEXTYEP_PET);
	cPet* pc = (cPet*)m_pIndex;
	pc->AutoPetData(modeid, stronglv, addpoint, Needskill);
	if (name == "")name = pc->GetData()->m_Name;
	m_TrueName.SetString(name);
	pc->m_PetDataZZ.m_Controler.Set();
}

void cObj::ResetIndex()
{
	switch (m_IndexType)
	{
	case INDEXTYEP_CHARACTER:
		if (1)
		{
			cCharacter* pc = (cCharacter*)m_pIndex;
			pc->m_PCData.Reset();
		}
		break;
	case INDEXTYEP_PET:
		if (1)
		{
			cPet* pet = (cPet*)m_pIndex;
			pet->m_PetDataZZ.Reset();
		}
		break;
	default:
		break;
	}
}

void cObj::Talk(string context, BOOL needret)
{
	if (g_pMainState->m_TriggerDialog.m_datalist[0].m_Nowtext > -1)
	{//�ȰѶԻ���ȡ��
		g_pMainState->m_Dialog.Close();
	}
	g_pMainState->m_Dialog.Reset();
	g_pMainState->m_Dialog.SetObj(this);
	if (!needret)
		g_pMainState->m_Dialog.m_pObj = 0;
	g_pMainState->m_Dialog.SetString(context);
	g_pMainState->m_Dialog.OnOff(TRUE);
}

void cObj::Talk(string context, cTaskFather* pTask, int stage, string select1 /*= ""*/, string select2 /*= ""*/, string select3 /*= ""*/, string select4 /*= ""*/)
{
	g_pMainState->m_Dialog.Reset();
	g_pMainState->m_Dialog.SetObj(this);
	g_pMainState->m_Dialog.SetString(context,select1,select2,select3,select4);
	g_pMainState->m_Dialog.SetTrigger(pTask, stage, stage, stage, stage);
	g_pMainState->m_Dialog.OnOff(TRUE);
}
void cObj::Answer(cTaskFather* pTask, int stage)
{
	g_pMainState->m_Dialog.Reset();
	g_pMainState->m_Dialog.SetObj(this);
	g_pMainState->m_Dialog.Question();
	g_pMainState->m_Dialog.SetTrigger(pTask, stage, stage, stage, stage);
	g_pMainState->m_Dialog.OnOff(TRUE);
}
void cObj::MoveShadow()
{
	m_Shadow.SetXY((int)m_xPos, (int)m_yPos); 
}

BOOL cObj::CheckHaveState(int skillid)
{
	for (list<sBuff>::iterator it = m_StateList.begin(); it != m_StateList.end(); it++)
	{
		if (it->ID == skillid)
			return TRUE;
	}
	return FALSE;
}

void cObj::Move2()
{
	MoveHpLine();
	MoveDialog();
	MoveMagicSuffer();
	MoveMagicSufferID();
	MoveShadow();
}

void cObj::Reset()
{ //ɾ����ɫ�Ķ���
	m_NowPos = -1;
	m_PrePos = -1;
	for (int i = 0; i < 8; i++)
	{
		m_data[i].Free();
	}
}



void cObj::SetFrame(int fram)
{
	for (int i = 0; i < 3; i++)
	{
		if (m_pShowData[i]->m_NeedShow)
			m_pShowData[i]->SetFrame(fram);

	}
}


sObjFather* cObj::GetObjFather()
{
	sObjFather* p;
	switch (m_IndexType)
	{
	case INDEXTYEP_PET:
		if (1)
		{
			cPet* pet = (cPet*)m_pIndex;
			p= pet->GetData();
			return p;
		}
		break;
	case INDEXTYEP_CHARACTER:
		if (1)
		{
			cCharacter* pet = (cCharacter*)m_pIndex;
			p = pet->GetData();
			return p;
		}
		break;
	}
	return 0;
}

BOOL cObj::GetSex()
{
	return GetObjFather()->m_bSex;
}

void cObj::SetShow(BOOL Needshow)
{
	m_NeedShow2 = Needshow;
}

BOOL cObj::GetShow()
{
	return m_NeedShow2;
}

void cObj::AutoPCPetData(string name, int petid,  sAddPoint* addpoint, eMengPai mengpaid /*= eMP_��*/, int stonglv /*= 50 */)
{
	sZiZhi zizhi;
	sPetData* petdata = g_pMainState->m_PetDataManager.GetPetData(petid);
	zizhi.Set(petdata->m_ZiZhi, 100,petdata->m_LVLimit);
	AutoPCData(name, 0, addpoint, &zizhi, mengpaid, 0, petid, stonglv);
}

void cObj::FollowLv(int lv)
{
	switch (m_IndexType)
	{
	case INDEXTYEP_CHARACTER:
		if (1)
		{
			cCharacter* pc = (cCharacter*)m_pIndex;
			pc->m_PCData.Followlv(lv);
		}
		break;
	case INDEXTYEP_PET:
		if (1)
		{
			cPet* pet = (cPet*)m_pIndex;
			pet->m_PetDataZZ.Followlv(lv);
		}
		break;
	default:
		ERRBOX;
		break;
	}
}

void cObj::SetShake(BOOL shake)
{
	m_bShake = shake; 
}

void cObj::AutoFightDirection(int id)
{
	if (id < 10)
		SetDirection(7);
	else
		SetDirection(3);
}

void cObj::SetWeapon(int type, int id)
{
	if (m_IndexType == INDEXTYEP_CHARACTER)
	{
		cCharacter* pc = (cCharacter*)m_pIndex;
		pc->m_PCData.m_Equip[2].Set(type, id, FALSE);
		pc->m_PCData.m_Equip[2].m_Num = 1;
		pc->m_PCData.m_pCharacterData2 = 0;
	}
}

void cObj::LoadCharacter(string name, string path)
{
	SetDataType(INDEXTYEP_CHARACTER);
	cCharacter* pc = (cCharacter*)m_pIndex;
	cPCData& pcd = pc->m_PCData;
	pcd.Read(name,path);
	
	pc->SetData(&g_pMainState->m_pCharacterDataList[pc->m_PCData.modeid]);
	m_TrueName.SetColor(RGB(0, 255, 0));
	m_TrueName.SetString(pc->m_PCData.m_Name);
}








cText::cText()
{
	m_PreShowRect.right = -1;
	m_NeedShow = TRUE;
}

BOOL cText::DrawAlphaEx(BYTE* Pic, RECT rect, cMap* pMap)
{
	if (m_time > 0)
	{
		m_time -= 1;
		if (!m_time)
			m_NeedShow = FALSE;
	}
	if (0 == m_Width)return TRUE;
	int xtrue = m_x;
	int	ytrue = m_y;
	if (!m_bShowOnScreen)
	{
		xtrue -= pMap->GetXCenter() - 320;
		ytrue -= pMap->GetYCenter() - 240;
	}
	if ((xtrue + GetShowWidth())  < 0 ||
		xtrue   > 640 ||
		ytrue > 480 ||
		ytrue + GetShowHeight() < 0)
		return TRUE;

	RECT  picrect;  //ͼƬ����Ĵ�С
	
	picrect.left = xtrue;
	picrect.top = ytrue;
	picrect.right = picrect.left + GetShowWidth();
	picrect.bottom = picrect.top + GetShowHeight();
	RECT nowrect;  //ͼƬ������ʾ�Ĵ�С
	if (!CheckCross(picrect, rect, nowrect))return TRUE;
	string str3 = m_Text;
	WPixel pix1, pix2;
	int index, index2,index11,index22,alphai,alphai2;
	//ʵ������Ĵ�С
	int Height = nowrect.bottom - nowrect.top;
	int Width = nowrect.right - nowrect.left;
	//ʵ���������ͼƬ�����ƫ��
	int xOffset = nowrect.left - picrect.left;// picrect.left;
	int yOffset = nowrect.top - picrect.top;// picrect.top;

	//ʵ���������Ŀ�������ƫ��
	int xOffset3 = nowrect.left - rect.left;
	int yOffset3 = nowrect.top - rect.top;

	BYTE*&  lpSrc = m_pData;
	BYTE*&  pAlpha = m_pAlpha;
	
	int alpha,unalpha;
	
	int alphaoffy = yOffset + m_yOffset2;
	int alphaoffx = xOffset + m_xOffset2;
	int widthpic = rect.right - rect.left;

	alphai2 = alphaoffy*m_Width + alphaoffx;
	alphai = alphai2;
	index11 = alphai2 << 1;
	index = index11;
	index22 = (yOffset3*widthpic+ xOffset3) <<1;
	index2 = index22;

	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			alpha = pAlpha[alphai];
			alphai += 1;
			if (!alpha)
			{
				index += 2;
				index2 += 2;
				continue;
			}
	
			if (alpha < 255)
			{
				pix1.l = lpSrc[index];
				pix1.h = lpSrc[index + 1];
				pix2.l = Pic[index2];
				pix2.h = Pic[index2 + 1];

				unalpha = 255 - alpha;
				pix2.red = (pix1.red *alpha + pix2.red * unalpha) >> 8;
				pix2.green = (pix1.green *alpha + pix2.green * unalpha) >> 8;
				pix2.blue = (pix1.blue *alpha + pix2.blue * unalpha) >> 8;
				Pic[index2] = pix2.l;
				Pic[index2 + 1] = pix2.h;
			}
			else
			{
				pix1.l = lpSrc[index];
				pix1.h = lpSrc[index + 1];
				Pic[index2] = pix1.l;
				Pic[index2 + 1] = pix1.h;
			}
			index += 2;
			index2 += 2;
		}
		index11 += m_Width << 1;
		index = index11;
		index22 += widthpic << 1;
		index2 = index22;
		alphai2 += m_Width;
		alphai = alphai2;
	}
	return TRUE;
}
void cText::UpdateData()
{
	//DWORD oldcolor = 0;
	HFONT oldFont = 0;
	if (m_hFont)
	{
		oldFont = (HFONT)SelectObject(g_DC2, m_hFont);//err
	}
	SIZE size1;
	CountSize(size1);
	if (0 != m_Style)
	{
		size1.cx += 2;
		size1.cy += 2;
	}
	m_tuewidth = size1.cx;
	m_Width = size1.cx;
	m_Height = size1.cy;



	m_Width = ((m_Width * 2 + 7) & 0xfffffff8) / 2;



	RECT rect2;
	rect2.left = 0;
	rect2.top = 0;
	rect2.right = m_Width;
	rect2.bottom = m_Height;



	HBITMAP hBitmap = CreateCompatibleBitmap(g_ShowDC, m_Width, m_Height + 1);
	HBITMAP oldmap = (HBITMAP)SelectObject(g_DC2, hBitmap);
	//����ͼƬ��С
	
	m_Size = m_Height*m_Width;
	if ( m_Size>m_MemSize)
	{
		m_MemSize=m_Size;
		SAFE_DELETE_ARRAY(m_pData);
		SAFE_DELETE_ARRAY(m_pAlpha);
		m_pData = new BYTE[m_MemSize <<1];
		m_pAlpha = new BYTE[m_MemSize];
	}
	g_pBmpinfo->bmiHeader.biCompression = 0;
	g_pBmpinfo->bmiHeader.biWidth = m_Width;
	g_pBmpinfo->bmiHeader.biHeight = -m_Height;
	g_pBmpinfo->bmiHeader.biSizeImage = m_Size <<1;
	//
	WPixel pix, pix2;
	//����������ɫ,ѡ�񱳾���ɫ
	pix2 = m_color;
	FillRect(g_DC2, &rect2, g_blackbrush);

	DrawTextA(
		g_DC2,
		m_Text.c_str(),
		-1,
		&rect2,
		DT_LEFT | DT_WORDBREAK
		);
	GetDIBits(g_DC2, hBitmap, 0, m_Height, m_pData, g_pBmpinfo, DIB_RGB_COLORS);
	int index;
	WORD* color, *color2;
	index = 0;
		for (int kk = 0; kk < m_Height; kk++)
		{
			for (int j = 0; j < m_Width; j++)
			{
				color = (WORD*)&m_pData[index <<1];
				pix = *color;
				if (0==(*color))
				{
					m_pAlpha[index] = 0;
				}
				else
				if ((*color) > 32766)
				{
					m_pAlpha[index] = 255;
					*color = pix2.color;
				}
				else
				{
					m_pAlpha[index] =(BYTE) pix.red<<3;
					*color = pix2.color;
				}
				index += 1;
			}
		}
	if (m_Style) //���ֵ���Ӱ
	{
		pix2 = m_BacColor;
		if (m_Size > m_MemSize2)
		{
			m_MemSize2 = m_Size;
			SAFE_DELETE_ARRAY(m_pData2);
			m_pData2 = new BYTE[m_MemSize2 <<1];
		}
		FillRect(g_DC2, &rect2, g_blackbrush);

		if (m_Style == 1)
		{
			rect2.left += 1;
			rect2.top += 1;
			rect2.bottom += 1;
			rect2.right += 1;
			DrawTextA(
				g_DC2,
				m_Text.c_str(),
				-1,
				&rect2,
				DT_LEFT | DT_WORDBREAK
				);
		}
		else
		{
			//ѡ�񻭱�  �������
			if (m_bBold)
				SelectObject(g_DC2, g_BWhitePen);
			//���
			BeginPath(g_DC2);
			TextOutA(g_DC2, rect2.left, rect2.top, m_Text.c_str(), m_Text.size());
			EndPath(g_DC2);
			StrokePath(g_DC2);
			if (m_bBold)
				SelectObject(g_DC2, g_WhitePen);
		}
		GetDIBits(g_DC2, hBitmap, 0, m_Height, m_pData2, g_pBmpinfo, DIB_RGB_COLORS);

		int oldindex = 0;
		index = 0;
		for (int kk = 0; kk < m_Height; kk++)
		{
			for (int j = 0; j < m_Width; j++)
			{
				color = (WORD*)&m_pData2[index<<1];
				if ((*color) == 0)
				{
					index += 1;
					continue;
				}
				else
				{

					if (m_pAlpha[index])
					{
						index += 1;
						continue;
					}
						color2 = (WORD*)&m_pData[index << 1];
						*color2 = pix2.color;
						m_pAlpha[index] = 255;
			
				}
				index += 1;
			}
			oldindex += m_Width;
			index = oldindex;
		}
	}
	SelectFont(g_DC2, oldFont);
	SelectObject(g_DC2, oldmap);
	DeleteObject(hBitmap);
	if (m_xCenter)
	{
		UpdateXCenter();
	}
}

void cText::SetString(char* str)
{
	string str2 = str; 
	SetString(str2);
}

void cText::SetString(string str)
{
	if (0 == str.compare(m_Text))return;
	m_bMove = TRUE; 
	m_Text = str; 
	SetxOffset(0); 
	SetyOffset(0);
	SetxOffset2(0);
	SetyOffset2(0);
	m_ShowWidth = 0;
	m_ShowHeight = 0;
	UpdateData(); 
}

void cText::SetString(string str, DWORD color)
{
	m_color = color;
	m_bMove = TRUE;
	m_Text = str;
	UpdateData();
}

void cText::SetString(int str)
{
	ostringstream oss;
	oss << str;
	SetString(oss.str());
}
void cText::SetString(INT64 str)
{
	ostringstream oss;
	oss << str;
	SetString(oss.str());
}
cText::~cText()
{
	SAFE_DELETE_ARRAY(m_pData);
	SAFE_DELETE_ARRAY(m_pData2);
	SAFE_DELETE_ARRAY(m_pAlpha);
}

void cText::CountSize(SIZE& size)
{
	if (m_MaxWidth)
	{
		GetTextExtentPoint32A(g_DC2, m_Text.c_str(), m_Text.size(), &size);
		

		RECT rect;
		rect.left = 0;
		rect.top = 0;
		if (size.cx <= m_MaxWidth)
			rect.right= size.cx;
		else
			rect.right = m_MaxWidth;
		rect.bottom = 300;
		size.cy = DrawTextA(g_DC2, m_Text.c_str(), -1, &rect, DT_CALCRECT | DT_LEFT | DT_WORDBREAK );//| DT_EDITCONTROL);

		if (size.cx>m_MaxWidth)
		size.cx = m_MaxWidth;
	}
	else
	{
		GetTextExtentPoint32A(g_DC2, m_Text.c_str(), m_Text.size(), &size);
	}
}

void cText::SetColor(DWORD newcolor)
{
	if (m_color == newcolor)return;
	if (!m_Size)
	{
		m_color = newcolor;
		return;
	}
	m_bMove = TRUE; 
	if (m_Style&&m_BacColor == m_color)
	{
		m_color = newcolor;
		UpdateData();
	}
	else
	{
		ChangeColor(m_color, newcolor);
		m_color = newcolor;
	}
	 
}
void cText::SetBackColor(DWORD color)
{
	if (m_BacColor == color)return;
	if (!m_Size)
	{
		m_BacColor = color;
		return;
	}
	m_bMove = TRUE;
	if (m_BacColor == m_color)
	{
		UpdateData();
		m_BacColor = color;
	}
	else
	{
		
		ChangeColor(m_BacColor, color);
		m_BacColor = color;
	}
	
}
//�ı���ɫ
void cText::ChangeColor(DWORD originalcolor, DWORD outcolor)
{
	int index=0;
	WPixel color;
	WPixel original;
	original= originalcolor;
	WPixel out;
	out= outcolor;
	for (int kk = 0; kk < m_Height; kk++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			if (m_pAlpha[index])
			{
				color.l =m_pData[index << 1];
				color.h = m_pData[(index << 1)+1];
				if (color.color == original.color)
				{
					m_pData[index << 1] = out.l;
					m_pData[(index << 1) + 1] = out.h;
				}

			}
			index +=1;
		}
	}
	m_bMove = TRUE;
}

void cText::SetTagColor(int starttag, int endtag, DWORD color)
{
	int k;
	WPixel pix;
	pix=color;
	if(starttag<0)starttag=0;
	if (endtag < 0)endtag = m_Width;
	for (int j = 0; j < m_Height;j++)
	for (int i = starttag; i < endtag; i++)
	{
		k = i + m_Width*j;
		if (m_pAlpha[k])
		{
			k *= 2;
			m_pData[k] = pix.l;
			m_pData[k+ 1]=pix.h;
		}
	}
}

void cText::SetTransparentColor(int startpos, int endpos, DWORD color)
{
	if (endpos < startpos)swap(endpos, startpos);
	int k;
	WPixel pix;
	pix = color;
	if (startpos < 0)startpos = 0;
	if (endpos < 0)endpos = m_Width;
	for (int j = 0; j < m_Height; j++)
	for (int i = startpos; i < endpos; i++)
	{
		k = i + m_Width*j;
		if (!m_pAlpha[k])
		{
			m_pData[k*2] = pix.l;
			m_pData[k*2 + 1] = pix.h;
			m_pAlpha[k] = 255;
		}
	}

}

void cText::UnTransparentColor(int startpos, int endpos, DWORD transparentcolor)
{
	if (endpos < startpos)swap(endpos, startpos);
	int k;
	WPixel pix,targetpix;
	targetpix = transparentcolor;
	if (startpos < 0)startpos = 0;
	if (endpos < 0)endpos = m_Width;
	for (int j = 0; j < m_Height; j++)
	for (int i = startpos; i < endpos; i++)
	{
		k = i + m_Width*j;
		pix.l = m_pData[k*2];
		pix.h = m_pData[k * 2+1];
		if (pix.color == targetpix.color)
		{
			m_pAlpha[k] = 0;
		}
	}
}

void cText::Show(cCanvas* pCanvas)
{
	if (!m_NeedShow)return;
	if (m_time > 0)
	{
		m_time -= 1;
		if (!m_time)
		{
			m_NeedShow = FALSE;
			return;
		}
	}
	pCanvas->Add(this);
}






BOOL sCharacterData::CanEquip(int EquipType)
{
	BOOL bBoy = TRUE;
	switch (m_ID)
	{
	case eJuSe������:
		if (EquipType == 0 || EquipType == 1)
			return TRUE;
		break;
	case eJuSe��ң��:
		if (EquipType == 0 || EquipType == 5)
			return TRUE;
		break;
	case eJuSe����Ů:
		bBoy = FALSE;
		if (EquipType == 10 || EquipType == 11)
			return TRUE;
		break;
	case eJuSeӢŮ��:
		bBoy = FALSE;
		if (EquipType == 11 || EquipType == 9)
			return TRUE;
		break;
	case eJuSe��ͷ��:
		if (EquipType == 3 || EquipType == 4)
			return TRUE;
		break;
	case eJuSe��ħ��:
		if (EquipType == 1 || EquipType == 4)
			return TRUE;
		break;
	case eJuSe�Ǿ���:
		bBoy = FALSE;
		if (EquipType == 6 || EquipType == 7)
			return TRUE;
		break;
	case eJuSe������:
		bBoy = FALSE;
		if (EquipType == 9 || EquipType == 6)
			return TRUE;
		break;
	case eJuSe��̫��:
		if (EquipType == 5 || EquipType == 2)
			return TRUE;
		break;
	case eJuSe�����:
		if (EquipType == 2 || EquipType == 3)
			return TRUE;
		break;
	case eJuSe���ʶ�:
		bBoy = FALSE;
		if (EquipType == 7 || EquipType == 8)
			return TRUE;
		break;
	case eJuSe���켧:
		bBoy = FALSE;
		if (EquipType == 8 || EquipType == 10)
			return TRUE;
		break;
	case eJuSe������:
		bBoy = FALSE;
		if (EquipType == 12 || EquipType == 14)
			return TRUE;
		break;
	case eJuSe������:
		if (EquipType == 14 || EquipType == 13)
			return TRUE;
		break;
	case eJuSeɱ����:
		if (EquipType == 13 || EquipType == 12)
			return TRUE;
		break;
	default: 
		bBoy = FALSE;
		if (EquipType == 11 || EquipType == 9)
			return TRUE;
	}

	switch (EquipType)
	{
	case 15:
		return bBoy;
	case 16:
		return !bBoy;
	case 17:
		return bBoy;
	case 18:
		return !bBoy;
	case 19:
		return TRUE;
	case 20:
		return TRUE;
	case 21:
		return TRUE;
	}

	return FALSE;
}



void cCharacter::GetID(int Pos, vector<DWORD>& out)
{
	if (18!=Pos)
	if (m_PCData.m_bUseCard)//��ɫ���ڱ���״̬�Ļ�
	{
		if (m_PCData.m_pCardPetData)
		{
			if (m_PCData.m_JinJieNum.m_PositionID>0)
			m_PCData.m_pCardPetData->m_PetDataZZ.m_JinJieNum = m_PCData.m_JinJieNum;
			if (20==Pos)
				return m_PCData.m_pCardPetData->GetID(18, out);
			else
				return m_PCData.m_pCardPetData->GetID(Pos, out);
		}
	}
	if (m_PCData.m_pCharacterData2)
	{
		DWORD testid;
		testid = m_PCData.m_pCharacterData2->m_Sound.GetSoundID(Pos);
		if (testid)
		{
			out.resize(1);
			out[0] = testid;
			return;
		}
	}
	switch (Pos)
	{
	case POS_STAND:
	case POS_WALK:
		if (m_PCData.m_bOnZuoQi)
		if (m_PCData.m_ZuoQi.m_pZuoQi)
		{
			return m_PCData.m_ZuoQi.GetID(Pos, out);
		}
		break;
	case POS_ANGRY:
		out.resize(1);
		out[0] = m_pCharacterData->m_posangry;
		return;
	case POS_SIT:
		out.resize(1);
		out[0] = m_pCharacterData->m_possit;
		return;
	case POS_BOW:
		out.resize(1);
		out[0] = m_pCharacterData->m_posbow;
		return;
	case POS_CRY:
		out.resize(1);
		out[0] = m_pCharacterData->m_poscry;
		break;
	case POS_GREET:
		out.resize(1);
		out[0] = m_pCharacterData->m_posgreet;
		return;
	case POS_DIALOG:
		out.resize(1);
		out[0] = m_pCharacterData->m_Head[m_PCData.m_bDuJie].m_dialog;
		return;
	case POS_SMALLHEAD:
		out.resize(1);
		out[0] = m_pCharacterData->m_Head[m_PCData.m_bDuJie].m_Head[m_PCData.m_face].m_smallhead;
		return;
	case POS_BIGHEAD:
		out.resize(1);
		out[0] = m_pCharacterData->m_Head[m_PCData.m_bDuJie].m_Head[m_PCData.m_face].m_bighead;
		return;
	}


	if (!m_PCData.m_pCharacterData2)
	{
		if (m_PCData.m_Equip[2].GetNum())
		{
			//��������
			m_PCData.m_pWeaponData2 = g_pMainState->m_ItemManage.GetWeaponList(m_PCData.modeid, m_PCData.m_Equip[2].GetID(), m_PCData.m_Equip[2].GetType());
			//ͨ������������ɫ������...�����
			if (m_PCData.m_pWeaponData2)
			{
				m_PCData.m_pCharacterData2 = &m_pCharacterData->m_Pos[m_PCData.m_pWeaponData2->MasterPos];
			}
			else
			{
				m_PCData.m_pCharacterData2 = &m_pCharacterData->m_Pos[0];
			}
		}
		else
		{
			m_PCData.m_pCharacterData2 = &m_pCharacterData->m_Pos[0];
		}
	}
	if (m_PCData.m_pWeaponData2)
	{
		out.resize(2);
		out[0] = m_PCData.m_pCharacterData2->GetID(Pos);
		out[1] = m_PCData.m_pWeaponData2->GetID(Pos);
	}
	else
	{
		out.resize(1);
		out[0] = m_PCData.m_pCharacterData2->GetID(Pos);
	}
}

BOOL cCharacter::CanEquip(int EquipType, int EquipLv, int Pos)
{
	if (m_PCData.m_LV < EquipLv)return FALSE;
	if (Pos != -1)
	{
		if (Pos < 6)
		{
			if (EquipType < 15)
			{
				if (Pos != 2)
					return FALSE;
			}
			switch (EquipType)
			{
			case 15://ͷ��
			case 16:
				if (Pos != 0)return FALSE;
				break;
			case 17://�·�
			case 18:
				if (Pos != 3)return FALSE;
				break;
			case 19://����
				if (Pos != 1)return FALSE;
				break;
			case 20://����
				if (Pos != 4)return FALSE;
				break;
			case 21://Ь
				if (Pos != 5)return FALSE;
				break;
			}
		}
	}
	return m_pCharacterData->CanEquip(EquipType);
}

sFightOffset* cCharacter::GetFightOffset()
{
	if (m_PCData.m_bUseCard)
	{
		return m_PCData.m_pCardPetData->GetFightOffset();
	}
	if (!m_PCData.m_pCharacterData2)
	{
		if (m_PCData.m_Equip[2].GetNum())
		{
			//��������
			m_PCData.m_pWeaponData2 = g_pMainState->m_ItemManage.GetWeaponList(m_PCData.modeid, m_PCData.m_Equip[2].GetID(), m_PCData.m_Equip[2].GetType());
			//ͨ������������ɫ������...�����
			m_PCData.m_pCharacterData2 = &m_pCharacterData->m_Pos[m_PCData.m_pWeaponData2->MasterPos];
		}
		else
		{
			m_PCData.m_pCharacterData2 = &m_pCharacterData->m_Pos[0];
		}
	}
	return &m_PCData.m_pCharacterData2->m_Offset;

}

BOOL cCharacter::AddMoney(int xianjin, int chubei, int cunyin)
{
	return m_PCData.AddMoney(xianjin, chubei, cunyin);
}


int cCharacter::GetSkillLv(int id,cSkillManager* p)
{
	if (id < 1000) //ֻ��ʦ�ż���
	{
		sSkill* pSkill = p->GetSkill(id);
		return m_PCData.m_MengPaiSkillLV[pSkill->m_Owner];
	}
	else return 0;
}

//Ⱦɫ
void cCharacter::GetRanse(int Pos, sChangeColorData* (&pChangeColor)[4])
{
	for (int i = 0; i < 4; i++)
		pChangeColor[i] = 0;
	if (m_PCData.m_bUseCard)//��ɫ���ڱ���״̬�Ļ�)
	{
		for (int i = 0; i < 4;i++)
			pChangeColor[i] = &m_PCData.m_pCardPetData->m_PetDataZZ.m_RanSe.m_ranse[i];
		return;
	}
    //����
	if (m_PCData.m_bOnZuoQi)
	if (m_PCData.m_ZuoQi.m_pZuoQi)
	{
		if (Pos == POS_STAND || Pos == POS_WALK)
		{
			pChangeColor[0] = &m_PCData.m_ZuoQiRanSe;
			pChangeColor[1] = &m_PCData.m_RanSe.m_ranse[0];
			if (m_PCData.m_ZuoQi.m_ZhuangShi.GetNum())
			{
				pChangeColor[2] = &m_PCData.m_ZhuangShiRanSe;
			}
			return;
		}
		
	}
	pChangeColor[0] = &m_PCData.m_RanSe.m_ranse[0];

}

void cCharacter::AutoPCData(string name, int modeid,  sAddPoint* addpoint, sZiZhi* zizhi, eMengPai mengpaid, int petid)
{
	if (modeid > 1999)modeid -= 2000;
	m_PCData.AutoPCData(name, modeid,  addpoint, zizhi, mengpaid, petid);
	m_pCharacterData = &g_pMainState->m_pCharacterDataList[modeid];
	m_PCData.m_race = m_pCharacterData->m_Autorace;
}


void cCharacter::Talk(string str)
{
	g_pMainState->m_Dialog.Reset();
	DWORD head;
	head= GetID(POS_DIALOG);
	g_pMainState->m_Dialog.SetHead(head);
	g_pMainState->m_Dialog.SetString(str);
	g_pMainState->m_Dialog.OnOff(TRUE);
}

void cCharacter::Free()
{
	//ɾ���ٻ���
	for (int i = 0; i < m_PCData.m_NumofBB; i++)
	{
		SAFE_DELETE(m_PCData.m_pPetDataList[i]);
	}

	if (m_PCData.m_bUseCard)
	if (m_PCData.m_CardPetID)
	{
		SAFE_DELETE(m_PCData.m_pCardPetData);
	}
}

int cCharacter::GetRace()
{
	if (m_PCData.m_race <0)
		m_PCData.m_race = m_pCharacterData->m_Autorace;
	return m_PCData.m_race;
}

void cCharacter::SetData(sCharacterData* data)
{
	if (m_PCData.m_race == -1)
		m_PCData.m_race = data->m_Autorace;
	m_pCharacterData = data;
	m_PCData.modeid = data->m_ID;
	m_PCData.m_pCharacterData2 = 0;
	m_PCData.m_pWeaponData2 = 0;
}
void cCharacter::UseCard(int id)
{
	if (id == -1)
	{
		SAFE_DELETE(m_PCData.m_pCardPetData);
		m_PCData.m_bUseCard =FALSE;
		
	}
	else
	{
		if (m_PCData.m_CardPetID == id)return UseCard(-1);
		if (0 == m_PCData.m_pCardPetData)
			m_PCData.m_pCardPetData = new cPet;
		m_PCData.m_bUseCard = TRUE;
		m_PCData.m_CardPetID = id;
		m_PCData.m_pCardPetData->SetData(g_pMainState->m_PetDataManager.GetPetData(id));
		m_PCData.m_pCardPetData->m_PetDataZZ.m_JinJieNum.m_PositionID = m_PCData.m_JinJieNum.m_PositionID;
		m_PCData.m_pCardPetData->m_PetDataZZ.m_JinJieNum = m_PCData.m_JinJieNum;
	}

	g_pMainState->m_PCHead.UpDateHead();
}

void cNpc::GetID(int POS_X, vector<DWORD>& out)
{
	out.resize(1);
	switch (POS_X)
	{
	case POS_STAND:
		out[0] = m_pNpcData->m_stand;
		break;
	case POS_WALK:
		out[0] = m_pNpcData->m_walk;
		break;
	case POS_DIALOG:
		out[0] = m_pNpcData->m_dialog;
		break;
	}
}



BOOL cPetData::LeveUp(int lv,BOOL bCost)
{
	if (!lv)return FALSE;
	if (bCost)
	{
		if (EXP < ExpMax)
		{ 
		//	���鲻��
			return FALSE;
		}
		if (m_AddPoint.idle)
		{
			g_pMainState->m_Tags.Add("�ӵ㷽ʽδ����");
			return FALSE;
		}
		if (m_LV >= 175)
		{
			g_pMainState->m_Tags.Add("�Ѵﵽ�ȼ�����");
			return FALSE;
		}
		for (int i = 0; i < lv; i++)
		{
			EXP -= ExpMax;
			ExpMax = GetCharacterExp(m_LV);
		}
	}
	
	m_ATK.AddDataOriginal(- CountATK());//�˺�
	m_DEF.AddDataOriginal(- CountDEF());//��
	m_HPMax.AddDataOriginal( - CountHP());//HP
	m_MPMax.AddDataOriginal( - CountMP());//MP
	m_Wakan.AddDataOriginal( - CountWakan());//����
	m_SPD.AddDataOriginal( - CountSpd());//�ٶ�
	m_Evade.AddDataOriginal( - CountEvade());//����
	m_HIT.AddDataOriginal( - CountHit());//����
	
	m_LV += lv;
	
	m_ProPertyAtk += (m_AddPoint.atk + 1)*lv;
	m_ProPertyDef += (m_AddPoint.def + 1)*lv;
	m_ProPertyHP += (m_AddPoint.hp + 1)*lv;
	m_ProPertyMag += (m_AddPoint.mp + 1)*lv;
	m_ProPertySpd += (m_AddPoint.spd + 1)*lv;


	//����������
	m_ATK.AddDataOriginal(CountATK());//�˺�
	m_DEF.AddDataOriginal(CountDEF());//��
	m_HPMax.AddDataOriginal(CountHP());//HP
	m_MPMax.AddDataOriginal(CountMP());//MP
	m_Wakan.AddDataOriginal(CountWakan());//����
	m_SPD.AddDataOriginal(CountSpd());//�ٶ�
	m_Evade.AddDataOriginal(CountEvade());//����
	m_HIT.AddDataOriginal(CountHit());//����


	m_PhysicalMax += 5*lv; //��������;     
	m_EnergyMax += 5 * lv;  //��������
	ExpMax = GetCharacterExp(m_LV);
	FillHPMP();
	return TRUE;
}

void cPetData::AddHpZZ(int num)
{
	if (!num)return;
	m_HPMax.AddDataOriginal(-CountHP());
	m_ZiZhi.m_HPZZ += num;
	m_HPMax.AddDataOriginal(CountHP());
	m_HPHurtMax = m_HPMax.GetData();
	m_HP = m_HPHurtMax;
}

void cPetData::AddGrowing(int num)
{
	if (!num)return;
	int atk, def, mag, spd, hp;
	atk = m_ProPertyAtk;
	def = m_ProPertyDef;
	spd = m_ProPertySpd;
	hp = m_ProPertyHP;
	mag = m_ProPertyMag;
	AddProPertyAtk(-atk);
	AddProPertyDef(-def);
	AddProPertyMag(-mag);
	AddProPertyHp(-hp);
	AddProPertySpd(-spd);
	m_ZiZhi.m_Growing += num;
	AddProPertyAtk(atk);
	AddProPertyDef(def);
	AddProPertyMag(mag);
	AddProPertyHp(hp);
	AddProPertySpd(spd);
}
void cPetData::AddMagZZ(int num)
{
	if (!num)return;
	m_MPMax.AddDataOriginal(-CountMP());
	m_Wakan.AddDataOriginal(-CountWakan());
	m_ZiZhi.m_MAGZZ += num;
	m_MPMax.AddDataOriginal(CountMP());
	m_Wakan.AddDataOriginal(CountWakan());
	m_MP = m_MPMax.GetData();
}
void cPetData::AddAtkZZ(int num)
{
	if (!num)return;
	m_ATK.AddDataOriginal(-CountATK());
	m_HIT.AddDataOriginal(-CountHit());
	m_ZiZhi.m_ATKZZ += num;
	m_ATK.AddDataOriginal(CountATK());
	m_HIT.AddDataOriginal(CountHit());
}
void cPetData::AddAvoidZZ(int num)
{
	if (!num)return;
	m_Evade.AddDataOriginal(-CountEvade());
	m_ZiZhi.m_AVOIDZZ += num;
	m_Evade.AddDataOriginal(CountEvade());
}
void cPetData::AddSpdZZ(int num)
{
	if (!num)return;
	m_SPD.AddDataOriginal(-CountSpd());
	m_ZiZhi.m_SPDZZ += num;
	m_SPD.AddDataOriginal(CountSpd());
}
void cPetData::AddDefZZ(int num)
{
	if (!num)return;
	m_DEF.AddDataOriginal(-CountDEF());
	m_ZiZhi.m_DEFZZ += num;
	m_DEF.AddDataOriginal(CountDEF());
}

int cPetData::CountATK()
{
	int atk = (m_LV*m_ZiZhi.m_ATKZZ * 7) / 2000 +
		m_ProPertyAtk*m_ZiZhi.m_Growing / 100;
	return atk;
}

int cPetData::CountDEF()
{
	int def = (m_LV*m_ZiZhi.m_DEFZZ) / 433 +
		(m_ProPertyDef*m_ZiZhi.m_Growing * 4) / 300;
	return def;
}

int cPetData::CountHP()
{
	int hp = (m_LV*m_ZiZhi.m_HPZZ) / 1000 +
		m_ProPertyHP*m_ZiZhi.m_Growing * 6 / 100;
	return hp;
}

int cPetData::CountWakan()
{
	//����=�ȼ�*(��������+1640)*(�ɳ�+1)/7500+����*0.3+����*0.7+����*0.4+����*0.2
	int	wakan = (m_LV* (m_ZiZhi.m_MAGZZ + 1640)*(m_ZiZhi.m_Growing + 100) / 75000 +
		m_ProPertyHP * 3 +
		m_ProPertyMag * 7 +
		m_ProPertyAtk * 4 +
		m_ProPertyDef * 2
		) / 10;
	return wakan;
}

int cPetData::CountMP()
{
	//
	int mp = m_LV*m_ZiZhi.m_MAGZZ / 300 + m_ZiZhi.m_Growing*m_ProPertyMag / 100;
	return mp;
}

int cPetData::CountSpd()
{
	int spd = (m_ZiZhi.m_SPDZZ*m_ProPertySpd) / 1000;
	return spd;
}

int cPetData::CountEvade()
{
	int evade = m_ProPertySpd;
	return evade;
}

int cPetData::CountHit()
{
	int hit = m_ProPertyAtk * 2;
	return hit;
}

cPetData::cPetData()
{
	for (int i = 0; i < c_NumFuZhu; i++)
		m_FuZhuSkillLvList[i]=0;
	m_pPetSkill.resize(24);
}


void cPetData::AutoPetData(BOOL m_bNeedSkill)
{
	if (-1 == modeid)return;
	
	sPetData* pPet = g_pMainState->m_PetDataManager.GetPetData(modeid);
	m_LV = 0;
	m_NumofBBSkill = 0;
	if (m_bNeedSkill)
	for (int i = 0; i < pPet->m_SkillNum; i++)
	{
		if (rand() % 100 >= (100 - m_StrongLv)) //ÿ��������10%-100%�ļ��ʿ��Եõ�
		{
			g_pMainState->m_SkillManager.SetPetSkill(this, pPet->m_pPetSkill[i]->m_Id, m_NumofBBSkill);
		}
	}
	//���ʳɳ�
	m_ZiZhi.Set(pPet->m_ZiZhi, m_StrongLv, pPet->m_LVLimit);
	AddProPertyAtk(30);
	AddProPertyDef(20);
	AddProPertyHp(20);
	AddProPertyMag(20);
	AddProPertySpd(20);
	m_Name = pPet->m_Name;
	m_dataFileName = m_Name;
	m_Popularity = g_pMainState->m_GlobalButton[0] * 10;
	FillHPMP();
}

void cPetData::Reset()
{
	//
	m_XiuLian.Reset();
	for (int i = 0; i < 3; i++)
		m_PetEquip[i].m_Num = 0;;
	//BB����
	  m_NumofBBSkill=0;
	//����
	 EXP = 0;
	 ExpMax = 0;
	//Ⱦɫ
	 for (int i = 0; i < 4;i++)
		m_RanSe.m_ranse[i].partnum=0;

	//ս�����Ա�
	   m_FightProperty.Clear();

	//��������
	   for (int i = 0; i < c_NumFuZhu; i++)
	 m_FuZhuSkillLvList[i]=0;
	//
	m_race=-1;//����
	 modeid = -1;
	 m_AiLv = 0;//AI�ȼ�  ������ɫ��ս��ʱ����Ϊ 0-100
	 m_StrongLv = 0; //ǿ��  ������ɫ������ǿ��
	 m_LV = 0;
	 m_HP = 0;
	 m_HPHurtMax = 0;
	 m_MP = 0;
	 m_CP = 0;      //��ŭ

	 m_LiveHpLv = 0;//��������
	 m_LiveMpLv = 0;//ڤ˼
	 m_LiveCp = 0; //ÿ�غ�CP�ظ�

	 m_Physical=0;    //����
	 m_PhysicalMax=0; //��������
	 m_Energy=0;      //����
	 m_EnergyMax=0;  //��������
	//���Ե�����
	m_ProPertyAtk = 0;
	m_ProPertyDef = 0;
	m_ProPertyMag = 0;
	m_ProPertyHP = 0;
	m_ProPertySpd = 0;

	nightLv = 0;//�ȼ�ҹս����
	mpsavepercent = 0; //�۸�	
	restoreRate = 0; //״̬�ָ���
	abnormalResist = 0; //������
	magicWavePercent=0;  //��������
	comboNumDown = 0;//��������
	comboNumUp = 2;//��������
	ShiXueAdd = 0; //�����˺�����
	buffAdd = 0;//BUFFʱ��
	m_MagicNumAdd = 0;//�������ö�������


	m_HPMax.Reset();
	m_MPMax.Reset();
	m_ATK.Reset();  //�˺�
	m_HIT.Reset();  //����
	m_DEF.Reset();  //����
	m_Wakan.Reset();//����
	m_SPD.Reset();  //�ٶ�
	m_Evade.Reset(); //����


	GoldLivePercent.Reset();//���
	m_ParryRate.Reset(); //����  
	//��ɱ��
	m_CriRate.Reset(); //����
	m_MagicCriRate.Reset(); //����
	m_CureCriRate.Reset();//����

	m_CriAddPercent.Reset(); //��ɱ�˺�����
	m_Combo.Reset();  //������
	m_MagicComboRate.Reset(); //������
	m_Pierce.Reset();//���ӷ�����
	m_PierceMagic.Reset(); //����������

	//������
	m_HitRate.Reset();  //����
	m_MagicHitRate.Reset();//����
	  m_SealRate.Reset();//��ӡ
	 poisonRate.Reset(); //��
	//�˺��ӳ�
	m_DamageAddPercentMagic2.Reset();//�̶�
	m_DamageAddPercentMagic.Reset();//����
	m_DamagePercent.Reset();//����
	m_HiddenWeaponDamagePercent.Reset(); //����
	m_DamageAddPercentPoision.Reset(); //��
	m_DamageAddPercentCure.Reset();//����
	//�ٻ����˺��ӳ�
	m_PetDamagePercentMagic.Reset();//�����˺�
	m_PetDamagePercent.Reset();//�����˺�
	//�˺�����
	m_DamageAddMagic.Reset();//�˺��������
	m_DamageAddMagicLv.Reset();//�����˺��������lv
	m_DamageAdd.Reset();//�����˺��������
	m_DamageAddLv.Reset();  //�����˺����
	//����
	m_CounterRate.Reset();
	//����
	m_StabRate.Reset();
	m_StabRateMagic.Reset();    //���������˺�
	m_StabDamageRate.Reset();
	m_StabDamageRateMagic.Reset();//����������
	//
	m_SuckBloodRate.Reset();//��Ѫ��

	m_DamageAddCure.Reset();
	//����������
	m_AtkDownCP.Reset();//����CP
	m_StartMagic.Reset();
	m_MagDownHPHurt.Reset();
	m_AtkDownHPHurt.Reset();
	m_AtkDownMP.Reset();
	m_AtkDownAtk.Reset();
	m_AtkDownHit.Reset();
	m_AtkDownDef.Reset();
	m_AtkDownWakan.Reset();
	m_AtkDownSpd.Reset();
	m_Spurting.Reset();
	//
	//������ºͽ�ħ����Ч��
	m_JinJiaRate.Reset();
	m_XiangMoRate.Reset();
	//�ع�
	m_MagicFire.Reset(); //��
	m_MagicSoil.Reset(); //��
	m_MagicWater.Reset();  //ˮ
	m_MagicThunder.Reset();//��

	BoySpecial.Reset();  //��
	GirlSpecial.Reset();   //Ů
	GoldSpecial.Reset();    //�����ع�
	MagSpecial.Reset();    //ħ���ع�
	ImmortalSpecial.Reset(); //�����ع�
	MonsterSpecial.Reset();//�����ع�
	PeopleSpecial.Reset(); //��
	ghostSpecial.Reset();  //���ع�
	machineSpecial.Reset();//��е�ع�
}

void cPetData::AddProPertyAtk(int num)
{
	//����Ӱ��  �˺�  ����  ����  �ٶ�
	m_ATK.AddDataOriginal(-CountATK());//�˺�
	m_Wakan.AddDataOriginal(-CountWakan());//����
	m_SPD.AddDataOriginal(-CountSpd());//�ٶ�
	m_HIT.AddDataOriginal(-CountHit());//����
	m_ProPertyAtk += num;
	m_ATK.AddDataOriginal(CountATK());//�˺�
	m_Wakan.AddDataOriginal(CountWakan());//����
	m_SPD.AddDataOriginal(CountSpd());//�ٶ�
	m_HIT.AddDataOriginal(CountHit());//����
}

void cPetData::AddProPertyDef(int num)
{
	//����Ӱ�� ���� ���� �ٶ�
	m_DEF.AddDataOriginal(-CountDEF());//��
	m_Wakan.AddDataOriginal(-CountWakan());//����
	m_SPD.AddDataOriginal(-CountSpd());//�ٶ�
	m_ProPertyDef += num;
	m_DEF.AddDataOriginal(CountDEF());//��
	m_Wakan.AddDataOriginal(CountWakan());//����
	m_SPD.AddDataOriginal(CountSpd());//�ٶ�
}

void cPetData::AddProPertyMag(int num)
{
	//���� Ӱ�� ���� ��ħ������
	m_MPMax.AddDataOriginal(-CountMP());//MP
	m_Wakan.AddDataOriginal(-CountWakan());//����
	m_ProPertyMag += num;
	m_MPMax.AddDataOriginal(CountMP());//MP
	m_Wakan.AddDataOriginal(CountWakan());//����
}

void cPetData::AddProPertyHp(int num)
{
	//���� Ӱ�� ��Ѫ���� ���� �ٶ� 
	m_HPMax.AddDataOriginal(-CountHP());//HP
	m_Wakan.AddDataOriginal(-CountWakan());//����
	m_SPD.AddDataOriginal(-CountSpd());//�ٶ�
	m_ProPertyHP += num;
	m_HPMax.AddDataOriginal(CountHP());//HP
	m_Wakan.AddDataOriginal(CountWakan());//����
	m_SPD.AddDataOriginal(CountSpd());//�ٶ�
}

void cPetData::AddProPertySpd(int num)
{
	//����Ӱ��  �ٶ� �Ͷ��
	m_SPD.AddDataOriginal(-CountSpd());//�ٶ�
	m_Evade.AddDataOriginal(-CountEvade());//����
	m_ProPertySpd += num;
	m_SPD.AddDataOriginal(CountSpd());//�ٶ�
	m_Evade.AddDataOriginal(CountEvade());//����
}

BOOL cPetData::CheckHavePetSkill(int SkillID)
{
	if (SkillID > 1999)SkillID -= 2000;
	for (int i = 0; i < m_NumofBBSkill; i++)
	{
		if (m_pPetSkill[i]->m_Id == SkillID)return TRUE;
	}
	return FALSE;
}

void cPetData::Set(sShangPinPet* pet)
{
	if (-1 == modeid)return;

	m_LV = 0;
	m_NumofBBSkill = 0;
	for (int i = 0; i < pet->m_skillid.size(); i++)
	{
		g_pMainState->m_SkillManager.SetPetSkill(this, pet->m_skillid[m_NumofBBSkill], m_NumofBBSkill);
	}
	//���ʳɳ�
	m_ZiZhi = pet->m_zizhi;
	AddProPertyAtk(30);
	AddProPertyDef(20);
	AddProPertyHp(20);
	AddProPertyMag(20);
	AddProPertySpd(20);
	m_Name = pet->m_name;
	m_dataFileName = m_Name;
}
void cPetData::PetEquip(int Pos,int bequip)
{
	if (Pos >= 12000)Pos -= 12000;
	if (0 == m_PetEquip[Pos].GetNum())return;
	sItem2& item = m_PetEquip[Pos];
	int nowid = 5;
	EquipExtra(item.m_Property, nowid,bequip);
	for (int i = 2; i < 5; i++)
		g_pMainState->m_SkillManager.SetSpecialSkill2(this, item.m_Property[i], bequip);
	
}
void cPetData::UnPetEquip(int Pos)
{
	PetEquip(Pos,-1);
}


cPCData::~cPCData()
{
	for (int i = m_NumofBB - 1; i >= 0; i--)
	{
		DeletePet(i);
	}

	if (m_pCardPetData)
		SAFE_DELETE(m_pCardPetData);
}

BOOL cPCData::DeletePet(int ID)
{
	if (ID < 0)return FALSE;
	if (m_NumofBB <= ID)return FALSE;
	if (m_pPetDataList[ID])
	{
		if (m_FightPetID == ID)return FALSE;
		if (m_FightPetID>ID)m_FightPetID -= 1;
		m_pPetDataList.erase(m_pPetDataList.begin() + ID);
		
		//SAFE_DELETE(m_pPetDataList[ID]);
		////ɾ����,��������BB��λ��
		//int i = ID;
		//for (; i < m_NumofBB - 1; i++)
		//{
		//	m_pPetDataList[i] = m_pPetDataList[i + 1];
		//}
		//m_pPetDataList[i] = 0;
		m_NumofBB -= 1;
	}
	return TRUE;
}

BOOL cPCData::GetPet(cPet* pPet)
{//�õ�һ���ٻ���
	if (!pPet)return FALSE;
	if (8 == m_NumofBB)return FALSE;
	m_pPetDataList[m_NumofBB] = pPet;
	m_NumofBB+=1;
	return TRUE;
}

cPCData::cPCData()
{
	Reset();
	m_pCardPetData = new cPet;
	m_pPetDataList.resize(8);
}



void cPCData::AutoPCData(
	string name, int pcmodeid,
	sAddPoint* addpoint,
	sZiZhi* zizhi, eMengPai mengpaid,int petid)
{
	m_Name = name;
	m_dataFileName = name;
	if (addpoint)
		m_AddPoint = *addpoint;
	else
	{
		sAddPoint addpoint2;
		m_AddPoint = addpoint2;
	}
	if (-1 != petid)
	{
		m_bUseCard = TRUE;
		m_CardPetID = petid;
		if (!m_pCardPetData)m_pCardPetData = new cPet;
		m_pCardPetData->SetData( g_pMainState->m_PetDataManager.GetPetData(m_CardPetID));
	}
	//���ʳɳ�
	if (zizhi)
		m_ZiZhi = *zizhi;
	else
	{
		if (-1 != petid)
		{
			m_ZiZhi.Set(m_pCardPetData->GetData()->m_ZiZhi,m_StrongLv , m_pCardPetData->GetData()->m_LVLimit);
		}
		else
		{
			m_ZiZhi.Set(1200, 1000, 2000, 4000, 1000, 1000, 50+g_pCharacter->m_PCData.m_LV/2);
		}
	}
	m_Popularity = g_pMainState->m_GlobalButton[0] * 10;
	if (mengpaid == eMP_���)
		mengpaid = (eMengPai)(rand() % 12);
	m_MengPaiID = mengpaid;
	modeid = pcmodeid;
	m_NumofBB = 0;
	AddProPertyAtk(30);
	AddProPertyDef(20);
	AddProPertyHp(20);
	AddProPertyMag(20);
	AddProPertySpd(20);
}

void cPCData::Reset()
{
	m_traderLV=0;
	m_traderExp=0;
	for (int i = 0; i < 8; i++)
		m_Accelarate[i] = -1;
	cPetData* pPetdata = this;
	pPetdata->Reset();
	//��
	m_face = 0;
	//����
	m_bUseCard=FALSE;
	m_CardPetID=-1;
	//m_CardRanSe.Reset();//��һ�����ʱ�ɱ����ٻ���
	m_pCardPetData=0;
	//����
	m_bOnZuoQi=FALSE; //�Ƿ���������
	zuoqiid=-1;  //����ID
	m_ZuoQiRanSe.partnum=0;//����Ⱦɫ
	m_ZhuangShiRanSe.partnum=0;//װ��Ⱦɫ
	m_ZuoQi.m_pZuoQi=0;
	//���е��ٻ�����
//	int  m_NumofBB = 0;
	m_FightPetID = -1;
	//����Ǯ�ӳ�
	m_SkillEnablelist.clear();
	m_MengPaiPoint=0;
	for (int i = 0; i < 6; i++)
	{
		m_SpecialSkillList[i] = -1;  //װ���ؼ�
	}
	//����
	 m_MengPaiID = -1;
	for (int i = 0; i < 7;i++)
	  m_MengPaiSkillLV[i]=0;
	   m_Popularity = 0;  //����
	//����
	m_BangPaiID = -1;
	 m_BangPaiPoint = 0;
	//����
	m_XiuLian.Reset();
	for (int i = 0; i < 3;i++)
	     m_Money[i]=0;       //Ǯ  ���� ������

}

void cPCData::SetCard(int id)
{
	m_CardPetID = id;
	if (id <0)return;
	if (!m_pCardPetData)m_pCardPetData = new cPet;
	m_pCardPetData->SetData(g_pMainState->m_PetDataManager.GetPetData(m_CardPetID));
}

BOOL cPCData::Equip(sItem2& item,int m_bEquip )
{
	if (!item.GetNum())return FALSE;
	if (m_bEquip==1)if (!CanEquip(item))return FALSE;
	if (!item.m_Property.size())
	{
		g_pMainState->m_Tags.Add("δ����");
		return FALSE;
	}
	int nowpos = 5;
	EquipExtra(item.m_Property, nowpos, m_bEquip);
	int pos;
	switch (item.GetType())
	{
	case 15:
	case 16:
		pos = 0;
		break;
	case 17:
	case 18:
		pos = 3;
		break;
	case 19:
		pos = 1;
		break;
	case 20:
		pos = 4;
		break;
	case 21:
		pos = 5;
		break;
	default:
		pos = 2;
		break;
	}
	if (m_bEquip)m_SpecialSkillList[pos] = item.m_Property[1];
	else m_SpecialSkillList[pos] = -1;
	for (int i = 2; i < 5; i++)
		g_pMainState->m_SkillManager.SetSpecialSkill2(this, item.m_Property[i], m_bEquip);

	if (2==pos)
	{
		m_pWeaponData2 = 0;
		m_pCharacterData2 = 0;
	}
	return TRUE;
}
BOOL cPCData::CanEquip(int type)
{
	return	g_pMainState->m_pCharacterDataList[modeid].CanEquip(type);
}
BOOL cPCData::CanEquip(sItem2& item)
{
	if (m_LV < g_pMainState->m_ItemManage.GetlvByID(item.GetType(), item.GetID()))return FALSE;
	return	g_pMainState->m_pCharacterDataList[modeid].CanEquip(item.GetType());
}


sItem2* cPCData::GetItem(int ID)
{
	if (ID >= 12000)//�ٻ���װ��
	{
		if (-1 == m_FightPetID)
			return 0;
		return &m_pPetDataList[m_FightPetID]->m_PetDataZZ.m_PetEquip[ID - 12000];
	}
	if (ID < 10000)//������Ʒ
		return &m_Item[ID];
	if (ID == 11000)//����װ��
		return &m_ZuoQi.m_ZhuangShi;
	return &m_Equip[ID-10000];//��ɫװ��
}

BOOL cPCData::CostBangGong(int num)
{
	if (m_BangPaiPoint < num)return FALSE;
	m_BangPaiPoint -= num;
	return TRUE;
}



BOOL cPCData::AddTraderExp(int num)
{
	m_traderExp += num;
	if (m_traderExp >= m_traderExpMax)
	{
		m_traderExp -= m_traderExpMax;
		m_traderLV -= 1;
		m_traderExpMax = GetCharacterExp(m_traderLV);
		return TRUE;
	}
	return FALSE;
}

void cPCData::Save(ofstream& File)
{
	cPetData* pPetdata = this;
	pPetdata->Save(File,FALSE);
	File << m_bDuJie << " ";
	File << m_face << " ";
	//���˵ȼ�
	File << m_traderLV << " ";
	File << m_traderExp << " ";
	//����
	File << m_BangPaiID << " ";
	File << m_BangPaiPoint << "\n";

	
	//����
	File << m_MengPaiID << " ";
	for (int i = 0; i < 7; i++)
	{
		File << m_MengPaiSkillLV[i] << " ";
	}
	File << "\n";
	File << m_MengPaiPoint << " ";
	File << "\n";
	//����
	File << m_Popularity << " ";
	//Ǯ  ���� ������
	for (int i = 0; i < 3; i++)
		File << m_Money[i] << " ";
	File << m_Physical << " ";	//����
	File << m_PhysicalMax << " ";	//��������
	File << m_Energy << " ";		//����
	File << m_EnergyMax << " ";	//��������
	File << m_CP << "\n";      //��ŭ

	//����
	m_XiuLian.Save(File);
	m_BBXiuLian.Save(File);
	//װ���ؼ�
	for (int i = 0; i < 6; i++)
		File << m_SpecialSkillList[i] << " ";
	File << "\n";
	//ʦ�ż���
	File << m_SkillEnablelist.size() << " ";
	for (int i = 0; i < m_SkillEnablelist.size(); i++)
	{
		File << m_SkillEnablelist[i]->m_Id << " ";
	}
	File << "\n";
	//��Ʒ
	for (int i = 0; i < 60; i++)
	{
		m_Item[i].Save(File);
	}
	//װ��
	for (int i = 0; i < 6; i++)
	{
		m_Equip[i].Save(File);
	}

	File << m_FightPetID << "\n";
	//�ٻ���
	File << m_NumofBB << "\n";
	for (int i = 0; i < m_NumofBB; i++)
	{
		File << 101 << "\n";
		if (i == m_FightPetID)
		{//��ս�ٻ��޵�����ȥ��
			m_pPetDataList[i]->m_PetDataZZ.m_XiuLian.Reset();
			m_pPetDataList[i]->m_PetDataZZ.Save(File);
			m_pPetDataList[i]->m_PetDataZZ.m_XiuLian=m_BBXiuLian;
		}
		else
		m_pPetDataList[i]->m_PetDataZZ.Save(File);
	}
	//����
	File <<m_bUseCard << " ";
	if (m_bUseCard)
	{
		File << GetCard() << " ";
		m_pCardPetData->m_PetDataZZ.m_RanSe.Save(File);
	}
	//����
	File << m_bOnZuoQi << " "; //�Ƿ���������
	File << zuoqiid << " ";

		m_ZuoQiRanSe.Save(File);
		m_ZuoQi.m_ZhuangShi.Save(File);//
		File << m_ZhuangShiRanSe.partnum << " \n";
		for (int i = 0; i < m_ZhuangShiRanSe.partnum; i++)//װ��Ⱦɫ
		{
			File << m_ZhuangShiRanSe.GetColorChange(i) << " ";
		}

	for (int i = 0; i < 8; i++)
		File << m_Accelarate[i] << " ";
	File << "\n\n";
}

void cPCData::Load(ifstream& File)
{
	cPetData* pPetdata = this;
	pPetdata->Load(File, FALSE);

	File >>m_bDuJie;
	File >> m_face;
	//���˵ȼ�
	File >> m_traderLV;
	File >> m_traderExp;
	m_traderExpMax = GetCharacterExp(m_traderLV);

	//����
	File >> m_BangPaiID;
	File >> m_BangPaiPoint;

	//����
	File >> m_MengPaiID;
	for (int i = 0; i < 7; i++)
	{
		File >> m_MengPaiSkillLV[i];
	}
	File >> m_MengPaiPoint;
	//����
	File >> m_Popularity;
	//Ǯ  ���� ������
	for (int i = 0; i < 3; i++)
		File >> m_Money[i];
	File >> m_Physical;	//����
	File >> m_PhysicalMax;	//��������
	File >> m_Energy;		//����
	File >> m_EnergyMax;	//��������
	File >> m_CP;      //��ŭ

	//����
	m_XiuLian.Load(File);
	m_BBXiuLian.Load(File);

	//װ���ؼ�
	for (int i = 0; i < 6; i++)
		File >> m_SpecialSkillList[i];
	//ʦ�ż���
	int numsmskill, smskillid;
	File >> numsmskill;
	for (int i = 0; i < numsmskill; i++)
	{
		File >> smskillid;
		sSkill* pSkill = g_pMainState->m_SkillManager.GetSkill(smskillid);
		m_SkillEnablelist.push_back(pSkill);
	}
	//��Ʒ
	for (int i = 0; i < 60; i++)
	{
		m_Item[i].Load(File);
	}

	for (int i = 0; i < 6; i++)
	{
		m_Equip[i].Load(File);
	}
	File >> m_FightPetID;
	//�ٻ���
	int mark;
	File >> m_NumofBB;
	int numofbb2 = m_NumofBB;
	for (int i = 0; i < numofbb2; i++)
	{
		File >> mark;
		if (!m_pPetDataList[i])m_pPetDataList[i] = new cPet;
		if (m_pPetDataList[i]->m_PetDataZZ.Load(File))
		{
			m_pPetDataList[i]->SetData(g_pMainState->m_PetDataManager.GetPetData(m_pPetDataList[i]->m_PetDataZZ.modeid));
		}
		else m_NumofBB -= 1;
	}
	//����
	File >> m_bUseCard;
	if (m_bUseCard)
	{
		int cardid;
		File >> cardid;
		SetCard(cardid);
		m_pCardPetData->m_PetDataZZ.m_RanSe.Read(cardid, File);
	}
	//����
	File >> m_bOnZuoQi; //�Ƿ���������
	File >> zuoqiid;
	g_pMainState->m_ZuoQi.GetZuoQi(*this, zuoqiid);

		m_ZuoQiRanSe.Read(File, zuoqiid, sChangeColorData::eZuoQi, 0);
		m_ZuoQi.m_ZhuangShi.Load(File);//��ȡ����װ��
		if (m_ZuoQi.m_ZhuangShi.GetNum()>0)
			m_ZhuangShiRanSe.Read(File, m_ZuoQi.m_ZhuangShi.GetID(), sChangeColorData::eZhuangShi, 0);
		else
			m_ZhuangShiRanSe.Read(File, 0, sChangeColorData::eZhuangShi, 0);
	for (int i = 0; i < 8; i++)
		File >>m_Accelarate[i];
	
}

int cPCData::FindItem(int type, int id)
{
	for (int i = 0; i < 60; i++)
	{
		if (m_Item[i].GetNum())
		{
			if (m_Item[i].GetType()==type)
			if (m_Item[i].GetID() == id)
			{
				return i;
			}
		}
	}
	return -1;
}

void cPCData::Followlv(int lv)
{
	if (lv == -1)lv = g_pCharacter->m_PCData.m_LV;
	//int prepclv = m_LV;
	

	if (m_LV != lv)
	{	
		LeveUp(-m_LV, FALSE);
		LeveUp(lv, FALSE);
	}


	int fuzhulv = lv;
	if (fuzhulv > 180)fuzhulv = 180;
	if (m_FuZhuSkillLvList[0]<g_pMainState->m_GlobalButton[4])
	for (int i = 0; i < 4; i++)
	{
		//ǿ�� ǿ׳
		lvUpFuZhuSkill(-m_FuZhuSkillLvList[i], i);
		//ǿ�� ǿ׳
		lvUpFuZhuSkill(fuzhulv, i);
	}

	sItem2 item;
	item.m_Num = 1;
	
			//��������
			int maxlv = lv - 20;
			if (maxlv < 0)maxlv = 0;
			maxlv /= 5;//��ǰ�ȼ�����������ȼ�
			maxlv = maxlv + m_StrongLv / 10 - 10;
			if (maxlv>30)maxlv = 30;
			if (maxlv < 0)maxlv = 0;
			if (maxlv>g_pMainState->m_GlobalButton[6])
				maxlv = g_pMainState->m_GlobalButton[6];
			int prexiu = m_XiuLian.GetAtk();
			if (maxlv != prexiu)
			{
				m_XiuLian.Set(maxlv,maxlv,maxlv,maxlv);
			}
			if (m_MengPaiID>-1)
			{
				//���ܸ���
				maxlv = lv - 10 + m_StrongLv / 5;
				if (maxlv > 179)maxlv = 180;
				prexiu = m_MengPaiSkillLV[0];
				int lvadd = maxlv - prexiu;
			
				if (prexiu != maxlv)
				{
					if (lvadd)
					for (int i = 0; i < 7; i++)
					{
						g_pMainState->m_SkillManager.lvUpMainSkill(*this, lvadd, i);
					}
				}
			}
			//�����ٻ���
			int petnum = m_NumofBB;
			maxlv = lv + 10;
			for (int i = 0; i < petnum; i++)
			{
				m_pPetDataList[i]->m_PetDataZZ.m_Popularity = m_Popularity;
				m_pPetDataList[i]->m_PetDataZZ.Followlv(maxlv);		
			}	
		
			if ((!m_Equip[2].m_Num) || g_pMainState->m_ItemManage.GetlvByID(m_Equip[2].GetType(), m_Equip[2].GetID())< lv-9)
			{	
				int baoshinum = maxlv / 10;
				if (baoshinum>g_pMainState->m_GlobalButton[5])
					baoshinum = g_pMainState->m_GlobalButton[5];
				//����,����,Ь��	
				for (int i = 0; i < 3; i++)
				{
					
					item.Set(19 + i, g_pMainState->m_ItemManage.GetIDByLv(19 + i, lv));
					g_pMainState->m_ItemManage.AutoItemProperty(&item, this);
					g_pMainState->m_ItemManage.AutoFuShi(&item, m_LV,(eMengPai)m_MengPaiID);
					sItem2 baoshi;
					int id;
					switch (item.GetType())
					{
					case 19://����
						id = 7;	
						break;
					case 20:
						id = 0;
						break;
					default:
						if (m_AddPoint.mp > 2)id = 8;
						else id = 1;
						break;
					}
					baoshi.Set(29,id);		
					for (int i2 = 0; i2<baoshinum; i2++)g_pMainState->m_ItemManage.XiangQian(item, baoshi);
					g_pMainState->m_InterfaceItem.Add(59, *this, &item);
				//	int def = m_DEF.GetData();////////
					g_pMainState->m_InterfaceItem.UseItem(59, *this);
				//	def = m_DEF.GetData();////////
				//	int k = 0;
				}
			
				//�·���ͷ�� ����Ů
				if (CanEquip(15))
					item.Set(15, g_pMainState->m_ItemManage.GetIDByLv(15, lv));
				else
					item.Set(16, g_pMainState->m_ItemManage.GetIDByLv(16, lv));
				g_pMainState->m_ItemManage.AutoItemProperty(&item, this);
				g_pMainState->m_ItemManage.AutoFuShi(&item, m_LV,(eMengPai)m_MengPaiID);
				sItem2 baoshi;
				int id;
				if (m_AddPoint.atk > 1)id = 2;
				else id = 10;
				baoshi.Set(29, id);
				for (int i2 = 0; i2 < baoshinum; i2++)g_pMainState->m_ItemManage.XiangQian(item, baoshi);

				g_pMainState->m_InterfaceItem.Add(59, *this, &item);
				g_pMainState->m_InterfaceItem.UseItem(59, *this);

	
				if (CanEquip(17))
					item.Set(17, g_pMainState->m_ItemManage.GetIDByLv(17, lv));
				else
					item.Set(18, g_pMainState->m_ItemManage.GetIDByLv(18, lv));

				g_pMainState->m_ItemManage.AutoItemProperty(&item, this);
				g_pMainState->m_ItemManage.AutoFuShi(&item, m_LV,eMengPai(m_MengPaiID));
				baoshi.Set(29, 10);
				for (int i2 = 0; i2 < baoshinum; i2++)g_pMainState->m_ItemManage.XiangQian(item, baoshi);

	
				g_pMainState->m_InterfaceItem.Add(59, *this, &item);
				g_pMainState->m_InterfaceItem.UseItem(59, *this);
				//����
				int typelist[2];
				GetWeaponType(modeid, typelist[0], typelist[1]);
				int k = typelist[rand() % 2];
				if (typelist[1] == -1)
					k = typelist[0];
				item.Set(k, g_pMainState->m_ItemManage.GetIDByLv(k, lv));
				g_pMainState->m_ItemManage.AutoItemProperty(&item, this);
				g_pMainState->m_ItemManage.AutoFuShi(&item, m_LV, eMengPai(m_MengPaiID));
				if (m_AddPoint.atk > 1)id = 2;
				else id = 8;

				baoshi.Set(29, id);
				for (int i2 = 0; i2 < baoshinum; i2++)g_pMainState->m_ItemManage.XiangQian(item, baoshi);

				g_pMainState->m_InterfaceItem.Add(59, *this, &item);
				g_pMainState->m_InterfaceItem.UseItem(59, *this);
			
				//��HP  0
				int itemid = 2 + g_pCharacter->m_PCData.m_LV / 17;
				if (itemid > 11)itemid = 11;
				m_Item[0].Set(22, itemid);
				m_Item[0].m_Num = 60;
				//ȫ���HP  1
				itemid = 12 + m_LV / 29;
				if (itemid > 18)itemid = 18;
				m_Item[1].Set(22, itemid);
				m_Item[1].m_Num = 20;
				//��MP  2
				itemid = 19 + m_LV / 29;
				if (itemid > 25)itemid = 25;
				m_Item[2].Set(22, itemid);
				m_Item[2].m_Num = 30;
				//ȫ���MP  3
				itemid = 26 + m_LV / 25;
				if (itemid > 33)itemid = 33;
				m_Item[3].Set(22, itemid);
				m_Item[3].m_Num = 20;
				//
				//��Ѫ����  4
				itemid = 40 + m_LV / 50;
				if (itemid > 43)itemid = 43;
				m_Item[4].Set(22, itemid);
				m_Item[4].m_Num = 30;
				//����  5
				itemid = 44 + m_LV / 100;
				if (itemid > 45)itemid = 45;
				m_Item[5].Set(22, itemid);
				m_Item[5].m_Num = 20;
				//���  6
				itemid = 49;
				m_Item[6].Set(22, itemid);
				m_Item[6].m_Num = 30;
				//��CP  7
				itemid = 54 + m_LV / 50;
				if (itemid > 57)itemid = 47;
				m_Item[7].Set(22, itemid);
				m_Item[7].m_Num = 30;
				
				//����  8
				itemid = m_LV / 17;
				if (itemid > 9)itemid = 9;
				m_Item[8].Set(25, itemid);
				m_Item[8].m_Num = 900;
			}
			FillHPMP();
}

BOOL cPCData::AddMoney(int xianjin, int chubei /*= 0*/, int cunyin /*= 0*/,BOOL bCost)
{
	int old[3];
	int data=0;
	for (int i = 0; i < 3; i++)
	{
		old[i] = m_Money[i];
	}
	BOOL ret = TRUE;
	if (xianjin)
	if (AddUpZero(m_Money[0], xianjin))ret = FALSE;
	if (cunyin)
	if (AddUpZero(m_Money[1], cunyin))ret = FALSE;
	if (chubei)
		data = AddUpZero(m_Money[2], chubei);
	if (data)if (AddUpZero(m_Money[0], data))ret = FALSE;
	if (!ret)
	if (!bCost)
	for (int i = 0; i < 3; i++)
	{
		m_Money[i] = old[i];
	}
	return ret;
}

BOOL cPCData::AddExp(INT64 exp, INT64 expbb/*=0*/,BOOL bCost)
{
	BOOL bRet=TRUE;
	if (exp)
	{
		EXP += exp;
		if (EXP < 0)
		{
			if (bCost)
				EXP = 0;
			else 
				EXP -= exp;
			bRet = FALSE;
		}
	}
	if (expbb)
	{	
		if (-1 < m_FightPetID)
		{
			INT64 & petexp = m_pPetDataList[m_FightPetID]->m_PetDataZZ.EXP;
			petexp += expbb;
			if (petexp < 0)
			{
				if (bCost)
				petexp = 0;
				else petexp -= exp;;
				bRet = FALSE;
			}
			else
			{
				g_pMainState->m_pFightState->CheckPetExp();
			}
		}
	}
	return bRet;
}

int& cPCData::GetCard()
{
	return m_CardPetID; 
}

void cPCData::GetShangPinPet(int ShangPinID)
{
	sShangPinPet pet;
	pet.LoadEasy(ShangPinID);
	GetShangPinPet2(&pet);
}
void cPCData::GetShangPinPet2(sShangPinPet* pet)
{
	cPet* pPet = new cPet;
	pPet->Set(pet, m_StrongLv);
	GetPet(pPet);
}

void cPCData::Read(string name, string path)
{
	path = path + name;
	path += ".txt";
	ifstream File;
	File.open(path);
	if (!File)ERRBOX;
		string dustbin;
		int intdustbin;
		File >> modeid;
		getline(File, dustbin);
		if (-1 == modeid)
		{//�ٻ���
			ERRBOX;
			/*bPet = TRUE;
			File >> modeid;*/
		}
		File >> m_CardPetID;
		if (m_CardPetID > -1)
		{

			if (0 == m_pCardPetData)
				m_pCardPetData = new cPet;
			m_pCardPetData->SetData(g_pMainState->m_PetDataManager.GetPetData(m_CardPetID));
			m_pCardPetData->m_PetDataZZ.m_JinJieNum =m_JinJieNum;
			m_bUseCard = TRUE;
		}
		else
		{
			m_bUseCard = FALSE;
			SAFE_DELETE(m_pCardPetData);
		}
		getline(File, dustbin);
		File >> m_Name;
		if (m_Name == "��")ERRBOX;
		getline(File, dustbin);
		m_ZiZhi.Load(File);
		getline(File, dustbin);
		m_AddPoint.Load(File);
		getline(File, dustbin);
		File >> m_MengPaiID;
		getline(File, dustbin);

		File >> m_NumofBBSkill;
		if (m_NumofBBSkill > 20)ERRBOX;
		getline(File, dustbin);
		for (int i = 0; i < m_NumofBBSkill; i++)
		{
			File >> intdustbin;
			m_pPetSkill[i]=g_pMainState->m_SkillManager.GetPetSkill(intdustbin) ;
		}
		File >> m_LiveHpLv;			getline(File, dustbin);
		File >> m_LiveMpLv;			getline(File, dustbin);
		File >> m_LiveCp;			getline(File, dustbin);
		nightLv=0;			
		File >> mpsavepercent;		getline(File, dustbin);
		File >> restoreRate;		getline(File, dustbin);
		File >> abnormalResist;		getline(File, dustbin);
		File >> magicWavePercent;	getline(File, dustbin);
		File >> comboNumDown;		getline(File, dustbin);
		File >> comboNumUp;			getline(File, dustbin);
		File >> ShiXueAdd;			getline(File, dustbin);
		File >> buffAdd;			getline(File, dustbin);
		File >> m_MagicNumAdd;		getline(File, dustbin);
		File >> intdustbin;			getline(File, dustbin);
		m_HPMax.SetDataAdd(intdustbin);

		File >> intdustbin;			getline(File, dustbin);
		m_MPMax.SetDataAdd(intdustbin);


		File >> intdustbin;			getline(File, dustbin);
		m_ATK.SetDataAdd(intdustbin);
		File >> intdustbin;			getline(File, dustbin);
		m_HIT.SetDataAdd(intdustbin);
		File >> intdustbin;			getline(File, dustbin);
		m_DEF.SetDataAdd(intdustbin);
		File >> intdustbin;			getline(File, dustbin);

		m_Wakan.SetDataAdd(intdustbin);
		File >> intdustbin;			getline(File, dustbin);
		m_SPD.SetDataAdd(intdustbin);
		File >> intdustbin;			getline(File, dustbin);
		m_Evade.SetDataAdd(intdustbin);
		GoldLivePercent.Load(File);	getline(File, dustbin);
		m_ParryRate.Load(File);		getline(File, dustbin);
		m_CriRate.Load(File);		getline(File, dustbin);
		m_MagicCriRate.Load(File);	getline(File, dustbin);
		m_CureCriRate.Load(File);	getline(File, dustbin);
		m_CriAddPercent.Load(File); getline(File, dustbin);
		m_Combo.Load(File);			getline(File, dustbin);
		m_MagicComboRate.Load(File); getline(File, dustbin);
		m_Pierce.Load(File);		getline(File, dustbin);
		m_PierceMagic.Load(File);	getline(File, dustbin);


		m_HitRate.Load(File);					getline(File, dustbin);
		m_MagicHitRate.Load(File);				getline(File, dustbin);
		m_SealRate.Load(File);					getline(File, dustbin);
		poisonRate.Load(File);					getline(File, dustbin);
		m_DamageAddPercentMagic2.Load(File);	getline(File, dustbin);
		m_DamageAddPercentMagic.Load(File);		getline(File, dustbin);
		m_DamagePercent.Load(File);				getline(File, dustbin);
		m_HiddenWeaponDamagePercent.Load(File);	getline(File, dustbin);
		m_DamageAddPercentPoision.Load(File);	getline(File, dustbin);
		m_DamageAddPercentCure.Load(File);		getline(File, dustbin);
		m_PetDamagePercentMagic.Load(File);		getline(File, dustbin);
		m_PetDamagePercent.Load(File);			getline(File, dustbin);
		m_DamageAddMagic.Load(File);			getline(File, dustbin);
		m_DamageAddMagicLv.Load(File);			getline(File, dustbin);
		m_DamageAdd.Load(File);					getline(File, dustbin);
		m_DamageAddLv.Load(File);				getline(File, dustbin);
		m_DamageAddCure.Load(File);				getline(File, dustbin);
		m_CounterRate.Load(File);				getline(File, dustbin);
		m_StabRate.Load(File);					getline(File, dustbin);
		m_StabRateMagic.Load(File);				getline(File, dustbin);
		m_StabDamageRate.Load(File);			getline(File, dustbin);
		m_StabDamageRateMagic.Load(File);		getline(File, dustbin);
		m_SuckBloodRate.Load(File);				getline(File, dustbin);

		getline(File, dustbin);
		m_AtkDownCP.Load(File);					getline(File, dustbin);
		m_StartMagic.Load(File);				getline(File, dustbin);
		m_MagDownHPHurt.Load(File);				getline(File, dustbin);
		m_AtkDownHPHurt.Load(File);				getline(File, dustbin);
		m_AtkDownMP.Load(File);					getline(File, dustbin);
		m_AtkDownAtk.Load(File);				getline(File, dustbin);
		m_AtkDownHit.Load(File);				getline(File, dustbin);
		m_AtkDownDef.Load(File);				getline(File, dustbin);
		m_AtkDownWakan.Load(File);				getline(File, dustbin);
		m_AtkDownSpd.Load(File);				getline(File, dustbin);
		m_Spurting.Load(File);				getline(File, dustbin);
		m_JinJiaRate.Load(File);				getline(File, dustbin);
		m_XiangMoRate.Load(File);				getline(File, dustbin);
		m_MagicFire.Load(File);				getline(File, dustbin);
		m_MagicSoil.Load(File);				getline(File, dustbin);
		m_MagicWater.Load(File);				getline(File, dustbin);
		m_MagicThunder.Load(File);				getline(File, dustbin);
		BoySpecial.Load(File);					getline(File, dustbin);
		GirlSpecial.Load(File);					getline(File, dustbin);
		GoldSpecial.Load(File);					getline(File, dustbin);
		MagSpecial.Load(File);					getline(File, dustbin);
		ImmortalSpecial.Load(File);				getline(File, dustbin);
		MonsterSpecial.Load(File);				getline(File, dustbin);
		PeopleSpecial.Load(File);				getline(File, dustbin);
		ghostSpecial.Load(File);				getline(File, dustbin);
		machineSpecial.Load(File);				getline(File, dustbin);
		File >> m_dataFileName; getline(File, dustbin);
		if (m_dataFileName == "��")
			m_dataFileName=name;
		File >> m_StrongLv; getline(File, dustbin);
		File >> m_AiLv;  getline(File, dustbin);
		File >> m_Popularity; getline(File, dustbin);
		m_RanSe.Read(2000+modeid, File);	

		m_ATK.SetDataOriginal(CountATK());
		m_DEF.SetDataOriginal(CountDEF());
		m_HPMax.SetDataOriginal(CountHP());
		m_HIT.SetDataOriginal(CountHit());
		m_MPMax.SetDataOriginal(CountMP());
		m_SPD.SetDataOriginal(CountSpd());
		m_Wakan.SetDataOriginal(CountWakan());
		m_Evade.SetDataOriginal(CountEvade());
		ExpMax = GetCharacterExp(m_LV);

		AddProPertyAtk(30);
		AddProPertyDef(20);
		AddProPertyHp(20);
		AddProPertyMag(20);
		AddProPertySpd(20);
		FillHPMP();
		File.close();
}



void cPetData::EquipExtra(vector<int>& prop, int& startid, BOOL m_bEquip)
{
	int property;

	for (int i = 0; i <prop[startid]; i++)
	{
		eProPerty  pro = (eProPerty)prop[startid + i * 2+1];
		property = prop[startid + i * 2 + 2] * m_bEquip;
		switch (pro)
		{
		case eProPerty�˺�:m_ATK.AddDataOriginal(property); break;
		case eProPerty����:m_HIT.AddDataOriginal(property); break;
		case eProPerty����:m_DEF.AddDataOriginal(property); break;
		case eProPerty����:m_Wakan.AddDataOriginal(property); break;
		case eProPerty�ٶ�:m_SPD.AddDataOriginal(property); break;
		case eProPerty��Ѫ:m_HPMax.AddDataOriginal(property); break;
		case eProPerty����:AddProPertyAtk(property); break;
		case eProPerty����:AddProPertySpd(property); break;
		case eProPerty����:AddProPertyHp(property); break;
		case eProPertyħ��:AddProPertyMag(property); break;
		case eProPerty����:AddProPertyDef(property); break;
		case eProPertyħ��:m_MPMax.AddDataOriginal(property); break;
		case eProPerty���:m_Evade.AddDataOriginal(property); break;
		case eProPertyˮ��:m_MagicWater.d2 += property;
		case eProPerty��:m_MagicFire.d2 += property;
		case eProPerty����:m_MagicSoil.d2 += property;
		case eProPerty�׿�:m_MagicThunder.d2 += property;

		default:ERRBOX; break;
		}
	}
	startid += prop[startid] * 2 + 1;
	if (m_HP > m_HPMax.GetData())
		m_HP = m_HPMax.GetData();
	if (m_HPHurtMax > m_HPMax.GetData())
		m_HPHurtMax = m_HPMax.GetData();
	if (m_MP > m_MPMax.GetData())
		m_MP = m_MPMax.GetData();
}

BOOL cPetData::Save(ofstream& File, BOOL bPet)
{
	File << modeid << " ";
	if (-1 == modeid)
		return FALSE;
	if (bPet)
	{
		m_JinJieNum.Save(File);
	}
	m_RanSe.Save(File);
	File << m_Name << "\n";
	File << m_dataFileName << " ";
	File << m_HP << "\n";
	File << m_HPHurtMax << " ";
	m_HPMax.Save(File);
	File << m_MP << "\n";
	m_MPMax.Save(File);
	File << m_CP << "\n";
	File << m_CPMax << "\n";

	File << m_LV << "\n";
	m_ZiZhi.Save(File);
	m_AddPoint.Save(File);
	//���Ե�
	File << m_ProPertyAtk << " ";
	File << m_ProPertyDef << " ";
	File << m_ProPertyMag << " ";
	File << m_ProPertyHP << " ";
	File << m_ProPertySpd << "\n";
	//����
	m_ATK.Save(File);
	m_HIT.Save(File);
	m_DEF.Save(File);
	m_Wakan.Save(File);
	m_SPD.Save(File);
	m_Evade.Save(File);

	File << EXP << " ";
	File << ExpMax << " ";
	File << m_race << " ";//����
	File << m_AiLv << " ";
	File << m_StrongLv << "\n";

	File << m_NumofBBSkill << "\n";
	for (int i = 0; i < m_NumofBBSkill; i++)
		File << m_pPetSkill[i]->m_Id << " ";
	File << "\n";
	m_Controler.Save(File);
	for (int i = 0; i < 3; i++)
		m_PetEquip[i].Save(File);
	File << "\n";
	//��������
	for (int i = 0; i < c_NumFuZhu; i++)
	 File<< m_FuZhuSkillLvList[i]<<" ";
	//
	File << "\n";
	File << m_LiveHpLv << " ";
	File << m_LiveMpLv << " ";
	File << m_LiveCp << " ";
	File << m_Physical << " ";
	File << m_PhysicalMax << " ";
	File << m_Energy << " ";
	File << m_EnergyMax << " ";
	File << nightLv << " ";
	File << mpsavepercent << " ";
	File << restoreRate << " ";
	File << abnormalResist << " ";
	File << magicWavePercent << " ";
	File << comboNumDown << " ";
	File << comboNumUp << " ";
	File << ShiXueAdd << " ";
	File << buffAdd << " ";
	File << m_MagicNumAdd << "\n";

	 GoldLivePercent.Save(File);//���
	 m_ParryRate.Save(File); //����  
	 m_CriRate.Save(File); //���� //��ɱ��
	 m_MagicCriRate.Save(File); //����
	 m_CureCriRate.Save(File);//����

	 m_CriAddPercent.Save(File); //��ɱ�˺�����
	 m_Combo.Save(File);  //������
	 m_MagicComboRate.Save(File); //������

	 m_Pierce.Save(File);//���ӷ�����
	 m_PierceMagic.Save(File); //����������

	//������  �����
	 m_HitRate.Save(File);  //����
	 m_MagicHitRate.Save(File);//����
	 m_SealRate.Save(File);//��ӡ
	 poisonRate.Save(File); //��
	//�˺��ӳ�  �����ӳ�
	 m_DamageAddPercentMagic2.Save(File);//�̶�
	 m_DamageAddPercentMagic.Save(File);//����
	 m_DamagePercent.Save(File);//����
	 m_HiddenWeaponDamagePercent.Save(File); //����
	 m_DamageAddPercentPoision.Save(File); //��
	 m_DamageAddPercentCure.Save(File);//����
	//�ٻ����˺��ӳ�

	 m_PetDamagePercentMagic.Save(File);//�����˺�
	 m_PetDamagePercent.Save(File);//�����˺�
	//�˺�����
	 m_DamageAddMagic.Save(File);//�˺��������
	 m_DamageAddMagicLv.Save(File);//�����˺��������lv
	 m_DamageAdd.Save(File);//�����˺��������
	 m_DamageAddLv.Save(File);  //�����˺����
	//����
	 m_CounterRate.Save(File);
	//����
	 m_StabRate.Save(File);
	 m_StabRateMagic.Save(File);    //���������˺�
	 m_StabDamageRate.Save(File);
	 m_StabDamageRateMagic.Save(File); //����������
	//
	 m_SuckBloodRate.Save(File);//��Ѫ��
	//
	 m_DamageAddCure.Save(File);//��������
	//����������,��������������
	 m_AtkDownCP.Save(File);//����CP
	 m_StartMagic.Save(File);
	 m_MagDownHPHurt.Save(File);
	 m_AtkDownHPHurt.Save(File);
	 m_AtkDownMP.Save(File);
	 m_AtkDownAtk.Save(File);
	 m_AtkDownHit.Save(File);
	 m_AtkDownDef.Save(File);
	 m_AtkDownWakan.Save(File);
	 m_AtkDownSpd.Save(File);
	 m_Spurting.Save(File);
	//
	//������ºͽ�ħ����Ч��
	 m_JinJiaRate.Save(File);
	 m_XiangMoRate.Save(File);
	//�ع�
	 m_MagicFire.Save(File); //��
	 m_MagicSoil.Save(File); //��
	 m_MagicWater.Save(File);  //ˮ
	 m_MagicThunder.Save(File);//��

	 BoySpecial.Save(File);  //��
	 GirlSpecial.Save(File);   //Ů
	 GoldSpecial.Save(File);     //�����ع�
	 MagSpecial.Save(File);    //ħ���ع�
	 ImmortalSpecial.Save(File); //�����ع�
	 MonsterSpecial.Save(File);//�����ع�
	 PeopleSpecial.Save(File); //��
	 ghostSpecial.Save(File);  //���ع�
	 machineSpecial.Save(File);//��е�ع�
	 if (bPet)
	 {
		 m_JinJieNum.Save(File);
	 }
	 File << "\n\n\n";
	 return TRUE;
}

BOOL cPetData::Load(ifstream& File,BOOL bPet)
{
	File >> modeid;
	if (-1 == modeid)
		return FALSE;
	if (bPet)
	{
		m_JinJieNum.Load(File);
		m_RanSe.Read(modeid, File, m_JinJieNum.m_PositionID);
	}
	else
	{
		m_RanSe.Read(modeid+2000, File, m_JinJieNum.m_PositionID);
	}


	File >> m_Name;
	File >> m_dataFileName ;
	File >> m_HP;
	File >> m_HPHurtMax;
	m_HPMax.Load(File);
	File >> m_MP;
	m_MPMax.Load(File);
	File >> m_CP;
	File >> m_CPMax ;

	File >> m_LV ;
	m_ZiZhi.Load(File);
	m_AddPoint.Load(File);
	//���Ե�
	File >> m_ProPertyAtk;
	File >> m_ProPertyDef;
	File >> m_ProPertyMag;
	File >> m_ProPertyHP ;
	File >> m_ProPertySpd;
	//����
	m_ATK.Load(File);
	m_HIT.Load(File);
	m_DEF.Load(File);
	m_Wakan.Load(File);
	m_SPD.Load(File);
	m_Evade.Load(File);

	File >> EXP;
	File >> ExpMax ;
	File >> m_race ;
	File >> m_AiLv ;
	File >> m_StrongLv;

	File >> m_NumofBBSkill;
	int petskillid;
	for (int i = 0; i < m_NumofBBSkill; i++)
	{
		File >> petskillid;
		m_pPetSkill[i]=g_pMainState->m_SkillManager.GetPetSkill(petskillid);
	}
	m_Controler.Load(File);
	for (int i = 0; i < 3; i++)
		m_PetEquip[i].Load(File);
	//��������
	for (int i = 0; i < c_NumFuZhu; i++)
		File >> m_FuZhuSkillLvList[i];
	//
	File >> m_LiveHpLv;
	File >> m_LiveMpLv;
	File >> m_LiveCp  ;
	File >> m_Physical	;
	File >> m_PhysicalMax;
	File >> m_Energy;
	File >> m_EnergyMax;
	File >> nightLv;
	File >> mpsavepercent;
	File >> restoreRate;
	File >> abnormalResist;
	File >> magicWavePercent;
	File >> comboNumDown;
	File >> comboNumUp;
	File >> ShiXueAdd ;
	File >> buffAdd;
	File >> m_MagicNumAdd;

	GoldLivePercent.Load(File);//���
	m_ParryRate.Load(File); //����  
	m_CriRate.Load(File); //���� //��ɱ��
	m_MagicCriRate.Load(File); //����
	m_CureCriRate.Load(File);//����

	m_CriAddPercent.Load(File); //��ɱ�˺�����
	m_Combo.Load(File);  //������
	m_MagicComboRate.Load(File); //������

	m_Pierce.Load(File);//���ӷ�����
	m_PierceMagic.Load(File); //����������

	//������  �����
	m_HitRate.Load(File);  //����
	m_MagicHitRate.Load(File);//����
	m_SealRate.Load(File);//��ӡ
	poisonRate.Load(File); //��
	//�˺��ӳ�  �����ӳ�
	m_DamageAddPercentMagic2.Load(File);//�̶�
	m_DamageAddPercentMagic.Load(File);//����
	m_DamagePercent.Load(File);//����
	m_HiddenWeaponDamagePercent.Load(File); //����
	m_DamageAddPercentPoision.Load(File); //��
	m_DamageAddPercentCure.Load(File);//����
	//�ٻ����˺��ӳ�
	m_PetDamagePercentMagic.Load(File);//�����˺�
	m_PetDamagePercent.Load(File);//�����˺�
	//�˺�����
	m_DamageAddMagic.Load(File);//�˺��������
	m_DamageAddMagicLv.Load(File);//�����˺��������lv
	m_DamageAdd.Load(File);//�����˺��������
	m_DamageAddLv.Load(File);  //�����˺����
	//����
	m_CounterRate.Load(File);
	//����
	m_StabRate.Load(File);
	m_StabRateMagic.Load(File);    //���������˺�
	m_StabDamageRate.Load(File);
	m_StabDamageRateMagic.Load(File); //����������
	//
	m_SuckBloodRate.Load(File);//��Ѫ��
	//
	m_DamageAddCure.Load(File);//������Ч��
	//����������,��������������
	m_AtkDownCP.Load(File);//����CP
	m_StartMagic.Load(File);
	m_MagDownHPHurt.Load(File);
	m_AtkDownHPHurt.Load(File);
	m_AtkDownMP.Load(File);
	m_AtkDownAtk.Load(File);
	m_AtkDownHit.Load(File);
	m_AtkDownDef.Load(File);
	m_AtkDownWakan.Load(File);
	m_AtkDownSpd.Load(File);
	m_Spurting.Load(File);
	//

	//������ºͽ�ħ����Ч��
	m_JinJiaRate.Load(File);
	m_XiangMoRate.Load(File);
	//�ع�
	m_MagicFire.Load(File); //��
	m_MagicSoil.Load(File); //��
	m_MagicWater.Load(File);  //ˮ
	m_MagicThunder.Load(File);//��

	BoySpecial.Load(File);  //��
	GirlSpecial.Load(File);   //Ů
	GoldSpecial.Load(File);     //�����ع�
	MagSpecial.Load(File);    //ħ���ع�
	ImmortalSpecial.Load(File); //�����ع�
	MonsterSpecial.Load(File);//�����ع�
	PeopleSpecial.Load(File); //��
	ghostSpecial.Load(File);  //���ع�
	machineSpecial.Load(File);//��е�ع�

	if (bPet)m_JinJieNum.Load(File);
	return TRUE;
}

void cPetData::FillHPMP()
{
	m_HPHurtMax = m_HPMax.GetData();
	m_HP  = m_HPMax.GetData();
	m_MP = m_MPMax.GetData();
}

void cPetData::Followlv(int lv /*= -1*/)
{
	if (lv == -1)lv = g_pCharacter->m_PCData.m_LV+10;
	int stronglv;
	stronglv = m_StrongLv;
	if (stronglv > 100)stronglv = 100;
	if (stronglv < 1)stronglv = 1;
//	int prepclv = m_LV;
	if (m_LV != lv)
	{
		LeveUp(-m_LV, FALSE);
		LeveUp(lv, FALSE);
		
	}
	for (int i = 0; i < 4; i++)
	{
		//ǿ�� ǿ׳
		lvUpFuZhuSkill(-m_FuZhuSkillLvList[i], i);
		//ǿ�� ǿ׳
		lvUpFuZhuSkill(lv, i);
	}

	sItem2 item;
	item.m_Num = 1;
	int itemid = (lv-5) / 10;


	if (itemid > 15)itemid = 15;
	if (0 == m_PetEquip[0].GetNum() || m_PetEquip[0].GetID()  < itemid)
	{
		int baoshinum = itemid;
		if (baoshinum > g_pMainState->m_GlobalButton[5])
			baoshinum = g_pMainState->m_GlobalButton[5];
		for (int j = 0; j < 3; j++)
		{
			
			sItem2 baoshi;
			int baoshiid = -1;
			item.Set(32 + j, itemid);
			g_pMainState->m_ItemManage.AutoItemProperty(&item,m_Popularity,1);
			g_pMainState->m_ItemManage.AutoFuShi(&item, m_LV,eMP_��);
			switch (j)
			{
			case 0:
				if (m_AddPoint.atk>2)
					baoshiid = 15;
				else
					baoshiid = 12;
				break;
			case 1:
				baoshiid = 11;
				break;
			case 2:
				baoshiid = 16;
				break;
			}
			baoshi.Set(29, baoshiid);

			for (int i = 0; i < baoshinum; i++)
				g_pMainState->m_ItemManage.XiangQian(item, baoshi);
			m_PetEquip[j] = item;
			PetEquip(j, 1);
		}
	}
	FillHPMP();
}

void cPetData::lvUpFuZhuSkill(int lv, int FuZhuSkillID)
{
	if (179 < m_FuZhuSkillLvList[FuZhuSkillID])return;
	if (m_FuZhuSkillLvList[FuZhuSkillID] + lv > 180)lv = 180 - m_FuZhuSkillLvList[FuZhuSkillID];
	m_FuZhuSkillLvList[FuZhuSkillID] += lv;
	switch (FuZhuSkillID)
	{
	case 0: //ǿ׳
		m_HPMax.AddDataAdd(10 * lv);
		break;
	case 1: //ǿ��
		m_HPMax.AddDataAddPercent(lv);
		break;
	case 2: //ڤ��
		m_MPMax.AddDataAddPercent(lv);
		break;
	case 3:  //����
		m_HiddenWeaponDamagePercent.d1 -= m_FuZhuSkillLvList[FuZhuSkillID] / 5;
		m_HiddenWeaponDamagePercent.d1 += (m_FuZhuSkillLvList[FuZhuSkillID] + lv) / 5;
		break;
	case 4:  //����
	case 5:  //�÷�
	case 6:  //����
	case 7:  //��ҩ
	case 8:  //���
		break;
	case 9:  //����
		break;
	case 10:  //׷��
		break;
	case 11:
		m_EnergyMax += 5;
		break;
	case 12:
		m_PhysicalMax += 5;
		break;
	default:
		ERRBOX;
		return ;
	}
	
}

void cPetData::AddSkill(ePetSkill id)
{
	g_pMainState->m_SkillManager.SetPetSkill(this, id);
}

void cPetData::AutoGetSkill(int id)
{
	sPetData* pPetdata= g_pMainState->m_PetDataManager.GetPetData(id);
	for (int i = 0; i < pPetdata->m_SkillNum; i++)
	{
		if (rand() % 100 >= (100 - m_StrongLv)) //ÿ��������10%-100%�ļ��ʿ��Եõ�
		{
			g_pMainState->m_SkillManager.SetPetSkill(this, pPetdata->m_pPetSkill[i]->m_Id, m_NumofBBSkill);
		}
	}
}

void cPetData::SetJinJie(int num)
{
	if (m_JinJieNum.m_JinJieNum > 2)return;
	if (m_JinJieNum.m_JinJieNum + num > 3)num = 3 - m_JinJieNum.m_JinJieNum;
	AddProPertyAtk(m_AddPoint.atk * 10 * num);
	AddProPertyDef(m_AddPoint.def * 10 * num);
	AddProPertySpd(m_AddPoint.spd * 10 * num);
	AddProPertyHp(m_AddPoint.hp* 10 * num);
	AddProPertyMag(m_AddPoint.mp * 10 * num);
	m_JinJieNum.m_JinJieNum += num;
	m_JinJieNum.m_PositionID += num;
	if (m_JinJieNum.m_JinJieNum>1)
		m_RanSe.m_ranse[0].partnum = 0;
	FillHPMP();
}

void cPetData::ResetAddPoint()
{
	if (m_AddPoint.idle)return;
	int propertynum;
	//��¼���Ե�����
	propertynum = m_ProPertyAtk + m_ProPertyDef + m_ProPertySpd + m_ProPertyHP + m_ProPertyMag-100-m_LV*5;
	int idle = propertynum % 5;//����������Ե�0-4���Զ��ӵ�������
	propertynum /= 5;
	//�Ȱ����е����Ե�ȥ��
	AddProPertyAtk(-m_ProPertyAtk);
	AddProPertyDef(-m_ProPertyDef);
	AddProPertySpd(-m_ProPertySpd);
	AddProPertyHp(-m_ProPertyHP);
	AddProPertyMag(-m_ProPertyMag);
	int num[5];
	for (int i = 0; i < 5; i++)
	{//��������
		num[i] = 20 + m_LV;
	}
	num[0] += propertynum*m_AddPoint.atk;
	num[1] += propertynum*m_AddPoint.def;
	num[2] += propertynum*m_AddPoint.hp+idle;
	num[3] += propertynum*m_AddPoint.mp;
	num[4] += propertynum*m_AddPoint.spd;
	//���·���
	AddProPertyAtk(num[0]);
	AddProPertyDef(num[1]);
	AddProPertySpd(num[4]);
	AddProPertyHp(num[2]);
	AddProPertyMag(num[3]);
}

void cPetData::AddPopularity(int num)
{
	if (m_Popularity > 1000)return;
	m_Popularity += num;
}

void cPetData::SetXLEffect(BOOL bSetup)
{
	if (m_bSetupXiuLian==bSetup)return;
	int up=-1;
	if (bSetup)up = 1;
	m_bSetupXiuLian = bSetup;
	m_DamagePercent.d1 += 2*m_XiuLian.GetAtk()*up;
	m_DamagePercent.d2 += 2 * m_XiuLian.GetDef()*up;
	int add;
	add = up * 2 * m_XiuLian.GetMag();
	m_DamageAddPercentMagic.d1 +=add;
	m_SealRate.d1 +=add;
	m_DamageAddPercentCure.d1 += add;
	add = up * 2 * m_XiuLian.GetDMag();
	m_DamageAddPercentMagic.d2 += add;
	m_SealRate.d2 += add;

}

void cPetData::ReadPet(string name, string path)
{
	path = path + name;
	path += ".txt";
	ifstream File;
	File.open(path);
	if (!File)ERRBOX;
	string dustbin;
	int intdustbin;
	File >> modeid;
	getline(File, dustbin);
	if (-1 != modeid)
	{
		return;
	}
	File >> modeid;
	getline(File, dustbin);
	File >> m_Name;
	if (m_Name == "��")ERRBOX;
	getline(File, dustbin);
	m_ZiZhi.Load(File);
	getline(File, dustbin);
	m_AddPoint.Load(File);
	getline(File, dustbin);
	int mengpai;
	File >> mengpai;
	getline(File, dustbin);

	File >> m_NumofBBSkill;
	if (m_NumofBBSkill > 20)ERRBOX;
	getline(File, dustbin);
	for (int i = 0; i < m_NumofBBSkill; i++)
	{
		File >> intdustbin;
		m_pPetSkill[i] = g_pMainState->m_SkillManager.GetPetSkill(intdustbin);
	}
	File >> m_LiveHpLv;			getline(File, dustbin);
	File >> m_LiveMpLv;			getline(File, dustbin);
	File >> m_LiveCp;			getline(File, dustbin);
	nightLv = 0;
	File >> mpsavepercent;		getline(File, dustbin);
	File >> restoreRate;		getline(File, dustbin);
	File >> abnormalResist;		getline(File, dustbin);
	File >> magicWavePercent;	getline(File, dustbin);
	File >> comboNumDown;		getline(File, dustbin);
	File >> comboNumUp;			getline(File, dustbin);
	File >> ShiXueAdd;			getline(File, dustbin);
	File >> buffAdd;			getline(File, dustbin);
	File >> m_MagicNumAdd;		getline(File, dustbin);
	File >> intdustbin;			getline(File, dustbin);
	m_HPMax.SetDataAdd(intdustbin);

	File >> intdustbin;			getline(File, dustbin);
	m_MPMax.SetDataAdd(intdustbin);


	File >> intdustbin;			getline(File, dustbin);
	m_ATK.SetDataAdd(intdustbin);
	File >> intdustbin;			getline(File, dustbin);
	m_HIT.SetDataAdd(intdustbin);
	File >> intdustbin;			getline(File, dustbin);
	m_DEF.SetDataAdd(intdustbin);
	File >> intdustbin;			getline(File, dustbin);

	m_Wakan.SetDataAdd(intdustbin);
	File >> intdustbin;			getline(File, dustbin);
	m_SPD.SetDataAdd(intdustbin);
	File >> intdustbin;			getline(File, dustbin);
	m_Evade.SetDataAdd(intdustbin);
	GoldLivePercent.Load(File);	getline(File, dustbin);
	m_ParryRate.Load(File);		getline(File, dustbin);
	m_CriRate.Load(File);		getline(File, dustbin);
	m_MagicCriRate.Load(File);	getline(File, dustbin);
	m_CureCriRate.Load(File);	getline(File, dustbin);
	m_CriAddPercent.Load(File); getline(File, dustbin);
	m_Combo.Load(File);			getline(File, dustbin);
	m_MagicComboRate.Load(File); getline(File, dustbin);
	m_Pierce.Load(File);		getline(File, dustbin);
	m_PierceMagic.Load(File);	getline(File, dustbin);


	m_HitRate.Load(File);					getline(File, dustbin);
	m_MagicHitRate.Load(File);				getline(File, dustbin);
	m_SealRate.Load(File);					getline(File, dustbin);
	poisonRate.Load(File);					getline(File, dustbin);
	m_DamageAddPercentMagic2.Load(File);	getline(File, dustbin);
	m_DamageAddPercentMagic.Load(File);		getline(File, dustbin);
	m_DamagePercent.Load(File);				getline(File, dustbin);
	m_HiddenWeaponDamagePercent.Load(File);	getline(File, dustbin);
	m_DamageAddPercentPoision.Load(File);	getline(File, dustbin);
	m_DamageAddPercentCure.Load(File);		getline(File, dustbin);
	m_PetDamagePercentMagic.Load(File);		getline(File, dustbin);
	m_PetDamagePercent.Load(File);			getline(File, dustbin);
	m_DamageAddMagic.Load(File);			getline(File, dustbin);
	m_DamageAddMagicLv.Load(File);			getline(File, dustbin);
	m_DamageAdd.Load(File);					getline(File, dustbin);
	m_DamageAddLv.Load(File);				getline(File, dustbin);
	m_DamageAddCure.Load(File);				getline(File, dustbin);
	m_CounterRate.Load(File);				getline(File, dustbin);
	m_StabRate.Load(File);					getline(File, dustbin);
	m_StabRateMagic.Load(File);				getline(File, dustbin);
	m_StabDamageRate.Load(File);			getline(File, dustbin);
	m_StabDamageRateMagic.Load(File);		getline(File, dustbin);
	m_SuckBloodRate.Load(File);				getline(File, dustbin);

	getline(File, dustbin);
	m_AtkDownCP.Load(File);					getline(File, dustbin);
	m_StartMagic.Load(File);				getline(File, dustbin);
	m_MagDownHPHurt.Load(File);				getline(File, dustbin);
	m_AtkDownHPHurt.Load(File);				getline(File, dustbin);
	m_AtkDownMP.Load(File);					getline(File, dustbin);
	m_AtkDownAtk.Load(File);				getline(File, dustbin);
	m_AtkDownHit.Load(File);				getline(File, dustbin);
	m_AtkDownDef.Load(File);				getline(File, dustbin);
	m_AtkDownWakan.Load(File);				getline(File, dustbin);
	m_AtkDownSpd.Load(File);				getline(File, dustbin);
	m_Spurting.Load(File);				getline(File, dustbin);
	m_JinJiaRate.Load(File);				getline(File, dustbin);
	m_XiangMoRate.Load(File);				getline(File, dustbin);
	m_MagicFire.Load(File);				getline(File, dustbin);
	m_MagicSoil.Load(File);				getline(File, dustbin);
	m_MagicWater.Load(File);				getline(File, dustbin);
	m_MagicThunder.Load(File);				getline(File, dustbin);
	BoySpecial.Load(File);					getline(File, dustbin);
	GirlSpecial.Load(File);					getline(File, dustbin);
	GoldSpecial.Load(File);					getline(File, dustbin);
	MagSpecial.Load(File);					getline(File, dustbin);
	ImmortalSpecial.Load(File);				getline(File, dustbin);
	MonsterSpecial.Load(File);				getline(File, dustbin);
	PeopleSpecial.Load(File);				getline(File, dustbin);
	ghostSpecial.Load(File);				getline(File, dustbin);
	machineSpecial.Load(File);				getline(File, dustbin);
	getline(File, m_dataFileName);
	if (m_dataFileName == "��")
		m_dataFileName = name;
	File >> m_StrongLv; getline(File, dustbin);
	File >> m_AiLv;  getline(File, dustbin);
	File >> m_Popularity; getline(File, dustbin);
	m_RanSe.Read(modeid, File);

	m_ATK.SetDataOriginal(CountATK());
	m_DEF.SetDataOriginal(CountDEF());
	m_HPMax.SetDataOriginal(CountHP());
	m_HIT.SetDataOriginal(CountHit());
	m_MPMax.SetDataOriginal(CountMP());
	m_SPD.SetDataOriginal(CountSpd());
	m_Wakan.SetDataOriginal(CountWakan());
	m_Evade.SetDataOriginal(CountEvade());
	ExpMax = GetCharacterExp(m_LV);

	AddProPertyAtk(30);
	AddProPertyDef(20);
	AddProPertyHp(20);
	AddProPertyMag(20);
	AddProPertySpd(20);
	FillHPMP();
	File.close();
}



void cWfileP::Set(CWFile* p)
{
	m_pFile = p;
	m_NextFrame = 0;
	now_frame = 0;
	m_NextFrame = 0;
	m_Width = p->GetWidth();
	m_Height = p->GetHeight();
}



cWfileP::cWfileP()
{
	m_PreShowRect.right = -1;
	m_NeedShow = TRUE;
	m_bMove = FALSE;
	m_bStatic = FALSE;
	m_pFile = 0;
	m_direction = 1;
	
}

RECT cWfileP::GetPicRange()
{
	int a = GetDataID();
	RECT rect;
	
	rect.left = m_x - m_pFile->m_Picture.Spr.m_pDataOffsetList[a ].FramX;
	rect.top = m_y - m_pFile->m_Picture.Spr.m_pDataOffsetList[a].FramY;
	if (0 == m_pFile->GetShowWidth())
		rect.right = m_x + m_pFile->m_Picture.m_Width;
	else
		rect.right = m_x +m_pFile->GetShowWidth();
	if (0 == m_pFile->GetShowHeight())
		rect.bottom = m_y + m_pFile->m_Picture.m_Height;
	else
		rect.bottom = m_y + m_pFile->GetShowHeight();
	return rect;
}
BOOL cWfileP::NextFrame()
{
	if (m_bStatic)return FALSE;

	if (m_FrameGap <= m_NextFrame)
	{
		m_NextFrame = 0;
		(now_frame < (m_pFile->m_Picture.Spr.GetMaxFrame() - 1)) ? (now_frame++) : (now_frame = 0);
		return TRUE;
	}
	else
		m_NextFrame += 1;
	return FALSE;
}

void cWfileP::operator=(cWfileP& file)
{
	m_x = file.m_x;
	m_y = file.m_y;
	m_xOffset = file.GetxOffset();
	m_xOffset2 = file.GetxOffset2();
	m_yOffset = file.GetyOffset();
	m_yOffset2 = file.GetyOffset2();
	m_direction = file.m_direction;
	m_pFile= file.m_pFile;
}



int cWfileP::GetCenter()
{
	return m_x + (GetShowWidth()>>1);
}

void WPixel::operator=(WORD color)
{
	blue = color & 0x1f;
	green = (color>>5 & 0x1f)<<1;
	if (green)green += 1;
	red = color>>10 & 0x1f;
}
void WPixel::operator=(DWORD color1)
{
	red = color1 >> 3 & 0x1f;
	green = (color1 >> 11 & 0x1f )<<1;
	if (green)green += 1;
	blue = color1 >> 19 & 0x1f;
}



void sFaceText::SetPos(int x, int y)
{
	int bottom = y + m_Gap + m_TextLineList[0].m_Height;
	SIZE  size1;

	//int firstgap = 0;//
	//��һ�л�Ҫ���ȥƵ��ͼƬ�Ŀ��
	GetTextExtentPointA(g_DC, m_TextLineList[0].m_line.GetString().c_str(), 3, &size1);
	m_TextLineList[0].m_line.SetXY(x + m_FirstAdd, bottom-size1.cy);
	switch (m_ShowStyle)
	{
	case 2:
		GetTextExtentPointA(g_DC, m_Name.GetString().c_str(), 3, &size1);
		m_Name.SetXY(x + 30, bottom - size1.cy);
	case 1:
		m_Channel.SetXY(x, bottom);
		break;
	}

	for (int j = 0; j < m_TextLineList[0].m_NumFace; j++)
	{
		m_TextLineList[0].m_Face[j].m_x = x + m_FirstAdd + m_TextLineList[0].m_xFace[j];
		m_TextLineList[0].m_Face[j].m_y = bottom+5;
	}
	for (int i = 1; i < m_LineNum; i++)
	{
		bottom += m_Gap + m_TextLineList[i].m_Height;
		GetTextExtentPointA(g_DC, m_TextLineList[i].m_line.GetString().c_str(), 3, &size1);
		m_TextLineList[i].m_line.SetXY(x, bottom - size1.cy);
		for (int j = 0; j < m_TextLineList[i].m_NumFace; j++)
		{
			m_TextLineList[i].m_Face[j].m_x = x +  m_TextLineList[i].m_xFace[j];
			m_TextLineList[i].m_Face[j].m_y = bottom;
		}
	}
}
BOOL sFaceText::SetFaceText(string tex, HFONT hFont, int LineWidth)
{
	return SetFaceText(0 , "", tex,  hFont,  LineWidth);
}
BOOL sFaceText::SetFaceText(CWFile* pChannel, string name, string tex, HFONT hFont, int LineWidth)
{
	for (int i = 0; i <m_TextLineList.size(); i++)
	{
		m_TextLineList[i].m_NumFace = 0;
	}
	m_ColorTag.clear();
	SIZE size1;
	cInterfaceChannel* p = &g_pMainState->m_Channel;
	HFONT oldFont = (HFONT)SelectObject(g_DC, hFont);
	char line[99];
	if (pChannel)
	m_Channel.Set( pChannel);
	if (0 < m_ShowStyle)
	{
		m_FirstAdd = 35;//������������
	}
	else
		m_FirstAdd = 0;
	if (2 == m_ShowStyle)
	{
		//��������
		if (name.size())
		{
			char name1[20];
			memcpy_s(name1, 20, name.c_str(), name.size());
			char name2[20];
			name2[0] = ' ';
			name2[1] = '[';
			int k = 2;
			for (int i = 0; i < name.size(); i++)
			{
				name2[k] = name1[i];
				k += 1;
			}
			name2[k] = ']';
			name2[k + 1] = 0;
			string name3 = name2;
			m_Name.SetString(name3);
			GetTextExtentPointA(g_DC, m_Name.GetString().c_str(), m_Name.GetString().size(), &size1);
			m_FirstAdd += size1.cx;
		}
		else m_ShowStyle = 1;
	}
	//����1���ո�Ŀ��,
	GetTextExtentPointA(g_DC, " ", 3, &size1);
	int spaceLength = size1.cx;
	
	m_LineNum = 0; //���¼�������
	//m_TextLineList[m_LineNum].m_NumFace = 0;

	int TextNum = 0;//����һ�е�����;
	int totalWidth = LineWidth - m_FirstAdd;//��һ�еĿ��
	BYTE  cha[256];
	memcpy_s(cha, 256, tex.c_str(), tex.size());
	int kk32 = tex.size();
	const int limitH = 22;
	m_TextLineList[0].m_Height = limitH;
	for (int i = 0; i < kk32; i++)
	{
		//�������0-128֮��(Ҳ����˵,���Ǻ���)
		if (cha[i]>127)
		{
			line[TextNum] = cha[i];
			i += 1;
			line[TextNum + 1] = cha[i];
			TextNum += 2;
			GetTextExtentPointA(g_DC, line, TextNum, &size1);
			if (size1.cx >= totalWidth)
			{

				line[TextNum - 2] = 0;
				m_TextLineList[m_LineNum].m_line.SetString(line);
				m_LineNum += 1;
				//����
				totalWidth = LineWidth - 3;
				TextNum = 0;
			//	m_TextLineList[m_LineNum].m_NumFace = 0;
				m_TextLineList[m_LineNum].m_Height = limitH;
				line[TextNum] = cha[i - 1];
				line[TextNum + 1] = cha[i];
				TextNum += 2;
			}
		}
		else
		{
			if (cha[i] == '\t')
			{
			//	i += 1;
				continue;
			}
			else
			if (cha[i] == '\n')
			{
			//	i += 1;
				line[TextNum] = 0;
				m_TextLineList[m_LineNum].m_line.SetString(line);
				m_LineNum += 1;
				//����
				totalWidth = LineWidth - 3;
				TextNum = 0;
			//	m_TextLineList[m_LineNum].m_NumFace = 0;
				m_TextLineList[m_LineNum].m_Height = limitH;
			}
			else
			if (cha[i] == '#')
			{
				int kk = 0; //���ֵ�λ��
				while (cha[i +1+ kk] == '\r' || cha[i +1+ kk] == '\n')
					kk += 1;
				int IDNum = 0;
				if (i == kk32 - 1)break;
				int type = cha[i + 1];
				if (type >= 47 && type < 58)//����
				{
					//��ȡ�����ID	
					
					char dust1[5];
					while (1)
					{
						if (cha[i + 1 + IDNum] == '\r' || cha[i + 1 + IDNum] == '\n')
							IDNum += 1;
						else
						{
							if (cha[i + 1 + IDNum] >= 47 && cha[i + 1 + IDNum] < 58)//�Ƿ�������
							{
								dust1[IDNum] = cha[i + 1 + IDNum];
								IDNum += 1;
							}
							else
								break;
						}
					}
					dust1[IDNum] = 0;
					//���������
					GetTextExtentPointA(g_DC, line, TextNum, &size1);
					int faceid = atoi(dust1);
					if (faceid > 128)faceid = 0;
					CWFile& pWFile = p->m_Face[faceid];
					if (0 == pWFile.m_NowID)
						pWFile.Load(g_pMainState->m_Channel.m_FaceId[faceid]);
					
					if (size1.cx + pWFile.m_Picture.m_Width > totalWidth)
					{

						line[TextNum] = 0;
						m_TextLineList[m_LineNum].m_line.SetString(line);

						//����
						m_LineNum += 1;
						totalWidth = LineWidth - 3;
						TextNum = 0;
						m_TextLineList[m_LineNum].m_Height = limitH;
					//	m_TextLineList[m_LineNum].m_NumFace += 1;
						m_TextLineList[m_LineNum].m_xFace[m_TextLineList[m_LineNum].m_NumFace] = 0;
					}
					else
					{
						m_TextLineList[m_LineNum].m_xFace[m_TextLineList[m_LineNum].m_NumFace] = size1.cx;// +namelength;
					}
					//����߶�
					if (pWFile.GetHeight() > m_Height)
					{
						m_TextLineList[m_LineNum].m_Height = pWFile.GetHeight();
					}
					m_TextLineList[m_LineNum].m_Face[m_TextLineList[m_LineNum].m_NumFace].Set(&pWFile);
					m_TextLineList[m_LineNum].m_NumFace += 1;
					int kkk = (pWFile.m_Picture.m_Width) / spaceLength + 2;
					for (int f = 0; f <= kkk; f++)
					{
						line[TextNum] = ' ';
						TextNum += 1;
					}
					i += IDNum+kk;
				}
				else
				{
					DWORD color = 0;
					switch (type)  //�Ƿ�����ɫ
					{
					case 87:
					case 119:  //W  ��
						color = RGB(255, 255, 255);
						break;
					case 88:
					case 120:  //X  B��ռ����,����X�����
						color = RGB(0, 0, 0);
						break;
					case  89:  //Y  ��
					case 121:
						color = RGB(255, 255, 0);
						break;
					case  82:  //r ��
					case 114:
						color = RGB(255, 0, 0);
						break;
					case  71:  //g  ��
					case 103:
						color = RGB(0, 255, 0);
						break;
					case  66:  //b   ��
					case  98:
						color = RGB(0, 0, 255);
						break;
					default:
						continue;
						break;
					}
					sColorTag tag;
					tag.m_Color = color;
					tag.m_linenum = m_LineNum;
					GetTextExtentPointA(g_DC, line, TextNum, &size1);
					tag.m_start = size1.cx;
					m_ColorTag.push_back(tag);
					i += 1+kk;
				}
			}
			else
			{
				line[TextNum] = cha[i];
				TextNum += 1;
				GetTextExtentPointA(g_DC, line, TextNum, &size1);
				if (size1.cx > totalWidth)
				{
					totalWidth = LineWidth - 3;
					line[TextNum - 1] = 0;
					m_TextLineList[m_LineNum].m_line.SetString(line);
					//��һ��
					m_LineNum += 1;
				//	m_TextLineList[m_LineNum].m_NumFace = 0;
					m_TextLineList[m_LineNum].m_Height = limitH;
					TextNum = 0;
					line[TextNum] = cha[i];
					TextNum += 1;
				}
			}
		}
	}
	line[TextNum] = 0;
	m_TextLineList[m_LineNum].m_line.SetString(line);
	m_LineNum += 1;
	//��ɫ����
	if (m_ColorTag.size())
	{
		int num = m_ColorTag.size() / 2;
		for (int i = 0; i <num ; i += 1)
		{
			if (m_ColorTag[i * 2].m_linenum!=m_ColorTag[i * 2 + 1].m_linenum)
			{
				int linenum = m_ColorTag[i * 2 + 1].m_linenum- m_ColorTag[i * 2].m_linenum;
			
				m_TextLineList[m_ColorTag[i * 2].m_linenum].m_line.SetTagColor(
					m_ColorTag[i * 2].m_start,
					-1,
					m_ColorTag[i * 2].m_Color);
				for (int i2 = 1; i2 < linenum ; i2++)
				{
					m_TextLineList[m_ColorTag[i * 2].m_linenum+i2].m_line.SetTagColor(
						-1,
						-1,
						m_ColorTag[i * 2].m_Color);
				}
				m_TextLineList[m_ColorTag[i * 2+1].m_linenum].m_line.SetTagColor(
					0,
					m_ColorTag[i * 2 + 1].m_start,
					m_ColorTag[i * 2].m_Color);
			}
			else
			m_TextLineList[m_ColorTag[i * 2].m_linenum].m_line.SetTagColor(
				m_ColorTag[i * 2].m_start, 
				m_ColorTag[i * 2 + 1].m_start, 
				m_ColorTag[i * 2].m_Color);
		}
	}

	//����߶�
	m_Height = 0;
	for (int i = 0; i < m_LineNum; i++)
	{
		m_Height += m_TextLineList[i].m_Height + m_Gap;
	}
	SelectObject(g_DC, oldFont);
	return TRUE;
}



void sFaceText::Show(cCanvas* pCanvas)
{
	switch (m_ShowStyle)
	{
	case 2:
		pCanvas->Add(&m_Name);
	case 1:
		pCanvas->Add(&m_Channel, 1);
		break;
	}
	for (int i = 0; i < m_LineNum; i++)
	{
		pCanvas->Add(&m_TextLineList[i].m_line);
		for (int j = 0; j < m_TextLineList[i].m_NumFace; j++)
		{
			pCanvas->Add(&m_TextLineList[i].m_Face[j], 1);
		}
	}
	
}

void sFaceText::SetTagColor(sColorTag& starttag, sColorTag& endtag)
{
	if (endtag.m_linenum == starttag.m_linenum)
		m_TextLineList[starttag.m_linenum].m_line.SetTagColor(starttag.m_start, endtag.m_start, starttag.m_Color);
	else
	{
		m_TextLineList[starttag.m_linenum].m_line.SetTagColor(starttag.m_start, -1, starttag.m_Color);

		for (int i = starttag.m_linenum+1; i < endtag.m_linenum; i++)
		{
			m_TextLineList[starttag.m_linenum].m_line.SetTagColor(-1, -1, starttag.m_Color);

		}
		m_TextLineList[starttag.m_linenum].m_line.SetTagColor(-1, endtag.m_start, starttag.m_Color);	
	}
}

BOOL sFightData::Init()
{
	chaos = 0;
	cannotAttack = 0; //�Ƿ����ʹ��������   
	cannotMagic = 0;  //�Ƿ����ʹ��ħ��
	cannotSkill = 0;  //�Ƿ����ʹ���ؼ�
	cannotUseItem = 0;//�Ƿ����ʹ�õ���
	cannotAddHP = 0;  //�Ƿ���Լ�Ѫ
	cannotAll = 0;  //�Ƿ���Զ�  ���߷���Ч���������,������Ҳ�����
	cannotLive = 0;
	m_bHit = 0; //��¼�Ƿ�����  �����ֵ���ж��Ƿ���Ҫ����
	//��¼�Ƿ񱻳ɹ����� �� �������Ľ�ɫ��ID
	m_guardID.clear();
	m_MaxDef = 0;
	m_MaxMagicDef = 0;

	m_TurnHPAdd = 0;
	m_TurnMPAdd = 0;
	
	m_TurnHPHurtAdd = 0;
	//һ����ɫ��״̬�б�
	Clear();
	return TRUE;
}

void sFightData::Reset()
{
	SufferNum = 0;//��¼һ�غ��н�ɫ����Ĵ���
	AttackNum = 0; //��¼��ɫ���غϵĹ�����
}


void cObjFather::SetShowWidth(int k)
{
	if (k > m_Width-m_xOffset2)
		k = m_Width - m_xOffset2;
	SetxOffset(m_Width - k);
}

void cObjFather::SetShowHeight(int k)
{
	if (k + m_yOffset2 > m_Height)
		k = m_Height - m_yOffset2;
	SetyOffset(m_Height - k); 
}

void cObjFather::Init(cObjFather* pObj, int alpha)
{
	if (!pObj)return;
	m_x = pObj->m_x;
	m_y = pObj->m_y;
	m_Width = pObj->GetShowWidth();
	m_Height = pObj->GetShowHeight();
	m_ShowHeight =0;
	m_ShowWidth = 0;
	m_xOffset  = 0;
	m_xOffset2 = 0;
	m_yOffset  = 0;
	m_yOffset2 = 0;
	int size = m_Width*m_Height;
	if (m_OldDataSize < size)
	{
		m_OldDataSize = size;
		SAFE_DELETE_ARRAY(m_pData);
		SAFE_DELETE_ARRAY(m_pAlpha);
		m_pAlpha = new BYTE[size];
		m_pData = new BYTE[size << 1];
	}
}

cObjFather::~cObjFather()
{
	SAFE_DELETE_ARRAY(m_pData);
	SAFE_DELETE_ARRAY(m_pAlpha);
}

void cObjFather::Draw(cObjFather* pObj, int xpos, int ypos, int alpha)
{
	RECT rect1, rect2;
	GetRect(rect1);
	if (xpos == -1)
	{
		pObj->GetRect(rect2);
	}
	else
	{
		rect2.left =  xpos;
		rect2.top = ypos;
		rect2.right = rect2.left + pObj->GetShowWidth();
		rect2.bottom = rect2.top + pObj->GetShowHeight();
	}
	RECT nowrect;  //����ͼ�����ֵĴ�С
	if (!CheckCross(rect1, rect2, nowrect))return ;
	//ʵ������Ĵ�С
	int Height = nowrect.bottom - nowrect.top;
	int Width = nowrect.right - nowrect.left;
	//ʵ���������ͼƬ�����ƫ��
	int xOffset1 = nowrect.left - rect1.left;
	int yOffset1 = nowrect.top - rect1.top;	
	int xOffset2 = nowrect.left - rect2.left;
	int yOffset2 = nowrect.top - rect2.top;
	WPixel pix1,pix2;
	int index1, index2, index11, index22, alpha1, alpha11;
	BYTE  alpha2;


	index11 = ((yOffset1 + m_yOffset2)*m_Width + (xOffset1 + m_xOffset2)) << 1;
	alpha11 = ((yOffset2 + pObj->m_yOffset2)*pObj->m_Width + (xOffset2 + pObj->m_xOffset2));
	index22 = alpha11 << 1;
	index1 = index11;
	index2 = index22;
	alpha1 = alpha11;
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			alpha2 = pObj->m_pAlpha[alpha1];
			if (alpha < 100)
			{
				alpha2 = (alpha1*alpha) / 100;
			}
			alpha1 += 1;
			if (!alpha2)
			{
				index1 += 2;
				index2 += 2;
				continue;
			}
			if (alpha2 < 255)
			{
				pix1.l = m_pData[index1];
				pix1.h = m_pData[(index1)+1];
				pix2.l = pObj->m_pData[index2];
				pix2.h = pObj->m_pData[(index2)+1];

				pix2.red = (pix2.red *alpha2 + pix1.red * (255 - alpha2))>>8;
				pix2.green = (pix2.green *alpha2 + pix1.green * (255 - alpha2))>>8;
				pix2.blue = (pix2.blue *alpha2 + pix1.blue * (255 - alpha2))>>8;
				m_pData[index1] = pix2.l;
				m_pData[index1 + 1] = pix2.h;
			}
			else
			{
				pix2.l = pObj->m_pData[index2];
				pix2.h = pObj->m_pData[(index2)+1];
				m_pData[index1] = pix2.l;
				m_pData[index1 + 1] = pix2.h;
			}
			if (m_pAlpha)
			{
				BYTE alpha1 = m_pAlpha[index1];
				if (alpha1 < 255)
				{
					alpha1 += alpha2;
					if (alpha1>255)alpha1 = 255;
					m_pAlpha[index1 >> 1] = alpha1;
				}
			}
			index1 += 2;
			index2 += 2;

		}
		index11 += m_Width<<1;
		index22 += pObj->m_Width<<1;
		index1 = index11;
		index2 = index22;
		alpha11 += pObj->m_Width;
		alpha1 = alpha11;
	}
}

void cObjFather::GetRect(RECT& rect1)
{
	rect1.left = m_x;
	rect1.right = rect1.left + GetShowWidth();
	rect1.top = m_y;
	rect1.bottom = rect1.top + GetShowHeight();
}

void cObjFather::Reset(cObjFather* pObj, int alpha /*= 100*/)
{
	int wid = GetShowWidth();
	int hei = GetShowHeight();
	int index11, index21;

	for (int i = 0; i < wid; i++)
	for (int j = 0; j < hei; j++)
	{
		index11 = i + m_xOffset2 + (j + m_yOffset2) *m_Width;
		index21 = i + pObj->m_xOffset2 + (j + m_yOffset2)*pObj->m_Width;

		if (alpha == 100)
			m_pAlpha[index11] = pObj->m_pAlpha[index11];
		else
			m_pAlpha[index11] = (pObj->m_pAlpha[index11] * alpha) / 100;

		index11 = index11 << 1;
		index21 = index21 << 1;
		m_pData[index11] =  pObj->m_pData[index21];
		m_pData[index11 + 1] =  pObj->m_pData[index21 + 1];
		
	
		
	}
}

void cObjFather::Reset2(CWFile* pic)
{
	int index11, index21;
	int wid = GetShowWidth();
	int hei = GetShowHeight();
	if (!pic)
	{
		for (int i = 0; i < wid; i++)
		for (int j = 0; j < hei; j++)
		{
			index11 = i + m_xOffset2 + (j + m_yOffset2) *m_Width;
			m_pAlpha[index11] = 255;
			index11 = index11 << 1;
			m_pData[index11] = 0;
			m_pData[index11 + 1] = 0;
		}
		return;
	}
	cObjFather* pObj = &pic->m_Picture;

	BYTE* pData = pic->m_Picture.Spr.lpDate[0];
	BYTE* pAlpha = pic->m_Picture.Spr.lpAlpha[0];
	int pitch2 = ((pic->GetWidth() << 1) + 7) & 0xfffffff8;
	for (int i = 0; i < wid; i++)
	for (int j = 0; j < hei; j++)
	{
		index11 = i + m_xOffset2 + (j + m_yOffset2) *m_Width;
		index21 = i + pObj->m_xOffset2 + (j + m_yOffset2)*pObj->m_Width;
		m_pAlpha[index11] = pAlpha[index21];
		index11 = index11 << 1;
		index21 = ((i + pObj->m_xOffset2)<<1) + (j + m_yOffset2)*pitch2;
		m_pData[index11] =  pData[index21];
		m_pData[index11 + 1] =  pData[index21 + 1];
	}
}

void cObjFather::SetxOffset2(int xOff2)
{
	m_xOffset2 = xOff2; 
	if (m_ShowWidth)
	{
		m_xOffset =m_Width- m_ShowWidth - m_xOffset2;
		if (m_xOffset < 0)
		{
			m_ShowWidth += m_xOffset;
			m_xOffset = 0;
		}	
	}
	else
	{
		m_ShowWidth = m_Width - m_xOffset2;
	}

}
void cObjFather::SetyOffset2(int yOff2)
{
	m_yOffset2 = yOff2;
	if (m_ShowHeight)
	{
		m_yOffset =m_Height- m_ShowHeight - m_yOffset2;
		if (m_yOffset < 0)
		{
			m_ShowHeight += m_yOffset;
			m_yOffset = 0;
		}
	}
	else
	{
		m_ShowHeight = m_Height - m_yOffset2;
	}
}
void cObjFather::SetyOffset(int yOff)
{
	m_yOffset = yOff;
	m_ShowHeight = m_Height - m_yOffset;
	if (m_ShowHeight < 0)m_ShowHeight = 0;
}
void cObjFather::SetxOffset(int xOff)
{
	m_xOffset = xOff;
	m_ShowWidth = m_Width - m_xOffset;
	if (m_ShowWidth < 0)m_ShowWidth = 0;
}



void sZuoQi2::GetID(int Pos, vector<DWORD>& out, BOOL haveChar /*= TRUE*/)
{
	int num=1;
	if (haveChar)num += 1;
	if (m_ZhuangShi.GetNum())num += 1;
	out.resize(num);
	switch (Pos)
	{
	case POS_STAND:
		if (1)
		{
			out[0] =m_pZuoQi->nodeStand;
			if (haveChar)
			{
				out[1] = m_pZuoQi->CharStand[m_ownerID];
				if (m_ZhuangShi.GetNum())
				{
					out[2] = g_pMainState->m_ItemManage.m_ZuoQiZhuangShiList[m_ZhuangShi.GetID()].Stand[m_ownerID];
				}
					
			}
			else
			{
				if (m_ZhuangShi.GetNum())
				{

					out[1] = g_pMainState->m_ItemManage.m_ZuoQiZhuangShiList[m_ZhuangShi.GetID()].NodeStand;
				}
			}
		}
		return;
	case POS_WALK:
		if (1)
		{
			out[0] = m_pZuoQi->nodeWalk;
			if (haveChar)
			{
				out[1] = m_pZuoQi->CharWalk[m_ownerID];
				if (m_ZhuangShi.GetNum())
				{
					out[2] = g_pMainState->m_ItemManage.m_ZuoQiZhuangShiList[m_ZhuangShi.GetID()].Walk[m_ownerID];
				}

			}
			else
			{
				if (m_ZhuangShi.GetNum())
				{

					out[1] = g_pMainState->m_ItemManage.m_ZuoQiZhuangShiList[m_ZhuangShi.GetID()].NodeWalk;
				}
			}
		}
		return;
	default:
		break;
	}
	return;
}



int sItem2::GetType()
{
	if (!m_pItemData)return -1;
	return m_pItemData->m_Type;
}

int sItem2::GetID()
{
	return m_pItemData->m_ID;
}

std::string sItem2::GetName()
{
	return m_pItemData->m_Name;
}

std::string sItem2::GetText()
{
	return m_pItemData->m_Text;
}

void sItem2::Set(int type, int id, BOOL needclear)
{
	if (needclear)m_Property.clear();
	m_pItemData = g_pMainState->m_ItemManage.GetItem(type, id);

}

void sItem2::Save(ofstream& File)
{
	File <<" "<< m_Num<<" ";
	if (m_Num > 0)
	{
		File << m_pItemData->m_Type<<" ";
		File << m_pItemData->m_ID<<" ";
		if (m_strProperty.size())
		{
			File << 1<<" ";
			File << m_strProperty << " ";
		}
		else
		{
			File << 0 << " ";
		}
		File << m_Property.size()<<" ";
		for (int i = 0; i < m_Property.size();i++)
		{
			File << m_Property[i] << " ";
		}
	}
}

void sItem2::Load(ifstream& File)
{
	File >> m_Num;
	if (m_Num > 0)
	{
		int type,  id;
		File >> type;
		File >> id;
		m_pItemData = g_pMainState->m_ItemManage.GetItem(type, id);

		int havestring;
		File >> havestring;
		if (havestring)
		{
			File >> m_strProperty;
		}
		else m_strProperty = "";
	
		int propnum;
		File >> propnum;
		if (propnum > 0)
		{
			m_Property.resize(propnum);
			for (int i = 0; i < m_Property.size(); i++)
			{
				File >> m_Property[i];
			}
		}
	}
}




void sInt::Save(ofstream& File)
{
	File << " ";
	File <<m_Data << " ";
	File <<m_DataAdd << " ";
	File <<m_DataAddPercent << " ";
	File <<m_DataOriginal << "\n";
}

void sInt::Load(ifstream& File)
{
	File >> m_Data;
	File >> m_DataAdd;
	File >> m_DataAddPercent;
	File >> m_DataOriginal;
}

BOOL cTaskFather::CheckNpc(string mapname, string npcname)
{
	if (!g_pMainState->m_Map.m_MapName.compare(mapname))
	{
		//if (!g_pMainState->m_pNowSelect->m_TrueName.GetString().compare(npcname))
			return TRUE;
	}
	return FALSE;
}

void cTaskFather::AddTriggerDialog(string path, int id, int ret)
{
	g_pMainState->m_TriggerDialog.AddTask(path, id, this, ret); 
}

void cTaskFather::AddTriggerObjMap(string mapname, int ret, BOOL bHuChi )
{
	g_pMainState->m_TriggerObjMap.AddTask(this, ret, mapname,bHuChi); 
}

void cTaskFather::AddTriggerObjMap(sNpc* npc, int ret,BOOL bHuChi)
{
	g_pMainState->m_TriggerObjMap.AddTask(this, ret, *npc, bHuChi);
	if (npc->m_mapname == g_pMainState->m_Map.m_MapName)
	{
		g_pMainState->m_Map.AddNpc(g_pMainState->m_TriggerObjMap.FindByName(npc->m_name));
		g_pMainState->FindObj();
	}
}

void cTaskFather::AddTriggerObjMap(sMapObjTrigger* data)
{
	g_pMainState->m_TriggerObjMap.AddTask(*data);
}



void cTaskFather::AddTriggerObj(sNpc* npc, int ret)
{
	g_pMainState->m_TriggerObj.AddTask(npc->m_name, npc->m_mapname, this, ret); 
}

void cTaskFather::AddTriggerObj(string name, string map, int ret, BOOL bHuChi)
{
	g_pMainState->m_TriggerObj.AddTask(name, map, this, ret,bHuChi);
}


void cTaskFather::AddNpcOne(sMapObjTrigger* data, BOOL needfind)
{
	g_pMainState->m_Map.AddNpc(data, needfind);
}



void cTaskFather::SetJiangLi(int time, BOOL needtag /*= FALSE*/, int money /*= 100*/, int chubei /*= 0*/, int exp /*= 100*/, int expbb /*= 0*/)
{ // chubei ��ʾ��Ǯ�ж����Ǵ����� ��chubeiΪ100ʱ  ���е��ֽ𶼻��ɴ�����
	int herolv = g_pCharacter->m_PCData.m_LV;
	if (herolv < 1)herolv = 1;
	if (herolv>200)herolv = 200;
	INT64  exp2, expbb2;
	int money2, chubei2;
	if (money)
	{
		money2 = 30 * time* herolv*(money + rand() % 5) / 100;
	}
	else money2 = 0;
	if (chubei)
	{
		chubei2 = money2*chubei / 100;
		money2 -= chubei2;
	}
	else chubei2 = 0;
	if (exp)
	{
		exp2 = 200 * time* herolv*(exp + rand() % 5) / 100;
	}
	else exp2 = 0;


	if (expbb)
	{
		int bblv = g_pCharacter->m_PCData.m_LV;
		expbb2 = 200 * time* bblv*(expbb + rand() % 5) / 100;
	}
	else expbb2 = 0;
	g_pMainState->SetJiangLi(exp2, expbb2, money2, chubei2, needtag);
}
void cTaskFather::SetJiangLi2(int exp, int money/*=0*/, int chubei/*=0*/, int petexp/*=0*/, BOOL needtag/*=TRUE*/)
{
	g_pMainState->SetJiangLi(exp, petexp, money, chubei, needtag);
}
void cTaskFather::SetJiangLiItem(int id, int type, int num /*= 1*/)
{
	sItem2 item;
	item.m_Num = num;
	item.Set(type, id);
	g_pMainState->m_InterfaceItem.GetItem(g_pMainState->m_HeroID, &item);
	g_pMainState->m_Tags.GetItem(item.GetName());
}

void cTaskFather::AddTriggerGive(string name, int type, int id, int ret, BOOL bHuChi )
{
	g_pMainState->m_TriggerGive.AddTask(name, id, type, this, ret,bHuChi);
}

void cTaskFather::AddTag(string text, int time /*= 500*/)
{
	g_pMainState->m_Tags.Add(text, time);
}

void cTaskFather::AddTriggerFightTurn(int ret)
{
	g_pMainState->m_TriggerFightTurn.AddTask(this, ret);
}
void cTaskFather::AddTriggerFightSuccess(int ret)
{
	g_pMainState->m_TriggerFightSuccess.AddTask(this, ret);
}
void cTaskFather::AddTriggerFightFalse(int ret)
{
	g_pMainState->m_TriggerFightFalse.AddTask(this, ret);
}

void cTaskFather::AddTriggerFightEscape(int ret)
{
	g_pMainState->m_TriggerFightEscape.AddTask(this, ret);
}

void cTaskFather::UpdateJob(string context, string title,int time,int ret)
{
	sJob job;
	job.m_Context = context;
	job.m_pTask = this;
	job.m_Stage = ret;
	job.m_Time = time;
	job.m_Title = title;
	g_pMainState->m_Job.AddaJob(job);
}
void cTaskFather::UpdateJob(string context, string title)
{
	sJob job;
	job.m_Context = context;
	job.m_pTask = this;
	job.m_Stage = -1;
	job.m_Time = 0;
	job.m_Title = title;
	g_pMainState->m_Job.AddaJob(job);
}
void cTaskFather::RemoveTriggerObj()
{
	return g_pMainState->m_TriggerObj.ReMove(this);
}

void cTaskFather::RemoveTriggerGive()
{
	return g_pMainState->m_TriggerGive.ReMove(this);
}

void cTaskFather::RemoveTriggerObjMap(int ret)
{
	g_pMainState->m_TriggerObjMap.ReMove(this,ret);
}

void cTaskFather::RemoveTriggerObjMap(int x, int y)
{
	g_pMainState->m_TriggerObjMap.ReMove(this, x,y);
}

void cTaskFather::AddTriggerLv(int ret, int num)
{
	g_pMainState->m_Triggerlv.AddTask(this, ret,num);
}

void cTaskFather::AddTriggerTalkList(string path, int id, int ret)
{
	g_pMainState->m_TriggerDialog.AddTask(path, id, this, ret);
}

void cTaskFather::AddTriggerFightStart(int ret)
{
	g_pMainState->m_TriggerFightStart.AddTask(this, ret);
}

void cTaskFather::UnInviteAll()
{
	for (int i = 1; i < 5; i++)
	{
		g_pMainState->m_Friend.UnInvite(i);
	}
}

void cTaskFather::AddChannel(string text,string name, int channel)
{
	g_pMainState->m_Channel.AddNewString(text,name,channel);
}

void cTaskFather::NowNpcTalk(string text)
{
	g_pMainState->m_pNowSelect->Talk(text);
}

void cTaskFather::RemoveTriggerSuccess()
{
	g_pMainState->m_TriggerFightSuccess.ReMove(this);
}
void cTaskFather::RemoveTriggerFalse()
{
	g_pMainState->m_TriggerFightFalse.ReMove(this);
}
void cTaskFather::RemoveTriggerEscape()
{
	g_pMainState->m_TriggerFightEscape.ReMove(this);
}

void cTaskFather::DialogTalkObj(string text,cObj* p)
{
	g_pMainState->m_Dialog.TalkObj(text, p);
}

void cTaskFather::DialogTalkNObj(string text)
{
	g_pMainState->m_Dialog.TalkNObj(text);
}

void cTaskFather::FollowLv(int id, int lv)
{
	g_pMainState->m_FightList[id].m_pData->FollowLv(lv);
}

BOOL cTaskFather::CheckHeroMoney(int money,BOOL bTag)
{//����ɫ�Ƿ����㹻��Ǯ
	if (g_pCharacter->m_PCData.m_Money[0] + g_pCharacter->m_PCData.m_Money[2] < money)
	{
		if (bTag)
		{
			ostringstream oss;
			oss << "��Ǯ����" << money<<"��";
			g_pMainState->m_Tags.Add(oss.str());
		}
		return FALSE;
	}
	return TRUE;
}

void cTaskFather::GetPet(string name, sShangPinPet* pet,int stronglv)
{
	int teamid = -1;
	for (int i = 0; i < 5; i++)
	{
		if (g_pMainState->m_FightList[i].m_pData->m_TrueName.GetString() == name)
		{
			teamid = i;
			break;
		}
	}
	int friendid= g_pMainState->m_Friend.FindFriend(name);
	if (friendid>-1)
	{
		sFriendList& lis= g_pMainState->m_Friend.m_FriendList[friendid];
		if (lis.m_BBList.size() > 7)return;
		lis.m_BBList.push_back(*pet);
		lis.m_FightBB = lis.m_BBList.size() - 1;
	}
	if (teamid>-1)
	{
		cPet* pet2;
		pet2 = new cPet;
		pet2->Set(pet, stronglv);
		g_pMainState->GivePcAPet(teamid,pet2);
	}
}

void cTaskFather::GetPet2(sShangPinPet* pet, int stronglv)
{
	cPet* pet2;
	pet2 = new cPet;
	pet2->Set(pet, stronglv);
	g_pMainState->GivePcAPet(g_pMainState->m_HeroID, pet2);
}

void cTaskFather::SetMusic(string name /*= ""*/)
{
	if (name == "")
	{
		name = "ս��BOSS";
	}
	g_pMainState->m_Mp3.Load(name);
}

void cTaskFather::AutoGetPet(string name, int id,  int id1, int id2, int id3)
{
	int idd = g_pMainState->m_Friend.FindFriend(name);
	sFriendList* plist = &g_pMainState->m_Friend.m_FriendList[idd];
	sShangPinPet item;
	item.LoadEasy(id);
	plist->m_BBList.push_back(item);
	if (id1 > -1)
	{
		item.LoadEasy(id1);
		plist->m_BBList.push_back(item);
	}
	if (id2 > -1)
	{
		item.LoadEasy(id2);
		plist->m_BBList.push_back(item);
	}
	if (id3 > -1)
	{
		item.LoadEasy(id3);
		plist->m_BBList.push_back(item);
	}
	if (plist->m_BBList.size())
	{
		plist->m_FightBB = plist->m_BBList.size() - 1;
	}
}

cPetData* cTaskFather::AddPetEnemyF(int hp, ePetID id, int lv, string name , BOOL bOur /*= FALSE*/, int pos)
{
	if (lv<0)lv = 0;
	cPetData* p;
	int k = g_pMainState->AddPetEnemy(name, id, lv,0, -1,pos, bOur);
	g_pMainState->m_FightList[k].m_pData->GetDateP(p);
	if (hp > 0)
	{
		p->m_HPMax.SetData(hp);
		p->FillHPMP();
	}
	return p;
}

cPetData* cTaskFather::AddPCPetEnemyF(int hp, ePetID id, int lv, eMengPai mp, string name /*= ""*/, BOOL bOur /*= 0*/, int pos /*= -1*/)
{
	cPetData* p;
	int k = g_pMainState->AddPCPetEnemy(id, lv, 0,mp, name, -1,bOur, pos);
	if (k == -1)return 0;
	g_pMainState->m_FightList[k].m_pData->GetDateP(p);
	if (hp > 0)
	{
		p->m_HPMax.SetData(hp);
		p->FillHPMP();
	}
	return p;
}

cPetData* cTaskFather::AddPCEnemyF(int hp, int id, sZiZhi* zz, int lv, eMengPai mp, string name /*= -1*/, int weapontype /*= -1*/, int weaponid /*= -1*/, BOOL bOur /*= FALSE*/, int pos)
{
	cPetData* p;
	sItem2 item;
	sItem2* pitem=0;
	if (weaponid > -1)
	{
		item.m_Num = 1;
		item.Set(weapontype, weaponid);
		pitem = &item;
	}
	int k = g_pMainState->AddPCEnemy(name, id, lv,0,zz, mp,pitem ,-1 ,-1, pos,bOur);
	if (k<0)return 0;
	g_pMainState->m_FightList[k].m_pData->GetDateP(p);
	if (hp > 0)
	{
		p->m_HPMax.SetData(hp);
		p->FillHPMP();
	}
	return p;
}

void cTaskFather::RemoveTriggerObjMap2(cObj* pObj)
{
	int x = pObj->GetX() / 20;
	int y = (g_pMainState->m_Map.m_pMap->m_Height - pObj->GetY()) / 20;
	RemoveTriggerObjMap(x, y);
}


void sChangeColorData::Save(ofstream& File)
{
	File << " ";
	File << partnum<<" ";
	for (int i = 0; i < partnum; i++)
	{
		File << m_colorchange[i]<<" ";
	}
}

void sChangeColorData::Read(ifstream& File, int modeid, eRanseType k, int partid)
{
	File >> partnum ;
	if (partid == -1)return;//��ʾ���Ǻ��������
	DWORD color=0;
	if (!partnum)return;
	
		switch (k)
		{
		case sChangeColorData::ePc:
			color = g_pMainState->m_pCharacterDataList[modeid].m_ColorID;
			break;
		case sChangeColorData::ePet:
			color = g_pMainState->m_PetDataManager.GetPetData(modeid)->m_PartList[partid].m_ColorID;
			break;
		case sChangeColorData::eZuoQi:
			color = g_pMainState->m_ZuoQi.m_ZuoQi[modeid].m_ColorID;
			break;
		case sChangeColorData::eZhuangShi:
			color= g_pMainState->m_ItemManage.m_ZuoQiZhuangShiList[modeid].m_ColorID;
			break;
		default:
			ERRBOX; break;
		}
		if (color == 0)
		{
			color= 0x1657;
		}
		g_pMainState->m_ColorManage.Set(color);
		for (int i = 0; i < partnum; i++)
		{
			File >> m_colorchange[i];	
			g_pMainState->m_ColorManage.GetColorMatrix(
				i,
				m_colorchange[i],
				m_colormatrix[i]);
		}
	
}

BOOL sChangeColorData::SetColorChange(int id, int num)
{
	if (g_pMainState->m_ColorManage.GetPartNum() <= id)return FALSE;
	int matrixnum = g_pMainState->m_ColorManage.GetMatirxNum(id);
	if (matrixnum< 1)return FALSE;
	if (matrixnum <= num)num = 0;
	m_colorchange[id] = num;
	g_pMainState->m_ColorManage.GetColorMatrix(id,num, m_colormatrix[id]);
	return TRUE;
}



BOOL sChangeColorData::AddColorChange(int id)
{
	return SetColorChange(id, m_colorchange[id] + 1);
}

void sChangeColorData::Set(int d0, int d1, int d2, int d3)
{
	m_colorchange[0] = d0;
	m_colorchange[1] = d1;
	m_colorchange[2] = d2;
	m_colorchange[3] = d3;
}

void sChangeColorData::operator=(sChangeColorData& data)
{
	if (0 == data.partnum)
	{
		partnum = 0;
		return;
	}
	partnum = data.partnum;
	for (int i = 0; i < 4; i++)
	{
		m_colorchange[i] = data.m_colorchange[i];
		m_colormatrix[i] = data.m_colormatrix[i];
	}
}

int sBBXiuLian::AddExp(int lv, int num, eXiuLian type)
{
	int* Skilllv=0;
	int* SkillExp=0;
	switch (type)
	{
	case eXLATK:
		Skilllv =&m_Atk;
		SkillExp =&m_AtkExp;
		break;
	case eXLDEF:
		Skilllv = &m_Def;
		SkillExp = &m_DefExp;
		break;
	case eXLMAG:
		Skilllv = &m_Mag;
		SkillExp = &m_MagExp;
		break;
	case eXLDMAG:
		Skilllv = &m_DMag;
		SkillExp = &m_DMagExp;
		break;
	}
	int maxlv = lv - 20;
	if (maxlv < 0)maxlv = 0;
	maxlv /= 5;//��ǰ�ȼ�����������ȼ�
	if (maxlv>30)maxlv = 30;
	if (*Skilllv >= maxlv)
	{
		return 0;
	}
	int maxexp = GetXiuLIanExp(*Skilllv);
	*SkillExp += num;
	if (*SkillExp >= maxexp)
	{
		AddXiuLianLv(1, type);
		*SkillExp -= maxexp;
		return 2;
	}
	return 1;
}

void sBBXiuLian::AddXiuLianLv(int num, eXiuLian type)
{
	switch (type)
	{
	case eXLATK:
		m_Atk += num;
		break;
	case eXLDEF:
		m_Def += num;
		break;
	case eXLMAG:
		m_Mag += num;
		break;
	case eXLDMAG:
		m_DMag += num;
		break;
	}
}

void sBBXiuLian::Save(ofstream& File)
{
	//����
	File << " ";
	File <<m_Atk << " ";
	File << m_Def << " ";
	File << m_Mag << " ";
	File << m_DMag << " ";
	
	File << m_AtkExp << " ";
	File << m_DefExp<< " ";
	File << m_MagExp << " ";
	File << m_DMagExp << "\n";
}

void sBBXiuLian::Load(ifstream& File)
{
	//����
	File >> m_Atk ;
	File >> m_Def ;
	File >> m_Mag ;
	File >> m_DMag;

	File >> m_AtkExp ;
	File >> m_DefExp ;
	File >> m_MagExp ;
	File >> m_DMagExp;
}




void sControler::Save(ofstream& File)
{
	File << " ";
	File<< m_CanGrab<<" "; //�Ƿ�����ҿ���
	File << m_bFly << " ";//�Ƿ�ᱻ����
	File << m_bDelete << " "; //ս�����Ƿ�ɾ��
	File << m_bBB << " ";//�Ƿ���ĳ�˵��ٻ���;
	File << m_HaveBB << " ";//�Ƿ���BB
}

void sControler::Load(ifstream& File)
{
	File >>m_CanGrab ; //�Ƿ�����ҿ���
	File >>m_bFly ;//�Ƿ�ᱻ����
	File >>m_bDelete ; //ս�����Ƿ�ɾ��
	File >>m_bBB ;//�Ƿ���ĳ�˵��ٻ���;
	File >>m_HaveBB ;//�Ƿ���BB
}

void cFangWu::GoIn()
{
	switch (m_GuiMo)
	{
	case 0:
		g_pMainState->m_Map.LoadMap(13, 8, (char*)GetMapName().c_str());
		break;
	case 1:
	case 3:
		g_pMainState->m_Map.LoadMap(18, 12, (char*)GetMapName().c_str());
		break;
	case 2:
		g_pMainState->m_Map.LoadMap(26, 18, (char*)GetMapName().c_str());
		break;
	case 4:
		g_pMainState->m_Map.LoadMap(49, 11, (char*)GetMapName().c_str());
		break;
	default:return;
	}
	for (int i = 0; i < m_JiaJu.size(); i++)
	{
		g_pMainState->m_Map.AddJiaJu(&m_JiaJu[i]);
	}
	g_pMainState->m_Map.AddNpc(&m_GuanJia,0, "�ܼ�");
	for (int i = 0; i < g_pMainState->m_FangWu.m_List.size(); i++)
	{
		g_pMainState->m_Map.AddNpc(&g_pMainState->m_FangWu.m_List[i], &g_pMainState->m_TaskNormal, "��",9);
	}
	g_pMainState->FindObj();
}

std::string cFangWu::GetMapName()
{
	string out;
	if (m_DiBan.GetNum() == 0)
	{
		switch (m_GuiMo)
		{
		case 0:	out = "��ͨ��լ0"; break;
		case 1:	out = "�߼���լ0"; break;
		case 2: out = "������լ0"; break;
		case 3:	out = "�߼���լ10"; break;
		case 4: out = "������լ10"; break;
		}	
	}
	else
	switch (m_DiBan.GetType())
	{
	case 35://1���Ҿ�
		switch (m_GuiMo)
		{
		case 0:	out = "��ͨ��լ1"; break;
		case 1:	out = "�߼���լ1"; break;
		case 2: out = "������լ1"; break;
		case 3:	out = "�߼���լ11"; break;
		case 4: out = "������լ11"; break;
		}
		break;
	case 36://2���Ҿ�
		switch (m_GuiMo)
		{
		case 0:	out = "��ͨ��լ2"; break;
		case 1:	out = "�߼���լ2"; break;
		case 2: out = "������լ2"; break;
		case 3:	out = "�߼���լ12"; break;
		case 4: out = "������լ12"; break;
		}
		break;
	case 37://3���Ҿ�
		switch (m_GuiMo)
		{
		case 0:	out = "��ͨ��լ3"; break;
		case 1:	out = "�߼���լ3"; break;
		case 2: out = "������լ3"; break;
		case 3:	out = "�߼���լ13"; break;
		case 4: out = "������լ13"; break;
		}
		break;
	case 38://4���Ҿ�
		switch (m_DiBan.GetID())
		{
		case 19://��
			switch (m_GuiMo)
			{
			case 0:	out = "��ͨ��լ5"; break;
			case 1:	out = "�߼���լ5"; break;
			case 2: out = "������լ5"; break;
			case 3:	out = "�߼���լ15"; break;
			case 4: out = "������լ15"; break;
			}
			break;
		case 18://����
			switch (m_GuiMo)
			{
			case 0:	out = "��ͨ��լ4"; break;
			case 1:	out = "�߼���լ4"; break;
			case 2: out = "������լ4"; break;
			case 3:	out = "�߼���լ14"; break;
			case 4: out = "������լ14"; break;
			}
			break;

		}
		
	}
	return out;
}

BOOL cFangWu::AddJiaJu(sJiaJu& jiaju)
{
	if (m_JiaJu.size() < (m_GuiMo + 1) * 5)
	{
		m_JiaJu.push_back(jiaju);
		m_HuanJinDu += jiaju.m_JiaJu.GetType() - 34;
		if (m_HuanJinDu>100)m_HuanJinDu = 100;
		
		
		CheckHuanJin();
		return TRUE;
	}
	else g_pMainState->m_Channel.AddNewString("�޷������ӼҾ���");
	return FALSE;
}

int cFangWu::GetHuanJin()
{// ������ 100 +40+ 55 + 60= 255
	int huanjing;
	if (m_DiBan.GetNum())
		huanjing = m_HuanJinDu + m_List.size() * 5 + (m_GuiMo + 1) * 11 + (m_DiBan.GetType() - 34) * 15;
	else
		huanjing = m_HuanJinDu + m_List.size() * 5 + (m_GuiMo + 1) * 11;
	if (huanjing < 0)huanjing = 0;
	return huanjing;
}



cFangWu::cFangWu()
{
	m_GuiMo = -1;
	m_DiBan.m_Num = 0;
	m_GuanJia.m_modeid = 1039;
	m_GuanJia.m_name = "�ܼ�";
	m_GuanJia.Set(-1, -1);
}

void cFangWu::Init()
{
	if (m_bInit)return;
	m_bInit = TRUE;
	ifstream File;
	string path = g_strMediaPath;
	path += "����//����//��լ���б�.txt";
	File.open(path.c_str());
	int num;
	File >> num;
	m_YaHuangIDList.resize(num);
	string dustbin;
	for (int i = 0; i < num; i++)
	{
		File >> m_YaHuangIDList[i];
		getline(File, dustbin);
	}
	File.close();
}



void cFangWu::RemoveBB(cObj* yahuan)
{
	if (!yahuan)ERRBOX;
	int x, y;
	x = yahuan->GetX() / 20;
	y = (g_pMainState->m_Map.m_pMap->m_Height - yahuan->GetY()) / 20;
	for (int i = 0; i < m_List.size(); i++)
	{
		if (m_List[i].m_x ==x &&m_List[i].m_y ==y)
		{
			m_List.erase(m_List.begin() + i);
			CheckHuanJin();
			return;
		}
	}
}

void cFangWu::Save(ofstream& File)
{
	File << "\n";
	File<< m_GuiMo<<" ";//��ģ ��ģԽ��,�ܰڷŵļҾ�Խ��
	if (-1 == m_GuiMo)return;
	File << m_HuanJinDu<<" ";//������
	m_DiBan.Save(File);//�ذ�
	m_GuanJia.Save(File);//�ܼ� 
	File << m_List.size() << " ";
	for (int i = 0; i < m_List.size(); i++)
	{
		m_List[i].Save(File);
	}
	File << " " << "�Ҿ�" << " ";
	File << m_JiaJu.size()<<" ";
	for (int i = 0; i < m_JiaJu.size(); i++)
	{
		m_JiaJu[i].Save(File);
	}
	
	File << m_YaHuangIDList.size()<<" ";
	for (int i = 0; i < m_YaHuangIDList.size(); i++)
	{
		File<< m_YaHuangIDList[i]<<" ";
	}
	File << m_PreHuanJin << " ";
	File << "\n";
}

void cFangWu::Load(ifstream& File)
{
	File >> m_GuiMo ;//��ģ ��ģԽ��,�ܰڷŵļҾ�Խ��
	if (-1 == m_GuiMo)return;
	File >> m_HuanJinDu;//������  ����˯������
	m_DiBan.Load(File);//�ذ�
	m_GuanJia.Load(File);//�ܼ� 
	int num;
	File >> num; 
	m_List.clear();
	m_List.resize(num);
	for (int i = 0; i < m_List.size(); i++)
	{
		m_List[i].Load(File);
	}
	string tag;
	File >> tag;
	File >> num;
	m_JiaJu.resize(num);
	for (int i = 0; i < m_JiaJu.size(); i++)
	{
		m_JiaJu[i].Load(File);
	}
	File >> num;
	m_YaHuangIDList.clear();
	m_YaHuangIDList.resize(num);
	for (int i = 0; i < m_YaHuangIDList.size(); i++)
	{
		File >> m_YaHuangIDList[i];
	}
	File >> m_PreHuanJin ;
}

void cFangWu::SetGuiMo(int num)
{
	if (num > 4)num = 4; 
	if (m_GuiMo == num)return;
	m_GuiMo = num; 
	switch (m_GuiMo)
	{
	case 0:m_GuanJia.Set(11, 9); break;
	case 1:m_GuanJia.Set(14, 14); break;
	case 2:m_GuanJia.Set(20, 21); break;
	case 3:m_GuanJia.Set(17, 13); break;
	case 4:m_GuanJia.Set(43, 12); break;
	}
	CheckHuanJin();
}

void cFangWu::Move(int x, int y, cObj* p)
{
	int xTarget = p->GetX() / 20;
	int yTarget = (g_pMainState->m_Map.m_pMap->m_Height - p->GetY()) / 20;
	int targetID;
	switch (p->m_IndexType)
	{
	case INDEXTYEP_NPC:
		if (1)
		{
			cNpc* npc = (cNpc*)p->m_pIndex;
			targetID = npc->m_pNpcData->m_ModeId;
		}
		break;
	case INDEXTYEP_PET:
		if (1)
		{
			cPet* pPet = (cPet*)p->m_pIndex;
			targetID = pPet->m_PetDataZZ.modeid; 
		}
		break;
	default:
		return;
		break;
	}

	for (int i = 0; i < m_List.size(); i++)
	{
		if (xTarget == m_List[i].m_x)
		if (yTarget == m_List[i].m_y)
		if (targetID == m_List[i].m_modeid)
		{
			m_List[i].Set(x / 20, (g_pMainState->m_Map.m_pMap->m_Height - y) / 20);
			m_List[i].m_direction= GetDirection4(p->GetX(), p->GetY(), x, y);
			p->SetTarget(x, y);
			p->m_time = -1;
			return;
		}
	}
	if (xTarget == m_GuanJia.m_x)
	if (yTarget == m_GuanJia.m_y)
	if (targetID == m_GuanJia.m_modeid)
	{
		m_GuanJia.Set(x / 20, (g_pMainState->m_Map.m_pMap->m_Height - y) / 20);
		m_GuanJia.m_direction = GetDirection4(p->GetX(), p->GetY(), x, y);
		p->SetTarget(x, y);
		p->m_time = -1;
		return;
	}
}

void cFangWu::RemovePreJiaJu()
{
	if (g_pMainState->m_FangWu.m_JiaJu.size())
	{
		sJiaJu& item = g_pMainState->m_FangWu.m_JiaJu[g_pMainState->m_FangWu.m_JiaJu.size() - 1];
		m_HuanJinDu -= item.m_JiaJu.GetType() - 34;
		g_pMainState->m_InterfaceItem.GetItem(g_pMainState->m_HeroID, &item.m_JiaJu);
		g_pMainState->m_Map.RemoveJiaJu(&item);
		g_pMainState->FindObj();
		g_pMainState->m_FangWu.m_JiaJu.pop_back();
		CheckHuanJin();
	}
	else g_pMainState->m_Tags.Add("����û�мҾ�");
}

void cFangWu::CheckHuanJin()
{
	int newhuanjin= GetHuanJin();
	if (newhuanjin == m_PreHuanJin)return;
	g_pCharacter->m_PCData.AddGrowing(-((m_PreHuanJin) / 25));
	g_pCharacter->m_PCData.AddGrowing((newhuanjin) / 25);
	m_PreHuanJin = newhuanjin;
}

void cFangWu::GetBB()
{
	int maxnum;
	switch (g_pMainState->m_FangWu.GetGuiMo())
	{
	case 1:maxnum = 2; break;
	case 2:maxnum = 4; break;
	case 3:maxnum = 6; break;
	case 4:maxnum = 8; break;
	default:
		if (1)
		{
			g_pMainState->FindObjByName("�ܼ�")->Talk("���ݹ�ģ̫С,�Ͳ�Ҫ����լ����");
			return ;
		}
		break;
	}
	if (maxnum == g_pMainState->m_FangWu.m_List.size())
	{
		g_pMainState->FindObjByName("�ܼ�")->Talk("��լ�޵��Ѿ�������,�ͱ�������");
		return ;
	}
	sNpc npc;
	g_pMainState->m_FangWu.Init();
	cObj* pobj = g_pHeroObj;
	int rate;
	switch (g_pMainState->m_FangWu.GetGuiMo())
	{
	case 0:return ;
	case 1:rate = 5; break;
	case 2:rate = 10; break;
	case 3:rate = 15; break;
	case 4:rate = 25; break;
	default:rate = g_pMainState->m_FangWu.m_YaHuangIDList.size(); break;
	}
	int petid = g_pMainState->m_FangWu.m_YaHuangIDList[rand() % rate];
	npc.Set("��լ��", g_pMainState->m_Map.m_MapName, petid,
		pobj->GetX() / 20, (g_pMainState->m_Map.m_pMap->m_Height - pobj->GetY()) / 20);
	g_pMainState->m_FangWu.m_List.push_back(npc);
	g_pMainState->m_Map.AddNpc(&npc, &g_pMainState->m_TaskNormal, "", 9);
	CheckHuanJin();
}




void sZiZhi::Save(ofstream& File)
{
	File << " ";
	File <<m_ATKZZ << " ";
	File <<m_DEFZZ << " ";
	File <<m_HPZZ << " ";
	File <<m_MAGZZ << " ";
	File <<m_SPDZZ << " ";
	File <<m_AVOIDZZ << " ";
	File <<m_Growing << " ";
}

void sZiZhi::Load(ifstream& File)
{
	File >> m_ATKZZ;
	File >> m_DEFZZ;
	File >> m_HPZZ ;
	File >> m_MAGZZ;
	File >> m_SPDZZ;
	File >> m_AVOIDZZ ;
	File >> m_Growing ;
}

void sAddPoint::Save(ofstream& File)
{
	//�ӵ�
	File << " ";
	File << atk << " ";
	File << def << " ";
	File << hp << " ";
	File << mp << " ";
	File << spd << " ";
	File << idle << " ";
}

void sAddPoint::Load(ifstream& File)
{
	File >> atk;
	File >> def;
	File >> hp ;
	File >> mp ;
	File >> spd;
	File >> idle;
}


void sNpc::Save(ofstream& File)
{
	File << " ";
	File << m_modeid << " ";
	if (-1 == m_modeid)
	{
		if (m_mapname.size())
		{
			File << 1 << " ";
			File << m_mapname << " ";
		}
		else File << 0<<" ";
		return;
	}
	File << m_name<<" ";
	if (m_mapname == "")
	{
		File << 0 << " ";
	}
	else
	{
		File << 1 << " ";
		File << m_mapname << " ";
	}
	File << m_x << " ";
	File << m_y << " ";
	File << m_direction << " ";

	if (m_modeid >= 2000 || m_modeid < 1000)
	{//��ɫ���ٻ��޲���Ⱦɫ�ͽ���
		m_JinJieID.Save(File);
		if (m_modeid >= 2000)
		{
			File << weaponid << " ";
			File << weapontype<<" ";
		}
		m_Ranse.Save(File);
	}
	File << "\n";
}

void sNpc::Load(ifstream& File)
{
	File >> m_modeid;
	if (-1 == m_modeid)
	{
		int b;
		File >> b;
		if (b)File >> m_mapname;
		return;
	}
	File>>m_name;
	BOOL havemap;
	File >> havemap;
	if (havemap)
		File>>m_mapname;
	File>> m_x ;
	File>> m_y ;
	File >> m_direction;
	if (m_modeid >= 2000 || m_modeid <1000)
	{//��ɫ���ٻ��޲���Ⱦɫ�ͽ���
		m_JinJieID.Load(File);
		if (m_modeid >= 2000)
		{
			File >> weaponid;
			File >> weapontype;
		}
		m_Ranse.Read(m_modeid, File, m_JinJieID.m_PositionID);
	}
}

void sNpc::Set(string name, string mapname, int modeid, int x, int y, int direction /*= -1*/,int jinjie )
{
	if (jinjie > 3)jinjie = 3;
	if (name == "")
	{
		if (modeid < 1000)
		{
			name = g_pMainState->m_PetDataManager.GetPetData(modeid)->m_Name;
		}
		else
		if (modeid < 2000)
		{
			name = g_pMainState->m_pNpcList[modeid - 1000].m_name;
		}
		else
		{
			name = g_pMainState->m_pCharacterDataList[modeid - 2000].m_Name;
		}
	}
	m_name = name;
	m_mapname = mapname;
	if (direction == -1) AutoDirection4();
	else
		m_direction = direction;
	m_x = x;
	m_y = y;
	m_modeid = modeid;
	m_JinJieID.m_PositionID = jinjie;
	m_JinJieID.m_JinJieNum = jinjie;
}

void sNpc::Reset()
{
	m_JinJieID.Reset();
	weaponid = -1;
	weapontype = -1;
}

void sNpc::SetRanse(int id0, int id1 /*= -1*/, int id2 /*= -1*/, int id3 /*= -1*/, int part)
{
	m_Ranse.m_ranse[part].Set(id0, id1, id2, id3);
	g_pMainState->m_ColorManage.Give2(&m_Ranse.m_ranse[part],m_modeid,part);
}

void sChangeColorData2::Read(int modeid, ifstream& File,int posid)
{
	Reset();
	BOOL bhaveranse;
	File >> bhaveranse;
	if (!bhaveranse)return;
	if (modeid < 1000)//�ٻ���
	{
		sPetData* pPet= g_pMainState->m_PetDataManager.GetPetData(modeid);
		vector<int>& lis=  pPet->m_posIDList[posid];
		for (int i = 0; i < 4; i++)
		{
			if (i<lis.size())
				m_ranse[i].Read(File, modeid, sChangeColorData::ePet, lis[i]);
			else
				m_ranse[i].Read(File, modeid, sChangeColorData::ePet, -1);
		}
	}
	else if (modeid < 2000)
	{
		return;
	}
	else
	{
		for (int i = 0; i < 4;i++)
			m_ranse[i].Read(File, modeid - 2000, sChangeColorData::ePc, i);
	}
}

void sChangeColorData2::Save(ofstream& File)
{
	BOOL haveranse = FALSE;
	for (int i = 0; i < 4; i++)
	{
		if (m_ranse[i].partnum)haveranse = TRUE;
	}
	if (haveranse)
	{
		File << " 1 ";
		for (int i = 0; i < 4; i++)
			m_ranse[i].Save(File);
	}
	else
	{
		File << 0 << " ";
	}
}

