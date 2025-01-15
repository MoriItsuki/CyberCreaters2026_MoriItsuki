//=========================================================================================================================
//	ダッシュゲージ表示CPP(dashgauge.cpp)
//
//
//=========================================================================================================================

//========================================================
//	インクルード
//========================================================
#include "dashgauge.h"
#include "player.h"
#include <vector>
#include <cmath>

using namespace std;

//========================================================
//	定数
//========================================================
namespace
{
	using vec3 = D3DXVECTOR3;
	using vec2 = D3DXVECTOR2;
	constexpr float SIZE_X = 50.0f;
	constexpr float SIZE_Y = 50.0f;
	constexpr int SEGMENT = 60;		//	円形部分の角数
	constexpr float ARCRADIUS = 100.0f;	//円形部分の半径
}

//========================================================
//	静的メンバ変数初期化
//========================================================
int CDashManager::m_DashCnt = 0;	//ダッシュ可能数
LPD3DXFONT CDashManager::m_pFont = nullptr;

//================================================================================================================
//	CDashManager
//================================================================================================================

//========================================================
//	コンストラクタ
//========================================================
CDashManager::CDashManager()
{

}

//========================================================
//	デストラクタ
//========================================================
CDashManager::~CDashManager()
{

}

//========================================================
//	初期化
//========================================================
HRESULT CDashManager::Init()
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
	strcpy_s(fontDesc.FaceName, "Arial");			// フォントの種類

	// フォントの作成
	if (FAILED(D3DXCreateFontIndirect(CManager::GetRenderer()->GetDevice(), &fontDesc, &m_pFont)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//========================================================
//	終了
//========================================================
void CDashManager::Uninit()
{
	if (m_pFont)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
}

//========================================================
//	更新
//========================================================
void CDashManager::Update()
{
	//オブジェクトとの当たり判定
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		//オブジェクト取得
		CObject* pObj = CObject::GetObj(3, nCntObj);

		if (pObj)
		{//	使われていたら
			//	オブジェクトのタイプを取得
			CObject::TYPE type = pObj->GetType();

			if (type == CObject::TYPE::PLAYER)
			{
				//プレイヤー型にキャスト
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);
				//プレイヤー定義確認
				if (!pPlayer) return;

				m_DashCnt = pPlayer->GetDashCnt();
			}
		}
	}

}

//========================================================
//	描画
//========================================================
void CDashManager::Draw()
{

}

//========================================================
//	ダッシュ情報表示
//========================================================
void CDashManager::DrawDashInfo()
{
	if (!m_pFont) return;

	RECT rect = { 0,0,CProcess::SCREEN_WIDTH,CProcess::SCREEN_HEIGHT };
	char aStr[256];

	snprintf(&aStr[0],
		256,
		"ダッシュ可能数:%d",
		m_DashCnt	//敵生成施設総数
	);

	m_pFont->DrawText(nullptr,
		&aStr[0],
		-1,
		&rect,
		DT_CENTER | DT_BOTTOM,
		D3DCOLOR_RGBA(255, 0, 0, 255));
}

//================================================================================================================
//	CDashgauge(各ゲージの管理や制御)
//================================================================================================================

//========================================================
//	コンストラクタ
//========================================================
CDashgauge::CDashgauge() :
	m_pArcPart(nullptr),
	m_pBarPart(nullptr),
	m_pFramePart(nullptr)
{

}

//========================================================
//	デストラクタ
//========================================================
CDashgauge::~CDashgauge()
{
	Uninit();
}

