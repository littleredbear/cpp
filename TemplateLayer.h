//
//  TemplateLayer.h
//  LyingDragon
//
//  Created by reid on 15/10/13.
//  Copyright © 2015年 www.uqee.com. All rights reserved.
//

#import "CCLayer.h"
#import "cocos2d.h"
#import "FileDownloadEngine.h"
#import "DownloadNode.h"
#import "CommonFunction.h"
#import "CCSprite_Category.h"
#import "LyingDragon.h"
#import "CastleLayer.h"
#import "PriorityMenu.h"



@interface TemplateAttribute : NSObject

- (void) updateValue:(NSString *)value withType:(NSString *)type;
- (NSDictionary *) attributes;

+ (instancetype) defaultAttribute;

@end

@interface ColorAttribute : TemplateAttribute

@property (nonatomic, retain) UIColor *color;

@end

@interface FontAttribute : TemplateAttribute

@property (nonatomic, retain) NSString *font;
@property (nonatomic, assign) CGFloat size;


@end

//<-------------下划线对应数值----------------->
//------------------------------------------->
//typedef CF_OPTIONS(int32_t, CTUnderlineStyle) {
//    kCTUnderlineStyleNone           = 0x00,
//    kCTUnderlineStyleSingle         = 0x01,
//    kCTUnderlineStyleThick          = 0x02,
//    kCTUnderlineStyleDouble         = 0x09
//};


@interface UnderlineAttribute : TemplateAttribute

@property (nonatomic, assign)CTUnderlineStyle style;


@end

//typedef NS_ENUM(NSInteger, NSTextAlignment) {
//    NSTextAlignmentLeft      = 0,    // Visually left aligned
//#if TARGET_OS_IPHONE
//    NSTextAlignmentCenter    = 1,    // Visually centered
//    NSTextAlignmentRight     = 2,    // Visually right aligned
//#else /* !TARGET_OS_IPHONE */
//    NSTextAlignmentRight     = 1,    // Visually right aligned
//    NSTextAlignmentCenter    = 2,    // Visually centered
//#endif
//    NSTextAlignmentJustified = 3,    // Fully-justified. The last line in a paragraph is natural-aligned.
//    NSTextAlignmentNatural   = 4,    // Indicates the default alignment for script
//} NS_ENUM_AVAILABLE_IOS(6_0);

@interface ParagraphStyleAttribute : TemplateAttribute

@property (nonatomic, retain)NSMutableParagraphStyle *style;

@end

@interface TemplateParseNode : NSObject {
    NSMutableArray *_nextNodes;
}

- (void) addNextNode:(TemplateParseNode *)node;
- (id) parse;

+ (instancetype) node;

@end

@interface RichTextParseNode : TemplateParseNode

@property (nonatomic, retain)NSString *attributes;
@property (nonatomic, retain)NSString *text;

- (BOOL) hasText;
- (BOOL) isEmpty;

@end

@interface TemplateTextParser : NSObject 


- (NSMutableAttributedString *) parseText:(NSString *)text;
+ (TemplateTextParser *) parser;

@end



//<--------------格式说明---------------->
//<color=0xff0000, size=10, underline=1, alignment = 1>test</>
//<size =10 ><color = 0xffff00>test</>test1</>

@interface TemplateLabel : CCSprite {
    CGSize _constraintSize;
    NSMutableDictionary *_defaultAttributes;
}

@property (nonatomic, retain)NSMutableAttributedString *text;


- (void) setConstraintWith:(CGFloat)width;
- (void) setConstraintHeight:(CGFloat)height;
- (void) setRichText:(NSString *)text;
- (void) addRichText:(NSString *)text;
- (void) setDefaultFontSize:(float)size;
- (void) setDefaultFont:(NSString *)name size:(float)size;
- (void) setDefaultAlignment:(int)alignment;
- (void) generate;


@end


@interface TemplateCDTime : CCNode {
    time_t _cdTime;
    time_t _goneTime;
    time_t _startStamp;
    time_t _endStamp;
    id  _target;
    SEL _selector;
    BOOL _isActivate;
}


- (void) done;
- (BOOL) isActivate;
- (void) setCDTime:(time_t)time;
- (void) setEndStamp:(time_t)stamp;
- (void) setTarget:(id)target Selector:(SEL)selector;
- (void) updateTime:(time_t)t;
- (void) reset;
- (void) start;

@end



@interface LabelCDTime : TemplateCDTime {
    CCLabelTTF *_lbl;
    int _flag;
}

