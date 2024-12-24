//
//	カメラヘッダ(camera.h)
//
//
//
#ifndef _CAMERA_H_
#define _CAMERA_H_

//インクルード
#include "main.h"
//#include "player.h"

//カメラの管理クラス
class CCamera
{
public:
	//カメラの向き
	typedef enum
	{
		FRONT = 0,
		RIGHT,
		BACK,
		LEFT,
		MAX
	}DIR;
	//カメラの状態
	typedef enum
	{
		NONE = 0,
		CORRECTING,
		FIN
	}STATE;
	//視点位置の切り替え
	enum class VpntPos
	{
		ONE = 0,
		THIRD,
	};
	CCamera();
	~CCamera();
	HRESULT Init();
	void Uninit();
	void Update();
	void SetCamera();	//カメラの設定
	void SetShake(int nShakeFrame, float fShakeValue);	//カメラ揺らしの設定
	D3DXVECTOR3 GetRot();
	int GetState();
	void SetCameraDir(int dir);
	D3DXVECTOR3 GetCameraDir();
private:
	D3DXVECTOR3 m_posV;		//視点
	D3DXVECTOR3 m_posR;		//注視点
	D3DXVECTOR3 m_vecU;		//上方向
	D3DXVECTOR3 m_rot;		//カメラの現在向き
	D3DXVECTOR3 m_rotgo;	//カメラの目標向き
	D3DXMATRIX m_mtxProjection;		//プロジェクション行列
	D3DXMATRIX m_mtxView;			//ビュー行列
	float m_distance;				//注視点と視点の距離倍率
	int m_Dir;						//カメラの向きタイプ
	int m_ShakeFrame;				//揺らす時間
	float m_ShakeValue;				//揺らす量(フレーム数)
	int m_CameraState;				//カメラの状態
	VpntPos m_Vpos;			//視点の位置
};

#endif // !_CAMERA_H_