//========================================================
//	初期化
//========================================================
HRESULT CDashgauge::Init()
{
	//CDashManagerの初期化
	static bool isManagerInitialized = false;
	if (!isManagerInitialized)
	{
		if (FAILED(CDashManager::Init()))
		{
			return E_FAIL;
		}
	}

	// 各ゲージ部品を初期化
	try
	{
		m_pArcPart = make_unique<CArcPart>(
			ARCRADIUS,							//	円の半径
			270.0f,							//	円最大角度(値がMAXのときの角度)
			D3DCOLOR_XRGB(255, 0, 0),		//	始まりの色
			D3DCOLOR_XRGB(255, 255, 0),		//	終わりの色
			vec3(ARCRADIUS + 20.0f, CProcess::SCREEN_HEIGHT - (ARCRADIUS + 20.0f), 0.0f)		//	画面上の位置(ｚは"0.0"固定)
			);

		if (FAILED(m_pArcPart->Init()))
		{
			throw runtime_error("CArcPart initialization failed");
		}
		//m_pBarPart = make_unique<CBarPart>(200.0f, 10.0f, D3DCOLOR_XRGB(0, 255, 0), D3DCOLOR_XRGB(0, 0, 255), vec3(150.0f, 150.0f, 0.0f));
		//m_pFramePart = make_unique<CFramePart>(300.0f, 300.0f, vec3(200.0f, 200.0f, 0.0f));
	}
	catch (const exception& e)
	{
		Uninit();
		return E_FAIL;
	}

	return S_OK;
}

//========================================================
//	終了処理
//========================================================
void CDashgauge::Uninit()
{
	m_pArcPart->Uninit();
	//m_pBarPart->Uninit();
	//m_pFramePart->Uninit();
}

//========================================================
//	更新処理
//========================================================
void CDashgauge::Update(float value)
{
	m_pArcPart->SetValue(value);
	m_pArcPart->Update();
	//m_pBarPart->SetValue(value);
	//m_pBarPart->Update();

	CDashManager::Update();
}

//========================================================
//	描画処理
//========================================================
void CDashgauge::Draw(LPDIRECT3DDEVICE9 device)
{
	m_pArcPart->Draw(device);
	//m_pBarPart->Draw(device);
	//m_pFramePart->Draw(device);

#ifdef _DEBUG
	CDashManager::DrawDashInfo();
#endif // _DEBUG
}

//========================================================
//	円形部分情報取得
//========================================================
CArcPart* CDashgauge::GetArcGauge() const
{
	return m_pArcPart.get();
}

//========================================================
//	直線部分情報取得
//========================================================
CBarPart* CDashgauge::GetBarGauge() const
{
	return m_pBarPart.get();
}

//========================================================
//	外側部分情報取得
//========================================================
CFramePart* CDashgauge::GetFrameGauge() const
{
	return m_pFramePart.get();
}

//========================================================
//	生成処理
//========================================================
CDashgauge* CDashgauge::Create()
{
	CDashgauge* pGauge = new CDashgauge();
	pGauge->Init();

	return pGauge;
}

//================================================================================================================
//	ダッシュゲージ親クラス
//================================================================================================================

//========================================================
//	コンストラクタ
//========================================================
CGaugePart::CGaugePart() :
	m_value(1.0f)
{

}

//========================================================
//	デストラクタ
//========================================================
CGaugePart::~CGaugePart()
{

}

//========================================================
//	ゲージ値を設定
//========================================================
void CGaugePart::SetValue(float value)
{
	m_value = clamp(value, 0.0f, 1.0f);
}

//========================================================
//	ゲージ値を取得
//========================================================
float CGaugePart::GetValue() const
{
	return m_value;
}

//================================================================================================================
//	ダッシュゲージ円形部分
//================================================================================================================

//========================================================
//	コンストラクタ
//========================================================
CArcPart::CArcPart(float radius, float maxAngle, D3DCOLOR startColor, D3DCOLOR endColor, const D3DXVECTOR3& pos) :
	m_radius(radius),
	m_maxAngle(maxAngle),
	m_colorStart(startColor == 0 ? D3DCOLOR_XRGB(255, 255, 255) : startColor), // デフォルト白
	m_colorEnd(endColor == 0 ? D3DCOLOR_XRGB(255, 255, 255) : endColor),       // デフォルト白	
	m_centerPos(pos),
	m_vertexCnt(0)
{
	m_value = 1.0f;
	m_vertexBuffer = nullptr;
}

//========================================================
//	デストラクタ
//========================================================
CArcPart::~CArcPart()
{
	Uninit();
}

