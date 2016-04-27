/* This file contains the definition of the functions that must be implemented
 * to allow the user access to the file system and the files.
 */

#include "include/filesystem.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

/* Types of structs */
typedef struct tag {
	unsigned char ID;
	char name [32];
	unsigned char counter;
	unsigned char files;

} tag;

typedef struct inode {
	char name [64];
	unsigned short filePointer;
	unsigned short size;
	unsigned char directBlock;
	unsigned char open;
	unsigned char tags [3];

} inode;

typedef struct sblock {
	struct tag tagsMap [30];
	unsigned char numberINodes;
	unsigned char firstDataBlock;
	unsigned char maximumFiles;
	unsigned int deviceSize;
	unsigned int magicNumber;

} sblock;

/* Global variables */
sblock superBlock;
inode inodes[50];


/***************************/
/* File system management. */
/***************************/

/*
 * Formats a device.
 * Returns 0 if the operation was correct or -1 in case of error.
 */
int mkFS(int maxNumFiles, long deviceSize) {

	// Checking if the number of maximum files is correct (1-50)
	if (maxNumFiles < 1 || maxNumFiles > 50){
		return -1;
	}

	// Checking if the size of the device is correct (320KB - 500KB)
	if (deviceSize < 327680 || deviceSize > 512000){
		return -1;
	}

	// Formating the device writting zeroes
	char zero [BLOCK_SIZE];
	memset(zero, '0', BLOCK_SIZE);

	int i;
	for (i = 0 ; i < ceil(deviceSize / BLOCK_SIZE) ; i++){
		if (bwrite(DEVICE_IMAGE, i, zero) == -1){
			return -1;
		}
	}


	// Initializing some fields of the superBlock 																// TODO REVISAR
	superBlock.firstDataBlock = ceil((maxNumFiles * sizeof(struct inode)) / 4096) + 1;
	superBlock.maximumFiles = (unsigned char) maxNumFiles;
	superBlock.deviceSize = (int) deviceSize;
	superBlock.magicNumber = (int) 317201;

	return 0;
}

/*
 * Mounts a file system from the device deviceName.
 * Returns 0 if the operation was correct or -1 in case of error.
 */
int mountFS() {

	char block [BLOCK_SIZE];

	// Checking if reading the superblock produces any error
	if (bread(DEVICE_IMAGE, 0, block) == -1){
		return -1;
	}

	memcpy(&superBlock, block, sizeof(superBlock));

	// Checking if reading the superblock produces any error
	if (bread(DEVICE_IMAGE, 1, block) == -1){
		return -1;
	}

	memcpy(&inodes, block, sizeof(inode) * 50);
	return 0;
}

/*
 * Unmount file system.
 * Returns 0 if the operation was correct or -1 in case of error.
 */
int umountFS() {

	// Checking if there are open files
	int i;
	for (i = 0 ; i < superBlock.numberINodes ; i++){
		if (inodes[i].open == 1){
			return -1;
		}
	}

	char block[BLOCK_SIZE];
	memset(block, '0', BLOCK_SIZE);
	memcpy(block, &superBlock, sizeof(block));

	// Writting the superBlock into the first block
	if (bwrite(DEVICE_IMAGE, 0, block) == -1){
		return -1;
	}

	memset(block, '0', BLOCK_SIZE);
	memcpy(block, &inodes, sizeof(inode) * 50);

	// Writting the array of i-nodes into the second block
	if (bwrite(DEVICE_IMAGE, 1, block) == -1){
		return -1;
	}

	return 0;

}


/*******************/
/* File read/write */
/*******************/

/*
 * Creates a new file, if it doesn't exist.
 * Returns 0 if a new file is created, 1 if the file already exists or -1 in
 * case of error.
 */
int creatFS(char *fileName) {

	// Checking if there is enough space for another inode
	if (superBlock.numberINodes == superBlock.maximumFiles){
		return -1;
	}

	// Checking if the inode name has the proper lenght
	if (sizeof(fileName) > 64){
		return -1;
	}

	// Checking if there is another inode with the same name
	int i;
	for (i = 0 ; i < superBlock.numberINodes ; i++){
		if (inodes[i].name == fileName){
			return -1;
		}
	}

	// Updating the information of the superBlock and the inode
	superBlock.numberINodes = superBlock.numberINodes + 1;
	strncpy(inodes[superBlock.numberINodes-1].name, fileName, 64);
	inodes[superBlock.numberINodes-1].directBlock = superBlock.numberINodes+1;

	return 0;
}

