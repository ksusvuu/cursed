#include "Graph_painting.h"

using namespace std;

// parse command line arguments
// format: ./graph_painting size input_file output_file
// size - размер матрицы смежности

int main(int argc, char* argv[]) {
  // проверка на количество аргументов
  if (argc != 4) {
    cout << "Wrong number of arguments" << endl;
    return 1;
  }

  // проверка на существование входного файла
  ifstream input(argv[2]);
  if (!input.is_open()) {
    cout << "File not found" << endl;
    return 1;
  }
  input.close();

  // проверка на существование выходного файла
  ofstream output(argv[3]);
  if (!output.is_open()) {
    cout << "File not found" << endl;
    return 1;
  }
  output.close();

  // создание графа
  Graph test_graph(std::atoi(argv[1]));
  test_graph.set_matrix(argv[2]);
  cout << "Coloring of graph" << endl;

  // поиск минимального количества цветов
  test_graph.greedy_coloring();
  test_graph.chromatic_number();
  test_graph.print_matrix();
  test_graph.draw_graph(argv[3]);

  test_graph.print_chromatic_number();

  return 0;
}