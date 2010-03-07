#include <iostream>
#include <sstream>
#include <fstream>
using namespace std ; 

#ifndef PARSER
#define PARSER

#define DEBUG 0 

const int DEF_SIZE_FOR_VERTICES = 100 ; 
const int DEF_SIZE_FOR_TRIANGLES = 100 ;  
const int DEF_SIZE_FOR_TEXTURES = 50 ; 
const int DEF_SIZE_FOR_COORDS = 100 ; 

class FileParser{
public: 
	FileParser(){
    num_of_vertices=0 ; 
    num_of_textures=0 ; 
    num_of_triangles=0 ; 
	}
 void Load(char * fname){
    readFromFile(fname) ; 
 }
 // Read coordinates and draw the model again. 
 void readFromFile(char * file_name){
    ifstream inFile ; 
    inFile.open(file_name) ; 
    if(!inFile){
      cerr<<"Error ! Invalid Model File Path ... \n" ; 
    }
    string line ; 
    string type_of_data ; 
    string token ; 
    int i = 0 ; 

    while(getline(inFile,line)){
      istringstream tokenizer(line) ; 
      // Check what the line begins with to see what data you are reading. 
      getline(tokenizer,token,'>'); 
      // Switch statement to see what tag it is. 
      if(strncmp(token.substr(0,2).c_str(),"</",2) == 0){
        type_of_data = "DONE" ; 
        continue ; 
      }else if(strncmp(token.substr(0,1).c_str(),"#",1) == 0){
        if(DEBUG)
          cout << "Hit this !\n" << endl ; 
        continue ; 
      }else if(strncmp(token.substr(0,10).c_str(),"<triangles",10) == 0){
        type_of_data = "triangles" ; 
      }else if(strncmp(token.substr(0,9).c_str(),"<vertices",9) == 0){
        type_of_data = "vertices" ; 
      }else if(strncmp(token.substr(0,9).c_str(),"<textures",9) == 0){
        type_of_data = "textures" ;
      }else if(strncmp(token.substr(0,8).c_str(),"<normals",8) == 0){
        type_of_data = "normals" ; 
      }else if(strncmp(token.substr(0,8).c_str(),"<name",5) == 0){
        type_of_data = "name" ; 
      }else{
        if(token.length() != 0 && (type_of_data.length() != 0) ){
          // Tokenize the string further to figure out the rest of the details
          istringstream values_on_line(token) ; 
          if(strncmp(type_of_data.substr(0,8).c_str(),"textures",8) == 0){
            texture_file_names[num_of_textures++] = token ;  
          }else if(strncmp(type_of_data.substr(0,4).c_str(),"name",4) == 0){
            name = new char[token.size()+1]; 
            strcpy(name,token.c_str());
          }else if(strncmp(type_of_data.substr(0,8).c_str(),"vertices",8) == 0){
            istringstream tokenizer(token) ; 
            for(int j=0 ; j <3 ; j++){
              float f;
              tokenizer >> f;
              vertices[num_of_vertices][j] = f ; 
            }
            i+=1 ; 
            num_of_vertices++ ; 
          }else if(strncmp(type_of_data.substr(0,9).c_str(),"triangles",9) == 0){
            istringstream stream(token) ;
            for(int j=0 ; j <6 ; j++){
              if(j == 0){
                for(int k=0 ; k < 3 ; k++){
                  int c ; 
                  stream >> c ; 
                  triangles[num_of_triangles][k] = c ; 
                }
              }else if(j == 1){
                for(int k=0 ; k < 3 ; k++){
                  float f ; 
                  stream >> f  ; 
                  normals[num_of_triangles][k] = f ; 
                }
              }else if(j == 2){
                  int k ; 
                  stream >> k ; 
                  texture_to_use[num_of_triangles] = k ; 
              }else if(j == 3){
                for(int k=0 ; k < 2 ; k++){
                  float f ; 
                  stream >> f ; 
                  texture_coords_0[num_of_triangles][k] = f ; 
                }
              }else if(j == 4){
                for(int k=0 ; k < 2 ; k++){
                  float f ; 
                  stream >> f ; 
                  texture_coords_1[num_of_triangles][k] = f ; 
                }
              }else if(j == 5){
                for(int k=0 ; k < 2 ; k++){
                  float f ; 
                  stream >> f ; 
                  texture_coords_2[num_of_triangles][k] = f ; 
                }
              }
            }
            num_of_triangles++ ; 
          }
        }
      }
    }
}

int getNumOfVertices(){
  return num_of_vertices;  
}
int getNumOfTextures(){
  return num_of_textures;  
}
int getNumOfTriangles(){
  return num_of_triangles;  
}

void buildModelBuilding(){
  glColor3f(1.0,0.0,0.0) ; 
  int num_of_vertices = getNumOfVertices() ; 
  int num_of_triangles = getNumOfTriangles() ; 
  int num_of_textures = getNumOfTextures() ; 
  float changeInColor = 0.2 ; 

  Vec3f vertices[num_of_vertices] ; 
  Vec3f triangles[num_of_triangles] ; 

  getListOfVertices(vertices) ; 
  getListOfTriangles(triangles) ; 


  // for each triangle
  glPushMatrix() ; 
  for(int i=0 ; i < num_of_triangles ; i++){
    changeInColor += 0.002 ; 
    glColor3f(changeInColor,0.0,0.0) ; 
  
    // grab it's index to the vertex and the coordinates from it. 
    int index0 = (int)triangles[i][0] ; 
    int index1 = (int)triangles[i][1] ; 
    int index2 = (int)triangles[i][2] ; 
  
    Vec3f ver_0,ver_1,ver_2 ; 
    ver_0[0] = vertices[index0][0] ; 
    ver_0[1] = vertices[index0][1] ; 
    ver_0[2] = vertices[index0][2] ; 

    ver_1[0] = vertices[index1][0] ; 
    ver_1[1] = vertices[index1][1] ; 
    ver_1[2] = vertices[index1][2] ; 

    ver_2[0] = vertices[index2][0] ; 
    ver_2[1] = vertices[index2][1] ; 
    ver_2[2] = vertices[index2][2] ; 

    // draw the triangle. 
    glBegin(GL_TRIANGLES);
      glVertex3f(ver_0[0], ver_0[1], ver_0[2]) ; 
      glVertex3f(ver_1[0], ver_1[1], ver_1[2]) ; 
      glVertex3f(ver_2[0], ver_2[1], ver_2[2]) ; 
    glEnd(); 
  }
  glPopMatrix() ; 
}

void getListOfVertices(Vec3f another_array[]){
  for(int i=0 ; i < num_of_vertices ; i++){
    another_array[i][0]  = vertices[i][0] ;
    another_array[i][1]  = vertices[i][1] ;
    another_array[i][2]  = vertices[i][2] ;
  }
}
void getListOfTriangles(Vec3f another_array[]){
  for(int i=0 ; i < num_of_triangles ; i++){
    another_array[i][0]  = triangles[i][0] ;
    another_array[i][1]  = triangles[i][1] ;
    another_array[i][2]  = triangles[i][2] ;
  }
}
void test(){
  string a = "1 2 4" ; 
  string token ; 
  istringstream tokenizer(a) ; 
  cout << name << endl ; 
  cout << texture_file_names[num_of_textures-1] << endl ; 
  for(int k=0 ; k < 3 ; k++){
    //getline(tokenizer,token,' '); 
    //istringstream float_iss(token) ;
    //float f;
    //float_iss >> f;
    //cout << f << endl ; 
    cout << "Vertices :: " << vertices[0][k] << endl ; 
    cout << "Triangle :: " << triangles[65][k] << endl ; 
    cout << normals[65][k] << endl ; 
    cout << texture_to_use[65] << endl ; 
    if(k<2){
      cout << texture_coords_0[65][k] << endl ; 
      cout << texture_coords_1[65][k] << endl ; 
      cout << texture_coords_2[65][k] << endl ; 
    }
  }
}
private: 
  int num_of_vertices ; 
  int num_of_textures ; 
  int num_of_triangles ; 
	Vec3f vertices[DEF_SIZE_FOR_VERTICES] ; 
	Vec3f triangles[DEF_SIZE_FOR_TRIANGLES] ; 
	Vec3f normals[DEF_SIZE_FOR_TRIANGLES] ; 
	string texture_file_names[DEF_SIZE_FOR_TEXTURES] ; 
	int texture_to_use[DEF_SIZE_FOR_TEXTURES] ; 
	Vec2f texture_coords_0[DEF_SIZE_FOR_COORDS] ; 
	Vec2f texture_coords_1[DEF_SIZE_FOR_COORDS] ; 
	Vec2f texture_coords_2[DEF_SIZE_FOR_COORDS] ; 
  char * name ; 
};
/*
struct Ray{
  Vec3f origin,direction ; 
  Ray(void): origin(0,0,5), direction(0,0,-1) {}
  Ray(const Vec3f& origin, const Vec3f& direction) :origin(origin),direction(direction) {}
};
*/
#endif
