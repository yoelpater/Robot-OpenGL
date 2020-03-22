#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform int pmode;


void main()
{
	vec3 col;
	if (pmode == 0){
		col = texture(screenTexture, TexCoords).rgb;
	} else  if (pmode == 1){
		col = 1 - texture(screenTexture, TexCoords).rgb;
	} else  if (pmode == 2){
		col = texture(screenTexture, TexCoords).rgb;

		float grayscale_col = 0.299*col.r + 0.587*col.g + 0.114*col.b;

		col = vec3(grayscale_col,grayscale_col,grayscale_col);

	} else  if (pmode == 3){
		col = texture(screenTexture, TexCoords).rgb;

		col = pow(col, vec3(0.9, 0.9, 0.9));
		col = col * 8.0;
		col = floor(col);
		col = col / 8.0;
		col = pow(col, vec3(1.0/0.9));


	} else {
		col = texture(screenTexture, TexCoords).rgb;
	}
    
    FragColor = vec4(col, 1.0);
} 