#define YZ_USE_SPIRV
#include <yzctx.hpp>

int main()
{
    try
    {
        yz::ctx context{true, false};
        yz::run(context);
    }
    catch (const std::exception& err)
    {
        std::cerr << err.what() << std::endl;
    }
}