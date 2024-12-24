//================================================================================
//	�J����CPP
//
//
//================================================================================

//=========================================================
//	�C���N���[�h
//=========================================================
#include "main.h"
#include "camera.h"
#include "manager.h"
#include "object.h"
#include "player.h"
//#include "guid2D.h"
#include <time.h>
#include <stdio.h>

//=========================================================
//	�ÓI�����o�֐�������
//=========================================================

//=========================================================
//	�}�N����`
//=========================================================
#define CAMERAMOVE	(10.0f)	//�J�����̑���
#define DIST_THIRD	(150.0f)	//�����_���_�{������(3�l��)
#define DIST_ONE	(100.0f)	//�����_���_�{������(1�l��)

//=========================================================
//	�R���X�g���N�^
//=========================================================
CCamera::CCamera() :m_ShakeFrame(0), m_ShakeValue(0.0f), m_CameraState(STATE::NONE), m_Vpos(VpntPos::THIRD)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�v���W�F�N�V�����}�g���b�N�X
	m_posV = D3DXVECTOR3(0.0f, 200.0f, -100.0f);	//���_
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�����_
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//������x�N�g��
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//���_�̌���
	m_rotgo = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���_�̌���
	m_distance = 0.5f;								//�����_�Ǝ��_�̋����{��
	m_Dir = FRONT;									//�J�����̌���( ���� )
}

//=========================================================
//	�f�X�g���N�^
//=========================================================
CCamera::~CCamera()
{

}

//=========================================================
//	�����ݒ�
//=========================================================
HRESULT CCamera::Init()
{
	//�v���W�F�N�V�����}�g���b�N�X
	m_posV = D3DXVECTOR3(0.0f, 200.0f, -100.0f);	//���_
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�����_
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//������x�N�g��
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���_�̌���
	m_rotgo = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���_�̌���
	srand((int)time(NULL));	//�����_���V�[�h��ݒ�

	return S_OK;
}

//=========================================================
//	�I������
//=========================================================
void CCamera::Uninit()
{

}

//=========================================================
//	�X�V����
//=========================================================
void CCamera::Update()
{
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();

	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		//�I�u�W�F�N�g�擾
		CObject* pObj = CObject::GetObj(3, nCntObj);

		if (pObj != nullptr)
		{
			CObject::TYPE type = pObj->GetType();

			//��ނ̎擾
			if (type == CObject::TYPE::PLAYER)
			{
				CPlayer* pPlayer = (CPlayer*)pObj;

				//�v���C���[�̊p�x���擾
				D3DXVECTOR3 PLrot = pPlayer->GetRot();

				//�v���C���[��z����]���擾
				if (m_Vpos == VpntPos::THIRD)
				{
					//	�J�����̒����_���v���C���[��菭����ɂ�����
					m_posR = pPlayer->GetPos() += { 0.0f, 20.0f, 0.0f };

					//	���_�͒����_�̂���ɏ�����ɐݒ�
					m_posV.y = m_posR.y + 15.0f;

					m_posV.x = m_posR.x - sinf(PLrot.y + D3DX_PI) * DIST_THIRD;
					m_posV.z = m_posR.z - cosf(PLrot.y + D3DX_PI) * DIST_THIRD;
				}
				else if (m_Vpos == VpntPos::ONE)
				{
					m_posV = pPlayer->GetPos();

					m_posR.x = m_posV.x - sinf(PLrot.y) * DIST_ONE;
					m_posR.z = m_posV.z - cosf(PLrot.y) * DIST_ONE;
					m_posR.y = m_posV.y - 10.0f;
				}

				////�J�����̏�����x�N�g�����v���C���[�̌X�����Q�l�ɒ���
				//D3DXMATRIX mtxRotZ;
				//D3DXVECTOR3 vecU = { 0.0f,1.0f,0.0f };
				//D3DXMatrixRotationZ(&mtxRotZ, PLrot.z);	//�v���C���[��z����]�s��
				//D3DXVec3TransformCoord(&m_vecU, &vecU, &mtxRotZ);

				//�J�����Ƀv���C���[�̊p�x�𔽉f
				//m_rot.z = PLrot.z;
			}
		}
	}

