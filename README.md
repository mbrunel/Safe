# Safe
## Local password manager for Linux

### A macos support is coming

## Building

- ./configure
- make : if you only want the executable
- make install : if you want to install the package
- make uninstall : if you want to remove the package

## Usage

at first use it will create a .safe directory in you $HOME repository

if you want to keep the same passwords accross computers you can copy the .safe file on each new computer's $HOME

everything else is pretty intuitive as you use it

## Features

- Use openssl and your password to crypt the entirety of the .safe file
- multiples users
- compile from sources
- Written in C

## Build with

* SDL2
* SDL2-image
* openssl
* [vurtun/nuklear](https://github.com/vurtun/nuklear)

## License

see [License](License)
