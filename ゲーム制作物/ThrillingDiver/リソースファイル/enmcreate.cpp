//
//	敵排出生成施設CPP
//
//
//

//
//	インクルード
//
#include "enmcreate.h"
#include "enemy.h"
#include <random>

//
//	定数
//
namespace
{
	constexpr float COOLTIME = 20.0f;	//敵を生成クールタイム
	constexpr int REFERENCEREMAIN = 4;	//生成時の参照敵総数
}

//
//	静的メンバ変数初期化
//
int CEnmCreManager::m_EnmCreCnt = 0;	//	敵生成施設総数
int CEnmCreManager::m_EnmCnt = 0;
LPD3DXFONT CEnmCreManager::m_pFont = nullptr;	

//
//	コンストラクタ
//
CEnmCreManager::CEnmCreManager()
{

}

//
//	デストラクタ
//
CEnmCreManager::~CEnmCreManager()
{
	Uninit();
}

//
//	初期化
//
HRESULT CEnmCreManager::Init()
{
	// フォント設定の構造体を初期化
	D3DXFONT_DESC fontDesc = {};
	fontDesc.Height = 24;                           //	フォントサイズ
	fontDesc.Width = 0;								//	フォントの幅(0->自動調整)
	fontDesc.Weight = FW_BOLD;						//	フォントの太さ
	fontDesc.MipLevels = 1;							//	ミップマップレベル
	fontDesc.Italic = false;						//	イタリック体
	fontDesc.CharSet = DEFAULT_CHARSET;				//	文字セット
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;	//	出力精度
	fontDesc.Quality = DEFAULT_QUALITY;				//	描画品質
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;	//ピッチとファミリ
	strcpy_s(fontDesc.FaceName, "Arial");           // フォントの種類

	// フォントの作成
	if (FAILED(D3DXCreateFontIndirect(CManager::GetRenderer()->GetDevice(), &fontDesc, &m_pFont)))
	{
		return E_FAIL;
	}
	
	return S_OK;
}

//
//	終了処理
//
void CEnmCreManager::Uninit()
{
	if (m_pFont)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
}

//
//	更新処理
//
void CEnmCreManager::Update()
{
	m_EnmCnt = CEnmManager::GetEnmCnt();
}

//
//	描画処理
//
void CEnmCreManager::Draw()
{
	
}

//
//	情報表記
//
void CEnmCreManager::DrawEnmCreInfo()
{
	if (!m_pFont) return;

	RECT rect = { 0,0,CProcess::SCREEN_WIDTH,CProcess::SCREEN_HEIGHT };
	char aStr[256];

	snprintf(&aStr[0],
		256,
		"施設総数:%d\n取得敵総数:%d",
		m_EnmCreCnt,	//敵生成施設総数
		m_EnmCnt		//取得敵総数
	);

	m_pFont->DrawText(nullptr,
		&aStr[0],
		-1,
		&rect,
		DT_LEFT | DT_BOTTOM,
		D3DCOLOR_RGBA(255, 0, 0, 255));
}

//
//	コンストラクタ
//
CEnmCreate::CEnmCreate() :
	m_nRemainCreCnt(0),
	m_fCreCoolTime(0.0f)
{
	Init();

	//	生成時に総数加算
	CEnmCreManager::IncEnmCnt();
}

//
//	デストラクタ
//
CEnmCreate::~CEnmCreate()
{
	//	削除時に総数減算
	CEnmCreManager::DecEnmCnt();
}

