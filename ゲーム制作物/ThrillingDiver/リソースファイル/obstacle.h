//
//	障害物管理ヘッダ
//
//
//
#pragma once

//
//	インクルード
//
#include "objectX.h"
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <map>

using namespace std;

//
//	定数
//
namespace
{
	using vec3 = D3DXVECTOR3;
}


//障害物管理クラス
class CObsManager
{
private:
	static int m_ObsCnt;	//障害物総数
public:
	static void IncCnt() { ++m_ObsCnt; };	//敵数加算
	static void DecCnt() { --m_ObsCnt; };	//敵数減算
	static int GetCnt() { return m_ObsCnt; };	//敵数取得
	static 	map<string, vector<vec3>> m_targetLists;	//ターゲット地点リスト
	static vector<string> m_targetListnameLists;		//ターゲット地点リストの名前リスト
	static void SetTargetLists(const map<string, vector<vec3>>& newtargetLists);
	static const vector<vec3>& GetTargetList(const string& listname);
};

//
//	前方宣言
//
class CObstacle;

//障害物の種類のスーパー
class ObsType
{
public:
	virtual ~ObsType() = default;
	virtual void AddUpdate(CObstacle* pObs) = 0;	//種類ごとの更新
};

//	ノーマルクラス
class NomalType :public ObsType
{
public:
	~NomalType()override {};
	void AddUpdate(CObstacle* pObs)override;
};

//	動きのあるクラス
class MoveingType :public ObsType
{
public:
	~MoveingType()override {};
	void AddUpdate(CObstacle* pObs)override;
};

//障害物クラス
class CObstacle :public CObjectX
{
public:
	CObstacle();
	~CObstacle()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void Moveing();			//動き設定
	static CObstacle* Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale);	//ノーマルタイプ固定
	static CObstacle* Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, const std::vector<vec3>& targetList, float speed, int moveIdx = 0);	//それ以外のタイプの時はタイプと動き・速さ設定
	void SetState(ObsType* obstype);	//敵の状態設定
	bool IsColliding(const vec3& pos1, const vec3& size1, const vec3& pos2, const vec3& size2);
private:
	ObsType* m_Type;				//障害物の種類
	D3DXVECTOR3 m_BlowDir;			//反射方向
	D3DXVECTOR3 m_Oldpos;			//過去位置
	float m_diffPL;					//プレイヤーとの距離
	D3DXVECTOR3 m_RefPos;			//一定間で動くための基準点
	std::vector<vec3> m_movePnt;	//移動先の位置リスト	
	int m_movePntIdx;				//移動先のインデックス
	float m_speed;					//移動スピード
	static const std::string FILE_OBSTACLE;
};