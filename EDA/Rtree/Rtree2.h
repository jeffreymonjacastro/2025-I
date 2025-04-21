#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <random>
const float EPSILON = 1e-6f;
typedef unsigned int  uint;
typedef unsigned char uchar;

class Point {
public:
    float x, y;
    Point() : x(0.0f), y(0.0f) {}
    Point(float x, float y) : x(x), y(y) {}

    // Distancia euclidiana
    float distanceTo(const Point &other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};



class RNode;

class MBB {
public:
    Point lower; // Esquina inferior izquierda
    Point upper; // Esquina superior derecha

    MBB() : lower(Point()), upper(Point()) {}
    MBB(const Point &p1, const Point &p2)
        : lower(Point(std::min(p1.x, p2.x), std::min(p1.y, p2.y))),
          upper(Point(std::max(p1.x, p2.x), std::max(p1.y, p2.y))) {}

    float area() const;
    float semiPerimeter() const;

    float distanceTo(const Point &p) const;

    // Incremento del semiperimetro si agrega p
    float deltaSemiPerimeter(const Point &p) const;

    // Expande para incluir point o MBB
    void expandToInclude(const Point &p);
    void expandToInclude(const MBB &other);

    // Retorna el area de interseccion
    float intersects(const MBB &other) const;


    // Crear MBB a partir de un vector de puntos
    static MBB computeFromPoints(const std::vector<Point> &pts);

    // Crear MBB a partir de un vector de nodos
    static MBB computeFromNodes(const std::vector<RNode*> &nodes);

    // Union MBBs
    static MBB unionOf(const MBB &a, const MBB &b);
};

float MBB::area() const {
    return (upper.y - lower.y)*(upper.x-lower.x);
}

float MBB::semiPerimeter() const {
    return (upper.y - lower.y)+(upper.x-lower.x);
}

float MBB::distanceTo(const Point &p) const {
     float x_axis = std::max(lower.x,std::min(p.x , upper.x));
     float y_axis = std::max(lower.y,std::min(p.y , upper.y));

     Point closest_one(x_axis,y_axis);

    return  p.distanceTo(closest_one);
}

float MBB::deltaSemiPerimeter(const Point &p) const {
    float new_upper_x = std::max(p.x , upper.x);
    float new_upper_y =  std::max(p.y , upper.y);
    float new_lower_x =  std::min(p.x , lower.x);
    float new_lower_y =  std::min(p.y , lower.y);

    float new_semi_per = (new_upper_y-new_lower_y)+(new_upper_x-new_lower_x);
    float current_per = semiPerimeter();

    return  new_semi_per - current_per;

}

void MBB::expandToInclude(const Point &p) {
    float new_upper_x = std::max(p.x , upper.x);
    float new_upper_y =  std::max(p.y , upper.y);
    float new_lower_x =  std::min(p.x , lower.x);
    float new_lower_y =  std::min(p.y , lower.y);

    upper.x = new_upper_x;
    upper.y = new_upper_y;
    lower.x = new_lower_x;
    lower.y = new_lower_y;
}


void MBB::expandToInclude(const MBB &other) {
    // Corrección: Faltaba actualizar upper.x y upper.y
    lower.x = std::min(lower.x, other.lower.x);
    lower.y = std::min(lower.y, other.lower.y);
    upper.x = std::max(upper.x, other.upper.x);
    upper.y = std::max(upper.y, other.upper.y);
}

float MBB::intersects(const MBB &other) const {


    float lx = std::max(float(0.0), std::min(upper.x , other.upper.x) - std::max(lower.x,other.lower.x));
    float ly = std::max(float(0.0), std::min(upper.y , other.upper.y) - std::max(lower.y,other.lower.y));

    return  lx*ly;
}

MBB MBB::computeFromPoints(const std::vector<Point> &pts) {
    // Proteger contra vectores vacíos
    if (pts.empty()) {
        return MBB();
    }

    float lx = pts[0].x;
    float ly = pts[0].y;
    float ux = pts[0].x;
    float uy = pts[0].y;

    for (auto p : pts) {
        if (p.x < lx) lx = p.x;
        if (p.x > ux) ux = p.x;
        if (p.y < ly) ly = p.y;
        if (p.y > uy) uy = p.y;
    }

    Point new_lower(lx, ly);
    Point new_upper(ux, uy);
    MBB new_MBB(new_lower, new_upper);

    return new_MBB;
}


// -------------------------------
// Clase RNode
// -------------------------------
class RNode {
private:
    // Linear Split para nodos hojas
    RNode* linearSplitLeaf(uchar maxEntries);

