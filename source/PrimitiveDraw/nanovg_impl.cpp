#include "OpenGUI/Context.h"
#include "OpenGUI/Core/PrimitiveDraw.h"
#include "OpenGUI/Core/nanovg.h"

enum NVGCreateFlags {
	// Flag indicating if geometry based anti-aliasing is used (may not be needed when using MSAA).
	NVG_ANTIALIAS 		= 1<<0,
	// Flag indicating if strokes should be drawn using stencil buffer. The rendering will be a little
	// slower, but path overlaps (i.e. self-intersecting or sharp turns) will be drawn just once.
	NVG_STENCIL_STROKES	= 1<<1,
	// Flag indicating that additional debug checks are done.
	NVG_DEBUG 			= 1<<2,
};


static void nvg__renderFill(void* uptr, NVGpaint* paint, NVGcompositeOperationState compositeOperation, NVGscissor* scissor, float fringe,
							  const float* bounds, const NVGpath* paths, int npaths)
{
    
}


static void nvg__renderStroke(void* uptr, NVGpaint* paint, NVGcompositeOperationState compositeOperation, NVGscissor* scissor, float fringe,
								float strokeWidth, const NVGpath* paths, int npaths)
{

}

static void nvg__renderTriangles(void* uptr, NVGpaint* paint, NVGcompositeOperationState compositeOperation, NVGscissor* scissor,
								   const NVGvertex* verts, int nverts, float fringe)
{
}

NVGcontext* NVGCreate(int flags, OGUI::PrimitiveDraw::DrawContext* dc)
{
    NVGparams params;
	NVGcontext* ctx = NULL;

	memset(&params, 0, sizeof(params));
	params.renderFill = nvg__renderFill;
	params.renderStroke = nvg__renderStroke;
	params.renderTriangles = nvg__renderTriangles;
	params.userPtr = &dc;
	params.edgeAntiAlias = flags & NVG_ANTIALIAS ? 1 : 0;

	ctx = nvgCreateInternal(&params);
	if (ctx == NULL) goto error;

	return ctx;

error:
	if (ctx != NULL) nvgDeleteInternal(ctx);
	return NULL;
}