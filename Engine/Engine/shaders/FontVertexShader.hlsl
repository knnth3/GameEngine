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
	bool useColor : USECOLOR;
};

VSOutput main(float4 pos : POSITION, float2 inTexCoord : TEXCOORD)
{
	int posInSequence = ascii.x;
	int c = (int)ascii.w;
	int letter = (c - 31) + 5;
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
	float minX = maxX - xLen;
	float minY = maxY - yLen;

	if (inTexCoord.x == 0.0f)
		inTexCoord.x = minX;
	else
		inTexCoord.x = maxX;

	if (inTexCoord.y == 0.0f)
		inTexCoord.y = minY;
	else
		inTexCoord.y = maxY;

	pos.x *= 0.6f;

	pos.x = pos.x + (posInSequence * 1.2f) - ascii.y;

	VSOutput output;
	pos.w = 1.0f;
	output.pos = mul(pos, worldMatrix);
	output.pos = mul(output.pos, viewMatrix);
	output.pos = mul(output.pos, projectionMatrix);
	output.pos.z = 0.0f;
	output.texCoord = inTexCoord;
	output.useColor = false;

	return output;
}