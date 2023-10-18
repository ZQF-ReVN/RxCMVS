#pragma once
#include "Basic.h"


namespace Rut::RxFile
{
	void SaveFileViaPath(std::wstring_view wsPath, void* pData, size_t nBytes);
	void SaveFileViaPath(std::string_view msPath, void* pData, size_t nBytes);

	class Binary : public Basic
	{
	public:
		Binary();
		Binary(std::string_view msPath, size_t nMode);
		Binary(std::wstring_view wsPath, size_t nMode);
		Binary(const Binary& refStream) = delete;
		~Binary();

	public:
		template <typename T_TYPE> operator T_TYPE() 
		{ 
			T_TYPE tmp{ 0 };
			this->Read((T_TYPE*)&tmp, sizeof(T_TYPE)); 
			return tmp; 
		}

		template <typename T_TYPE> Binary& operator >>(T_TYPE& TYPE) 
		{ 
			this->Read((void*)&TYPE, sizeof(TYPE));
			return *this; 
		}

		template <typename T_TYPE> Binary& operator <<(T_TYPE& TPYE) 
		{ 
			this->Write((void*)&TPYE, sizeof(TPYE));
			return *this; 
		}
	};

}