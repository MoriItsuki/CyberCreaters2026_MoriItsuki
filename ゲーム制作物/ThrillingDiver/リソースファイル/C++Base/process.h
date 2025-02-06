//
//	�v���Z�X�w�b�_(process.h)	
//
//
//
#ifndef _PROCESS_H_
#define _PROCESS_H_

//
//	�C���N���[�h
//
#include "main.h"
#include <windows.h>

class CProcess
{
public:
	CProcess();
	~CProcess();
	static const int MAX_VERTEX = 4;		//�|���S���̍Œᒸ�_��
	static const int SCREEN_WIDTH = 1280;	//�X�N���[�����T�C�Y
	static const int SCREEN_HEIGHT = 720;	//�X�N���[���c�T�C�Y
	static const int FVF_VERTEX_2D = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	static const int FVF_VERTEX_3D = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	static const int OBSTIME = 500;			//0.5�b�v���p�萔
	static const int MEASURE_MS = 1000;		//1000�~���b�v���p�萔
	static const int FPS_BASE = 60;			//�Q�[����FPS��p�萔
	HRESULT Init();
	void Uninit();
	void Update();
	float GetDeltaTime()const;				//�f���^�^�C���擾

	void DrawFPS();
	static int m_nCntFPS;	//FPS
	static LPD3DXFONT m_pFont;
private:
	LARGE_INTEGER m_LastTime;	//�O��̃t���[������
	float m_DeltaTime;			//�f���^�^�C��
};
#endif // !_PROCESS_H_
