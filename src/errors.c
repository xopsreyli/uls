#include "../inc/uls.h"

void print_usage() {
    mx_printstr("usage: uls [-l] [file ...]");
    mx_printchar('\n');

    exit(1);
}

void throw_wrong_flag(char* flag) {
    mx_printstr("uls: illegal option -- ");
    mx_printstr(flag);
    mx_printchar('\n');

    print_usage();
}

void throw_wrong_path(const char* path) {
    mx_printstr("uls: ");
    mx_printstr(path);
    mx_printstr(": No such file or directory");
    mx_printchar('\n');
}

void throw_permission_denied(char* path) {
    mx_printstr("uls: ");
    int words = mx_count_words(path, '/');
    if (words > 1) {
        char** splited_words = mx_strsplit(path, '/');
        mx_printstr(splited_words[words - 1]);
        mx_del_strarr(&splited_words);
    } else {
        mx_printstr(path);
    }
    mx_printstr(": Permission denied");
    mx_printchar('\n');
}
