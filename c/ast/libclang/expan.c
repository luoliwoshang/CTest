#include <clang-c/CXString.h>
#include <clang-c/Index.h>
#include <stdio.h>
#include <string.h>

void print_token_info(CXTranslationUnit tu, CXToken token) {
    CXString spelling = clang_getTokenSpelling(tu, token);
    CXTokenKind kind = clang_getTokenKind(token);

    // 获取token的位置
    CXSourceLocation loc = clang_getTokenLocation(tu, token);

    // 获取拼写位置（原始位置）
    CXFile spelling_file;
    unsigned spelling_line, spelling_column, spelling_offset;
    clang_getSpellingLocation(loc, &spelling_file, &spelling_line, &spelling_column, &spelling_offset);

    // 获取展开位置
    CXFile expansion_file;
    unsigned expansion_line, expansion_column, expansion_offset;
    clang_getExpansionLocation(loc, &expansion_file, &expansion_line, &expansion_column, &expansion_offset);

    // 获取文件名
    CXString spelling_filename = clang_getFileName(spelling_file);
    CXString expansion_filename = clang_getFileName(expansion_file);

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
    printf("  Spelling Location: %s:%u:%u\n", clang_getCString(spelling_filename), spelling_line, spelling_column);
    printf("  Expansion Location: %s:%u:%u\n", clang_getCString(expansion_filename), expansion_line, expansion_column);
    printf("\n");

    clang_disposeString(spelling);
    clang_disposeString(spelling_filename);
    clang_disposeString(expansion_filename);
}

// 处理游标的回调函数
enum CXChildVisitResult visitCursor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    if (clang_getCursorKind(cursor) == CXCursor_TypedefDecl) {
        CXString name = clang_getCursorSpelling(cursor);
        if (strcmp(clang_getCString(name), "NewType") == 0) {
            printf("Found typedef NewType:\n");

            // 获取typedef的范围
            CXSourceRange typedef_range = clang_getCursorExtent(cursor);

            // 获取底层类型
            CXType underlying_type = clang_getTypedefDeclUnderlyingType(cursor);
            printf("Underlying type: %s\n", clang_getCString(clang_getTypeSpelling(underlying_type)));

            // 获取该范围内的所有tokens
            CXTranslationUnit tu = clang_Cursor_getTranslationUnit(cursor);
            CXToken *tokens;
            unsigned num_tokens;
            clang_tokenize(tu, typedef_range, &tokens, &num_tokens);

            printf("\nTokens in typedef declaration:\n");
            for (unsigned i = 0; i < num_tokens; i++) {
                print_token_info(tu, tokens[i]);
            }

            // 获取展开后的类型定义
            CXCursor type_ref = clang_getTypeDeclaration(underlying_type);
            if (!clang_Cursor_isNull(type_ref)) {
                CXSourceRange type_range = clang_getCursorExtent(type_ref);
                CXToken *expanded_tokens;
                unsigned num_expanded_tokens;
                clang_tokenize(tu, type_range, &expanded_tokens, &num_expanded_tokens);

                printf("\nTokens in expanded type definition:\n");
                for (unsigned i = 0; i < num_expanded_tokens; i++) {
                    print_token_info(tu, expanded_tokens[i]);
                }

                clang_disposeTokens(tu, expanded_tokens, num_expanded_tokens);
            }

            clang_disposeTokens(tu, tokens, num_tokens);
        }
        clang_disposeString(name);
    }
    return CXChildVisit_Continue;
}

int main() {
    CXIndex index = clang_createIndex(0, 0);

    // 创建测试用的源文件内容
    const char *def_content = "#define __FSID_T_TYPE \\\n"
                              "    struct { \\\n"
                              "        int __val[2]; \\\n"
                              "    }\n";

    const char *ref_content = "#include \"def.h\"\n"
                              "typedef __FSID_T_TYPE NewType;\n";

    // 设置未保存的文件
    struct CXUnsavedFile unsaved_files[] = {{"def.h", def_content, strlen(def_content)},
                                            {"ref.h", ref_content, strlen(ref_content)}};

    // 设置编译参数
    const char *args[] = {
        "-x", "c", // 指定语言为C
        "-I.",     // 包含当前目录
        "-E",      // 启用预处理
        "-v"       // 显示详细信息
    };

    // 解析翻译单元
    CXTranslationUnit tu = clang_parseTranslationUnit(index,
                                                      "ref.h", // 主文件名
                                                      args,    // 命令行参数
                                                      4,       // 参数数量
                                                      unsaved_files,
                                                      2, // 未保存文件数量
                                                      CXTranslationUnit_DetailedPreprocessingRecord);

    if (!tu) {
        printf("Failed to parse translation unit\n");
        return 1;
    }

    // 遍历AST
    CXCursor cursor = clang_getTranslationUnitCursor(tu);
    clang_visitChildren(cursor, visitCursor, NULL);

    // 清理资源
    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(index);

    return 0;
}