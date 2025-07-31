#include <iostream>
#include "websocket/websocket_protocol.h"

int main()
{
    int res = 0;
    //启动websocket服务器
    pt_run();
    
    if(res != -1)
    {
        do
        {
            sleep(1);
        }
        while(true);
    }
    return res;
}
