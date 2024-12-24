//============================================================================
//	�v���C���[CPP(player.cpp)
//
//
//============================================================================
//==========================================================
//�C���N���[�h
//==========================================================
#include "main.h"
#include "player.h"
#include "manager.h"
#include "block.h"
#include "obstacle.h"
#include "texture.h"
#include "dirmarker.h"
#include "enemy.h"
#include "dashgauge.h"
#include "item.h"

#include <windows.h>

//==========================================================
//	�萔
//==========================================================
namespace
{
	using vec3 = D3DXVECTOR3;

	constexpr float pool = 5.0f;
	constexpr float PLSIZE = 30.0f;
	constexpr float BLOCKSIZE = 10.0f;
	constexpr float BOXSIZE = 30.0f;
	constexpr float ITEMSIZE = 50.0f;
	constexpr float PL_FRICION = 0.005f;

	//�ړ��l
	constexpr float NORMAL = 0.2f;		//�ʏ푖�s
	constexpr float HIGHSPEED = 30.0f;	//�_�b�V��

	constexpr int INITIALDASHCNT = 3;	//�_�b�V���ł��鏉����
	constexpr int ITEMTODASH = 5;		//�A�C�e�������Q�b�g������_�b�V���񐔂ɕϊ�����邩

	constexpr float FPRot = 0.02f;		//�v���C���[���Ȃ������Ƃ���1�t���[���ɉ��p�x
	constexpr float LIM_PL_ROT_Z = D3DX_PI * 0.15f;	//�v���C���[���Ȃ������Ƃ��̌X���̌��E+-

	constexpr int DASH_MAX = 10;		//���߂���_�b�V���̏��
}

//==========================================================
//	�ÓI�����o�֐�������
//==========================================================
const std::string CPlayer::FILE_PLAYER = "data\\texture\\machine.png";

//==========================================================
//	�R���X�g���N�^
//==========================================================
CPlayer::CPlayer() :
	m_nDashCnt(0),
	m_nItemCnt(0),
	m_moveValue(0.0f),
	m_Oldpos(0.0f, 0.0f, 0.0f),
	m_vecFront(0.0f, 0.0f, 0.0f),
	m_nState(STATE::NONE),
	m_MoveState(MOVESTATE::STOP),
	m_fFriction(PL_FRICION),
	m_bStop(false),
	m_bDash(false),
	m_pNearestEnemy(nullptr),
	m_NearestEnmPos(0.0f, 0.0f, 0.0f),
	m_pDashGauge(nullptr)
{
	m_pFont = nullptr;
	Init();
}

//==========================================================
//	�f�X�g���N�^
//==========================================================
CPlayer::~CPlayer()
{
	
}

//==========================================================
//	�����ݒ�(���_�o�b�t�@����)
//==========================================================
HRESULT CPlayer::Init()
{
	// �t�H���g�ݒ�̍\���̂�������
	D3DXFONT_DESC fontDesc = {};
	fontDesc.Height = 24;                           //	�t�H���g�T�C�Y
	fontDesc.Width = 0;								//	�t�H���g�̕�(0->��������)
	fontDesc.Weight = FW_BOLD;						//	�t�H���g�̑���
	fontDesc.MipLevels = 1;							//	�~�b�v�}�b�v���x��
	fontDesc.Italic = false;						//	�C�^���b�N��
	fontDesc.CharSet = DEFAULT_CHARSET;				//	�����Z�b�g
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;	//	�o�͐��x
	fontDesc.Quality = DEFAULT_QUALITY;				//	�`��i��
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;	//�s�b�`�ƃt�@�~��
	strcpy_s(fontDesc.FaceName, "Arial");           // �t�H���g�̎��

	// �t�H���g�̍쐬
	if (FAILED(D3DXCreateFontIndirect(CManager::GetRenderer()->GetDevice(), &fontDesc, &m_pFont)))
	{
		return E_FAIL;
	}

	if (FAILED(CObjectX::Init()))	//���_�o�b�t�@�쐬
	{
		return E_FAIL;
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_moveValue = 0.5f;

	m_ModelFile = "data\\model\\player.x";

	//�_�b�V���\�񐔂����Z�b�g
	m_nDashCnt = INITIALDASHCNT;

	//�e�N�X�`���쐬
	CTexture* pTex = CManager::GetTexture();	//�e�N�X�`�����擾
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_PLAYER)));	//�ݒ�

	CObject::SetType(PLAYER);	//�I�u�W�F�N�g�Ƀv���C���[�^�C�v��t�^

	CDIRMARKER::Create(m_pos);

	m_pDashGauge = CDashgauge::Create();

	return S_OK;
}