//========================================================
//	初期化
//========================================================
HRESULT CArcPart::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	UINT vertexSize = static_cast<UINT>(sizeof(VERTEX) * (SEGMENT + 2));	//中心点+セグメント分

	HRESULT hr =
		pDevice->CreateVertexBuffer
		(
			vertexSize,
			0,
			D3DFVF_XYZ | D3DFVF_DIFFUSE,
			D3DPOOL_MANAGED,
			&m_vertexBuffer,
			nullptr
		);
	
	if (FAILED(hr))
	{
		OutputDebugString("Failed to create vertex buffer in CArcPart::Init.\n");
		return hr;
	}

	//初期状態設定
	m_value = 1.0f;	//最大値
	Update();	//初期値に基づいて頂点情報を更新

	return S_OK;
}

//========================================================
//	終了
//========================================================
void CArcPart::Uninit()
{
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}
}

//========================================================
//	更新
//========================================================
void CArcPart::Update()
{
	// ゲージの値を更新
	float currentAngle = D3DXToRadian(m_maxAngle * m_value);
	vector<vec2> vertices2D = GenerateArcVertices(m_radius, currentAngle, SEGMENT);

	vector<VERTEX> vertices;
	D3DCOLOR color = m_colorStart;

	// 中心点
	vertices.push_back({ m_centerPos, 1.0f, color });

	size_t ver2Dsize = vertices2D.size();

	// 外周の頂点
	for (size_t i = 0; i < ver2Dsize; ++i)
	{
		float t = static_cast<float>(i) / ver2Dsize;
		t = clamp(t, 0.0f, 1.0f);
		color = InterpolateColor(m_colorStart, m_colorEnd, t);
		vertices.push_back(
			{
				vec3
				(
					vertices2D[i].x + m_centerPos.x,
					vertices2D[i].y + m_centerPos.y,
					m_centerPos.z
				),
				1.0f,
				color
			});
	}

	m_vertexCnt = vertices.size();
	UINT vertexSize = static_cast<UINT>(m_vertexCnt * sizeof(VERTEX));

	if (m_vertexCnt == 0)
	{
		OutputDebugString("Vertex count is zero in CArcPart::Update.\n");
		return;
	}

	// バッファにデータ書き込み
	VERTEX* pVtx;
	if (FAILED(m_vertexBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	{
		OutputDebugString("Failed to lock vertex buffer in CArcPart::Update.\n");
		return;
	}

	memcpy(pVtx, vertices.data(), vertexSize);
	m_vertexBuffer->Unlock();
}

//========================================================
//	描画
//========================================================
void CArcPart::Draw(LPDIRECT3DDEVICE9 device)
{
	if (!device || !m_vertexBuffer || m_vertexCnt < 3) // 三角形描画には少なくとも3つの頂点が必要
	{
		OutputDebugString("Invalid state in CArcPart::Draw.\n");
		return;
	}

	device->SetStreamSource(0, m_vertexBuffer, 0, sizeof(VERTEX));
	device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	HRESULT hr = device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, m_vertexCnt - 2);
	if (FAILED(hr))
	{
		OutputDebugString("Failed to draw primitive in CArcPart::Draw.\n");
	}
}

//========================================================
//	ゲージの色設定
//========================================================
void CArcPart::SetColors(D3DCOLOR startColor, D3DCOLOR endColor)
{
	m_colorStart = startColor;
	m_colorEnd = endColor;
}

//========================================================
//	円形ゲージの頂点を生成
//========================================================
vector<D3DXVECTOR2> CArcPart::GenerateArcVertices(float radius, float angle, int segments)const
{
	vector<D3DXVECTOR2> vertices;
	for (int i = 0; i <= segments; ++i)
	{
		float t = static_cast<float>(i) / segments;
		float theta = angle * t;
		vertices.emplace_back(radius * cosf(theta), radius * sinf(theta));
	}
	return vertices;
}

//========================================================
//	色の補間
//========================================================
D3DCOLOR CArcPart::InterpolateColor(D3DCOLOR start, D3DCOLOR end, float t)const
{
	float
		sr = static_cast<float>((start >> 16) & 0xFF),
		sg = static_cast<float>((start >> 8) & 0xFF),
		sb = static_cast<float>(start & 0xFF),
		sa = static_cast<float>((start >> 24) & 0xFF);

	float
		er = static_cast<float>((end >> 16) & 0xFF),
		eg = static_cast<float>((end >> 8) & 0xFF),
		eb = static_cast<float>(end & 0xFF),
		ea = static_cast<float>((end >> 24) & 0xFF);

	BYTE r = static_cast<BYTE>(sr + t * (er - sr));
	BYTE g = static_cast<BYTE>(sg + t * (eg - sg));
	BYTE b = static_cast<BYTE>(sb + t * (eb - sb));
	BYTE a = static_cast<BYTE>(sa + t * (ea - sa));

	return (a << 24) | (r << 16) | (g << 8) | b;
}

//================================================================================================================
//	ダッシュゲージ直線部分
//================================================================================================================

//========================================================
//	コンストラクタ
//========================================================
CBarPart::CBarPart(float length, float thickness, D3DCOLOR startColor, D3DCOLOR endColor, const D3DXVECTOR3& pos) :
	m_length(length),
	m_thickness(thickness),
	m_colorStart(startColor),
	m_colorEnd(endColor),
	m_startPos(pos)
{
	m_value = 0.0f;
	m_vertexBuffer = nullptr;
}

//========================================================
//	デストラクタ
//========================================================
CBarPart::~CBarPart()
{
	Uninit();
}

//========================================================
//	初期化
//========================================================
HRESULT CBarPart::Init()
{
	return S_OK;
}

//========================================================
//	終了
//========================================================
void CBarPart::Uninit()
{
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}
}

