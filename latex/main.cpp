#include <iostream>
#include <vector>
#include <utility> 
#include <set>
#include <array>
#include <string>
#include <fstream>
#include <numeric> // for std::accumulate
#include <sstream> // for std::istringstream
#include <algorithm> // for std::for_each, std::find, etc.
#include <cmath> // for std::sqrt
#include <list>
#include <chrono> // to measure time

double infinity = 10000.; // handled as infinity in prim's algorithm
constexpr int dim = 6;
constexpr double scalingFactor = 13.7; // for distance calculations

struct DataPoints{
    
        std::vector<int> hadron;
        std::vector<int> charge;
        std::vector<double> mass;
        std::vector< std::array< double, dim > > posAndMom;
        std::vector<int> num_of_coll;
    
};

std::istream& operator>>(std::istream& is, DataPoints& points);

double distance( const std::array<double, dim>& a, const std::array<double, dim>& b);

bool prim(std::ostream& fout, 
          std::vector<bool>& visitedVertices, 
          std::vector< int >& vertexAncestors,
          std::vector<std::pair<int, int>>& edges,
          std::vector<std::vector<std::pair<int, double>>>& Graph,
          std::list<int>& unclusteredVertices);

int main(int argc, char* argv[]){

    std::ifstream fin(argv[1]);
    infinity = atof(argv[2]);

    DataPoints data;
    fin >> data;

    int numberOfVertices = data.mass.size();

    std::vector<std::vector<std::pair<int, double>>> Graph(numberOfVertices);
    std::vector<bool> visitedVertices(numberOfVertices, false);
    std::vector<int> vertexAncestors(numberOfVertices, -1);
    std::vector<std::pair<int,int>> edges;

    int P,Q;
    double distanceBetweenVertices;

    auto t0 = std::chrono::high_resolution_clock::now();
    
    for( unsigned int i = 0; i < numberOfVertices; i++ ){
    
        P = i;
    
        for( unsigned int j = 0; j < i; j++ ){
    
            Q = j;

            distanceBetweenVertices = distance( data.posAndMom[i],data.posAndMom[j] );
    
            if(distanceBetweenVertices < infinity){
    
                Graph[P].push_back(std::make_pair(Q, distanceBetweenVertices));
    
                Graph[Q].push_back(std::make_pair(P, distanceBetweenVertices));
    
            }
        }
    }

    auto t1 = std::chrono::high_resolution_clock::now();

    std::cout << "Graph construction took: " << 
    std::chrono::duration_cast< std::chrono::microseconds >(t1-t0).count() << " microseconds\n";

    std::list<int> unclusteredVertices;
    for(unsigned int i = 0; i < numberOfVertices; i++){
        unclusteredVertices.push_back(i);
    }

    std::ofstream fout(argv[3]);

    auto t2 = std::chrono::high_resolution_clock::now();
    
    while(!prim(fout, visitedVertices, vertexAncestors, edges, Graph,unclusteredVertices) && unclusteredVertices.size()!=0);
    
    auto t3 = std::chrono::high_resolution_clock::now();

    std::cout << "Clustering took: " <<
    std::chrono::duration_cast< std::chrono::microseconds >(t3-t2).count() << " microseconds\n";

    return 0;

}

std::istream& operator>>(std::istream& is, DataPoints& points){

    std::array<double, dim> tempPosAndMom;
    
    std::string line;

    while(std::getline(is, line)){

        double tempData;

        std::istringstream rawData(line);

        rawData >> tempData;
        points.hadron.push_back(int(tempData));
        rawData >> tempData;
        points.charge.push_back(int(tempData));
        rawData >> tempData;
        points.mass.push_back(tempData);

        for(unsigned int i=0; i < dim; i++){
            rawData >> tempPosAndMom[i];
        }

        points.posAndMom.push_back(tempPosAndMom);

        rawData >> tempData;
        points.num_of_coll.push_back(int(tempData));

    }

    return is;

}

double distance( const std::array<double, dim>& a, const std::array<double, dim>& b){

    double distPos = 0.0;
    double distMom = 0.0;
    
    for(unsigned int i = 0; i < dim/2; i++){
    
        distPos += (a[i] - b[i])*(a[i] - b[i]);
        distMom += scalingFactor*(a[dim/2+i] - b[dim/2+i])*(a[dim/2+i] - b[dim/2+i]);
        
     
    }
    
    return std::sqrt(distPos+distMom);
    
}

bool prim(std::ostream& fout, 
          std::vector<bool>& visitedVertices, 
          std::vector< int >& vertexAncestors,
          std::vector<std::pair<int, int>>& edges,
          std::vector<std::vector<std::pair<int, double>>>& Graph,
          std::list<int>& unclusteredVertices){

    if(unclusteredVertices.size()==0){
        return true;
    }

    int numberOfVertices = visitedVertices.size();
    std::list<int> unclusteredVerticesBefore = unclusteredVertices;

    int statingVertex;
    
    statingVertex = unclusteredVertices.front();
    unclusteredVertices.remove(statingVertex);

    std::vector<double> distanceSet(Graph.size(), infinity);

    std::set<std::pair<double, int>> vertexSet;
    for(unsigned int i = 0; i < numberOfVertices; i++){
        vertexSet.insert(std::make_pair(distanceSet[i], i));
    }
    vertexSet.erase(vertexSet.find(std::make_pair(distanceSet[statingVertex], statingVertex)));
    distanceSet[statingVertex] = 0;
    vertexSet.insert(std::make_pair(distanceSet[statingVertex],statingVertex));

    while(!vertexSet.empty()){

        std::pair<double, int> topVertex = *vertexSet.begin();
        vertexSet.erase(vertexSet.begin());

        int nextVertex = topVertex.second;

        if(topVertex.first == infinity) break;

        unclusteredVertices.remove(nextVertex);

        visitedVertices[nextVertex]  = true;

        if(nextVertex != statingVertex){
            edges.push_back(std::make_pair(vertexAncestors[nextVertex], nextVertex));
        }

        for(unsigned int i = 0; i < Graph[nextVertex].size(); i++){
            if(visitedVertices[Graph[nextVertex][i].first]==false){
                int updatedNextVertex = Graph[nextVertex][i].first;
                double edgeWeight = Graph[nextVertex][i].second;
                vertexSet.erase(vertexSet.find(std::make_pair(distanceSet[updatedNextVertex], updatedNextVertex)));
                distanceSet[updatedNextVertex] = edgeWeight;
                vertexSet.insert(std::make_pair(distanceSet[updatedNextVertex], updatedNextVertex));
                vertexAncestors[updatedNextVertex] = nextVertex;
            }
        }
    }

    std::list<int> clusteredVertices;
    std::set_difference(unclusteredVerticesBefore.begin(), unclusteredVerticesBefore.end(),
                        unclusteredVertices.begin(), unclusteredVertices.end(),
                        std::back_inserter(clusteredVertices));
    fout << clusteredVertices.size() << "\n";

    edges.clear();
    
    int pointsToCluster = 0;
    for(unsigned int i = 0; i < numberOfVertices; i++){
    if(visitedVertices[i] == true){
        Graph[i].clear();
        }else{
            pointsToCluster++;
        }
    }

    return pointsToCluster == 0 ? true : false;

}