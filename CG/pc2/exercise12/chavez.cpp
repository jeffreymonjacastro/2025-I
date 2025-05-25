#include <iostream>
#include <climits>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>
#include <gtest/gtest.h>
#define ll long long
#define ld long double

using namespace std;

template <typename T>
int sgn(T val) {
    if (val < static_cast<T>(0)) return -1;
    if (val > static_cast<T>(0)) return 1;
    return 0;
}

template <typename T>
class Vector {
private:
    bool is_int;

public:
    T x_, y_, z_;

    Vector() : x_(T{}), y_(T{}), z_(T{}) {
        is_int = is_same<int, T>::value;
    }

    Vector(int x, int y, int z) {
        is_int = is_same<int, T>::value;
        if (is_int) {
            x_ = static_cast<T>(x);
            y_ = static_cast<T>(y);
            z_ = static_cast<T>(z);
        } else {
            x_ = static_cast<T>(x);
            y_ = static_cast<T>(y);
            z_ = static_cast<T>(z);
        }
    }

    Vector(double x, double y, double z) {
        is_int = is_same<int, T>::value;
        if (is_int) {
            x_ = (x > INT_MAX) ? INT_MAX : ((x < INT_MIN) ? INT_MIN : static_cast<T>(x));
            y_ = (y > INT_MAX) ? INT_MAX : ((y < INT_MIN) ? INT_MIN : static_cast<T>(y));
            z_ = (z > INT_MAX) ? INT_MAX : ((z < INT_MIN) ? INT_MIN : static_cast<T>(z));
        } else {
            x_ = static_cast<T>(x);
            y_ = static_cast<T>(y);
            z_ = static_cast<T>(z);
        }
    }

    Vector(long double x, long double y, long double z) {
        is_int = is_same<int, T>::value;
        if (is_int) {
            x_ = (x > INT_MAX) ? INT_MAX : ((x < INT_MIN) ? INT_MIN : static_cast<T>(x));
            y_ = (y > INT_MAX) ? INT_MAX : ((y < INT_MIN) ? INT_MIN : static_cast<T>(y));
            z_ = (z > INT_MAX) ? INT_MAX : ((z < INT_MIN) ? INT_MIN : static_cast<T>(z));
        } else {
            x_ = static_cast<T>(x);
            y_ = static_cast<T>(y);
            z_ = static_cast<T>(z);
        }
    }

    Vector(long long x, long long y, long long z) {
        is_int = is_same<int, T>::value;
        if (is_int) {
            x_ = (x > INT_MAX) ? INT_MAX : ((x < INT_MIN) ? INT_MIN : static_cast<T>(x));
            y_ = (y > INT_MAX) ? INT_MAX : ((y < INT_MIN) ? INT_MIN : static_cast<T>(y));
            z_ = (z > INT_MAX) ? INT_MAX : ((z < INT_MIN) ? INT_MIN : static_cast<T>(z));
        } else {
            x_ = static_cast<T>(x);
            y_ = static_cast<T>(y);
            z_ = static_cast<T>(z);
        }
    }

    [[nodiscard]] ld dot_product(const Vector<T>& other) const {
        if (is_int) {
            ll a = (ll)other.x_ * (ll)x_;
            ll b = (ll)other.y_ * (ll)y_;
            ll c = (ll)other.z_ * (ll)z_;
            return a + b + c;
        }

        ld a = (ld)other.x_ * (ld)x_;
        ld b = (ld)other.y_ * (ld)y_;
        ld c = (ld)other.z_ * (ld)z_;
        return a + b + c;
    }

    Vector<ld> cross_product(const Vector<T>& other) const {
        if (is_int) {
            ll x = (ll)y_ * (ll)other.z_ - (ll)z_ * (ll)other.y_;
            ll y = (ll)z_ * (ll)other.x_ - (ll)x_ * (ll)other.z_; 
            ll z = (ll)x_ * (ll)other.y_ - (ll)y_ * (ll)other.x_;
            return Vector<ld>((ld)x, (ld)y, (ld)z);
        }
        
        ld x = (ld)y_ * (ld)other.z_ - (ld)z_ * (ld)other.y_;
        ld y = (ld)z_ * (ld)other.x_ - (ld)x_ * (ld)other.z_; 
        ld z = (ld)x_ * (ld)other.y_ - (ld)y_ * (ld)other.x_;
        return Vector<ld>(x, y, z);
    }

