//=========================================================================================================================
//	�_�b�V���Q�[�W�\��CPP(dashgauge.cpp)
//
//
//=========================================================================================================================

//========================================================
//	�C���N���[�h
//========================================================
#include "dashgauge.h"
#include "player.h"
#include <vector>
#include <cmath>

using namespace std;

//========================================================
//	�萔
//========================================================
namespace
{
	using vec3 = D3DXVECTOR3;
	using vec2 = D3DXVECTOR2;
	constexpr float SIZE_X = 50.0f;
	constexpr float SIZE_Y = 50.0f;
	constexpr int SEGMENT = 60;		//	�~�`�����̊p��
	constexpr float ARCRADIUS = 100.0f;	//�~�`�����̔��a
}

//========================================================
//	�ÓI�����o�ϐ�������
//========================================================
int CDashManager::m_DashCnt = 0;	//�_�b�V���\��
LPD3DXFONT CDashManager::m_pFont = nullptr;

//================================================================================================================
//	CDashManager
//================================================================================================================

//========================================================
//	�R���X�g���N�^
//========================================================
CDashManager::CDashManager()
{

}

//========================================================
//	�f�X�g���N�^
//========================================================
CDashManager::~CDashManager()
{

}

//========================================================
//	������
//========================================================
HRESULT CDashManager::Init()
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
	strcpy_s(fontDesc.FaceName, "Arial");			// �t�H���g�̎��

	// �t�H���g�̍쐬
	if (FAILED(D3DXCreateFontIndirect(CManager::GetRenderer()->GetDevice(), &fontDesc, &m_pFont)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//========================================================
//	�I��
//========================================================
void CDashManager::Uninit()
{
	if (m_pFont)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
}

//========================================================
//	�X�V
//========================================================
void CDashManager::Update()
{
	//�I�u�W�F�N�g�Ƃ̓����蔻��
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		//�I�u�W�F�N�g�擾
		CObject* pObj = CObject::GetObj(3, nCntObj);

		if (pObj)
		{//	�g���Ă�����
			//	�I�u�W�F�N�g�̃^�C�v���擾
			CObject::TYPE type = pObj->GetType();

			if (type == CObject::TYPE::PLAYER)
			{
				//�v���C���[�^�ɃL���X�g
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);
				//�v���C���[��`�m�F
				if (!pPlayer) return;

				m_DashCnt = pPlayer->GetDashCnt();
			}
		}
	}

}

//========================================================
//	�`��
//========================================================
void CDashManager::Draw()
{

}

//========================================================
//	�_�b�V�����\��
//========================================================
void CDashManager::DrawDashInfo()
{
	if (!m_pFont) return;

	RECT rect = { 0,0,CProcess::SCREEN_WIDTH,CProcess::SCREEN_HEIGHT };
	char aStr[256];

	snprintf(&aStr[0],
		256,
		"�_�b�V���\��:%d",
		m_DashCnt	//�G�����{�ݑ���
	);

	m_pFont->DrawText(nullptr,
		&aStr[0],
		-1,
		&rect,
		DT_CENTER | DT_BOTTOM,
		D3DCOLOR_RGBA(255, 0, 0, 255));
}

//================================================================================================================
//	CDashgauge(�e�Q�[�W�̊Ǘ��␧��)
//================================================================================================================

//========================================================
//	�R���X�g���N�^
//========================================================
CDashgauge::CDashgauge() :
	m_pArcPart(nullptr),
	m_pBarPart(nullptr),
	m_pFramePart(nullptr)
{

}

//========================================================
//	�f�X�g���N�^
//========================================================
CDashgauge::~CDashgauge()
{
	Uninit();
}

