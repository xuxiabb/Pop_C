#pragma once

#include <stddef.h>
#include <stdlib.h>

// Interface
#define INTERFACE(Class)        \
    typedef struct Class Class; \
    struct Class

// Abstract Class
#define ABSTRACT(Class)         \
    typedef struct Class Class; \
    void _##Class##Ctor();      \
    void _##Class##Dtor();      \
    struct Class

// Abstract Constructor
#define ABSTRACT_CTOR(Class, type_args...) \
    void _##Class##Ctor(Class *_this, ##type_args)

// Abstract Destructor
#define ABSTRACT_DTOR(Class) \
    void _##Class##Dtor(Class *_this)

// Class
#define CLASS(Class)            \
    typedef struct Class Class; \
    void _##Class##Ctor();      \
    void _##Class##Dtor();      \
    int _New##Class();          \
    int _Delete##Class();       \
    struct Class

// Class Constructor
#define CLASS_CTOR(Class, type_args...)          \
    int _New##Class(Class **c_ptr)               \
    {                                            \
        *c_ptr = malloc(sizeof(Class)); \
        if (!*c_ptr)                             \
            return 1;                            \
        return 0;                                \
    }                                            \
    void _##Class##Ctor(Class *_this, ##type_args)

// Class Destructor
#define CLASS_DTOR(Class)        \
    int _Delete##Class(Class *c) \
    {                            \
        if (!c)                  \
            return 1;            \
        _##Class##Dtor(c);       \
        free(c);                 \
        return 0;                \
    }                            \
    void _##Class##Dtor(Class *_this)

// New
#define NEW(Class, c, args...)         \
    ({                                 \
        {                              \
            _New##Class(&c);           \
            _##Class##Ctor(c, ##args); \
        }                              \
        (Class *)c;                    \
    })

// Delete
#define DELETE(Class, c) \
    (_Delete##Class(c))

//  Class Public Static Member Variable Declaration
#define PSVAR(Class, type, arg) \
    extern type Class_arg

// Class Public Static Member Variable Definition
#define MEMBER(Class, type, arg) \
    type Class_arg

// Public Member Function Declaration
#define FUNC(Class, return_type, Func, type_args...) \
    return_type (*Func)(Class * _this, ##type_args)

// Public Member Function Definition
#define METHOD(Class, return_type, Func, type_args...) \
    return_type Class##Func(Class *_this, ##type_args)

// Private Member Function Declaration
#define _FUNC(Class, return_type, Func, type_args...) \
    return_type (*_##Func)(Class * _this, ##type_args)

// Private Member Function Definition
#define _METHOD(Class, return_type, Func, type_args...) \
    return_type Class##_##Func(Class *_this, ##type_args)

// Virtual Member Function Declaration in Base
#define VBFUNC(Base, return_type, Func, type_args...) \
    return_type (*Func)(Base * _base, ##type_args)

// Virtual Member Function Declaration in Class
#define VFUNC(Base, return_type, Func, type_args...) \
    ;

// Virtual Member Function Definition
#define VMETHOD(Base, Class, return_type, Func, type_args...) \
    return_type Class##V##Func(Base *_base, ##type_args)

// Public Static Member Function Declaration
#define PSFUNC(Class, return_type, Func) \
    extern return_type Class##Func()

// Public Inheritance
#define INHERIT(Father) \
    struct Father Father

// Binding
#define BIND(Class, Func) \
    _this->Func = Class##Func

// Override
#define OVERRIDE(inheritance, Class, Func) \
    _this->inheritance.Func = Class##V##Func

// Super
#define SUPER(Father, c) \
    ((Father *)(&(c->Father)))

// Super 2
#define SUPER_2(GrandFather, Father, c) \
    SUPER(GrandFather, SUPER(Father, c))

// Super 3
#define SUPER_3(GrandGrandFather, GrandFather, Father, c) \
    SUPER(GrandGrandFather, SUPER(GrandFather, SUPER(Father, c)))

// Sub
#define SUB(Father, Son, c) \
    ((Son *)((char *)c - offsetof(Son, Father)))

// Sub 2
#define SUB_2(GrandFather, Father, Son, c) \
    SUB(Father, Son, SUB(GrandFather, Father, c))

// Sub 3
#define SUB_3(GrandGrandFather, GrandFather, Father, Son, c) \
    SUB(Father, Son, SUB(GrandFather, Father, SUB(GrandGrandFather, GrandFather, c)))

// Father Constructor Only in CLASS_CTOR Start
#define SUPER_CTOR(Father, args...) \
    _##Father##Ctor(SUPER(Father, _this), ##args)

// Father Destructor Only in CLASS_DTOR End
#define SUPER_DTOR(Father) \
    _##Father##Dtor(SUPER(Father, _this))

// Sub then Super Only in Virtual Member Function
#define SWITCH(DestFather, SrcFather, Son, c) \
    SUPER(DestFather, SUB(SrcFather, Son, c));

// This Pointer Only in Virtual Member Function
#define _THIS(Father, Son) \
    Son *_this = SUB(Father, Son, _base)

// This Pointer 2 Only in Virtual Member Function
#define _THIS_2(GrandFather, Father, Son) \
    Son *_this = SUB_2(GrandFather, Father, Son, _base)

// This Pointer 3 Only in Virtual Member Function
#define _THIS_3(GrandGrandFather, GrandFather, Father, Son) \
    Son *_this = SUB_3(GrandGrandFather, GrandFather, Father, Son, _base)

// This Pointer Switch Only in Virtual Member Function
#define _THIS_SW(DestFather, SrcFather, Son) \
    DestFather *_this = SWITCH(DestFather, SrcFather, Son, _base);

// Do Member Function of the Instance and input the Instance
#define DO(c, Func, args...) \
    c->Func(c, ##args)

// Lambda
#define LAMBDA(return_type, Function_body) \
    ({                                     \
        return_type __fn__ Function_body   \
            __fn__;                        \
    })
