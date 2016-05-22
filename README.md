# File System

### What is it?
The <a href="https://en.wikipedia.org/wiki/File_system">File System</a> is the part of the Operating System in charge of organizing and dealing with folders and files. In order to do so, it needs to manage not only the data, but also the metadata of every folder or file.

### What is in the repository?
The files contained with the repository are the following ones:

#### 1. create_disk
File that checks if the file <i>disk.dat</i> exist and can be open. This file will simulate a real hard disk.

#### 2. disk.dat
File that simulates a real hard disk. It is full of zeroes.

#### 3. filesystem
File where the file system is implemented. It has the following <b>properties</b>:

<b>1.</b> It can contain up to 50 files.<br>
<b>2.</b> The maximum file name could contain up to 64 characters.<br>
<b>3.</b> The maximum file size is 4KB.<br>
<b>4.</b> The file system block size is 4KB.<br>
<b>5.</b> There could be up to 30 tags into the file system.<br>
<b>6.</b> The maximum tag name could contain up to 32 characters.<br>
<b>7.</b> Each file could have up to 3 associated tags.<br>

These characteristics give us an idea of the size of the file system. Although it is a small one, the concepts applied to its development should be applied into the development of largest file systems.

#### 4. Makefile
File to compile the other repository files. It can be also used to remove the compiled ones.
```shell
$ make
gcc -g -Wall -I./include -o create_disk create_disk.c
gcc -g -Wall -I./include   -c -o ufs.o ufs.c
gcc -g -Wall -I./include   -c -o filesystem.o filesystem.c
ar rcv libfs.a ufs.o filesystem.o
a - ufs.o
a - filesystem.o
gcc -g -Wall -I./include -o test test.c libfs.a
```

```shell
$ make clean 
rm -f libfs.a ufs.o filesystem.o test create_disk create_disk.o
```

#### 5. ufs
File that simulates the hard disk driver. It allow us to read and write entire blocks of data from the disk. Remember that the block size is 4KB.

### Possible operations

* <b>mkFS (maxNumFiles, deviceSize):</b> It will initialize all the metadata structures.<br>
<b>Parameters:</b> The maximum number of files and the total device size (between 320KB and 500KB).<br>
<br>
* <b>mountFS ( ):</b> It will simulate the mounting process into the Operating System current file system.<br>
<br>
* <b>umountFS ( ):</b> It will simulate the unmounting process into the Operating System current file system.<br>
<br>
* <b>creatFS (fileName):</b> It will create a file. If the file already exist it will return an error.<br>
<b>Parameters:</b> The name of the created file.<br>
<br>
* <b>openFS (fileName):</b> It will open a file in order to read or write on it.<br>
<b>Parameters:</b> The name of the file.<br>
<br>
* <b>closeFS (fileDescriptor):</b> It will close the specified file.<br>
<b>Parameters:</b> The name of the file.<br>
<br>
* <b>readFS (fileDescriptor, buffer, numBytes):</b> It will read the data of a given file from the reader/writer header up to the number of bytes specified, storing the result into the buffer.<br>
<b>Parameters:</b> The file descriptor, a buffer of at least 4KB, and the number of bytes to read.<br>
<br>
* <b>writeFS (fileDescriptor, buffer, numBytes):</b> It will write as many bytes as specified into the file, from the buffer.<br>
<b>Parameters:</b> The file descriptor, a buffer containing the content to be written and the number of bytes to write.<br>
<br>
* <b>lseekFS (fileDescriptor, offset, whence):</b> It will move the reading / writting header of a file to the specified position.<br>
<b>Parameters:</b> The file descriptor, the new position of the reading / writing header and the option {FS_SEEK_SET, FS_SEEK_BEGIN or FS_SEEK_END}.<br>
<br>
* <b>tagFS (fileName, tagName):</b> It will tag a file with the specified tag. If the tag does not exist, it will be created.<br>
<b>Parameters:</b> The file name and the tag name.<br>
<br>
* <b>untagFS (fileName, tagName):</b> It will remove the specified tag from the specified file. Returns and error if the tag was not previously associated to that file.<br>
<b>Parameters:</b> The file name and the tag name.<br>
<br>
* <b>listFS (tagName, files):</b> It will store into the <i>files</i> array all the files containing the specified tag.<br>
<b>Parameters:</b> The tag name and an an array of strings <i>(char**)</i>.


### Testing
There is an additional file with the repository, called <i>test.c</i>. It is where all the different possible operations for the file system are tested, checking if they do what they are supossed to do. <b>Every operation has been checked</b>. Additional advance tests could be added if needed.


### Requirements
This File System is designed to work in a UNIX operating system. The reasons of this requirement are two: First of all a GCC (GNU Compiler-Compiler) is needed to execute the Makefile and compile the files; and secondly, because the System Calls could have a different name in other operating systems. Those which fulfill the requirements are:

<b>A) Mac OSX.</b><br>
<b>B) Any Linux distribution.</b>
