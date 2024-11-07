#include <clang-c/CXString.h>
#include <clang-c/Index.h>
#include <stdio.h>
#include <string.h>

void inclusionVisitor(CXFile included_file, CXSourceLocation *inclusion_stack, unsigned include_len,
                      void *client_data) {
    CXString filename = clang_getFileName(included_file);
    printf("Included file: %s\n", clang_getCString(filename));

    for (unsigned i = 0; i < include_len; i++) {
        CXFile file;
        unsigned line, column;
        clang_getSpellingLocation(inclusion_stack[i], &file, &line, &column, NULL);

        CXString includingFilename = clang_getFileName(file);
        printf("  included from: %s:%u\n", clang_getCString(includingFilename), line);

        clang_disposeString(includingFilename);
    }

    clang_disposeString(filename);
}

int main() {
    CXIndex index = clang_createIndex(0, 0);

    const char *source = "#include <stdio.h>\n"
                         "#include \"test.h\"\n"
                         "int main() {\n"
                         "    return 0;\n"
                         "}\n";

    struct CXUnsavedFile unsaved_file;
    unsaved_file.Filename = "test.c";
    unsaved_file.Contents = source;
    unsaved_file.Length = strlen(source);

    int parse_options = CXTranslationUnit_DetailedPreprocessingRecord;

    CXTranslationUnit tu = clang_parseTranslationUnit(index,
                                                      "test.c",         // 源文件名
                                                      NULL, 0,          // 命令行参数
                                                      &unsaved_file, 1, // 未保存的文件
                                                      parse_options);

    if (tu == NULL) {
        printf("Error creating translation unit\n");
        return 1;
    }

    clang_getInclusions(tu, inclusionVisitor, NULL);

    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(index);

    return 0;
}