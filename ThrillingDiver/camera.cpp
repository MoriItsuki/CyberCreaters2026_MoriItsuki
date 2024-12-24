//================================================================================
//	カメラCPP
//
//
//================================================================================

//=========================================================
//	インクルード
//=========================================================
#include "main.h"
#include "camera.h"
#include "manager.h"
#include "object.h"
#include "player.h"
//#include "guid2D.h"
#include <time.h>
#include <stdio.h>

//=========================================================
//	静的メンバ関数初期化
//=========================================================

//=========================================================
//	マクロ定義
//=========================================================
#define CAMERAMOVE	(10.0f)	//カメラの速さ
#define DIST_THIRD	(150.0f)	//注視点視点倍率距離(3人称)
#define DIST_ONE	(100.0f)	//注視点視点倍率距離(1人称)

//=========================================================
//	コンストラクタ
//=========================================================
CCamera::CCamera() :m_ShakeFrame(0), m_ShakeValue(0.0f), m_CameraState(STATE::NONE), m_Vpos(VpntPos::THIRD)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//プロジェクションマトリックス
	m_posV = D3DXVECTOR3(0.0f, 200.0f, -100.0f);	//視点
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//注視点
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//上方向ベクトル
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//視点の向き
	m_rotgo = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//視点の向き
	m_distance = 0.5f;								//注視点と視点の距離倍率
	m_Dir = FRONT;									//カメラの向き( 正面 )
}

//=========================================================
//	デストラクタ
//=========================================================
CCamera::~CCamera()
{

}

//=========================================================
//	初期設定
//=========================================================
HRESULT CCamera::Init()
{
	//プロジェクションマトリックス
	m_posV = D3DXVECTOR3(0.0f, 200.0f, -100.0f);	//視点
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//注視点
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//上方向ベクトル
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//視点の向き
	m_rotgo = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//視点の向き
	srand((int)time(NULL));	//ランダムシードを設定

	return S_OK;
}

//=========================================================
//	終了処理
//=========================================================
void CCamera::Uninit()
{

}

//=========================================================
//	更新処理
//=========================================================
void CCamera::Update()
{
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();

	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		//オブジェクト取得
		CObject* pObj = CObject::GetObj(3, nCntObj);

		if (pObj != nullptr)
		{
			CObject::TYPE type = pObj->GetType();

			//種類の取得
			if (type == CObject::TYPE::PLAYER)
			{
				CPlayer* pPlayer = (CPlayer*)pObj;

				//プレイヤーの角度を取得
				D3DXVECTOR3 PLrot = pPlayer->GetRot();

				//プレイヤーのz軸回転を取得
				if (m_Vpos == VpntPos::THIRD)
				{
					//	カメラの注視点をプレイヤーより少し上にあげる
					m_posR = pPlayer->GetPos() += { 0.0f, 20.0f, 0.0f };

					//	視点は注視点のさらに少し上に設定
					m_posV.y = m_posR.y + 15.0f;

					m_posV.x = m_posR.x - sinf(PLrot.y + D3DX_PI) * DIST_THIRD;
					m_posV.z = m_posR.z - cosf(PLrot.y + D3DX_PI) * DIST_THIRD;
				}
				else if (m_Vpos == VpntPos::ONE)
				{
					m_posV = pPlayer->GetPos();

					m_posR.x = m_posV.x - sinf(PLrot.y) * DIST_ONE;
					m_posR.z = m_posV.z - cosf(PLrot.y) * DIST_ONE;
					m_posR.y = m_posV.y - 10.0f;
				}

				////カメラの上方向ベクトルをプレイヤーの傾きを参考に調整
				//D3DXMATRIX mtxRotZ;
				//D3DXVECTOR3 vecU = { 0.0f,1.0f,0.0f };
				//D3DXMatrixRotationZ(&mtxRotZ, PLrot.z);	//プレイヤーのz軸回転行列
				//D3DXVec3TransformCoord(&m_vecU, &vecU, &mtxRotZ);

				//カメラにプレイヤーの角度を反映
				//m_rot.z = PLrot.z;
			}
		}
	}

#ifdef _DEBUG
	//視点の方向(視点自転)
	if (pKeyboard->GetTrigger(DIK_NUMPAD4) == true)
	{
		m_Dir++;
		if (m_Dir >= LEFT + 1)
		{
			m_Dir = FRONT;
		}
	}
	if (pKeyboard->GetTrigger(DIK_NUMPAD6) == true)
	{
		m_Dir--;
		if (m_Dir <= FRONT - 1)
		{
			m_Dir = LEFT;
		}
	}

	//注視点と視点間の距離操作
	if (pKeyboard->GetPress(DIK_P) == true)
	{
		m_distance += 0.01f;
	}
	if (pKeyboard->GetPress(DIK_L) == true)
	{
		m_distance -= 0.01f;
	}
