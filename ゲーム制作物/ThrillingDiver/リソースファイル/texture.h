//
//	�e�N�X�`���Ǘ��w�b�_
//
//
//
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//	�C���N���[�h
#include "main.h"
#include "manager.h"
#include <string>

//
//	�}�N����`
// 
#define MAX_TEXTURE		(128)	//�e�N�X�`���̍ő吔
#define MAX_CHAR		(128)	//�p�X�̍ő啶����

//	�e�N�X�`���Ǘ��N���X
class CTexture
{
public:
	CTexture();		//�R���X�g���N�^
	~CTexture();	//�f�X�g���N�^
	void Unload();	//�e�N�X�`���j��
	void Init();	//�e�N�X�`���Ǎ�	�K���K�v�ȃe�N�X�`����ǂݍ���ł���
	int Regist(std::string pTex);	//�e�N�X�`���o�^
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);	//�e�N�X�`���擾
private:
	LPDIRECT3DTEXTURE9 m_aTexture[MAX_TEXTURE];	//�e�N�X�`���̔z��
	std::string m_TexName[MAX_TEXTURE];			//pTextureName�̕ۊǔz��
	static int m_nNumTexAll;					//�e�N�X�`���̑���
};

#endif // !_TEXTURE_H_
