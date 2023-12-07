#ifndef OBOLTENKOV_ULS_H
#define OBOLTENKOV_ULS_H

#include "../libmx/inc/libmx.h"
#include "dirent.h"
#include "sys/ioctl.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "pwd.h"
#include "grp.h"
#include "time.h"
#include "sys/xattr.h"
#include "sys/acl.h"

typedef struct s_data {
    char flag;
    char** wrong_paths;
    char** files;
    char** directories;
    int amount;
    int amount_of_files;
    int amount_of_dirs;
} t_data;

typedef struct s_dir {
    char* dir_name;
    char** items;
    int total;
    char** permissions;
    int* links;
    char** owners;
    char** groups;
    int* size;
    char** months;
    char** dates;
    char** time;
} t_dir;

//parser
void parse(int argc, char **argv, t_data* data);

//errors
void print_usage();
void throw_wrong_path(const char* path);
void throw_wrong_flag(char* flag);
void throw_permission_denied(char* path);

// extra functions
int is_hidden(const char *name);
int count_items_in_dir(char* dir_name);
void get_items_from_dir(t_dir *dir);
void quicksort(char **arr, int left, int right);
int get_longest_length(char** arr, int size);
int get_terminal_width();
int get_number_of_columns(int terminal_width, int longest_item_width);
int is_directory(const char *path);
int is_file(const char* path);
int has_flag(char* first_arg);
int is_flag_valid(char* flag);
char** create_char_array(int size);
void sort_params(char** argv, int size, int index, t_data* data);
int get_length_of_strarr(char** array);
t_dir* build_t_dir(int size);
void delete_t_dir(t_dir* dir);
char* create_full_path(char* dir_name, char* item_name);
char* get_path_permissions(const struct stat* path_stat, char* full_path);
char get_path_type(const struct stat* path_stat);
char get_extra_attr(char* full_path);
int is_able_to_read(char* path);
void delete_t_data(t_data* data);

// resolver
void solve(t_data* data);
void solve_wrong_paths(char** paths);
void solve_files(char** files, int amount, char flag);
void solve_dir(char* dir_name, int amount, char flag);
void get_path_info_l(t_dir *dir, int index);

// output
void print_uls_result(char** items, int size);
void print_uls_l_result(t_dir* dir, int size, int with_total);

#endif //OBOLTENKOV_ULS_H
