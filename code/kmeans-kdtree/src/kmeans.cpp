#include "../include/kmeans.hpp"
/* <vector> <string> */
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <sstream>


Point::Point(int id_point, std::vector<double>& data) {
    this->id_point = id_point;

    for(int i = 0; i < POINT_SIZE; i++)
        this->data.push_back(data[i]);

    id_cluster = -1;
}

int Point::get_ID() {
    return id_point;
}

void Point::set_cluster(int id_cluster) {
    this->id_cluster = id_cluster;
}

int Point::get_cluster() {
    return id_cluster;
}

double Point::get_data(int index) {
    return data[index];
}

Cluster::Cluster(int id_cluster, Point point) {
    this->id_cluster = id_cluster;

    for(int i = 0; i < POINT_SIZE; i++)
        center.push_back(point.get_data(i));

    points.push_back(point);
}

void Cluster::add_point(Point point) {
    points.push_back(point);
}

bool Cluster::remove_point(int id_point) {
    int total_points = points.size();

    for(int i = 0; i < total_points; i++)
    {
        if(points[i].get_ID() == id_point)
        {
            points.erase(points.begin() + i);
            return true;
        }
    }
    return false;
}

double Cluster::get_center(int index) {
    return center[index];
}

void Cluster::set_center(int index, double value) {
    center[index] = value;
}

Point Cluster::get_point(int index) {
    return points[index];
}

int Cluster::get_cluster_size() {
    return points.size();
}

int Cluster::get_ID() {
    return id_cluster;
}

int KMeans::get_nearest_center(Point kmpoint)
{
    //from KMEANS POINT to KDTREE POINT
    point kdpoint = stuart_little.nearest(point(kmpoint.get_data(0),kmpoint.get_data(1)));

    for (int i = 0; i < k; ++i) {
        if (clusters[i].get_center(0) == kdpoint.x && clusters[i].get_center(1) == kdpoint.y) {
            return i;
        }
    }
    return -1;

}

KMeans::KMeans(std::string dataset_file_name, int k, int n) {
    this->k = k;
    this->n = n;
    std::ifstream file(dataset_file_name);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file: " << dataset_file_name << std::endl;
        return;
    }
    std::vector<Point> points; std::vector<int> prohibited_indexes; 
    std::string line; int id(0), n_points(0); 

    // Skip the header
    std::getline(file, line);

    // Read the data
    while (std::getline(file, line) && n_points <= n) {
        std::stringstream ss(line); std::string x_str, y_str; std::vector<double> data;

        std::getline(ss, x_str, ',');
        std::getline(ss, y_str, ',');

        data.push_back(std::stod(x_str));
        data.push_back(std::stod(y_str));

        points.emplace_back(id++, data);
        n_points++;
    }
    file.close();

    if (k > n_points)
            return;

    // choose K distinct values for the centers of the clusters
    for(int i = 0; i < k; i++)
    {
        while(true)
        {
            int index_point = rand() % n_points;

            if(find(prohibited_indexes.begin(), prohibited_indexes.end(),
                    index_point) == prohibited_indexes.end())
            {
                prohibited_indexes.push_back(index_point);
                points[index_point].set_cluster(i);
                Cluster cluster(i, points[index_point]);
                clusters.push_back(cluster);
                break;
            }
        }
    }

    int iter = 1;

    while(true)
    {
        bool done = true;
        std::vector<point> centers;
        for (int i = 0; i < k; i++) {
            centers.emplace_back(point(clusters[i].get_center(0), clusters[i].get_center(1)));
        }
        stuart_little.clear(stuart_little.root);
        stuart_little.build(centers);
        // associates each point to the nearest center
        for(int i = 0; i < n_points; i++)
        {
            int id_old_cluster = points[i].get_cluster();
            int id_nearest_center = get_nearest_center(points[i]);

            if(id_old_cluster != id_nearest_center)
            {
                if(id_old_cluster != -1)
                    clusters[id_old_cluster].remove_point(points[i].get_ID());

                points[i].set_cluster(id_nearest_center);
                clusters[id_nearest_center].add_point(points[i]);
                done = false;
            }
        }

        // recalculating the center of each cluster
        for(int i = 0; i < k; i++)
        {
            for(int j = 0; j < POINT_SIZE; j++)
            {
                int total_points_cluster = clusters[i].get_cluster_size();
                double sum = 0.0;

                if(total_points_cluster > 0)
                {
                    for(int p = 0; p < total_points_cluster; p++)
                        sum += clusters[i].get_point(p).get_data(j);
                    clusters[i].set_center(j, sum / total_points_cluster);
                }
            }
        }

        if(done == true || iter >= MAX_ITERATIONS)
        {
            std::cout << "Break in iteration " << iter << std::endl << std::endl;
            break;
        }

        iter++;
    }

    // shows elements of clusters
    for(int i = 0; i < k; i++)
    {
        int total_points_cluster =  clusters[i].get_cluster_size();

        std::cout << "Cluster " << clusters[i].get_ID() + 1 << ": \t("<<clusters[i].get_center(0)<<", "<<clusters[i].get_center(1)<<")"<< std::endl;
        for(int j = 0; j < total_points_cluster; j++)
        {
            std::cout << "\tPoint " << clusters[i].get_point(j).get_ID() + 1 << ": \t("<<clusters[i].get_point(j).get_data(0)<<", "<<clusters[i].get_point(j).get_data(1)<<")"<< std::endl;
        }

        std::cout << std::endl << std::endl;
    }
}


