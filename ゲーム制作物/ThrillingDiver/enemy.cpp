//======================================================================================
//	�GCPP(enemy.cpp)
//
//
//======================================================================================

//==========================================================
//	�C���N���[�h
//==========================================================
#include "enemy.h"
#include "manager.h"
#include "texture.h"
#include "player.h"
#include "dirmarker.h"

//==========================================================
//	�萔
//==========================================================
namespace
{
	using vec3 = D3DXVECTOR3;
	constexpr float ENMSIZE = 100.0f;	//�G�̑傫��
	constexpr float BLOWTIMER = 180.0f;	//PL�ڐG���������܂ł̎���
	constexpr float BLOWSCALE = 50.0f;	//PL�ڐG���̔��ł����̃x�N�g���ւ̔{��
	const vec3 BlowVecNor = { 0.0f,5.0f,0.0f };
}

//==========================================================
//	�ÓI�����o�֐�������
//==========================================================
const std::string CEnemy::FILE_ENEMY = "data\\texture\\BG.png";
LPDIRECT3DTEXTURE9 CEnemy::m_textures[ENMTYPE::MAX] = {};
int CEnmManager::m_EnmCnt = 0;	//0������
//�^�[�Q�b�g�n�_���X�g
map<string, vector<vec3>> CEnmManager::m_targetLists =	
{
	{//	���v����̏���
		"���v���",
		{
			vec3(+4750.0f, 0.0f, -4750.0f),
			vec3(-4750.0f, 0.0f, -4750.0f),
			vec3(-4750.0f, 0.0f, +4750.0f),
			vec3(+4750.0f, 0.0f, +4750.0f)
		}
	},
	{//	�����v���̏���
		"�����v���",
		{
			vec3(-4250.0f, 0.0f, +4250.0f),
			vec3(-4250.0f, 0.0f, -4250.0f),
			vec3(+4250.0f, 0.0f, -4250.0f),
			vec3(+4250.0f, 0.0f, +4250.0f)
		}
	},
	{//	���񂵂Ȃ�(�~�܂�)
		"�����Ȃ�",
		{
			
		}
	}
};
//�^�[�Q�b�g�n�_���X�g�̖��O���X�g
vector<string> CEnmManager::m_targetListnameLists = 	
{
	"���v���",
	"�����v���",
	"�����Ȃ�"
};

//==========================================================
//	�R���X�g���N�^
//==========================================================
CEnemy::CEnemy(std::vector<vec3> value) :
	CObjectX(),
	m_EnmType(ENMTYPE::NONE),
	m_moveValue(0.0f),
	m_Oldpos(0.0f, 0.0f, 0.0f),
	m_bLand(false),
	m_move{ 0.0f, 0.0f, 0.0f },
	m_movePnt(value),
	m_movePntIdx(0),
	m_InitPos(false),
	m_currentState(nullptr),
	m_blowTime(BLOWTIMER),
	m_pFont(nullptr),
	m_diffPL(0.0f),
	m_directionPL(0.0f, 0.0f, 0.0f),
	m_BlowDir(0.0f, 0.0f, 0.0f)
{
	Init();

	//	�J�E���g�A�b�v
	CEnmManager::IncEnmCnt();
}

//==========================================================
//	�f�X�g���N�^
//==========================================================
CEnemy::~CEnemy()
{
	//	�J�E���g�_�E��
	//CEnmManager::DecEnmCnt();
}

//==========================================================
//	�����ݒ�(���_�o�b�t�@����)
//==========================================================
HRESULT CEnemy::Init()
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

	m_pos = vec3(0.0f, 0.0f, 0.0f);

	CObject::SetType(ENEMY);

	m_moveValue = 0.5f;

	m_ModelFile = "data\\model\\enemy.x";

	m_scale = { 1.0f, 1.0f, 1.0f };

	//�e�N�X�`���쐬
	CTexture* pTex = CManager::GetTexture();	//�e�N�X�`�����擾
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_ENEMY)));	//�ݒ�

	SetState(new NormalState());

	return S_OK;
}