/*
 * Opens an existing file.
 * Returns file descriptor if possible, -1 if file does not exist or -1 in case
 * of error.
 */
int openFS(char *fileName) {

	int i;
	for (i = 0 ; i < superBlock.numberINodes ; i++){

		if (inodes[i].name == fileName){
			inodes[i].open = 1;
			inodes[i].filePointer = 0;
			return inodes[i].directBlock;
		}
	}

	return -1;
}

/*
 * Closes a file.
 * Returns 0 if the operation was correct or -1 in case of error.
 */
int closeFS(int fileDescriptor) {

	if (inodes[fileDescriptor-2].open == 1){
		inodes[fileDescriptor-2].open = 0;
		return 0;
	}

	return -1;
}

/*
 * Reads a number of bytes from a file and stores them in a buffer.
 * Returns the number of bytes read or -1 in case of error.
 */
int readFS(int fileDescriptor, void *buffer, int numBytes) {

	// Checking if the file is open
	if (inodes[fileDescriptor-2].open == 0){
		return -1;
	}

	// Checking if the number of bytes is valid
	int offset = inodes[fileDescriptor-2].filePointer;
	if (numBytes < 0 || numBytes > MAX_FILE_SIZE){
		return -1;
	}

	// Reading data from the disk
	char block[BLOCK_SIZE];
	if (bread(DEVICE_IMAGE, fileDescriptor, block) == -1){
		return -1;
	}

	// If the number of bytes to read is higher than the number of remaining ones
	if (offset + numBytes > inodes[fileDescriptor-2].size){
			memcpy(buffer, &(block[offset]), inodes[fileDescriptor-2].size - offset);
			inodes[fileDescriptor-2].filePointer = inodes[fileDescriptor-2].size;
			return inodes[fileDescriptor-2].size - offset;
	}

	// If it is the common behaviour
	memcpy(buffer, &(block[offset]), numBytes);																		// TODO REVISAR
	inodes[fileDescriptor-2].filePointer = offset + numBytes;
	return numBytes;
}

/*
 * Reads number of bytes from a buffer and writes them in a file.
 * Returns the number of bytes written, 0 in case of end of file or -1 in case
 * of error.
 */
int writeFS(int fileDescriptor, void *buffer, int numBytes) {

	// Checking if the file is open
	if (inodes[fileDescriptor-1].open == 0){
		return -1;
	}

	// Checking if the number of bytes is valid
	int offset = inodes[fileDescriptor-2].filePointer;
	if (numBytes < 0 || (offset + numBytes) > MAX_FILE_SIZE){
		return -1;
	}

	// Checking if the filepointer is just at the end
	if (offset == MAX_FILE_SIZE){
		return 0;
	}

	// Writting data into the disk
	char block[BLOCK_SIZE];
	if (bwrite(DEVICE_IMAGE, fileDescriptor, buffer) == -1){
		return -1;
	}

	memcpy(&(buffer[offset]), block, numBytes);																		// TODO REVISAR
	inodes[fileDescriptor-2].filePointer = offset + numBytes;
	if (offset + numBytes > inodes[fileDescriptor-2].size){
		inodes[fileDescriptor-2].size = offset + numBytes;
	}

	return numBytes;
}


/*
 * Repositions the pointer of a file. A greater offset than the current size, or
 * an offset lower than 0 are considered errors.
 * Returns new position or -1 in case of error.
 */
int lseekFS(int fileDescriptor, long offset, int whence) {

	if (offset < 0 || offset > inodes[fileDescriptor-2].size){
		return -1;
	}

	if (whence == 0){
		inodes[fileDescriptor-2].filePointer = offset;
	}
	else if (whence == 1){
		inodes[fileDescriptor-2].filePointer = 0;
	}
	else if (whence == 2){
		inodes[fileDescriptor-2].filePointer = MAX_FILE_SIZE;
	}

	return offset;
}

/**********************/
/* Version management */
/**********************/

/*
 * Tags a file with the given tag name. Returns 0 if the operation is
 * successful, 1 if the file already had that tag or -1 in case of error.
 */
