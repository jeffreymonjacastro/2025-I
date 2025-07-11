#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <random>
#include <functional>
#include <string>
using namespace std;

double f(double x, double y) {
    double dx = x - 200;
    double dy = y - 200;
    return (dx*dx)/(15*15) - (dy*dy)/(10*10) - 1;
}

class MS_adaptative
{
private:
    double x_min , y_min , x_max , y_max;
    int sign0 , sign1 , sign2 , sign3;
    char val;
    bool is_leaf;
    MS_adaptative* NE;
    MS_adaptative* NO;
    MS_adaptative* SE;
    MS_adaptative* SO;

public:
    MS_adaptative(char val, bool is_leaf, double xmin, double ymin, double xmax, double ymax, int s0, int s1, int s2, int s3) 
        : NE(nullptr), NO(nullptr), SO(nullptr), SE(nullptr),
          val(val), is_leaf(is_leaf), 
          x_min(xmin), y_min(ymin), x_max(xmax), y_max(ymax),  
          sign0(s0), sign1(s1), sign2(s2), sign3(s3) {
    }
    ~MS_adaptative();
    
   void draw_line(fstream & file, double init_x, double init_y, double end_x, double end_y) {
    file << "newpath" << endl;
    file << init_x << " " << init_y << " moveto" << endl;
    file << end_x << " " << end_y << " lineto" << endl;
    file << "stroke" << endl;
}

double find_intersection(double x1, double y1, double x2, double y2, function<double(double, double)> func, bool is_horizontal = true) {
    double f1 = func(x1, y1);
    double f2 = func(x2, y2);
    
    if (f1 * f2 > 0) {
        return is_horizontal ? (x1 + x2) / 2 : (y1 + y2) / 2;
    }
    
    for (int i = 0; i < 10; i++) {  
        double x_mid = (x1 + x2) / 2;
        double y_mid = (y1 + y2) / 2;
        double f_mid = func(x_mid, y_mid);
        
        if (abs(f_mid) < 1e-6) break;  
        
        if (f1 * f_mid < 0) {
            x2 = x_mid;
            y2 = y_mid;
            f2 = f_mid;
        } else {
            x1 = x_mid;
            y1 = y_mid;
            f1 = f_mid;
        }
    }
    
    return is_horizontal ? (x1 + x2) / 2 : (y1 + y2) / 2;
}

void draw(fstream & file, function<double(double, double)> func, double x_min, double y_min, double x_max, double y_max) {
    double x_mid = (x_min + x_max) / 2;
    double y_mid = (y_min + y_max) / 2;
    
    int bit_0 = int(func(x_min, y_min) > 0);
    int bit_1 = int(func(x_max, y_min) > 0);
    int bit_2 = int(func(x_max, y_max) > 0);
    int bit_3 = int(func(x_min, y_max) > 0);
    
    int to_case = 1 * bit_0 + 2 * bit_1 + 4 * bit_2 + 8 * bit_3;
    
    double left_intersect = find_intersection(x_min, y_min, x_min, y_max, func, false);   
    double right_intersect = find_intersection(x_max, y_min, x_max, y_max, func, false);   
    double bottom_intersect = find_intersection(x_min, y_min, x_max, y_min, func, true);   
    double top_intersect = find_intersection(x_min, y_max, x_max, y_max, func, true);      
    
    switch (to_case) {
        case 0:
            break;
        case 1:
            draw_line(file, x_min, left_intersect, bottom_intersect, y_min);
            break;
        case 2:
            draw_line(file, bottom_intersect, y_min, x_max, right_intersect);
            break;
        case 3:
            draw_line(file, x_min, left_intersect, x_max, right_intersect);
            break;
        case 4:
            draw_line(file, top_intersect, y_max, x_max, right_intersect);
            break;
        case 5:
            draw_line(file, x_min, left_intersect, top_intersect, y_max);
            draw_line(file, x_max, right_intersect, bottom_intersect, y_min);
            break;
        case 6:
            draw_line(file, bottom_intersect, y_min, top_intersect, y_max);
            break;
        case 7:
            draw_line(file, x_min, left_intersect, top_intersect, y_max);
            break;
        case 8:
            draw_line(file, top_intersect, y_max, x_min, left_intersect);
            break;
        case 9:
            draw_line(file, top_intersect, y_max, bottom_intersect, y_min);
            break;
        case 10:
            draw_line(file, top_intersect, y_max, x_max, right_intersect);
            draw_line(file, bottom_intersect, y_min, x_min, left_intersect);
            break;
        case 11:
            draw_line(file, top_intersect, y_max, x_max, right_intersect);
            break;
        case 12:
            draw_line(file, x_max, right_intersect, x_min, left_intersect);
            break;
        case 13:
            draw_line(file, x_max, right_intersect, bottom_intersect, y_min);
            break;
        case 14:
            draw_line(file, bottom_intersect, y_min, x_min, left_intersect);
            break;
        case 15:
            break;
        default:
            break;
    }
}

