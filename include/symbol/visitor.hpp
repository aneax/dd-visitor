#ifndef DDV_INCLUDE_VISITOR_HPP
#define DDV_INCLUDE_VISITOR_HPP

namespace v1
{
template <typename T>
struct SingleVisitor {
    virtual void visit(T*) = 0;
};

template <typename... T>
struct Visitor : public SingleVisitor<T>... {
    using SingleVisitor<T>::visit...;
};
}   //namespace v1

namespace v2
{
template <typename... Ts>
class Visitor;

template <typename T>
class Visitor<T>
{
  public:
    virtual void visit(T*) = 0;
};

template <typename T, typename... Ts>
class Visitor<T, Ts...> : public Visitor<Ts...>
{
  public:
    using Visitor<Ts...>::visit;
    virtual void visit(T*) = 0;
};

}   //namespace v2

using namespace v1;

#endif