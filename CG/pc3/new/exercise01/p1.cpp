#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
using namespace std;


class point3d{
private:
    double x , y, z ;

public:
    point3d(){

    }
    point3d(double x , double y  , double z):x(x),y(y),z(z){

    }

    double getX(){
        return x;
    }

    double getY(){
        return y;
    }

    double getZ(){
        return z;
    }

    void get_all(fstream & file ){
        file<<x<<" "<<y<<" "<<z<<endl;
    }

    void print_all( ){
        cout<<x<<" "<<y<<" "<<z<<endl;
    }

    point3d operator+(point3d other ){
        return point3d(this->x + other.getX() ,this->y + other.getY(),this->z + other.getZ());
    }

    point3d operator/(double val){
        return point3d(this->x/val ,this->y/val ,this->z/val);
    }
};

void cube_with_square_faces( double min_x, double max_x,
double min_y, double max_y,
double min_z, double max_z,
std::string const& full_path_output_file ){

    fstream plyfile( full_path_output_file + ".ply", ios::out);

    double deltaz =  max_z - min_z;
    double deltay =  max_y - min_y;
    double deltax =  max_x - min_x;


    vector<point3d> cubepoints(8);
    cubepoints[0] = point3d(min_x  , min_y , min_z);
    cubepoints[1] = point3d(min_x  + deltax , min_y , min_z);
    cubepoints[2] = point3d(min_x  + deltax , min_y + deltay, min_z);
    cubepoints[3] = point3d(min_x  , min_y + deltay , min_z);
    cubepoints[4] = point3d(min_x  , min_y , min_z+ deltaz);
    cubepoints[5] = point3d(min_x + deltax , min_y , min_z+ deltaz);
    cubepoints[6] = point3d(min_x + deltax, min_y + deltay, min_z+ deltaz);
    cubepoints[7] = point3d(min_x  , min_y + deltay, min_z+ deltaz);

    vector<vector<int>> faces{
        {0,1,2,3},
        {4,5,6,7},
        {0,4,7,3},
        {1,5,6,2},
        {3,2,6,7},
        {0,1,5,4}
    };

    plyfile<<"ply"<<endl;
    plyfile<<"format ascii 1.0"<<endl;
    plyfile<<"comment Created in Blender version 4.0.2"<<endl;
    plyfile<<"element vertex 8"<<endl;
    plyfile<<"property float x"<<endl;
    plyfile<<"property float y"<<endl;
    plyfile<<"property float z"<<endl;
    plyfile<<"element face 6"<<endl;
    plyfile<<"property list uchar uint vertex_indices"<<endl;
    plyfile<<"end_header"<<endl;
    for(auto p:cubepoints){
        p.get_all(plyfile);
    }
    for(auto face:faces){
        plyfile<<"4 "<<face[0]<<" "<<face[1]<<" "<<face[2]<<" "<<face[3]<<endl;
    }

}
int main(){

    cube_with_square_faces(-1,1,-1,1,-1,1,"cube");


}