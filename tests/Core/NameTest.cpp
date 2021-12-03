#include "catch.hpp"
#include "OpenGUI/Core/OName.h"

using namespace ostr::literal;
TEST_CASE("Name", "NameTest")
{
    using namespace OGUI;
    Name n_empty;
    REQUIRE(n_empty.ToStringView() == u"None"_o);
    REQUIRE(n_empty.ToStringView() == Name(SpecialName::None).ToStringView());
    REQUIRE(n_empty.Compare_Id(Name(SpecialName::None)) == 0);
    REQUIRE(n_empty.Compare_Id(Name("")) == 0);
    REQUIRE(n_empty.Compare_Id("") == 0);
    REQUIRE(n_empty.IsNone());

    Name n_test(SpecialName::Test);
    REQUIRE(n_test != n_empty);
    REQUIRE(!n_test.IsNone());

    Name n_custom("abc");
    REQUIRE(n_custom.ToStringView() == u"abc"_o);
    REQUIRE(n_custom.Compare_Id(Name("abc")) == 0);
    REQUIRE(n_custom.Compare_Id(Name(u"abc")) == 0);
    REQUIRE(n_custom.Compare_Id("abc") == 0);
    REQUIRE(!n_custom.IsNone());

    REQUIRE(n_custom.Compare_Id(n_empty) > 0);
    REQUIRE(n_custom.Compare_Literal(n_empty) > 0); // 'a' is greater than 'N'

}
