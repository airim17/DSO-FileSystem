#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/filesystem.h"

#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"

int main() {

	char bufferR [BLOCK_SIZE];
	char bufferW [BLOCK_SIZE];
	char **files = (char**) malloc(50 * sizeof(char*));
	int ret = 0, i = 0, fd = 0, error = 0;


	/* -------------------------------- mkFS ---------------------------------- */
	fprintf(stdout, "%s", "-------- TESTS mkFS -------\n");

	fprintf(stdout, "%s", "TEST 1: Creating a device under / above the limits of files\n");
	ret = mkFS(50, 600000);
	if (ret != -1) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 2: Creating a device under / above the limits of size\n");
	ret = mkFS(0, 400000);
	if (ret != -1) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 3: Creating a device with valid parameters\n");
	ret = mkFS(50, 400000);
	if (ret != 0) {
		fprintf(stdout, "%s%s%s\n", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s\n", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}



	/* ------------------------- mounFS / umountFS --------------------------- */
	fprintf(stdout, "%s", "-------- TESTS mountFS / unmountFS -------\n");

	fprintf(stdout, "%s", "TEST 1: mountFS + unmountFS\n");
	ret = mountFS();
	if (ret != 0) {
		error = 1;
	}

	ret = umountFS();
	if (ret != 0 || error == 1) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		error = 0;
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 2: umountFS without mountFS\n");
	ret = umountFS();
	if (ret != 0) {
		fprintf(stdout, "%s%s%s\n", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s\n", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	/* From now on we need to have the file system mounted */
	if (mountFS() == -1){
		return -1;
	}


	/* ------------------------------- creatFS -------------------------------- */
	fprintf(stdout, "%s", "-------- TESTS creatFS -------\n");

	fprintf(stdout, "%s", "TEST 1: creatFS with valid name\n");
	ret = creatFS("test.txt");
	if (ret != 0) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 2: creatFS with a name that already exist\n");
	ret = creatFS("test.txt");
	if (ret != 1) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 3: creatFS with a name of length 0\n");
	ret = creatFS("");
	if (ret != -1) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 4: creatFS with a long name\n");
	ret = creatFS("loooooooooooooooooooooooooooooooooooooooooooooooooooooooooong.txt");
	if (ret != -1) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 5: creatFS when there is no space\n");
	mkFS(10, 400000);
	mountFS();
	creatFS("test1.txt");
	creatFS("test2.txt");
	creatFS("test3.txt");
	creatFS("test4.txt");
	creatFS("test5.txt");
	creatFS("test6.txt");
	creatFS("test7.txt");
	creatFS("test8.txt");
	creatFS("test9.txt");
	creatFS("test10.txt");

	ret = creatFS("test11.txt");
	if (ret != -1) {
		fprintf(stdout, "%s%s%s\n", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s\n", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	mkFS(50, 400000);
	mountFS();
	creatFS("test.txt");



	/* ------------------------------- openFS / closeFS -------------------------------- */
	fprintf(stdout, "%s", "-------- TESTS openFS / closeFS -------\n");

	fprintf(stdout, "%s", "TEST 1: openFS + closeFS an existing file\n");
	fd = openFS("test.txt");
	if (fd == -1 || fd == -2) {
		error = 1;
	}

	ret = closeFS(fd);
	if (ret != 0 || error == 1) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		error = 0;
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 2: openFS a NOT existing file\n");
	ret = openFS("notExistingFile.txt");
	if (ret != -1) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 3: openFS + openFS\n");
	fd = openFS("test.txt");
	if (fd == -1 || fd == -2) {
		error = 1;
	}

	fd = openFS("test.txt");
	if (fd == -1 || fd == -2 || error == 1) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		error = 0;
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 4: closeFS + closeFS\n");
	ret = closeFS(fd);
	if (ret != 0) {
		error = 1;
	}

	ret = closeFS(3);
	if (ret != 0 || error == 1) {
		fprintf(stdout, "%s%s%s\n", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		error = 0;
	}
	else {
		fprintf(stdout, "%s%s%s\n", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}



	/* ------------------------------- readFS  -------------------------------- */
	fprintf(stdout, "%s", "-------- TESTS readFS -------\n");

	fprintf(stdout, "%s", "TEST 1: readFS without opening the file\n");
	ret = readFS(3, bufferR, BLOCK_SIZE);

	if (ret != -1) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fd = openFS("test.txt");

	fprintf(stdout, "%s", "TEST 2: readFS a wrong file descriptor\n");
	ret = readFS(55, bufferR, BLOCK_SIZE);
	if (ret != -1) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 3: readFS when the pointer is at the end\n");
	ret = readFS(fd, bufferR, BLOCK_SIZE);
	if (ret != 0) {
		fprintf(stdout, "%s%s%s\n", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s\n", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	closeFS(fd);



	/* ------------------------------- writeFS  -------------------------------- */
	fprintf(stdout, "%s", "-------- TESTS writeFS -------\n");
	memset(bufferW, 't', BLOCK_SIZE);

	fprintf(stdout, "%s", "TEST 1: writeFS without opening the file\n");
	ret = writeFS(3, bufferW, BLOCK_SIZE);
	if (ret != -1) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fd = openFS("test.txt");

	fprintf(stdout, "%s", "TEST 2: writeFS an wrong file descriptor\n");
	ret = writeFS(55, bufferW, BLOCK_SIZE);
	if (ret != -1) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 3: writeFS with more bytes than the maximum size\n");
	ret = writeFS(fd, bufferW, BLOCK_SIZE+1);
	if (ret != -1) {
		fprintf(stdout, "%s%s%s\n", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s\n", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	closeFS(fd);



	/* -------------------------------- lseekFS  ------------------------------ */
	fprintf(stdout, "%s", "-------- TESTS lseekFS -------\n");


	fprintf(stdout, "%s", "TEST 1: lseek without opening the file\n");
	ret = lseekFS(3, 0, BLOCK_SIZE/2);
	if (ret != -1) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fd = openFS("test.txt");

	fprintf(stdout, "%s", "TEST 2: lseek with an invalid \"whence\"\n");
	ret = lseekFS(fd, 0, 3);
	if (ret != -1) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 3: lseek with an invalid \"offset\"\n");
	ret = lseekFS(fd, 4097, 0);
	if (ret != -1) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 4: lseek at the end of the file\n");

	if (writeFS(fd, bufferW, BLOCK_SIZE/2) == -1){
		return -1;
	}

	ret = lseekFS(fd, 50000, 2);
	if (ret != BLOCK_SIZE/2) {
		fprintf(stdout, "%s%s%s\n", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s\n", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	closeFS(fd);



	/* -------------------------- lseekFS + readFS + writeFS  --------------------------- */
	fprintf(stdout, "%s", "-------- TESTS lseekFS + readFS + writeFS -------\n");

	memset(bufferW, 't', BLOCK_SIZE);
	fd = openFS("test.txt");

	fprintf(stdout, "%s", "TEST 1: writeFS 2KB\n");
	ret = writeFS(fd, bufferW, BLOCK_SIZE/2);
	if (ret != BLOCK_SIZE/2) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 2: lseekFS(0) + readFS more than the file size\n");
	ret = lseekFS(fd, 0, FS_SEEK_BEGIN);
	if (ret != 0) {
		error = 1;
	}

	ret = readFS(fd, bufferR, BLOCK_SIZE);
	if (ret != BLOCK_SIZE/2 || error == 1) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		error = 0;
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 3: readFS when the pointer is at the end\n");
	ret = readFS(fd, bufferR, BLOCK_SIZE/2);
	if (ret != 0) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 4: writeFS more than the maximum size\n");
	ret = writeFS(fd, bufferW, BLOCK_SIZE);
	if (ret != -1) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 5: lseekFS(0) + writeFS + lseekFS(0) + readFS\n");
	memset(bufferW, 'X', BLOCK_SIZE/2);
	ret = lseekFS(fd, 0, FS_SEEK_BEGIN);
	ret = writeFS(fd, bufferW, BLOCK_SIZE/2);
	ret = lseekFS(fd, 0, FS_SEEK_BEGIN);
	ret = readFS(fd, bufferR, BLOCK_SIZE/2);

	for (i = 0 ; i < ret ; i++){
		if (bufferW[i] != bufferR[i] || bufferR[i] != 'X'){
			error = 1;
			fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
			break;
		}
	}

	if (error == 0){
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	error = 0;


	fprintf(stdout, "%s", "TEST 6: lseekFS(1848) + writeFS + lseekFS(0) + readFS\n");
	memset(bufferW, 'Y', 300);
	ret = lseekFS(fd, ((BLOCK_SIZE / 2) - 200), FS_SEEK_SET);
	ret = writeFS(fd, bufferW, 300);
	ret = lseekFS(fd, 0, FS_SEEK_BEGIN);
	ret = readFS(fd, bufferR, BLOCK_SIZE);

	for (i = 0 ; i < BLOCK_SIZE ; i++){
		if (i >= 0 && i < ((BLOCK_SIZE / 2) - 200) && bufferR[i] != 'X'){
			error = 1;
			fprintf(stdout, "%s%s%s\n", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
			break;
		}
		else if (i >= ((BLOCK_SIZE / 2) - 200) && i < ((BLOCK_SIZE / 2) + 100) && bufferR[i] != 'Y'){
			error = 1;
			fprintf(stdout, "%s%s%s\n", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
			break;
		}
	}

	if (error == 0){
		fprintf(stdout, "%s%s%s\n", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	closeFS(fd);



	/* --------------------------------- tagFS  ------------------------------- */
	fprintf(stdout, "%s", "-------- TESTS tagFS -------\n");

	fd = openFS("test.txt");

	fprintf(stdout, "%s", "TEST 1: tag an open file\n");
	ret = tagFS("test.txt", "tag1");
	if (ret != -1) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	closeFS(fd);


	fprintf(stdout, "%s", "TEST 2: tag a new valid tag\n");
	ret = tagFS("test.txt", "tag1");
	if (ret != 0) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 3: tag a an already existing tag\n");
	ret = tagFS("test.txt", "tag1");
	if (ret != 1) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	if (tagFS("test.txt", "tag2") != 0){
		return -1;
	}
	if (tagFS("test.txt", "tag3") != 0){
		return -1;
	}

	fprintf(stdout, "%s", "TEST 4: tag a file without tag spaces\n");
	ret = tagFS("test.txt", "tag4");
	if (ret != -1) {
		fprintf(stdout, "%s%s%s\n", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s\n", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}



	/* ------------------------------- untagFS  ------------------------------- */
	fprintf(stdout, "%s", "-------- TESTS untagFS -------\n");

	fprintf(stdout, "%s", "TEST 1: untag a NON existing tag\n");
	ret = untagFS("test.txt", "tag5");
	if (ret != -1) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	fprintf(stdout, "%s", "TEST 2: untag an existing tag\n");
	ret = untagFS("test.txt", "tag1");
	if (ret != 0) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	fprintf(stdout, "%s", "TEST 3: untag + tag another one\n");
	ret = tagFS("test.txt", "tag1");
	if (ret != 0) {
		fprintf(stdout, "%s%s%s\n", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s\n", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}



	/* --------------------------------- listFS  ------------------------------ */
	fprintf(stdout, "%s", "-------- TESTS listFS -------\n");

	fprintf(stdout, "%s", "TEST 1: list files with a tag that do not exist\n");
	ret = listFS("test.txt", files);
	if (ret != -1) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	if (creatFS("test2.txt") == -1){
		return -1;
	}
	tagFS("test2.txt", "tag1");

	if (creatFS("test3.txt") == -1){
		return -1;
	}

	if (creatFS("test4.txt") == -1){
		return -1;
	}
	tagFS("test4.txt", "tag1");


	fprintf(stdout, "%s", "TEST 2: list files with the tag (test.txt, test2.txt and test4.txt)\n");
	ret = listFS("tag1", files);
	if (strcmp(files[0], "test.txt") != 0 || strcmp(files[1], "test2.txt") != 0 || strcmp(files[2], "test4.txt") != 0) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	if(untagFS("test2.txt", "tag1") == -1){
		return -1;
	}

	for (i = 0 ; i < 50 ; i++){
		files[i] = NULL;
	}

	fprintf(stdout, "%s", "TEST 3: list files with the tag (test.txt and test4.txt)\n");
	ret = listFS("tag1", files);
	if (strcmp(files[0], "test.txt") != 0 || strcmp(files[1], "test4.txt") != 0) {
		fprintf(stdout, "%s%s%s\n", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s\n", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}



	/* ----------------------------- Persistencia  ---------------------------- */
	fprintf(stdout, "%s", "-------- TESTS persistency of data -------\n");

	for (i = 0 ; i < 50 ; i++){
		files[i] = NULL;
	}

	fprintf(stdout, "%s", "TEST 1: unmount + mount + listFS\n");

	if (umountFS() == -1){
		return -1;
	}

	if (mountFS() == -1){
		return -1;
	}

	ret = listFS("tag1", files);
	if (strcmp(files[0], "test.txt") != 0 || strcmp(files[1], "test4.txt") != 0) {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	free(files);
	return 0;
}
