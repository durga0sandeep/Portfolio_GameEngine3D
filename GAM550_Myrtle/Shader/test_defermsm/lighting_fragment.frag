/////////////////////////////////////////////////////////////////////////
// Pixel shader for lighting
////////////////////////////////////////////////////////////////////////
#version 430

out vec4 FragColor;

in vec2 texcoord;//


//Shadow
uniform sampler2D shadowMap;
vec4 ShadowCoord;
vec2 shadowIndex;
float lightDepth;
float pixelDepth;
bool inShadow;

uniform vec3 lightColor;    
uniform vec3 ambientLight; 

const float PI = 3.14159265;

//in vec3 tanVec;

vec3 BRDF,F;
float G,D;
vec2 uv;//
vec4 color = vec4(0.0, 0.0, 0.0, 1.0);//

bool TexturePresent = false;
bool NormalMapPresent = false;

//vec3 delta;
//
uniform sampler2D BlurredShadowMap;

//	
//uniform vec3 lightPos1;
//	
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gMaterial;

uniform mat4 WorldInverse, ShadowMatrix;

uniform vec3 lightPos;//LIGHT POSITION
vec3 eyePos;
vec3 normalVec, lightVec, eyeVec;

vec3 uv3;
vec3 WorldPos_Sha;//for shadow

uniform float z0;//z_near
uniform float z1;//z_far

float S;

vec3 CholeskyDecomposition(float m11, float m12, float m13, float m22, float m23, float m33, float z1, float z2, float z3)
{
 float a = sqrt(m11);
 float b = m12 / a;
 float c = m13 / a;

 float d = sqrt(m22 - b * b);
 float e = (m23 - b * c) / d;
 float f = sqrt(m33 - c * c - e * e);

 // Capped value (a, d, f)
 a = max(a, 1e-4f);
 d = max(d, 1e-4f);
 f = max(f, 1e-4f);

 float _c1 = z1 / a;
 float _c2 = (z2 - b * _c1) / d;
 float _c3 = (z3 - c * _c1 - e * _c2) / f;

 float c3 = _c3 / f;
 float c2 = (_c2 - e * c3) / d;
 float c1 = (_c1 - b * c2 - c * c3) / a;

 return vec3(c1, c2, c3);
}


vec2 SolveQuadratic(float a, float b, float c)
{
    const float epsilon = 1e-6f;
    float discriminant = b * b - 4 * a * c;
 
     // 2 real roots
     if (discriminant > epsilon)
     {
      float root1 = (-b + sqrt(discriminant)) / (2.0f * a);
      float root2 = (-b - sqrt(discriminant)) / (2.0f * a);
      return vec2(min(root1, root2), max(root1, root2));
     }
     // 1 real root
     else if (discriminant >= 0.0f && discriminant <= epsilon)
     {
      float root = -b / (2.0f * a);
      return vec2(root);
     }
     // 2 complex roots
     else
     {
      float real = -b / (2.0f * a);
      float imag = sqrt(-discriminant) / (2.0f * a);
      return vec2(real, imag);
     }
}

//MSM
 float MomentShadowMap(sampler2D shadowMap, vec4 shadowCoord){
			
	float zF = pixelDepth;

	const float epsilon = 1e-3f;
	float zF_2 = pixelDepth * pixelDepth;
	normalVec = texture(gNormal, uv).xyz;


	float Shadowbias =  max(0.05 * (1.0 - dot(normalVec, lightVec)), 0.001f);
	vec4 b = texture2D(BlurredShadowMap, shadowIndex.xy);
	
	//float shadowBias = 1e-3f;
	if (abs(b.x - Shadowbias) < epsilon){
		return 0.0f;
	}

	float alpha = 1e-3f;
	vec4 bPrime = (1.0f - alpha) * b + (alpha * vec4(0.5f));


	vec3 c = CholeskyDecomposition(1.0f, bPrime.x, bPrime.y, bPrime.y, bPrime.z, bPrime.w, 1.0f, zF, zF_2);

		 
	vec2 solutions = SolveQuadratic(c.z, c.y, c.x);
	float z2 = solutions.x;
	float z3 = solutions.y;

	if (zF <= z2 || abs(zF - z2) <= epsilon)
	{
	  return 0.0;
	}

	if (zF <= z3 || abs(zF - z3) <= epsilon)
	{
	  float numerator = zF * z3 - bPrime.x * (zF + z3) + bPrime.y;
	  float denominator = (z3 - z2) * (zF - z2);
	  return numerator / denominator;
	}

	float numerator = z2 * z3 - bPrime.x * (z2 + z3) + bPrime.y;
	float denominator = (zF - z2) * (zF - z3);
	return 1.0 - numerator / denominator;
}




