//
//	シーン管理ヘッダ
//
//
//
#ifndef _SCENE_H_
#define _SCENE_H_

//
//	インクルード
//
#include "main.h"
#include "timemanager.h"
#include "manager.h"

#include "dashgauge.h"
#include "item.h"
#include "enemycount.h"
#include "startreserve.h"
#include "object2D.h"

#include "ranking.h"

#include <cassert>  // assertを使用するために必要
#include <string>
#include <vector>

//終始の合図などを表示するポリゴンクラス
class CAnnounceEvent :public CObject2D
{
public:
	CAnnounceEvent();
	~CAnnounceEvent();

	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CAnnounceEvent* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void SetNum(int num);
private:
	int m_nNum;	//番号
	bool m_isVisible;
	static const std::string FILE_ANNOUNCE;
};

//シーン同士で共有するデータ
struct ShereData
{
	float Resukttime;	//ゲーム中で記録された時間
};

//シーン管理クラス
class CScene
{
public:
	//画面の種類
	typedef enum
	{
		MODE_TITLE = 0,
		MODE_GAME,
		MODE_RESULT,
		MODE_MAX
	}MODE;
	CScene();	//コンストラクタ
	virtual ~CScene();	//デストラクタ
	virtual HRESULT Init();	//初期設定
	virtual void Uninit();	//終了
	virtual void Update();	//更新
	virtual void Draw();	//描画
	static CScene* Create(MODE mode);	//生成
	void SetTimeManager(CTimeManager* timeManager);
	void SetShareData(std::shared_ptr<ShereData> data);
	std::weak_ptr<ShereData> GetShareData()const;
protected:
	CTimeManager* m_pTimeManager; //時間管理ポインタ
private:
	MODE m_mode;	//現在のモード
	std::weak_ptr<ShereData> m_ShareData;
};

//タイトルシーン管理クラス
class CTitle :public CScene
{
public:
	CTitle();
	~CTitle();
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
private:

};

class CPlayer;

//ゲームシーン管理クラス
class CGame :public CScene
{
public:
	CGame();
	~CGame();

	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static void LoadInfo(const std::string& filename, std::vector<CItem*>& items);	//ゲーム開始時に中の情報を読み込んでアイテムを生成
	static void SaveInfo(const std::string& filename, std::vector<CItem*>& items);	//ゲーム終了時に中の情報を書き出してアイテム情報を記録

	//	ヘルパー関数
	static std::string Trim(const std::string& str);	//文字列のトリム処理
private:
	std::vector<CItem*> m_ItemList;
	CPlayer* m_pPlayer;							//アイテム処理用プレイヤー
	CENMCNTDisplayManager* m_pENMCNTManager;	//敵数表示マネージャー
	CStartReserve* m_pStartRes;					//ゲーム開始前準備待機用ポリゴン
	CAnnounceEvent* m_pAnnoEve;					//ゲーム終始のアナウンス表示用ポリゴン
};

//リザルトシーン管理クラス
class CResult :public CScene
{
public:
	CResult();
	~CResult();
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
private:
	std::unique_ptr<CRankingManager> m_pRanking;	//リザルト表示用ランキング
};

#endif // !_SCENE_H_
