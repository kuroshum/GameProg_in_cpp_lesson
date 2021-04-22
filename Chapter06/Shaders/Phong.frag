// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

// Inputs from vertex shader
// Tex coord
in vec2 fragTexCoord;
// Normal (in world space)
in vec3 fragNormal;
// Position (in world space)
in vec3 fragWorldPos;

// This corresponds to the output color to the color buffer
out vec4 outColor;

// This is used for the texture sampling
uniform sampler2D uTexture;

// Create a struct for directional light
struct DirectionalLight
{
	// Direction of light
	vec3 mDirection;
	// Diffuse color
	vec3 mDiffuseColor;
	// Specular color
	vec3 mSpecColor;
};

// ===========================================
// ‰Û‘è6.2

// “_ŒõŒ¹—p‚Ì\‘¢‘Ì
struct PointLight
{
	// ŒõŒ¹‚ÌˆÊ’u
	vec3 mPosition;
	// ŠgŽU”½ŽËF
	vec3  mDiffuseColor;
	// ‹¾–Ê”½ŽËF
	vec3 mSpecColor;
	// ‹¾–Ê”½ŽËŽw”
	float mSpecPower;
	// ‰e‹¿”¼Œa
	float mRadius;
};

// “_ŒõŒ¹
uniform PointLight uPointLight[4];
// ===========================================

// Uniforms for lighting
// Camera position (in world space)
uniform vec3 uCameraPos;
// Specular power for this surface
uniform float uSpecPower;
// Ambient light level
uniform vec3 uAmbientLight;

// Directional Light
uniform DirectionalLight uDirLight;


void main()
{
	// ===========================================
	// ‰Û‘è6.2
	
	/*
	// Surface normal
	vec3 N = normalize(fragNormal);
	// Vector from surface to light
	vec3 L = normalize(-uDirLight.mDirection);
	// Vector from surface to camera
	vec3 V = normalize(uCameraPos - fragWorldPos);
	// Reflection of -L about N
	vec3 R = normalize(reflect(-L, N));

	// Compute phong reflection
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
		vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
		Phong += Diffuse + Specular;
	}
	*/
	
	// Surface normal
	vec3 N = normalize(fragNormal);
	vec3 Phong = uAmbientLight;

	for(int i = 0; i < 4; i++)
	{
		float distnce = distance(uPointLight[i].mPosition, fragWorldPos);
		// Vector from surface to light
		//vec3 L_point = normalize(-uPointLight.mDirection);
		// Vector from surface to light
		vec3 V_point = normalize(uPointLight[i].mPosition - fragWorldPos);
		// Reflection of -L about N
		vec3 R_point = normalize(reflect(-V_point, N));

		// Compute phong reflection
		
		float NdotL = dot(N, V_point);
		if (NdotL > 0 && distnce < uPointLight[i].mRadius)
		{
			vec3 Diffuse = uPointLight[i].mDiffuseColor * NdotL;
			vec3 Specular = uPointLight[i].mSpecColor * pow(max(0.0, dot(R_point, V_point)), uSpecPower);
			Phong += Diffuse + Specular;
		}
	}
	// ===========================================

	// Final color is texture color times phong light (alpha = 1)
    outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}
