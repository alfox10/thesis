// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>
// Include GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
using namespace glm; 
#include <string.h>
//this function is used to load the material info about the mesh
int FillMatInfo(MeshData & mesh,
				const char* mat_path,
				bool isBall);
void SceneSetOrigin(std::vector<MeshData> & mscene,
			   const char* origins_path);

FILE *stream,*streamMat,*streamOri;
int count=0;
std::vector<MeshData> MatInfo;
bool first=true;
/***************
*  DEPRECATED  *
****************/
int loadMesh( const char * path, 
			 std::vector<glm::vec3> & out_vertices,
			 std::vector<glm::vec3> & out_normals)
{
	
	std::vector<glm::vec3> tmp_vertices;
	std::vector<unsigned int> vertexIndices;
	char s[128];
	unsigned int v[3]/*array for verticies*/,n[3]/*array for normals - not used yet -*/;
	errno_t err = fopen_s( &stream, path, "r" );
	if( err ){
		printf_s( "The file %s was not opened\n",path );
		return 1;
	}

		printf("Loading Mesh %s ...\n",path);

		while(fscanf_s( stream, "%s", s, _countof(s) )!=EOF){
			if(strcmp(s,"v")==0){
				glm::vec3 vert;
				fscanf_s( stream, "%f %f %f", &vert.x,&vert.y,&vert.z);
				tmp_vertices.push_back(vert);	
			}
			if(strcmp(s,"f")==0){		
				fscanf_s( stream, "%d//%d %d//%d %d//%d",&v[0],&n[0],&v[1],&n[1],&v[2],&n[2]);
				vertexIndices.push_back(v[0]);
				vertexIndices.push_back(v[1]);
				vertexIndices.push_back(v[2]);
			}
		}
		count = vertexIndices.size();
		for( unsigned int i=0; i<vertexIndices.size(); i++ ){

			// Get the indices of its attributes
			unsigned int vertexIndex = vertexIndices[i];

			// Get the attributes thanks to the index
			glm::vec3 vertex = tmp_vertices[ vertexIndex-1 ];

			// Put the attributes in buffers
			out_vertices.push_back(vertex);
		}

		fclose( stream );
		printf("Mesh Loaded!\n");
	return 0;
}

