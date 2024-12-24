//================================================================
//	�����_���[�N���X�w�b�_(renderer.h)
//
//
//================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include<windows.h>
#include"d3dx9.h"	//��ʕ`��ɕK�v

class CRenderer
{
public:
	CRenderer();								//�R���X�g���N�^
	~CRenderer();								//�f�X�g���N�^
	HRESULT Init(HWND hWnd, BOOL bWindow);		//�����ݒ�
	void Uninit();								//�I������
	void Update();								//�X�V����
	void Draw();								//�`�揈��
	LPDIRECT3DDEVICE9 GetDevice();				//3D�f�o�C�X�̎擾
	void WireFrame();
private:
	LPDIRECT3D9 m_pD3D;				//Direct3D
	LPDIRECT3DDEVICE9 m_pD3DDevice;	//Direct3D�f�o�C�X
	bool m_bWireFrame;				//���C���[�t���[���̎��F�\��
	float m_fFogStartPos;
	float m_fFogEndPos;
	float m_fFogDensity;

};
#endif // !_RENDERER_H_
