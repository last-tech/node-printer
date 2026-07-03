#ifndef NODE_PRINTER_SRC_MACROS_H
#define NODE_PRINTER_SRC_MACROS_H

#include <nan.h>

#define MY_NODE_MODULE_ISOLATE_DECL v8::Isolate* isolate = v8::Isolate::GetCurrent();
#define MY_NODE_MODULE_ISOLATE      isolate
#define MY_NODE_MODULE_HANDLESCOPE MY_NODE_MODULE_ISOLATE_DECL Nan::HandleScope scope
#define V8_VALUE_NEW(type, value)   v8::type::New(MY_NODE_MODULE_ISOLATE, value)
#define V8_VALUE_NEW_DEFAULT(type)   v8::type::New(MY_NODE_MODULE_ISOLATE)
#define V8_STRING_NEW_UTF8(value)   v8::String::NewFromUtf8(MY_NODE_MODULE_ISOLATE, value).ToLocalChecked()
#define V8_STRING_NEW_2BYTES(value)   v8::String::NewFromTwoByte(MY_NODE_MODULE_ISOLATE, value).ToLocalChecked()

#define RETURN_EXCEPTION(msg)  isolate->ThrowException(Nan::Error(msg));    \
    return

#define RETURN_EXCEPTION_STR(msg) RETURN_EXCEPTION(V8_STRING_NEW_UTF8(msg))
#define MY_NODE_MODULE_RETURN_VALUE(value)   iArgs.GetReturnValue().Set(value);   \
    return
#define MY_NODE_MODULE_RETURN_UNDEFINED()   return

#define V8_LOCAL_STRING_FROM_VALUE(value) value->ToString(Nan::GetCurrentContext()).FromMaybe(v8::Local<v8::String>())
#define REQUIRE_ARGUMENT_INTEGER(args, i, var)                             \
    int var;                                                                   \
    if (args[i]->IsInt32()) {                                             \
        var = args[i]->Int32Value(Nan::GetCurrentContext()).FromJust();                                           \
    }                                                                          \
    else {                                                                     \
        RETURN_EXCEPTION_STR("Argument " #i " must be an integer");                 \
    }

#define MY_MODULE_SET_METHOD(exports, name, method) Nan::SetMethod(exports, name, method)
#define MY_NODE_MODULE_CALLBACK(name) void name(const Nan::FunctionCallbackInfo<v8::Value>& iArgs)

#define V8_STR_CONC(left, right)                              \
	v8::String::Concat(V8_STRING_NEW_UTF8(left), V8_STRING_NEW_UTF8(right))
		
#define REQUIRE_ARGUMENTS(args, n)                                                   \
    if (args.Length() < (n)) {                                                 \
        RETURN_EXCEPTION_STR("Expected " #n " arguments");                       \
    }


#define REQUIRE_ARGUMENT_EXTERNAL(i, var)                                      \
    if (args.Length() <= (i) || !args[i]->IsExternal()) {                      \
        RETURN_EXCEPTION_STR("Argument " #i " invalid");       \
    }                                                                          \
    v8::Local<v8::External> var = v8::Local<v8::External>::Cast(args[i]);

#define REQUIRE_ARGUMENT_OBJECT(args, i, var)                                      \
    if (args.Length() <= (i) || !args[i]->IsObject()) {                      \
        RETURN_EXCEPTION_STR("Argument " #i " is not an object");       \
    }                                                                          \
    v8::Local<v8::Object> var = v8::Local<v8::Object>::Cast(args[i]);


#define REQUIRE_ARGUMENT_FUNCTION(i, var)                                      \
    if (args.Length() <= (i) || !args[i]->IsFunction()) {                      \
        RETURN_EXCEPTION_STR("Argument " #i " must be a function");                 \
    }                                                                          \
    v8::Local<v8::Function> var = v8::Local<v8::Function>::Cast(args[i]);


#define ARG_CHECK_STRING(args, i)                                        \
    if (args.Length() <= (i) || !args[i]->IsString()) {                        \
        RETURN_EXCEPTION_STR("Argument " #i " must be a string");                   \
    }                                                                          \

#define REQUIRE_ARGUMENT_STRING(args, i, var)                                        \
    ARG_CHECK_STRING(args, i);                                                       \
    Nan::Utf8String var(V8_LOCAL_STRING_FROM_VALUE(args[i])); \

#define REQUIRE_ARGUMENT_STRINGW(args, i, var)                                        \
    ARG_CHECK_STRING(args, i);                                                       \
    v8::String::Value var(MY_NODE_MODULE_ISOLATE, (args[i]));

#define OPTIONAL_ARGUMENT_FUNCTION(i, var)                                     \
    v8::Local<v8::Function> var;                                                       \
    if (args.Length() > i && !args[i]->IsUndefined()) {                        \
        if (!args[i]->IsFunction()) {                                          \
            RETURN_EXCEPTION_STR("Argument " #i " must be a function");             \
        }                                                                      \
        var = v8::Local<v8::Function>::Cast(args[i]);                                  \
    }


#define OPTIONAL_ARGUMENT_INTEGER(args, i, var, default)                             \
    int var;                                                                   \
    if (args.Length() <= (i)) {                                                \
        var = (default);                                                       \
    }                                                                          \
    else if (args[i]->IsInt32()) {                                             \
        var = args[i]->Int32Value(Nan::GetCurrentContext()).FromJust();                                           \
    }                                                                          \
    else {                                                                     \
        RETURN_EXCEPTION_STR("Argument " #i " must be an integer");                 \
    }
#define EMIT_EVENT(obj, argc, argv)                                            \
    TRY_CATCH_CALL((obj),                                                      \
        Local<Function>::Cast((obj)->Get(String::NewSymbol("emit"))),          \
        argc, argv                                                             \
    );

#define TRY_CATCH_CALL(context, callback, argc, argv)                          \
{   TryCatch try_catch;                                                        \
    (callback)->Call((context), (argc), (argv));                               \
    if (try_catch.HasCaught()) {                                               \
        FatalException(try_catch);                                             \
    }                                                                          \
}

#endif
