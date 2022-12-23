#include "graph.h"

#include <fstream>
#include <iostream>

using namespace std;

// конструктор
// принимает на вход имя файла с матрицей смежности
// считывает матрицу смежности из файла
// конвертирует матрицу смежности в список смежности
// инициализирует массив цветов вершин
// инициализирует хроматическое число
Graph::Graph(const std::string& matrix_filename) {
  std::ifstream input(matrix_filename);
  // проверка на существование входного файла
  if (!input.is_open()) {
    std::cout << "File not found" << std::endl;
    throw std::invalid_argument("File not found");
  }
  std::string s;
  // считывание размера матрицы смежности
  getline(input, s);

};

// функция для добавления цветов в вектор
void Graph::add_colors_drawing() {
  for (int i = 0; i < vertex_num; i++) {
    colored_vertices.push_back(color[i]);
  }
};

// возвращает хроматическое число (максимальное число цветов)
int Graph::chromatic_number() { return chromatic_num; };

// поиск хроматического числа (максимального числа цветов)
void Graph::find_chromatic_number() {
  std::vector<int> result(vertex_num);

  result[0] = 0;

  for (int u = 1; u < vertex_num; u++) {
    result[u] = -1;
  }
  // Временный массив для хранения доступных цветов

  std::vector<bool> available(vertex_num);
  for (int cr = 0; cr < vertex_num; cr++) {
    available[cr] = false;
  }
  // Назначаем цвета оставшимся вершинам vertex_num-1
  for (int u = 1; u < vertex_num; u++) {
    // Смотрим все соседние вершины и отмечаем их цвета как недоступные
    std::list<int>::iterator i;
    for (i = adjacency[u].begin(); i != adjacency[u].end(); ++i) {
      if (result[*i] != -1) {
        available[result[*i]] = true;
      }
    }
    // Ищем первый доступный цвет
    int cr;
    for (cr = 0; cr < vertex_num; cr++) {
      if (available[cr] == false) {
        break;
      }
    }
    result[u] = cr;
  }
  int max = 0;
  for (int i = 0; i < vertex_num; i++) {
    if (result[i] > max) {
      max = result[i];
    }
  }

  // присваиваем хроматическому числу максимальное число цветов
  chromatic_num = max;
};

// Проверка, можно ли раскрасить граф
bool Graph::rlf_coloring() {
  int m = chromatic_num;
  for (int i = 0; i < vertex_num; i++) {
    color.push_back(0);
  }

  if (!graph_color_util(m, 0)) {
    // Если раскрасить невозможно
    std::cout << "Color scheme is not possible" << std::endl;
    return false;
  }

  add_colors_drawing();
  return true;
}

// выводим матрицу смежности
void Graph::print_matrix() {
  for (int i = 0; i < graph_matrix.size(); i++) {
    for (int j = 0; j < graph_matrix.size(); j++) {
      std::cout << graph_matrix[i][j] << " ";
    }
    std::cout << std::endl;
  }
};

// Присваивание цветов
void Graph::greedy_coloring() {
  std::vector<int> result(vertex_num);

  result[0] = 0;

  for (int u = 1; u < vertex_num; u++) {
    result[u] = -1;
  }
  // Временный массив для хранения доступных цветов

  std::vector<bool> available(vertex_num);
  for (int cr = 0; cr < vertex_num; cr++) {
    available[cr] = false;
  }
  // Присваиваем цвет оставшимся vertex_num-1 вершинам
  for (int u = 1; u < vertex_num; u++) {
    // Смотрим все соседние вершины и отмечаем их цвета как недоступные
    std::list<int>::iterator i;
    for (i = adjacency[u].begin(); i != adjacency[u].end(); ++i) {
      if (result[*i] != -1) {
        available[result[*i]] = true;
      }
    }

    // Ищем первый доступный цвет
    int cr;
    for (cr = 0; cr < vertex_num; cr++) {
      if (available[cr] == false) {
        break;
      }
    }

    result[u] = cr;
    // Возвращаем значения обратно к false для следующей итерации
    for (i = adjacency[u].begin(); i != adjacency[u].end(); ++i) {
      if (result[*i] != -1) {
        available[result[*i]] = false;
      }
    }
  }

  for (int u = 0; u < vertex_num; u++) {
    colored_vertices.push_back(result[u]);
  }
};

// экспорт в файл .dot (для отрисовки графа в graphviz)
void Graph::export_graph(const std::string& output_filename) {
  std::ofstream out;
  out.open(output_filename);

  if (out.is_open()) {
    out << "graph Graph_coloring { \n\tnode [shape=circle width=0.66 "
           "style=filled]\n";

    for (int u = 0; u < vertex_num; u++) {
      out << "\t" << u
          << " [color = " << colors[colored_vertices[u]]
          << std::uppercase << " label=\"" << u << "\"]\n";
    }

    for (int u = 0; u < vertex_num; u++) {
      for (int i : adjacency[u]) {
        if (i > u) {
          out << "\t" << u << " -- " << i << ";\n";
        }
      }
    }
  }
  out << "}";
  out.close();
};
