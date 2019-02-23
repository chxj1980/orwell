#include <iostream>
std::string newCameraUtil(std::string ip) {
    std::string username("admin");
    std::string password("19929394");
    std::string port("10554");
    std::string end("/tcp/av0_0");
    return "rtsp://" + username + ":" + password + "@" + ip + ":" + port + end;
}