    Vector<ld> operator+(const Vector<T>& other) const {
        if (is_int) {
            ll x = (ll)x_ + (ll)other.x_;
            ll y = (ll)y_ + (ll)other.y_;
            ll z = (ll)z_ + (ll)other.z_;
            return Vector<ld>((ld)x, (ld)y, (ld)z);
        } else {
            ld x = (ld)x_ + (ld)other.x_;
            ld y = (ld)y_ + (ld)other.y_;
            ld z = (ld)z_ + (ld)other.z_;
            return Vector<ld>(x, y, z);
        }
    }

    Vector<ld> operator-(const Vector<T>& other) const {
        if (is_int) {
            ll x = (ll)x_ - (ll)other.x_;
            ll y = (ll)y_ - (ll)other.y_;
            ll z = (ll)z_ - (ll)other.z_;
            return Vector<ld>((ld)x, (ld)y, (ld)z);
        } else {
            ld x = (ld)x_ - (ld)other.x_;
            ld y = (ld)y_ - (ld)other.y_;
            ld z = (ld)z_ - (ld)other.z_;
            return Vector<ld>(x, y, z);
        }
    }

    bool operator==(const Vector<T>& other) const {
        return x_ == other.x_ && y_ == other.y_ && z_ == other.z_;
    }

    ld norm() {
        return sqrt(dot_product(*this));
    }

    void print() {
        cout << "X :" << x_ << endl;
        cout << "Y :" << y_ << endl;
        cout << "Z :" << z_ << endl;
    }
};

bool my_xor(bool a, bool b) {
    return a != b;
}

template<typename T>
vector<Vector<ld>> vector_vertices(vector<vector<T>> const& vertices){
    vector<Vector<ld>> ans;
    ld zero = static_cast<ld>(0);
    for(auto vertice:vertices){
        ans.emplace_back(static_cast<ld>(vertice[0]),static_cast<ld>(vertice[1]),zero);
    }

    return ans;

} 


template <typename T>
double area(vector<vector<T>> const& vertices){

    vector<Vector<ld>> v = vector_vertices(vertices);
    ld zero = static_cast<ld>(0);
    Vector<ld> triangle_vertex = v[0];
    
    double current_area = 0.0;
    
    for (int i = 2; i < v.size(); i++)
    {   
        Vector<ld> b_vertice (v[i].x_ , v[i].y_ , zero);
        Vector<ld> a_vertice (v[i-1].x_ , v[i-1].y_ , zero);
  
        Vector<ld> side_a = b_vertice -  triangle_vertex;
        Vector<ld> side_b = a_vertice -  triangle_vertex;
        
        Vector<ld> parelelo_gram = side_b.cross_product(side_a);
 
     
        current_area += parelelo_gram.norm();
    }
    
    return double(current_area)/double(2.0);
    

}

template<typename T>
bool is_vx_inside(Vector<T> v1 , Vector<T> v2 , Vector<T> vx , int orientation){

    Vector<T> segment_dir = v2- v1;

    Vector<T> vx_dir = vx-v1;

    int placement = sgn<T>(vx_dir.cross_product(segment_dir).z_);

    return placement == orientation;
}


template<typename T> 
bool is_vx_inside_colinear_ver(Vector<T> v1 , Vector<T> v2 , Vector<T> p){


    Vector<T> v1p = p - v1;
    Vector<T> v2p = p - v2;

    int sign = sgn<T>(v2p.dot_product(v1p));

    return sign == -1;

}

template<typename T> 
pair<int , int> is_there_intersection_line_seg(Vector<T> v1 , Vector<T> v2 , Vector<T> v3 , Vector<T> v4){

    Vector<T> dir = v2 - v1;
    Vector<T> pos3 = v3 - v1;
    Vector<T> pos4 = v4 - v1;
    pair<int , int> ans = make_pair(0, -1);
    int sign3 = sgn<T>(dir.cross_product(pos3).z_);
    int sign4 = sgn<T>(dir.cross_product(pos4).z_);

    if(sign3 == 0 and sign4 == 0) 
    {
        ans.first = 1;
        ans.second = 0;
        return ans;
    }
    else if (sign3 == 0 and sign4 != 0)
    {
        ans.first = (is_vx_inside_colinear_ver(v1, v2, v3)) ? 1 : 0;
        ans.second = 3;
        return ans;
    }
    else if(sign3 != 0 and sign4 == 0)
    {
        ans.first = (is_vx_inside_colinear_ver(v1, v2, v4)) ? 1 : 0;
        ans.second = 4;
        return ans;
    }
    else if (sign3 != sign4)
    {
        ans.first = 1;
        ans.second = -1;
        return ans;
    }

    ans.first = 0;
    return ans;   
}


