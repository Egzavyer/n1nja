#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

class Message
{
private:
    std::string method;
    std::string body;
    std::string senderIP;
    int recvSocket;

public:
    Message();
    std::string getMethod();
    void setMethod(const std::string &otherMethod);
    std::string getBody();
    void setBody(const std::string &otherBody);
};

#endif /* MESSAGE_H */