# SHA1check
**A tool to check file hashsums**

## Building:
Use `make` to build without debug symbols

Use `make debug` to build with address sanitizer and debug symbols

## Usage:
`main FOLDERPATH [-s HASHFILENAME] [-d RESULTFILENAME]`

Supply the executable with the following:
### Positional (Required)
1. `FOLDERPATH` : The directory in which the files exist
### Options
1.  `-h` : display this help message
1. `-s` : source file i.e. file containing hashes in "FILE  HASHSUM" format
1. `-d` : destination file

>HASHPATH default: FOLDERPATH/hashes.sha1
>
>RESULTPATH default: FOLDERPATH/checksum.txt
>
>Both -s and -d uses relative path to FOLDERPATH. To specify absolute path, 
>prefix paths with ':' e.g. :/C:/Program Files/file.txt
>FOLDERPATH always uses absolute path, there is no need for ':'

## Supported OS
The current source supports Linux ONLY, plans are in place to make it cross-platform (Mainly for Windows)