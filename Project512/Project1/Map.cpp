#include "Global.h"
using namespace std;
BOOL cMap::Load(CHAR* name, int mapid)
{
	while (!m_bLoadReady2)
	{
		Sleep(0);
	}
	m_MapID = mapid;
	m_bLoadReady2 = FALSE;
	m_bLoadReady = FALSE;
	g_pMainState->m_Canvas.m_Time = 0;
	Free();

	m_File.open(name, ios::binary | ios::in);
	if (!m_File)
		ERRBOX;
	m_File2.open(name, ios::binary | ios::in);
	if (!m_File2)
		ERRBOX;

	SMap_Header	FileHeader;
	m_File.read((CHAR*)&FileHeader, sizeof(SMap_Header));//读取地图头




	// 计算地图的大小,数量,并分配内存
	m_HeightNum = (int)ceil((float)FileHeader.map_height / 240.0f);
	m_WidthNum = (int)ceil((float)FileHeader.map_width / 320.0f);
	m_Width = FileHeader.map_width;// m_WidthNum * 320;
	m_Height = FileHeader.map_height;//m_HeightNum * 240;


	//----------------------------------
	// 读地图块的偏移信息
	//----------------------------------
	int block_num = m_WidthNum * m_HeightNum;
	m_NumBlock = block_num;
	m_pBlockOffset = new UINT[block_num];
	m_File.read((char*)m_pBlockOffset, sizeof(UINT)*block_num);

	//----------------------------------
	// 读SMaskHead数据
	//----------------------------------
	SMaskHead maskHead;
	m_File.read((char*)&maskHead, sizeof(SMaskHead));


	//----------------------------------
	// 读mask数据的偏移信息
	//----------------------------------
	m_pMaskDataOffset = new UINT[maskHead.mask_num];
	m_File.read((char*)m_pMaskDataOffset, sizeof(UINT)*maskHead.mask_num);

	//创建mask矩形
	m_pMaskRect = new sMASKRect[maskHead.mask_num];
	//----------------------------------
	// 填充矩形数据
	//----------------------------------
	m_MaskNum = maskHead.mask_num;

	m_align_width = new int[m_MaskNum];
	m_pMaskDataDec = new BYTE*[m_MaskNum];
	for (int i = 0; i < m_MaskNum; i++)
	{
		m_align_width[i] = 0;
		m_pMaskDataDec[i] = 0;
	}
	for (long i = 0; i < m_MaskNum; i++)
		ReadMask(m_pMaskDataOffset[i], i);

	//创建线程
	if (!m_hThread)
	{
		m_hThread = (HANDLE)_beginthreadex(NULL, 0,ThreadProc, this, CREATE_SUSPENDED, NULL);
		//m_hThread = CreateThread(0, 0, ThreadProc, this, CREATE_SUSPENDED, 0);
	}
	return TRUE;
}

void cMap::ReadBlock(int block_offset,WORD* map)
{
	m_pTexture = map;
	//读取数据头
	SBlockInfo blockInfo = { 0 };

	m_File.seekg(block_offset, ios_base::beg);
	m_File.read((char*)&blockInfo.UnitN, sizeof(UINT) );
	if (blockInfo.UnitN>0)
	{
		if (blockInfo.UnitN > m_UnitNDataSize)
		{
			m_UnitNDataSize = blockInfo.UnitN;
			SAFE_DELETE_ARRAY(m_pUnitNData);
			m_pUnitNData = new BYTE[m_UnitNDataSize << 2];
		}
		m_File.read((char*)m_pUnitNData, blockInfo.UnitN << 2);
	}
	
	m_File.read((char*)&blockInfo.flag, 8 );
	
	if (m_MapJpgDataSize < blockInfo.size)
	{
		m_MapJpgDataSize=blockInfo.size;
		SAFE_DELETE_ARRAY(m_pMapJpgData);
		m_pMapJpgData = new BYTE[m_MapJpgDataSize];
	}
	m_File.read((char*)m_pMapJpgData, blockInfo.size);

	UINT jpg_size = 0;
	BYTE* pJpgData = JpgHandler(m_pMapJpgData, blockInfo.size, &jpg_size);
	// 解压jpg
	BMP* pBmpData = Unpak_jpg(pJpgData, jpg_size);	// 解压后的像素格式为ARGB565
	if (pBmpData == 0)  return;

	memcpy_s(m_pTexture, 153600, pBmpData->line[0], 153600);
	destroy_bitmap(pBmpData);

}

BYTE* cMap::JpgHandler(BYTE* Buffer, int inSize, UINT* outSize)
{

	// JPEG数据处理原理
	// 1、复制D8到D9的数据到缓冲区中
	// 2、删除第3、4个字节 FFA0
	// 3、修改FFDA的长度00 09 为 00 0C
	// 4、在FFDA数据的最后添加00 3F 00
	// 5、替换FFDA到FF D9之间的FF数据为FF 00

	UINT TempNum = 0;						// 临时变量，表示已读取的长度
	BYTE* outBuffer;
	if (inSize > m_TempBufferNum)
	{
		m_TempBufferNum = inSize;
		SAFE_DELETE_ARRAY(m_pTempBuffer);
		m_pTempBuffer = new BYTE[inSize <<1];		// 临时变量，表示处理后的数据
	}
	outBuffer = m_pTempBuffer;					// 已处理数据的开始地址
	WORD TempTimes = 0;					// 临时变量，表示循环的次数
	UINT Temp = 0;

	// 当已读取数据的长度小于总长度时继续
	while (TempNum < inSize && *Buffer++ == 0xFF)
	{
		*m_pTempBuffer++ = 0xFF;
		TempNum++;
		switch (*Buffer)
		{
		case 0xD8:
			*m_pTempBuffer++ = 0xD8;
			*Buffer++;
			TempNum++;
			break;
		case 0xA0:
			*Buffer++;
			m_pTempBuffer--;
			TempNum++;
			break;
		case 0xC0:
			*m_pTempBuffer++ = 0xC0;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes, Buffer, sizeof(WORD)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序


			for (int i = 0; i< TempTimes; i++)
			{
				*m_pTempBuffer++ = *Buffer++;
				TempNum++;
			}

			break;
		case 0xC4:
			*m_pTempBuffer++ = 0xC4;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes, Buffer, sizeof(WORD)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序

			for (int i = 0; i< TempTimes; i++)
			{
				*m_pTempBuffer++ = *Buffer++;
				TempNum++;
			}
			break;
		case 0xDB:
			*m_pTempBuffer++ = 0xDB;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes, Buffer, sizeof(WORD)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序

			for (int i = 0; i< TempTimes; i++)
			{
				*m_pTempBuffer++ = *Buffer++;
				TempNum++;
			}
			break;
		case 0xDA:
			*m_pTempBuffer++ = 0xDA;
			*m_pTempBuffer++ = 0x00;
			*m_pTempBuffer++ = 0x0C;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes, Buffer, sizeof(WORD)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序
			*Buffer++;
			TempNum++;
			*Buffer++;

			for (int i = 2; i< TempTimes; i++)
			{
				*m_pTempBuffer++ = *Buffer++;
				TempNum++;
			}
			*m_pTempBuffer++ = 0x00;
			*m_pTempBuffer++ = 0x3F;
			*m_pTempBuffer++ = 0x00;
			Temp += 1; // 这里应该是+3的，因为前面的0xFFA0没有-2，所以这里只+1。

			// 循环处理0xFFDA到0xFFD9之间所有的0xFF替换为0xFF00
			for (; TempNum < inSize - 2;)
			{
				if (*Buffer == 0xFF)
				{
					*m_pTempBuffer++ = 0xFF;
					*m_pTempBuffer++ = 0x00;
					*Buffer++;
					TempNum++;
					Temp++;
				}
				else
				{
					*m_pTempBuffer++ = *Buffer++;
					TempNum++;
				}
			}
			// 直接在这里写上了0xFFD9结束Jpeg图片.
			Temp--; // 这里多了一个字节，所以减去。
			*m_pTempBuffer--;
			*m_pTempBuffer-- = 0xD9;
			break;
		case 0xD9:
			// 算法问题，这里不会被执行，但结果一样。
			*m_pTempBuffer++ = 0xD9;
			TempNum++;
			break;
		default:
			break;
		}
	}
	Temp += inSize;
	*outSize = Temp;
	m_pTempBuffer = outBuffer;
	return outBuffer;
}






