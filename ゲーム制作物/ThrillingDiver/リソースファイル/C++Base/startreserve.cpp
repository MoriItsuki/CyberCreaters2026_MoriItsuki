//========================================================================================================================
//	�X�^�[�g�����pCPP(startreserve.cpp)
//
//
//========================================================================================================================

//
//	�C���N���[�h
//
#include "startreserve.h"
#include "process.h"
#include "texture.h"

//
//	�萔
//
namespace
{
	using vec2 = D3DXVECTOR2;
	using vec3 = D3DXVECTOR3;
	
	constexpr int DIVISION_SPELL = 2;	//�����̕�����
	constexpr int DIVISION_NUMBER = 10;	//�����̕�����
	constexpr float PRESSSTART = 4.0f;
}

//
//	�ÓI�����o�ϐ�������
//
const std::string CStartReserve::FILE_STARTRESERVE = "data\\texture\\.png";
const std::string CStartConfirm::FILE_STARTCONFIRM = "data\\texture\\confirm.png";
const std::string CStartSpell::FILE_STARTSPELL = "data\\texture\\startspell.png";
const std::string CPressTime::FILE_NUMBER = "data\\texture\\numbers.png";

//========================================================================================================================
//	�w�i���߃|���S��
//========================================================================================================================

//
//	�R���X�g���N�^
//
CStartReserve::CStartReserve() :
	m_pConfirm(nullptr),
	m_pSpell(nullptr),
	m_bstart(false)
{

}

//
//	�f�X�g���N�^
//
CStartReserve::~CStartReserve()
{

}

//
//	������
//
HRESULT CStartReserve::Init()
{
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	//�F�����l�ݒ�
	SetColor(D3DCOLOR_RGBA(0, 0, 0, 150));

	//	�m�F�A�C�R��������
	m_pConfirm = CStartConfirm::Create(m_pos + vec3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(200.0f, 200.0f, 0.0f));
	m_pSpell = CStartSpell::Create(m_pos + vec3(0.0f, 300.0f, 0.0f), D3DXVECTOR3(300.0f, 40.0f, 0.0f));

	//�e�N�X�`���쐬
	CTexture* pTex = CManager::GetTexture();	//�e�N�X�`�����擾
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_STARTRESERVE)));	//�ݒ�

	return S_OK;
}

//
//	�I������
//
void CStartReserve::Uninit()
{
	m_pConfirm->Uninit();
	m_pSpell->Uninit();

	CObject2D::Uninit();
}

//
//	�X�V����
//
void CStartReserve::Update()
{
	CObject2D::Update();

	if (m_pConfirm->IsLBPressed() && 
		m_pConfirm->IsRBPressed())
	{
		m_pSpell->SetNum(1);
	}
	else
	{
		m_pSpell->SetNum(0);
	}

	if (m_pConfirm->IsStartCan())
	{
		m_bstart = true;
	}
}

//
//	�`��
//
void CStartReserve::Draw()
{
	CObject2D::Draw();
}

//
//	����
//
CStartReserve* CStartReserve::Create()
{
	CStartReserve* pstartReserve = new CStartReserve();
	pstartReserve->Init();
	auto
		pos = D3DXVECTOR3(CProcess::SCREEN_WIDTH * 0.5f, CProcess::SCREEN_HEIGHT * 0.5, 0.0f),
		size = D3DXVECTOR3(CProcess::SCREEN_WIDTH * 0.5f, CProcess::SCREEN_HEIGHT * 0.5, 0.0f)
		;

	pstartReserve->SetPos(pos);
	pstartReserve->SetSize(size);

	return pstartReserve;
}

//
//	�X�^�[�g�ł��邩�擾
//
bool CStartReserve::IsStart() const
{
	return m_bstart;
}

//========================================================================================================================
//	�m�F�|���S��
//========================================================================================================================

//
//	�R���X�g���N�^
//
CStartConfirm::CStartConfirm() :
	m_isLBPressed(false),
	m_isRBPressed(false),
	m_currentColor(D3DCOLOR_XRGB(255, 255, 255)),	//�������͔�
	m_fLongPressTime(PRESSSTART),
	m_bstart(false),
	m_pPressTime(nullptr)
{

}

