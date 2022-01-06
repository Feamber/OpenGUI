#include "OpenGUI/Style2/Lerp/TextLerp.h"
#include "OpenGUI/Style2/Lerp/MathLerp.h"
#include "OpenGUI/Style2/Lerp/CommonLerp.h"
#include "OpenGUI/Style2/Shadow.h"

namespace OGUI
{
    TextShadow Lerp(const TextShadow& a, const TextShadow& b, float alpha)
    {
        TextShadow result;
        result.offset = Lerp(a.offset, b.offset, alpha);
        result.color = Lerp(a.color, b.color, alpha);
        return result;
    }

    std::vector<TextShadow> Lerp(gsl::span<const TextShadow> a, gsl::span<const TextShadow> b, float alpha)
    {
        return LerpArray<TextShadow>(a, b, alpha);
    }

    
    std::vector<ostr::string> Lerp(gsl::span<const ostr::string> a, gsl::span<const ostr::string> b, float alpha)
    {
        return LerpArray<ostr::string>(a, b, alpha);
    }
}