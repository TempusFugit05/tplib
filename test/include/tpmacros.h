#define ARRAY_LENGTH(array) sizeof(array) / sizeof(array[0])
#define RANGE(iterator, range) for(int iterator = 0; iterator < range; iterator++)
#define LOCAL_FUNC(name, type, def) struct {type operator () def} name;
#define MEMBER_FUNCTION_POINTER(POINTER_NAME, CLASS_NAME, FUNCTION_NAME, OBJ_REF, RETURN_TYPE) \
RETURN_TYPE(*POINTER_NAME)(void); \
{ \
struct FUNCTION_NAME##_local_struct \
{ \
    static CLASS_NAME* init(CLASS_NAME* object = NULL) \
    { \
        static bool isInitialized = false; \
        static CLASS_NAME * objPtr; \
        if(!isInitialized) \
        { \
            objPtr = object; \
            isInitialized = true; \
            return NULL; \
        } \
        return objPtr; \
    } \
    static RETURN_TYPE func() \
    { \
        return init()->FUNCTION_NAME(); \
    } \
    FUNCTION_NAME##_local_struct(CLASS_NAME* ptr) \
    { \
        init(ptr); \
    } \
    }; \
FUNCTION_NAME##_local_struct FUNCTION_NAME##_local_instance = FUNCTION_NAME##_local_struct(OBJ_REF); \
POINTER_NAME = &FUNCTION_NAME##_local_struct::func; \
}

// Get number of arguments in __VA_ARGS__: https://groups.google.com/g/comp.std.c/c/d-6Mj5Lko_s/m/5R6bMWTEbzQJ
#define GET_NUM_ARGS(...) CONCATENATE_ARGS(__VA_ARGS__, NUM_ARGS)
#define CONCATENATE_ARGS(...) ARG_CURSOR(__VA_ARGS__)
#define ARG_CURSOR(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10, ARG_NUM, ...) ARG_NUM
#define NUM_ARGS 10,9,8,7,6,5,4,3,2,1,0

// Credit: https://stackoverflow.com/a/1872506
// This set of defines is needed to operate on macros after expanding them.
#define TO_STRING_INTERNAL(STRING) #STRING
#define TO_STRING(STRING) TO_STRING_INTERNAL(STRING)

#define CONCATENATE_INTERNAL(ARG1, ARG2) ARG1 ## ARG2
#define CONCATENATE(ARG1, ARG2) CONCATENATE_INTERNAL(ARG1, ARG2)

#define CONCATENATE_INTERNAL_2(ARG1, ARG2) ARG1 ## ARG2
#define CONCATENATE_2(ARG1, ARG2) CONCATENATE_INTERNAL_2(ARG1, ARG2)

#define ARG_PREFIX ARGUMENT

// This beauty takes variable types and creates a list of arguments.
// Example:
// TYPES_TO_ARGS(float, int, bool)
// -> float ARGUMENT_3, int ARGUMENT_2, bool ARGUMENT_1
#define ARG_1(TYPE, ...) TYPE CONCATENATE(ARG_PREFIX, _1)
#define ARG_2(TYPE, ...) TYPE CONCATENATE(ARG_PREFIX, _2), ARG_1(__VA_ARGS__)
#define ARG_3(TYPE, ...) TYPE CONCATENATE(ARG_PREFIX, _3), ARG_2(__VA_ARGS__)
#define ARG_4(TYPE, ...) TYPE CONCATENATE(ARG_PREFIX, _4), ARG_3(__VA_ARGS__)
#define ARG_5(TYPE, ...) TYPE CONCATENATE(ARG_PREFIX, _5), ARG_4(__VA_ARGS__)
#define ARG_6(TYPE, ...) TYPE CONCATENATE(ARG_PREFIX, _6), ARG_5(__VA_ARGS__)
#define ARG_7(TYPE, ...) TYPE CONCATENATE(ARG_PREFIX, _7), ARG_6(__VA_ARGS__)
#define ARG_8(TYPE, ...) TYPE CONCATENATE(ARG_PREFIX, _8), ARG_7(__VA_ARGS__)
#define ARG_9(TYPE, ...) TYPE CONCATENATE(ARG_PREFIX, _9), ARG_8(__VA_ARGS__)
#define ARG_10(TYPE, ...) TYPE CONCATENATE(ARG_PREFIX, _10), ARG_9(__VA_ARGS__)
#define TYPES_TO_ARGS_NUMBERED(NUM_ARGS, ...) CONCATENATE(ARG_, NUM_ARGS)(__VA_ARGS__)
#define TYPES_TO_ARGS(...) TYPES_TO_ARGS_NUMBERED(GET_NUM_ARGS(__VA_ARGS__), __VA_ARGS__)

#define EXPAND(...) __VA_ARGS__
#define EXPAND_2(...) __VA_ARGS__

