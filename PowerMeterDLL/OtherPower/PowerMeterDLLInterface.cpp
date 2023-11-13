// Include User header file
#include "C:/Users/Mannan/source/repos/PowerMeterDLL/PowerMeterDLL/PowerMeterDLL.h"

// MATLAB Includes
//
#include "MatlabDataArray.hpp"

// Language Includes
//
#include <cstdint>
#include <exception>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <type_traits>

#ifdef _WIN64
   #define MATLAB_CPP_INTERFACE_API __declspec(dllexport)
#else
   #define MATLAB_CPP_INTERFACE_API __attribute__((visibility("default")))
#endif

namespace { // begin anonymous namespace

class mwCppInterfaceException {
public:
    virtual const char *what() const = 0;
    virtual ~mwCppInterfaceException() {}
};
class mwCppInterfaceExceptionAmbiguousConversion : public mwCppInterfaceException {
public:
    virtual const char *what() const override {
        return "AmbiguousConversion";
    }
};
class mwCppInterfaceExceptionAccessViolation : public mwCppInterfaceException {
public:
    virtual const char *what() const override {
        return "AccessViolation";
    }
};
class mwCppInterfaceExceptionArraySizeMismatch : public mwCppInterfaceException {
public:
    virtual const char *what() const override {
        return "ArraySizeMismatch";
    }
};
class mwCppInterfaceExceptionForFunctionType : public mwCppInterfaceException {
    std::string message;
public:
    mwCppInterfaceExceptionForFunctionType(const char* msg) : message(msg) {}
    void setMessageInfo(const std::string& functionTypeMLName, const std::string& functionTypeInput)
    {
        message += ";" + functionTypeMLName + ";" + functionTypeInput;
    }
    virtual const char *what() const override {
        return message.c_str();
    }
};
class mwCppInterfaceExceptionFunctionTypeInputNotFound : public mwCppInterfaceExceptionForFunctionType {
public:
    mwCppInterfaceExceptionFunctionTypeInputNotFound() : mwCppInterfaceExceptionForFunctionType("FunctionNotFound") {}
};
class mwCppInterfaceExceptionFunctionTypeInputNotMatching : public mwCppInterfaceExceptionForFunctionType {
public:
    mwCppInterfaceExceptionFunctionTypeInputNotMatching() : mwCppInterfaceExceptionForFunctionType("NotMatchingFunction") {}
};
class mwCppInterfaceExceptionFunctionTypeOverloadInputNotAllowed : public mwCppInterfaceExceptionForFunctionType {
public:
    mwCppInterfaceExceptionFunctionTypeOverloadInputNotAllowed() : mwCppInterfaceExceptionForFunctionType("OverloadFunctionNotAllowed") {}
};

template<typename mwCppT>
mwCppT mwCppDynamicCast(uint64_t, uint32_t)
{
    throw std::runtime_error("Unsupported conversion");
    return nullptr;
}

template<typename mwCppT> inline uint32_t mwClassIDInInterface()
{
    throw std::runtime_error("Undefined classID for a C++ type");
    return 0;
}

template<typename T> class mwContainerClassForClassID {};

template<typename T> class mwOpaqueTypeForClassID {};
template<typename mwCppT> mwCppT mwCppGetFunctionReferenceForStdFunction(const std::string& mwCppFcnName);
template<typename mwCppT> mwCppT mwCppGetFunctionReferenceForCFunctionPtr(const std::string& mwCppFcnName);

template<typename mwCppT>
inline
mwCppT *mwCppGetPointer(const matlab::data::Array& mwCpparr)
{
    matlab::data::StructArray const mwCpps(mwCpparr);
    matlab::data::Array mwCppelem1 = mwCpps[0]["address"];
    uint64_t mwCppaddress = mwCppelem1[0];
    matlab::data::Array mwCppelem2 = mwCpps[0]["classID"];
    uint32_t mwCppclassID = mwCppelem2[0];
    return mwCppDynamicCast<mwCppT*>(mwCppaddress, mwCppclassID);
}

template<typename mwCppT>
inline
mwCppT *mwCppGetSharedPointer(const matlab::data::Array& mwCpparr)
{
    matlab::data::StructArray const mwCpps(mwCpparr);
    matlab::data::Array mwCppelem1 = mwCpps[0]["sharedPtrAddress"];
    uint64_t mwCppaddress = mwCppelem1[0];
    matlab::data::Array mwCppelem2 = mwCpps[0]["sharedPtrClassID"];
    uint32_t mwCppclassID = mwCppelem2[0];
    return mwCppDynamicCast<mwCppT*>(mwCppaddress, mwCppclassID);
}

template<typename mwCppT>
inline
mwCppT mwCppGetSharedPtrObject(const matlab::data::Array& mwCpparr)
{
    matlab::data::StructArray const mwCpps(mwCpparr);
    matlab::data::Array mwCppelem1 = mwCpps[0]["sharedPtrAddress"];
    uint64_t mwCppaddress = mwCppelem1[0];
    matlab::data::Array mwCppelem2 = mwCpps[0]["sharedPtrClassID"];
    uint32_t mwCppclassID = mwCppelem2[0];
    return mwCppDynamicCast<mwCppT>(mwCppaddress, mwCppclassID);
}

template<typename mwCppT>
inline
mwCppT mwCppGetScalarValue(const matlab::data::Array& mwCpparr)
{
    matlab::data::TypedArray<int64_t> const mwCppval(mwCpparr);
    int64_t mwCppival = mwCppval[0];
    return static_cast<mwCppT>(mwCppival);
}

template<>
inline
double mwCppGetScalarValue<double>(const matlab::data::Array& mwCpparr)
{
    matlab::data::TypedArray<double> const mwCppval(mwCpparr);
    double mwCppdval = mwCppval[0];
    return mwCppdval;
}

template<>
inline
float mwCppGetScalarValue<float>(const matlab::data::Array& mwCpparr)
{
    matlab::data::TypedArray<double> const mwCppval(mwCpparr);
    float mwCppfval = static_cast<float>(mwCppval[0]);
    return mwCppfval;
}

template<typename mwCharT, size_t mwCharZ = sizeof(mwCharT)>
struct MwMDATypeForCPPChar
{
    using mwType = int8_t;
};

template<typename mwCharT>
struct MwMDATypeForCPPChar<mwCharT, 2>
{
    using mwType = int16_t;
};

template<typename mwCharT>
struct MwMDATypeForCPPChar<mwCharT, 4>
{
    using mwType = int32_t;
};

template<typename mwStrT>
inline
mwStrT mwCppGetScalarString(const matlab::data::Array& mwCpparr)
{
    matlab::data::TypedArray<typename MwMDATypeForCPPChar<typename mwStrT::value_type>::mwType> mwCppta(mwCpparr);
    mwStrT mwCppbuf(mwCppta.getNumberOfElements(), 0);
    auto mwCppsrc = mwCppta.begin();
    for (auto & mwCpptgt : mwCppbuf) {
        mwCpptgt = *mwCppsrc++;
    }
    return mwCppbuf;
}

template<>
inline
std::string mwCppGetScalarValue<std::string>(const matlab::data::Array& mwCpparr)
{
    return mwCppGetScalarString<std::string>(mwCpparr);
}

template<>
inline
std::wstring mwCppGetScalarValue<std::wstring>(const matlab::data::Array& mwCpparr)
{
    return mwCppGetScalarString<std::wstring>(mwCpparr);
}

template<>
inline
std::u16string mwCppGetScalarValue<std::u16string>(const matlab::data::Array& mwCpparr)
{
    return mwCppGetScalarString<std::u16string>(mwCpparr);
}

template<>
inline
std::u32string mwCppGetScalarValue<std::u32string>(const matlab::data::Array& mwCpparr)
{
    return mwCppGetScalarString<std::u32string>(mwCpparr);
}

template<>
inline
std::complex<double> mwCppGetScalarValue<std::complex<double>>(const matlab::data::Array& mwCpparr)
{
    return static_cast<const std::complex<double>&>(mwCpparr[0]);
}

template<>
inline
std::complex<float> mwCppGetScalarValue<std::complex<float>>(const matlab::data::Array& mwCpparr)
{
    return static_cast<const std::complex<float>&>(mwCpparr[0]);
}

template<>
inline
std::complex<int8_t> mwCppGetScalarValue<std::complex<int8_t>>(const matlab::data::Array& mwCpparr)
{
    return static_cast<const std::complex<int8_t>&>(mwCpparr[0]);
}

template<>
inline
std::complex<uint8_t> mwCppGetScalarValue<std::complex<uint8_t>>(const matlab::data::Array& mwCpparr)
{
    return static_cast<const std::complex<uint8_t>&>(mwCpparr[0]);
}

template<>
inline
std::complex<int16_t> mwCppGetScalarValue<std::complex<int16_t>>(const matlab::data::Array& mwCpparr)
{
    return static_cast<const std::complex<int16_t>&>(mwCpparr[0]);
}

template<>
inline
std::complex<uint16_t> mwCppGetScalarValue<std::complex<uint16_t>>(const matlab::data::Array& mwCpparr)
{
    return static_cast<const std::complex<uint16_t>&>(mwCpparr[0]);
}

template<>
inline
std::complex<int32_t> mwCppGetScalarValue<std::complex<int32_t>>(const matlab::data::Array& mwCpparr)
{
    return static_cast<const std::complex<int32_t>&>(mwCpparr[0]);
}

template<>
inline
std::complex<uint32_t> mwCppGetScalarValue<std::complex<uint32_t>>(const matlab::data::Array& mwCpparr)
{
    return static_cast<const std::complex<uint32_t>&>(mwCpparr[0]);
}

template<>
inline
std::complex<int64_t> mwCppGetScalarValue<std::complex<int64_t>>(const matlab::data::Array& mwCpparr)
{
    return static_cast<const std::complex<int64_t>&>(mwCpparr[0]);
}

template<>
inline
std::complex<uint64_t> mwCppGetScalarValue<std::complex<uint64_t>>(const matlab::data::Array& mwCpparr)
{
    return static_cast<const std::complex<uint64_t>&>(mwCpparr[0]);
}

template<typename mwCppT>
class mwCppGetScalarPointer
{
private:
    mwCppT mwCppfval;
public:
    mwCppGetScalarPointer() {};
    explicit mwCppGetScalarPointer(const matlab::data::Array& mwCpparr) : mwCppfval(mwCppGetScalarValue<mwCppT>(mwCpparr)) {};
    operator mwCppT *() { return &mwCppfval; }
    mwCppT mwCppvalue() const { return mwCppfval; }
};

template<bool mwCppfromML, typename mwCppmlItr, typename mwCppcppVal>
void mwCppTransformCopyNDArray(mwCppmlItr mwCppmlData, mwCppcppVal *mwCppcppData, const matlab::data::ArrayDimensions &mwCppdims)
{
    const size_t mwCpprank = mwCppdims.size();
    std::vector<size_t> mwCppofs(mwCpprank);
    std::vector<size_t> mwCppofs_reset(mwCpprank);
    std::vector<size_t> mwCppcounters(mwCpprank, 0U);

    mwCppofs[mwCpprank - 1] = 1;
    if (mwCppfromML) {
        mwCppofs_reset[mwCpprank - 1] = mwCppdims[0] - 1;
        for (size_t mwCppi = mwCpprank - 1; mwCppi > 0; --mwCppi) {
            const size_t mwCppdim_i = mwCpprank - mwCppi;
            mwCppofs[mwCppi - 1] = mwCppofs[mwCppi] * mwCppdims[mwCppdim_i - 1];
            mwCppofs_reset[mwCppi - 1] = (mwCppdims[mwCppdim_i] - 1) * mwCppofs[mwCppi - 1];
        }

        size_t mwCppofs_ml = 0;
        size_t mwCppr;
        do {
            *mwCppcppData++ = static_cast<mwCppcppVal>(*(mwCppmlData + mwCppofs_ml));

            // increment the mwCppcounters and calculate matlab offset
            for (mwCppr = 0U; mwCppr < mwCpprank; ++mwCppr) {
                if (++mwCppcounters[mwCppr] < mwCppdims[mwCpprank - mwCppr - 1]) {
                    mwCppofs_ml += mwCppofs[mwCppr];
                    break;
                }
                mwCppcounters[mwCppr] = 0U;
                mwCppofs_ml -= mwCppofs_reset[mwCppr];
            }
        } while (mwCppr < mwCpprank);
    }
    else {
        // to MATLAB
        mwCppofs_reset[mwCpprank - 1] = mwCppdims[mwCpprank - 1] - 1;
        for (size_t mwCppi = mwCpprank - 1; mwCppi > 0; --mwCppi) {
            mwCppofs[mwCppi - 1] = mwCppofs[mwCppi] * mwCppdims[mwCppi];
            mwCppofs_reset[mwCppi - 1] = (mwCppdims[mwCppi - 1] - 1) * mwCppofs[mwCppi - 1];
        }

        size_t mwCppofs_cpp = 0;
        size_t mwCppr;
        do {
            *mwCppmlData++ = *(mwCppcppData + mwCppofs_cpp);

            // increment the mwCppcounters and calculate cpp offset
            for (mwCppr = 0U; mwCppr < mwCpprank; ++mwCppr) {
                if (++mwCppcounters[mwCppr] < mwCppdims[mwCppr]) {
                    mwCppofs_cpp += mwCppofs[mwCppr];
                    break;
                }
                mwCppcounters[mwCppr] = 0U;
                mwCppofs_cpp -= mwCppofs_reset[mwCppr];
            }
        } while (mwCppr < mwCpprank);
    }
}

template<bool mwCppfromML, typename mwCppmlItr, typename mwCppcppVal>
void mwCppTransformCopyArray(mwCppmlItr mwCppmlData, mwCppcppVal *mwCppcppData, const matlab::data::ArrayDimensions &mwCppdims)
{
    const size_t mwCpprank = mwCppdims.size();
    if (mwCpprank <= 3) {
        if (mwCppfromML) {
            size_t mwCppstep = 1;
            for (size_t mwCppk = 0; mwCppk < mwCpprank - 1; ++mwCppk) {
                mwCppstep *= mwCppdims[mwCppk];
            }

            const size_t mwCpplvdSize = mwCppdims[0];              // size of the least varying dimension
            const size_t mwCppmvdSize = mwCppdims[mwCpprank - 1];  // size of the most varying dimension
            size_t mwCppmlIndx;             // index of MATLAB (source) data elements
            size_t mwCppstart = 0;          // starting position offset when rolling over the step
            size_t mwCppoffset = 0;         // element offset to step over least varying demension size

            for (size_t mwCppi = 0; mwCppi < mwCppstep; ++mwCppi) {
                for (size_t mwCppj = 0; mwCppj < mwCppmvdSize; ++mwCppj) {
                    mwCppmlIndx = mwCppstart + mwCppoffset + mwCppstep * mwCppj;
                    *mwCppcppData++ = static_cast<mwCppcppVal>(*(mwCppmlData + mwCppmlIndx));
                }
                mwCppoffset += mwCpplvdSize;
                if (mwCppoffset >= mwCppstep) {
                    ++mwCppstart;
                    mwCppoffset = 0;
                }
            }
        }
        else {
            // to ML
            size_t mwCppstep = 1;
            for (size_t mwCppk = mwCpprank - 1; mwCppk > 0; --mwCppk) {
                mwCppstep *= mwCppdims[mwCppk];
            }

            const size_t mwCpplvdSize = mwCppdims[mwCpprank - 1];         // size of the least varying dimension
            const size_t mwCppmvdSize = mwCppdims[0];  // size of the most varying dimension
            size_t mwCppcppIndx;             // index of MATLAB (source) data elements
            size_t mwCppstart = 0;           // starting position offset when rolling over the step
            size_t mwCppoffset = 0;          // element offset to step over least varying demension size

            for (size_t mwCppi = 0; mwCppi < mwCppstep; ++mwCppi) {
                for (size_t mwCppj = 0; mwCppj < mwCppmvdSize; ++mwCppj) {
                    mwCppcppIndx = mwCppstart + mwCppoffset + mwCppstep * mwCppj;
                    *mwCppmlData++ = *(mwCppcppData + mwCppcppIndx);
                }
                mwCppoffset += mwCpplvdSize;
                if (mwCppoffset >= mwCppstep) {
                    ++mwCppstart;
                    mwCppoffset = 0;
                }
            }
        }
    }
    else {
        mwCppTransformCopyNDArray<mwCppfromML>(mwCppmlData, mwCppcppData, mwCppdims);
    }
}

template<typename mwCppmlItr, typename mwCppcppVal>
void mwCppCopyArrayFromML(mwCppmlItr mwCppmlData, mwCppcppVal *mwCppcppData, const matlab::data::ArrayDimensions &mwCppdims)
{
    mwCppTransformCopyArray<true>(mwCppmlData, mwCppcppData, mwCppdims);
}

template<typename mwCppmlItr, typename mwCppcppVal>
void mwCppCopyArrayFromCPP(mwCppmlItr mwCppmlData, mwCppcppVal *mwCppcppData, const matlab::data::ArrayDimensions &mwCppdims)
{
    using mwCppnoConstCpp = typename std::remove_cv<mwCppcppVal>::type;
    mwCppTransformCopyArray<false>(mwCppmlData, const_cast<mwCppnoConstCpp *>(mwCppcppData), mwCppdims);
}

uint64_t mwProdArray(const matlab::data::TypedArray<int64_t> &arr)
{
    uint64_t prod = 1;
    for(auto elem : arr)
        prod *= elem;
    return prod;
}

template<bool trailingSingletons=false>
std::vector<size_t> mwCppConvertDimsFromMLtoCPP(const matlab::data::ArrayDimensions &mwCppmlDims, size_t mwCppcRank)
{
    std::vector<size_t> mwCppcDims(mwCppcRank);
    size_t mwCppci = 0;
    size_t mwCppmli = 0;
    const auto mwCppmlSize = mwCppmlDims.size();
    if (mwCppcRank == 1) {
        mwCppcDims[0] = matlab::data::getNumElements(mwCppmlDims);
    }
    else {
        if (mwCppmlSize > mwCppcRank) {
            size_t mwCppdiff = mwCppmlSize - mwCppcRank;
            for (; mwCppmli < mwCppmlSize && mwCppdiff > 0; ++mwCppmli) {
                if (mwCppmlDims[mwCppmli] != 1) {
                    mwCppcDims[mwCppci++] = mwCppmlDims[mwCppmli];
                }
                else {
                    --mwCppdiff;
                }
            }
        }
        else if (mwCppmlSize < mwCppcRank) {
            mwCppci = mwCppcRank - mwCppmlSize;
            if(!trailingSingletons){
                std::fill(mwCppcDims.begin(), mwCppcDims.begin() + mwCppci, 1);
            }
            else{
                std::fill(mwCppcDims.rbegin(), mwCppcDims.rbegin() + mwCppci, 1);
                mwCppci = 0;
            }
        }
        std::copy(mwCppmlDims.begin() + mwCppmli, mwCppmlDims.end(), mwCppcDims.begin() + mwCppci);
    }
    return mwCppcDims;
}

template<typename mwCppTML, typename mwCppTCPP, bool mwTrailingSingletons=false>
class MwCppArrayForArgument {
private:
    using mwCppTRAW = typename std::remove_all_extents<mwCppTCPP>::type;
    std::unique_ptr<mwCppTRAW> mwCppfData;
    const matlab::data::ArrayDimensions mwCppfDims;
    const std::vector<size_t> mwCppfcDims;

public:
    MwCppArrayForArgument(const matlab::data::Array& mwCpparr, size_t mwCppcRank);
    explicit MwCppArrayForArgument(const matlab::data::ArrayDimensions &mwCppdims);
    explicit MwCppArrayForArgument(size_t mwCpplen);
    operator mwCppTCPP*() { return reinterpret_cast<mwCppTCPP*>(mwCppfData.get()); }
    operator const mwCppTCPP*() const { return reinterpret_cast<const mwCppTCPP*>(mwCppfData.get()); }
    size_t mwCppdim(int mwCppd) const { return mwCppfcDims[mwCppd]; }
    size_t size() const { return matlab::data::getNumElements(mwCppfDims); }
    matlab::data::Array mwCppMLArray();
};

template<typename mwCppTML, typename mwCppTCPP, bool mwTrailingSingletons>
MwCppArrayForArgument<mwCppTML, mwCppTCPP, mwTrailingSingletons>::MwCppArrayForArgument(const matlab::data::Array& mwCpparr, size_t mwCppcRank)
    : mwCppfDims(mwCpparr.getDimensions())
    , mwCppfcDims(mwCppConvertDimsFromMLtoCPP<mwTrailingSingletons>(mwCppfDims, mwCppcRank))
{
    auto mwCppnumElms = matlab::data::getNumElements(mwCppfDims);
    mwCppfData.reset(new mwCppTRAW[mwCppnumElms]);

    matlab::data::TypedArray<mwCppTML> mwCpptarr(mwCpparr);
    mwCppCopyArrayFromML(mwCpptarr.begin(), mwCppfData.get(), mwCppfDims);
}

template<typename mwCppTML, typename mwCppTCPP, bool mwTrailingSingletons>
MwCppArrayForArgument<mwCppTML, mwCppTCPP, mwTrailingSingletons>::MwCppArrayForArgument(const matlab::data::ArrayDimensions &mwCppdims)
    : mwCppfDims(mwCppdims)
    , mwCppfcDims(mwCppfDims)
{
    auto mwCppnumElms = matlab::data::getNumElements(mwCppfDims);
    mwCppfData.reset(new mwCppTRAW[mwCppnumElms]);
}

template<typename mwCppTML, typename mwCppTCPP,bool mwTrailingSingletons>
MwCppArrayForArgument<mwCppTML, mwCppTCPP, mwTrailingSingletons>::MwCppArrayForArgument(size_t mwCpplen)
    : mwCppfDims({ 1,mwCpplen })
    , mwCppfcDims({ mwCpplen })
{
    auto mwCppnumElms = matlab::data::getNumElements(mwCppfDims);
    mwCppfData.reset(new mwCppTRAW[mwCppnumElms]);
}

template<typename mwCppTML, typename mwCppTCPP, bool mwTrailingSingletons>
inline
matlab::data::Array MwCppArrayForArgument<mwCppTML, mwCppTCPP, mwTrailingSingletons>::mwCppMLArray()
{
    const auto mwCppnumElms = matlab::data::getNumElements(mwCppfDims);
    matlab::data::ArrayFactory mwCppfactory;
    auto mwCppmlBuf = mwCppfactory.createBuffer<mwCppTML>(mwCppnumElms);
    mwCppCopyArrayFromCPP(mwCppmlBuf.get(), mwCppfData.get(), mwCppfDims);
    return mwCppfactory.createArrayFromBuffer(mwCppfDims, std::move(mwCppmlBuf));
}

template<typename mwCppT>
mwCppT * mwCppGetVecData(std::vector<mwCppT> const *v)
{
    return (*const_cast<std::vector<mwCppT>*>(v)).data();
}

template<>
bool * mwCppGetVecData<bool>(std::vector<bool> const *)
{
    return nullptr;
}

template<typename mwCppVecT>
class MwCppContainer {
protected:
    using mwCppElemT = typename mwCppVecT::value_type;
    mwCppVecT *mwCppPVec;
    mwCppElemT *mwCppPRaw;
    matlab::data::ArrayDimensions mwCppfcDims;
public:
    explicit MwCppContainer(matlab::data::Array const &mwCppMDA);
    mwCppElemT * mwCppGetPtr() {return mwCppPRaw;}
    const matlab::data::ArrayDimensions &mwCppDims() const {return mwCppfcDims;}
    matlab::data::ArrayDimensions mwCppMlDims() const;
    size_t mwCppDim(int mwCppD) const {return mwCppfcDims[mwCppD];}
    size_t mwCppNumel() const {return matlab::data::getNumElements(mwCppfcDims);}
};

template<typename mwCppVecT>
MwCppContainer<mwCppVecT>::MwCppContainer(matlab::data::Array const &mwCppMDA)
{
    matlab::data::StructArray mwCppMdaStrct(mwCppMDA);
    matlab::data::Array mwCppArrAdrType = mwCppMdaStrct[0]["addressType"];
    matlab::data::Array mwCppArrAdrVal = mwCppMdaStrct[0]["address"];
    const uint64_t mwCppAdrType{static_cast<uint64_t>(mwCppArrAdrType[0])};
    uint64_t mwCppAdrVal{static_cast<uint64_t>(mwCppArrAdrVal[0])};
    if(mwCppAdrType==0) {
        mwCppPVec = reinterpret_cast<mwCppVecT*>(mwCppAdrVal);
        mwCppPRaw = mwCppGetVecData(mwCppPVec);
        mwCppfcDims.push_back((*mwCppPVec).size());
    } else {
        mwCppPVec = nullptr;
        mwCppPRaw = reinterpret_cast<mwCppElemT*>(mwCppAdrVal);
        matlab::data::TypedArray<int64_t> mwCppContDims = mwCppMdaStrct[0]["dimensions"];
        std::copy(mwCppContDims.begin(), mwCppContDims.end(), std::back_inserter(mwCppfcDims));
    }
}

template<typename mwCppVecT>
matlab::data::ArrayDimensions MwCppContainer<mwCppVecT>::mwCppMlDims() const
{
    matlab::data::ArrayDimensions mwCppMlDims;
    if(mwCppfcDims.size()==1) {
        mwCppMlDims.push_back(1);
        mwCppMlDims.push_back(mwCppfcDims[0]);
    }
    else
        mwCppMlDims = mwCppfcDims;
    return mwCppMlDims;
}

template<>
class MwCppContainer<std::vector<bool>> {
protected:
    using mwCppElemT = bool;
    std::vector<bool> *mwCppPVec;
    bool *mwCppPRaw;
    matlab::data::ArrayDimensions mwCppfcDims;
public:
    explicit MwCppContainer(matlab::data::Array const &mwCppMDA);
    ~MwCppContainer();
    bool * mwCppGetPtr();
    const matlab::data::ArrayDimensions &mwCppDims() const {return mwCppfcDims;}
    matlab::data::ArrayDimensions mwCppMlDims() const;
    size_t mwCppDim(int mwCppD) const {return mwCppfcDims[mwCppD];}
    size_t mwCppNumel() const {return matlab::data::getNumElements(mwCppfcDims);}
};

MwCppContainer<std::vector<bool>>::MwCppContainer(matlab::data::Array const &mwCppMDA)
{
    matlab::data::StructArray mwCppMdaStrct(mwCppMDA);
    matlab::data::Array mwCppArrAdrType = mwCppMdaStrct[0]["addressType"];
    matlab::data::Array mwCppArrAdrVal = mwCppMdaStrct[0]["address"];
    const uint64_t mwCppAdrType{static_cast<uint64_t>(mwCppArrAdrType[0])};
    uint64_t mwCppAdrVal{static_cast<uint64_t>(mwCppArrAdrVal[0])};
    if(mwCppAdrType==0) {
        mwCppPVec = reinterpret_cast<std::vector<bool>*>(mwCppAdrVal);
        mwCppPRaw = nullptr;
        mwCppfcDims.push_back((*mwCppPVec).size());
    } else {
        mwCppPVec = nullptr;
        mwCppPRaw = reinterpret_cast<bool*>(mwCppAdrVal);
        matlab::data::TypedArray<int64_t> mwCppContDims = mwCppMdaStrct[0]["dimensions"];
        std::copy(mwCppContDims.begin(), mwCppContDims.end(), std::back_inserter(mwCppfcDims));
    }
}

bool * MwCppContainer<std::vector<bool>>::mwCppGetPtr()
{
    if(!mwCppPRaw) {
        mwCppPRaw = new bool[(*mwCppPVec).size()];
        std::copy((*mwCppPVec).begin(), (*mwCppPVec).end(), mwCppPRaw);
    }
    return mwCppPRaw;
}

MwCppContainer<std::vector<bool>>::~MwCppContainer()
{
    if(mwCppPVec && mwCppPRaw) {
        std::copy(mwCppPRaw, mwCppPRaw+(*mwCppPVec).size(), (*mwCppPVec).begin());
        delete [] mwCppPRaw;
    }
}

matlab::data::ArrayDimensions MwCppContainer<std::vector<bool>>::mwCppMlDims() const
{
    matlab::data::ArrayDimensions mwCppMlDims;
    if(mwCppfcDims.size()==1) {
        mwCppMlDims.push_back(1);
        mwCppMlDims.push_back(mwCppfcDims[0]);
    }
    else
        mwCppMlDims = mwCppfcDims;
    return mwCppMlDims;
}

template<typename mwCppVecT, typename mwCppArrT>
class MwCppContainerForBoxed : public MwCppContainer<mwCppVecT> {
    using mwCppElmT = typename std::remove_extent<mwCppArrT>::type;
public:
    MwCppContainerForBoxed(matlab::data::Array const &mwCppMDA) : MwCppContainer<mwCppVecT>(mwCppMDA) {}
    operator mwCppElmT *() { return reinterpret_cast<mwCppElmT*>(MwCppContainer<mwCppVecT>::mwCppGetPtr()); }
};

template<typename mwCppVecT>
class MwCppContainerForVector : public MwCppContainer<mwCppVecT> {
public:
    MwCppContainerForVector(matlab::data::Array const &mwCppMDA) : MwCppContainer<mwCppVecT>(mwCppMDA) {}
    operator mwCppVecT&() {return *MwCppContainer<mwCppVecT>::mwCppPVec;}
};

template<typename mwCppVecT>
class MwCppContainerForPointer : public MwCppContainer<mwCppVecT> {
public:
    MwCppContainerForPointer(matlab::data::Array const &mwCppMDA) : MwCppContainer<mwCppVecT>(mwCppMDA) {}
    operator typename MwCppContainer<mwCppVecT>::mwCppElemT*() {return MwCppContainer<mwCppVecT>::mwCppGetPtr();}
};

template<typename mwCppCharT>
class MwCStringForArgument {
private:
    std::unique_ptr<mwCppCharT[]> mwCppfData;
public:
    explicit MwCStringForArgument(const matlab::data::Array& mwCpparr);
    operator mwCppCharT *() const { return mwCppfData.get(); }
};

template<typename mwCppCharT>
MwCStringForArgument<mwCppCharT>::MwCStringForArgument(const matlab::data::Array& mwCpparr)
{
    matlab::data::TypedArray<typename MwMDATypeForCPPChar<mwCppCharT>::mwType> mwCppta(mwCpparr);
    const matlab::data::ArrayDimensions mwCppdims{mwCppta.getDimensions()};
    if (mwCppdims[0] == 0 && mwCppdims[1] == 0) {
        mwCppfData = nullptr;
    }
    else {
        const auto mwCppnum = mwCppta.getNumberOfElements();
        mwCppfData = std::unique_ptr<mwCppCharT[]>(new mwCppCharT[mwCppnum + 1]);
        auto mwCppsrc = mwCppta.begin();
        unsigned mwCppi;
        for (mwCppi = 0; mwCppi < mwCppnum; ++mwCppi) {
            mwCppfData[mwCppi] = *mwCppsrc++;
        }
        mwCppfData[mwCppi] = 0;
    }
}

template<typename mwCppCharT>
matlab::data::Array mwCppCreateArrayAsString(mwCppCharT const *mwCppcStr) {
    using mwCppIntT = typename MwMDATypeForCPPChar<mwCppCharT>::mwType;
    matlab::data::ArrayFactory mwCppfactory;
    if (mwCppcStr == nullptr) {
        return mwCppfactory.createArray<mwCppIntT>({ 0,0 });
    }

    auto mwCppfa = mwCppfactory.createArray<mwCppIntT>({ 1,std::char_traits<mwCppCharT>::length(mwCppcStr) });
    for (auto mwCppdst = mwCppfa.begin(); mwCppdst != mwCppfa.end(); ++mwCppdst) {
        *mwCppdst = *mwCppcStr++;
    }
    return mwCppfa;
}

template<typename mwCppCharT>
class MwCppCStringForOutput {
private:
    std::unique_ptr<mwCppCharT[]> mwCppfData;
public:
    explicit MwCppCStringForOutput(size_t mwCppLen) : mwCppfData(new mwCppCharT[mwCppLen+1]) {
        mwCppfData[mwCppLen] = 0;
    }
    operator mwCppCharT* () { return mwCppfData.get(); }
    operator matlab::data::Array () { return mwCppCreateArrayAsString(mwCppfData.get()); }
};

template<typename mwCppTMDA, typename mwCppTCPP>
class MwCppCStringArrayForArgument {
private:
    using mwCppTCPP_NOCV = typename std::remove_cv<mwCppTCPP>::type;
    std::vector<std::unique_ptr<mwCppTCPP_NOCV> > mwCppfCStrings;
    std::unique_ptr<mwCppTCPP_NOCV*[]> mwCppfArray;
public:
    explicit MwCppCStringArrayForArgument(const matlab::data::Array& mwCppMdaArr);
    operator mwCppTCPP**() { return const_cast<mwCppTCPP**>(mwCppfArray.get()); }
    mwCppTCPP** operator &() { return const_cast<mwCppTCPP**>(mwCppfArray.get()); }
    size_t size() const { return mwCppfCStrings.size(); }
};

template<typename mwCppTMDA, typename mwCppTCPP>
MwCppCStringArrayForArgument<mwCppTMDA, mwCppTCPP>::MwCppCStringArrayForArgument(const matlab::data::Array& mwCppMdaArr) {
    matlab::data::TypedArray<matlab::data::Array> mwCppStrArr(mwCppMdaArr);
    const auto mwCppNumel{ mwCppStrArr.getNumberOfElements() };
    mwCppfArray = std::unique_ptr<mwCppTCPP_NOCV *[]>(new mwCppTCPP_NOCV*[mwCppNumel]);
    mwCppfCStrings.reserve(mwCppNumel);
    auto mwCppIdx = 0u;
    for(const auto & mwCppMdaStr : mwCppStrArr) {
        matlab::data::TypedArray<mwCppTMDA> mwCppStr(mwCppMdaStr);
        const auto mwCppStrLen{ mwCppStr.getNumberOfElements() };
        mwCppfCStrings.emplace_back(new mwCppTCPP_NOCV [mwCppStrLen + 1]);
        mwCppfArray[mwCppIdx] = mwCppfCStrings.back().get();
        std::copy(mwCppStr.begin(), mwCppStr.end(), mwCppfArray[mwCppIdx]);
        mwCppfArray[mwCppIdx++][mwCppStrLen] = 0;
    }
}

template<typename mwCppT>
class MwCppEmbeddedArgument {
    bool mwCppfInit;
    mwCppT mwCppfVal;
public:
    MwCppEmbeddedArgument() : mwCppfInit(false) {}
    void operator=(const mwCppT&&mwCppt);
    operator mwCppT&() { return mwCppfVal; }
};

template<typename mwCppT>
void MwCppEmbeddedArgument<mwCppT>::operator=(const mwCppT&&mwCppt)
{
    if (mwCppfInit) {
        if (mwCppt != mwCppfVal)
            throw mwCppInterfaceExceptionArraySizeMismatch();
    }
    else {
        mwCppfVal = mwCppt;
        mwCppfInit = true;
    }
}

struct mwCppObjKey {
uint64_t mwCppAddress;
uint32_t mwCppClassID;
};

bool operator< (const mwCppObjKey& mwCppLhs, const mwCppObjKey& mwCppRhs) {
    return mwCppLhs.mwCppAddress < mwCppRhs.mwCppAddress;
}

std::set<mwCppObjKey> mwCppSharedPtrObjMDASet, mwCppSharedPtrConstObjMDASet;

template<typename mwCppT>
bool mwCppAddToSet(mwCppT* mwCppPtr)
{
    mwCppObjKey mwCppKey{ reinterpret_cast<uint64_t>(mwCppPtr), mwClassIDInInterface<typename std::remove_cv<mwCppT>::type>() };
    bool mwCppIsConstObj = std::is_const<mwCppT>::value;

    if (mwCppIsConstObj) {
        auto mwCppResult = mwCppSharedPtrConstObjMDASet.insert(mwCppKey);
        return mwCppResult.second;
    }
    else {
        auto mwCppResult = mwCppSharedPtrObjMDASet.insert(mwCppKey);
        return mwCppResult.second;
    }
}

template<typename mwCppT>
void mwCppRemoveFromSet(mwCppT* mwCppPtr)
{
    mwCppObjKey mwCppKey{ reinterpret_cast<uint64_t>(mwCppPtr), mwClassIDInInterface<typename std::remove_cv<mwCppT>::type>() };

    auto mwCppResult = mwCppSharedPtrObjMDASet.find(mwCppKey);
    if (mwCppResult != mwCppSharedPtrObjMDASet.end())
        mwCppSharedPtrObjMDASet.erase(mwCppResult);
    else {
        auto mwCppResult = mwCppSharedPtrConstObjMDASet.find(mwCppKey);
        if (mwCppResult != mwCppSharedPtrConstObjMDASet.end())
            mwCppSharedPtrConstObjMDASet.erase(mwCppResult);
    }
}

template<typename mwCppT1, typename mwCppT2>
inline
matlab::data::Array mwCppCreateStructFromPointer(mwCppT1* mwCppptr, mwCppT2* mwCppsharedPtr)
{
    if (mwCppsharedPtr != nullptr && !mwCppAddToSet(mwCppptr)) {
        delete mwCppsharedPtr;
        mwCppsharedPtr = nullptr;
    }
    matlab::data::ArrayFactory mwCppfactory;
    auto mwCpparr = mwCppfactory.createStructArray({1,1}, {"address", "classID", "sharedPtrAddress", "sharedPtrClassID"});
    mwCpparr[0][0]["address"] = mwCppfactory.createScalar(reinterpret_cast<uint64_t>(mwCppptr));
    mwCpparr[0][0]["classID"] = mwCppfactory.createScalar(mwClassIDInInterface<typename std::remove_cv<mwCppT1>::type>());
    mwCpparr[0][0]["sharedPtrAddress"] = mwCppfactory.createScalar(reinterpret_cast<uint64_t>(mwCppsharedPtr));
    mwCpparr[0][0]["sharedPtrClassID"] = mwCppfactory.createScalar(mwClassIDInInterface<typename std::remove_cv<mwCppT2>::type>());
    return mwCpparr;
}

template<typename mwCppT1, typename mwCppT2>
inline
matlab::data::Array mwCppCreateStructFromPointer(mwCppT1& mwCppref, mwCppT2* mwCppsharedPtr)
{
    if (mwCppsharedPtr != nullptr && !mwCppAddToSet(&mwCppref)) {
        delete mwCppsharedPtr;
        mwCppsharedPtr = nullptr;
    }
    matlab::data::ArrayFactory mwCppfactory;
    auto mwCpparr = mwCppfactory.createStructArray({1,1}, {"address", "classID", "sharedPtrAddress", "sharedPtrClassID"});
    mwCpparr[0][0]["address"] = mwCppfactory.createScalar(reinterpret_cast<uint64_t>(&mwCppref));
    mwCpparr[0][0]["classID"] = mwCppfactory.createScalar(mwClassIDInInterface<typename std::remove_cv<mwCppT1>::type>());
    mwCpparr[0][0]["sharedPtrAddress"] = mwCppfactory.createScalar(reinterpret_cast<uint64_t>(mwCppsharedPtr));
    mwCpparr[0][0]["sharedPtrClassID"] = mwCppfactory.createScalar(mwClassIDInInterface<typename std::remove_cv<mwCppT2>::type>());
    return mwCpparr;
}

template<typename mwCppT1, typename mwCppT2>
inline
matlab::data::Array mwCppCreateStructFromPointer(mwCppT1* mwCppptr, mwCppT2& mwCppsharedPtr)
{
    matlab::data::ArrayFactory mwCppfactory;
    auto mwCpparr = mwCppfactory.createStructArray({1,1}, {"address", "classID", "sharedPtrAddress", "sharedPtrClassID"});
    mwCpparr[0][0]["address"] = mwCppfactory.createScalar(reinterpret_cast<uint64_t>(mwCppptr));
    mwCpparr[0][0]["classID"] = mwCppfactory.createScalar(mwClassIDInInterface<typename std::remove_cv<mwCppT1>::type>());
    mwCpparr[0][0]["sharedPtrAddress"] = mwCppfactory.createScalar(reinterpret_cast<uint64_t>(&mwCppsharedPtr));
    mwCpparr[0][0]["sharedPtrClassID"] = mwCppfactory.createScalar(mwClassIDInInterface<typename std::remove_cv<mwCppT2>::type>());
    return mwCpparr;
}

template<typename mwCppT>
inline
matlab::data::Array mwCppCreateStructFromPointer(mwCppT* mwCppptr)
{
    return mwCppCreateStructFromPointer(mwCppptr, static_cast<std::shared_ptr<mwCppT>*>(nullptr));
}

template<typename mwCppT1, typename mwCppT2>
inline
matlab::data::Array mwCppCreateStructFromOpaqueType(mwCppT2* mwCppptr)
{
    matlab::data::ArrayFactory mwCppfactory;
    auto mwCppRet = mwCppfactory.createStructArray({1,1}, {"address", "classID" });
    mwCppRet[0][0]["address"] = mwCppfactory.createScalar(reinterpret_cast<uint64_t>(mwCppptr));
    mwCppRet[0][0]["classID"] = mwCppfactory.createScalar(mwClassIDInInterface<mwOpaqueTypeForClassID<mwCppT1>>());
    return mwCppRet;
}


template<typename mwCppT>
inline
matlab::data::Array mwCppCreateArray(mwCppT mwCppval)
{
    matlab::data::ArrayFactory mwCppfactory;
    int64_t mwCppival = static_cast<int64_t>(mwCppval);
    return mwCppfactory.createScalar(mwCppival);
}

template<>
inline
matlab::data::Array mwCppCreateArray<double>(double mwCppval)
{
    matlab::data::ArrayFactory mwCppfactory;
    return mwCppfactory.createScalar(mwCppval);
}

template<>
inline
matlab::data::Array mwCppCreateArray<float>(float mwCppval)
{
    matlab::data::ArrayFactory mwCppfactory;
    return mwCppfactory.createScalar<double>(mwCppval);
}

template<>
inline
matlab::data::Array mwCppCreateArray(std::complex<double> mwCppval)
{
    matlab::data::ArrayFactory mwCppfactory;
    return mwCppfactory.createScalar<std::complex<double>>(mwCppval);
}

template<>
inline
matlab::data::Array mwCppCreateArray(std::complex<float> mwCppval)
{
    matlab::data::ArrayFactory mwCppfactory;
    return mwCppfactory.createScalar<std::complex<float>>(mwCppval);
}

template<>
inline
matlab::data::Array mwCppCreateArray(std::complex<int8_t> mwCppval)
{
    matlab::data::ArrayFactory mwCppfactory;
    return mwCppfactory.createScalar<std::complex<int8_t>>(mwCppval);
}

template<>
inline
matlab::data::Array mwCppCreateArray(std::complex<uint8_t> mwCppval)
{
    matlab::data::ArrayFactory mwCppfactory;
    return mwCppfactory.createScalar<std::complex<uint8_t>>(mwCppval);
}

template<>
inline
matlab::data::Array mwCppCreateArray(std::complex<int16_t> mwCppval)
{
    matlab::data::ArrayFactory mwCppfactory;
    return mwCppfactory.createScalar<std::complex<int16_t>>(mwCppval);
}

template<>
inline
matlab::data::Array mwCppCreateArray(std::complex<uint16_t> mwCppval)
{
    matlab::data::ArrayFactory mwCppfactory;
    return mwCppfactory.createScalar<std::complex<uint16_t>>(mwCppval);
}

template<>
inline
matlab::data::Array mwCppCreateArray(std::complex<int32_t> mwCppval)
{
    matlab::data::ArrayFactory mwCppfactory;
    return mwCppfactory.createScalar<std::complex<int32_t>>(mwCppval);
}

template<>
inline
matlab::data::Array mwCppCreateArray(std::complex<uint32_t> mwCppval)
{
    matlab::data::ArrayFactory mwCppfactory;
    return mwCppfactory.createScalar<std::complex<uint32_t>>(mwCppval);
}

template<>
inline
matlab::data::Array mwCppCreateArray(std::complex<int64_t> mwCppval)
{
    matlab::data::ArrayFactory mwCppfactory;
    return mwCppfactory.createScalar<std::complex<int64_t>>(mwCppval);
}

template<>
inline
matlab::data::Array mwCppCreateArray(std::complex<uint64_t> mwCppval)
{
    matlab::data::ArrayFactory mwCppfactory;
    return mwCppfactory.createScalar<std::complex<uint64_t>>(mwCppval);
}

template<typename mwCppT>
inline
matlab::data::Array mwCppCreateArray(mwCppT *mwCppval)
{
    if (mwCppval == 0) {
        matlab::data::ArrayFactory mwCppfactory;
        return mwCppfactory.createArray<double>({ 0,0 });
    }
    else
        return mwCppCreateArray(*mwCppval);
}

template<typename mwCppStrT>
inline
matlab::data::Array mwCppCreateArrayString(const mwCppStrT &mwCppStr)
{
    matlab::data::ArrayFactory mwCppfactory;
    auto mwCppfa = mwCppfactory.createArray<typename MwMDATypeForCPPChar<typename mwCppStrT::value_type>::mwType>({ 1,mwCppStr.size() });
    auto mwCpptgt = mwCppfa.begin();
    for (const auto mwCppc : mwCppStr) {
        *mwCpptgt++ = mwCppc;
    }
    return mwCppfa;
}

matlab::data::Array mwCppCreateArray(const std::string &mwCppval)
{
    return mwCppCreateArrayString(mwCppval);
}

matlab::data::Array mwCppCreateArray(const std::wstring &mwCppval)
{
    return mwCppCreateArrayString(mwCppval);
}

matlab::data::Array mwCppCreateArray(const std::u16string &mwCppval)
{
    return mwCppCreateArrayString(mwCppval);
}

matlab::data::Array mwCppCreateArray(const std::u32string &mwCppval)
{
    return mwCppCreateArrayString(mwCppval);
}

template<typename mwCppTMDA, typename mwCppT>
inline
matlab::data::Array mwCppCreateArray(mwCppT *mwCppval, const matlab::data::ArrayDimensions &mwCppdims)
{
    matlab::data::ArrayFactory mwCppfactory;
    if (mwCppval == 0) {
        return mwCppfactory.createArray<double>({ 0,0 });
    }
    matlab::data::TypedArray<mwCppTMDA> mwCppta = mwCppfactory.createArray<mwCppTMDA>(mwCppdims);
    mwCppCopyArrayFromCPP(mwCppta.begin(), mwCppval, mwCppdims);
    return mwCppta;
}

template<typename mwCppTMDA, typename mwCppT>
inline
matlab::data::Array mwCppCreateArray(mwCppT *mwCppval, size_t mwCpplen)
{
    matlab::data::ArrayFactory mwCppfactory;
    if (mwCppval == 0) {
        return mwCppfactory.createArray<double>({ 0,0 });
    }
    matlab::data::ArrayDimensions mwCppdims = {1U, mwCpplen};
    matlab::data::TypedArray<mwCppTMDA> mwCppta = mwCppfactory.createArray<mwCppTMDA>(mwCppdims);
    mwCppCopyArrayFromCPP(mwCppta.begin(), mwCppval, mwCppdims);
    return mwCppta;
}

template<typename mwCppT>
inline
matlab::data::Array mwCppCreateContainer(std::vector<mwCppT> const *mwCppPVec, mwCppT const *mwCppPRaw, matlab::data::TypedArray<int64_t> &mwCppDim)
{
    matlab::data::ArrayFactory mwCppFactory;
    auto mwCppRet = mwCppFactory.createStructArray({1,1}, {"address", "addressType", "dimensions", "classID"});
    if(mwCppPVec) {
        mwCppRet[0][0]["address"] = mwCppFactory.createScalar(reinterpret_cast<uint64_t>(mwCppPVec));
        mwCppRet[0][0]["addressType"] = mwCppFactory.createScalar(static_cast<uint64_t>(0));
    } else {
        mwCppRet[0][0]["address"] = mwCppFactory.createScalar(reinterpret_cast<uint64_t>(mwCppPRaw));
        mwCppRet[0][0]["addressType"] = mwCppFactory.createScalar(static_cast<uint64_t>(1));
    }
    mwCppRet[0][0]["dimensions"] = mwCppDim;
    mwCppRet[0][0]["classID"] = mwCppFactory.createScalar(mwClassIDInInterface<mwContainerClassForClassID<typename std::remove_cv<mwCppT>::type>>());
    return mwCppRet;
}

template<typename mwCppT>
inline
matlab::data::Array mwCppCreateVectorContainer(std::vector<mwCppT> const *mwCppPVec)
{
    matlab::data::ArrayFactory mwCppFactory;
    auto mwCppDims = mwCppFactory.createArray<int64_t>({1,1});
    mwCppDims[0] = mwCppPVec->size();
    return mwCppCreateContainer(mwCppPVec, static_cast<mwCppT*>(nullptr), mwCppDims);
}

template<typename mwCppT>
inline
matlab::data::Array mwCppCreatePointerContainer(mwCppT const *mwCppPArr, matlab::data::ArrayDimensions const & mwCppDims)
{
    matlab::data::ArrayFactory mwCppFactory;
    auto mwCppDimsArr = mwCppFactory.createArray<int64_t>({1,mwCppDims.size()});
    for(size_t mwCppI=0; mwCppI<mwCppDims.size(); ++mwCppI)
        mwCppDimsArr[mwCppI] = mwCppDims[mwCppI];
    return mwCppCreateContainer(static_cast<std::vector<mwCppT>*>(nullptr), mwCppPArr, mwCppDimsArr);
}

template<typename T, bool isFund = std::is_fundamental<T>::value>
struct mwCppContainerDeleter {
    uint64_t mwCppAdrType;
    uint64_t mwCppAdrVal;

