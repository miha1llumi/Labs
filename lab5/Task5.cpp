#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <sstream>
#include <map>
#include <algorithm> // for std::reverse


// Exception class for an empty stack
class EmptyStackException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Stack is empty!";
    }
};

class Stack {
private:
    std::stack<std::string> s;
public:
    void push(const std::string& val) { s.push(val); }
    std::string pop() {
        if (s.empty()) throw EmptyStackException();
        std::string top = s.top(); s.pop(); return top;
    }
    bool empty() const { return s.empty(); }
    std::vector<std::string> getList() const {
        std::vector<std::string> list(s.size());
        std::stack<std::string> temp = s;
        for (size_t i = 0; i < list.size(); ++i) {
            list[i] = temp.top(); temp.pop();
        }
        std::reverse(list.begin(), list.end());
        return list;
    }
};

int main() {
    //Graph representation using an adjacency list (more efficient than an adjacency matrix for sparse graphs)
    std::map<std::string, std::vector<std::string>> adjList = {
        {"a", {"w", "b", "c"}},
        {"w", {"d"}},
        {"d", {"h", "k"}},
        {"h", {}},
        {"k", {"r", "s"}},
        {"r", {"y", "z"}},
        {"y", {}},
        {"z", {}},
        {"s", {}},
        {"b", {"e"}},
        {"e", {"l"}},
        {"l", {"t", "x"}},
        {"t", {"i"}},
        {"i", {}},
        {"x", {"o"}},
        {"o", {}},
        {"c", {"f", "g"}},
        {"f", {"m"}},
        {"m", {}},
        {"g", {"n", "p"}},
        {"n", {}},
        {"p", {"u", "q"}},
        {"u", {"v"}},
        {"v", {}},
        {"q", {}}
    };

    std::vector<std::string> vertexes;
    for (const auto& pair : adjList) {
        vertexes.push_back(pair.first);
    }


    std::vector<std::string> visited;
    Stack stack;
    std::string startNode = "a";
    stack.push(startNode);
    visited.push_back(startNode);

    auto printVector = [](const std::vector<std::string>& vec) {
        for (size_t i = 0; i < vec.size(); ++i) {
            std::cout << vec[i] << (i < vec.size() - 1 ? ", " : "");
        }
        };

    std::cout << startNode << " | ";
    printVector(stack.getList());
    std::cout << " | ";
    printVector(visited);
    std::cout << std::endl;

    auto dfs = [&](auto self, const std::string& node) -> void {
        for (const std::string& neighbor : adjList[node]) {
            bool alreadyVisited = std::find(visited.begin(), visited.end(), neighbor) != visited.end();
            if (!alreadyVisited) {
                std::cout << node << " => " << neighbor << " | ";
                printVector(stack.getList()); // Используем printVector
                std::cout << " | ";
                printVector(visited); // Используем printVector
                std::cout << std::endl;
                stack.push(neighbor);
                visited.push_back(neighbor);
                self(self, neighbor);
            }
        }
        if (!stack.empty()) {
            std::string backtrackNode = stack.pop();
            std::cout << node << " <= " << backtrackNode << " | ";
            printVector(stack.getList()); // Используем printVector
            std::cout << " | ";
            printVector(visited); // Используем printVector
            std::cout << std::endl;
        }
        };
    dfs(dfs, startNode);


    std::cout << "DFS traversal: ";
    printVector(visited);
    std::cout << std::endl;
    return 0;
}
