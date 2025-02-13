#include "openai/networkrequest.h"
#include <curl/curl.h>
#include <iostream>
#include <sstream>

namespace openai {

struct WriteCallbackData {
    std::string receivedData;
};

struct HeaderCallbackData {
    std::map<std::string, std::string> headers;
};

size_t writeCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    WriteCallbackData* data = static_cast<WriteCallbackData*>(userdata);
    if (data) {
        data->receivedData.append(ptr, size * nmemb);
        return size * nmemb;
    }
    return 0;
}

size_t headerCallback(char *buffer, size_t size, size_t nitems, void *userdata)
{
    size_t numBytes = size * nitems;
    std::string headerLine(buffer, numBytes);

    size_t colonPos = headerLine.find(':');
    if (colonPos != std::string::npos) {
        std::string key = headerLine.substr(0, colonPos);
        std::string value = headerLine.substr(colonPos + 1);

        size_t firstNonSpace = value.find_first_not_of(" \t");
        size_t lastNonSpace = value.find_last_not_of(" \t\r\n");

        if (firstNonSpace != std::string::npos && lastNonSpace != std::string::npos) {
            value = value.substr(firstNonSpace, lastNonSpace - firstNonSpace + 1);
        }
        else
        {
            value = "";
        }

        firstNonSpace = key.find_first_not_of(" \t");
        lastNonSpace = key.find_last_not_of(" \t\r\n");
        if (firstNonSpace != std::string::npos && lastNonSpace != std::string::npos) {
            key = key.substr(firstNonSpace, lastNonSpace - firstNonSpace + 1);
        }
        else
        {
            key = "";
        }

        HeaderCallbackData* data = static_cast<HeaderCallbackData*>(userdata);
        if(data){
            data->headers[key] = value;
        }

    }

    return numBytes;
}

void NetworkRequest::sendRequest(
    const std::string& url,
    const std::string& method,
    const std::map<std::string, std::string>& headers,
    const std::string& body,
    std::function<void(Response)> callback) {

    CURL* curl = curl_easy_init();
    if (curl) {
        CURLcode res;
        WriteCallbackData writeData;
        HeaderCallbackData headerData;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        if (method == "POST") {
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        } else if (method != "GET") {
            Response errorResponse;
            errorResponse.statusCode = -1;
            errorResponse.body = "Unsupported HTTP method: " + method;
            callback(errorResponse);
            curl_easy_cleanup(curl);
            return;
        }

        struct curl_slist* headerList = nullptr;
        for (const auto& [key, value] : headers) {
            std::string header = key + ": " + value;
            headerList = curl_slist_append(headerList, header.c_str());
        }
        if (headerList) {
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
        }

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &writeData);

        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, headerCallback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &headerData);

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        res = curl_easy_perform(curl);

        Response response;
        if (res != CURLE_OK) {
            response.statusCode = -1;
            response.body = curl_easy_strerror(res);
        } else {
            long httpCode = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
            response.statusCode = static_cast<int>(httpCode);
            response.body = writeData.receivedData;
            response.headers = headerData.headers;
        }

        callback(response);

        if (headerList) {
            curl_slist_free_all(headerList);
        }
        curl_easy_cleanup(curl);
    } else {
        Response errorResponse;
        errorResponse.statusCode = -1;
        errorResponse.body = "Failed to initialize libcurl";
        callback(errorResponse);
    }
}

} // namespace openai