//
//	�f�X�g���N�^
//
CStartConfirm::~CStartConfirm()
{

}

//
//	������
//
HRESULT CStartConfirm::Init()
{
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	m_pPressTime = CPressTime::Create(-1, m_pos + vec3(-140.0f, 290.0f, 0.0f), vec3(30.0f, 50.0f, 0.0f));

	//�e�N�X�`���쐬
	CTexture* pTex = CManager::GetTexture();	//�e�N�X�`�����擾
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_STARTCONFIRM)));	//�ݒ�

	return S_OK;
}

//
//	�I������
//
void CStartConfirm::Uninit()
{
	m_pPressTime->Uninit();

	CObject2D::Uninit();
}

//
//	�X�V����
//
void CStartConfirm::Update()
{
	CProcess* pProcess = CManager::GetProcess();

	//���͏�Ԃ��擾
	CInputGamePad* pPad = CManager::GetGamepad();
	m_isLBPressed = pPad->GetPress(4);
	m_isRBPressed = pPad->GetPress(5);

	VERTEX_2D* pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	CObject2D::Update();

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//rhw�̐ݒ�(�Œ�)
	//���_�J���[�̐ݒ�
	for (int nCnt = 0; nCnt < CProcess::MAX_VERTEX; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
	}

	auto white = D3DCOLOR_XRGB(255, 255, 255);

	//�F�̏�Ԃ��X�V
	if (m_isLBPressed && m_isRBPressed)
	{
		m_currentColor = D3DCOLOR_XRGB(255, 255, 0);	//���F
		pVtx[0].col = m_currentColor;
		pVtx[1].col = m_currentColor;
		pVtx[2].col = m_currentColor;
		pVtx[3].col = m_currentColor;

		//	���F��Ԃ̎���
		m_fLongPressTime -= pProcess->GetDeltaTime();
		m_pPressTime->SetValue((int)m_fLongPressTime);
	}
	else if (m_isLBPressed)
	{
		m_currentColor = D3DCOLOR_XRGB(255, 100, 0);	//���F
		pVtx[0].col = m_currentColor;
		pVtx[1].col = m_currentColor;
		pVtx[2].col = white;
		pVtx[3].col = white;

		m_fLongPressTime = PRESSSTART;
		m_pPressTime->SetValue(-1);
	}
	else if (m_isRBPressed)
	{
		m_currentColor = D3DCOLOR_XRGB(100, 0, 255);	//���F
		pVtx[0].col = white;
		pVtx[1].col = white;
		pVtx[2].col = m_currentColor;
		pVtx[3].col = m_currentColor;

		m_fLongPressTime = PRESSSTART;
		m_pPressTime->SetValue(-1);
	}
	else
	{
		m_currentColor = D3DCOLOR_XRGB(255, 255, 255);	//���F
		pVtx[0].col = white;
		pVtx[1].col = white;
		pVtx[2].col = white;
		pVtx[3].col = white;

		m_fLongPressTime = PRESSSTART;
		m_pPressTime->SetValue(-1);
	}

	if (m_fLongPressTime <= 0.0f)
	{
		m_bstart = true;
		m_fLongPressTime = PRESSSTART;
	}

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//
//	�`�揈��
//
void CStartConfirm::Draw()
{
	CObject2D::Draw();
}

//
//	��������
//
CStartConfirm* CStartConfirm::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CStartConfirm* pStartCon = new CStartConfirm();
	pStartCon->Init();
	pStartCon->SetPos(pos);
	pStartCon->SetSize(size);

	return pStartCon;
}

//
//	LB�t���O
//
bool CStartConfirm::IsLBPressed() const
{
	return m_isLBPressed;
}

//
//	RB�t���O
//
bool CStartConfirm::IsRBPressed() const
{
	return m_isRBPressed;
}

//
//	�X�^�[�g�ł��邩�擾
//
bool CStartConfirm::IsStartCan() const
{
	return m_bstart;
}

//========================================================================================================================
//	�m�F�|���S��
//========================================================================================================================

//
//	�R���X�g���N�^
//
CStartSpell::CStartSpell() :
	m_nNum(0)
{

}

