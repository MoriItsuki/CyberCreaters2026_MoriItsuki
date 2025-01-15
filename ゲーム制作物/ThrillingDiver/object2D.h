//
//	オブジェクト2Dヘッダ(object2D.h)
//
//
//
#pragma once

//インクルード
#include "object.h"
#include <memory>

//2Dポリゴンクラス管理クラス
class CObject2D :public CObject
{
public:
	typedef struct
	{
		D3DXVECTOR3 pos;	//頂点情報
		float rhw;	//座標変換用係数(1.0fで固定)
		D3DCOLOR col;	//頂点カラー
		D3DXVECTOR2 tex;
	}VERTEX_2D;

	CObject2D(int nPriority = 3);
	virtual ~CObject2D()override;
	virtual HRESULT Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Draw()override;
	static CObject2D* Create();	//Object2Dの生成
	void BindTexture(LPDIRECT3DTEXTURE9 pTex);	//テクスチャ設定
	const D3DXVECTOR3& GetPos() { return m_pos; }
	D3DXVECTOR3& GetMove() { return m_move; }
	void SetSize(const D3DXVECTOR3 size);	//サイズセット
	void SetPos(const D3DXVECTOR3 pos);		//ポジションセット
	void SetColor(const D3DCOLOR color);	//カラーセット
protected:
	//	LPDIRECT3DTEXTURE9 = *IDirect3DVertexBuffer9
	//std::unique_ptr<IDirect3DVertexBuffer9, void(*)(IDirect3DVertexBuffer9*)> m_pVtxBuff;	//頂点バッファ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャのポインタ
	D3DXVECTOR3 m_pos;					//オブジェクトの座標
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_size;					//オブジェクトごとのサイズ
	D3DCOLOR m_color;
	float m_fAngle;
	float m_fLength;
private:
};
