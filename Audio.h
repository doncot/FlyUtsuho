/*
Audio.h
音楽やSEの再生を担当
*/

#pragma once
#include<Windows.h>
#include<xaudio2.h>
#include<MyException.h>
#include<Common.h>
#include<sstream>
#include<algorithm>
#include<memory>
#include<cassert>

namespace Inferno
{
class AudioMaster
{
public:
	AudioMaster() : m_xaudio(nullptr), m_masterVoice(nullptr) {}
	~AudioMaster()
	{
		if (m_masterVoice)
		{
			m_masterVoice->DestroyVoice();
			m_masterVoice = nullptr;
		}
		SAFE_RELEASE(m_xaudio);

		//com用
		CoUninitialize();
	}

	void Initialize()
	{
		HRESULT hr;
		if (FAILED(hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
		{
			throw Inferno::CreationFailed(TEXT("ColnitializeEX"));
		}
		if (FAILED(hr = XAudio2Create(&m_xaudio, XAUDIO2_DEBUG_ENGINE)))
		{
			throw Inferno::CreationFailed(TEXT("XAudio2"));
		}
		if (FAILED(hr = m_xaudio->CreateMasteringVoice(&m_masterVoice)))
		{
			throw Inferno::CreationFailed(TEXT("MasteringVoice"));
		}
	}

	//XAudioEngineを返す
	//XAudio依存になるが……
	IXAudio2* GetXAudioEngine() const
	{
		return m_xaudio;
	}

private:
	IXAudio2* m_xaudio;
	IXAudio2MasteringVoice* m_masterVoice;
};


class Audio
{
public:
	Audio() : m_isDataLoaded(false){}
	~Audio()
	{
		if (m_sourceVoice)
		{
			m_sourceVoice->Stop();
			m_sourceVoice->DestroyVoice();
		}

	}

	void Initialize(AudioMaster* master)
	{
		if (!m_audioMaster)
		{
			m_audioMaster = master;
		}
	}

	//ラの音を一秒間鳴らす
	void CreateTestSound()
	{
		//既にロードしている場合は何もしない
		if (m_isDataLoaded) return;

		m_waveFormat.wFormatTag = WAVE_FORMAT_PCM;
		m_waveFormat.nChannels = 1;
		m_waveFormat.wBitsPerSample = 16;
		m_waveFormat.nSamplesPerSec = 44100;
		m_waveFormat.nBlockAlign = m_waveFormat.wBitsPerSample / 8 * m_waveFormat.nChannels;
		m_waveFormat.nAvgBytesPerSec = m_waveFormat.nSamplesPerSec * m_waveFormat.nBlockAlign;
	
		//  SourceVoiceの作成
		//
		if (FAILED(m_audioMaster->GetXAudioEngine()->CreateSourceVoice(&m_sourceVoice, &m_waveFormat)))
			throw Inferno::CreationFailed(TEXT("CreateSourceVoice"));

		//バッファのサイズを確保

		//ラを表すsin波（１秒）
		m_waveData.resize(m_waveFormat.nAvgBytesPerSec * 1);
		short* p = (short*) &m_waveData[0];
		for (size_t i = 0; i < m_waveData.size() / 2; i++)
		{
			float length = m_waveFormat.nSamplesPerSec / 440.0f;          //波長
			*p = (short)(32767 * sinf(i * 3.1415926535f / (length / 2)));
			p++;
		}

		//
		//  SourceVoiceにデータを送信
		//
		m_buffer.AudioBytes = m_waveData.size();            //バッファのバイト数
		m_buffer.pAudioData = &m_waveData[0];             //バッファの先頭アドレス
		m_buffer.Flags = XAUDIO2_END_OF_STREAM;       // tell the source voice not to expect any data after this buffer
		m_sourceVoice->SubmitSourceBuffer(&m_buffer);

		m_isDataLoaded = true;
	}

	void LoadWaveFile(const wstring& fileName)
	{
		//既にロードしている場合は何もしない
		if (m_isDataLoaded) return;

		//http://www13.plala.or.jp/kymats/study/MULTIMEDIA/load_wave.html より
		//ファイルを開いて、その全てをメモリ領域にコピーします。
		HANDLE fh = CreateFile(fileName.c_str(), GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (fh == INVALID_HANDLE_VALUE)
		{
			throw Inferno::MyExceptionBase_RuntimeError(
				_T("ファイル") + fileName + _T("が開けません。")
				);
		}
		DWORD dwFileSize = GetFileSize(fh, NULL);
		BYTE *lpBuf = (BYTE*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwFileSize);
		DWORD dwReadSize;
		ReadFile(fh, lpBuf, dwFileSize, &dwReadSize, NULL);
		CloseHandle(fh);
		
		//ファイルの先頭から8～11バイトに "WAVE" という文字列があるかチェック
		char str[4];
		strncpy(str, (char*)(lpBuf + 8), 4);
		if (strncmp(str, "WAVE", 4))
		{
			HeapFree(GetProcessHeap(), 0, lpBuf);
			throw Inferno::GeneralFileError(fileName.c_str(),_T("WAVEファイルを指定して下さい")	);
		}

		// フォーマット情報をコピー
		CopyMemory(&m_waveFormat, lpBuf + 20, 16);

		//PCM形式のwavファイルかどうかをチェック
		if (m_waveFormat.wFormatTag != WAVE_FORMAT_PCM){
			HeapFree(GetProcessHeap(), 0, lpBuf);
			Inferno::GeneralFileError(fileName.c_str(), _T("PCM形式のWAVEファイルを指定して下さい"));
		}

		//波形データまでのオフセット
		strncpy(str, (char*)(lpBuf + 36), 4);
		int offset;
		if (!strncmp(str, "fact", 4)) offset = 56;         // factチャンクが有る
		else if (!strncmp(str, "data", 4)) offset = 44;    // dataチャンク(factチャンクが無い)
		else{
			HeapFree(GetProcessHeap(), 0, lpBuf);
			Inferno::GeneralFileError(fileName.c_str(), _T("正体不明のファイルです"));
		}

		// 波形データをコピー（解放するためのポインタを残しておくため）
		DWORD size = *(DWORD*)(lpBuf + offset - 4);
		m_waveData.resize(size);
		std::copy(lpBuf, lpBuf + size, m_waveData.begin());
		HeapFree(GetProcessHeap(), 0, lpBuf);

		m_waveHeader.lpData = reinterpret_cast<LPSTR>( &m_waveData[0] );
		m_waveHeader.dwBufferLength = size;
		m_waveHeader.dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP;
		m_waveHeader.dwLoops = 1;

		//  SourceVoiceの作成
		//
		if (FAILED(m_audioMaster->GetXAudioEngine()->CreateSourceVoice(&m_sourceVoice, &m_waveFormat)))
			throw Inferno::CreationFailed(TEXT("CreateSourceVoice"));

		//  SourceVoiceにデータを送信
		//
		m_buffer.AudioBytes = m_waveData.size();            //バッファのバイト数
		m_buffer.pAudioData = &m_waveData[0];             //バッファの先頭アドレス
		m_buffer.Flags = XAUDIO2_END_OF_STREAM;       // tell the source voice not to expect any data after this buffer
		m_sourceVoice->SubmitSourceBuffer(&m_buffer);

		m_isDataLoaded = true;
	}

	void Play() const
	{
		if (m_isDataLoaded)
		{
			m_sourceVoice->Start();
		}
	}

	void Stop()
	{
		if (m_isDataLoaded)
		{
			m_sourceVoice->Stop();
			m_sourceVoice->FlushSourceBuffers();
			m_sourceVoice->SubmitSourceBuffer(&m_buffer);
		}
	}

	//0.0-1.0の範囲でボリュームを指定する
	void SetVolume(const float vol)
	{
		assert( ( 0.0f <= vol && vol <= 1.0f) );
		if (m_isDataLoaded)
		{
			m_sourceVoice->SetVolume(vol);
		}
	}

private:
	bool m_isDataLoaded;

	AudioMaster* m_audioMaster;
	IXAudio2SourceVoice* m_sourceVoice;
	XAUDIO2_BUFFER m_buffer;
	WAVEFORMATEX m_waveFormat;
	WAVEHDR m_waveHeader;
	std::vector< BYTE > m_waveData; //ロードした波形情報

};

namespace
{
	
	//sourcevoiceのコールバック用関数
	class VoiceCallback : public IXAudio2VoiceCallback
	{
	public:
		HANDLE hBufferEndEvent;
		VoiceCallback() : hBufferEndEvent(CreateEvent(NULL, FALSE, FALSE, NULL)){}
		~VoiceCallback(){ CloseHandle(hBufferEndEvent); }

		//Called when the voice has just finished playing a contiguous audio stream.
		void _stdcall OnStreamEnd() 
		{
			SetEvent(hBufferEndEvent); 
		}

		/*
		//Unused methods are stubs
		void OnVoiceProcessingPassEnd() { }
		void OnVoiceProcessingPassStart(UINT32 SamplesRequired) {    }
		void OnBufferEnd(void * pBufferContext)    { }
		void OnBufferStart(void * pBufferContext) {    }
		void OnLoopEnd(void * pBufferContext) {    }
		void OnVoiceError(void * pBufferContext, HRESULT Error) { }
		*/
	};
	
}

}