//
//	メインヘッダ(main.h)
//
//
//
#ifndef _MAIN_H_
#define _MAIN_H_

//
//	インクルード
//
#define DIRECTINPUT_VERSION	(0x800)
#include<windows.h>
#include <dinput.h>
#include"d3dx9.h"	//画面描画に必要
//#include "renderer.h"

//ライブラリのリンク
#pragma comment(lib,"d3d9.lib")	//描画処理に必要
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	//DirectXコンポーネント(部品)使用に必要
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib, "winmm.lib")

//マクロ定義
#define CLASS_NAME	"WindowClass"	//ウィンドウクラスの名前
#define WINDOW_NAME	"ウィンドウ"	//ウィンドウの名前(キャプション表示)

//==========================================================
//	< TODO : mainではないヘッダなどを作って移行 >
//	clamp 指定された数値を特定の値の範囲内に収めたいとき 
//==========================================================
template<typename T>
constexpr const T& clamp(const T& value, const T& min, const T& max)
{
	return (value < min) ? min : (value > max ? max : value);
}

#endif // !_MAIN_H_
