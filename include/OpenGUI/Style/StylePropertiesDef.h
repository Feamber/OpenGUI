#pragma once

//			name,				type,				id,								initial							inherited,	
//Begin Yoga props
#define YOGAPROP(F) \
F(	flex,						float,				"flex",							YGUndefined,					NonInherited	) \
F(	flexGrow,					float,				"flex-grow",					0.f,							NonInherited	) \
F(	flexShrink,					float,				"flow-shrink",					1.f,							NonInherited	) \
F(	flexBasis,					YGValue,			"flex-basis",					YGValueAuto,					NonInherited	) \
F(	left,						YGValue,			"left",							YGValueAuto,					NonInherited	) \
F(	top,						YGValue,			"top",							YGValueAuto,					NonInherited	) \
F(	right,						YGValue,			"right",						YGValueAuto,					NonInherited	) \
F(	bottom,						YGValue,			"bottom",						YGValueAuto,					NonInherited	) \
F(	marginLeft,					YGValue,			"margin-left",					YGValueZero,					NonInherited	) \
F(	marginTop,					YGValue,			"margin-top",					YGValueZero,					NonInherited	) \
F(	marginRight,				YGValue,			"margin-right",					YGValueZero,					NonInherited	) \
F(	marginBottom,				YGValue,			"margin-bottom",				YGValueZero,					NonInherited	) \
F(	paddingLeft,				YGValue,			"padding-left",					YGValueZero,					NonInherited	) \
F(	paddingTop,					YGValue,			"padding-top",					YGValueZero,					NonInherited	) \
F(	paddingRight,				YGValue,			"padding-right",				YGValueZero,					NonInherited	) \
F(	paddingBottom,				YGValue,			"padding-bottom",				YGValueZero,					NonInherited	) \
F(	borderLeftWidth,			float,				"border-left-width",			0.f,							NonInherited	) \
F(	borderTopWidth,				float,				"border-top-width",				0.f,							NonInherited	) \
F(	borderRightWidth,			float,				"border-right-width",			0.f,							NonInherited	) \
F(	borderBottomWidth,			float,				"border-bottom-width",			0.f,							NonInherited	) \
F(	width,						YGValue,			"width",						YGValueAuto,					NonInherited	) \
F(	height,						YGValue,			"height",						YGValueAuto,					NonInherited	) \
F(	position,					YGPositionType,		"position",						YGPositionTypeRelative,			NonInherited	) \
F(	overflow,					YGOverflow,			"overflow",						YGOverflowVisible,				NonInherited	) \
F(	alignSelf,					YGAlign,			"align-self",					YGAlignAuto,					NonInherited	) \
F(	maxWidth,					YGValue,			"max-width",					YGValueUndefined,				NonInherited	) \
F(	maxHeight,					YGValue,			"max-height",					YGValueUndefined,				NonInherited	) \
F(	minWidth,					YGValue,			"min-width",					YGValueAuto,					NonInherited	) \
F(	minHeight,					YGValue,			"min-height",					YGValueAuto,					NonInherited	) \
F(	flexDirection,				YGFlexDirection,	"flex-direction",				YGFlexDirectionRow,				NonInherited	) \
F(	alignContent,				YGAlign,			"align-content",				YGAlignFlexStart,				NonInherited	) \
F(	alignItems,					YGAlign,			"align-items",					YGAlignStretch,					NonInherited	) \
F(	justifyContent,				YGJustify,			"justify-content",				YGJustifyFlexStart,				NonInherited	) \
F(	wrap,						YGWrap,				"flex-wrap",					YGWrapNoWrap,					NonInherited	) \
F(	display,					YGDisplay,			"flex-display",					YGDisplayFlex,					NonInherited	) \

//End Yoga props																												
//Begin draw props		

#define SHAPEPROP(F) \
F(	fontSize,					float,				"font-size",					0.f,							Inherited		) \
F(	borderTopLeftRadius,		YGValue,			"border-top-left-radius",		YGValueZero,					NonInherited	) \
F(	borderTopRightRadius,		YGValue,			"border-top-right-radius",		YGValueZero,					NonInherited	) \
F(	borderBottomLeftRadius,		YGValue,			"border-bottom-left-radius",	YGValueZero,					NonInherited	) \
F(	borderBottomRightRadius,	YGValue,			"border-bottom-right-radius",	YGValueZero,					NonInherited	) \

#define TRSPROP(F) \
F(	translation,				Vector2f,			"translation",					Vector2f::vector_zero(),		NonInherited	) \
F(	rotation,					float,				"rotation",						0,								NonInherited	) \
F(	scale,						Vector2f,			"scale",						Vector2f::vector_one(),			NonInherited	) \

#define STYLEPROP(F) \
YOGAPROP(F) \
SHAPEPROP(F) \
TRSPROP(F) \
F(	backgroundColor,			Color4f,			"background-color",				Color4f(1.f,1.f,1.f,1.f),		Inherited		) \
F(	backgroundImage,			std::string,		"background-image",				"",								NonInherited	) \

//End draw props