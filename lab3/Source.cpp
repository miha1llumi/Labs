#include <iostream>
#include <vector>
#include <string>

using namespace std;

void printVector(const vector<vector<int> >& v) {
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[i].size(); j++) 
        {
            cout << v[i][j] << " ";
        }
        cout << endl;
    }
}

vector<vector<int>> createZeroTriangle(const vector<vector<int> >& triangle) {
    int n = triangle.size();
    vector<vector<int> > zeroTriangle(n); 

    for (int i = 0; i < n; i++) {
        // plus two to correct work of algorithm
        zeroTriangle[i].resize(triangle[i].size() + 2, 0); // fill lines by zeros
    }

    return zeroTriangle;
}

pair<int, vector<int>> findMaxTrianglePath(const vector<vector<int> >& triangle) {
    int n = triangle.size();

    vector<vector<int>> triangle_sum_paths = createZeroTriangle(triangle);

    int first_value = triangle[0][0];
    triangle_sum_paths[0][1] = first_value;
    
    for (int i = 1; i < n; i++) {
        for (int j = 1; j < i + 2; j++) {
            int left = triangle_sum_paths[i - 1][j - 1];
            int right = triangle_sum_paths[i - 1][j];

            int max_el = max(right, left);
            triangle_sum_paths[i][j] = triangle[i][j - 1] + max_el;
        }
    }
    // find max sum and index sum
    int max_sum = 0;
    int max_index = -1;

    for (int j = 1; j <= n; j++) { 
        if (triangle_sum_paths[n - 1][j] > max_sum) {
            max_sum = triangle_sum_paths[n - 1][j];
            max_index = j; 
        }
    }

    // rebuild path
    vector<int> path;
    path.push_back(triangle[n - 1][max_index - 1]); 
    
    for (int i = n - 1; i > 0; i--) {
        if (max_index > 1 && triangle_sum_paths[i - 1][max_index - 1] >= triangle_sum_paths[i - 1][max_index]) {
            max_index--; // go left
        }
        path.push_back(triangle[i - 1][max_index - 1]); 
    }

    reverse(path.begin(), path.end());
    return { max_sum, path };
}

void find_path_to_start(int i, int j, const vector<vector<int>>& paths_matrix, const vector<vector<int>>& matrix, vector<int>& vertices) {
    int top_border = 0;
    int right_border = matrix[0].size();

    if (i - 1 < top_border && j + 1 > right_border) {
        return;
    }

    int top = INT_MAX;
    pair<int, int> top_ind = { 0, 0 };
    if (i - 1 >= top_border) {  // don't exit of top border
        int original_i = 2 * i - 1;
        int original_j = j;

        top = paths_matrix[i - 1][j] + matrix[original_i][original_j];
        top_ind = { original_i, original_j };
    }

    int right = INT_MAX;
    pair<int, int> right_ind = { 0, 0 };
    if (j + 1 < right_border) {
        int original_i = 2 * i;
        int original_j = j;

        right = paths_matrix[i][j + 1] + matrix[original_i][original_j];
        right_ind = { original_i, original_j };
    }

    pair<int, int> indexes;
    if (top == paths_matrix[i][j]) {
        indexes = top_ind;
        i--;
    }
    else {
        indexes = right_ind;
        j++;
    }

    vertices.push_back(matrix[indexes.first][indexes.second]);
    find_path_to_start(i, j, paths_matrix, matrix, vertices);
}