//========================================================
//	������
//========================================================
HRESULT CDashgauge::Init()
{
	//CDashManager�̏�����
	static bool isManagerInitialized = false;
	if (!isManagerInitialized)
	{
		if (FAILED(CDashManager::Init()))
		{
			return E_FAIL;
		}
	}

	// �e�Q�[�W���i��������
	try
	{
		m_pArcPart = make_unique<CArcPart>(
			ARCRADIUS,							//	�~�̔��a
			270.0f,							//	�~�ő�p�x(�l��MAX�̂Ƃ��̊p�x)
			D3DCOLOR_XRGB(255, 0, 0),		//	�n�܂�̐F
			D3DCOLOR_XRGB(255, 255, 0),		//	�I���̐F
			vec3(ARCRADIUS + 20.0f, CProcess::SCREEN_HEIGHT - (ARCRADIUS + 20.0f), 0.0f)		//	��ʏ�̈ʒu(����"0.0"�Œ�)
			);

		if (FAILED(m_pArcPart->Init()))
		{
			throw runtime_error("CArcPart initialization failed");
		}
		//m_pBarPart = make_unique<CBarPart>(200.0f, 10.0f, D3DCOLOR_XRGB(0, 255, 0), D3DCOLOR_XRGB(0, 0, 255), vec3(150.0f, 150.0f, 0.0f));
		//m_pFramePart = make_unique<CFramePart>(300.0f, 300.0f, vec3(200.0f, 200.0f, 0.0f));
	}
	catch (const exception& e)
	{
		Uninit();
		return E_FAIL;
	}

	return S_OK;
}

//========================================================
//	�I������
//========================================================
void CDashgauge::Uninit()
{
	m_pArcPart->Uninit();
	//m_pBarPart->Uninit();
	//m_pFramePart->Uninit();
}

//========================================================
//	�X�V����
//========================================================
void CDashgauge::Update(float value)
{
	m_pArcPart->SetValue(value);
	m_pArcPart->Update();
	//m_pBarPart->SetValue(value);
	//m_pBarPart->Update();

	CDashManager::Update();
}

//========================================================
//	�`�揈��
//========================================================
void CDashgauge::Draw(LPDIRECT3DDEVICE9 device)
{
	m_pArcPart->Draw(device);
	//m_pBarPart->Draw(device);
	//m_pFramePart->Draw(device);

#ifdef _DEBUG
	CDashManager::DrawDashInfo();
#endif // _DEBUG
}

//========================================================
//	�~�`�������擾
//========================================================
CArcPart* CDashgauge::GetArcGauge() const
{
	return m_pArcPart.get();
}

//========================================================
//	�����������擾
//========================================================
CBarPart* CDashgauge::GetBarGauge() const
{
	return m_pBarPart.get();
}

//========================================================
//	�O���������擾
//========================================================
CFramePart* CDashgauge::GetFrameGauge() const
{
	return m_pFramePart.get();
}

//========================================================
//	��������
//========================================================
CDashgauge* CDashgauge::Create()
{
	CDashgauge* pGauge = new CDashgauge();
	pGauge->Init();

	return pGauge;
}

//================================================================================================================
//	�_�b�V���Q�[�W�e�N���X
//================================================================================================================

//========================================================
//	�R���X�g���N�^
//========================================================
CGaugePart::CGaugePart() :
	m_value(1.0f)
{

}

//========================================================
//	�f�X�g���N�^
//========================================================
CGaugePart::~CGaugePart()
{

}

//========================================================
//	�Q�[�W�l��ݒ�
//========================================================
void CGaugePart::SetValue(float value)
{
	m_value = clamp(value, 0.0f, 1.0f);
}

//========================================================
//	�Q�[�W�l���擾
//========================================================
float CGaugePart::GetValue() const
{
	return m_value;
}

//================================================================================================================
//	�_�b�V���Q�[�W�~�`����
//================================================================================================================

//========================================================
//	�R���X�g���N�^
//========================================================
CArcPart::CArcPart(float radius, float maxAngle, D3DCOLOR startColor, D3DCOLOR endColor, const D3DXVECTOR3& pos) :
	m_radius(radius),
	m_maxAngle(maxAngle),
	m_colorStart(startColor == 0 ? D3DCOLOR_XRGB(255, 255, 255) : startColor), // �f�t�H���g��
	m_colorEnd(endColor == 0 ? D3DCOLOR_XRGB(255, 255, 255) : endColor),       // �f�t�H���g��	
	m_centerPos(pos),
	m_vertexCnt(0)
{
	m_value = 1.0f;
	m_vertexBuffer = nullptr;
}

//========================================================
//	�f�X�g���N�^
//========================================================
CArcPart::~CArcPart()
{
	Uninit();
}

