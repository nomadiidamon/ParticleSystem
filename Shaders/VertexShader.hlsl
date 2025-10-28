struct Vertex_Input
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float4 tangent : TANGENT;
};

struct Vertex_Output
{
    float4 positionH : SV_POSITION;
    float3 positionW : WORLD;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float4 tangent : TANGENT;
    float4 camPos : CAMPOS;

};

cbuffer SHADER_VARS
{
    float4x4 myViewMatrix;
    float4x4 myProjectionMatrix;
    float4 lightDirection;
    float4 lightColor;
    float4 camPos;
};

Vertex_Output main(Vertex_Input inVert) : SV_POSITION
{
    Vertex_Output vOut = (Vertex_Output)0;
    //float4x4 identity = float4x4(1, 0, 0, 0,
    //                              0, 1, 0, 0,
    //                              0, 0, 1, 0,
    //                              0, 0, 0, 1);
    
    //float4 newWorld = mul(identity, float4(inVert.position, 1));
    float4 newWorld =  float4(inVert.position, 1);

    vOut.positionW = newWorld.xyz;
    float4 newView =  mul(myViewMatrix, newWorld);
    float4 newProj = mul(myProjectionMatrix, newView); 
    
    vOut.positionH = newProj;
    vOut.normal = inVert.normal;
    vOut.texcoord = inVert.texcoord;
    vOut.tangent = inVert.tangent;
    
    return vOut;
}
