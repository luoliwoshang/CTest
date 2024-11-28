#include <clang-c/CXString.h>
#include <clang-c/Index.h>
#include <stdio.h>
#include <string.h>

void print_macro_expansion(CXTranslationUnit tu, CXSourceRange range) {
    CXToken *tokens;
    unsigned num_tokens;
    clang_tokenize(tu, range, &tokens, &num_tokens);

    printf("Macro expansion:\n");
    for (unsigned i = 0; i < num_tokens; i++) {
        CXString spelling = clang_getTokenSpelling(tu, tokens[i]);
        printf("  %s", clang_getCString(spelling));
        clang_disposeString(spelling);
    }
    printf("\n");

    clang_disposeTokens(tu, tokens, num_tokens);
}

void print_token_info(CXTranslationUnit tu, CXToken token) {
    CXString spelling = clang_getTokenSpelling(tu, token);
    CXTokenKind kind = clang_getTokenKind(token);

    CXSourceLocation loc = clang_getTokenLocation(tu, token);

    CXFile file;
    unsigned line, column, offset;
    clang_getExpansionLocation(loc, &file, &line, &column, &offset);

    CXString filename = clang_getFileName(file);

    const char *kind_str;
    switch (kind) {
    case CXToken_Punctuation:
        kind_str = "Punctuation";
        break;
    case CXToken_Keyword:
        kind_str = "Keyword";
        break;
    case CXToken_Identifier:
        kind_str = "Identifier";
        break;
    case CXToken_Literal:
        kind_str = "Literal";
        break;
    case CXToken_Comment:
        kind_str = "Comment";
        break;
    default:
        kind_str = "Unknown";
        break;
    }

    printf("Token: %s\n", clang_getCString(spelling));
    printf("  Kind: %s\n", kind_str);
    printf("  Location: %s:%u:%u\n", clang_getCString(filename), line, column);

    clang_disposeString(spelling);
    clang_disposeString(filename);
}

enum CXChildVisitResult visitCursor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    CXSourceLocation loc = clang_getCursorLocation(cursor);
    if (clang_Location_isInSystemHeader(loc)) {
        return CXChildVisit_Continue;
    }

    enum CXCursorKind kind = clang_getCursorKind(cursor);
    CXString name = clang_getCursorSpelling(cursor);
    const char *cursor_name = clang_getCString(name);

    // 专门处理宏展开
    if (kind == CXCursor_MacroExpansion) {
        printf("Found macro expansion: %s\n", cursor_name);
        CXSourceRange range = clang_getCursorExtent(cursor);
        CXTranslationUnit tu = clang_Cursor_getTranslationUnit(cursor);
        print_macro_expansion(tu, range);
        printf("\n");
    }
    // 处理 typedef
    else if (kind == CXCursor_TypedefDecl && strcmp(cursor_name, "NewType") == 0) {
        printf("Found typedef NewType:\n");

        CXType underlying_type = clang_getTypedefDeclUnderlyingType(cursor);
        CXString type_spelling = clang_getTypeSpelling(underlying_type);
        printf("Underlying type: %s\n\n", clang_getCString(type_spelling));

        CXSourceRange cursor_range = clang_getCursorExtent(cursor);
        CXTranslationUnit tu = clang_Cursor_getTranslationUnit(cursor);

        CXToken *tokens;
        unsigned num_tokens;
        clang_tokenize(tu, cursor_range, &tokens, &num_tokens);

        for (unsigned i = 0; i < num_tokens; i++) {
            print_token_info(tu, tokens[i]);
            printf("\n");
        }

        clang_disposeTokens(tu, tokens, num_tokens);
        clang_disposeString(type_spelling);
    }

    clang_disposeString(name);
    return CXChildVisit_Recurse;
}

int main() {
    CXIndex index = clang_createIndex(0, 1); // 启用诊断输出

    const char *def_content = "#define __FSID_T_TYPE \\\n"
                              "    struct { \\\n"
                              "        int __val[2]; \\\n"
                              "    }\n";

    const char *ref_content = "#include \"def.h\"\n"
                              "typedef __FSID_T_TYPE NewType;\n";

    struct CXUnsavedFile unsaved_files[] = {{"def.h", def_content, strlen(def_content)},
                                            {"ref.h", ref_content, strlen(ref_content)}};

    const char *args[] = {
        "-xc", // 指定C语言
        "-I.",
        "-v" // 详细输出
    };

    unsigned options = CXTranslationUnit_DetailedPreprocessingRecord | CXTranslationUnit_KeepGoing |
                       CXTranslationUnit_SkipFunctionBodies;

    CXTranslationUnit tu = clang_parseTranslationUnit(index, "ref.h", args, 3, unsaved_files, 2, options);

    if (!tu) {
        printf("Failed to parse translation unit\n");
        return 1;
    }

    CXCursor cursor = clang_getTranslationUnitCursor(tu);
    clang_visitChildren(cursor, visitCursor, NULL);

    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(index);

    return 0;
}