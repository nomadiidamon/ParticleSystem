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

static float4 diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
static float4 specular = { 1.0f, 1.0f, 1.0f, 1.0f };
static float4 ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
static float4 emissive = { 0.0f, 0.0f, 0.0f, 1.0f };
static float Ns = 100.0f;

// need variables for accessing the texture
Texture2D myFirstTexture : register(t0, space1);
SamplerState myFirstSampler : register(s0, space1);


float4 main(Vertex_Output fromVertShader) : SV_TARGET
{
    float3 retval;
    
    float3 _lightDirection = normalize(lightDirection.xyz);
    float4 _lightColor = lightColor;
    
    //float3 surfaceColor = float3(diffuse.xyz);
    float3 surfaceColor = myFirstTexture.Sample(myFirstSampler, fromVertShader.texcoord).xyz;

    float3 newNorm = normalize(fromVertShader.normal);
    float lightRatio = saturate(dot(newNorm, -_lightDirection));
        
    
    float3 _diffuse = lightRatio * _lightColor.xyz * surfaceColor;
    
    
    float3 viewDir = normalize(fromVertShader.camPos.xyz - fromVertShader.positionW);
    float3 halfVector = normalize((-_lightDirection) + viewDir);
    float intensity = pow(saturate(dot(newNorm, halfVector)), Ns);
    float3 reflectedLight = _lightColor.xyz * specular.xyz * intensity;
    
    
    retval = saturate(_diffuse + ambient.xyz) * surfaceColor.xyz + reflectedLight;
    
    
    return float4(retval.xyz, 1);
    
}