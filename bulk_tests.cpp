#include "bulk.h"

#define BOOST_TEST_MODULE bulk_tests

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
using boost::test_tools::output_test_stream;

BOOST_AUTO_TEST_SUITE(bulk_suite)

struct cout_redirect {
    cout_redirect( std::streambuf * new_buffer )
        : old( std::cout.rdbuf( new_buffer ) )
    { }

    ~cout_redirect( ) {
        std::cout.rdbuf( old );
    }

private:
    std::streambuf * old;
};

//BOOST_AUTO_TEST_CASE(adding)
//{
////    auto blk = bulk::BulkContext(6);

////    std::vector<std::string> lines;
////    lines.push_back("1");
////    lines.push_back("2");
////    lines.push_back("3");
////    lines.push_back("4");
////    lines.push_back("5");
////    lines.push_back("6");

////    for(auto l : lines)
////    {
////        blk.add_line(l);
////    }

////    BOOST_CHECK(blk.cmds.cmdCounter == 6);
//    BOOST_CHECK(6 == 6);
//}

//BOOST_AUTO_TEST_CASE(adding_2_commands)
//{


//    output_test_stream output;
//    {
//        cout_redirect guard( output.rdbuf( ) );

//        print_ip(char(-1));
//        print_ip(short(0));
//        print_ip(int(2130706433));
//        print_ip(long(8875824491850138409));
//    }
//    BOOST_CHECK(output.is_equal("255\n0.0\n127.0.0.1\n123.45.67.89.101.112.131.41\n"));
//}

BOOST_AUTO_TEST_SUITE_END()
