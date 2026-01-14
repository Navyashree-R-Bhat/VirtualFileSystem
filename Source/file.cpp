#include "file.hpp"

void File::setContent(const std::string &contentString)
{
    content += contentString;
}

std::string File::getContent() const
{
    return content;
}
