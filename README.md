# JerrOS 
### A simple x86 operating system
## About JerrOS
### How is it Written?
Because of my incompetence to achieve good programming skills, I had to resort to ChatGPT to write assembly code and pointer stuff (yes, I am still not good with pointers). However, JerrOS comes packed with a shell and a custom stdlib (if you have the patience to compile it properly), and I wrote the C for it :D. 
### Incomplete Stuff
Previously, I tried to add support for running ELF executables, but it failed miserably. At least it gave me a reason to have `jerrlib.a` even be a thing. That is why you see many "NOT FUNCTIONAL" comments in the source code.
### Is This all Just ChatGPT?
Sort of, but not quite. Here is a list of what ChatGPT did:
- Build stuff (Makefile, linker.ld, grub.cfg, etc.)
- Assembly code
- Pointers/Memory management in code
- Very specific hex values

That's only 4 things, meanwhile I'm over here writing a whole standard library. TL;DR **NO!**

## Running It

Running JerrOS is quite simple. Ensure you have [`qemu`](https://gitlab.com/qemu-project/qemu) installed (specifically the i386 version, because that is what JerrOS runs on):

### Debian
```bash
$ sudo apt install qemu-system-i386
```
### Arch
```bash
$ sudo pacman -S qemu
```
### Fedora
```bash
$ sudo dnf install qemu-system-i386
```
### macOS
```bash
$ brew install qemu
```
### Windows
get a linux computer.

--------------------
Once that is completed, we can make and run.
```bash
$ make
$ chmod +x run.sh
$ ./run.sh
```
And there you have it! JerrOS running in all its glory.