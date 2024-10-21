#ifndef AsioServer_Include_Config
#define AsioServer_Include_Config

#include <unordered_map>
#include <string>

#include "Utility/CastUtil.h"

class Config {
public:
    Config();
    explicit Config(const std::string &path);

    void Load(const std::string &path);

    template <typename _DstType>
    _DstType Get(const std::string &key, const _DstType defaultValue = _DstType{}) {
        if(!confMap.count(key)) {
            return defaultValue;
        }

        if constexpr(std::is_same_v<_DstType, std::string>) {
            return GetImpl(key);
        } else {
            return CastTo<_DstType>(GetImpl(key));
        }
    }

protected:
    std::string GetImpl(const std::string &key);
private:
    std::unordered_map<std::string, std::string> confMap;
};

#endif // ifdef AsioServer_include_AsioConfig
