#define TBS_RESULT_TYPE const byte*

#include <pattern_entry.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <TBS.hpp>

using namespace TBS;

void TBSCacheFlush(State& state)
{
    state.mSharedDescriptions.clear();
}

struct TBSScanner : pattern_scanner
{
    TBSScanner()
        : templateBuilder(state
            .PatternBuilder()
            .setUID("UID").setScanType(Pattern::EScan::SCAN_ALL))
    {}

    virtual std::vector<const byte*> Scan(
        const byte* pattern, const char* mask, const byte* data, size_t length) const override
    {
        TBSCacheFlush(const_cast<State&>(state));

        Pattern::Description patternDesc = 
            templateBuilder
            .Clone()
            .setPatternRaw(pattern)
            .setMask(mask)
            .setScanStart(data)
            .setScanEnd(data + length)
            .Build();

        while (Pattern::Scan(patternDesc))
            (void) 0;

        return patternDesc.mShared.mResult;
    }

    virtual const char* GetName() const override
    {
        return "TBS";
    }

    State state;
    Pattern::DescriptionBuilder templateBuilder;
};

REGISTER_PATTERN(TBSScanner);