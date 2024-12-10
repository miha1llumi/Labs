#include <iostream>
#include <vector>
#include <limits> // for numeric_limits
#include <algorithm> // for min_element


using namespace std;

const double INF = numeric_limits<double>::infinity();


pair<int, int> find_min_edge(int N, const vector<int>& colours, const vector<vector<double>>& distance_matrix) {
    double min_dist = INF;
    int i_min = -1, j_min = -1; 
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) { // top triangle part of matrix
            double cur_dist = distance_matrix[i][j];
            if (cur_dist != INF && colours[i] != colours[j] && cur_dist < min_dist) {
                min_dist = cur_dist;
                i_min = i;
                j_min = j;
            }
        }
    }
    return make_pair(i_min, j_min);
};

pair<vector<int>, vector<int>> prim_kraskal_modified(const vector<vector<double>>& distance_matrix) {
    int N = distance_matrix.size();
    if (N == 0) return { {}, {} }; // zero matrix

    vector<int> res1(N - 1);
    vector<int> res2(N - 1);
    vector<int> colours(N);
    for (int i = 0; i < N; i++) colours[i] = i;

    for (int k = 0; k < N - 1; k++) {
        pair<int, int> edge = find_min_edge(N, colours, distance_matrix);
        if (edge.first == -1) break; // Выход из цикла, если ребро не найдено

        res1[k] = edge.first;
        res2[k] = edge.second;

        int l2 = colours[edge.first];
        int r2 = colours[edge.second];
        for (int j = 0; j < N; j++) {
            if (colours[j] == r2) {
                colours[j] = l2;
            }
        }
    }

    return { res1, res2 };
}


pair<vector<double>, vector<vector<int>>> dijkstra(const vector<vector<double>>& distance_matrix, int start_vertex) {
    int n_vertexes = distance_matrix.size();
    if (n_vertexes == 0) return { {}, {} }; 

    vector<double> dist(n_vertexes, INF);
    dist[start_vertex] = 0;

    vector<int> prev(n_vertexes, -1); // restore the path
    vector<bool> visited(n_vertexes, false);

    for (int count = 0; count < n_vertexes - 1; ++count) {
        double min_dist = INF;
        int u = -1;

        for (int v = 0; v < n_vertexes; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                u = v;
            }
        }

        if (u == -1) break; // unconnected graph
        visited[u] = true;


        for (int v = 0; v < n_vertexes; v++) {
            if (!visited[v] && distance_matrix[u][v] != 0 && dist[u] != INF && dist[u] + distance_matrix[u][v] < dist[v]) {
                dist[v] = dist[u] + distance_matrix[u][v];
                prev[v] = u;
            }
        }
    }

    vector<vector<int>> shortest_paths(n_vertexes);
    for (int i = 0; i < n_vertexes; i++) {
        vector<int> path;
        int current = i;
        while (current != -1) {
            path.push_back(current + 1);
            current = prev[current];
        }
        reverse(path.begin(), path.end());
        shortest_paths[i] = path;
    }
    return { dist, shortest_paths };
}


// Function to input a distance matrix
std::vector<std::vector<double>> inputDistanceMatrix() {
    int rows, cols;
    std::cout << "Enter the size of the matrix (rows and columns): ";
    std::cin >> rows >> cols;

    std::vector<std::vector<double>> matrix(rows, std::vector<double>(cols));

    std::cout << "Enter the distance matrix:" << std::endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cin >> matrix[i][j];
        }
    }
    return matrix;
}

// Predefined distance matrix
std::vector<std::vector<double>> predefinedMatrix = {
    {0, 3, 0, 0, 0, 0, 0, 10, 0},
    {3, 0, 4, 5, 2, 0, 0, 0, 0},
    {0, 4, 0, 6, 0, 0, 0, 0, 0},
    {0, 5, 6, 0, 8, 0, 7, 0, 0},
    {0, 2, 0, 8, 0, 5, 0, 9, 0},
    {0, 0, 0, 0, 5, 0, 4, 8, 5},
    {0, 0, 0, 7, 0, 4, 0, 0, 6},
    {10, 0, 0, 0, 9, 8, 0, 0, 9},
    {0, 0, 0, 0, 0, 5, 6, 9, 0}
};


int main() {
    int choice;
    std::cout << "Choose the distance matrix option:\n1. Use predefined matrix\n2. Input your own matrix\nEnter 1 or 2: ";
    std::cin >> choice;

    std::vector<std::vector<double>> distance_matrix;

    if (choice == 1) {
        distance_matrix = predefinedMatrix;
    }
    else if (choice == 2) {
        distance_matrix = inputDistanceMatrix();
    }
    else {
        std::cout << "Invalid choice. Exiting." << std::endl;
        return 1; // Exit on invalid choice
    }

    // Call to the modified Prim-Kraskal function
    auto result = prim_kraskal_modified(distance_matrix);
    std::cout << "Prim-Kraskal modified:" << std::endl;
    for (int i = 0; i < result.first.size(); ++i) {
        std::cout << result.first[i] << (i == result.first.size() - 1 ? "" : " ");
    }
    std::cout << std::endl;
    for (int i = 0; i < result.second.size(); ++i) {
        std::cout << result.second[i] << (i == result.second.size() - 1 ? "" : " ");
    }
    std::cout << std::endl;

    // Dijkstra's algorithm
    int start_vertex = 3; // You might want to allow user input here as well
    auto result_dijkstra = dijkstra(distance_matrix, start_vertex);
    std::vector<double> distances = result_dijkstra.first; // Correct assignment
    std::vector<std::vector<int>> paths = result_dijkstra.second;

    std::cout << "\nShortest Paths from vertex " << start_vertex + 1 << ":" << std::endl;
    for (size_t i = 0; i < paths.size(); ++i) {
        std::cout << i + 1 << ": ";
        for (size_t j = 0; j < paths[i].size(); ++j) {
            std::cout << paths[i][j] << (j == paths[i].size() - 1 ? "" : "->");
        }
        std::cout << " (distance: " << distances[i] << ")" << std::endl;
    }
    return 0;
}