#ifdef _DEBUG
	//���_�̕���(���_���])
	if (pKeyboard->GetTrigger(DIK_NUMPAD4) == true)
	{
		m_Dir++;
		if (m_Dir >= LEFT + 1)
		{
			m_Dir = FRONT;
		}
	}
	if (pKeyboard->GetTrigger(DIK_NUMPAD6) == true)
	{
		m_Dir--;
		if (m_Dir <= FRONT - 1)
		{
			m_Dir = LEFT;
		}
	}

	//�����_�Ǝ��_�Ԃ̋�������
	if (pKeyboard->GetPress(DIK_P) == true)
	{
		m_distance += 0.01f;
	}
	if (pKeyboard->GetPress(DIK_L) == true)
	{
		m_distance -= 0.01f;
	}
#endif // DEBUG

	if (pKeyboard->GetTrigger(DIK_1) == true)
	{
		m_Vpos = VpntPos::ONE;
	}
	else if (pKeyboard->GetTrigger(DIK_3) == true)
	{
		m_Vpos = VpntPos::THIRD;
	}

	//y������
	if (m_rotgo.y > D3DX_PI)
	{
		float fOverRot = fabsf(m_rotgo.y) - D3DX_PI;
		m_rotgo.y = -(D3DX_PI - fOverRot);
	}
	else if (m_rotgo.y < -D3DX_PI)
	{
		float fOverRot = fabsf(m_rotgo.y) - D3DX_PI;
		m_rotgo.y = (D3DX_PI - fOverRot);
	}

	//z������
	/*if (m_rotgo.z > D3DX_PI)
	{
		float fOverRot = fabsf(m_rotgo.z) - D3DX_PI;
		m_rotgo.z = -(D3DX_PI - fOverRot);
	}
	else if (m_rotgo.z < -D3DX_PI)
	{
		float fOverRot = fabsf(m_rotgo.z) - D3DX_PI;
		m_rotgo.z = (D3DX_PI - fOverRot);
	}*/

	//�ړI�̊p�x�ƌ��݂̊p�x�̍������߂�
	float rotDiffY = m_rotgo.y - m_rot.y;
	//float rotDiffZ = m_rotgo.z - m_rot.z;

	//y������
	if (rotDiffY > D3DX_PI)
	{//�ړI�ƌ��݂̊p�x�̍���3.14�c�ȏ�Ȃ�
		m_rotgo.y -= (D3DX_PI * 2.0f);
		rotDiffY = m_rotgo.y - m_rot.y;
	}
	else if (rotDiffY < -D3DX_PI)
	{//-3.14�c�ȉ��Ȃ�
		m_rotgo.y += (D3DX_PI * 2.0f);
		rotDiffY = m_rotgo.y - m_rot.y;
	}

	////z������
	//if (rotDiffZ > D3DX_PI)
	//{//�ړI�ƌ��݂̊p�x�̍���3.14�c�ȏ�Ȃ�
	//	m_rotgo.z -= (D3DX_PI * 2.0f);
	//	rotDiffZ = m_rotgo.z - m_rot.z;
	//}
	//else if (rotDiffZ < -D3DX_PI)
	//{//-3.14�c�ȉ��Ȃ�
	//	m_rotgo.z += (D3DX_PI * 2.0f);
	//	rotDiffZ = m_rotgo.z - m_rot.z;
	//}

	m_rot.y += (rotDiffY * 0.080f);	//���񂾂�ƖړI�̊p�x�ɕ␳���Ă���
	//m_rot.z += (rotDiffZ * 0.080f);	//���񂾂�ƖړI�̊p�x�ɕ␳���Ă���

	//y������
	if ((m_rot.y) > D3DX_PI)
	{
		float overRot = fabsf(m_rot.y) - D3DX_PI;
		m_rot.y = -(D3DX_PI - overRot);
	}
	else if ((m_rot.y) < -D3DX_PI)
	{
		float overRot = fabsf(m_rot.y) - D3DX_PI;
		m_rot.y = (D3DX_PI - overRot);
	}

	////z������
	//if ((m_rot.z) > D3DX_PI)
	//{
	//	float overRot = fabsf(m_rot.z) - D3DX_PI;
	//	m_rot.z = -(D3DX_PI - overRot);
	//}
	//else if ((m_rot.z) < -D3DX_PI)
	//{
	//	float overRot = fabsf(m_rot.z) - D3DX_PI;
	//	m_rot.z = (D3DX_PI - overRot);
	//}

	//	�ʒu�ݒ�p�̕ϐ�
	float VRDist = 0.0f;

	if (m_Vpos == VpntPos::THIRD)
	{//	3�l��
		VRDist = DIST_THIRD;
	}
	else if (m_Vpos == VpntPos::ONE)
	{//	1�l��
		VRDist = DIST_ONE;
	}

	//�␳�����̊m�F
	if (fabsf(rotDiffY) >= 0.001f)
	{
		m_CameraState = STATE::CORRECTING;
	}
	else
	{
		m_CameraState = STATE::NONE;
	}
}

