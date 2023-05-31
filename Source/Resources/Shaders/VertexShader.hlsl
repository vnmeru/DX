// This is the input struct for the vertex shader, containing the position and color of a vertex
struct VS_INPUT
{
	float4 pos : POSITION; // vertex position in 3D space
	float4 color : COLOR; // vertex color
	float2 uv : TEXCOORD; // texture coordinates
};

// This is the output struct for the vertex shader, containing the position and color of a vertex after transformation
struct VS_OUTPUT
{
	float4 pos : SV_POSITION; // vertex position in 2D space after transformation
	float4 color : COLOR; // vertex color
	float2 uv : TEXCOORD; // texture coordinates
};

// This is the constant buffer containing the World-View-Projection matrix
cbuffer ConstantBuffer : register(b0)
{
	float4x4 wvpMat; // world-view-projection matrix
};

// This is the main function of the vertex shader
VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	// Transform the vertex position using the World-View-Projection matrix
	output.pos = mul(input.pos, wvpMat);

	// Pass the vertex color through to the output struct
	output.color = input.color;

	output.uv = input.uv;

	return output; // Return the transformed vertex position, color, and texture coordinates
}