# ESP32 ESP-IDF component for json serialization

## Tested on

1. [ESP32 ESP-IDF v5.5.2](https://docs.espressif.com/projects/esp-idf/en/v5.5.2/esp32/index.html)

## SAST Tools

[PVS-Studio](https://pvs-studio.com/pvs-studio/?utm_source=website&utm_medium=github&utm_campaign=open_source) - static analyzer for C, C++, C#, and Java code.

## Using

In an existing project, run the following command to install the components:

```text
cd ../your_project/components
git clone http://git.zh.com.ru/esp_components/zh_json
```

In the application, add the component:

```c
#include "zh_json.h"
```

## Example

Recommended use <https://jsonformatter.org/json-viewer> for json formatting.

Create simple json object or json array:

```c
#include "zh_json.h"

void app_main(void)
{
    esp_log_level_set("zh_json", ESP_LOG_ERROR);
    zh_json_t json = {0};
    zh_json_init(&json);
    zh_json_add_char(&json, "Name 1", "Hello world!");
    zh_json_add_int(&json, "Name 2", -123456);
    zh_json_add_float(&json, "Name 3", 1234.56789, 2);
    zh_json_add_bool(&json, "Name 4", true);
    char buffer[128] = {0};
    zh_json_object_create(&json, buffer);
    printf("JSON object %s.\n", buffer);
    zh_json_array_create(&json, buffer);
    printf("JSON array %s.\n", buffer);
    zh_json_free(&json);
}
```

Create a complex json object or json array (with object and array):

```c
#include "zh_json.h"

void app_main(void)
{
    esp_log_level_set("zh_json", ESP_LOG_ERROR);

    zh_json_t json_sub_obj = {0};
    zh_json_init(&json_sub_obj);
    zh_json_add_char(&json_sub_obj, "Subname 1", "Hello world!");
    zh_json_add_int(&json_sub_obj, "Subname 2", -123456);
    zh_json_add_float(&json_sub_obj, "Subname 3", 123.456);
    zh_json_add_bool(&json_sub_obj, "Subname 4", true);
    char sub_obj_buffer[128] = {0};
    zh_json_object_create(&json_sub_obj, sub_obj_buffer);
    zh_json_free(&json_sub_obj);

    zh_json_t json_sub_array = {0};
    zh_json_init(&json_sub_array);
    zh_json_add_char(&json_sub_array, NULL, "Hello world!");
    zh_json_add_int(&json_sub_array, NULL, -123456);
    zh_json_add_float(&json_sub_array, NULL, 123.456);
    zh_json_add_bool(&json_sub_array, NULL, true);
    char sub_array_buffer[64] = {0};
    zh_json_array_create(&json_sub_array, sub_array_buffer);
    zh_json_free(&json_sub_array);

    zh_json_t json_complex = {0};
    zh_json_init(&json_complex);
    zh_json_add_char(&json_complex, "Name 1", "Hello world!");
    zh_json_add_int(&json_complex, "Name 2", -123456);
    zh_json_add_float(&json_complex, "Name 3", 123.456);
    zh_json_add_bool(&json_complex, "Name 4", true);
    zh_json_add_object(&json_complex, "Name 5", sub_obj_buffer);
    zh_json_add_array(&json_complex, "Name 6", sub_array_buffer);
    char complex_buffer[256] = {0};
    zh_json_object_create(&json_complex, complex_buffer);
    printf("JSON complex object %s.\n", complex_buffer);
    zh_json_array_create(&json_complex, complex_buffer);
    printf("JSON complex array %s.\n", complex_buffer);
    zh_json_free(&json_complex);
}
```
