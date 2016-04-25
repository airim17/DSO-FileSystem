/* This file contains the definition of the functions that must be implemented
 * to allow the user access to the file system and the files.
 */

#include "include/filesystem.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

typedef struct tag {
	unsigned char ID;
	char name [32];
	unsigned char repetitions;
	unsigned char files;

};

typedef struct inode {
	char name [64];
	unsigned short size;
	unsigned short filePointer;
	unsigned char directBlock;
	unsigned char tags [3];

};

typedef struct superblock {
	unsigned long bitmap;
	struct tag tagsMap [30];
	unsigned char inodeSize;
	unsigned char numberINodes;
	unsigned char firstFreeBlock;
	unsigned char firstDataBlock;
	unsigned char maximumFiles;
	unsigned int deviceSize;
	unsigned int magicNumber;

};



struct superblock superBlock;
char * deviceName = "disk.dat";

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
	memset(zero, 0, BLOCK_SIZE);

	int error, i;
	for (i = 0 ; i < ceil(deviceSize / BLOCK_SIZE) ; i++){
		error = bwrite(deviceName, i, zero);
		if (error == -1){
			return -1;
		}
	}

	// Initializing some fields of the superBlock <----------------TODO REVISAR
	superBlock.inodeSize = sizeof (struct inode);
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

	char sblock [BLOCK_SIZE];

	// Checking if reading the superblock produces any error
	if (bread(deviceName, 0, sblock) == -1){
		return -1;
	}

	// If not, the information is stored in the superBlock
	memcpy(&superBlock, sblock, sizeof(superBlock));
	return 0;

}

/*
 * Unmount file system.
 * Returns 0 if the operation was correct or -1 in case of error.
 */
int umountFS() {

	// Preapering the block that is going to be writen, including the padding
	char sblock[BLOCK_SIZE];
	memset(sblock, 0, BLOCK_SIZE);
	memcpy(&superBlock, sblock, sizeof(superBlock));

	// Writting the superBlock into the first block of the disk
	if (bwrite(deviceName, 0, sblock) == -1){
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

	char block [BLOCK_SIZE];
	int i;

	// Checking if there is another inode with the same name
	for (i = 1 ; i < superBlock.firstDataBlock ; i++){

		if (bread(deviceName, i, block) == -1){
			return -1;
		}

		for (i = 0 ; i < floor(BLOCK_SIZE / superBlock.inodeSize) ; i++){

			if (fileName == block[i * superBlock.inodeSize]){
				return -1;
			}
		}
	}

	struct inode newNode;
	memcpy(newNode.name, fileName, sizeof(fileName));
	newNode.directBlock = superBlock.firstFreeBlock;

	// TODO GUARDAR EL INODO EN EL DISCO

	superBlock.firstFreeBlock = superBlock.firstFreeBlock + 1;
	superBlock.numberINodes = superBlock.numberINodes + 1;

	return 0;
}

/*
 * Opens an existing file.
 * Returns file descriptor if possible, -1 if file does not exist or -1 in case
 * of error.
 */
int openFS(char *fileName) {

	char block [BLOCK_SIZE];
	int i;

	// Checking if there is any inode with that name
	for (i = 1 ; i < superBlock.firstDataBlock ; i++){

		if (bread(deviceName, i, block) == -1){
			return -1;
		}

		for (i = 0 ; i < floor(BLOCK_SIZE / superBlock.inodeSize) ; i++){

			if (fileName == block[i * superBlock.inodeSize]){
				return (superBlock.firstDataBlock + block[(i * superBlock.inodeSize) + 78]);
			}
		}
	}
	return 0;
}

/*
 * Closes a file.
 * Returns 0 if the operation was correct or -1 in case of error.
 */
int closeFS(int fileDescriptor) {
	return 0;
}

/*
 * Reads a number of bytes from a file and stores them in a buffer.
 * Returns the number of bytes read or -1 in case of error.
 */
int readFS(int fileDescriptor, void *buffer, int numBytes) {
	return 0;
}

/*
 * Reads number of bytes from a buffer and writes them in a file.
 * Returns the number of bytes written, 0 in case of end of file or -1 in case
 * of error.
 */
int writeFS(int fileDescriptor, void *buffer, int numBytes) {
	return 0;
}


/*
 * Repositions the pointer of a file. A greater offset than the current size, or
 * an offset lower than 0 are considered errors.
 * Returns new position or -1 in case of error.
 */
int lseekFS(int fileDescriptor, long offset, int whence) {
	return 0;
}

/**********************/
/* Version management */
/**********************/

/*
 * Tags a file with the given tag name. Returns 0 if the operation is
 * successful, 1 if the file already had that tag or -1 in case of error.
 */
int tagFS(char *fileName, char *tagName) {
	return 0;
}

/*
 * Removes a tag from a file. Returns 0 if the operation is successful, 1 if
 * the tag wasn't associated to the file or -1 in case of error.
 */
int untagFS(char *fileName, char *tagName) {
	return 0;
}

/*
 * Looks for all files tagged with the tag tagName, and stores them in the list
 * passed as a parameter. Returns the number of tagged files found or -1 in
 * case of error.
 */
int listFS(char *tagName, char **files) {
	return 0;
}
