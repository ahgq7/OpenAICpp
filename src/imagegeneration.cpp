#include "openai/imagegeneration.h"
#include "openai/error.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace openai{

ImageGeneration::ImageGeneration(const std::string &apiKey) : m_apiKey(apiKey)
{

}

void ImageGeneration::setModel(const std::string &model)
{
    m_model = model;
}

void ImageGeneration::generateImage(const std::string &prompt, const std::string &size, int n, const std::string &quality, const std::string &style, std::function<void (const std::vector<std::string>&, const Error&)> callback)
{
   if (m_apiKey.empty()) {
        callback({}, APIKeyError());
        return;
    }

    json requestBody;
    requestBody["model"] = m_model;
    requestBody["prompt"] = prompt;
    requestBody["n"] = n;
    requestBody["size"] = size;
    requestBody["quality"] = quality;
    requestBody["style"] = style;

    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "application/json";
    headers["Authorization"] = "Bearer " + m_apiKey;

    m_networkRequest.sendRequest(
        "https://api.openai.com/v1/images/generations",
        "POST",
        headers,
        requestBody.dump(),
        [callback](const NetworkRequest::Response& response){
            if (response.statusCode >= 200 && response.statusCode < 300) {
                 try{
                    json responseJson = json::parse(response.body);
                    if(responseJson.contains("data") && responseJson["data"].is_array()){
                        std::vector<std::string> imageUrls;
                        for(const auto& imageObj : responseJson["data"]){
                            if(imageObj.contains("url")){
                                imageUrls.push_back(imageObj["url"].get<std::string>());
                            }
                        }
                         callback(imageUrls, Error());
                         return;
                    }
                    callback({}, JSONParseError("Unexpected image generation response format."));

                 }catch(const json::parse_error &e){
                    callback({}, JSONParseError("JSON parsing error: " + std::string(e.what())));
                 }
            }
            else
            {
                std::string errorMessage = "API Error: ";
                try{
                    json errorJson = json::parse(response.body);
                    if(errorJson.contains("error") && errorJson["error"].contains("message")){
                        errorMessage += errorJson["error"]["message"].get<std::string>();
                    } else {
                        errorMessage += "Status code: " + std::to_string(response.statusCode);
                    }

                }catch(const json::parse_error& e){
                    errorMessage += "Status code: " + std::to_string(response.statusCode) + ", " + response.body;
                }
                callback({}, NetworkError(errorMessage));
            }
        }
    );
}

} //namespace openai
