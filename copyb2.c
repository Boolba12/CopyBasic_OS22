#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

#define _PP 2

int main(int argc, char **argv, char *envp[])
{

 int des, vystup;
 int i, written;
 int sumread = 0, sumwrite = 0;

 char buffer[256];

 if(argc < 3) {fprintf(stderr, "Chyba argumenty\n");
 return 99;
 }
 if(strcmp(argv[1], "-E") == 0)
 {
    for(int i = 0; envp[i] != NULL; i++)
    {
        printf("\n%s", envp[i]);
    }
 }
 if(strcmp(argv[1], "-D")  == 0 )
 {
    DIR *adresar;
    FILE *stream_point;
    stream_point = fopen(argv[argc - 1], "w");
    struct dirent *subor;
    adresar=opendir(argv[argc - 2]); //Otvorime si adresar
    if(adresar == NULL){ fprintf(stderr, "adresar nejde otvorit?\n"); return 28;}
    if(stream_point == NULL){fprintf(stderr, "vystup nejde otvorit!\n"); return 28;}
    while ((subor=readdir(adresar))!=NULL){ //Citame kym je co citat
    fprintf(stream_point,"%s\n",subor->d_name); //A vypiseme nazov suboru
    }
    closedir(adresar); //A nakoniec adresar zatvorime
    return 0;
}


 des = open(argv[argc - 2], O_RDONLY);
 
 if(des < 1)
 {
 if(errno == 2)
 {
    fprintf(stderr,"Subor file neexistuje\n"); return 21;}
    fprintf(stderr, "Chyba infile\n"); return 21;
 }
if(strcmp(argv[1], "-a")==0)
{
    vystup = open(argv[argc - 1], O_RDWR + O_APPEND);
}else
{
vystup = open(argv[argc -1], O_CREAT +O_RDWR + O_TRUNC, 00700);
}
if(vystup == -1)
{
if(errno == 2)
{
    fprintf(stderr, "Subor outfile neexistuje\n"); return 22;
}
fprintf(stderr, "Chyba outfile\n"); return 21;
}

do
{
i = read(des, &buffer, sizeof(buffer));
written = write(vystup, &buffer, i);
sumread += i; sumwrite += written;
if(written == -1) {fprintf(stderr,"Chyba write\n"); return 22;}
}while(i > 0);

printf("%d, %d", sumread, sumwrite);
if((strcmp(argv[1], "-d") == 0) && (sumread == sumwrite))
{
if(vystup < 3) { fprintf(stderr, "Vystupny subor ne existuje!\n"); return 26;}
if(unlink(argv[argc - 2]) < 0)
{
fprintf(stderr,"Chyba delete\n"); return 26;
}    
}

close(des);
close(vystup);
return 0;

}
