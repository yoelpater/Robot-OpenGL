// ADS Point lighting Shader
// Fragment Shader
// Richard S. Wright Jr.
// OpenGL SuperBible
#version 430
struct MaterialInfo{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
};

uniform MaterialInfo Material;
uniform vec4 ambientColor;
uniform int mode;
out vec4 vFragColor;

//lighting color
//vec4    ambientColor = vec4(0.1,0.1,0.1,1);
vec4    diffuseColor = vec4(0.8,0.8,0.8,1);   
vec4    specularColor = vec4(1.0,1.0,1.0,1);

in vec3 vVaryingNormal;
in vec3 vVaryingLightDir;
in vec3 vPosition3;
in vec2 UV;
float Shininess = 1.0;//for material specular

void main(void)
{ 
	if(mode == 0) 
	{
		// Dot product gives us diffuse intensity
		float diff = max(0.0, dot(normalize(vVaryingNormal),
					normalize(vVaryingLightDir)));

		// Multiply intensity by diffuse color, force alpha to 1.0
		vFragColor = diff * diffuseColor*vec4(Material.Kd,1);

		// Add in ambient light
		vFragColor += ambientColor;


		// Specular Light
		vec3 vReflection = normalize(reflect(-normalize(vVaryingLightDir), normalize(vVaryingNormal)));
			//反射角
			
		float spec = max(0.0, dot(normalize(vVaryingNormal), vReflection));
		if(diff != 0) 
		{
			spec = pow(spec, Shininess);
			vFragColor += specularColor*vec4(Material.Ka,1)*spec;
		}
	}
	else
	{
		vec3 normal = normalize(vVaryingNormal);
		vec3 lightDir = normalize(vVaryingLightDir);
		
		float lambertian = max(dot(lightDir,normal), 0.0);
		float specular = 0.0;
		
		if(lambertian > 0.0) 
		{
			vec3 viewDir = normalize(-vPosition3);
			
			vec3 halfDir = normalize(lightDir + viewDir);
			float specAngle = max(dot(halfDir, normal), 0.0);
			specular = pow(specAngle, 16.0);
			
			if(mode == 2) {
				vec3 reflectDir = reflect(-lightDir, normal);
				specAngle = max(dot(reflectDir, viewDir), 0.0);
				// note that the exponent is different here
				specular = pow(specAngle, 4.0);
			}
			if(mode == 3)  specular = pow(specAngle, 16.0);
			// according to the rendering equation we would need to multiply
			// with the the "lambertian", but this has little visual effect
			if(mode == 4) specular *= lambertian;
			// switch to mode 4 to turn off the specular component
			if(mode == 5) specular *= 0.0;
		}
		
		vFragColor = lambertian * diffuseColor * vec4(Material.Kd,1);
		
		vFragColor += ambientColor;
		
		vFragColor += specularColor*specular*vec4(Material.Ka,1);
		//vFragColor += specularColor * specular;
		
		//vFragColor = vec4(ambientColor +
        //              lambertian * diffuseColor * vec4(Material.Kd,1) +
        //              specular * specularColor * vec4(Material.Ka,1), 1.0);
	}
}
	