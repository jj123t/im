//
// Created by 25421 on 2024/4/24.
//

#ifndef SERVER_BUFFER_H
#define SERVER_BUFFER_H

#include <string>
#include <iostream>

class Buffer {
private:
    std::string buf;
public:
    Buffer();
    ~Buffer();
    void append(const char *_str, int _size);
    ssize_t size();
    const char* c_str();
    void clear();
    void getline();
    void setBuf(const char*);
    void setBuf(const std::string&);
};

#endif //SERVER_BUFFER_H
