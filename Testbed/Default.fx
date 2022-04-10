
struct VS_INPUT {
    float4 pos : POSITION;
    float3 color : COLOR;
    float3 color2 : COLOR1;
};

struct VS_OUTPUT {
    float4 pos : SV_POSITION;
    float3 color : COLOR;
    float3 color2 : COLOR1;
};

cbuffer CB : register(b0) {
    row_major float4x4 m_WorldMatrix;
    row_major float4x4 m_ViewMatrix;
    row_major float4x4 m_ProjectionMatrix;
    unsigned int m_Time;
};

VS_OUTPUT VertexShaderMain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    //output.pos = lerp(input.pos, input.pos2, (sin(m_Time / 1000.0f) + 1.0f) / 2.0f);
    
    output.pos = mul(input.pos, m_WorldMatrix);
    output.pos = mul(output.pos, m_ViewMatrix);
    output.pos = mul(output.pos, m_ProjectionMatrix);
    

    output.color = input.color;
    output.color2 = input.color2;
    return output;
}

struct PS_INPUT {
    float4 pos : SV_POSITION;
    float3 color : COLOR;
    float3 color2 : COLOR1;
};

float4 PixelShaderMain(PS_INPUT input) : SV_Target
{
    input.color = lerp(input.color, input.color2, (sin(m_Time / 100.0f) + 1.0f) / 2.0f);
    return float4(input.color, 1.0f);
}