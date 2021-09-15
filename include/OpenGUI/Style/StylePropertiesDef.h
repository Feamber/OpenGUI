#pragma once

//			name,				type,				id,								initial							
//Begin Yoga props
#define YOGAPROP(F) \
F(	flex,						float,				"flex",							YGUndefined,					) \
F(	flexGrow,					float,				"flex-grow",					0.f,							) \
F(	flexShrink,					float,				"flow-shrink",					1.f,							) \
F(	flexBasis,					YGValue,			"flex-basis",					YGValueAuto,					) \
F(	left,						YGValue,			"left",							YGValueAuto,					) \
F(	top,						YGValue,			"top",							YGValueAuto,					) \
F(	right,						YGValue,			"right",						YGValueAuto,					) \
F(	bottom,						YGValue,			"bottom",						YGValueAuto,					) \
F(	marginLeft,					YGValue,			"margin-left",					YGValueZero,					) \
F(	marginTop,					YGValue,			"margin-top",					YGValueZero,					) \
F(	marginRight,				YGValue,			"margin-right",					YGValueZero,					) \
F(	marginBottom,				YGValue,			"margin-bottom",				YGValueZero,					) \
F(	paddingLeft,				YGValue,			"padding-left",					YGValueZero,					) \
F(	paddingTop,					YGValue,			"padding-top",					YGValueZero,					) \
F(	paddingRight,				YGValue,			"padding-right",				YGValueZero,					) \
F(	paddingBottom,				YGValue,			"padding-bottom",				YGValueZero,					) \
F(	borderLeftWidth,			float,				"border-left-width",			0.f,							) \
F(	borderTopWidth,				float,				"border-top-width",				0.f,							) \
F(	borderRightWidth,			float,				"border-right-width",			0.f,							) \
F(	borderBottomWidth,			float,				"border-bottom-width",			0.f,							) \
F(	width,						YGValue,			"width",						YGValueAuto,					) \
F(	height,						YGValue,			"height",						YGValueAuto,					) \
F(	position,					YGPositionType,		"position",						YGPositionTypeRelative,			) \
F(	overflow,					YGOverflow,			"overflow",						YGOverflowVisible,				) \
F(	alignSelf,					YGAlign,			"align-self",					YGAlignAuto,					) \
F(	maxWidth,					YGValue,			"max-width",					YGValueUndefined,				) \
F(	maxHeight,					YGValue,			"max-height",					YGValueUndefined,				) \
F(	minWidth,					YGValue,			"min-width",					YGValueAuto,					) \
F(	minHeight,					YGValue,			"min-height",					YGValueAuto,					) \
F(	flexDirection,				YGFlexDirection,	"flex-direction",				YGFlexDirectionRow,				) \
F(	alignContent,				YGAlign,			"align-content",				YGAlignFlexStart,				) \
F(	alignItems,					YGAlign,			"align-items",					YGAlignStretch,					) \
F(	justifyContent,				YGJustify,			"justify-content",				YGJustifyFlexStart,				) \
F(	wrap,						YGWrap,				"flex-wrap",					YGWrapNoWrap,					) \
F(	display,					YGDisplay,			"flex-display",					YGDisplayFlex,					) \

//End Yoga props																								
//Begin draw props		
#define TEXTPROP(F) \
F(	fontSize,					float,				"font-size",					20.f,							) \


#define SHAPEPROP(F) \
F(	borderTopLeftRadius,		YGValue,			"border-top-left-radius",		YGValueZero,					) \
F(	borderTopRightRadius,		YGValue,			"border-top-right-radius",		YGValueZero,					) \
F(	borderBottomLeftRadius,		YGValue,			"border-bottom-left-radius",	YGValueZero,					) \
F(	borderBottomRightRadius,	YGValue,			"border-bottom-right-radius",	YGValueZero,					) \

#define TRSPROP(F) \
F(	translation,				Vector2f,			"translation",					Vector2f::vector_zero(),		) \
F(	rotation,					float,				"rotation",						0,								) \
F(	scale,						Vector2f,			"scale",						Vector2f::vector_one(),			) \

#define INHERITEDPROP(F) \
TEXTPROP(F) \

#define NONINHERITEDPROP(F) \
YOGAPROP(F) \
SHAPEPROP(F) \
TRSPROP(F) \
F(	backgroundColor,			Color4f,			"background-color",				Color4f(1.f,1.f,1.f,1.f),		) \
F(	backgroundImage,			std::string,		"background-image",				"",								) \

#define STYLEPROP(F) \
INHERITEDPROP(F) \
NONINHERITEDPROP(F) \


//End draw props