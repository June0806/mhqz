#include "MyMath.h"
#include "global.h"
//#include <windows.h>
//#include <string>
#include <math.h>
int GetDirection4(int x1, int y1, int xTarget, int yTarget)
{
	if (x1 > xTarget)
	{
		if (y1 > yTarget)
		{
			return 7;
		}
		else
			return 1;
	}
	else
	{
		if (y1 > yTarget)
		{
			return 9;
		}
		else
			return 3;
	}
}


int GetReverseDirection(int Direction)
{
	switch (Direction)
	{
	case 1:
		return 9;
	case 2:
		return 8;
	case 3:
		return 7;
	case 4:
		return 6;
	case 6:
		return 4;
	case 7:
		return 3;
	case 8:
		return 2;
	case 9:
		return 1;
	}
	return 0;
}

BOOL isOn(int x, int y, RECT& rect)
{
	if (x > rect.left &&
		x<rect.right&&
		y > rect.top&&
		y<rect.bottom)return TRUE;
	return FALSE;
}

BOOL isOn(int x, int y, int w, int h, int left, int top, int right, int bottom)
{
	if (x > right)return FALSE;
	if (y > bottom)return FALSE;
	if (x + w < left)return FALSE;
	if (y + h < top)return FALSE;
	return TRUE;
}

INT64 GetCharacterExp(int Lv)
{
	INT64 Exp = GetNormalFightExp(Lv, Lv, 5);
	int k = Lv - 1;
	if (k < 0)k = 0;
	Exp = 200 + (Exp*((Lv*Lv*Lv) -(k*k*k)) / 100);
	if (Lv>145)
	{
		Exp = (Exp*(Lv - 140)) / 2;
	/*	if (Lv > 170)
			Exp *= 3;
		else
		if (Lv > 160)
			Exp *= 2;*/
	}
	return Exp;
}

INT64 GetNormalFightExp(int OurLv, int EnemyLv, int NumOfPeople)
{
	INT64 EarnExp = (25 + EnemyLv*8)*(NumOfPeople + 5);
	int Offset = OurLv - EnemyLv;// abs(OurLv - EnemyLv);
	if (Offset > 15)
		EarnExp = 1;
	else
	if (Offset > 10)
	{
		EarnExp=EarnExp >> 1;
	}
	else
	if (Offset > 5)
	{
		EarnExp = EarnExp * 80 / 100;
	}
	else
	if (Offset < -5)//�ҷ��ȼ����ڹ���ȼ�5��
	{
		EarnExp = EarnExp * 120 / 100;

	}
	else
	if (Offset < -10)//�ҷ��ȼ����ڹ���ȼ�10��
	{
		EarnExp = EarnExp * 150 / 100;
	}
	if (EarnExp < 1)EarnExp = 1;
	return EarnExp;
}

DWORD GetPriceColor(int Price)
{
	if (Price < 10000)
		return  RGB(0,0,0);
	if (Price < 100000)
		return  RGB(0,0,255);
	if (Price<1000000)
		return  RGB(0, 140, 0);
	if (Price < 10000000)
		return  RGB(255, 0, 0);
    return  RGB(238,0,238);
}

int GetXiuLIanExp(int lv)
{
	return  lv*lv * 10 + 140;
}

BOOL CheckCross(RECT& rect1, RECT& rect2, RECT& outrect)
{
	if (rect1.right < rect2.left)return FALSE;
	if (rect2.right < rect1.left)return FALSE;
	if (rect1.bottom < rect2.top)return FALSE;
	if (rect2.bottom < rect1.top)return FALSE;
	outrect.left = rect1.left>rect2.left ? rect1.left : rect2.left;
	outrect.right = rect1.right > rect2.right ? rect2.right : rect1.right;
	outrect.top = rect1.top > rect2.top ? rect1.top : rect2.top;
	outrect.bottom = rect1.bottom > rect2.bottom ? rect2.bottom : rect1.bottom;
	return TRUE;
}
BOOL CheckCross(RECT& rect1, RECT& rect2)
{
	if (rect1.right < rect2.left)return FALSE;
	if (rect2.right < rect1.left)return FALSE;
	if (rect1.bottom < rect2.top)return FALSE;
	if (rect2.bottom < rect1.top)return FALSE;
	return TRUE;
}




INT64 GetMainSkillExp(int lv)
{
	INT64 exp = 3*lv*lv*lv + lv*lv + 14 * lv+10;
	return exp;
}

INT64 GetMainSkillMoney(int lv)
{
	INT64 money = lv*lv*lv + lv*lv + 12 * lv+10;
	return money;
}

BOOL StringToWString(const std::string &str, std::wstring &wstr)
{
	int nLen = (int)str.length();
	wstr.resize(nLen, L' ');

	int nResult = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), nLen, (LPWSTR)wstr.c_str(), nLen);

	if (nResult == 0)
	{
		return FALSE;
	}

	return TRUE;
}

