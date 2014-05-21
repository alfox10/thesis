#include "includes.h"

int ii=0;
//polygons
GLuint colorbufferFLOOR;
GLuint vertexbufferFLOOR;
GLuint vertexbuffer;
GLuint colorbuffer;
GLuint colorbufferCASTLE;
GLuint vertexbufferCASTLE;
GLuint colorbufferWALL;
GLuint vertexbufferWALL;
GLuint vertexbufferBALL;
GLuint colorbufferBALL;
GLuint vertexbufferSCENE[CASTLE_OBJ_NUM];
GLuint colorbufferSCENE[CASTLE_OBJ_NUM];
GLuint vertexbuffer2;
GLuint colorbuffer2;
std::vector<MeshData> ms;
std::vector<MeshData> msBall;
int BallCount = 0;
//bullet
GLUquadricObj* quad;
btDynamicsWorld* world;
btDispatcher* dispatcher;
btCollisionConfiguration* collisionConfig;
btBroadphaseInterface* broadphase;
btConstraintSolver* solver;
std::vector<btRigidBody*> bodies;
std::vector<glm::vec3> g_vert_scene;
std::vector<glm::vec3> g_vert_ball_in;

//bullet
void initBullet(){	
	quad=gluNewQuadric();
	collisionConfig=new btDefaultCollisionConfiguration();
	dispatcher=new btCollisionDispatcher(collisionConfig);
	broadphase=new btDbvtBroadphase();
	solver=new btSequentialImpulseConstraintSolver();
	world=new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfig);
	world->setGravity(btVector3(0,-10,0));
	
}
//floor
void initFloor()
{
	//Bullet floor init
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(0,-1,0));
	btStaticPlaneShape* plane=new btStaticPlaneShape(btVector3(0,1,0),0);
	btMotionState* motion=new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(0.0,motion,plane);
	btRigidBody* body=new btRigidBody(info);
	world->addRigidBody(body);
	bodies.push_back(body);

	//
	
	glGenBuffers(1, &vertexbufferFLOOR);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferFLOOR);
	glBufferData(GL_ARRAY_BUFFER, sizeof( g_vertex_floor),  g_vertex_floor, GL_STATIC_DRAW);

	
	glGenBuffers(1, &colorbufferFLOOR);
	glBindBuffer(GL_ARRAY_BUFFER, colorbufferFLOOR);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_floor), g_color_floor, GL_STATIC_DRAW);
}
void initWall()
{
	//Bullet floor init
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(0,-1,-50));
	btStaticPlaneShape* plane=new btStaticPlaneShape(btVector3(0,0,1),0);
	btMotionState* motion=new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(0.0,motion,plane);
	btRigidBody* body=new btRigidBody(info);
	world->addRigidBody(body);
	bodies.push_back(body);

	//
	glGenBuffers(1, &vertexbufferWALL);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferWALL);
	glBufferData(GL_ARRAY_BUFFER, sizeof( g_vertex_wall),  g_vertex_wall, GL_STATIC_DRAW);

	glGenBuffers(1, &colorbufferWALL);
	glBindBuffer(GL_ARRAY_BUFFER, colorbufferWALL);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_wall), g_color_wall, GL_STATIC_DRAW);
}
void addScene(){

	if(int res = loadScene(CASTLE_OBJ_NAME,CASTLE_OBJ_MTL,CASTLE_OBJ_POS,ms,false)!=0){
		printf("\nError loading scene\n");
		return;
	}
	//bullet castle INIT
	
	float mass = 2.0;
	for(int bt = 0 ;bt < ms.size();bt++){
		btBoxShape* box;
		btCylinderShape* cyl;
		btConeShape* cone;
		char btn;
		btn = ms[bt].name[1];
		btTransform t;
		t.setIdentity();
		t.setOrigin(btVector3(ms[bt].origins.x,ms[bt].origins.y,ms[bt].origins.z));

		if(btn == 'o') //cone
		{
			cone = new btConeShape(CONE_RADIUS,CONE_HEIGHT);
			btVector3 inertia(0,0,0);
			if(mass!=0.0)
				cone->calculateLocalInertia(mass,inertia);
			btMotionState* motion=new btDefaultMotionState(t);
			btRigidBody::btRigidBodyConstructionInfo info(mass,motion,cone,inertia);
			btRigidBody* body=new btRigidBody(info);
			world->addRigidBody(body);
			ms[bt].body = body;
		}
		else if(btn == 'y') //cylinder
		{

			cyl = new btCylinderShape(btVector3(CYL_W/2,CYL_H/2,CYL_D/2));
			btVector3 inertia(0,0,0);
			if(mass!=0.0)
				cyl->calculateLocalInertia(mass,inertia);
			btMotionState* motion=new btDefaultMotionState(t);
			btRigidBody::btRigidBodyConstructionInfo info(mass,motion,cyl,inertia);
			btRigidBody* body=new btRigidBody(info);
			world->addRigidBody(body);
			ms[bt].body = body;
		}
		else //cube
		{

			box = new btBoxShape(btVector3(BOX_W/2,BOX_H/2,BOX_D/2));
			btVector3 inertia(0,0,0);
			if(mass!=0.0)
				box->calculateLocalInertia(mass,inertia);
			btMotionState* motion=new btDefaultMotionState(t);
			btRigidBody::btRigidBodyConstructionInfo info(mass,motion,box,inertia);
			btRigidBody* body=new btRigidBody(info);
			world->addRigidBody(body);
			ms[bt].body = body;
		}
	}

	
	for (int i = 0;i<ms.size();i++){
	
		glm::vec3 color;
		std::vector<glm::vec3> g_color_scene;
		int cc = ms[i].count;
		float xx = ms[i].Kd.x;
		float yy = ms[i].Kd.y;
		float zz = ms[i].Kd.z;

		for (int j=0; j<cc; j++)
		{
			color.r = xx;
			color.g = yy;
			color.b = zz;
			g_color_scene.push_back(color);
		}

		glGenBuffers(1, &colorbufferSCENE[i]);
		glBindBuffer(GL_ARRAY_BUFFER, colorbufferSCENE[i]);
		glBufferData(GL_ARRAY_BUFFER,ms[i].count * sizeof(glm::vec3), &g_color_scene[0], GL_STATIC_DRAW);
	}
	

}
void renderScene(bool isShadow)

