#include "io_file.h"

// Affichage du contenu d'un fichier
// \param	path			Nom/chemin du fichier à afficher
// \return					-1 si échec à l'ouverture, 0 sinon
int print(const char *path){
	IO_FILE file=IO_open(path, O_RDONLY);
	// test it's open successfully
	if((file.path == NULL) || (file.desc == -1)) return -1;

	char print_char;
	int success=1;
	do{
		success = IO_char_read(file, &print_char);
		// test success for don't print the last \n
		if(success) printf("%c", print_char);
	}while(success>0);

	IO_close(file);
	return 0;
}
// Copie de fichier
// \param	path1			Nom/chemin du fichier source
// \param	path2			Nom/chemin du fichier destination
// \return					-1 si échec à l'ouverture, 0 sinon
int copy(const char *path1, const char *path2){
	// Open src file
	IO_FILE file1 = IO_open(path1, O_RDONLY);
	// test it's open successfully
	if((file1.path == NULL) || (file1.desc == -1)) return -1;

	// Save the file1
	char *buffer = malloc(SIZE_BUF*sizeof(char));
	if(!buffer) exit(FAILED_PTR_NULL);
	int success, pos_read;
	char char_read;
	success=1; pos_read=0;

	do{
		if(pos_read == SIZE_BUF-1) buffer = (char *) realloc(buffer, 1.5*SIZE_BUF*sizeof(char));

		success = IO_char_read(file1, &char_read);
		// test success for don't print the last \n
		if(success)
		{
			buffer[pos_read] = char_read;
			pos_read++;
		}
	}while(success>0);

	IO_close(file1);
	// Open destination file
	IO_FILE file2 = IO_open(path2, O_WRONLY);
	// test it's open successfully
	if((file2.path == NULL) || (file2.desc == -1)) return -1;
	int pos_write=0;
	success=1;

	do{
		success = IO_char_write(file2, buffer[pos_write]);
		pos_write++;
	}while((pos_write<pos_read) && (success>0));

	IO_close(file2);
	free(buffer);
	return 0;
}
// Déplacement de fichier
// \param	path1			Nom/chemin du fichier source
// \param	path2			Nom/chemin du fichier destination
// \return					-1 si échec à l'ouverture, 0 sinon
int move(const char *path1, const char *path2){
	int success_open = copy(path1, path2);
	IO_remove(path1);
	return success_open;
}
// Comparaison entre deux fichiers
// \param	path1			Nom/chemin du premier fichier
// \param	path2			Nom/chemin du second fichier
// \return					-1 si échec à l'ouverture,
//							1 si fichiers identiques, 0 sinon
int are_the_same(const char *path1, const char *path2){
	// Open file1
	IO_FILE file1 = IO_open(path1, O_RDONLY);

	// Open file2
	IO_FILE file2 = IO_open(path2, O_RDONLY);

	// Save the file1
	char *buffer = malloc(SIZE_BUF*sizeof(char));
	if(!buffer) exit(FAILED_PTR_NULL);
	int success, pos_read;
	char char_read;
	success=1; pos_read=0;

	do{
		if(pos_read == SIZE_BUF-1) buffer = (char *) realloc(buffer, 1.5*SIZE_BUF*sizeof(char));

		success = IO_char_read(file1, &char_read);
		// test success for don't print the last \n
		if(success)
		{
			buffer[pos_read] = char_read;
			pos_read++;
		}
	}while(success>0);

	// Save the file2
	char *buffer2 = malloc(SIZE_BUF*sizeof(char));
	if(!buffer2) exit(FAILED_PTR_NULL);
	int success2, pos_read2;
	char char_read2;
	success2=1; pos_read2=0;

	do{
		if(pos_read2 == SIZE_BUF-1) buffer2 = (char *) realloc(buffer2, 1.5*SIZE_BUF*sizeof(char));

		success2 = IO_char_read(file2, &char_read2);
		// test success for don't print the last \n
		if(success2)
		{
			buffer2[pos_read2] = char_read2;
			pos_read2++;
		}
	}while(success2>0);

	IO_close(file1); IO_close(file2);
	if(!strcmp(buffer, buffer2)) {
		free(buffer); free(buffer2);
		return 1;
	}

	return 0;

}

void check_print() {
	// Save the file descriptor
	int nstdout = creat("test2.txt", 0644);
	int tmp;

	system("echo 'This file\nis\n\nGREAT!\n' >test1.txt");

	// dup: duplicate the file descriptor
	// fileno: return file descripter of his parameter
	tmp = dup(fileno(stdout));

	// Vider (entre autre) le buffer de stdout
	fflush(NULL);
	dup2(nstdout, fileno(stdout));

	print("test1.txt");

	// Vider (entre autre) le buffer de stdout
	fflush(NULL);
	dup2(tmp, fileno(stdout));

	close(nstdout); close(tmp);
	int val_diff=system("diff test1.txt test2.txt >diff.log");

	if (!val_diff)
		printf("'print()' test has succeeded.\n");
	else
		printf("'print()' test has failed.\n");

	system("rm -f diff.log test1.txt test2.txt");
}

void check_copy() {
	system("echo 'This file\nis\n\nGREAT!\n' >test1.txt");

	copy("test1.txt", "test2.txt");

	if (!system("diff test1.txt test2.txt >diff.log"))
		printf("'copy()' test has succeeded.\n");
	else
		printf("'copy()' test has failed.\n");

	system("rm -f diff.log test1.txt test2.txt");
}

void check_move() {
	system("echo 'This file\nis\n\nGREAT!\n' >test1.txt");
	system("cat test1.txt >test2.txt");

	move("test2.txt", "test3.txt");

	// if( system("[ -e test2.txt ]") != 0)
	int file2_existe = system("test -e test2.txt");
	if (! file2_existe )
		printf("'move()' test has failed.\n");
	else if (!system("diff test1.txt test3.txt >diff.log"))
		printf("'move()' test has succeeded.\n");
	else
		printf("'move()' test has failed.\n");

	system("rm -f diff.log test1.txt test3.txt");
}

void check_are_the_same () {
	system("echo 'This file\nis\n\nGREAT!\n' >test1.txt");
	system("echo 'This file\nis\n\nGREAT!\n' >test2.txt");
	system("echo 'This file\nis\n\nGREAT!\n\n' >test3.txt");
	system("echo 'This fil\nis\n\nGREAT!\n' >test4.txt");
	system("echo 'This file\nis\nGREAT!\n' >test5.txt");

	if (!are_the_same("test1.txt", "test2.txt"))
		printf("'are_the_same()' test#1 has failed.\n");
	else if (are_the_same("test1.txt", "test3.txt"))
		printf("'are_the_same()' test#2 has failed.\n");
	else if (are_the_same("test2.txt", "test4.txt"))
		printf("'are_the_same()' test#3 has failed.\n");
	else if (are_the_same("test3.txt", "test5.txt"))
		printf("'are_the_same()' test#4 has failed.\n");
	else
		printf("'are_the_same()' test has succeeded.\n");

	system("rm -f test1.txt test2.txt test3.txt test4.txt test5.txt");
}

int main(int argc, char **argv) {
	puts("\n\n##########   Test function(s)   ##########");
	check_print();
	check_copy();
	check_move();
	check_are_the_same();
	puts("########## End test function(s) ##########\n");

	return 0;
}
