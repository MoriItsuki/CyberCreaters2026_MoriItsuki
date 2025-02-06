//====================================================================================
//	背景ヘッダ(BG.h)
//
//
//====================================================================================
#ifndef _BG_H_
#define _BG_H_

//===================================================
//	インクルード
//===================================================
#include "main.h"
#include "object2D.h"
#include "texture.h"

//	背景管理クラス
class CBG :public CObject2D
{
public:
	typedef enum
	{
		TITLE = 0,
		GAME,
		RESULT,
		MAX
	}TYPE;
	CBG(int nPriority = 2);
	~CBG()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CBG* Create(TYPE type);	//BGの生成
private:
	float m_PosTexBGU;
	float m_PosTexBGV;
	TYPE m_textureType;
	static const std::string FILE_BG[MAX];
};
#endif // !_BG_H_
