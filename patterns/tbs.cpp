#define TBS_RESULT_TYPE const byte*

#include <pattern_entry.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <TBS.hpp>

using namespace TBS;

std::string PatternStringFromPatternMask(const byte* pattern, const char* mask)
{
    auto len = strlen(mask);
    std::string result;
                                // 3 Caracters per byte maximum
                                // 'XX ' ...
    result.reserve(len * 3);    //  123  ...

    for (size_t i = 0; i < len; i++)
    {
        if (mask[i] != '?')
        {
            char _byte[3] {0};
            snprintf(_byte, sizeof(_byte), "%02X", pattern[i]);
            result += std::string(_byte);
        }
        else
            result += '?';

        if (i + 1 < len)
            result += ' ';
    }

    return result;
}

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
        std::string fullPattern = PatternStringFromPatternMask(pattern, mask);

        Pattern::Description patternDesc = 
            templateBuilder
            .Clone()
            .setPattern(fullPattern)
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