//==========================================================
//	�I������(���_�o�b�t�@�̔j��)
//==========================================================
void CPlayer::Uninit()
{
	CObjectX::Uninit();

	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
}

//==========================================================
//	�X�V����(���_���̍X�V)
//==========================================================
void CPlayer::Update()
{
	CTimeManager* pTimeManager = CManager::GetTimeManager();
	bool bStop = pTimeManager->IsTimeStop();
	if (bStop)
	{
		return;
	}

	CObjectX::Update();

	//�ʒu���ߋ���
	m_Oldpos = m_pos;

	//�ړ��l��ʏ��
	m_moveValue = 0.0f;

	//	�v���C���[�̓��͏���
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();
	CInputGamePad* pButton = CManager::GetGamepad();
	CCamera* pCamera = CManager::GetCamera();
	
	//	�g���K�[�̒l���擾
	float NorTriggerValue = pButton->GetNorTriggerValue();	//�K����0.0�`2.0

	//===============================
	//			    X
	//		 "�_�b�V���ؑ�"
	//===============================
	m_bDash = false;
	if (pButton->GetTrigger(2) == true &&
		m_nDashCnt > 0 && 
		m_MoveState != MOVESTATE::STOP)
	{
		m_bDash = true;
		--m_nDashCnt;	//�_�b�V���ł���񐔂����炷
	}

	//===============================
	//			�E�g���K�[
	//			"�O�i"
	//===============================
	if (NorTriggerValue < 1.0)
	{
		if (!m_bDash)
		{
			//�O�i��Ԃɂ���
			SetState(MOVESTATE::FORWARD);

			m_moveValue = NORMAL;
		}
		else
		{
			//�_�b�V����Ԃɂ���
			SetState(MOVESTATE::DASH);

			m_moveValue = HIGHSPEED;
		}
	}
	else
	{
		SetState(MOVESTATE::STOP);
	}
	//===============================
	//			���g���K�[
	//		"�u���[�L�E��i"
	//===============================
	if (pButton->GetPress(5) == true)
	{//	�E�V�����_�[��������
		//��i��Ԃɂ���
		SetState(MOVESTATE::BACK);

		m_fFriction += 0.0045f;

		m_bStop = true;
	}
	if (pButton->GetRelease(5) == true)
	{//	�E�V�����_�[��������
		m_fFriction = PL_FRICION;

		m_bStop = false;
	}

	//��]
	if (pButton->GetNorStick_lx() > 1.8)
	{
		//��肾���Ȃ���
		m_rotgo.y += FPRot;
		//�u���[�L���|�����Ă�����
		if (m_bStop)	m_rotgo.y += FPRot;
		if (m_rot.z < LIM_PL_ROT_Z)	m_rot.z += FPRot;
	}
	if (pButton->GetNorStick_lx() < 0.2)
	{
		//��肾���Ȃ���
		m_rotgo.y -= 0.02f;
		//�u���[�L���|�����Ă�����
		if (m_bStop)	m_rotgo.y -= FPRot;
		if (m_rot.z > -LIM_PL_ROT_Z)	m_rot.z -= FPRot;
	}
	if (pButton->GetNorStick_lx() == 1.0)
	{
		if (m_rot.z > 0.0)
		{
			m_rot.z -= FPRot;
		}
		if (m_rot.z < 0.0)
		{
			m_rot.z += FPRot;
		}
	}

	//=============================================================
	//	�_�b�V���Q�[�W�̍X�V
	//=============================================================
	float ratioDashgauge = static_cast<float>(m_nDashCnt) / static_cast<float>(DASH_MAX);
	m_pDashGauge->Update(ratioDashgauge);

#ifdef _DEBUG
		if (pKeyboard->GetPress(DIK_TAB) == true)
		{
			m_pos.y += 0.5f;
		}
		if (pKeyboard->GetPress(DIK_LSHIFT) == true)
		{
			m_pos.y -= 0.5f;
		}

		//�f�o�b�O�p�_�b�V���񐔒ǉ�
		if (pButton->GetTrigger(4) == true)
		{
			++m_nDashCnt;
		}
#endif // _DEBUG

	//�ړ��l���玲���Ƃ̃��[�u�l���Z�o
	m_move.x -= sinf(m_rot.y) * m_moveValue;
	m_move.z -= cosf(m_rot.y) * m_moveValue;

	//�x�N�g�����Z�o
	float fLength = -sqrtf(m_move.x * m_move.x + m_move.z * m_move.z);
	
	//�v���C���[�̌����ɍ��킹�Ĉړ��ʂ�␳
	m_move.x = sinf(m_rot.y) * fLength;
	m_move.z = cosf(m_rot.y) * fLength;

	//	Y�l�ȊO�̈ړ��l�Ɍ��Z����
	m_move.x += (0.0f - m_move.x) * m_fFriction;
	m_move.z += (0.0f - m_move.z) * m_fFriction;

	//	�|�W�V�����Ƀ��[�u�l�����Z
	m_pos += m_move;

	//���Z�������ʓ����Ă��Ȃ����
	if (m_move.x == 0.0f && m_move.z == 0.0f)
	{//	�ړ���Ԃ�"��~"�ɂ���
		SetState(MOVESTATE::STOP);
	}

	//	�G�Ƃ̒�������p
	float closestEnmDist = FLT_MAX;

	//�I�u�W�F�N�g�Ƃ̓����蔻��
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		//�I�u�W�F�N�g�擾
		CObject* pObj = CObject::GetObj(3, nCntObj);

		if (pObj != nullptr)
		{//	�g���Ă�����
			//	�I�u�W�F�N�g�̃^�C�v���擾
			CObject::TYPE type = pObj->GetType();

			//�ۑ��p�ϐ�
			D3DXVECTOR3 pos;	//�~�܂锻��p�ϐ�
			D3DXVECTOR3 move(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 size;
			D3DXVECTOR3 rot;

			if (type == BLOCK)
			{//�I�u�W�F�N�g�̃^�C�v���u���b�N�Ȃ�

				//�I�u�W�F�N�g�̃^�C�v���u���b�N�^�ɃL���X�g
				CBlock* pBlock = dynamic_cast<CBlock*>(pObj);
				//	�u���b�N����`����Ă��Ȃ��������΂�
				if (!pBlock) return;

				D3DXVECTOR3 scale = pBlock->GetScale();

				//�u���b�N�̈ʒu���擾
				pos = pBlock->GetPos();

				//�v���C���[�ƃu���b�N�̓����蔻��̃T�C�Y����
				size = D3DXVECTOR3(PLSIZE + BLOCKSIZE * scale.x, PLSIZE + BLOCKSIZE * scale.y, PLSIZE + BLOCKSIZE * scale.z);

				rot = pBlock->GetRot();
			}
			else if (type == OBSTACLE)
			{//�I�u�W�F�N�g�̃^�C�v����Q���Ȃ�
				//�I�u�W�F�N�g�̃^�C�v����Q���^�ɃL���X�g
				CObstacle* pObs = dynamic_cast<CObstacle*>(pObj);
				//	��Q������`����Ă��Ȃ��������΂�
				if (!pObs) return;

				D3DXVECTOR3 scale = pObs->GetScale();

				pos = pObs->GetPos();

				//�v���C���[�Ə�Q���̓����蔻��̃T�C�Y����
				size = D3DXVECTOR3(PLSIZE + BLOCKSIZE * scale.x, PLSIZE + BLOCKSIZE * scale.y, PLSIZE + BLOCKSIZE * scale.z);
			}
			else if (type == ENEMY)
			{//�I�u�W�F�N�g�̃^�C�v���G�Ȃ�
				//�I�u�W�F�N�g�̃^�C�v���G�l�~�[�^�ɃL���X�g
				CEnemy* pEnemy = dynamic_cast<CEnemy*>(pObj);
				//	�G����`����Ă��Ȃ��������΂�
				if (!pEnemy) return;
				//	�ʒu�擾
				vec3 enmPos = pEnemy->GetPos();
				//	�����x�N�g�������߂�
				vec3 direction = enmPos - m_pos;
				//	�������Z�o
				float distSq = D3DXVec3LengthSq(&direction);

				//�Ŋ��̓G���X�V
				if (distSq < closestEnmDist)
				{//	����ɋ߂��G�����������ꍇ
					closestEnmDist = distSq;	//�G�Ƃ̋������X�V
					m_pNearestEnemy = pEnemy;	//�Ŋ��̓G��ێ�
					m_NearestEnmPos = enmPos;	//�G�̈ʒu��ێ�
				}
			}
			else if (type == ITEM)
			{//�^�C�v���A�C�e��
				//�A�C�e���^�ɃL���X�g
				CItem* pItem = dynamic_cast<CItem*>(pObj);
				if (!pItem) return;
				//	�ʒu�擾
				vec3 itemPos = pItem->GetPos();

				vec3 vecDir = m_pos - itemPos;

				float diffItem = D3DXVec3LengthSq(&vecDir);

				if (diffItem < ITEMSIZE * ITEMSIZE)
				{//	�͈͓��ɓ�������
					if (m_nDashCnt >= DASH_MAX)
					{//	�_�b�V���\����MAX��������

					}
					else
					{//	����ȓ���������
						m_nItemCnt++;	//�����A�C�e���������Z
						pItem->OutOn();	//�����t���O�𗧂Ă�
					}
				}
			}

			//	�A�C�e������萔�ɓ��B������
			if (m_nItemCnt >= ITEMTODASH)
			{
				//	�A�C�e���������Z�b�g
				m_nItemCnt = 0;
				//	�_�b�V���񐔂����Z
				m_nDashCnt++;
			}

			//�ړ��ʂ��m�F
			float absMoveY = std::abs(m_move.y);
			float absMoveX = std::abs(m_move.x);
			float absMoveZ = std::abs(m_move.z);

			//��ԑ傫���ړ��ʂ��������ǂꂩ���m�F
			if (absMoveY >= absMoveX && absMoveY >= absMoveZ)
			{//Y������ԑ傫�����
				//X���̔���
				if (m_pos.z < pos.z + size.z && m_pos.z > pos.z - size.z &&
					m_pos.y < pos.y + size.y && m_pos.y > pos.y - size.y)
				{
					//	������
					if (m_Oldpos.x <= pos.x - size.x && m_pos.x > pos.x - size.x)
					{
						{
							m_pos.x = pos.x - size.x;
							m_move.x = 0.0f;
							//m_moveValue = -pool;
						}
					}
					//	�E����
					else if (m_Oldpos.x >= pos.x + size.x && m_pos.x < pos.x + size.x)
					{
						{
							m_pos.x = pos.x + size.x;
							m_move.x = 0.0f;
							//m_moveValue = -pool;
						}
					}
				}
				//Z���̔���
				if (m_pos.x < pos.x + size.x && m_pos.x > pos.x - size.x &&
					m_pos.y < pos.y + size.y && m_pos.y > pos.y - size.y)
				{
					if (m_Oldpos.z <= pos.z - size.z && m_pos.z > pos.z - size.z)
					{
						{
							m_pos.z = pos.z - size.z;
							m_move.z = 0.0f;
							//m_moveValue = -pool;

						}
					}
					else if (m_Oldpos.z >= pos.z + size.z && m_pos.z < pos.z + size.z)
					{
						{
							m_pos.z = pos.z + size.z;
							m_move.z = 0.0f;
							//m_moveValue = -pool;

						}
					}
				}
				//Y���̔���
				if (m_pos.x < pos.x + size.x && m_pos.x > pos.x - size.x &&
					m_pos.z < pos.z + size.z && m_pos.z > pos.z - size.z)
				{
					//�ォ�痎��
					if (m_Oldpos.y >= pos.y + size.y && m_pos.y < pos.y + size.y)
					{
						m_pos.y = pos.y + size.y;	//�v���C���[���u���b�N�̏�ɃZ�b�g
						m_move.y = 0.0f;
						//m_moveValue = -pool;
					}
					//������㏸
					else if (m_Oldpos.y <= pos.y - size.y && m_pos.y > pos.y - size.y)
					{
						m_pos.y = pos.y - size.y;
						m_move.y = 0.0f;
						//m_moveValue = -pool;

					}
				}
			}
			else if (absMoveX >= absMoveY && absMoveX >= absMoveZ)
			{//X������ԑ傫�����
				//Y���̔���
				if (m_pos.x < pos.x + size.x && m_pos.x > pos.x - size.x &&
					m_pos.z < pos.z + size.z && m_pos.z > pos.z - size.z)
				{
					//�ォ�痎��
					if (m_Oldpos.y >= pos.y + size.y && m_pos.y < pos.y + size.y)
					{
						m_pos.y = pos.y + size.y;	//�v���C���[���u���b�N�̏�ɃZ�b�g
						m_move.y = 0.0f;
						//m_moveValue = -pool;
					}
					//������㏸
					else if (m_Oldpos.y <= pos.y - size.y && m_pos.y > pos.y - size.y)
					{
						m_pos.y = pos.y - size.y;
						m_move.y = 0.0f;
						//m_moveValue = -pool;

					}
				}
				//Z���̔���
				if (m_pos.x < pos.x + size.x && m_pos.x > pos.x - size.x &&
					m_pos.y < pos.y + size.y && m_pos.y > pos.y - size.y)
				{
					if (m_Oldpos.z <= pos.z - size.z && m_pos.z > pos.z - size.z)
					{
						
						{
							m_pos.z = pos.z - size.z;
							m_move.z = 0.0f;
							//m_moveValue = -pool;

						}
					}
					else if (m_Oldpos.z >= pos.z + size.z && m_pos.z < pos.z + size.z)
					{
						
						{
							m_pos.z = pos.z + size.z;
							m_move.z = 0.0f;
							//m_moveValue = -pool;

						}

					}
				}
				//X���̔���
				if (m_pos.z < pos.z + size.z && m_pos.z > pos.z - size.z &&
					m_pos.y < pos.y + size.y && m_pos.y > pos.y - size.y)
				{
					//	������
					if (m_Oldpos.x <= pos.x - size.x && m_pos.x > pos.x - size.x)
					{
						
						{
							m_pos.x = pos.x - size.x;
							m_move.x = 0.0f;
							//m_moveValue = -pool;

						}
					}
					//	�E����
					else if (m_Oldpos.x >= pos.x + size.x && m_pos.x < pos.x + size.x)
					{
						
						{
							m_pos.x = pos.x + size.x;
							m_move.x = 0.0f;
							//m_moveValue = -pool;

						}
					}
				}
			}
			else if (absMoveZ >= absMoveY && absMoveZ >= absMoveX)
			{//Z������ԑ傫�����
				//Y���̔���
				if (m_pos.x < pos.x + size.x && m_pos.x > pos.x - size.x &&
					m_pos.z < pos.z + size.z && m_pos.z > pos.z - size.z)
				{
					//�ォ�痎��
					if (m_Oldpos.y >= pos.y + size.y && m_pos.y < pos.y + size.y)
					{
						m_pos.y = pos.y + size.y;	//�v���C���[���u���b�N�̏�ɃZ�b�g
						m_move.y = 0.0f;
						//m_moveValue = -pool;
					}
					//������㏸
					else if (m_Oldpos.y <= pos.y - size.y && m_pos.y > pos.y - size.y)
					{
						m_pos.y = pos.y - size.y;
						m_move.y = 0.0f;
						//m_moveValue = -pool;

					}
				}
				//X���̔���
				if (m_pos.z < pos.z + size.z && m_pos.z > pos.z - size.z &&
					m_pos.y < pos.y + size.y && m_pos.y > pos.y - size.y)
				{
					//	������
					if (m_Oldpos.x <= pos.x - size.x && m_pos.x > pos.x - size.x)
					{
						
						{
							m_pos.x = pos.x - size.x;
							m_move.x = 0.0f;
							//m_moveValue = -pool;

						}
					}
					//	�E����
					else if (m_Oldpos.x >= pos.x + size.x && m_pos.x < pos.x + size.x)
					{
						
						{
							m_pos.x = pos.x + size.x;
							m_move.x = 0.0f;
							//m_moveValue = -pool;

						}
					}
				}
				//Z���̔���
				if (m_pos.x < pos.x + size.x && m_pos.x > pos.x - size.x &&
					m_pos.y < pos.y + size.y && m_pos.y > pos.y - size.y)
				{
					if (m_Oldpos.z <= pos.z - size.z && m_pos.z > pos.z - size.z)
					{
						
						{
							m_pos.z = pos.z - size.z;
							m_move.z = 0.0f;
							//m_moveValue = -pool;

						}
					}
					else if (m_Oldpos.z >= pos.z + size.z && m_pos.z < pos.z + size.z)
					{
						
						{
							m_pos.z = pos.z + size.z;
							m_move.z = 0.0f;
							//m_moveValue = -pool;

						}
					}
				}
			}
		}
	}

	if ((m_rotgo.y) > D3DX_PI)
	{
		float fOverRot = fabsf(m_rotgo.y) - D3DX_PI;
		m_rotgo.y = -(D3DX_PI - fOverRot);
	}
	else if ((m_rotgo.y) < -D3DX_PI)
	{
		float fOverRot = fabsf(m_rotgo.y) - D3DX_PI;
		m_rotgo.y = (D3DX_PI - fOverRot);
	}

	//�ړI�̊p�x�ƌ��݂̊p�x�̍������߂�
	float rotDiff = m_rotgo.y - m_rot.y;

	if (rotDiff > D3DX_PI)
	{//�ړI�ƌ��݂̊p�x�̍���3.14�c�ȏ�Ȃ�
		m_rotgo.y -= (D3DX_PI * 2.0f);
		rotDiff = m_rotgo.y - m_rot.y;
	}
	else if (rotDiff < -D3DX_PI)
	{//-3.14�c�ȉ��Ȃ�
		m_rotgo.y += (D3DX_PI * 2.0f);
		rotDiff = m_rotgo.y - m_rot.y;
	}

	m_rot.y += (rotDiff * 0.100f);	//���񂾂�ƖړI�̊p�x�ɕ␳���Ă���

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
}

