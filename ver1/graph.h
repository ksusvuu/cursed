#pragma once

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <vector>

// класс графа
class Graph {
 private:
  int chromatic_num;  // хроматическое число графа
  int vertex_num;     // количество вершин графа
  std::vector<int> colored_vertices;  // вектор цветов вершин
  std::vector<std::vector<int>> graph_matrix;  // матрица смежности графа
  std::vector<int> color;     // вектор цветов вершин
  std::list<int> *adjacency;  // список смежности графа
  std::string colors[8] = {"magenta", "blue",   "yellow",      "green",
                           "red",     "orange", "yellowgreen", "purple"};

 public:
  // конструктор и деструктор
  Graph(const std::string &matrix_filename);
  ~Graph() { delete[] adjacency; }

  void print_matrix();  // вывод матрицы смежности
  void greedy_coloring();  // жадный алгоритм раскраски графа
  // экспорт графа в файл формата .dot
  void export_graph(const std::string &output_filename);
  void find_chromatic_number();  // поиск хроматического числа графа
  int chromatic_number();  // возврат хроматического числа графа

  // проверки для переборного алгоритма раскраски графа
  bool graph_color_util(int m, int v);
  // проверка на возможность добавления цвета к вершине
  bool is_safe_to_color(int v, int c);
  void add_colors_drawing();  // добавление цветов в вектор цветов вершин
  bool rlf_coloring();  // решение задачи о раскраске графа с помощью метода RLF
};