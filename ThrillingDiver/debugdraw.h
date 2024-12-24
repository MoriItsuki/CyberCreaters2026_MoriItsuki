//
//	デバッグ用表示ヘッダ
//
//
//
#pragma once

//	インクルード
#include "main.h"
#include <vector>

class DebugDraw
{
public:
	DebugDraw(LPDIRECT3DDEVICE9 pDevice);
	~DebugDraw();

	void DrawSphere(const D3DXVECTOR3& pos, float radius = 1.0f, D3DCOLOR color = D3DCOLOR_ARGB(100, 255, 0, 0));
	void InitSphereMesh(float radius, UINT slices = 20, UINT stacks = 20);
	void ReleaseSphereMesh();
	void Render();
private:
	LPDIRECT3DDEVICE9 m_pDevice;
	LPD3DXMESH m_pSphereMesh;

	//複数のスフィア情報を保持するリスト
	std::vector<std::tuple<D3DXVECTOR3, float, D3DXCOLOR>> m_spheres;
};