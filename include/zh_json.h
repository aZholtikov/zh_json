/**
 * @file zh_json.h
 */

#pragma once

#include "stdio.h"
#include "string.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_heap_caps.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Main structure of json data.
     */
    typedef struct
    {
        void **names;        /*!< Array of pointers of json names. */
        void **values;       /*!< Array of pointers of json values. */
        void **types;        /*!< Array of pointers of json types. */
        uint16_t capacity;   /*!< Maximum capacity of the json. */
        uint16_t size;       /*!< Number of items in the json. */
        bool is_initialized; /*!< Json initialization status flag. */
    } zh_json_t;

    /**
     * @brief Initialize json.
     *
     * @param[in,out] json Pointer to structure of json.
     *
     * @return ESP_OK if success or an error code otherwise.
     */
    esp_err_t zh_json_init(zh_json_t *json);

    /**
     * @brief Deinitialize json. Free all allocated memory.
     *
     * @param[in,out] json Pointer to structure of json.
     *
     * @return ESP_OK if success or an error code otherwise.
     */
    esp_err_t zh_json_free(zh_json_t *json);

    /**
     * @brief Add char item at end of json.
     *
     * @note For json array name will be ignored (can be NULL).
     *
     * @param[in,out] json Pointer to structure of json.
     * @param[in] name Pointer to name for add.
     * @param[in] value Pointer to value for add.
     *
     * @return ESP_OK if success or an error code otherwise.
     */
    esp_err_t zh_json_add_char(zh_json_t *json, char *name, char *value);

    /**
     * @brief Add int item at end of json.
     *
     * @note For json array name will be ignored (can be NULL).
     *
     * @param[in,out] json Pointer to structure of json.
     * @param[in] name Pointer to name for add.
     * @param[in] value Value for add.
     *
     * @return ESP_OK if success or an error code otherwise.
     */
    esp_err_t zh_json_add_int(zh_json_t *json, char *name, int value);

    /**
     * @brief Add float item at end of json.
     *
     * @note For json array name will be ignored (can be NULL).
     *
     * @param[in,out] json Pointer to structure of json.
     * @param[in] name Pointer to name for add.
     * @param[in] value Value for add.
     *
     * @return ESP_OK if success or an error code otherwise.
     */
    esp_err_t zh_json_add_float(zh_json_t *json, char *name, float value);

    /**
     * @brief Add bool item at end of json.
     *
     * @note For json array name will be ignored (can be NULL).
     *
     * @param[in,out] json Pointer to structure of json.
     * @param[in] name Pointer to name for add.
     * @param[in] value Value for add.
     *
     * @return ESP_OK if success or an error code otherwise.
     */
    esp_err_t zh_json_add_bool(zh_json_t *json, char *name, bool value);

    /**
     * @brief Add object item at end of json.
     *
     * @note For json array name will be ignored (can be NULL).
     *
     * @param[in,out] json Pointer to structure of json.
     * @param[in] name Pointer to name for add.
     * @param[in] value Pointer to value for add.
     *
     * @return ESP_OK if success or an error code otherwise.
     */
    esp_err_t zh_json_add_object(zh_json_t *json, char *name, char *object);

    /**
     * @brief Add array item at end of json.
     *
     * @note For json array name will be ignored (can be NULL).
     *
     * @param[in,out] json Pointer to structure of json.
     * @param[in] name Pointer to name for add.
     * @param[in] value Pointer to value for add.
     *
     * @return ESP_OK if success or an error code otherwise.
     */
    esp_err_t zh_json_add_array(zh_json_t *json, char *name, char *array);

    /**
     * @brief Create json object.
     *
     * @param[in,out] json Pointer to structure of json.
     * @param[out] buffer Pointer to buffer for json.
     *
     * @return ESP_OK if success or an error code otherwise.
     */
    esp_err_t zh_json_object_create(zh_json_t *json, char *buffer);

    /**
     * @brief Create json array.
     *
     * @param[in,out] json Pointer to structure of json.
     * @param[out] buffer Pointer to buffer for json.
     *
     * @return ESP_OK if success or an error code otherwise.
     */
    esp_err_t zh_json_array_create(zh_json_t *json, char *buffer);

#ifdef __cplusplus
}
#endif
