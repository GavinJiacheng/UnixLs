#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

int bufsize = 1024;
char* buf;
char* cwd_name;
DIR *cdir;
DIR *cdir2;
struct dirent *entry;
char foldername[32];
int check;

void getAndPrintGroup(gid_t grpNum) {
  struct group *grp;
  grp = getgrgid(grpNum);
  printf("  %s", grp->gr_name);
}

void getAndPrintUserName(uid_t uid) {
  struct passwd *pw = NULL;
  pw = getpwuid(uid);
  printf(" %s", pw->pw_name);
}

void I()
{
  check =0;
  int num = 0;
  while((entry = readdir(cdir))){
    if(entry->d_name[0] != '.')
    {
      check =1;
      printf("%lu %s\t", entry->d_ino, entry->d_name);
      num++;
    }
    if (num == 3)
    {
      printf("\n");
      num=0;
    }
  }
  printf("\n");
}

void L()
{
  check =0;
  int s = 0;
  while((entry = readdir(cdir))){
    if(entry->d_name[0] != '.'){
      struct stat file;
      stat(entry->d_name, &file);
      s += file.st_blocks;
    }
  }
  s=s/2;
  printf("total %d\n", s);
  rewinddir(cdir);
  while((entry = readdir(cdir))){
    if(entry->d_name[0] != '.'){
      check =1;
      struct stat file;
      stat(entry->d_name, &file);
      printf( (S_ISDIR(file.st_mode)) ? "d" : "-");
      printf( (file.st_mode & S_IRUSR) ? "r" : "-");
      printf( (file.st_mode & S_IWUSR) ? "w" : "-");
      printf( (file.st_mode & S_IXUSR) ? "x" : "-");
      printf( (file.st_mode & S_IRGRP) ? "r" : "-");
      printf( (file.st_mode & S_IWGRP) ? "w" : "-");
      printf( (file.st_mode & S_IXGRP) ? "x" : "-");
      printf( (file.st_mode & S_IROTH) ? "r" : "-");
      printf( (file.st_mode & S_IWOTH) ? "w" : "-");
      printf( (file.st_mode & S_IXOTH) ? "x" : "-");
      printf("  %ld", file.st_nlink);
      getAndPrintUserName(file.st_uid);
      getAndPrintGroup(file.st_gid);
      printf("  %lu", file.st_size);
      struct tm *modif_time;
      char time_buf[20];
      modif_time = localtime(&file.st_mtime);
      strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", modif_time);
      printf(" %s", time_buf);
      printf(" %s\n", entry->d_name);
    }
  }
}

void IL()
{
  check =0;
  int s = 0;
  while((entry = readdir(cdir))){
    if(entry->d_name[0] != '.'){
      struct stat file;
      stat(entry->d_name, &file);
      s += file.st_blocks;
    }
  }
  s=s/2;
  printf("total %d\n", s);
  rewinddir(cdir);
  while((entry = readdir(cdir))){
    if(entry->d_name[0] != '.'){
      check =1;
      struct stat file;
      stat(entry->d_name, &file);
      printf("%lu  ", entry->d_ino);
      printf( (S_ISDIR(file.st_mode)) ? "d" : "-");
      printf( (file.st_mode & S_IRUSR) ? "r" : "-");
      printf( (file.st_mode & S_IWUSR) ? "w" : "-");
      printf( (file.st_mode & S_IXUSR) ? "x" : "-");
      printf( (file.st_mode & S_IRGRP) ? "r" : "-");
      printf( (file.st_mode & S_IWGRP) ? "w" : "-");
      printf( (file.st_mode & S_IXGRP) ? "x" : "-");
      printf( (file.st_mode & S_IROTH) ? "r" : "-");
      printf( (file.st_mode & S_IWOTH) ? "w" : "-");
      printf( (file.st_mode & S_IXOTH) ? "x" : "-");
      printf("  %ld", file.st_nlink);
      getAndPrintUserName(file.st_uid);
      getAndPrintGroup(file.st_gid);
      printf("  %lu", file.st_size);
      struct tm *modif_time;
      char time_buf[20];
      modif_time = localtime(&file.st_mtime);
      strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", modif_time);
      printf(" %s", time_buf);
      printf(" %s\n", entry->d_name);
    }
  }
}

void R1()
{
  DIR *printdir=cdir;
  check =0;
  while((entry = readdir(printdir))){
    if(entry->d_name[0] != '.'){
      check=1;
      printf("%s\t", entry->d_name);
    }
  }
  if(check)
    printf("\n");
}

