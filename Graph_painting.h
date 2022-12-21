#pragma once

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <vector>

class Graph
{
private:
  int chromatic_num;
  int V;
  std::vector<int> colored_vertices;
  std::vector<std::vector<int>> graph_matrix;
  std::vector<int> color;
  std::string colors[8] = {"magenta", "blue", "yellow", "green",
                           "red", "orange", "yellowgreen", "purple"};
  std::list<int> *adjacency;

public:
  Graph(int V)
  {
    this->V = V;
    adjacency = new std::list<int>[V];
    this->chromatic_num = V;
  }
  ~Graph() { delete[] adjacency; }
  void set_matrix(const std::string &matrix_filename);
  void print_matrix();
  void greedy_coloring();
  void draw_graph(const std::string &output_filename);
  void chromatic_number();
  void print_chromatic_number();

  bool graph_color_util(int m, int v);
  bool is_safe_to_color(int v, int c);
  void add_colors_drawing();
  bool rlf_coloring();
};