    mwCppContainerDeleter(matlab::data::StructArray &mwCppMdaStr) {
        matlab::data::Array mwCppArrAdrType = mwCppMdaStr[0]["addressType"];
        matlab::data::Array mwCppArrAdrVal = mwCppMdaStr[0]["address"];
        mwCppAdrType = static_cast<uint64_t>(mwCppArrAdrType[0]);
        mwCppAdrVal = static_cast<uint64_t>(mwCppArrAdrVal[0]);
        if(mwCppAdrType==0) {
            auto mwCppPVec = reinterpret_cast<std::vector<T> *>(mwCppAdrVal);
            delete mwCppPVec;
        } else {
            auto pAddress = reinterpret_cast<T*>(mwCppAdrVal);
            delete[] reinterpret_cast<char*>(pAddress);
        }
    }
};

template<typename T>
struct mwCppContainerDeleter<T,false> {
    uint64_t mwCppAdrType;
    uint64_t mwCppAdrVal;

    mwCppContainerDeleter(matlab::data::StructArray &mwCppMdaStr) {
        matlab::data::Array mwCppArrAdrType = mwCppMdaStr[0]["addressType"];
        matlab::data::Array mwCppArrAdrVal = mwCppMdaStr[0]["address"];
        mwCppAdrType = static_cast<uint64_t>(mwCppArrAdrType[0]);
        mwCppAdrVal = static_cast<uint64_t>(mwCppArrAdrVal[0]);
        if(mwCppAdrType==0) {
            auto mwCppPVec = reinterpret_cast<std::vector<T> *>(mwCppAdrVal);
            delete mwCppPVec;
        } else {
            auto pAddress = reinterpret_cast<T*>(mwCppAdrVal);
            matlab::data::TypedArray<int64_t> mdaDim = mwCppMdaStr[0]["dimensions"];
            const auto mwCppNumel = mwProdArray(mdaDim);
            for(size_t i=0; i<mwCppNumel; ++i)
                (pAddress+i)->~T();
            delete[] reinterpret_cast<char*>(pAddress);
        }
    }
};

template<typename mwCppT>
class mwCppContainerWrapper {
    uint64_t mwCppFAdrType;
    uint64_t mwCppFAdrVal;
public:
    mwCppContainerWrapper(const matlab::data::Array &mwCppArr) {
        matlab::data::StructArray mwCppMdaStr(mwCppArr);
        matlab::data::Array mwCppArrAdrType = mwCppMdaStr[0]["addressType"];
        matlab::data::Array mwCppArrAdrVal = mwCppMdaStr[0]["address"];
        mwCppFAdrType = static_cast<uint64_t>(mwCppArrAdrType[0]);
        mwCppFAdrVal = static_cast<uint64_t>(mwCppArrAdrVal[0]);
    }
    mwCppT mwCppGetElemVal(uint64_t mwCppOfs) {
        if(mwCppFAdrType==0) {
            auto mwCppPVec = reinterpret_cast<std::vector<mwCppT>*>(mwCppFAdrVal);
            return (*mwCppPVec)[mwCppOfs];
        }
        else {
            auto mwCppPRaw = reinterpret_cast<mwCppT*>(mwCppFAdrVal);
            return *(mwCppPRaw+mwCppOfs);
        }
    }
    mwCppT *mwCppGetElement(uint64_t mwCppOfs) {
        if(mwCppFAdrType==0) {
            auto mwCppPVec = reinterpret_cast<std::vector<mwCppT>*>(mwCppFAdrVal);
            return &(*mwCppPVec)[mwCppOfs];
        }
        else {
            auto mwCppPRaw = reinterpret_cast<mwCppT*>(mwCppFAdrVal);
            return mwCppPRaw+mwCppOfs;
        }
    }
    void mwCppSetValue(uint64_t mwCppOfs, mwCppT mwCppValue) {
        if(mwCppFAdrType==0) {
            auto mwCppPVec = reinterpret_cast<std::vector<mwCppT>*>(mwCppFAdrVal);
            (*mwCppPVec)[mwCppOfs] = mwCppValue;
        }
        else {
            auto mwCppPRaw = reinterpret_cast<mwCppT*>(mwCppFAdrVal);
            *(mwCppPRaw+mwCppOfs) = mwCppValue;
        }
    }
    void mwCppSetValue(uint64_t mwCppOfs, mwCppT *mwCppValue) {
        if(mwCppFAdrType==0) {
            auto mwCppPVec = reinterpret_cast<std::vector<mwCppT>*>(mwCppFAdrVal);
            (*mwCppPVec)[mwCppOfs] = *mwCppValue;
        }
        else {
            auto mwCppPRaw = reinterpret_cast<mwCppT*>(mwCppFAdrVal);
            *(mwCppPRaw+mwCppOfs) = *mwCppValue;
        }
    }
};

template<typename mwCppT>
void mwCppSetElemVal(const matlab::data::Array &mwCppArr, uint64_t mwCppOfs, mwCppT mwCppVal)
{
    matlab::data::StructArray mwCppMdaStr(mwCppArr);
    matlab::data::Array mwCppArrAdrType = mwCppMdaStr[0]["addressType"];
    matlab::data::Array mwCppArrAdrVal = mwCppMdaStr[0]["address"];
    auto mwCppAdrType{static_cast<uint64_t>(mwCppArrAdrType[0])};
    auto mwCppAdrVal{static_cast<uint64_t>(mwCppArrAdrVal[0])};
    if(mwCppAdrType==0) {
        auto mwCppPVec = reinterpret_cast<std::vector<mwCppT>*>(mwCppAdrVal);
        (*mwCppPVec)[mwCppOfs] = mwCppVal;
    }
    else {
        auto mwCppPRaw = reinterpret_cast<mwCppT*>(mwCppAdrVal);
        *(mwCppPRaw+mwCppOfs) = mwCppVal;
    }
}

//---------------------------------------------------------------------------//
// Function for Exception Dispatch
//
// Clients of Pass-Through functions must provide a function pointer as the
// mwCppErrorBack parameter. The mwCppErrorBack is called in the case of an exception.
// The argument to the error back is the error message, result of what() member
// function of std::exception.
//---------------------------------------------------------------------------//
typedef void (*mwCppErrorBack)(void* mwCppcallbackData, const char *, const char *);
void mwCpphandleError(void* mwCppcallbackData, mwCppErrorBack mwCpperrBack)
{
    try {
        throw;
    } catch (const mwCppInterfaceExceptionForFunctionType & mwCpperr) {
        mwCpperrBack(mwCppcallbackData, "mwCppInterfaceExceptionForFunctionType", mwCpperr.what());
    } catch (const mwCppInterfaceException & mwCpperr) {
        mwCpperrBack(mwCppcallbackData, "mwCppInterfaceException", mwCpperr.what());
    } catch (const matlab::Exception & mwCpperr) {
        mwCpperrBack(mwCppcallbackData, "matlab::Exception", mwCpperr.what());
    } catch (const std::exception & mwCpperr) {
        mwCpperrBack(mwCppcallbackData, "std::exception", mwCpperr.what());
    } catch (...) {
        mwCpperrBack(mwCppcallbackData, "...", "");
    }
}

std::vector<matlab::data::Array> mwCppcreateprhs(int mwCppnrhs, matlab::data::impl::ArrayImpl* mwCpp_prhs[])
{
    std::vector<matlab::data::Array> mwCppprhs;
    mwCppprhs.reserve(mwCppnrhs);
    std::transform(mwCpp_prhs, mwCpp_prhs+mwCppnrhs, std::back_inserter(mwCppprhs),
       [](matlab::data::impl::ArrayImpl* mwCppimpl){
           return matlab::data::detail::Access::createObj<matlab::data::Array>(mwCppimpl);});
    return mwCppprhs;
}

void handleOutput(void* mwCppcallbackData,
    void (*mwCppcallbackPlhs)(void *mwCppcallbackData, int mwCppnlhs, matlab::data::impl::ArrayImpl* mwCppplhs[]),
    std::vector<matlab::data::Array>& mwCppoutput)
{
    const int mwCppnlhs = static_cast<int>(mwCppoutput.size());
    std::vector<matlab::data::impl::ArrayImpl*> mwCppplhs(mwCppnlhs);
    std::transform(mwCppoutput.begin(), mwCppoutput.end(), mwCppplhs.begin(),
        matlab::data::detail::Access::getImpl<matlab::data::impl::ArrayImpl, matlab::data::Array>);
    mwCppcallbackPlhs(mwCppcallbackData, mwCppnlhs, mwCppplhs.data());
}

template<> inline uint32_t mwClassIDInInterface<PowerMeterDLL>() {return 0;}
template<> inline uint32_t mwClassIDInInterface<std::shared_ptr<PowerMeterDLL>>() {return 1;}
template<> inline uint32_t mwClassIDInInterface<std::shared_ptr<const PowerMeterDLL>>() {return 2;}


template<>
PowerMeterDLL *mwCppDynamicCast<PowerMeterDLL*>(uint64_t mwCppadr, uint32_t mwCppclsID)
{
    if (mwCppadr == 0) { return nullptr; }
   switch(mwCppclsID) {
    case 0: return reinterpret_cast<PowerMeterDLL*>(mwCppadr);
    default: throw mwCppInterfaceExceptionAccessViolation();
    }
    return nullptr;
}

template<>
std::shared_ptr<PowerMeterDLL> *mwCppDynamicCast<std::shared_ptr<PowerMeterDLL>*>(uint64_t mwCppadr, uint32_t mwCppclsID)
{
    if (mwCppadr == 0) { return nullptr; }
   switch(mwCppclsID) {
    case 1: return reinterpret_cast<std::shared_ptr<PowerMeterDLL>*>(mwCppadr);
    case 2: return reinterpret_cast<std::shared_ptr<PowerMeterDLL>*>(mwCppadr);
    default: throw mwCppInterfaceExceptionAccessViolation();
    }
    return nullptr;
}

template<>
std::shared_ptr<const PowerMeterDLL> *mwCppDynamicCast<std::shared_ptr<const PowerMeterDLL>*>(uint64_t mwCppadr, uint32_t mwCppclsID)
{
    if (mwCppadr == 0) { return nullptr; }
   switch(mwCppclsID) {
    case 2: return reinterpret_cast<std::shared_ptr<const PowerMeterDLL>*>(mwCppadr);
    case 1: return reinterpret_cast<std::shared_ptr<const PowerMeterDLL>*>(mwCppadr);
    default: throw mwCppInterfaceExceptionAccessViolation();
    }
    return nullptr;
}


template<>
std::shared_ptr<PowerMeterDLL> mwCppDynamicCast<std::shared_ptr<PowerMeterDLL>>(uint64_t mwCppadr, uint32_t mwCppclsID)
{
    if (mwCppadr == 0) { return nullptr; }
    std::shared_ptr<PowerMeterDLL> *mwCppResult(nullptr);
   switch(mwCppclsID) {
    case 0+1: return *reinterpret_cast<std::shared_ptr<PowerMeterDLL>*>(mwCppadr);
    default: throw mwCppInterfaceExceptionAccessViolation();
    }
    return *mwCppResult;
}

template<>
std::shared_ptr<const PowerMeterDLL> mwCppDynamicCast<std::shared_ptr<const PowerMeterDLL>>(uint64_t mwCppadr, uint32_t mwCppclsID)
{
    if (mwCppadr == 0) { return nullptr; }
    std::shared_ptr<const PowerMeterDLL> *mwCppResult(nullptr);
   switch(mwCppclsID) {
    case 0+1: return *reinterpret_cast<std::shared_ptr<PowerMeterDLL>*>(mwCppadr);
    case 0+2: return *reinterpret_cast<std::shared_ptr<const PowerMeterDLL>*>(mwCppadr);
    default: throw mwCppInterfaceExceptionAccessViolation();
    }
    return *mwCppResult;
}

//---------------------------------------------------------------------------//
// Pass-Through functions - access functionality on user's library
//---------------------------------------------------------------------------//

void DPassThrough1(
          void* mwCppPassThroughParameterCallbackData,
          void (*mwCppPassThroughParameterCallbackPlhs)(void* mwCppcallbackData, int mwCppnlhs, matlab::data::impl::ArrayImpl* mwCppplhs[]),
          int mwCppPassThroughParameterNrhs,
          matlab::data::impl::ArrayImpl* mwCppPassThroughParameterPrhsPImpl[],
          void (*mwCppPassThroughParameterCallbackError)(void* mwCppcallbackData, const char* mwCppname, const char* mwCppmsg))
{
    try {
        // Deallocate: ~PowerMeterDLL()
        std::vector<matlab::data::Array> mwCppPassThroughLocalVariablePrhs = mwCppcreateprhs(mwCppPassThroughParameterNrhs, mwCppPassThroughParameterPrhsPImpl);
        std::shared_ptr<PowerMeterDLL> * mwCppPassThroughLocalVariableObj = mwCppGetSharedPointer<std::shared_ptr<PowerMeterDLL>>(mwCppPassThroughLocalVariablePrhs[0]);
        if (mwCppPassThroughLocalVariableObj != NULL) {
            mwCppRemoveFromSet(mwCppPassThroughLocalVariableObj->get());
            delete mwCppPassThroughLocalVariableObj;
        }
        else {
            PowerMeterDLL * mwCppPassThroughLocalVariableObj = mwCppGetPointer<PowerMeterDLL>(mwCppPassThroughLocalVariablePrhs[0]);
            delete mwCppPassThroughLocalVariableObj;
        }
    } catch (...) {
        mwCpphandleError(mwCppPassThroughParameterCallbackData, mwCppPassThroughParameterCallbackError);
    }
}

void APassThrough1(
          void* mwCppPassThroughParameterCallbackData,
          void (*mwCppPassThroughParameterCallbackPlhs)(void* mwCppcallbackData, int mwCppnlhs, matlab::data::impl::ArrayImpl* mwCppplhs[]),
          int mwCppPassThroughParameterNrhs,
          matlab::data::impl::ArrayImpl* mwCppPassThroughParameterPrhsPImpl[],
          void (*mwCppPassThroughParameterCallbackError)(void* mwCppcallbackData, const char* mwCppname, const char* mwCppmsg))
{
    // Invoke: new PowerMeterDLL()
    try {
        std::vector<matlab::data::Array> mwCppPassThroughLocalVariablePrhs = mwCppcreateprhs(mwCppPassThroughParameterNrhs, mwCppPassThroughParameterPrhsPImpl);
        switch (mwCppPassThroughParameterNrhs) {
            case 0: {
                PowerMeterDLL *mwCppPassThroughLocalVariableRetVal = new PowerMeterDLL();
                std::vector<matlab::data::Array> mwCppPassThroughLocalVariablePlhs;
                std::shared_ptr<PowerMeterDLL> *mwCppPassThroughLocalVariableRetValShared = new std::shared_ptr<PowerMeterDLL>(mwCppPassThroughLocalVariableRetVal);
                mwCppPassThroughLocalVariablePlhs.push_back(mwCppCreateStructFromPointer(mwCppPassThroughLocalVariableRetVal, mwCppPassThroughLocalVariableRetValShared));
                handleOutput(mwCppPassThroughParameterCallbackData, mwCppPassThroughParameterCallbackPlhs, mwCppPassThroughLocalVariablePlhs);
                break;
            }
            default: {
                // error case - mwCppnrhs was not an expected value
                break;
            }
        }
    } catch (...) {
        mwCpphandleError(mwCppPassThroughParameterCallbackData, mwCppPassThroughParameterCallbackError);
    }
}

void MFPassThrough1(
          void* mwCppPassThroughParameterCallbackData,
          void (*mwCppPassThroughParameterCallbackPlhs)(void* mwCppcallbackData, int mwCppnlhs, matlab::data::impl::ArrayImpl* mwCppplhs[]),
          int mwCppPassThroughParameterNrhs,
          matlab::data::impl::ArrayImpl* mwCppPassThroughParameterPrhsPImpl[],
          void (*mwCppPassThroughParameterCallbackError)(void* mwCppcallbackData, const char* mwCppname, const char* mwCppmsg))
{
    // Invoke: mwCppPassThroughLocalVariableObj->Open(std::wstring const &, unsigned long)
    try {
        std::vector<matlab::data::Array> mwCppPassThroughLocalVariablePrhs = mwCppcreateprhs(mwCppPassThroughParameterNrhs, mwCppPassThroughParameterPrhsPImpl);
        switch (mwCppPassThroughParameterNrhs) {
            case 3: {
                PowerMeterDLL *mwCppPassThroughLocalVariableObj = mwCppGetPointer<PowerMeterDLL>(mwCppPassThroughLocalVariablePrhs[0]);
                std::wstring const mwCppParameter1 = mwCppGetScalarValue<std::wstring>(mwCppPassThroughLocalVariablePrhs[1]);
                unsigned long mwCppParameter2 = mwCppGetScalarValue<unsigned long>(mwCppPassThroughLocalVariablePrhs[2]);
                bool mwCppParameter0 = mwCppPassThroughLocalVariableObj->Open(mwCppParameter1, mwCppParameter2);
                std::vector<matlab::data::Array> mwCppPassThroughLocalVariablePlhs;
                mwCppPassThroughLocalVariablePlhs.push_back(mwCppCreateArray(mwCppParameter0));
                handleOutput(mwCppPassThroughParameterCallbackData, mwCppPassThroughParameterCallbackPlhs, mwCppPassThroughLocalVariablePlhs);
                break;
            }
            default: {
                // error case - mwCppnrhs was not an expected value
                break;
            }
        }
    } catch (...) {
        mwCpphandleError(mwCppPassThroughParameterCallbackData, mwCppPassThroughParameterCallbackError);
    }
}

void MFPassThrough2(
          void* mwCppPassThroughParameterCallbackData,
          void (*mwCppPassThroughParameterCallbackPlhs)(void* mwCppcallbackData, int mwCppnlhs, matlab::data::impl::ArrayImpl* mwCppplhs[]),
          int mwCppPassThroughParameterNrhs,
          matlab::data::impl::ArrayImpl* mwCppPassThroughParameterPrhsPImpl[],
          void (*mwCppPassThroughParameterCallbackError)(void* mwCppcallbackData, const char* mwCppname, const char* mwCppmsg))
{
    // Invoke: mwCppPassThroughLocalVariableObj->IsOpen()
    try {
        std::vector<matlab::data::Array> mwCppPassThroughLocalVariablePrhs = mwCppcreateprhs(mwCppPassThroughParameterNrhs, mwCppPassThroughParameterPrhsPImpl);
        switch (mwCppPassThroughParameterNrhs) {
            case 1: {
                PowerMeterDLL *mwCppPassThroughLocalVariableObj = mwCppGetPointer<PowerMeterDLL>(mwCppPassThroughLocalVariablePrhs[0]);
                bool mwCppParameter0 = mwCppPassThroughLocalVariableObj->IsOpen();
                std::vector<matlab::data::Array> mwCppPassThroughLocalVariablePlhs;
                mwCppPassThroughLocalVariablePlhs.push_back(mwCppCreateArray(mwCppParameter0));
                handleOutput(mwCppPassThroughParameterCallbackData, mwCppPassThroughParameterCallbackPlhs, mwCppPassThroughLocalVariablePlhs);
                break;
            }
            default: {
                // error case - mwCppnrhs was not an expected value
                break;
            }
        }
    } catch (...) {
        mwCpphandleError(mwCppPassThroughParameterCallbackData, mwCppPassThroughParameterCallbackError);
    }
}

void MFPassThrough3(
          void* mwCppPassThroughParameterCallbackData,
          void (*mwCppPassThroughParameterCallbackPlhs)(void* mwCppcallbackData, int mwCppnlhs, matlab::data::impl::ArrayImpl* mwCppplhs[]),
          int mwCppPassThroughParameterNrhs,
          matlab::data::impl::ArrayImpl* mwCppPassThroughParameterPrhsPImpl[],
          void (*mwCppPassThroughParameterCallbackError)(void* mwCppcallbackData, const char* mwCppname, const char* mwCppmsg))
{
    // Invoke: mwCppPassThroughLocalVariableObj->Close()
    try {
        std::vector<matlab::data::Array> mwCppPassThroughLocalVariablePrhs = mwCppcreateprhs(mwCppPassThroughParameterNrhs, mwCppPassThroughParameterPrhsPImpl);
        switch (mwCppPassThroughParameterNrhs) {
            case 1: {
                PowerMeterDLL *mwCppPassThroughLocalVariableObj = mwCppGetPointer<PowerMeterDLL>(mwCppPassThroughLocalVariablePrhs[0]);
                mwCppPassThroughLocalVariableObj->Close();
                break;
            }
            default: {
                // error case - mwCppnrhs was not an expected value
                break;
            }
        }
    } catch (...) {
        mwCpphandleError(mwCppPassThroughParameterCallbackData, mwCppPassThroughParameterCallbackError);
    }
}

void MFPassThrough4(
          void* mwCppPassThroughParameterCallbackData,
          void (*mwCppPassThroughParameterCallbackPlhs)(void* mwCppcallbackData, int mwCppnlhs, matlab::data::impl::ArrayImpl* mwCppplhs[]),
          int mwCppPassThroughParameterNrhs,
          matlab::data::impl::ArrayImpl* mwCppPassThroughParameterPrhsPImpl[],
          void (*mwCppPassThroughParameterCallbackError)(void* mwCppcallbackData, const char* mwCppname, const char* mwCppmsg))
{
    // Invoke: mwCppPassThroughLocalVariableObj->Write(std::string const &)
    try {
        std::vector<matlab::data::Array> mwCppPassThroughLocalVariablePrhs = mwCppcreateprhs(mwCppPassThroughParameterNrhs, mwCppPassThroughParameterPrhsPImpl);
        switch (mwCppPassThroughParameterNrhs) {
            case 2: {
                PowerMeterDLL *mwCppPassThroughLocalVariableObj = mwCppGetPointer<PowerMeterDLL>(mwCppPassThroughLocalVariablePrhs[0]);
                std::string const mwCppParameter1 = mwCppGetScalarValue<std::string>(mwCppPassThroughLocalVariablePrhs[1]);
                bool mwCppParameter0 = mwCppPassThroughLocalVariableObj->Write(mwCppParameter1);
                std::vector<matlab::data::Array> mwCppPassThroughLocalVariablePlhs;
                mwCppPassThroughLocalVariablePlhs.push_back(mwCppCreateArray(mwCppParameter0));
                handleOutput(mwCppPassThroughParameterCallbackData, mwCppPassThroughParameterCallbackPlhs, mwCppPassThroughLocalVariablePlhs);
                break;
            }
            default: {
                // error case - mwCppnrhs was not an expected value
                break;
            }
        }
    } catch (...) {
        mwCpphandleError(mwCppPassThroughParameterCallbackData, mwCppPassThroughParameterCallbackError);
    }
}

void MFPassThrough5(
          void* mwCppPassThroughParameterCallbackData,
          void (*mwCppPassThroughParameterCallbackPlhs)(void* mwCppcallbackData, int mwCppnlhs, matlab::data::impl::ArrayImpl* mwCppplhs[]),
          int mwCppPassThroughParameterNrhs,
          matlab::data::impl::ArrayImpl* mwCppPassThroughParameterPrhsPImpl[],
          void (*mwCppPassThroughParameterCallbackError)(void* mwCppcallbackData, const char* mwCppname, const char* mwCppmsg))
{
    // Invoke: mwCppPassThroughLocalVariableObj->GetValue()
    try {
        std::vector<matlab::data::Array> mwCppPassThroughLocalVariablePrhs = mwCppcreateprhs(mwCppPassThroughParameterNrhs, mwCppPassThroughParameterPrhsPImpl);
        switch (mwCppPassThroughParameterNrhs) {
            case 1: {
                PowerMeterDLL *mwCppPassThroughLocalVariableObj = mwCppGetPointer<PowerMeterDLL>(mwCppPassThroughLocalVariablePrhs[0]);
                double mwCppParameter0 = mwCppPassThroughLocalVariableObj->GetValue();
                std::vector<matlab::data::Array> mwCppPassThroughLocalVariablePlhs;
                mwCppPassThroughLocalVariablePlhs.push_back(mwCppCreateArray(mwCppParameter0));
                handleOutput(mwCppPassThroughParameterCallbackData, mwCppPassThroughParameterCallbackPlhs, mwCppPassThroughLocalVariablePlhs);
                break;
            }
            default: {
                // error case - mwCppnrhs was not an expected value
                break;
            }
        }
    } catch (...) {
        mwCpphandleError(mwCppPassThroughParameterCallbackData, mwCppPassThroughParameterCallbackError);
    }
}

void APassThrough2(
          void* mwCppPassThroughParameterCallbackData,
          void (*mwCppPassThroughParameterCallbackPlhs)(void* mwCppcallbackData, int mwCppnlhs, matlab::data::impl::ArrayImpl* mwCppplhs[]),
          int mwCppPassThroughParameterNrhs,
          matlab::data::impl::ArrayImpl* mwCppPassThroughParameterPrhsPImpl[],
          void (*mwCppPassThroughParameterCallbackError)(void* mwCppcallbackData, const char* mwCppname, const char* mwCppmsg))
{
    // Invoke: new PowerMeterDLL(PowerMeterDLL const &)
    try {
        std::vector<matlab::data::Array> mwCppPassThroughLocalVariablePrhs = mwCppcreateprhs(mwCppPassThroughParameterNrhs, mwCppPassThroughParameterPrhsPImpl);
        switch (mwCppPassThroughParameterNrhs) {
            case 1: {
                PowerMeterDLL const &mwCppParameter1 = *mwCppGetPointer<PowerMeterDLL>(mwCppPassThroughLocalVariablePrhs[0]);
                PowerMeterDLL *mwCppPassThroughLocalVariableRetVal = new PowerMeterDLL(mwCppParameter1);
                std::vector<matlab::data::Array> mwCppPassThroughLocalVariablePlhs;
                std::shared_ptr<PowerMeterDLL> *mwCppPassThroughLocalVariableRetValShared = new std::shared_ptr<PowerMeterDLL>(mwCppPassThroughLocalVariableRetVal);
                mwCppPassThroughLocalVariablePlhs.push_back(mwCppCreateStructFromPointer(mwCppPassThroughLocalVariableRetVal, mwCppPassThroughLocalVariableRetValShared));
                handleOutput(mwCppPassThroughParameterCallbackData, mwCppPassThroughParameterCallbackPlhs, mwCppPassThroughLocalVariablePlhs);
                break;
            }
            default: {
                // error case - mwCppnrhs was not an expected value
                break;
            }
        }
    } catch (...) {
        mwCpphandleError(mwCppPassThroughParameterCallbackData, mwCppPassThroughParameterCallbackError);
    }
}

typedef void (*mwCppcppPassThroughPtr)(
          void* mwCppcallbackData,
          void (*mwCppcallbackPlhs)(void* mwCppcallbackData, int mwCppnlhs, matlab::data::impl::ArrayImpl* mwCppplhs[]),
          int mwCppnrhs,
          matlab::data::impl::ArrayImpl* mwCpp_prhs[],
          void (*mwCppcallbackError)(void* mwCppcallbackData, const char* mwCppname, const char* mwCppmsg));
} // end anonymous namespace

