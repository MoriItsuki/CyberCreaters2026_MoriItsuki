//
//	テクスチャ管理ヘッダ
//
//
//
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//	インクルード
#include "main.h"
#include "manager.h"
#include <string>

//
//	マクロ定義
// 
#define MAX_TEXTURE		(128)	//テクスチャの最大数
#define MAX_CHAR		(128)	//パスの最大文字数

//	テクスチャ管理クラス
class CTexture
{
public:
	CTexture();		//コンストラクタ
	~CTexture();	//デストラクタ
	void Unload();	//テクスチャ破棄
	void Init();	//テクスチャ読込	必ず必要なテクスチャを読み込んでおく
	int Regist(std::string pTex);	//テクスチャ登録
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);	//テクスチャ取得
private:
	LPDIRECT3DTEXTURE9 m_aTexture[MAX_TEXTURE];	//テクスチャの配列
	std::string m_TexName[MAX_TEXTURE];			//pTextureNameの保管配列
	static int m_nNumTexAll;					//テクスチャの総数
};

#endif // !_TEXTURE_H_
