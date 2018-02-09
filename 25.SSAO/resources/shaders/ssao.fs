#version 330 core

in vec2 texCoords;

out float fragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;
uniform mat4 projection;
uniform vec3 samplesKernel[64];

float radius = 0.5;
float bias = 0.025;
const vec2 noiseScale = vec2(1280.0/4.0, 720.0/4.0); 

void main()
{
  vec3 randomVec = normalize(texture(texNoise, texCoords * noiseScale).xyz);
  float occlusion = 0.0;
  vec3 normal = normalize(texture(gNormal, texCoords).rgb);
  vec3 tangent = normalize(randomVec - dot(normal, randomVec) * normal);
  vec3 bitangent = cross(normal, tangent);
  mat3 TBN = mat3(tangent, bitangent, normal);
  vec3 fragPos = texture(gPosition, texCoords).rgb;
  
  for(int i = 0; i < 64; i++)
  {       
    vec3 kernelSampleViewSpace = TBN * samplesKernel[i];
	vec3 sample = fragPos + kernelSampleViewSpace * radius;

	vec4 sampleScreenSpace = projection * vec4(sample, 1.0);
	sampleScreenSpace.xyz /= sampleScreenSpace.w;
	sampleScreenSpace.xyz = 0.5 * sampleScreenSpace.xyz + 0.5;
	float sampleDepth = texture(gPosition, sampleScreenSpace.xy).z;
	float rangeCheck = smoothstep(0.0, 1.0, radius/abs(sampleDepth - sample.z));
	occlusion += (sampleDepth >= sample.z + bias ? 1.0 : 0.0) * rangeCheck;
  }
  occlusion = 1.0 - (occlusion/64.0);
  occlusion = pow(occlusion, 2.0);
  fragColor = occlusion;
}