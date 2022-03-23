# DBC-qemu
DBC qemu, statistics gpu information

Based on [qemu](https://gitlab.com/qemu-project/qemu) stable version 6.1, the function of NVIDIA Management Library is integrated into the qemu guest agent, and the detailed information of the graphics card in the virtual machine is obtained by communicating with the virtual machine.

## Building
QEMU is multi-platform software intended to be buildable on all modern Linux platforms, OS-X, Win32 (via the Mingw64 toolchain) and a variety of other UNIX targets.

### linux
```
mkdir build
cd build
../configure --prefix=/usr/local --sysconfdir=/etc --localstatedir=/var
make

```
https://wiki.qemu.org/Hosts/Linux

### build for W64
Fedora supports both W64 and W32 cross builds.
```
dnf install mingw64-pixman mingw64-glib2 mingw64-gmp mingw64-SDL2 mingw64-pkg-config
mkdir build
cd build
../configure --cross-prefix=x86_64-w64-mingw32- --enable-guest-agent --enable-guest-agent-msi
make
```
https://wiki.qemu.org/Hosts/W32