void R2()
{
  DIR *newdir=cdir2;
  char now_name[32];
  strcpy(now_name, foldername);
  while((entry = readdir(newdir)))
  {
    if(!strcmp(entry->d_name,".") || !strcmp(entry->d_name,".."))
      continue;
    if (entry->d_type & DT_DIR)
    {
      printf("\n%s\\",now_name);
      cdir = opendir(entry->d_name);
      cdir2 = opendir(entry->d_name);
      strcpy(foldername, entry->d_name);
      chdir(entry->d_name);
      printf("%s:\n", foldername);
      R1();
      if(check){
        R2();
      }
      chdir("..");
    }
  }
}
void R()
{
  R1();
  R2();
}

void IR(){
  DIR *newdir=cdir2;
  char now_name[32];
  strcpy(now_name, foldername);
  while((entry = readdir(newdir)))
  {
    if(!strcmp(entry->d_name,".") || !strcmp(entry->d_name,".."))
      continue;
    if (entry->d_type & DT_DIR)
    {
      printf("\n%s\\",now_name);
      cdir = opendir(entry->d_name);
      cdir2 = opendir(entry->d_name);
      strcpy(foldername, entry->d_name);
      chdir(entry->d_name);
      printf("%s:\n", foldername);
      I();
      if(check){
        IR();
      }
      chdir("..");
    }
  }
}
void LR(){
  DIR *newdir=cdir2;
  char now_name[32];
  strcpy(now_name, foldername);
  while((entry = readdir(newdir)))
  {
    if(!strcmp(entry->d_name,".") || !strcmp(entry->d_name,".."))
      continue;
    if (entry->d_type & DT_DIR)
    {
      printf("\n%s\\",now_name);
      cdir = opendir(entry->d_name);
      cdir2 = opendir(entry->d_name);
      strcpy(foldername, entry->d_name);
      chdir(entry->d_name);
      printf("%s:\n", foldername);
      L();
      if(check){
        LR();
      }
      chdir("..");
    }
  }
}
void ILR(){
  DIR *newdir=cdir2;
  char now_name[32];
  strcpy(now_name, foldername);
  while((entry = readdir(newdir)))
  {
    if(!strcmp(entry->d_name,".") || !strcmp(entry->d_name,".."))
      continue;
    if (entry->d_type & DT_DIR)
    {
      printf("\n%s\\",now_name);
      cdir = opendir(entry->d_name);
      cdir2 = opendir(entry->d_name);
      strcpy(foldername, entry->d_name);
      chdir(entry->d_name);
      printf("%s:\n", foldername);
      IL();
      if(check){
        ILR();
      }
      chdir("..");
    }
  }
}

void illegalinput(char* arg)
{
  printf("ls: cannot access '%s': No such file or directory \n", arg);
}

int main(int argc, char* argv[]){
  buf = malloc(bufsize);
  cwd_name = getcwd(buf, bufsize);
  cdir = opendir(cwd_name);
  cdir2 = opendir(cwd_name);
  strcpy(foldername, ".");
  check =0;
  if(cdir != NULL){

    if(argc < 2){
      while((entry = readdir(cdir))){
        if(entry->d_name[0] != '.')
          printf("%s\t", entry->d_name);
      }
      closedir(cdir);
    }
    else if ((strncmp(argv[1], "-Ril", 4) == 0)||(strncmp(argv[1], "-Rli", 4) == 0)||(strncmp(argv[1], "-ilR", 4) == 0)||(strncmp(argv[1], "-iRl", 4) == 0)){
      printf("%s:\n", foldername);
      IL();
      ILR();
      closedir(cdir);
    }
    else if ((strncmp(argv[1], "-liR", 4) == 0)||(strncmp(argv[1], "-lRi", 4) == 0)){
      printf("%s:\n", foldername);
      IL();
      ILR();
      closedir(cdir);
    }
    else if ((strncmp(argv[1], "-li", 3) == 0)||(strncmp(argv[1], "-il", 3) == 0))
    {
      IL();
      closedir(cdir);
    }
    else if ((strncmp(argv[1], "-Ri", 3) == 0)||(strncmp(argv[1], "-iR", 3) == 0)){
      printf("%s:\n", foldername);
      I();
      IR();
      closedir(cdir);
    }
    else if ((strncmp(argv[1], "-lR", 3) == 0)||(strncmp(argv[1], "-Rl", 3) == 0)){
      printf("%s:\n", foldername);
      L();
      LR();
      closedir(cdir);
    }

    else if (strncmp(argv[1], "-R", 2) == 0){
        printf("%s:\n", foldername);
        R();
        closedir(cdir);
    }

    else if (strncmp(argv[1], "-i", 2) == 0){
      I();
      closedir(cdir);
    }

    else if (strncmp(argv[1], "-l", 2) == 0){
      L();
      closedir(cdir);
    }
    else
      illegalinput(argv[1]);
    return 0;
  }
}
