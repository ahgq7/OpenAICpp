#ifndef EMBEDDING_H
#define EMBEDDING_H

#pragma once
#include "openai_export.h"

#include <string>
#include <vector>
#include <map> // EKLENDİ
#include <functional>
#include "openai/networkrequest.h"
#include "openai/error.h"

namespace openai{

class OPENAI_EXPORT Embedding
{
public:
    Embedding(const std::string& apiKey);
    void setModel(const std::string& model);
    void getEmbedding(const std::string& input,  std::function<void(const std::vector<float>&, const openai::Error&)> callback);

private:
    std::string m_apiKey;
    std::string m_model = "text-embedding-ada-002";
    NetworkRequest m_networkRequest;
};

} //namespace openai
#endif // EMBEDDING_H
