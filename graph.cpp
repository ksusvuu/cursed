#include "graph.h"

using namespace std;

// конструктор класса Graph
// считывание матрицы смежности из файла
// конвертация в список смежности
// также инициализируется вектор цветов и хроматическое число

Graph::Graph(const string& filename) {
  // считывание матрицы смежности из файла
  ifstream input(filename);
  // проверка на существование входного файла
  if (!input.is_open()) {
    cout << "File not found" << endl;
    throw invalid_argument("File " + filename + " not found");
  }

  input >> vertex_count;  // считывание количества вершин
  graph_matrix.resize(vertex_count);  // инициализация матрицы смежности (такого
                                      // же размера как и количество вершин)
  for (int i = 0; i < vertex_count; ++i) {  // считывание матрицы смежности
    graph_matrix[i].resize(vertex_count);  // одна строка матрицы смежности
    for (int j = 0; j < vertex_count; ++j) {  // считывание всех чисел в строке
      input >> graph_matrix[i][j];  // считывание элемента матрицы смежности
    }
  }
  input.close();

  // конвертация матрицы смежности в список смежности
  graph_list.resize(vertex_count);
  for (int i = 0; i < vertex_count; ++i) {  // проходит по всем вершинам
    for (int j = 0; j < vertex_count; ++j) {
      if (graph_matrix[i][j] ==
          1) {  // если вершины смежны (если существует ребро)
        graph_list[i].push_back(j);  // добавление вершины в список смежности
      }
    }
  }

  // инициализация массива цветов вершин
  color.resize(vertex_count, 0);

  // инициализация хроматического числа
  // хроматическое число графа не может быть больше количества вершин
  chromatic_num = vertex_count;
}

void Graph::debug() {
  // вывод матрицы смежности и списка смежности
  cout << "Matrix:" << endl;
  for (int i = 0; i < vertex_count; ++i) {
    for (int j = 0; j < vertex_count; ++j) {
      cout << graph_matrix[i][j] << " ";
    }
    cout << endl;
  }

  cout << endl << "List:" << endl;
  for (int i = 0; i < vertex_count; ++i) {
    cout << i << ": ";
    for (int j = 0; j < graph_list[i].size(); ++j) {
      cout << graph_list[i][j] << " ";
    }
    cout << endl;
  }
}

// переборный алгоритм раскраски графа
// перебор всех возможных раскрасок графа и выбор наилучшей из них
// (наилучшая раскраска - это раскраска с минимальным хроматическим числом)

void Graph::graph_coloring() {
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
      if (!is_correct) break;
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
      current_color[i++] = 0;
    }
    if (i == vertex_count) break;
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
// не гарантирует нахождение наилучшей раскраски, но работает быстрее

void Graph::greedy_coloring() {
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
    while (color < used_color.size() && used_color[color]) ++color;

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

void Graph::export_graph(const std::string& filename) {
  std::ofstream output(filename);
  output << "graph G {" << std::endl;
  output << "\tnode [shape=circle style=filled]" << std::endl;
  // выводим цвета вершин
  for (int i = 0; i < vertex_count; ++i) {
    output << '\t' << i;
    output << " [color=\"" << rainbow[color[i]] << "\"]\n";
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