#define EMPTY()
#define DEFER(ARG) ARG EMPTY() 
#define EXPAND(...) __VA_ARGS__

#define EMPTY2()
#define DEFER2(ARG) ARG EMPTY() 
#define EXPAND2(...) __VA_ARGS__


//#define VA_ARGS_SAFE(ARGS) VA_ARGS_SAFE_DEFER(VA_ARGS, ARGS)
//#define VA_ARGS_SAFE_DEFER(MACRO, COUNT, TYPE, VALUE) MACRO(COUNT, TYPE, VALUE)

#define EXPAND_CALL(a, b) a b
#define VA_ARGS_SAFE(ARGS) EXPAND_CALL(VA_ARGS, ARGS)

#define VA_ARGS(COUNT, TYPE, VALUE) _VA_ARG_DEFER(CONCATENATE(_VA_ARG_, COUNT), TYPE, VALUE)
#define _VA_ARG_DEFER(ARG, TYPE, VALUE) ARG(TYPE, VALUE)
#define _VA_ARG_1(TYPE, VALUE) TYPE CONCATENATE(ARG_PREFIX, _1) = VALUE
#define _VA_ARG_2(TYPE, VALUE) TYPE CONCATENATE(ARG_PREFIX, _2) = VALUE, _VA_ARG_1(TYPE, VALUE)
#define _VA_ARG_3(TYPE, VALUE) TYPE CONCATENATE(ARG_PREFIX, _3) = VALUE, _VA_ARG_2(TYPE, VALUE)
#define _VA_ARG_4(TYPE, VALUE) TYPE CONCATENATE(ARG_PREFIX, _4) = VALUE, _VA_ARG_3(TYPE, VALUE)
#define _VA_ARG_5(TYPE, VALUE) TYPE CONCATENATE(ARG_PREFIX, _5) = VALUE, _VA_ARG_4(TYPE, VALUE)
#define _VA_ARG_6(TYPE, VALUE) TYPE CONCATENATE(ARG_PREFIX, _6) = VALUE, _VA_ARG_5(TYPE, VALUE)
#define _VA_ARG_7(TYPE, VALUE) TYPE CONCATENATE(ARG_PREFIX, _7) = VALUE, _VA_ARG_6(TYPE, VALUE)
#define _VA_ARG_8(TYPE, VALUE) TYPE CONCATENATE(ARG_PREFIX, _8) = VALUE, _VA_ARG_7(TYPE, VALUE)
#define _VA_ARG_9(TYPE, VALUE) TYPE CONCATENATE(ARG_PREFIX, _9) = VALUE, _VA_ARG_8(TYPE, VALUE)
#define _VA_ARG_10(TYPE, VALUE) TYPE CONCATENATE(ARG_PREFIX, _10) = VALUE, _VA_ARG_9(TYPE, VALUE)

#define _VA_INIT_LABEL _va_end_init
#define VA_LIST_INIT_SAFE(ARGS) EXPAND_CALL(VA_LIST_INIT, ARGS)
#define VA_LIST_INIT(COUNT, TYPE, VALUE) \
TYPE args [COUNT]; \
int arg_count = 0; \
_VA_LIST_INIT_DEFER(CONCATENATE(_VA_INIT_, COUNT), TYPE, VALUE) \
_VA_INIT_LABEL:
#define _VA_LIST_INIT_DEFER(ARG, TYPE, VALUE) ARG(TYPE, VALUE)
#define _VA_INIT_1(TYPE, VALUE) if(CONCATENATE(ARG_PREFIX, _1) == VALUE){goto _VA_INIT_LABEL;} else{args[arg_count] = CONCATENATE_2(ARG_PREFIX, _1); arg_count++;}
#define _VA_INIT_2(TYPE, VALUE) if(CONCATENATE(ARG_PREFIX, _2) == VALUE){goto _VA_INIT_LABEL;} else{args[arg_count] = CONCATENATE_2(ARG_PREFIX, _2); arg_count++;} _VA_INIT_1(TYPE, VALUE)
#define _VA_INIT_3(TYPE, VALUE) if(CONCATENATE(ARG_PREFIX, _3) == VALUE){goto _VA_INIT_LABEL;} else{args[arg_count] = CONCATENATE_2(ARG_PREFIX, _3); arg_count++;} _VA_INIT_2(TYPE, VALUE)
#define _VA_INIT_4(TYPE, VALUE) if(CONCATENATE(ARG_PREFIX, _4) == VALUE){goto _VA_INIT_LABEL;} else{args[arg_count] = CONCATENATE_2(ARG_PREFIX, _4); arg_count++;} _VA_INIT_3(TYPE, VALUE)
#define _VA_INIT_5(TYPE, VALUE) if(CONCATENATE(ARG_PREFIX, _5) == VALUE){goto _VA_INIT_LABEL;} else{args[arg_count] = CONCATENATE_2(ARG_PREFIX, _5); arg_count++;} _VA_INIT_4(TYPE, VALUE)
#define _VA_INIT_6(TYPE, VALUE) if(CONCATENATE(ARG_PREFIX, _6) == VALUE){goto _VA_INIT_LABEL;} else{args[arg_count] = CONCATENATE_2(ARG_PREFIX, _6); arg_count++;} _VA_INIT_5(TYPE, VALUE)
#define _VA_INIT_7(TYPE, VALUE) if(CONCATENATE(ARG_PREFIX, _7) == VALUE){goto _VA_INIT_LABEL;} else{args[arg_count] = CONCATENATE_2(ARG_PREFIX, _7); arg_count++;} _VA_INIT_6(TYPE, VALUE)
#define _VA_INIT_8(TYPE, VALUE) if(CONCATENATE(ARG_PREFIX, _8) == VALUE){goto _VA_INIT_LABEL;} else{args[arg_count] = CONCATENATE_2(ARG_PREFIX, _8); arg_count++;} _VA_INIT_7(TYPE, VALUE)
#define _VA_INIT_9(TYPE, VALUE) if(CONCATENATE(ARG_PREFIX, _9) == VALUE){goto _VA_INIT_LABEL;} else{args[arg_count] = CONCATENATE_2(ARG_PREFIX, _9); arg_count++;} _VA_INIT_8(TYPE, VALUE)
#define _VA_INIT_10(TYPE, VALUE) if(CONCATENATE(ARG_PREFIX, _10) == VALUE){goto _VA_INIT_LABEL;} else{args[arg_count] = CONCATENATE_2(ARG_PREFIX, _10); arg_count++;} _VA_INIT_9(TYPE, VALUE)

