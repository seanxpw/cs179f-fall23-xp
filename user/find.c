/* find all the files in a directory tree with a specific name.*/

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *
only_name(char *path)
{
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    return p;
}

int find(char *dir_path, char *file_name)
{

    static int flag_check_dir = 1;
    static int flag_found = 0;
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(dir_path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", dir_path);
        return 0;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", dir_path);
        close(fd);
        return 0;
    }
    // memmove(buf,dir_path,strlen(dir_path));
    // printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    switch (st.type)
    {
    case T_FILE:
        if (flag_check_dir)
        {
            printf("The first argument %s should be a directory!\n", dir_path);
            exit();
        }
        // for now, the dir_path is not a dir, but a file
        // We're going to compare the names.
        if (strcmp(only_name(dir_path), file_name) == 0)
        {
            // printf("Found!\n");
            printf("%s\n", dir_path);
            flag_found += 1;
            return flag_found;
        }
        break;

    case T_DIR:
        flag_check_dir = 0;
        if (strlen(dir_path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("ls: path too long\n");
            break;
        }
        strcpy(buf, dir_path);
        p = buf + strlen(buf);
        *p++ = '/';
        // I think there's no \0 in the end
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0; // set '\0'
            if (stat(buf, &st) < 0)
            {
                printf("ls: cannot stat %s\n", buf);
                continue;
            }
            // printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
            find(buf, file_name);
        }
        break;
    }
    close(fd);
    return flag_found;
}

int main(int argc, char *argv[])
// argv[1] should be the dir, argv[2] is the file
{
    // int i;
    int find_count;

    if (argc != 3)
    {
        printf("Error: not the right arg number.\n");
        exit();
    }
    // printf("first arg: %s, second: %s\n",argv[1],argv[2]);
    find_count = find(argv[1], argv[2]);
    if (find_count == 0)
        printf("Found nothing.\n");
    exit();
}