//
//	�f�X�g���N�^
//
CStartSpell::~CStartSpell()
{

}

//
//	������
//
HRESULT CStartSpell::Init()
{
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}
	
	m_color = D3DCOLOR_XRGB(255, 0, 0);

	m_nNum = 0;

	//�e�N�X�`���쐬
	CTexture* pTex = CManager::GetTexture();	//�e�N�X�`�����擾
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_STARTSPELL)));	//�ݒ�

	return S_OK;
}

//
//	�I������
//
void CStartSpell::Uninit()
{
	CObject2D::Uninit();
}

//
//	�X�V
//
void CStartSpell::Update()
{
	CObject2D::Update();

	VERTEX_2D* pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = vec3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[1].pos = vec3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = vec3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = vec3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	//	�����ɉ����������T�C�Y
	float divY = 1.0f / DIVISION_SPELL;

	pVtx[0].tex = vec2(0.0f, divY * m_nNum);
	pVtx[1].tex = vec2(1.0f, divY * m_nNum);
	pVtx[2].tex = vec2(0.0f, divY * (m_nNum + 1));
	pVtx[3].tex = vec2(1.0f, divY * (m_nNum + 1));

	//rhw�̐ݒ�(�Œ�)	���_�J���[�̐ݒ�
	for (int nCnt = 0; nCnt < CProcess::MAX_VERTEX; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
		pVtx[nCnt].col = m_color;
	}

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//
//	�`��
//
void CStartSpell::Draw()
{
	CObject2D::Draw();
}

//
//	����
//
CStartSpell* CStartSpell::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CStartSpell* pSpell = new CStartSpell();
	pSpell->Init();
	pSpell->SetPos(pos);
	pSpell->SetSize(size);

	return pSpell;
}

//
//	�������l�ݒ�
//
void CStartSpell::SetNum(int num)
{
	m_nNum = num;
}

//========================================================================================================================
//	���������Ԓl1���\���p�|���S��
//========================================================================================================================

//
//	�R���X�g���N�^
//
CPressTime::CPressTime() :
	m_value(-1),
	m_isVisible(false)
{

}

//
//	�f�X�g���N�^
//
CPressTime::~CPressTime()
{

}

//
//	������
//
HRESULT CPressTime::Init()
{
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	m_isVisible = true;

	//�e�N�X�`���쐬
	CTexture* pTex = CManager::GetTexture();	//�e�N�X�`�����擾
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_NUMBER)));	//�ݒ�

	return S_OK;
}

//
//	�I��
//
void CPressTime::Uninit()
{
	CObject2D::Uninit();
}

//
//	�X�V
//
void CPressTime::Update()
{
	CObject2D::Update();

	VERTEX_2D* pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = vec3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[1].pos = vec3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = vec3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = vec3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	//	�����ɉ����������T�C�Y
	float divX = 1.0f / DIVISION_NUMBER;
	
	pVtx[0].tex = vec2(divX * m_value, 0.0f);
	pVtx[1].tex = vec2(divX * (m_value + 1), 0.0f);
	pVtx[2].tex = vec2(divX * m_value, 0.5f);
	pVtx[3].tex = vec2(divX * (m_value + 1), 0.5f);

	//rhw�̐ݒ�(�Œ�)	���_�J���[�̐ݒ�
	for (int nCnt = 0; nCnt < CProcess::MAX_VERTEX; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
		pVtx[nCnt].col = m_color;
	}

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//
//	�`��
//
void CPressTime::Draw()
{
	if (m_isVisible)
	{
		CObject2D::Draw();
	}
}

//
//	����
//
CPressTime* CPressTime::Create(int value, D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CPressTime* pPressTime = new CPressTime();
	pPressTime->Init();
	pPressTime->SetValue(value);
	pPressTime->SetPos(pos);
	pPressTime->SetSize(size);

	return pPressTime;
}

//
//	���l�ݒ�
//
void CPressTime::SetValue(int value)
{
	if (value < -1 || value > 9)
	{
		return;
	}

	if (value == -1)
	{
		//����l -1: ��\���ɂ���
		m_isVisible = false;
	}
	else
	{
		//�ʏ�͈�
		m_isVisible = true;
		m_value = value;
	}
}