//==========================================================
//	�I������(���_�o�b�t�@�̔j��)
//==========================================================
void CEnemy::Uninit()
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
void CEnemy::Update()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	CObjectX::Update();

	if (m_currentState)
	{//	��Ԃ��Ƃ̍X�V���������œ����
		m_currentState->Addupdate(this);
		
		if (dynamic_cast<KnockbackState*>(m_currentState) != nullptr)
		{//	���
			if (m_blowTime > 0.0f)
			{
				m_blowTime -= 1.0f;

				if (m_blowTime <= 0.0f)
				{
					this->Release();
				}
			}

			return;
		}
	}

	CInputKeyboard* pKeyboard = CManager::GetKeyboard();
	
	if (!m_movePnt.empty())
	{//	�^�[�Q�b�g�n�_���X�g����łȂ����
		if (!m_InitPos)
		{
			//�ŋ߃^�[�Q�b�g�ʒu���������AID�Z�b�g
			float minDistance = FLT_MAX;

			//�^�[�Q�b�g�n�_�̍ő�擾																		
			size_t size = m_movePnt.size();
			for (size_t i = 0; i < size; i++)
			{
				vec3 diff = m_movePnt[i] - m_pos;
				float distance = D3DXVec3Length(&diff);
				if (distance < minDistance)
				{
					minDistance = distance;
					m_movePntIdx = i;
				}
			}
			m_InitPos = true;	//1�񂵂����Ȃ�
		}

		//	���݂̃^�[�Q�b�g�n�_���擾
		vec3 tgtPos = m_movePnt[m_movePntIdx];

		//	���݂̈ʒu�ƃ^�[�Q�b�g�ʒu�̕����x�N�g�����Z�o
		vec3 direction = tgtPos - m_pos;

		//�ړ��ʂ��w��
		float speed = 20.0f;

		//�P�ʃx�N�g���ɐ��K��
		D3DXVec3Normalize(&direction, &direction);

		//�������ړ������ɍ��킹��
		if (D3DXVec3Length(&direction) > 0.0f)
		{
			//z����O�����Ƃ����ꍇ�A�ړ������x�N�g�������]���v�Z
			float rotationY = atan2f(-direction.x, -direction.z);	//y������̉�]�p���v�Z
			m_rot.y = rotationY;	//y����]�ɔ��f
		}

		//	�x�N�g���ɑ����������Ĉړ��l���v�Z
		m_move = direction * speed;

		//	�ړ��l���Z
		m_pos += m_move;

		//	�^�[�Q�b�g�n�_�ɋ߂��ꍇ�Ɏ��̃^�[�Q�b�g���w�肷��
		vec3 targetDiff = tgtPos - m_pos;
		float distanceToTarget = D3DXVec3Length(&targetDiff);

		//	���ݒn����^�[�Q�b�g�n�_�܂ł̋��������������Ȃ�
		size_t size = m_movePnt.size();
		if (distanceToTarget < speed)
		{//	���̃^�[�Q�b�g�n�_���w�肷��
			m_movePntIdx = (m_movePntIdx + 1) % size;
		}
	}

	//�ŋߓG�Ƃ̋���
	float closestDistance = FLT_MAX;	//�����l�ݒ�
	CEnemy* pNearestEnemy = nullptr;	//�ŋߓG�p�ϐ�

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
				vec3 PLPos = pPlayer->GetPos();

				m_directionPL = m_pos - PLPos;
				//	�x�N�g���p
				vec3 collisionDirection = m_directionPL;

				D3DXVec3Normalize(&m_BlowDir, &collisionDirection);

				//	�����̗ݏ���v�Z
				m_diffPL = D3DXVec3LengthSq(&m_directionPL);

				//	�v���C���[�����̓G�̓����蔻������ǂ����𔻒�
				if (m_diffPL <= ENMSIZE * ENMSIZE)
				{
					//	���ł�����Ԃɂ���
					SetState(new KnockbackState);

					//	���������u�Ԃɐ������炷
					CEnmManager::DecEnmCnt();
				}
			}
		}
	}
}

//==========================================================
//	�`�揈��(�|���S���`��)
//==========================================================
void CEnemy::Draw()
{
	CObjectX::Draw();

#ifdef _DEBUG
	DrawEnmInfo();
#endif // _DEBUG
}

//==========================================================
//	�G����
//==========================================================
CEnemy* CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, std::vector<vec3> movePos)
{
	CEnemy* pEnemy = new CEnemy(movePos);

	pEnemy->Init();
	pEnemy->SetPos(pos);
	pEnemy->m_scale = scale;
	pEnemy->m_movePnt = movePos;
	
	return pEnemy;
}

//================================================
//	�e�N�X�`���擾
//================================================
LPDIRECT3DTEXTURE9 CEnemy::GetTextures(const TYPE& inType)
{
	return m_textures[inType];
}

//================================================
//	���\�L
//================================================
void CEnemy::DrawEnmInfo()
{
	RECT rect = { 0,0,CProcess::SCREEN_WIDTH,CProcess::SCREEN_HEIGHT };
	char aStr[256];
	float judgedis = ENMSIZE * ENMSIZE;

	snprintf(&aStr[0],
		256,
		"�G����:%d\ntimer:%f\n��������:%f\n���苗��%f",
		CEnmManager::GetEnmCnt(),
		m_blowTime,
		m_diffPL,
		judgedis
	);

	m_pFont->DrawText(nullptr,
		&aStr[0],
		-1,
		&rect,
		DT_CENTER,
		D3DCOLOR_RGBA(255, 0, 0, 255));
}


//================================================
//	�G�l�~�[�̏�Ԑݒ�
//================================================
void CEnemy::SetState(EnemyState* pNewstate)
{
	if (m_currentState)
	{
		//�Â���Ԃ̍폜
		delete m_currentState;
		m_currentState = nullptr;
	}

	//�V������Ԃ̐ݒ�
	m_currentState = pNewstate;
}

//================================================
//	������ԓ���
//================================================
void CEnemy::BlowOff()
{		
	vec3 BlowAmount = m_BlowDir * BLOWSCALE + BlowVecNor;
	SetPos(m_pos + BlowAmount);
	m_rot.y += (D3DX_PI * 0.05f);
	m_rot.x += (D3DX_PI * 0.05f);
}

//==============================================
//	��Ԃ��Ƃ̒ǉ��X�V����
//==============================================

//================================================
//	�ʏ�ړ���Ԃ̒ǉ��X�V����
//================================================
void NormalState::Addupdate(CEnemy* enemy)
{
	//����ʏ��Ԃł̎�Ȓǉ��X�V�̓i�V
}

//================================================
//	��΂��ꂽ�Ƃ��̒ǉ��X�V����
//================================================
void KnockbackState::Addupdate(CEnemy* enemy)
{
	//	��΂���鏈��
	enemy->BlowOff();
}

//================================================
//	�^�[�Q�b�g���X�g�̐ݒ�
//================================================
void CEnmManager::SetTargetLists(const map<string, vector<vec3>>& newtargetLists)
{
	m_targetLists = newtargetLists;
}

//================================================
//	�^�[�Q�b�g���X�g�擾
//================================================
const vector<vec3>& CEnmManager::GetTargetList(const string& listname)
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
