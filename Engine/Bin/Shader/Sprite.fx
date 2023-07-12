#include "Share.fx"

struct VS_INPUT_COLOR
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
};

// SV : System Value로 사용되는 값으로 출력레지스터에서 사용할 수 있다.
// 출력레지스터에 아래처럼 SV_POSITION 을 넣으면 최종 투영공간에서의 위치를
// 지정하는것으로 이 값을 이용해서 화면의 어디에 출력할지를 결정한다.
// SV 값은 다른 Shader에서 절대로 건드리면 안된다.
struct VS_OUTPUT_COLOR
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

struct VS_INPUT_UV
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VS_OUTPUT_UV
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
};


// 인자로 VS_INPUT_COLOR를 쓰면 저 구조체에 지정된 입력 레지스터에서 값을
// 가져와서 사용하므로 인자를 따로 넣어줄 필요가 없다.
VS_OUTPUT_COLOR SpriteColorVS(VS_INPUT_COLOR input)
{
    VS_OUTPUT_COLOR output = (VS_OUTPUT_COLOR) 0;

    float3  Pos = input.Pos - g_Pivot * g_MeshSize;
    
    // mul : 행렬 곱. g_matWVP 는 World * View * Proj 이므로 정점을 여기에 곱하게 되면
    // 투영 공간으로 변환된 정점의 위치가 나온다.
    output.Pos = mul(float4(Pos, 1.f), g_matWVP);
    output.Color = input.Color;
    
    return output;
}

PS_OUTPUT_SINGLE SpriteColorPS(VS_OUTPUT_COLOR input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;
    
    output.Color = input.Color;
    
    return output;
}

// 인자로 VS_INPUT_COLOR를 쓰면 저 구조체에 지정된 입력 레지스터에서 값을
// 가져와서 사용하므로 인자를 따로 넣어줄 필요가 없다.
VS_OUTPUT_UV SpriteVS(VS_INPUT_UV input)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;

    float3 Pos = input.Pos - g_Pivot * g_MeshSize;
    
    // mul : 행렬 곱. g_matWVP 는 World * View * Proj 이므로 정점을 여기에 곱하게 되면
    // 투영 공간으로 변환된 정점의 위치가 나온다.
    output.Pos = mul(float4(Pos, 1.f), g_matWVP);
    output.UV = UpdateAnimation2D(input.UV);
    
    return output;
}

PS_OUTPUT_SINGLE SpritePS(VS_OUTPUT_UV input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    float4  TextureColor = g_BaseTexture.Sample(g_PointSmp, input.UV);
    
    output.Color.rgb = TextureColor.rgb * g_MtrlBaseColor.rgb;

    output.Color.a = TextureColor.a * g_MtrlOpacity;
    
    return output;
}

