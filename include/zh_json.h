#pragma once

#include "stdio.h"
#include "string.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct zh_json_t
    {
        char **names;
        char **values;
        uint8_t capacity;
        uint8_t size;
    } zh_json_t;

    esp_err_t zh_json_init(zh_json_t *json);
    esp_err_t zh_json_free(zh_json_t *json);
    esp_err_t zh_json_add(zh_json_t *json, char *name, char *value);
    void zh_json_create(zh_json_t *json, char *buffer);

#ifdef __cplusplus
}
#endif
