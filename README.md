# jpec - a JPEG encoder in C

## Overview

jpec is a minimalistic JPEG encoder written in C with a focus on **simplicity**.

It is both **portable** (tested on x86 and ARM) and **lightweight** (around 600 LOC).

The library has no external dependencies.

It is a perfect candidate if you have some embedded C code you need to extend with a JPEG encoder the easy way.

## Features

The library allows you to write JPEG compressed images from input image data on memory. No more, no less.

More precisely:

* it works in **grayscale only** (monochrome JPEG file): there is no support for color so far,
* it produces baseline, DCT-based (SOF0), JFIF 1.01 (APP0) JPEG-s,
* it supports **8x8 blocks only**,
* it includes default quantization and Huffman tables that are not customizable at runtime.

## Build

Just type:

    make

This produces:

* `libjpec.a`: the static library,
* `jpec`: a simple command-line tool used to test the library (see `main.c`)

> **NOTE** the command-line tool depends on OpenCV 2 HighGUI module to easily read any kind of input image. You need to have OpenCV development libraries installed on your machine to use it (e.g. `brew install opencv` on Mac OS X with Homebrew).

## Usage

If you want to embed the library into your own C code you are free to create the static library file, and include `jpec.h` main header, or to copy-paste the whole code into your project.

Then, encoding a raw image is a matter of a few lines of code:

```C
/* Create a JPEG encoder provided image data */
jpec_enc_t *e = jpec_enc_new(img, w, h);

/* Compress */
int len;
const uint8_t *jpeg = jpec_enc_run(e, &len);

/* Do something with the JPEG blob (e.g. save into a file, etc) */

/* Release the encoder */
jpec_enc_del(e);
```

## Authors

[Maxime Brénon](https://github.com/mbrenon) & [Cédric Deltheil](http://about.me/deltheil).

## Copyright

Copyright (c) 2012-2016 Moodstocks SAS
