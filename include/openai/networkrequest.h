#ifndef NETWORKREQUEST_H
#define NETWORKREQUEST_H

#include <string>
#include <functional>
#include <map>
#include "openai/error.h"

namespace openai {

class NetworkRequest {
public:
    struct Response {
        int statusCode;
        std::string body;
        std::map<std::string, std::string> headers;
    };

    void sendRequest(
        const std::string& url,
        const std::string& method, // "GET" veya "POST"
        const std::map<std::string, std::string>& headers,
        const std::string& body,
        std::function<void(Response)> callback);


};

} // namespace openai

#endif // NETWORKREQUEST_H
