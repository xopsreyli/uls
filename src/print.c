#include "../inc/uls.h"

void print_uls_result(char** items, int size) {
    if (isatty(1)) {
        int terminal_width = get_terminal_width();
        int longest_length = get_longest_length(items, size);
        int columns = get_number_of_columns(terminal_width, longest_length);
        int rows = (size + columns - 1) / columns;

        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < columns; col++) {
                int index = col * rows + row;

                if (index < size) {
                    mx_printstr(items[index]);

                    int rest = longest_length - mx_strlen(items[index]);
                    for (int i = 0; i < rest; i++) {
                        write(1, " ", 1);
                    }

                    mx_printchar('\t');
                }
            }

            mx_printchar('\n');
        }
    } else {
        for (int i = 0; i < size; i++) {
            mx_printstr(items[i]);
            mx_printchar('\n');
        }
    }
}

void print_uls_l_result(t_dir* dir, int size, int with_total) {
    if (with_total == 1) {
        mx_printstr(mx_strjoin("total ", mx_itoa(dir->total)));
        mx_printchar('\n');
    }

    int longest_link = 0;
    for (int i = 0; i < size; i++) {
        if (longest_link < mx_strlen(mx_itoa(dir->links[i]))) {
            longest_link = mx_strlen(mx_itoa(dir->links[i]));
        }
    }

    int longest_owner = 0;
    for (int i = 0; i < size; i++) {
        if (longest_owner < mx_strlen(dir->owners[i])) {
            longest_owner = mx_strlen(dir->owners[i]);
        }
    }

    int longest_group = 0;
    for (int i = 0; i < size; i++) {
        if (longest_group < mx_strlen(dir->groups[i])) {
            longest_group = mx_strlen(dir->groups[i]);
        }
    }

    int longest_size = 0;
    for (int i = 0; i < size; i++) {
        if (longest_size < mx_strlen(mx_itoa(dir->size[i]))) {
            longest_size = mx_strlen(mx_itoa(dir->size[i]));
        }
    }

    int longest_date = 0;
    for (int i = 0; i < size; i++) {
        if (longest_date < mx_strlen(dir->dates[i])) {
            longest_date = mx_strlen(dir->dates[i]);
        }
    }

    int longest_time = 0;
    for (int i = 0; i < size; i++) {
        if (longest_time < mx_strlen(dir->time[i])) {
            longest_time = mx_strlen(dir->time[i]);
        }
    }

    for (int i = 0; i < size; i++) {
        mx_printstr(dir->permissions[i]);
        mx_printstr(" ");

        for (int j = 0; j < longest_link - mx_strlen(mx_itoa(dir->links[i])); j++) {
            mx_printchar(' ');
        }
        mx_printint(dir->links[i]);
        mx_printstr(" ");

        for (int j = 0; j < longest_owner - mx_strlen(dir->owners[i]); j++) {
            mx_printchar(' ');
        }
        mx_printstr(dir->owners[i]);
        mx_printstr("  ");

        for (int j = 0; j < longest_group - mx_strlen(dir->groups[i]); j++) {
            mx_printchar(' ');
        }
        mx_printstr(dir->groups[i]);
        mx_printstr("  ");

        for (int j = 0; j < longest_size - mx_strlen(mx_itoa(dir->size[i])); j++) {
            mx_printchar(' ');
        }
        mx_printint(dir->size[i]);
        mx_printstr(" ");

        mx_printstr(dir->months[i]);
        mx_printstr(" ");

        for (int j = 0; j < longest_date - mx_strlen(dir->dates[i]); j++) {
            mx_printchar(' ');
        }
        mx_printstr(dir->dates[i]);
        mx_printstr(" ");

        if (longest_time > mx_strlen(dir->time[i])) {
            mx_printchar(' ');
        }
        mx_printstr(dir->time[i]);
        mx_printstr(" ");

        mx_printstr(dir->items[i]);

        mx_printchar('\n');
    }
}