void main()
{
    
    /*
    //ShadowMap Reading Correctly ... working
	vec2 uv = gl_FragCoord.xy/vec2(750,750); // Or use textureSize
    FragColor = texture(shadowMap, uv);
    //FragColor = vec4(texture(BlurredShadowMap, uv).r);
    //FragColor = texture(BlurredShadowMap, uv);
	return;
    */


    /*
    // Purple (R,G,B,A)
    FragColor = vec4(0.5, 0.0, 0.5, 1.0);
    return;
    */
    uv = gl_FragCoord.xy/vec2(800,600);//800x600

    /*
    vec3 albedo = texture(gAlbedo, uv).rgb;
    FragColor = vec4(albedo, 1.0);
    return;
    */

    /*
    vec3 normal = texture(gNormal, uv).xyz;
    normal = normalize(normal);
    normal = normal * 0.5 + 0.5;   // map -1..+1 → 0..1
    FragColor = vec4(normal, 1.0);
    return;
    */
    /*
    vec3 P = texture(gPosition, uv).xyz;
    FragColor = vec4(abs(P), 1.0);
    return;
    */

    /*
    vec3 N = texture(gNormal, uv).xyz;
    FragColor = vec4((N * 0.5 + 0.5), 1.0); 
    return;
    */

    //
    /*
    FragColor = vec4(texture(gAlbedo, uv).rgb, 1.0);
    return;
    //*/

	
	//
	WorldPos_Sha = texture(gPosition, uv).xyz;
	ShadowCoord = ShadowMatrix * vec4(WorldPos_Sha, 1.0);

	//EYE POSITION
	eyePos = (WorldInverse*vec4(0,0,0,1)).xyz;
	//uv3 = vec3(uv, 0.0);
	//eyeVec = eyePos - uv3;// texture(WorldPosMap, uv).xyz;
	eyeVec = eyePos - vec3(uv, 0.0);// texture(WorldPosMap, uv).xyz;
	//
	lightVec = lightPos - texture(gPosition, uv).rgb;//light

	vec3 N = texture(gNormal, uv).xyz;//changed
    vec3 L = normalize(lightVec);
	vec3 V = normalize(eyeVec);//done
	
	vec3 H = normalize(L+V);
	float LN = max(dot(L,N),0.0);
	float HN = max(dot(H,N),0.0);

    vec3 Kd = texture(gAlbedo, uv).xyz;
	vec3 Ks = texture(gMaterial, uv).xyz;
	
	vec3 Ii = lightColor;
	vec3 Ia = ambientLight;
	float Alpha = texture(gMaterial, uv).a;

	//vec3 T = normalize(tanVec);
        //vec3 B = normalize(cross(T,N));

	shadowIndex = ShadowCoord.xy/ShadowCoord.w;

	if (shadowIndex.x >= 0.0 && shadowIndex.x <= 1.0 &&
        shadowIndex.y >= 0.0 && shadowIndex.y <= 1.0 &&
        ShadowCoord.w > 0.0)
    {

		//BlurredShadowMap
        lightDepth = texture2D(shadowMap, shadowIndex).w;
        pixelDepth = (ShadowCoord.w - z0)/(z1 - z0);//zf in the document

		S = (1 - MomentShadowMap(shadowMap, ShadowCoord));

        
		inShadow =  (pixelDepth > lightDepth);
	
    }
    else
    {
        inShadow = false;  // Default to being in shadow if out of bounds
    }

	
	    H = normalize(L+V);
	    LN = max(dot(L,N),0.0);
	    HN = max(dot(H,N),0.0);

	    //
	    //THE LIGHTING
		D =	((Alpha + 2.0)/(2.0*PI)) * pow((HN),Alpha);
		F = Ks + ((1,1,1) - Ks) * pow((1 - max(dot(H,L),0.0)),5);
		G = (1.0/(pow(max(dot(H,L),0.00001),2)));
		BRDF = (Kd/PI) + (F * G * D) / 4.0;

		//
		// If in shadow, only apply ambient lighting
        if (inShadow == true) 
		{
			//In Shadow
			FragColor.xyz = (Ia * Kd); // Ambient lighting only

		} else if (inShadow == false) 
		{
			//In lighting
			FragColor.xyz = (Ia * Kd) + S *(Ii*(LN)	* (BRDF));
		}

		//FragColor.xyz = (Ia * Kd) + S *(Ii*(LN)	* (BRDF));
		FragColor.w = 1.0;
		

    //FragColor = vec4(Kd, 1.0);


}

