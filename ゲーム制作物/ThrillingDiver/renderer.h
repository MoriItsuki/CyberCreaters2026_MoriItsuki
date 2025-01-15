//================================================================
//	レンダラークラスヘッダ(renderer.h)
//
//
//================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include<windows.h>
#include"d3dx9.h"	//画面描画に必要

class CRenderer
{
public:
	CRenderer();								//コンストラクタ
	~CRenderer();								//デストラクタ
	HRESULT Init(HWND hWnd, BOOL bWindow);		//初期設定
	void Uninit();								//終了処理
	void Update();								//更新処理
	void Draw();								//描画処理
	LPDIRECT3DDEVICE9 GetDevice();				//3Dデバイスの取得
	void WireFrame();
private:
	LPDIRECT3D9 m_pD3D;				//Direct3D
	LPDIRECT3DDEVICE9 m_pD3DDevice;	//Direct3Dデバイス
	bool m_bWireFrame;				//ワイヤーフレームの視認可能か
	float m_fFogStartPos;
	float m_fFogEndPos;
	float m_fFogDensity;

};
#endif // !_RENDERER_H_
