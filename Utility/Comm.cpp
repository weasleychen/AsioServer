#include <fstream>
#include <numeric>
#include <format>

#include "Utility/Comm.h"
#include "boost/filesystem.hpp"
#include "boost/regex.hpp"
#include "boost/algorithm/string/regex.hpp"

bool IsFileExist(const std::string &filePath) {
    return boost::filesystem::exists(filePath);
}

bool IsDirectory(const std::string &filePath) {
    return boost::filesystem::is_directory(filePath);
}

bool IsRegularFile(const std::string &filePath) {
    return boost::filesystem::is_regular_file(filePath);
}

std::string ReadFile(const std::string &filePath) {
    if(!IsFileExist(filePath) || !IsRegularFile(filePath)) {
        return "";
    }

    std::ifstream file(filePath);  // 打开文件
    if(!file.is_open()) {
        return "";
    }

    return std::string{std::istreambuf_iterator<char>(file),
                       std::istreambuf_iterator<char>()};
}

std::string AccumulateHttpHeadersAsString(const std::map<std::string, std::string> &headers) {
    std::size_t totalSize = std::accumulate(headers.begin(), headers.end(), std::size_t(0),
                                [](std::size_t cur, const std::pair<std::string, std::string> &pair) {
                                    // {}: {}\r\n
                                    return cur + pair.first.size() + pair.second.size() + 4;
                                });

    std::string result;
    result.reserve(totalSize);

    for(auto &[k, v] : headers) {
        result += std::format("{}: {}\r\n", k, v);
    }

    return result;
}


std::string Concat(const std::vector<std::string> &vec, const std::string &delim) {
    std::size_t totalLength = std::accumulate(vec.begin(), vec.end(), std::size_t(0),
                                              [](std::size_t cur, const std::string &s){
                                                  return cur + s.size();
                                              })
                              + (vec.empty() ? 0 : ((vec.size() - 1) * delim.size()));

    std::string result;
    result.reserve(totalLength);

    for(auto &s : vec) {
        if(!result.empty()) {
            result += delim;
        }
        result += s;
    }

    return result;
}

std::string Join(const std::vector<std::string> &vec, const std::string &delim) {
    return Concat(vec, delim);
}


std::string Remove(const std::string &s, char c) {
    std::string result;
    std::for_each(s.begin(), s.end(), [&](char _c) {
        if(_c != c) {
            result += _c;
        }
    });

    return result;
}

std::vector<std::string> Split(const std::string &s, const std::string &regex) {
    std::vector<std::string> result;
    boost::algorithm::split_regex(result, s, boost::regex(regex));
    return result;
}

void Trim(std::string &data, const std::set<char> &targets) {
    int firstIdx = 0, lastIdx = static_cast<int>(data.size()) - 1;
    while(firstIdx < data.size() && targets.count(data[firstIdx])) ++firstIdx;
    while(lastIdx >= 0 && targets.count(data[lastIdx])) --lastIdx;

    if(firstIdx > lastIdx) {
        data.clear();
        return;
    }

    data = data.substr(firstIdx, lastIdx - firstIdx + 1);
}
