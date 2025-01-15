//
//	プレイヤーヘッダ(player.h)
//
//
//
#ifndef _PLAYER_H_
#define _PLAYER_H_

//==================================
//	インクルード
//==================================
#include "objectX.h"
#include "camera.h"
#include "enemy.h"
#include "dashgauge.h"

#include <string>

//
//	定数
//
namespace
{
	using vec3 = D3DXVECTOR3;
	using vec2 = D3DXVECTOR2;
}

//プレイヤー管理クラス
class CPlayer :public CObjectX
{
public:
	//生死の状態
	typedef enum
	{
		NONE = 0,
		DEATH,
		MAX
	}STATE;
	//移動の状態
	typedef enum
	{
		STOP = 0,	//停止
		FORWARD,	//前進
		BACK,		//後進
		DASH,		//ダッシュ
	}MOVESTATE;	
	CPlayer();
	~CPlayer()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	//playerの生成
	void SetPos(D3DXVECTOR3 pos);
	void SetMove(D3DXVECTOR3 move);
	void SetReflec();		//障害物反射
	void SetState(MOVESTATE state);		//プレイヤー状態設定
	D3DXVECTOR3 GetPos();
	D3DXVECTOR3 GetVecFront();
	int GetState();
	CEnemy* GetNearestEnm()const;	//最寄りの敵取得
	D3DXVECTOR3 GetNearestEnmPos()const;	//最寄りの敵位置取得
	int GetDashCnt()const;	//残りダッシュ回数取得
	void DrawPLInfo();
	bool IsColliding(const vec3& pos1, const vec3& size1, const vec3& pos2, const vec3& size2);
private:
	int m_nDashCnt;				//ダッシュできる回数
	int m_nItemCnt;				//もっているアイテムの数
	D3DXVECTOR3 m_rotgo;		//目的の角度
	D3DXVECTOR3 m_Oldpos;		//記録用位置
	float m_fFriction;			//摩擦係数
	float m_moveValue;			//移動値
	int m_nState;				//プレイヤーの状態
	MOVESTATE m_MoveState;		//移動の状態
	D3DXVECTOR3 m_vecFront;		//プレイヤーの前方向のベクトル
	bool m_bStop;				//止まっているか
	bool m_bDash;				//ダッシュしているか
	CEnemy* m_pNearestEnemy = nullptr;	//最も近い敵ポインタ
	D3DXVECTOR3 m_NearestEnmPos;	//最寄りの敵位置
	CDashgauge* m_pDashGauge;		//ダッシュ情報視覚化ゲージ

	static const std::string FILE_PLAYER;
	LPD3DXFONT m_pFont;
};

#endif // !_PLAYER_H_

