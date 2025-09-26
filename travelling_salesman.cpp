#include <iostream>
#include <stdbool.h>
#include <vector>
#include <algorithm>

using namespace std;

struct node {
    int id; // Which edge is this?
    vector<int> edges; // Which edges are connected to this one?
    vector<float> distances; // How far is each edge?
};

vector<node> main_graph;

/*
    search_ids() -- Search for the ID within a node.
    IN
        vector<node> graph - The graph consisting of nodes.
        int value - The value to search for.
    OUT
        int - The index of the value. Returns -1 if not found.
*/
int search_ids(vector<node> graph, int value){
    for (int i = 0; i < graph.size(); i++){
        if (graph[i].id == value) return i;
    }
    return -1;
}

/*
    search_vector() - Search for val within a vector.
    IN
        vector<int> v - A vector consisting of integers.
        int val - The value to search for.
    OUT
        int - The index of the value. Returns -1 if not found.
*/
int search_vector(vector<int> v, int val){
    for (int i = 0; i < v.size(); i++){
        if (v[i] == val) return i;
    }
    return -1;
}

/*
    add_edge() - Adds an edge between u and v.
    IN
        vector<node> graph - The graph consisting of nodes.
        int u - The vector to connect from.
        int v - The vector to connect to.
        int distance - The distance between these vectors.
*/
void add_edge(vector<node> &graph, int u, int v, float distance){
    int index_u = search_ids(graph, u); // Find the index of u.

    if (index_u == -1) { // If the starting node does not exist...
        cerr << "Error: u not found within graph." << endl;
        return; // Then return due to an error.
    }
    if (search_ids(graph, v) == -1) { // If the ending node does not exist...
        graph.push_back(node()); // Add a node to the end
        graph.back().id = v; // Set its ID to v
    }

    graph[index_u].edges.push_back(v); // Add the edge
    graph[index_u].distances.push_back(distance); // Add the distance

    // If the graph is directional, remove the next two lines.
    graph[v].edges.push_back(u);
    graph[v].distances.push_back(distance);
}

/*
    print_edges() - Print out edges and distances of each vertice.
    IN
        vector<node> graph - The graph consisting of nodes.
*/
void print_edges(vector<node> graph){
    for (node n : graph){
        cout << n.id;
        for (int i = 0; i < n.edges.size(); i++){
            cout << " | to " << n.edges[i] << ", d " << n.distances[i] << "\t\t";
        }  
        cout << endl;
    }
}

/*
    nearest_neighbour_step() - Find the nearest neighbour to start that is not in visited.
    IN
        vector<node> graph - The graph consisting of nodes.
        int start - The ID of the starting node.
        vector<int> visited - The IDs of already visited nodes.
    OUT
        int - The ID of the next node.
*/
int nearest_neighbour_step(vector<node> graph, int start, vector<int> visited){
    int start_id = search_ids(graph, start);
    float distance = -1; // Distance to the next node.
    int next_node = -1;
    for (int i = 0; i < graph[start_id].distances.size(); i++){ // For each distance in the graph starting from start,
        if (find(visited.begin(), visited.end(), graph[start_id].edges[i]) != visited.end()){ // If the edge is visited,
            continue; // continue;
        }
        if ((graph[start_id].distances[i] < distance) || (distance == -1)){ // if it is shorter than the previous shortest (or if it is the first),  
            distance = graph[start_id].distances[i]; // then save the distance,
            next_node = graph[start_id].edges[i]; // and the node ID.
        }
    }

    return next_node;
}

/*
    nearest_neighbour() - Find the path around the graph using the Nearest Neighbour algorithm.
    IN
        vector<node> graph - The graph consistingo of nodes.
        int start - The ID of the starting node.
    OUT
        vector<int> - The path of nodes to follow.
*/
vector<int> nearest_neighbour(vector<node> graph, int start){
    vector<int> visited = {start}; // List of visited nodes in order of visitation
    for (int i = 0; i < (graph.size()); i++){ // For each node in the graph,
        int next_node = nearest_neighbour_step(graph, visited[i], visited); // Get the nearest neighbour.
        if (next_node == -1){
            next_node = start;
        }
        visited.push_back(next_node); // Add it to the visited nodes.
    }

    return visited;
}

float nn_distance(vector<node> graph, vector<int> path){
    float total_distance = 0;
    // 0, 2, 4, 1, 3, 5, 0
    for (int i = 0; i < path.size() - 1; i++){
        int next_node = path[i + 1]; // Get the next node's ID.
        int current_index = search_ids(graph, path[i]); // Get the index of the current node.
        int edge = search_vector(graph[current_index].edges, next_node); // Find the edge corresponding to the next node.
        float distance = graph[current_index].distances[edge]; // Get the distance to this edge from the current node.
        total_distance += distance;
    }

    return total_distance;
}

int main(){
    cout << "Hello!" << endl;

    main_graph.push_back(node());
    main_graph.back().id = 0;

    add_edge(main_graph, 0, 1, 8);
    add_edge(main_graph, 0, 2, 2);
    add_edge(main_graph, 0, 3, 6);
    add_edge(main_graph, 0, 4, 7);
    add_edge(main_graph, 0, 5, 9);

    add_edge(main_graph, 1, 2, 3);
    add_edge(main_graph, 1, 3, 2); // distance[2]
    add_edge(main_graph, 1, 4, 5);
    add_edge(main_graph, 1, 5, 8);
    
    add_edge(main_graph, 2, 3, 6);
    add_edge(main_graph, 2, 4, 1);
    add_edge(main_graph, 2, 5, 7);

    add_edge(main_graph, 3, 4, 8);
    add_edge(main_graph, 3, 5, 2);

    add_edge(main_graph, 4, 5, 9);
    
    vector<int> path = nearest_neighbour(main_graph, 0);
    for (int i = 0; i < path.size(); i++){
        cout << path[i] << endl;
    }

    float distance = nn_distance(main_graph, path);
    cout << "The total distance is " << distance << endl;

    print_edges(main_graph);

    return 0;
}