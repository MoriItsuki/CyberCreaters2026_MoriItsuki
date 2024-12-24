//
//	�I�u�W�F�N�g2D�w�b�_(object2D.h)
//
//
//
#pragma once

//�C���N���[�h
#include "object.h"
#include <memory>

//2D�|���S���N���X�Ǘ��N���X
class CObject2D :public CObject
{
public:
	typedef struct
	{
		D3DXVECTOR3 pos;	//���_���
		float rhw;	//���W�ϊ��p�W��(1.0f�ŌŒ�)
		D3DCOLOR col;	//���_�J���[
		D3DXVECTOR2 tex;
	}VERTEX_2D;

	CObject2D(int nPriority = 3);
	virtual ~CObject2D()override;
	virtual HRESULT Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Draw()override;
	static CObject2D* Create();	//Object2D�̐���
	void BindTexture(LPDIRECT3DTEXTURE9 pTex);	//�e�N�X�`���ݒ�
	const D3DXVECTOR3& GetPos() { return m_pos; }
	D3DXVECTOR3& GetMove() { return m_move; }
	void SetSize(const D3DXVECTOR3 size);	//�T�C�Y�Z�b�g
	void SetPos(const D3DXVECTOR3 pos);		//�|�W�V�����Z�b�g
	void SetColor(const D3DCOLOR color);	//�J���[�Z�b�g
protected:
	//	LPDIRECT3DTEXTURE9 = *IDirect3DVertexBuffer9
	//std::unique_ptr<IDirect3DVertexBuffer9, void(*)(IDirect3DVertexBuffer9*)> m_pVtxBuff;	//���_�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pTexture;			//�e�N�X�`���̃|�C���^
	D3DXVECTOR3 m_pos;					//�I�u�W�F�N�g�̍��W
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_size;					//�I�u�W�F�N�g���Ƃ̃T�C�Y
	D3DCOLOR m_color;
	float m_fAngle;
	float m_fLength;
private:
};
