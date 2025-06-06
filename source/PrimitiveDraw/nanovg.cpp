//
// Copyright (c) 2013 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include "OpenGUI/Core/nanovg.h"

#ifdef _MSC_VER
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4127)  // conditional expression is constant
#pragma warning(disable: 4204)  // nonstandard extension used : non-constant aggregate initializer
#pragma warning(disable: 4706)  // assignment within conditional expression
#endif

#define NVG_INIT_FONTIMAGE_SIZE  512
#define NVG_MAX_FONTIMAGE_SIZE   2048
#define NVG_MAX_FONTIMAGES       4

#define NVG_INIT_COMMANDS_SIZE 256
#define NVG_INIT_POINTS_SIZE 128
#define NVG_INIT_PATHS_SIZE 16
#define NVG_INIT_VERTS_SIZE 256
#define NVG_MAX_STATES 32

#define NVG_KAPPA90 0.5522847493f	// Length proportional to radius of a cubic bezier handle for 90deg arcs.

#define NVG_COUNTOF(arr) (sizeof(arr) / sizeof(0[arr]))


enum NVGcommands {
	NVG_MOVETO = 0,
	NVG_LINETO = 1,
	NVG_BEZIERTO = 2,
	NVG_CLOSE = 3,
	NVG_WINDING = 4,
};

enum NVGpointFlags
{
	NVG_PT_CORNER = 0x01,
	NVG_PT_LEFT = 0x02,
	NVG_PT_BEVEL = 0x04,
	NVG_PR_INNERBEVEL = 0x08,
};

struct NVGstate {
	NVGcompositeOperationState compositeOperation;
	int shapeAntiAlias;
	NVGpaint fill;
	NVGpaint stroke;
	float strokeWidth;
	float miterLimit;
	int lineJoin;
	int lineCap;
	float alpha;
	float xform[6];
	float fontSize;
	float letterSpacing;
	float lineHeight;
	float fontBlur;
	int textAlign;
	int fontId;
};
typedef struct NVGstate NVGstate;

struct NVGpoint {
	float x,y;
	float dx, dy;
	float len;
	float dmx, dmy;
	unsigned char flags;
};
typedef struct NVGpoint NVGpoint;

struct NVGpathCache {
	NVGpoint* points;
	int npoints;
	int cpoints;
	NVGpath* paths;
	int npaths;
	int cpaths;
	NVGvertex* verts;
	int nverts;
	int cverts;
	float bounds[4];
};
typedef struct NVGpathCache NVGpathCache;

struct NVGcontext {
	NVGparams params;
	float* commands;
	int ccommands;
	int ncommands;
	float commandx, commandy;
	NVGstate states[NVG_MAX_STATES];
	int nstates;
	NVGpathCache* cache;
	float tessTol;
	float distTol;
	float fringeWidth;
	float devicePxRatio;
	int drawCallCount;
	int fillTriCount;
	int strokeTriCount;
	int textTriCount;
};

static float nvg__sqrtf(float a) { return sqrtf(a); }
static float nvg__modf(float a, float b) { return fmodf(a, b); }
static float nvg__sinf(float a) { return sinf(a); }
static float nvg__cosf(float a) { return cosf(a); }
static float nvg__tanf(float a) { return tanf(a); }
static float nvg__atan2f(float a,float b) { return atan2f(a, b); }
static float nvg__acosf(float a) { return acosf(a); }

static int nvg__mini(int a, int b) { return a < b ? a : b; }
static int nvg__maxi(int a, int b) { return a > b ? a : b; }
static int nvg__clampi(int a, int mn, int mx) { return a < mn ? mn : (a > mx ? mx : a); }
static float nvg__minf(float a, float b) { return a < b ? a : b; }
static float nvg__maxf(float a, float b) { return a > b ? a : b; }
static float nvg__absf(float a) { return a >= 0.0f ? a : -a; }
static float nvg__signf(float a) { return a >= 0.0f ? 1.0f : -1.0f; }
static float nvg__clampf(float a, float mn, float mx) { return a < mn ? mn : (a > mx ? mx : a); }
static float nvg__cross(float dx0, float dy0, float dx1, float dy1) { return dx1*dy0 - dx0*dy1; }

static float nvg__normalize(float *x, float* y)
{
	float d = nvg__sqrtf((*x)*(*x) + (*y)*(*y));
	if (d > 1e-6f) {
		float id = 1.0f / d;
		*x *= id;
		*y *= id;
	}
	return d;
}


static void nvg__deletePathCache(NVGpathCache* c)
{
	if (c == NULL) return;
	if (c->points != NULL) free(c->points);
	if (c->paths != NULL) free(c->paths);
	if (c->verts != NULL) free(c->verts);
	free(c);
}

static NVGpathCache* nvg__allocPathCache(void)
{
	NVGpathCache* c = (NVGpathCache*)malloc(sizeof(NVGpathCache));
	if (c == NULL) goto error;
	memset(c, 0, sizeof(NVGpathCache));

	c->points = (NVGpoint*)malloc(sizeof(NVGpoint)*NVG_INIT_POINTS_SIZE);
	if (!c->points) goto error;
	c->npoints = 0;
	c->cpoints = NVG_INIT_POINTS_SIZE;

	c->paths = (NVGpath*)malloc(sizeof(NVGpath)*NVG_INIT_PATHS_SIZE);
	if (!c->paths) goto error;
	c->npaths = 0;
	c->cpaths = NVG_INIT_PATHS_SIZE;

	c->verts = (NVGvertex*)malloc(sizeof(NVGvertex)*NVG_INIT_VERTS_SIZE);
	if (!c->verts) goto error;
	c->nverts = 0;
	c->cverts = NVG_INIT_VERTS_SIZE;

	return c;
error:
	nvg__deletePathCache(c);
	return NULL;
}

static void nvg__setDevicePixelRatio(NVGcontext* ctx, float ratio)
{
	ctx->tessTol = 0.25f / ratio;
	ctx->distTol = 0.01f / ratio;
	ctx->fringeWidth = 1.0f / ratio;
	ctx->devicePxRatio = ratio;
}

static NVGcompositeOperationState nvg__compositeOperationState(int op)
{
	int sfactor, dfactor;

	if (op == NVG_SOURCE_OVER)
	{
		sfactor = NVG_ONE;
		dfactor = NVG_ONE_MINUS_SRC_ALPHA;
	}
	else if (op == NVG_SOURCE_IN)
	{
		sfactor = NVG_DST_ALPHA;
		dfactor = NVG_ZERO;
	}
	else if (op == NVG_SOURCE_OUT)
	{
		sfactor = NVG_ONE_MINUS_DST_ALPHA;
		dfactor = NVG_ZERO;
	}
	else if (op == NVG_ATOP)
	{
		sfactor = NVG_DST_ALPHA;
		dfactor = NVG_ONE_MINUS_SRC_ALPHA;
	}
	else if (op == NVG_DESTINATION_OVER)
	{
		sfactor = NVG_ONE_MINUS_DST_ALPHA;
		dfactor = NVG_ONE;
	}
	else if (op == NVG_DESTINATION_IN)
	{
		sfactor = NVG_ZERO;
		dfactor = NVG_SRC_ALPHA;
	}
	else if (op == NVG_DESTINATION_OUT)
	{
		sfactor = NVG_ZERO;
		dfactor = NVG_ONE_MINUS_SRC_ALPHA;
	}
	else if (op == NVG_DESTINATION_ATOP)
	{
		sfactor = NVG_ONE_MINUS_DST_ALPHA;
		dfactor = NVG_SRC_ALPHA;
	}
	else if (op == NVG_LIGHTER)
	{
		sfactor = NVG_ONE;
		dfactor = NVG_ONE;
	}
	else if (op == NVG_COPY)
	{
		sfactor = NVG_ONE;
		dfactor = NVG_ZERO;
	}
	else if (op == NVG_XOR)
	{
		sfactor = NVG_ONE_MINUS_DST_ALPHA;
		dfactor = NVG_ONE_MINUS_SRC_ALPHA;
	}
	else
	{
		sfactor = NVG_ONE;
		dfactor = NVG_ZERO;
	}

	NVGcompositeOperationState state;
	state.srcRGB = sfactor;
	state.dstRGB = dfactor;
	state.srcAlpha = sfactor;
	state.dstAlpha = dfactor;
	return state;
}

static NVGstate* nvg__getState(NVGcontext* ctx)
{
	return &ctx->states[ctx->nstates-1];
}

NVGcontext* nvgCreateInternal(NVGparams* params)
{
	NVGcontext* ctx = (NVGcontext*)malloc(sizeof(NVGcontext));
	if (ctx == NULL) goto error;
	memset(ctx, 0, sizeof(NVGcontext));

	ctx->params = *params;

	ctx->commands = (float*)malloc(sizeof(float)*NVG_INIT_COMMANDS_SIZE);
	if (!ctx->commands) goto error;
	ctx->ncommands = 0;
	ctx->ccommands = NVG_INIT_COMMANDS_SIZE;

	ctx->cache = nvg__allocPathCache();
	if (ctx->cache == NULL) goto error;

	nvgSave(ctx);
	nvgReset(ctx);

	nvg__setDevicePixelRatio(ctx, 1.0f);

	return ctx;

error:
	nvgDeleteInternal(ctx);
	return 0;
}

NVGparams* nvgInternalParams(NVGcontext* ctx)
{
    return &ctx->params;
}

void nvgDeleteInternal(NVGcontext* ctx)
{
	if (ctx == NULL) return;
	if (ctx->commands != NULL) free(ctx->commands);
	if (ctx->cache != NULL) nvg__deletePathCache(ctx->cache);

	free(ctx);
}

