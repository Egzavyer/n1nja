#include "../include/messageParser.h"

MessageParser::MessageParser(std::string otherRawMessage)
{
    currentState = ParseState::BEGIN;
    rawMessage = otherRawMessage;
}

std::string MessageParser::parseMethod()
{
    size_t methodEnd = rawMessage.find("/r/n");
    std::string method = rawMessage.substr(0, methodEnd);
    if (methodTypes.find(method) != methodTypes.end())
    {
        rawMessage = rawMessage.substr(methodEnd + 2);
        return method;
    }
    else
    {
        throw std::runtime_error("Invalid Method: " + std::string(strerror(errno)));
    }
}

std::string MessageParser::parseBody()
{
    size_t bodyEnd = rawMessage.find("/r/n/r/n");
    std::string body = rawMessage.substr(0, bodyEnd);
    rawMessage = rawMessage.substr(bodyEnd + 4);
    if (rawMessage.length() == 0)
    {
        return body;
    }
    else
    {
        throw std::runtime_error("Invalid Body Format: " + std::string(strerror(errno)));
    }
}

Message MessageParser::parseMessage()
{
    while (currentState != ParseState::DONE)
    {
        switch (currentState)
        {
        case ParseState::BEGIN:
        {
            std::cout << "Parsing Begins" << std::endl;
            currentState = ParseState::METHOD;
            break;
        }

        case ParseState::METHOD:
        {
            std::cout << "Parsing Method" << std::endl;
            currentMessage.setMethod(parseMethod());
            currentState = ParseState::BODY;
            break;
        }

        case ParseState::BODY:
        {
            std::cout << "Parsing Body" << std::endl;
            currentMessage.setBody(parseBody());
            break;
        }

        case ParseState::DONE:
        {
            std::cout << "Parsing Done" << std::endl;
            break;
        }
        }
    }
    std::cout << "Parsing Done" << std::endl;
    return currentMessage;
}