//
//	オブジェクトXのヘッダ
//
//
//
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

//
//	インクルード
//
#include "object.h"
#include <string>

//	オブジェクトX管理クラス
class CObjectX :public CObject
{
public:
	typedef struct
	{
		D3DXVECTOR3 pos;	//頂点座標
		D3DXVECTOR3 nor;	//法線ベクトル
		D3DCOLOR col;		//頂点カラー
		D3DXVECTOR2 tex;	//テクスチャ座標
	}VERTEX_3D;

	CObjectX(int nPriority = 3);
	virtual ~CObjectX()override;
	virtual HRESULT Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Draw()override;
	static CObjectX* Create();	//ObjectXの生成
	void BindTexture(LPDIRECT3DTEXTURE9 pTex);	//テクスチャ設定
	D3DXVECTOR3& GetPos() { return m_pos; }
	D3DXVECTOR3& GetRot() { return m_rot; }
	D3DXVECTOR3& GetMove() { return m_move; }
	D3DXVECTOR3& GetScale() { return m_scale; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファ
	LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャのポインタ
	D3DXVECTOR3 m_pos;					//オブジェクトの座標
	D3DXVECTOR3 m_move;					//オブジェクトの移動量
	D3DXVECTOR3 m_rot;					//オブジェクトの回転
	D3DXVECTOR3 m_scale;				//オブジェクトの拡縮
	D3DXMATRIX m_mtxWorld;				//ワールド変換行列
	LPD3DXMESH m_pMesh;					//メッシュへのポインタ
	LPD3DXBUFFER m_pBuffMat;			//マテリアルへのポインタ
	DWORD m_dwNumMat;					//マテリアルへの数
	const char* m_ModelFile;			//Xファイル名のchar型
private:
};
#endif // !_OBJECTX_H_
