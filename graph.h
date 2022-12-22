#pragma once

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <vector>

class Graph {
 private:
  int chromatic_num;
  int vertex_num;
  std::vector<int> colored_vertices;
  std::vector<std::vector<int>> graph_matrix;
  std::vector<int> color;
  std::string colors[8] = {"magenta", "blue",   "yellow",      "green",
                           "red",     "orange", "yellowgreen", "purple"};
  std::list<int> *adjacency;

 public:
  // конструктор и деструктор
  Graph(int vertex_num, const std::string &matrix_filename);
  ~Graph() { delete[] adjacency; }

  // методы
  void print_matrix();
  void greedy_coloring();
  void export_graph(const std::string &output_filename);
  void find_chromatic_number();
  int chromatic_number();

  // методы для решения задачи о раскраске графа
  bool graph_color_util(int m, int v);
  bool is_safe_to_color(int v, int c);
  void add_colors_drawing();
  bool rlf_coloring();
};