int GetRotateDirection(int nowDirection, BOOL directNum4 /*= true*/, BOOL turn /*= 0*/)
{
	if (turn)
	{
		switch (nowDirection)
		{
		case 1:if (directNum4)return 7; else return 4;
		case 2:return 1;
		case 3:if (directNum4)return 1; else return 2;
		case 4:return 7;
		case 6:return 3;
		case 7:if (directNum4)return 9; else return 8;
		case 8:return 9;
		default:if (directNum4)return 3; else return 3;
		}
	}
	else
	{
		switch (nowDirection)
		{
		case 1:if (directNum4)return 3; else return 2;
		case 2:return 3;
		case 3:if (directNum4)return 9; else return 6;
		case 4:return 1;
		case 6:return 9;
		case 7:if (directNum4)return 1; else return 4;
		case 8:return 7;
		default:if (directNum4)return 7; else return 8;
		}
	}
}

int Direction8To4(int direction)
{
	if (direction > 9)direction = 9;
	switch (direction)
	{
	case 2:return 1;
	case 4:return 3;
	case 5:return 9;
	case 6:return 3;
	case 8:return 7;
	}
	return 1;
}

void GetWeaponType(int modeid, int& out1, int & out2)
{
	switch (modeid)
	{
	case eJuSe������:
		out1 = eWeaponType��;
		out2 = eWeaponType��;
		break;
	case eJuSe��ң��:
		out1 = eWeaponType��;
		out2 = eWeaponType��;
		break;
	case eJuSe����Ů:
		out1 = eWeaponType��;
		out2 = eWeaponType˫��;
		break;
	case eJuSeӢŮ��:
		out1 = eWeaponType˫��;
		out2 = eWeaponType��;
		break;
	case eJuSe��ͷ��:
		out1 = eWeaponType��;
		out2 = eWeaponType��;
		break;
	case eJuSe��ħ��:
		out1 = eWeaponType��;
		out2 = eWeaponType��;
		break;
	case eJuSe�Ǿ���:
		out1 = eWeaponTypeצ;
		out2 = eWeaponType��;
		break;
	case eJuSe������:
		out1 = eWeaponType��;
		out2 = eWeaponTypeצ;
		break;
	case eJuSe��̫��:
		out1 = eWeaponType��;
		out2 = eWeaponTypeǹ;
		break;
	case eJuSe�����:
		out1 = eWeaponTypeǹ;
		out2 = eWeaponType��;
		break;
	case eJuSe���ʶ�:
		out1 = eWeaponType��;
		out2 = eWeaponTypeƮ��;
		break;
	case eJuSe���켧:
		out1 = eWeaponTypeƮ��;
		out2 = eWeaponType��;
		break;
	case eJuSe������:
		out1 = eWeaponType��;
		out2 = eWeaponType��;
		break;
	case eJuSe������:
		out1 = eWeaponType��;
		out2 = eWeaponType��;
		break;
	case eJuSeɱ����:
		out1 = eWeaponType��;
		out2 = eWeaponType��;
		break;
	default://�ɵ�ӢŮ��
		out1 = eWeaponType˫��;
		out2 = eWeaponType��;
		break;
	}
}

void CoutPoint(float distance, float xstart, float ystart, float xend, float yend, float &xout, float& yout)
{//���� �� xstart ystart  �� xstart ystart  ����Ϊ ditance �ĵ������
	float angel = atan2(yend - ystart, xend - xstart);
	yout =ystart+ distance*sin(angel);
	xout =xstart+ distance*cos(angel);
}

float CoutDistance(float xstart, float ystart, float xend, float yend)
{
	float x = (xstart - xend);
	float y = (ystart - yend);
	x *= x;
	y *= y;
	return sqrt(x + y);
}

