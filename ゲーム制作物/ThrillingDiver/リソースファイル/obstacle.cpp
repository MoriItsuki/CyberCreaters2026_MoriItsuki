//=========================================================================================
//	��Q��CPP
//
//
//=========================================================================================

//======================================
//	�C���N���[�h
//======================================
#include "obstacle.h"
#include "texture.h"
#include "player.h"

//======================================
//	�ÓI�����o�ϐ�������
//======================================
int CObsManager::m_ObsCnt = 0;
const std::string CObstacle::FILE_OBSTACLE = "data\\texture\\machine.png";

//�^�[�Q�b�g�n�_���X�g
map<string, vector<vec3>> CObsManager::m_targetLists =
{
	{
		"���Ex",
		{
			vec3(+200.0f,0.0f,0.0f),
			vec3(-200.0f,0.0f,0.0f)
		}
	},
	{
		"���Ez",
		{
			vec3(0.0f,0.0f,+200.0f),
			vec3(0.0f,0.0f,-200.0f)
		}
	},
	{
		"�E��x",
		{
			vec3(-200.0f,0.0f,0.0f),
			vec3(+200.0f,0.0f,0.0f)
		}
	},
{
		"�E��z",
		{
			vec3(0.0f,0.0f,-200.0f),
			vec3(0.0f,0.0f,+200.0f)
		}
	},
	{
		"�㉺",
		{
			vec3(0.0f,+250.0f,0.0f),
			vec3(0.0f,-250.0f,0.0f)
		}
	},
	{
		"����",
		{
			vec3(0.0f,-250.0f,0.0f),
			vec3(0.0f,+250.0f,0.0f)
		}
	},
	{
		"�����E",
		{
			vec3(+200.0f,0.0f,0.0f),
			vec3(-200.0f,0.0f,0.0f)
		}
	},
	{
		"�E��]",
		{
			vec3(0.0f,+200.0f,0.0f),
			vec3(0.0f,+150.0f,-150.0f),
			vec3(0.0f,0.0f,-200.0f),
			vec3(0.0f,-150.0f,-150.0f),
			vec3(0.0f,-200.0f,0.0f),
			vec3(0.0f,-150.0f,+150.0f),
			vec3(0.0f,0.0f,+200.0f),
			vec3(0.0f,+150.0f,+150.0f),
		}
	},
	{
		"����]",
		{
			vec3(0.0f,-200.0f,0.0f),
			vec3(0.0f,-150.0f,-150.0f),
			vec3(0.0f,0.0f,-200.0f),
			vec3(0.0f,+150.0f,-150.0f),
			vec3(0.0f,+200.0f,0.0f),
			vec3(0.0f,+150.0f,+150.0f),
			vec3(0.0f,0.0f,+200.0f),
			vec3(0.0f,-150.0f,+150.0f),
		}
	},
};

//�^�[�Q�b�g�n�_���X�g�̖��O���X�g
vector<string> CObsManager::m_targetListnameLists =
{
	"���Ex",
	"���Ez",
	"�E��x",
	"�E��z",
	"�㉺",
	"����",
	"�����E",
	"�E��]",
	"����]",
};

//
//	�萔
//
namespace
{
	using vec3 = D3DXVECTOR3;
	constexpr float PLSIZE = 30.0f;		//�v���C���[�̃T�C�Y
	constexpr float REFSIZE = 10.0f;	//��Q����̃T�C�Y
	constexpr float SPEED = 10.0f;	//��Q�������X�s�[�h
}

//======================================
//	�R���X�g���N�^
//======================================
CObstacle::CObstacle() :
	m_Type(nullptr),
	m_diffPL(0.0f),
	m_BlowDir(0.0f, 0.0f, 0.0f),
	m_Oldpos(0.0f, 0.0f, 0.0f),
	m_RefPos(0.0f, 0.0f, 0.0f),
	m_movePnt(),
	m_movePntIdx(0),
	m_speed(0.0f)
{
	Init();
	CObsManager::IncCnt();
}

//======================================
//	�f�X�g���N�^
//======================================
CObstacle::~CObstacle()
{
	CObsManager::DecCnt();
}

//======================================
//	������
//======================================
HRESULT CObstacle::Init()
{
	if (FAILED(CObjectX::Init()))	//���_�o�b�t�@�쐬
	{
		return E_FAIL;
	}

	m_ModelFile = "data\\model\\cube.x";

	m_scale = { 1.0f, 1.0f, 1.0f };

	m_speed = 0.0f;

	CObject::SetType(OBSTACLE);

	//�e�N�X�`���쐬
	CTexture* pTex = CManager::GetTexture();	//�e�N�X�`�����擾
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_OBSTACLE)));	//�ݒ�

	return S_OK;
}

//======================================
//	�I������
//======================================
void CObstacle::Uninit()
{
	CObjectX::Uninit();
}