//========================================================
//	������
//========================================================
HRESULT CArcPart::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	UINT vertexSize = static_cast<UINT>(sizeof(VERTEX) * (SEGMENT + 2));	//���S�_+�Z�O�����g��

	HRESULT hr =
		pDevice->CreateVertexBuffer
		(
			vertexSize,
			0,
			D3DFVF_XYZ | D3DFVF_DIFFUSE,
			D3DPOOL_MANAGED,
			&m_vertexBuffer,
			nullptr
		);
	
	if (FAILED(hr))
	{
		OutputDebugString("Failed to create vertex buffer in CArcPart::Init.\n");
		return hr;
	}

	//������Ԑݒ�
	m_value = 1.0f;	//�ő�l
	Update();	//�����l�Ɋ�Â��Ē��_�����X�V

	return S_OK;
}

//========================================================
//	�I��
//========================================================
void CArcPart::Uninit()
{
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}
}

//========================================================
//	�X�V
//========================================================
void CArcPart::Update()
{
	// �Q�[�W�̒l���X�V
	float currentAngle = D3DXToRadian(m_maxAngle * m_value);
	vector<vec2> vertices2D = GenerateArcVertices(m_radius, currentAngle, SEGMENT);

	vector<VERTEX> vertices;
	D3DCOLOR color = m_colorStart;

	// ���S�_
	vertices.push_back({ m_centerPos, 1.0f, color });

	size_t ver2Dsize = vertices2D.size();

	// �O���̒��_
	for (size_t i = 0; i < ver2Dsize; ++i)
	{
		float t = static_cast<float>(i) / ver2Dsize;
		t = clamp(t, 0.0f, 1.0f);
		color = InterpolateColor(m_colorStart, m_colorEnd, t);
		vertices.push_back(
			{
				vec3
				(
					vertices2D[i].x + m_centerPos.x,
					vertices2D[i].y + m_centerPos.y,
					m_centerPos.z
				),
				1.0f,
				color
			});
	}

	m_vertexCnt = vertices.size();
	UINT vertexSize = static_cast<UINT>(m_vertexCnt * sizeof(VERTEX));

	if (m_vertexCnt == 0)
	{
		OutputDebugString("Vertex count is zero in CArcPart::Update.\n");
		return;
	}

	// �o�b�t�@�Ƀf�[�^��������
	VERTEX* pVtx;
	if (FAILED(m_vertexBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	{
		OutputDebugString("Failed to lock vertex buffer in CArcPart::Update.\n");
		return;
	}

	memcpy(pVtx, vertices.data(), vertexSize);
	m_vertexBuffer->Unlock();
}

//========================================================
//	�`��
//========================================================
void CArcPart::Draw(LPDIRECT3DDEVICE9 device)
{
	if (!device || !m_vertexBuffer || m_vertexCnt < 3) // �O�p�`�`��ɂ͏��Ȃ��Ƃ�3�̒��_���K�v
	{
		OutputDebugString("Invalid state in CArcPart::Draw.\n");
		return;
	}

	device->SetStreamSource(0, m_vertexBuffer, 0, sizeof(VERTEX));
	device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	HRESULT hr = device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, m_vertexCnt - 2);
	if (FAILED(hr))
	{
		OutputDebugString("Failed to draw primitive in CArcPart::Draw.\n");
	}
}

//========================================================
//	�Q�[�W�̐F�ݒ�
//========================================================
void CArcPart::SetColors(D3DCOLOR startColor, D3DCOLOR endColor)
{
	m_colorStart = startColor;
	m_colorEnd = endColor;
}

//========================================================
//	�~�`�Q�[�W�̒��_�𐶐�
//========================================================
vector<D3DXVECTOR2> CArcPart::GenerateArcVertices(float radius, float angle, int segments)const
{
	vector<D3DXVECTOR2> vertices;
	for (int i = 0; i <= segments; ++i)
	{
		float t = static_cast<float>(i) / segments;
		float theta = angle * t;
		vertices.emplace_back(radius * cosf(theta), radius * sinf(theta));
	}
	return vertices;
}

//========================================================
//	�F�̕��
//========================================================
D3DCOLOR CArcPart::InterpolateColor(D3DCOLOR start, D3DCOLOR end, float t)const
{
	float
		sr = static_cast<float>((start >> 16) & 0xFF),
		sg = static_cast<float>((start >> 8) & 0xFF),
		sb = static_cast<float>(start & 0xFF),
		sa = static_cast<float>((start >> 24) & 0xFF);

	float
		er = static_cast<float>((end >> 16) & 0xFF),
		eg = static_cast<float>((end >> 8) & 0xFF),
		eb = static_cast<float>(end & 0xFF),
		ea = static_cast<float>((end >> 24) & 0xFF);

	BYTE r = static_cast<BYTE>(sr + t * (er - sr));
	BYTE g = static_cast<BYTE>(sg + t * (eg - sg));
	BYTE b = static_cast<BYTE>(sb + t * (eb - sb));
	BYTE a = static_cast<BYTE>(sa + t * (ea - sa));

	return (a << 24) | (r << 16) | (g << 8) | b;
}

//================================================================================================================
//	�_�b�V���Q�[�W��������
//================================================================================================================

//========================================================
//	�R���X�g���N�^
//========================================================
CBarPart::CBarPart(float length, float thickness, D3DCOLOR startColor, D3DCOLOR endColor, const D3DXVECTOR3& pos) :
	m_length(length),
	m_thickness(thickness),
	m_colorStart(startColor),
	m_colorEnd(endColor),
	m_startPos(pos)
{
	m_value = 0.0f;
	m_vertexBuffer = nullptr;
}

//========================================================
//	�f�X�g���N�^
//========================================================
CBarPart::~CBarPart()
{
	Uninit();
}

//========================================================
//	������
//========================================================
HRESULT CBarPart::Init()
{
	return S_OK;
}

//========================================================
//	�I��
//========================================================
void CBarPart::Uninit()
{
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}
}

