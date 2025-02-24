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

const int MAX_RAY_LEVEL = 5;
const int SAMPLE_NUM = 20;
const int NUM_OF_STEPS = 128;
const float MIN_DIST_TO_SDF = 0.001f;
const float MAX_DIST_TO_TRAVEL = 64.0f;

const float PI = 3.1415926535f;
const float MAXFLOAT = 99999.99f;

// ========================================================
// Util function
struct RaymarchRes
{
    // surface properties
    float  dist;
	
    // material properties
    vec3  albedo;
    
};

float opSmoothUnion(float d1, float d2, float k){
    float h = clamp(0.5f + 0.5f * (d2-d1) / k, 0.0, 1.0);
    return mix(d2, d1, h) - k * h * (1.0 - h);
}

vec2 randState;

float rand2D()
{
    randState.x = fract(sin(dot(randState.xy, vec2(12.9898f, 78.233f))) * 43758.5453f);
    randState.y = fract(sin(dot(randState.xy, vec2(12.9898f, 78.233f))) * 43758.5453f);
    
    return randState.x;
}

vec3 random_in_unit_sphere()
{
    float phi = 2.0f * PI * rand2D();
    float cosTheta = 2.0f * rand2D() - 1.0f;
    float u = rand2D();

    float theta = acos(cosTheta);
    float r = pow(u, 1.0 / 3.0);

    float x = r * sin(theta) * cos(phi);
    float y = r * sin(theta) * sin(phi);
    float z = r * cos(theta);

    return vec3(x, y, z);
}

// ========================================================
// Geometry SDF function
float sdPlane(vec3 p, vec3 n, float h )
{
    // n must be normalized
    return dot(p,n) + h;
}

float sdSphere(vec3 p , float r)
{
	return length(p) - r;
}

// ========================================================
// Map function
RaymarchRes mapFloor(vec3 p){
    RaymarchRes res;
    float distToSDF = MAX_DIST_TO_TRAVEL;
    res.dist = distToSDF;
    
    distToSDF = min(distToSDF, sdPlane(p, vec3(0.0f, 1.0f, 0.0f), 0.5f));
    if(res.dist > distToSDF){
        res.dist = distToSDF;
        res.albedo = vec3(0.678f, 0.663f, 0.569f);
    }

    return res;
}
RaymarchRes mapSphere(vec3 p){
    RaymarchRes res;
    float distToSDF = MAX_DIST_TO_TRAVEL;
    res.dist = distToSDF;
    
    float distToSphere1 = sdSphere(p - vec3(0.0f, sin(iTime) * 1.0f - 0.5f, 0.0f), 0.5f);
    distToSDF = min(distToSDF, distToSphere1);

    if(res.dist > distToSDF){
        res.dist = distToSDF;
        res.albedo = vec3(0.596f, 0.847f, 0.937f);
    }

    return res;
}
RaymarchRes map(vec3 p){
    RaymarchRes res;
    res.dist = MAX_DIST_TO_TRAVEL;

    RaymarchRes test;
    
    test = mapFloor(p);
    if(res.dist > test.dist){
        res.dist = test.dist;
        res.albedo = test.albedo;
    }

    test = mapSphere(p);
    if(res.dist > test.dist){
        // res.dist = test.dist;
        res.dist = opSmoothUnion(res.dist, test.dist, 0.5f);
        res.albedo = test.albedo;
    }

    return res;
}

// ========================================================
// Raycast (with raymarch)
vec3 calcNorm(vec3 p){    
    const vec3 delta = vec3(0.001f, 0.0, 0.0);

    float gradX = map(p + delta.xyy).dist - map(p - delta.xyy).dist;
    float gradY = map(p + delta.yxy).dist - map(p - delta.yxy).dist;
    float gradZ = map(p + delta.yyx).dist - map(p - delta.yyx).dist;

    return normalize(vec3(gradX, gradY, gradZ));
}

vec3 background(vec3 ro, vec3 rd){
    float t = 0.5f*(rd.y + 1.0f);
    return (1.0f-t)*vec3(1.0f, 1.0f, 1.0f) + t*vec3(0.5f, 0.7f, 1.0f);
}

RaymarchRes rayMarch(vec3 ro, vec3 rd){
    RaymarchRes res;
    res.dist = 0.0f;

    for(int i = 0; i < NUM_OF_STEPS; i++){
        vec3 pos = ro + rd * res.dist;

        RaymarchRes tempRes = map(pos);

        if(tempRes.dist < MIN_DIST_TO_SDF){
            break;
        }

        res.dist += tempRes.dist;
        res.albedo = tempRes.albedo;

        if(res.dist > MAX_DIST_TO_TRAVEL){
            break;
        }
    }

    return res;
}

vec3 rayCast(vec3 ro, vec3 rd, int depth){
    vec3 col = vec3(1.0f, 1.0f, 1.0f);

    vec3 ro_t = ro;
    vec3 rd_t = rd;

    for(int i = 0; i < MAX_RAY_LEVEL; i++){
        RaymarchRes res = rayMarch(ro_t, rd_t);

        if(res.dist < MAX_DIST_TO_TRAVEL){
            vec3 p = ro + rd * res.dist;
            vec3 n = calcNorm(p);
            vec3 target = normalize(n + random_in_unit_sphere());

            col *= res.albedo;
            ro_t = p+target*0.1f;
            rd_t = target;
        }
        else{
            col *= background(ro, rd);
            break;
        }
    }

    return col;
}

void main()
{
    randState = fragCoord.xy / iResolution.xy;

    // Normalized pixel coordinates (from 0 to 1)
    // Time varying pixel color
    vec2 uv = fragCoord;
    uv.x *= iResolution.x / iResolution.y;

    // Camera
    vec3 camPos = vec3(3f*sin(iTime), 0.0f, 3f*cos(iTime));
    vec3 camVUp = vec3(0.0f, 1.0f, 0.0f);
    vec3 camW = normalize(-camPos);
    vec3 camU = normalize(cross(camVUp, camW));
    vec3 camV = normalize(cross(camW, camU));

    vec3 col = vec3(0.0f);

    for(int i = 0; i < SAMPLE_NUM; i++){
        // Ray origin and Ray direction
        vec3 ro = camPos;
        vec3 rd = normalize(camW + (uv.x + rand2D()/iResolution.x) * camU + (uv.y + rand2D()/iResolution.y) * camV);
        col += rayCast(ro, rd, 0);
    }

    col /= SAMPLE_NUM;
    
    // Gamma correction
    col = pow(col, vec3(1.0f / 2.2f));

    // Output to screen
    fragColor = vec4(col,1.0);
}