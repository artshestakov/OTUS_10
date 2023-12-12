#include "utils.h"
//-----------------------------------------------------------------------------
std::vector<std::string> utils::split_string(const std::string& s, char sep)
{
    std::vector<std::string> v;

    if (!s.empty())
    {
        size_t pos = 0, last_pos = 0;
        while ((pos = s.find(sep, last_pos)) != std::string::npos)
        {
            if (pos != 0)
            {
                if ((pos - last_pos) > 0)
                {
                    v.emplace_back(s.substr(last_pos, pos - last_pos));
                }
                last_pos = ++pos;
            }
            else
            {
                ++last_pos;
            }
        }

        if (pos == std::string::npos)
        {
            size_t Size = s.size();
            if (last_pos < Size)
            {
                v.emplace_back(s.substr(last_pos, Size - last_pos));
            }
        }
    }

    return v;
}
//-----------------------------------------------------------------------------
