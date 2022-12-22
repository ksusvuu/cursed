#include "graph.h"

// конструктор
Graph::Graph(const std::string& matrix_filename) {
  std::ifstream input(matrix_filename);  // открытие файла
  std::string s;  // строка для считывания
  // считывание размера матрицы смежности
  getline(input, s);  // считывание первой строки
  this->vertex_num = std::stoi(s);  // преобразование строки в число
  adjacency = new std::list<int>[vertex_num];  // создание списка смежности
  this->chromatic_num = this->vertex_num;  // начальное количество цветов

  int row = 0;            // номер строки
  if (input.is_open()) {  // проверка на открытие файла
    while (getline(input, s)) {  // считывание построчно
      std::vector<int> row_vector;  // вектор для хранения строки
      for (int col = 0; col < s.size(); col += 2) {  // считывание
        // добавление в вектор элемента строки
        row_vector.push_back(std::stoi(s.substr(col, 1)));
        if (std::stoi(s.substr(col, 1)) == 1) {  // если есть ребро
          if (col >= row * 2) {
            // если ребро не ведет к вершине, которая уже была рассмотрена
            // то добавление в список смежности
            this->adjacency[row].push_back(col / 2);
            this->adjacency[col / 2].push_back(row);
          }
        }
      }
      row++;  // переход к следующей строке
      this->graph_matrix.push_back(row_vector);  // добавление строки в матрицу
    }
    input.close();  // закрытие файла
  }
};

// Проверка на двудольность графа
bool Graph::graph_color_util(int m, int v) {
  if (v == vertex_num) return true;  // если все вершины раскрашены

  for (int i = 1; i <= m; i++) {  // цикл по всем цветам
    // Проверка, можно ли присвоить цвет i этой вершине
    if (is_safe_to_color(v, i)) {
      // присвоение цвета
      color[v] = i;
      // рекурсия для проверки следующих вершин
      if (graph_color_util(m, v + 1)) return true;
      // если цвет не приводит к решению
      color[v] = 0;
    }
  }
  // если цвет не приводит к решению
  return false;  // не удалось раскрасить граф
}

// Проверка, можно ли присвоить цвет i этой вершине
bool Graph::is_safe_to_color(int v, int c) {
  for (int i = 0; i < vertex_num; i++) {  // проверка всех вершин
    // если есть ребро и цвета совпадают
    if (this->graph_matrix[v][i] == 1 && c == color[i]) {
      return false;  // цвет не подходит
    }
  }
  // цвет подходит
  return true;
};

// функция для добавления цветов в вектор
void Graph::add_colors_drawing() {
  for (int i = 0; i < vertex_num; i++) {
    this->colored_vertices.push_back(color[i]);
  }
};

// возвращает хроматическое число (максимальное число цветов)
int Graph::chromatic_number() { return this->chromatic_num; };

// поиск хроматического числа (максимального числа цветов)
void Graph::find_chromatic_number() {
  std::vector<int> result(vertex_num);
  // Присваиваем первый цвет первой вершине
  result[0] = 0;
  // Инициализируем оставшиеся вершины vertex_num-1 как не присвоенные
  for (int u = 1; u < vertex_num; u++) {
    result[u] = -1;
    // Для u не присвоено ни одного цвета
  }
  // Временный массив для хранения доступных цветов. True
  // значение available[cr] будет означать, что цвет cr
  // присваивается одной из его смежных вершин
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
    for (cr = 0; cr < vertex_num; cr++) {  // проверка всех цветов
      if (available[cr] == false) {        // если цвет не занят
        break;  // присваиваем этот цвет
      }
    }
    result[u] = cr;  // Присваиваем найденный цвет
  }
  int max = 0;  // максимальное число цветов
  for (int i = 0; i < this->vertex_num; i++) {  // проверка всех вершин
    if (result[i] > max) {  // если число цветов больше максимального
      max = result[i];  // присваиваем максимальному
    }
  }

  // присваиваем хроматическому числу максимальное число цветов
  this->chromatic_num = max;
};

// Проверка, можно ли раскрасить граф
bool Graph::rlf_coloring() {
  // Обозначаем все значения цвета как 0
  int m = this->chromatic_num;
  for (int i = 0; i < this->vertex_num; i++) {
    color.push_back(0);
  }

  // graph_color_util() для вершины 0
  if (!graph_color_util(m, 0)) {
    // Если раскрасить невозможно
    std::cout << "Color scheme is not possible" << std::endl;
    return false;
  }

  // Выводим раскраску
  add_colors_drawing();
  return true;  // раскраска возможна
}

// выводим матрицу смежности
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
  std::vector<int> result(vertex_num);
  // Присваиваем первый цвет первой вершине
  result[0] = 0;
  // Назначаем цвета оставшимся вершинам vertex_num-1
  for (int u = 1; u < vertex_num; u++) {
    result[u] = -1;
    // Никакой цвет не присвоен u
  }
  // Временный массив для хранения доступных цветов. True
  // значение available[cr] будет означать, что цвет cr
  // присваивается одной из его смежных вершин
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

    result[u] = cr;  // Присваиваем найденный цвет
    // Возвращаем значения обратно к false для следующей итерации
    for (i = adjacency[u].begin(); i != adjacency[u].end(); ++i) {
      if (result[*i] != -1) {
        available[result[*i]] = false;
      }
    }
  }

  for (int u = 0; u < vertex_num; u++) {
    this->colored_vertices.push_back(result[u]);
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
          << " [color = " << this->colors[this->colored_vertices[u]]
          << std::uppercase << " label=\"" << u << "\"]\n";
    }

    for (int u = 0; u < vertex_num; u++) {
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