void nvgBeginFrame(NVGcontext* ctx, float devicePixelRatio)
{
/*	printf("Tris: draws:%d  fill:%d  stroke:%d  text:%d  TOT:%d\n",
		ctx->drawCallCount, ctx->fillTriCount, ctx->strokeTriCount, ctx->textTriCount,
		ctx->fillTriCount+ctx->strokeTriCount+ctx->textTriCount);*/

	ctx->nstates = 0;
	nvgSave(ctx);
	nvgReset(ctx);

	nvg__setDevicePixelRatio(ctx, devicePixelRatio);

	ctx->drawCallCount = 0;
	ctx->fillTriCount = 0;
	ctx->strokeTriCount = 0;
	ctx->textTriCount = 0;
}

NVGcolor nvgRGB(unsigned char r, unsigned char g, unsigned char b)
{
	return nvgRGBA(r,g,b,255);
}

NVGcolor nvgRGBf(float r, float g, float b)
{
	return nvgRGBAf(r,g,b,1.0f);
}

NVGcolor nvgRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	NVGcolor color;
	// Use longer initialization to suppress warning.
	color.r = r / 255.0f;
	color.g = g / 255.0f;
	color.b = b / 255.0f;
	color.a = a / 255.0f;
	return color;
}

NVGcolor nvgRGBAf(float r, float g, float b, float a)
{
	NVGcolor color;
	// Use longer initialization to suppress warning.
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	return color;
}

NVGcolor nvgTransRGBA(NVGcolor c, unsigned char a)
{
	c.a = a / 255.0f;
	return c;
}

NVGcolor nvgTransRGBAf(NVGcolor c, float a)
{
	c.a = a;
	return c;
}

NVGcolor nvgLerpRGBA(NVGcolor c0, NVGcolor c1, float u)
{
	int i;
	float oneminu;
	NVGcolor cint = {{{0}}};

	u = nvg__clampf(u, 0.0f, 1.0f);
	oneminu = 1.0f - u;
	for( i = 0; i <4; i++ )
	{
		cint.rgba[i] = c0.rgba[i] * oneminu + c1.rgba[i] * u;
	}

	return cint;
}

NVGcolor nvgHSL(float h, float s, float l)
{
	return nvgHSLA(h,s,l,255);
}

static float nvg__hue(float h, float m1, float m2)
{
	if (h < 0) h += 1;
	if (h > 1) h -= 1;
	if (h < 1.0f/6.0f)
		return m1 + (m2 - m1) * h * 6.0f;
	else if (h < 3.0f/6.0f)
		return m2;
	else if (h < 4.0f/6.0f)
		return m1 + (m2 - m1) * (2.0f/3.0f - h) * 6.0f;
	return m1;
}

NVGcolor nvgHSLA(float h, float s, float l, unsigned char a)
{
	float m1, m2;
	NVGcolor col;
	h = nvg__modf(h, 1.0f);
	if (h < 0.0f) h += 1.0f;
	s = nvg__clampf(s, 0.0f, 1.0f);
	l = nvg__clampf(l, 0.0f, 1.0f);
	m2 = l <= 0.5f ? (l * (1 + s)) : (l + s - l * s);
	m1 = 2 * l - m2;
	col.r = nvg__clampf(nvg__hue(h + 1.0f/3.0f, m1, m2), 0.0f, 1.0f);
	col.g = nvg__clampf(nvg__hue(h, m1, m2), 0.0f, 1.0f);
	col.b = nvg__clampf(nvg__hue(h - 1.0f/3.0f, m1, m2), 0.0f, 1.0f);
	col.a = a/255.0f;
	return col;
}

void nvgTransformIdentity(float* t)
{
	t[0] = 1.0f; t[1] = 0.0f;
	t[2] = 0.0f; t[3] = 1.0f;
	t[4] = 0.0f; t[5] = 0.0f;
}

void nvgTransformTranslate(float* t, float tx, float ty)
{
	t[0] = 1.0f; t[1] = 0.0f;
	t[2] = 0.0f; t[3] = 1.0f;
	t[4] = tx; t[5] = ty;
}

void nvgTransformScale(float* t, float sx, float sy)
{
	t[0] = sx; t[1] = 0.0f;
	t[2] = 0.0f; t[3] = sy;
	t[4] = 0.0f; t[5] = 0.0f;
}

void nvgTransformRotate(float* t, float a)
{
	float cs = nvg__cosf(a), sn = nvg__sinf(a);
	t[0] = cs; t[1] = sn;
	t[2] = -sn; t[3] = cs;
	t[4] = 0.0f; t[5] = 0.0f;
}

void nvgTransformSkewX(float* t, float a)
{
	t[0] = 1.0f; t[1] = 0.0f;
	t[2] = nvg__tanf(a); t[3] = 1.0f;
	t[4] = 0.0f; t[5] = 0.0f;
}

void nvgTransformSkewY(float* t, float a)
{
	t[0] = 1.0f; t[1] = nvg__tanf(a);
	t[2] = 0.0f; t[3] = 1.0f;
	t[4] = 0.0f; t[5] = 0.0f;
}

void nvgTransformMultiply(float* t, const float* s)
{
	float t0 = t[0] * s[0] + t[1] * s[2];
	float t2 = t[2] * s[0] + t[3] * s[2];
	float t4 = t[4] * s[0] + t[5] * s[2] + s[4];
	t[1] = t[0] * s[1] + t[1] * s[3];
	t[3] = t[2] * s[1] + t[3] * s[3];
	t[5] = t[4] * s[1] + t[5] * s[3] + s[5];
	t[0] = t0;
	t[2] = t2;
	t[4] = t4;
}

void nvgTransformPremultiply(float* t, const float* s)
{
	float s2[6];
	memcpy(s2, s, sizeof(float)*6);
	nvgTransformMultiply(s2, t);
	memcpy(t, s2, sizeof(float)*6);
}

int nvgTransformInverse(float* inv, const float* t)
{
	double invdet, det = (double)t[0] * t[3] - (double)t[2] * t[1];
	if (det > -1e-6 && det < 1e-6) {
		nvgTransformIdentity(inv);
		return 0;
	}
	invdet = 1.0 / det;
	inv[0] = (float)(t[3] * invdet);
	inv[2] = (float)(-t[2] * invdet);
	inv[4] = (float)(((double)t[2] * t[5] - (double)t[3] * t[4]) * invdet);
	inv[1] = (float)(-t[1] * invdet);
	inv[3] = (float)(t[0] * invdet);
	inv[5] = (float)(((double)t[1] * t[4] - (double)t[0] * t[5]) * invdet);
	return 1;
}

void nvgTransformPoint(float* dx, float* dy, const float* t, float sx, float sy)
{
	*dx = sx*t[0] + sy*t[2] + t[4];
	*dy = sx*t[1] + sy*t[3] + t[5];
}

float nvgDegToRad(float deg)
{
	return deg / 180.0f * NVG_PI;
}

float nvgRadToDeg(float rad)
{
	return rad / NVG_PI * 180.0f;
}

static void nvg__setPaintColor(NVGpaint* p, NVGcolor color)
{
	memset(p, 0, sizeof(*p));
	nvgTransformIdentity(p->xform);
	p->radius = 0.0f;
	p->feather = 1.0f;
	p->innerColor = color;
	p->outerColor = color;
}


// State handling
void nvgSave(NVGcontext* ctx)
{
	if (ctx->nstates >= NVG_MAX_STATES)
		return;
	if (ctx->nstates > 0)
		memcpy(&ctx->states[ctx->nstates], &ctx->states[ctx->nstates-1], sizeof(NVGstate));
	ctx->nstates++;
}

void nvgRestore(NVGcontext* ctx)
{
	if (ctx->nstates <= 1)
		return;
	ctx->nstates--;
}

void nvgReset(NVGcontext* ctx)
{
	NVGstate* state = nvg__getState(ctx);
	memset(state, 0, sizeof(*state));

	nvg__setPaintColor(&state->fill, nvgRGBA(255,255,255,255));
	nvg__setPaintColor(&state->stroke, nvgRGBA(0,0,0,255));
	state->compositeOperation = nvg__compositeOperationState(NVG_SOURCE_OVER);
	state->shapeAntiAlias = 1;
	state->strokeWidth = 1.0f;
	state->miterLimit = 10.0f;
	state->lineCap = NVG_BUTT;
	state->lineJoin = NVG_MITER;
	state->alpha = 1.0f;
	nvgTransformIdentity(state->xform);

	state->fontSize = 16.0f;
	state->letterSpacing = 0.0f;
	state->lineHeight = 1.0f;
	state->fontBlur = 0.0f;
	state->textAlign = NVG_ALIGN_LEFT | NVG_ALIGN_BASELINE;
	state->fontId = 0;
}

// State setting
void nvgShapeAntiAlias(NVGcontext* ctx, int enabled)
{
	NVGstate* state = nvg__getState(ctx);
	state->shapeAntiAlias = enabled;
}

void nvgStrokeWidth(NVGcontext* ctx, float width)
{
	NVGstate* state = nvg__getState(ctx);
	state->strokeWidth = width;
}

void nvgMiterLimit(NVGcontext* ctx, float limit)
{
	NVGstate* state = nvg__getState(ctx);
	state->miterLimit = limit;
}

void nvgLineCap(NVGcontext* ctx, int cap)
{
	NVGstate* state = nvg__getState(ctx);
	state->lineCap = cap;
}

void nvgLineJoin(NVGcontext* ctx, int join)
{
	NVGstate* state = nvg__getState(ctx);
	state->lineJoin = join;
}

