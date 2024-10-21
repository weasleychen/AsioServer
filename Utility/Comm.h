#ifndef AsioServer_Biz_Comm
#define AsioServer_Biz_Comm

#include <string>
#include <map>
#include <unordered_map>
#include <set>
#include <vector>

bool IsFileExist(const std::string &filePath);
bool IsDirectory(const std::string &filePath);
bool IsRegularFile(const std::string &filePath);
std::string ReadFile(const std::string &filePath);
std::string AccumulateHttpHeadersAsString(const std::map<std::string, std::string> &headers);

std::string Remove(const std::string &s, char c);
std::vector<std::string> Split(const std::string &s, const std::string &regex);
void Trim(std::string &data, const std::set<char> &targets = {'\r', '\t', '\n', ' '});
std::string Join(const std::vector<std::string> &vec, const std::string &delim);
std::string Concat(const std::vector<std::string> &vec, const std::string &delim);

/*
 * Merge Two map into one, take the key in lhs which is not contained in rhs into rhs
 */
template <typename T, typename P>
std::map<T, P> MergeMap(const std::map<T, P> &lhs, const std::map<T, P> &rhs) {
    std::map<T, P> result = rhs;
    for(auto &[k, v] : lhs) {
        if(!rhs.count(k)) {
            result[k] = v;
        }
    }

    return rhs;
}

template <typename T, typename P>
std::unordered_map<T, P> MergeMap(const std::unordered_map<T, P> &lhs, const std::unordered_map<T, P> &rhs) {
    std::unordered_map<T, P> result = rhs;
    for(auto &[k, v] : lhs) {
        if(!rhs.count(k)) {
            result[k] = v;
        }
    }

    return rhs;
}

#endif //AsioServer_Biz_Comm
