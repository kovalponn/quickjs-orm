#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

extern "C" {
    #include "quickjs.h"
    #include "quickjs-libc.h"
}

static JSValue js_print(JSContext *ctx, JSValueConst, int argc, JSValueConst *argv) {
    for (int i = 0; i < argc; i++) {
        const char *s = JS_ToCString(ctx, argv[i]);
        if (s) { printf("%s", s); JS_FreeCString(ctx, s); }
        if (i + 1 < argc) printf(" ");
    }
    printf("\n");
    return JS_UNDEFINED;
}

static JSValue js_add(JSContext *ctx, JSValueConst this_val,
                      int argc, JSValueConst *argv)
{
    if (argc < 2) {
        return JS_ThrowTypeError(ctx, "add() need 2 arguments");
    }

    int a, b;
    if (JS_ToInt32(ctx, &a, argv[0]) < 0) return JS_EXCEPTION;
    if (JS_ToInt32(ctx, &b, argv[1]) < 0) return JS_EXCEPTION;

    return JS_NewInt32(ctx, a + b);
}

static JSValue js_multiply(JSContext *ctx, JSValueConst this_val,
                      int argc, JSValueConst *argv)
{
    if (argc < 2) {
        return JS_ThrowTypeError(ctx, "multiply() need 2 arguments");
    }

    int a, b;
    if (JS_ToInt32(ctx, &a, argv[0]) < 0) return JS_EXCEPTION;
    if (JS_ToInt32(ctx, &b, argv[1]) < 0) return JS_EXCEPTION;

    return JS_NewInt32(ctx, a * b);
}

int main()
{
    JSRuntime *rt = JS_NewRuntime();
    if (!rt) { fprintf(stderr, "JS_NewRuntime failed\n"); return 1; }

    JSContext *ctx = JS_NewContext(rt);
    if (!ctx) { fprintf(stderr, "JS_NewContext failed\n"); return 1; }

    js_std_add_helpers(ctx, 0, nullptr);

    JSValue global = JS_GetGlobalObject(ctx);
    JS_SetPropertyStr(ctx, global, "add",
                      JS_NewCFunction(ctx, js_add, "add", 2));
    JS_FreeValue(ctx, global);

    JS_SetPropertyStr(ctx, global, "print",
                  JS_NewCFunction(ctx, js_print, "print", 1));

    JS_SetPropertyStr(ctx, global, "multiply",
                  JS_NewCFunction(ctx, js_multiply, "multiply", 2));


    std::string filename = "script.js";
    std::ifstream fs(filename, std::ios::binary);
    if (!fs.is_open()) {
        std::cerr << "program cannot open your script!";
        return 1;
    }
    std::string script(
        (std::istreambuf_iterator<char>(fs)),
        std::istreambuf_iterator<char>()
    );

    JSValue result = JS_Eval(ctx, script.c_str(), script.size(),
                             "<input>", JS_EVAL_TYPE_GLOBAL);

    if (JS_IsException(result)) {
        JSValue exc = JS_GetException(ctx);
        const char *s = JS_ToCString(ctx, exc);
        fprintf(stderr, "JS Exception: %s\n", s ? s : "(null)");
        JS_FreeCString(ctx, s);
        JS_FreeValue(ctx, exc);
    } else {
        int32_t val = 0;
        JS_ToInt32(ctx, &val, result);
        printf("Returned to C++: %d\n", val);
    }
    JS_FreeValue(ctx, result);

    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
    return 0;
}