//======================================
//	�X�V����
//======================================
void CObstacle::Update()
{
	CCamera* pCamera = CManager::GetCamera();

	CObjectX::Update();

	//�ʒu���ߋ���
	m_Oldpos = m_pos;

	if (m_Type)
	{//	�^�C�v������΂��ꂲ�Ƃ̒ǉ��X�V
		m_Type->AddUpdate(this);
	}

	//�I�u�W�F�N�g�Ƃ̓����蔻��
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		//�I�u�W�F�N�g�擾
		CObject* pObj = CObject::GetObj(3, nCntObj);

		if (pObj != nullptr)
		{//	�g���Ă�����
			if (pObj->GetType() == CObject::TYPE::PLAYER)
			{//�I�u�W�F�N�g�̃^�C�v���v���C���[�Ȃ�
				//�I�u�W�F�N�g�̃^�C�v���v���C���[�^�Ƀ_�E���L���X�g
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);

				//�v���C���[�̈ʒu���擾
				vec3 targetpos = pPlayer->GetPos();
				vec3 targetSize = { PLSIZE,PLSIZE,PLSIZE };
				vec3 MySize = m_scale * REFSIZE;

				if (IsColliding(m_pos, MySize, targetpos, targetSize))
				{
					//pCamera->SetShake(60, 20);
				}
			}
		}
	}
}

//======================================
//	�`�揈��
//======================================
void CObstacle::Draw()
{
	CObjectX::Draw();
}

//======================================
//	�����̒ǉ�
//======================================
void CObstacle::Moveing()
{
	if (!m_movePnt.empty())
	{//	�^�[�Q�b�g�n�_���X�g����łȂ����
		//	���݂̃^�[�Q�b�g�n�_���擾
		const vec3& tgtPos = m_RefPos + m_movePnt[m_movePntIdx];
		
		//	���݂̈ʒu�ƃ^�[�Q�b�g�ʒu�̕����x�N�g�����Z�o
		vec3 direction = tgtPos - m_pos;

		//�ړ��ʂ��w��
		float Speed = m_speed;

		//�P�ʃx�N�g���ɐ��K��
		D3DXVec3Normalize(&direction, &direction);

		//	�x�N�g���ɑ����������Ĉړ��l���v�Z
		m_move = direction * Speed;

		//	�ړ��l���Z
		m_pos += m_move;

		//	�^�[�Q�b�g�n�_�ɋ߂��ꍇ�Ɏ��̃^�[�Q�b�g���w�肷��
		vec3 targetDiff = tgtPos - m_pos;
		float distanceToTarget = D3DXVec3Length(&targetDiff);

		//	���ݒn����^�[�Q�b�g�n�_�܂ł̋��������������Ȃ�
		size_t size = m_movePnt.size();
		if (distanceToTarget < Speed)
		{//	���̃^�[�Q�b�g�n�_���w�肷��
			m_movePntIdx = (m_movePntIdx + 1) % size;
		}
	}
}

//======================================
//	��������(�m�[�}���^�C�v�Œ�)
//======================================
CObstacle* CObstacle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	CObstacle* pObs = new CObstacle;

	pObs->Init();
	pObs->m_RefPos = pos;
	pObs->m_pos = pos;
	pObs->m_scale = scale;
	pObs->SetState(new NomalType());

	return pObs;
}

//======================================
//	��������
//======================================
CObstacle* CObstacle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, const std::vector<vec3>& targetList, float speed, int moveIdx)
{
	CObstacle* pObs = new CObstacle;

	pObs->Init();
	pObs->m_RefPos = pos;
	pObs->m_pos = pos;
	pObs->m_scale = scale;
	pObs->SetState(new MoveingType);
	pObs->m_movePnt = targetList;
	pObs->m_speed = speed;
	pObs->m_movePntIdx = moveIdx;

	return pObs;
}

//======================================
//	��Ԑݒ�
//======================================
void CObstacle::SetState(ObsType* obstype)
{
	if (m_Type)
	{
		//�Â���Ԃ̍폜
		delete m_Type;
		m_Type = nullptr;
	}

	//	�V������ԍڐݒ�
	m_Type = obstype;
}

//======================================
//	�����蔻��
//======================================
bool CObstacle::IsColliding(const vec3& pos1, const vec3& size1, const vec3& pos2, const vec3& size2)
{
	return 
		(pos1.x - size1.x <= pos2.x + size2.x && pos1.x + size1.x >= pos2.x - size2.x &&
		pos1.y - size1.y <= pos2.y + size2.y && pos1.y + size1.y >= pos2.y - size2.y &&
		pos1.z - size1.z <= pos2.z + size2.z && pos1.z + size1.z >= pos2.z - size2.z)
		;
}

//============================================================================
//	��Ԃ��Ƃ̒ǉ��X�V����
//============================================================================

//======================================
//	�m�[�}���^�C�v
//======================================
void NomalType::AddUpdate(CObstacle* pObs)
{

}

//======================================
//	�����^�C�v
//======================================
void MoveingType::AddUpdate(CObstacle* pObs)
{
	//�����֐�
	pObs->Moveing();
}

//======================================
//	�^�[�Q�b�g���X�g�ݒ�
//======================================
void CObsManager::SetTargetLists(const map<string, vector<vec3>>& newtargetLists)
{
	m_targetLists = newtargetLists;
}

//======================================
//	�^�[�Q�b�g���X�g�擾
//======================================
const vector<vec3>& CObsManager::GetTargetList(const string& listname)
{
	static const vector<vec3> emptylist;	//�ÓI�󃊃X�g
	auto it = m_targetLists.find(listname);
	if (it != m_targetLists.end())
	{
		return it->second;
	}
	else
	{//	������Ȃ���΋󃊃X�g
		return emptylist;
	}
}