void nvgGlobalAlpha(NVGcontext* ctx, float alpha)
{
	NVGstate* state = nvg__getState(ctx);
	state->alpha = alpha;
}

void nvgTransform(NVGcontext* ctx, float a, float b, float c, float d, float e, float f)
{
	NVGstate* state = nvg__getState(ctx);
	float t[6] = { a, b, c, d, e, f };
	nvgTransformPremultiply(state->xform, t);
}

void nvgResetTransform(NVGcontext* ctx)
{
	NVGstate* state = nvg__getState(ctx);
	nvgTransformIdentity(state->xform);
}

void nvgTranslate(NVGcontext* ctx, float x, float y)
{
	NVGstate* state = nvg__getState(ctx);
	float t[6];
	nvgTransformTranslate(t, x,y);
	nvgTransformPremultiply(state->xform, t);
}

void nvgRotate(NVGcontext* ctx, float angle)
{
	NVGstate* state = nvg__getState(ctx);
	float t[6];
	nvgTransformRotate(t, angle);
	nvgTransformPremultiply(state->xform, t);
}

void nvgSkewX(NVGcontext* ctx, float angle)
{
	NVGstate* state = nvg__getState(ctx);
	float t[6];
	nvgTransformSkewX(t, angle);
	nvgTransformPremultiply(state->xform, t);
}

void nvgSkewY(NVGcontext* ctx, float angle)
{
	NVGstate* state = nvg__getState(ctx);
	float t[6];
	nvgTransformSkewY(t, angle);
	nvgTransformPremultiply(state->xform, t);
}

void nvgScale(NVGcontext* ctx, float x, float y)
{
	NVGstate* state = nvg__getState(ctx);
	float t[6];
	nvgTransformScale(t, x,y);
	nvgTransformPremultiply(state->xform, t);
}

void nvgCurrentTransform(NVGcontext* ctx, float* xform)
{
	NVGstate* state = nvg__getState(ctx);
	if (xform == NULL) return;
	memcpy(xform, state->xform, sizeof(float)*6);
}

void nvgStrokeColor(NVGcontext* ctx, NVGcolor color)
{
	NVGstate* state = nvg__getState(ctx);
	nvg__setPaintColor(&state->stroke, color);
}

void nvgStrokePaint(NVGcontext* ctx, NVGpaint paint)
{
	NVGstate* state = nvg__getState(ctx);
	state->stroke = paint;
	nvgTransformMultiply(state->stroke.xform, state->xform);
}

void nvgFillColor(NVGcontext* ctx, NVGcolor color)
{
	NVGstate* state = nvg__getState(ctx);
	nvg__setPaintColor(&state->fill, color);
}

void nvgFillPaint(NVGcontext* ctx, NVGpaint paint)
{
	NVGstate* state = nvg__getState(ctx);
	state->fill = paint;
	nvgTransformMultiply(state->fill.xform, state->xform);
}

NVGpaint nvgLinearGradient(NVGcontext* ctx,
								  float sx, float sy, float ex, float ey,
								  NVGcolor icol, NVGcolor ocol)
{
	NVGpaint p;
	float dx, dy, d;
	const float large = 1e5;
	NVG_NOTUSED(ctx);
	memset(&p, 0, sizeof(p));

	// Calculate transform aligned to the line
	dx = ex - sx;
	dy = ey - sy;
	d = sqrtf(dx*dx + dy*dy);
	if (d > 0.0001f) {
		dx /= d;
		dy /= d;
	} else {
		dx = 0;
		dy = 1;
	}

	p.xform[0] = dy; p.xform[1] = -dx;
	p.xform[2] = dx; p.xform[3] = dy;
	p.xform[4] = sx - dx*large; p.xform[5] = sy - dy*large;

	p.extent[0] = large;
	p.extent[1] = large + d*0.5f;

	p.radius = 0.0f;

	p.feather = nvg__maxf(1.0f, d);
	p.noGamma = true;

	p.innerColor = icol;
	p.outerColor = ocol;

	return p;
}

NVGpaint nvgRadialGradient(NVGcontext* ctx,
								  float cx, float cy, float inr, float outr,
								  NVGcolor icol, NVGcolor ocol)
{
	NVGpaint p;
	float r = (inr+outr)*0.5f;
	float f = (outr-inr);
	NVG_NOTUSED(ctx);
	memset(&p, 0, sizeof(p));

	nvgTransformIdentity(p.xform);
	p.xform[4] = cx;
	p.xform[5] = cy;

	p.extent[0] = r;
	p.extent[1] = r;

	p.radius = r;

	p.feather = nvg__maxf(1.0f, f);
	p.noGamma = true;

	p.innerColor = icol;
	p.outerColor = ocol;

	return p;
}

NVGpaint nvgBoxGradient(NVGcontext* ctx,
							   float x, float y, float w, float h, float r, float f,
							   NVGcolor icol, NVGcolor ocol)
{
	NVGpaint p;
	NVG_NOTUSED(ctx);
	memset(&p, 0, sizeof(p));

	nvgTransformIdentity(p.xform);
	p.xform[4] = x+w*0.5f;
	p.xform[5] = y+h*0.5f;

	p.extent[0] = w*0.5f;
	p.extent[1] = h*0.5f;

	p.radius = r;

	p.feather = nvg__maxf(1.0f, f);
	p.noGamma = true;

	p.innerColor = icol;
	p.outerColor = ocol;

	return p;
}


NVGpaint nvgImagePattern(NVGcontext* ctx,
								float cx, float cy, float w, float h, float angle,
								void* image, NVGcolor ocol)
{
	NVGpaint p;
	NVG_NOTUSED(ctx);
	memset(&p, 0, sizeof(p));

	nvgTransformRotate(p.xform, angle);
	p.xform[4] = cx;
	p.xform[5] = cy;

	p.extent[0] = w;
	p.extent[1] = h;

	p.image = image;
	p.noGamma = true;

	p.innerColor = p.outerColor = ocol;

	return p;
}

NVGpaint nvgMaterialPattern(NVGcontext* ctx,
								float cx, float cy, float w, float h, float angle,
								void* material, NVGcolor ocol)
{
	NVGpaint p;
	NVG_NOTUSED(ctx);
	memset(&p, 0, sizeof(p));

	nvgTransformRotate(p.xform, angle);
	p.xform[4] = cx;
	p.xform[5] = cy;

	p.extent[0] = w;
	p.extent[1] = h;

	p.material = material;
	p.noGamma = true;

	p.innerColor = p.outerColor = ocol;

	return p;
}

NVGpaint nvgImagePatternEx(NVGcontext* ctx, float cx, float cy, float w, float h,
						 float angle, void* image, NVGcolor ocol, float px, float py, NVGbox box)
{
	NVGpaint p;
	NVG_NOTUSED(ctx);
	memset(&p, 0, sizeof(p));

	nvgTransformRotate(p.xform, angle);
	p.xform[4] = cx;
	p.xform[5] = cy;

	p.extent[0] = w;
	p.extent[1] = h;

	p.image = image;

	p.innerColor = p.outerColor = ocol;

	p.box = box;
	p.noGamma = true;

	return p;
}

// Global composite operation.
void nvgGlobalCompositeOperation(NVGcontext* ctx, int op)
{
	NVGstate* state = nvg__getState(ctx);
	state->compositeOperation = nvg__compositeOperationState(op);
}

void nvgGlobalCompositeBlendFunc(NVGcontext* ctx, int sfactor, int dfactor)
{
	nvgGlobalCompositeBlendFuncSeparate(ctx, sfactor, dfactor, sfactor, dfactor);
}

void nvgGlobalCompositeBlendFuncSeparate(NVGcontext* ctx, int srcRGB, int dstRGB, int srcAlpha, int dstAlpha)
{
	NVGcompositeOperationState op;
	op.srcRGB = srcRGB;
	op.dstRGB = dstRGB;
	op.srcAlpha = srcAlpha;
	op.dstAlpha = dstAlpha;

	NVGstate* state = nvg__getState(ctx);
	state->compositeOperation = op;
}

static int nvg__ptEquals(float x1, float y1, float x2, float y2, float tol)
{
	float dx = x2 - x1;
	float dy = y2 - y1;
	return dx*dx + dy*dy < tol*tol;
}

static float nvg__distPtSeg(float x, float y, float px, float py, float qx, float qy)
{
	float pqx, pqy, dx, dy, d, t;
	pqx = qx-px;
	pqy = qy-py;
	dx = x-px;
	dy = y-py;
	d = pqx*pqx + pqy*pqy;
	t = pqx*dx + pqy*dy;
	if (d > 0) t /= d;
	if (t < 0) t = 0;
	else if (t > 1) t = 1;
	dx = px + t*pqx - x;
	dy = py + t*pqy - y;
	return dx*dx + dy*dy;
}

