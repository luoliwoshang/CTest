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

void print_cursor_info(CXCursor cursor, const char *target_dir)
{
    CXSourceLocation location = clang_getCursorLocation(cursor);

    CXString cursor_spelling = clang_getCursorSpelling(cursor);
    CXString symbol = clang_Cursor_getMangling(cursor);

    unsigned line, column;
    CXFile file;
    clang_getSpellingLocation(location, &file, &line, &column, NULL);
    CXString file_name = clang_getFileName(file);

    enum CXCursorKind cursor_kind = clang_getCursorKind(cursor);

    // 获取文件的绝对路径
    const char *absolute_path = get_absolute_path(clang_getCString(file_name));

    // 仅输出目标目录中的文件信息
    if (strstr(absolute_path, target_dir) != NULL)
    {
        printf("(%s):%s\n", get_cursor_kind_spelling(cursor_kind), clang_getCString(cursor_spelling));
        printf("Pos: line %d, column %d\n", line, column);
        printf("File: '%s'\n", absolute_path);

        if (clang_getCString(symbol))
        {
            printf("Symbol: '%s'\n", clang_getCString(symbol));
        }
        else
        {
            printf("Symbol: None\n");
        }
        printf("----------------------------------------\n");
    }

    clang_disposeString(cursor_spelling);
    clang_disposeString(symbol);
    clang_disposeString(file_name);
}

enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
    const char *target_dir = (const char *)client_data;
    enum CXCursorKind cursor_kind = clang_getCursorKind(cursor);
    if (cursor_kind == CXCursor_FunctionDecl || cursor_kind == CXCursor_CXXMethod || cursor_kind == CXCursor_ClassDecl)
    {
        print_cursor_info(cursor, target_dir);
    }
    return CXChildVisit_Recurse;
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

    const char *absolute_path = get_absolute_path(filename); // 获取目标文件的绝对路径
    CXCursor cursor = clang_getTranslationUnitCursor(unit);
    clang_visitChildren(cursor, visitor, (CXClientData)absolute_path);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);
    if (absolute_path != filename)
    {
        free((void *)absolute_path); // 释放由realpath分配的内存
    }
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
