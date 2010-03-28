#ifndef FILEPARSER_H
#define FILEPARSER_H
#define DEBUG 0 
const int DEF_SIZE_FOR_VERTICES = 300 ; 
const int DEF_SIZE_FOR_TRIANGLES = 300 ;  
const int DEF_SIZE_FOR_TEXTURES = 300 ; 
const int DEF_SIZE_FOR_COORDS = 300 ; 

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
            cout << "FILE IS " << token <<endl ; 
            texture_file_names[num_of_textures++] = token ;  
            cout << "FILE IS 2 " << texture_file_names[num_of_textures - 1] <<endl ;  
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
unsigned char *Read(const std::string &filename, int &width, int &height)
{
	FILE *ifp;
	char buffer[80];
	int i;
	int header[3]; // width, height, maxval
	int tmp;

	// Warning - Number of references to this class might cause problems when reading images
	  
	ifp = fopen(filename.c_str(), "rb");
	if( !ifp ) {
    cout <<"Test"<<endl ; 
		cout << "Error opening file " << filename << endl ; 
		throw std::string("Error opening file \"") + filename + std::string("\"");
	}
    cout <<"Test2"<<endl ; 
	  
	i = 0;
	fgets(buffer, 80, ifp);
	if( strncmp(buffer, "P6", 2) ) {
		fclose(ifp);
		throw std::string("File is not in valid PPM format");
	}

	while( (i < 3) ) {
		if( (tmp=fgetc(ifp)) == '#' ) {
		fgets(buffer, 80, ifp); // read out comment
		continue;
		} else {
		ungetc(tmp, ifp);
		fscanf(ifp, "%d", &header[i++]);
		}
	}
	fgets(buffer, 80, ifp); // read to newline

	// Renew image
	width = header[0];
	height = header[1];
	unsigned char *img = new unsigned char[width*height*3];

	fread(img, 3, width * height, ifp);
	  
	fclose(ifp);
	return img;
}

int LoadTextures(){
  if(!num_of_textures)
    return -1 ; 

  cout << "Total num_of_textures is "<< num_of_textures << endl ; 
  cout << "Loading .. " << texture_file_names[0] << endl ; 
  int width,height,i ; 

  // Create unique ID's for all textures that will be used. 
  glGenTextures(num_of_textures,ids) ; 

  for(i=0 ; i < num_of_textures ; i++){
    unsigned char * texarr ; 
    cout << "Loading .. " << texture_file_names[i].c_str() << endl ; 
    texarr = Read(texture_file_names[i].c_str(),width,height) ; 

    // Associate properties with each texture. 
    glBindTexture(GL_TEXTURE_2D,ids[i]) ; 
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT) ; 
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT) ; 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR) ; 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR) ; 
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,texarr) ; 

  }
  return 1 ; 
}

void buildModelBuilding(){
  int num_of_vertices = getNumOfVertices() ; 
  int num_of_triangles = getNumOfTriangles() ; 
  int num_of_textures = getNumOfTextures() ; 

  Vec3f vertices[num_of_vertices] ; 
  Vec3f triangles[num_of_triangles] ; 

  getListOfVertices(vertices) ; 
  getListOfTriangles(triangles) ; 

  // for each triangle
  glPushMatrix() ; 
  for(int i=0 ; i < num_of_triangles ; i++){

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

    // Print the normals. 
    Vec3f norm_vec_a,norm_vec_b,result;
    norm_vec_a = ver_1 - ver_0 ;  
    norm_vec_b = ver_2 - ver_1 ;  
    normalize(norm_vec_a) ; 
    normalize(norm_vec_b) ; 
    result = cross(norm_vec_a,norm_vec_b);  
    //cout << "Triangle " << index0 <<"," << index1 <<"," << index2 << " .. normal is : " << result[0] << "," << result[1] << "," << result[2] <<endl ;  
   
    // Enable texturing
    if(num_of_textures > 0){
      glEnable(GL_TEXTURE_2D) ; 
      glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE) ; 
      glBindTexture(GL_TEXTURE_2D,ids[texture_to_use[i]]) ; 
    }

    // draw the triangle. 
    glBegin(GL_TRIANGLES);
      glNormal3d(normals[i][0],normals[i][1],normals[i][2]) ; 

      glTexCoord2d(texture_coords_0[i][0],texture_coords_0[i][1]) ; 
      glVertex3f(ver_0[0], ver_0[1], ver_0[2]) ; 

      glTexCoord2d(texture_coords_1[i][0],texture_coords_1[i][1]) ; 
      glVertex3f(ver_1[0], ver_1[1], ver_1[2]) ; 

      glTexCoord2d(texture_coords_2[i][0],texture_coords_2[i][1]) ; 
      glVertex3f(ver_2[0], ver_2[1], ver_2[2]) ; 
    glEnd(); 

    // Disable texturing
    glDisable(GL_TEXTURE_2D) ; 

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
  GLuint ids[50] ; 
};
/*
struct Ray{
  Vec3f origin,direction ; 
  Ray(void): origin(0,0,5), direction(0,0,-1) {}
  Ray(const Vec3f& origin, const Vec3f& direction) :origin(origin),direction(direction) {}
};
*/
#endif
