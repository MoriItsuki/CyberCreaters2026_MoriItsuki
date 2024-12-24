//
//	�I�u�W�F�N�g�RD
//
//
//

//
//	�C���N���[�h
//
#include "main.h"
#include "object3D.h"
#include "manager.h"

//
//	�R���X�g���N�^
//
CObject3D::CObject3D()
{
	m_pVtxBuff = nullptr;
	m_pTexture = nullptr;
	Init();

}

//
//	�f�X�g���N�^
//
CObject3D::~CObject3D()
{

}

//
//	�����ݒ�
//
HRESULT CObject3D::Init()
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * CProcess::MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		CProcess::FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D* pVtx;	//���_���ւ̃|�C���^

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, 0.0f, +m_size.z);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, 0.0f, +m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, 0.0f, -m_size.z);

	//�@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//
//	�I������
//
void CObject3D::Uninit()
{
	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

}

//==========================================================
//	�X�V����(���_���̍X�V)
//==========================================================
void CObject3D::Update()
{
	VERTEX_3D* pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, 0.0f, +m_size.z);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, 0.0f, +m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, 0.0f, -m_size.z);

	////���_���W�̐ݒ�
	//pVtx[0].pos.x = m_pos.x + sinf(m_rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
	//pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
	//pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	//pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	//pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * m_fLength;
	//pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * m_fLength;
	//pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * m_fLength;
	//pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * m_fLength;

	////�e�N�X�`�����W�̐ݒ�
	//pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	//pVtx[0].tex = D3DXVECTOR2((nCntAnim - 1) * KOMAWIDTH, 0.0f);
	//pVtx[1].tex = D3DXVECTOR2(nCntAnim * KOMAWIDTH, 0.0f);
	//pVtx[2].tex = D3DXVECTOR2((nCntAnim - 1) * KOMAWIDTH, 1.0f);
	//pVtx[3].tex = D3DXVECTOR2(nCntAnim * KOMAWIDTH, 1.0f);

	//���_�J���[�̐ݒ�
	/*for (int nCnt = 0; nCnt < CProcess::MAX_VERTEX; nCnt++)
	{
		pVtx[nCnt].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}*/

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//==========================================================
//	�`�揈��(�|���S���`��)
//==========================================================
void CObject3D::Draw()
{
	CRenderer* pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pRendererDevice = pRenderer->GetDevice();	//�f�o�C�X�̃|�C���^

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y,
		m_rot.x,
		m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X
	pRendererDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pRendererDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pRendererDevice->SetFVF(CProcess::FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pRendererDevice->SetTexture(0, m_pTexture);

	pRendererDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//==========================================================
//	Object3D����
//==========================================================
CObject3D* CObject3D::Create()
{
	CObject3D* pCObject2D = new CObject3D;

	pCObject2D->Init();

	return pCObject2D;
}

//==========================================================
//	�e�N�X�`���ݒ�
//==========================================================
void CObject3D::BindTexture(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTexture = pTex;
}

//==========================================================
//	�T�C�Y�ύX
//==========================================================
void CObject3D::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}
