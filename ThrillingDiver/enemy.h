//
//	敵ヘッダ(enemy.h)
//
//
//
#ifndef _ENEMY_H_
#define _ENEMY_H_

//
//	インクルード
//
#include "objectX.h"
#include <vector>
#include <iostream>
#include <memory>
#include <map>

using namespace std;

namespace
{
	using vec3 = D3DXVECTOR3;
}

//エネミー数管理クラス
class CEnmManager
{
private:
	static int m_EnmCnt;	//敵総数
public:
	static void IncEnmCnt() { ++m_EnmCnt; };	//敵数加算
	static void DecEnmCnt() { --m_EnmCnt; };	//敵数減算
	static int GetEnmCnt() { return m_EnmCnt; };	//敵数取得
	static 	map<string, vector<vec3>> m_targetLists;	//ターゲット地点リスト
	static vector<string> m_targetListnameLists;		//ターゲット地点リストの名前リスト
	static void SetTargetLists(const map<string, vector<vec3>>& newtargetLists);
	static const vector<vec3>& GetTargetList(const string& listname);
};

//前方宣言
class CEnemy;

//エネミー状態のスーパークラス
class EnemyState
{
public:
	virtual ~EnemyState() = default;
	virtual void Addupdate(CEnemy* pEnemy) = 0;	//状態ごとの追加処理を行う関数
};

//	常移動状態
class NormalState :public EnemyState
{
public:
	~NormalState()override {};
	void Addupdate(CEnemy* enemy)override;
};

//	飛ばされ状態
class KnockbackState :public EnemyState
{
public:
	~KnockbackState()override {};
	void Addupdate(CEnemy* enemy)override;
};

//	エネミー管理クラス
class CEnemy :public CObjectX
{
public:
	typedef enum
	{
		NONE = 0,
		MAX
	}ENMTYPE;
	
	CEnemy(std::vector<D3DXVECTOR3> value);
	~CEnemy()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CEnemy* Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, std::vector<D3DXVECTOR3> movePos);
	D3DXVECTOR3& GetMove() { return m_move; }	//移動値取得
	void DrawEnmInfo();
	void SetState(EnemyState* pNewstate);	//敵の状態設定
	void BlowOff();	//吹っ飛ぶ動き

	static LPDIRECT3DTEXTURE9 GetTextures(const TYPE& inType);
private:
	D3DXVECTOR3 m_Oldpos;	//記録用位置
	D3DXVECTOR3 m_move;		//移動量
	bool m_bLand;			//着地フラグ
	float m_moveValue;		//移動値
	ENMTYPE m_EnmType;		//敵種
	float m_diffPL;			//プレイヤーとの距離
	D3DXVECTOR3 m_directionPL;	//プレイヤーとのベクトル
	bool m_InitPos;			//更新に入るときにその位置から1番近いところをスタートにしたかどうか
	float m_blowTime;		//飛んでから消えるまでの時間
	D3DXVECTOR3 m_BlowDir;	//飛んでいく方向
	std::vector<D3DXVECTOR3> m_movePnt;	//移動先の位置	動的配列(ベクター)を保持
	int m_movePntIdx = 0;	//移動先のインデックス	'0' 初期化
	static const std::string FILE_ENEMY;
	static LPDIRECT3DTEXTURE9 m_textures[ENMTYPE::MAX];	//タイプの分だけテクスチャを作る
	EnemyState* m_currentState;	//敵のエネミーの状態

	LPD3DXFONT m_pFont;
};

#endif // !_ENEMY_H_
