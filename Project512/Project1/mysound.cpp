#include "global.h"
#include <d3d9.h>                   // D3D头文件
#include <d3dx9.h>                  // D3DX库头文件
#include <DSound.h>
#include <dshow.h>

#include <string.h>
#include <mmreg.h>
#include <msacm.h>
#include <string.h>
#include <malloc.h>



void cMySound::Load(string path)
{
	if (0 == m_PrePath.compare(path))return;
	m_PrePath = path;
	ostringstream oss;
	oss << g_strMediaPath << "music\\"<<path<<".mp3";
	path = oss.str();
	if (m_pMediaControl)
	{
		m_pMediaControl->Stop();
		m_media_event->Release();
		m_media_event = NULL;
		m_pMediaControl->Release();
		m_pMediaControl = NULL;
		m_pMediaPosition->Release();
		m_pMediaPosition = 0;
		m_pGraphBuilder->Release();
		m_pGraphBuilder = NULL;
	}
	// 创建IGraphBuilder接口对象
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder, (void**)&m_pGraphBuilder);
	// 创建IMediaControl和IMediaPosition对象
	m_pGraphBuilder->QueryInterface(IID_IMediaControl, (void**)&m_pMediaControl);
	m_pGraphBuilder->QueryInterface(IID_IMediaPosition, (void**)&m_pMediaPosition);
	m_pGraphBuilder->QueryInterface(IID_IMediaEvent, (void**)&m_media_event);

	wstring wstr;
	StringToWString(path, wstr);
	// 加载MP3数据
	if(m_pGraphBuilder->RenderFile(wstr.c_str(), NULL))
		return;
	if (g_pMainState->m_GlobalButton[16])return;
		m_pMediaControl->Run();

}

void cMySound::Free()
{
	if (!m_pMediaControl)return;
	m_pMediaControl->Stop();
	m_media_event->Release();
	m_media_event = NULL;
	m_pMediaControl->Release();
	m_pMediaControl = NULL;
	m_pMediaPosition->Release();
	m_pMediaPosition = 0;
	m_pGraphBuilder->Release();
	m_pGraphBuilder = NULL;
}

void cMySound::CheckEnd()
{
	if (!m_media_event)return;
	long msg;
	m_media_event->WaitForCompletion(0, &msg);
	if (msg)
	{
		m_pMediaPosition->put_CurrentPosition(0);
	}
}

void cMySound::Reset()
{
	// 创建IGraphBuilder接口对象
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder, (void**)&m_pGraphBuilder);
	// 创建IMediaControl和IMediaPosition对象
	m_pGraphBuilder->QueryInterface(IID_IMediaControl, (void**)&m_pMediaControl);
	m_pGraphBuilder->QueryInterface(IID_IMediaPosition, (void**)&m_pMediaPosition);
	m_pGraphBuilder->QueryInterface(IID_IMediaEvent, (void**)&m_media_event);
}

void cMySound::Stop()
{
	m_pMediaControl->Stop();
}


