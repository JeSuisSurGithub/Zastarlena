#include <yzctx.hpp>

int main()
{
    try
    {
        yz::ctx context{false, false};
        yz::run(context);
    }
    catch (const std::exception& err)
    {
        std::cerr << err.what() << std::endl;
    }
}