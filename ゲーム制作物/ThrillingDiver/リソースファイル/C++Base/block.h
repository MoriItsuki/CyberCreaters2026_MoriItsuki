//
//	ブロックヘッダ(block.h)
//
//
//
#ifndef _BLOCK_H_
#define _BLOCK_H_

//==================================
//	インクルード
//==================================
#include "objectX.h"
#include "camera.h"

//プレイヤー管理クラス
class CBlock :public CObjectX
{
public:
	typedef enum
	{
		GROUND = 0,
		MAX
	}BLTYPE;
	CBlock();
	~CBlock()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CBlock* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, BLTYPE type, D3DXVECTOR3 scale);	//Blockの生成
	BLTYPE GetBlockType();
	D3DXVECTOR3 GetScale();
	static LPDIRECT3DTEXTURE9 GetTextures(const BLTYPE& inType);
private:
	float m_moveValue;
	BLTYPE m_type;
	int m_dir1;
	int m_dir2;
	bool m_bTouchPL;	//プレイヤーと接触しているかどうか
	static const std::string FILE_BLOCK;
	
	static LPDIRECT3DTEXTURE9 m_textures[BLTYPE::MAX];	//タイプの分だけテクスチャを作る
};

#endif // !_BLOCK_H_

