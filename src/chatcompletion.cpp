#include "openai/chatcompletion.h"
#include "openai/error.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace openai {

ChatCompletion::ChatCompletion(const std::string& apiKey)
    : m_apiKey(apiKey) {}

void ChatCompletion::setModel(const std::string& model) {
    m_model = model;
}

void ChatCompletion::setEndPoint(const std::string &newEndPoint) {
    m_endPoint = newEndPoint;
}

void ChatCompletion::setTemperature(double temperature) {
    m_temperature = temperature;
}
void ChatCompletion::setMaxTokens(int maxTokens)
{
    m_maxTokens = maxTokens;
}

void ChatCompletion::send(const std::vector<std::map<std::string, std::string>>& messages,
                         std::function<void(const std::string&, const Error&)> callback)
{
   if (m_apiKey.empty()) {
       callback("", APIKeyError());
        return;
    }
    json requestBody;
    requestBody["model"] = m_model;
    requestBody["messages"] = messages;
    requestBody["temperature"] = m_temperature;
    requestBody["max_tokens"] = m_maxTokens;

    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "application/json";
    headers["Authorization"] = "Bearer " + m_apiKey;

    m_networkRequest.sendRequest(
        m_endPoint,
        "POST",
        headers,
        requestBody.dump(),
        [callback](const NetworkRequest::Response& response) {
            if (response.statusCode >= 200 && response.statusCode < 300) {
                try {
                    json responseJson = json::parse(response.body);
                    if (responseJson.contains("choices") && responseJson["choices"].is_array() && !responseJson["choices"].empty()) {
                        std::string content = responseJson["choices"][0]["message"]["content"];
                        callback(content, Error()); // Başarılı, hata yok
                    } else {
                        callback("", JSONParseError("Unexpected JSON format."));
                    }

                } catch (const json::parse_error& e) {
                     callback("", JSONParseError("JSON parsing error: " + std::string(e.what())));
                }
            }
            else {
                std::string errorMessage = "API Error: ";
                try{
                    json errorJson = json::parse(response.body);
                    if(errorJson.contains("error") && errorJson["error"].contains("message")){
                        errorMessage += errorJson["error"]["message"].get<std::string>();
                    } else {
                        errorMessage += "Status code: " + std::to_string(response.statusCode);
                    }

                }catch(const json::parse_error& e){
                    errorMessage += "Status code: " + std::to_string(response.statusCode) + ",  " + response.body; //Parse edilemeyen hatalı json
                }
                callback("", NetworkError(errorMessage));
            }
        }
    );
}

} // namespace openai