cMap::cMap()
{
	m_pMaskDataOffset = 0;
	m_pBlockOffset = 0;
	m_pMaskRect = 0;
	m_WidthNum = 0;
	m_HeightNum = 0;
	m_Width = 0;
	m_Height = 0;
	m_hThread = 0;
	m_bLoadReady = TRUE;
	m_bLoadReady2 = TRUE;
	jpeg_setcolormode(0, 1);

	m_pBmpinfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO)+sizeof(RGBQUAD)* 2);
	m_pBmpinfo->bmiHeader.biPlanes = 1;
	m_pBmpinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBmpinfo->bmiHeader.biBitCount = 16;
	m_pBmpinfo->bmiHeader.biClrUsed = 0;
	m_pBmpinfo->bmiHeader.biClrImportant = 0;
	m_pBmpinfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBmpinfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBmpinfo->bmiHeader.biWidth = 320;
	m_pBmpinfo->bmiHeader.biHeight = -240;
	m_pBmpinfo->bmiHeader.biSizeImage = 320 * 240 * 2;
	m_pBmpinfo->bmiHeader.biCompression = 0;


	DWORD colorbit = 0xf800;
	memmove(&g_pBmpinfo->bmiColors[0], &colorbit, sizeof(RGBQUAD));
	colorbit = 0x7e0;
	memmove(&g_pBmpinfo->bmiColors[1], &colorbit, sizeof(RGBQUAD));
	colorbit = 0x1f;
	memmove(&g_pBmpinfo->bmiColors[2], &colorbit, sizeof(RGBQUAD));

	m_BitmapID.resize(1600);
	m_pBitmap = new BYTE*[1600];
	for (int i = 0; i < 1600; i++)
	{
		m_BitmapID[i] = -1;
		m_pBitmap[i] = 0;
	}
}

BOOL cMap::Show(BYTE* pPic, RECT rect)
{
	int xstart = rect.left + m_xCenterOffset;
	int ystart = rect.top + m_yCenterOffset;
	int xstart2 = (xstart) % 320;
	int ystart2 = (ystart) % 240;
	int ycube = 0;
	int xcube = 0;
	int xOff1, yOff1;
	if (ystart < 240)
	{
		ycube = -1;
		yOff1 = ystart;//第一排要加120
	}
	else
	if (ystart < 480)
	{
		ycube = 0;
		yOff1 = ystart - 240;
	}
	else
	if (ystart < 720)
	{
		ycube = 1;
		yOff1 = ystart - 480;
	}
	else
		return FALSE;

	if (xstart < 320)
	{
		xcube = -1;
		xOff1 = xstart; //第一排的图片只有半张,所以要+160
	}
	else
	if (xstart < 640)
	{
		xcube = 0;
		xOff1 = xstart - 320;
	}
	else
	if (xstart < 960)
	{
		xcube = 1;
		xOff1 = xstart - 640;
	}
	else
		return FALSE;
	int k = m_xCenterNum + xcube + (ycube + m_yCenterNum)*m_WidthNum;


	int w1 = 32;
	if ((xstart2) > 288)
		w1 = 320 - (xstart2);

	int h1 = 32;
	if ((ystart2) > 208)
		h1 = 240 - (ystart2);

	if (k > -1)
	{
		BYTE* P1 = pPic;
		BYTE* P2 = &m_pBitmap[k][(320 * yOff1 + xOff1) << 1];
		int w2 = w1 << 1;
		for (int i = 0; i < h1; i++)
		{
			memcpy_s(P1, 64, P2, w2);
			P1 += 64;
			P2 += 640;
		}
	}
	int index11, index22;
	//图块可能在边界
	if ((xstart2 > 288) && (xcube < 1))
	{
		int k2 = k + 1;
		if (k2 < m_NumBlock&&k2>-1)
		{
			int index = 640 * yOff1;
			int index2 = w1 << 1;
			index11 = index;
			index22 = index2;
			int  w2 = 32 - w1;
			for (int i = 0; i < h1; i++)
			{
				for (int j = 0; j < w2; j++)
				{
					pPic[index22] = m_pBitmap[k2][index11];
					pPic[index22 + 1] = m_pBitmap[k2][index11 + 1];
					index11 += 2;
					index22 += 2;
				}
				index += 640;
				index2 += 64;
				index11 = index;
				index22 = index2;
			}
		}
	}

	if ((ystart2 > 208) && (ycube < 1))
	{
		int k3 = k + m_WidthNum;
		if (k3 < m_NumBlock&&k3>-1)
		{
			int index = xOff1 << 1;
			int index2 = h1 << 6;
			index11 = index;
			index22 = index2;
			int h2 = 32 - h1;
			for (int i = 0; i < h2; i++)
			{
				for (int j = 0; j < w1; j++)
				{
					pPic[index22] = m_pBitmap[k3][index11];
					pPic[index22 + 1] = m_pBitmap[k3][index11 + 1];
					index11 += 2;
					index22 += 2;
				}
				index += 640;
				index2 += 64;
				index11 = index;
				index22 = index2;
			}
		}
	}

	if ((xstart2 > 288) && (ystart2 > 208) && (xcube < 1) && (ycube < 1))
	{
		k = k + 1 + m_WidthNum;
		if (k < m_NumBlock&&k>-1)
		{
			int h2 = 32 - h1;
			int w2 = 32 - w1;
			int index = 0;
			int index2 = ((h1 << 5) + w1) << 1;
			index11 = index;
			index22 = index2;
			for (int i = 0; i < h2; i++)
			{
				for (int j = 0; j < w2; j++)
				{
					pPic[index22] = m_pBitmap[k][index11];
					pPic[index22 + 1] = m_pBitmap[k][index11 + 1];
					index11 += 2;
					index22 += 2;
				}
				index += 640;
				index2 += 64;
				index11 = index;
				index22 = index2;
			}
		}
	}
	////图块可能在边界
	//if ((xstart2 > 288) && (ystart2 > 208) && (xcube < 1) && (ycube < 1))
	//{
	//	k = k + 1 + m_WidthNum;
	//	if (k < m_NumBlock&&k>-1)
	//	{
	//		int k3 = k + m_WidthNum;
	//		if (k3 < m_NumBlock&&k3>-1)
	//		{
	//			BYTE* P1 = &pPic[((h1 << 5) + w1) << 1];
	//			BYTE* P2 = &m_pBitmap[k][0];
	//			int h2 = 32 - h1;
	//			int w2 = (32 - w1) << 1;
	//			for (int i = 0; i < h2; i++)
	//			{
	//				memcpy_s(P1, 64, P2, w2);
	//				P1 += 64;
	//				P2 += 640;
	//			}
	//		}
	//	}
	//}

	//if ((xstart2 > 288) && (xcube < 1))
	//{
	//	int k2 = k + 1;
	//	if (k2 < m_NumBlock&&k2>-1)
	//	{
	//		BYTE* P1 = &pPic[w1 << 1];
	//		BYTE* P2 = &m_pBitmap[k2][640 * yOff1];
	//		int  w2 = (32 - w1) << 1;
	//		for (int i = 0; i < h1; i++)
	//		{
	//			memcpy_s(P1, 64, P2, w2);
	//			P1 += 64;
	//			P2 += 640;
	//		}
	//	}
	//}

	//if ((ystart2 > 208) && (ycube < 1))
	//{
	//	int k3 = k + m_WidthNum;
	//	if (k3 < m_NumBlock&&k3>-1)
	//	{
	//		BYTE* P1 = &pPic[h1 << 6];
	//		BYTE* P2 = &m_pBitmap[k3][xOff1 << 1];
	//		int  w2 = w1 << 1;
	//		int h2 = 32 - h1;
	//		for (int i = 0; i < h2; i++)
	//		{
	//			memcpy_s(P1, 64, P2, w2);
	//			P1 += 64;
	//			P2 += 640;
	//		}
	//	}
	//}

	

	return TRUE;
}





