#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/types.h>

int main(void) {
   int fd;
   FILE *fp;
   char *nomfich="/tmp/test.txt";

   if(mkfifo(nomfich, 0644) != 0) {
      perror("Problème de création du noeud de tube");
      exit(1);
   }

   fd = open(nomfich, O_WRONLY);
   fp = fdopen(fd, "w");
   fprintf(fp, "coucou\n");

   unlink(nomfich);

   return 0;
}
