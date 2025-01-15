//
//	�ŋߓG�����\���p���
//
//
//

//
//	�C���N���[�h
//
#include "dirmarker.h"
#include "player.h"
#include "enemy.h"

namespace
{
	const int DIRDISPCNT = 5;	//�G�������c�肱�ꂾ���ɂȂ�����Ŋ��G������\��
	constexpr float DIR_ROT_X = D3DX_PI * 0.05f;	//�}�[�J�[�����₷�����邽�߂̌X������(x��)
}

//
//	�R���X�g���N�^
//
CDIRMARKER::CDIRMARKER() :
	m_bDisp(false)
{
	Init();
}

//
//	�f�X�g���N�^
//
CDIRMARKER::~CDIRMARKER()
{

}

//
//	������
//
HRESULT CDIRMARKER::Init()
{
	if (FAILED(CObjectX::Init()))	//���_�o�b�t�@�쐬
	{
		return E_FAIL;
	}

	m_ModelFile = "data\\model\\dirmarker.x";
	m_scale = { 1.0f,1.0f,1.0f };

	//���̃^�C�v���Z�b�g
	SetType(DIRMARKER);

	return S_OK;
}

//
//	�I������
//
void CDIRMARKER::Uninit()
{
	CObjectX::Uninit();
}

//
//	�X�V����
//
void CDIRMARKER::Update()
{
	//�Q�[�����̓G�������擾
	int enmCnt = CEnmManager::GetEnmCnt();
	if (enmCnt <= DIRDISPCNT)
	{//�G���������ȉ��ɂȂ�����
		//	�����\����ON�ɂ���
		m_bDisp = true;
	}
	if (enmCnt <= 0)
	{//	�S����΂��������
		m_bDisp = false;
	}

	CObjectX::Update();

	//�I�u�W�F�N�g�Ƃ̓����蔻��
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		//�I�u�W�F�N�g�擾
		CObject* pObj = CObject::GetObj(3, nCntObj);

		if (pObj != nullptr)
		{//	�g���Ă�����
			CObject::TYPE type = pObj->GetType();

			if (type == PLAYER)
			{//�I�u�W�F�N�g�̃^�C�v���v���C���[�Ȃ�
				//�I�u�W�F�N�g�̃^�C�v���v���C���[�^�Ƀ_�E���L���X�g
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);

				//�v���C���[�̈ʒu���擾
				vec3 targetpos = pPlayer->GetPos();
				vec3 targetrot = pPlayer->GetRot();
				//�擾���Ă������Ń}�g���b�N�X���쐬
				D3DXMATRIX rotmtx;
				D3DXMatrixRotationYawPitchRoll(&rotmtx, targetrot.y, targetrot.x, targetrot.z);

				//�C�ӂ̃I�t�Z�b�g
				vec3 offset = { 0.0f,20.0f,0.0f };

				//�I�t�Z�b�g�����[���h���W�ɕϊ�
				vec3 offsetInWorld;
				offsetInWorld.x = offset.x * rotmtx._11 + offset.y * rotmtx._21 + offset.z * rotmtx._31;
				offsetInWorld.y = offset.x * rotmtx._12 + offset.y * rotmtx._22 + offset.z * rotmtx._32;
				offsetInWorld.z = offset.x * rotmtx._13 + offset.y * rotmtx._23 + offset.z * rotmtx._33;

				//�ʒu�v�Z
				vec3 newPos =
				{
					targetpos.x + offsetInWorld.x,
					targetpos.y + offsetInWorld.y,
					targetpos.z + offsetInWorld.z
				};

				//	�ʒu��ݒ�
				SetPos(newPos);

				//	�Ŋ��̓G�擾
				CEnemy* pNearestEnemy = pPlayer->GetNearestEnm();

				if (pNearestEnemy)
				{//�Ŋ��̓G��񂪕ۑ�����Ă����
					//�G�ʒu�擾
					vec3 enmPos = pNearestEnemy->GetPos();

					//�v���C���[�̈ʒu����Ŋ��̓G�̕������v�Z
					vec3 direction = targetpos - enmPos;
					D3DXVec3Normalize(&direction, &direction);	//���K��

					//�}�[�J�[�̉�]���v�Z
					float yaw = atan2f(direction.x, direction.z);	//y����]

					//	�}�[�J�[�̉�]��ݒ�
					SetRot({ DIR_ROT_X, yaw, 0.0f });
				}
			}
		}
	}
}

//
//	�`�揈��
//
void CDIRMARKER::Draw()
{
	if (m_bDisp)
	{
		CObjectX::Draw();
	}
}

//
//	��������
//
CDIRMARKER* CDIRMARKER::Create(D3DXVECTOR3 pos)
{
	CDIRMARKER* pDirmarker = new CDIRMARKER;
	pDirmarker->Init();
	pDirmarker->m_pos = pos;

	return pDirmarker;
}