void cMap::WriteMapPixel(WORD* pBmp16, int width, int height, int pitch)
{

	WORD* pDst = m_pTexture;
	WPixel pix;
	for (int h = 0; h<height; h++)
	{
		for (int i = 0; i < width; i++)
		{
			/*pix = *(pBmp16 + i);
			pDst[i] = pix.color;*/
			pDst[i] =* (pBmp16 + i);
		}
		pBmp16 += width;
		pDst += width;
	}

}

BOOL cMap::MoveMap()
{
	g_pMainState->FindObj();

	m_xCenterNum = GetXCenter() / 320;
	m_yCenterNum = GetYCenter() / 240;
	CountMaskList();
	ResumeThread(m_hThread);
	return TRUE;
}

unsigned int WINAPI cMap::ThreadProc(_In_ LPVOID lpParameter)

{
	cMap*  pMap = (cMap*)lpParameter;
	while (1)
	{
		pMap->m_bLoadReady = FALSE;
		pMap->m_bLoadReady2 = FALSE;
		int HeightNum = pMap->m_yCenterNum;
		int WidthNum =  pMap->m_xCenterNum;
		pMap->m_NowMapNum = (HeightNum)*pMap->m_WidthNum + WidthNum;
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				int k = pMap->m_NowMapNum + j + i * pMap->m_WidthNum;
				//防止数组越界
				if (k < 0)
					continue;
				if (i + HeightNum >= pMap->m_HeightNum)
					continue;
				if (j + WidthNum >= pMap->m_WidthNum)
					continue;
				if (pMap->m_MapID != pMap->m_BitmapID[k])
				{
					pMap->m_BitmapID[k] = pMap->m_MapID;
					if (!pMap->m_pBitmap[k])
						pMap->m_pBitmap[k] = new BYTE[153600];
					pMap->ReadBlock(pMap->m_pBlockOffset[k],(WORD*)pMap->m_pBitmap[k]);
				}

			}
		}


		pMap->m_bLoadReady = TRUE;

		for (int i = -2; i < 3; i += 4)
		for (int j = -2; j < 3; j++)
		{
			int k = pMap->m_NowMapNum + j + i*pMap->m_WidthNum;
			//防止数组越界
			if (k < 0)
				continue;
			if (i + HeightNum >= pMap->m_HeightNum)
				continue;
			if (j + WidthNum >= pMap->m_WidthNum)
				continue;

			if (pMap->m_MapID != pMap->m_BitmapID[k])
			{
				pMap->m_BitmapID[k] = pMap->m_MapID;
				if (!pMap->m_pBitmap[k])pMap->m_pBitmap[k] = new BYTE[153600];
				pMap->ReadBlock(pMap->m_pBlockOffset[k], (WORD*)pMap->m_pBitmap[k]);
			}
		}
		
		for (int i = -1; i < 2; i++)
		{

				for (int j = -2; j < 3; j += 4)
				{

					int k = pMap->m_NowMapNum + j + i*pMap->m_WidthNum;
					//防止数组越界
					if (k < 0)
						continue;
					if (i + HeightNum >= pMap->m_HeightNum)
						continue;
					if (j + WidthNum >= pMap->m_WidthNum)
						continue;

					if (pMap->m_MapID != pMap->m_BitmapID[k])
					{
						pMap->m_BitmapID[k] = pMap->m_MapID;
						if (!pMap->m_pBitmap[k])pMap->m_pBitmap[k] = new BYTE[153600];
						{
							pMap->ReadBlock(pMap->m_pBlockOffset[k], (WORD*)pMap->m_pBitmap[k]);
						}
					}
				}
				
		}
	
		pMap->m_bLoadReady2 = TRUE;
		//加载完就睡觉
		SuspendThread(pMap->m_hThread);
	}
	return 0;
}

void cMap::Free()
{
	SAFE_DELETE_ARRAY(m_pBlockOffset);
	SAFE_DELETE_ARRAY(m_pMaskDataOffset);
	SAFE_DELETE_ARRAY(m_pMaskRect);
	for (int i = 0; i < m_MaskNum; i++)
	{
		SAFE_DELETE_ARRAY(m_pMaskDataDec[i]);
	}
	SAFE_DELETE_ARRAY(m_pMaskDataDec);
	SAFE_DELETE_ARRAY(m_align_width);

	m_MaskNum = 0;
	m_File.close();
	m_File2.close();
}


void cMap::ReadMask(int offset, int mask_index)
{

	SMaskInfo maskInfo = { 0 };

	// 读mask文件头
	m_File.seekg(offset, ios_base::beg);
	m_File.read((char*)&maskInfo, sizeof(SMaskInfo));

	if (maskInfo.width == 0 || maskInfo.height == 0 || maskInfo.mask_size == 0)
		return;

	m_pMaskRect[mask_index].w = maskInfo.width;
	m_pMaskRect[mask_index].h = maskInfo.height;
	m_pMaskRect[mask_index].x = maskInfo.startX;
	m_pMaskRect[mask_index].y = maskInfo.startY;
//	m_pMaskRect[mask_index].Index = mask_index;
}