Vector<ld> calculate_intersection(Vector<ld> v1 , Vector<ld> v2 , Vector<ld> v3 , Vector<ld> v4){
    auto dir1 = v2 - v1;
    auto dir2 = v4 - v3;
    auto diff = v3 - v1;
    
    auto cross_dirs = dir1.cross_product(dir2);
    ld denominator = (ld)cross_dirs.z_;
    
    auto cross1 = diff.cross_product(dir2);
    ld t1 = (ld)cross1.z_ / denominator;
    
    ld intersection_x = (ld)v1.x_ + t1 * (ld)dir1.x_;
    ld intersection_y = (ld)v1.y_ + t1 * (ld)dir1.y_;
    ld zero = static_cast<ld>(0);
    
    return Vector<ld>(intersection_x, intersection_y, zero);
}

    
Vector<ld>  calculate_intersection_colinear(Vector<ld> v1 , Vector<ld> v2 , Vector<ld> outside){
        auto disv1 = outside - v1; 
        auto disv2 = outside - v2;

        ld d1 = disv1.norm();
        ld d2 = disv2.norm();

        auto ans = (d1>d2)?v1:v2;
        return Vector<ld>((ld)ans.x_ , (ld)ans.y_ , (ld)ans.z_);
}



    template<typename T>
    vector<Vector<ld>> to_vector_class(vector<vector<T>> const& vertices){
        ld zero = static_cast<ld>(0);
        vector<Vector<ld>>  ans;
        for(auto v:vertices){
            ans.emplace_back((ld)v[0], (ld)v[1], zero);
        }

        return ans;

    }


    template<typename T>
    void clasify_points(bool & bv3 , bool&bv4 , int colinear_code  , Vector<T> v1 , Vector<T> v2 , Vector<T> v3 , Vector<T> v4 , int orientation ){
        if(colinear_code!=-1)
        {
                if(colinear_code == 0) 
                {
                     bv3 = is_vx_inside_colinear_ver(v1 , v2 , v3);
                     bv4 = is_vx_inside_colinear_ver(v1 , v2 , v4);
                }else if(colinear_code == 3)
                {
                    bv3 = is_vx_inside_colinear_ver(v1 , v2 , v3);
                    bv4 = is_vx_inside(v1,v2 ,v4 ,orientation);
                }else if(colinear_code == 4){
                    bv4 = is_vx_inside_colinear_ver(v1 , v2 , v4);
                    bv3 = is_vx_inside(v1,v2, v3, orientation);
                }
        }else{
                 bv3 = is_vx_inside(v1,v2 ,v3, orientation);
                 bv4 = is_vx_inside(v1,v2 ,v4, orientation);
        }
        
    }


