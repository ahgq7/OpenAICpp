# OpenAICpp

OpenAICpp is a simple and easy-to-use C++ library for accessing the OpenAI API.  This library is built using `libcurl` and `nlohmann/json` and *does not* have any external dependencies like Qt or Boost.

## Features

*   **Chat Completion:** Chat with GPT models.
*   **Embeddings:** Generate embedding vectors for text.
*   **Image Generation:** Create images with DALL-E.
*   **Asynchronous Operations:** Asynchronous API calls using callback functions.
*   **Error Handling:** Catch and handle API errors.
*   **Easy Setup:** Easy compilation and installation with CMake.
*   **Cross-Platform:** Works on Windows, Linux, and macOS (anywhere with libcurl and C++17 support).
*   **MIT License:** Open source and free to use.

## Requirements

*   A C++17 compliant compiler (e.g., GCC, Clang, MSVC)
*   CMake (3.16 or higher)
*   libcurl (including development headers)
*   nlohmann/json (header-only library, automatically downloaded with CMake)
*   An OpenAI API key

## Installation

1.  **Clone the Repository:**

    ```bash
    git clone https://github.com/ahgq7/OpenAICpp.git
    cd OpenAICpp
    ```

2.  **vcpkg (Optional, Recommended):** If `libcurl` is not installed on your system or if you are looking for an easy way to install it, you can use `vcpkg`:
    *   Install vcpkg: [https://vcpkg.io/en/getting-started](https://vcpkg.io/en/getting-started)
    *   Install `curl` (e.g., for Windows): `vcpkg install curl:x64-windows`
    *   vcpkg integration is handled automatically in `CMakeLists.txt` (using CMAKE_TOOLCHAIN_FILE).

3.  **Build with CMake:**

    ```bash
    # If you are using vcpkg (example, if vcpkg is installed in the project root directory):
    cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake
    cmake --build build

    # If you are not using vcpkg and libcurl is installed on your system:
    cmake -B build -S .
    cmake --build build
    ```

    *   **`-B build`:** Specifies the directory where build files will be generated (a directory named `build` will be created).
    *   **`-S .`:** Specifies the directory where the source code is located (`.` means the current directory).
    *   **`-DCMAKE_TOOLCHAIN_FILE=...`:** If you are using `vcpkg`, specifies the path to the toolchain file.
    *   **`cmake --build build`:** Builds the project.

4.  **(Optional) Installation:** To install the library system-wide (usually requires administrator privileges):

    ```bash
    cmake --install build
    ```

    You can change the installation directory by adding `-DCMAKE_INSTALL_PREFIX=<installation_directory>` to the CMake command.

## Usage

```c++
#include <iostream>
#include "openai/openai.h"

int main() {
    // Set your API key
    std::string apiKey = "YOUR_OPENAI_API_KEY"; // Enter your own API key here!
    openai::OpenAI openai(apiKey);

    // Chat completion example
    auto chat = openai.chat();
    std::vector<std::map<std::string, std::string>> messages = {
        {{"role", "system"}, {"content", "You are a helpful assistant."}},
        {{"role", "user"}, {"content", "Hello, how are you?"}}
    };

    chat->send(messages, [](const std::string& response, const openai::Error& error) {
        if (!error.message.empty()) {
            std::cerr << "Error: " << error.message << std::endl;
        } else {
            std::cout << "Assistant: " << response << std::endl;
        }
    });
     std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get(); // Wait for the asynchronous operation to complete

    return 0;
}