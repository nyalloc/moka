#include <catch.hpp>
#include <graphics/buffer.hpp>
#include <maths/vector2.hpp>
#include <maths/vector3.hpp>

TEST_CASE("Construct a buffer", "[buffer]")
{
	moka::static_buffer buffer;

	buffer.write_value(moka::vector2{ 0, 1 });
	buffer.write_value(moka::vector2{ 0, 2 });
	buffer.write_value(moka::vector2{ 0, 3 });
	buffer.write_value(moka::vector2{ 0, 4 });
	buffer.write_value(moka::vector3{ 0, 1, 0 });
	buffer.write_value(moka::vector3{ 0, 2, 0 });
	buffer.write_value(moka::vector3{ 0, 3, 0 });
	buffer.write_value(moka::vector3{ 0, 4, 0 });

	REQUIRE(buffer.read_value<moka::vector2>() == moka::vector2{ 0, 1 });
	REQUIRE(buffer.read_value<moka::vector2>() == moka::vector2{ 0, 2 });
	REQUIRE(buffer.read_value<moka::vector2>() == moka::vector2{ 0, 3 });
	REQUIRE(buffer.read_value<moka::vector2>() == moka::vector2{ 0, 4 });
	REQUIRE(buffer.read_value<moka::vector3>() == moka::vector3{ 0, 1, 0 });
	REQUIRE(buffer.read_value<moka::vector3>() == moka::vector3{ 0, 2, 0 });
	REQUIRE(buffer.read_value<moka::vector3>() == moka::vector3{ 0, 3, 0 });
	REQUIRE(buffer.read_value<moka::vector3>() == moka::vector3{ 0, 4, 0 });
}