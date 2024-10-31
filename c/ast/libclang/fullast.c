#include <clang-c/Index.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct AAA {
} AAA;

void print_cursor_info(CXCursor cursor, int depth) {
    CXString cursor_spelling = clang_getCursorSpelling(cursor);
    CXString cursor_kind_spelling = clang_getCursorKindSpelling(clang_getCursorKind(cursor));
    CXType cursor_type = clang_getCursorType(cursor);
    CXString type_spelling = clang_getTypeSpelling(cursor_type);

    for (int i = 0; i < depth; ++i)
        printf("  ");
    printf("%s: %s (Type: %s)\n", clang_getCString(cursor_kind_spelling), clang_getCString(cursor_spelling),
           clang_getCString(type_spelling));
    clang_disposeString(cursor_spelling);
    clang_disposeString(cursor_kind_spelling);
    clang_disposeString(type_spelling);
}

enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    int *depth = (int *)client_data;
    print_cursor_info(cursor, *depth);

    int new_depth = *depth + 1;
    clang_visitChildren(cursor, visitor, &new_depth);

    return CXChildVisit_Continue;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <path_to_c_header>\n", argv[0]);
        return 1;
    }
    const char *args[] = {"-x", "c++", "-std=c++11"};

    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit unit =
        clang_parseTranslationUnit(index, argv[1], args, 3, NULL, 0, CXTranslationUnit_DetailedPreprocessingRecord);

    if (unit == NULL) {
        printf("Unable to parse translation unit. Quitting.\n");
        return 1;
    }

    printf("AST for %s:\n\n", argv[1]);
    CXCursor cursor = clang_getTranslationUnitCursor(unit);
    int depth = 0;
    clang_visitChildren(cursor, visitor, &depth);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);

    return 0;
}