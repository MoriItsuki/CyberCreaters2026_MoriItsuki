//
//	�I�u�W�F�N�g3D�w�b�_(object3D.h)
//
//
//
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//�C���N���[�h
#include "object.h"

//3D�|���S���N���X�Ǘ��N���X
class CObject3D :public CObject
{
public:
	typedef struct
	{
		D3DXVECTOR3 pos;	//���_���W
		D3DXVECTOR3 nor;	//�@���x�N�g��
		D3DCOLOR col;		//���_�J���[
		D3DXVECTOR2 tex;	//�e�N�X�`�����W
	}VERTEX_3D;

	CObject3D();
	virtual ~CObject3D()override;
	virtual HRESULT Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Draw()override;
	static CObject3D* Create();	//Object3D�̐���
	void BindTexture(LPDIRECT3DTEXTURE9 pTex);	//�e�N�X�`���ݒ�
	D3DXVECTOR3& GetPos() { return m_pos; }
	void SetSize(D3DXVECTOR3 size);
protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@
	LPDIRECT3DTEXTURE9 m_pTexture;		//�e�N�X�`���̃|�C���^
	D3DXVECTOR3 m_pos;					//�I�u�W�F�N�g�̍��W
	D3DXVECTOR3 m_rot;					//�I�u�W�F�N�g�̉�]
	D3DXVECTOR3 m_move;					//�I�u�W�F�N�g���Ƃ̈ړ��l
	D3DXVECTOR3 m_size;					//�I�u�W�F�N�g���Ƃ̃T�C�Y
	D3DXMATRIX m_mtxWorld;				//���[���h�ϊ��s��
};
#endif // !_OBJECT3D_H_
