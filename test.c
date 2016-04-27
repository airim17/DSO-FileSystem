#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/filesystem.h"

#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"

int main() {
	int ret, aux, i, fd;
	char bufferR[BLOCK_SIZE];
	char bufferW[BLOCK_SIZE];


	/* -------------------------------- mkFS ---------------------------------- */
	fprintf(stdout, "%s", "-------- TESTS mkFS -------\n");

	fprintf(stdout, "%s", "TEST 1: Creating a device under / above the limits of files\n");
	ret = mkFS(50, 600000);
	if (ret != -1) {
		fprintf(stdout, "%s%s%s%s", "TEST 1: mkFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 1: mkFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 2: Creating a device under / above the limits of size\n");
	ret = mkFS(0, 400000);
	if (ret != -1) {
		fprintf(stdout, "%s%s%s%s", "TEST 2: mkFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 2: mkFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 3: Creating a device with valid parameters\n");
	ret = mkFS(50, 400000);
	if (ret != 0) {
		fprintf(stdout, "%s%s%s%s\n", "TEST 3: mkFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s\n", "TEST 3: mkFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}



	/* ------------------------- mounFS / umountFS --------------------------- */
	fprintf(stdout, "%s", "-------- TESTS mountFS / unmountFS -------\n");

	fprintf(stdout, "%s", "TEST 1: mountFS + unmountFS\n");
	ret = mountFS();
	if (ret != 0) {
		fprintf(stdout, "%s%s%s%s", "TEST 1: mountFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 1: mountFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	ret = umountFS();
	if (ret != 0) {
		fprintf(stdout, "%s%s%s%s", "TEST 1: unmountFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 1: unmountFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 2: umountFS without mountFS\n");
	ret = umountFS();
	if (ret != 0) {
		fprintf(stdout, "%s%s%s%s\n", "TEST 2: unmountFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s\n", "TEST 2: unmountFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}



	/* ------------------------------- creatFS -------------------------------- */
	fprintf(stdout, "%s", "-------- TESTS creatFS -------\n");

	fprintf(stdout, "%s", "TEST 1: creatFS with valid name\n");
	ret = creatFS("test.txt");
	if (ret != 0) {
		fprintf(stdout, "%s%s%s%s", "TEST 1: creatFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 1: creatFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 2: creatFS with a name that already exist\n");
	ret = creatFS("test.txt");
	if (ret != 1) {
		fprintf(stdout, "%s%s%s%s", "TEST 2: creatFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 2: creatFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 3: creatFS with an invalid name\n");
	ret = creatFS("loooooooooooooooooooooooooooooooooooooooooooooooooooooooooong.txt");
	if (ret != -1) {
		fprintf(stdout, "%s%s%s%s\n", "TEST 3: creatFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s\n", "TEST 3: creatFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}



	/* ------------------------------- openFS / closeFS -------------------------------- */
	fprintf(stdout, "%s", "-------- TESTS openFS / closeFS -------\n");

	fprintf(stdout, "%s", "TEST 1: openFS + closeFS an existing file\n");
	ret = openFS("test.txt");
	if (ret == -1 || ret == -2) {
		fprintf(stdout, "%s%s%s%s", "TEST 1: openFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 1: openFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	ret = closeFS(ret);
	if (ret != 0) {
		fprintf(stdout, "%s%s%s%s", "TEST 1: closeFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 1: closeFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 2: openFS a NOT existing file\n");
	ret = openFS("notExistingFile.txt");
	if (ret != -1) {
		fprintf(stdout, "%s%s%s%s", "TEST 2: openFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 2: openFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 3: openFS an already open file\n");
	aux = openFS("test.txt");
	if (aux == -1 || aux == -2) {
		fprintf(stdout, "%s%s%s%s", "TEST 3: openFS #1 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 3: openFS #1 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	ret = openFS("test.txt");
	if (ret != -2) {
		fprintf(stdout, "%s%s%s%s", "TEST 3: openFS #2 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 3: openFS #2 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	ret = closeFS(aux);
	if (ret != 0) {
		fprintf(stdout, "%s%s%s%s", "TEST 3: closeFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 3: closeFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 4: closeFS without openFS\n");
	ret = closeFS(3);
	if (ret != -1) {
		fprintf(stdout, "%s%s%s%s\n", "TEST 4: closeFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s\n", "TEST 4: closeFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}



	/* ------------------------------- readFS  -------------------------------- */
	fprintf(stdout, "%s", "-------- TESTS readFS -------\n");

	fprintf(stdout, "%s", "TEST 1: readFS without opening the file\n");
	ret = readFS(3, bufferR, BLOCK_SIZE);

	if (ret != -1) {
		fprintf(stdout, "%s%s%s%s", "TEST 1: readFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 1: readFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fd = openFS("test.txt");

	fprintf(stdout, "%s", "TEST 2: readFS a wrong file descriptor\n");
	ret = readFS(55, bufferR, BLOCK_SIZE);
	if (ret != -1) {
		fprintf(stdout, "%s%s%s%s", "TEST 2: readFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 2: readFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 3: readFS when the pointer is at the end\n");
	ret = readFS(fd, bufferR, BLOCK_SIZE);
	if (ret != 0) {
		fprintf(stdout, "%s%s%s%s\n", "TEST 3: readFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s\n", "TEST 3: readFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	closeFS(fd);



	/* ------------------------------- writeFS  -------------------------------- */
	fprintf(stdout, "%s", "-------- TESTS writeFS -------\n");
	memset(bufferW, 't', BLOCK_SIZE);

	fprintf(stdout, "%s", "TEST 1: writeFS without opening the file\n");
	ret = writeFS(3, bufferW, BLOCK_SIZE);
	if (ret != -1) {
		fprintf(stdout, "%s%s%s%s", "TEST 1: writeFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 1: writeFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fd = openFS("test.txt");

	fprintf(stdout, "%s", "TEST 2: writeFS an wrong file descriptor\n");
	ret = writeFS(55, bufferW, BLOCK_SIZE);
	if (ret != -1) {
		fprintf(stdout, "%s%s%s%s", "TEST 2: writeFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 2: writeFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 3: writeFS with more bytes than the maximum size\n");
	ret = writeFS(fd, bufferW, BLOCK_SIZE+1);
	if (ret != -1) {
		fprintf(stdout, "%s%s%s%s\n", "TEST 3: writeFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s\n", "TEST 3: writeFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	closeFS(fd);



	/* -------------------------- readFS + writeFS  --------------------------- */
	fprintf(stdout, "%s", "-------- TESTS readFS + writeFS -------\n");
	memset(bufferW, 't', BLOCK_SIZE);
	fd = openFS("test.txt");


	fprintf(stdout, "%s", "TEST 1: writeFS 2KB\n");
	ret = writeFS(fd, bufferW, BLOCK_SIZE/2);
	if (ret != BLOCK_SIZE/2) {
		fprintf(stdout, "%s%s%s%s", "TEST 1: writeFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 1: writeFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 2: lseekFS(0) + readFS more than the file size\n");
	ret = lseekFS(fd, 0, FS_SEEK_BEGIN);
	if (ret != 0) {
		fprintf(stdout, "%s%s%s%s", "TEST 2: lseekFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 2: lseekFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	ret = readFS(fd, bufferR, BLOCK_SIZE);
	if (ret != BLOCK_SIZE/2) {
		fprintf(stdout, "%s%s%s%s", "TEST 2: readFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 2: readFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 3: readFS when the pointer is at the end\n");
	ret = readFS(fd, bufferR, BLOCK_SIZE/2);
	if (ret != 0) {
		fprintf(stdout, "%s%s%s%s", "TEST 3: readFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 3: readFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 4: writeFS more than the maximum size\n");
	ret = writeFS(fd, bufferW, BLOCK_SIZE);
	if (ret != -1) {
		fprintf(stdout, "%s%s%s%s\n", "TEST 4: writeS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s\n", "TEST 4: writeFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	closeFS(fd);



	/* -------------------------------- lseekFS  ------------------------------ */
	fprintf(stdout, "%s", "-------- TESTS lseekFS -------\n");


	fprintf(stdout, "%s", "TEST 1: lseek withour opening the file\n");
	ret = lseekFS(3, 0, BLOCK_SIZE/2);
	if (ret != -1) {
		fprintf(stdout, "%s%s%s%s", "TEST 1: lseekFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 1: lseekFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fd = openFS("test.txt");

	fprintf(stdout, "%s", "TEST 2: lseek with an invalid \"whence\"\n");
	ret = lseekFS(fd, 0, 3);
	if (ret != -1) {
		fprintf(stdout, "%s%s%s%s", "TEST 2: lseekFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 2: lseekFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 3: lseek with an invalid \"offset\"\n");
	ret = lseekFS(fd, 4097, 0);
	if (ret != -1) {
		fprintf(stdout, "%s%s%s%s", "TEST 3: lseekFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 3: lseekFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 4: lseek at the end of the file\n");
	ret = lseekFS(fd, 50000, 2);
	if (ret != BLOCK_SIZE/2) {
		fprintf(stdout, "%s%s%s%s", "TEST 4: lseekFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 4: lseekFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	closeFS(fd);


	// fprintf(stdout, "%s", "TEST tagFS + listFS + untagFS\n");
	//
	// ret = tagFS("test.txt", "sample");
	// if(ret != 0) {
	// 	fprintf(stdout, "%s%s%s%s%s", "TEST tagFS + listFS + untagFS ", ANSI_COLOR_RED, "FAILED ", ANSI_COLOR_RESET, "at tagFS\n");
	// 	return -1;
	// }
	// char **files = (char**) malloc(50*sizeof(char*));
	// for(i = 0; i < 50; ++i) {
	// 	files[i] = (char*) malloc(64*sizeof(char));
	// }
	// ret = listFS("sample", files);
	// if(ret != 1 || strcmp(files[0], "test.txt") != 0) {
	// 	fprintf(stdout, "%s%s%s%s%s", "TEST tagFS + listFS + untagFS ", ANSI_COLOR_RED, "FAILED ", ANSI_COLOR_RESET, "at listFS\n");
	// 	return -1;
	// }
	//
	// for (i = 0 ; i < 50 ; i++){
	// 	printf ("%s ", files[i]);
	// }
	//
	// ret = tagFS("test.txt", "2");
	// ret = tagFS("test.txt", "3");
	// ret = tagFS("test.txt", "4");
	// ret = untagFS("test.txt", "sample");
	// if(ret == -1) {
	// 	fprintf(stdout, "%s%s%s%s%s", "TEST tagFS + listFS + untagFS ", ANSI_COLOR_RED, "FAILED ", ANSI_COLOR_RESET, "at untagFS\n");
	// 	return -1;
	// }
	//
	// for (i = 0 ; i < 50 ; i++){
	// 	files[i] = "";
	// }
	//
	// ret = listFS("sample", files);
	// if(ret == -1) {
	// 	fprintf(stdout, "%s%s%s%s%s", "TEST tagFS + listFS + untagFS ", ANSI_COLOR_RED, "FAILED ", ANSI_COLOR_RESET, "at listFS #2\n");
	// 	return -1;
	// }
	//
	// for (i = 0 ; i < 50 ; i++){
	// 	printf ("%s ", files[i]);
	// }
	//
	// fprintf(stdout, "%s%s%s%s", "TEST tagFS + listFS + untagFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	// fprintf(stdout, "%s", "TEST umountFS\n");
	//
	//
	// ret = umountFS();
	// if(ret != 0) {
	// 	fprintf(stdout, "%s%s%s%s", "TEST umountFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	// 	return -1;
	// }
	//
	// fprintf(stdout, "%s%s%s%s", "TEST umountFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);


	return 0;
}
