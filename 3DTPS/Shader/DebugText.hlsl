//ｸﾞﾛｰﾊﾞﾙ変数.

//ﾃｸｽﾁｬはﾚｼﾞｽﾀ-t(n)
Texture2D		g_texColor	: register(t0);
//ｻﾝﾌﾟﾗｰはﾚｼﾞｽﾀｰs(n)
SamplerState	g_samLinear	: register(s0);

//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
cbuffer global
{
	matrix	g_mWVP;		//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,ﾌﾟﾛｼﾞｪｸｼｮﾝの合成行列.
	float4	g_vColor;	//色.
	float4	g_fAlpha;	//透過値.
};

//頂点ｼｪｰﾀﾞの出力ﾊﾟﾗﾒｰﾀ.
struct VS_OUT
{
	float4	Pos		: SV_Position;	//位置.
	float2	Tex		: TEXCOORD;		//ﾃｸｽﾁｬ.
};

//頂点ｼｪｰﾀﾞ.
VS_OUT VS(float4 Pos	: POSITION,
	float2 Tex : TEXCOORD)
{
	VS_OUT Out = (VS_OUT)0;
	Out.Pos = mul(Pos, g_mWVP);
	//ﾃｸｽﾁｬ座標.
	Out.Tex = Tex;

	return Out;
}

//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
float4 PS(VS_OUT In) : SV_Target
{
	float4 color
	= g_vColor * g_texColor.Sample(g_samLinear, In.Tex);
	return color * g_fAlpha.x;
}