int cMap::DecompressMask(void* in, void* out)
{

	register BYTE *op;
	register  BYTE *ip;
	register unsigned t;
	register  BYTE *m_pos;

	op = (BYTE *)out;
	ip = (BYTE *)in;

	if (*ip > 17) {
		t = *ip++ - 17;
		if (t < 4)
			goto match_next;
		do *op++ = *ip++; while (--t > 0);
		goto first_literal_run;
	}

	while (1) {
		t = *ip++;
		if (t >= 16) goto match;
		if (t == 0) {
			while (*ip == 0) {
				t += 255;
				ip++;
			}
			t += 15 + *ip++;
		}

		*(unsigned *)op = *(unsigned *)ip;
		op += 4; ip += 4;
		if (--t > 0)
		{
			if (t >= 4)
			{
				do {
					*(unsigned *)op = *(unsigned *)ip;
					op += 4; ip += 4; t -= 4;
				} while (t >= 4);
				if (t > 0) do *op++ = *ip++; while (--t > 0);
			}
			else do *op++ = *ip++; while (--t > 0);
		}

	first_literal_run:

		t = *ip++;
		if (t >= 16)
			goto match;

		m_pos = op - 0x0801;
		m_pos -= t >> 2;
		m_pos -= *ip++ << 2;

		*op++ = *m_pos++; *op++ = *m_pos++; *op++ = *m_pos;

		goto match_done;

		while (1)
		{
		match:
			if (t >= 64)
			{

				m_pos = op - 1;
				m_pos -= (t >> 2) & 7;
				m_pos -= *ip++ << 3;
				t = (t >> 5) - 1;

				goto copy_match;

			}
			else if (t >= 32)
			{
				t &= 31;
				if (t == 0)	{
					while (*ip == 0) {
						t += 255;
						ip++;
					}
					t += 31 + *ip++;
				}

				m_pos = op - 1;
				m_pos -= (*(unsigned short *)ip) >> 2;
				ip += 2;
			}
			else if (t >= 16) {
				m_pos = op;
				m_pos -= (t & 8) << 11;
				t &= 7;
				if (t == 0) {
					while (*ip == 0) {
						t += 255;
						ip++;
					}
					t += 7 + *ip++;
				}
				m_pos -= (*(unsigned short *)ip) >> 2;
				ip += 2;
				if (m_pos == op)
					goto eof_found;
				m_pos -= 0x4000;
			}
			else {
				m_pos = op - 1;
				m_pos -= t >> 2;
				m_pos -= *ip++ << 2;
				*op++ = *m_pos++; *op++ = *m_pos;
				goto match_done;
			}

			if (t >= 6 && (op - m_pos) >= 4) {
				*(unsigned *)op = *(unsigned *)m_pos;
				op += 4; m_pos += 4; t -= 2;
				do {
					*(unsigned *)op = *(unsigned *)m_pos;
					op += 4; m_pos += 4; t -= 4;
				} while (t >= 4);
				if (t > 0) do *op++ = *m_pos++; while (--t > 0);
			}
			else {
			copy_match:
				*op++ = *m_pos++; *op++ = *m_pos++;
				do *op++ = *m_pos++; while (--t > 0);
			}

		match_done:

			t = ip[-2] & 3;
			if (t == 0)	break;

		match_next:
			do *op++ = *ip++; while (--t > 0);
			t = *ip++;
		}
	}

eof_found:
	//   if (ip != ip_end) return -1;

	return (op - (BYTE*)out);
}



BOOL cMap::isInRect(int x1, int y1, int w1, int h1, int x, int y, int w, int h)
{
	if (x1>x + w)
		return FALSE;
	if (x1 + w1<x)
		return FALSE;
	if (y1>y + h)
		return FALSE;
	if (y1 + h1<y)
		return FALSE;
	return TRUE;
}

void cMap::ScreenToWorld(int x, int y, int& xOut, int& yOut)
{
	xOut = x + GetXCenter() - 320;
	yOut = 480 - y + GetYCenter() - 120;
}

void cMap::WorldToScreen(int x, int y, int& xOut, int& yOut)
{
	xOut = x - GetXCenter() + 320;
	yOut = y - GetYCenter() + 120;
}



BOOL cMyMap::LoadMap(long x, long y, const CHAR* Name, BOOL bFly)
{
	if (bFly)
	{
		if (g_pMainState->m_GlobalButton[10])
		{
			g_pMainState->m_Tags.Add("当前无法飞行");
			return TRUE;
		}
	}
	if (m_LockList.size())
	{
		for (int i = 0; i < m_LockList.size();i++)
			if (0 == m_LockList[i].compare(Name))return TRUE;
	}
	if (g_StateType != STATE_MAIN)return FALSE;
	while (!m_pMap->m_bLoadReady2)
	{
		Sleep(0);
	}
	g_pMainState->m_InterfaceSmallMap.OnOff(FALSE);
	g_pMainState->m_Dialog.Close();
	g_pMainState->m_TaskNormal.m_pNpc = 0;
	if (g_pMainState->m_pNowSelect)
	{
		g_pMainState->m_pNowSelect->SetShine(FALSE);
		g_pMainState->m_pNowSelect = 0;
	}
	for (int i = 1; i < MAX_OBJ_NUM; i++)
	{
		g_pMainState->m_pObj[i] = 0;
	}
	string prename = m_MapName;
	m_MapName = Name;
	ostringstream oss;
	oss <<g_MapPath<< Name << ".txt";
	string txtpath = oss.str();
	oss.str("");
	ifstream   file;
	file.open(txtpath.c_str());//,ios::binary|ios::in);
	if (!file)
	{
		m_MapName = prename;
		return FALSE;
	}
	int MapNumber;
	file >> MapNumber;    //读取地图编号
	if (MapNumber<0 || MapNumber>20000)
	{
		ERRBOX;
	}
	if (prename != m_MapName)
	{
		Free(); //在释放之前先要得到新地图的名字	
		oss << g_MapPath << MapNumber << ".map";
		string  path = oss.str();
		oss.str("");
		oss <<MapNumber;
		string  path1 = oss.str();
		oss.str("");
		m_pMap->Load((CHAR*)path.c_str(),MapNumber);
		m_pBmp->Load((CHAR*)path1.c_str());
		g_pMainState->m_FindPt.ResetMap(this);

	}
	
	m_pMap->SetCenter((float)x * 20, (float)m_pMap->m_Height - y * 20);
	for (int i = 0; i < 5; i++)
	{
		if (g_pMainState->m_FightList[i].isLive)
		{
			g_pMainState->m_FightList[i].m_pData->SetPos(x * 20, m_pMap->m_Height - y * 20);
			g_pMainState->m_FightList[i].m_pData->Stand();
			g_pMainState->m_FightList[i].m_pData->MoveDialog();
		}
	}
	m_pMap->m_xCenterNum = m_pMap->GetXCenter() / 320;
	m_pMap->m_yCenterNum = m_pMap->GetYCenter() / 240;
	m_pMap->m_xCenterOffset = m_pMap->GetXCenter() % 320;
	m_pMap->m_yCenterOffset = m_pMap->GetYCenter() % 240;
	m_pMap->MoveMap();
	if (prename != m_MapName)
	{
		file >> g_pMainState->m_MusicName;
		if (prename != m_MapName)
			g_pMainState->m_Mp3.Load(g_pMainState->m_MusicName);
		//小地图
		DWORD smallmapid;
		file >> smallmapid;
		
		if (smallmapid)
		{
			if (prename != m_MapName)
			{
				m_SmallMap.Load(smallmapid);//小地图ID
				float w11 = (float)m_pMap->m_Width;
				float w22 = (float)(m_SmallMap.m_Picture.m_Width - 36);
				m_bili = w11 / w22;
				//调整图片位置
				m_SmallMap.m_Picture.m_x = 320 - m_SmallMap.m_Picture.m_Width / 2;
				m_SmallMap.m_Picture.m_y = 240 - m_SmallMap.m_Picture.m_Height / 2;
			}
		}
		else m_bili = 0;
		int numofnpc;
		file >> numofnpc;    //读取NPC数
	
		string name1;
		int x, y;
		string script;
		int direction;
		int modid;
		m_numberOfNpc = 0;
		sChangeColorData2 ranse;
		cJinJieNum jinjie;
		for (int i = 0; i < numofnpc; i++)
		{
			//读取名字
			file >> name1;
			
			file >> modid;
			//设置NPC位置
			file >> x;
			file >> y;
		
			//读取脚本
			file >> script;
			if (script == "单")
			{
				file>> m_pNpc[m_numberOfNpc]->m_ScriptOneType;
				file >> m_pNpc[m_numberOfNpc]->m_ScriptOneId;
				if (m_pNpc[m_numberOfNpc]->m_ScriptOneType == 1013)//宝箱
				{
					int baoxiangid;
					int baoxiangindex;
					g_pMainState->m_ZhiXian.GetBaoXiangID(m_pNpc[m_numberOfNpc]->m_ScriptOneId, baoxiangid, baoxiangindex);
					if (CheckINT(baoxiangid, baoxiangindex))
					{
						string dustbin;
						getline(file, dustbin);
						continue;
					}
				}
			}
			file >> direction;
			int weapontype = -1;
			int	weaponid = -1;
			if (modid >= 2000||modid<1000)
			{
				if (modid < 1000)
				{
					file >> jinjie.m_PositionID;
					ranse.Read(modid, file, jinjie.m_PositionID);
					
					AddNpc(name1, modid, x, y, direction, 0, script, FALSE, FALSE, -1, -1, &ranse,jinjie.m_PositionID);
				}
				else
				{
					
					file >> weapontype;
					file >> weaponid;
					ranse.Read(modid, file, jinjie.m_PositionID);
					AddNpc(name1, modid, x, y, direction, 0, script, FALSE, FALSE, weapontype, weaponid, &ranse);
				}
			}
			else
			{

				AddNpc(name1, modid, x, y, direction, 0, script, FALSE, FALSE);
			}

		}
		//读取建筑数
		file >> m_numofJianZhu;

		for (int i = 0; i < m_numofJianZhu; i++)
		{
			int xpos, ypos;
			file >> m_JianZhuID[i];
			file >> xpos;
			file >> ypos;

			xpos *= 20;
			ypos = m_pMap->m_Height - ypos * 20;


			m_pJianZhu[i]->SetXY(xpos, ypos);
			file >> m_pJianZhu[i]->m_ShowOnce;
			file >> m_pJianZhu[i]->m_PosType;

		}
		//读取传送点
		file >> m_numberOfTransmit;
	
		m_pTransmit = new sTransmit[m_numberOfTransmit];
		for (int i = 0; i < m_numberOfTransmit; i++)
		{
			file >> m_pTransmit[i].m_x;
			file >> m_pTransmit[i].m_y;
			file >> m_pTransmit[i].name;
			file >> m_pTransmit[i].m_xTarget;
			file >> m_pTransmit[i].m_yTarget;
			file >> m_pTransmit[i].m_range;

			if (m_pTransmit[i].m_range>100)
			{
				ERRBOX;
				return FALSE;
			}


			m_pTransmit[i].m_x *= 20;
			m_pTransmit[i].m_y *= 20;
			m_pTransmit[i].m_range *= 20;
			m_pTransmit[i].m_y = m_pMap->m_Height - m_pTransmit[i].m_y;


			m_JianZhuID[m_numofJianZhu] = 0x7F4CBC8C;
			m_pJianZhu[m_numofJianZhu]->SetXY(m_pTransmit[i].m_x, m_pTransmit[i].m_y);
			m_pJianZhu[m_numofJianZhu]->m_ShowOnce = FALSE;
			m_pJianZhu[m_numofJianZhu]->m_PosType = 0;
			m_numofJianZhu += 1;

		}
		//场景类型
		file >> m_MapType; //场景类型
		
		switch (m_MapType)
		{
		case 1:
		case 2://明雷场景
			if (1)
			{
				file >> m_NumOfEmemyType;
				for (int i = 0; i < m_NumOfEmemyType; i++)
				{
					file >> m_EnemyList[i];
				}
				file >> m_EnemyLv;
				file >> m_Weaponlv;
				file >> m_WeaponRate;
				switch (m_MapType)
				{
				case 1:
					if (m_EnemyLv+5<g_pCharacter->m_PCData.m_LV)
						g_pMainState->m_InterfaceDoor.SetTrigger(&g_pMainState->m_TaskNormal, 1,600); //遇怪开关
					else
						g_pMainState->m_InterfaceDoor.SetTrigger(&g_pMainState->m_TaskNormal, 1, 400); //遇怪开关
					break;
				case 2://
					if (1)
					{
						g_pMainState->m_InterfaceDoor.SetTrigger(&g_pMainState->m_TaskNormal, 3); //遇怪开关
						int num;
						file >> num;//怪物的数量
						for (int i = 0; i < num; i++)
						{
							sPetData* petdata = g_pMainState->m_PetDataManager.GetPetData(m_EnemyList[rand() % m_NumOfEmemyType]);
							int xpos, ypos;
							g_pMainState->m_Map.AutoGetPos(xpos, ypos, m_MapName);
							AddNpc(petdata->m_Name, petdata->m_ID, xpos, ypos, Direction8To4(rand() % 10), 0, "", 3, FALSE);
						}
					}
					break;
				}
			}
			break;
		default:
			g_pMainState->m_InterfaceDoor.SetTrigger(0);
			break;

		}
		g_pMainState->m_TriggerObjMap.Process();
	}
	file.close();

	m_MapNumber=MapNumber;
	
	g_pMainState->m_Canvas.SetRedrawAll();
	g_pMainState->m_InterfaceDoor.SetName(m_MapName);

	g_pMainState->m_InterfaceDoor.UpdatePos(g_pHeroObj);
	
	m_pMap->CountMaskList();

	
	g_pMainState->FindObj();
	return TRUE;
}

