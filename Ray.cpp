#include "includes.h"
#include "shader.hpp"
#include "controls.hpp"
#include "structs.h"
#include "objloader.hpp"
#include "functions.h"
GLFWwindow* window;


int main( void )
{
	//init bullet
	initBullet();
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//screen resolution GLFW
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int  w = mode->width;
	int  h = mode->height;

	if(MessageBox(NULL, L"Would you like to run in fullscreen?", L"Fullscreen", MB_ICONQUESTION | MB_YESNO) == IDYES) 
	{
		window = glfwCreateWindow( w, h, "Ray Tracing - Alfonso Oricchio", glfwGetPrimaryMonitor(), NULL);
		printf("fullscreen\n");
	}
	else
	{
		window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Ray Tracing - Alfonso Oricchio", NULL, NULL);
		printf("window\n");
	}
	// Open a window and create its OpenGL context
	
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(window, WINDOW_WIDTH/2, WINDOW_HEIGHT/2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	// Cull triangles which normal is not towards the camera
	//glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	
		// Create and compile our GLSL program from the shaders
	GLuint depthProgramID = LoadShaders( "DepthRTT.vertexshader", "DepthRTT.fragmentshader" );

	// Get a handle for our "MVP" uniform
	GLuint depthMatrixID = glGetUniformLocation(depthProgramID, "depthMVP");
	
	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	GLuint depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 1024, 1024, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		 
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	// No color output in the bound framebuffer, only depth.
	glDrawBuffer(GL_NONE);

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;
	/***********************************************************/
		// The quad's FBO. Used only for visualizing the shadowmap.
	static const GLfloat g_quad_vertex_buffer_data[] = { 
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
	};

	GLuint quad_vertexbuffer;
	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	// Create and compile our GLSL program from the shaders
	GLuint quad_programID = LoadShaders( "Passthrough.vertexshader", "SimpleTexture.fragmentshader" );
	GLuint texID = glGetUniformLocation(quad_programID, "texture");
	/**********************************************************/


		// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );



	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
	GLuint DepthBiasID = glGetUniformLocation(programID, "DepthBiasMVP");
	GLuint ShadowMapID = glGetUniformLocation(programID, "shadowMap");
	
	// Get a handle for our "LightPosition" uniform
	GLuint lightInvDirID = glGetUniformLocation(programID, "LightInvDirection_worldspace");

	//CUBO1
	addBox(2,2,2,0,0,0,1.0);

	//CUBO2
	addBox2(2,2,2,0,6,0,1.0);
	//FLOOR
	initFloor();

	//WALL
	initWall();
	//BALL
	addScene();

	do{
		world->stepSimulation(1/60.f);
		if (glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS){
			btRigidBody* ball = addBall(1.0,CamGetPosition().x,CamGetPosition().y,CamGetPosition().z,2.0);
			ball->setLinearVelocity(btVector3((CamGetDirection().x*50),(CamGetDirection().y*50),(CamGetDirection().z*50)));	
		}
		// Render to our framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		glViewport(0,0,1024,1024); // Render on the whole framebuffer, complete from the lower left corner to the upper right

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//////////////////////////////
		// Use our shader
		//SHADOW RENDER
		glUseProgram(depthProgramID);

		glm::vec3 lightInvDir = glm::vec3(0.5f,2,2);

		// Compute the MVP matrix from the light's point of view
		glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-10,20);
		glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,1,0));
		// or, for spot light :
		//glm::vec3 lightPos(5, 20, 20);
		//glm::mat4 depthProjectionMatrix = glm::perspective<float>(45.0f, 1.0f, 2.0f, 50.0f);
		//glm::mat4 depthViewMatrix = glm::lookAt(lightPos, lightPos-lightInvDir, glm::vec3(0,1,0));

		glm::mat4 depthModelMatrix = glm::mat4(1.0);
		glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);

		// CUBE1 -------------------------------------------------------------------------
		renderBox(bodies[0],true); //just a cube not really needed
		//CUBE2 --------------------------------------------------------------------------
		renderBox2(bodies[1],true); //just a cube not really needed
		//FLOOR --------------------------------------------------------------------------
		renderPlane(bodies[2],true); //floor 
		//WALL ---------------------------------------------------------------------------
		renderWall(bodies[3],true); //back wall
		//CASTLE -------------------------------------------------------------------------
		renderScene(true); //castle, main scene
		//BALL ---------------------------------------------------------------------------
		int iv;
		for(iv = 4; iv < bodies.size();iv++)
			{
				renderBall(bodies[iv],(iv - 4),true); //"cannon balls" shooted from the camera 
			}
		
		

		
		//////////////////////////////
		//STANDARD RENDER
		// Compute the MVP matrix from keyboard and mouse input
		// Clear the screen
		// Render to the screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0,0,1024,768); // Render on the whole framebuffer, complete from the lower left corner to the upper right

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		// Use our shader
		glUseProgram(programID);

		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;		
		
				
		glm::mat4 biasMatrix(
			0.5, 0.0, 0.0, 0.0, 
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
		);

		glm::mat4 depthBiasMVP = biasMatrix*depthMVP;

// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		glUniformMatrix4fv(DepthBiasID, 1, GL_FALSE, &depthBiasMVP[0][0]);

		glUniform3f(lightInvDirID, lightInvDir.x, lightInvDir.y, lightInvDir.z);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glUniform1i(ShadowMapID, 0);

		// CUBE1 -------------------------------------------------------------------------
		renderBox(bodies[0],false); //just a cube not really needed
		//CUBE2 --------------------------------------------------------------------------
		renderBox2(bodies[1],false); //just a cube not really needed
		//FLOOR --------------------------------------------------------------------------
		renderPlane(bodies[2],false); //floor 
		//WALL ---------------------------------------------------------------------------
		renderWall(bodies[3],false); //back wall
		//CASTLE -------------------------------------------------------------------------
		renderScene(false); //castle, main scene
		//BALL ---------------------------------------------------------------------------
	//	int iv;
		for(iv = 4; iv < bodies.size();iv++)
			{ 
				renderBall(bodies[iv],(iv - 4),false); //"cannon balls" shooted from the camera 
			}
		
	/*--------------------------------------------------*/
	// Optionally render the shadowmap (for debug only)

		// Render only on a corner of the window (or we we won't see the real rendering...)
		glViewport(0,0,512,512);

		// Use our shader
		glUseProgram(quad_programID);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		// Set our "renderedTexture" sampler to user Texture Unit 0
		glUniform1i(texID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		// You have to disable GL_COMPARE_R_TO_TEXTURE above in order to see anything !
		//glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
		glDisableVertexAttribArray(0);

	/*--------------------------------------------------*/
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );



	deleteALL();
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
