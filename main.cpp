#include <cstring>

#include "graph.h"

using namespace std;

// формат аргументов командной строки:
// ./graph_painting input_file output_file [-b/-g]
// ./graph_painting input.txt output.txt bruteforce

// -b (bruteforce) - перебор всех цветов
// -g (greedy) - жадный алгоритм

int main(int argc, char* argv[]) {
  // проверка на количество аргументов
  if (argc != 4) {
    cout << "Wrong number of arguments" << endl;
    return 1;
  }

  // проверка на существование входного файла
  ifstream input(argv[1]);
  if (!input.is_open()) {
    cout << "File not found" << endl;
    return 1;
  }
  input.close();

  // создание графа
  Graph test_graph(argv[1]);

  cout << "Coloring of graph" << endl;

  // проверка на правильность аргумента
  // и вызов соответствующего метода
  if (!strcmp(argv[3], "-b")) {
    test_graph.graph_coloring();
  } else if (!strcmp(argv[3], "-g")) {
    test_graph.greedy_coloring();
  } else {
    cout << "Wrong argument" << endl;
    return 1;
  }

  // экспорт графа в файл graphviz
  test_graph.export_graph(argv[2]);

  return 0;
}