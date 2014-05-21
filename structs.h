#include "includes.h"
//mine
#include "shader.hpp"
#include "controls.hpp"

struct MeshData { //struct used for store all the informations about a mesh.
	char name[128]; // name of the mesh
	std::vector<glm::vec3> vertices; // verticies of the mesh
	std::vector<glm::vec3> normals; // normals of the mesh
	int count; //count of the mesh verticies
	char matName[128]; // material name of the mesh
	glm::vec3 Kd; //diffuse color of the mesh
	float Ns; //weight of the self illumination
	float d; //opacity of the mesh float between 0 - 1
	glm::vec3 origins; // initial position of the object
	btRigidBody* body; //associated rigid body from Bullet
};
//FLOOR
static const GLfloat g_vertex_floor[] = {
	    -1000.0f, -1.0f,-50.0f,
		-1000.0f, -1.0f, 50.0f,
		 1000.0f, -1.0f,-50.0f,//t1
		-1000.0f, -1.0f, 50.0f,
		 1000.0f, -1.0f,-50.0f,
		 1000.0f, -1.0f, 50.0f //t2
	   };
//FLOOR COLOR
	static const GLfloat g_color_floor[] = { 
		0.824f, 0.824f, 0.824f,
		0.937f, 0.937f, 0.937f,
		0.824f, 0.824f, 0.824f,
		0.937f, 0.937f, 0.937f,
		0.824f, 0.824f, 0.824f,
		0.937f, 0.937f, 0.937f
	 };

//WALL
static const GLfloat g_vertex_wall[] = {
	    -1000.0f, -1.5f, -50.0f,
		-1000.0f, 100.0f, -50.0f,
		 1000.0f, -1.5f, -50.0f,//t1
		-1000.0f, 100.0f, -50.0f,
		 1000.0f, -1.5f, -50.0f,
		 1000.0f, 100.0f, -50.0f //t2
	   };
//WALL COLOR
	static const GLfloat g_color_wall[] = { 
		0.824f, 0.824f, 0.824f,
		0.937f, 0.937f, 0.937f,
		0.824f, 0.824f, 0.824f,
		0.937f, 0.937f, 0.937f,
		0.824f, 0.824f, 0.824f,
		0.937f, 0.937f, 0.937f

	 };

//CUBE1
	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = { //origin at 0,0,0
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f
	};

	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = { 
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f,
		0.583f,  0.771f,  0.014f
	};
//-----
//CUBE2 VERTEX DATA AND COLOR
GLfloat g_vertex_cube_2[] = { //origin at 0,6,0
		-1.0f, 5.0f,-1.0f,
		-1.0f, 5.0f, 1.0f,
		-1.0f, 7.0f, 1.0f,
		 1.0f, 7.0f,-1.0f,
		-1.0f, 5.0f,-1.0f,
		-1.0f, 7.0f,-1.0f,
		 1.0f, 5.0f, 1.0f,
		-1.0f, 5.0f,-1.0f,
		 1.0f, 5.0f,-1.0f,
		 1.0f, 7.0f,-1.0f,
		 1.0f, 5.0f,-1.0f,
		-1.0f, 5.0f,-1.0f,
		-1.0f, 5.0f,-1.0f,
		-1.0f, 7.0f, 1.0f,
		-1.0f, 7.0f,-1.0f,
		 1.0f, 5.0f, 1.0f,
		-1.0f, 5.0f, 1.0f,
		-1.0f, 5.0f,-1.0f,
		-1.0f, 7.0f, 1.0f,
		-1.0f, 5.0f, 1.0f,
		 1.0f, 5.0f, 1.0f,
		 1.0f, 7.0f, 1.0f,
		 1.0f, 5.0f,-1.0f,
		 1.0f, 7.0f,-1.0f,
		 1.0f, 5.0f,-1.0f,
		 1.0f, 7.0f, 1.0f,
		 1.0f, 5.0f, 1.0f,
		 1.0f, 7.0f, 1.0f,
		 1.0f, 7.0f,-1.0f,
		-1.0f, 7.0f,-1.0f,
		 1.0f, 7.0f, 1.0f,
		-1.0f, 7.0f,-1.0f,
		-1.0f, 7.0f, 1.0f,
		 1.0f, 7.0f, 1.0f,
		-1.0f, 7.0f, 1.0f,
		 1.0f, 5.0f, 1.0f
	};

	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_cube_2[] = { 
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f,
		0.671f,  0.804f,  0.937f
		
	};


//castle

	static const GLfloat g_color_castle[] = { 
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f,
		0.18f,0.31f,0.31f		
	};

	

//LIGHT SHAPE
	//CUBE2 VERTEX DATA AND COLOR
	GLfloat g_vertex_light[] = { 
		0,9,0,
		-1,10,1,
		1,10,1,
		0,9,0,
		-1,10,1,
		-1,10,-1,
		0,9,0,
		-1,10,-1,
		1,10,-1,
		0,9,0,
		1,10,-1,
		1,10,1,//
		0,11,0,
		-1,10,1,
		1,10,1,
		0,11,0,
		-1,10,1,
		-1,10,-1,
		0,11,0,
		-1,10,-1,
		1,10,-1,
		0,11,0,
		1,10,-1,
		1,10,1

	};

		static const GLfloat g_color_light[] = { 
			1,0.847,0,
			1,0.847,0,
			1,0.847,0,
			1,0.847,0,
			1,0.847,0,
			1,0.847,0,
			1,0.847,0,
			1,0.847,0,
			1,0.847,0,
			1,0.847,0,
			1,0.847,0,
			1,0.847,0,
			1,0.847,0,
			1,0.847,0,
			1,0.847,0,
			1,0.847,0,
			1,0.847,0,
			1,0.847,0,
			1,0.847,0,
			1,0.847,0,
			1,0.847,0,
			1,0.847,0,
			1,0.847,0,
			1,0.847,0
	};
