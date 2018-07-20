/*
 * refcounted.h
 *
 *  Created on: 28 mar 2017
 *      Author: crm
 */

#pragma once

namespace Engine
	{
	namespace Core
		{
		class RefCounted
			{
			private:
				mutable int _refs;

			public:
				RefCounted(): _refs(0) {}
				RefCounted(const RefCounted& r): _refs(0) {}
				virtual ~RefCounted() {}

				inline void   ref() const {++_refs;}
				inline void unref() const {--_refs;}

				inline int refs() const {return _refs;}
				inline int valid() const {return _refs>0;}
			};
		}
	}