void cMyMap::LoadMap(ifstream& File)
{
	int num;
	File >> num;
	m_LockList.resize(num);
	for (int i = 0; i < num; i++)
		File >> m_LockList[i];
	File >> num;
	m_RangeTriggerList.resize(num);
	for (int i = 0; i < m_RangeTriggerList.size(); i++)
		m_RangeTriggerList[i].Load(File);
}

void cMyMap::Save(ofstream& File)
{
	File <<"\n"<< m_LockList.size()<<" ";
	for (int i = 0; i < m_LockList.size(); i++)
		File << m_LockList[i]<<" ";
	File << m_RangeTriggerList.size() << " ";
	for (int i = 0; i < m_RangeTriggerList.size(); i++)
		m_RangeTriggerList[i].Save(File);
}

cMyMap::cMyMap()
{
	m_pMap = new cMap();
	m_pBmp = new cBmp();
	m_pTransmit = 0;
}



cMyMap::~cMyMap()
{
	Free();
	SAFE_DELETE(m_pBmp);
	SAFE_DELETE(m_pMap);
	if (m_pNpc.size())
	for (int i = 0; i < MAX_NPC_NUM; i++)
	{
		SAFE_DELETE(m_pNpc[i]);
		SAFE_DELETE(m_pJianZhu[i]);
	}
}

BOOL cMyMap::CheckAlong(int xPos, int yPos, int xTarget, int yTarget)
{
	float xPos2 = (float)xPos;
	float yPos2 = (float)yPos;
	float dus1 =(float)( yTarget - yPos);
	float dus2 =(float)( xTarget - xPos);
	float angel = atan2(dus1 , dus2);
	float yMove = 5 * sin(angel);
	float xMove = 5 * cos(angel);
	while (true)
	{
		if (abs(xPos2 - xTarget) < 5 && abs(yPos2 - yTarget) < 5)return TRUE;
		xPos2 += xMove;
		yPos2 += yMove;
		int dus3, dus4;
		dus3 = (int )xPos2;
		dus4 = (int)yPos2;
		if (m_pBmp->isOK(dus3,dus4 ))return FALSE;
	}
	return FALSE;
}

BOOL cMyMap::Free()
{
	m_pMap->Free();
	SAFE_DELETE_ARRAY(m_pTransmit);
	for (int i = 0; i < m_numberOfNpc; i++)
	{
		m_pNpc[i]->Free();
		m_pJianZhu[i]->Free();
	}
	m_numberOfNpc = 0;
	return TRUE;
}

