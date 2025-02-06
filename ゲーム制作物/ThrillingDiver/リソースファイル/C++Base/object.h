//
//	�I�u�W�F�N�g�w�b�_(object.h)
//
//
//
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"

//�}�N����`
#define MAX_OBJECT	(1024)	//�I�u�W�F�N�g�̍ő吔
#define NUM_PRIORITY	(64)	//�v���C�I���e�B�̍ő吔

//�I�u�W�F�N�g���N���X
class CObject
{
public:
	typedef enum
	{
		NONE = 0,	//����
		ENEMY,		//�G
		BLOCK,		//�u���b�N
		PLAYER,		//�v���C���[
		OBSTACLE,	//��Q��
		DIRMARKER,	//�����\��
		ITEM,		//�A�C�e��
		MAX
	}TYPE;

	CObject(int nPriority = 3);	//�R���X�g���N�^ �`��D�揇�ݒ�
	virtual ~CObject();			//�f�X�g���N�^
	virtual HRESULT Init() = 0;	//�����ݒ�
	virtual void Uninit() = 0;	//�I������
	virtual void Update() = 0;	//�X�V����
	virtual void Draw() = 0;	//�`�揈��
	static void ReleaseAll();	//�S�I�u�W�F�N�g���
	static void UpdateAll();	//�S�I�u�W�F�N�g�X�V
	static void DrawAll();		//�S�I�u�W�F�N�g�`��
	static CObject* GetObj(int nPri, int nIdx);	//�I�u�W�F�N�g�擾
	TYPE GetType();				//�^�C�v�擾
	void SetType(TYPE type);	//�^�C�v�ݒ�
protected:
	void Release();			//�������g�̉��
private:
	static CObject* m_apObject[NUM_PRIORITY][MAX_OBJECT];	//�I�u�W�F�N�g�Ǘ�
	int m_nPriority;	//�`��D��x
	static int m_nNumAll;			//�I�u�W�F�N�g����
	int m_nID;						//�������g��ID
	TYPE m_type;		//�I�u�W�F�N�g�^�C�v
};
#endif // !_OBJECT_H_

