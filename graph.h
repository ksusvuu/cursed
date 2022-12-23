#pragma once

#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const std::string rainbow[8] = {"#dd71e5", "#3AE6CA", "#ffef6b", "#0c77ff",
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
  Graph(const std::string& matrix_filename) {
    // принимает на вход имя файла с матрицей смежности
    // считывает матрицу смежности из файла
    // конвертирует матрицу смежности в список смежности
    // инициализирует массив цветов вершин
    // инициализирует хроматическое число

    // считывание матрицы смежности из файла
    std::ifstream input(matrix_filename);
    // проверка на существование входного файла
    if (!input.is_open()) {
      std::cout << "File not found" << std::endl;
      throw std::invalid_argument("File " + matrix_filename + " not found");
    }

    input >> vertex_count;
    graph_matrix.resize(vertex_count);
    for (int i = 0; i < vertex_count; ++i) {
      graph_matrix[i].resize(vertex_count);
      for (int j = 0; j < vertex_count; ++j) {
        input >> graph_matrix[i][j];
      }
    }
    input.close();

    // конвертация матрицы смежности в список смежности
    graph_list.resize(vertex_count);
    for (int i = 0; i < vertex_count; ++i) {
      for (int j = 0; j < vertex_count; ++j) {
        if (graph_matrix[i][j] == 1) {
          graph_list[i].push_back(j);
        }
      }
    }

    // инициализация массива цветов вершин
    color.resize(vertex_count, 0);

    // инициализация хроматического числа
    // хроматическое число графа не может быть больше количества вершин
    chromatic_num = vertex_count;

    // вывод матрицы смежности и списка смежности
    std::cout << "Matrix:" << std::endl;
    for (int i = 0; i < vertex_count; ++i) {
      for (int j = 0; j < vertex_count; ++j) {
        std::cout << graph_matrix[i][j] << " ";
      }
      std::cout << std::endl;
    }

    std::cout << "List:" << std::endl;
    for (int i = 0; i < vertex_count; ++i) {
      std::cout << i << ": ";
      for (int j = 0; j < graph_list[i].size(); ++j) {
        std::cout << graph_list[i][j] << " ";
      }
      std::cout << std::endl;
    }
  }

  // переборный алгоритм раскраски графа
  void graph_coloring() {
    // перебор всех возможных раскрасок графа
    // и выбор наилучшей из них
    // наилучшая раскраска - это раскраска с минимальным хроматическим числом

    // массив для хранения текущей раскраски
    std::vector<int> current_color(vertex_count, 0);

    // массив для хранения наилучшей раскраски
    std::vector<int> best_color(vertex_count, 0);

    // перебор всех возможных раскрасок графа
    while (true) {
      // проверка текущей раскраски на корректность
      bool is_correct = true;
      for (int i = 0; i < vertex_count; ++i) {
        for (int j = 0; j < graph_list[i].size(); ++j) {
          if (current_color[i] == current_color[graph_list[i][j]]) {
            is_correct = false;
            break;
          }
        }
        if (!is_correct) {
          break;
        }
      }

      // если текущая раскраска корректна, то проверяем ее на лучшую
      if (is_correct) {
        int current_chromatic_num = 0;
        for (int i = 0; i < vertex_count; ++i) {
          if (current_color[i] > current_chromatic_num) {
            current_chromatic_num = current_color[i];
          }
        }
        if (current_chromatic_num < chromatic_num) {
          chromatic_num = current_chromatic_num;
          best_color = current_color;
        }
      }

      // переход к следующей раскраске
      int i = 0;
      while (i < vertex_count && current_color[i] == chromatic_num) {
        current_color[i] = 0;
        ++i;
      }
      if (i == vertex_count) {
        break;
      }
      ++current_color[i];
    }

    // вывод наилучшей раскраски
    std::cout << "Best coloring:" << std::endl;
    for (int i = 0; i < vertex_count; ++i) {
      std::cout << i << ": " << best_color[i] << std::endl;
    }

    // устанавливаем хроматическое число
    chromatic_num++;

    // вывод хроматического числа
    std::cout << "Chromatic number: " << chromatic_num << std::endl;

    // записываем раскраску в массив
    color = best_color;
  }

  // жадный алгоритм раскраски графа
  // Эта функция реализует жадный алгоритм раскраски графа. Она начинает
  // раскраску с нулевой вершины, и затем перебирает все оставшиеся вершины
  // графа в порядке их номеров.

  // Для каждой вершины мы сначала проверяем, какие цвета используются у ее
  // соседей. Затем мы выбираем тот цвет, который используется минимальное число
  // раз среди соседей этой вершины. Если такого цвета нет, то мы выбираем
  // следующий неиспользованный цвет.

  // После того, как мы окрасили все вершины, мы выводим результат раскраски на
  // экран. Для этого мы перебираем все вершины и выводим их номера и цвета.

  // Жадный алгоритм раскраски графа не гарантирует, что он всегда найдет
  // оптимальное решение, то есть раскрасит граф с минимальным числом цве
  void greedy_coloring() {
    // массив, в котором будет храниться текущая раскраска графа
    std::vector<int> current_color(vertex_count, 0);

    // массив, в котором будет храниться минимальный цвет, который еще не
    // использовался
    int min_unused_color = 1;

    // перебор всех вершин графа
    for (int i = 0; i < vertex_count; ++i) {
      // массив, в котором будет храниться информация о том, какие цвета
      // используются у соседей вершины i
      std::vector<bool> used_color(chromatic_num + 1, false);

      // перебор всех соседей вершины i
      for (int j = 0; j < graph_list[i].size(); ++j) {
        // помечаем цвет, который используется у соседа вершины i
        used_color[current_color[graph_list[i][j]]] = true;
      }

      // ищем минимальный цвет, который еще не использовался
      int color = 0;
      while (color < used_color.size() && used_color[color]) {
        ++color;
      }

      // если такого цвета нет, то мы выбираем следующий неиспользованный цвет
      if (color == used_color.size()) {
        color = min_unused_color;
        ++min_unused_color;
      }

      // окрашиваем вершину i в найденный цвет
      current_color[i] = color;
    }

    // вывод текущей раскраски графа
    std::cout << "Current coloring:" << std::endl;
    for (int i = 0; i < vertex_count; ++i) {
      std::cout << i << ": " << current_color[i] << std::endl;
    }

    // ищем хроматическое число
    chromatic_num = 0;
    for (int i = 0; i < vertex_count; ++i) {
      chromatic_num = std::max(chromatic_num, current_color[i]);
    }
    chromatic_num++;

    std::cout << "Chromatic number: " << chromatic_num << std::endl;

    // записываем раскраску в массив
    color = current_color;
  }

  // экспорт в файл .dot (для отрисовки графа в graphviz)
  void export_graph(const std::string& output_filename) {
    std::ofstream output(output_filename);
    output << "graph G {" << std::endl;
    output << "\tnode [shape=circle style=filled]" << std::endl;
    // выводим цвета вершин
    for (int i = 0; i < vertex_count; ++i) {
      output << '\t' << i;
      output << " [color=\"" << rainbow[color[i]] << "\"]" << std::endl;
    }
    // выводим все ребра графа (до главной диагонали)
    for (int i = 0; i < vertex_count; ++i) {
      for (int j = 0; j < i; ++j) {
        if (graph_matrix[i][j]) {
          output << '\t' << i << " -- " << j << ";\n";
        }
      }
    }
    output << "}" << std::endl;
  }
};