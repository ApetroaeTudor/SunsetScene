#version 420 core

in vec2 fr_textureCoords;
in vec3 fr_vertexPosition;
out vec4 fr_colour;

struct Material{
    sampler2D texture_diffuse0;
};

uniform Material material;
uniform samplerCube cubemap;
uniform int isDrawingSkybox;

// cubemap reflections
uniform vec3 cameraPos;
uniform int normalsInverted;
in vec3 fr_normal;
in vec3 fr_position;

// lighting UBO stuff ------------------------------------------------------------------- 
struct DirLight {
    vec4 lightDirection;
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 specularColor;

    mat4 lightVP;
    
    float ambientStr;
    float specularShininess;

    int pad;
    int pad2;
};

struct SpotLight {
    vec4 lightDirection;
    vec4 lightPosition;
    vec4 diffuseColor;
    vec4 specularColor;
    vec4 ambientColor;

    mat4 lightVP;

    float specularShininess;
    float cutoffAngleCos;
    float outerCutoffAngleCos;
    float ambientStrength;
};

struct PointLight {
    vec4 lightPosition;
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 specularColor;

    float ambientStrength;
    float specularShininess;
    float KC;
    float KL;
    
    float KQ;
    int pad0;
    int pad1;
    int pad2;
};

layout(std140, binding = 0) uniform LightingUbo {
    DirLight directionalLights[1];
    int nrOfDirectionalLights;
    int pad0;
    int pad1;
    int pad2;

    SpotLight spotLights[3];
    int nrOfSpotLights;
    int pad3;
    int pad4;
    int pad5;

    PointLight pointLights[3];
    int nrOfPointLights;
    int pad6;
    int pad7;
    int pad8;
};
// ~ lighting UBO stuff ------------------------------------------------------------------- 

// SHADOWS ---------------------------------------------------------------------------------------
// shadow maps
uniform sampler2D directionalLightSM1;
uniform sampler2D spotLightSM[3];

