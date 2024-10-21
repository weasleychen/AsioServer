#include <vector>
#include <algorithm>

#include "BaseFrame/HttpHelper.h"
#include "Utility/Comm.h"

std::optional<HttpRequestLine_t> ParseHttpRequestLine(const std::string &line) {
    std::vector<std::string> result = Split(line, " ");
    std::erase(result, "");
    std::for_each(result.begin(), result.end(), [](std::string &s) { Trim(s); });

    if(result.size() != 3) {
        return std::nullopt;
    }

    static std::set<std::string> kHttpMethod = {
        "GET", "POST", "PUT", "DELETE", "HEAD", "OPTIONS", "PATCH", "TRACE", "CONNECT",
    };

    if(!kHttpMethod.count(result[0])) {
        return std::nullopt;
    }

    return HttpRequestLine_t(result[0], result[1], result[2]);
}

std::optional<HttpHeader_t> ParseHttpHeader(const std::string &line) {
    auto it = std::find(line.begin(), line.end(), ':');
    if(it == line.end()) {
        return std::nullopt;
    }

    std::string key = line.substr(0, it - line.begin());
    std::string value = line.substr(it - line.begin() + 1);
    Trim(key);
    Trim(value);

    return HttpHeader_t(key, value);
}

std::unordered_map<std::string, std::string> ParseQueryString(const std::string &queryString) {
    std::unordered_map<std::string, std::string> result;
    std::vector kvs = Split(queryString, "&");
    std::for_each(kvs.begin(), kvs.end(),
        [&result](std::string &kv) {
            Trim(kv);
            std::vector splitKv = Split(kv, "=");
            std::for_each(splitKv.begin(), splitKv.end(),
                [](std::string &s) {
                    Trim(s);
                });

            if(splitKv.size() != 2) {
                return;
            }

            result[splitKv[0]] = splitKv[1];
        });

    return result;
}
