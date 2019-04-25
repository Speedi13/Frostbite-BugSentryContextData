#pragma once

//slim eastl
//full library is on Github
//https://github.com/electronicarts/EASTL
////////////////////////////////////////// EASTL BEGIN //////////////////////////////////////////
namespace seastl
{
	struct rbtree_node_base
	{
		struct rbtree_node_base* mpNodeRight; //0x00
		struct rbtree_node_base* mpNodeLeft; //0x08
		struct rbtree_node_base* mpNodeParent; //0x10
		char       mColor; //0x18
	};
	template <typename Value>
	struct rbtree_node : public rbtree_node_base
	{
		Value mValue;
	};

	template <typename value_type>
	struct string
	{
		value_type* mpBegin;
		value_type* mpEnd;
		value_type* mpCapacity;
		void*       mAllocator;
	};
	template <typename T>
	struct vector
	{
		T*		mpBegin;
		T*		mpEnd;
		T*		mpCapacity;
		void*	mAllocator;
	};

	struct rbtree
	{
		void* mCompare; //???? idk
		rbtree_node_base  mAnchor;
		UINT32 mnSize;
		void* mAllocator; //ptr to game addr
	};

	template <typename value1, typename value2>
	struct pair
	{
		value1 first;
		value2 secound;
	};

	template <typename type>
	struct ring_buffer
	{
		vector<void> c; //<< wtf idk don't use that
		type* mBegin;
		type* mEnd;
		unsigned int mSize;
	};
};
/////////////////////////////////////////// EASTL END ///////////////////////////////////////////


/////////////////////////////////////// GAME CLASSES BEGIN ///////////////////////////////////////
class BugSentry
{
public:
	
	static BugSentry* GetInstance()
	{
		return *(BugSentry**)(BugSentry__instance);
	}
#if defined(_CompileMode_SWBF2)
	PAD(0x30);
	seastl::rbtree m_unknown;
#endif
	seastl::rbtree m_intContextData;
	seastl::rbtree m_floatContextData;
	seastl::rbtree m_vec3ContextData;
	seastl::rbtree m_stringContextData;
};
//////////////////////////////////////// GAME CLASSES END ////////////////////////////////////////


////////////////////////////////////// PATTERN SCANNING BEGIN /////////////////////////////////////
#define ResolveRelativePtr(Address) ((ULONG_PTR)(Address) + *(__int32*)(Address) + sizeof(__int32))
static void GetBugSentryInstance()
{
	__int64 fb__BugSentry__singleton = FindPattern( "48 83 E3 FC E8 ?? ?? ?? ?? 48 8B C8" );
	fb__BugSentry__singleton += 5;
	fb__BugSentry__singleton = (DWORD64)ResolveRelativePtr( fb__BugSentry__singleton );
	
	while (*(BYTE*)fb__BugSentry__singleton == 0xE9)
		fb__BugSentry__singleton += ( *( signed __int32*)(fb__BugSentry__singleton+1) + 4i64 );
	
	DebugOffset( fb__BugSentry__singleton );
	
	//48 8B 05
	BugSentry__instance = fb__BugSentry__singleton;
	for (; ; BugSentry__instance++)
	{
		if ( *(WORD*)(BugSentry__instance+0) == 0x8B48 &&
			 *(BYTE*)(BugSentry__instance+2) == 0x05 )
		{
			BugSentry__instance += 3;
			BugSentry__instance = (DWORD64)ResolveRelativePtr( (void*)BugSentry__instance );
			break;
		}
	}
	DebugOffset( BugSentry__instance );
}
////////////////////////////////////// PATTERN SCANNING END ///////////////////////////////////////
