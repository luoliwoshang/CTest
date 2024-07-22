#include <clang-c/Index.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 获取绝对路径
const char *get_absolute_path(const char *path)
{
    char *absolute_path = realpath(path, NULL);
    if (absolute_path == NULL)
    {
        return path;
    }
    return absolute_path;
}

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

// 获取返回值类型的字符串表示
const char *get_type_spelling(CXType type)
{
    CXString type_spelling = clang_getTypeSpelling(type);
    const char *type_cstr = clang_getCString(type_spelling);
    char *result = strdup(type_cstr);
    clang_disposeString(type_spelling);
    return result;
}

// 用于存储当前的命名空间和类名
typedef struct
{
    char namespace_name[1024];
    char class_name[1024];
} Context;

void init_context(Context *context)
{
    context->namespace_name[0] = '\0';
    context->class_name[0] = '\0';
}

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
    if (context->namespace_name[0] != '\0' && context->class_name[0] != '\0')
    {
        printf("%s:%s:%s\n", context->namespace_name, context->class_name, clang_getCString(cursor_spelling));
    }
    else if (context->class_name[0] != '\0')
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

    // 输出返回值类型
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

enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
    Context *context = (Context *)client_data;
    enum CXCursorKind cursor_kind = clang_getCursorKind(cursor);

    if (cursor_kind == CXCursor_Namespace)
    {
        CXString namespace_name = clang_getCursorSpelling(cursor);
        snprintf(context->namespace_name, sizeof(context->namespace_name), "%s", clang_getCString(namespace_name));
        clang_disposeString(namespace_name);
        // 访问命名空间内的所有子元素
        clang_visitChildren(cursor, visitor, context);
        // 访问完成后清除命名空间名称
        context->namespace_name[0] = '\0';
    }
    else if (cursor_kind == CXCursor_ClassDecl)
    {
        CXString class_name = clang_getCursorSpelling(cursor);
        snprintf(context->class_name, sizeof(context->class_name), "%s", clang_getCString(class_name));
        clang_disposeString(class_name);
        // 访问类内的所有子元素
        clang_visitChildren(cursor, visitor, context);
        // 访问完成后清除类名称
        context->class_name[0] = '\0';
    }
    else if (cursor_kind == CXCursor_CXXMethod || cursor_kind == CXCursor_FunctionDecl)
    {
        print_cursor_info(cursor, context);
    }
    else
    {
        // 对于非命名空间和非类声明的其他类型的光标，进行全局访问
        clang_visitChildren(cursor, visitor, context);
    }

    return CXChildVisit_Continue;
}

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