    // Quadratic Split para nodos internos
    RNode* quadraticSplitInternal(uchar maxEntries);

    int choose_subtree(Point p);



public:
    bool isLeaf;

    MBB mbr;
    std::vector<Point>    points;
    std::vector<RNode*> children;

    RNode(bool leaf) : isLeaf(leaf) {}

    RNode* insert(const Point &p, uchar maxEntries);

    void search_(const MBB &query , std::vector<Point>& ans) const;
    std::vector<Point> search(const MBB &query) const;

};

RNode *RNode::linearSplitLeaf(uchar maxEntries) {
    // select any at random , calculate the maximun dsitance to that element , from that recalculate the other maximun distance
    // have p1 and p2 be the largest
    // select any other MBB or points and add to the group with less area aumentation

    size_t min_fill_factor = (maxEntries + 1) / 2;
    //heuristic to calculate the two more separeted points


    int rand_index = rand()%points.size();

    Point p1 = points[rand_index];

    Point p2 = points[0];

    float dis = p1.distanceTo(p2);

    for (int i = 1; i < points.size(); ++i) {
        float current_dis=p1.distanceTo(points[i]);
        if( current_dis>dis)
        {
            dis=current_dis;
            p2 = points[i];
        }

    }

    p1 = points[0];


    dis = p2.distanceTo(p1);

    for (int i = 1; i < points.size(); ++i) {
        float current_dis=p2.distanceTo(points[i]);
        if( current_dis>dis)
        {
            dis=current_dis;
            p1 = points[i];
        }

    }

    MBB for_this(p1,p1) ;
    MBB new_child(p2,p2);

    RNode* splited_child_A = new RNode(true);
    splited_child_A->mbr = for_this;

    RNode* splited_child_B = new RNode(true);
    splited_child_B->mbr = new_child;


    for (auto p:points) {

        MBB A = splited_child_A->mbr;
        MBB B = splited_child_B->mbr;


        float  area_before_a = A.area();
        float  area_before_b = B.area();

        A.expandToInclude(p);
        B.expandToInclude(p);

        float diff_a = A.area() - area_before_a ;
        float diff_b = B.area() - area_before_b;


        if(diff_a < diff_b){


            splited_child_A->mbr = A;
            splited_child_A->points.push_back(p);


        }else if(diff_a > diff_b){


            splited_child_B->mbr = B;
            splited_child_B->points.push_back(p);


        }else{ // both are equal

            if(A.area()<B.area()){ // Area check

                splited_child_A->mbr = A;
                splited_child_A->points.push_back(p);


            }else if(A.area()>B.area()){


                splited_child_B->mbr = B;
                splited_child_B->points.push_back(p);


            }else{//number of points already there test
                //assign to the node with the less number of current points
                if(splited_child_A->points.size()<splited_child_B->points.size()){

                    splited_child_A->mbr =A;
                    splited_child_A->points.push_back(p);


                }else{


                    splited_child_B->mbr = B;
                    splited_child_B->points.push_back(p);
                }
            }

        }// end both are equal

    }



    auto balanceFillFactor = [min_fill_factor](RNode* a, RNode* b) { // CAMBIO Y BALANCEO PUNTOS LOS MBR SE CORRIGEN LUEGO
        while (a->points.size() < min_fill_factor and !b->points.empty()) {
            Point last = b->points.back();
            a->points.push_back(last);
            b->points.pop_back();
        }
    };



    balanceFillFactor(splited_child_A , splited_child_B);
    balanceFillFactor(splited_child_B,splited_child_A);

    splited_child_A->mbr = MBB::computeFromPoints(splited_child_A->points);
    splited_child_B->mbr = MBB::computeFromPoints(splited_child_B->points);

    this->mbr = splited_child_A->mbr;
    this->points = splited_child_A->points;
    this->isLeaf=splited_child_A->isLeaf;



    //delete splited_child_A;

    return splited_child_B;

}

RNode *RNode::quadraticSplitInternal(uchar maxEntries) {
    // calcular ambos con death region mas larga
    // cada region se mete al que tiene menor aumento de mbr muerto

    size_t min_fill_factor = (maxEntries+1)/2;

    int index_a , index_b;
    index_a = 0;
    index_b = 1;
    MBB first_union_mbb(children[index_a]->mbr.lower, children[index_a]->mbr.upper);
    first_union_mbb.expandToInclude(children[index_b]->mbr);
    float death_area = first_union_mbb.area() -  (children[index_a]->mbr.area() + children[index_b]->mbr.area());
    for (int i = 0; i < children.size(); ++i) {

        for (int j = i+1; j < children.size() ; ++j) {

            MBB union_mbb;
            union_mbb.upper = children[i]->mbr.upper;
            union_mbb.lower = children[i]->mbr.lower;
            union_mbb.expandToInclude(children[j]->mbr);

            if(death_area>union_mbb.area() -  (children[i]->mbr.area()+ children[j]->mbr.area()-children[i]->mbr.intersects(children[j]->mbr))){

                index_a =i;
                index_b =j;
                death_area =union_mbb.area() -  (children[i]->mbr.area()+ children[j]->mbr.area()-children[i]->mbr.intersects(children[j]->mbr));

            }

        }
    }


    MBB A = children[index_a]->mbr;
    MBB B = children[index_b]->mbr;

    RNode* alfa = new RNode(false);
    RNode* beta = new RNode(false);

    alfa->mbr =A;
    alfa->children.push_back(children[index_a]);
    beta->mbr = B;
    beta->children.push_back(children[index_b]);


    for (int i = 0; i < children.size(); ++i) {

        if(i==index_a or i==index_b) continue;

        MBB union_MBB_A = alfa->mbr;
        MBB union_MBB_B = beta->mbr;


        union_MBB_A.expandToInclude(children[i]->mbr);
        union_MBB_B.expandToInclude(children[i]->mbr);

        float diff_a = union_MBB_A.area() - alfa->mbr.area();
        float diff_b = union_MBB_B.area() - beta->mbr.area();

        if(diff_a < diff_b){
            alfa->children.push_back(children[i]);
            alfa->mbr = union_MBB_A;
        }else if(diff_a > diff_b){
            beta->children.push_back(children[i]);
            beta->mbr = union_MBB_B;
        }else{

            if(alfa->mbr.area() < beta->mbr.area()){

                alfa->children.push_back(children[i]);
                alfa->mbr = union_MBB_A;
            }else if(alfa->mbr.area() > beta->mbr.area()){
                beta->children.push_back(children[i]);
                beta->mbr = union_MBB_B;
            }else{
                if(alfa->children.size() < beta->children.size()){
                    alfa->children.push_back(children[i]);
                    alfa->mbr = union_MBB_A;
                }else{
                    beta->children.push_back(children[i]);
                    beta->mbr = union_MBB_B;
                }

            }

        }


    }


    auto balanceFillFactor =  [min_fill_factor](RNode* a , RNode* b){

        while (a->children.size() < min_fill_factor and !b->children.empty()) {
            RNode* last = b->children.back();
            a->children.push_back(last);
            b->children.pop_back();
        }
    };


    balanceFillFactor(alfa,beta);
    balanceFillFactor(beta,alfa);

    alfa->mbr=MBB::computeFromNodes(alfa->children);
    beta->mbr=MBB::computeFromNodes(beta->children);

    this->mbr = alfa->mbr;
    this->children = alfa->children;
    this->isLeaf = alfa->isLeaf;


    return beta;





}





RNode *RNode::insert(const Point &p, uchar maxEntries) {
    RNode* result;
    if(children[0]->isLeaf){

        int left_choosen = choose_subtree(p);

        children[left_choosen]->points.push_back(p);


        if(children[left_choosen]->points.size()>maxEntries){  // si la hoja donde se inserto explota se splitea
            RNode* splited_one = children[left_choosen]->linearSplitLeaf(maxEntries);
            children.push_back(splited_one);
           this->mbr= MBB::computeFromNodes(children);//recalcular el mbr a partid de los nuevos mbb

            return this; //retornar este nodo a su padre
        }


        children[left_choosen]->mbr.expandToInclude(p);//si no hubo overflow del nodo hoja quiza si se recambio el mbr al insertar el punto

        this->mbr= MBB::computeFromNodes(children); // simplemetne recalcula por sia caso el mbr
        return this;


    }else{

        int next_node = choose_subtree(p);
       result = children[next_node]->insert(p,maxEntries);
    }

    //vuelta de la recursion el hijo anterior fue devuelto

    if(result->children.size() > maxEntries){ //existe overflow del hijo lo spliteo
        RNode* my_splited_child = result->quadraticSplitInternal(maxEntries);
        children.push_back(my_splited_child);
        this->mbr= MBB::computeFromNodes(children);
        return this;
    }

    this->mbr= MBB::computeFromNodes(children);
    return this;



}

int RNode::choose_subtree(Point p) {
    // Check if children vector is empty
    if (children.empty()) {
        return -1; // Return invalid index to indicate error
    }

    float perimeter = children[0]->mbr.deltaSemiPerimeter(p);
    int index = 0; // Initialize with first child instead of -1

    std::vector<int> draws;

    for (int i = 1; i < children.size(); ++i) {
        if (perimeter > children[i]->mbr.deltaSemiPerimeter(p)) {
            index = i;
            perimeter = children[i]->mbr.deltaSemiPerimeter(p);
        }
    }

    for (int i = 0; i < children.size(); ++i) {
        if (perimeter == children[i]->mbr.deltaSemiPerimeter(p)) {
            draws.push_back(i);
        }
    }

    if (!draws.empty()) {
        index = draws[0]; // Initialize with first draw
        float area = children[draws[0]]->mbr.area();

        for (int i = 1; i < draws.size(); ++i) {
            if (area > children[draws[i]]->mbr.area()) {
                index = draws[i];
                area = children[draws[i]]->mbr.area();
            }
        }
    }

    return index;
}
void RNode::search_(const MBB &query, std::vector<Point> &ans) const {
    if (isLeaf) {
        for (const Point &p : points) {

            if (p.x >= query.lower.x - EPSILON and p.x <= query.upper.x + EPSILON &&
                p.y >= query.lower.y - EPSILON and p.y <= query.upper.y + EPSILON) {
                ans.push_back(p);
            }
        }
    } else {
        for (auto* next_node : children) {


            if (query.intersects(next_node->mbr) > -EPSILON) {
                next_node->search_(query, ans);
            }
        }
    }
}
std::vector<Point> RNode::search(const MBB &query) const {
    std::vector<Point> ans;
    search_(query,ans);

    return  ans;
}



MBB MBB::computeFromNodes(const std::vector<RNode*> &nodes) {

    if (nodes.empty()) {
        return MBB();
    }


    MBB result = nodes[0]->mbr;


    for (size_t i = 1; i < nodes.size(); i++) {
        result.expandToInclude(nodes[i]->mbr);
    }

    return result;
}

MBB MBB::unionOf(const MBB &a, const MBB &b) {
  MBB new_MBB = a;

  new_MBB.expandToInclude(b);

    return  new_MBB;

}


// -------------------------------
// Para Best-First
// -------------------------------
struct  QueueEntry {
    float distance;  // Distancia desde el query al MBB
    bool isNode;     // Si true, es un nodo; si false, es un punto
    RNode* node;
    Point pt;
};

struct QueueEntryComparator {
    bool operator()(const QueueEntry &a, const QueueEntry &b) const {
        return a.distance > b.distance;
    }
};

// -------------------------------
// Clase RTree
// -------------------------------
class RTree {
public:
    RNode* root;
    uchar maxEntries;  // Capacidad maxima

