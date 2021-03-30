#include "OpenGUI/Managers/RenderTextureManager.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Core/AsyncBitmap.h"

namespace OGUI
{
using namespace std;

shared_ptr<TextureInterface> RenderTextureManager::Require(
    const char* url, bool sync, shared_ptr<AsyncBitmap>* bmOut)
{

    return nullptr;
}

}