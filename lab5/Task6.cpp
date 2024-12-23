#include <iostream>
#include <vector>
#include <queue>


std::vector<int> topological_sort(const std::vector<std::vector<int>>& adjacency_matrix) {
    int num_vertices = adjacency_matrix.size();
    std::vector<int> in_degree(num_vertices, 0); // Initialize in-degrees to 0

    // Calculate in-degrees for each vertex
    for (int i = 0; i < num_vertices; ++i) {
        for (int j = 0; j < num_vertices; ++j) {
            if (adjacency_matrix[j][i] == 1) {
                in_degree[i]++;
            }
        }
    }

    std::queue<int> queue; // Queue for vertices with in-degree 0
    for (int i = 0; i < num_vertices; ++i) {
        if (in_degree[i] == 0) {
            queue.push(i);
        }
    }

    std::vector<int> sorted_vertices; // Result vector
    while (!queue.empty()) {
        int vertex = queue.front();
        queue.pop();
        sorted_vertices.push_back(vertex);

        // Update in-degrees of neighbors
        for (int i = 0; i < num_vertices; ++i) {
            if (adjacency_matrix[vertex][i] == 1) {
                in_degree[i]--;
                if (in_degree[i] == 0) {
                    queue.push(i);
                }
            }
        }
    }

    // Check for cycles (if not all vertices were added to sorted_vertices)
    if (sorted_vertices.size() != num_vertices) {
        return {}; // Return empty vector if cycle detected
    }

    return sorted_vertices;
}

int main() {
    std::vector<std::vector<int>> adjacency_matrix = {
        {0, 1, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
        {0, 0, 0, 0}
    };

    std::vector<int> sorted_vertices = topological_sort(adjacency_matrix);
    if (!sorted_vertices.empty()) {
        std::cout << "Topologically sorted vertices: ";
        for (int vertex : sorted_vertices) {
            std::cout << vertex << " ";
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "Graph contains a cycle" << std::endl;
    }

    return 0;
}