@property (nonatomic, retain)NSString *prefix;
@property (nonatomic, retain)NSString *suffix;
@property (nonatomic, retain)NSString *format;

- (void) setFormatFlag:(int)flag;           // 1111 ----> 表示全都显示 0011 ------>显示分钟和秒数
- (void) setLblColor:(ccColor3B)color Size:(CGFloat)size;

@end


@interface AdvancedItem : CCMenuItemSprite


@property (nonatomic, retain) CCLabelTTF *lbl;
@property (nonatomic, retain) id obj;


- (void) setLabel:(NSString *)label;
- (void) setLabel:(NSString *)label color:(ccColor3B)color size:(CGFloat)size;
- (void) setItemImage:(NSString *)normal;
- (void) setItemImage:(NSString *)normal Select:(NSString *)select;
- (void) setItemImage:(NSString *)normal Select:(NSString *)select Disable:(NSString *)disable;


@end

@class ShopItem;
@interface ShopItemObserver : NSObject

@property (nonatomic, assign)ShopItem *subject;

- (void) publish:(ShopItem *)subject;

+(ShopItemObserver *) observer;

@end

@interface ShopItem : AdvancedItem

@property (nonatomic, retain)CCLabelTTF *num;
@property (nonatomic, retain)DownloadableSprite *itemIcon;
@property (nonatomic, retain)CCSprite *pickIcon;
@property (nonatomic, retain)ShopItemObserver *observer;

- (void) setItemNum:(int)num;
- (void) setItemNum:(int)num color:(ccColor3B)color;
- (void) setItemIconImage:(NSString *)image;
- (void) setPickedIconImage:(NSString *)image;
- (void) show:(BOOL)show;

@end


@interface TemplateLayer : CCLayer<DownloadDelegate> {
    
}

@property (nonatomic, retain) CCSprite *background;
@property (nonatomic, retain) CCSprite *title;
@property (nonatomic, retain) CCSprite *titleback;
@property (nonatomic, retain) AdvancedItem *exitBtn;
@property (nonatomic, retain) PriorityMenu *menu;
@property (nonatomic, assign) int touchPriority;

- (NSMutableArray *) resourceArray;
- (NSString *) resourceDir;
- (void) prepareResource;
- (void) loadResource;
- (void) updateUI;

- (void) setBackGroundImage:(NSString *)back;
- (void) setBackGroundImage:(NSString *)back full:(BOOL)full;
- (void) setTitleImage:(NSString *)title;
- (void) setTitlebackImage:(NSString *)image;
- (void) setExitBtnImage:(NSString *)image;
- (void) setExitBtnImage:(NSString *)normal Select:(NSString *)select;
- (void) setExitBtnImage:(NSString *)normal Select:(NSString *)select Disable:(NSString *)disable;
- (void) setExitLabel:(NSString *)label;


@end


typedef enum {
    AlertButtonTypeConfirm = 1,                                             //确认按钮
    AlertButtonTypeCancel = 1 << 1,                                         //取消按钮
    AlertButtonTypeBoth = AlertButtonTypeConfirm | AlertButtonTypeCancel    //全部包含
} AlertButtonType;


@interface TemplateAlert : TemplateLayer

@property (nonatomic, assign) id confirmTarget;
@property (nonatomic, assign) SEL confirmSEL;       //确认回调
@property (nonatomic, assign) id cancelTarget;
@property (nonatomic, assign) SEL cancelSEL;        //取消回调
@property (nonatomic, retain) NSString *remind;
@property (nonatomic, assign) BOOL remindON;


- (void) buildButtons:(AlertButtonType)type remind:(NSString *)remind;

+ (TemplateAlert *) alertWithRichMsg:(NSString *)msg remind:(NSString *)remind;
- (void) buildMsgLabel:(NSString *)msg color:(ccColor3B)color size:(CGFloat)size;

+ (void) showRichMsg:(NSString *)msg;
+ (void) showRichMsg:(NSString *)msg target:(id)target sel:(SEL)selector;
+ (void) showRichMsg:(NSString *)msg target:(id)target sel:(SEL)selector remind:(NSString *)remind;


@end


@interface AdvancedScrollView : SWScrollView

@end



//--------------------------------------------
//<-------------------on probation------------


typedef NS_ENUM(int, LrbDrawType) {
    DrawTypeNone,
    DrawTypeText,
    DrawTypeImage
};

@interface LrbDrawInfo : NSObject

