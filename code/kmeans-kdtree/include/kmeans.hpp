#include "kdtree.hpp"

#include <vector>
#include <string>

#ifndef POINT_SIZE
    #define POINT_SIZE 2
#endif
#ifndef MAX_ITERATIONS
    #define MAX_ITERATIONS 100
#endif

class Point {
private:
	int id_point, id_cluster;
	std::vector<double> data;

public:
	Point(int, std::vector<double>&);

	double get_data(int);
	int get_cluster();
	int get_ID();

    void set_cluster(int);
};

class Cluster {
private:
	std::vector<double> center;
	std::vector<Point> points;
	int id_cluster;

public:
	Cluster(int, Point);

	int get_cluster_size();
	double get_center(int);
	Point get_point(int);
	int get_ID();

	void set_center(int, double);

	bool remove_point(int);
	void add_point(Point);
};

class KMeans {
private:
	std::vector<Cluster> clusters;
	int k, n;

	int get_nearest_center(Point);
	KDTree stuart_little;

public:
    KMeans(std::string dataset_file_name, int k, int n);
};
