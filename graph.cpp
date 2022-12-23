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

// вывод матрицы смежности и списка смежности
void Graph::debug() {
  // вывод матрицы смежности
  cout << "Matrix:" << endl;
  for (int i = 0; i < vertex_count; ++i) {
    for (int j = 0; j < vertex_count; ++j) {
      cout << graph_matrix[i][j] << " ";
    }
    cout << endl;
  }

  // вывод списка смежности
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

// псевдокод алгоритма:
// 1. Перебрать все возможные раскраски графа
// 2. Проверить текущую раскраску на корректность
// 3. Если текущая раскраска корректна, то проверить ее на лучшую
// 4. Перейти к следующей раскраске
// 5. Если все раскраски перебраны, то завершить работу алгоритма
// 6. Записать раскраску в массив
// 7. Установить хроматическое число
// 8. Вывести наилучшую раскраску

// пояснение к алгоритму:
// Метод graph_coloring() реализует переборный алгоритм раскраски графа. Он
// перебирает все возможные раскраски графа, проверяет их на корректность (чтобы
// соседние вершины имели разные цвета) и выбирает наилучшую из них (наилучшая
// раскраска - это раскраска с минимальным хроматическим числом, то есть
// минимальным количеством цветов).

void Graph::graph_coloring() {
  // массив для хранения текущей раскраски
  vector<int> current_color(vertex_count, 0);

  // массив для хранения наилучшей раскраски
  vector<int> best_color(vertex_count, 0);

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
        // поиск максимального цвета в раскраске
        if (current_color[i] > current_chromatic_num) {
          current_chromatic_num = current_color[i];
        }
      }
      // если текущая раскраска лучше, чем наилучшая, то заменяем ее
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
    // если все раскраски перебраны, то завершаем работу алгоритма
    if (i == vertex_count) break;
    ++current_color[i];
  }

  // вывод наилучшей раскраски
  cout << "Best coloring:" << endl;
  for (int i = 0; i < vertex_count; ++i) {
    cout << "Edge " << i << ": color ";
    cout << best_color[i] << endl;
  }

  // записываем раскраску в массив
  color = best_color;

  // устанавливаем хроматическое число
  chromatic_num++;  // на 1 больше т.к. цвета нумеруются с 0

  // вывод хроматического числа
  cout << "Chromatic number: " << chromatic_num << endl;
}

// жадный алгоритм раскраски графа
// не гарантирует нахождение наилучшей раскраски, но работает быстрее

// псевдокод алгоритма:
// 1. Перебираем все вершины графа
// 2. Для каждой вершины перебираем все ее соседей
// 3. Для каждого соседа проверяем, какой у него цвет
// 4. Если у соседа цвета нет, то мы его выбираем
// 5. Если у соседа цвет есть, то мы помечаем его как использованный
// 6. Если все цвета использованы, то мы выбираем следующий неиспользованный
// 7. Повторяем шаги 2-6 для всех вершин
// 8. Записываем полученную раскраску в массив
// 9. Ищем хроматическое число
// 10. Выводим результат

// пояснение к алгоритму:
// Жадный алгоритм покраски графа - это алгоритм, который пытается раскрасить
// граф так, чтобы не существовало ребер, соединяющих вершины одного цвета,
// используя минимальное число цветов. Жадный алгоритм отличается от переборного
// алгоритма тем, что он не пытается перебрать все возможные варианты раскраски,
// а выбирает оптимальный вариант на каждом шаге. В результате этого жадный
// алгоритм работает быстрее переборного, но не всегда может найти оптимальное
// решение. Переборный алгоритм, напротив, пытается перебрать все возможные
// варианты раскраски и выбрать из них оптимальный.Это дает гарантированно
// оптимальное решение, но такой алгоритм может работать медленнее жадного,
// особенно для больших графов.

void Graph::greedy_coloring() {
  // массив, в котором будет храниться текущая раскраска графа
  vector<int> current_color(vertex_count, 0);

  // массив, в котором будет храниться минимальный цвет, который еще не
  // использовался
  int min_unused_color = 1;

  // перебор всех вершин графа
  for (int i = 0; i < vertex_count; ++i) {
    // массив, в котором будет храниться информация о том, какие цвета
    // используются у соседей вершины i
    vector<bool> used_color(chromatic_num + 1, false);

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
  cout << "Current coloring:" << endl;
  for (int i = 0; i < vertex_count; ++i) {
    cout << "Edge " << i << ": color ";
    cout << current_color[i] << endl;
  }

  // записываем раскраску в массив
  color = current_color;

  // ищем хроматическое число
  chromatic_num = 0;
  for (int i = 0; i < vertex_count; ++i) {
    chromatic_num = max(chromatic_num, color[i]);
  }
  chromatic_num++;  // на 1 больше т.к. цвета нумеруются с 0

  cout << "Chromatic number: " << chromatic_num << endl;
}

// экспорт в файл .dot (для отрисовки графа в graphviz)

void Graph::export_graph(const string& filename) {
  ofstream output(filename);
  // выводим заголовок
  output << "graph G {" << endl;
  output << "\tnode [shape=circle style=filled]" << endl;
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
  output << "}" << endl;
}