#endif // DEBUG

	if (pKeyboard->GetTrigger(DIK_1) == true)
	{
		m_Vpos = VpntPos::ONE;
	}
	else if (pKeyboard->GetTrigger(DIK_3) == true)
	{
		m_Vpos = VpntPos::THIRD;
	}

	//y軸処理
	if (m_rotgo.y > D3DX_PI)
	{
		float fOverRot = fabsf(m_rotgo.y) - D3DX_PI;
		m_rotgo.y = -(D3DX_PI - fOverRot);
	}
	else if (m_rotgo.y < -D3DX_PI)
	{
		float fOverRot = fabsf(m_rotgo.y) - D3DX_PI;
		m_rotgo.y = (D3DX_PI - fOverRot);
	}

	//z軸処理
	/*if (m_rotgo.z > D3DX_PI)
	{
		float fOverRot = fabsf(m_rotgo.z) - D3DX_PI;
		m_rotgo.z = -(D3DX_PI - fOverRot);
	}
	else if (m_rotgo.z < -D3DX_PI)
	{
		float fOverRot = fabsf(m_rotgo.z) - D3DX_PI;
		m_rotgo.z = (D3DX_PI - fOverRot);
	}*/

	//目的の角度と現在の角度の差を求める
	float rotDiffY = m_rotgo.y - m_rot.y;
	//float rotDiffZ = m_rotgo.z - m_rot.z;

	//y軸処理
	if (rotDiffY > D3DX_PI)
	{//目的と現在の角度の差が3.14…以上なら
		m_rotgo.y -= (D3DX_PI * 2.0f);
		rotDiffY = m_rotgo.y - m_rot.y;
	}
	else if (rotDiffY < -D3DX_PI)
	{//-3.14…以下なら
		m_rotgo.y += (D3DX_PI * 2.0f);
		rotDiffY = m_rotgo.y - m_rot.y;
	}

	////z軸処理
	//if (rotDiffZ > D3DX_PI)
	//{//目的と現在の角度の差が3.14…以上なら
	//	m_rotgo.z -= (D3DX_PI * 2.0f);
	//	rotDiffZ = m_rotgo.z - m_rot.z;
	//}
	//else if (rotDiffZ < -D3DX_PI)
	//{//-3.14…以下なら
	//	m_rotgo.z += (D3DX_PI * 2.0f);
	//	rotDiffZ = m_rotgo.z - m_rot.z;
	//}

	m_rot.y += (rotDiffY * 0.080f);	//だんだんと目的の角度に補正していく
	//m_rot.z += (rotDiffZ * 0.080f);	//だんだんと目的の角度に補正していく

	//y軸処理
	if ((m_rot.y) > D3DX_PI)
	{
		float overRot = fabsf(m_rot.y) - D3DX_PI;
		m_rot.y = -(D3DX_PI - overRot);
	}
	else if ((m_rot.y) < -D3DX_PI)
	{
		float overRot = fabsf(m_rot.y) - D3DX_PI;
		m_rot.y = (D3DX_PI - overRot);
	}

	////z軸処理
	//if ((m_rot.z) > D3DX_PI)
	//{
	//	float overRot = fabsf(m_rot.z) - D3DX_PI;
	//	m_rot.z = -(D3DX_PI - overRot);
	//}
	//else if ((m_rot.z) < -D3DX_PI)
	//{
	//	float overRot = fabsf(m_rot.z) - D3DX_PI;
	//	m_rot.z = (D3DX_PI - overRot);
	//}

	//	位置設定用の変数
	float VRDist = 0.0f;

	if (m_Vpos == VpntPos::THIRD)
	{//	3人称
		VRDist = DIST_THIRD;
	}
	else if (m_Vpos == VpntPos::ONE)
	{//	1人称
		VRDist = DIST_ONE;
	}

	//補正中かの確認
	if (fabsf(rotDiffY) >= 0.001f)
	{
		m_CameraState = STATE::CORRECTING;
	}
	else
	{
		m_CameraState = STATE::NONE;
	}
}

//=========================================================
//	カメラセット
//=========================================================
void CCamera::SetCamera()
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//キーボード受付
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),												//視野角
		(float)CProcess::SCREEN_WIDTH / (float)CProcess::SCREEN_HEIGHT,		//画面のアスペクト比
		10.0f,																//zの最小値
		15000.0f);															//zの最大値

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//カメラ揺れの反映
	D3DXVECTOR3 adjust = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (m_ShakeFrame > 0)
	{
		m_ShakeFrame--;
		adjust.x = ((float)rand() / ((float)RAND_MAX + 1)) * m_ShakeValue;
		adjust.y = ((float)rand() / ((float)RAND_MAX + 1)) * m_ShakeValue;
		adjust.z = ((float)rand() / ((float)RAND_MAX + 1)) * m_ShakeValue;
	}

	////平行投影でカメラを設定
	//D3DXMatrixOrthoLH(&m_mtxProjection,
	//	(float)CProcess::SCREEN_WIDTH * m_distance,
	//	(float)CProcess::SCREEN_HEIGHT * m_distance,
	//	DISTANCE - 22.0f,		//プレイヤーの左右のみを認識可能にする
	//	DISTANCE + 500.0f);	//同じ面に存在する場合に映る

	D3DXVECTOR3 posV = m_posV + adjust;
	D3DXVECTOR3 posR = m_posR + adjust;

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&posV,			//視点
		&posR,			//注視点
		&m_vecU);		//上方向
	
	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=========================================================
//	カメラの向き取得
//=========================================================
D3DXVECTOR3 CCamera::GetRot()
{
	return m_rot;
}

//=========================================================
//	カメラの揺れ設定
//=========================================================
void CCamera::SetShake(int nShakeFrame, float fShakeValue)
{
	m_ShakeFrame = nShakeFrame;
	m_ShakeValue = fShakeValue;
}

//=========================================================
//	カメラの状態取得
//=========================================================
int CCamera::GetState()
{
	return m_CameraState;
}

//=========================================================
//	カメラの方向設定
//=========================================================
void CCamera::SetCameraDir(int dir)
{
	m_Dir = dir;
}

//=========================================================
//	カメラの方向取得
//=========================================================
D3DXVECTOR3 CCamera::GetCameraDir()
{
	return m_rot;
}