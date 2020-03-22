#include "main.h"

vec3 camera = vec3(0,0,20);
int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitContextVersion(4,3);//以OpenGL version4.3版本為基準
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);//是否向下相容,GLUT_FORWARD_COMPATIBLE不支援(?
	glutInitContextProfile(GLUT_CORE_PROFILE);

	//multisample for golygons smooth
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL 4.3 - Robot");

	glewExperimental = GL_TRUE; //置於glewInit()之前
	if (glewInit()) {
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;//c error
		exit(EXIT_FAILURE);
	}

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(Keyboard);
	int ActionMenu,ModeMenu,ShaderMenu,LightMenu, ShaderAlgorithmMenu, PostProcessMenu;
	ActionMenu = glutCreateMenu(ActionMenuEvents);//建立右鍵菜單
	//加入右鍵物件
	glutAddMenuEntry("Idle",0);
	glutAddMenuEntry("Walk",1);
	glutAddMenuEntry("Jump", 2);
	glutAddMenuEntry("Bow Down", 3);
	glutAddMenuEntry("Orz", 4);
	glutAddMenuEntry("Attack", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	ModeMenu = glutCreateMenu(ModeMenuEvents);//建立右鍵菜單
	//加入右鍵物件
	glutAddMenuEntry("Line",0);
	glutAddMenuEntry("Fill",1);
	glutAddMenuEntry("Point", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯


	ShaderMenu = glutCreateMenu(ShaderMenuEvents);//建立右鍵菜單
	glutAddMenuEntry("Normal", 0);
	glutAddMenuEntry("Glowing", 1);
	glutAddMenuEntry("Glowing Red", 2);
	glutAddMenuEntry("Glowing Green", 3);
	glutAddMenuEntry("Glowing Blue", 4);
	glutAddMenuEntry("Glowing Gold", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯


	LightMenu = glutCreateMenu(LightMenuEvents);//建立右鍵菜單
	glutAddMenuEntry("Normal", 0);
	glutAddMenuEntry("Rotate A", 1);
	glutAddMenuEntry("Rotate B", 2);
	glutAddMenuEntry("Rotate C", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	ShaderAlgorithmMenu = glutCreateMenu(ShaderAlgorithmEvents);//建立右鍵菜單
	glutAddMenuEntry("Basic", 0);
	glutAddMenuEntry("Toon Shader", 1);
	glutAddMenuEntry("Reflection", 2);
	glutAddMenuEntry("Glass Refraction", 3);
	glutAddMenuEntry("Air Refraction", 4);
	glutAddMenuEntry("Diamond Refraction", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	PostProcessMenu = glutCreateMenu(PostProcessEvents);//建立右鍵菜單
	glutAddMenuEntry("Normal", 0);
	glutAddMenuEntry("Average Blur", 1);
	glutAddMenuEntry("Median Blur", 2);
	glutAddMenuEntry("Triangle Blur", 3);
	glutAddMenuEntry("Gaussian blur", 4);
	glutAddMenuEntry("Quantization", 5);
	glutAddMenuEntry("Difference of Gauss", 6);
	glutAddMenuEntry("Quant and DoG", 7);
	glutAddMenuEntry("Inverted", 8);
	glutAddMenuEntry("Grayscale", 9);
	glutAddMenuEntry("10", 10);
	glutAddMenuEntry("11", 11);
	glutAddMenuEntry("12", 12);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	glutCreateMenu(menuEvents);//建立右鍵菜單
	//加入右鍵物件
	glutAddSubMenu("Action",ActionMenu);
	glutAddSubMenu("Mode",ModeMenu);
	glutAddSubMenu("Model Shader", ShaderMenu);
	glutAddSubMenu("Light Direction", LightMenu);
	glutAddSubMenu("Shader Algorithm", ShaderAlgorithmMenu);
	glutAddSubMenu("Filter", PostProcessMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	glutMouseFunc(Mouse);
	glutTimerFunc (100, idle, 0); 
	glutTimerFunc(10, AmbientChange, 0);
	glutTimerFunc(10, LightChange, 0);
	glutMainLoop();
	return 0;
}


//recursion of glut timer func changing the uniform ambient of the shader language
void AmbientChange(int loop) {
	if (loop >= 50) {
		loop = 0;
	}

	if (ambientAnim == NORMAL_AMBIENT){
		ambientValue[0] = 0.1;
		ambientValue[1] = 0.1;
		ambientValue[2] = 0.1;
	} 
	else if (ambientAnim == GLOWING){
		ambientValue[0] = ((double)loop - (loop * loop * 0.02)) / 25.0;//quadratic formula
		ambientValue[1] = ((double)loop - (loop * loop * 0.02)) / 25.0;
		ambientValue[2] = ((double)loop - (loop * loop * 0.02)) / 25.0;
	}
	else if (ambientAnim == GLOWING_RED) {
		ambientValue[0] = ((double)loop - (loop * loop * 0.02)) / 25.0;
		ambientValue[1] = 0.1;
		ambientValue[2] = 0.1;
	}
	else if (ambientAnim == GLOWING_GREEN) {
		ambientValue[0] = 0.1;
		ambientValue[1] = ((double)loop - (loop * loop * 0.02)) / 25.0;
		ambientValue[2] = 0.1;
	}
	else if (ambientAnim == GLOWING_BLUE) {
		ambientValue[0] = 0.1;
		ambientValue[1] = 0.1;
		ambientValue[2] = ((double)loop - (loop * loop * 0.02)) / 25.0;
	}
	else if (ambientAnim == GLOWING_GOLD) {
		ambientValue[0] = ((double)loop - (loop * loop * 0.02)) / 25.0;
		ambientValue[1] = ((double)loop - (loop * loop * 0.02)) / 27.0;
		ambientValue[2] = 0.1;
	}

	glutPostRedisplay();
	glutTimerFunc(40, AmbientChange, loop+1);
}



void LightChange(int loop2) {
	if (loop2 >= 360) {
		loop2 = 0;
	}

	if (lightAnim == 0) {
		LightPosition[0] = 1.0;
		LightPosition[1] = 1.0;
		LightPosition[2] = 1.0;
	}
	else if(lightAnim == 1) {
		LightPosition[0] = 50 * cos(loop2*PI / 180);
		LightPosition[1] = 1.0;
		LightPosition[2] = 50 * sin(loop2*PI / 180);
	}
	else if (lightAnim == 2) {
		LightPosition[0] = 50 * cos(loop2*PI / 180);
		LightPosition[1] = 50 * sin(loop2*PI / 180);
		LightPosition[2] = 10.0;
	}
	else if (lightAnim == 3) {
		LightPosition[0] = 10.0;
		LightPosition[1] = 50 * cos(loop2*PI / 180);
		LightPosition[2] = 50 * sin(loop2*PI / 180);
	}

	glutPostRedisplay();
	glutTimerFunc(10, LightChange, loop2 + 1);
}





void ChangeSize(int w,int h){
	if(h == 0) h = 1;
	glViewport(0,0,w,h);
	Projection = perspective(80.0f,(float)w/h,0.1f,100.0f);

	
	glDeleteRenderbuffers(1, &rbo);
	glDeleteTextures(1, &textureColorbuffer);
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);

	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
}



void Mouse(int button,int state,int x,int y){
	if(button == 2) isFrame = false;
}
void idle(int dummy){
	isFrame = true;
	out = 0;
	if(action == WALK){
		updateObj(dummy);
		out = dummy+1;
		if(out > 12) out = 1;
	}
	else if (action == JUMP) {
		updateObjJump(dummy);
		out = dummy + 1;
		if (out > 12)action = IDLE; //out = 1;
	}
	else if (action == BOWDOWN) {
		updateObjBowDown(dummy);
		out = dummy + 1;
		if (out > 20)action = IDLE; //out = 1;
	}
	else if (action == ORZ) {
		updateObjOrz(dummy);
		out = dummy + 1;
		if (out > 15)out = 4; //out = 1;
	}
	else if (action == ATTACK) {
		updateObjAttack(dummy);
		out = dummy + 1;
		if (out > 12)out = 4; //out = 1;
	}
	else if(action == IDLE){
		resetObj(dummy);
		out = 0;
	}
	
	glutPostRedisplay();
	
	glutTimerFunc (150, idle, out); 
}
void resetObj(int f){
	for(int i = 0 ; i < PARTSNUM;i++){
		angles[i] = 0.0f;
	}	
}
void updateObj(int frame){
	switch(frame){
	case 0:
		//左手
		angles[2] = -45;
		//右手
		angles[7] = angles[2];
		//腿
		angles[13] = 45;
		angles[16] = angles[13];
		
		break;
	case 1:
	case 2:
	case 3:
		angles[1] +=10;
		angles[6] = -angles[1];
		angles[12] -=15;
		angles[15] = -angles[12];
		angles[16] = angles[13];
		position += 0.1;
		break;
	case 4:
	case 5:
	case 6:
		angles[1] -=10;
		angles[6] = -angles[1];
		angles[12] +=15;
		angles[13] -= 15;
		angles[15] = -angles[12];
		angles[16] = angles[13];
		position -= 0.1;
		break;
	case 7:
	case 8:
	case 9:
		angles[1] -=10;
		angles[6] = -angles[1];
		angles[12] +=15;
		angles[13] = 0;
		angles[15] = -angles[12];
		angles[16] = angles[13];
		position += 0.1;
		break;
	case 10:
	case 11:
	case 12:
		angles[1] +=10;
		angles[6] = -angles[1];
		angles[12] -=15;
		angles[13] += 15;
		angles[15] = -angles[12];
		angles[16] = angles[13];
		position -= 0.1;
		break;
	}
}

void updateObjJump(int frame) {
	switch (frame) {
	case 0:
		for (int i = 0; i < PARTSNUM; i++) {
			angles[i] = 0.0f;
		}
		//左手
		angles[2] = -45;
		//右手
		angles[7] = angles[2];
		//腿
		angles[13] = 45;
		angles[16] = angles[13];

		break;
	case 1:
	case 2:
	case 3:
		//ancang ancang loncat
		angles[1] += 10;
		angles[6] = angles[1];
		//kaki
		angles[12] -= 15;
		angles[15] = angles[12];
		angles[16] = angles[13];
		position -= 0.1;
		break;
	case 4:
	case 5:
	case 6:
		//tangan naik
		angles[1] -= 30;
		angles[2] += 10;

		angles[6] = angles[1];
		angles[7] = angles[2];
		//leg
		angles[12] += 15;
		angles[13] -= 15;
		angles[15] = angles[12];
		angles[16] = angles[13];
		position += 5;
		break;
	case 7:
	case 8:
	case 9:
		angles[1] -= 30;
		angles[2] += 10;

		angles[6] = angles[1];
		angles[7] = angles[2];

		angles[12] += 15;
		angles[13] = 0;
		angles[15] = angles[12];
		angles[16] = angles[13];
		position += 5;
		break;
	case 10:
	case 11:
	case 12:
		//tangan
		angles[1] += 30;
		angles[6] = angles[1];
		//kaki
		angles[12] -= 15;
		angles[13] += 15;
		angles[15] = angles[12];
		angles[16] = angles[13];
		position -= 10;
		break;
	}
}

void updateObjBowDown(int frame) {
	switch (frame) {
	case 0:
		for (int i = 0; i < PARTSNUM; i++) {
			angles[i] = 0.0f;
		}
		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
		angles[0] += 10;
		break;
	case 9:
	case 10:
	case 11:
	case 12:
		angles[0] += 0;
		break;
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
		angles[0] -= 10;
		break;
	}
}

void updateObjOrz(int frame) {
	switch (frame) {
	case 0:
		for (int i = 0; i < PARTSNUM; i++) {
			angles[i] = 0.0f;
		}
		break;
	case 1:
	case 2:
	case 3:
		angles[0] += 40;

		angles[1] -= 40;
		angles[6] -= 40;

		angles[2] -= 30;
		angles[7] -= 30;

		angles[13] += 20;//dleftleg
		angles[16] += 20;//drightleg

		angles[12] += 20;
		angles[15] += 20;
		break;
	case 4:
	case 5:
	case 6:
		angles[1] -= 20;
		angles[2] -= 10;
		break;
	case 7:
	case 8:
	case 9:
		angles[1] += 20;
		angles[2] += 10;
		break;
	case 10:
	case 11:
	case 12:
		angles[1] -= 20;
		angles[2] -= 10;
		break;
	case 13:
	case 14:
	case 15:
		angles[1] += 20;
		angles[2] += 10;
		break;
	}
}

void updateObjAttack(int frame) {
	switch (frame) {
	case 0:
		for (int i = 0; i < PARTSNUM; i++) {
			angles[i] = 0.0f;
		}
		break;
	case 1:
	case 2:
	case 3:
		angles[0] += 5;
		angles[1] -= 5;
		angles[6] -= 5;

		angles[2] -= 40;
		angles[7] -= 40;
		break;
	case 4:
	case 5:
		
	case 6:
		angles[1] -= 25;
		//angles[6] -= 5;

		angles[2] += 40;
		//angles[7] -= 40;
		break;
	case 7:
	case 8:
	case 9:
		angles[1] += 25;
		angles[6] -= 25;

		angles[2] -= 40;
		angles[7] += 40;
		break;
	case 10:
	case 11:
	case 12:
		//angles[1] += 25;
		angles[6] += 25;

		//angles[2] -= 40;
		angles[7] -= 40;
		break;
	}
}

 GLuint M_KaID;
 GLuint M_KdID;
 GLuint M_KsID;
 GLuint AmbientColor;
 GLuint ShaderMode;
 GLuint LightPos;
 GLuint fraction_val;
 GLuint skyProjection;
 GLuint skyView;
 GLuint uniformSkybox;
 GLuint cameraPosition;
 GLuint _PostMode;
 GLuint _DeltaTime;

void init(){

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	isFrame = false;
	pNo = 0;
	for(int i = 0;i<PARTSNUM;i++)// Every of the body part
		angles[i] = 0.0;

	//VAO
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);



	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "DSPhong_Material.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER, "DSPhong_Material.fp" },//fragment shader
		{ GL_NONE, NULL }};

	ShaderInfo SkyboxShaders[] = {
		{ GL_VERTEX_SHADER, "skybox.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER, "skybox.fp" },//fragment shader
		{ GL_NONE, NULL } };

	//ShaderInfo ScreenShaders[] = {
	//	{ GL_VERTEX_SHADER, "5.1.framebuffers_screen.vp" },//vertex shader
	//	{ GL_FRAGMENT_SHADER, "5.1.framebuffers_screen.fp" },//fragment shader
	//	{ GL_NONE, NULL } };

	ShaderInfo ScreenShaders[] = {
		{ GL_VERTEX_SHADER, "Image_Processing.vs.glsl" },//vertex shader
		{ GL_FRAGMENT_SHADER, "Image_Processing.fs.glsl" },//fragment shader
		{ GL_NONE, NULL } };


	program = LoadShaders(shaders);// load shader
	SkyboxProgram = LoadShaders(SkyboxShaders);// load shader
	screenShaderProgram = LoadShaders(ScreenShaders);

	glUseProgram(program);//uniformm use shader
	
	MatricesIdx = glGetUniformBlockIndex(program,"MatVP");
	ModelID =  glGetUniformLocation(program,"Model");
    M_KaID = glGetUniformLocation(program,"Material.Ka");
	M_KdID = glGetUniformLocation(program,"Material.Kd");
	M_KsID = glGetUniformLocation(program,"Material.Ks");
	//or
	M_KdID = M_KaID+1;
	M_KsID = M_KaID+2;

	AmbientColor = glGetUniformLocation(program, "ambientColor");

	ShaderMode = glGetUniformLocation(program, "mode");

	LightPos = glGetUniformLocation(program, "vLightPosition");

	fraction_val = glGetUniformLocation(program, "fraction");
	uniformSkybox = glGetUniformLocation(program, "skybox");
	cameraPosition = glGetUniformLocation(program, "cameraPos");
	Projection = glm::perspective(80.0f,4.0f/3.0f,0.1f,100.0f);
	
	// Camera matrix
	View       = glm::lookAt(
		glm::vec3(0,10,25) , // Camera is at (0,10,25), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,1,0 to look upside-down)
		);

	Obj2Buffer();



	//UBO
	glGenBuffers(1,&UBO);
	glBindBuffer(GL_UNIFORM_BUFFER,UBO);
	glBufferData(GL_UNIFORM_BUFFER,sizeof(mat4)*2,NULL,GL_DYNAMIC_DRAW);
	//get uniform struct size
	int UBOsize = 0;
	glGetActiveUniformBlockiv(program, MatricesIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &UBOsize);  
	//bind UBO to its idx
	glBindBufferRange(GL_UNIFORM_BUFFER,0,UBO,0,UBOsize);
	glUniformBlockBinding(program, MatricesIdx,0);


	// skybox VAO
	
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	vector<std::string> faces
	{
		"skybox/right.jpg",
		"skybox/left.jpg",
		"skybox/top.jpg",
		"skybox/bottom.jpg",
		"skybox/front.jpg",
		"skybox/back.jpg"
	};


	cubemapTexture = loadCubemap(faces);

	glUseProgram(program);
	glUniform1i(uniformSkybox, 0);

	glUseProgram(SkyboxProgram);

	skyView = glGetUniformLocation(SkyboxProgram, "view");
	skyProjection = glGetUniformLocation(SkyboxProgram, "projection");


	// screen quad VAO
	
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a color attachment texture
	
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glUseProgram(screenShaderProgram);//uniformm use shader
	_PostMode = glGetUniformLocation(screenShaderProgram, "shader_now");
	_DeltaTime = glGetUniformLocation(screenShaderProgram, "iTime");
	glUniform1i(_PostMode, postmode);

	glClearColor(0.0,0.0,0.0,1);//black screen

	oldTimeSinceStart = 0;
}


void display(){

	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	//float deltaTime = (float)(timeSinceStart - oldTimeSinceStart);
	//oldTimeSinceStart = timeSinceStart;

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)


	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	glDepthMask(GL_FALSE);
	glUseProgram(SkyboxProgram);

	float eyey = DOR(eyeAngley);
	View = glm::mat4(glm::mat3(lookAt(vec3(eyedistance*sin(eyey), 2, eyedistance*cos(eyey)), vec3(0, 0, 0), vec3(0, 1, 0)))); // remove translation from the view matrix
	glUniformMatrix4fv(skyView, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(skyProjection, 1, GL_FALSE, &Projection[0][0]);

	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
	glDepthMask(GL_TRUE);
	glUseProgram(0);

	glBindVertexArray(VAO);
	glUseProgram(program);
	
	View       = lookAt(vec3(eyedistance*sin(eyey),2,eyedistance*cos(eyey)) ,vec3(0,0,0),vec3(0,1,0));
	updateModels();

	//sbMainSkybox.renderSkybox();
	//update data to UBO for MVP
	glBindBuffer(GL_UNIFORM_BUFFER,UBO);
	glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(mat4),&View);
	glBufferSubData(GL_UNIFORM_BUFFER,sizeof(mat4),sizeof(mat4),&Projection);
	glBindBuffer(GL_UNIFORM_BUFFER,0);

	//9 3 2019 bikin value glsl untuk uba nilai ambient light nya
	// update the ambient value in the shader using uniform format
	glUniform4f(AmbientColor, ambientValue[RED], ambientValue[GREEN], ambientValue[BLUE], 1);

	glUniform1f(fraction_val, .5f);

	glUniform1i(ShaderMode, shadermode);

	//LightPos = glGetUniformLocation(program, "vLightPosition");
	glUniform3f(LightPos, LightPosition[0], LightPosition[1], LightPosition[2]);

	glUniform3fv(cameraPosition, 1, &vec3(eyedistance*sin(eyey), 2, eyedistance*cos(eyey))[0]);

	GLuint offset[3] = {0,0,0};
	for(int i = 0;i < PARTSNUM ;i++){
		
		glUniformMatrix4fv(ModelID,1,GL_FALSE,&Models[i][0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,				//location
							  3,				//vec3
							  GL_FLOAT,			//type
							  GL_FALSE,			//not normalized
							  0,				//strip
							  (void*)offset[0]);//buffer offset
	
		offset[0] +=  vertices_size[i]*sizeof(vec3);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);//location 1 :vec2 UV
		glBindBuffer(GL_ARRAY_BUFFER, uVBO);
		glVertexAttribPointer(1, 
							  2, 
							  GL_FLOAT, 
							  GL_FALSE, 
							  0,
							  (void*)offset[1]);
		//(location,vec2,type,固定點,連續點的偏移量,point)
		offset[1] +=  uvs_size[i]*sizeof(vec2);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);//location 2 :vec3 Normal
		glBindBuffer(GL_ARRAY_BUFFER, nVBO);
		glVertexAttribPointer(2,
							  3, 
							  GL_FLOAT, 
							  GL_FALSE, 
							  0,
							  (void*)offset[2]);
		//(location,vec3,type,固定點,連續點的偏移量,point)
		offset[2] +=  normals_size[i]*sizeof(vec3);

		int vertexIDoffset = 0;//glVertexID's offset 
		string mtlname;//material name
		vec3 Ks = vec3(1,1,1);//because .mtl excluding specular , so give it here.
		for(int j = 0;j <mtls[i].size() ;j++){//
			mtlname = mtls[i][j];	
			//find the material diffuse color in map:KDs by material name.
			glUniform3fv(M_KdID,1,&KDs[mtlname][0]);
			glUniform3fv(M_KsID,1,&Ks[0]);
			//          (primitive   , glVertexID base , vertex count    )
			glDrawArrays(GL_TRIANGLES, vertexIDoffset  , faces[i][j+1]*3);
			//we draw triangles by giving the glVertexID base and vertex count is face count*3
			vertexIDoffset += faces[i][j+1]*3;//glVertexID's base offset is face count*3
		}//end for loop for draw one part of the robot	
		
	}//end for loop for updating and drawing model
	// get back to the default framebuffer

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	// clear all relevant buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(screenShaderProgram);

	glUniform1i(_PostMode, postmode);

	glUniform1f(_DeltaTime, (float)timeSinceStart);
	

	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glFlush();//強制執行上次的OpenGL commands
	glutSwapBuffers();//調換前台和後台buffer ,當後臺buffer畫完和前台buffer交換使我們看見它

	
}

void Obj2Buffer(){
	std::vector<vec3> Kds;
	std::vector<vec3> Kas;
	std::vector<vec3> Kss;
	std::vector<std::string> Materials;//mtl-name
	std::string texture;
	loadMTL("Obj/gundam.mtl",Kds,Kas,Kss,Materials,texture);
	//printf("%d\n",texture);
	for(int i = 0;i<Materials.size();i++){
		string mtlname = Materials[i];
		//  name            vec3
		KDs[mtlname] = Kds[i];
	}

	
	load2Buffer("Obj/body.obj",0);

	load2Buffer("Obj/ulefthand.obj",1);
	load2Buffer("Obj/dlefthand.obj",2);
	load2Buffer("Obj/lefthand.obj",3);
	//load2Buffer("Obj/lshouder.obj",4);
	
	load2Buffer("Obj/head.obj",5);

	load2Buffer("Obj/urighthand.obj",6);
	load2Buffer("Obj/drighthand.obj",7);
	load2Buffer("Obj/righthand.obj",8);
	//load2Buffer("Obj/rshouder.obj",9);

	load2Buffer("Obj/dbody.obj",11);
	//load2Buffer("Obj/back2.obj",10);

	load2Buffer("Obj/uleftleg.obj",12);
	load2Buffer("Obj/dleftleg.obj",13);
	load2Buffer("Obj/leftfoot.obj",14);

	load2Buffer("Obj/urightleg.obj",15);	
	load2Buffer("Obj/drightleg.obj",16);	
	load2Buffer("Obj/rightfoot.obj",17);
	
	GLuint totalSize[3] = {0,0,0};
	GLuint offset[3] = {0,0,0};
	for(int i = 0;i < PARTSNUM ;i++){
		totalSize[0] += vertices_size[i]*sizeof(vec3);
		totalSize[1] += uvs_size[i] * sizeof(vec2);
		totalSize[2] += normals_size[i] * sizeof(vec3);
	}
	//generate vbo
	glGenBuffers(1,&VBO);
	glGenBuffers(1,&uVBO);
	glGenBuffers(1,&nVBO);
	//bind vbo ,第一次bind也同等於 create vbo 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//VBO的target是GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER,totalSize[0],NULL,GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uVBO);//VBO的target是GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER,totalSize[1],NULL,GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, nVBO);//VBO的target是GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER,totalSize[2],NULL,GL_STATIC_DRAW);
	
	
	for(int i = 0;i < PARTSNUM ;i++){
		glBindBuffer(GL_COPY_WRITE_BUFFER,VBO);
		glBindBuffer(GL_COPY_READ_BUFFER, VBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,offset[0],vertices_size[i]*sizeof(vec3));
		offset[0] += vertices_size[i]*sizeof(vec3);
		glInvalidateBufferData(VBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER,0);

		glBindBuffer(GL_COPY_WRITE_BUFFER,uVBO);
		glBindBuffer(GL_COPY_READ_BUFFER, uVBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,offset[1],uvs_size[i]*sizeof(vec2));
		offset[1] += uvs_size[i]*sizeof(vec2);
		glInvalidateBufferData(uVBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER,0);

		glBindBuffer(GL_COPY_WRITE_BUFFER,nVBO);
		glBindBuffer(GL_COPY_READ_BUFFER, nVBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,offset[2],normals_size[i]*sizeof(vec3));
		offset[2] += normals_size[i] * sizeof(vec3);
		glInvalidateBufferData(uVBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER,0);
	}
	glBindBuffer(GL_COPY_WRITE_BUFFER,0);


}

void updateModels(){
	mat4 Rotatation[PARTSNUM];
	mat4 Translation[PARTSNUM];
	for(int i = 0 ; i < PARTSNUM;i++){
		Models[i] = mat4(1.0f);
		Rotatation[i] = mat4(1.0f);
		Translation[i] = mat4(1.0f); 
	}
	float r,pitch,yaw,roll;
	float alpha, beta ,gamma;

	//Body
	beta = angle;
	alpha = angles[0];
	Rotatation[0] = rotate(beta,0,1,0)*rotate(alpha, 1, 0, 0);
	pitch = DOR(alpha);
	yaw = DOR(((180 - alpha) / 2));
	roll = DOR(beta);
	int h = 5;
	Translation[0] = translate(sin(roll)*sin(pitch)*h,2.9+position - ((sin(pitch)*h)/tan(yaw)),cos(roll)*sin(pitch)*h);
	Models[0] = Translation[0]*Rotatation[0];
	//左手=======================================================
	//左上手臂 Left Upper Arm
	yaw = DOR(beta);r = 3.7;
	alpha = angles[1];
	gamma = 10;
	Rotatation[1] = rotate(alpha,1,0,0)*rotate(gamma,0,0,1);//向前旋轉*向右旋轉
	Translation[1] = translate(3.7,1,-0.5);

	Models[1] = Models[0]*Translation[1]*Rotatation[1];
	
	//左肩膀 Left Shoulder
	Rotatation[4] = rotate(alpha,1,0,0)*rotate(gamma,0,0,1);//向前旋轉*向右旋轉
	Translation[4] =translate(3.7,1,-0.5);//位移到左上手臂處
	Models[4] =Models[0]*Translation[1]*Rotatation[1];
	
	//左下手臂 Left Lower Arm
	pitch = DOR(alpha);r = 3;
	roll = DOR(gamma);
	static int i=0;
	i+=5;
	alpha = angles[2]-20;
	//上手臂+下手臂向前旋轉*向右旋轉
	Rotatation[2] = rotate(alpha,1,0,0);
	//延x軸位移以上手臂為半徑的圓周長:translate(0,r*cos,r*sin)
	//延z軸位移以上手臂為半徑角度:translate(r*sin,-rcos,0)
	Translation[2] = translate(0,-3,0);

	Models[2] = Models[1]*Translation[2]*Rotatation[2];
	

	pitch = DOR(alpha);
	//b = DOR(angles[2]);
	roll = DOR(gamma);
	//手掌角度與下手臂相同 The palm angle is the same as the lower arm
	//Rotatation[3] = Rotatation[2];
	//延x軸位移以上手臂為半徑的圓周長:translate(0,r*cos,r*sin) ,角度為上手臂+下手臂
	Translation[3] = translate(0,-4.8,0);
	Models[3] = Models[2]*Translation[3]*Rotatation[3];
	//============================================================
	//頭Head==========================================================
	Translation[5] = translate(0,3.9,-0.5);
	Models[5] = Models[0]*Translation[5]*Rotatation[5];
	//============================================================
	//右手=========================================================
	gamma = -10; alpha = angles[6]; //= -angles[1];
	Rotatation[6] = rotate(alpha,1,0,0)*rotate(gamma,0,0,1);
	Translation[6] = translate(-3.0,1.4,-0.2);
	Models[6] = Models[0]*Translation[6]*Rotatation[6];

	Rotatation[9] = rotate(alpha,1,0,0)*rotate(gamma,0,0,1);
	Translation[9] = translate(-3.9,1.1,-0.2);
	Models[9] = Models[0]*Translation[9]*Rotatation[9];

	angles[7]; //= angles[2];
	pitch = DOR(alpha);r = -3;
	roll = DOR(gamma);
	alpha = angles[7]-20;
	Rotatation[7] = rotate(alpha,1,0,0);
	Translation[7] = translate(-1,-3,0);
	Models[7] = Models[6]*Translation[7]*Rotatation[7];

	pitch = DOR(alpha);
	//b = DOR(angles[7]);
	roll = DOR(gamma);
	Translation[8] =translate(0,-6,0);
	Models[8] = Models[7]*Translation[8]*Rotatation[8];
	//=============================================================
	//back&DBody===================================================
	Translation[10] =translate(0,2,-4.5);
	Models[10] = Models[0]*Translation[10]*Rotatation[10];

	Translation[11] =translate(0,-5.3,0);
	Models[11] = Models[0]*Translation[11]*Rotatation[11];
	//=============================================================
	//左腳
	alpha = angles[12];gamma = 10;
	Rotatation[12] = rotate(alpha,1,0,0)*rotate(gamma,0,0,1);
	Translation[12] =translate(1.8,-4.5 + position,0);
	Models[12] = Translation[12]*Rotatation[12]*Models[12];

	pitch = DOR(alpha);r = -7;
	roll = DOR(gamma);
	alpha = angles[13]+angles[12];
	Translation[13] = translate(-r*sin(roll),r*cos(pitch),r*sin(pitch))*Translation[12];
	Rotatation[13] = rotate(alpha,1,0,0);
	Models[13] = Translation[13]*Rotatation[13]*Models[13];

	pitch = DOR(alpha); r = -5;
	//b = DOR(angles[13]);
	roll = DOR(gamma);
	Translation[14] = translate(-(r+2)*sin(roll),r*cos(pitch),r*sin(pitch)-1)*Translation[13];
	Rotatation[14] = rotate(alpha,1,0,0);
	Models[14] = Translation[14]*Rotatation[14]*Models[14];
	//=============================================================
	//右腳
	alpha = angles[15]; //= -angles[12];
	gamma = -10;
	Rotatation[15] = rotate(alpha ,1,0,0)*rotate(gamma ,0,0,1);
	Translation[15] =translate(-1.8,-4.5 + position,0);
	Models[15] = Translation[15]*Rotatation[15]*Models[15];

	angles[16]; //= angles[13];
	pitch = DOR(alpha);r = -7;
	roll = DOR(gamma);
	alpha = angles[16]+angles[15];
	Rotatation[16] = rotate(alpha,1,0,0);
	Translation[16] = translate(-r*sin(roll),r*cos(pitch),r*sin(pitch))*Translation[15];
	Models[16] = Translation[16]*Rotatation[16]*Models[16];

	pitch = DOR(alpha); r = -5;
	//b = DOR(angles[16]);
	roll = DOR(gamma);
	alpha = angles[15]+angles[16];
	Translation[17] = translate(-(r+2)*sin(roll),r*cos(pitch),r*sin(pitch)-0.5)*Translation[16];
	Rotatation[17] = rotate(alpha,1,0,0);
	Models[17] = Translation[17]*Rotatation[17]*Models[17];
	//=============================================================
}

void load2Buffer(char* obj,int i){
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals; // Won't be used at the moment.
	std::vector<unsigned int> materialIndices;

	bool res = loadOBJ(obj, vertices, uvs, normals,faces[i],mtls[i]);
	if(!res) printf("load failed\n");

	//glUseProgram(program);

	glGenBuffers(1,&VBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
	glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(vec3),&vertices[0],GL_STATIC_DRAW);
	vertices_size[i] = vertices.size();

	//(buffer type,data起始位置,data size,data first ptr)
	//vertices_size[i] = glm_model->numtriangles;
	
	//printf("vertices:%d\n",vertices_size[);

	glGenBuffers(1,&uVBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, uVBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);
	uvs_size[i] = uvs.size();

	glGenBuffers(1,&nVBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, nVBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);
	normals_size[i] = normals.size();
}
mat4 translate(float x,float y,float z){
	vec4 t = vec4(x,y,z,1);//w = 1 ,則x,y,z=0時也能translate
	vec4 c1 = vec4(1,0,0,0);
	vec4 c2 = vec4(0,1,0,0);
	vec4 c3 = vec4(0,0,1,0);
	mat4 M = mat4(c1,c2,c3,t);
	return M;
} 
mat4 scale(float x,float y,float z){
	vec4 c1 = vec4(x,0,0,0);
	vec4 c2 = vec4(0,y,0,0);
	vec4 c3 = vec4(0,0,z,0);
	vec4 c4 = vec4(0,0,0,1);
	mat4 M = mat4(c1,c2,c3,c4);
	return M;
}

mat4 rotate(float angle,float x,float y,float z){
	float r = DOR(angle);
	mat4 M = mat4(1);

	vec4 c1 = vec4(cos(r)+(1-cos(r))*x*x,(1-cos(r))*y*x+sin(r)*z,(1-cos(r))*z*x-sin(r)*y,0);
	vec4 c2 = vec4((1-cos(r))*y*x-sin(r)*z,cos(r)+(1-cos(r))*y*y,(1-cos(r))*z*y+sin(r)*x,0);
	vec4 c3 = vec4((1-cos(r))*z*x+sin(r)*y,(1-cos(r))*z*y-sin(r)*x,cos(r)+(1-cos(r))*z*z,0);
	vec4 c4 = vec4(0,0,0,1);
	M = mat4(c1,c2,c3,c4);
	return M;
}
void Keyboard(unsigned char key, int x, int y){
	switch(key){
	case '1':
		angle += 5;
		if(angle>=360) angle = 0;
		printf("beta:%f\n",angle);
		break;
	case '2':
		angle -= 5;
		if(angle<=0) angle = 360;
		printf("beta:%f\n",angle);
		break;
	case 'w':
		eyedistance -= 0.2;
		break;
	case 's':
		eyedistance += 0.2;
		break;
	case 'a':
		eyeAngley -=10;
		break;
	case 'd':
		eyeAngley +=10;
		break;
	case 'r':
		angles[1] -= 5; 
		if(angles[1] == -360) angles[1] = 0;
		movey = 0;
		movex = 0;
		break;
	case 't':
		angles[2] -= 5;
		if(angles[2] == -360) angles[2] = 0;
		movey = 0;
		movex = 0;
		break;
	case 'q':
		break;
	case 'e':
		break;
	}
	glutPostRedisplay();
}
void menuEvents(int option){}
void ActionMenuEvents(int option){
	switch(option){
	case 0:
		action = IDLE;
		break;
	case 1:
		action = WALK;
		break;
	case 2:
		action = JUMP;
		break;
	case 3:
		action = BOWDOWN;
		break;
	case 4:
		action = ORZ;
		break;
	case 5:
		action = ATTACK;
		break;
	}
}
void ModeMenuEvents(int option){
	switch(option){
	case 0:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 1:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 2:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	}
}
void ShaderMenuEvents(int option){
	switch (option) {
	case 0:
		ambientAnim = NORMAL_AMBIENT;
		break;
	case 1:
		ambientAnim = GLOWING;
		break;
	case 2:
		ambientAnim = GLOWING_RED;
		break;
	case 3:
		ambientAnim = GLOWING_GREEN;
		break;
	case 4:
		ambientAnim = GLOWING_BLUE;
		break;
	case 5:
		ambientAnim = GLOWING_GOLD;
		break;
	}
}


void LightMenuEvents(int option) {
	lightAnim = option;
}


void ShaderAlgorithmEvents(int option) {
	shadermode = option;
}

void PostProcessEvents(int option) {
	postmode = option;
}

unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}