{
	int c_scene = ms.size();
	
	for(int i = 0; i < c_scene; i++ ){	
		///////////////////////////////////////////////////////////
		if(isShadow){
		if(g_vert_scene.size() > 0)
			g_vert_scene.clear();
		btRigidBody* body = ms[i].body;
		btTransform t;
		body->getMotionState()->getWorldTransform(t);		
		glm::vec3 ori_scene = ms[i].origins;
		btScalar xx = t.getOrigin().getX() - ori_scene.x;
		btScalar yy = t.getOrigin().getY() - ori_scene.y;
		btScalar zz = t.getOrigin().getZ() - ori_scene.z;	
		for(int c = 0; c < ms[i].count;c++){
			glm::vec3 tmp_vec;
			tmp_vec.x = ms[i].vertices[c].x + xx;
			tmp_vec.y = ms[i].vertices[c].y + yy;
			tmp_vec.z = ms[i].vertices[c].z + zz;
			g_vert_scene.push_back(tmp_vec);
		}
		}
		//////////////////////////////////////////////////////////
		glGenBuffers(1, &vertexbufferSCENE[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferSCENE[i]);
		glBufferData(GL_ARRAY_BUFFER,  g_vert_scene.size() * sizeof(glm::vec3),  &g_vert_scene[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferSCENE[i]);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbufferSCENE[i]);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		
		// Draw the triangle !
		int c_mesh = ms[i].count;
		glDrawArrays(GL_TRIANGLES, 0, c_mesh);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1); 
	}
	
}

