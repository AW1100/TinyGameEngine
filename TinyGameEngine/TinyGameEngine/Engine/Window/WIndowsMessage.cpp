#include "WindowsMessage.h"

#define REGISTER_MESSAGE(msg) {msg,#msg}

WindowsMessage::WindowsMessage()
{
    messageMap = {
        REGISTER_MESSAGE(WM_CLOSE),
        REGISTER_MESSAGE(WM_DESTROY),
        REGISTER_MESSAGE(WM_QUIT),
        REGISTER_MESSAGE(WM_MOUSEMOVE),
        REGISTER_MESSAGE(WM_LBUTTONDOWN),
        REGISTER_MESSAGE(WM_RBUTTONDOWN),
        REGISTER_MESSAGE(WM_KEYDOWN),
        REGISTER_MESSAGE(WM_KEYUP),
    };
}

WindowsMessage& WindowsMessage::GetInstance()
{
    static WindowsMessage instance;
    return instance;
}

int WindowsMessage::NumOfRegisteredMsg() const
{
    return messageMap.size();
}

std::string WindowsMessage::Find(UINT msg) const
{
    return (messageMap.find(msg) != messageMap.end()) ? messageMap.at(msg) : "Unknown Message";
}
