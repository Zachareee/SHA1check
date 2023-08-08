# SHA1check
**Check your game installations against SHA1 hashsums from SteamDB**

## Building:
### For machines with `make` installed
Use `make` to build without debug symbols

Use `make debug` to build with address sanitizer and debug symbols

### For command prompt users (Windows)
Use `./build.bat` or double click the `build.bat` file

### For bash users
Use `./build.sh` to build

Use `./build.sh debug` to build with address sanitizer and debug symbols

## Usage:
`SHA1check FOLDERPATH [-s HASHFILENAME] [-d RESULTFILENAME]`

`SHA1check.exe FOLDERPATH [-s HASHFILENAME] [-d RESULTFILENAME]` (for Windows, you can also drag and drop the folder onto the executable)

Supply the executable with the following:
### Positional (Required)
1. `FOLDERPATH` : The directory in which the files exist
### Options
1. `-h` : display this help message
1. `-s` : source file i.e. file containing hashes in "FILE  HASHSUM" format
1. `-d` : destination file
1. `-v` : display current version

>HASHPATH default: FOLDERPATH/hashes.sha1
>
>RESULTPATH default: FOLDERPATH/checksum.txt
>
>Both -s and -d uses relative path to FOLDERPATH. To specify absolute path, 
>prefix paths with ':' e.g. :/C:/Program Files/file.txt
>FOLDERPATH always uses absolute path, do not use ':'

## Supported OS
The current source supports Linux and Windows, plans are in place to make it cross-platform (Latest commits yet to be tested on macOS)

## Dependencies
On Unix systems, `openssl/evp.h` is required, found in package `libssl-dev`