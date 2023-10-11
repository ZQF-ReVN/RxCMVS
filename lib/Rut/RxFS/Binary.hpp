#pragma once
#include <string>
#include <stdexcept>

#include "Basic.h"
#include "../RxPath.h"


namespace Rut::RxFS
{
	class Binary : public Basic
	{
	public:
		Binary() 
		{

		}

		template <typename T_STR> Binary(T_STR PATH, RIO emAccess, RCO emCreate = RCO::RCO_AUTO) 
		{ 
			this->Create(PATH, emAccess, emCreate);
		}

		Binary(const Binary& refStream) = delete;

		~Binary()
		{ 
			this->Close();
		}

	public:
		template <typename T_TYPE> operator T_TYPE() 
		{ 
			T_TYPE tmp{ 0 };
			Read((T_TYPE*)&tmp, sizeof(T_TYPE)); 
			return tmp; 
		}

		template <typename T_TYPE> Binary& operator >>(T_TYPE& TYPE) 
		{ 
			Read((void*)&TYPE, sizeof(TYPE)); 
			return *this; 
		}

		template <typename T_TYPE> Binary& operator <<(T_TYPE& TPYE) 
		{ 
			Write((void*)&TPYE, sizeof(TPYE));
			return *this; 
		}

	};

	template <typename T_STR> void SaveFileViaPath(T_STR PATH, void* pData, size_t nBytes)
	{
		RxPath::MakeDirViaPath(PATH);
		Binary{ PATH, RIO::RIO_OUT }.Write(pData, nBytes);
	}
}