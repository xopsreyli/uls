#include "../inc/uls.h"

void solve(t_data *data) {
    if (data->wrong_paths) {
        solve_wrong_paths(data->wrong_paths);
    }

    if (data->files) {
        solve_files(data->files, data->amount_of_files, data->flag);
        if (data->amount_of_files > 0 && data->amount_of_dirs > 0) {
            mx_printchar('\n');
        }
    }

    for (int i = 0; data->directories[i]; i++) {
        solve_dir(data->directories[i], data->amount, data->flag);
        if (data->directories[i + 1]) {
            mx_printchar('\n');
        }
    }
}

void solve_wrong_paths(char **paths) {
    for (int i = 0; paths[i]; i++) {
        throw_wrong_path(paths[i]);
    }
}

void solve_files(char **files, int size, char flag) {
    t_dir *dir = build_t_dir(size);
    dir->dir_name = mx_strdup(".");

    for (int i = 0; i < size; i++) {
        dir->items[i] = mx_strdup(files[i]);
    }

    if (flag == 'l') {
        for (int i = 0; i < size; i++) {
            get_path_info_l(dir, i);
        }
        print_uls_l_result(dir, size, 0);
    } else {
        print_uls_result(dir->items, size);
    }

    delete_t_dir(dir);
}

void solve_dir(char *dir_name, int amount, char flag) {
    if (amount > 1) {
        mx_printstr(mx_strjoin(dir_name, ":\n"));
    }

    if (is_able_to_read(create_full_path(".", dir_name)) == 0) {
        throw_permission_denied(dir_name);
        return;
    }

    int size = count_items_in_dir(dir_name);
    if (size > 0) {
        t_dir *dir = build_t_dir(size);

        dir->dir_name = mx_strdup(dir_name);
        get_items_from_dir(dir);
        quicksort(dir->items, 0, size - 1);

        if (flag == 'l') {
            for (int i = 0; i < size; i++) {
                get_path_info_l(dir, i);
            }
            print_uls_l_result(dir, size, 1);
        } else {
            print_uls_result(dir->items, size);
        }

        delete_t_dir(dir);
    }
}

void get_path_info_l(t_dir *dir, int index) {
    struct stat path_stat;

    if (lstat(create_full_path(dir->dir_name, dir->items[index]), &path_stat) == 0) {
        dir->total += path_stat.st_blocks;
        dir->permissions[index] = get_path_permissions(&path_stat, create_full_path(dir->dir_name, dir->items[index]));
        dir->links[index] = path_stat.st_nlink;

        struct passwd *owner_info = getpwuid(path_stat.st_uid);
        dir->owners[index] = mx_strdup(owner_info->pw_name);

        struct group *group_info = getgrgid(path_stat.st_gid);
        dir->groups[index] = mx_strdup(group_info->gr_name);

        dir->size[index] = path_stat.st_size;

        char **path_datetime = mx_strsplit(ctime(&path_stat.st_mtime), ' ');
        dir->months[index] = mx_strdup(path_datetime[1]);
        dir->dates[index] = mx_strdup(path_datetime[2]);

        time_t current_time = time(NULL);
        if (current_time - path_stat.st_mtime > 6 * 30 * 24 * 60 * 60) {
            dir->time[index] = mx_strndup(path_datetime[4], 4);
        } else {
            dir->time[index] = mx_strndup(path_datetime[3], 5);
        }

        mx_del_strarr(&path_datetime);

        if (dir->permissions[index][0] == 'l') {
            char link_target[1024];
            ssize_t len = readlink(create_full_path(dir->dir_name, dir->items[index]), link_target, sizeof(link_target) - 1);

            if (len != -1) {
                link_target[len] = '\0';
                char* new_item = mx_strjoin(mx_strjoin(dir->items[index], " -> "), link_target);
                free(dir->items[index]);
                dir->items[index] = new_item;
            }
        }
    }
}
