# Z80-LKM

This tiny project allows you to build the [Z80 library](https://github.com/redcode/Z80) as a Linux kernel module.

# Build

### Prerequisites

* CMake v3.14 or later.
* Linux kernel headers.

### Configuration options

The CMake script supports the following potions:

* <span id="option_Z80_LKM_STABLE">**<code>-DLINUX_KERNEL_HEADERS_DIR=<path></code>**</span>  
	Specify the directory of the Linux kernel headers.  
	If this optionnot defined, the directory will be autodetected.  
	Not defined by default.

* <span id="option_Z80_LKM_STABLE">**<code>-DZ80_LKM_STABLE=(YES|NO)</code>**</span>  
	Download the latest stable source code packages of Z80 and Zeta rather than clone the master branch of their respective Git repositories.  
	The default is `NO`.

### Build

```shell
git clone https://github.com/redcode/Z80-LKM.git
mkdir Z80-LKM/build
cd Z80-LKM/build
cmake .. [options]
make
```

# License

<img src="https://www.gnu.org/graphics/gplv3-127x51.png" align="right">

Copyright © 2022 Manuel Sainz de Baranda y Goñi.  

This program is [free software](https://www.gnu.org/philosophy/free-sw.html): you can redistribute it and/or modify it under the terms of the **[GNU General Public License](https://www.gnu.org/licenses/gpl-3.0.en.html)** as published by the [Free Software Foundation](https://www.fsf.org), either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but **WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE**. See the GNU General Public License for more details.

You should have received a [copy](COPYING) of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.