void renderPlane(btRigidBody* plane, bool isShadow)
{

        if(plane->getCollisionShape()->getShapeType()!=STATIC_PLANE_PROXYTYPE)
                return;
        btTransform t;
        plane->getMotionState()->getWorldTransform(t);
      //translation,rotation
        glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferFLOOR);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		if(!isShadow){
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbufferFLOOR);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		}
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 2*3); // 2*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		if(!isShadow)
			glDisableVertexAttribArray(1); 
}

//wall
void renderWall(btRigidBody* plane, bool isShadow)
{

        if(plane->getCollisionShape()->getShapeType()!=STATIC_PLANE_PROXYTYPE)
                return;

        btTransform t;
        plane->getMotionState()->getWorldTransform(t);
    
        glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferWALL);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		if(!isShadow){
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbufferWALL);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		}
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 2*3); // 2*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		if(!isShadow)
			glDisableVertexAttribArray(1); 
}


btRigidBody* addBox(float width,float height,float depth,float x,float y,float z,float mass)
{

	//Cube OpenGL init
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	//Cube Bullet init
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x,y,z));
	btBoxShape* box=new btBoxShape(btVector3(width/2.0,height/2.0,depth/2.0));
	btVector3 inertia(0,0,0);
	if(mass!=0.0)
		box->calculateLocalInertia(mass,inertia);

	btMotionState* motion=new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass,motion,box,inertia);
	btRigidBody* body=new btRigidBody(info);
	world->addRigidBody(body);
	bodies.push_back(body);
	return body;
}
btRigidBody* addBox2(float width,float height,float depth,float x,float y,float z,float mass)
{

	//Cube OpenGL init

	glGenBuffers(1, &colorbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_cube_2), g_color_cube_2, GL_STATIC_DRAW);

	//Cube Bullet init
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x,y,z));
	btBoxShape* box=new btBoxShape(btVector3(width/2.0,height/2.0,depth/2.0));
	btVector3 inertia(0,0,0);
	if(mass!=0.0)
		box->calculateLocalInertia(mass,inertia);

	btMotionState* motion=new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass,motion,box,inertia);
	btRigidBody* body=new btRigidBody(info);
	world->addRigidBody(body);
	bodies.push_back(body);
	return body;
}

void renderBox(btRigidBody* box, bool isShadow)
{

	if(box->getCollisionShape()->getShapeType()!=BOX_SHAPE_PROXYTYPE)
		return;
	
	btTransform t;
	box->getMotionState()->getWorldTransform(t);

	btScalar xx = t.getOrigin().getX();
	btScalar yy = t.getOrigin().getY();
	btScalar zz = t.getOrigin().getZ();
	
	GLfloat g_vertex_cube_2in[] = {
		xx-1, yy-1,zz-1,
		xx-1, yy-1, zz+1,
		xx-1, yy+1, zz+1,
		xx+1, yy+1,zz-1,
		xx-1, yy-1,zz-1,
		xx-1, yy+1,zz-1,
		xx+1, yy-1, zz+1,
		xx-1, yy-1,zz-1,
		xx+1, yy-1,zz-1,
		xx+1, yy+1,zz-1,
		xx+1, yy-1,zz-1,
		xx-1, yy-1,zz-1,
		xx-1, yy-1,zz-1,
		xx-1, yy+1, zz+1,
		xx-1, yy+1,zz-1,
		xx+1, yy-1, zz+1,
		xx-1, yy-1, zz+1,
		xx-1, yy-1,zz-1,
		xx-1, yy+1, zz+1,
		xx-1, yy-1, zz+1,
		xx+1, yy-1, zz+1,
		xx+1, yy+1, zz+1,
		xx+1, yy-1,zz-1,
		xx+1, yy+1,zz-1,
		xx+1, yy-1,zz-1,
		xx+1, yy+1, zz+1,
		xx+1, yy-1, zz+1,
		xx+1, yy+1, zz+1,
		xx+1, yy+1,zz-1,
		xx-1, yy+1,zz-1,
		xx+1, yy+1, zz+1,
		xx-1, yy+1,zz-1,
		xx-1, yy+1, zz+1,
		xx+1, yy+1, zz+1,
		xx-1, yy+1, zz+1,
		xx+1, yy-1, zz+1
	};

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof( g_vertex_cube_2in),  g_vertex_cube_2in, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// 2nd attribute buffer : colors
	if(!isShadow){
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);
	}
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

	glDisableVertexAttribArray(0);
	
	if(!isShadow)
		glDisableVertexAttribArray(1);             

}