int tagFS(char *fileName, char *tagName) {

	if (sizeof(tagName) > 32 || sizeof(fileName) > 64){
		return -1;
	}

	int i, fileExist = 0, numINode;
	for (i = 0 ; i < superBlock.numberINodes ; i++){
		if (inodes[i].name == fileName){
			fileExist = 1;
			numINode = i;
		}
	}

	if (fileExist == 0){
		return -1;
	}

	// If there is already a tag with that name: its counter is increase
	int tagID = 0;
	int found = 0;
	for (i = 0 ; i < 30 ; i++){
		if (strcmp(superBlock.tagsMap[i].name, tagName)){
			superBlock.tagsMap[i].counter = superBlock.tagsMap[i].counter + 1;
			superBlock.tagsMap[i].files = superBlock.tagsMap[i].files + pow(2, numINode);
			found = 1;
			tagID = i;
		}
	}

	// If there is not any tag with that name: is created
	if (found == 0){
		for (i = 0 ; i < 30 ; i++){
			if (strcmp (superBlock.tagsMap[i].name, "FREE") && superBlock.tagsMap[i].counter == 0){
				superBlock.tagsMap[i].ID = i+1;
				strncpy(superBlock.tagsMap[i].name, tagName, 32);
				superBlock.tagsMap[i].counter = 1;
				superBlock.tagsMap[i].files = superBlock.tagsMap[i].files + pow(2, numINode);
				tagID = i;
			}
		}
	}

	int tagSpace = -1, error = 0;
	for (i = 0 ; i < 3 ; i++){
		if (inodes[numINode].tags[i] == 0){
			tagSpace = i;
		}
		if (inodes[numINode].tags[i] == tagID){
			error = 1;
		}
	}

	if (error == 1){
		return 1;
	}
	else if (tagSpace == -1){
		return -1;
	}
	else {
		inodes[numINode].tags[tagSpace] = tagID;
	}

	return 0;
}

/*
 * Removes a tag from a file. Returns 0 if the operation is successful, 1 if
 * the tag wasn't associated to the file or -1 in case of error.
 */
int untagFS(char *fileName, char *tagName) {

	if (sizeof(tagName) > 32 || sizeof(fileName) > 64){
		return -1;
	}

	int fileExist = 0, numINode, i;
	for (i = 0 ; i < superBlock.numberINodes ; i++){
		if (inodes[i].name == fileName){
			fileExist = 1;
			numINode = i;
		}
	}

	if (fileExist == 0){
		return -1;
	}

	int tagID = 0;
	// If there is already a tag with that name: its counter is decrease
	int found = 0;
	for (i = 0 ; i < 30 ; i++){
		if (strcmp(superBlock.tagsMap[i].name, tagName)){
			superBlock.tagsMap[i].counter = superBlock.tagsMap[i].counter - 1;
			superBlock.tagsMap[i].files = superBlock.tagsMap[i].files - pow(2, numINode);
			if (superBlock.tagsMap[i].counter == 0){
				strncpy(superBlock.tagsMap[i].name, "FREE", 32);												// TODO ¿Qué se considera eliminar?
			}
			found = 1;
			tagID = i;
		}
	}

	// If there is not a tag with that name: error
	if (found == 0){
		return -1;
	}

	int tagFound = 0;
	for (i = 0 ; i < 3 ; i++){
		if (inodes[numINode].tags[i] == tagID){
			inodes[numINode].tags[i] = 0;
			tagFound = 1;
		}
	}

	if (tagFound == 0){
		return -1;
	}

	return 0;
}

/*
 * Looks for all files tagged with the tag tagName, and stores them in the list
 * passed as a parameter. Returns the number of tagged files found or -1 in
 * case of error.
 */
int listFS(char *tagName, char **files) {

	if (sizeof(tagName) > 32){
		return -1;
	}

	// If there is already a tag with that name: its counter is decrease
	int i, found = 0, remain;
	for (i = 0 ; i < 30 ; i++){
		if (superBlock.tagsMap[i].name == tagName){
			remain = superBlock.tagsMap[i].files;
			found = 1;
		}
	}

	if (found == 0){
		return -1;
	}

	char filesMap [superBlock.numberINodes];
	int index = superBlock.numberINodes-1;
	for (i = 0 ; remain > 0 ; i++){
		filesMap[index] = remain % 2;
		remain = floor(remain / 2);
		index--;
	}

	for (i = 0 ; i < superBlock.numberINodes ; i++){
		if (filesMap[i] == 1){
			files[i] = inodes[i].name;
		}
	}

	return 0;
}