//==========================================================
//	�`�揈��(�|���S���`��)
//==========================================================
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X�̃|�C���^

	CObjectX::Draw();
	m_pDashGauge->Draw(pDevice);

#ifdef _DEBUG
	DrawPLInfo();
#endif // _DEBUG
}

//==========================================================
//	CPlayer����
//==========================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer* pPlayer = new CPlayer;
	pPlayer->Init();
	pPlayer->m_pos = pos;
	pPlayer->m_rot = rot;

	return pPlayer;
}

//==========================================================
//	�v���C���[�̃|�W�V�����Z�b�g
//==========================================================
void CPlayer::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==========================================================
//	�v���C���[�̈ړ��l�Z�b�g
//==========================================================
void CPlayer::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//==========================================================
//	����
//==========================================================
void CPlayer::SetReflec()
{

}

//==========================================================
//	�v���C���[�ړ���Ԑݒ�
//==========================================================
void CPlayer::SetState(MOVESTATE state)
{
	m_MoveState = state;
}

//==========================================================
//	�v���C���[�̃|�W�V�����Q�b�g
//==========================================================
D3DXVECTOR3 CPlayer::GetPos()
{
	return m_pos;
}

//==========================================================
//	�v���C���[�̃|�W�V�����Q�b�g
//==========================================================
D3DXVECTOR3 CPlayer::GetVecFront()
{
	return m_vecFront;
}

