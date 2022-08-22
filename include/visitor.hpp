#ifndef DDV_INCLUDE_VISITOR_HPP
#define DDV_INCLUDE_VISITOR_HPP

template <typename T>
struct SingleVisitor {
    virtual void visit(T*) = 0;
};

template <typename... T>
struct Visitor : public SingleVisitor<T>... {
    using SingleVisitor<T>::visit...;
};

#endif