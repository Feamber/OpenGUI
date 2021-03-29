#if defined(YOGAPROP) || defined(TRSPROP) || defined(SHAPEPROP)
#define STYLEPROP(...)
#endif

#if !defined(YOGAPROP)
#define YOGAPROP STYLEPROP
#endif	

#if !defined(TRSPROP)
#define TRSPROP STYLEPROP
#endif	

#if !defined(SHAPEPROP)
#define SHAPEPROP STYLEPROP
#endif		

//			name,						index,			inherited,				type,				id,								initial
//Begin Yoga props
YOGAPROP(	flex,						0,				NonInherited,			float,				"flex",							YGUndefined					)
YOGAPROP(	flexGrow,					1,				NonInherited,			float,				"flex-grow",					0.f							)
YOGAPROP(	flexShrink,					2,				NonInherited,			float,				"flow-shrink",					1.f							)
YOGAPROP(	flexBasis,					3,				NonInherited,			YGValue,			"flex-basis",					YGValueAuto					)
YOGAPROP(	left,						4,				NonInherited,			YGValue,			"left",							YGValueAuto					)
YOGAPROP(	top,						5,				NonInherited,			YGValue,			"top",							YGValueAuto					)
YOGAPROP(	right,						6,				NonInherited,			YGValue,			"right",						YGValueAuto					)
YOGAPROP(	bottom,						7,				NonInherited,			YGValue,			"bottom",						YGValueAuto					)
YOGAPROP(	marginLeft,					8,				NonInherited,			YGValue,			"margin-left",					YGValueZero					)
YOGAPROP(	marginTop,					9,				NonInherited,			YGValue,			"margin-top",					YGValueZero					)
YOGAPROP(	marginRight,				10,				NonInherited,			YGValue,			"margin-right",					YGValueZero					)
YOGAPROP(	marginBottom,				11,				NonInherited,			YGValue,			"margin-bottom",				YGValueZero					)
YOGAPROP(	paddingLeft,				12,				NonInherited,			YGValue,			"padding-left",					YGValueZero					)
YOGAPROP(	paddingTop,					13,				NonInherited,			YGValue,			"padding-top",					YGValueZero					)
YOGAPROP(	paddingRight,				14,				NonInherited,			YGValue,			"padding-right",				YGValueZero					)
YOGAPROP(	paddingBottom,				15,				NonInherited,			YGValue,			"padding-bottom",				YGValueZero					)
YOGAPROP(	borderLeftWidth,			16,				NonInherited,			float,				"border-left-width",			0.f							)
YOGAPROP(	borderTopWidth,				17,				NonInherited,			float,				"border-top-width",				0.f							)
YOGAPROP(	borderRightWidth,			18,				NonInherited,			float,				"border-right-width",			0.f							)
YOGAPROP(	borderBottomWidth,			19,				NonInherited,			float,				"border-bottom-width",			0.f							)
YOGAPROP(	width,						20,				NonInherited,			YGValue,			"width",						YGValueAuto					)
YOGAPROP(	height,						21,				NonInherited,			YGValue,			"height",						YGValueAuto					)
YOGAPROP(	position,					22,				NonInherited,			YGPositionType,		"position",						YGPositionTypeRelative		)
YOGAPROP(	overflow,					23,				NonInherited,			YGOverflow,			"overflow",						YGOverflowVisible			)
YOGAPROP(	alignSelf,					24,				NonInherited,			YGAlign,			"align-self",					YGAlignAuto					)
YOGAPROP(	maxWidth,					25,				NonInherited,			YGValue,			"max-width",					YGValueUndefined			)
YOGAPROP(	maxHeight,					26,				NonInherited,			YGValue,			"max-height",					YGValueUndefined			)
YOGAPROP(	minWidth,					27,				NonInherited,			YGValue,			"min-width",					YGValueAuto					)
YOGAPROP(	minHeight,					28,				NonInherited,			YGValue,			"min-height",					YGValueAuto					)
YOGAPROP(	flexDirection,				29,				NonInherited,			YGFlexDirection,	"flex-direction",				YGFlexDirectionRow			)
YOGAPROP(	alignContent,				30,				NonInherited,			YGAlign,			"align-content",				YGAlignFlexStart			)
YOGAPROP(	alignItems,					31,				NonInherited,			YGAlign,			"align-items",					YGAlignStretch				)
YOGAPROP(	justifyContent,				32,				NonInherited,			YGJustify,			"justify-content",				YGJustifyFlexStart			)
YOGAPROP(	wrap,						33,				NonInherited,			YGWrap,				"wrap",							YGWrapNoWrap				)
YOGAPROP(	display,					34,				NonInherited,			YGDisplay,			"display",						YGDisplayFlex				)
//End Yoga props																							
//Begin draw props																														
STYLEPROP(	backgroundColor,			35,				Inherited,				Color4f,			"background-color",				Color4f(1.f,1.f,1.f,1.f)	)
SHAPEPROP(	fontSize,					36,				Inherited,				float,				"font-size",					0.f							)
SHAPEPROP(	borderTopLeftRadius,		37,				NonInherited,			YGValue,			"border-top-left-radius",		YGValueZero					)
SHAPEPROP(	borderTopRightRadius,		38,				NonInherited,			YGValue,			"border-top-right-radius",		YGValueZero					)
SHAPEPROP(	borderBottomRightRadius,	39,				NonInherited,			YGValue,			"border-bottom-right-radius",	YGValueZero					)
SHAPEPROP(	borderBottomLeftRadius,		40,				NonInherited,			YGValue,			"border-bottom-left-radius",	YGValueZero					)
TRSPROP(	translation,				41,				NonInherited,			Vector2f,			"translation",					Vector2f::vector_zero()		)
TRSPROP(	rotation,					42,				NonInherited,			float,				"rotation",						0							)
TRSPROP(	scale,						43,				NonInherited,			Vector2f,			"scale",						Vector2f::vector_one()		)
STYLEPROP(	backgroundImage,			44,				NonInherited,			std::string,		"background-image",				""							)
//End draw props

#undef TRSPROP
#undef YOGAPROP
#undef SHAPEPROP
#undef STYLEPROP