
#include<iostream>

bool isNetByteOrder()
{
    unsigned short mode = 0x1234;
    char* pmode = (char*)&mode;
    //低位存在低地址
    if(*pmode == 0x34)
        return false;
    return true;
}

int main()
{
    if(isNetByteOrder)
        std::cout << "this system is big-endian." << std::endl;
    else
        std::cout << "this system is little-endian." << std::endl;
    return 0;
}