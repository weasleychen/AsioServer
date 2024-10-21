#pragma once

#include <string>
#include <optional>
#include <vector>
#include <set>
#include <unordered_map>

struct HttpRequestLine_t {
    std::string method, host, version;

    HttpRequestLine_t() = default;
    HttpRequestLine_t(const std::string &m, const std::string &h, const std::string &v) {
        this->method = m;
        this->host = h;
        this->version = v;
    }
};

struct HttpHeader_t {
    std::string key, value;

    HttpHeader_t() = default;
    HttpHeader_t(const std::string &k, const std::string &v) {
        this->key = k;
        this->value = v;
    }
};

std::optional<HttpRequestLine_t> ParseHttpRequestLine(const std::string &line);
std::optional<HttpHeader_t> ParseHttpHeader(const std::string &line);
std::unordered_map<std::string, std::string> ParseQueryString(const std::string &queryString);