    RTree(uchar maxEntries = 3) : maxEntries(maxEntries) {
        std::srand(std::time(nullptr)); // Semilla inicializada aquí
        root = new RNode(true);
    }

    void insert(const Point &p);
    std::pair<RNode*,RNode*> split_root_leaf(RNode* root);
    std::pair<RNode*,RNode*> split_root(RNode* root);
    std::vector<Point> search(const MBB &query) const;
    std::vector<Point> kNN(const Point &query, uchar k) const;
};

void RTree::insert(const Point &p) {

    if(root== nullptr){//create new node if nothing is there
        root = new RNode(true);
        root->points.push_back(p);
        root->mbr.expandToInclude(p);
        return;
    }else{
        if(root->isLeaf){ // fill until split
            root->points.push_back(p);

            if(root->points.size()>maxEntries){ //first split of root
                std::pair<RNode*,RNode*> new_leaf_nodes = split_root_leaf(root);

                root = new RNode(false);
                root->children.push_back(new_leaf_nodes.first);
                root->children.push_back(new_leaf_nodes.second);
                root->mbr = MBB::computeFromNodes(root->children);
                return;

            }
            root->mbr.expandToInclude(p);
            return;
        }

        root = root->insert(p,maxEntries); //normal insert when root is a normal one

        if(root->children.size()>maxEntries){
            std::pair<RNode*,RNode*> new_children_nodes = split_root(root);
            root = new RNode(false);
            root->children.push_back(new_children_nodes.first);
            root->children.push_back(new_children_nodes.second);
            root->mbr = MBB::computeFromNodes(root->children);
            return;
        }





    }






}

std::pair<RNode *, RNode *> RTree::split_root_leaf(RNode *root) {
    // select any at random , calculate the maximun dsitance to that element , from that recalculate the other maximun distance
    // have p1 and p2 be the largest
    // select any other MBB or points and add to the group with less area aumentation

    size_t min_fill_factor = (maxEntries + 1) / 2;
    //heuristic to calculate the two more separeted points


    int rand_index = rand()%root->points.size();

    Point p1 = root->points[rand_index];

    Point p2 = root->points[0];

    float dis = p1.distanceTo(p2);

    for (int i = 1; i < root->points.size(); ++i) {
        float current_dis=p1.distanceTo(root->points[i]);
        if( current_dis>dis)
        {
            dis=current_dis;
            p2 = root->points[i];
        }

    }

    p1 = root->points[0];


    dis = p2.distanceTo(p1);

    for (int i = 1; i < root->points.size(); ++i) {
        float current_dis=p2.distanceTo(root->points[i]);
        if( current_dis>dis)
        {
            dis=current_dis;
            p1 = root->points[i];
        }

    }

    MBB for_this(p1,p1) ;
    MBB new_child(p2,p2);

    RNode* splited_child_A = new RNode(true);
    splited_child_A->mbr = for_this;

    RNode* splited_child_B = new RNode(true);
    splited_child_B->mbr = new_child;


    for (auto p:root->points) {

        MBB A = splited_child_A->mbr;
        MBB B = splited_child_B->mbr;


        float  area_before_a = A.area();
        float  area_before_b = B.area();

        A.expandToInclude(p);
        B.expandToInclude(p);

        float diff_a = A.area() - area_before_a ;
        float diff_b = B.area() - area_before_b;


        if(diff_a < diff_b){


            splited_child_A->mbr = A;
            splited_child_A->points.push_back(p);


        }else if(diff_a > diff_b){


            splited_child_B->mbr = B;
            splited_child_B->points.push_back(p);


        }else{ // both are equal

            if(A.area()<B.area()){ // Area check

                splited_child_A->mbr = A;
                splited_child_A->points.push_back(p);


            }else if(A.area()>B.area()){


                splited_child_B->mbr = B;
                splited_child_B->points.push_back(p);


            }else{//number of points already there test
                //assign to the node with the less number of current points
                if(splited_child_A->points.size()<splited_child_B->points.size()){

                    splited_child_A->mbr =A;
                    splited_child_A->points.push_back(p);


                }else{


                    splited_child_B->mbr = B;
                    splited_child_B->points.push_back(p);
                }
            }

        }// end both are equal

    }



    auto balanceFillFactor = [min_fill_factor](RNode* a, RNode* b) { // CAMBIO Y BALANCEO PUNTOS LOS MBR SE CORRIGEN LUEGO
        while (a->points.size() < min_fill_factor and !b->points.empty()) {
            Point last = b->points.back();
            a->points.push_back(last);
            b->points.pop_back();
        }
    };



    balanceFillFactor(splited_child_A , splited_child_B);
    balanceFillFactor(splited_child_B,splited_child_A);

    splited_child_A->mbr = MBB::computeFromPoints(splited_child_A->points);
    splited_child_B->mbr = MBB::computeFromPoints(splited_child_B->points);





    //delete splited_child_A;

    return std::make_pair(splited_child_A,splited_child_B);
}

std::pair<RNode *, RNode *> RTree::split_root(RNode *root) {
    // calcular ambos con death region mas larga
    // cada region se mete al que tiene menor aumento de mbr muerto

    size_t min_fill_factor = (maxEntries+1)/2;

    int index_a , index_b;
    index_a = 0;
    index_b = 1;
    MBB first_union_mbb(root->children[index_a]->mbr.lower,root->children[index_a]->mbr.upper);
    first_union_mbb.expandToInclude(root->children[index_b]->mbr);
    float death_area = first_union_mbb.area() -  (root->children[index_a]->mbr.area()+ root->children[index_b]->mbr.area()-root->children[index_a]->mbr.intersects(root->children[index_b]->mbr));
    for (int i = 0; i < root->children.size(); ++i) {

        for (int j = i+1; j < root->children.size() ; ++j) {

            MBB union_mbb;
            union_mbb.upper = root->children[i]->mbr.upper;
            union_mbb.lower = root->children[i]->mbr.lower;
            union_mbb.expandToInclude(root->children[j]->mbr);

            if(death_area>union_mbb.area() -  (root->children[i]->mbr.area()+ root->children[j]->mbr.area()-root->children[i]->mbr.intersects(root->children[j]->mbr))){

                index_a =i;
                index_b =j;
                death_area =union_mbb.area() -  (root->children[i]->mbr.area()+ root->children[j]->mbr.area()-root->children[i]->mbr.intersects(root->children[j]->mbr));

            }

        }
    }


    MBB A = root->children[index_a]->mbr;
    MBB B = root->children[index_b]->mbr;

    RNode* alfa = new RNode(false);
    RNode* beta = new RNode(false);

    alfa->mbr = A;
    alfa->children.push_back(root->children[index_a]);
    beta->mbr = B;
    beta->children.push_back(root->children[index_b]);


    for (int i = 0; i < root->children.size(); ++i) {

        if(i==index_a or i==index_b) continue;

        MBB union_MBB_A = alfa->mbr;
        MBB union_MBB_B = beta->mbr;


        union_MBB_A.expandToInclude(root->children[i]->mbr);
        union_MBB_B.expandToInclude(root->children[i]->mbr);

        float diff_a = union_MBB_A.area() - alfa->mbr.area();
        float diff_b = union_MBB_B.area() - beta->mbr.area();

        if(diff_a < diff_b){
            alfa->children.push_back(root->children[i]);
            alfa->mbr = union_MBB_A;
        }else if(diff_a > diff_b){
            beta->children.push_back(root->children[i]);
            beta->mbr = union_MBB_B;
        }else{

            if(alfa->mbr.area() < beta->mbr.area()){

                alfa->children.push_back(root->children[i]);
                alfa->mbr = union_MBB_A;
            }else if(alfa->mbr.area() > beta->mbr.area()){
                beta->children.push_back(root->children[i]);
                beta->mbr = union_MBB_B;
            }else{
                if(alfa->children.size() < beta->children.size()){
                    alfa->children.push_back(root->children[i]);
                    alfa->mbr = union_MBB_A;
                }else{
                    beta->children.push_back(root->children[i]);
                    beta->mbr = union_MBB_B;
                }

            }

        }


    }


    auto balanceFillFactor =  [min_fill_factor](RNode* a , RNode* b){

        while (a->children.size() < min_fill_factor and !b->children.empty()) {
            RNode* last = b->children.back();
            a->children.push_back(last);
            b->children.pop_back();
        }
    };


    balanceFillFactor(alfa,beta);
    balanceFillFactor(beta,alfa);

    MBB::computeFromNodes(alfa->children);
    MBB::computeFromNodes(beta->children);

    return std::make_pair(alfa,beta);
}

std::vector<Point> RTree::search(const MBB &query) const {
    return root->search(query);
}

struct Point_Dist{

