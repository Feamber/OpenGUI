#include <limits>

#include "OpenGUI/Core/Math/Vector.h"
#include "OpenGUI/Core/PrimitiveDraw.h"
#include "OpenGUI/Core/nanovg.h"
#include "OpenGUI/Interface/Interfaces.h"
#include "OpenGUI/Style2/Transform.h"

uint32_t ToColor32ABGR(NVGcolor color)
{
    color.r = std::clamp(color.r, 0.f, 1.f);   
    color.g = std::clamp(color.g, 0.f, 1.f);   
    color.b = std::clamp(color.b, 0.f, 1.f);   
    color.a = std::clamp(color.a, 0.f, 1.f);   
    uint8_t r = color.r * 255;
    uint8_t g = color.g * 255;
    uint8_t b = color.b * 255;
    uint8_t a = color.a * 255;
    return ((uint32_t)a << 24) + ((uint32_t)b << 16) + ((uint32_t)g << 8) + (uint32_t)r;
}

static void nvg__xformIdentity(float* t)
{
	t[0] = 1.0f; t[1] = 0.0f;
	t[2] = 0.0f; t[3] = 1.0f;
	t[4] = 0.0f; t[5] = 0.0f;
}

static void nvg__xformInverse(float* inv, float* t)
{
	double invdet, det = (double)t[0] * t[3] - (double)t[2] * t[1];
	if (det > -1e-6 && det < 1e-6) {
		nvg__xformIdentity(t);
		return;
	}
	invdet = 1.0 / det;
	inv[0] = (float)(t[3] * invdet);
	inv[2] = (float)(-t[2] * invdet);
	inv[4] = (float)(((double)t[2] * t[5] - (double)t[3] * t[4]) * invdet);
	inv[1] = (float)(-t[1] * invdet);
	inv[3] = (float)(t[0] * invdet);
	inv[5] = (float)(((double)t[1] * t[4] - (double)t[0] * t[5]) * invdet);
}

static OGUI::Vector2f nvg__remapUV(OGUI::Vector2f is, OGUI::Vector2f size, const NVGbox& box)
{
    OGUI::Vector2f result;
    if(box.extend[0] == 0.f || box.extend[1] == 0.f)
        return is / size;

    float marginLeft;
    float marginRight;
    float marginTop;
    float marginBottom;
    {
        if(is.y < box.margin.bottom)
            marginLeft = box.margin.left + box.radius[3];
        else if(is.y < box.margin.bottom + box.radius[3])
        {
            auto off = box.margin.bottom + box.radius[3] - is.y;
            off = off*off + box.radius[3]*box.radius[3];
            marginLeft = box.margin.left + box.radius[3] - std::sqrt(off);
        }
        else if(is.y > (size.y - box.margin.top))
            marginLeft = box.margin.left + box.radius[0];
        else if(is.y > (size.y - box.margin.top) - box.radius[0])
        {
            auto off = is.y - (size.y - box.margin.top) + box.radius[0];
            off = off*off + box.radius[0]*box.radius[0];
            marginLeft = box.margin.left + box.radius[0] - std::sqrt(off);
        }
        else 
            marginLeft = box.margin.left;
        if(is.y < box.margin.bottom)
            marginRight = box.margin.right + box.radius[2];
        else if(is.y < box.margin.bottom + box.radius[2])
        {
            auto off = box.margin.bottom + box.radius[2] - is.y;
            off = off*off + box.radius[2]*box.radius[2];
            marginRight = box.margin.right + box.radius[2] - std::sqrt(off);
        }
        else if(is.y > (size.y - box.margin.top))
            marginRight = box.margin.right + box.radius[1];
        else if(is.y > (size.y - box.margin.top) - box.radius[1])
        {
            auto off = is.y - (size.y - box.margin.top) + box.radius[1];
            off = off*off + box.radius[1]*box.radius[1];
            marginRight = box.margin.right + box.radius[1] - std::sqrt(off);
        }
        else 
            marginRight = box.margin.right;
        if(is.x < box.margin.left)
            marginTop = box.margin.top + box.radius[0];
        else if(is.x < box.margin.left + box.radius[0])
        {
            auto off = box.margin.left + box.radius[0] - is.x;
            off = off*off + box.radius[0]*box.radius[0];
            marginTop = box.margin.top + box.radius[0] - std::sqrt(off);
        }
        else if(is.x > (size.x - box.margin.right))
            marginTop = box.margin.right + box.radius[1];
        else if(is.x > (size.x - box.margin.right) - box.radius[1])
        {
            auto off = is.x - (size.x - box.margin.right) + box.radius[1];
            off = off*off + box.radius[1]*box.radius[1];
            marginTop = box.margin.top + box.radius[1] - std::sqrt(off);
        }
        else 
            marginTop = box.margin.top;
        if(is.x < box.margin.left)
            marginBottom = box.margin.bottom + box.radius[3];
        else if(is.x < box.margin.left + box.radius[3])
        {
            auto off = box.margin.left + box.radius[3] - is.x;
            off = off*off + box.radius[3]*box.radius[3];
            marginBottom = box.margin.bottom + box.radius[3] - std::sqrt(off);
        }
        else if(is.x > (size.x - box.margin.right))
            marginBottom = box.margin.right + box.radius[2];
        else if(is.x > (size.x - box.margin.right) - box.radius[2])
        {
            auto off = is.x - (size.x - box.margin.right) + box.radius[2];
            off = off*off + box.radius[2]*box.radius[2];
            marginBottom = box.margin.bottom + box.radius[2] - std::sqrt(off);
        }
        else 
            marginBottom = box.margin.bottom;
    }

    if(is.x < marginLeft)
    {
        result.x = is.x / box.extend[0];
    }
    else if((size.x - is.x) < marginRight)
    {
        result.x = 1 - (size.x - is.x) / box.extend[0];
    }
    else 
    {
        auto alpha = (is.x - marginLeft) / (size.x - marginLeft - marginRight);
        result.x = (marginLeft / box.extend[0]) * alpha + (1 - marginRight / box.extend[0]) * (1-alpha);
    }

    if(is.y < marginBottom)
    {
        result.y = is.y / box.extend[1];
    }
    else if((size.y - is.y) < marginTop)
    {
        result.y = 1 - (size.y - is.y) / box.extend[1];
    }
    else 
    {
        auto alpha = (is.y - marginBottom) / (size.y - marginBottom - marginTop);
        result.y = (marginBottom / box.extend[1]) * alpha + (1 - marginTop / box.extend[1]) * (1-alpha);
    }
    return result;
}

