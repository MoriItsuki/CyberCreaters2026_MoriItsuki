//
//	�}�l�[�W���[�w�b�_(manager.h)
//
//
//
#ifndef _MANAGER_H_
#define _MANAGER_H_

//========================================
//	�C���N���[�h
//========================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "process.h"
#include "camera.h"
#include "light.h"
#include "scene.h"
#include "object.h"
#include "timemanager.h"

#include "dashgauge.h"

//
//
//
class CTexture;

//�}�l�[�W���[�Ǘ��N���X
class CManager
{
public:
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit();
	void Update();
	void Draw();
	static CRenderer* GetRenderer();
	static CProcess* GetProcess();
	static CInputKeyboard* GetKeyboard();
	static CInputGamePad* GetGamepad();
	static CCamera* GetCamera();
	static CLight* GetLight();
	static CTexture* GetTexture();
	static CTimeManager* GetTimeManager();
	static void SetMode(CScene::MODE mode);
private:
	static CRenderer* m_pRenderer;		//�����_���[
	static CProcess* m_pProcess;		//�v���Z�X
	static CInputKeyboard* m_pKeyboard;	//�L�[�{�[�h
	static CInputGamePad* m_pGamepad;	//�Q�[���p�b�h
	static CCamera* m_pCamera;			//�r���[���
	static CLight* m_pLight;			//���C�g���
	static CTexture* m_pTexture;		//�e�N�X�`�����
	static CScene* m_pScene;			//���݂̉��
	static CTimeManager* m_pTimeManager;	//���ԊǗ�
};

#endif // !_MANAGER_H_
