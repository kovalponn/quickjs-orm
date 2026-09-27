# quickjs-orm (test_1)

A small test framework built on top of QuickJS, exposing a few native
C++ functions to JavaScript.

## Requirements

- **Linux / macOS:** g++ or clang with C++17, make, git
- **Windows:** MSYS2 UCRT64 with g++ or clang (C++17), make, git
  > On Windows, run all commands only in the MSYS2 UCRT64 shell —
  > not in cmd, PowerShell, or any other terminal.

## 1. Clone the repo

```bash
git clone https://github.com/kovalponn/quickjs-orm.git
cd quickjs-orm
```

## 2. Build QuickJS

```bash
cd tests/quickJStests/quickjs
make
```

Then verify the static libraries exist:

```bash
ls -la tests/quickJStests/quickjs/libquickjs*.a
```

You should see:
- `libquickjs.a`
- `libquickjs-libc.a`

### 2.1 If `libquickjs-libc.a` is missing

```bash
gcc -c -O2 -fPIC -I. quickjs-libc.c -o quickjs-libc.o
ar rcs libquickjs-libc.a quickjs-libc.o
ls -la libquickjs*.a
cd ../../..
```

### 2.2 If `libquickjs.a` is missing

```bash
gcc -c -O2 -fPIC -I. -DCONFIG_VERSION=\"$(cat VERSION)\" quickjs.c -o quickjs.o
gcc -c -O2 -fPIC -I. libregexp.c  -o libregexp.o
gcc -c -O2 -fPIC -I. libunicode.c -o libunicode.o
gcc -c -O2 -fPIC -I. cutils.c     -o cutils.o
ar rcs libquickjs.a quickjs.o libregexp.o libunicode.o cutils.o
cd ../../..
```

## 3. Quick check

```bash
./tests/quickJStests/quickjs/qjs -e "console.log('ok', 1+2)"
```

## 4. Build the demo example

```bash
cd tests/quickJStests/_HLTEST
g++ -O2 -std=c++17 main.cpp -I../quickjs -L../quickjs \
    -lquickjs-libc -lquickjs -lm -lpthread -o myapp
```

On Windows the output binary will be `myapp.exe`.

## 5. Run

```bash
./myapp
```

## 6. Script

Place `script.js` next to the built binary (`myapp` / `myapp.exe`).
The example `script.js` in `tests/quickJStests/_HLTEST` shows basic usage.

## Available libraries and native functions

**Libraries**
- `qjs` — basic JavaScript CLI utility (`console.*` and other)

**Functions (module `test_1`)**
- `print(str)` — same to `console.log`
- `add(a, b)` — adds two numbers within C++ `int` range
- `multiply(a, b)` — multiplies two numbers within C++ `int` range

To modify a function, edit `main.cpp` in `tests/quickJStests/_HLTEST`
and rebuild the project.

Thanks for your attention! return 0; }