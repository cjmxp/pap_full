// Copyright (c) 1997-2000  Max-Planck-Institute Saarbruecken (Germany).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.2-branch/Nef_2/include/CGAL/Nef_2/geninfo.h $
// $Id: geninfo.h 28567 2006-02-16 14:30:13Z lsaboret $
// 
//
// Author(s)     : Michael Seel <seel@mpi-sb.mpg.de>

#ifndef GENINFO_H
#define GENINFO_H

#include <memory>

/*{\Moptions outfile=geninfo.man}*/
/*{\Moptions constref=yes}*/
/*{\Manpage {geninfo} {T} {Information association via GenPtr} {}}*/    

template <typename T>
struct geninfo {
  typedef void* GenPtr;

/*{\Mdefinition |\Mname| encapsulates information association via
generic pointers of type |GenPtr (=void*)|. An object |t| of type |T|
is stored directly in a variable |p| of type |GenPtr| if |sizeof(T)|
is not larger than |sizeof(GenPtr)| (also called word size). Otherwise
|t| is allocated on the heap and referenced via |p|. This class
encapsulates the technicalities, however the user always has to obey
the order of its usage: |create|-|access/const_access|-|clear|. On
misuse memory problems occur.}*/

/*{\Moperations 2 1}*/

#ifdef __BORLANDC__
#pragma option -w-rch -w-eff -w-ccc
#endif

  static void create(GenPtr& p) 
  /*{\Mstatic create a slot for an object of type |T| referenced 
    via |p|.}*/
  { if (sizeof(T) <= sizeof(GenPtr)) new((void*)(&p)) T;
    if (sizeof(T) >  sizeof(GenPtr)) p = (GenPtr) new T;
  }

  static T& access(GenPtr& p)
  /*{\Mstatic access an object of type |T| via |p|.
    \precond |p| was initialized via |create| and was not cleared 
    via |clear|.}*/
  { if (sizeof(T) <= sizeof(GenPtr)) return *(T*)(&p);
    else                             return *(T*)p;
  }

  static const T& const_access(const GenPtr& p) 
  /*{\Mstatic read-only access of an object of type |T| via |p|.
    \precond |p| was initialized via |create| and was not cleared 
    via |clear|.}*/
  { if (sizeof(T) <= sizeof(GenPtr)) return *(const T*)(&p);
    else                             return *(const T*)p; 
  }

  static void clear(GenPtr& p) 
  /*{\Mstatic clear the memory used for the object of type |T| via
     |p|. \precond |p| was initialized via |create|.}*/
  { if (sizeof(T) <= sizeof(GenPtr)) ((T*)(&p))->~T();
    if (sizeof(T) >  sizeof(GenPtr)) delete (T*) p;
    p=0;
  }

};

/*{\Mexample In the first example we store a pair of boolean values
which normally fit into one word. Thus there will no heap allocation
take place.
\begin{Mverb}
  struct A { bool a,b };
  GenPtr a;
  geninfo<A>::create(a);
  A& a_access = geninfo<A>::access(a);
  geninfo<A>::clear(a);
\end{Mverb}
The second example uses the heap scheme as two longs do not fit into
one word.
\begin{Mverb}
  struct B { long a,b };
  GenPtr b;
  geninfo<B>::create(b);
  B& b_access = geninfo<B>::access(b);
  geninfo<B>::clear(b);
\end{Mverb}
Note that usage of the scheme takes away with the actual check for the
type size. Even more important this size might depend on the platform
which is used to compile the code and thus the scheme enables platform
independent programming.}*/

#endif //GENINFO_H


