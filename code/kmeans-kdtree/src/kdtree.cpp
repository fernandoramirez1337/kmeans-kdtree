#include "../include/kdtree.hpp"

#include <algorithm>
#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>

Node::Node(point coord) : coord(coord), left(nullptr), right(nullptr) {}

KDTree::KDTree() : root(nullptr) {}

Node* KDTree::buildTree(std::vector<point>& points, int depth) {
    if (points.empty()) return nullptr;

    int axis = depth % 2;

    if (axis == 0) {
        std::sort(points.begin(), points.end(), [](point a, point b) { return a.x < b.x; });
    } else {
        std::sort(points.begin(), points.end(), [](point a, point b) { return a.y < b.y; });
    }

    int medianIndex = points.size() / 2;
    point medianPoint = points[medianIndex];

    Node* node = new Node(medianPoint);

    std::vector<point> leftPoints(points.begin(), points.begin() + medianIndex);
    std::vector<point> rightPoints(points.begin() + medianIndex + 1, points.end());

    node->left = buildTree(leftPoints, depth + 1);
    node->right = buildTree(rightPoints, depth + 1);

    return node;
}

void KDTree::build(std::vector<point>& points) {
    root = buildTree(points);
}

void KDTree::printTree(Node* node) {
    if (node == nullptr) return;
    printTree(node->left);
    std::cout << node->coord.x << " " << node->coord.y << std::endl;
    printTree(node->right);
}

void KDTree::print() {
    printTree(root);
}

double KDTree::distanceSquared(point a, point b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return dx * dx + dy * dy;
}

Node* KDTree::findNearest(Node* node, point target, int depth, Node* best, double& bestDist) {
    if (node == nullptr) return best;

    float d = distanceSquared(target, node->coord);
    if (d < bestDist) {
        bestDist = d;
        best = node;
    }

    int axis = depth % 2;
    float diff = (axis == 0) ? target.x - node->coord.x : target.y - node->coord.y;
    Node* first = (diff < 0) ? node->left : node->right;
    Node* second = (diff < 0) ? node->right : node->left;

    best = findNearest(first, target, depth + 1, best, bestDist);
    if (diff * diff < bestDist) {
        best = findNearest(second, target, depth + 1, best, bestDist);
    }

    return best;
}

point KDTree::nearest(point target) {
    double bestDist = std::numeric_limits<double>::max();
    Node* best = findNearest(root, target, 0, nullptr, bestDist);
    return best->coord;
}

void readData(std::vector<point>& data,std::string filename){
    std::ifstream file(filename);
    std::string line;
    //Skip first line
    getline(file, line);
    while(getline(file, line)){
        std::stringstream ss(line);
        std::string x, y;
        getline(ss, x, ',');
        getline(ss, y, ',');
        data.push_back(point(stof(x), stof(y)));
    }
}