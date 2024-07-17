#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>

/* 解析 JSON 字符串的函数 */
void parse_json(const char *json_string) {
    cJSON *json = cJSON_Parse(json_string);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error in cJSON_Parse: %s\n", error_ptr);
        }
        return;
    }

    const cJSON *name = cJSON_GetObjectItemCaseSensitive(json, "name");
    const cJSON *age = cJSON_GetObjectItemCaseSensitive(json, "age");

    if (cJSON_IsString(name) && (name->valuestring != NULL)) {
        printf("Name: %s\n", name->valuestring);
    }

    if (cJSON_IsNumber(age)) {
        printf("Age: %d\n", age->valueint);
    }

    cJSON_Delete(json);
}

/* 生成 JSON 字符串的函数 */
void create_json() {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "name", "John Doe");
    cJSON_AddNumberToObject(json, "age", 30);
    cJSON_AddBoolToObject(json, "has_license", 1);

    char *json_string = cJSON_Print(json);
    if (json_string != NULL) {
        printf("Generated JSON:\n%s\n", json_string);
        free(json_string);
    }

    cJSON_Delete(json);
}

int main() {
    const char *json_data = "{\"name\":\"John Doe\",\"age\":30}";
    parse_json(json_data);
    create_json();

    return 0;
}
