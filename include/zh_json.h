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
    } __attribute__((packed)) zh_json_t;

    /**
     * @brief      Initialize json.
     *
     * @param[in]  json   Pointer to structure of json.
     *
     * @return
     *              - ESP_OK if initialization was success
     *              - ESP_ERR_INVALID_ARG if parameter error
     */
    esp_err_t zh_json_init(zh_json_t *json);

    /**
     * @brief      Deinitialize json. Free all allocated memory.
     *
     * @param[in]  json   Pointer to structure of json.
     *
     * @return
     *              - ESP_OK if deinitialization was success
     *              - ESP_ERR_INVALID_ARG if parameter error
     */
    esp_err_t zh_json_free(zh_json_t *json);

    /**
     * @brief      Add item at end of json. If sufficient memory is not available then it will resize the memory.
     *
     * @param[in]  json   Pointer to structure of json.
     * @param[in]  name   Pointer to name for add.
     * @param[in]  value  Pointer to value for add.
     *
     * @return
     *              - ESP_OK if add was success
     *              - ESP_ERR_INVALID_ARG if parameter error
     */
    esp_err_t zh_json_add(zh_json_t *json, char *name, char *value);

    /**
     * @brief      Create json.
     *
     * @param[in]  json   Pointer to structure of json.
     * @param[out] buffer Pointer to buffer for json.
     *
     * @return
     *              - ESP_OK if creation was success
     *              - ESP_ERR_INVALID_ARG if parameter error
     */
    esp_err_t zh_json_create(zh_json_t *json, char *buffer);

#ifdef __cplusplus
}
#endif
