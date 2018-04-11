//========================================================================================
//
//	※[dinput.h]を読込む際にしなければいけないこと。
//
//		[ﾂｰﾙ]→[ｵﾌﾟｼｮﾝ]→[ﾌﾟﾛｼﾞｪｸﾄおよびｿﾘｭｰｼｮﾝ]→
//			[全般]→[VC++ ﾃﾞｨﾚｸﾄﾘ]→[ﾃﾞｨﾚｸﾄﾘを表示するﾌﾟﾛｼﾞｪｸﾄ]→
//				[ｲﾝｸﾙｰﾄﾞ ﾌｧｲﾙ]
//					C:\Program Files\Microsoft DirectX SDK (June 2010)\Include
//				[ﾗｲﾌﾞﾗﾘ ﾌｧｲﾙ]
//					C:\Program Files\Microsoft DirectX SDK (June 2010)\Lib\x86
//
//		[ﾌﾟﾛｼﾞｪｸﾄ]→[○○のﾌﾟﾛﾊﾟﾃｨ]→[構成ﾌﾟﾛﾊﾟﾃｨ]→
//			[ﾘﾝｶ]→[入力]→[追加の依存ﾌｧｲﾙ]
//					dxguid.lib dinput8.lib			←DirectInput で使用する必要最低限。
//
//		追加依存ﾌｧｲﾙは、下記の #pragma comment( lib, "○○.lib") で代用可能。
//		他のプロジェクトにコードを移植することを考えるとこちらの方が楽でよい。
//
//========================================================================================

#ifndef _DX_INPUT_H_
#define _DX_INPUT_H_

#include <dinput.h>	// DirectInput用.

// DirectX 関係.
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "dinput8.lib" )	// コントローラ操作で使用.

//列挙体宣言.
//ｷｰ情報.
enum enPKey{
	enPKey_Up,
	enPKey_Down,
	enPKey_Left,
	enPKey_Right,
	enPKey_00,
	enPKey_01,
	enPKey_02,
	enPKey_03,
	enPKey_04,
	enPKey_05,
	enPKey_06,
	enPKey_07,
	enPKey_08,
	enPKey_09,
	enPKey_10,
	enPKey_11,

	enPKey_Max,	//ｷｰ最大数.
};

class clsDxInput
{
public:
	clsDxInput();
	~clsDxInput();

	//DirectInput初期設定.
	bool initDI(HWND hWnd);

	//入力情報更新関数.
	HRESULT UpdateInputState();

	//入力情報を追加する関数.
	void AddInputState(enPKey enKey);

	//入力情報を初期化する関数.
	void InitInputState();

	//入力ﾁｪｯｸする関数.
	bool IsPressKey(enPKey enKey);

private:
	LPDIRECTINPUT8			m_pDI;//DxInputｵﾌﾞｼﾞｪｸﾄ.
	LPDIRECTINPUTDEVICE8	m_pPad;//ﾃﾞﾊﾞｲｽ(ｺﾝﾄﾛｰﾗ)ｵﾌﾞｼﾞｪｸﾄ.

	unsigned int			m_uInputState;//入力情報.
};

#endif//#ifndef _DX_INPUT_H_