BOOL cMyMap::Init()
{
	if (m_bInit)return TRUE;
	m_bInit = TRUE;
	m_pNpc.resize(MAX_NPC_NUM);
	m_pJianZhu.resize(MAX_NPC_NUM);
	m_JianZhuID.resize(MAX_NPC_NUM);
	
	for (int i = 0; i < MAX_NPC_NUM; i++)
	{
		m_pNpc[i] = new cObj;
		m_pNpc[i]->Init();
		m_pNpc[i]->m_bCheckWall = TRUE;
		m_pNpc[i]->m_TrueName.m_hFont = g_pMainState->m_hFont[0];

		m_pJianZhu[i] = new CWFile;
	}
	


	return TRUE;
}

void cMyMap::AddNpc(string name, int id, int x, int y, int direction, cTaskFather* p /*= 0*/, string path,
	int stage, BOOL NeedFind, int weapontype, int weaponid, sChangeColorData2* ranse,int positionID)
{
	//设置NPC位置
	x *= 20;
	y = m_pMap->m_Height - y * 20;

	cObj* target = m_pNpc[m_numberOfNpc];
	m_numberOfNpc += 1;
	target->InitScript(path);
	//读取名字
	target->m_TrueName.SetColor(RGB(224, 212, 40)); //NPC名字的颜色
	target->m_TrueName.SetString(name);   //名字
	//读取NPC模型
	g_pMainState->FindData(target, id);
	
	target->SetXY(x, y);
	target->SetTarget(x, y);
	target->MoveName();
	target->MoveShadow();
	target->m_time = -2;
	
	target->SetDirection(direction);
	target->m_TaskTrigger.m_pTask = p;
	target->m_TaskTrigger.m_Stage = stage;
	
	
	if (weapontype >-1)
	{
		cCharacter* pc = (cCharacter*)target->m_pIndex;
		pc->m_PCData.m_Equip[2].Set(weapontype, weaponid);
		pc->m_PCData.m_Equip[2].m_Num = 1;
	}
	
	if (id >= 2000 || id<1000)
	{
		if (ranse)
		{
			if (ranse->m_ranse[0].partnum)
			{
				cPetData* pc;
				target->GetDateP(pc);
				pc->m_RanSe = *ranse;
				target->Set(POS_STAND);
			}
		}

		cPetData* pPet;
		target->GetDateP(pPet);
		pPet->m_JinJieNum.m_PositionID = positionID;
		pPet->m_JinJieNum.m_JinJieNum = positionID;
	}
	
	if (NeedFind)
	{
		if (abs(target->GetX() - g_pMainState->m_Map.m_pMap->m_xCenter) > 320)return;
		if (abs(target->GetY() - g_pMainState->m_Map.m_pMap->m_yCenter) > 240)return;
		g_pMainState->FindObj();
	}
}

void cMyMap::AddNpc(sNpc* npc, cTaskFather* p /*= 0*/, string scriptname /*= ""*/, int stage /*= 0*/, BOOL NeedFind)
{
	if (npc->m_direction<0 || npc->m_direction>9)npc->AutoDirection4();
	AddNpc(npc->m_name, npc->m_modeid, npc->m_x, npc->m_y, npc->m_direction,
		p, scriptname, stage, NeedFind, npc->weapontype, npc->weaponid, &npc->m_Ranse,npc->m_JinJieID.m_PositionID);
}

void cMyMap::AddNpc(sMapObjTrigger* data, BOOL needfind )
{
	AddNpc(&data->m_Npc, data->m_pTask, "无", data->m_Stage, needfind);
}

void cMyMap::RemoveNpc(int xPos, int yPos, string name, BOOL NeedFind)
{
	for (int i = 0; i< m_numberOfNpc; i++)
	{
		if (xPos != m_pNpc[i]->GetX())continue;
		if (yPos != m_pNpc[i]->GetY())continue;
		if (name.size()>1)
		{
			if (0 == name.compare(m_pNpc[i]->m_TrueName.GetString()))
			{
				RemoveNpc(i, NeedFind);
				return;
			}
		}
		else
		{
			RemoveNpc(i, NeedFind);
			return;
		}
	}
}

void cMyMap::RemoveNpc(string name, BOOL NeedFind)
{
	for (int i = m_numberOfNpc - 1; i > -1; i--)
	{
		if (0 == name.compare(m_pNpc[i]->m_TrueName.GetString()))
		{
			RemoveNpc(i,NeedFind);
			return;
		}
	}
}
void cMyMap::ShadowNpc(string name, BOOL NeedShow, BOOL NeedFind)
{
	for (int i = m_numberOfNpc - 1; i > -1; i--)
	{
		if (0 == name.compare(m_pNpc[i]->m_TrueName.GetString()))
		{
			m_pNpc[i]->SetShow(NeedShow);
		}
	}
	if (NeedFind)
		g_pMainState->FindObj();
}
void cMyMap::RemoveNpc(int id, BOOL NeedFind)
{
	m_pNpc[id]->Reset();
	swap(m_pNpc[id], m_pNpc[m_numberOfNpc - 1]);
	m_numberOfNpc -= 1;
	if (NeedFind)
	g_pMainState->FindObj();
}

void cMyMap::RemoveNpc(cObj* pObj, BOOL Needfind)
{
	for (int i = 0; i < m_numberOfNpc; i++)
	{
		if (m_pNpc[i] == pObj)
		{
			RemoveNpc(i, Needfind);
			return;
		}
	}
}

HBITMAP cMap::DIBtoDDB(WORD* data,HDC dc)
{

	
	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)m_pBmpinfo;
	HBITMAP hbm;
	HPALETTE Pal = 0;
	HPALETTE OldPal = 0;
	
	int nColors = lpbi->biClrUsed ? lpbi->biClrUsed : 1 << lpbi->biBitCount; //颜色表项的个数
	if (nColors <= 256 && GetDeviceCaps(dc, RASTERCAPS) & RC_PALETTE)
	{
		UINT nSize = sizeof(LOGPALETTE)+(sizeof(PALETTEENTRY)* nColors);
		LOGPALETTE *plp = (LOGPALETTE *) new BYTE[nSize];
		plp->palVersion = 0x300;
		plp->palNumEntries = nColors;

		for (int i = 0; i < nColors; i++)
		{
			plp->palPalEntry[i].peRed = g_pBmpinfo->bmiColors[i].rgbRed;
			plp->palPalEntry[i].peGreen = g_pBmpinfo->bmiColors[i].rgbGreen;
			plp->palPalEntry[i].peBlue = g_pBmpinfo->bmiColors[i].rgbBlue;
			plp->palPalEntry[i].peFlags = 0;
		}
		Pal = CreatePalette(plp);
		delete[] plp; //要及时处理
		// select and realize the palette
		OldPal = SelectPalette(dc, Pal, false);
		RealizePalette(dc);
	}
	hbm = CreateDIBitmap(
		dc, //设备上下文的句柄
		(LPBITMAPINFOHEADER)lpbi, //位图信息头指针
		(long)CBM_INIT, //初始化标志
		(void*)data,//lpDIBits, //初始化数据指针
		(LPBITMAPINFO)lpbi, //位图信息指针
		DIB_RGB_COLORS
		); //颜色数据的使用方式
	if (Pal)
	{
		SelectPalette(dc, OldPal, false);
	}
	
	return hbm;
}

cMap::~cMap()
{
	Free();
	free(m_pBmpinfo);
	for (int i = 0; i < 1600; i++)
	{
		SAFE_DELETE_ARRAY(m_pBitmap[i]);
	}
	SAFE_DELETE_ARRAY(m_pBitmap);
	SAFE_DELETE_ARRAY(m_pMapJpgData);
	SAFE_DELETE_ARRAY(m_pUnitNData);
	SAFE_DELETE_ARRAY(m_pTempBuffer);

	SAFE_DELETE_ARRAY(m_pBlockOffset);
	SAFE_DELETE_ARRAY(m_pMaskDataOffset);
	SAFE_DELETE_ARRAY(m_pMaskRect);
	for (int i = 0; i < m_MaskNum; i++)
	{
		SAFE_DELETE_ARRAY(m_pMaskDataDec[i]);
	}
	SAFE_DELETE_ARRAY(m_pMaskDataDec);
	SAFE_DELETE_ARRAY(m_align_width);
}

