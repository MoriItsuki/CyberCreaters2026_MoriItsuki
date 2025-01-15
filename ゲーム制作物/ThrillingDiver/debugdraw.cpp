//===================================================================
//  �f�o�b�O�p�\��CPP
//
//
//===================================================================

//  �C���N���[�h
#include "DebugDraw.h"
#include <tuple>

//
//  �R���X�g���N�^
//
DebugDraw::DebugDraw(LPDIRECT3DDEVICE9 pDevice) :
    m_pDevice(pDevice), 
    m_pSphereMesh(nullptr)
{
    // �K�v�Ȃ�R���X�g���N�^�ŏ�����
}

//
//  �f�X�g���N�^
//
DebugDraw::~DebugDraw()
{
    ReleaseSphereMesh();
}

//
//  ������
//
void DebugDraw::InitSphereMesh(float radius, UINT slices, UINT stacks)
{
    if (!m_pSphereMesh)
    {
        if (FAILED(D3DXCreateSphere(m_pDevice, radius, slices, stacks, &m_pSphereMesh, nullptr)))
        {
            // �G���[�n���h�����O
            return;
        }
    }
}

//
//  �`�揈��
//
void DebugDraw::DrawSphere(const D3DXVECTOR3& position, float radius, D3DCOLOR color)
{
    InitSphereMesh(radius);
    
    if (!m_pSphereMesh)
    {
        //���������s
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
//  �������
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
    //�S�̂Ń��C�e�B���O�𖳌���
    m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    // �X�t�B�A��`��
    for (const auto& sphere : m_spheres)
    {
        D3DXVECTOR3 position;
        float radius;
        D3DCOLOR color;
        std::tie(position, radius, color) = sphere;

        D3DXMATRIX matWorld;
        D3DXMatrixTranslation(&matWorld, position.x, position.y, position.z);
        m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);

        // �}�e���A���ŃJ���[�ݒ�
        D3DMATERIAL9 material = {};
        material.Diffuse.r = ((color >> 16) & 0xFF) / 255.0f;
        material.Diffuse.g = ((color >> 8) & 0xFF) / 255.0f;
        material.Diffuse.b = (color & 0xFF) / 255.0f;
        material.Diffuse.a = ((color >> 24) & 0xFF) / 255.0f;
        m_pDevice->SetMaterial(&material);

        m_pSphereMesh->DrawSubset(0);
    }

    // �`���̓X�t�B�A���X�g���N���A���邩�A�K�v�ɉ����ĕێ�
    m_spheres.clear();
}