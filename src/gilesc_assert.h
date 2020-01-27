#if !defined(GILESC_ASSERT_H)

#define GILESC_ASSERT_H

#if GILESC_DEBUG && USE_GILESC_ASSERT
    #include <stddef.h>
    #include <stdarg.h>

    void PRINT_MESSAGE(char *msg, ...) {
        va_list args;
        va_start(args, msg);
        printf("Message: ");
        vprintf(msg, args);
        va_end(args); 
        printf("\n");
    }
    #if 0
    void assert_handler(bool eval, char* expression, char* file, int line, int argc, char* msg, ...)
    {
        if (!eval)
        {
            printf("===============================================\n");
            printf("--              Assertion Failed             --\n");
            printf("-----------------------------------------------\n");
            printf("File: %s\n", file);                             
            printf("Line: %i\n", line);                             
            printf("Expression: %s\n", expression);                    
            if (argc)
            {
                va_list args;
                va_start(args, msg);
                vprintf(msg, args);
                va_end(args); 
                printf("\n");
            } else {
                printf("Message: %s\n", msg);
            }
            printf("-----------------------------------------------\n");
            __builtin_trap();                                           
        }
    }
    #define AssertTest3(expression, msg, ...) assert_handler(expression, #expression, __FILE__, __LINE__, 1, msg, ##__VA_ARGS__)
    #define AssertTest2(expression, msg) assert_handler(expression, #expression, __FILE__, __LINE__, 0, msg)
    #define AssertTest(expression) assert_handler(expression, #expression, __FILE__, __LINE__, 0, "")
    #endif

    #define AssertWithMessageArg(expression, msg, ...)                  \
        if(!(expression)) {                                             \
        printf("===============================================\n");    \
        printf("--              Assertion Failed             --\n");    \
        printf("-----------------------------------------------\n");    \
        printf("File: %s\n", __FILE__);                                 \
        printf("Line: %i\n", __LINE__);                                 \
        printf("Expression: %s\n", #expression);                        \
        PRINT_MESSAGE(msg, ##__VA_ARGS__);                              \
        printf("-----------------------------------------------\n");    \
        exit(EXIT_FAILURE);                                             \
        }
    #define AssertWithMessage(exp, msg) AssertWithMessageArg(exp, msg, "")
    #define Assert(expression) AssertWithMessage(expression, "")

    #define InvalidCodePath AssertWithMessage(false, "InvalidCodePath")
    #define InvalidDefaultCase default: {InvalidCodePath;} break
#else
    #define AssertWithMessageArg(...)
    #define AssertWithMessage(...)
    #define Assert(...)
#endif

#define InvalidCodePath AssertWithMessage(false, "InvalidCodePath")
#define InvalidDefaultCase default: {InvalidCodePath;} break

#endif
