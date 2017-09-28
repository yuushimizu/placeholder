#include <gtest/gtest.h>
#include "placeholder.hpp"

namespace {
  using namespace placeholder;
  
  TEST(PlaceholderTest, Identity) {
    ASSERT_EQ(13, _(13));
  }
    
  TEST(PlaceholderTest, LogicalNot) {
    auto f = !_;
    ASSERT_EQ(!false, f(false));
    ASSERT_EQ(!true, f(true));
  }
    
  TEST(PlaceholderTest, LogicalNotDouble) {
    auto f = !!_;
    ASSERT_EQ(!!true, f(true));
    ASSERT_EQ(!!false, f(false));
  }
    
  TEST(PlaceholderTest, AddressOf) {
    int x = 10;
    auto f = &_;
    ASSERT_EQ(&x, f(x));
  }
    
  TEST(PlaceholderTest, Dereference) {
    int x = 5;
    int *p = &x;
    auto f = *_;
    ASSERT_EQ(*p, f(p));
    ASSERT_EQ(&*p, &f(p));
    f(p) = 3;
    ASSERT_EQ(3, x);
  }
    
  TEST(PlaceholderTest, UnaryPlus) {
    struct X {
      int operator+() const {
        return 100;
      }
    } x;
    auto f = +_;
    ASSERT_EQ(+x, f(x));
  }
    
  TEST(PlaceholderTest, UnaryMinus) {
    auto f = -_;
    ASSERT_EQ(-42, f(42));
  }
    
  TEST(PlaceholderTest, BitwiseNot) {
    auto f = ~_;
    ASSERT_EQ(~50, f(50));
  }
    
  TEST(PlaceholderTest, NotEqualLeft) {
    auto f = (_ != 10);
    ASSERT_TRUE(f(8));
    ASSERT_FALSE(f(10));
  }
    
  TEST(PlaceholderTest, NotEqualRight) {
    auto f = (55 != _);
    ASSERT_TRUE(f(66));
    ASSERT_FALSE(f(55));
  }
    
  TEST(PlaceholderTest, ModuloLeft) {
    auto f = (_ % 2);
    ASSERT_EQ(1, f(7));
  }
    
  TEST(PlaceholderTest, ModuloRight) {
    auto f = (5 % _);
    ASSERT_EQ(2, f(3));
  }
    
  TEST(PlaceholderTest, BitwiseAndLeft) {
    auto f = (_ & 2);
    ASSERT_EQ(100 & 2, f(100));
  }
    
  TEST(PlaceholderTest, BitwiseAndRight) {
    auto f = (3 & _);
    ASSERT_EQ(3 & 5, f(5));
  }
    
  TEST(PlaceholderTest, PlusLeft) {
    auto f = (_ + 1);
    ASSERT_EQ(15, f(14));
  }
    
  TEST(PlaceholderTest, PlusRight) {
    auto f = (10 + _);
    ASSERT_EQ(23, f(13));
  }
    
  TEST(PlaceholderTest, EqualLeft) {
    auto f = (_ == 10);
    ASSERT_TRUE(f(10));
    ASSERT_FALSE(f(11));
  }
    
  TEST(PlaceholderTest, EqualRight) {
    auto f = (24 == _);
    ASSERT_TRUE(f(24));
    ASSERT_FALSE(f(20));
  }
    
  TEST(PlaceholderTest, NotAndEqual) {
    auto f = !(_ == 3);
    ASSERT_TRUE(f(4));
    ASSERT_FALSE(f(3));
  }
    
  TEST(PlaceholderTest, ModuloAndEqual) {
    auto f = (_ % 2 == 0);
    ASSERT_TRUE(f(8));
    ASSERT_FALSE(f(11));
  }
    
  TEST(PlaceholderTest, NestedEqual) {
    auto f = (_ == 10 == false == true);
    ASSERT_TRUE(f(12));
    ASSERT_FALSE(f(10));
  }
}

