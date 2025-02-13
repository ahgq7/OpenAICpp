#ifndef OPENAI_H
#define OPENAI_H

#pragma once
#include "openai_export.h"

#include <string>
#include <memory>

namespace openai {

class ChatCompletion;
class Embedding;
class ImageGeneration;

class OPENAI_EXPORT OpenAI {
public:
    OpenAI(const std::string& apiKey);
    ~OpenAI();

    void setApiKey(const std::string& apiKey);
    std::string getApiKey() const;

    std::shared_ptr<ChatCompletion> chat();
    std::shared_ptr<Embedding> embeddings();
    std::shared_ptr<ImageGeneration> image();


private:
    std::string m_apiKey;
    std::shared_ptr<ChatCompletion> m_chatCompletion;
    std::shared_ptr<Embedding> m_embedding;
    std::shared_ptr<ImageGeneration> m_imageGeneration;
};

} // namespace openai

#endif // OPENAI_H
