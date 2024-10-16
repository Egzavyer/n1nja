#ifndef MESSAGE_PARSER_H
#define MESSAGE_PARSER_H

#include <iostream>
#include <unordered_set>
#include <string.h>
#include <errno.h>
#include "message.h"

enum class ParseState
{
    BEGIN,
    METHOD,
    BODY,
    DONE
};

class MessageParser
{
private:
    Message currentMessage;
    ParseState currentState;
    std::string rawMessage;
    std::unordered_set<std::string> methodTypes{"PING", "PONG", "QUERY", "QUERYHIT", "GET"};

    std::string parseMethod();
    std::string parseBody();

public:
    MessageParser(std::string otherRawMessage);
    Message parseMessage();
};

#endif /* MESSAGE_PARSER_H */