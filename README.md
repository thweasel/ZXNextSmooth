# ZXNextSmooth
A library for making C developement on the ZXNext smooth

Going for a readable style over performant, the compiler can sort the mess out.


# Quick start

- configure `zpragma.inc`
- 


# Important files and folders

## zpragma.inc

Configure the pragmas for your project here

## main.c

The main file for your project

## /assets/
Put none code files in here, music, pictures, binary blobs etc...

## /build/
This is where the make stuff will happen

## /src/
Source code

## /src/project/
Put your .C files in here

## /src/project/include/
Put your .H files in here


## /src/ZXNextSmooth_lib

The library files, you only need to add ZXNextSmooth.h in main.c you should stayout of here unless you are updating the library...


# Important patterns

These are things you should be doing in your code...

## Header guards

```
#ifdef "HEADERFILENAME"_H
#define "HEADERFILENAME"_H

... code ...

#endif

```

## Method scopes

`static` localise methods or variables to files.

`static` can be used to persist variables inside a method.

`extern` can be used to reference variables/methods in other files

Localise methods by not adding them to headers that will be used externally