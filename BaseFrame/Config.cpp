#include <fstream>

#include "BaseFrame/Config.h"
#include "boost/filesystem.hpp"
#include "BaseFrame/Log.h"
#include "Utility/Comm.h"

Config::Config() = default;

Config::Config(const std::string &path) {
    Load(path);
}

void Config::Load(const std::string &path) {
    if(!boost::filesystem::exists(path)) {
        RERROR("file: {} not exist", path);
        return;
    }

    if(!boost::filesystem::is_regular_file(path)) {
        RERROR("file: {} not a regular file", path);
    }


    std::ifstream file(path);
    if(file.is_open()) {
        std::string line;
        while(std::getline(file, line)) {
            auto splitResult = Split(line, "=");
            for(auto &s : splitResult) {
                Trim(s);
            }

            if(splitResult.size() != 2) {
                continue;
            }

            confMap.emplace(splitResult[0], splitResult[1]);
        }
    }
}

std::string Config::GetImpl(const std::string &key) {
    if(!confMap.count(key)) {
        return "";
    }

    return confMap[key];
}