//========================================================
//	�X�V
//========================================================
void CBarPart::Update()
{

}

//========================================================
//	�`��
//========================================================
void CBarPart::Draw(LPDIRECT3DDEVICE9 device)
{
	vector<vec2> vertices2D = GenerateBarVertices(m_length * m_value, m_thickness);

	struct VERTEX
	{
		vec3 pos;
		D3DCOLOR color;
	};
	
	D3DCOLOR colorStart = m_colorStart;
	D3DCOLOR colorEnd = m_colorEnd;

	//���_�f�[�^���쐬
	VERTEX vertices[4] =
	{
		{ vec3(vertices2D[0].x + m_startPos.x, vertices2D[0].y + m_startPos.y, m_startPos.z), colorStart },
		{ vec3(vertices2D[1].x + m_startPos.x, vertices2D[1].y + m_startPos.y, m_startPos.z), colorEnd },
		{ vec3(vertices2D[2].x + m_startPos.x, vertices2D[2].y + m_startPos.y, m_startPos.z), colorStart },
		{ vec3(vertices2D[3].x + m_startPos.x, vertices2D[3].y + m_startPos.y, m_startPos.z), colorEnd },
	};

	//	���_�o�b�t�@�쐬
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer = nullptr;

	if (FAILED(device->CreateVertexBuffer(
		sizeof(vertices), 
		0, 
		D3DFVF_XYZ | D3DFVF_DIFFUSE, 
		D3DPOOL_MANAGED, 
		&vertexBuffer, 
		nullptr)))
	{
		return;
	}

	//	�o�b�t�@�Ƀf�[�^��������
	VERTEX* pVtx;
	if (SUCCEEDED(vertexBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	{
		memcpy(pVtx, vertices, sizeof(vertices));
		vertexBuffer->Unlock();
	}

	//�`��
	device->SetStreamSource(0, vertexBuffer, 0, sizeof(VERTEX));
	device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//�o�b�t�@���
	vertexBuffer->Release();

}

//========================================================
//	�Q�[�W�̐F�ݒ�
//========================================================
void CBarPart::SetColors(D3DCOLOR startColor, D3DCOLOR endColor)
{
	m_colorStart = startColor;
	m_colorEnd = endColor;
}

//========================================================
//	�����Q�[�W�̒��_�𐶐�
//========================================================
std::vector<D3DXVECTOR2> CBarPart::GenerateBarVertices(float length, float thickness) const
{
	float halfThickness = thickness / 2;
	return
	{
		{-length / 2,-halfThickness},{-length / 2,halfThickness},
		{length / 2,halfThickness},{length / 2,-halfThickness}
	};
}

//========================================================
//	�F�̕ۊ�
//========================================================
D3DCOLOR CBarPart::InterpolateColor(D3DCOLOR start, D3DCOLOR end, float t) const
{
	BYTE 
		sr = (start >> 16) & 0xFF, 
		sg = (start >> 8) & 0xFF, 
		sb = start & 0xFF, 
		sa = (start >> 24) & 0xFF;

	BYTE 
		er = (end >> 16) & 0xFF, 
		eg = (end >> 8) & 0xFF, 
		eb = end & 0xFF, 
		ea = (end >> 24) & 0xFF;

	BYTE r = static_cast<BYTE>(sr + t * (er - sr));
	BYTE g = static_cast<BYTE>(sg + t * (eg - sg));
	BYTE b = static_cast<BYTE>(sb + t * (eb - sb));
	BYTE a = static_cast<BYTE>(sa + t * (ea - sa));

	return (a << 24) | (r << 16) | (g << 8) | b;
}

//================================================================================================================
//	�_�b�V���Q�[�W�O������
//================================================================================================================

//========================================================
//	�R���X�g���N�^
//========================================================
CFramePart::CFramePart(float width, float height, const D3DXVECTOR3& pos) :
	m_width(width),
	m_height(height),
	m_centerPos(pos),
	m_texture(nullptr)
{
	m_value = 0.0f;
}

//========================================================
//	�f�X�g���N�^
//========================================================
CFramePart::~CFramePart()
{
	Uninit();
}

//========================================================
//	������
//========================================================
HRESULT CFramePart::Init()
{
	return S_OK;
}

//========================================================
//	�I������
//========================================================
void CFramePart::Uninit()
{
	if (m_texture)
	{
		m_texture->Release();
		m_texture = nullptr;
	}
}

//========================================================
//	�X�V����
//========================================================
void CFramePart::Update()
{

}

//========================================================
//	�`�揈��
//========================================================
void CFramePart::Draw(LPDIRECT3DDEVICE9 device)
{
	if (m_texture || !m_texture) return;
	
	device->SetTexture(0, m_texture);

	struct VERTEX
	{
		vec3 pos;
		vec2 tex;
	};

	//�l�p�̒��_�f�[�^���쐬
	float halfWidth = m_width / 2.0f;
	float halfHeight = m_height / 2.0f;

	VERTEX vertices[4] =
	{
		{ vec3(m_centerPos.x - halfWidth, m_centerPos.y - halfHeight, m_centerPos.z), vec2(0.0f, 0.0f) },
		{ vec3(m_centerPos.x + halfWidth, m_centerPos.y - halfHeight, m_centerPos.z), vec2(1.0f, 0.0f) },
		{ vec3(m_centerPos.x - halfWidth, m_centerPos.y + halfHeight, m_centerPos.z), vec2(0.0f, 1.0f) },
		{ vec3(m_centerPos.x + halfWidth, m_centerPos.y + halfHeight, m_centerPos.z), vec2(1.0f, 1.0f) },
	};

	//	���_�o�b�t�@�쐬
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer = nullptr;

	if (FAILED(device->CreateVertexBuffer(
		sizeof(vertices),
		0,
		D3DFVF_XYZRHW | D3DFVF_TEX1,
		D3DPOOL_MANAGED,
		&vertexBuffer,
		nullptr)))
	{
		return;
	}

	//	�o�b�t�@�Ƀf�[�^��������
	VERTEX* pVtx;
	if (SUCCEEDED(vertexBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	{
		memcpy(pVtx, vertices, sizeof(vertices));
		vertexBuffer->Unlock();
	}

	//�`��
	device->SetTexture(0, m_texture);
	device->SetStreamSource(0, vertexBuffer, 0, sizeof(VERTEX));
	device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//	�o�b�t�@���
	vertexBuffer->Release();
}

//========================================================
//	�e�N�X�`���ݒ�
//========================================================
void CFramePart::SetTexture(LPDIRECT3DTEXTURE9 texture)
{
	if (m_texture)
	{
		m_texture->Release();
	}
	m_texture = texture;
	if (m_texture)
	{
		m_texture->AddRef();
	}
}
