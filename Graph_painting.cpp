#include "Graph_painting.h"

void Graph::set_matrix(const std::string& matrix_filename) {
  std::string s;
  std::ifstream input(matrix_filename);
  int row = 0;

  if (input.is_open()) {
    while (getline(input, s)) {
      std::vector<int> row_vector;
      for (int coloumn = 0; coloumn < (s.size()); coloumn += 2) {
        row_vector.push_back(std::stoi(s.substr(coloumn, 1)));
        if (std::stoi(s.substr(coloumn, 1)) == 1) {
          if (coloumn >= row * 2) {
            this->adjacency[row].push_back(coloumn / 2);
            this->adjacency[coloumn / 2].push_back(row);
          }
        }
      }
      row++;
      this->graph_matrix.push_back(row_vector);
    }
    input.close();
  }
};

bool Graph::graph_color_util(int m, int v) {
  if (v == V) {
    return true;
  }

  for (int i = 1; i <= m; i++) {
    // check for assignment safety
    if (is_safe_to_color(v, i)) {
      color[v] = i;
      // рекурсия для проверки следующих вершин
      if (graph_color_util(m, v + 1)) {
        return true;
      }
      // если цвет не приводит к решению
      color[v] = 0;
    }
  }
  // If no color can be assigned to  vertex
  return false;
}

bool Graph::is_safe_to_color(int v, int c) {
  for (int i = 0; i < V; i++) {
    if (this->graph_matrix[v][i] == 1 && c == color[i]) {
      return false;
    }
  }
  return true;
};

void Graph::add_colors_drawing() {
  for (int i = 0; i < V; i++) {
    this->colored_vertices.push_back(color[i]);
  }
};

void Graph::print_chromatic_number() {
  std::cout << "Chromatic number: " << this->chromatic_num << std::endl;
};

void Graph::chromatic_number() {
  std::vector<int> result(V);
  // Присваиваем первый цвет первой вершине
  result[0] = 0;
  // Инициализируем оставшиеся вершины V-1 как не присвоенные
  for (int u = 1; u < V; u++) {
    result[u] = -1;
    // Для u не присвоено ни одного цвета
  }
  // Временный массив для хранения доступных цветов. True
  // значение available[cr] будет означать, что цвет cr
  // присваивается одной из его смежных вершин
  std::vector<bool> available(V);
  for (int cr = 0; cr < V; cr++) {
    available[cr] = false;
  }
  // Назначаем цвета оставшимся вершинам V-1
  for (int u = 1; u < V; u++) {
    // Смотрим все соседние вершины и отмечаем их цвета как недоступные
    std::list<int>::iterator i;
    for (i = adjacency[u].begin(); i != adjacency[u].end(); ++i) {
      if (result[*i] != -1) {
        available[result[*i]] = true;
      }
    }

    // Ищем первый доступный цвет
    int cr;
    for (cr = 0; cr < V; cr++) {
      if (available[cr] == false) {
        break;
      }
    }

    result[u] = cr;  // Присваиваем найденный цвет
  }
  int max = 0;
  for (int i = 0; i < this->V; i++) {
    if (result[i] > max) {
      max = result[i];
    }
  }

  this->chromatic_num = max;
};

bool Graph::rlf_coloring() {
  // Обозначаем все значения цвета как 0
  int m = this->chromatic_num;
  for (int i = 0; i < this->V; i++) {
    color.push_back(0);
  }

  // graphColorUtil() для вершины 0
  if (!graph_color_util(m, 0)) {
    std::cout << "Color schema is not possible" << std::endl;
    return false;
  }

  add_colors_drawing();
  return true;
}

void Graph::print_matrix() {
  for (int i = 0; i < this->graph_matrix.size(); i++) {
    for (int j = 0; j < this->graph_matrix.size(); j++) {
      std::cout << this->graph_matrix[i][j] << " ";
    }
    std::cout << std::endl;
  }
};

// Присваивание цветов
void Graph::greedy_coloring() {
  std::vector<int> result(V);
  // Присваиваем первый цвет первой вершине
  result[0] = 0;
  // Назначаем цвета оставшимся вершинам V-1
  for (int u = 1; u < V; u++) {
    result[u] = -1;
    // Никакой цвет не присвоен u
  }
  // Временный массив для хранения доступных цветов. True
  // значение available[cr] будет означать, что цвет cr
  // присваивается одной из его смежных вершин
  std::vector<bool> available(V);
  for (int cr = 0; cr < V; cr++) {
    available[cr] = false;
  }
  // Присваиваем цвет оставшимся V-1 вершинам
  for (int u = 1; u < V; u++) {
  // Смотрим все соседние вершины и отмечаем их цвета как недоступные
    std::list<int>::iterator i;
    for (i = adjacency[u].begin(); i != adjacency[u].end(); ++i) {
      if (result[*i] != -1) {
        available[result[*i]] = true;
      }
    }

    // Ищем первый доступный цвет
    int cr;
    for (cr = 0; cr < V; cr++) {
      if (available[cr] == false) {
        break;
      }
    }

    result[u] = cr;  // Присваиваем найденный цвет
    // Возвращаем значения обратно к false для следующей итерации
    for (i = adjacency[u].begin(); i != adjacency[u].end(); ++i) {
      if (result[*i] != -1) {
        available[result[*i]] = false;
      }
    }
  }

  for (int u = 0; u < V; u++) {
    this->colored_vertices.push_back(result[u]);
  }
};

void Graph::draw_graph(const std::string& output_filename) {
  std::ofstream out;
  out.open(output_filename);

  if (out.is_open()) {
    out << "graph Graph_coloring { \n\tnode [shape=circle width=0.66 "
           "style=filled]\n";

    for (int u = 0; u < V; u++) {
      out << "\t" << u
          << " [color = " << this->colors[this->colored_vertices[u]]
          << std::uppercase << " label=\"" << u << "\"]\n";
    }

    for (int u = 0; u < V; u++) {
      for (int i : this->adjacency[u]) {
        if (i > u) {
          out << "\t" << u << " -- " << i << ";\n";
        }
      }
    }
  }
  out << "}";
  out.close();
};
