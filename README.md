# Presuff
Prepend or append a string to the name of all files in a directory with specified extension.

## Dependencies
* GCC
* GNU make
* gzip

## Installation
Compile the project
```
make
```
Test functionality (optional)
```
make test
```
Install the manpage and compiled binary
```
sudo make install
```

### Make Targets 
- `make` - Compile the binary
- `make test` – Run test suite
- `make install` – Copy binary and manpage to system directories
- `make clean` – Remove build objects
- `make fclean` - Remove build objects and binary

## Usage
```
presuff <-a|-b> [-d DIRECTORY] -p PATTERN [-x EXTENSION]
```

### Options
```
-a                      Append pattern to basenames
-b                      Prepend pattern to basenames
-d <directory>          Specify directory to perform renames in
-h                      Display program usage
-p <pattern>            Specify pattern to prepand/append
-x <extension>          Specify extension of files to perform renaming on
```

## License
The MIT License (MIT)

Copyright (c) 2025 Jacob Niemeir