//========================================================
//	更新
//========================================================
void CBarPart::Update()
{

}

//========================================================
//	描画
//========================================================
void CBarPart::Draw(LPDIRECT3DDEVICE9 device)
{
	vector<vec2> vertices2D = GenerateBarVertices(m_length * m_value, m_thickness);

	struct VERTEX
	{
		vec3 pos;
		D3DCOLOR color;
	};
	
	D3DCOLOR colorStart = m_colorStart;
	D3DCOLOR colorEnd = m_colorEnd;

	//頂点データを作成
	VERTEX vertices[4] =
	{
		{ vec3(vertices2D[0].x + m_startPos.x, vertices2D[0].y + m_startPos.y, m_startPos.z), colorStart },
		{ vec3(vertices2D[1].x + m_startPos.x, vertices2D[1].y + m_startPos.y, m_startPos.z), colorEnd },
		{ vec3(vertices2D[2].x + m_startPos.x, vertices2D[2].y + m_startPos.y, m_startPos.z), colorStart },
		{ vec3(vertices2D[3].x + m_startPos.x, vertices2D[3].y + m_startPos.y, m_startPos.z), colorEnd },
	};

	//	頂点バッファ作成
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer = nullptr;

	if (FAILED(device->CreateVertexBuffer(
		sizeof(vertices), 
		0, 
		D3DFVF_XYZ | D3DFVF_DIFFUSE, 
		D3DPOOL_MANAGED, 
		&vertexBuffer, 
		nullptr)))
	{
		return;
	}

	//	バッファにデータ書き込み
	VERTEX* pVtx;
	if (SUCCEEDED(vertexBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	{
		memcpy(pVtx, vertices, sizeof(vertices));
		vertexBuffer->Unlock();
	}

	//描画
	device->SetStreamSource(0, vertexBuffer, 0, sizeof(VERTEX));
	device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//バッファ解放
	vertexBuffer->Release();

}

//========================================================
//	ゲージの色設定
//========================================================
void CBarPart::SetColors(D3DCOLOR startColor, D3DCOLOR endColor)
{
	m_colorStart = startColor;
	m_colorEnd = endColor;
}

//========================================================
//	直線ゲージの頂点を生成
//========================================================
std::vector<D3DXVECTOR2> CBarPart::GenerateBarVertices(float length, float thickness) const
{
	float halfThickness = thickness / 2;
	return
	{
		{-length / 2,-halfThickness},{-length / 2,halfThickness},
		{length / 2,halfThickness},{length / 2,-halfThickness}
	};
}

//========================================================
//	色の保管
//========================================================
D3DCOLOR CBarPart::InterpolateColor(D3DCOLOR start, D3DCOLOR end, float t) const
{
	BYTE 
		sr = (start >> 16) & 0xFF, 
		sg = (start >> 8) & 0xFF, 
		sb = start & 0xFF, 
		sa = (start >> 24) & 0xFF;

	BYTE 
		er = (end >> 16) & 0xFF, 
		eg = (end >> 8) & 0xFF, 
		eb = end & 0xFF, 
		ea = (end >> 24) & 0xFF;

	BYTE r = static_cast<BYTE>(sr + t * (er - sr));
	BYTE g = static_cast<BYTE>(sg + t * (eg - sg));
	BYTE b = static_cast<BYTE>(sb + t * (eb - sb));
	BYTE a = static_cast<BYTE>(sa + t * (ea - sa));

	return (a << 24) | (r << 16) | (g << 8) | b;
}

//================================================================================================================
//	ダッシュゲージ外側部分
//================================================================================================================

//========================================================
//	コンストラクタ
//========================================================
CFramePart::CFramePart(float width, float height, const D3DXVECTOR3& pos) :
	m_width(width),
	m_height(height),
	m_centerPos(pos),
	m_texture(nullptr)
{
	m_value = 0.0f;
}

//========================================================
//	デストラクタ
//========================================================
CFramePart::~CFramePart()
{
	Uninit();
}

//========================================================
//	初期化
//========================================================
HRESULT CFramePart::Init()
{
	return S_OK;
}

//========================================================
//	終了処理
//========================================================
void CFramePart::Uninit()
{
	if (m_texture)
	{
		m_texture->Release();
		m_texture = nullptr;
	}
}

//========================================================
//	更新処理
//========================================================
void CFramePart::Update()
{

}

//========================================================
//	描画処理
//========================================================
void CFramePart::Draw(LPDIRECT3DDEVICE9 device)
{
	if (m_texture || !m_texture) return;
	
	device->SetTexture(0, m_texture);

	struct VERTEX
	{
		vec3 pos;
		vec2 tex;
	};

	//四角の頂点データを作成
	float halfWidth = m_width / 2.0f;
	float halfHeight = m_height / 2.0f;

	VERTEX vertices[4] =
	{
		{ vec3(m_centerPos.x - halfWidth, m_centerPos.y - halfHeight, m_centerPos.z), vec2(0.0f, 0.0f) },
		{ vec3(m_centerPos.x + halfWidth, m_centerPos.y - halfHeight, m_centerPos.z), vec2(1.0f, 0.0f) },
		{ vec3(m_centerPos.x - halfWidth, m_centerPos.y + halfHeight, m_centerPos.z), vec2(0.0f, 1.0f) },
		{ vec3(m_centerPos.x + halfWidth, m_centerPos.y + halfHeight, m_centerPos.z), vec2(1.0f, 1.0f) },
	};

	//	頂点バッファ作成
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer = nullptr;

	if (FAILED(device->CreateVertexBuffer(
		sizeof(vertices),
		0,
		D3DFVF_XYZRHW | D3DFVF_TEX1,
		D3DPOOL_MANAGED,
		&vertexBuffer,
		nullptr)))
	{
		return;
	}

	//	バッファにデータ書き込み
	VERTEX* pVtx;
	if (SUCCEEDED(vertexBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	{
		memcpy(pVtx, vertices, sizeof(vertices));
		vertexBuffer->Unlock();
	}

	//描画
	device->SetTexture(0, m_texture);
	device->SetStreamSource(0, vertexBuffer, 0, sizeof(VERTEX));
	device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//	バッファ解放
	vertexBuffer->Release();
}

//========================================================
//	テクスチャ設定
//========================================================
void CFramePart::SetTexture(LPDIRECT3DTEXTURE9 texture)
{
	if (m_texture)
	{
		m_texture->Release();
	}
	m_texture = texture;
	if (m_texture)
	{
		m_texture->AddRef();
	}
}
