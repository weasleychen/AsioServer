#ifndef AsioServer_Utility_CastUtil
#define AsioServer_Utility_CastUtil

#include <string>
#include <cstdint>
#include <type_traits>

template <typename T>
struct IsString {
    static constexpr bool value = std::is_convertible<T, std::string>::value;
};

template <typename T>
constexpr bool IsString_v = IsString<T>::value;

/*
 * This template allow cast
 *      number type to std::string
 *      or std::string to number type
 */
template <typename _DstType, typename _SrcType,
        typename = typename std::enable_if<
                IsString_v<_DstType> ||
                (IsString_v<_SrcType> && std::is_arithmetic_v<_DstType>)>>
_DstType CastTo(_SrcType &&v) {
    if constexpr(std::is_same_v<std::string, _DstType>) {
        return std::to_string(v);
    } else {
        if constexpr(std::is_same_v<std::int32_t, _DstType> ||
                     std::is_same_v<std::int16_t, _DstType> ||
                     std::is_same_v<std::int8_t, _DstType>)
        {
            return std::stoi(v);
        }

        if constexpr(std::is_same_v<double, _DstType>) {
            return std::stod(v);
        }

        if constexpr(std::is_same_v<float, _DstType>) {
            return std::stof(v);
        }
    }
}

#endif // AsioServer_Utility_CastUtil