#include <clang-c/Index.h>
#include <stdio.h>
enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
    // 检查节点是否是函数声明
    if (clang_getCursorKind(cursor) == CXCursor_FunctionDecl)
    {
        CXString name = clang_getCursorSpelling(cursor);
        printf("Function: %s\n", clang_getCString(name));
        clang_disposeString(name);

        // 打印函数的参数
        int numArgs = clang_Cursor_getNumArguments(cursor);
        for (int i = 0; i < numArgs; i++)
        {
            CXCursor arg = clang_Cursor_getArgument(cursor, i);
            CXString argName = clang_getCursorSpelling(arg);
            CXString argType = clang_getTypeSpelling(clang_getCursorType(arg));
            printf("  Param %d: %s (%s)\n", i + 1, clang_getCString(argName), clang_getCString(argType));
            clang_disposeString(argName);
            clang_disposeString(argType);
        }
        printf("\n");
    }
    return CXChildVisit_Recurse;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <header-file>\n", argv[0]);
        return 1;
    }

    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit unit = clang_parseTranslationUnit(
        index, argv[1], NULL, 0, NULL, 0, CXTranslationUnit_None);

    if (!unit)
    {
        fprintf(stderr, "Unable to parse the input file.\n");
        return 1;
    }

    // 使用回调函数遍历 AST
    CXCursor rootCursor = clang_getTranslationUnitCursor(unit);
    clang_visitChildren(rootCursor, visitor, NULL);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);
    return 0;
}