//=========================================================
//	�J�����Z�b�g
//=========================================================
void CCamera::SetCamera()
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�L�[�{�[�h��t
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),												//����p
		(float)CProcess::SCREEN_WIDTH / (float)CProcess::SCREEN_HEIGHT,		//��ʂ̃A�X�y�N�g��
		10.0f,																//z�̍ŏ��l
		15000.0f);															//z�̍ő�l

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	//�J�����h��̔��f
	D3DXVECTOR3 adjust = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (m_ShakeFrame > 0)
	{
		m_ShakeFrame--;
		adjust.x = ((float)rand() / ((float)RAND_MAX + 1)) * m_ShakeValue;
		adjust.y = ((float)rand() / ((float)RAND_MAX + 1)) * m_ShakeValue;
		adjust.z = ((float)rand() / ((float)RAND_MAX + 1)) * m_ShakeValue;
	}

	////���s���e�ŃJ������ݒ�
	//D3DXMatrixOrthoLH(&m_mtxProjection,
	//	(float)CProcess::SCREEN_WIDTH * m_distance,
	//	(float)CProcess::SCREEN_HEIGHT * m_distance,
	//	DISTANCE - 22.0f,		//�v���C���[�̍��E�݂̂�F���\�ɂ���
	//	DISTANCE + 500.0f);	//�����ʂɑ��݂���ꍇ�ɉf��

	D3DXVECTOR3 posV = m_posV + adjust;
	D3DXVECTOR3 posR = m_posR + adjust;

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&posV,			//���_
		&posR,			//�����_
		&m_vecU);		//�����
	
	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=========================================================
//	�J�����̌����擾
//=========================================================
D3DXVECTOR3 CCamera::GetRot()
{
	return m_rot;
}

//=========================================================
//	�J�����̗h��ݒ�
//=========================================================
void CCamera::SetShake(int nShakeFrame, float fShakeValue)
{
	m_ShakeFrame = nShakeFrame;
	m_ShakeValue = fShakeValue;
}

//=========================================================
//	�J�����̏�Ԏ擾
//=========================================================
int CCamera::GetState()
{
	return m_CameraState;
}

//=========================================================
//	�J�����̕����ݒ�
//=========================================================
void CCamera::SetCameraDir(int dir)
{
	m_Dir = dir;
}

//=========================================================
//	�J�����̕����擾
//=========================================================
D3DXVECTOR3 CCamera::GetCameraDir()
{
	return m_rot;
}