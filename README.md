09.27.26 (test_1)

For now, how to build and use a few native function?

Needs: 
- Linux/macOS: g++ or clang with C++17, make, git
- Windows: MSYS2 UCRT64, g++ or clang with C++17, make, git
(If you got windows, open it ONLY IN MSYS32 UCRT64, not cmd, powershell or other)

1.  Download repo

git clone https://github.com/YOU/quickjs-orm.git
cd quickjs-orm

2. Build quickjs (you need make utility)

cd tests/quickJStests/quickjs
make
cd ../../.. (back to parent directory)

3. Little chek

./tests/quickJStests/quickjs/qjs -e "console.log('ok', 1+2)"

4. Build demo example (you need g++)

cd tests/quickJStests/_HLTEST

g++ -O2 -std=c++17 main.cpp \
    -I../quickjs \
    -L../quickjs \
    -lquickjs-libc -lquickjs \
    -lm -lpthread \
    -o myapp

5. Start

./myapp

6. Almost done, keep script.js near myapp.exe in same directory and write there what you want
The script.js (~/_HLTEST) file contains a test usage example
Below i was wrote about almost include libraries, and my native functions

AVAILABLE FUNCTIONS AND LIBRARIES:
    libraries:
        - qjs (basic JavaScript CLI-utility (console. and other))
    functions:
        - test_1:
            print(str) - console.log full analogue
            add(a, b) - simple addition of numbers in the range of the C++ int type
            multiply(a, b) - simple multiplication of numbers within the range of the C++ int type

If you want to modify any function, rewrite it in main.cpp (~/_HLTEST) and rebuild the project

Thanks for you attention!; return 0;