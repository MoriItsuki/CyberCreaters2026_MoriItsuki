//
//	ダッシュゲージヘッダ(dashgauge.h)
//
//
//
#pragma once

//
//	インクルード
//
#include "texture.h"
#include "object2D.h"

//ダッシュ管理クラス
class CDashManager
{
private:
	static int m_DashCnt;	//ダッシュできる回数
	static LPD3DXFONT m_pFont;	//フォント設定
public:
	CDashManager();
	~CDashManager();
	static void IncDashCnt() { ++m_DashCnt; }	//ダッシュ可能回数加算
	static void DecDashCnt() { --m_DashCnt; }	//ダッシュ可能回数減算
	static int GetDashCnt() { return m_DashCnt; }	//ダッシュできる回数取得
	static HRESULT Init();
	static void Uninit();
	static void Update();
	static void Draw();
	static void DrawDashInfo();
};

//ダッシュゲージスーパークラス
class CGaugePart
{
public:
	CGaugePart();
	virtual ~CGaugePart();

	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw(LPDIRECT3DDEVICE9 device) = 0;
	//ゲージ値の設定/取得
	virtual void SetValue(float value);
	virtual float GetValue()const;
protected:
	float m_value;	//部品のゲージ値
};

//円形部分クラス
class CArcPart :public CGaugePart
{
public:
	//頂点データを作成
	struct VERTEX
	{
		D3DXVECTOR3 pos;
		float rhw;
		D3DCOLOR color;
	};
	CArcPart(float radius, float maxAngle, D3DCOLOR startColor, D3DCOLOR endColor, const D3DXVECTOR3& pos);
	~CArcPart()override;

	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw(LPDIRECT3DDEVICE9 device)override;
	//	ゲージ色の設定
	void SetColors(D3DCOLOR startColor, D3DCOLOR endColor);
private:
	D3DXVECTOR3 m_centerPos;//円形部分の中心点
	float m_maxAngle;		//円形部分の最大角度
	float m_radius;			//円形ゲージの半径
	D3DCOLOR m_colorStart;	//グラデーション開始色
	D3DCOLOR m_colorEnd;	//グラデーション終了色

	size_t m_vertexCnt;	//描画する頂点数

	LPDIRECT3DVERTEXBUFFER9 m_vertexBuffer;	//頂点バッファ

	std::vector<D3DXVECTOR2> GenerateArcVertices(float radius, float angle, int segments) const; // 円形頂点生成
	D3DCOLOR InterpolateColor(D3DCOLOR start, D3DCOLOR end, float t) const;  // グラデーション補間
};

//直線部分クラス
class CBarPart :public CGaugePart
{
public:
	CBarPart(float length, float thickness, D3DCOLOR startColor, D3DCOLOR endColor, const D3DXVECTOR3& pos);
	~CBarPart()override;

	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw(LPDIRECT3DDEVICE9 device)override;
	//ゲージ色の設定
	void SetColors(D3DCOLOR startColor, D3DCOLOR endColor);
private:
	D3DXVECTOR3 m_startPos;	//直線部分の開始位置
	float m_length;
	float m_thickness;
	D3DCOLOR m_colorStart;	//グラデーション開始色
	D3DCOLOR m_colorEnd;	//グラデーション終了色

	LPDIRECT3DVERTEXBUFFER9 m_vertexBuffer;	//頂点バッファ

	std::vector<D3DXVECTOR2> GenerateBarVertices(float length, float thickness) const; // 直線部分頂点生成
	D3DCOLOR InterpolateColor(D3DCOLOR start, D3DCOLOR end, float t)const;	//	グラデーション補間
};

//外側部分クラス
class CFramePart :public CGaugePart
{
public:
	CFramePart(float width, float height, const D3DXVECTOR3& pos);
	~CFramePart()override;

	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw(LPDIRECT3DDEVICE9 device)override;
	//テクスチャの設定
	void SetTexture(LPDIRECT3DTEXTURE9 texture);
private:
	LPDIRECT3DTEXTURE9 m_texture;
	D3DXVECTOR3 m_centerPos;	//フレームの中心位置
	float m_width;				//フレームの幅
	float m_height;				//フレームの高さ
};

//ダッシュゲージ管理クラス
class CDashgauge
{
public:
	CDashgauge();
	~CDashgauge();

	HRESULT Init();
	void Uninit();
	void Update(float value);
	void Draw(LPDIRECT3DDEVICE9 device);
	CArcPart* GetArcGauge()const;
	CBarPart* GetBarGauge()const;
	CFramePart* GetFrameGauge()const;

	static CDashgauge* Create();
private:
	std::unique_ptr<CArcPart> m_pArcPart;
	std::unique_ptr<CBarPart> m_pBarPart;
	std::unique_ptr<CFramePart> m_pFramePart;
};
