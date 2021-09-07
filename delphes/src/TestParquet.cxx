#include "DelphesParquet/DelphesParquet.hpp"
#include <string>

int main(int argc, char** argv)
{
    std::string outname = "dummy.parquet";
    DelphesParquet parquet(outname);
    return 0;
}