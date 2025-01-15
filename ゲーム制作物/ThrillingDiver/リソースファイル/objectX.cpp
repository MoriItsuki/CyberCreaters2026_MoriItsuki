//
//	�I�u�W�F�N�gXCPP
//
//
//

//
//	�C���N���[�h
//
#include "main.h"
#include "objectX.h"
#include "manager.h"
//
//	�ÓI�����o�֐�����
//

//
//	�R���X�g���N�^
//
CObjectX::CObjectX(int nPriority) :CObject(nPriority)
{
	m_pVtxBuff = nullptr;
	m_pTexture = nullptr;
	Init();
}

//
//	�f�X�g���N�^
//
CObjectX::~CObjectX()
{

}

//
//	�����ݒ�
//
HRESULT CObjectX::Init()
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(m_ModelFile,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);

	return S_OK;
}

//
//	�I������
//
void CObjectX::Uninit()
{
	//���b�V���̔j��
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}
	//�}�e���A���̔j��
	if (m_pBuffMat != nullptr)
	{
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}
}

//
//	�X�V����
//
void CObjectX::Update()
{

}

//
//	�`�揈��
//
void CObjectX::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
 	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;	//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;		//�}�e���A���f�[�^�̃|�C���^
	D3DXMATRIX mtxParent;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�傫���𔽉f
	D3DXMatrixScaling(&mtxScale,
		m_scale.x,
		m_scale.y,
		m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y,
		m_rot.x,
		m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x,
		m_pos.y,
		m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pTexture);

		//�v���C���[(�p�[�c)�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}
	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//==========================================================
//	ObjectX����
//==========================================================
CObjectX* CObjectX::Create()
{
	CObjectX* pCObjectX = new CObjectX;

	pCObjectX->Init();

	return pCObjectX;
}

//==========================================================
//	�e�N�X�`���ݒ�
//==========================================================
void CObjectX::BindTexture(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTexture = pTex;
}
