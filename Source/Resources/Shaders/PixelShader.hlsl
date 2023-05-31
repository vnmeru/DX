struct VS_OUTPUT
{
	float4 pos: SV_POSITION;
	nointerpolation float4 color: COLOR;
	float2 tex : TEXCOORD; // texture coordinates
};

Texture2D objTexture : register(t0); // texture resource
SamplerState objSampler : register(s0); // sampler state

float4 main(VS_OUTPUT input) : SV_TARGET
{
	// Sample the texture using the input texture coordinates
	float4 texColor = objTexture.Sample(objSampler, input.tex);

	// Combine the vertex color with the texture color
	float4 finalColor = input.color * texColor;

	return finalColor; // Return the final color
}