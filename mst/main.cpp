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

double MAX_VAL = 6666.;

const int dim = 3;

int num_of_clusters = 0;

struct DataPoints{

    std::vector<int> hadron;
    std::vector<int> charge;
    std::vector<double> mass;
    std::vector< std::array< double, dim > > pos;
    std::vector< std::array< double, dim > > mom;
    std::vector<int> num_of_coll;

};

std::istream& operator>>(std::istream& is, DataPoints& points);

double distance( const std::array<double, dim>& a, const std::array<double, dim>& b);

std::ostream& operator<<(std::ostream& os, const std::array<double, 3>& arr);

std::ostream& operator<<(std::ostream& os, const std::array<double, 2>& arr);

bool prim(std::ostream& fout, 
          std::vector<bool>& visited, 
          std::vector< int >& ancest,
          std::vector< std::pair< int, int > >& edges,
          const DataPoints& data,
          std::vector< std::vector< std::pair< int, double > > >& Graph,
          std::list<int>& open_nodes);

int main( int argc, char* argv[] )
{

    std::ifstream fin(argv[1]);

    MAX_VAL = atof(argv[2]);

    DataPoints data;

    fin >> data;

    int num_of_vertices = data.pos.size(); // number of vertices

    std::vector< std::vector< std::pair< int, double > > > Graph(num_of_vertices); // my graph

    std::vector<bool> visited(num_of_vertices, false); // list of visited vertices

    std::vector< int > ancest(num_of_vertices, -1);

    std::vector< std::pair< int, int > > edges;

    // calculate edges based on euclidian distance
    // later only need to change pos to mom !!!

    int P, Q;
    double dist;

    auto t0 = std::chrono::high_resolution_clock::now();

    for( unsigned int i = 0; i < data.pos.size(); i++ ){

        P = i;

        for( unsigned int j = 0; j < i; j++ ){

            Q = j;

            dist = distance( data.pos[i],data.pos[j] );

            if(dist < MAX_VAL){

                Graph[P].push_back(std::make_pair(Q, dist));

                Graph[Q].push_back(std::make_pair(P, dist));

            }

        }

        /*

        for( unsigned int j=i+1; j < data.pos.size(); j++ ){

            Q = j;

            dist = distance( data.pos[i],data.pos[j] );

            Graph[P].push_back(std::make_pair(Q, dist));

            Graph[Q].push_back(std::make_pair(P, dist));

        }

        */

    }

    auto t1 = std::chrono::high_resolution_clock::now();

    std::cout << "Graph construction took: " << std::chrono::duration_cast< std::chrono::microseconds >(t1-t0).count() <<
                 " microseconds\n";

    std::list<int> open_nodes;

    for(int i = 0; i < num_of_vertices; i++){

        open_nodes.push_back(i);

    }
    
    std::ofstream fout("output.dat");

    auto t2 = std::chrono::high_resolution_clock::now();

    while(!prim(fout, visited, ancest, edges, data, Graph,open_nodes) && open_nodes.size()!=0);

    auto t3 = std::chrono::high_resolution_clock::now();

    std::cout << "Clustering took: " << std::chrono::duration_cast< std::chrono::microseconds >(t3-t2).count() <<
                 " microseconds\n";

    return 0;

}

std::istream& operator>>(std::istream& is, DataPoints& points){

    std::array< double, dim > temp_pos;
    std::array< double, dim > temp_mom;

    std::string line;

    while(std::getline(is, line)){

        double spare;

        std::istringstream data(line);

        data >> spare;
        points.hadron.push_back(int(spare));
       
        data >> spare;
        points.charge.push_back(int(spare));
        
        data >> spare;
        points.mass.push_back(spare);

        int i = 0;

        while(i<dim){

            data >> temp_mom[i];
            i++;

        }

        i=0;

        while(i<dim){

            data >> temp_pos[i];
            i++;

        }

        points.pos.push_back(temp_pos);
        points.mom.push_back(temp_mom);

        data >> spare;
        points.num_of_coll.push_back(int(spare));

    }

    return is;

}

double distance( const std::array<double, dim>& a, const std::array<double, dim>& b){

    std::array<double, dim> dist;

    for(unsigned int i = 0; i < dim; i++){

        dist[i] = a[i] - b[i];

    }

    return std::sqrt(std::accumulate(dist.begin(),dist.end(),0.0,[&](double x, double y){ return x+y*y; }));

}

std::ostream& operator<<(std::ostream& os, const std::array<double, 3>& arr){

    os << " (" << arr[0] << ";" << arr[1] << ";" << arr[2] << ") ";

    return os;

}

std::ostream& operator<<(std::ostream& os, const std::array<double, 2>& arr){

    os << " (" << arr[0] << ";" << arr[1] << ") ";

    return os;

}

