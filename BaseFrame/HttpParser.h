#ifndef AsioServer_Include_HttpParser
#define AsioServer_Include_HttpParser

#include <string>
#include <unordered_map>
#include <functional>
#include <shared_mutex>
#include <mutex>
#include <numeric>
#include <cassert>

#include "Utility/Comm.h"
#include "BaseFrame/HttpHelper.h"
#include "BaseFrame/Log.h"

struct HttpMessage {
    // request line
    std::string httpMethod, requestPath, httpVersion;

    std::unordered_map<std::string, std::string> queryString;

    // headers
    std::unordered_map<std::string, std::string> headers;

    // body
    std::string body;

    HttpMessage() = default;
    HttpMessage(HttpMessage &&msg) {
        *this = std::move(msg);
    }

    HttpMessage& operator=(HttpMessage &&msg) {
        httpMethod = std::move(msg.httpMethod);
        requestPath = std::move(msg.requestPath);
        httpVersion = std::move(msg.httpVersion);
        queryString = std::move(msg.queryString);
        headers = std::move(msg.headers);
        body = std::move(msg.body);
        return *this;
    }

    [[nodiscard]] std::optional<std::string> GetHeader(const std::string &key) const {
        if(queryString.count(key)) {
            return headers.at(key);
        }
        return std::nullopt;
    }

    [[nodiscard]] std::optional<std::string> GetQueryString(const std::string &key) const {
        if(queryString.count(key)) {
            return queryString.at(key);
        }
        return std::nullopt;
    }

    [[nodiscard]] std::string GetRawHttp() const {
        return std::format("{} {} {}\r\n", httpMethod, requestPath + GetRawQueryString(), httpVersion)
                + std::accumulate(headers.begin(), headers.end(), std::string(),
                    [](std::string &&cur, auto &kv) {
                        return cur + std::format("{}: {}\r\n", kv.first, kv.second);
                    })
                + "\r\n"
                + body;
    }

    // the special version of GetRawHttp which is replaced "\r\n" into "\n"
    [[nodiscard]] std::string GetVisibleHttp() const {
        return std::format("{} {} {}\n", httpMethod, requestPath + GetRawQueryString(), httpVersion)
               + std::accumulate(headers.begin(), headers.end(), std::string(),
                                 [](std::string &&cur, auto &kv) {
                                     return cur + std::format("{}: {}\n", kv.first, kv.second);
                                 })
               + "\n"
               + body;
    }

private:
    [[nodiscard]] std::string GetRawQueryString() const {
        std::string result;
        for(auto &[k, v] : queryString) {
            result += (result.empty() ? "?" : "&") + std::format("{}={}", k, v);
        }
        return result;
    }
};

template <typename T>
class HttpParser {
public:
    using CallBack_t = std::function<void(T, HttpMessage &)>;

    explicit HttpParser(CallBack_t cb)
        : callBack(std::move(cb)) {}

    void Feed(T key, const std::string &data) {
        if(!key2Info.count(key)) {
            std::unique_lock<std::shared_mutex> ul(sharedMutex);
            key2Info[key] = HttpInfo();
        }

        std::shared_lock<std::shared_mutex> sl(sharedMutex);
        if(key2Info[key].readingStatus == kReadingHeaders) {
            // read until \r\n
            key2Info[key].readingStatus = kReadingHeaders;

            int pos = 0;
            do {
                auto nxtPos = data.find("\r\n", pos);
                if(nxtPos == std::string::npos) {
                    key2Info[key].uncompletedLine += data.substr(pos);
                    return;
                }

                std::string line = key2Info[key].GetUncompletedLine() + data.substr(pos, nxtPos - pos + 2);
                RERROR("read line: {}", Remove(line, '\r'));
                if(line == "\r\n") {
                    key2Info[key].readingStatus = kReadingBody;
                    Feed(key, data.substr(nxtPos + 2));
                    return;
                }

                do {
                    auto requestLineResult = ParseHttpRequestLine(line);
                    if(requestLineResult) {
                        key2Info[key].msg.httpMethod = requestLineResult->method;
                        key2Info[key].msg.httpVersion = requestLineResult->version;

                        std::vector splitRequestPath = Split(requestLineResult->host, "\\?");
                        assert(splitRequestPath.size() <= 2);
                        key2Info[key].msg.requestPath = splitRequestPath[0];
                        if(splitRequestPath.size() == 2) {
                            key2Info[key].msg.queryString = ParseQueryString(splitRequestPath[1]);
                        }
                        break;
                    }

                    auto headerResult = ParseHttpHeader(line);
                    if(headerResult) {
                        if(headerResult->key == "Content-Length" && key2Info[key].msg.httpMethod != "GET") {
                            try {
                                key2Info[key].leftBodySize = std::stoi(headerResult->value);
                            } catch(const std::string& exception) {
                                RERROR("Invalid Content-Length: {}", headerResult->value.c_str());
                                key2Info[key].leftBodySize = 0;
                            }
                        }

                        key2Info[key].msg.headers[headerResult->key] = headerResult->value;

                        break;
                    }

                } while(false);

                pos = nxtPos + 2;
            } while(true);
        } else {
            int validSize = std::min(key2Info[key].leftBodySize, (int)data.size());
            key2Info[key].msg.body += data.substr(0, validSize);
            key2Info[key].leftBodySize -= validSize;

            if(key2Info[key].leftBodySize == 0) {
                key2Info[key].readingStatus = kReadingEnd;
                this->callBack(key, key2Info[key].msg);
                Cancel(key);

                if(data.size() > validSize) {
                    Feed(key, data.substr(validSize));
                }
            }
        }
    }
    
    void Cancel(T key) {
        this->key2Info.erase(key);
    }

    void SetHandler(HttpParser::CallBack_t cb) {
        this->callBack = std::move(cb);
    }
private:
    enum keyReadingStatus {
        kReadingHeaders = 1, // 正在读请求行和请求头
        kReadingBody = 2, // 正在读body，还需要读HttpInfo::leftBodySize个字节
        kReadingEnd = 3, // 这个报文读完了
    };

    struct HttpInfo {
        HttpMessage msg;

        std::string uncompletedLine;
        int readingStatus;
        int leftBodySize;

        HttpInfo() {
            readingStatus = 1;
            leftBodySize = 0;
        }

        std::string GetUncompletedLine() {
            std::string result = uncompletedLine;
            uncompletedLine.clear();
            return result;
        }
    };

    CallBack_t callBack;

    std::shared_mutex sharedMutex;
    std::unordered_map<T, HttpInfo> key2Info;
};

#endif // ifdef AsioServer_Include_HttpParser