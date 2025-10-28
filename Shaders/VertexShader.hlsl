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
    float rotationAmount; // rotation in radians
};

// Helper: rotation around Z-axis
float4x4 CreateZRotation(float angle)
{
    float s = sin(angle);
    float c = cos(angle);
    return float4x4(
        c, -s, 0, 0,
        s, c, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
}

float4 main(VERTEX inputVertex : POSITION,
uint matrix_index : SV_InstanceID) : SV_POSITION
{
    // Create a rotation matrix for this frame
    float4x4 rot = CreateZRotation(rotationAmount);

    // Rotate vertex in object space
    float4 rotatedVertex = mul(rot, inputVertex.pos);
    
    // Apply world, view, and projection transforms
    float4 retval = mul((myWorldMatrix[matrix_index]), inputVertex.pos);
    retval = mul((myViewMatrix), retval);
    retval = mul((myProjectionMatrix), retval);
	return retval;
}