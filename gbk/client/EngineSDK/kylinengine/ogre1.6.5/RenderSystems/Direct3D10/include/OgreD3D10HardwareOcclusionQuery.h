/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

You may alternatively use this source under the terms of a specific version of
the OGRE Unrestricted License provided you have obtained such a license from
Torus Knot Software Ltd.
-----------------------------------------------------------------------------
*/

#ifndef _D3D10HARWAREOCCLUSIONQUERY_H__
#define _D3D10HARWAREOCCLUSIONQUERY_H__

#include "OgreD3D10Prerequisites.h"
#include "OgreHardwareOcclusionQuery.h"
#include "OgreD3D10Device.h"

namespace Ogre {

	// If you use multiple rendering passes you can test only the first pass and all other passes don't have to be rendered 
	// if the first pass results has too few pixels visible.

	// Be sure to render all occluder first and whats out so the RenderQue don't switch places on 
	// the occluding objects and the tested objects because it thinks it's more effective..

	/**
	* This is a class that is the DirectX9 implementation of 
	* hardware occlusion testing.
	*
	* @author Lee Sandberg, email lee@abcmedia.se
	*
	* Updated on 12/7/2004 by Chris McGuirk
	* Updated on 4/8/2005 by Tuan Kuranes email: tuan.kuranes@free.fr
	*/
	class D3D10HardwareOcclusionQuery : public HardwareOcclusionQuery
	{
		//----------------------------------------------------------------------
		// Public methods
		//--
	public:

		/**
		* Default object constructor
		* 
		*/
		D3D10HardwareOcclusionQuery( D3D10Device & device );

		/**
		* Object destructor
		*/
		~D3D10HardwareOcclusionQuery();

		//------------------------------------------------------------------
		// Occlusion query functions (see base class documentation for this)
		//--

		void beginOcclusionQuery();	
		void endOcclusionQuery();
		bool pullOcclusionQuery( unsigned int* NumOfFragments);
		unsigned int getLastQuerysPixelcount() { return mPixelCount; }
        bool isStillOutstanding(void);

	

		//----------------------------------------------------------------------
		// private members
		//--
	private:	
		ID3D10Query*	mpQuery;
		D3D10Device &   mDevice;
	};


}


#endif
