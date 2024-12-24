//
//	���C�g�w�b�_(light.h)
//
//
//
#ifndef _LIGHT_H_
#define _LIGHT_H_

//�C���N���[�h
#include "main.h"

//�}�N����`
#define MAX_LIGHT	(4)	//���C�g�̍ő吔

//���C�g�Ǘ��N���X
class CLight
{
public:
	CLight();
	~CLight();
	HRESULT Init();
	void Uninit();
	void Update();
private:
	D3DLIGHT9 m_aLight[MAX_LIGHT];	//���C�g�\����
};

#endif // !_LIGHT_H_