bool prim(std::ostream& fout, 
          std::vector<bool>& visited, 
          std::vector< int >& ancest,
          std::vector< std::pair< int, int > >& edges,
          const DataPoints& data,
          std::vector< std::vector< std::pair< int, double > > >& Graph,
          std::list<int>& open_nodes){

    std::vector< double > dst(Graph.size(), MAX_VAL);

    std::list<int> open_nodes_before = open_nodes;

/*

    fout << "\nOpen nodes before: ";

    std::for_each(open_nodes.begin(),open_nodes.end(),[&](int& x){ fout << x << " "; });
    
    fout << "\n";

*/

    int start_vertex;

    if(open_nodes.size()==0){

        return true;

    }
        
    start_vertex = open_nodes.front();
    
    open_nodes.remove(start_vertex);

    std::set< std::pair< double, int > > mySet; // distance and vertex

    for( int i = 0; i < visited.size(); i++ ){ // visited.size() is equal to number of vertices
 
        mySet.insert(std::make_pair(dst[i],i));

        // make pairs with vertex number and it's distance at the beginning 
        // dst[i]s are all set to MAX_VAL

    }

    // erase start value, then set its distance to zero

    mySet.erase(mySet.find(std::make_pair(dst[start_vertex],start_vertex)));

    dst[start_vertex] = 0.; // current distance from itself
                            // all the others are set to MAX_VAL

    mySet.insert(std::make_pair(dst[start_vertex],start_vertex));

    // PRIM'S ALGORITHM

    while(!mySet.empty()){

        /*

        std::for_each(mySet.begin(),mySet.end(),[](const std::pair< double, int >& i){ std::cout << i.first << " " << i.second << "\n"; });
        std::cout << "\n";

        */

        // a set is always ordered, the order is based on the first element of it, so the distance from itself

        std::pair< double, int > top_vertex = *mySet.begin(); // gives back an iterator
                                                // to the top element of
                                                // the set so need to derefer it
                                                // and make it a pair

        mySet.erase(mySet.begin());

        int next_vertex = top_vertex.second; // current vertex closest neighbor

        if(top_vertex.first == MAX_VAL){

            break; // somethins is wrong !

        }

        visited[next_vertex] = true; // now this is visited

        open_nodes.remove(next_vertex);

        if(next_vertex != start_vertex  
        /* && ( std::find(open_nodes.begin(),open_nodes.end(),start_vertex ) != open_nodes.end()  ) */){ 
                                        // at first 0 is the first vertex and zero is from 0 distance from itself
                                        // so then it is skipped because the next_vertex is zero
                                        // this starts with 0's neighbor when ancest is already set

            edges.push_back(std::make_pair(ancest[next_vertex],next_vertex)); // make connection between next_vertex and
                                                                                // its ancestor

        }

        for( unsigned int i = 0; i<Graph[next_vertex].size(); i++ ){

            if( visited[Graph[next_vertex][i].first] == false ){ // Graph[next_vertex] is a vector of pairs to-vertex and its distance

                int next_next_vertex = Graph[next_vertex][i].first; // if next_vertexes neighbor is not visited then make it the
                                                                    // next vertex to check
                double weight = Graph[next_vertex][i].second; // set the weight of the egde between them

                /*

                if(dst[next_next_vertex] > weight){ // if there's a connection between the vertices then the weight must be smaller
                                                    // than the distance of the next vertex 

                                                    */

                    mySet.erase(mySet.find( std::make_pair(dst[next_next_vertex],next_next_vertex) )); 
                    dst[next_next_vertex] = weight;

                    mySet.insert( std::make_pair(dst[next_next_vertex],next_next_vertex) );

                    ancest[next_next_vertex] = next_vertex;

                    /*

                } */

            }

        }

    }

    std::list<int> clustered;

    std::set_difference(open_nodes_before.begin(), open_nodes_before.end(),
                        open_nodes.begin(), open_nodes.end(),
                        std::back_inserter(clustered));

    fout << clustered.size() << "\n";

    /*

    fout << "Cluster size: " << clustered.size() << "\n";

    if(clustered.size() == 1){

        fout << "Isolated point: " << data.pos[clustered.front()] << "\n";

    }else{


    fout << "Clustered vertices: ";
    std::for_each(clustered.begin(), clustered.end(), [&](int& x){ fout << x << " "; });
    fout << "\n";


    */

    /*

    fout << "MST is set:\n";

    for( unsigned int i = 0; i < edges.size(); i++ ){

            if( std::find(clustered.begin(), clustered.end(), edges[i].first) != clustered.end()
            &&  std::find(clustered.begin(), clustered.end(), edges[i].second) != clustered.end() ){
         
                fout << data.pos[edges[i].first] << " --> " << data.pos[edges[i].second] << "\n";

            }

    }

    }

    */

    edges.clear();

    // delete the connecting branches to visited verticies
    // if the size of the bracnhes of the graph add up to 0 then return true
    // else return false

    int counter = 0;

    for( unsigned int i = 0; i < visited.size(); i++ ){

        if(visited[i] == true){

            Graph[i].clear();

        }else{

            counter++;

        }

    }

    /*

    fout << "Open nodes after: ";
    std::for_each(open_nodes.begin(),open_nodes.end(),[&](int& x){ fout << x << " "; });
    fout << "\n";
    
    */

    num_of_clusters++;
    std::stringstream ss;
    ss << num_of_clusters;     

    std::ofstream cluster("C"+ss.str()+".dat");

    std::for_each(clustered.begin(), clustered.end(), [&](int& x){ 
        cluster << data.hadron[x] << " " << data.charge[x] << " " << data.mass[x] << " ";
        
            for(unsigned int b = 0; b < dim; b++){
        
                cluster << data.mom[x][b] << " ";
        
            }
        
            for(unsigned int b = 0; b < dim; b++){
                
                cluster << data.pos[x][b] << " ";
                
            }
        
          cluster << data.num_of_coll[x] << "\n";
     });

    cluster.close();

    if(counter == 0){
        fout << "*************************\n NUMBER OF CLUSTERS: " << num_of_clusters;
        return true;
    }else{
        return false;
    }

    

}