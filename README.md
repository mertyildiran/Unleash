# Unleash

Unleash your inner power using Unleash Shell

<p align="center">
  <img src="https://raw.githubusercontent.com/mertyildiran/Unleash/master/docs/img/super-hero2.png" alt="Unleash Your Power"/>
</p>

### Version

0.1.9

### Installation

On the maintainer’s system(*Make sure you have automake installed*):

```Shell
aclocal # Set up an m4 environment
autoconf # Generate configure from configure.ac
automake --add-missing # Generate Makefile.in from Makefile.am
./configure # Generate Makefile from Makefile.in
make distcheck # Use Makefile to build and test a tarball to distribute
```

On the end-user’s system:

```Shell
./configure # Generate Makefile from Makefile.in
make # Use Makefile to build the program
make install # Use Makefile to install the program
unleash
```

**CAUTION:** Unleash's development state currently on Alpha state. There are some minor memory leaking issues that haven't been fixed yet. Additionally, the developer of Unleash is not an expert on C programming. The program's itself developed as an educational material. So, don't use Unleash as your default shell!

### License

The MIT License (MIT)

Copyright (c) 2016 Mehmet Mert Yıldıran mert.yildiran@bil.omu.edu.tr

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
