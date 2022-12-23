#pragma once

#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const std::string rainbow[8] = {"#dd71e5", "#3AE6CA", "#ffef6b", "#008eff",
                                "#e34557", "#b8ea59", "#da9062", "#6356ef"};

class Graph {
 private:
  int vertex_count;        // количество вершин
  int chromatic_num;       // хроматическое число
  std::vector<int> color;  // массив цветов вершин
  std::vector<std::vector<int>> graph_matrix;  // матрица смежности
  std::vector<std::vector<int>> graph_list;  // список смежности

 public:
  // конструктор
  Graph(const std::string& matrix_filename);

  // отладочная функция 
  void debug();

  // переборный алгоритм раскраски графа
  void graph_coloring();

  // жадный алгоритм раскраски графа
  void greedy_coloring();

  // экспорт в файл .dot (для отрисовки графа в graphviz)
  void export_graph(const std::string& output_filename);
};