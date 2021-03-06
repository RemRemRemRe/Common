//

#pragma once

#include "MacroAsFunctionOverloads.h"
#include "Misc.h"

#pragma region Config Macro

// #define LET_IT_CRASH
// #define DISABLE_LOG
// #define DISABLE_ASSERTION
// #define DISABLE_CHECK_MACRO

#ifdef LET_IT_CRASH
    #define INVALID_HANDLING_STATEMENT(...)
#else
    #define INVALID_HANDLING_STATEMENT(...) __VA_ARGS__
#endif // LET_IT_CRASH


#ifdef DISABLE_LOG
    #define LOG_HELPER(CategoryName, Verbosity, Message)
#else
    #define LOG_HELPER(CategoryName, Verbosity, Message) \
        do \
        { \
            if constexpr ( constexpr std::string_view StringView = #Message; \
            !StringView.empty()) \
            { \
                UE_LOG(CategoryName, Verbosity, TEXT("%s"), *FString(Message) ) \
            } \
        } while (false)
#endif // DISABLE_LOG


#ifdef DISABLE_ASSERTION
    #define ASSER_CONDITION(AssertionMacro, Condition)
    #define ASSER_CONDITION_EVALUATED(AssertionMacro, Condition)
#else
    #define ASSER_CONDITION(AssertionMacro, Condition) \
        do \
        { \
            if constexpr ( constexpr std::string_view StringView = #AssertionMacro; \
            !StringView.empty()) \
            { \
                AssertionMacro(Condition); \
            } \
        } while (false)

    #define ASSER_CONDITION_EVALUATED(AssertionMacro, Condition) \
        do \
        { \
            constexpr bool RightConditionIsFalse = false; \
            ASSER_CONDITION(AssertionMacro, RightConditionIsFalse && ( Condition )); \
        } while (false)

#endif // DISABLE_ASSERTION

#pragma endregion Config Macro

#pragma region Ensure Condition

#define ENSURE_CONDITION_6(Condition, InvalidHandlingStatement, AssertionMacro, Message, CategoryName, Verbosity) \
    { \
        if ( UNLIKELY( !(Condition) ) ) \
        { \
            LOG_HELPER(CategoryName, Verbosity, Message); \
            { \
                ASSER_CONDITION_EVALUATED(AssertionMacro, Condition ); \
            } \
            INVALID_HANDLING_STATEMENT(InvalidHandlingStatement); \
        } \
    }

#define ENSURE_CONDITION_5(Condition, InvalidHandlingStatement, AssertionMacro, Message, CategoryName) \
    ENSURE_CONDITION_6(Condition, InvalidHandlingStatement, AssertionMacro, Message, CategoryName, Warning)

#define ENSURE_CONDITION_4(Condition, InvalidHandlingStatement, AssertionMacro, Message) \
    ENSURE_CONDITION_5(Condition, InvalidHandlingStatement, AssertionMacro, Message, LogTemp)

#define ENSURE_CONDITION_3(Condition, InvalidHandlingStatement, AssertionMacro) \
    ENSURE_CONDITION_4(Condition, InvalidHandlingStatement, AssertionMacro, /* Empty Message*/)

#define ENSURE_CONDITION_2(Condition, InvalidHandlingStatement) \
    ENSURE_CONDITION_3(Condition, InvalidHandlingStatement, ensureAlways)

#define ENSURE_CONDITION_1(Condition) \
    ENSURE_CONDITION_2(Condition, /*InvalidHandlingStatement*/)

/**
 * Use this to validate a condition.
 *  - Condition is necessary, other parameters are optional.
 *
 * @param Condition                 the condition to assert
 * @param InvalidHandlingStatement  statements to execute when condition is false. can be any number of statements as you want (surround them with '{}' if ',' is used)
 * @param AssertionMacro            assertion macro to use when condition is false. default to ensureAlways
 * @param Message                   formatted log message when condition is false
 * @param CategoryName              name of the logging category. default to LogTemp
 * @param Verbosity                 verbosity type. default to ELogVerbosity::Type::Warning
 */
#define EnsureCondition(...) MULTI_MACRO(ENSURE_CONDITION, __VA_ARGS__)

#pragma endregion Ensure Condition

#pragma region Ensure Pointer

#define ENSURE_POINTER_6(Pointer, InvalidHandlingStatement, AssertionMacro, Message, CategoryName, Verbosity) \
    ENSURE_CONDITION_6(VALIDATE_POINTER(Pointer), InvalidHandlingStatement, AssertionMacro, Message, CategoryName, Verbosity)

#define ENSURE_POINTER_5(Pointer, InvalidHandlingStatement, AssertionMacro, Message, CategoryName) \
    ENSURE_CONDITION_5(VALIDATE_POINTER(Pointer), InvalidHandlingStatement, AssertionMacro, Message, CategoryName)

#define ENSURE_POINTER_4(Pointer, InvalidHandlingStatement, AssertionMacro, Message) \
    ENSURE_CONDITION_4(VALIDATE_POINTER(Pointer), InvalidHandlingStatement, AssertionMacro, Message)

#define ENSURE_POINTER_3(Pointer, InvalidHandlingStatement, AssertionMacro) \
    ENSURE_CONDITION_3(VALIDATE_POINTER(Pointer), InvalidHandlingStatement, AssertionMacro)

#define ENSURE_POINTER_2(Pointer, InvalidHandlingStatement) \
    ENSURE_CONDITION_2(VALIDATE_POINTER(Pointer), InvalidHandlingStatement)

#define ENSURE_POINTER_1(Pointer) \
    ENSURE_CONDITION_1(VALIDATE_POINTER(Pointer))

/**
 * Use this to validate a pointer.
 *  - Pointer is necessary, other parameters are optional.
 *
* @param Condition                 the condition to assert
 * @param InvalidHandlingStatement  statements to execute when pointer is invalid. can be any number of statements as you want (surround them with '{}' if ',' is used)
 * @param AssertionMacro            assertion macro to use when pointer is invalid. default to ensureAlways
 * @param Message                   formatted log message when pointer is invalid
 * @param CategoryName              name of the logging category. default to LogTemp
 * @param Verbosity                 verbosity type. default to ELogVerbosity::Type::Warning
 */
#define EnsurePointer(...) MULTI_MACRO(ENSURE_POINTER, __VA_ARGS__)

#pragma endregion Ensure Pointer

#pragma region Check Condition

#if defined(DISABLE_CHECK_MACRO) && !DO_CHECK
    #define CheckCondition(...)
    #define CheckPointer(...)
#else
    #define CheckCondition(...) EnsureCondition(__VA_ARGS__)
    #define CheckPointer(...) EnsurePointer(__VA_ARGS__)
#endif // DISABLE_CHECK_MACRO

#pragma endregion Check Condition
// complement