static void nvg__appendCommands(NVGcontext* ctx, float* vals, int nvals)
{
	NVGstate* state = nvg__getState(ctx);
	int i;

	if (ctx->ncommands+nvals > ctx->ccommands) {
		float* commands;
		int ccommands = ctx->ncommands+nvals + ctx->ccommands/2;
		commands = (float*)realloc(ctx->commands, sizeof(float)*ccommands);
		if (commands == NULL) return;
		ctx->commands = commands;
		ctx->ccommands = ccommands;
	}

	if ((int)vals[0] != NVG_CLOSE && (int)vals[0] != NVG_WINDING) {
		ctx->commandx = vals[nvals-2];
		ctx->commandy = vals[nvals-1];
	}

	// transform commands
	i = 0;
	while (i < nvals) {
		int cmd = (int)vals[i];
		switch (cmd) {
		case NVG_MOVETO:
			nvgTransformPoint(&vals[i+1],&vals[i+2], state->xform, vals[i+1],vals[i+2]);
			i += 3;
			break;
		case NVG_LINETO:
			nvgTransformPoint(&vals[i+1],&vals[i+2], state->xform, vals[i+1],vals[i+2]);
			i += 3;
			break;
		case NVG_BEZIERTO:
			nvgTransformPoint(&vals[i+1],&vals[i+2], state->xform, vals[i+1],vals[i+2]);
			nvgTransformPoint(&vals[i+3],&vals[i+4], state->xform, vals[i+3],vals[i+4]);
			nvgTransformPoint(&vals[i+5],&vals[i+6], state->xform, vals[i+5],vals[i+6]);
			i += 7;
			break;
		case NVG_CLOSE:
			i++;
			break;
		case NVG_WINDING:
			i += 2;
			break;
		default:
			i++;
		}
	}

	memcpy(&ctx->commands[ctx->ncommands], vals, nvals*sizeof(float));

	ctx->ncommands += nvals;
}


static void nvg__clearPathCache(NVGcontext* ctx)
{
	ctx->cache->npoints = 0;
	ctx->cache->npaths = 0;
}

static NVGpath* nvg__lastPath(NVGcontext* ctx)
{
	if (ctx->cache->npaths > 0)
		return &ctx->cache->paths[ctx->cache->npaths-1];
	return NULL;
}

static void nvg__addPath(NVGcontext* ctx)
{
	NVGpath* path;
	if (ctx->cache->npaths+1 > ctx->cache->cpaths) {
		NVGpath* paths;
		int cpaths = ctx->cache->npaths+1 + ctx->cache->cpaths/2;
		paths = (NVGpath*)realloc(ctx->cache->paths, sizeof(NVGpath)*cpaths);
		if (paths == NULL) return;
		ctx->cache->paths = paths;
		ctx->cache->cpaths = cpaths;
	}
	path = &ctx->cache->paths[ctx->cache->npaths];
	memset(path, 0, sizeof(*path));
	path->first = ctx->cache->npoints;
	path->winding = NVG_CCW;

	ctx->cache->npaths++;
}

static NVGpoint* nvg__lastPoint(NVGcontext* ctx)
{
	if (ctx->cache->npoints > 0)
		return &ctx->cache->points[ctx->cache->npoints-1];
	return NULL;
}

static void nvg__addPoint(NVGcontext* ctx, float x, float y, int flags)
{
	NVGpath* path = nvg__lastPath(ctx);
	NVGpoint* pt;
	if (path == NULL) return;

	if (path->count > 0 && ctx->cache->npoints > 0) {
		pt = nvg__lastPoint(ctx);
		if (nvg__ptEquals(pt->x,pt->y, x,y, ctx->distTol)) {
			pt->flags |= flags;
			return;
		}
	}

	if (ctx->cache->npoints+1 > ctx->cache->cpoints) {
		NVGpoint* points;
		int cpoints = ctx->cache->npoints+1 + ctx->cache->cpoints/2;
		points = (NVGpoint*)realloc(ctx->cache->points, sizeof(NVGpoint)*cpoints);
		if (points == NULL) return;
		ctx->cache->points = points;
		ctx->cache->cpoints = cpoints;
	}

	pt = &ctx->cache->points[ctx->cache->npoints];
	memset(pt, 0, sizeof(*pt));
	pt->x = x;
	pt->y = y;
	pt->flags = (unsigned char)flags;

	ctx->cache->npoints++;
	path->count++;
}

static void nvg__closePath(NVGcontext* ctx)
{
	NVGpath* path = nvg__lastPath(ctx);
	if (path == NULL) return;
	path->closed = 1;
}

static void nvg__pathWinding(NVGcontext* ctx, int winding)
{
	NVGpath* path = nvg__lastPath(ctx);
	if (path == NULL) return;
	path->winding = winding;
}

static float nvg__getAverageScale(float *t)
{
	float sx = sqrtf(t[0]*t[0] + t[2]*t[2]);
	float sy = sqrtf(t[1]*t[1] + t[3]*t[3]);
	return (sx + sy) * 0.5f;
}

static NVGvertex* nvg__allocTempVerts(NVGcontext* ctx, int nverts)
{
	if (nverts > ctx->cache->cverts) {
		NVGvertex* verts;
		int cverts = (nverts + 0xff) & ~0xff; // Round up to prevent allocations when things change just slightly.
		verts = (NVGvertex*)realloc(ctx->cache->verts, sizeof(NVGvertex)*cverts);
		if (verts == NULL) return NULL;
		ctx->cache->verts = verts;
		ctx->cache->cverts = cverts;
	}

	return ctx->cache->verts;
}

static float nvg__triarea2(float ax, float ay, float bx, float by, float cx, float cy)
{
	float abx = bx - ax;
	float aby = by - ay;
	float acx = cx - ax;
	float acy = cy - ay;
	return acx*aby - abx*acy;
}

static float nvg__polyArea(NVGpoint* pts, int npts)
{
	int i;
	float area = 0;
	for (i = 2; i < npts; i++) {
		NVGpoint* a = &pts[0];
		NVGpoint* b = &pts[i-1];
		NVGpoint* c = &pts[i];
		area += nvg__triarea2(a->x,a->y, b->x,b->y, c->x,c->y);
	}
	return area * 0.5f;
}

static void nvg__polyReverse(NVGpoint* pts, int npts)
{
	NVGpoint tmp;
	int i = 0, j = npts-1;
	while (i < j) {
		tmp = pts[i];
		pts[i] = pts[j];
		pts[j] = tmp;
		i++;
		j--;
	}
}


static void nvg__vset(NVGvertex* vtx, float x, float y, float u, float v)
{
	vtx->x = x;
	vtx->y = y;
	vtx->u = u;
	vtx->v = v;
}

static void nvg__tesselateBezier(NVGcontext* ctx,
								 float x1, float y1, float x2, float y2,
								 float x3, float y3, float x4, float y4,
								 int level, int type)
{
	float x12,y12,x23,y23,x34,y34,x123,y123,x234,y234,x1234,y1234;
	float dx,dy,d2,d3;

	if (level > 10) return;

	x12 = (x1+x2)*0.5f;
	y12 = (y1+y2)*0.5f;
	x23 = (x2+x3)*0.5f;
	y23 = (y2+y3)*0.5f;
	x34 = (x3+x4)*0.5f;
	y34 = (y3+y4)*0.5f;
	x123 = (x12+x23)*0.5f;
	y123 = (y12+y23)*0.5f;

	dx = x4 - x1;
	dy = y4 - y1;
	d2 = nvg__absf(((x2 - x4) * dy - (y2 - y4) * dx));
	d3 = nvg__absf(((x3 - x4) * dy - (y3 - y4) * dx));

	if ((d2 + d3)*(d2 + d3) < ctx->tessTol * (dx*dx + dy*dy)) {
		nvg__addPoint(ctx, x4, y4, type);
		return;
	}

/*	if (nvg__absf(x1+x3-x2-x2) + nvg__absf(y1+y3-y2-y2) + nvg__absf(x2+x4-x3-x3) + nvg__absf(y2+y4-y3-y3) < ctx->tessTol) {
		nvg__addPoint(ctx, x4, y4, type);
		return;
	}*/

	x234 = (x23+x34)*0.5f;
	y234 = (y23+y34)*0.5f;
	x1234 = (x123+x234)*0.5f;
	y1234 = (y123+y234)*0.5f;

	nvg__tesselateBezier(ctx, x1,y1, x12,y12, x123,y123, x1234,y1234, level+1, 0);
	nvg__tesselateBezier(ctx, x1234,y1234, x234,y234, x34,y34, x4,y4, level+1, type);
}

static void nvg__flattenPaths(NVGcontext* ctx)
{
	NVGpathCache* cache = ctx->cache;
//	NVGstate* state = nvg__getState(ctx);
	NVGpoint* last;
	NVGpoint* p0;
	NVGpoint* p1;
	NVGpoint* pts;
	NVGpath* path;
	int i, j;
	float* cp1;
	float* cp2;
	float* p;
	float area;

	if (cache->npaths > 0)
		return;

	// Flatten
	i = 0;
	while (i < ctx->ncommands) {
		int cmd = (int)ctx->commands[i];
		switch (cmd) {
		case NVG_MOVETO:
			nvg__addPath(ctx);
			p = &ctx->commands[i+1];
			nvg__addPoint(ctx, p[0], p[1], NVG_PT_CORNER);
			i += 3;
			break;
		case NVG_LINETO:
			p = &ctx->commands[i+1];
			nvg__addPoint(ctx, p[0], p[1], NVG_PT_CORNER);
			i += 3;
			break;
		case NVG_BEZIERTO:
			last = nvg__lastPoint(ctx);
			if (last != NULL) {
				cp1 = &ctx->commands[i+1];
				cp2 = &ctx->commands[i+3];
				p = &ctx->commands[i+5];
				nvg__tesselateBezier(ctx, last->x,last->y, cp1[0],cp1[1], cp2[0],cp2[1], p[0],p[1], 0, NVG_PT_CORNER);
			}
			i += 7;
			break;
		case NVG_CLOSE:
			nvg__closePath(ctx);
			i++;
			break;
		case NVG_WINDING:
			nvg__pathWinding(ctx, (int)ctx->commands[i+1]);
			i += 2;
			break;
		default:
			i++;
		}
	}

	cache->bounds[0] = cache->bounds[1] = 1e6f;
	cache->bounds[2] = cache->bounds[3] = -1e6f;

	// Calculate the direction and length of line segments.
	for (j = 0; j < cache->npaths; j++) {
		path = &cache->paths[j];
		pts = &cache->points[path->first];

		// If the first and last points are the same, remove the last, mark as closed path.
		p0 = &pts[path->count-1];
		p1 = &pts[0];
		if (nvg__ptEquals(p0->x,p0->y, p1->x,p1->y, ctx->distTol)) {
			path->count--;
			p0 = &pts[path->count-1];
			path->closed = 1;
		}

		// Enforce winding.
		if (path->count > 2) {
			area = nvg__polyArea(pts, path->count);
			if (path->winding == NVG_CCW && area < 0.0f)
				nvg__polyReverse(pts, path->count);
			if (path->winding == NVG_CW && area > 0.0f)
				nvg__polyReverse(pts, path->count);
		}

		for(i = 0; i < path->count; i++) {
			// Calculate segment direction and length
			p0->dx = p1->x - p0->x;
			p0->dy = p1->y - p0->y;
			p0->len = nvg__normalize(&p0->dx, &p0->dy);
			// Update bounds
			cache->bounds[0] = nvg__minf(cache->bounds[0], p0->x);
			cache->bounds[1] = nvg__minf(cache->bounds[1], p0->y);
			cache->bounds[2] = nvg__maxf(cache->bounds[2], p0->x);
			cache->bounds[3] = nvg__maxf(cache->bounds[3], p0->y);
			// Advance
			p0 = p1++;
		}
	}
}