//==========================================================
//	�v���C���[�̏�Ԏ擾
//==========================================================
int CPlayer::GetState()
{
	return m_nState;
}

//==========================================================
//	�Ŋ��̓G�擾
//==========================================================
CEnemy* CPlayer::GetNearestEnm()const
{
	return m_pNearestEnemy;
}

//==========================================================
//	�Ŋ��̓G�ʒu�擾
//==========================================================
D3DXVECTOR3 CPlayer::GetNearestEnmPos() const
{
	return m_NearestEnmPos;
}

//==========================================================
//	�c��_�b�V���񐔎擾
//==========================================================
int CPlayer::GetDashCnt() const
{
	return m_nDashCnt;
}

//==========================================================
//	���\�L
//==========================================================
void CPlayer::DrawPLInfo()
{
	RECT rect = { 0,0,CProcess::SCREEN_WIDTH,CProcess::SCREEN_HEIGHT };
	char aStr[256];

	snprintf(&aStr[0],
		256,
		"<POS>\nX:%f\nY:%f\nZ:%f\n<ROT>\nX:%f\nY:%f\nZ:%f\n�ړ��l:%f\n�Ŋ��G�ʒu\nX:%f\nY:%f\nZ:%f\n�_�b�V���\��:%d\n�A�C�e��:%d",
		m_pos.x, m_pos.y, m_pos.z,
		m_rot.x, m_pos.y, m_rot.z,
		m_moveValue,
		m_NearestEnmPos.x,
		m_NearestEnmPos.y,
		m_NearestEnmPos.z,
		m_nDashCnt,
		m_nItemCnt
	);

	m_pFont->DrawText(nullptr,
		&aStr[0],
		-1,
		&rect,
		DT_RIGHT,
		D3DCOLOR_RGBA(0, 0, 0, 255));
}

//==========================================================
//	�����蔻��
//==========================================================
bool CPlayer::IsColliding(const vec3& pos1, const vec3& size1, const vec3& pos2, const vec3& size2)
{
	return
		(
			pos1.x - size1.x <= pos2.x + size2.x && pos1.x + size1.x >= pos2.x - size2.x &&
			pos1.y - size1.y <= pos2.y + size2.y && pos1.y + size1.y >= pos2.y - size2.y &&
			pos1.z - size1.z <= pos2.z + size2.z && pos1.z + size1.z >= pos2.z - size2.z
		);
}