//
//	初期化
//
HRESULT CEnmCreate::Init()
{
	//CEnmCreManagerの初期化
	static bool isManagerInitialized = false;
	if (!isManagerInitialized)
	{
		if (FAILED(CEnmCreManager::Init()))
		{
			return E_FAIL;
		}
	}

	//CObjectXの初期化
	if (FAILED(CObjectX::Init()))	//頂点バッファ作成
	{
		return E_FAIL;
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

	m_ModelFile = "data\\model\\gate.x";

	return S_OK;
}

//
//	終了処理	
//
void CEnmCreate::Uninit()
{
	CObjectX::Uninit();

	CEnmCreManager::Uninit();
}

//
//	更新処理
//
void CEnmCreate::Update()
{
	CProcess* pProcess = CManager::GetProcess();

	CObjectX::Update();

	CEnmCreManager::Update();

	//生成敵サイズ
	D3DXVECTOR3
		E_ = { 5.0f,5.0f,5.0f }
	;

	auto
		CW = CEnmManager::m_targetLists.find("時計回り"),		//	"時計回り"	で検索
		unCW = CEnmManager::m_targetLists.find("反時計回り"),	//	"反時計回り"で検索
		Nomove = CEnmManager::m_targetLists.find("動きなし")	//	"動きなし"	で検索
		;

	if (CW == CEnmManager::m_targetLists.end())
	{ // 存在しない
		assert(false);
	}
	else if (unCW == CEnmManager::m_targetLists.end())
	{
		assert(false);
	}
	else if (Nomove == CEnmManager::m_targetLists.end())
	{
		assert(false);
	}
	
	//	デルタタイムを取得
	m_fCreCoolTime += pProcess->GetDeltaTime();
	//	20秒経つか敵が4以下になったら
	if (m_fCreCoolTime >= COOLTIME || CEnmCreManager::GetEnmCnt() <= REFERENCEREMAIN)
	{
		//	生成可能だったら
		if (m_nRemainCreCnt > 0)
		{
			vector<string> filteredNames;
			for (const auto& name : CEnmManager::m_targetListnameLists)
			{
				//除外したい文字列はスキップ
				if (name.find("動きなし") != string::npos)
				{
					continue;
				}
				filteredNames.push_back(name);
			}

			//フィルタリング後のリストが空ではないことを確認する
			if (filteredNames.empty())
			{
				OutputDebugString("ランダムの選択できる名前がない\n");
				return;
			}

			//	ターゲット地点リストからランダムで動きを選択
			//	(止まる→しない)
			size_t size = filteredNames.size();
			random_device rd;
			mt19937 gen(rd());
			uniform_int_distribution<> dist(0, size - 1);
			int rand = dist(gen);

			//ランダム値を使用してイテレータを取得
			string selectedName = filteredNames[rand];

			//選んだ名前に対応する巡回リストを取得
			auto it = CEnmManager::m_targetLists.find(selectedName);
			if (it != CEnmManager::m_targetLists.end())
			{
				//	ランダム値を使って巡回リストを決定し生成
				//						ちょっと上に生成
				CEnemy::Create(m_pos + vec3(0.0f, 200.0f, 0.0f), E_, it->second);
				--m_nRemainCreCnt;	//生成できる数を減らす
			}
			else
			{
				OutputDebugString("この名前の巡回リストはない\n");
			}

			//生成後クールタイムリセット
			m_fCreCoolTime = 0.0f;
		}
	}
}

//
//	描画処理
//
void CEnmCreate::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxNormal;	//計算用マトリックス
	D3DMATERIAL9 matDef;	//現在のマテリアル保存用
	D3DXMATERIAL* pMat;		//マテリアルデータのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//大きさを反映
	D3DXMatrixScaling(&mtxScale,
		m_scale.x,
		m_scale.y,
		m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y,
		m_rot.x,
		m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x,
		m_pos.y,
		m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスをデバイスに設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_pTexture);

		//プレイヤー(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);

#ifdef _DEBUG
	CEnmCreManager::DrawEnmCreInfo();
#endif // _DEBUG
}

//
//	施設生成
//
CEnmCreate* CEnmCreate::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int remCre)
{
	CEnmCreate* pEnmCre = new CEnmCreate();
	pEnmCre->Init();
	pEnmCre->SetPos(pos);
	pEnmCre->SetRot(rot);
	pEnmCre->m_scale = scale;
	pEnmCre->m_nRemainCreCnt = remCre;

	return pEnmCre;
}