static int nvg__curveDivs(float r, float arc, float tol)
{
	float da = acosf(r / (r + tol)) * 2.0f;
	return nvg__maxi(2, (int)ceilf(arc / da));
}

static void nvg__chooseBevel(int bevel, NVGpoint* p0, NVGpoint* p1, float w,
							float* x0, float* y0, float* x1, float* y1)
{
	if (bevel) {
		*x0 = p1->x + p0->dy * w;
		*y0 = p1->y - p0->dx * w;
		*x1 = p1->x + p1->dy * w;
		*y1 = p1->y - p1->dx * w;
	} else {
		*x0 = p1->x + p1->dmx * w;
		*y0 = p1->y + p1->dmy * w;
		*x1 = p1->x + p1->dmx * w;
		*y1 = p1->y + p1->dmy * w;
	}
}

static NVGvertex* nvg__roundJoin(NVGvertex* dst, NVGpoint* p0, NVGpoint* p1,
								 float lw, float rw, float lu, float ru, int ncap,
								 float fringe)
{
	int i, n;
	float dlx0 = p0->dy;
	float dly0 = -p0->dx;
	float dlx1 = p1->dy;
	float dly1 = -p1->dx;
	NVG_NOTUSED(fringe);

	if (p1->flags & NVG_PT_LEFT) {
		float lx0,ly0,lx1,ly1,a0,a1;
		nvg__chooseBevel(p1->flags & NVG_PR_INNERBEVEL, p0, p1, lw, &lx0,&ly0, &lx1,&ly1);
		a0 = atan2f(-dly0, -dlx0);
		a1 = atan2f(-dly1, -dlx1);
		if (a1 > a0) a1 -= NVG_PI*2;

		nvg__vset(dst, lx0, ly0, lu,1); dst++;
		nvg__vset(dst, p1->x - dlx0*rw, p1->y - dly0*rw, ru,1); dst++;

		n = nvg__clampi((int)ceilf(((a0 - a1) / NVG_PI) * ncap), 2, ncap);
		for (i = 0; i < n; i++) {
			float u = i/(float)(n-1);
			float a = a0 + u*(a1-a0);
			float rx = p1->x + cosf(a) * rw;
			float ry = p1->y + sinf(a) * rw;
			nvg__vset(dst, p1->x, p1->y, 0.5f,1); dst++;
			nvg__vset(dst, rx, ry, ru,1); dst++;
		}

		nvg__vset(dst, lx1, ly1, lu,1); dst++;
		nvg__vset(dst, p1->x - dlx1*rw, p1->y - dly1*rw, ru,1); dst++;

	} else {
		float rx0,ry0,rx1,ry1,a0,a1;
		nvg__chooseBevel(p1->flags & NVG_PR_INNERBEVEL, p0, p1, -rw, &rx0,&ry0, &rx1,&ry1);
		a0 = atan2f(dly0, dlx0);
		a1 = atan2f(dly1, dlx1);
		if (a1 < a0) a1 += NVG_PI*2;

		nvg__vset(dst, p1->x + dlx0*rw, p1->y + dly0*rw, lu,1); dst++;
		nvg__vset(dst, rx0, ry0, ru,1); dst++;

		n = nvg__clampi((int)ceilf(((a1 - a0) / NVG_PI) * ncap), 2, ncap);
		for (i = 0; i < n; i++) {
			float u = i/(float)(n-1);
			float a = a0 + u*(a1-a0);
			float lx = p1->x + cosf(a) * lw;
			float ly = p1->y + sinf(a) * lw;
			nvg__vset(dst, lx, ly, lu,1); dst++;
			nvg__vset(dst, p1->x, p1->y, 0.5f,1); dst++;
		}

		nvg__vset(dst, p1->x + dlx1*rw, p1->y + dly1*rw, lu,1); dst++;
		nvg__vset(dst, rx1, ry1, ru,1); dst++;

	}
	return dst;
}

static NVGvertex* nvg__bevelJoin(NVGvertex* dst, NVGpoint* p0, NVGpoint* p1,
										float lw, float rw, float lu, float ru, float fringe)
{
	float rx0,ry0,rx1,ry1;
	float lx0,ly0,lx1,ly1;
	float dlx0 = p0->dy;
	float dly0 = -p0->dx;
	float dlx1 = p1->dy;
	float dly1 = -p1->dx;
	NVG_NOTUSED(fringe);

	if (p1->flags & NVG_PT_LEFT) {
		nvg__chooseBevel(p1->flags & NVG_PR_INNERBEVEL, p0, p1, lw, &lx0,&ly0, &lx1,&ly1);

		nvg__vset(dst, lx0, ly0, lu,1); dst++;
		nvg__vset(dst, p1->x - dlx0*rw, p1->y - dly0*rw, ru,1); dst++;

		if (p1->flags & NVG_PT_BEVEL) {
			nvg__vset(dst, lx0, ly0, lu,1); dst++;
			nvg__vset(dst, p1->x - dlx0*rw, p1->y - dly0*rw, ru,1); dst++;

			nvg__vset(dst, lx1, ly1, lu,1); dst++;
			nvg__vset(dst, p1->x - dlx1*rw, p1->y - dly1*rw, ru,1); dst++;
		} else {
			rx0 = p1->x - p1->dmx * rw;
			ry0 = p1->y - p1->dmy * rw;

			nvg__vset(dst, p1->x, p1->y, 0.5f,1); dst++;
			nvg__vset(dst, p1->x - dlx0*rw, p1->y - dly0*rw, ru,1); dst++;

			nvg__vset(dst, rx0, ry0, ru,1); dst++;
			nvg__vset(dst, rx0, ry0, ru,1); dst++;

			nvg__vset(dst, p1->x, p1->y, 0.5f,1); dst++;
			nvg__vset(dst, p1->x - dlx1*rw, p1->y - dly1*rw, ru,1); dst++;
		}

		nvg__vset(dst, lx1, ly1, lu,1); dst++;
		nvg__vset(dst, p1->x - dlx1*rw, p1->y - dly1*rw, ru,1); dst++;

	} else {
		nvg__chooseBevel(p1->flags & NVG_PR_INNERBEVEL, p0, p1, -rw, &rx0,&ry0, &rx1,&ry1);

		nvg__vset(dst, p1->x + dlx0*lw, p1->y + dly0*lw, lu,1); dst++;
		nvg__vset(dst, rx0, ry0, ru,1); dst++;

		if (p1->flags & NVG_PT_BEVEL) {
			nvg__vset(dst, p1->x + dlx0*lw, p1->y + dly0*lw, lu,1); dst++;
			nvg__vset(dst, rx0, ry0, ru,1); dst++;

			nvg__vset(dst, p1->x + dlx1*lw, p1->y + dly1*lw, lu,1); dst++;
			nvg__vset(dst, rx1, ry1, ru,1); dst++;
		} else {
			lx0 = p1->x + p1->dmx * lw;
			ly0 = p1->y + p1->dmy * lw;

			nvg__vset(dst, p1->x + dlx0*lw, p1->y + dly0*lw, lu,1); dst++;
			nvg__vset(dst, p1->x, p1->y, 0.5f,1); dst++;

			nvg__vset(dst, lx0, ly0, lu,1); dst++;
			nvg__vset(dst, lx0, ly0, lu,1); dst++;

			nvg__vset(dst, p1->x + dlx1*lw, p1->y + dly1*lw, lu,1); dst++;
			nvg__vset(dst, p1->x, p1->y, 0.5f,1); dst++;
		}

		nvg__vset(dst, p1->x + dlx1*lw, p1->y + dly1*lw, lu,1); dst++;
		nvg__vset(dst, rx1, ry1, ru,1); dst++;
	}

	return dst;
}

static NVGvertex* nvg__buttCapStart(NVGvertex* dst, NVGpoint* p,
									float dx, float dy, float w, float d,
									float aa, float u0, float u1)
{
	float px = p->x - dx*d;
	float py = p->y - dy*d;
	float dlx = dy;
	float dly = -dx;
	nvg__vset(dst, px + dlx*w - dx*aa, py + dly*w - dy*aa, u0,0); dst++;
	nvg__vset(dst, px - dlx*w - dx*aa, py - dly*w - dy*aa, u1,0); dst++;
	nvg__vset(dst, px + dlx*w, py + dly*w, u0,1); dst++;
	nvg__vset(dst, px - dlx*w, py - dly*w, u1,1); dst++;
	return dst;
}

