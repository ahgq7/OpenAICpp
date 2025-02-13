#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "openai/openai.h"
#include "openai/chatcompletion.h"
#include "openai/embedding.h"
#include "openai/imagegeneration.h"

using namespace openai;

int main() {
    std::string apiKey = "OPENAI_API_KEY";
    OpenAI openai(apiKey);

    // Chat Completion Example
    auto chat = openai.chat();

    std::vector<std::map<std::string, std::string>> messages;
    messages.push_back({{"role", "system"}, {"content", "You are a helpful assistant."}});
    messages.push_back({{"role", "user"}, {"content", "Merhaba, nasılsın?"}});


    // Send the first message
    chat->send(messages, [&messages, &chat](const std::string& result, const Error& error) {
        if (!error.message.empty()) {
            std::cerr << "Chat Error: " << error.message << std::endl;
            return;
        }
        std::cout << "Assistant: " << result << std::endl;

        std::cout << "User: ";
        std::string userInput;
        if (std::getline(std::cin, userInput) && userInput != "exit") {
            messages.push_back({{"role", "assistant"}, {"content", result}});
            messages.push_back({{"role", "user"}, {"content", userInput}});

            chat->send(messages, [&messages, &chat](const std::string& result, const Error& error){
                if (!error.message.empty()) {
                    std::cerr << "Chat Error: " << error.message << std::endl;
                    return;
                }
                std::cout << "Assistant: " << result << std::endl;

                std::cout << "User: ";
                std::string nextInput;
                if(std::getline(std::cin, nextInput) && nextInput != "exit"){
                    messages.push_back({{"role", "assistant"}, {"content", result}});
                    messages.push_back({{"role", "user"}, {"content", nextInput}});
                    chat->send(messages, [&messages, &chat](const std::string& result, const Error& error){
                        if (!error.message.empty()) {
                            std::cerr << "Chat Error: " << error.message << std::endl;
                            return;
                        }
                        std::cout << "Assistant: " << result << std::endl;

                    });
                }


            });

        }
    });


    // Embedding Example
    auto embedding = openai.embeddings();
    embedding->getEmbedding("Hello World", [](const std::vector<float>& embeddingResult, const Error& error){
        if (!error.message.empty()) {
            std::cerr << "Embedding Error: " << error.message << std::endl;
            return;
        }
        std::cout << "Embedding size: " << embeddingResult.size() << std::endl;
        std::cout << "First 10 values: ";
        for (size_t i = 0; i < std::min(embeddingResult.size(), (size_t)10); ++i) {
            std::cout << embeddingResult[i] << " ";
        }
        std::cout << std::endl;
    });



    //Image generation example
    auto imageGen = openai.image();
    imageGen->generateImage("A cute cat wearing a hat and sunglasses.", "1024x1024", 1, "hd", "natural",
                            [](const std::vector<std::string>& imageUrls, const Error& error){
                                if(!error.message.empty()){
                                    std::cerr << "Image Generation Error: " << error.message << std::endl;
                                    return;
                                }

                                std::cout << "Generated Image URLs:" << std::endl;
                                for(const auto& url : imageUrls){
                                    std::cout << url << std::endl;
                                }

                            });

    // Wait
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return 0;
}