void renderBox2(btRigidBody* box, bool isShadow)
{

	if(box->getCollisionShape()->getShapeType()!=BOX_SHAPE_PROXYTYPE)
		return;

	btTransform t;
	box->getMotionState()->getWorldTransform(t);

	btScalar xx = t.getOrigin().getX();
	btScalar yy = t.getOrigin().getY();
	btScalar zz = t.getOrigin().getZ();
	
	GLfloat g_vertex_cube_2in[] = {
		xx-1, yy-1,zz-1,
		xx-1, yy-1, zz+1,
		xx-1, yy+1, zz+1,
		xx+1, yy+1,zz-1,
		xx-1, yy-1,zz-1,
		xx-1, yy+1,zz-1,
		xx+1, yy-1, zz+1,
		xx-1, yy-1,zz-1,
		xx+1, yy-1,zz-1,
		xx+1, yy+1,zz-1,
		xx+1, yy-1,zz-1,
		xx-1, yy-1,zz-1,
		xx-1, yy-1,zz-1,
		xx-1, yy+1, zz+1,
		xx-1, yy+1,zz-1,
		xx+1, yy-1, zz+1,
		xx-1, yy-1, zz+1,
		xx-1, yy-1,zz-1,
		xx-1, yy+1, zz+1,
		xx-1, yy-1, zz+1,
		xx+1, yy-1, zz+1,
		xx+1, yy+1, zz+1,
		xx+1, yy-1,zz-1,
		xx+1, yy+1,zz-1,
		xx+1, yy-1,zz-1,
		xx+1, yy+1, zz+1,
		xx+1, yy-1, zz+1,
		xx+1, yy+1, zz+1,
		xx+1, yy+1,zz-1,
		xx-1, yy+1,zz-1,
		xx+1, yy+1, zz+1,
		xx-1, yy+1,zz-1,
		xx-1, yy+1, zz+1,
		xx+1, yy+1, zz+1,
		xx-1, yy+1, zz+1,
		xx+1, yy-1, zz+1
	};
	glGenBuffers(1, &vertexbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof( g_vertex_cube_2in),  g_vertex_cube_2in, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// 2nd attribute buffer : colors
	if(!isShadow){
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);
	}
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

	glDisableVertexAttribArray(0);
	if(!isShadow)
		glDisableVertexAttribArray(1);             

}

//castle

btRigidBody* addBall(float radius,float x,float y,float z,float mass)
{
	
	// Read our .obj file
	std::vector<glm::vec3> g_color_ball;
	glm::vec3 color;
	if(int res = loadScene(BALL_OBJ_NAME,BALL_OBJ_MTL,BALL_OBJ_POS, msBall, true)!=0)
		return NULL;
	
	glGenBuffers(1, &vertexbufferBALL);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferBALL);
	glBufferData(GL_ARRAY_BUFFER, msBall[BallCount].count * sizeof(glm::vec3), &msBall[BallCount].vertices[0], GL_STATIC_DRAW);
	//create color!!!
	

	int vtx_clr_cnt = msBall[BallCount].count;
	int gt;
	for (gt = 0; gt < vtx_clr_cnt; gt++)
	{	
		color.r = msBall[BallCount].Kd.x;
		color.g = msBall[BallCount].Kd.y;
		color.b = msBall[BallCount].Kd.z;	
		g_color_ball.push_back(color);	
	} 
	
	glGenBuffers(1, &colorbufferBALL);
	glBindBuffer(GL_ARRAY_BUFFER, colorbufferBALL);
	glBufferData(GL_ARRAY_BUFFER, g_color_ball.size() * sizeof(glm::vec3), &g_color_ball[0], GL_STATIC_DRAW);

	//Sphere Bullet init
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x,y,z));
	btSphereShape* sphere=new btSphereShape(btScalar(radius));
	btVector3 inertia(0,0,0);
	if(mass!=0.0)
		sphere->calculateLocalInertia(mass,inertia);

	btMotionState* motion=new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass,motion,sphere,inertia);
	btRigidBody* body = new btRigidBody(info);
	world->addRigidBody(body);
	bodies.push_back(body);
	msBall[BallCount].body = body;
	BallCount++;
	return body;
}

