//
//	マネージャーヘッダ(manager.h)
//
//
//
#ifndef _MANAGER_H_
#define _MANAGER_H_

//========================================
//	インクルード
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

//マネージャー管理クラス
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
	static CRenderer* m_pRenderer;		//レンダラー
	static CProcess* m_pProcess;		//プロセス
	static CInputKeyboard* m_pKeyboard;	//キーボード
	static CInputGamePad* m_pGamepad;	//ゲームパッド
	static CCamera* m_pCamera;			//ビュー情報
	static CLight* m_pLight;			//ライト情報
	static CTexture* m_pTexture;		//テクスチャ情報
	static CScene* m_pScene;			//現在の画面
	static CTimeManager* m_pTimeManager;	//時間管理
};

#endif // !_MANAGER_H_
