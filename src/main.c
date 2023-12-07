#include "../inc/uls.h"

int main(int argc, char **argv) {
    t_data *data = (t_data *) malloc(sizeof(t_data));

    parse(argc, argv, data);
    solve(data);

    delete_t_data(data);

    return 0;
}
