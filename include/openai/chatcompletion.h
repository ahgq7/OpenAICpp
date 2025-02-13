#ifndef CHATCOMPLETION_H
#define CHATCOMPLETION_H

#pragma once
#include "openai_export.h"

#include <string>
#include <vector>
#include <map>
#include <functional>
#include "openai/networkrequest.h"
#include "openai/error.h"

namespace openai {

class OPENAI_EXPORT ChatCompletion {
public:
    ChatCompletion(const std::string& apiKey);

    void setModel(const std::string& model);
    void setTemperature(double temperature);
    void setMaxTokens(int maxTokens);
    void send(const std::vector<std::map<std::string, std::string>>& messages,
              std::function<void(const std::string&, const openai::Error&)> callback);
    void setEndPoint(const std::string& newEndPoint);


private:
    std::string m_apiKey;
    std::string m_model = "gpt-4o"; //default model
    std::string m_endPoint = "https://api.openai.com/v1/chat/completions";
    double m_temperature = 0.7;
    int m_maxTokens = 1024;
    NetworkRequest m_networkRequest;
};

} // namespace openai

#endif // CHATCOMPLETION_H
