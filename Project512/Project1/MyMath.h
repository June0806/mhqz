#pragma  once

#include <windows.h>
#include <string>


using namespace std;

enum eWeaponType
{
	eWeaponType��,
	eWeaponType��,
	eWeaponTypeǹ,
	eWeaponType��,
	eWeaponType��,
	eWeaponType��,
	eWeaponTypeצ,
	eWeaponType��,
	eWeaponTypeƮ��,
	eWeaponType��,
	eWeaponType��,
	eWeaponType˫��,
	eWeaponType��,
	eWeaponType��,
	eWeaponType��
};
enum eJueSe
{
	eJuSe������,
	eJuSe��ң��,
	eJuSe����Ů,
	eJuSeӢŮ��,
	eJuSe��ͷ��,
	eJuSe��ħ��,
	eJuSe�Ǿ���,
	eJuSe������,
	eJuSe��̫��,
	eJuSe�����,
	eJuSe���ʶ�,
	eJuSe���켧,
	eJuSe������,
	eJuSe������,
	eJuSeɱ����
};
void GetWeaponType(int modeid, int& out1, int & out2);
int GetDirection4(int x1, int y1, int xTarget, int yTarget);//ͨ��ԭ����յ�õ������ķ���
//int GetDirection8(int x1, int y1, int xTarget, int yTarget);//ͨ��ԭ����յ�õ������ķ���
int GetDirection8Ex(int predirection, int x1, int y1, int xTarget, int yTarget);
int GetReverseDirection(int Direction);                     //�õ��෴�ķ���
int GetRotateDirection(int nowDirection, BOOL directNum4=TRUE, BOOL turn = 0); //���ڽ�ɫ��ת,�õ���һ����
BOOL isOn(int x, int y, RECT& rect);
inline BOOL isOn(int x, int y,int w,int h, int left,int top,int right,int bottom);
DWORD GetPriceColor(int Price);

INT64 GetCharacterExp(int Lv); //ͨ���ȼ�������������
INT64 GetNormalFightExp(int OurLv, int EnemyLv,int NumOfPeople=1);//������˫����
INT64 GetMainSkillExp(int lv); //����ʦ�ż���ѧϰ���� 
INT64 GetMainSkillMoney(int lv);//����ʦ�ż���ѧϰ��Ǯ
int   GetXiuLIanExp(int lv);
BOOL  CheckCross(RECT& rect1, RECT& rect2, RECT& outrect);  //��������ȡ����,���û�н���,����0
BOOL  CheckCross(RECT& rect1, RECT& rect2);
int   GetOffsetID(int FrameMax, int NowFrame, int DirectionNum, int NowDirection);

BOOL StringToWString(const std::string &str, std::wstring &wstr);
int Direction8To4(int direction); 
void CoutPoint(float distance, float xstart, float ystart, float xend, float yend, float  &xout, float& yout);
float CoutDistance(float xstart, float ystart, float xend, float yend);
int AddUpZero(int& target, const int num);//target����num ������С��0  ����target+num��ֵ   target����0 ���򷵻�0
int ReadInt(HANDLE& f);
int ReadCharNum(HANDLE& f,char& out);//��ȡ1�ֽ�,���������,����1,���򷵻�0 ������ļ���β,����2
int GetNumByChar(char data);
void SetINT(int num, int id, BOOL bCheck=TRUE);
BOOL CheckINT(int num, int id);