   bool should_split(double x_mn , double y_mn , double x_m , double y_m , function<double(double, double)> func) {
   int num_samples = 70000;
   
   random_device rd;
   mt19937 gen(rd());
   uniform_real_distribution<double> dis_x(x_mn, x_m);
   uniform_real_distribution<double> dis_y(y_mn, y_m);
   
   int pos = 0;
   int neg = 0;
   vector<pair<double , double>> weird_cases;
   for (int i = 0; i < num_samples; i++) {
       double rand_x = dis_x(gen);
       double rand_y = dis_y(gen);
       double value = func(rand_x, rand_y); 
       
       if(value > 0) pos++;
       else if(value < 0 ) {
        neg++;
        weird_cases.push_back(make_pair(rand_x , rand_y));
       }
   }
   
   if(pos==num_samples or neg == num_samples) return false;
   else return true;
}

   MS_adaptative* split_grid(fstream & file , double x_min , double y_min , double x_max , double y_max , double presicion , bool draw_div, function<double(double, double)> func){
        double h_ = y_max - y_min;
        double w_ = x_max - x_min;
         double x_mid = (x_min+x_max)/2;
        double y_mid = (y_min+y_max)/2;
        int bit_0 = int(func(x_min, y_min)>0);
        int bit_1 = int(func(x_max, y_min)>0);
        int bit_2 = int(func(x_max, y_max)>0);
        int bit_3 = int(func(x_min , y_max)>0);

        bool must_stop = h_<= presicion and w_ <=presicion;

        bool split =  should_split(x_min , y_min , x_max , y_max , func);
        
        char v = (split)?'g':'w';  
        if(must_stop)
        {
            if(v=='g')
                draw(file, func ,  x_min , y_min , x_max , y_max);
            return new MS_adaptative(v , true , x_min , y_min , x_max , y_max , bit_0 , bit_1  , bit_2 , bit_3);

        }else{

                MS_adaptative* No = nullptr;
                MS_adaptative* Ne = nullptr;
                MS_adaptative* So = nullptr;
                MS_adaptative* Se = nullptr;
                MS_adaptative* new_node = new MS_adaptative(v, false , x_min , y_min , x_max , y_max ,bit_0 , bit_1  , bit_2 , bit_3);
              if(split){
               
                if(draw_div)
                    {
                        draw_line(file , x_mid , y_max , x_mid , y_min);
                        draw_line(file , x_min , y_mid , x_max , y_mid);
                    }
                No = split_grid(file , x_min , y_mid , x_mid , y_max , presicion ,draw_div, func); 
                Ne = split_grid(file , x_mid , y_mid , x_max , y_max , presicion ,draw_div, func); 
                So = split_grid(file , x_min , y_min , x_mid , y_mid , presicion ,draw_div, func); 
                Se = split_grid(file , x_mid , y_min , x_max , y_mid , presicion ,draw_div, func);    
                
              }

              new_node->NO = No;
              new_node->NE = Ne;
              new_node->SO = So;
              new_node->SE = Se;
              
              return new_node;
        }
   } 
};

class EPS
{
private:
    double h_, w_;
    double x_min_, y_min_, x_max_, y_max_;
    double precision_;
    string filename_;
    MS_adaptative* root;
    
public:
    EPS(double h, double w, string filename, double xmin, double ymin, double xmax, double ymax, double precision) 
        : h_(h), w_(w), filename_(filename), x_min_(xmin), y_min_(ymin), x_max_(xmax), y_max_(ymax), precision_(precision) {}

    void header(fstream & file) {
        file << "%!PS-Adobe-3.0 EPSF-3.0" << endl;
        file << "%%BoundingBox: 0 0 " << (int)w_ << " " << (int)h_ << endl;
        file << "%%EndComments" << endl;
        file << endl;
        file << "0.5 setlinewidth" << endl;
        file << "0 0 0 setrgbcolor" << endl;
        file << endl;
    }

    void generate_function(function<double(double, double)> func) {
        fstream file(filename_, ios::out);
        header(file);   
     
        root = new MS_adaptative('w' , true , x_min_, y_min_ , x_max_ , y_max_ , 0 , 0  , 0 , 0);
        root = root->split_grid(file , x_min_, y_min_ , x_max_ , y_max_ , precision_ ,false, func);  
        file << "%%EOF" << endl;
        file.close();
        cout<<"Done..."<<endl;
    }

    ~EPS() {}
};

void draw_curve(function<double(double, double)> f, 
                string output_filename, 
                double xmin, double ymin, double xmax, double ymax, 
                double precision) {
    
    double width = xmax - xmin;
    double height = ymax - ymin;
    
    EPS eps(height, width, output_filename, xmin, ymin, xmax, ymax, precision);
    
    eps.generate_function(f);
}

double circle_function(double x, double y) {
    double dx = x - 200;
    double dy = y - 200;
    return dx*dx + dy*dy - 45*45;
}

int main() {

    //Nota se usa un sampling largo para cerrar las curvas , por que si no las divisiones correctas no se hacen toma un poco pero termina de correr
    cout << "Generando curva con función original adaptative..." << endl;
    draw_curve(f, "hyperbola_adaptative.eps", 0, 0, 400, 400, 2);
    
    cout << "Generando círculo adaptative..." << endl;
    draw_curve(circle_function, "circle_adaptative.eps", 0, 0, 400, 400, 2);
    
    return 0;
}