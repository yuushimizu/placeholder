#ifndef PLACEHOLDER_INCLUDED_PLACEHOLDER_HPP
#define PLACEHOLDER_INCLUDED_PLACEHOLDER_HPP

#include <utility>
#include <type_traits>

namespace placeholder {
  namespace detail {
    template <typename F>
    struct Placeable {
      constexpr Placeable(F f) noexcept : f_(std::move(f)) {
      }
      
      template <typename ... Args>
      constexpr decltype(auto) operator()(Args && ... args) const& {
        return f_(std::forward<Args>(args) ...);
      }
      
      template <typename ... Args>
      constexpr decltype(auto) operator()(Args && ... args) & {
        return f_(std::forward<Args>(args) ...);
      }
      
      template <typename ... Args>
      constexpr decltype(auto) operator()(Args && ... args) && {
        return f_(std::forward<Args>(args) ...);
      }
      
    private:
      F f_;
    };

    template <typename F>
    constexpr auto placeable(F &&f) noexcept {
      return Placeable<F>(std::forward<F>(f));
    }

    template <template <typename ...> class Template, typename T>
    struct is_instantiation_of : std::false_type {};
    
    template <template <typename ...> class Template, typename ... Args>
    struct is_instantiation_of<Template, Template<Args ...>> : std::true_type {};
    
    template <template <typename ...> class Template, typename T>
    constexpr bool is_instantiation_of_v = is_instantiation_of<Template, T>::value;
    
    template <typename T>
    constexpr const bool is_placeable_v = is_instantiation_of_v<Placeable, std::decay_t<T>>;

    template <typename T>
    using unary_operator_enable_if_t = std::enable_if_t<is_placeable_v<T>, std::nullptr_t>;

    template <typename F, typename T>
    constexpr auto unary_operator(F &&f, T &&operand) noexcept {
      return placeable([f = std::forward<F>(f), operand = std::forward<T>(operand)](auto && ... args) constexpr -> decltype(auto) {
          return f(operand(std::forward<decltype(args)>(args) ...));
        });
    }
        
#define PLACEHOLDER_DEFINE_UNARY_OPERATOR(op)                           \
    template <typename T, unary_operator_enable_if_t<T> = nullptr>      \
    constexpr auto operator op(T &&operand) noexcept {                  \
      return unary_operator([](auto &&arg) constexpr -> decltype(auto) { \
          return op std::forward<decltype(arg)>(arg);                   \
        }, std::forward<T>(operand));                                   \
    }
    
    PLACEHOLDER_DEFINE_UNARY_OPERATOR(!);
    PLACEHOLDER_DEFINE_UNARY_OPERATOR(&);
    PLACEHOLDER_DEFINE_UNARY_OPERATOR(*);
    PLACEHOLDER_DEFINE_UNARY_OPERATOR(+);
    PLACEHOLDER_DEFINE_UNARY_OPERATOR(-);
    PLACEHOLDER_DEFINE_UNARY_OPERATOR(~);
        
#undef PLACEHOLDER_DEFINE_UNARY_OPERATOR

    template <typename LHS, typename RHS>
    using binary_operator_enable_if_t = std::enable_if_t<is_placeable_v<LHS> != is_placeable_v<RHS>, std::nullptr_t>;

    template <typename F, typename LHS, typename RHS, std::enable_if_t<is_placeable_v<LHS>, std::nullptr_t> = nullptr>
    constexpr auto binary_operator(F &&f, LHS &&lhs, RHS &&rhs) noexcept {
      return placeable([f = std::forward<F>(f), lhs = std::forward<LHS>(lhs), rhs = std::forward<RHS>(rhs)](auto && ... args) constexpr -> decltype(auto) {
          return f(lhs(args ...), rhs);
        });
    }

    template <typename F, typename LHS, typename RHS, std::enable_if_t<is_placeable_v<RHS>, std::nullptr_t> = nullptr>
    constexpr auto binary_operator(F &&f, LHS &&lhs, RHS &&rhs) noexcept {
      return placeable([f = std::forward<F>(f), lhs = std::forward<LHS>(lhs), rhs = std::forward<RHS>(rhs)](auto && ... args) constexpr -> decltype(auto) {
          return f(lhs, rhs(args ...));
        });
    }

    template <typename LHS, typename RHS, binary_operator_enable_if_t<LHS, RHS> = nullptr>
    constexpr auto operator,(LHS &&lhs, RHS &&rhs) noexcept {
      return binary_operator([](auto &&lhs, auto &&rhs) constexpr -> decltype(auto) {
          return std::forward<decltype(lhs)>(lhs), std::forward<decltype(rhs)>(rhs);
        }, std::forward<LHS>(lhs), std::forward<RHS>(rhs));
    }
        
#define PLACEHOLDER_DEFINE_BINARY_OPERATOR(op)                          \
    template <typename LHS, typename RHS, binary_operator_enable_if_t<LHS, RHS> = nullptr> \
    constexpr auto operator op(LHS &&lhs, RHS &&rhs) noexcept {         \
      return binary_operator([](auto &&lhs, auto &&rhs) constexpr -> decltype(auto) { \
          return std::forward<decltype(lhs)>(lhs) op std::forward<decltype(rhs)>(rhs); \
        }, std::forward<LHS>(lhs), std::forward<RHS>(rhs));             \
    }
        
    PLACEHOLDER_DEFINE_BINARY_OPERATOR(!=);
    PLACEHOLDER_DEFINE_BINARY_OPERATOR(%);
    PLACEHOLDER_DEFINE_BINARY_OPERATOR(&);
    PLACEHOLDER_DEFINE_BINARY_OPERATOR(&&);
    PLACEHOLDER_DEFINE_BINARY_OPERATOR(*);
    PLACEHOLDER_DEFINE_BINARY_OPERATOR(+);
    PLACEHOLDER_DEFINE_BINARY_OPERATOR(-);
    PLACEHOLDER_DEFINE_BINARY_OPERATOR(->*);
    PLACEHOLDER_DEFINE_BINARY_OPERATOR(/);
    PLACEHOLDER_DEFINE_BINARY_OPERATOR(<);
    PLACEHOLDER_DEFINE_BINARY_OPERATOR(<<);
    PLACEHOLDER_DEFINE_BINARY_OPERATOR(<=);
    PLACEHOLDER_DEFINE_BINARY_OPERATOR(==);
    PLACEHOLDER_DEFINE_BINARY_OPERATOR(>);
    PLACEHOLDER_DEFINE_BINARY_OPERATOR(>=);
    PLACEHOLDER_DEFINE_BINARY_OPERATOR(>>);
    PLACEHOLDER_DEFINE_BINARY_OPERATOR(^);
    PLACEHOLDER_DEFINE_BINARY_OPERATOR(|);
    PLACEHOLDER_DEFINE_BINARY_OPERATOR(||);
        
#undef PLACEHOLDER_DEFINE_BINARY_OPERATOR
        
  }
    
  constexpr const auto _ = detail::placeable([](auto &&x) constexpr noexcept -> decltype(auto) {
      return std::forward<decltype(x)>(x);
    });
}

#endif
