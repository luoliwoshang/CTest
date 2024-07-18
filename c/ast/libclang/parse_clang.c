#include <clang-c/Index.h>
#include <stdio.h>

enum CXChildVisitResult enumValueVisitor(CXCursor c, CXCursor parent, CXClientData client_data)
{
    if (clang_getCursorKind(c) == CXCursor_EnumConstantDecl)
    {
        CXString valueName = clang_getCursorSpelling(c);
        long long value = clang_getEnumConstantDeclValue(c);
        printf("  Value: %s = %lld\n", clang_getCString(valueName), value);
        clang_disposeString(valueName);
    }
    return CXChildVisit_Continue;
}

enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
    enum CXCursorKind kind = clang_getCursorKind(cursor);
    CXString name, enumName, argName, argType;
    int numArgs, i;
    unsigned int numTokens, j;
    CXToken *tokens;
    CXTranslationUnit tu;

    switch (kind)
    {
    case CXCursor_FunctionDecl:
        name = clang_getCursorSpelling(cursor);
        printf("Function: %s\n", clang_getCString(name));
        clang_disposeString(name);

        numArgs = clang_Cursor_getNumArguments(cursor);
        for (i = 0; i < numArgs; i++)
        {
            CXCursor arg = clang_Cursor_getArgument(cursor, i);
            argName = clang_getCursorSpelling(arg);
            argType = clang_getTypeSpelling(clang_getCursorType(arg));
            printf("  Param %d: %s (%s)\n", i + 1, clang_getCString(argName), clang_getCString(argType));
            clang_disposeString(argName);
            clang_disposeString(argType);
        }
        printf("\n");
        break;
    case CXCursor_MacroDefinition:
        name = clang_getCursorSpelling(cursor);
        tu = clang_Cursor_getTranslationUnit(cursor);
        clang_tokenize(tu, clang_getCursorExtent(cursor), &tokens, &numTokens);
        printf("Macro: %s = ", clang_getCString(name));
        clang_disposeString(name);
        for (j = 0; j < numTokens; j++)
        {
            CXString tokenText = clang_getTokenSpelling(tu, tokens[j]);
            printf("%s", clang_getCString(tokenText));
            clang_disposeString(tokenText);
        }
        printf("\n");
        clang_disposeTokens(tu, tokens, numTokens);
        break;
    default:
        // Optionally handle other kinds or add a default handler
        break;
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
        index, argv[1], NULL, 0, NULL, 0, CXTranslationUnit_DetailedPreprocessingRecord);
    if (!unit)
    {
        fprintf(stderr, "Unable to parse the input file.\n");
        return 1;
    }

    CXCursor rootCursor = clang_getTranslationUnitCursor(unit);
    clang_visitChildren(rootCursor, visitor, NULL);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);
    return 0;
}