static OGUI::ComputedTransform nvg__getMatrix(NVGpaint* paint)
{
    float invxform[6];
    nvg__xformInverse(invxform, paint->xform);
    return {invxform, {invxform[4], invxform[5]}};
}

static void nvg__renderPath(OGUI::PrimDrawContext* dc, const NVGpath& path, NVGpaint* paint, const OGUI::ComputedTransform& transform, float fringe)
{
    using namespace OGUI;
    Vector2f extend{paint->extent[0], paint->extent[1]};
    auto& vertices = dc->current->vertices;
    auto& indices = dc->current->indices;
    auto push_vertex = [&](const NVGvertex& nv)
    {
        Vertex v;
        v.position = {nv.x, nv.y};
        v.aa = {nv.u, fringe};
        auto imgSpace = multiply(transform, v.position);
        v.texcoord = nvg__remapUV(imgSpace, extend, paint->box);
        v.color = ToColor32ABGR(paint->innerColor);
        vertices.push_back(v);
    };
    //auto& path = paths[i];
    if(path.nfill)
    {
        vertices.reserve(vertices.size() + path.nfill);
        indices.reserve(indices.size() + path.nfill * 3);
        size_t start = vertices.size();
        for(int j=0; j<path.nfill; ++j)
        {
            push_vertex(path.fill[j]);
            if(j<path.nfill-2)
            {
                size_t id = vertices.size();
                indices.push_back(start);
                indices.push_back(id + 1);
                indices.push_back(id);
            }
        }
    }
    if(path.nstroke)
    {
        vertices.reserve(vertices.size() + path.nstroke);
        indices.reserve(indices.size() + path.nstroke * 3);
        for(int j=0; j<path.nstroke; ++j)
        {
            push_vertex(path.stroke[j]);
            if(j<path.nstroke-2)
            {
                size_t id = vertices.size() - 1;
                indices.push_back(id);
                indices.push_back(id + 1 + (j % 2));
                indices.push_back(id + 1 + !(j % 2));
            }
        }
    }
}

