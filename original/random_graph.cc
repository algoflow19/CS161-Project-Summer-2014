#include <assert.h>
#include <cstdlib>

/*
 * Main takes three arguments:
 * 1). number of nodes
 * 2). Number of edges
 * 3). output file
 */
int
main(int argc, char **argv){
  assert(argc == 4);
  unsigned n = strtoul(argv[1], NULL, 0);
  unsigned m = strtoul(argv[2], NULL, 0);
  char *output_file = argv[3];
  return 0;
}
