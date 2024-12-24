//============================================================================
//	�u���b�NCPP(block.cpp)
//
//
//============================================================================
//==========================================================
//�C���N���[�h
//==========================================================
#include "main.h"
#include "block.h"
#include "manager.h"
#include "texture.h"

//==========================================================
//	�}�N����`
//==========================================================

//==========================================================
//	�O���[�o���ϐ��錾
//==========================================================

//==========================================================
//	�ÓI�����o�֐�������
//==========================================================
const std::string CBlock::FILE_BLOCK = "data\\texture\\ground.png";
LPDIRECT3DTEXTURE9 CBlock::m_textures[BLTYPE::MAX] = {};

//==========================================================
//	�R���X�g���N�^
//==========================================================
CBlock::CBlock() :
	m_moveValue(0.0f),
	m_type(BLTYPE::GROUND),
	m_bTouchPL(false)
{
	m_pVtxBuff = nullptr;
	m_pTexture = nullptr;

	Init();
}

//==========================================================
//	�f�X�g���N�^
//==========================================================
CBlock::~CBlock()
{

}

//==========================================================
//	�����ݒ�(���_�o�b�t�@����)
//==========================================================
HRESULT CBlock::Init()
{
	if (FAILED(CObjectX::Init()))	//���_�o�b�t�@�쐬
	{
		return E_FAIL;
	}

	CObject::SetType(BLOCK);	//�I�u�W�F�N�g�Ƀu���b�N�^�C�v��t�^

	m_moveValue = 0.0f;

	m_ModelFile = "data\\model\\cube.x";

	m_scale = { 20.0f, 20.0f, 20.0f };

	//�e�N�X�`���쐬
	CTexture* pTex = CManager::GetTexture();	//�e�N�X�`�����擾
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_BLOCK)));	//�ݒ�

	return S_OK;
}

//==========================================================
//	�I������(���_�o�b�t�@�̔j��)
//==========================================================
void CBlock::Uninit()
{
	CObjectX::Uninit();
}

//==========================================================
//	�X�V����(���_���̍X�V)
//==========================================================
void CBlock::Update()
{
	CObjectX::Update();
}

//==========================================================
//	�`�揈��(�|���S���`��)
//==========================================================
void CBlock::Draw()
{
	CObjectX::Draw();
}

//==========================================================
//	CBlock����
//==========================================================
CBlock* CBlock::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, BLTYPE type, D3DXVECTOR3 scale)
{
	CBlock* pBlock = new CBlock;

	pBlock->Init();
	pBlock->SetPos(pos);
	pBlock->SetRot(rot);
	pBlock->m_type = type;
	pBlock->m_scale = scale;

	return pBlock;
}

//�@�{���́�
// ����火2�͑S�ă}�l�[�W���[�ŌĂ�

//
//	�e�N�X�`���擾
//
LPDIRECT3DTEXTURE9 CBlock::GetTextures(const BLTYPE& inType)
{
	return m_textures[inType];
}

//
//	�u���b�N�^�C�v�擾
//
CBlock::BLTYPE CBlock::GetBlockType()
{
	return m_type;
}

//
//	�u���b�N�X�P�[���擾
//
D3DXVECTOR3 CBlock::GetScale()
{
	return m_scale;
}