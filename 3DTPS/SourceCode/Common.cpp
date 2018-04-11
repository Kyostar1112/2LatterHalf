#include "Common.h"

//============================================================
//“§‰ß(±ÙÌ§ÌÞÚÝÄÞ)Ý’è‚ÌØ‚è‘Ö‚¦.
//============================================================
void clsCommon::SetBlend(bool flg)
{
	//±ÙÌ§ÌÞÚÝÄÞ—pÌÞÚÝÄÞ½Ã°Ä\‘¢‘Ì.
	//pngÌ§²Ù“à‚É±ÙÌ§î•ñ‚ª‚ ‚é‚Ì‚ÅA
	//“§‰ß‚·‚é‚æ‚¤‚ÉÌÞÚÝÄÞ½Ã°Ä‚ðÝ’è‚·‚é.
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));//‰Šú‰».

	blendDesc.IndependentBlendEnable
		= false;//false:RenderTarget[0]‚ÌÒÝÊÞ°‚Ì‚Ý‚ªŽg—p‚·‚é.
	//true :RenderTarget[0`7]‚ªŽg—p‚Å‚«‚é.
	//      (ÚÝÀÞ°À°¹Þ¯Ä–ˆ‚É“Æ—§‚µ‚½ÌÞÚÝÄÞˆ—)
	blendDesc.AlphaToCoverageEnable
		= false;//true :±ÙÌ§Ä©¶ÊÞÚ¯¼Þ‚ðŽg—p‚·‚é.
	blendDesc.RenderTarget[0].BlendEnable
		= flg;	//true :±ÙÌ§ÌÞÚÝÄÞ‚ðŽg—p‚·‚é.
	blendDesc.RenderTarget[0].SrcBlend	//Œ³‘fÞ‚É‘Î‚·‚éÝ’è.
		= D3D11_BLEND_SRC_ALPHA;		//	±ÙÌ§ÌÞÚÝÄÞ‚ðŽw’è.
	blendDesc.RenderTarget[0].DestBlend	//d‚Ë‚é‘fÞ‚É‘Î‚·‚éÝ’è.
		= D3D11_BLEND_INV_SRC_ALPHA;	//	±ÙÌ§ÌÞÚÝÄÞ‚Ì”½“]‚ðŽw’è.

	blendDesc.RenderTarget[0].BlendOp	//ÌÞÚÝÄÞµÌß¼®Ý.
		= D3D11_BLEND_OP_ADD;			//	ADD:‰ÁŽZ‡¬.

	blendDesc.RenderTarget[0].SrcBlendAlpha	//Œ³‘fÞ‚Ì±ÙÌ§‚É‘Î‚·‚éÝ’è.
		= D3D11_BLEND_ONE;					//	‚»‚Ì‚Ü‚ÜŽg—p.
	blendDesc.RenderTarget[0].DestBlendAlpha//d‚Ë‚é‘fÞ‚Ì±ÙÌ§‚É‘Î‚·‚éÝ’è.
		= D3D11_BLEND_ZERO;					//	‰½‚à‚µ‚È‚¢.

	blendDesc.RenderTarget[0].BlendOpAlpha	//±ÙÌ§‚ÌÌÞÚÝÄÞµÌß¼®Ý.
		= D3D11_BLEND_OP_ADD;				//	ADD:‰ÁŽZ‡¬.

	blendDesc.RenderTarget[0].RenderTargetWriteMask	//Ëß¸¾Ù–ˆ‚Ì‘‚«ž‚ÝÏ½¸.
		= D3D11_COLOR_WRITE_ENABLE_ALL;				//	‘S‚Ä‚Ì¬•ª(RGBA)‚Ö‚ÌÃÞ°À‚ÌŠi”[‚ð‹–‰Â‚·‚é.

	//ÌÞÚÝÄÞ½Ã°Äì¬.
	if (FAILED(
		m_pDevice11->CreateBlendState(
		&blendDesc, &m_pBlendState)))
	{
		MessageBox(NULL, "ÌÞÚÝÄÞ½Ã°Äì¬Ž¸”s", "clsCommon::SetBlend", MB_OK);
	}

	//ÌÞÚÝÄÞ½Ã°Ä‚ÌÝ’è.
	UINT mask = 0xffffffff;	//Ï½¸’l.
	m_pDeviceContext11->OMSetBlendState(
		m_pBlendState, NULL, mask);
}