//==========================================================================
//	���C�gCPP(light.cpp)
//
//
//==========================================================================

//===========================================
//	�C���N���[�h
//===========================================
#include "light.h"
#include "manager.h"

//===========================================
//	�R���X�g���N�^
//===========================================
CLight::CLight()
{
	for (int nCntlight = 0; nCntlight < MAX_LIGHT; nCntlight++)
	{
		//���C�g�̏����N���A����
		ZeroMemory(&m_aLight[nCntlight], sizeof(D3DLIGHT9));
	}
}

//===========================================
//	�f�X�g���N�^
//===========================================
CLight::~CLight()
{

}

//===========================================
//	�����ݒ�
//===========================================
HRESULT CLight::Init()
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();
	D3DXVECTOR3 vecDir[MAX_LIGHT];		//�ݒ�p�����x�N�g��

	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		//���C�g�̎�ނ̐ݒ�
		m_aLight[nCnt].Type = D3DLIGHT_DIRECTIONAL;

		//���C�g�̊g�U��
		m_aLight[nCnt].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		switch (nCnt)
		{
		case 0:
			vecDir[nCnt] = D3DXVECTOR3(0.4f, -0.8f, 0.2f);
			break;
		case 1:
			vecDir[nCnt] = D3DXVECTOR3(-0.4f, -0.8f, -0.2f);
			break;
		case 2:
			vecDir[nCnt] = D3DXVECTOR3(0.2f, -0.8f, 0.2f);
			break;
		case 3:
			vecDir[nCnt] = D3DXVECTOR3(-0.2f, -0.8f, -0.2f);
			break;
		default:
			//vecDir[nCnt] = D3DXVECTOR3(0.2f, 0.3f, 0.4f);
			break;
		}
		//���C�g�̕������w��

		//�x�N�g���𐳋K��
		D3DXVec3Normalize(&vecDir[nCnt], &vecDir[nCnt]);
		m_aLight[nCnt].Direction = vecDir[nCnt];

		//���C�g��ݒ�
		pDevice->SetLight(nCnt, &m_aLight[nCnt]);

		//���C�g�̗L����
		pDevice->LightEnable(nCnt, TRUE);
	}
	return S_OK;
}

//===========================================
//	�I������
//===========================================
void CLight::Uninit()
{

}

//===========================================
//	�X�V����
//===========================================
void CLight::Update()
{

}