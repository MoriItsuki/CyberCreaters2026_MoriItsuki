//
//	オブジェクトヘッダ(object.h)
//
//
//
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"

//マクロ定義
#define MAX_OBJECT	(1024)	//オブジェクトの最大数
#define NUM_PRIORITY	(64)	//プライオリティの最大数

//オブジェクト基底クラス
class CObject
{
public:
	typedef enum
	{
		NONE = 0,	//無し
		ENEMY,		//敵
		BLOCK,		//ブロック
		PLAYER,		//プレイヤー
		OBSTACLE,	//障害物
		DIRMARKER,	//方向表示
		ITEM,		//アイテム
		MAX
	}TYPE;

	CObject(int nPriority = 3);	//コンストラクタ 描画優先順設定
	virtual ~CObject();			//デストラクタ
	virtual HRESULT Init() = 0;	//初期設定
	virtual void Uninit() = 0;	//終了処理
	virtual void Update() = 0;	//更新処理
	virtual void Draw() = 0;	//描画処理
	static void ReleaseAll();	//全オブジェクト解放
	static void UpdateAll();	//全オブジェクト更新
	static void DrawAll();		//全オブジェクト描画
	static CObject* GetObj(int nPri, int nIdx);	//オブジェクト取得
	TYPE GetType();				//タイプ取得
	void SetType(TYPE type);	//タイプ設定
protected:
	void Release();			//自分自身の解放
private:
	static CObject* m_apObject[NUM_PRIORITY][MAX_OBJECT];	//オブジェクト管理
	int m_nPriority;	//描画優先度
	static int m_nNumAll;			//オブジェクト総数
	int m_nID;						//自分自身のID
	TYPE m_type;		//オブジェクトタイプ
};
#endif // !_OBJECT_H_

