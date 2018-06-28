Vector BLF to MATLAB/Simulink VNT Logfiles Converter
====================================================

Use this tool to convert CAN frame log (without a DBC file) into a MAT file that can be read
by the MATLAB/Simulink Vehicle Networking Toolbox CAN Replay block. The two formats are CAN
frame based, meaning there is no information about signals. You will need a DBC file or another
signal description inside your CAN Unpack blocks.

# Required software

* Boost Libraries (tested with 1.67.0): [[boost_1_67_0-msvc-14.1-64.exe](https://dl.bintray.com/boostorg/release/1.67.0/binaries/boost_1_67_0-msvc-14.1-64.exe)]
* BLF Reader by Tobias Lorenz: https://bitbucket.org/tobylorenz/vector_blf
* MATLAB MAT File I/O Library: http://matio.sf.net

# How to build

1. Open Visual Studio solution file
2. Check include and lib folders and adjust them to your installation
3. Build x64

# License

Copyright (c) 2018 Stefan Urban

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject
to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
