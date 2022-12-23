#include <cstring>

#include "graph.h"

using namespace std;

// формат аргументов командной строки:
// ./graph_painting input_file output_file [bruteforce/greedy]
// ./graph_painting input.txt output.txt bruteforce

// bruteforce - перебор всех цветов
// greedy - жадный алгоритм

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

  Graph test_graph(argv[1]);

  cout << "Coloring of graph" << endl;

  // проверка на правильность аргумента
  if (strcmp(argv[3], "bruteforce") == 0) {
    test_graph.graph_coloring();
  } else if (strcmp(argv[3], "greedy") == 0) {
    test_graph.greedy_coloring();
  } else {
    cout << "Wrong argument" << endl;
    return 1;
  }

  test_graph.export_graph(argv[2]);

  return 0;
}