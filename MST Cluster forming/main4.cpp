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

const double MAX_VAL = 0.0000001;

//typedef std::vector<double> vec_d; // distance
//typedef std::vector<int> vec_int; // vertices
//typedef std::pair<int, int> pair_int; // from - to
//typedef std::vector< std::pair< int, int > > vec_pair_int; // edges
//typedef std::pair<double, int> pair_d_int; // for the set
//typedef std::pair<int, double> pair_int_d; // edge and distance pair
//typedef std::vector< std::pair< int, double > > vec_pair_int_d; // vector of edges and distances
//typedef std::vector< std::vector< std::pair< int, double > > > vec_vec_pair_int_d; // graph

struct DataPoints{

    std::vector< std::array< double, 3 > > pos;
    std::vector< std::array< double, 3 > > mom;

};

std::istream& operator>>(std::istream& is, DataPoints& points){

    std::array< double, 3 > temp_pos;
    std::array< double, 3 > temp_mom;

    std::string line;

    while(std::getline(is, line)){

        std::istringstream data(line);

        int i = 0;

        while(i<3){

            data >> temp_pos[i];
            i++;

        }

        i=0;

        while(i<3){

            data >> temp_mom[i];
            i++;

        }

        points.pos.push_back(temp_pos);
        points.mom.push_back(temp_mom);

    }

    return is;

}

std::ostream& operator<<(std::ostream& os,const DataPoints& points){

    os << "Positions (x,y,z):\n";

    for( auto i : points.pos ){

        for( auto j : i ){

            os << j << " ";

        }

        os << "\n";

    }

    os << "Momenta (x,y,z):\n";

    for( auto i : points.mom ){

        for( auto j : i ){

            os << j << " ";

        }

        os << "\n";

    }

    return os;

}

double distance( const std::array<double, 3>& a, const std::array<double, 3>& b){

    std::array<double, 3> dist;

    for(unsigned int i = 0; i < a.size(); i++){

        dist[i] = a[i] - b[i];

    }

    return std::sqrt(std::accumulate(dist.begin(),dist.end(),0.0,[&](double a, double c){ return a+c*c; }));

}

std::ostream& operator<<(std::ostream& os, const std::array<double, 3>& arr){

    os << " (" << arr[0] << ";" << arr[1] << ";" << arr[2] << ") ";

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

    fout << "\nOpen nodes before: ";

    std::for_each(open_nodes.begin(),open_nodes.end(),[&](int& x){ fout << x << " "; });
    
    fout << "\n";

    int start_vertex;

    if(open_nodes.size()!=0){

        start_vertex = open_nodes.front();

    }else{

        // there are no more open nodes
        return true;

    }

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

        std::for_each(mySet.begin(),mySet.end(),[](const std::pair< double, int >& i){ std::cout << i.first << " " << i.second << "\n"; });
        std::cout << "\n";

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

                if(dst[next_next_vertex] > weight){ // if there's a connection between the vertices then the weight must be smaller
                                                    // than the distance of the next vertex 

                    mySet.erase(mySet.find( std::make_pair(dst[next_next_vertex],next_next_vertex) )); 
                    dst[next_next_vertex] = weight;

                    mySet.insert( std::make_pair(dst[next_next_vertex],next_next_vertex) );

                    ancest[next_next_vertex] = next_vertex;

                }

            }

        }

    }

    std::list<int> clustered;

    std::set_difference(open_nodes_before.begin(), open_nodes_before.end(),
                        open_nodes.begin(), open_nodes.end(),
                        std::back_inserter(clustered));

    if(clustered.size() == 1){

        fout << "Isolated point: " << data.pos[clustered.front()] << "\n";

    }else{

    fout << "Clustered vertices: ";
    std::for_each(clustered.begin(), clustered.end(), [&](int& x){ fout << x << " "; });
    fout << "\n";

    fout << "MST is set:\n";

    for( unsigned int i = 0; i < edges.size(); i++ ){

            if( std::find(clustered.begin(), clustered.end(), edges[i].first) != clustered.end()
            &&  std::find(clustered.begin(), clustered.end(), edges[i].second) != clustered.end() ){
         
                fout << data.pos[edges[i].first] << " --> " << data.pos[edges[i].second] << "\n";

            }

    }

    }

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

    fout << "Open nodes after: ";
    std::for_each(open_nodes.begin(),open_nodes.end(),[&](int& x){ fout << x << " "; });
    fout << "\n";

    return (counter==0) ? true : false;

    

}

int main()
{

    std::ifstream fin("data.dat");

    DataPoints data;

    fin >> data;

    int num_of_vertices = data.pos.size(); // number of vertices

    std::vector< std::vector< std::pair< int, double > > > Graph(num_of_vertices); // my graph

    std::vector<bool> visited(num_of_vertices, false); // list of visited vertices

    /*
    std::cout << "\n";
    std::for_each(dst.begin(), dst.end(), [&](double& x){ std::cout << x << " "; });
    std::cout << "\n\n";
    */

    std::vector< int > ancest(num_of_vertices, -1);

    std::vector< std::pair< int, int > > edges;

    // calculate edges based on euclidian distance
    // later only need to change pos to mom !!!

    int P, Q;
    double dist;

    for( unsigned int i = 0; i < data.pos.size(); i++ ){

        P = i;

        for( unsigned int j = 0; j < i; j++ ){

            Q = j;

            dist = distance( data.pos[i],data.pos[j] );

            Graph[P].push_back(std::make_pair(Q, dist));

            Graph[Q].push_back(std::make_pair(P, dist));

        }

        for( unsigned int j=i+1; j < data.pos.size(); j++ ){

            Q = j;

            dist = distance( data.pos[i],data.pos[j] );

            Graph[P].push_back(std::make_pair(Q, dist));

            Graph[Q].push_back(std::make_pair(P, dist));

        }

    }

    std::list<int> open_nodes;

    for(int i = 0; i < num_of_vertices; i++){

        open_nodes.push_back(i);

    }
    
    std::ofstream fout("output.txt");

    while(!prim(fout, visited, ancest, edges, data, Graph,open_nodes) && open_nodes.size()!=0);

    std::cout << "\nThe algorithm is done.\n";

    return 0;

}
