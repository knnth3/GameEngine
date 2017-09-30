cbuffer ConstBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	//float4x4 WVP;
};

cbuffer TextBuffer
{
	float4 ascii;
};

struct VSOutput
{
	float4 pos : SV_POSITION;
	float2 texCoord  : TEXCOORD;
};

float4 GetCharUVCoords(int char)
{
	int letter = (char - 31) + 5;
	int posy;
	int posx = letter % 20;
	if (posx == 0)
	{
		posx = letter;
		posy = letter / 20;
	}
	else
	{
		posy = (letter / 20) + 1;
	}
	float xLen = 0.05f;
	float yLen = 0.2f;
	float row = (float)posx;
	float collumn = (float)posy;
	float maxX = xLen*row;
	float maxY = yLen*collumn;
	return float4(maxX, maxY, maxX - xLen, maxY - yLen);
}

VSOutput main(float4 pos : POSITION, float2 inTexCoord : TEXCOORD)
{
	int posInSequence = ascii.x;
	float4 uv = GetCharUVCoords((int)ascii.y);

	if (inTexCoord.x == 0.0f)
		inTexCoord.x = uv.z;
	else
		inTexCoord.x = uv.x;

	if (inTexCoord.y == 0.0f)
		inTexCoord.y = uv.w;
	else
		inTexCoord.y = uv.y;

	//Increase pos for next char in string.
	pos.x += (posInSequence * 1.2f);
	pos.y -= 1.0f;
	pos.w = 1.0f;

	VSOutput output;
	output.pos = mul(pos, worldMatrix);
	output.pos = mul(output.pos, viewMatrix);
	output.pos = mul(output.pos, projectionMatrix);
	output.texCoord = inTexCoord;
	return output;
}