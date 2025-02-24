#shader vertex
#version 330 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 texCoord;

out vec2 fragCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
   gl_Position = projection * view * model * vec4(pos,0.0f,1.0f);
   fragCoord = texCoord * 2.0f - 1.0f;
};


#shader fragment
#version 330 core
out vec4 fragColor;

in vec2 fragCoord;

uniform float iTime;
uniform vec3 iResolution;

const int NUM_OF_STEPS = 128;
const float MIN_DIST_TO_SDF = 0.001f;
const float MAX_DIST_TO_TRAVEL = 64.0f;

float opSmoothUnion(float d1, float d2, float k){
    float h = clamp(0.5f + 0.5f * (d2-d1) / k, 0.0, 1.0);
    return mix(d2, d1, h) - k * h * (1.0 - h);
}

float sdPlane( vec3 p, vec3 n, float h )
{
    // n must be normalized
    return dot(p,n) + h;
}

float sdSphere(vec3 p , float r)
{
	return length(p) - r;
}

float map(vec3 p){
    float distToSDF = MAX_DIST_TO_TRAVEL;
    
    distToSDF = min(distToSDF, sdPlane(p, vec3(0.0f, 1.0f, 0.0f), 1.0f));
    
    float displacement = sin(5.0f * p.x + iTime*2f) * sin(10.0 * p.y + iTime*2f) * sin(15.0 * p.z + iTime*2f) * 0.1f;
    float distToSphere1 = sdSphere(p - vec3(0.0f, sin(iTime) * 0.5f - 0.5f, 0.0f), 0.5f) + displacement;
    distToSDF = opSmoothUnion(distToSDF, distToSphere1, 0.5f);
    // float distToSphere2 = sdSphere(p - vec3(0.75f, 0.0f, 0.0f), 0.25f);
    // distToSDF = min(distToSDF, opSmoothUnion(distToSphere1, distToSphere2, 0.5f));

    return distToSDF;
}

float rayMarch(vec3 ro, vec3 rd){
    float dist = 0.0f;

    for(int i = 0; i < NUM_OF_STEPS; i++){
        vec3 pos = ro + rd * dist;

        float distToSDF = map(pos);

        if(distToSDF < MIN_DIST_TO_SDF){
            break;
        }

        dist += distToSDF;

        if(dist > MAX_DIST_TO_TRAVEL){
            break;
        }
    }

    return dist;
}

vec3 calcNorm(vec3 p){    
    const vec3 delta = vec3(0.001, 0.0, 0.0);

    float gradX = map(p + delta.xyy) - map(p - delta.xyy);
    float gradY = map(p + delta.yxy) - map(p - delta.yxy);
    float gradZ = map(p + delta.yyx) - map(p - delta.yyx);

    return normalize(vec3(gradX, gradY, gradZ));
}

void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    // Time varying pixel color
    //vec3 col = vec3(fragCoord.xy, 0.0f);
    vec2 uv = fragCoord;
    uv.x *= iResolution.x / iResolution.y;

    vec3 col = vec3(0.0f);

    // Camera
    vec3 camPos = vec3(2.0f*sin(iTime), 0.0f, 2.0f*cos(iTime));
    vec3 camVUp = vec3(0.0f, 1.0f, 0.0f);
    vec3 camW = normalize(-camPos);
    vec3 camU = normalize(cross(camVUp, camW));
    vec3 camV = cross(camW, camU);

    // Ray origin and Ray direction
    vec3 ro = camPos;
    vec3 rd = camW + uv.x * camU + uv.y * camV;

    float dist = rayMarch(ro, rd);

    if(dist < MAX_DIST_TO_TRAVEL){
        vec3 p = ro + rd * dist;
        vec3 n = calcNorm(p);

        vec3 lightColor = vec3(1.0f);
        vec3 lightSource = vec3(2.5f, 2.5f, -2.0f);
        vec3 lightDir = -normalize(lightSource);

        // diffuse light
        float diffuseStrength = max(0.0f, dot(-lightDir, n));
        vec3 diffuse = lightColor * diffuseStrength;
        
        // specular (Blinn phong)
        vec3 viewSource = normalize(ro);
        vec3 blinnSource = (lightDir + viewSource) * 0.5f;
        float specularStrength = max(0.0f, dot(blinnSource, n));
        specularStrength = pow(specularStrength, 64.0f);
        vec3 specular = lightColor * specularStrength;

        vec3 lighting = diffuse * 0.75f + specular * 0.25f;
        
        col = lighting;

        // shadow (rayMarch toward light source)
        float maxDistToLightSource = length(lightSource - p);
        ro = p + n*0.1f;
        rd = -lightDir;
        float distToLightSource = rayMarch(ro, rd);
        if(distToLightSource < maxDistToLightSource){
            col = col * vec3(0.25f);
        }

        // gamma correction
        col = pow(col, vec3(1.0f / 2.2f));
    }

    // Output to screen
    fragColor = vec4(col,1.0);
}