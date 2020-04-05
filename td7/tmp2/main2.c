#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/types.h>

int main()
{
   int fd;
   FILE *fp;

   char *nomfich="/tmp/test.txt", chaine[50];

   fd = open(nomfich, O_RDONLY);
   fp = fdopen(fd, "r");

   fscanf(fp, "%s", chaine);
   puts(chaine);

   unlink(nomfich);

   return 0;
}
