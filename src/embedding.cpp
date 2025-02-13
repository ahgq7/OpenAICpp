#include "openai/embedding.h"
#include "openai/error.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace openai{


Embedding::Embedding(const std::string &apiKey) : m_apiKey(apiKey)
{

}

void Embedding::setModel(const std::string &model)
{
    m_model = model;
}

void Embedding::getEmbedding(const std::string &input, std::function<void (const std::vector<float>&, const Error&)> callback)
{
     if (m_apiKey.empty()) {
       callback({}, APIKeyError());
        return;
    }

    json requestBody;
    requestBody["model"] = m_model;
    requestBody["input"] = input;

     std::map<std::string, std::string> headers;
    headers["Content-Type"] = "application/json";
    headers["Authorization"] = "Bearer " + m_apiKey;

     m_networkRequest.sendRequest(
        "https://api.openai.com/v1/embeddings",
        "POST",
        headers,
        requestBody.dump(),
        [callback](const NetworkRequest::Response& response){
             if (response.statusCode >= 200 && response.statusCode < 300) {
                try{
                    json responseJson = json::parse(response.body);
                    if(responseJson.contains("data") && responseJson["data"].is_array() && !responseJson["data"].empty()){
                        if(responseJson["data"][0].contains("embedding") && responseJson["data"][0]["embedding"].is_array()){
                            std::vector<float> embedding = responseJson["data"][0]["embedding"].get<std::vector<float>>();
                            callback(embedding, Error()); //Başarılı
                            return;
                        }
                    }
                    callback({}, JSONParseError("Unexpected embedding response format"));

                }catch(const json::parse_error& e){
                    callback({}, JSONParseError("JSON parsing error: " + std::string(e.what())));
                }
             }
             else{
                std::string errorMessage = "API Error: ";
                try{
                    json errorJson = json::parse(response.body);
                    if(errorJson.contains("error") && errorJson["error"].contains("message")){
                        errorMessage += errorJson["error"]["message"].get<std::string>();
                    } else {
                        errorMessage += "Status code: " + std::to_string(response.statusCode);
                    }

                }catch(const json::parse_error& e){
                    errorMessage += "Status code: " + std::to_string(response.statusCode) + ", " + response.body; //Parse edilemeyen json
                }

                 callback({}, NetworkError(errorMessage));
             }
        }
     );
}

} //namespace openai
