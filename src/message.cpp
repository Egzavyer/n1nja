#include "../include/message.h"

Message::Message()
{
    method;
    body;
}

std::string Message::getMethod()
{
    return method;
}

void Message::setMethod(const std::string &otherMethod)
{
    method = otherMethod;
}

std::string Message::getBody()
{
    return body;
}

void Message::setBody(const std::string &otherBody)
{
    body = otherBody;
}