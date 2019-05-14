#include "global.h"
void cChangeColor::Init()
{
}

void cChangeColor::GetColorMatrix(int part, int matrixid, sChangeColorMatrix& out)
{
	if (part < m_CharacterColor.m_stagesplit.size())
	{
		if (matrixid < m_CharacterColor.m_stagesplit[part].m_ColorMatrix.size())
			out = m_CharacterColor.m_stagesplit[part].m_ColorMatrix[matrixid];
	}
}

void cChangeColor::Set(DWORD id)
{
	if (m_oldid == id)return;
	m_oldid = id;
	if (id == 0)return;
	CWFile finddata;
	ifstream* f=0;
	int plitnum;
	int split[10];
	int matrixnum;
	int matrixid[9];
	finddata.GetFileP(id, f);
	*f >> plitnum;
	for (int i1 = 0; i1 < plitnum + 1; i1++)
	{
		*f >> split[i1];
	}
	m_CharacterColor.m_stagesplit.clear();
	for (int i3 = 0; i3 < plitnum; i3++)
	{
		sChangeColorSplit  part;
		 *f >> matrixnum;
		for (int i1 = 0; i1 < matrixnum; i1++)//¶ÁÈ¡¾ØÕó
		{
		/*	if (i1 == 7)
				int k = 0;*/
			sChangeColorMatrix matrix ;
			for (int i2 = 0; i2 < 9; i2++)
			{
				*f >> matrixid[i2];
			}
			matrix.m_start = split[i3];
			matrix.m_end = split[i3 + 1];
			matrix.x1 = matrixid[0];
			matrix.x2 = matrixid[1];
			matrix.x3 = matrixid[2];

			matrix.y1 = matrixid[3];
			matrix.y2 = matrixid[4];
			matrix.y3 = matrixid[5];

			matrix.z1 = matrixid[6];
			matrix.z2 = matrixid[7];
			matrix.z3 = matrixid[8];
			part.m_ColorMatrix.push_back(matrix);
		}
		m_CharacterColor.m_stagesplit.push_back(part);
	}
}

int cChangeColor::GetPartNum()
{
	return m_CharacterColor.m_stagesplit.size();
}

int cChangeColor::GetMatirxNum(int id)
{
	return m_CharacterColor.m_stagesplit[id].m_ColorMatrix.size();
}

void cChangeColor::Give(sChangeColorData* pData)
{
	pData->partnum = GetPartNum();
	for (int i = 0; i < pData->partnum; i++)
	{
		GetColorMatrix(i, pData->GetColorChange(i) , pData->m_colormatrix[i]);
	}
}

void cChangeColor::SetCharacter(DWORD id)
{
	Set(g_pMainState->m_pCharacterDataList[id].m_ColorID);
}

void cChangeColor::SetPet(DWORD id,int part)
{
	Set(g_pMainState->m_PetDataManager.GetPetData(id)->m_PartList[part].m_ColorID);
}

void cChangeColor::Give2(sChangeColorData* pData, int modeid,int part)
{
	if (modeid < 1000)
	{
		SetPet(modeid, part);
	}
	else
	if (modeid>1999)
	{
		SetCharacter(modeid - 2000);
	}
	else
	return;
	Give(pData);
}


