// d3d_helpers.h
#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

struct Vertex {
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT4 color;
};

static const char* vertexShaderCode = R"(
struct VS_INPUT {
    float3 pos : POSITION;
    float4 col : COLOR;
};
struct PS_INPUT {
    float4 pos : SV_POSITION;
    float4 col : COLOR;
};
PS_INPUT main(VS_INPUT input) {
    PS_INPUT output;
    output.pos = float4(input.pos.xy, 0.f, 1.f);
    output.col = input.col;
    return output;
})";

static const char* pixelShaderCode = R"(
struct PS_INPUT {
    float4 pos : SV_POSITION;
    float4 col : COLOR;
};
float4 main(PS_INPUT input) : SV_Target {
    return input.col;
})";