static void nvg__renderFill(void* uptr, NVGpaint* paint, NVGcompositeOperationState compositeOperation, float fringe,
							  const float* bounds, const NVGpath* paths, int npaths)
{
    using namespace OGUI;
    auto dc = (PrimDrawContext*)uptr;
    PrimDrawResource resource;
    resource.texture = (TextureHandle)paint->image;
    resource.material = (MaterialHandle)paint->material;
    resource.compositeOperation = compositeOperation;
    resource.noGamma = paint->noGamma;
    auto invTransform = nvg__getMatrix(paint);
    //fast path
    if(npaths == 1 && paths[0].convex) 
    {
        auto& command = dc->current->GetCommand(resource);
        auto begin = dc->current->indices.size();
        for(int i=0; i<npaths; ++i)
            nvg__renderPath(dc, paths[i], paint, invTransform, 1.f);
        command.element_count += dc->current->indices.size()-begin;
    }
    //slow path
    else 
    {
    }
}


static void nvg__renderStroke(void* uptr, NVGpaint* paint, NVGcompositeOperationState compositeOperation, float fringe,
								float strokeWidth, const NVGpath* paths, int npaths)
{
    using namespace OGUI;
    auto dc = (PrimDrawContext*)uptr;
    PrimDrawResource resource;
    resource.texture = (TextureHandle)paint->image;
    resource.material = (MaterialHandle)paint->material;
    resource.compositeOperation = compositeOperation;
    resource.noGamma = paint->noGamma;
    auto invTransform = nvg__getMatrix(paint);
    //fast path
    auto& command = dc->current->GetCommand(resource);
    auto& indices = dc->current->indices;
    auto begin = indices.size();
    float aa = (fringe*0.5f + strokeWidth*0.5f) / fringe;
    for(int i=0; i<npaths; ++i)
        nvg__renderPath(dc, paths[i], paint, invTransform, aa);
    command.element_count += indices.size()-begin;
}

namespace OGUI
{

    const Vector2f Transform(Vector2f p, const float4x4& transform)
    {
        const Vector4f dummy = Vector4f(p.X, p.Y, 0.f, 1.f);
        const Vector4f result = math::multiply(dummy, transform);
        p.X = result.X;
        p.Y = result.Y;
        return p;
    }

    void PrimDrawContext::BeginDraw()
    {
        PrimDrawList& list = *current;
        list.beginCount = list.vertices.size();
    }

    void PrimDrawContext::EndDraw(
        const float4x4& inTransform)
    {
        PrimDrawList& list = *current;
        int count = list.vertices.size();
        float4x4 transform = inTransform;
        if(inverseTransform)
            transform = math::multiply(transform, *inverseTransform);
        for (int i = list.beginCount; i < count; ++i)
        {
            auto& vertex = list.vertices[i];
            vertex.position = Transform(vertex.position, transform);
            if(!clipStack.empty())
            {
                auto& back = clipStack.back();
                vertex.clipUV = Transform(vertex.position, back);
                if(clipStack.size() > 1)
                {
                    auto& back2 = clipStack[clipStack.size() - 2];
                    vertex.clipUV2 = Transform(vertex.position, back2);
                }
            }
            else 
            {
                vertex.clipUV = Vector2f::vector_zero();
            }
        }
    }

    PrimDrawCommand& PrimDrawList::GetCommand(const PrimDrawResource& resource)
    {
        if(!command_list.empty()) 
        {
            auto& res = command_list.back().resource;
            if(res.texture == resource.texture && res.material == resource.material &&
            res.compositeOperation.dstAlpha == resource.compositeOperation.dstAlpha &&
            res.noGamma == resource.noGamma &&
            res.compositeOperation.dstRGB == resource.compositeOperation.dstRGB &&
            res.compositeOperation.srcAlpha == resource.compositeOperation.srcAlpha &&
            res.compositeOperation.srcRGB == resource.compositeOperation.srcRGB)
            {
                return command_list.back();
            }
        }
        auto& result = command_list.emplace_back();
        result.resource = resource;
        result.index_offset = indices.size();
        result.element_count = 0;
        return result;
    }

    PrimDrawContext::PrimDrawContext(WindowContext& wctx)
        :wctx(wctx)
    {
        NVGparams params;

        memset(&params, 0, sizeof(params));
        params.renderFill = nvg__renderFill;
        params.renderStroke = nvg__renderStroke;
        params.userPtr = this;
        params.edgeAntiAlias = true;

        nvg = nvgCreateInternal(&params);
    }

    PrimDrawContext::~PrimDrawContext()
    {
        nvgDeleteInternal(nvg);
    }
}