pair<int, vector<int> > find_min_turtle_path(const vector<vector<int>>& matrix) {
    int n_lines = matrix.size() / 2 + 1;
    int n_columns = matrix[1].size();

    vector<vector<int>> paths_matrix(n_lines, vector<int>(n_columns, 0));

    // fill top borders
    for (int i = n_columns - 2; i >= 0; i--) {
        int now_and_prev = matrix[0][i] + paths_matrix[0][i + 1];
        paths_matrix[0][i] = now_and_prev;
    }

    // fill right borders
    for (int i = 1; i < n_lines; i++) {
        int now_and_prev = matrix[2 * i - 1][n_columns - 1] + paths_matrix[i - 1][n_columns - 1];
        paths_matrix[i][n_columns - 1] = now_and_prev;
    }

    for (int i = 1; i < n_lines; i++) {
        for (int j = n_columns - 2; j >= 0; j--) {
            // path weight from top
            int top_vl = paths_matrix[i - 1][j] + matrix[2 * i - 1][j];
            // path weight from right
            int right_vl = paths_matrix[i][j + 1] + matrix[2 * i][j];

            paths_matrix[i][j] = min(top_vl, right_vl);
        }
    }

    vector<int> vertices;
    find_path_to_start(n_lines - 1, 0, paths_matrix, matrix, vertices);
    reverse(vertices.begin(), vertices.end());

    return { paths_matrix[n_lines - 1][0], vertices };
}

void find_max_operations_sequence(const int number_degree) {
    vector<int> min_ops(number_degree); // min operations
    min_ops[1] = 0;

    for (int i = 2; i <= number_degree; i++)
    {
        min_ops[i] = min_ops[i - 1] + 1;
        for (int j = 2; j <= i - 1; j++)
        {
            min_ops[i] = min(min_ops[i], min_ops[j] + min_ops[i - j] + 1);
            if (i % j == 0)
                min_ops[i] = min(min_ops[i], min_ops[i / j] + j - 1);
        }
    }
    
}

bool is_valid_word(const string& word) {
    for (char c : word) {
        if (c != 'a' && c != 'g' && c != 'c' && c != 't') {
            return false;
        }
    }
    return true;
}

string find_max_subsequence(string word1, string word2) {
    int n = word1.length();
    int m = word2.length();

    // + 1 for null line and column
    vector<vector<int>> W(m + 1, vector<int>(n + 1, 0)); 
    vector<vector<pair<int, int>>> prev(m + 1, vector<pair<int, int>>(n + 1)); 

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (word1[j - 1] == word2[i - 1]) {
                W[i][j] = W[i - 1][j - 1] + 1;
                prev[i][j] = { i - 1, j - 1 };
            }
            else {
                W[i][j] = max(W[i - 1][j], W[i][j - 1]);
                if (W[i][j] == W[i - 1][j]) {
                    prev[i][j] = { i - 1, j };
                }
                else {
                    prev[i][j] = { i, j - 1 };
                }
            }
        }
    }


    string result = "";
    int row = m, col = n;
    while (row > 0 && col > 0) {
        if (W[row][col] == W[row - 1][col]) {
            row--;
        }
        else if (W[row][col] == W[row][col - 1]) {
            col--;
        }
        else {
            result = word2[row - 1] + result;
            row--;
            col--;
        }
    }
    return result;
}

pair<int, vector<int>> find_max_utility() {
    int max_weight;
    int num_items;

    cout << "Enter max_weight of bagpack: ";
    cin >> max_weight;
    cout << "Enter number of items: ";
    cin >> num_items;

    vector<int> weights(num_items);
    vector<int> values(num_items);
    vector<int> indices(num_items);

    for (int i = 0; i < num_items; ++i) {
        cout << "Enter weight and utility of item: " << i + 1 << ": ";
        cin >> weights[i] >> values[i];
        indices[i] = i + 1;
    }

    vector<vector<int>> knapsack_values(num_items + 1, vector<int>(max_weight + 1, 0));
    vector<vector<int>> item_used(num_items + 1, vector<int>(max_weight + 1, 0));

    for (int i = 1; i <= num_items; ++i) {
        for (int w = 1; w <= max_weight; ++w) {
            if (weights[i - 1] <= w) {
                int value_with_item = values[i - 1] + knapsack_values[i - 1][w - weights[i - 1]];
                if (value_with_item > knapsack_values[i - 1][w]) {
                    knapsack_values[i][w] = value_with_item;
                    item_used[i][w] = 1;
                }
                else {
                    knapsack_values[i][w] = knapsack_values[i - 1][w];
                }
            }
            else {
                knapsack_values[i][w] = knapsack_values[i - 1][w];
            }
        }
    }


    int max_value = knapsack_values[num_items][max_weight];
    vector<int> selected_items;
    int current_weight = max_weight;
    int current_item = num_items;

    while (current_item > 0 && current_weight > 0) {
        if (item_used[current_item][current_weight] == 1) {
            selected_items.push_back(indices[current_item - 1]);
            current_weight -= weights[current_item - 1];
        }
        current_item--;
    }

    reverse(selected_items.begin(), selected_items.end()); // Reverse for correct order
    return make_pair(max_value, selected_items);
}

