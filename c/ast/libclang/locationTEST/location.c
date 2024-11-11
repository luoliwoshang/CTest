#include <clang-c/Index.h>
#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

void visitCursor(CXCursor cursor) {
    enum CXCursorKind kind = clang_getCursorKind(cursor);
    if (kind == CXCursor_MacroExpansion) {
        printf("macro expansion: %s\n", clang_getCString(clang_getCursorSpelling(cursor)));
        CXSourceRange range = clang_getCursorExtent(cursor);
        unsigned startLine, startColumn, endLine, endColumn;
        clang_getFileLocation(clang_getRangeStart(range), NULL, &startLine, &startColumn, NULL);
        clang_getFileLocation(clang_getRangeEnd(range), NULL, &endLine, &endColumn, NULL);
        printf("macro expansion range start: %d:%d, range end: %d:%d\n", startLine, startColumn, endLine, endColumn);

        // 获取宏展开的标记（tokens）
        CXToken *tokens;
        unsigned numTokens;
        clang_tokenize(clang_Cursor_getTranslationUnit(cursor), range, &tokens, &numTokens);

        printf("Macro expansion tokens:\n");
        for (unsigned i = 0; i < numTokens; i++) {
            CXString tokenSpelling = clang_getTokenSpelling(clang_Cursor_getTranslationUnit(cursor), tokens[i]);
            printf("  %s\n", clang_getCString(tokenSpelling));
            clang_disposeString(tokenSpelling);
        }
    }
    if (kind == CXCursor_TypedefDecl) {
        CXType type = clang_getTypedefDeclUnderlyingType(cursor);
        printf("underlying type: %s\n", clang_getCString(clang_getTypeSpelling(type)));
        // 获取类型声明的cursor
        CXCursor typeCursor = clang_getTypeDeclaration(type);
        // 输出其 Cursor Kind
        printf("type declaration: %s\n", clang_getCString(clang_getCursorSpelling(typeCursor)));
        printf("type declaration kind: %s\n",
               clang_getCString(clang_getCursorKindSpelling(clang_getCursorKind(typeCursor))));

        // 获得这个类型定义的Range，为实际引用中的范围，也就是__FSID_T_TYPE，并不是展开后的
        // typedef __FSID_T_TYPE NewType;
        CXSourceRange range = clang_getCursorExtent(typeCursor);

        // 尝试从这个范围的开始获得一个Cursor
        CXSourceLocation location = clang_getRangeStart(range);
        CXCursor cursorAtLoc = clang_getCursor(clang_Cursor_getTranslationUnit(typeCursor), location);
        printf("Cursor at range start:\n");
        printf("  Kind: %s\n", clang_getCString(clang_getCursorKindSpelling(clang_getCursorKind(cursorAtLoc))));
        printf("  Spelling: %s\n", clang_getCString(clang_getCursorSpelling(cursorAtLoc)));

        CXToken *tokens;
        unsigned numTokens;
        clang_tokenize(clang_Cursor_getTranslationUnit(typeCursor), range, &tokens, &numTokens);
        for (unsigned i = 0; i < numTokens; i++) {
            CXString tokenSpelling = clang_getTokenSpelling(clang_Cursor_getTranslationUnit(typeCursor), tokens[i]);
            printf("%s ", clang_getCString(tokenSpelling));
            clang_disposeString(tokenSpelling);
        }
        // 输出 range 的开始和结束位置
        unsigned startLine, startColumn, endLine, endColumn;
        clang_getFileLocation(clang_getRangeStart(range), NULL, &startLine, &startColumn, NULL);
        clang_getFileLocation(clang_getRangeEnd(range), NULL, &endLine, &endColumn, NULL);
        printf("range start: %d:%d, range end: %d:%d\n", startLine, startColumn, endLine, endColumn);
        printf("\n");
    }
}

enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    visitCursor(cursor);
    return CXChildVisit_Continue;
}

int main() {
    CXIndex index = clang_createIndex(0, 0);

    // 移除虚拟文件相关代码
    const char *args[] = {
        "-I.", // Add current directory to include path
        "-xc"  // Explicitly specify C language
    };

    // 直接解析真实文件
    CXTranslationUnit tu = clang_parseTranslationUnit(index, "typedef.h", args, 2, NULL, 0, // 不再使用 unsaved_files
                                                      CXTranslationUnit_DetailedPreprocessingRecord);

    if (tu == NULL) {
        printf("Error: Could not parse translation unit\n");
        return 1;
    }

    CXCursor cursor = clang_getTranslationUnitCursor(tu);
    clang_visitChildren(cursor, visitor, NULL);

    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(index);

    return 0;
}