float directionalShadow(){
    DirLight light = directionalLights[0];
    // transforming the fragment coordinates to light space
    vec4 fragPosLightSpace = light.lightVP * vec4(fr_position, 1.0);
    vec3 projectionCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    if(projectionCoords.z > 1){
        return 0.0;
    }

    // setting them in [0, 1] so they can be used to index ShadowMap
    projectionCoords = projectionCoords * 0.5 + 0.5;

    float closestDepth = texture(directionalLightSM1, projectionCoords.xy).r;
    float currentDepth = projectionCoords.z;

    float bias = 0.005;
    
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(directionalLightSM1, 0);
    for(int x = -2; x <= 2; ++x)
    {
        for(int y = -2; y <= 2; ++y)
        {
            float pcfDepth = texture(directionalLightSM1, projectionCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 25;
    return shadow;
}

float calculateSpotShadow(int idx){
    SpotLight light = spotLights[idx];
    vec4 fragPosLightSpace = light.lightVP * vec4(fr_position, 1.0);
    vec3 projectionCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    if(projectionCoords.z > 1){
        return 0.0;
    }
    projectionCoords = projectionCoords * 0.5 + 0.5;

    float closestDepth = texture(spotLightSM[idx], projectionCoords.xy).r;
    float currentDepth = projectionCoords.z;

    float bias = 0.005;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(spotLightSM[idx], 0);
    for(int x = -2; x <= 2; ++x)
    {
        for(int y = -2; y <= 2; ++y)
        {
            float pcfDepth = texture(spotLightSM[idx], projectionCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 25;
    return shadow;
}
// ~ SHADOWS ---------------------------------------------------------------------------------------

// PHONG lighting functions ------------------------------------------------------------------- 
// Blinn Phong uses fragmentToCamera
vec4 directionalLightB_P(vec3 cameraDirection, vec3 normalizedNorm, float shadow){
    DirLight light = directionalLights[0];

    vec4 ambientComponent = vec4( vec3(light.ambientStr * light.ambientColor), 1.0 );

    float diffuseComponent = max( dot(normalizedNorm, vec3(light.lightDirection)), 0.0);

    // blinn phong
    vec3 halfwayDir = normalize(vec3(light.lightDirection) + cameraDirection);
    float specularComponent = pow( max(dot(normalizedNorm, halfwayDir), 0.0), light.specularShininess);

    return clamp(ambientComponent + (1.0 - shadow) * (diffuseComponent + specularComponent), 0.0, 1.0);
}

vec4 spotLightsB_P(vec3 cameraDirection, vec3 normalizedNorm){
    vec3 summedColors = vec3(0, 0, 0);
    for( int i = 0; i < nrOfSpotLights; i++ ){
        SpotLight light = spotLights[i];
        // thetta angle cos between lightDir and fragmentLightDir
        vec3 fragmentToLightDir = normalize(vec3(light.lightPosition) - fr_position); // from fragment to light
        float thettaCos = dot(fragmentToLightDir, normalize(-vec3(light.lightDirection))); // in c++ this is passed as light from the source downwards

        if(thettaCos > light.outerCutoffAngleCos){
            float epsilon = light.cutoffAngleCos - light.outerCutoffAngleCos;
            float intensity = clamp((thettaCos - light.outerCutoffAngleCos) / epsilon, 0.0, 1.0);

            // ambient
            vec3 ambient = vec3(light.ambientStrength * light.ambientColor);

            // diffuse
            float diffuseComponent = max( dot(normalizedNorm, fragmentToLightDir), 0.0);

            // blinn phong
            vec3 halfwayDir = normalize(fragmentToLightDir + cameraDirection);
            float specularComponent = pow( max(dot(normalizedNorm, halfwayDir), 0.0), light.specularShininess);

            vec3 diffuse  = diffuseComponent * vec3(light.diffuseColor);
            vec3 specular = specularComponent * vec3(light.specularColor);
            
            float shadow = calculateSpotShadow(i);
            summedColors += (ambient + (1.0 - shadow) * (specular + diffuse)) * intensity;
        }
    }

    return vec4(summedColors, 1.0);
}

vec4 pointLightsB_P(vec3 cameraDirection, vec3 normalizedNorm){
    vec3 summedColors = vec3(0.0f, 0.0f, 0.0f);
    for( int i = 0; i < nrOfPointLights; i++ ){
        PointLight light = pointLights[i];

        vec3 fragmentToLight = vec3(light.lightPosition) - fr_position;
        float d = length(fragmentToLight);
        float attenuation = 1.0 / (light.KC + light.KL * d + light.KQ * (d * d));
        if(attenuation > 0.01){
            // ambient        
            vec3 ambientComponent = light.ambientStrength * vec3(light.ambientColor);

            // diffuse
            float diffuseComponent = max(dot(normalizedNorm, fragmentToLight), 0.0); 

            // specular
            vec3 halfway = normalize(fragmentToLight + cameraDirection);
            float specularComponent = pow( max(dot(normalizedNorm, halfway), 0.0), light.specularShininess);

            summedColors += (ambientComponent + diffuseComponent + specularComponent) * attenuation;
        }
    }
    return vec4(summedColors, 1.0);
}
// ~ PHONG lighting functions ------------------------------------------------------------------- 

// FRESNEL effect function -----------------------------------------------------------------------
// Fresnel uses fragmentToCamera
float calculateFresnel(vec3 cameraDirection, vec3 normalizedNorm){
    // Fresnel eq: F = F0 + (1 - F0) * ( (1-cos(thetta)) ^ 5 )
    // F = final reflection
    // F0 = reflection material constant
    // cos(thetta) = dot(cameraDir, vertexNormal)

    float F0 = 0.04;
    int inversePower = 18;
    float cosThetta = max(dot(cameraDirection, normalizedNorm), 0.0);
    float F = F0 + (1.0 - F0) * pow( 1 - cosThetta, inversePower );

    return F;
}
// ~ FRESNEL effect function -----------------------------------------------------------------------

// CUBEMAP reflection function --------------------------------------------------------------------
// cubemap reflection uses cameraToFragment
vec4 calculateCubemapReflection(vec3 cameraDirection, vec3 normalizedNorm){
    vec3 reflectVect      = reflect(cameraDirection, normalizedNorm);
    vec4 reflectionColor  = vec4(texture(cubemap, reflectVect).rgb, 1.0);
    return reflectionColor;
}
// ~CUBEMAP reflection function --------------------------------------------------------------------

// ~ SHADOWS ---------------------------------------------------------------------------------------
void main() {
    if(isDrawingSkybox == 1){
        fr_colour = texture(cubemap, fr_vertexPosition);
        return;
    }

    vec3 fragmentToCamera = normalize(cameraPos - fr_position);
    vec3 cameraToFragment = normalize(fr_position - cameraPos); 

    vec3 normalizedNorm  = normalize(fr_normal);
    if(normalsInverted == 1){
        normalizedNorm = -normalizedNorm;
    }

    float F = calculateFresnel(fragmentToCamera, normalizedNorm);

    vec4 cubemapReflectionColor = calculateCubemapReflection(cameraToFragment, normalizedNorm);

    vec4 textureBaseColor = texture(material.texture_diffuse0, fr_textureCoords); 
    vec4 mixedColor = mix(textureBaseColor, cubemapReflectionColor, F);

    vec4 lightsSum = vec4(0.0, 0.0, 0.0, 1.0);
    int lightingExists = 0;

    if(nrOfDirectionalLights == 1){
        float directionalShadow = directionalShadow();
        lightsSum += directionalLightB_P(fragmentToCamera, normalizedNorm, directionalShadow);
        lightingExists = 1;
    }
    if(nrOfSpotLights >= 1){
        lightsSum += spotLightsB_P(fragmentToCamera, normalizedNorm);
        lightingExists = 1;
    }
    if(nrOfPointLights >= 1){
        lightsSum += pointLightsB_P(fragmentToCamera, normalizedNorm);
        lightingExists = 1;
    }

    if(lightingExists == 1){
        fr_colour = lightsSum * mixedColor; 
    }
    else{
        fr_colour = mixedColor;
    }
}