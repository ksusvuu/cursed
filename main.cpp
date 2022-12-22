#include "graph.h"

using namespace std;

// формат аргументов командной строки:
// ./graph_painting input_file output_file

int main(int argc, char* argv[]) {
  // проверка на количество аргументов
  if (argc != 3) {
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
  test_graph.print_matrix();

  // поиск минимального количества цветов
  test_graph.greedy_coloring();
  test_graph.find_chromatic_number();
  cout << "Chromatic number: " << test_graph.chromatic_number() << endl;

  test_graph.export_graph(argv[2]);

  return 0;
}