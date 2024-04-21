#define TBS_RESULT_TYPE const byte*

#define TBS_CONTAINER_MAX_SIZE 256

#ifdef _WIN32
#define FINLINE __forceinline 
#else 
#define FINLINE inline
#endif

#include <pattern_entry.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <TBS.hpp>

using namespace TBS;

void TBSCacheFlush(State<>& state)
{
    state.mSharedDescriptions.clear();
}

struct TBSScanner : pattern_scanner
{
    TBSScanner()
        : sharedDesc(Pattern::EScan::SCAN_ALL)
    {}

    FINLINE std::vector<const byte*> _Scan(
        const byte* pattern, const char* mask, const byte* data, size_t length) // Removing Const ID
    {
        sharedDesc.mResult.clear();
        Pattern::Description patternDesc(sharedDesc, "_", data, data + length, emptyResTf, pattern, mask);

        while (Pattern::Scan(patternDesc))
            (void) 0;

        return patternDesc.mShared.mResult;
    }

    virtual std::vector<const byte*> Scan(
        const byte* pattern, const char* mask, const byte* data, size_t length) const override
    {
        return const_cast<TBSScanner*>(this)->_Scan(pattern, mask, data, length);
    }

    virtual const char* GetName() const override
    {
        return "TBS";
    }

    State<> state;
    TBS::Vector<Pattern::ResultTransformer> emptyResTf;
    Pattern::SharedDescription sharedDesc;
};

REGISTER_PATTERN(TBSScanner);