// This creates a set of matching arguments to pass to a function who's parameters were generated with TYPES_TO_ARGS.
#define ARG_NAME_1 CONCATENATE_2(ARG_PREFIX, _1)
#define ARG_NAME_2 CONCATENATE_2(ARG_PREFIX, _2), ARG_NAME_1
#define ARG_NAME_3 CONCATENATE_2(ARG_PREFIX, _3), ARG_NAME_2
#define ARG_NAME_4 CONCATENATE_2(ARG_PREFIX, _4), ARG_NAME_3
#define ARG_NAME_5 CONCATENATE_2(ARG_PREFIX, _5), ARG_NAME_4
#define ARG_NAME_6 CONCATENATE_2(ARG_PREFIX, _6), ARG_NAME_5
#define ARG_NAME_7 CONCATENATE_2(ARG_PREFIX, _7), ARG_NAME_6
#define ARG_NAME_8 CONCATENATE_2(ARG_PREFIX, _8), ARG_NAME_7
#define ARG_NAME_9 CONCATENATE_2(ARG_PREFIX, _9), ARG_NAME_8
#define ARG_NAME_10 CONCATENATE_2(ARG_PREFIX, _10), ARG_NAME_9
#define PASTE_ARG_NAMES_(NUM_ARGS) CONCATENATE(ARG_NAME_, NUM_ARGS)
#define PASTE_ARG_NAMES(...) PASTE_ARG_NAMES_(GET_NUM_ARGS(__VA_ARGS__))

#define MEMBER_FUNCTION_POINTER_WITH_ARGS(POINTER_NAME, CLASS_NAME, FUNCTION_NAME, OBJ_REF, RETURN_TYPE, ...) \
RETURN_TYPE(*POINTER_NAME)(__VA_ARGS__); \
{ \
struct FUNCTION_NAME##_local_struct \
{ \
    static CLASS_NAME* init(CLASS_NAME* object = NULL) \
    { \
        static bool isInitialized = false; \
        static CLASS_NAME* objPtr; \
        if(!isInitialized) \
        { \
            objPtr = object; \
            isInitialized = true; \
            return NULL; \
        } \
        return objPtr; \
    } \
    static RETURN_TYPE func(TYPES_TO_ARGS(__VA_ARGS__)) \
    { \
        return init()->FUNCTION_NAME(PASTE_ARG_NAMES(__VA_ARGS__)); \
    } \
    FUNCTION_NAME##_local_struct(CLASS_NAME* ptr) \
    { \
        init(ptr); \
    } \
    }; \
FUNCTION_NAME##_local_struct FUNCTION_NAME##_local_instance = FUNCTION_NAME##_local_struct(OBJ_REF); \
POINTER_NAME = &FUNCTION_NAME##_local_struct::func; \
}

#define MAX_SIGNED(TYPE) (~(TYPE(1) << ((sizeof(TYPE) * 8) - 1)))
#define MIN_SIGNED(TYPE) ((TYPE(1) << ((sizeof(TYPE) * 8) - 1)))
#define MAX_UNSIGNED(TYPE) (~TYPE(0))
#define MIN_UNSIGNED() (0)
