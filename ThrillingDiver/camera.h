//
//	�J�����w�b�_(camera.h)
//
//
//
#ifndef _CAMERA_H_
#define _CAMERA_H_

//�C���N���[�h
#include "main.h"
//#include "player.h"

//�J�����̊Ǘ��N���X
class CCamera
{
public:
	//�J�����̌���
	typedef enum
	{
		FRONT = 0,
		RIGHT,
		BACK,
		LEFT,
		MAX
	}DIR;
	//�J�����̏��
	typedef enum
	{
		NONE = 0,
		CORRECTING,
		FIN
	}STATE;
	//���_�ʒu�̐؂�ւ�
	enum class VpntPos
	{
		ONE = 0,
		THIRD,
	};
	CCamera();
	~CCamera();
	HRESULT Init();
	void Uninit();
	void Update();
	void SetCamera();	//�J�����̐ݒ�
	void SetShake(int nShakeFrame, float fShakeValue);	//�J�����h�炵�̐ݒ�
	D3DXVECTOR3 GetRot();
	int GetState();
	void SetCameraDir(int dir);
	D3DXVECTOR3 GetCameraDir();
private:
	D3DXVECTOR3 m_posV;		//���_
	D3DXVECTOR3 m_posR;		//�����_
	D3DXVECTOR3 m_vecU;		//�����
	D3DXVECTOR3 m_rot;		//�J�����̌��݌���
	D3DXVECTOR3 m_rotgo;	//�J�����̖ڕW����
	D3DXMATRIX m_mtxProjection;		//�v���W�F�N�V�����s��
	D3DXMATRIX m_mtxView;			//�r���[�s��
	float m_distance;				//�����_�Ǝ��_�̋����{��
	int m_Dir;						//�J�����̌����^�C�v
	int m_ShakeFrame;				//�h�炷����
	float m_ShakeValue;				//�h�炷��(�t���[����)
	int m_CameraState;				//�J�����̏��
	VpntPos m_Vpos;			//���_�̈ʒu
};

#endif // !_CAMERA_H_