//---------------------------------------------------------------------------//
// entry-point
//
// The entry-point is used by clients to get Pass-Through functions. With the
// Pass-Through function, the clients can access units of functionality. The
// units of functionality included invoking functions, constructing object,
// accessing data members, invoking member functions, etc. A mwCppkey is used to get
// Pass-Through function. The mwCppkey is unique to a unit of functionality and
// it is derived from the meta-data about the functionality, such as function
// name and signature.
//---------------------------------------------------------------------------//
extern "C" MATLAB_CPP_INTERFACE_API
mwCppcppPassThroughPtr cppGetPassThrough(const char * mwCppkey)
{
    mwCppcppPassThroughPtr mwCpphandle = 0;
    try {
        static const std::map<std::string, mwCppcppPassThroughPtr> mwCpppassThroughMap{
            {"D/PowerMeterDLL", DPassThrough1 },
            {"A/PowerMeterDLL", APassThrough1 },
            {"MF/PowerMeterDLL/Open@[std::wstring]ConstRef,[unsigned long]#[bool]", MFPassThrough1 },
            {"MF/PowerMeterDLL/IsOpen#[bool]", MFPassThrough2 },
            {"MF/PowerMeterDLL/Close#[void]", MFPassThrough3 },
            {"MF/PowerMeterDLL/Write@[std::string]ConstRef#[bool]", MFPassThrough4 },
            {"MF/PowerMeterDLL/GetValue#[double]", MFPassThrough5 },
            {"A/PowerMeterDLL@[PowerMeterDLL]ConstRef", APassThrough2 },
        };
        mwCpphandle = mwCpppassThroughMap.at(mwCppkey);
    }
    catch (...)
    { }
    return mwCpphandle;
}
