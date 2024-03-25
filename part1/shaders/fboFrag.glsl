// ====================================================
#version 330 core

// ======================= uniform ====================
// If we have texture coordinates, they are stored in this sampler.
uniform sampler2D u_DiffuseMap; 
uniform sampler2D u_DepthMap; 
uniform sampler2D u_NormalMap;
uniform sampler2D u_VelMap;
uniform float focus_depth;
uniform int depth_debug;

// ======================= IN =========================
in vec2 v_texCoord; // Import our texture coordinates from vertex shader

// ======================= out ========================
// The final output color of each 'fragment' from our fragment shader.
out vec4 FragColor;

float LinearizeDepth(in vec2 uv)
{
    float zNear = 0.1;   
    float zFar  = 32.0;
    float depth = texture(u_DepthMap, uv).r;
    return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}

vec2 LinearizeVel(in vec2 uv)
{
    vec2 vel = texture(u_VelMap, uv).rg;
    float x = abs(vel.x) * 10;
    float y = abs(vel.y) * 10;
    return vec2(x,y);
}

const float offset = 3.0/300.0;
const float searchDist = 5.0/300.0;
const float inFocus = 0.02;
vec3 DepthOfFieldColor(in vec2 uv)
{
    float dist = LinearizeDepth(uv);
    float amountOffset = 0.0;
    float distFromFocus = abs(dist - focus_depth);
    if(distFromFocus > inFocus)
    {
        amountOffset = distFromFocus - inFocus;
    }
    float true_offset = amountOffset * offset; 
    vec2 offsets[9] = vec2[](
        vec2(-true_offset,  true_offset), // top-left
        vec2( 0.0f,         true_offset), // top-center
        vec2( true_offset,  true_offset), // top-right
        vec2(-true_offset,  0.0f),        // center-left
        vec2( 0.0f,         0.0f),        // center-center
        vec2( true_offset,  0.0f),        // center-right
        vec2(-true_offset, -true_offset), // bottom-left
        vec2( 0.0f,        -true_offset), // bottom-center
        vec2( true_offset, -true_offset)  // bottom-right    
    );

    float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        //If the pixel we are adding to the blur is in focus, do not add it.
        float depthOfBlutTarget = LinearizeDepth(uv + offsets[i]);
        if(dist > focus_depth && depthOfBlutTarget < focus_depth + inFocus && depthOfBlutTarget > focus_depth - inFocus)
        {
            sampleTex[i] = texture(u_DiffuseMap, uv).rgb;
        }
        else {
            sampleTex[i] = texture(u_DiffuseMap, uv + offsets[i]).rgb;
        }
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    return col;
}

vec4 MotionBlurColor(in vec2 uv)
{
    vec2 texelSize = 1.0 / vec2(textureSize(u_DiffuseMap, 0));

    vec2 velocity = texture(u_VelMap, uv).rg;

    float speed = length(velocity / texelSize);
    int nSamples = clamp(int(speed), 1, 10);

    vec4 result = texture(u_DiffuseMap, uv);
    for (int i = 1; i < nSamples; ++i) {
        vec2 offset = velocity * (float(i) / float(nSamples - 1) - 0.5);
        result += texture(u_DiffuseMap, uv + offset);
    }
    result /= float(nSamples);
    return result;
}

void main()
{
    if(depth_debug == 0)
    {
        FragColor = MotionBlurColor(v_texCoord);
        return;
        
    }
    if(depth_debug == 1)
    {
        vec3 depthCol = DepthOfFieldColor(v_texCoord);
        FragColor = vec4(depthCol, 1.0);
        return;
    }
    if(depth_debug == 2)
    {
        float dist = LinearizeDepth(v_texCoord);
        FragColor = vec4(dist, dist, dist, 1.0);
        return;
    }
    if(depth_debug == 3)
    {
        FragColor = vec4(LinearizeVel(v_texCoord), 0.0, 1.0);
        return;
    }
    if(depth_debug == 4)
    {
        FragColor = vec4(texture(u_DiffuseMap, v_texCoord).rgb, 1.0);
        return;
    }
    if(depth_debug == 5)
    {
        vec3 depthCol = DepthOfFieldColor(v_texCoord);
        vec4 motionBlurCol = MotionBlurColor(v_texCoord);
        vec4 combined = motionBlurCol + vec4(depthCol, 1.0);
        FragColor = combined / 2;
        return;
    }
    if(depth_debug == 6)
    {
        float dist = LinearizeDepth(v_texCoord) - focus_depth;
        if(dist >= -inFocus && dist <= inFocus)
        {
            FragColor = vec4(1.0, 0.0, 0.0, 1.0);
            return;
        }
        else 
        {
            vec3 depthCol = DepthOfFieldColor(v_texCoord);
            FragColor = vec4(depthCol, 1.0);
            return;
        }
    }
}
// ==================================================================
