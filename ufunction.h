// This file is part of the ustl library, an STL implementation.
// Copyright (C) 2003 by Mike Sharov <msharov@talentg.com>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the 
// Free Software Foundation, Inc., 59 Temple Place - Suite 330, 
// Boston, MA  02111-1307  USA.
//
/// \file ufunction.h
///
/// \brief Implements STL standard functors.
///
/// See STL specification and bvts for usage of these. The only
/// extension is the mem_var functors for member variable access:
/// \code
///	f = find_if (ctr, mem_var_equal_to(&MyClass::m_Var, matchVar));
///	f = find_if (ctr, mem_var_less(&MyClass::m_Var, matchVar));
/// \endcode
/// There are a couple of others but the syntax is much harder to grasp.
/// See bvt10.cc for more examples.
///

#ifndef UFUNCTION_H_221ABA8551801799263C927234C085F3
#define UFUNCTION_H_221ABA8551801799263C927234C085F3

namespace ustl {

//----------------------------------------------------------------------
// Standard functors
//----------------------------------------------------------------------

#ifndef DOXYGEN_SHOULD_SKIP_THIS

template <typename Arg, typename Result>
struct unary_function {
    typedef Arg		argument_type;
    typedef Result	result_type;
};

template <typename Arg1, typename Arg2, typename Result>
struct binary_function {
    typedef Arg1	first_argument_type;
    typedef Arg2	second_argument_type;
    typedef Result	result_type;
};

template <class T> struct plus		: public binary_function<T,T,T> { inline T operator()(const T& a, const T& b) const { return (a + b); } };
template <class T> struct minus		: public binary_function<T,T,T> { inline T operator()(const T& a, const T& b) const { return (a - b); } };
template <class T> struct divides	: public binary_function<T,T,T> { inline T operator()(const T& a, const T& b) const { return (a / b); } };
template <class T> struct modulus	: public binary_function<T,T,T> { inline T operator()(const T& a, const T& b) const { return (a % b); } };
template <class T> struct multiplies	: public binary_function<T,T,T> { inline T operator()(const T& a, const T& b) const { return (a * b); } };
template <class T> struct logical_and	: public binary_function<T,T,T> { inline T operator()(const T& a, const T& b) const { return (a && b); } };
template <class T> struct logical_or	: public binary_function<T,T,T> { inline T operator()(const T& a, const T& b) const { return (a || b); } };
template <class T> struct negate	: public unary_function<T,T>    { inline T operator()(const T& a) const { return (-a); } };
template <class T> struct logical_not	: public unary_function<T,T>    { inline T operator()(const T& a) const { return (!a); } };
template <class T> struct equal_to	: public binary_function<T,T,bool> { inline bool operator()(const T& a, const T& b) const { return (a == b); } };
template <class T> struct not_equal_to	: public binary_function<T,T,bool> { inline bool operator()(const T& a, const T& b) const { return (!(a == b)); } };
template <class T> struct greater	: public binary_function<T,T,bool> { inline bool operator()(const T& a, const T& b) const { return (b < a); } };
template <class T> struct less		: public binary_function<T,T,bool> { inline bool operator()(const T& a, const T& b) const { return (a < b); } };
template <class T> struct greater_equal	: public binary_function<T,T,bool> { inline bool operator()(const T& a, const T& b) const { return (b < a || a == b); } };
template <class T> struct less_equal	: public binary_function<T,T,bool> { inline bool operator()(const T& a, const T& b) const { return (a < b || a == b); } };
template <class T> struct compare	: public binary_function<T,T,int> { inline int operator()(const T& a, const T& b) const { return (a < b ? -1 : (a == b ? 0 : 1)); } };

//----------------------------------------------------------------------
// Generic function to functor converters.
//----------------------------------------------------------------------

template <typename Arg, typename Result, typename UnaryFunction>
class functor1 : public unary_function<Arg,Result> {
public:
    typedef Arg		argument_type;
    typedef Result	result_type;
public:
    explicit		functor1 (UnaryFunction pfn) : m_pfn (pfn) {}
    			functor1 (const functor1& v) : m_pfn (v.m_pfn) {}
    const functor1&	operator= (const functor1& v) { m_pfn = v.m_pfn; return (*this); }
    inline result_type	operator() (argument_type v) const { return (m_pfn(v)); }
private:
    UnaryFunction	m_pfn;
};

template <typename Arg1, typename Arg2, typename Result, typename BinaryFunction>
class functor2 : public binary_function<Arg1,Arg2,Result> {
public:
    typedef Arg1	first_argument_type;
    typedef Arg2	second_argument_type;
    typedef Result	result_type;
public:
    explicit		functor2 (BinaryFunction pfn) : m_pfn (pfn) {}
    			functor2 (const functor2& v) : m_pfn (v.m_pfn) {}
    const functor2&	operator= (const functor2& v) { m_pfn = v.m_pfn; return (*this); }
    inline result_type	operator() (first_argument_type v1, second_argument_type v2) const { return (m_pfn(v1, v2)); }
private:
    BinaryFunction	m_pfn;
};

template <typename Arg, typename UnaryFunction>
class vfunctor1 : public unary_function<Arg,void> {
public:
    typedef Arg		argument_type;
    typedef void	result_type;
public:
    explicit		vfunctor1 (UnaryFunction pfn) : m_pfn (pfn) {}
    			vfunctor1 (const vfunctor1& v) : m_pfn (v.m_pfn) {}
    const vfunctor1&	operator= (const vfunctor1& v) { m_pfn = v.m_pfn; return (*this); }
    inline void		operator() (argument_type v) const { m_pfn(v); }
private:
    UnaryFunction	m_pfn;
};

template <typename Arg1, typename Arg2, typename BinaryFunction>
class vfunctor2 : public binary_function<Arg1,Arg2,void> {
public:
    typedef Arg1	first_argument_type;
    typedef Arg2	second_argument_type;
    typedef void	result_type;
public:
    explicit		vfunctor2 (BinaryFunction pfn) : m_pfn (pfn) {}
    			vfunctor2 (const vfunctor2& v) : m_pfn (v.m_pfn) {}
    const vfunctor2&	operator= (const vfunctor2& v) { m_pfn = v.m_pfn; return (*this); }
    inline void		operator() (first_argument_type v1, second_argument_type v2) const { m_pfn(v1, v2); }
private:
    BinaryFunction	m_pfn;
};

#endif // DOXYGEN_SHOULD_SKIP_THIS

/// functor(pfn) wraps function pointer pfn into a functor class that calls it.
template <typename Arg, typename Result>
inline functor1<Arg,Result,Result (*)(Arg)> functor (Result (*pfn)(Arg))
{
    return (functor1<Arg,Result,Result (*)(Arg)> (pfn));
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

template <typename Arg>
inline vfunctor1<Arg,void (*)(Arg)> functor (void (*pfn)(Arg))
{
    return (vfunctor1<Arg,void (*)(Arg)> (pfn));
}

template <typename Arg1, typename Arg2, typename Result>
inline functor2<Arg1,Arg2,Result,Result (*)(Arg1,Arg2)> functor (Result (*pfn)(Arg1,Arg2))
{
    return (functor2<Arg1,Arg2,Result,Result (*)(Arg1,Arg2)> (pfn));
}

template <typename Arg1, typename Arg2>
inline vfunctor2<Arg1,Arg2,void (*)(Arg1,Arg2)> functor (void (*pfn)(Arg1,Arg2))
{
    return (vfunctor2<Arg1,Arg2,void (*)(Arg1,Arg2)> (pfn));
}

//----------------------------------------------------------------------
// Negators.
//----------------------------------------------------------------------

template <class UnaryFunction>
class unary_negate : public unary_function<typename UnaryFunction::argument_type,
					   typename UnaryFunction::result_type> {
public:
    typedef typename UnaryFunction::argument_type	argument_type;
    typedef typename UnaryFunction::result_type		result_type;
public:
    explicit	unary_negate (UnaryFunction pfn) : m_pfn (pfn) {}
    		unary_negate (const unary_negate& v) : m_pfn (v.m_pfn) {}
    inline const unary_negate& operator= (const unary_negate& v) { m_pfn = v.m_pfn; return (*this); }
    inline result_type operator() (const argument_type& v) const { return (!m_pfn(v)); }
private:
    UnaryFunction	m_pfn;
};

#endif // DOXYGEN_SHOULD_SKIP_THIS

/// Returns the functor that negates the result of *pfn().
template <class UnaryFunction>
inline unary_negate<UnaryFunction> unary_negator (UnaryFunction pfn)
{
    return (unary_negate<UnaryFunction>(pfn));
}

//----------------------------------------------------------------------
// Argument binders
//----------------------------------------------------------------------

#ifndef DOXYGEN_SHOULD_SKIP_THIS

template <class BinaryFunction> 
class binder1st : public unary_function<typename BinaryFunction::second_argument_type,
					typename BinaryFunction::result_type> {
public:
    typedef typename BinaryFunction::first_argument_type	arg1_t;
    typedef typename BinaryFunction::second_argument_type	arg2_t;
    typedef typename BinaryFunction::result_type		result_t;
public:
    binder1st (const BinaryFunction& pfn, const arg1_t& v) : m_pfn (pfn), m_Value(v) {}
    inline result_t operator()(const arg2_t& v2) const { return (m_pfn (m_Value, v2)); }
protected:
    BinaryFunction	m_pfn;
    arg1_t		m_Value;
};

template <class BinaryFunction> 
class binder2nd : public unary_function<typename BinaryFunction::first_argument_type,
					typename BinaryFunction::result_type> {
public:
    typedef typename BinaryFunction::first_argument_type	arg1_t;
    typedef typename BinaryFunction::second_argument_type	arg2_t;
    typedef typename BinaryFunction::result_type		result_t;
public:
    binder2nd (const BinaryFunction& pfn, const arg2_t& v) : m_pfn (pfn), m_Value(v) {}
    inline result_t operator()(const arg1_t& v1) const { return (m_pfn (v1, m_Value)); }
protected:
    BinaryFunction	m_pfn;
    arg2_t		m_Value;
};

#endif // DOXYGEN_SHOULD_SKIP_THIS

/// Converts \p pfn into a unary function by binding the first argument to \p v.
template <typename BinaryFunction>
inline binder1st<BinaryFunction>
bind1st (const BinaryFunction& pfn, const typename BinaryFunction::first_argument_type& v) 
{
    return (binder1st<BinaryFunction> (pfn, v));
}

/// Converts \p pfn into a unary function by binding the second argument to \p v.
template <typename BinaryFunction>
inline binder2nd<BinaryFunction>
bind2nd (const BinaryFunction& pfn, const typename BinaryFunction::second_argument_type& v) 
{
    return (binder2nd<BinaryFunction> (pfn, v));
}

//----------------------------------------------------------------------
// Member function adaptors
//----------------------------------------------------------------------

#ifndef DOXYGEN_SHOULD_SKIP_THIS

template <class T>
class mem_fun_t : public unary_function<T*,void> {
public:
    explicit	mem_fun_t (void (T::*pf)()) : m_pf (pf) {}
    		mem_fun_t (const mem_fun_t& v) : m_pf (v.m_pf) {}
    inline const mem_fun_t& operator= (const mem_fun_t& v) { m_pf = v.m_pf; return (*this); }
    inline void	operator() (T* p) const { (p->*m_pf)(); }
private:
    void	(T::*m_pf)();
};

template <class T>
class const_mem_fun_t : public unary_function<const T*,void> {
public:
    explicit	const_mem_fun_t (void (T::*pf)() const) : m_pf (pf) {}
    		const_mem_fun_t (const const_mem_fun_t& v) : m_pf (v.m_pf) {}
    inline const const_mem_fun_t& operator= (const const_mem_fun_t& v) { m_pf = v.m_pf; return (*this); }
    inline void	operator() (const T* p) const { (p->*m_pf)(); }
private:
    void	(T::*m_pf)() const;
};

template <typename Ret, class T>
class mem_rfun_t : public unary_function<T*,Ret> {
public:
    explicit	mem_rfun_t (void (T::*pf)()) : m_pf (pf) {}
    		mem_rfun_t (const mem_rfun_t& v) : m_pf (v.m_pf) {}
    inline const mem_rfun_t& operator= (const mem_rfun_t& v) { m_pf = v.m_pf; return (*this); }
    inline Ret	operator() (T* p) const { return ((p->*m_pf)()); }
private:
    Ret		(T::*m_pf)();
};

template <typename Ret, class T>
class const_mem_rfun_t : public unary_function<const T*,Ret> {
public:
    explicit	const_mem_rfun_t (void (T::*pf)() const) : m_pf (pf) {}
    		const_mem_rfun_t (const const_mem_rfun_t& v) : m_pf (v.m_pf) {}
    inline const const_mem_rfun_t& operator= (const const_mem_rfun_t& v) { m_pf = v.m_pf; return (*this); }
    inline Ret	operator() (const T* p) const { return ((p->*m_pf)()); }
private:
    Ret		(T::*m_pf)() const;
};

template <class T, typename V>
class ext_mem_fun_t : public unary_function<V,void> {
public:
    explicit	ext_mem_fun_t (T* t, void (T::*pf)(V& v)) : m_t (t), m_pf (pf) {}
    		ext_mem_fun_t (const ext_mem_fun_t& v) : m_t (v.m_t), m_pf (v.m_pf) {}
    inline const ext_mem_fun_t& operator= (const ext_mem_fun_t& v) { m_t = v.m_t; m_pf = v.m_pf; return (*this); }
    inline void	operator() (V& v) { (m_t->*m_pf)(v); }
private:
    T*		m_t;
    void	(T::*m_pf)(V& v);
};

template <class T, typename V>
class const_ext_mem_fun_t : public unary_function<const V,void> {
public:
    explicit	const_ext_mem_fun_t (const T* t, void (T::*pf)(const V& v) const) : m_t (t), m_pf (pf) {}
    		const_ext_mem_fun_t (const const_ext_mem_fun_t& v) : m_pf (v.m_pf) {}
    inline const const_ext_mem_fun_t& operator= (const const_ext_mem_fun_t& v) { m_pf = v.m_pf; return (*this); }
    inline void	operator() (const V& v) const { (m_t->*m_pf)(v); }
private:
    const T*	m_t;
    void	(T::*m_pf)(const V& v) const;
};

#endif // DOXYGEN_SHOULD_SKIP_THIS

/// A unary functor that calls member function \p pf of given object.
template <class T>
inline mem_fun_t<T> mem_fun (void (T::*pf)())
{
    return (mem_fun_t<T> (pf));
}

/// A unary functor that calls member function \p pf of given object.
template <class T>
inline const_mem_fun_t<T> mem_fun (void (T::*pf)() const)
{
    return (const_mem_fun_t<T> (pf));
}

/// A unary functor that calls member function \p pf of given object.
template <class Ret, typename T>
inline mem_rfun_t<Ret,T> mem_fun (Ret (T::*pf)())
{
    return (mem_rfun_t<Ret,T> (pf));
}

/// A unary functor that calls member function \p pf of given object.
template <class Ret, typename T>
inline const_mem_rfun_t<Ret,T> mem_fun (Ret (T::*pf)() const)
{
    return (const_mem_rfun_t<Ret,T> (pf));
}

/// A unary functor that calls member function \p pf of bound object \p t.
template <class T, typename V>
inline ext_mem_fun_t<T,V> mem_fun (T* t, void (T::*pf)(V& v))
{
    return (ext_mem_fun_t<T,V> (t, pf));
}

/// A unary functor that calls member function \p pf of bound object \p t.
template <class T, typename V>
inline const_ext_mem_fun_t<T,V> mem_fun (const T* t, void (T::*pf)(const V& v) const)
{
    return (const_ext_mem_fun_t<T,V> (t, pf));
}

//----------------------------------------------------------------------
// Member variable adaptors (uSTL extension)
//----------------------------------------------------------------------

#ifndef DOXYGEN_SHOULD_SKIP_THIS

template <typename UnaryFunction, class T, typename VT>
class mem_var1_t : public unary_function<T&,typename UnaryFunction::result_type> {
public:
    typedef T&					argument_type;
    typedef typename UnaryFunction::result_type	result_type;
    typedef VT T::*				mem_var_ptr_t;
public:
    mem_var1_t (mem_var_ptr_t pv, UnaryFunction pfn) : m_pv(pv), m_pfn(pfn) {}
    mem_var1_t (const mem_var1_t& v) : m_pv(v.m_pv), m_pfn(v.m_pfn) {}
    inline const mem_var1_t& operator= (const mem_var1_t& v) { m_pv = v.m_pv; m_pfn = v.m_pfn; return (*this); }
    inline result_type operator() (argument_type p) const { return (m_pfn(p.*m_pv)); }
private:
    mem_var_ptr_t	m_pv;
    UnaryFunction	m_pfn;
};

template <typename UnaryFunction, class T, typename VT>
class const_mem_var1_t : public unary_function<const T&,typename UnaryFunction::result_type> {
public:
    typedef const T&				argument_type;
    typedef typename UnaryFunction::result_type	result_type;
    typedef const VT T::*			mem_var_ptr_t;
public:
    const_mem_var1_t (mem_var_ptr_t pv, UnaryFunction pfn) : m_pv(pv), m_pfn(pfn) {}
    const_mem_var1_t (const const_mem_var1_t& v) : m_pv(v.m_pv), m_pfn(v.m_pfn) {}
    inline const const_mem_var1_t& operator= (const const_mem_var1_t& v) { m_pv = v.m_pv; m_pfn = v.m_pfn; return (*this); }
    inline result_type operator() (argument_type p) const { return (m_pfn(p.*m_pv)); }
private:
    mem_var_ptr_t	m_pv;
    UnaryFunction	m_pfn;
};

template <typename BinaryFunction, class T, typename VT>
class mem_var2_t : public unary_function<T&,typename BinaryFunction::result_type> {
public:
    typedef T&					argument_type;
    typedef typename BinaryFunction::result_type	result_type;
    typedef VT T::*				mem_var_ptr_t;
public:
    mem_var2_t (mem_var_ptr_t pv, BinaryFunction pfn) : m_pv(pv), m_pfn(pfn) {}
    mem_var2_t (const mem_var2_t& v) : m_pv(v.m_pv), m_pfn(v.m_pfn) {}
    inline const mem_var2_t& operator= (const mem_var2_t& v) { m_pv = v.m_pv; m_pfn = v.m_pfn; return (*this); }
    inline result_type operator() (argument_type p1, argument_type p2) const { return (m_pfn(p1.*m_pv, p2.*m_pv)); }
private:
    mem_var_ptr_t	m_pv;
    BinaryFunction	m_pfn;
};

template <typename BinaryFunction, class T, typename VT>
class const_mem_var2_t : public binary_function<const T&,const T&,typename BinaryFunction::result_type> {
public:
    typedef const T&					argument_type;
    typedef typename BinaryFunction::result_type	result_type;
    typedef const VT T::*				mem_var_ptr_t;
public:
    const_mem_var2_t (mem_var_ptr_t pv, BinaryFunction pfn) : m_pv(pv), m_pfn(pfn) {}
    const_mem_var2_t (const const_mem_var2_t& v) : m_pv(v.m_pv), m_pfn(v.m_pfn) {}
    inline const const_mem_var2_t& operator= (const const_mem_var2_t& v) { m_pv = v.m_pv; m_pfn = v.m_pfn; return (*this); }
    inline result_type operator() (argument_type p1, argument_type p2) const { return (m_pfn(p1.*m_pv, p2.*m_pv)); }
private:
    mem_var_ptr_t	m_pv;
    BinaryFunction	m_pfn;
};

#endif // DOXYGEN_SHOULD_SKIP_THIS

/// Returned functor passes member variable \p mvp reference of given object to \p pfn.
template <typename UnaryFunction, class T, typename VT>
inline mem_var1_t<UnaryFunction, T, VT>
mem_var1 (VT T::*mvp, UnaryFunction pfn)
{
    return (mem_var1_t<UnaryFunction,T,VT> (mvp, pfn));
}

/// Returned functor passes member variable \p mvp reference of given object to \p pfn.
template <typename UnaryFunction, class T, typename VT>
inline const_mem_var1_t<UnaryFunction, T, VT>
const_mem_var1 (const VT T::*mvp, UnaryFunction pfn)
{
    return (const_mem_var1_t<UnaryFunction,T,VT> (mvp, pfn));
}

/// Returned functor passes member variable \p mvp reference of given object to \p pfn.
template <typename BinaryFunction, class T, typename VT>
inline mem_var2_t<BinaryFunction, T, VT>
mem_var2 (VT T::*mvp, BinaryFunction pfn)
{
    return (mem_var2_t<BinaryFunction,T,VT> (mvp, pfn));
}

/// Returned functor passes member variable \p mvp reference of given object to \p pfn.
template <typename BinaryFunction, class T, typename VT>
inline const_mem_var2_t<BinaryFunction, T, VT>
const_mem_var2 (const VT T::*mvp, BinaryFunction pfn)
{
    return (const_mem_var2_t<BinaryFunction,T,VT> (mvp, pfn));
}

/// Returned functor passes member variable \p mvp reference of given object to equal\<VT\>.
template <class T, typename VT>
inline const_mem_var1_t<binder2nd<equal_to<VT> >, T, VT>
mem_var_equal_to (const VT T::*mvp, const VT& v)
{
    return (const_mem_var1_t<binder2nd<equal_to<VT> >,T,VT> (mvp, bind2nd(equal_to<VT>(), v)));
}

/// Returned functor passes member variable \p mvp reference of given object to less\<VT\>.
template <class T, typename VT>
inline const_mem_var1_t<binder2nd<less<VT> >, T, VT>
mem_var_less (const VT T::*mvp, const VT& v)
{
    return (const_mem_var1_t<binder2nd<less<VT> >,T,VT> (mvp, bind2nd(less<VT>(), v)));
}

/// Returned functor passes member variable \p mvp reference of given object to equal\<VT\>.
template <class T, typename VT>
inline const_mem_var2_t<equal_to<VT>, T, VT>
mem_var_equal_to (const VT T::*mvp)
{
    return (const_mem_var2_t<equal_to<VT>,T,VT> (mvp, equal_to<VT>()));
}

/// Returned functor passes member variable \p mvp reference of given object to less\<VT\>.
template <class T, typename VT>
inline const_mem_var2_t<less<VT>, T, VT>
mem_var_less (const VT T::*mvp)
{
    return (const_mem_var2_t<less<VT>,T,VT> (mvp, less<VT>()));
}

} // namespace ustl

#endif

