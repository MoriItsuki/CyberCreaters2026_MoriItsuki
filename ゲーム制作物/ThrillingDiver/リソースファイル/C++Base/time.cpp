//
//	���ԕ\��CPP
//
//
//

//
//	�C���N���[�h
//
#include "time.h"
#include "process.h"

using namespace std;

//
//	�萔
//
namespace
{
	using vec3 = D3DXVECTOR3;
	using vec2 = D3DXVECTOR2;
	constexpr int DIVISION = 10;	//�����e�N�X�`���̕�����
	constexpr float TIME_X = 60.0f;
	constexpr float TIME_Y = 40.0f;
	constexpr float TIME_XL = 120.0f;
	constexpr float TIME_YL = 080.0f;
}

//
//	�ÓI�����o�ϐ�������
//
const string CTimer::FILE_TIME = "data\\texture\\numbers.png";

//
//	�R���X�g���N�^
//
CTimer::CTimer() :
	m_digit(-1),
	m_bUse(true)
{

}

//
//	�f�X�g���N�^
//
CTimer::~CTimer()
{

}

//
//	������
//
HRESULT CTimer::Init()
{
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	m_pos = vec3(CProcess::SCREEN_WIDTH * 0.5, CProcess::SCREEN_HEIGHT * 0.5, 0.0f);
	m_size = vec3(TIME_X, TIME_Y, 0.0f);

	//�e�N�X�`���쐬
	CTexture* pTex = CManager::GetTexture();	//�e�N�X�`�����擾
	BindTexture(pTex->GetAddress(pTex->Regist(FILE_TIME)));	//�ݒ�

	return S_OK;
}

//
//	�I������
//
void CTimer::Uninit()
{
	CObject2D::Uninit();
}

//
//	�X�V����
//
void CTimer::Update()
{
	if (m_digit == -1)
	{
		m_bUse = false;
	}

	VERTEX_2D* pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = vec3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[1].pos = vec3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = vec3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = vec3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	//	�����ɉ����������T�C�Y
	float divX = 1.0f / DIVISION;

	if (m_digit != 11)
	{
		//���l���Ƃ̃e�N�X�`�����W�̐ݒ�
		float divXIn = m_digit * divX;
		float divXOut = (m_digit + 1) * divX;

		pVtx[0].tex = vec2(divXIn, 0.0f);
		pVtx[1].tex = vec2(divXOut, 0.0f);
		pVtx[2].tex = vec2(divXIn, 0.5f);
		pVtx[3].tex = vec2(divXOut, 0.5f);
	}
	else
	{//
		pVtx[0].tex = vec2(0.0f, 0.0f);
		pVtx[1].tex = vec2(0.0f, 0.0f);
		pVtx[2].tex = vec2(0.0f, 0.5f);
		pVtx[3].tex = vec2(0.0f, 0.5f);
	}

	//rhw�̐ݒ�(�Œ�)	���_�J���[�̐ݒ�
	for (int nCnt = 0; nCnt < CProcess::MAX_VERTEX; nCnt++)
	{
		pVtx[nCnt].rhw = 1.0f;
		pVtx[nCnt].col = D3DCOLOR_RGBA(0, 0, 0, 255);
	}

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//
//	�`�揈��
//
void CTimer::Draw()
{
	CRenderer* pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pRendererDevice = pRenderer->GetDevice();	//�f�o�C�X�̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pRendererDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pRendererDevice->SetFVF(CProcess::FVF_VERTEX_2D);

	if (m_bUse)
	{
		//�e�N�X�`���̐ݒ�
		pRendererDevice->SetTexture(0, m_pTexture);

		pRendererDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);
	}
}

//
//	�\�����l�ݒ�
//
void CTimer::SetDigit(int digit)
{
	m_digit = digit;

	if (digit != 11)
	{
		if (digit < 0 || digit >= DIVISION)
		{
			m_digit = -1;
		}
	}
}

//
//	��ʒu�ݒ�
//
void CTimer::SetPos(const D3DXVECTOR3& pos)
{
	m_pos = pos;
}

//
//	��������
//
CTimer* CTimer::Create(D3DXVECTOR3 pos)
{
	CTimer* pTimer = new CTimer;
	pTimer->Init();
	pTimer->m_pos = pos;

	return pTimer;
}


//
//	�g�p�󋵐ݒ�
//
void CTimer::SetUse(bool buse)
{
	m_bUse = buse;
}