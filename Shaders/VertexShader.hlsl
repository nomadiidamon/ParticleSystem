// an ultra simple hlsl vertex shader
// TODO: Part 1c
struct VERTEX
{
    float4 pos : SV_POSITION;
};
// TODO: Part 2b
cbuffer SHADER_VARS
{
	float4x4 myWorldMatrix[6];
    float4x4 myViewMatrix;
    float4x4 myProjectionMatrix;
};
	// TODO: Part 3b
	// TODO: Part 3f
// TODO: Part 3g
float4 main(VERTEX inputVertex : POSITION,
uint matrix_index : SV_InstanceID) : SV_POSITION
{
	// TODO: Part 2i
    float4 retval = mul((myWorldMatrix[matrix_index]), inputVertex.pos);
	// TODO: Part 3b
    retval = mul((myViewMatrix), retval);
    retval = mul((myProjectionMatrix), retval);
	// TODO: Part 3g
	return retval;
}