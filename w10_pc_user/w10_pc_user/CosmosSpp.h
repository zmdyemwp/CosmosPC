#ifndef		__COSMOS_SPP_H__
#define		__COSMOS_SPP_H__

#include "ComIO.h"
#include "CosmosReadCommProc.h"

class CosmosSpp:public ComIO {
	public:
		CosmosSpp(DWORD com):ComIO(com) {
			unsigned char theack[] = {0xaa, 0xbb, 0xcc, 0x03};
			memcpy(ack, theack, sizeof(theack));
		}
		CosmosSpp(TCHAR* com):ComIO(com) {}
		static void RequestFrom(CosmosSpp& obj, DWORD,CosmosReadCommProc*);
};

#endif		//__COSMOS_SPP_H__

