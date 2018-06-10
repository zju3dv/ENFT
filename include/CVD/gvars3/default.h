/*                       
	This file is part of the GVars3 Library.

	Copyright (C) 2009 The Authors

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 
    51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef GV3_INC_DEFUALT_H
#define GV3_INC_DEFUALT_H
#include <gvars3/config.h>

#include <memory> 
#ifdef GVARS3_HAVE_TOON
#include <TooN/TooN.h>
#endif

namespace GVars3
{
template<class C> struct DefaultValue
{
	static inline const C val()
	{
		return C();
	}
};

template <class C> inline const C defaultValue() {
	return C();
}

template<class C> struct IsAwkward
{
	static const int is=0;
};


#ifdef GVARS3_HAVE_TOON
template<> struct DefaultValue<TooN::Vector<-1> >
{
	static inline const TooN::Vector<-1> val()
	{
		return TooN::makeVector(0);
	}
};

template <> inline const TooN::Vector<-1> defaultValue() {
	return TooN::makeVector(0);
}

template<> struct IsAwkward<TooN::Vector<-1> >
{
	static const int is=1;
};
#endif

template<class C, int PainInTheNeck = IsAwkward<C>::is> struct ValueHolder
{
	C val;
	C& get() 
	{ 
		return val; 
	}
	
	const C& get() const 
	{ 
		return val; 
	}

	ValueHolder(const C& c)
	:val(c)
	{}

	void set(const C& c)
	{
		val = c;
	}

	C* ptr() { return &val; }
};


template<class C> struct ValueHolder<C, 1>
{
	std::auto_ptr<C> val;

	C& get()
	{ 
		return *val; 
	}
	const C& get() const 
	{ 
		return *val; 
	}

	ValueHolder()
	:val(new C(DefaultValue<C>::val()))
	{}

	ValueHolder(const ValueHolder& c)
	:val(new C(c.get()))
	{}

	ValueHolder(const C& c)
	:val(new C(c))
	{}

	void set(const C& c)
	{
		val = std::auto_ptr<C>(new C(c));
	}

	C* ptr()
	{
		return val.get();
	}
};

}

#endif
