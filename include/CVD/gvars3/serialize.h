/*                       
	This file is part of the GVars3 Library.

	Copyright (C) 2005 The Authors

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

#ifndef GV3_INC_SERIALIZE_H
#define GV3_INC_SERIALIZE_H
#include <gvars3/config.h>
#include <gvars3/default.h>
#include <string>
#include <vector>
#include <sstream>
#include <cstdio>
#include <iomanip>

namespace GVars3
{
	namespace serialize
	{
		/// Checks a stream and returns a statis code
		/// @param input stream to check.
		int check_stream(std::istream& i);

		
		/// Function which sets a stream into "precise" mode. This will losslessly
		/// save any numbers up to and including double precision.
		inline void generic_setup(std::ostream& o, bool b)
		{
			if(b)
				o << std::setprecision(20) << std::scientific;
		}
			
		//Define a serializer for everything that works with iostreams
		//override to add new types with unusual serializers
		template<class T> std::string to_string(const T& val, bool precise)
		{
		         
			std::ostringstream o;
			generic_setup(o,precise);
			o << val;
			return o.str();
		}

		std::string to_string(const std::string& val, bool);

		template<class T> struct FromStream
		{
			static T from(std::istream& i)
			{	
				T result = DefaultValue<T>::val();
				i >> result;
				return result;
			}
		};
		
		//Special reading of strings
		template<> struct FromStream<std::string>
		{
			static std::string from(std::istream& in);
		};

		template<class T> struct FromStream<std::vector<T> >
		{
			static std::vector<T> from(std::istream& in)
			{
				std::vector<T> v;
				using std::ws;
				using std::ios;
				v.clear();
				in >> ws;
				int c;


				if((c = in.get()) == EOF)
					return v;

				bool bracket=1;

				if(c != '[')
				{
					bracket = 0;
					in.unget();
				}
					
				for(;;)
				{
					in >> ws;

					if(in.eof())
						return v;
					
					c = in.get();
					
					if(c == EOF || (bracket && c == ']')) 
						return v;

					in.unget();

					T val =  FromStream<T>::from(in);

					if(!in.fail() && !in.bad())
						v.push_back(val);
					else
						return v;
				}
			}
		};

		template<typename T> std::string to_string( const std::vector<T> & v, bool precise)
		{
			std::ostringstream o;
			generic_setup(o, precise);
			o << "[ ";
			for(unsigned i = 0; i < v.size(); ++i)
				o << to_string(v[i]) << " ";
			o << "]";
			return o.str();
		}

		template<class T> struct FromStream<std::vector<std::vector<T> > >
		{
			static std::vector<std::vector<T> > from(std::istream& in)
			{
				std::vector<std::vector<T> > v;
				using std::ws;
				using std::ios;
				v.clear();
				in >> ws;
				int c;

				if((c = in.get()) == EOF)
					return v;

				if(c != '[')
				{
					in.setstate(ios::failbit);
					return v;
				}

				std::vector<T> current;

				for(;;)
				{
					in >> ws;
					
					if((c = in.get()) == EOF || c == ']') 
					{
						if(!current.empty())
							v.push_back(current);
						return v;
					}
					else if(c == ';')
					{
						v.push_back(current);
						current.clear();
					}
					else
						in.unget();

					T val = FromStream<T>::from(in);

					if(!in.fail() && !in.bad())
						current.push_back(val);
					else
						return v;
				}
			}
		};



		#ifdef GVARS3_HAVE_TOON
			template<int N> std::string to_string(const TooN::Vector<N>& m, bool precise)
			{
				std::ostringstream o;
				generic_setup(o, precise);
				o << "[ ";
				for(int i=0; i<m.size(); i++)
				  o << m[i] << " ";
				o << "]";
				return o.str();
			}

			template<int N, int M> std::string to_string(const TooN::Matrix<N, M>& m, bool precise)
			{
				std::ostringstream o;
				generic_setup(o, precise);
				o << "[ ";
				for(int i=0; i<m.num_rows(); i++)
				{
					if(i != 0)
						o << "; ";

					for(int j=0; j<m.num_cols(); j++)
					{
						if(j != 0)
							o << " ";
						o << m[i][j];
					}
				}
				o << "]";
				return o.str();
			}
			

			template<int N> struct FromStream<TooN::Vector<N> >
			{
				static TooN::Vector<N> from(std::istream& i)
				{
					std::vector<double> v = FromStream<std::vector<double> >::from(i);

					if(i.fail() || i.bad() || (N != -1 && (int)v.size() != N) || v.size() == 0)
					{
						i.setstate(std::ios::failbit);
						i.setstate(std::ios::badbit);
						return DefaultValue<TooN::Vector<N> >::val();
					}
					else
					{
						return TooN::wrapVector(&v[0], v.size());
					}
				}
			};

			template<int R, int C, class Precision> struct FromStream<TooN::Matrix<R, C, Precision> >
			{
				static TooN::Matrix<R, C, Precision> from(std::istream& i)
				{
					using std::vector;
					vector<vector<double> > v = FromStream<vector<vector<double> > >::from(i);

					if(i.fail() || i.bad())
						 goto fail;

					for(unsigned int r=1; r < v.size(); r++)
						if(v[r].size() != v[0].size())
							goto fail;

					if(R != -1 && v.size() != R)
						goto fail;

					if(C != -1 && v[0].size() != C)
						goto fail;

					{
						TooN::Matrix<R, C, Precision> retval(v.size(), v[0].size());

						for(int r=0; r < retval.num_rows(); r++)
							for(int c=0; c < retval.num_cols(); c++)
								retval[r][c] = v[r][c];

						return retval;
					}

					fail:
						i.setstate(std::ios::failbit);
						i.setstate(std::ios::badbit);
						return DefaultValue<TooN::Matrix<R, C, Precision> >::val();
				}
			};

		#endif

		template<class T> T from_stream(std::istream& i)
		{
			return FromStream<T>::from(i);
		}

		template<class T> int from_string(const std::string& s, T& t)
		{
			std::istringstream is(s);
			t = from_stream<T>(is);
			return check_stream(is);
		}

	}
}


#endif
