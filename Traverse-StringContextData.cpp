#include "classes.hpp"


void TraverseStringContextData()
{
  	typedef seastl::string<char> string;
	seastl::rbtree_node< seastl::pair<char*, seastl::ring_buffer<string>*> > * Node = NULL;
  
	//48 83 E3 FC E8 ?? ?? ?? ?? 48 8B C8
	//https://github.com/Speedi13/Frostbite-BugSentry/blob/master/classes.hpp#L89
	BugSentry* g_BugSentry = BugSentry::GetInstance();
	
	seastl::rbtree* StringContextData = &g_BugSentry->m_stringContextData;
	seastl::rbtree_node_base *Base = &g_BugSentry->m_stringContextData.mAnchor;

	FILE* pFile = fopen("D:\\BugSentryStringContextData.log", "a");

	unsigned int TreeNodes = StringContextData->mnSize;
	void** TreeBuffer = (void**)alloca( TreeNodes * sizeof( void* ) );
	unsigned int BufferedNodes = 0;

	Node = (decltype(Node))Base->mpNodeParent;

	TreeBuffer[BufferedNodes++] = Node;
	
	fprintf(pFile, "TreeNodes: %u\nRoot Node: 0x%I64X\n\n",TreeNodes,Node);

	while (BufferedNodes != NULL)
	{
		Node = (decltype(Node))TreeBuffer[ (BufferedNodes--) - 1 ];

		char* dataKey = Node->mValue.first;
		if ( ValidPointer(dataKey) )
		{
			seastl::ring_buffer<string>* RingBuffer = Node->mValue.secound;
			if ( ValidPointer(RingBuffer) )
			{
				string* dataValueAsEastlString = RingBuffer->mBegin;
				if ( ValidPointer(dataValueAsEastlString) )
				{
					char* dataValue = dataValueAsEastlString->mpBegin;
					if ( ValidPointer(dataValue) )
					{
						
						fprintf(pFile, "[%s]\n%s\n\n",dataKey,dataValue);
						
					}
				}
			}
		}

		//overflow check
		if ( (BufferedNodes+1) >= TreeNodes ) continue;

		if (Node->mpNodeLeft != NULL)
			TreeBuffer[BufferedNodes++] = Node->mpNodeLeft;

		//overflow check
		if ( (BufferedNodes+1) >= TreeNodes ) continue;

		if (Node->mpNodeRight != NULL)
			TreeBuffer[BufferedNodes++] = Node->mpNodeRight;
	}
	fclose(pFile);
}
