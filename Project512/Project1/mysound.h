#pragma once
#include <d3d9.h>                   // D3D头文件
#include <d3dx9.h>                  // D3DX库头文件
#include <DSound.h>
#include <dshow.h>
using namespace std;
class cMySound
{
public:
	void Reset();
	void Load(string path);
	void CheckEnd();
	void Free();
	void Stop();
	void Play(){ m_pMediaControl->Run(); }
	~cMySound(){ Free(); };
private:
	string m_PrePath;
	IGraphBuilder*      m_pGraphBuilder = NULL;
	IMediaControl*      m_pMediaControl = NULL;
	IMediaPosition*     m_pMediaPosition = NULL;
	IMediaEvent*    m_media_event = 0;
	

};


class cMyWav
{
public:
	// struct sWaveHeader {
	//	char  RiffSig[4];         // 'RIFF'
	//	long  WaveformChunkSize;  // 8
	//	char  WaveSig[4];         // 'WAVE'
	//	char  FormatSig[4];       // 'fmt ' (notice space after)
	//	long  FormatChunkSize;    // 16
	//	short FormatTag;          // WAVE_FORMAT_PCM
	//	short Channels;           // # of channels
	//	long  SampleRate;         // sampling rate
	//	long  BytesPerSec;        // bytes per second
	//	short BlockAlign;         // sample block alignment
	//	short BitsPerSample;      // bits per second
	//	char  DataSig[4];         // 'data'
	//	long  DataSize;           // size of waveform data
	//} ;
	~cMyWav(){ Free(); };
	void Load(BYTE* pdata,int size,int pos=0,BOOL bPlay=TRUE);
	//void Load2(BYTE* pdata, int size, int pos = 0, BOOL bPlay = TRUE);
	void Free();
	//int GetNowPostion(){ return m_pPrimaryBuffer->GetCurrentPosition(0, 0); }
	LPDIRECTSOUNDBUFFER     m_pPrimaryBuffer = 0;
private:
	void SetPosition(int num);
	int m_size;
	BOOL bLoad = FALSE;
	
};
