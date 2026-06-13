# zh_json - JSON Serialization Component for ESP-IDF

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
- [API Reference](#api-reference)
- [Usage Examples](#usage-examples)
- [Technical Specifications](#technical-specifications)
- [Error Codes](#error-codes)
- [Contributing](#contributing)
- [License](#license)

---

## Overview

`zh_json` is a lightweight JSON serialization component for ESP-IDF (Espressif IoT Development Framework). It provides a simple and efficient way to create JSON objects and arrays in ESP32 applications. The component supports various data types (strings, integers, floats, booleans) and nested structures (objects and arrays).

The component is designed specifically for ESP32 microcontrollers and uses ESP-IDF's heap management functions for optimal memory allocation with different memory caps (internal/external RAM).

---

## Features

1. **Support for Various Data Types**: Strings (char*), integers (int), floats, booleans (bool)
2. **Nested Structures**: Support for nesting JSON objects and arrays
3. **Automatic Memory Management**: Automatic memory allocation and deallocation for names and values
4. **Dynamic Resizing**: JSON capacity automatically grows when items are added
5. **Maximum Capacity**: Up to 65,535 elements (16-bit index limit)
6. **ESP-IDF Optimized**: Uses heap_caps functions for memory allocation with memory caps
7. **Error Handling**: Comprehensive error checking with detailed logging
8. **Thread-Safe**: Not thread-safe by default (can be made thread-safe with external mutex)
9. **Minimal Overhead**: Low memory and CPU overhead

---

## Installation

1. Navigate to your project's components directory:

```bash
cd ../your_project/components
```

2. Clone the repository:

```bash
git clone https://github.com/aZholtikov/zh_json
```

3. In your application, include the header:

```c
#include "zh_json.h"
```

4. The component will be automatically built with your project.

---

## API Reference

### zh_json_t Structure

```c
typedef struct
{
    void **names;        // Array of pointers of JSON names
    void **values;       // Array of pointers of JSON values
    void **types;        // Array of pointers of JSON value types
    uint16_t capacity;   // Maximum capacity of the JSON
    uint16_t size;       // Number of items in the JSON
    bool is_initialized; // JSON initialization status flag
} zh_json_t;
```

---

### zh_json_init()

Initializes the JSON structure.

**Parameters:**

- `json` - Pointer to the JSON structure

**Returns:**

- `ESP_OK` - Success
- `ESP_ERR_INVALID_ARG` - Invalid argument (NULL json)
- `ESP_ERR_INVALID_STATE` - JSON already initialized

**Example:**

```c
zh_json_t json = {0};
zh_json_init(&json);
```

---

### zh_json_free()

Deinitializes the JSON and frees all allocated memory.

**Parameters:**

- `json` - Pointer to the JSON structure

**Returns:**

- `ESP_OK` - Success
- `ESP_ERR_INVALID_ARG` - Invalid argument (NULL json)
- `ESP_ERR_INVALID_STATE` - JSON not initialized

**Note:** All dynamically allocated memory for names and values is also freed.

---

### zh_json_add_char()

Adds a string item to the end of JSON.

**Note:** For JSON array, the name will be ignored (can be NULL).

**Parameters:**

- `json` - Pointer to the JSON structure
- `name` - Pointer to the name to add
- `value` - Pointer to the value to add

**Returns:**

- `ESP_OK` - Success
- `ESP_ERR_INVALID_ARG` - Invalid argument (NULL json or value)
- `ESP_ERR_INVALID_STATE` - JSON not initialized

---

### zh_json_add_int()

Adds an integer item to the end of JSON.

**Note:** For JSON array, the name will be ignored (can be NULL).

**Parameters:**

- `json` - Pointer to the JSON structure
- `name` - Pointer to the name to add
- `value` - Value to add

**Returns:**

- `ESP_OK` - Success
- `ESP_ERR_INVALID_ARG` - Invalid argument (NULL json)
- `ESP_ERR_INVALID_STATE` - JSON not initialized

---

### zh_json_add_float()

Adds a float item to the end of JSON.

**Note:** For JSON array, the name will be ignored (can be NULL).

**Parameters:**

- `json` - Pointer to the JSON structure
- `name` - Pointer to the name to add
- `value` - Value to add
- `precision` - Number of decimal places

**Returns:**

- `ESP_OK` - Success
- `ESP_ERR_INVALID_ARG` - Invalid argument (NULL json)
- `ESP_ERR_INVALID_STATE` - JSON not initialized

---

### zh_json_add_bool()

Adds a boolean item to the end of JSON.

**Note:** For JSON array, the name will be ignored (can be NULL).

**Parameters:**

- `json` - Pointer to the JSON structure
- `name` - Pointer to the name to add
- `value` - Value to add

**Returns:**

- `ESP_OK` - Success
- `ESP_ERR_INVALID_ARG` - Invalid argument (NULL json)
- `ESP_ERR_INVALID_STATE` - JSON not initialized

---

### zh_json_add_object()

Adds a nested JSON object to the end of JSON.

**Note:** For JSON array, the name will be ignored (can be NULL).

**Parameters:**

- `json` - Pointer to the JSON structure
- `name` - Pointer to the name to add
- `object` - Pointer to the string with prepared JSON object

**Returns:**

- `ESP_OK` - Success
- `ESP_ERR_INVALID_ARG` - Invalid argument (NULL json or object)
- `ESP_ERR_INVALID_STATE` - JSON not initialized

---

### zh_json_add_array()

Adds a nested JSON array to the end of JSON.

**Note:** For JSON array, the name will be ignored (can be NULL).

**Parameters:**

- `json` - Pointer to the JSON structure
- `name` - Pointer to the name to add
- `array` - Pointer to the string with prepared JSON array

**Returns:**

- `ESP_OK` - Success
- `ESP_ERR_INVALID_ARG` - Invalid argument (NULL json or array)
- `ESP_ERR_INVALID_STATE` - JSON not initialized

---

### zh_json_object_create()

Creates a JSON object in a buffer.

**Parameters:**

- `json` - Pointer to the JSON structure
- `buffer` - Pointer to the buffer for the result

**Returns:**

- `ESP_OK` - Success
- `ESP_ERR_INVALID_ARG` - Invalid argument (NULL json or buffer)
- `ESP_ERR_INVALID_STATE` - JSON not initialized

---

### zh_json_array_create()

Creates a JSON array in a buffer.

**Parameters:**

- `json` - Pointer to the JSON structure
- `buffer` - Pointer to the buffer for the result

**Returns:**

- `ESP_OK` - Success
- `ESP_ERR_INVALID_ARG` - Invalid argument (NULL json or buffer)
- `ESP_ERR_INVALID_STATE` - JSON not initialized

---

## Usage Examples

### Creating a Simple JSON Object or Array

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

### Creating a Complex JSON Object or Array (with Nested Structures)

```c
#include "zh_json.h"

void app_main(void)
{
    esp_log_level_set("zh_json", ESP_LOG_ERROR);
    // Create nested object
    zh_json_t json_sub_obj = {0};
    zh_json_init(&json_sub_obj);
    zh_json_add_char(&json_sub_obj, "Subname 1", "Hello world!");
    zh_json_add_int(&json_sub_obj, "Subname 2", -123456);
    zh_json_add_float(&json_sub_obj, "Subname 3", 1234.56789, 2);
    zh_json_add_bool(&json_sub_obj, "Subname 4", true);
    char sub_obj_buffer[128] = {0};
    zh_json_object_create(&json_sub_obj, sub_obj_buffer);
    zh_json_free(&json_sub_obj);
    // Create nested array
    zh_json_t json_sub_array = {0};
    zh_json_init(&json_sub_array);
    zh_json_add_char(&json_sub_array, NULL, "Hello world!");
    zh_json_add_int(&json_sub_array, NULL, -123456);
    zh_json_add_float(&json_sub_array, NULL, 1234.56789, 2);
    zh_json_add_bool(&json_sub_array, NULL, true);
    char sub_array_buffer[64] = {0};
    zh_json_array_create(&json_sub_array, sub_array_buffer);
    zh_json_free(&json_sub_array);
    // Create main object with nested structures
    zh_json_t json_complex = {0};
    zh_json_init(&json_complex);
    zh_json_add_char(&json_complex, "Name 1", "Hello world!");
    zh_json_add_int(&json_complex, "Name 2", -123456);
    zh_json_add_float(&json_complex, "Name 3", 1234.56789, 2);
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

---

## Technical Specifications

| Parameter | Value |
|-----------|-------|
| **Maximum Capacity** | 65,535 elements |
| **Index Type** | uint16_t (16-bit) |
| **Memory Management** | heap_caps_calloc, heap_caps_realloc, heap_caps_free |
| **Memory Caps** | MALLOC_CAP_8BIT |
| **Thread Safety** | Not thread-safe |
| **ESP-IDF Version** | >= 5.0 |
| **Platform** | ESP32 series |
| **Language** | C (C99) |

---

## Error Codes

| Error Code | Description |
|------------|-------------|
| `ESP_OK` | Operation successful |
| `ESP_ERR_INVALID_ARG` | Invalid argument (NULL pointer) |
| `ESP_ERR_INVALID_STATE` | JSON not initialized or already initialized |
| `ESP_ERR_NO_MEM` | Memory allocation failed (out of memory) |
| `ESP_FAIL` | General failure |

---

## Contributing

Contributions are welcome! To contribute:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

Please ensure your code follows the existing style and includes appropriate documentation.

---

## License

This project is licensed under the Apache License, Version 2.0 - see the [LICENSE](LICENSE) file for details.

### Apache License, Version 2.0

Copyright (c) 2026 Alexey Zholtikov

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

---

## Additional Notes

- **Memory Overhead**: Each element requires additional memory for pointers in internal arrays
- **Performance**: O(1) for adding elements, O(n) for creating JSON string
- **Best Practices**:
  - Always initialize JSON before use
  - Free JSON when done to avoid memory leaks
  - Consider the maximum size limit (65,535 elements)
  - For nested structures, create objects/arrays separately and pass them to the main object

---

*Updated for zh_json v1.1.0*