static NVGvertex* nvg__buttCapEnd(NVGvertex* dst, NVGpoint* p,
								  float dx, float dy, float w, float d,
								  float aa, float u0, float u1)
{
	float px = p->x + dx*d;
	float py = p->y + dy*d;
	float dlx = dy;
	float dly = -dx;
	nvg__vset(dst, px + dlx*w, py + dly*w, u0,1); dst++;
	nvg__vset(dst, px - dlx*w, py - dly*w, u1,1); dst++;
	nvg__vset(dst, px + dlx*w + dx*aa, py + dly*w + dy*aa, u0,0); dst++;
	nvg__vset(dst, px - dlx*w + dx*aa, py - dly*w + dy*aa, u1,0); dst++;
	return dst;
}


static NVGvertex* nvg__roundCapStart(NVGvertex* dst, NVGpoint* p,
									 float dx, float dy, float w, int ncap,
									 float aa, float u0, float u1)
{
	int i;
	float px = p->x;
	float py = p->y;
	float dlx = dy;
	float dly = -dx;
	NVG_NOTUSED(aa);
	for (i = 0; i < ncap; i++) {
		float a = i/(float)(ncap-1)*NVG_PI;
		float ax = cosf(a) * w, ay = sinf(a) * w;
		nvg__vset(dst, px - dlx*ax - dx*ay, py - dly*ax - dy*ay, u0,1); dst++;
		nvg__vset(dst, px, py, 0.5f,1); dst++;
	}
	nvg__vset(dst, px + dlx*w, py + dly*w, u0,1); dst++;
	nvg__vset(dst, px - dlx*w, py - dly*w, u1,1); dst++;
	return dst;
}

static NVGvertex* nvg__roundCapEnd(NVGvertex* dst, NVGpoint* p,
								   float dx, float dy, float w, int ncap,
								   float aa, float u0, float u1)
{
	int i;
	float px = p->x;
	float py = p->y;
	float dlx = dy;
	float dly = -dx;
	NVG_NOTUSED(aa);
	nvg__vset(dst, px + dlx*w, py + dly*w, u0,1); dst++;
	nvg__vset(dst, px - dlx*w, py - dly*w, u1,1); dst++;
	for (i = 0; i < ncap; i++) {
		float a = i/(float)(ncap-1)*NVG_PI;
		float ax = cosf(a) * w, ay = sinf(a) * w;
		nvg__vset(dst, px, py, 0.5f,1); dst++;
		nvg__vset(dst, px - dlx*ax + dx*ay, py - dly*ax + dy*ay, u0,1); dst++;
	}
	return dst;
}


static void nvg__calculateJoins(NVGcontext* ctx, float w, int lineJoin, float miterLimit)
{
	NVGpathCache* cache = ctx->cache;
	int i, j;
	float iw = 0.0f;

	if (w > 0.0f) iw = 1.0f / w;

	// Calculate which joins needs extra vertices to append, and gather vertex count.
	for (i = 0; i < cache->npaths; i++) {
		NVGpath* path = &cache->paths[i];
		NVGpoint* pts = &cache->points[path->first];
		NVGpoint* p0 = &pts[path->count-1];
		NVGpoint* p1 = &pts[0];
		int nleft = 0;

		path->nbevel = 0;

		for (j = 0; j < path->count; j++) {
			float dlx0, dly0, dlx1, dly1, dmr2, cross, limit;
			dlx0 = p0->dy;
			dly0 = -p0->dx;
			dlx1 = p1->dy;
			dly1 = -p1->dx;
			// Calculate extrusions
			p1->dmx = (dlx0 + dlx1) * 0.5f;
			p1->dmy = (dly0 + dly1) * 0.5f;
			dmr2 = p1->dmx*p1->dmx + p1->dmy*p1->dmy;
			if (dmr2 > 0.000001f) {
				float scale = 1.0f / dmr2;
				if (scale > 600.0f) {
					scale = 600.0f;
				}
				p1->dmx *= scale;
				p1->dmy *= scale;
			}

			// Clear flags, but keep the corner.
			p1->flags = (p1->flags & NVG_PT_CORNER) ? NVG_PT_CORNER : 0;

			// Keep track of left turns.
			cross = p1->dx * p0->dy - p0->dx * p1->dy;
			if (cross > 0.0f) {
				nleft++;
				p1->flags |= NVG_PT_LEFT;
			}

			// Calculate if we should use bevel or miter for inner join.
			limit = nvg__maxf(1.01f, nvg__minf(p0->len, p1->len) * iw);
			if ((dmr2 * limit*limit) < 1.0f)
				p1->flags |= NVG_PR_INNERBEVEL;

			// Check to see if the corner needs to be beveled.
			if (p1->flags & NVG_PT_CORNER) {
				if ((dmr2 * miterLimit*miterLimit) < 1.0f || lineJoin == NVG_BEVEL || lineJoin == NVG_ROUND) {
					p1->flags |= NVG_PT_BEVEL;
				}
			}

			if ((p1->flags & (NVG_PT_BEVEL | NVG_PR_INNERBEVEL)) != 0)
				path->nbevel++;

			p0 = p1++;
		}

		path->convex = (nleft == path->count) ? 1 : 0;
	}
}


static int nvg__expandStroke(NVGcontext* ctx, float w, float fringe, int lineCap, int lineJoin, float miterLimit)
{
	NVGpathCache* cache = ctx->cache;
	NVGvertex* verts;
	NVGvertex* dst;
	int cverts, i, j;
	float aa = fringe;//ctx->fringeWidth;
	float u0 = 0.0f, u1 = 1.0f;
	int ncap = nvg__curveDivs(w, NVG_PI, ctx->tessTol);	// Calculate divisions per half circle.

	w += aa * 0.5f;

	// Disable the gradient used for antialiasing when antialiasing is not used.
	if (aa == 0.0f) {
		u0 = 0.5f;
		u1 = 0.5f;
	}

	nvg__calculateJoins(ctx, w, lineJoin, miterLimit);

	// Calculate max vertex usage.
	cverts = 0;
	for (i = 0; i < cache->npaths; i++) {
		NVGpath* path = &cache->paths[i];
		int loop = (path->closed == 0) ? 0 : 1;
		if (lineJoin == NVG_ROUND)
			cverts += (path->count + path->nbevel*(ncap+2) + 1) * 2; // plus one for loop
		else
			cverts += (path->count + path->nbevel*5 + 1) * 2; // plus one for loop
		if (loop == 0) {
			// space for caps
			if (lineCap == NVG_ROUND) {
				cverts += (ncap*2 + 2)*2;
			} else {
				cverts += (3+3)*2;
			}
		}
	}

	verts = nvg__allocTempVerts(ctx, cverts);
	if (verts == NULL) return 0;

	for (i = 0; i < cache->npaths; i++) {
		NVGpath* path = &cache->paths[i];
		NVGpoint* pts = &cache->points[path->first];
		NVGpoint* p0;
		NVGpoint* p1;
		int s, e, loop;
		float dx, dy;

		path->fill = 0;
		path->nfill = 0;

		// Calculate fringe or stroke
		loop = (path->closed == 0) ? 0 : 1;
		dst = verts;
		path->stroke = dst;

		if (loop) {
			// Looping
			p0 = &pts[path->count-1];
			p1 = &pts[0];
			s = 0;
			e = path->count;
		} else {
			// Add cap
			p0 = &pts[0];
			p1 = &pts[1];
			s = 1;
			e = path->count-1;
		}

		if (loop == 0) {
			// Add cap
			dx = p1->x - p0->x;
			dy = p1->y - p0->y;
			nvg__normalize(&dx, &dy);
			if (lineCap == NVG_BUTT)
				dst = nvg__buttCapStart(dst, p0, dx, dy, w, -aa*0.5f, aa, u0, u1);
			else if (lineCap == NVG_BUTT || lineCap == NVG_SQUARE)
				dst = nvg__buttCapStart(dst, p0, dx, dy, w, w-aa, aa, u0, u1);
			else if (lineCap == NVG_ROUND)
				dst = nvg__roundCapStart(dst, p0, dx, dy, w, ncap, aa, u0, u1);
		}

		for (j = s; j < e; ++j) {
			if ((p1->flags & (NVG_PT_BEVEL | NVG_PR_INNERBEVEL)) != 0) {
				if (lineJoin == NVG_ROUND) {
					dst = nvg__roundJoin(dst, p0, p1, w, w, u0, u1, ncap, aa);
				} else {
					dst = nvg__bevelJoin(dst, p0, p1, w, w, u0, u1, aa);
				}
			} else {
				nvg__vset(dst, p1->x + (p1->dmx * w), p1->y + (p1->dmy * w), u0,1); dst++;
				nvg__vset(dst, p1->x - (p1->dmx * w), p1->y - (p1->dmy * w), u1,1); dst++;
			}
			p0 = p1++;
		}

		if (loop) {
			// Loop it
			nvg__vset(dst, verts[0].x, verts[0].y, u0,1); dst++;
			nvg__vset(dst, verts[1].x, verts[1].y, u1,1); dst++;
		} else {
			// Add cap
			dx = p1->x - p0->x;
			dy = p1->y - p0->y;
			nvg__normalize(&dx, &dy);
			if (lineCap == NVG_BUTT)
				dst = nvg__buttCapEnd(dst, p1, dx, dy, w, -aa*0.5f, aa, u0, u1);
			else if (lineCap == NVG_BUTT || lineCap == NVG_SQUARE)
				dst = nvg__buttCapEnd(dst, p1, dx, dy, w, w-aa, aa, u0, u1);
			else if (lineCap == NVG_ROUND)
				dst = nvg__roundCapEnd(dst, p1, dx, dy, w, ncap, aa, u0, u1);
		}

		path->nstroke = (int)(dst - verts);

		verts = dst;
	}

	return 1;
}