    Point p_;
    float d_;

    Point_Dist(const Point&p , float  d):p_(p),d_(d){

    }

    bool operator<(const Point_Dist & other ) const{
        return d_ < other.d_;
    }
};

std::vector<Point> RTree::kNN(const Point &query, uchar k) const {
    std::priority_queue<QueueEntry , std::vector<QueueEntry>, QueueEntryComparator> pq;


    std::priority_queue<Point_Dist> ans;


    QueueEntry explorer;
    explorer.pt = query;
    explorer.distance = root->mbr.distanceTo(query);
    explorer.isNode = true;
    explorer.node = root;

    pq.push(explorer);


    while (!pq.empty()){

        QueueEntry current = pq.top();
        pq.pop();

        if(ans.size()>=k and current.distance > ans.top().d_){
            break;
        }

        if(current.isNode){

            if(current.node->isLeaf){
               for(auto  ptr : current.node->points){
                   float dist = query.distanceTo(ptr);
                   pq.push({dist, false, nullptr, ptr});
               }

            }else{

                for (RNode* child : current.node->children) {
                    pq.push({child->mbr.distanceTo(query), true, child, Point()});
                }
            }


        }else{

            float dist = query.distanceTo(current.pt);


            if (ans.size() < k) {
                ans.push(Point_Dist(current.pt, dist));
            }
            else if (dist < ans.top().d_) {
                ans.pop();  // Eliminar el más lejano
                ans.push(Point_Dist(current.pt, dist));
            }
        }




    }


    std::vector<Point> answer;



    while (!ans.empty()){
        Point current = ans.top().p_;
        answer.push_back(current);
        ans.pop();
    }

    std::reverse(answer.begin(), answer.end());

    return answer;


}
