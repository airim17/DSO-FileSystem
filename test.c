#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/filesystem.h"

#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"

int main() {
	int ret, i, fd;
	char bufferR[BLOCK_SIZE];
	char bufferW[BLOCK_SIZE];
	char **files = (char**) malloc(50*sizeof(char*));


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


	/* From now on we need to have the file system mounted */
	if (mountFS() == -1){
		return -1;
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


	fprintf(stdout, "%s", "TEST 3: creatFS with a name of length 0\n");
	ret = creatFS("");
	if (ret != -1) {
		fprintf(stdout, "%s%s%s%s", "TEST 3: creatFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 3: creatFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 4: creatFS with a long name\n");
	ret = creatFS("loooooooooooooooooooooooooooooooooooooooooooooooooooooooooong.txt");
	if (ret != -1) {
		fprintf(stdout, "%s%s%s%s\n", "TEST 4: creatFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s\n", "TEST 4: creatFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
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
	fd = openFS("test.txt");
	if (fd == -1 || fd == -2) {
		fprintf(stdout, "%s%s%s%s", "TEST 3: openFS #1 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 3: openFS #1 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	fd = openFS("test.txt");
	if (fd == -1 || fd == -2) {
		fprintf(stdout, "%s%s%s%s", "TEST 3: openFS #2 ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 3: openFS #2 ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	ret = closeFS(fd);
	if (ret != 0) {
		fprintf(stdout, "%s%s%s%s", "TEST 3: closeFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 3: closeFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 4: closeFS an already closed file\n");
	ret = closeFS(3);
	if (ret != 0) {
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


	fprintf(stdout, "%s", "TEST 1: lseek without opening the file\n");
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
		fprintf(stdout, "%s%s%s%s\n", "TEST 4: lseekFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s\n", "TEST 4: lseekFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	closeFS(fd);



	/* --------------------------------- tagFS  ------------------------------- */
	fprintf(stdout, "%s", "-------- TESTS tagFS -------\n");

	fd = openFS("test.txt");

	fprintf(stdout, "%s", "TEST 1: tag with an open file\n");
	ret = tagFS("test.txt", "tag1");
	if (ret != -1) {
		fprintf(stdout, "%s%s%s%s", "TEST 1: tagFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 1: tagFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	closeFS(fd);


	fprintf(stdout, "%s", "TEST 2: tag a new valid tag\n");
	ret = tagFS("test.txt", "tag1");
	if (ret != 0) {
		fprintf(stdout, "%s%s%s%s", "TEST 2: tagFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 2: tagFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	fprintf(stdout, "%s", "TEST 3: tag a an already existing tag\n");
	ret = tagFS("test.txt", "tag1");
	if (ret != 1) {
		fprintf(stdout, "%s%s%s%s", "TEST 3: tagFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 3: tagFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
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
		fprintf(stdout, "%s%s%s%s\n", "TEST 4: tagFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s\n", "TEST 4: tagFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}



	/* ---------------------------- tagFS + untagFS  -------------------------- */
	fprintf(stdout, "%s", "-------- TESTS tagFS + untagFS -------\n");

	fprintf(stdout, "%s", "TEST 1: untag a NON existing tag\n");
	ret = untagFS("test.txt", "tag5");
	if (ret != -1) {
		fprintf(stdout, "%s%s%s%s", "TEST 1: untagFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 1: untagFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	fprintf(stdout, "%s", "TEST 2: untag an existing tag\n");
	ret = untagFS("test.txt", "tag1");
	if (ret != 0) {
		fprintf(stdout, "%s%s%s%s", "TEST 2: untagFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 2: untagFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}

	fprintf(stdout, "%s", "TEST 3: tag after being untag another one\n");
	ret = tagFS("test.txt", "tag1");
	if (ret != 0) {
		fprintf(stdout, "%s%s%s%s\n", "TEST 3: untagFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s\n", "TEST 3: untagFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}


	/* --------------------------------- listFS  ------------------------------ */
	fprintf(stdout, "%s", "-------- TESTS listFS -------\n");

	fprintf(stdout, "%s", "TEST 1: list files with a tag that do not exist\n");
	ret = listFS("test.txt", files);
	if (ret != -1) {
		fprintf(stdout, "%s%s%s%s", "TEST 1: listFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 1: listFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
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
		fprintf(stdout, "%s%s%s%s", "TEST 2: listFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 2: listFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
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
		fprintf(stdout, "%s%s%s%s\n", "TEST 3: listFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s\n", "TEST 3: listFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}



	/* ----------------------------- Persistencia  ---------------------------- */
	fprintf(stdout, "%s", "-------- TESTS persistency of data -------\n");

	for (i = 0 ; i < 50 ; i++){
		files[i] = NULL;
	}

	fprintf(stdout, "%s", "TEST 1: unmount, mount and listFS\n");

	if (umountFS() == -1){
		return -1;
	}

	if (mountFS() == -1){
		return -1;
	}

	ret = listFS("tag1", files);
	if (strcmp(files[0], "test.txt") != 0 || strcmp(files[1], "test4.txt") != 0) {
		fprintf(stdout, "%s%s%s%s", "TEST 1: listFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
	}
	else {
		fprintf(stdout, "%s%s%s%s", "TEST 1: listFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);
	}
}
