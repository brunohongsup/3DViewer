#pragma once

#include <typeinfo>
#include <afxmt.h>
#include <type_traits>

#define RavidTheInstanceKindOf(pSrcInstance, pCheckingInstance) (dynamic_cast<decltype(pSrcInstance)>(pCheckingInstance) ? true : false)

#define RavidUseDynamicCreation(...) public: virtual CRavidObject* CreateInstance() { return new std::remove_pointer<decltype(this)>::type(__VA_ARGS__); }

#define RavidPreventCopySelf(x) x(const x&) = delete; x& operator=(const x&) = delete

class AFX_EXT_CLASS CRavidObject
{
 	RavidUseDynamicCreation();

public:

	template<class T>
	struct CheckIntegerAndRealNumber : std::integral_constant<bool, false> { };

	template<>
	struct CheckIntegerAndRealNumber<long long> : std::integral_constant<bool, true> { };

	template<>
	struct CheckIntegerAndRealNumber<int> : std::integral_constant<bool, true> { };

	template<>
	struct CheckIntegerAndRealNumber<float> : std::integral_constant<bool, true> { };

	template<>
	struct CheckIntegerAndRealNumber<double> : std::integral_constant<bool, true> { };

	template<class T>
	struct CheckRealNumber : std::integral_constant<bool, false> { };

	template<>
	struct CheckRealNumber<float> : std::integral_constant<bool, true> { };

	template<>
	struct CheckRealNumber<double> : std::integral_constant<bool, true> { };

public:
	CRavidObject();
	virtual ~CRavidObject();

	void __fastcall SetObjectID(_In_ long nObjectID);
	long __fastcall GetObjectID();

	void __fastcall Lock();
	void __fastcall Unlock();

	const CString __fastcall GetClassNameStr();
	const CString __fastcall GetClassRawNameStr();
	const type_info& __fastcall GetClassType();

	virtual bool __fastcall IsValid();

	bool __fastcall IsValid(_In_ int nValue);
	bool __fastcall IsValid(_In_ long long llValue);
	bool __fastcall IsValid(_In_ float fValue);
	bool __fastcall IsValid(_In_ double dblValue);
	
protected:

	void __fastcall MakeMin(_Out_ int& value);
	void __fastcall MakeMin(_Out_ int* pValue);
	void __fastcall MakeMin(_Out_ long long& value);
	void __fastcall MakeMin(_Out_ long long* pValue);
	void __fastcall MakeMin(_Out_ float& value);
	void __fastcall MakeMin(_Out_ float* pValue);
	void __fastcall MakeMin(_Out_ double& value);
	void __fastcall MakeMin(_Out_ double* pValue);

	void __fastcall MakeMax(_Out_ int& value);
	void __fastcall MakeMax(_Out_ int* pValue);
	void __fastcall MakeMax(_Out_ long long& value);
	void __fastcall MakeMax(_Out_ long long* pValue);
	void __fastcall MakeMax(_Out_ float& value);
	void __fastcall MakeMax(_Out_ float* pValue);
	void __fastcall MakeMax(_Out_ double& value);
	void __fastcall MakeMax(_Out_ double* pValue);

	void __fastcall MakeInvalid(_Out_ int& value);
	void __fastcall MakeInvalid(_Out_ int* pValue);
	void __fastcall MakeInvalid(_Out_ long long& value);
	void __fastcall MakeInvalid(_Out_ long long* pValue);
	void __fastcall MakeInvalid(_Out_ float& value);
	void __fastcall MakeInvalid(_Out_ float* pValue);
	void __fastcall MakeInvalid(_Out_ double& value);
	void __fastcall MakeInvalid(_Out_ double* pValue);

private:
	CCriticalSection* m_pCriticalSection = nullptr;
	long m_nObjectID = -1;
};

