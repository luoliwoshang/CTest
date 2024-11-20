#include <clang-c/CXString.h>
#include <clang-c/Index.h>
#include <stdio.h>
#include <string.h>

// 处理游标的回调函数
enum CXChildVisitResult visitCursor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    if (clang_getCursorKind(cursor) == CXCursor_TypedefDecl) {
        CXString name = clang_getCursorSpelling(cursor);
        if (strcmp(clang_getCString(name), "NewType") == 0) {
            // 获取底层类型
            CXType underlying_type = clang_getTypedefDeclUnderlyingType(cursor);

            // 获取类型声明的位置
            CXCursor type_decl = clang_getTypeDeclaration(underlying_type);

            if (!clang_Cursor_isNull(type_decl)) {
                // 获取类型声明的范围
                CXSourceRange range = clang_getCursorExtent(type_decl);

                // 获取该范围内的所有tokens
                CXTranslationUnit tu = clang_Cursor_getTranslationUnit(cursor);
                CXToken *tokens;
                unsigned num_tokens;
                clang_tokenize(tu, range, &tokens, &num_tokens);

                printf("Type declaration tokens:\n");
                for (unsigned i = 0; i < num_tokens; i++) {
                    CXString token_spelling = clang_getTokenSpelling(tu, tokens[i]);
                    printf("%s ", clang_getCString(token_spelling));
                    clang_disposeString(token_spelling);
                }
                printf("\n");

                clang_disposeTokens(tu, tokens, num_tokens);
            }
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
        "-E"       // 启用预处理
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