void renderBall(btRigidBody* sphere, int sphere_count, bool isShadow)
{
	
	if(sphere->getCollisionShape()->getShapeType()!=SPHERE_SHAPE_PROXYTYPE)
		return;
	if(isShadow){
		if(g_vert_ball_in.size()>0)
			g_vert_ball_in.clear();
		btTransform t;
		sphere->getMotionState()->getWorldTransform(t);
		glm::vec3 ori_ball =msBall[sphere_count].origins;
		btScalar xx = t.getOrigin().getX() - ori_ball.x;
		btScalar yy = t.getOrigin().getY() - ori_ball.y;
		btScalar zz = t.getOrigin().getZ() - ori_ball.z;


		for(int c = 0; c < msBall[sphere_count].count;c ++){
			glm::vec3 tmp_vec;
			tmp_vec.x = msBall[sphere_count].vertices[c].x + xx;
			tmp_vec.y = msBall[sphere_count].vertices[c].y + yy;
			tmp_vec.z = msBall[sphere_count].vertices[c].z + zz;
			g_vert_ball_in.push_back(tmp_vec);
		}
	}

	glGenBuffers(1, &vertexbufferBALL);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferBALL);
	glBufferData(GL_ARRAY_BUFFER, g_vert_ball_in.size() * sizeof(glm::vec3),  &g_vert_ball_in[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferBALL);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// 2nd attribute buffer : colors
	if(!isShadow){
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbufferBALL);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);
	}
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, msBall[sphere_count].count); // 12*3 indices starting at 0 -> 12 triangles

	glDisableVertexAttribArray(0);
	
	if(!isShadow)
		glDisableVertexAttribArray(1);             
	
}

void deleteALL(){
	for(int i=0;i<bodies.size();i++)
	{
		world->removeCollisionObject(bodies[i]);
		btMotionState* motionState=bodies[i]->getMotionState();
		btCollisionShape* shape=bodies[i]->getCollisionShape();
		delete bodies[i];
		delete shape;
		delete motionState;
	}
	for(int i=0;i<ms.size();i++)
	{
		world->removeCollisionObject(ms[i].body);
		btMotionState* motionState=ms[i].body->getMotionState();
		btCollisionShape* shape=ms[i].body->getCollisionShape();
		delete ms[i].body;
		delete shape;
		delete motionState;
	}
	for(int i=0;i<msBall.size();i++)
	{
		world->removeCollisionObject(msBall[i].body);
		btMotionState* motionState=msBall[i].body->getMotionState();
		btCollisionShape* shape=msBall[i].body->getCollisionShape();
		delete msBall[i].body;
		delete shape;
		delete motionState;
	}
	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteBuffers(1, &vertexbuffer2);
	glDeleteBuffers(1, &colorbuffer2);
	glDeleteBuffers(1, &vertexbufferWALL);
	glDeleteBuffers(1, &colorbufferWALL);
	glDeleteBuffers(1, &vertexbufferFLOOR);
	glDeleteBuffers(1, &colorbufferFLOOR);
	glDeleteBuffers(1, &vertexbufferCASTLE);
	glDeleteBuffers(1, &colorbufferCASTLE);
	delete dispatcher;
	delete collisionConfig;
	delete solver;
	delete broadphase;
	delete world;
	gluDeleteQuadric(quad);

}
