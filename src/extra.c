#include "../inc/uls.h"

int is_hidden(const char *name) {
    return name[0] == '.';
}

int count_items_in_dir(char *dir_name) {
    DIR *dir = opendir(dir_name);
    int size = 0;
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (is_hidden(entry->d_name) == 0) {
            size++;
        }
    }

    closedir(dir);

    return size;
}

void get_items_from_dir(t_dir* dir) {
    DIR *directory = opendir(create_full_path(".", dir->dir_name));
    struct dirent *entry;
    int i = 0;

    while ((entry = readdir(directory)) != NULL) {
        if (is_hidden(entry->d_name) == 0) {
            dir->items[i++] = mx_strdup(entry->d_name);
        }
    }
    closedir(directory);
}

void quicksort(char **arr, int left, int right) {
    if (left < right) {
        char *pivot = arr[right];
        int i = left - 1;

        for (int j = left; j < right; j++) {
            if (mx_strcmp(arr[j], pivot) < 0) {
                i++;
                char *temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }

        char *temp = arr[i + 1];
        arr[i + 1] = arr[right];
        arr[right] = temp;

        quicksort(arr, left, i);
        quicksort(arr, i + 1, right);
    }
}

int get_longest_length(char **arr, int size) {
    int result = 0;

    for (int i = 0; i < size; i++) {
        if (mx_strlen(arr[i]) > result) {
            result = mx_strlen(arr[i]);
        }
    }

    return result;
}

int get_terminal_width() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

int get_number_of_columns(int terminal_width, int longest_item_width) {
    return terminal_width / (longest_item_width + 8);
}

int is_directory(const char *path) {
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        return 0;
    }

    return S_ISDIR(path_stat.st_mode);
}

int is_file(const char *path) {
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        return 0;
    }

    return S_ISREG(path_stat.st_mode);
}

int has_flag(char *first_arg) {
    if (!first_arg) {
        return 0;
    }

    if (first_arg[0] == '-') {
        return 1;
    }

    return 0;
}

int is_flag_valid(char *flag) {
    if (mx_strlen(flag) == 2) {
        if (flag[1] == 'l') {
            return 1;
        }
    }

    throw_wrong_flag(flag + 1);

    return 0;
}

char **create_char_array(int size) {
    char **array = (char **) malloc((size + 1) * sizeof(char *));

    for (int i = 0; i < size + 1; i++) {
        array[i] = NULL;
    }

    return array;
}

void sort_params(char **argv, int size, int index, t_data *data) {
    int w = 0, f = 0, d = 0;

    for (int i = index; i < size; i++) {
        if (is_directory(create_full_path(".", argv[i])) != 0) {
            data->directories[d++] = mx_strdup(argv[i]);
        } else if (is_file(create_full_path(".", argv[i])) != 0) {
            data->files[f++] = mx_strdup(argv[i]);
        } else {
            data->wrong_paths[w++] = mx_strdup(argv[i]);
        }
    }

    data->amount_of_files = f;
    data->amount_of_dirs = d;
}

int get_length_of_strarr(char **array) {
    if (!array) {
        return 0;
    }

    int length = 0;

    while (array[length]) {
        length++;
    }

    return length;
}

t_dir *build_t_dir(int size) {
    t_dir *dir = (t_dir *) malloc(sizeof(t_dir));

    dir->total = 0;
    dir->items = create_char_array(size);
    dir->permissions = create_char_array(size);
    dir->links = (int *) malloc(size * sizeof(int));
    dir->owners = create_char_array(size);
    dir->groups = create_char_array(size);
    dir->size = (int *) malloc(size * sizeof(int));
    dir->months = create_char_array(size);
    dir->dates = create_char_array(size);
    dir->time = create_char_array(size);

    return dir;
}

void delete_t_dir(t_dir *dir) {
    free(dir->dir_name);
    mx_del_strarr(&dir->items);
    mx_del_strarr(&dir->permissions);
    free(dir->links);
    mx_del_strarr(&dir->owners);
    mx_del_strarr(&dir->groups);
    free(dir->size);
    mx_del_strarr(&dir->months);
    mx_del_strarr(&dir->dates);
    mx_del_strarr(&dir->time);

    free(dir);
}

char *create_full_path(char *dir_name, char *item_name) {
    int length_dir_name = mx_strlen(dir_name);
    int length_item_name = mx_strlen(item_name);

    char *result = (char *) malloc(length_dir_name + length_item_name + 2);

    mx_strcpy(result, dir_name);
    result[length_dir_name] = '/';
    mx_strcpy(result + length_dir_name + 1, item_name);

    return result;
}

char *get_path_permissions(const struct stat *path_stat, char* full_path) {
    char *result = mx_strnew(12);

    result[0] = get_path_type(path_stat);
    result[1] = path_stat->st_mode & S_IRUSR ? 'r' : '-';
    result[2] = path_stat->st_mode & S_IWUSR ? 'w' : '-';
    result[3] = path_stat->st_mode & S_IXUSR ? 'x' : '-';
    result[4] = path_stat->st_mode & S_IRGRP ? 'r' : '-';
    result[5] = path_stat->st_mode & S_IWGRP ? 'w' : '-';
    result[6] = path_stat->st_mode & S_IXGRP ? 'x' : '-';
    result[7] = path_stat->st_mode & S_IROTH ? 'r' : '-';
    result[8] = path_stat->st_mode & S_IWOTH ? 'w' : '-';
    result[9] = path_stat->st_mode & S_IXOTH ? 'x' : '-';
    result[10] = get_extra_attr(full_path);
    result[11] = '\0';

    return result;
}

char get_extra_attr(char* full_path) {
    if (listxattr(full_path, NULL, 0, 0x0001) > 0) {
        return '@';
    } else if (acl_get_file(full_path, ACL_TYPE_EXTENDED)) {
        return '+';
    }

    return ' ';
}

char get_path_type(const struct stat *path_stat) {
    if (S_ISREG(path_stat->st_mode)) {
        return '-';
    } else if (S_ISDIR(path_stat->st_mode)) {
        return 'd';
    } else if (S_ISCHR(path_stat->st_mode)) {
        return 'c';
    } else if (S_ISBLK(path_stat->st_mode)) {
        return 'b';
    } else if (S_ISFIFO(path_stat->st_mode)) {
        return 'p';
    } else if (S_ISLNK(path_stat->st_mode)) {
        return 'l';
    } else if (S_ISSOCK(path_stat->st_mode)) {
        return 's';
    } else {
        return '?';
    }
}

int is_able_to_read(char* path) {
    struct stat path_stat;

    if (stat(path, &path_stat) == 0) {
        if (path_stat.st_mode & S_IRUSR) {
            return 1;
        }
    }

    return 0;
}

void delete_t_data(t_data* data) {
    mx_del_strarr(&data->wrong_paths);
    mx_del_strarr(&data->files);
    mx_del_strarr(&data->directories);

    free(data);
}
