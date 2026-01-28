#include "zh_json.h"

static const char *TAG = "zh_json";

#define ZH_LOGI(msg, ...) ESP_LOGI(TAG, msg, ##__VA_ARGS__)
#define ZH_LOGE(msg, err, ...) ESP_LOGE(TAG, "[%s:%d:%s] " msg, __FILE__, __LINE__, esp_err_to_name(err), ##__VA_ARGS__)

#define ZH_ERROR_CHECK(cond, err, cleanup, msg, ...) \
    if (!(cond))                                     \
    {                                                \
        ZH_LOGE(msg, err, ##__VA_ARGS__);            \
        cleanup;                                     \
        return err;                                  \
    }

typedef enum
{
    ZH_JSON_CHAR = 1,
    ZH_JSON_INT,
    ZH_JSON_FLOAT,
    ZH_JSON_BOOL,
    ZH_JSON_OBJ,
    ZH_JSON_ARRAY

} zh_json_type_t;

static esp_err_t _zh_json_add(zh_json_t *json, char *name, void *value, uint8_t type, uint8_t precision);
static esp_err_t _zh_json_resize(zh_json_t *json, uint16_t capacity);
static esp_err_t _zh_json_create(zh_json_t *json, char *buffer, bool is_object);

esp_err_t zh_json_init(zh_json_t *json)
{
    ZH_LOGI("Json initialization begin.");
    ZH_ERROR_CHECK(json != NULL, ESP_ERR_INVALID_ARG, NULL, "Json initialization failed. Invalid argument.");
    ZH_ERROR_CHECK(json->is_initialized == false, ESP_ERR_INVALID_STATE, NULL, "Json initialization failed. Json is already initialized.");
    json->capacity = 0;
    json->size = 0;
    json->is_initialized = true;
    ZH_LOGI("Json initialization success.");
    return ESP_OK;
}

esp_err_t zh_json_free(zh_json_t *json)
{
    ZH_LOGI("Json deletion begin.");
    ZH_ERROR_CHECK(json != NULL, ESP_ERR_INVALID_ARG, NULL, "Json deletion failed. Invalid argument.");
    ZH_ERROR_CHECK(json->is_initialized == true, ESP_ERR_INVALID_STATE, NULL, "Json deletion fail. Json not initialized.");
    for (uint16_t i = 0; i < json->size; ++i)
    {
        heap_caps_free(json->names[i]);
        heap_caps_free(json->values[i]);
        heap_caps_free(json->types[i]);
    }
    heap_caps_free(json->names);
    heap_caps_free(json->values);
    heap_caps_free(json->types);
    json->is_initialized = false;
    ZH_LOGI("Json deletion success.");
    return ESP_OK;
}

esp_err_t zh_json_add_char(zh_json_t *json, char *name, char *value)
{
    ZH_LOGI("Adding char item to json begin.");
    ZH_ERROR_CHECK(json != NULL && value != NULL, ESP_ERR_INVALID_ARG, NULL, "Adding char item to json fail. Invalid argument.");
    ZH_ERROR_CHECK(json->is_initialized == true, ESP_ERR_INVALID_STATE, NULL, "Adding char item to json fail. Json not initialized.");
    esp_err_t err = _zh_json_add(json, name, value, ZH_JSON_CHAR, 0);
    ZH_ERROR_CHECK(err == ESP_OK, err, NULL, "Adding char item to json fail.");
    ZH_LOGI("Adding char item to json success.");
    return ESP_OK;
}

esp_err_t zh_json_add_int(zh_json_t *json, char *name, int value)
{
    ZH_LOGI("Adding int item to json begin.");
    ZH_ERROR_CHECK(json != NULL, ESP_ERR_INVALID_ARG, NULL, "Adding int item to json fail. Invalid argument.");
    ZH_ERROR_CHECK(json->is_initialized == true, ESP_ERR_INVALID_STATE, NULL, "Adding int item to json fail. Json not initialized.");
    esp_err_t err = _zh_json_add(json, name, &value, ZH_JSON_INT, 0);
    ZH_ERROR_CHECK(err == ESP_OK, err, NULL, "Adding int item to json fail.");
    ZH_LOGI("Adding int item to json success.");
    return ESP_OK;
}

esp_err_t zh_json_add_float(zh_json_t *json, char *name, float value, uint8_t precision)
{
    ZH_LOGI("Adding float item to json begin.");
    ZH_ERROR_CHECK(json != NULL, ESP_ERR_INVALID_ARG, NULL, "Adding float item to json fail. Invalid argument.");
    ZH_ERROR_CHECK(json->is_initialized == true, ESP_ERR_INVALID_STATE, NULL, "Adding float item to json fail. Json not initialized.");
    esp_err_t err = _zh_json_add(json, name, &value, ZH_JSON_FLOAT, precision);
    ZH_ERROR_CHECK(err == ESP_OK, err, NULL, "Adding float item to json fail.");
    ZH_LOGI("Adding float item to json success.");
    return ESP_OK;
}

esp_err_t zh_json_add_bool(zh_json_t *json, char *name, bool value)
{
    ZH_LOGI("Adding bool item to json begin.");
    ZH_ERROR_CHECK(json != NULL, ESP_ERR_INVALID_ARG, NULL, "Adding bool item to json fail. Invalid argument.");
    ZH_ERROR_CHECK(json->is_initialized == true, ESP_ERR_INVALID_STATE, NULL, "Adding bool item to json fail. Json not initialized.");
    esp_err_t err = _zh_json_add(json, name, &value, ZH_JSON_BOOL, 0);
    ZH_ERROR_CHECK(err == ESP_OK, err, NULL, "Adding bool item to json fail.");
    ZH_LOGI("Adding bool item to json success.");
    return ESP_OK;
}

esp_err_t zh_json_add_object(zh_json_t *json, char *name, char *object)
{
    ZH_LOGI("Adding object item to json begin.");
    ZH_ERROR_CHECK(json != NULL && object != NULL, ESP_ERR_INVALID_ARG, NULL, "Adding object item to json fail. Invalid argument.");
    ZH_ERROR_CHECK(json->is_initialized == true, ESP_ERR_INVALID_STATE, NULL, "Adding object item to json fail. Json not initialized.");
    esp_err_t err = _zh_json_add(json, name, object, ZH_JSON_OBJ, 0);
    ZH_ERROR_CHECK(err == ESP_OK, err, NULL, "Adding object item to json fail.");
    ZH_LOGI("Adding object item to json success.");
    return ESP_OK;
}

esp_err_t zh_json_add_array(zh_json_t *json, char *name, char *array)
{
    ZH_LOGI("Adding array item to json begin.");
    ZH_ERROR_CHECK(json != NULL && array != NULL, ESP_ERR_INVALID_ARG, NULL, "Adding array item to json fail. Invalid argument.");
    ZH_ERROR_CHECK(json->is_initialized == true, ESP_ERR_INVALID_STATE, NULL, "Adding array item to json fail. Json not initialized.");
    esp_err_t err = _zh_json_add(json, name, array, ZH_JSON_ARRAY, 0);
    ZH_ERROR_CHECK(err == ESP_OK, err, NULL, "Adding array item to json fail.");
    ZH_LOGI("Adding array item to json success.");
    return ESP_OK;
}

esp_err_t zh_json_object_create(zh_json_t *json, char *buffer)
{
    ZH_LOGI("Create json object begin.");
    ZH_ERROR_CHECK(json != NULL && buffer != NULL, ESP_ERR_INVALID_ARG, NULL, "Create json object fail. Invalid argument.");
    ZH_ERROR_CHECK(json->is_initialized == true, ESP_ERR_INVALID_STATE, NULL, "Create json object json fail. Json not initialized.");
    esp_err_t err = _zh_json_create(json, buffer, true);
    ZH_ERROR_CHECK(err == ESP_OK, err, NULL, "Create json object fail.");
    ZH_LOGI("Create json object success.");
    return ESP_OK;
}

esp_err_t zh_json_array_create(zh_json_t *json, char *buffer)
{
    ZH_LOGI("Create json array begin.");
    ZH_ERROR_CHECK(json != NULL && buffer != NULL, ESP_ERR_INVALID_ARG, NULL, "Create json array fail. Invalid argument.");
    ZH_ERROR_CHECK(json->is_initialized == true, ESP_ERR_INVALID_STATE, NULL, "Create json array json fail. Json not initialized.");
    esp_err_t err = _zh_json_create(json, buffer, false);
    ZH_ERROR_CHECK(err == ESP_OK, err, NULL, "Create json array fail.");
    ZH_LOGI("Create json array success.");
    return ESP_OK;
}

static esp_err_t _zh_json_add(zh_json_t *json, char *name, void *value, uint8_t type, uint8_t precision) // -V2008
{
    if (json->capacity == json->size)
    {
        esp_err_t err = _zh_json_resize(json, json->capacity + 1);
        ZH_ERROR_CHECK(err == ESP_OK, err, NULL, "Memory allocation fail or no free memory in the heap.");
    }
    if (name == NULL)
    {
        name = "NULL";
    }
    json->names[json->size] = (char *)heap_caps_calloc(1, strlen(name) + 1, MALLOC_CAP_8BIT);
    ZH_ERROR_CHECK(json->names[json->size] != NULL, ESP_ERR_NO_MEM, NULL, "Memory allocation fail or no free memory in the heap.");
    strcpy(json->names[json->size], name);
    char char_buffer[100];
    switch (type)
    {
    case ZH_JSON_CHAR:
    case ZH_JSON_OBJ:
    case ZH_JSON_ARRAY:
        json->values[json->size] = (char *)heap_caps_calloc(1, strlen(value) + 1, MALLOC_CAP_8BIT);
        ZH_ERROR_CHECK(json->values[json->size] != NULL, ESP_ERR_NO_MEM, NULL, "Memory allocation fail or no free memory in the heap.");
        strcpy(json->values[json->size], value);
        break;
    case ZH_JSON_INT:
        sprintf(char_buffer, "%d", *(int *)value);
        json->values[json->size] = (char *)heap_caps_calloc(1, strlen(char_buffer) + 1, MALLOC_CAP_8BIT);
        ZH_ERROR_CHECK(json->values[json->size] != NULL, ESP_ERR_NO_MEM, NULL, "Memory allocation fail or no free memory in the heap.");
        strcpy(json->values[json->size], char_buffer);
        break;
    case ZH_JSON_FLOAT:
        sprintf(char_buffer, "%.*f", precision, *(float *)value);
        json->values[json->size] = (char *)heap_caps_calloc(1, strlen(char_buffer) + 1, MALLOC_CAP_8BIT);
        ZH_ERROR_CHECK(json->values[json->size] != NULL, ESP_ERR_NO_MEM, NULL, "Memory allocation fail or no free memory in the heap.");
        strcpy(json->values[json->size], char_buffer);
        break;
    case ZH_JSON_BOOL:
        sprintf(char_buffer, "%s", (*(bool *)value == true) ? "true" : "false");
        json->values[json->size] = (char *)heap_caps_calloc(1, strlen(char_buffer) + 1, MALLOC_CAP_8BIT);
        ZH_ERROR_CHECK(json->values[json->size] != NULL, ESP_ERR_NO_MEM, NULL, "Memory allocation fail or no free memory in the heap.");
        strcpy(json->values[json->size], char_buffer);
        break;
    default:
        break;
    }
    json->types[json->size] = heap_caps_calloc(1, sizeof(type), MALLOC_CAP_8BIT);
    ZH_ERROR_CHECK(json->types[json->size] != NULL, ESP_ERR_NO_MEM, NULL, "Memory allocation fail or no free memory in the heap.");
    memcpy(json->types[json->size++], &type, sizeof(type));
    return ESP_OK;
}

static esp_err_t _zh_json_resize(zh_json_t *json, uint16_t capacity)
{
    if (capacity != 0)
    {
        json->names = heap_caps_realloc(json->names, sizeof(void *) * capacity, MALLOC_CAP_8BIT);
        json->values = heap_caps_realloc(json->values, sizeof(void *) * capacity, MALLOC_CAP_8BIT);
        json->types = heap_caps_realloc(json->types, sizeof(void *) * capacity, MALLOC_CAP_8BIT);
        ZH_ERROR_CHECK(json->names != NULL && json->values != NULL && json->types != NULL, ESP_ERR_NO_MEM, NULL, "Memory allocation fail or no free memory in the heap.");
    }
    json->capacity = capacity;
    return ESP_OK;
}

static esp_err_t _zh_json_create(zh_json_t *json, char *buffer, bool is_object)
{
    ZH_ERROR_CHECK(json != NULL && buffer != NULL, ESP_ERR_INVALID_ARG, NULL, "Invalid argument.");
    ZH_ERROR_CHECK(json->is_initialized == true, ESP_ERR_INVALID_STATE, NULL, "Json not initialized.");
    memset(buffer, 0, strlen(buffer));
    strcat(buffer, (is_object == true) ? "{" : "[");
    for (uint16_t i = 0; i < json->size; ++i)
    {
        if (is_object == true)
        {
            strcat(buffer, "\"");
            strcat(buffer, json->names[i]);
            strcat(buffer, "\":");
        }
        switch (*(uint8_t *)json->types[i])
        {
        case ZH_JSON_CHAR:
            strcat(buffer, "\"");
            strcat(buffer, json->values[i]);
            strcat(buffer, "\"");
            break;
        case ZH_JSON_INT:
        case ZH_JSON_FLOAT:
        case ZH_JSON_BOOL:
        case ZH_JSON_OBJ:
        case ZH_JSON_ARRAY:
            strcat(buffer, json->values[i]);
            break;
        default:
            break;
        }
        if (i != json->size - 1)
        {
            strcat(buffer, ",");
        }
    }
    strcat(buffer, (is_object == true) ? "}" : "]");
    return ESP_OK;
}