//============================================================================
//	ブロックCPP(block.cpp)
//
//
//============================================================================
//==========================================================
//インクルード
//==========================================================
#include "main.h"
#include "block.h"
#include "manager.h"
#include "texture.h"

//==========================================================
//	マクロ定義
//==========================================================

//==========================================================
//	グローバル変数宣言
//==========================================================

//==========================================================
//	静的メンバ関数初期化
//==========================================================
const std::string CBlock::FILE_BLOCK = "data\\texture\\ground.png";
LPDIRECT3DTEXTURE9 CBlock::m_textures[BLTYPE::MAX] = {};

//==========================================================
//	コンストラクタ
//==========================================================
CBlock::CBlock() :
	m_moveValue(0.0f),
	m_type(BLTYPE::GROUND),
	m_bTouchPL(false)
{
	m_pVtxBuff = nullptr;
	m_pTexture = nullptr;

	Init();
}

//==========================================================
//	デストラクタ
//==========================================================
CBlock::~CBlock()
{

}

//==========================================================
//	初期設定(頂点バッファ生成)
//==========================================================
HRESULT CBlock::Init()
{
	if (FAILED(CObjectX::Init()))	//頂点バッファ作成
	{
		return E_FAIL;
	}

	CObject::SetType(BLOCK);	//オブジェクトにブロックタイプを付与

	m_moveValue = 0.0f;

	m_ModelFile = "data\\model\\cube.x";

	m_scale = { 20.0f, 20.0f, 20.0f };

	//テクスチャ作成
	CTexture* pTex = CManager::GetTexture();	//テクスチャ情報取得
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_BLOCK)));	//設定

	return S_OK;
}

//==========================================================
//	終了処理(頂点バッファの破棄)
//==========================================================
void CBlock::Uninit()
{
	CObjectX::Uninit();
}

//==========================================================
//	更新処理(頂点情報の更新)
//==========================================================
void CBlock::Update()
{
	CObjectX::Update();
}

//==========================================================
//	描画処理(ポリゴン描画)
//==========================================================
void CBlock::Draw()
{
	CObjectX::Draw();
}

//==========================================================
//	CBlock生成
//==========================================================
CBlock* CBlock::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, BLTYPE type, D3DXVECTOR3 scale)
{
	CBlock* pBlock = new CBlock;

	pBlock->Init();
	pBlock->SetPos(pos);
	pBlock->SetRot(rot);
	pBlock->m_type = type;
	pBlock->m_scale = scale;

	return pBlock;
}

//　本来は↓
// これら↓2つは全てマネージャーで呼ぶ

//
//	テクスチャ取得
//
LPDIRECT3DTEXTURE9 CBlock::GetTextures(const BLTYPE& inType)
{
	return m_textures[inType];
}

//
//	ブロックタイプ取得
//
CBlock::BLTYPE CBlock::GetBlockType()
{
	return m_type;
}

//
//	ブロックスケール取得
//
D3DXVECTOR3 CBlock::GetScale()
{
	return m_scale;
}