static int nvg__expandFill(NVGcontext* ctx, float w, int lineJoin, float miterLimit)
{
	NVGpathCache* cache = ctx->cache;
	NVGvertex* verts;
	NVGvertex* dst;
	int cverts, convex, i, j;
	float aa = ctx->fringeWidth;
	int fringe = w > 0.0f;

	nvg__calculateJoins(ctx, w, lineJoin, miterLimit);

	// Calculate max vertex usage.
	cverts = 0;
	for (i = 0; i < cache->npaths; i++) {
		NVGpath* path = &cache->paths[i];
		cverts += path->count + path->nbevel + 1;
		if (fringe)
			cverts += (path->count + path->nbevel*5 + 1) * 2; // plus one for loop
	}

	verts = nvg__allocTempVerts(ctx, cverts);
	if (verts == NULL) return 0;

	convex = cache->npaths == 1 && cache->paths[0].convex;

	for (i = 0; i < cache->npaths; i++) {
		NVGpath* path = &cache->paths[i];
		NVGpoint* pts = &cache->points[path->first];
		NVGpoint* p0;
		NVGpoint* p1;
		float rw, lw, woff;
		float ru, lu;

		// Calculate shape vertices.
		woff = 0.5f*aa;
		dst = verts;
		path->fill = dst;

		if (fringe) {
			// Looping
			p0 = &pts[path->count-1];
			p1 = &pts[0];
			for (j = 0; j < path->count; ++j) {
				if (p1->flags & NVG_PT_BEVEL) {
					float dlx0 = p0->dy;
					float dly0 = -p0->dx;
					float dlx1 = p1->dy;
					float dly1 = -p1->dx;
					if (p1->flags & NVG_PT_LEFT) {
						float lx = p1->x + p1->dmx * woff;
						float ly = p1->y + p1->dmy * woff;
						nvg__vset(dst, lx, ly, 0.5f,1); dst++;
					} else {
						float lx0 = p1->x + dlx0 * woff;
						float ly0 = p1->y + dly0 * woff;
						float lx1 = p1->x + dlx1 * woff;
						float ly1 = p1->y + dly1 * woff;
						nvg__vset(dst, lx0, ly0, 0.5f,1); dst++;
						nvg__vset(dst, lx1, ly1, 0.5f,1); dst++;
					}
				} else {
					nvg__vset(dst, p1->x + (p1->dmx * woff), p1->y + (p1->dmy * woff), 0.5f,1); dst++;
				}
				p0 = p1++;
			}
		} else {
			for (j = 0; j < path->count; ++j) {
				nvg__vset(dst, pts[j].x, pts[j].y, 0.5f,1);
				dst++;
			}
		}

		path->nfill = (int)(dst - verts);
		verts = dst;

		// Calculate fringe
		if (fringe) {
			lw = w + woff;
			rw = w - woff;
			lu = 0;
			ru = 1;
			dst = verts;
			path->stroke = dst;

			// Create only half a fringe for convex shapes so that
			// the shape can be rendered without stenciling.
			if (convex) {
				lw = woff;	// This should generate the same vertex as fill inset above.
				lu = 0.5f;	// Set outline fade at middle.
			}

			// Looping
			p0 = &pts[path->count-1];
			p1 = &pts[0];

			for (j = 0; j < path->count; ++j) {
				if ((p1->flags & (NVG_PT_BEVEL | NVG_PR_INNERBEVEL)) != 0) {
					dst = nvg__bevelJoin(dst, p0, p1, lw, rw, lu, ru, ctx->fringeWidth);
				} else {
					nvg__vset(dst, p1->x + (p1->dmx * lw), p1->y + (p1->dmy * lw), lu,1); dst++;
					nvg__vset(dst, p1->x - (p1->dmx * rw), p1->y - (p1->dmy * rw), ru,1); dst++;
				}
				p0 = p1++;
			}

			// Loop it
			nvg__vset(dst, verts[0].x, verts[0].y, lu,1); dst++;
			nvg__vset(dst, verts[1].x, verts[1].y, ru,1); dst++;

			path->nstroke = (int)(dst - verts);
			verts = dst;
		} else {
			path->stroke = NULL;
			path->nstroke = 0;
		}
	}

	return 1;
}


// Draw
void nvgBeginPath(NVGcontext* ctx)
{
	ctx->ncommands = 0;
	nvg__clearPathCache(ctx);
}

void nvgMoveTo(NVGcontext* ctx, float x, float y)
{
	float vals[] = { NVG_MOVETO, x, y };
	nvg__appendCommands(ctx, vals, NVG_COUNTOF(vals));
}

void nvgLineTo(NVGcontext* ctx, float x, float y)
{
	float vals[] = { NVG_LINETO, x, y };
	nvg__appendCommands(ctx, vals, NVG_COUNTOF(vals));
}

void nvgBezierTo(NVGcontext* ctx, float c1x, float c1y, float c2x, float c2y, float x, float y)
{
	float vals[] = { NVG_BEZIERTO, c1x, c1y, c2x, c2y, x, y };
	nvg__appendCommands(ctx, vals, NVG_COUNTOF(vals));
}

void nvgQuadTo(NVGcontext* ctx, float cx, float cy, float x, float y)
{
    float x0 = ctx->commandx;
    float y0 = ctx->commandy;
    float vals[] = { NVG_BEZIERTO,
        x0 + 2.0f/3.0f*(cx - x0), y0 + 2.0f/3.0f*(cy - y0),
        x + 2.0f/3.0f*(cx - x), y + 2.0f/3.0f*(cy - y),
        x, y };
    nvg__appendCommands(ctx, vals, NVG_COUNTOF(vals));
}

void nvgArcTo(NVGcontext* ctx, float x1, float y1, float x2, float y2, float radius)
{
	float x0 = ctx->commandx;
	float y0 = ctx->commandy;
	float dx0,dy0, dx1,dy1, a, d, cx,cy, a0,a1;
	int dir;

	if (ctx->ncommands == 0) {
		return;
	}

	// Handle degenerate cases.
	if (nvg__ptEquals(x0,y0, x1,y1, ctx->distTol) ||
		nvg__ptEquals(x1,y1, x2,y2, ctx->distTol) ||
		nvg__distPtSeg(x1,y1, x0,y0, x2,y2) < ctx->distTol*ctx->distTol ||
		radius < ctx->distTol) {
		nvgLineTo(ctx, x1,y1);
		return;
	}

	// Calculate tangential circle to lines (x0,y0)-(x1,y1) and (x1,y1)-(x2,y2).
	dx0 = x0-x1;
	dy0 = y0-y1;
	dx1 = x2-x1;
	dy1 = y2-y1;
	nvg__normalize(&dx0,&dy0);
	nvg__normalize(&dx1,&dy1);
	a = nvg__acosf(dx0*dx1 + dy0*dy1);
	d = radius / nvg__tanf(a/2.0f);

//	printf("a=%f° d=%f\n", a/NVG_PI*180.0f, d);

	if (d > 10000.0f) {
		nvgLineTo(ctx, x1,y1);
		return;
	}

	if (nvg__cross(dx0,dy0, dx1,dy1) > 0.0f) {
		cx = x1 + dx0*d + dy0*radius;
		cy = y1 + dy0*d + -dx0*radius;
		a0 = nvg__atan2f(dx0, -dy0);
		a1 = nvg__atan2f(-dx1, dy1);
		dir = NVG_CW;
//		printf("CW c=(%f, %f) a0=%f° a1=%f°\n", cx, cy, a0/NVG_PI*180.0f, a1/NVG_PI*180.0f);
	} else {
		cx = x1 + dx0*d + -dy0*radius;
		cy = y1 + dy0*d + dx0*radius;
		a0 = nvg__atan2f(-dx0, dy0);
		a1 = nvg__atan2f(dx1, -dy1);
		dir = NVG_CCW;
//		printf("CCW c=(%f, %f) a0=%f° a1=%f°\n", cx, cy, a0/NVG_PI*180.0f, a1/NVG_PI*180.0f);
	}

	nvgArc(ctx, cx, cy, radius, a0, a1, dir);
}

void nvgClosePath(NVGcontext* ctx)
{
	float vals[] = { NVG_CLOSE };
	nvg__appendCommands(ctx, vals, NVG_COUNTOF(vals));
}

void nvgPathWinding(NVGcontext* ctx, int dir)
{
	float vals[] = { NVG_WINDING, (float)dir };
	nvg__appendCommands(ctx, vals, NVG_COUNTOF(vals));
}

