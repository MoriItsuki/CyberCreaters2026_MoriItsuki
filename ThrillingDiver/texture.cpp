//
//	�e�N�X�`���ꊇ�Ǘ�cpp
//
//
//

//
//	�C���N���[�h
//
#include "texture.h"

//
//	�ÓI�����o�֐�������
//
int CTexture::m_nNumTexAll = 0;		//�e�N�X�`����������

//
//	�R���X�g���N�^
//
CTexture::CTexture():
	m_TexName()
{
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		m_aTexture[nCnt] = nullptr;
		m_nNumTexAll = 0;
	}
}

//
//	�f�X�g���N�^
//
CTexture::~CTexture()
{

}

//
//	�e�N�X�`���j��
//
void CTexture::Unload()
{
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		if (m_aTexture[nCnt] != nullptr)
		{
			m_aTexture[nCnt]->Release();
			m_aTexture[nCnt] = nullptr;
		}
	}
	m_nNumTexAll = 0;
}

//
//	�e�N�X�`���Ǎ�
//
void CTexture::Init()
{

}

//
//	�e�N�X�`���o�^
//
int CTexture::Regist(std::string pTexName)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	int nIdx = 0;
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		if (m_aTexture[nCnt] == nullptr)
		{//	�o�^����Ă��Ȃ�
			D3DXCreateTextureFromFile(pDevice,
				pTexName.c_str(),
				&m_aTexture[nCnt]);

			//�V�K�e�N�X�`���̍쐬
			m_TexName[nCnt] = pTexName;
			nIdx = nCnt;	//ID�ݒ�
			m_nNumTexAll++;	//�e�N�X�`�������Z
			break;
		}
		else if (m_TexName[nCnt] == pTexName)
		{//	�����ς݂̃e�N�X�`���ƈ�v
			nIdx = nCnt;
			break;
		}
	}
	return nIdx;	//ID��Ԃ�
}

//
//	�e�N�X�`���擾
//
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	return m_aTexture[nIdx];
}
