//
//	オブジェクト3Dヘッダ(object3D.h)
//
//
//
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//インクルード
#include "object.h"

//3Dポリゴンクラス管理クラス
class CObject3D :public CObject
{
public:
	typedef struct
	{
		D3DXVECTOR3 pos;	//頂点座標
		D3DXVECTOR3 nor;	//法線ベクトル
		D3DCOLOR col;		//頂点カラー
		D3DXVECTOR2 tex;	//テクスチャ座標
	}VERTEX_3D;

	CObject3D();
	virtual ~CObject3D()override;
	virtual HRESULT Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Draw()override;
	static CObject3D* Create();	//Object3Dの生成
	void BindTexture(LPDIRECT3DTEXTURE9 pTex);	//テクスチャ設定
	D3DXVECTOR3& GetPos() { return m_pos; }
	void SetSize(D3DXVECTOR3 size);
protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファ
	LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャのポインタ
	D3DXVECTOR3 m_pos;					//オブジェクトの座標
	D3DXVECTOR3 m_rot;					//オブジェクトの回転
	D3DXVECTOR3 m_move;					//オブジェクトごとの移動値
	D3DXVECTOR3 m_size;					//オブジェクトごとのサイズ
	D3DXMATRIX m_mtxWorld;				//ワールド変換行列
};
#endif // !_OBJECT3D_H_
