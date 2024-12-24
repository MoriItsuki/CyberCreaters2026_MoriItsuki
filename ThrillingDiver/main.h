//
//	���C���w�b�_(main.h)
//
//
//
#ifndef _MAIN_H_
#define _MAIN_H_

//
//	�C���N���[�h
//
#define DIRECTINPUT_VERSION	(0x800)
#include<windows.h>
#include <dinput.h>
#include"d3dx9.h"	//��ʕ`��ɕK�v
//#include "renderer.h"

//���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")	//�`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")	//DirectX�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib, "winmm.lib")

//�}�N����`
#define CLASS_NAME	"WindowClass"	//�E�B���h�E�N���X�̖��O
#define WINDOW_NAME	"�E�B���h�E"	//�E�B���h�E�̖��O(�L���v�V�����\��)

//==========================================================
//	< TODO : main�ł͂Ȃ��w�b�_�Ȃǂ�����Ĉڍs >
//	clamp �w�肳�ꂽ���l�����̒l�͈͓̔��Ɏ��߂����Ƃ� 
//==========================================================
template<typename T>
constexpr const T& clamp(const T& value, const T& min, const T& max)
{
	return (value < min) ? min : (value > max ? max : value);
}

#endif // !_MAIN_H_
