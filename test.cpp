#include <iostream>
#include <string>
#include <vector>

int main() {
    std::string line;
    std::vector<std::string> headers;
    headers.push_back("Content-Length:1212\r\n");
    headers.push_back("Content-Type:text/html\r\n");
    headers.push_back("Connection:keep-alive\r\n");
    headers.push_back("Cache-Control:no-cache\r\n");

    for (size_t i = 0; i < headers.size(); ++i) {
        std::string str = headers[i];
        std::string::size_type pos = 0;
        pos = str.find("\r\n");
        line = str.substr(0, pos);
        std::cout << "Line: " << line << std::endl;
    }

    return 0;
}