template <typename T>
double area_of_intersection(vector<vector<T>> const& vertices1, vector<vector<T>> const& vertices2){
    ld zero = static_cast<ld>(0);
    vector<Vector<ld>> pol1 = to_vector_class(vertices1);
    vector<Vector<ld>> pol2 = to_vector_class(vertices2);
    pol1.push_back(pol1[0]);
    pol2.push_back(pol2[0]);

    Vector<ld> p0p1 = pol1[1]-pol1[0];
    Vector<ld> p0p2 = pol1[2]-pol1[0];
    int p_orientation =  sgn<ld>(p0p2.cross_product(p0p1).z_);

    int n = pol1.size();
    

    vector<Vector<ld>> polc;
    polc = pol2;

    for (int i = 1; i < n; i++)
    {   Vector<ld> v1 = pol1[i-1];
        Vector<ld> v2 = pol1[i];    
        vector<Vector<ld>> temp_pol;
        
        for (int j = 1; j < polc.size(); j++)
        {   
            Vector<ld> v3 = polc[j-1];
            Vector<ld> v4 = polc[j]; 
            bool is_insidev3 = false;
            bool is_insidev4 = false;
            

            pair<int, int> signaling = is_there_intersection_line_seg(v1 ,v2 ,v3 , v4);
            clasify_points(is_insidev3 , is_insidev4 , signaling.second,v1 ,v2 ,v3 , v4 , p_orientation);

            
            if(is_insidev3 and is_insidev4){
                temp_pol.push_back(v4);
            }else if(!is_insidev3 and !is_insidev4){
                continue;
            }else if(is_insidev3 and !is_insidev4){

                Vector<ld> intersection;

                  if(signaling.second!=-1)
        {
                if(signaling.second == 0) 
                {
                     intersection =calculate_intersection_colinear(v1 ,v2 ,v4);
                }else if(signaling.second == 3)
                { 
                    intersection = v3;
                }else if(signaling.second == 4){
                    intersection = v4;
                }
        }else{
               intersection =  calculate_intersection(v1,v2,v3,v4);
        }
                temp_pol.push_back(intersection);
            
            }else if(!is_insidev3 and is_insidev4){
               Vector<ld> intersection;  
               
               
                if(signaling.second!=-1)
                {       
                if(signaling.second == 0) 
                {
                     intersection =calculate_intersection_colinear(v1 ,v2 ,v3);
                }else if(signaling.second == 3)
                { 
                    intersection = v3;
                }else if(signaling.second == 4){
                    intersection = v4;
                }
                }else{
                    intersection =  calculate_intersection(v1,v2,v3,v4);
                }
                temp_pol.push_back(intersection);
                temp_pol.push_back(v4);
                    
               
            }
            
        }

        polc.clear();
        polc = temp_pol;
        polc.push_back(polc[0]);
    
    }

    vector<vector<ld>> final_pol;

    for(auto v:polc){
        vector<ld> ver{v.x_ , v.y_};
        final_pol.push_back(ver);
    }


    return area<ld>(final_pol);
    
    

}

TEST(PolygonIntersectionTest, TestCase1) {
    // Primer polígono: A, B, C, D, E, F, G
    vector<vector<double>> polygon1 = {
        {-5, 2},     // A
        {2, 2},      // B
        {4, 0},      // C
        {2, -2},     // D
        {-2.79, -1.99}, // E
        {-5, -2},    // F
        {-7, 0}      // G
    };
    
    // Segundo polígono: H, I, J (triángulo)
    vector<vector<double>> polygon2 = {
        {-3, 2},     // H
        {5, 2},      // I
        {0, -2}      // J
    };
    
    // Calcular área de intersección
    double intersection_area = area_of_intersection(polygon1, polygon2);
    
    // Calcular áreas individuales para verificación
    double area1 = area(polygon1);
    double area2 = area(polygon2);
    
    cout << "Área del polígono 1: " << area1 << endl;
    cout << "Área del polígono 2: " << area2 << endl;
    cout << "Área de intersección: " << intersection_area << endl;
    
    // Verificaciones básicas
    EXPECT_GE(intersection_area, 0.0);
    EXPECT_LE(intersection_area, min(area1, area2));
    
    // Verificar áreas individuales
    EXPECT_NEAR(area1, 35.97, 0.1);
    EXPECT_NEAR(area2, 15.98, 0.1);
    
    // Comparar con el valor esperado de intersección (13.98)
    EXPECT_NEAR(intersection_area, 13.98, 0.1);
}

TEST(PolygonIntersectionTest, TestCase2) {
    // Test case anterior para regresión
    vector<vector<double>> polygon1 = {
        {-12.89, -1.23}, {-11.61, 2.07}, {-6.93, 4.27}, {-1.39, 4.25}, {5.15, 3.09},
        {5.73, 0}, {5.49, -3.55}, {0.41, -6.01}, {-3.87, -5.77}, {-11.23, -5.39}
    };
    
    vector<vector<double>> polygon2 = {
        {-1.5, -1.48}, {-0.8, 1.87}, {1.78, 4.71}, {9.14, 6.29}, {12.45, 0.26},
        {11.05, -4.62}, {6.63, -6.63}, {-1.1, -7.4}, {-1.4, -3.78}
    };
    
    double intersection_area = area_of_intersection(polygon1, polygon2);
    cout << "Test case 2 - Área de intersección: " << intersection_area << endl;
    
    EXPECT_GE(intersection_area, 0.0);
    EXPECT_NEAR(intersection_area, 56.1, 1.0);
}

int main(int argc, char **argv) { 
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}