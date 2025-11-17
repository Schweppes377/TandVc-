#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <set>
#include <locale.h>

using namespace std;

struct Node {
    vector<int> path;
    int cost;
    int reducedCost;
    int vertex;
    int level;
};

struct Compare {
    bool operator()(const Node& a, const Node& b) {
        return a.reducedCost > b.reducedCost;
    }
};

void reduceMatrix(vector<vector<int>>& matrix, int& cost) {
    // Уменьшение строк
    for (int i = 0; i < matrix.size(); ++i) {
        int rowMin = INT_MAX;
        for (int j = 0; j < matrix.size(); ++j) {
            if (matrix[i][j] < rowMin) {
                rowMin = matrix[i][j];
            }
        }
        if (rowMin != INT_MAX && rowMin > 0) {
            for (int j = 0; j < matrix.size(); ++j) {
                if (matrix[i][j] != INT_MAX) {
                    matrix[i][j] -= rowMin;
                }
            }
            cost += rowMin;
        }
    }

    // Уменьшение столбцов
    for (int j = 0; j < matrix.size(); ++j) {
        int colMin = INT_MAX;
        for (int i = 0; i < matrix.size(); ++i) {
            if (matrix[i][j] < colMin) {
                colMin = matrix[i][j];
            }
        }
        if (colMin != INT_MAX && colMin > 0) {
            for (int i = 0; i < matrix.size(); ++i) {
                if (matrix[i][j] != INT_MAX) {
                    matrix[i][j] -= colMin;
                }
            }
            cost += colMin;
        }
    }
}

int solveTSP(vector<vector<int>>& graph) {
    priority_queue<Node, vector<Node>, Compare> pq;
    int n = graph.size();
    vector<int> bestPath;
    int bestCost = INT_MAX;

    // Инициализация начальной матрицы
    Node root;
    root.path.push_back(0);
    root.cost = 0;
    root.level = 0;
    root.vertex = 0;

    vector<vector<int>> tempMatrix = graph;
    reduceMatrix(tempMatrix, root.reducedCost);
    root.reducedCost += root.cost;
    pq.push(root);

    while (!pq.empty()) {
        Node curr = pq.top();
        pq.pop();

        if (curr.reducedCost >= bestCost) continue;

        if (curr.level == n - 1) {
            int lastVertex = curr.vertex;
            int startVertex = curr.path[0];
            if (graph[lastVertex][startVertex] != INT_MAX) {
                int totalCost = curr.cost + graph[lastVertex][startVertex];
                if (totalCost < bestCost) {
                    bestCost = totalCost;
                    bestPath = curr.path;
                    bestPath.push_back(startVertex);
                }
            }
            continue;
        }

        for (int i = 0; i < n; ++i) {
            if (find(curr.path.begin(), curr.path.end(), i) != curr.path.end() || graph[curr.vertex][i] == INT_MAX)
                continue;

            Node child;
            child.path = curr.path;
            child.path.push_back(i);
            child.cost = curr.cost + graph[curr.vertex][i];
            child.level = curr.level + 1;
            child.vertex = i;

            tempMatrix = graph;
            for (int j = 0; j < n; ++j) {
                tempMatrix[curr.vertex][j] = INT_MAX;
                tempMatrix[j][i] = INT_MAX;
            }
            tempMatrix[i][0] = INT_MAX;

            reduceMatrix(tempMatrix, child.reducedCost);
            child.reducedCost += child.cost;

            if (child.reducedCost < bestCost) {
                pq.push(child);
            }
        }
    }

    cout << "Минимальная стоимость: " << bestCost << endl;
    cout << "Лучший путь: ";
    for (int v : bestPath) {
        cout << v << " ";
    }
    cout << endl;

    return bestCost;
}

int main() {
    setlocale(LC_ALL, "Russian");

    vector<vector<int>> graph = {
        {INT_MAX, 10, 15, 20},
        {10, INT_MAX, 35, 25},
        {15, 35, INT_MAX, 30},
        {20, 25, 30, INT_MAX}
    };

    solveTSP(graph);

    return 0;
}