void cMyWav::Load(BYTE* pdata, int size, int pos, BOOL bPlay)
{
	if (g_pMainState->m_GlobalButton[17])return;
	Free();
	struct {
		DWORD id;
		DWORD size;
	}chunk;

	memcpy_s(&chunk, sizeof(chunk), pdata, sizeof(chunk));
	pdata += sizeof(chunk);
	if (chunk.id != WFourCC("RIFF"))
	{
		g_pMainState->m_GlobalButton[17] = TRUE;
		ERRBOX;
		return;
	}
	memcpy_s(&chunk.id, sizeof(chunk.id), pdata, sizeof(chunk.id));
	pdata += sizeof(chunk.id);

	memcpy_s(&chunk, sizeof(chunk), pdata, sizeof(chunk));
	pdata += sizeof(chunk);

	int SrcHeaderSize = chunk.size;
	if (SrcHeaderSize < sizeof(WAVEFORMATEX))SrcHeaderSize = sizeof(WAVEFORMATEX);
	WAVEFORMATEX *SrcHeader = (WAVEFORMATEX *)_alloca(SrcHeaderSize);
	ZeroMemory(SrcHeader, SrcHeaderSize);
	memcpy_s(SrcHeader, chunk.size, pdata, chunk.size);
	pdata += chunk.size;

	for (;;) {
		memcpy_s(&chunk, sizeof(chunk), pdata, sizeof(chunk));
		pdata += sizeof(chunk);
		if (chunk.id == WFourCC("data")) break;
		pdata += chunk.size;
	}
	if (SrcHeader->wFormatTag != WAVE_FORMAT_PCM) 
	{
		// 准备变换后的handle
		WAVEFORMATEX *DstHeader = (WAVEFORMATEX *)_alloca(SrcHeaderSize);
		memcpy(DstHeader, SrcHeader, SrcHeaderSize);

		DstHeader->wFormatTag = WAVE_FORMAT_PCM;
		
		MMRESULT Result;
		Result = acmFormatSuggest(NULL, SrcHeader, DstHeader, SrcHeaderSize, ACM_FORMATSUGGESTF_WFORMATTAG);
		if (Result != 0)
		{
			g_pMainState->m_GlobalButton[17] = TRUE;
			ERRBOX;
			return;
		}

		// 准备ACM STREAM
		HACMSTREAM acm;
		Result = acmStreamOpen(&acm, NULL, SrcHeader, DstHeader, NULL, NULL, 0, 0);
		if (Result != 0)
		{
			g_pMainState->m_GlobalButton[17] = TRUE;
			ERRBOX;
			return;
		}

		// 对应输出Bytes获得输入Bytes
		DWORD DefaultWriteSize;
		DWORD DefaultReadSize = chunk.size;
		Result = acmStreamSize(acm, DefaultReadSize, &DefaultWriteSize, ACM_STREAMSIZEF_SOURCE);
		if (Result != 0) {
			acmStreamClose(acm, 0);
			g_pMainState->m_GlobalButton[17] = TRUE;
			ERRBOX;
			return;
		}
		DSBUFFERDESC dsbd;
		dsbd.dwSize = sizeof(DSBUFFERDESC);
		dsbd.dwFlags = DSBCAPS_LOCSOFTWARE | DSBCAPS_STATIC;
		dsbd.dwBufferBytes = DefaultWriteSize;
		dsbd.dwReserved = 0;
		dsbd.lpwfxFormat = DstHeader;
		g_pDirectSound->CreateSoundBuffer(&dsbd, &m_pPrimaryBuffer, NULL);
			char *soundbuf;
			m_pPrimaryBuffer->Lock(0, 0, (void**)&soundbuf, &DefaultWriteSize, NULL, NULL, DSBLOCK_ENTIREBUFFER);
			// 准备变换buffer
			ACMSTREAMHEADER stream;
			ZeroMemory(&stream, sizeof(stream));
			stream.cbStruct = sizeof(stream);
			stream.pbSrc = (BYTE*)WAlloc(DefaultReadSize);
			stream.cbSrcLength = DefaultReadSize;
			stream.pbDst = (BYTE*)soundbuf;
			stream.cbDstLength = DefaultWriteSize;
			Result = acmStreamPrepareHeader(acm, &stream, 0);
			if (Result != 0) return;
			memcpy_s(stream.pbSrc, DefaultReadSize, pdata, DefaultReadSize);
			pdata += DefaultReadSize;
			stream.cbSrcLength = DefaultReadSize;
			// 变换
			Result = acmStreamConvert(acm, &stream, 0);
			if (Result != 0) return;
			m_pPrimaryBuffer->Unlock(soundbuf, DefaultWriteSize, NULL, 0);
			WFree(stream.pbSrc);
			acmStreamUnprepareHeader(acm, &stream, 0);
			Result = acmStreamClose(acm, 0);
			m_size = DefaultWriteSize;
	}
	else
	{
			DSBUFFERDESC dsbd;
			DWORD size = chunk.size;

			dsbd.dwSize = sizeof(DSBUFFERDESC);
			dsbd.dwFlags = DSBCAPS_LOCSOFTWARE | DSBCAPS_STATIC;// WSSB_CAPS
			dsbd.dwBufferBytes = size;
			dsbd.dwReserved = 0;
			dsbd.lpwfxFormat = SrcHeader;

			g_pDirectSound->CreateSoundBuffer(&dsbd, &m_pPrimaryBuffer, NULL);
			char *soundbuf;
			m_pPrimaryBuffer->Lock(0, 0, (void**)&soundbuf, &size, NULL, NULL, DSBLOCK_ENTIREBUFFER);
			memcpy_s(soundbuf, size, pdata, size);
			pdata += size;
			m_pPrimaryBuffer->Unlock(soundbuf, size, NULL, 0);
		
	}
	bLoad = TRUE;
	SetPosition(pos);
	
	if (bPlay)
		m_pPrimaryBuffer->Play(0,0,0);
}

void cMyWav::Free()
{
	if (!m_pPrimaryBuffer)return;
	m_pPrimaryBuffer->Release();
	m_pPrimaryBuffer = 0;
	bLoad = FALSE;
}

void cMyWav::SetPosition(int num)
{
	if (g_pMainState->m_GlobalButton[17])return;
	if (!num)
		m_pPrimaryBuffer->SetCurrentPosition(0);
	else
		m_pPrimaryBuffer->SetCurrentPosition((m_size*num) / 100);
	
}




