#include <yzctx.hpp>
#include <chrono>

int main()
{
    std::cout << "build " << __TIME__ << ' ' << __DATE__ << std::endl;
    try
    {
        yz::ctx context{false, (yz::u32)std::chrono::system_clock::now().time_since_epoch().count()};
        yz::run(context);
    }
    catch (const std::exception& err)
    {
        std::cerr << err.what() << std::endl;
    }
}