BOOL cMap::Follow(cObj* pObj)
{
	int xHero,  yHero;
	xHero = pObj->GetX();
	yHero = pObj->GetY();
	m_bMove = FALSE;
	
	if (m_LockMap)
	{
		if (abs(m_xLock - m_xCenter) < 5 && abs(m_yLock - m_yCenter) < 5)
			return TRUE;
		float dust1, dust2;
		dust1 = (float)(m_yLock - m_yCenter);
		dust2 = (float)(m_xLock - m_xCenter);
		float angel = atan2(dust1,dust2);
		
		int yMove1 =(int)( 6*sin(angel));
		int xMove1 =(int)( 6*cos(angel));
		xMove(xMove1);
		yMove(yMove1);

		m_bMove = TRUE;
	}
	else if (m_ReturnMap)
	{
		if (abs(xHero - m_xCenter) < 10 && abs(yHero - m_yCenter) < 10)
		{
			m_ReturnMap = FALSE;
			return TRUE;
		}
		float dus1, dus2;
		dus1 = (float)(yHero - m_yCenter);
		dus2 = (float)(xHero - m_xCenter);
		float angel = atan2(dus1,dus2);
		int yMove1 = (int)(6 * sin(angel));
		int xMove1 = (int)(6 * cos(angel));
		xMove(xMove1);
		yMove(yMove1);
		m_bMove = TRUE;
	}
	else
	{
		if (GetXCenter() != pObj->GetX() || GetYCenter() != pObj->GetY())
		{
			m_bMove = TRUE;
			m_xCenter = pObj->m_xPos;
			m_yCenter = pObj->m_yPos;
			g_pMainState->m_Canvas.SetRedrawAll(FALSE);
		}
	}
	if (!m_bMove)
	{
		return TRUE;
	}
	if (m_xCenter<320)m_xCenter = 320;
	if (m_yCenter<240)m_yCenter = 240;
	if (m_xCenter>(float)m_Width - 321)m_xCenter = (float)m_Width - 321;
	if (m_yCenter>(float)m_Height - 241)m_yCenter = (float)m_Height - 241;
	
	
	m_xCenterOffset = GetXCenter() % 320;
	m_yCenterOffset = GetYCenter() % 240;

	//如果X Y方向上都发生了改变
	if (m_xCenterNum != (GetXCenter()/320) || m_yCenterNum != (GetYCenter() / 240))
	{
		while (!m_bLoadReady2)
		{
			Sleep(0);
		}
		MoveMap();
	}
	return TRUE;
}

BOOL cMap::SetCenter(float xHero, float yHero)
{
	m_xCenter = xHero;
	m_yCenter = yHero;
	if (xHero < 320.0f)
		m_xCenter = 320.0f;
	else
	if (xHero>float(m_Width - 321))
		m_xCenter = (float)(m_Width - 321);
	if (yHero < 240.0f)
		m_yCenter = 240.0f;
	else
	if (yHero>(float)(m_Height - 241))
		m_yCenter =(float) (m_Height - 241);
	return TRUE;
}

void cMap::CountAlpha(WPixel& color1, WPixel& color2, BYTE* pPic,int index2,int time)
{
	WPixel color3;
	color3.red = color1.red + (color2.red - color1.red) / time;
	color3.green = color1.green + (color2.green - color1.green) / time;
	color3.blue = color1.blue + (color2.blue - color1.blue) / time;
	pPic[index2] = color3.l;
	pPic[index2 + 1] = color3.h;
}


void cMap::xMove(int x)
{
	if (0 == x)return;
	if (x > 0)
	{
		if (x * m_AddSpeed > m_xPreSpeed)
		{
			m_xPreSpeed += 1;
			m_xCenter += m_xPreSpeed / m_AddSpeed;
		}
		else  m_xCenter += x;
	}
	else
	{
		if (x * m_AddSpeed < m_xPreSpeed)
		{
			m_xPreSpeed -= 1;
			m_xCenter += m_xPreSpeed / m_AddSpeed;
		}
		else  m_xCenter += x;
	}
}
void cMap::yMove(int y)
{
	if (y > 0)
	{
		if (y * m_AddSpeed > m_yPreSpeed)
		{
			m_yPreSpeed += 1;
			m_yCenter += m_yPreSpeed / m_AddSpeed;
		}
		else  m_yCenter += y;
	}
	else
	{
		if (y * m_AddSpeed < m_yPreSpeed)
		{
			m_yPreSpeed -= 1;
			m_yCenter += m_yPreSpeed / m_AddSpeed;
		}
		else  m_yCenter += y;
	}
}

void cMap::CountMaskList()
{
	m_MaskIDList.clear();
	RECT rect,rect2;
	rect.left = GetXCenter()-480;
	rect.right = GetXCenter() + 480;
	rect.top=GetYCenter()-360;
	rect.bottom = GetYCenter() + 360;
	for (int i = 0; i < m_MaskNum; i++)
	{
		rect2.left = m_pMaskRect[i].x;
		rect2.right = m_pMaskRect[i].x + m_pMaskRect[i].w;
		rect2.top = m_pMaskRect[i].y;
		rect2.bottom = rect2.top+ m_pMaskRect[i].h;
		if (CheckCross(rect2, rect))
			m_MaskIDList.push_back(i);
	}
}
void cMyMap::AutoGetPos(int& xout, int& yout, string mapname)
{
	//计算NPC的位置
	if (m_BmpTest.oldmap != mapname)
	{
		ostringstream oss;
		oss.str("");
		oss << g_MapPath << mapname << ".txt";
		string txtpath = oss.str();
		ifstream   file;
		file.open(txtpath.c_str());
		if (!file)ERRBOX;
		int MapNumber;
		file >> MapNumber;    //读取地图编号
		file.close();
		oss.str("");
		oss << MapNumber;
		string  path1 = oss.str();
		oss.str("");
		m_BmpTest.Load((CHAR*)path1.c_str());
	}
	int width = m_BmpTest.GetWidth() / 20;
	int height = m_BmpTest.GetHeight() / 20;
	xout = rand() % width;
	yout = rand() % height;
	while (m_BmpTest.isOK(xout * 20, yout * 20))
	{
		xout = rand() % width;
		yout = rand() % height;
	}
	yout = m_BmpTest.GetHeight() / 20 - yout;
	if (xout < 15)xout = 15;
	if (yout >(m_BmpTest.GetHeight() / 20)- 20)
	{
		yout = (m_BmpTest.GetHeight() / 20) - 20;
	}
}

void cMyMap::AddNpcSmall(string name, int id, int x, int y, int direction, cTaskFather* p /*= 0*/, string scriptname /*= ""*/, int stage /*= 0*/, BOOL NeedFind)
{
	x = x * 20;
	y = m_pMap->m_Height - y * 20;
	AddNpc(name, id, x, y, direction, p, scriptname, stage,NeedFind);
}

