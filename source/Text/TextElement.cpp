#include "OpenGUI/Text/TextElement.h"
#include "OpenGUI/Core/open_string.h"

namespace OGUI
{
    struct TextShape
    {
         
    };

    struct TextShaper
    {
        virtual bool ShapeText(ostr::string_view text, TextShape& shape) = 0;
    };

    struct HarfBuzzShaper : TextShaper
    {
        bool ShapeText(ostr::string_view text, TextShape& shape) override;
    };

    struct TextRun : TextShape
    {

    };

    struct TextRunsBuilder
    {
        void Build(TextElement* element);

    };
}