@property (nonatomic, retain) NSString *text;
@property (nonatomic, assign) CGRect drawRect;
@property (nonatomic, assign) BOOL isChunk;
@property (nonatomic, assign) LrbDrawType type;

- (int) fragmentNum;
- (BOOL) isEmpty;
- (CGSize) drawSize;
- (CGSize) fragmentSizeToFlag:(int)flag;
- (LrbDrawInfo *) fragmentFromFlag:(int)flag;
- (LrbDrawInfo *) fragmentToFlag:(int)flag;
- (void) drawInCurrentContext;
- (void) setDrawPoint:(CGPoint)pos;

+ (LrbDrawInfo *) drawInfo;

@end

@interface LrbTextDrawInfo : LrbDrawInfo

@property (nonatomic, retain) NSDictionary *attributes;

- (void) addAbsentAttributes:(NSDictionary *)attributes;

@end

@interface LrbImageDrawInfo : LrbDrawInfo

@property (nonatomic, retain) UIImage *image;

@end

@interface LrbParsedInfo : NSObject

@property (nonatomic, retain) LrbDrawInfo *drawInfo;
@property (nonatomic, retain) NSString *touchText;

- (void) mixInfo:(LrbParsedInfo *)info;
+ (LrbParsedInfo *) parsedInfo;

@end

@interface LrbTexture : NSObject

+ (CCTexture2D *) createTexture:(NSArray *)list size:(CGSize)size;

@end

@interface LrbLayout : NSObject

- (void) reset;
- (void) layoutElements:(NSArray *)elements;

@end

typedef NS_ENUM(int, LrbTextRoundType) {
    TextRoundTypeNone,
    TextRoundTypeTxtRound,
    TextRoundTypeImgTop,
    TextRoundTypeImgCenter,
    TextRoundTypeImgBottom
};

@interface LrbTextLayout : LrbLayout {
    float _x;
    float _y;
    float _lineHeight;
    
    NSMutableArray *_currentLine;
    
    NSMutableArray *_chunkList;
}

@property (nonatomic, assign) LrbTextRoundType roundType;
@property (nonatomic, readonly) NSMutableArray *touchList;
@property (nonatomic, readonly) NSMutableArray *drawList;
@property (nonatomic, readonly) float width;
@property (nonatomic, readonly) float height;
@property (nonatomic, assign) float limitWidth;
@property (nonatomic, assign) float limitHeight;

- (void) reset;
- (void) layoutElements:(NSArray *)elements;

@end

@interface LrbParseNode : NSObject {
    NSMutableArray *_nextNodes;
}


- (void) addNextNode:(LrbParseNode *)node;
- (id) parse;

+ (LrbParseNode *) parseNode;

@end

@interface LrbTextParseNode : LrbParseNode

@property (nonatomic, retain) NSString *attributes;
@property (nonatomic, retain) NSString *text;

- (BOOL) hasText;

@end

@interface LrbTextParser : NSObject

@property (nonatomic, readonly) NSMutableArray *list;

- (void) reset;
- (void) parseRichText:(NSString *)text;
- (void) setDefaultAttributes:(NSDictionary *)attributes;

@end

@interface LrbTouchObject : NSObject

@property (nonatomic, retain) id obj;
@property (nonatomic, retain) CCSprite *sprite;

+ (LrbTouchObject *) touchObject;

@end


//-----------------------格式说明-----------------
//<color= 0xffffff, size=10>text</><img = d890_4.png> </>
//<color = 0xffff00>test<size = 10>for</></>
@interface LrbMixSprite : CCSprite<CCTargetedTouchDelegate> {
    LrbTextLayout *_layout;
    LrbTextParser *_parser;
    NSMutableDictionary *_defaultAttributes;
    NSMutableArray *_touchList;
    id _target;
    SEL _selector;
    id _obj;
    BOOL _isMoved;
    LrbTouchObject *_current;
}

@property (nonatomic, retain) NSString *text;
@property (nonatomic, assign) int touchPriority;
@property (nonatomic, assign) BOOL isTouchEnabled;

- (void) setDefaultSize:(CGFloat)size;
- (void) setDefaultFont:(NSString *)name size:(CGFloat)size;
- (void) setDefaultColor:(int)color;
- (void) setDefaultAlignment:(NSTextAlignment)alignment;
- (void) setLimitWidth:(float)width;
- (void) setLimitHeight:(float)height;
- (void) setTextRoundType:(LrbTextRoundType)type;
- (void) generate;

- (void) setTarget:(id)target selector:(SEL)selector obj:(id)obj;

@end