void cMyMap::AddJiaJu(sJiaJu* jiaju)
{
	int direction = jiaju->m_Direction;
	sJiaJuData& data = g_pMainState->m_ItemManage.m_JiaJuData[jiaju->m_JiaJu.GetType() - 35][jiaju->m_JiaJu.GetID()];

	int pos;
	switch (jiaju->m_JiaJu.m_pItemData->GetLv())
	{
	case 1:
	case 2:
		pos = 0;
		break;
	default:
		pos = 1;
		break;
	}
	int x = jiaju->m_xPos;
	int y = jiaju->m_yPos;

	switch (jiaju->m_JiaJu.m_pItemData->GetLv())
	{
	case 4:
		if (1)
		{
			for (int i = 0; i < 3; i++)
			{
				DWORD id = data.m_data[direction * 3 + i];
				m_JianZhuID[m_numofJianZhu] = id;
				m_pJianZhu[m_numofJianZhu]->SetXY(x, y);
				m_pJianZhu[m_numofJianZhu]->m_PosType = pos;
				m_numofJianZhu += 1;
			}
		}
		break;
	case 5:
		if (jiaju->m_Direction)
		{
			for (int i = 0; i < 5; i++)
			{
				DWORD id = data.m_data[6+ i];
				m_JianZhuID[m_numofJianZhu] = id;
				m_pJianZhu[m_numofJianZhu]->SetXY(x, y);
				m_pJianZhu[m_numofJianZhu]->m_PosType = pos;
				m_numofJianZhu += 1;
			}
		}
		else
		{
			for (int i = 0; i < 6; i++)
			{
				DWORD id = data.m_data[i];
				m_JianZhuID[m_numofJianZhu] = id;
				m_pJianZhu[m_numofJianZhu]->SetXY(x, y);
				m_pJianZhu[m_numofJianZhu]->m_PosType = pos;
				m_numofJianZhu += 1;
			}
		}
		break;
	default:
		if (1)
		{
			DWORD id = data.m_data[direction];
			m_JianZhuID[m_numofJianZhu] = id;
			m_pJianZhu[m_numofJianZhu]->SetXY(x, y);
			m_pJianZhu[m_numofJianZhu]->m_PosType = pos;
			m_numofJianZhu += 1;
		}
		break;
	}
}

void cMyMap::RemoveJiaJu(sJiaJu* jiaju)
{
	//sJiaJuData& data = g_pMainState->m_ItemManage.m_JiaJuData[jiaju->m_JiaJu.GetType() - 35][jiaju->m_JiaJu.GetID()];
	

	for (int i = 0; i < m_pJianZhu.size(); i++)
	{
		int x = m_pJianZhu[i]->GetX();
		int y = m_pJianZhu[i]->GetY();
		if (x==jiaju->m_xPos)
		if (y == jiaju->m_yPos )
		{
			swap(m_pJianZhu[i], m_pJianZhu[m_numofJianZhu-1]);
			swap(m_JianZhuID[i], m_JianZhuID[m_numofJianZhu - 1]);
			m_numofJianZhu -= 1;
		}
	}
}

void cMyMap::CheckChuanSong()
{
	int xC = g_pHeroObj->GetX();
	int yC = g_pHeroObj->GetY();
	//检测角色是否到传送点
	for (int i = 0; i < m_numberOfTransmit; i++)
	{
		if (m_pTransmit[i].Check(xC,yC))
		{
			LoadMap(m_pTransmit[i].m_xTarget, m_pTransmit[i].m_yTarget, (char*)m_pTransmit[i].name.c_str(),FALSE);
			return;
		}
	}
	for (int i = 0; i < m_RangeTriggerList.size(); i++)
	{
		if (m_RangeTriggerList[i].Check(xC, yC))
		{
			cTaskFather* p = m_RangeTriggerList[i].m_p;
			int ret = m_RangeTriggerList[i].m_ret;
			m_RangeTriggerList.erase(m_RangeTriggerList.begin() + i);
			p->Process(ret);
		}
	}
}

void cMyMap::RemoveAllNpc()
{
	for (int i = 0; i < m_numberOfNpc; i++)
	{
		m_pNpc[i]->Free();
	}
	m_numberOfNpc = 0;
}

void cMyMap::SetLock(string mapname /*= ""*/)
{
	if (mapname == "")
	{
		m_LockList.clear();
	}
	else
	{
		m_LockList.push_back(mapname);
	}
}

void cMyMap::SetLock(BOOL b)
{
	g_pMainState->m_GlobalButton[10] = b; 
}

void cMyMap::AddTriggerRange(sTriggerRange& data)
{
	m_RangeTriggerList.push_back(data);
}



BOOL cBmp::Load(CHAR* Name)
{
	if (oldmap == Name)
		return TRUE;
	ostringstream oss;
	oss << g_MapPath << Name << ".map";
	string txtmap = oss.str();
	oss.str("");
	oss << g_MapPath << Name << ".bmp";
	string textbmp = oss.str();
	//得到地图的高度 MASK的高度要和地图的高度一致,否则计算不正确
	ifstream File2;
	File2.open(txtmap, ios::binary | ios::in);
	if (!File2)
		ERRBOX;
	cMap::SMap_Header	FileHeader2;
	File2.read((CHAR*)&FileHeader2, sizeof(cMap::SMap_Header));//读取地图头
	m_Height = FileHeader2.map_height;
	File2.close();
	//
	ifstream File;
	File.open(textbmp, ios::binary);
	if (!File)
		ERRBOX;
	BITMAPFILEHEADER FileHeader;		// bitmap file-header
	BITMAPINFO info;
	File.read((char*)&FileHeader, sizeof(BITMAPFILEHEADER));
	File.read((char*)&info, sizeof(BITMAPINFO));
	m_Width = info.bmiHeader.biWidth;
	File.seekg(FileHeader.bfOffBits,ios::beg);  //设置输入流的指针位置
	
	int wpitch = (m_Width + 7)>>3;
	int gap = (info.bmiHeader.biHeight - m_Height) *wpitch;
	int size = (wpitch*m_Height);
	if (m_capacity < size)
	{
		Free();
		m_capacity = size;
		m_pBmp = new BYTE[size];
	}
	File.read((char*)m_pBmp, gap);
	File.read((char*)m_pBmp, size);
	File.close();
	return TRUE;
}

BOOL cBmp::isOK(int x, int y)
{
	int index = (m_Height - 1 - y)*m_Width + x;
	if (m_pBmp[index >> 3])return TRUE; return FALSE;
}

cBmp::~cBmp()
{
	Free();
}

BOOL cBmp::Free()
{
	SAFE_DELETE_ARRAY(m_pBmp);
	return TRUE;
}


void sTriggerRange::Save(ofstream& File)
{
	if (!m_p)
	{
		File << 0;
		return;
	}
	else File << 1 << " ";
	File << m_range << " ";
	File << m_x << " ";
	File << m_y << " ";
	File << m_p->GetType() << " ";
	File << m_ret << " ";
	File << m_name << " ";
}

void sTriggerRange::Load(ifstream& File)
{
	BOOL live;
	File >> live;
	if (!live)return;

	File >> m_range ;
	File >> m_x ;
	File >> m_y ;
	int type;
	File >>type;
	m_p= g_pMainState->FindTaskByID(type);
	File >> m_ret ;
	File >> m_name;
}

void sTriggerRange::Set(string name, int x, int y, int range, cTaskFather* p, int ret)
{
	m_name = name;
	m_range=range*20;
	m_x =x*20;
	m_y =g_pMainState->m_Map.m_pMap->m_Height - m_y * 20;
	m_p = p;
	m_ret=ret;
}

BOOL sRangeTr::Check(int x, int y)
{
	if (abs(x - m_x) < m_range&&abs(y - m_y) < m_range)
	{
		if (m_name == "")return TRUE;
		if (m_name==g_pMainState->m_Map.m_MapName)
			return TRUE;
		return FALSE;
	}
	return FALSE;
}