pair<int, vector<string>> minimize_operations(int n) {
    vector<int> Op(n + 1);
    Op[1] = 0;

    for (int i = 2; i <= n; ++i) {
        Op[i] = Op[i - 1] + 1; 

        for (int j = 2; j <= i / 2; ++j) {
            Op[i] = min(Op[i], Op[j] + Op[i - j] + 1); 
            if (i % j == 0) {
                Op[i] = min(Op[i], Op[i / j] + j - 1); 
            }
        }
    }
    vector<vector<string>> seq(n + 1);
    Op[1] = 0; 

    for (int i = 2; i <= n; ++i) {
        Op[i] = Op[i - 1] + 1;
        seq[i] = seq[i - 1]; 
        seq[i].push_back(" * 1"); 

        for (int j = 1; j < i; ++j) {
            if (Op[j] + Op[i - j] + 1 < Op[i]) {
                Op[i] = Op[j] + Op[i - j] + 1;
                seq[i].clear(); 
                seq[i].insert(seq[i].end(), seq[j].begin(), seq[j].end());
                seq[i].insert(seq[i].end(), seq[i - j].begin(), seq[i - j].end());
                seq[i].push_back(" * "); 
            }
        }

        for (int j = 2; j <= i; ++j) {
            if (i % j == 0) {
                if (Op[i / j] + j - 1 < Op[i]) {
                    Op[i] = Op[i / j] + j - 1;
                    seq[i].clear(); 
                    seq[i].insert(seq[i].end(), seq[i / j].begin(), seq[i / j].end());
                    seq[i].push_back(" ^ " + to_string(j)); 
                }
            }
        }
    }

    return { Op[n], seq[n] };
}

int main() {
    setlocale(LC_ALL, "russian");

    vector<vector<int>> vec = {
        {7},
        {3, 8},
        {8, 1, 0},
    };

    auto result = findMaxTrianglePath(vec);
    cout << "Maximum path in triangle: " << result.first << endl;
    cout << "Path: ";
    for (int value : result.second) {
        cout << value << " ";
    }
    cout << endl;

    vector<vector<int>> mt = {
       {5, 7, 8},
       {5, 6, 7, 6},
       {3, 2, 5},
       {2, 4, 5, 4},
       {5, 7, 9},
       {3, 2, 5, 2},
       {4, 6, 7}
    };

    pair<int, vector<int>> result2 = find_min_turtle_path(mt);
    cout << "Min path sum in rectangle: " << result2.first << endl;
    cout << "Path: ";
    for (int val : result2.second) {
        cout << val << " ";
    }
    cout << endl;

    // ---------------------------------------------------------------
    int n;

    cout << "Enter degree of number: ";
    cin >> n;

    pair<int, vector<string>> result3 = minimize_operations(n);
    cout << "Min numbers of operations: " << result3.first << endl;
    cout << "Sequence of operations:" << endl;
    for (string val : result3.second) {
        cout << val << " ";
    }
    cout << endl;

    string word1, word2;

    // Enter first word
    do {
        cout << "Enter first word (only letters 'a', 'g', 'c', 't'): ";
        cin >> word1;
    } while (!is_valid_word(word1));

    // Enter second word
    do {
        cout << "Enter second word (only letters 'a', 'g', 'c', 't'): ";
        cin >> word2;
    } while (!is_valid_word(word2));

    string result4 = find_max_subsequence(word1, word2);
    cout << "Max subsequence: " << result4 << endl;

    pair<int, vector<int>> result5 = find_max_utility();
    cout << "Max utility: " << result5.first << endl;
    cout << "Items in bagpack: ";
    for (int item : result5.second) {
        cout << item << " ";
    }
    cout << endl;
    return 0;
}