/***************
*  DEPRECATED  *
****************/
int getVertexCount(){
	return count;
}
/*********************
*  Load an OBJ File  *
**********************/
int loadScene(const char* path,
			  const char* mat_path,
			  const char* ori_path,
			  std::vector<MeshData> & scene,
			  bool isBall){
	bool isFirst = true;
	
	std::vector<glm::vec3> tmp_vertices;
	std::vector<glm::vec3> tmp_normals;
	std::vector<unsigned int> vertexIndices,normalIndices;
	static const MeshData EmptyStruct;
	MeshData tmp_mesh;
	char s[128];
	unsigned int v[3]/*array for verticies*/,n[3]/*array for normals - not used yet -*/;

	errno_t err = fopen_s( &stream, path, "r" );
	if( err ){
		printf_s( "The file %s was not opened\n",path );
		return 1;
	}
	printf("Processing Scene...%s\n",path);
	while(fscanf_s( stream, "%s", s, _countof(s) )!=EOF){
		if(strcmp(s,"o")==0){
			if(isFirst){
				fscanf_s( stream, "%s", tmp_mesh.name,  _countof(tmp_mesh.name));
				isFirst = false;
				//printf("Processing mesh: %s\n",tmp_mesh.name);
			}
			else{
				
				//impacchetta i dati, mettili in std::vector<MeshData> scene
				tmp_mesh.count = vertexIndices.size();
				
				for( unsigned int i=0; i<vertexIndices.size(); i++ ){
					// Get the indices of its attributes
					
					unsigned int vertexIndex = vertexIndices[i];
					unsigned int normalIndex = normalIndices[i];
					// Get the attributes thanks to the index
					
					glm::vec3 vertex = tmp_vertices[ vertexIndex-1 ];
					glm::vec3 norm = tmp_normals[ normalIndex-1 ];
					// Put the attributes in buffers
					
					tmp_mesh.vertices.push_back(vertex);
					tmp_mesh.normals.push_back(norm);
					
				}
				//process material

				FillMatInfo(tmp_mesh,mat_path,isBall);
				//printf("%s %s count:%d kd:%f %f %f ns:%f d:%f",tmp_mesh.name,tmp_mesh.matName,tmp_mesh.count,tmp_mesh.Kd.x,tmp_mesh.Kd.y,tmp_mesh.Kd.z,tmp_mesh.Ns,tmp_mesh.d);
				//insert into meshdata vector				
				scene.push_back(tmp_mesh);

				//reset all, start new object
				vertexIndices.clear();
				normalIndices.clear();
				//tmp_vertices.clear(); tmp_normals.clear();
				tmp_mesh = EmptyStruct;
				fscanf_s( stream, "%s", tmp_mesh.name, _countof(tmp_mesh.name));
				//printf("Processing mesh: %s\n",tmp_mesh.name);
			}
		}
		if(strcmp(s,"v")==0){
			glm::vec3 vert;
			fscanf_s( stream, "%f %f %f", &vert.x,&vert.y,&vert.z);
			tmp_vertices.push_back(vert);	
		}
		if ( strcmp( s, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf_s(stream, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			tmp_normals.push_back(normal);
		}
		if(strcmp(s,"f")==0){		
			fscanf_s( stream, "%d//%d %d//%d %d//%d",&v[0],&n[0],&v[1],&n[1],&v[2],&n[2]);
			vertexIndices.push_back(v[0]);
			vertexIndices.push_back(v[1]);
			vertexIndices.push_back(v[2]);
			normalIndices.push_back(n[0]);
			normalIndices.push_back(n[1]);
			normalIndices.push_back(n[2]);
		}
		if (strcmp(s,"usemtl")==0){
			fscanf_s( stream, "%s", tmp_mesh.matName,  _countof(tmp_mesh.matName));
			
		}

	}
	// insert into the vector last mesh
	tmp_mesh.count = vertexIndices.size();
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){
		// Get the indices of its attributes

		unsigned int vertexIndex = vertexIndices[i];
		unsigned int normalIndex = normalIndices[i];
		// Get the attributes thanks to the index

		glm::vec3 vertex = tmp_vertices[ vertexIndex-1 ];
		glm::vec3 norm = tmp_normals[ normalIndex-1 ];
		// Put the attributes in buffers

		tmp_mesh.vertices.push_back(vertex);
		tmp_mesh.normals.push_back(norm);

	}
	//process last material
	FillMatInfo(tmp_mesh,mat_path,isBall);
	//push last mesh in the vector
	scene.push_back(tmp_mesh);
	//set origins for all meshs
	SceneSetOrigin(scene,ori_path);
	//close stream
	fclose( stream );
	printf("Scene %s Processed!\n",path);
	return 0;
}
/**************************************************************
*  Load a MTL File (internal function called by loadScene())  *
***************************************************************/
int FillMatInfo(MeshData & mesh,
				const char* mat_path,
				bool isBall)
{
	
	if(first || isBall){
		char s[128];
		errno_t err = fopen_s( &streamMat, mat_path, "r" );
		if( err ){
			printf_s( "The file %s was not opened\n",mat_path );
			return 1;
		}
		MeshData tmpm;
		while(fscanf_s( streamMat, "%s", s, _countof(s) )!=EOF){
			
			if(strcmp(s,"newmtl")==0){
				if(first){
					first = false;
					fscanf_s( streamMat, "%s", tmpm.matName, _countof(tmpm.matName));
					
				}
				else{
					MatInfo.push_back(tmpm);
					
					MeshData reset;
					tmpm = reset;
					fscanf_s( streamMat, "%s", tmpm.matName, _countof(tmpm.matName));
				}
			}
			else if(strcmp(s,"Kd")==0){
				fscanf_s( streamMat, "%f %f %f", &tmpm.Kd.x,&tmpm.Kd.y,&tmpm.Kd.z);
				
			}
			else if(strcmp(s,"d")==0){
				fscanf_s( streamMat, "%f", &tmpm.d);
				
			}
			else if(strcmp(s,"Ns")==0){
				float c;
				fscanf_s( streamMat, "%f", &tmpm.Ns);
				
			}


		}
		MatInfo.push_back(tmpm);
		fclose(streamMat);
	}
	bool found = false;
	for(int i = 0; i < MatInfo.size();i++){
		if(strcmp(MatInfo[i].matName,mesh.matName)==0){
			
			mesh.d = MatInfo[i].d;
			mesh.Ns = MatInfo[i].Ns;
			mesh.Kd.x = MatInfo[i].Kd.x;
			mesh.Kd.y = MatInfo[i].Kd.y;
			mesh.Kd.z = MatInfo[i].Kd.z;
			found = true;
		}
	}

	if (!found)
		printf("material not found\n");
	return 0;
}

void SceneSetOrigin(std::vector<MeshData> & mscene, const char* origins_path){
	bool found = false;
	char s[128];
	errno_t err = fopen_s( &streamOri, origins_path, "r" );
		if( err ){
			printf_s( "The file %s was not opened\n",origins_path );
			return;
		}
		while(fscanf_s( streamOri, "%s", s, _countof(s) )!=EOF){
			for(int i = 0; i<mscene.size();i++){
				if(strcmp(mscene[i].name,s)==0){
					found = true;
					fscanf_s( streamOri, "%f %f %f", &mscene[i].origins.x, &mscene[i].origins.y,&mscene[i].origins.z);
				}
			}
		}
		if(!found)
			printf("Origins not found!\n");
	fclose(streamOri);

}
