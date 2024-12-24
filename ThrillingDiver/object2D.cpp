//============================================================================
//	�I�u�W�F�N�g2DCPP(object2D.cpp)
//
//
//============================================================================
//==========================================================
//�C���N���[�h
//==========================================================
#include "main.h"
#include "object2D.h"
#include "manager.h"

//==========================================================
//	�}�N����`
//==========================================================

//==========================================================
//	�O���[�o���ϐ��錾
//==========================================================

//==========================================================
//	�R���X�g���N�^
//==========================================================
CObject2D::CObject2D(int nPriority) :
	m_fAngle(0.0f),
	m_fLength(0.0f),
	CObject(nPriority),
	m_pVtxBuff(nullptr),
	m_pTexture(nullptr),
	m_color(D3DCOLOR_RGBA(255, 255, 255, 255))
{
	//Init();
}

//==========================================================
//	�f�X�g���N�^
//==========================================================
CObject2D::~CObject2D()
{

}

//==========================================================
//	�����ݒ�(���_�o�b�t�@����)
//==========================================================
HRESULT CObject2D::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X�̃|�C���^

	//���_�o�b�t�@�̐���
	HRESULT hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * CManager::GetProcess()->MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		CManager::GetProcess()->FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,	
		NULL);

	m_pos = D3DXVECTOR3(CProcess::SCREEN_WIDTH * 0.5, CProcess::SCREEN_HEIGHT * 0.5, 0.0f);

	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	VERTEX_2D* pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//rhw�̐ݒ�(�Œ�)
	//���_�J���[�̐ݒ�
	for (int nCnt = 0; nCnt < CProcess::MAX_VERTEX; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
		pVtx[nCnt].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==========================================================
//	�I������(���_�o�b�t�@�̔j��)
//==========================================================
void CObject2D::Uninit()
{
	CObject::Release();
}

//==========================================================
//	�X�V����(���_���̍X�V)
//==========================================================
void CObject2D::Update()
{
	VERTEX_2D* pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�Ίp���̒������Z�o
	//m_fLength = sqrtf(MACHINESIZE * MACHINESIZE + MACHINESIZE * MACHINESIZE) * 0.5f;
	//�Ίp���̊p�x���Z�o
	//m_fAngle = atan2f(MACHINESIZE, MACHINESIZE);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	////���_���W�̐ݒ�
	//pVtx[0].pos.x = m_pos.x + sinf(m_rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
	//pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
	//pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	//pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	//pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * m_fLength;
	//pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * m_fLength;
	//pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * m_fLength;
	//pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * m_fLength;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	//pVtx[0].tex = D3DXVECTOR2((nCntAnim - 1) * KOMAWIDTH, 0.0f);
	//pVtx[1].tex = D3DXVECTOR2(nCntAnim * KOMAWIDTH, 0.0f);
	//pVtx[2].tex = D3DXVECTOR2((nCntAnim - 1) * KOMAWIDTH, 1.0f);
	//pVtx[3].tex = D3DXVECTOR2(nCntAnim * KOMAWIDTH, 1.0f);

	//rhw�̐ݒ�(�Œ�)
	//���_�J���[�̐ݒ�
	for (int nCnt = 0; nCnt < CProcess::MAX_VERTEX; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
		pVtx[nCnt].col = m_color;
	}

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//==========================================================
//	�`�揈��(�|���S���`��)
//==========================================================
void CObject2D::Draw()
{
	CRenderer* pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pRendererDevice = pRenderer->GetDevice();	//�f�o�C�X�̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pRendererDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pRendererDevice->SetFVF(CProcess::FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�(nullptr�Ȃ疳����)
	pRendererDevice->SetTexture(0, m_pTexture);

	pRendererDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		2);
}

//==========================================================
//	Object2D����
//==========================================================
CObject2D* CObject2D::Create()
{
	CObject2D* pCobject2D = new CObject2D;

	pCobject2D->Init();

	return pCobject2D;
}

//==========================================================
//	�e�N�X�`���ݒ�
//==========================================================
void CObject2D::BindTexture(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTexture = pTex;
}

//==========================================================
//	�T�C�Y�Z�b�g
//==========================================================
void CObject2D::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

//==========================================================
//	�T�C�Y�Z�b�g
//==========================================================
void CObject2D::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==========================================================
//	�T�C�Y�Z�b�g
//==========================================================
void CObject2D::SetColor(const D3DCOLOR color)
{
	m_color = color;
}
