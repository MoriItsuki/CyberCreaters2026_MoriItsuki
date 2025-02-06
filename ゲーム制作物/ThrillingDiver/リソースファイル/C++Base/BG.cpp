//======================================================================================
//	�w�iCPP(BG.cpp)
//
//
//======================================================================================

//==========================================================
//	�C���N���[�h
//==========================================================
#include "BG.h"
#include "manager.h"

//==========================================================
//	�ÓI�����o�֐�������
//==========================================================
const std::string CBG::FILE_BG[MAX] = { "data\\texture\\title1.png","data\\texture\\BG.png" ,"data\\texture\\result.png" };

//==========================================================
//	�R���X�g���N�^
//==========================================================
CBG::CBG(int nPriority) :
	m_textureType(TYPE::TITLE), 
	CObject2D(nPriority)
{
	m_PosTexBGV = 0.0f;
	m_PosTexBGU = 0.0f;
	//Init();
}

//==========================================================
//	�f�X�g���N�^
//==========================================================
CBG::~CBG()
{

}

//==========================================================
//	�����ݒ�(���_�o�b�t�@����)
//==========================================================
HRESULT CBG::Init()
{
	if (FAILED(CObject2D::Init()))	//���_�o�b�t�@�쐬
	{
		return E_FAIL;
	}

	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	SetSize(D3DXVECTOR3(CProcess::SCREEN_WIDTH * 0.5f, CProcess::SCREEN_HEIGHT * 0.5f, 0.0f));

	return S_OK;
}

//==========================================================
//	�I������(���_�o�b�t�@�̔j��)
//==========================================================
void CBG::Uninit()
{
	CObject2D::Uninit();
}

//==========================================================
//	�X�V����(���_���̍X�V)
//==========================================================
void CBG::Update()
{
	CObject2D::Update();

	VERTEX_2D* pVtx;	//���_���ւ̃|�C���^

	//m_PosTexBGV += 0.001f;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(m_PosTexBGV, m_PosTexBGU);
	pVtx[1].tex = D3DXVECTOR2(m_PosTexBGV + 1.0f, m_PosTexBGU);
	pVtx[2].tex = D3DXVECTOR2(m_PosTexBGV, m_PosTexBGU + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_PosTexBGV + 1.0f, m_PosTexBGU + 1.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//==========================================================
//	�`�揈��(�|���S���`��)
//==========================================================
void CBG::Draw()
{
	CObject2D::Draw();
}

//==========================================================
//	BG����
//==========================================================
CBG* CBG::Create(TYPE type)
{
	CBG* pCBG = new CBG;

	pCBG->Init();

	CTexture* pTex = CManager::GetTexture();	//�e�N�X�`�����擾

	pCBG->BindTexture(pTex->GetAddress(pTex->Regist(FILE_BG[type])));	//�ݒ�

	return pCBG;
}