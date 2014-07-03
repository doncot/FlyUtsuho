/*
Audio.h
���y��SE�̍Đ���S��
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

		//com�p
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

	//XAudioEngine��Ԃ�
	//XAudio�ˑ��ɂȂ邪�c�c
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

	//���̉�����b�Ԗ炷
	void CreateTestSound()
	{
		//���Ƀ��[�h���Ă���ꍇ�͉������Ȃ�
		if (m_isDataLoaded) return;

		m_waveFormat.wFormatTag = WAVE_FORMAT_PCM;
		m_waveFormat.nChannels = 1;
		m_waveFormat.wBitsPerSample = 16;
		m_waveFormat.nSamplesPerSec = 44100;
		m_waveFormat.nBlockAlign = m_waveFormat.wBitsPerSample / 8 * m_waveFormat.nChannels;
		m_waveFormat.nAvgBytesPerSec = m_waveFormat.nSamplesPerSec * m_waveFormat.nBlockAlign;
	
		//  SourceVoice�̍쐬
		//
		if (FAILED(m_audioMaster->GetXAudioEngine()->CreateSourceVoice(&m_sourceVoice, &m_waveFormat)))
			throw Inferno::CreationFailed(TEXT("CreateSourceVoice"));

		//�o�b�t�@�̃T�C�Y���m��

		//����\��sin�g�i�P�b�j
		m_waveData.resize(m_waveFormat.nAvgBytesPerSec * 1);
		short* p = (short*) &m_waveData[0];
		for (size_t i = 0; i < m_waveData.size() / 2; i++)
		{
			float length = m_waveFormat.nSamplesPerSec / 440.0f;          //�g��
			*p = (short)(32767 * sinf(i * 3.1415926535f / (length / 2)));
			p++;
		}

		//
		//  SourceVoice�Ƀf�[�^�𑗐M
		//
		m_buffer.AudioBytes = m_waveData.size();            //�o�b�t�@�̃o�C�g��
		m_buffer.pAudioData = &m_waveData[0];             //�o�b�t�@�̐擪�A�h���X
		m_buffer.Flags = XAUDIO2_END_OF_STREAM;       // tell the source voice not to expect any data after this buffer
		m_sourceVoice->SubmitSourceBuffer(&m_buffer);

		m_isDataLoaded = true;
	}

	void LoadWaveFile(const wstring& fileName)
	{
		//���Ƀ��[�h���Ă���ꍇ�͉������Ȃ�
		if (m_isDataLoaded) return;

		//http://www13.plala.or.jp/kymats/study/MULTIMEDIA/load_wave.html ���
		//�t�@�C�����J���āA���̑S�Ă��������̈�ɃR�s�[���܂��B
		HANDLE fh = CreateFile(fileName.c_str(), GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (fh == INVALID_HANDLE_VALUE)
		{
			throw Inferno::MyExceptionBase_RuntimeError(
				_T("�t�@�C��") + fileName + _T("���J���܂���B")
				);
		}
		DWORD dwFileSize = GetFileSize(fh, NULL);
		BYTE *lpBuf = (BYTE*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwFileSize);
		DWORD dwReadSize;
		ReadFile(fh, lpBuf, dwFileSize, &dwReadSize, NULL);
		CloseHandle(fh);
		
		//�t�@�C���̐擪����8�`11�o�C�g�� "WAVE" �Ƃ��������񂪂��邩�`�F�b�N
		char str[4];
		strncpy(str, (char*)(lpBuf + 8), 4);
		if (strncmp(str, "WAVE", 4))
		{
			HeapFree(GetProcessHeap(), 0, lpBuf);
			throw Inferno::GeneralFileError(fileName.c_str(),_T("WAVE�t�@�C�����w�肵�ĉ�����")	);
		}

		// �t�H�[�}�b�g�����R�s�[
		CopyMemory(&m_waveFormat, lpBuf + 20, 16);

		//PCM�`����wav�t�@�C�����ǂ������`�F�b�N
		if (m_waveFormat.wFormatTag != WAVE_FORMAT_PCM){
			HeapFree(GetProcessHeap(), 0, lpBuf);
			Inferno::GeneralFileError(fileName.c_str(), _T("PCM�`����WAVE�t�@�C�����w�肵�ĉ�����"));
		}

		//�g�`�f�[�^�܂ł̃I�t�Z�b�g
		strncpy(str, (char*)(lpBuf + 36), 4);
		int offset;
		if (!strncmp(str, "fact", 4)) offset = 56;         // fact�`�����N���L��
		else if (!strncmp(str, "data", 4)) offset = 44;    // data�`�����N(fact�`�����N������)
		else{
			HeapFree(GetProcessHeap(), 0, lpBuf);
			Inferno::GeneralFileError(fileName.c_str(), _T("���̕s���̃t�@�C���ł�"));
		}

		// �g�`�f�[�^���R�s�[�i������邽�߂̃|�C���^���c���Ă������߁j
		DWORD size = *(DWORD*)(lpBuf + offset - 4);
		m_waveData.resize(size);
		std::copy(lpBuf, lpBuf + size, m_waveData.begin());
		HeapFree(GetProcessHeap(), 0, lpBuf);

		m_waveHeader.lpData = reinterpret_cast<LPSTR>( &m_waveData[0] );
		m_waveHeader.dwBufferLength = size;
		m_waveHeader.dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP;
		m_waveHeader.dwLoops = 1;

		//  SourceVoice�̍쐬
		//
		if (FAILED(m_audioMaster->GetXAudioEngine()->CreateSourceVoice(&m_sourceVoice, &m_waveFormat)))
			throw Inferno::CreationFailed(TEXT("CreateSourceVoice"));

		//  SourceVoice�Ƀf�[�^�𑗐M
		//
		m_buffer.AudioBytes = m_waveData.size();            //�o�b�t�@�̃o�C�g��
		m_buffer.pAudioData = &m_waveData[0];             //�o�b�t�@�̐擪�A�h���X
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

	//0.0-1.0�͈̔͂Ń{�����[�����w�肷��
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
	std::vector< BYTE > m_waveData; //���[�h�����g�`���

};

namespace
{
	
	//sourcevoice�̃R�[���o�b�N�p�֐�
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