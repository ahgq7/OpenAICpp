
#ifndef IMAGEGENERATION_H
#define IMAGEGENERATION_H

#pragma once
#include "openai_export.h"

#include <string>
#include <vector>
#include <map> // EKLENDİ
#include <functional>
#include "openai/networkrequest.h"
#include "openai/error.h"

namespace openai{

class OPENAI_EXPORT ImageGeneration
{
public:
    ImageGeneration(const std::string& apiKey);
    void setModel(const std::string& model);
    void generateImage(const std::string& prompt, const std::string& size, int n, const std::string& quality, const std::string& style, std::function<void(const std::vector<std::string>&, const openai::Error&)> callback);

private:
     std::string m_apiKey;
     std::string m_model = "dall-e-3";
     NetworkRequest m_networkRequest;
};

} //namespace openai
#endif //IMAGEGENERATION_H
