//===================================================================
//  デバッグ用表示CPP
//
//
//===================================================================

//  インクルード
#include "DebugDraw.h"
#include <tuple>

//
//  コンストラクタ
//
DebugDraw::DebugDraw(LPDIRECT3DDEVICE9 pDevice) :
    m_pDevice(pDevice), 
    m_pSphereMesh(nullptr)
{
    // 必要ならコンストラクタで初期化
}

//
//  デストラクタ
//
DebugDraw::~DebugDraw()
{
    ReleaseSphereMesh();
}

//
//  初期化
//
void DebugDraw::InitSphereMesh(float radius, UINT slices, UINT stacks)
{
    if (!m_pSphereMesh)
    {
        if (FAILED(D3DXCreateSphere(m_pDevice, radius, slices, stacks, &m_pSphereMesh, nullptr)))
        {
            // エラーハンドリング
            return;
        }
    }
}

//
//  描画処理
//
void DebugDraw::DrawSphere(const D3DXVECTOR3& position, float radius, D3DCOLOR color)
{
    InitSphereMesh(radius);
    
    if (!m_pSphereMesh)
    {
        //初期化失敗
        return;
    }

    D3DXMATRIX mtxWorld;
    D3DXMatrixTranslation(&mtxWorld, position.x, position.y, position.z);
    m_pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

    m_pDevice->SetRenderState(D3DRS_AMBIENT, color);

    D3DMATERIAL9 material = {};
    material.Diffuse.r = ((color >> 16) & 0xFF) / 255.0f;
    material.Diffuse.g = ((color >> 8) & 0xFF) / 255.0f;
    material.Diffuse.b = (color & 0xFF) / 255.0f;
    material.Diffuse.a = ((color >> 24) & 0xFF) / 255.0f;
    m_pDevice->SetMaterial(&material);

    m_pSphereMesh->DrawSubset(0);
}

//
//  解放処理
//
void DebugDraw::ReleaseSphereMesh()
{
    if (m_pSphereMesh)
    {
        m_pSphereMesh->Release();
        m_pSphereMesh = nullptr;
    }
}

//
//
//
void DebugDraw::Render()
{
    //全体でライティングを無効化
    m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    // スフィアを描画
    for (const auto& sphere : m_spheres)
    {
        D3DXVECTOR3 position;
        float radius;
        D3DCOLOR color;
        std::tie(position, radius, color) = sphere;

        D3DXMATRIX matWorld;
        D3DXMatrixTranslation(&matWorld, position.x, position.y, position.z);
        m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);

        // マテリアルでカラー設定
        D3DMATERIAL9 material = {};
        material.Diffuse.r = ((color >> 16) & 0xFF) / 255.0f;
        material.Diffuse.g = ((color >> 8) & 0xFF) / 255.0f;
        material.Diffuse.b = (color & 0xFF) / 255.0f;
        material.Diffuse.a = ((color >> 24) & 0xFF) / 255.0f;
        m_pDevice->SetMaterial(&material);

        m_pSphereMesh->DrawSubset(0);
    }

    // 描画後はスフィアリストをクリアするか、必要に応じて保持
    m_spheres.clear();
}