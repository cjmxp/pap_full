
/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef OVERLAPPING_PAIR_CACHE_H
#define OVERLAPPING_PAIR_CACHE_H


#include "btBroadphaseInterface.h"
#include "btBroadphaseProxy.h"
#include "LinearMath/btPoint3.h"
#include "LinearMath/btAlignedObjectArray.h"
class btDispatcher;

struct	btOverlapCallback
{
	virtual ~btOverlapCallback()
	{}
	//return true for deletion of the pair
	virtual bool	processOverlap(btBroadphasePair& pair) = 0;
};

struct btOverlapFilterCallback
{
	virtual ~btOverlapFilterCallback()
	{}
	// return true when pairs need collision
	virtual bool	needBroadphaseCollision(btBroadphaseProxy* proxy0,btBroadphaseProxy* proxy1) const = 0;
};

typedef btAlignedObjectArray<btBroadphasePair>	btBroadphasePairArray;

///btOverlappingPairCache maintains the objects with overlapping AABB
///Typically managed by the Broadphase, Axis3Sweep or btSimpleBroadphase
class	btOverlappingPairCache
{
	protected:
		//avoid brute-force finding all the time
		btBroadphasePairArray	m_overlappingPairArray;
		
		//during the dispatch, check that user doesn't destroy/create proxy
		bool		m_blockedForChanges;
		
		//if set, use the callback instead of the built in filter in needBroadphaseCollision
		btOverlapFilterCallback* m_overlapFilterCallback;

	public:
			
		btOverlappingPairCache();	
		virtual ~btOverlappingPairCache();

		virtual void	processAllOverlappingPairs(btOverlapCallback*,btDispatcher* dispatcher);

		void	removeOverlappingPair(btBroadphasePair& pair,btDispatcher* dispatcher);

		void	cleanOverlappingPair(btBroadphasePair& pair,btDispatcher* dispatcher);
		
		void	addOverlappingPair(btBroadphaseProxy* proxy0,btBroadphaseProxy* proxy1);

		btBroadphasePair*	findPair(btBroadphaseProxy* proxy0,btBroadphaseProxy* proxy1);
			
		
		void	cleanProxyFromPairs(btBroadphaseProxy* proxy,btDispatcher* dispatcher);

		void	removeOverlappingPairsContainingProxy(btBroadphaseProxy* proxy,btDispatcher* dispatcher);


		inline bool needsBroadphaseCollision(btBroadphaseProxy* proxy0,btBroadphaseProxy* proxy1) const
		{
			if (m_overlapFilterCallback)
				return m_overlapFilterCallback->needBroadphaseCollision(proxy0,proxy1);

			bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0;
			collides = collides && (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);
			
			return collides;
		}
		
		btBroadphasePairArray&	getOverlappingPairArray()
		{
			return m_overlappingPairArray;
		}

		const btBroadphasePairArray&	getOverlappingPairArray() const
		{
			return m_overlappingPairArray;
		}

		btBroadphasePair*	getOverlappingPairArrayPtr()
		{
			return &m_overlappingPairArray[0];
		}

		const btBroadphasePair*	getOverlappingPairArrayPtr() const
		{
			return &m_overlappingPairArray[0];
		}

		int	getNumOverlappingPairs() const
		{
			return m_overlappingPairArray.size();
		}
		
		btOverlapFilterCallback* getOverlapFilterCallback()
		{
			return m_overlapFilterCallback;
		}

		void setOverlapFilterCallback(btOverlapFilterCallback* callback)
		{
			m_overlapFilterCallback = callback;
		}

};
#endif //OVERLAPPING_PAIR_CACHE_H