void nvgArc(NVGcontext* ctx, float cx, float cy, float r, float a0, float a1, int dir)
{
	float a = 0, da = 0, hda = 0, kappa = 0;
	float dx = 0, dy = 0, x = 0, y = 0, tanx = 0, tany = 0;
	float px = 0, py = 0, ptanx = 0, ptany = 0;
	float vals[3 + 5*7 + 100];
	int i, ndivs, nvals;
	int move = ctx->ncommands > 0 ? NVG_LINETO : NVG_MOVETO;

	// Clamp angles
	da = a1 - a0;
	if (dir == NVG_CW) {
		if (nvg__absf(da) >= NVG_PI*2) {
			da = NVG_PI*2;
		} else {
			while (da < 0.0f) da += NVG_PI*2;
		}
	} else {
		if (nvg__absf(da) >= NVG_PI*2) {
			da = -NVG_PI*2;
		} else {
			while (da > 0.0f) da -= NVG_PI*2;
		}
	}

	// Split arc into max 90 degree segments.
	ndivs = nvg__maxi(1, nvg__mini((int)(nvg__absf(da) / (NVG_PI*0.5f) + 0.5f), 5));
	hda = (da / (float)ndivs) / 2.0f;
	kappa = nvg__absf(4.0f / 3.0f * (1.0f - nvg__cosf(hda)) / nvg__sinf(hda));

	if (dir == NVG_CCW)
		kappa = -kappa;

	nvals = 0;
	for (i = 0; i <= ndivs; i++) {
		a = a0 + da * (i/(float)ndivs);
		dx = nvg__cosf(a);
		dy = nvg__sinf(a);
		x = cx + dx*r;
		y = cy + dy*r;
		tanx = -dy*r*kappa;
		tany = dx*r*kappa;

		if (i == 0) {
			vals[nvals++] = (float)move;
			vals[nvals++] = x;
			vals[nvals++] = y;
		} else {
			vals[nvals++] = NVG_BEZIERTO;
			vals[nvals++] = px+ptanx;
			vals[nvals++] = py+ptany;
			vals[nvals++] = x-tanx;
			vals[nvals++] = y-tany;
			vals[nvals++] = x;
			vals[nvals++] = y;
		}
		px = x;
		py = y;
		ptanx = tanx;
		ptany = tany;
	}

	nvg__appendCommands(ctx, vals, nvals);
}

void nvgRect(NVGcontext* ctx, float x, float y, float w, float h)
{
	float vals[] = {
		NVG_MOVETO, x,y,
		NVG_LINETO, x,y+h,
		NVG_LINETO, x+w,y+h,
		NVG_LINETO, x+w,y,
		NVG_CLOSE
	};
	nvg__appendCommands(ctx, vals, NVG_COUNTOF(vals));
}

void nvgRoundedRect(NVGcontext* ctx, float x, float y, float w, float h, float r)
{
	nvgRoundedRectVarying(ctx, x, y, w, h, r, r, r, r);
}

void nvgRoundedRectVarying(NVGcontext* ctx, float x, float y, float w, float h, float radTopLeft, float radTopRight, float radBottomRight, float radBottomLeft)
{
	if(radTopLeft < 0.1f && radTopRight < 0.1f && radBottomRight < 0.1f && radBottomLeft < 0.1f) {
		nvgRect(ctx, x, y, w, h);
		return;
	} else {
		float halfw = nvg__absf(w)*0.5f;
		float halfh = nvg__absf(h)*0.5f;
		float rxBL = nvg__minf(radBottomLeft, halfw) * nvg__signf(w), ryBL = nvg__minf(radBottomLeft, halfh) * nvg__signf(h);
		float rxBR = nvg__minf(radBottomRight, halfw) * nvg__signf(w), ryBR = nvg__minf(radBottomRight, halfh) * nvg__signf(h);
		float rxTR = nvg__minf(radTopRight, halfw) * nvg__signf(w), ryTR = nvg__minf(radTopRight, halfh) * nvg__signf(h);
		float rxTL = nvg__minf(radTopLeft, halfw) * nvg__signf(w), ryTL = nvg__minf(radTopLeft, halfh) * nvg__signf(h);
		float vals[] = {
			NVG_MOVETO, x, y + ryTL,
			NVG_LINETO, x, y + h - ryBL,
			NVG_BEZIERTO, x, y + h - ryBL*(1 - NVG_KAPPA90), x + rxBL*(1 - NVG_KAPPA90), y + h, x + rxBL, y + h,
			NVG_LINETO, x + w - rxBR, y + h,
			NVG_BEZIERTO, x + w - rxBR*(1 - NVG_KAPPA90), y + h, x + w, y + h - ryBR*(1 - NVG_KAPPA90), x + w, y + h - ryBR,
			NVG_LINETO, x + w, y + ryTR,
			NVG_BEZIERTO, x + w, y + ryTR*(1 - NVG_KAPPA90), x + w - rxTR*(1 - NVG_KAPPA90), y, x + w - rxTR, y,
			NVG_LINETO, x + rxTL, y,
			NVG_BEZIERTO, x + rxTL*(1 - NVG_KAPPA90), y, x, y + ryTL*(1 - NVG_KAPPA90), x, y + ryTL,
			NVG_CLOSE
		};
		nvg__appendCommands(ctx, vals, NVG_COUNTOF(vals));
	}
}

void nvgEllipse(NVGcontext* ctx, float cx, float cy, float rx, float ry)
{
	float vals[] = {
		NVG_MOVETO, cx-rx, cy,
		NVG_BEZIERTO, cx-rx, cy+ry*NVG_KAPPA90, cx-rx*NVG_KAPPA90, cy+ry, cx, cy+ry,
		NVG_BEZIERTO, cx+rx*NVG_KAPPA90, cy+ry, cx+rx, cy+ry*NVG_KAPPA90, cx+rx, cy,
		NVG_BEZIERTO, cx+rx, cy-ry*NVG_KAPPA90, cx+rx*NVG_KAPPA90, cy-ry, cx, cy-ry,
		NVG_BEZIERTO, cx-rx*NVG_KAPPA90, cy-ry, cx-rx, cy-ry*NVG_KAPPA90, cx-rx, cy,
		NVG_CLOSE
	};
	nvg__appendCommands(ctx, vals, NVG_COUNTOF(vals));
}

void nvgCircle(NVGcontext* ctx, float cx, float cy, float r)
{
	nvgEllipse(ctx, cx,cy, r,r);
}

void nvgDebugDumpPathCache(NVGcontext* ctx)
{
	const NVGpath* path;
	int i, j;

	printf("Dumping %d cached paths\n", ctx->cache->npaths);
	for (i = 0; i < ctx->cache->npaths; i++) {
		path = &ctx->cache->paths[i];
		printf(" - Path %d\n", i);
		if (path->nfill) {
			printf("   - fill: %d\n", path->nfill);
			for (j = 0; j < path->nfill; j++)
				printf("%f\t%f\n", path->fill[j].x, path->fill[j].y);
		}
		if (path->nstroke) {
			printf("   - stroke: %d\n", path->nstroke);
			for (j = 0; j < path->nstroke; j++)
				printf("%f\t%f\n", path->stroke[j].x, path->stroke[j].y);
		}
	}
}

void nvgFill(NVGcontext* ctx)
{
	NVGstate* state = nvg__getState(ctx);
	const NVGpath* path;
	NVGpaint fillPaint = state->fill;
	int i;

	nvg__flattenPaths(ctx);
	if (ctx->params.edgeAntiAlias && state->shapeAntiAlias)
		nvg__expandFill(ctx, ctx->fringeWidth, NVG_MITER, 2.4f);
	else
		nvg__expandFill(ctx, 0.0f, NVG_MITER, 2.4f);

	// Apply global alpha
	fillPaint.innerColor.a *= state->alpha;
	fillPaint.outerColor.a *= state->alpha;

	if(ctx->cache->npaths > 0)
		ctx->params.renderFill(ctx->params.userPtr, &fillPaint, state->compositeOperation, ctx->fringeWidth,
							ctx->cache->bounds, ctx->cache->paths, ctx->cache->npaths);

	// Count triangles
	for (i = 0; i < ctx->cache->npaths; i++) {
		path = &ctx->cache->paths[i];
		ctx->fillTriCount += path->nfill-2;
		ctx->fillTriCount += path->nstroke-2;
		ctx->drawCallCount += 2;
	}
}

void nvgStroke(NVGcontext* ctx)
{
	NVGstate* state = nvg__getState(ctx);
	float scale = nvg__getAverageScale(state->xform);
	float strokeWidth = nvg__clampf(state->strokeWidth * scale, 0.0f, 200.0f);
	NVGpaint strokePaint = state->stroke;
	const NVGpath* path;
	int i;


	if (strokeWidth < ctx->fringeWidth) {
		// If the stroke width is less than pixel size, use alpha to emulate coverage.
		// Since coverage is area, scale by alpha*alpha.
		float alpha = nvg__clampf(strokeWidth / ctx->fringeWidth, 0.0f, 1.0f);
		strokePaint.innerColor.a *= alpha*alpha;
		strokePaint.outerColor.a *= alpha*alpha;
		strokeWidth = ctx->fringeWidth;
	}

	// Apply global alpha
	strokePaint.innerColor.a *= state->alpha;
	strokePaint.outerColor.a *= state->alpha;

	nvg__flattenPaths(ctx);

	if (ctx->params.edgeAntiAlias && state->shapeAntiAlias)
		nvg__expandStroke(ctx, strokeWidth*0.5f, ctx->fringeWidth, state->lineCap, state->lineJoin, state->miterLimit);
	else
		nvg__expandStroke(ctx, strokeWidth*0.5f, 0.0f, state->lineCap, state->lineJoin, state->miterLimit);

	if(ctx->cache->npaths > 0)
		ctx->params.renderStroke(ctx->params.userPtr, &strokePaint, state->compositeOperation, ctx->fringeWidth,
							 	strokeWidth, ctx->cache->paths, ctx->cache->npaths);

	// Count triangles
	for (i = 0; i < ctx->cache->npaths; i++) {
		path = &ctx->cache->paths[i];
		ctx->strokeTriCount += path->nstroke-2;
		ctx->drawCallCount++;
	}
}

static float nvg__quantize(float a, float d)
{
	return ((int)(a / d + 0.5f)) * d;
}
// vim: ft=c nu noet ts=4
