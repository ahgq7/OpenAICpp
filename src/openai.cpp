#include "openai/openai.h"
#include "openai/chatcompletion.h"
#include "openai/embedding.h"
#include "openai/imagegeneration.h"

namespace openai {

OpenAI::OpenAI(const std::string& apiKey) : m_apiKey(apiKey) {}

OpenAI::~OpenAI() {}

void OpenAI::setApiKey(const std::string& apiKey) {
    m_apiKey = apiKey;
}

std::string OpenAI::getApiKey() const {
    return m_apiKey;
}

std::shared_ptr<ChatCompletion> OpenAI::chat() {
    if (!m_chatCompletion) {
        m_chatCompletion = std::make_shared<ChatCompletion>(m_apiKey);
    }
    return m_chatCompletion;
}

std::shared_ptr<Embedding> OpenAI::embeddings()
{
    if(!m_embedding){
        m_embedding = std::make_shared<Embedding>(m_apiKey);
    }
    return m_embedding;
}

std::shared_ptr<ImageGeneration> OpenAI::image()
{
    if(!m_imageGeneration)
    {
        m_imageGeneration = std::make_shared<ImageGeneration>(m_apiKey);
    }
    return m_imageGeneration;
}

} // namespace openai