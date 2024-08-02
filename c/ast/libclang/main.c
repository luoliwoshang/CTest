#include <clang-c/Index.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *get_absolute_path(const char *path) {
    char *absolute_path = realpath(path, NULL);
    return absolute_path ? absolute_path : path;
}

const char *get_cursor_kind_spelling(enum CXCursorKind kind) {
    switch (kind) {
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

const char *get_type_spelling(CXType type) {
    CXString type_spelling = clang_getTypeSpelling(type);
    const char *type_cstr = clang_getCString(type_spelling);
    char *result = strdup(type_cstr);
    clang_disposeString(type_spelling);
    return result;
}

typedef struct {
    CXTranslationUnit TU;
    char *namespace_name;
    char *class_name;
} Context;

void init_context(Context *context, CXTranslationUnit TU) {
    context->TU = TU;
    context->namespace_name = NULL;
    context->class_name = NULL;
}

void free_context(Context *context) {
    free(context->namespace_name);
    free(context->class_name);
}

void set_context_name(char **context_name, const char *new_name) {
    free(*context_name);
    *context_name = new_name ? strdup(new_name) : NULL;
}

void process_complex_type(CXType type, int depth) {
    const char *type_spelling = get_type_spelling(type);
    for (int i = 0; i < depth; i++)
        printf("  ");
    printf("%s\n", type_spelling);
    free((void *)type_spelling);

    // 处理函数类型
    if (type.kind == CXType_FunctionProto) {

        // 处理返回类型
        CXType return_type = clang_getResultType(type);
        for (int i = 0; i < depth + 1; i++)
            printf("  ");
        printf("Return type:\n");
        process_complex_type(return_type, depth + 2);

        // 处理参数类型
        int num_args = clang_getNumArgTypes(type);
        for (int i = 0; i < num_args; i++) {
            CXType arg_type = clang_getArgType(type, i);
            for (int j = 0; j < depth + 1; j++)
                printf("  ");
            printf("Argument %d:\n", i + 1);
            process_complex_type(arg_type, depth + 2);
        }
    }
    // 处理指针类型
    else if (type.kind == CXType_Pointer) {
        CXType pointee_type = clang_getPointeeType(type);
        process_complex_type(pointee_type, depth + 1);
    }
    // 处理数组类型
    else if (type.kind == CXType_ConstantArray) {
        CXType element_type = clang_getArrayElementType(type);
        long long array_size = clang_getArraySize(type);
        for (int i = 0; i < depth + 1; i++)
            printf("  ");
        printf("Array size: %lld\n", array_size);
        process_complex_type(element_type, depth + 1);
    }
    // 可以根据需要添加其他复杂类型的处理
}

const char *get_cursor_comment(CXCursor cursor) {
    CXString comment = clang_Cursor_getRawCommentText(cursor);
    const char *comment_str = clang_getCString(comment);
    char *result = comment_str ? strdup(comment_str) : NULL;
    clang_disposeString(comment);
    return result;
}

void print_cursor_location(CXCursor cursor) {
    CXSourceLocation location = clang_getCursorLocation(cursor);
    unsigned line, column;
    CXFile file;
    clang_getSpellingLocation(location, &file, &line, &column, NULL);
    CXString file_name = clang_getFileName(file);
    printf("File: '%s', Pos: line %d, column %d\n", clang_getCString(file_name), line, column);
    clang_disposeString(file_name);
}

void print_func_info(CXCursor cursor, Context *context) {
    CXString cursor_spelling = clang_getCursorSpelling(cursor);
    CXString symbol = clang_Cursor_getMangling(cursor);

    enum CXCursorKind cursor_kind = clang_getCursorKind(cursor);

    if (context->namespace_name && context->class_name) {
        printf("%s:%s:%s\n", context->namespace_name, context->class_name, clang_getCString(cursor_spelling));
    } else if (context->class_name) {
        printf("%s:%s\n", context->class_name, clang_getCString(cursor_spelling));
    } else {
        printf("%s\n", clang_getCString(cursor_spelling));
    }

    print_cursor_location(cursor);

    const char *comment = get_cursor_comment(cursor);
    if (comment) {
        printf("Comment:\n%s\n", comment);
        free((void *)comment);
    } else {
        printf("Comment: None\n");
    }

    if (clang_getCString(symbol)) {
        printf("Symbol: '%s'\n", clang_getCString(symbol));
    } else {
        printf("Symbol: None\n");
    }

    if (cursor_kind == CXCursor_CXXMethod || cursor_kind == CXCursor_FunctionDecl) {
        CXType func_type = clang_getCursorType(cursor);
        printf("Function type:\n");
        process_complex_type(func_type, 1);
    }
    printf("----------------------------------------\n");

    clang_disposeString(cursor_spelling);
    clang_disposeString(symbol);
}

void print_marco_info(CXTranslationUnit TU, CXCursor cursor) {
    CXSourceRange range = clang_getCursorExtent(cursor);
    CXToken *tokens;
    unsigned numTokens;
    clang_tokenize(TU, range, &tokens, &numTokens);

    CXString name = clang_getCursorSpelling(cursor);
    printf("Marco Name: %s\n", clang_getCString(name));
    print_cursor_location(cursor);
    printf("Content: ");

    for (unsigned i = 1; i < numTokens; ++i) {
        CXString spelling = clang_getTokenSpelling(TU, tokens[i]);
        printf("%s ", clang_getCString(spelling));
        clang_disposeString(spelling);
    }
    printf("\n----------------------------------------\n");
    clang_disposeString(name);
}

enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    Context *context = (Context *)client_data;
    enum CXCursorKind cursor_kind = clang_getCursorKind(cursor);

    if (cursor_kind == CXCursor_MacroDefinition) {
        print_marco_info(context->TU, cursor);
    } else if (cursor_kind == CXCursor_Namespace) {
        CXString namespace_name = clang_getCursorSpelling(cursor);
        set_context_name(&context->namespace_name, clang_getCString(namespace_name));
        clang_disposeString(namespace_name);
        clang_visitChildren(cursor, visitor, context);
        set_context_name(&context->namespace_name, NULL);
    } else if (cursor_kind == CXCursor_ClassDecl) {
        // 处理类声明
        CXString class_name = clang_getCursorSpelling(cursor);
        set_context_name(&context->class_name, clang_getCString(class_name));
        clang_disposeString(class_name);
        // 访问类内的所有子元素
        clang_visitChildren(cursor, visitor, context);
        // 访问完成后清除类名称
        set_context_name(&context->class_name, NULL);
    } else if (cursor_kind == CXCursor_CXXMethod || cursor_kind == CXCursor_FunctionDecl) {
        print_func_info(cursor, context);
    } else {
        // 对于其他类型的光标，继续遍历子元素
        clang_visitChildren(cursor, visitor, context);
    }

    return CXChildVisit_Continue;
}

// 解析C++文件
void parse(const char *filename) {
    CXIndex index = clang_createIndex(0, 0);
    const char *args[] = {"-x", "c++", "-std=c++11"};
    CXTranslationUnit unit =
        clang_parseTranslationUnit(index, filename, args, 3, NULL, 0, CXTranslationUnit_DetailedPreprocessingRecord);

    if (unit == NULL) {
        printf("Unable to parse translation unit. Quitting.\n");
        return;
    }

    Context context;
    init_context(&context, unit);
    CXCursor cursor = clang_getTranslationUnitCursor(unit);
    clang_visitChildren(cursor, visitor, &context);

    free_context(&context);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        parse(argv[1]);
    } else {
        printf("Usage: %s <C++ header file>\n", argv[0]);
    }
    return 0;
}