#include <clang-c/Index.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 获取绝对路径
const char *get_absolute_path(const char *path)
{
    char *absolute_path = realpath(path, NULL);
    return absolute_path ? absolute_path : path;
}

// 获取光标类型的字符串表示
const char *get_cursor_kind_spelling(enum CXCursorKind kind)
{
    switch (kind)
    {
    case CXCursor_UnexposedDecl:
        return "UnexposedDecl";
    case CXCursor_StructDecl:
        return "StructDecl";
    case CXCursor_UnionDecl:
        return "UnionDecl";
    case CXCursor_ClassDecl:
        return "ClassDecl";
    case CXCursor_EnumDecl:
        return "EnumDecl";
    case CXCursor_FieldDecl:
        return "FieldDecl";
    case CXCursor_EnumConstantDecl:
        return "EnumConstantDecl";
    case CXCursor_FunctionDecl:
        return "FunctionDecl";
    case CXCursor_VarDecl:
        return "VarDecl";
    case CXCursor_ParmDecl:
        return "ParmDecl";
    case CXCursor_TypedefDecl:
        return "TypedefDecl";
    case CXCursor_CXXMethod:
        return "CXXMethod";
    case CXCursor_Namespace:
        return "Namespace";
    case CXCursor_Constructor:
        return "Constructor";
    case CXCursor_Destructor:
        return "Destructor";
    case CXCursor_ConversionFunction:
        return "ConversionFunction";
    case CXCursor_TemplateTypeParameter:
        return "TemplateTypeParameter";
    case CXCursor_TemplateTemplateParameter:
        return "TemplateTemplateParameter";
    case CXCursor_FunctionTemplate:
        return "FunctionTemplate";
    case CXCursor_CXXAccessSpecifier:
        return "AccessSpecifier";
    // 可以继续添加其他光标类型的处理
    default:
        return "Unknown";
    }
}

// 获取类型的字符串表示
const char *get_type_spelling(CXType type)
{
    CXString type_spelling = clang_getTypeSpelling(type);
    const char *type_cstr = clang_getCString(type_spelling);
    char *result = strdup(type_cstr);
    clang_disposeString(type_spelling);
    return result;
}

// 用于存储当前的命名空间和类名的上下文结构
typedef struct
{
    char *namespace_name; // 当前命名空间名称
    char *class_name;     // 当前类名
} Context;

// 初始化上下文
void init_context(Context *context)
{
    context->namespace_name = NULL;
    context->class_name = NULL;
}

// 释放上下文中的动态内存
void free_context(Context *context)
{
    free(context->namespace_name);
    free(context->class_name);
}

// 设置上下文名称（命名空间或类名）
void set_context_name(char **context_name, const char *new_name)
{
    free(*context_name);
    *context_name = new_name ? strdup(new_name) : NULL;
}

// 打印光标信息
void print_cursor_info(CXCursor cursor, Context *context)
{
    CXSourceLocation location = clang_getCursorLocation(cursor);
    CXString cursor_spelling = clang_getCursorSpelling(cursor);
    CXString symbol = clang_Cursor_getMangling(cursor);

    unsigned line, column;
    CXFile file;
    clang_getSpellingLocation(location, &file, &line, &column, NULL);
    CXString file_name = clang_getFileName(file);

    enum CXCursorKind cursor_kind = clang_getCursorKind(cursor);

    // 打印命名空间:类名:函数名
    if (context->namespace_name && context->class_name)
    {
        printf("%s:%s:%s\n", context->namespace_name, context->class_name, clang_getCString(cursor_spelling));
    }
    else if (context->class_name)
    {
        printf("%s:%s\n", context->class_name, clang_getCString(cursor_spelling));
    }
    else
    {
        printf("%s\n", clang_getCString(cursor_spelling));
    }

    printf("Pos: line %d, column %d\n", line, column);
    printf("File: '%s'\n", clang_getCString(file_name));

    if (clang_getCString(symbol))
    {
        printf("Symbol: '%s'\n", clang_getCString(symbol));
    }
    else
    {
        printf("Symbol: None\n");
    }

    // 输出返回值类型和参数信息（仅针对方法和函数）
    if (cursor_kind == CXCursor_CXXMethod || cursor_kind == CXCursor_FunctionDecl)
    {
        CXType return_type = clang_getCursorResultType(cursor);
        const char *return_type_spelling = get_type_spelling(return_type);
        printf("Return Type: %s\n", return_type_spelling);
        free((void *)return_type_spelling);

        // 输出参数信息
        int num_args = clang_Cursor_getNumArguments(cursor);
        printf("Parameters: (");
        for (int i = 0; i < num_args; ++i)
        {
            CXCursor arg_cursor = clang_Cursor_getArgument(cursor, i);
            CXType arg_type = clang_getCursorType(arg_cursor);
            CXString arg_str = clang_getCursorSpelling(arg_cursor);
            const char *arg_cstr = clang_getCString(arg_str);

            const char *arg_type_spelling = get_type_spelling(arg_type);
            printf("%s %s%s", arg_type_spelling, arg_cstr, (i < num_args - 1) ? ", " : "");
            clang_disposeString(arg_str);
            free((void *)arg_type_spelling);
        }
        printf(")\n");
    }

    printf("----------------------------------------\n");

    clang_disposeString(cursor_spelling);
    clang_disposeString(symbol);
    clang_disposeString(file_name);
}

// 遍历AST的回调函数
enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
    Context *context = (Context *)client_data;
    enum CXCursorKind cursor_kind = clang_getCursorKind(cursor);

    if (cursor_kind == CXCursor_Namespace)
    {
        // 处理命名空间
        CXString namespace_name = clang_getCursorSpelling(cursor);
        set_context_name(&context->namespace_name, clang_getCString(namespace_name));
        clang_disposeString(namespace_name);
        // 访问命名空间内的所有子元素
        clang_visitChildren(cursor, visitor, context);
        // 访问完成后清除命名空间名称
        set_context_name(&context->namespace_name, NULL);
    }
    else if (cursor_kind == CXCursor_ClassDecl)
    {
        // 处理类声明
        CXString class_name = clang_getCursorSpelling(cursor);
        set_context_name(&context->class_name, clang_getCString(class_name));
        clang_disposeString(class_name);
        // 访问类内的所有子元素
        clang_visitChildren(cursor, visitor, context);
        // 访问完成后清除类名称
        set_context_name(&context->class_name, NULL);
    }
    else if (cursor_kind == CXCursor_CXXMethod || cursor_kind == CXCursor_FunctionDecl)
    {
        // 处理方法和函数声明
        print_cursor_info(cursor, context);
    }
    else
    {
        // 对于其他类型的光标，继续遍历子元素
        clang_visitChildren(cursor, visitor, context);
    }

    return CXChildVisit_Continue;
}

// 解析C++文件
void parse(const char *filename)
{
    CXIndex index = clang_createIndex(0, 0);
    const char *args[] = {"-x", "c++", "-std=c++11"};
    CXTranslationUnit unit = clang_parseTranslationUnit(index, filename, args, 3, NULL, 0, CXTranslationUnit_None);

    if (unit == NULL)
    {
        printf("Unable to parse translation unit. Quitting.\n");
        return;
    }

    Context context;
    init_context(&context);
    CXCursor cursor = clang_getTranslationUnitCursor(unit);
    clang_visitChildren(cursor, visitor, &context);

    free_context(&context);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);
}

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        parse(argv[1]);
    }
    else
    {
        printf("Usage: %s <C++ header file>\n", argv[0]);
    }
    return 0;
}