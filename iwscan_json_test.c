#include "iwscan_json.h"

int main(int argc, char **argv) {
  char *ifname;
  char *fp;
  int ERR;
  if (argc == 1) {
    ERR = EINVAL;
    exit(ERR);
  } else if (argc == 2) {
    ifname = argv[1];
    fp = "jscn_rslt.json";
  } else if (argc == 3) {
    ifname = argv[1];
    fp = argv[2];

  } else if (argc > 3) {
    ERR = E2BIG;
    exit(ERR);
  }
  cJSON *out = jscn_iwscan_json(ifname);
  int r = jscn_print_file(fp, out);
  if (r != 0) {
    ERR = errno;
    exit(ERR);
  } else {
    return 0;
  }
}
