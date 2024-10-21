#include <format>

#include "HttpHeaderHelper.h"

std::pair<std::string, std::string> SpecifyDownloadFileName(const std::string &fileName) {
    return {
        "Content-Disposition",
        std::format("attachment; filename=\"{}\"", fileName)
    };
}