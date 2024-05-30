#include <vector>

class KMeans;

struct point {
    double x, y;
    point(double x, double y) : x(x), y(y) {}
};

struct Node {
    point coord;
    Node* left;
    Node* right;
    Node(point coord);
};

class KDTree {
private:
    Node* root;
    Node* buildTree(std::vector<point>& points, int depth = 0);
    void printTree(Node* node);
    Node* findNearest(Node* node, point target, int depth, Node* best, double& bestDist);
    double distanceSquared(point a, point b);
public:
    KDTree();
    ~KDTree();

    void build(std::vector<point>& points);
    void print();
    point nearest(point target);
    void clear(Node* node);

    friend class KMeans;

};

