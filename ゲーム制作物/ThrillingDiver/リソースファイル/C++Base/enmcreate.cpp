//
//	�G�r�o�����{��CPP
//
//
//

//
//	�C���N���[�h
//
#include "enmcreate.h"
#include "enemy.h"
#include <random>

//
//	�萔
//
namespace
{
	constexpr float COOLTIME = 20.0f;	//�G�𐶐��N�[���^�C��
	constexpr int REFERENCEREMAIN = 4;	//�������̎Q�ƓG����
}

//
//	�ÓI�����o�ϐ�������
//
int CEnmCreManager::m_EnmCreCnt = 0;	//	�G�����{�ݑ���
int CEnmCreManager::m_EnmCnt = 0;
LPD3DXFONT CEnmCreManager::m_pFont = nullptr;	

//
//	�R���X�g���N�^
//
CEnmCreManager::CEnmCreManager()
{

}

//
//	�f�X�g���N�^
//
CEnmCreManager::~CEnmCreManager()
{
	Uninit();
}

//
//	������
//
HRESULT CEnmCreManager::Init()
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
	
	return S_OK;
}

//
//	�I������
//
void CEnmCreManager::Uninit()
{
	if (m_pFont)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
}

//
//	�X�V����
//
void CEnmCreManager::Update()
{
	m_EnmCnt = CEnmManager::GetEnmCnt();
}

//
//	�`�揈��
//
void CEnmCreManager::Draw()
{
	
}

//
//	���\�L
//
void CEnmCreManager::DrawEnmCreInfo()
{
	if (!m_pFont) return;

	RECT rect = { 0,0,CProcess::SCREEN_WIDTH,CProcess::SCREEN_HEIGHT };
	char aStr[256];

	snprintf(&aStr[0],
		256,
		"�{�ݑ���:%d\n�擾�G����:%d",
		m_EnmCreCnt,	//�G�����{�ݑ���
		m_EnmCnt		//�擾�G����
	);

	m_pFont->DrawText(nullptr,
		&aStr[0],
		-1,
		&rect,
		DT_LEFT | DT_BOTTOM,
		D3DCOLOR_RGBA(255, 0, 0, 255));
}

//
//	�R���X�g���N�^
//
CEnmCreate::CEnmCreate() :
	m_nRemainCreCnt(0),
	m_fCreCoolTime(0.0f)
{
	Init();

	//	�������ɑ������Z
	CEnmCreManager::IncEnmCnt();
}

//
//	�f�X�g���N�^
//
CEnmCreate::~CEnmCreate()
{
	//	�폜���ɑ������Z
	CEnmCreManager::DecEnmCnt();
}

//
//	������
//
HRESULT CEnmCreate::Init()
{
	//CEnmCreManager�̏�����
	static bool isManagerInitialized = false;
	if (!isManagerInitialized)
	{
		if (FAILED(CEnmCreManager::Init()))
		{
			return E_FAIL;
		}
	}

	//CObjectX�̏�����
	if (FAILED(CObjectX::Init()))	//���_�o�b�t�@�쐬
	{
		return E_FAIL;
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

	m_ModelFile = "data\\model\\gate.x";

	return S_OK;
}

//
//	�I������	
//
void CEnmCreate::Uninit()
{
	CObjectX::Uninit();

	CEnmCreManager::Uninit();
}

//
//	�X�V����
//
void CEnmCreate::Update()
{
	CProcess* pProcess = CManager::GetProcess();

	CObjectX::Update();

	CEnmCreManager::Update();

	//�����G�T�C�Y
	D3DXVECTOR3
		E_ = { 5.0f,5.0f,5.0f }
	;

	auto
		CW = CEnmManager::m_targetLists.find("���v���"),		//	"���v���"	�Ō���
		unCW = CEnmManager::m_targetLists.find("�����v���"),	//	"�����v���"�Ō���
		Nomove = CEnmManager::m_targetLists.find("�����Ȃ�")	//	"�����Ȃ�"	�Ō���
		;

	if (CW == CEnmManager::m_targetLists.end())
	{ // ���݂��Ȃ�
		assert(false);
	}
	else if (unCW == CEnmManager::m_targetLists.end())
	{
		assert(false);
	}
	else if (Nomove == CEnmManager::m_targetLists.end())
	{
		assert(false);
	}
	
	//	�f���^�^�C�����擾
	m_fCreCoolTime += pProcess->GetDeltaTime();
	//	20�b�o���G��4�ȉ��ɂȂ�����
	if (m_fCreCoolTime >= COOLTIME || CEnmCreManager::GetEnmCnt() <= REFERENCEREMAIN)
	{
		//	�����\��������
		if (m_nRemainCreCnt > 0)
		{
			vector<string> filteredNames;
			for (const auto& name : CEnmManager::m_targetListnameLists)
			{
				//���O������������̓X�L�b�v
				if (name.find("�����Ȃ�") != string::npos)
				{
					continue;
				}
				filteredNames.push_back(name);
			}

			//�t�B���^�����O��̃��X�g����ł͂Ȃ����Ƃ��m�F����
			if (filteredNames.empty())
			{
				OutputDebugString("�����_���̑I���ł��閼�O���Ȃ�\n");
				return;
			}

			//	�^�[�Q�b�g�n�_���X�g���烉���_���œ�����I��
			//	(�~�܂遨���Ȃ�)
			size_t size = filteredNames.size();
			random_device rd;
			mt19937 gen(rd());
			uniform_int_distribution<> dist(0, size - 1);
			int rand = dist(gen);

			//�����_���l���g�p���ăC�e���[�^���擾
			string selectedName = filteredNames[rand];

			//�I�񂾖��O�ɑΉ����鏄�񃊃X�g���擾
			auto it = CEnmManager::m_targetLists.find(selectedName);
			if (it != CEnmManager::m_targetLists.end())
			{
				//	�����_���l���g���ď��񃊃X�g�����肵����
				//						������Ə�ɐ���
				CEnemy::Create(m_pos + vec3(0.0f, 200.0f, 0.0f), E_, it->second);
				--m_nRemainCreCnt;	//�����ł��鐔�����炷
			}
			else
			{
				OutputDebugString("���̖��O�̏��񃊃X�g�͂Ȃ�\n");
			}

			//������N�[���^�C�����Z�b�g
			m_fCreCoolTime = 0.0f;
		}
	}
}

//
//	�`�揈��
//
void CEnmCreate::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxNormal;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;	//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;		//�}�e���A���f�[�^�̃|�C���^

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�傫���𔽉f
	D3DXMatrixScaling(&mtxScale,
		m_scale.x,
		m_scale.y,
		m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y,
		m_rot.x,
		m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x,
		m_pos.y,
		m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X���f�o�C�X�ɐݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pTexture);

		//�v���C���[(�p�[�c)�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}
	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);

#ifdef _DEBUG
	CEnmCreManager::DrawEnmCreInfo();
#endif // _DEBUG
}

//
//	�{�ݐ���
//
CEnmCreate* CEnmCreate::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int remCre)
{
	CEnmCreate* pEnmCre = new CEnmCreate();
	pEnmCre->Init();
	pEnmCre->SetPos(pos);
	pEnmCre->SetRot(rot);
	pEnmCre->m_scale = scale;
	pEnmCre->m_nRemainCreCnt = remCre;

	return pEnmCre;
}
