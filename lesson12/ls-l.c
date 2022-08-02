#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

int main(int argc, char * argv[]){
    if(argc < 2){
        printf("%s filename\n", argv[0]);
        return -1;
    }

    struct stat statbuf;

    int ret = stat(argv[1], &statbuf);

    if(ret == -1){
        perror("stat");
        return -1;
    }

    char perms[11] = {0};

    switch(statbuf.st_mode & S_IFMT){
        case S_IFLNK:
            perms[0] = 'l';
            break;
        case S_IFDIR:
            perms[0] = 'd';
            break;
        case S_IFREG:
            perms[0] = '-';
            break;
        case S_IFBLK:
            perms[0] = 'b';
            break;
        case S_IFCHR:
            perms[0] = 'c';
            break;
        case S_IFSOCK:
            perms[0] = 's';
            break;
        case S_IFIFO:
            perms[0] = 'p';
            break;
        default:
            perms[0] = '?';
            break;
    }

    perms[1] = (statbuf.st_mode & S_IRUSR) ? 'r' : '-';
    perms[2] = (statbuf.st_mode & S_IWUSR) ? 'w' : '-';
    perms[3] = (statbuf.st_mode & S_IXUSR) ? 'x' : '-';

    perms[4] = (statbuf.st_mode & S_IRGRP) ? 'r' : '-';
    perms[5] = (statbuf.st_mode & S_IWGRP) ? 'w' : '-';
    perms[6] = (statbuf.st_mode & S_IXGRP) ? 'x' : '-';

    perms[7] = (statbuf.st_mode & S_IROTH) ? 'r' : '-';
    perms[8] = (statbuf.st_mode & S_IWOTH) ? 'w' : '-';
    perms[9] = (statbuf.st_mode & S_IXOTH) ? 'x' : '-';

    int linkNum = statbuf.st_nlink;

    char * fileUser = getpwuid(statbuf.st_uid)->pw_name;

    char * fileGrp = getgrgid(statbuf.st_gid)->gr_name;

    long int fileSize = statbuf.st_size;

    char * time = ctime(&statbuf.st_mtime);

    char mtime[512] = {0};
    strncpy(mtime, time, strlen(time) - 1);

    char buf[1024];
    sprintf(buf, "%s %d %s %s %ld %s %s", perms, linkNum, fileUser, fileGrp, fileSize, mtime, argv[1]);

    printf("%s\n", buf);

    return 0;
}