//
//	�I�u�W�F�N�gX�̃w�b�_
//
//
//
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

//
//	�C���N���[�h
//
#include "object.h"
#include <string>

//	�I�u�W�F�N�gX�Ǘ��N���X
class CObjectX :public CObject
{
public:
	typedef struct
	{
		D3DXVECTOR3 pos;	//���_���W
		D3DXVECTOR3 nor;	//�@���x�N�g��
		D3DCOLOR col;		//���_�J���[
		D3DXVECTOR2 tex;	//�e�N�X�`�����W
	}VERTEX_3D;

	CObjectX(int nPriority = 3);
	virtual ~CObjectX()override;
	virtual HRESULT Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Draw()override;
	static CObjectX* Create();	//ObjectX�̐���
	void BindTexture(LPDIRECT3DTEXTURE9 pTex);	//�e�N�X�`���ݒ�
	D3DXVECTOR3& GetPos() { return m_pos; }
	D3DXVECTOR3& GetRot() { return m_rot; }
	D3DXVECTOR3& GetMove() { return m_move; }
	D3DXVECTOR3& GetScale() { return m_scale; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@
	LPDIRECT3DTEXTURE9 m_pTexture;		//�e�N�X�`���̃|�C���^
	D3DXVECTOR3 m_pos;					//�I�u�W�F�N�g�̍��W
	D3DXVECTOR3 m_move;					//�I�u�W�F�N�g�̈ړ���
	D3DXVECTOR3 m_rot;					//�I�u�W�F�N�g�̉�]
	D3DXVECTOR3 m_scale;				//�I�u�W�F�N�g�̊g�k
	D3DXMATRIX m_mtxWorld;				//���[���h�ϊ��s��
	LPD3DXMESH m_pMesh;					//���b�V���ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMat;			//�}�e���A���ւ̃|�C���^
	DWORD m_dwNumMat;					//�}�e���A���ւ̐�
	const char* m_ModelFile;			//X�t�@�C������char�^
private:
};
#endif // !_OBJECTX_H_
