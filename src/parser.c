#include "../inc/uls.h"

void parse(int argc, char **argv, t_data *data) {
    int is_with_flag = has_flag(argv[1]);
    if (is_with_flag == 1 && is_flag_valid(argv[1]) == 1) {
        data->flag = argv[1][1];
    }

    int number_of_args = argc - 1 - is_with_flag;
    if (number_of_args > 0) {
        data->wrong_paths = create_char_array(number_of_args);
        data->files = create_char_array(number_of_args);
        data->directories = create_char_array(number_of_args);

        quicksort(argv, argc - number_of_args, argc - 1);
        sort_params(argv, argc, argc - number_of_args, data);
        data->amount = number_of_args;
    } else {
        data->directories = create_char_array(1);
        data->directories[0] = mx_strdup(".");
        data->amount = 1;
    }
}
