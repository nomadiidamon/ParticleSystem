// an ultra simple hlsl vertex shader
struct VERTEX
{
    float4 pos : SV_POSITION;
};
cbuffer SHADER_VARS
{
	float4x4 myWorldMatrix[6];
    float4x4 myViewMatrix;
    float4x4 myProjectionMatrix;
};


float4 main(VERTEX inputVertex : POSITION,
uint matrix_index : SV_InstanceID) : SV_POSITION
{
    float4 retval = mul((myWorldMatrix[matrix_index]), inputVertex.pos);
    retval = mul((myViewMatrix), retval);
    retval = mul((myProjectionMatrix), retval);
	return retval;
}