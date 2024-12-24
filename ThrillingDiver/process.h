//
//	プロセスヘッダ(process.h)	
//
//
//
#ifndef _PROCESS_H_
#define _PROCESS_H_

//
//	インクルード
//
#include "main.h"
#include <windows.h>

class CProcess
{
public:
	CProcess();
	~CProcess();
	static const int MAX_VERTEX = 4;		//ポリゴンの最低頂点数
	static const int SCREEN_WIDTH = 1280;	//スクリーン横サイズ
	static const int SCREEN_HEIGHT = 720;	//スクリーン縦サイズ
	static const int FVF_VERTEX_2D = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	static const int FVF_VERTEX_3D = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	static const int OBSTIME = 500;			//0.5秒計測用定数
	static const int MEASURE_MS = 1000;		//1000ミリ秒計測用定数
	static const int FPS_BASE = 60;			//ゲーム内FPS基準用定数
	HRESULT Init();
	void Uninit();
	void Update();
	float GetDeltaTime()const;				//デルタタイム取得

	void DrawFPS();
	static int m_nCntFPS;	//FPS
	static LPD3DXFONT m_pFont;
private:
	LARGE_INTEGER m_LastTime;	//前回のフレーム時間
	float m_DeltaTime;			//デルタタイム
};
#endif // !_PROCESS_H_
