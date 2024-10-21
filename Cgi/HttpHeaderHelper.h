#ifndef AsioServer_Cgi_HttpHeaderHelper
#define AsioServer_Cgi_HttpHeaderHelper

#include <utility>

inline constexpr std::pair<const char*, const char*> kKeepAlive = {"Connection", "keep-alive"};
inline constexpr std::pair<const char*, const char*> kHtmlFormat = {"Content-Type", "text/html"};
inline constexpr std::pair<const char*, const char*> kRawTextFormat = {"Content-Type", "text/plain"};
inline constexpr std::pair<const char*, const char*> kJpgFormat = {"Content-Type", "image/jpg;"};
inline constexpr std::pair<const char*, const char*> kPngFormat = {"Content-Type", "image/png;"};

std::pair<std::string, std::string> SpecifyDownloadFileName(const std::string &fileName);

#endif // AsioServer_Cgi_HttpHeaderHelper