int GetDirection8Ex(int predirection, int x1, int y1, int xTarget, int yTarget)
{//ͨ��ԭ����յ�õ������ķ���
	float  yOffset = (float)(yTarget-y1);
	float  xOffset = (float)(xTarget-x1);
	float angel = atan2(yOffset, xOffset);

	if (angel>0)
	{
		if (angel < 0.2618)
			return 6;
		else
		if (angel < 0.5236)
		{
			if (predirection == 6 || predirection == 3)
				return predirection;
			else
			{
				if (angel < 0.3927)
					return 6;
				else return 3;
			}
		}
		else if (angel < 1.0472)
			return 3;
		else if (angel < 1.3089)
		{
			if (predirection == 2 || predirection == 3)
				return predirection;
			else
			{
				if (angel < 1.178)
					return 6;
				else return 3;
			}
		}
		else if (angel < 1.8326)
			return 2;
		else if (angel < 2.0944)
		{
			if (predirection == 2 || predirection == 1)
				return predirection;
			else
			{
				if (angel < 1.9635)
					return 2;
				else return 1;
			}
		}
		else if (angel < 2.618)
			return 1;
		else  if (angel < 2.8798)
		{
			if (predirection == 4 || predirection == 1)
				return predirection;
			else
			{
				if (angel < 2.7489)
					return 1;
				else return 4;
			}
		}
		else return 4;
	}
	else
	{//��
		if (angel > -0.2618)
			return 6;
		else
		if (angel > -0.5236)
		{
			if (predirection == 6 || predirection == 9)
				return predirection;
			else
			{
				if (angel > -0.3927)
					return 6;
				else return 9;
			}
		}
		else if (angel > -1.0472)
			return 9;
		else if (angel > -1.3089)
		{
			if (predirection == 9 || predirection == 8)
				return predirection;
			else
			{
				if (angel > -1.178)
					return 9;
				else return 8;
			}
		}
		else if (angel > -1.8326)
			return 8;
		else if (angel > -2.0944)
		{
			if (predirection == 8 || predirection == 7)
				return predirection;
			else
			{
				if (angel > -1.9635)
					return 8;
				else return 7;
			}
		}
		else if (angel > -2.618)
			return 7;
		else  if (angel > -2.8798)
		{
			if (predirection == 7 || predirection == 4)
				return predirection;
			else
			{
				if (angel > -2.7489)
					return 7;
				else return 4;
			}
		}
		else return 4;
	}
}

int AddUpZero(int& target, const int num)
{
	target += num;
	if (target < 0)
	{
		int ret = target;
		target = 0;
		return ret;
	}
	else return 0;
}

int ReadInt(HANDLE& f)
{
	//DWORD exdata;
	char data;
	while (1)
	{
		BOOL bFind = FALSE;
		while (1)
		{
			switch (ReadCharNum(f, data))
			{
			case 0:
				break;
			case 1:
				bFind = TRUE;
				break;
			case 2:
				return -1;
			}
			if (bFind)break;
		}
		int numlist[20];
		int num = 0;
		numlist[0] = GetNumByChar(data);
		num += 1;
				while (1==ReadCharNum(f, data))
				{//�ҵ�ʣ�µ�����
					numlist[num] = GetNumByChar(data);
					num += 1;
					if (num == 11)
						return -1;
				}
			
				int outnum=0;
				for (int i = 0; i < num; i++)
				{					
					switch (num-i-1)
					{
					case 0:
						outnum += numlist[i];
						break;
					case 1:outnum  += numlist[i] * 10; break;
					case 2:outnum  += numlist[i] * 100; break;
					case 3:outnum  += numlist[i] * 1000; break;
					case 4:outnum  += numlist[i] * 10000; break;
					case 5:outnum  += numlist[i] * 100000; break;
					case 6:outnum += numlist[i] * 1000000; break;
					case 7:outnum += numlist[i] * 10000000; break;
					case 8:outnum += numlist[i] * 100000000; break;
					case 9:outnum += numlist[i] * 1000000000; break;
					}

				}
				return outnum;
	}
}
#include <io.h>
int ReadCharNum(HANDLE& f, char& out)
{
	DWORD exdata;
	//BOOL bSpace = FALSE;

	ReadFile(f, &out, 1, &exdata, 0);
	//if (0 == exdata)
	//	int k = 0;
	if (out >= '0'&&out <= '9')
	{
		return 1;
	}
	return 0;
}

int GetNumByChar(char data)
{
	data -= '0';
	return (int)data;
}

void SetINT(int num, int id, BOOL bCheck)
{
	int k = 0x01;
	k = k << id;
	if (bCheck)
	{
		g_pMainState->m_GlobalButton[num] |= k;
	}
	else
	{
		if (CheckINT(num,id))
			g_pMainState->m_GlobalButton[num] -=k;
	}
}

BOOL CheckINT(int num, int id)
{
	g_pMainState->m_GlobalButton[id];
	if ((g_pMainState->m_GlobalButton[num] >> id) & 0x01)return TRUE;
	return FALSE;
}

int GetOffsetID(int FrameMax, int NowFrame, int DirectionNum, int NowDirection)
{
	if (FrameMax == 1 && DirectionNum == 1)
	{
		return 0;
	}
	else
	{
		if (DirectionNum == 1)
			return NowFrame;
		else
		{
			int a = 0;
			switch (NowDirection)
			{
			case 8:a = 6; break;
			case 2:a = 4; break;
			case 4:a = 5; break;
			case 6:a = 7; break;
			case 9:a = 3; break;
			case 3:return NowFrame;
			case 1:a = 1; break;
			case 7:a = 2; break;
			}
			a = FrameMax*a + NowFrame;
			return a;
		}
	}
}


