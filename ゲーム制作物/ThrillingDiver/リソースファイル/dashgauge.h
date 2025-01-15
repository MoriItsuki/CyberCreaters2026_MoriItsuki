//
//	�_�b�V���Q�[�W�w�b�_(dashgauge.h)
//
//
//
#pragma once

//
//	�C���N���[�h
//
#include "texture.h"
#include "object2D.h"

//�_�b�V���Ǘ��N���X
class CDashManager
{
private:
	static int m_DashCnt;	//�_�b�V���ł����
	static LPD3DXFONT m_pFont;	//�t�H���g�ݒ�
public:
	CDashManager();
	~CDashManager();
	static void IncDashCnt() { ++m_DashCnt; }	//�_�b�V���\�񐔉��Z
	static void DecDashCnt() { --m_DashCnt; }	//�_�b�V���\�񐔌��Z
	static int GetDashCnt() { return m_DashCnt; }	//�_�b�V���ł���񐔎擾
	static HRESULT Init();
	static void Uninit();
	static void Update();
	static void Draw();
	static void DrawDashInfo();
};

//�_�b�V���Q�[�W�X�[�p�[�N���X
class CGaugePart
{
public:
	CGaugePart();
	virtual ~CGaugePart();

	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw(LPDIRECT3DDEVICE9 device) = 0;
	//�Q�[�W�l�̐ݒ�/�擾
	virtual void SetValue(float value);
	virtual float GetValue()const;
protected:
	float m_value;	//���i�̃Q�[�W�l
};

//�~�`�����N���X
class CArcPart :public CGaugePart
{
public:
	//���_�f�[�^���쐬
	struct VERTEX
	{
		D3DXVECTOR3 pos;
		float rhw;
		D3DCOLOR color;
	};
	CArcPart(float radius, float maxAngle, D3DCOLOR startColor, D3DCOLOR endColor, const D3DXVECTOR3& pos);
	~CArcPart()override;

	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw(LPDIRECT3DDEVICE9 device)override;
	//	�Q�[�W�F�̐ݒ�
	void SetColors(D3DCOLOR startColor, D3DCOLOR endColor);
private:
	D3DXVECTOR3 m_centerPos;//�~�`�����̒��S�_
	float m_maxAngle;		//�~�`�����̍ő�p�x
	float m_radius;			//�~�`�Q�[�W�̔��a
	D3DCOLOR m_colorStart;	//�O���f�[�V�����J�n�F
	D3DCOLOR m_colorEnd;	//�O���f�[�V�����I���F

	size_t m_vertexCnt;	//�`�悷�钸�_��

	LPDIRECT3DVERTEXBUFFER9 m_vertexBuffer;	//���_�o�b�t�@

	std::vector<D3DXVECTOR2> GenerateArcVertices(float radius, float angle, int segments) const; // �~�`���_����
	D3DCOLOR InterpolateColor(D3DCOLOR start, D3DCOLOR end, float t) const;  // �O���f�[�V�������
};

//���������N���X
class CBarPart :public CGaugePart
{
public:
	CBarPart(float length, float thickness, D3DCOLOR startColor, D3DCOLOR endColor, const D3DXVECTOR3& pos);
	~CBarPart()override;

	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw(LPDIRECT3DDEVICE9 device)override;
	//�Q�[�W�F�̐ݒ�
	void SetColors(D3DCOLOR startColor, D3DCOLOR endColor);
private:
	D3DXVECTOR3 m_startPos;	//���������̊J�n�ʒu
	float m_length;
	float m_thickness;
	D3DCOLOR m_colorStart;	//�O���f�[�V�����J�n�F
	D3DCOLOR m_colorEnd;	//�O���f�[�V�����I���F

	LPDIRECT3DVERTEXBUFFER9 m_vertexBuffer;	//���_�o�b�t�@

	std::vector<D3DXVECTOR2> GenerateBarVertices(float length, float thickness) const; // �����������_����
	D3DCOLOR InterpolateColor(D3DCOLOR start, D3DCOLOR end, float t)const;	//	�O���f�[�V�������
};

//�O�������N���X
class CFramePart :public CGaugePart
{
public:
	CFramePart(float width, float height, const D3DXVECTOR3& pos);
	~CFramePart()override;

	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw(LPDIRECT3DDEVICE9 device)override;
	//�e�N�X�`���̐ݒ�
	void SetTexture(LPDIRECT3DTEXTURE9 texture);
private:
	LPDIRECT3DTEXTURE9 m_texture;
	D3DXVECTOR3 m_centerPos;	//�t���[���̒��S�ʒu
	float m_width;				//�t���[���̕�
	float m_height;				//�t���[���̍���
};

//�_�b�V���Q�[�W�Ǘ��N���X
class CDashgauge
{
public:
	CDashgauge();
	~CDashgauge();

	HRESULT Init();
	void Uninit();
	void Update(float value);
	void Draw(LPDIRECT3DDEVICE9 device);
	CArcPart* GetArcGauge()const;
	CBarPart* GetBarGauge()const;
	CFramePart* GetFrameGauge()const;

	static CDashgauge* Create();
private:
	std::unique_ptr<CArcPart> m_pArcPart;
	std::unique_ptr<CBarPart> m_pBarPart;
	std::unique_ptr<CFramePart> m_pFramePart;
};
