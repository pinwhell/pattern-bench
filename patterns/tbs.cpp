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

Pattern::Results res;

struct TBSScanner : pattern_scanner
{
    TBSScanner()
        : sharedDesc(Pattern::EScan::SCAN_ALL)
    {
        res.reserve(10000);
    }

    virtual std::vector<const byte*> Scan(
        const byte* pattern, const char* mask, const byte* data, size_t length) const override
    {
        res.clear();
        Light::Scan(data, data + length, res, pattern, mask);
        return res;
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