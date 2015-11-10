//
//  TemplateLayer.m
//  LyingDragon
//
//  Created by reid on 15/10/13.
//  Copyright © 2015年 www.uqee.com. All rights reserved.
//

#import "TemplateLayer.h"

@implementation TemplateAttribute

- (void) updateValue:(NSString *)value withType:(NSString *)type {
    
}

- (NSDictionary *) attributes {
    return [NSDictionary dictionary];
}

+ (instancetype) defaultAttribute {
    return [[[self alloc] init] autorelease];
}

@end


@implementation ColorAttribute

- (instancetype) init {
    if (self = [super init]) {
        self.color = [UIColor whiteColor];
    }
    
    return self;
}

- (void) dealloc {
    self.color = nil;
    [super dealloc];
}

- (void) updateValue:(NSString *)value withType:(NSString *)type {
    unsigned long val = strtoul([value UTF8String], 0, 0);
    self.color = [UIColor colorWithRed:(val >> 16 & 0xff)/255.0 green:(val >> 8 & 0xff)/255.0 blue:(val & 0xff)/255.0 alpha:1.0];
}


- (NSDictionary *) attributes {
    return [NSDictionary dictionaryWithObjectsAndKeys:self.color, NSForegroundColorAttributeName, nil];
}

@end


@implementation FontAttribute

- (instancetype) init {
    if(self = [super init]) {
        self.font = FONT3;
        self.size = 11.0;
    }
    return self;
}

- (void) dealloc {
    self.font = nil;
    [super dealloc];
}

- (void) setSize:(CGFloat)size {
    NSString *sizeStr = NSLocalizedString(@"current.fontsize.floatvalue", nil);
    CGFloat floatValue = [sizeStr floatValue];
    size = floatValue + size;
    _size = UNIVERSAL_RATE_VALUE(size) * CC_CONTENT_SCALE_FACTOR();
}

- (void) updateValue:(NSString *)value withType:(NSString *)type {
    if ([type containsString:@"size"]) {
        self.size = strtoul([value UTF8String], 0, 0);
    } else if ([type containsString:@"font"]) {
        self.font = value;
    }
}

- (NSDictionary *) attributes {
    UIFont *font = [UIFont fontWithName:self.font size:self.size];
    return [NSDictionary dictionaryWithObjectsAndKeys:font, NSFontAttributeName, nil];
}



@end

@implementation UnderlineAttribute

- (instancetype) init {
    if (self = [super init]) {
        self.style = NSUnderlineStyleNone;
    }
    return self;
}

- (NSDictionary *) attributes {
    return [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithInt:self.style], NSUnderlineStyleAttributeName, nil];
}

- (void) updateValue:(NSString *)value withType:(NSString *)type {
    self.style = strtoul([value UTF8String], 0, 0);
}


@end

@implementation ParagraphStyleAttribute

- (instancetype) init {
    if (self = [super init]) {
        self.style = [[[NSMutableParagraphStyle alloc] init] autorelease];
    }
    
    return self;
}

- (void) dealloc {
    self.style = nil;
    [super dealloc];
}

- (void) updateValue:(NSString *)value withType:(NSString *)type {
    if ([type containsString:@"alignment"]) {
        self.style.alignment = strtoul([value UTF8String], 0, 0);
    }
}

- (NSDictionary *) attributes {
    return [NSDictionary dictionaryWithObjectsAndKeys:self.style, NSParagraphStyleAttributeName, nil];
}

@end


@implementation TemplateParseNode

- (instancetype) init {
    if (self = [super init]) {
        _nextNodes = [[NSMutableArray alloc] init];
    }
    
    return self;
}

- (void) dealloc {
    [_nextNodes release];
    [super dealloc];
}

- (void) addNextNode:(TemplateParseNode *)node {
    [_nextNodes addObject:node];
}

- (id) parse {
    return nil;
}

+ (instancetype) node {
    return [[[self alloc] init] autorelease];
}


@end

@implementation RichTextParseNode

- (instancetype) init {
    if (self = [super init]) {
        self.attributes = nil;
        self.text = @"";
    }
    return self;
}

- (void) dealloc {
    self.attributes = nil;
    self.text = nil;
    [super dealloc];
}

- (BOOL) isEmpty {
    return ![self hasText] && self.attributes.length == 0;
}

- (BOOL) hasText {
    return self.text.length > 0 || _nextNodes.count > 0;
}

- (NSString *) attributeClassString:(NSString *)type {
    static NSDictionary *dict = nil;
    if (!dict) {
        dict = [[NSDictionary alloc] initWithObjectsAndKeys:@"font", @"size", @"ParagraphStyle", @"alignment", nil];
    }
    
    NSString *tmp = [dict objectForKey:type];
    if (!tmp)
        tmp = type;
    tmp = [tmp stringByReplacingCharactersInRange:NSMakeRange(0, 1) withString:[[tmp substringWithRange:NSMakeRange(0, 1)] uppercaseString]];
    return [tmp stringByAppendingString:@"Attribute"];
}

- (NSDictionary *) parseAttributes:(NSString *)attributes {
    NSMutableDictionary *dict = [NSMutableDictionary dictionary];
    
    if (!attributes || !attributes.length)
        return dict;
    
    NSMutableDictionary *tmp = [NSMutableDictionary dictionary];
    NSArray *list = [attributes componentsSeparatedByString:@","];
    
    for (NSString *attribute in list) {
        NSArray *l = [attribute componentsSeparatedByString:@"="];
        if (l.count != 2)
            continue;
        
        NSString *type = [[l objectAtIndex:0] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
        NSString *value = [[l objectAtIndex:1] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
        
        
        NSString *clsString = [self attributeClassString:type];
        TemplateAttribute *instance = (TemplateAttribute *)[tmp objectForKey:clsString];
        if (!instance) {
            Class cls = NSClassFromString(clsString);
            instance = [[[cls alloc] init] autorelease];
            [tmp setObject:instance forKey:clsString];
        }
        
        [instance updateValue:value withType:type];
        [dict addEntriesFromDictionary:[instance attributes]];
    }
    
    
    return dict;
}

- (id) parse {
    NSDictionary *attributes = [self parseAttributes:self.attributes];
    NSMutableAttributedString *last = [[NSMutableAttributedString alloc] initWithString:self.text attributes:attributes];
    for (RichTextParseNode *node in _nextNodes) {
        NSMutableAttributedString *s = [node parse];
        if (attributes.count) {
            NSRange r = NSMakeRange(0, s.length);
            [s addAbsentAttributes:attributes range:r];
        }
        
        [last appendAttributedString:s];
    }
    
    return last;
}

@end



@implementation TemplateTextParser


- (NSMutableAttributedString *) parseText:(NSString *)text {
                        //开始|属性|属性间文字｜结束符｜结束文字
    NSString *pattern = @"((?<=>|^)[^<]+(?=<|$))|((?<=<)[^>/]+(?=>))|(</>)";
    NSRegularExpression *regex = [NSRegularExpression regularExpressionWithPattern:pattern options:NSRegularExpressionAnchorsMatchLines error:0];
    
    __block NSMutableArray *list = [NSMutableArray array];
    RichTextParseNode *head = [RichTextParseNode node];
    [list addObject:head];
    
    [regex enumerateMatchesInString:text options:NSMatchingReportCompletion range:NSMakeRange(0, text.length) usingBlock:^(NSTextCheckingResult * result, NSMatchingFlags flags, BOOL * stop) {
        
        for (int i=1;i<result.numberOfRanges;++i) {
            NSRange r = [result rangeAtIndex:i];
            if (r.length) {
                RichTextParseNode *node = [list lastObject];
                switch (i) {
                    case 1:
                    {
                        if ([node hasText]) {
                            RichTextParseNode *n = [RichTextParseNode node];
                            n.text = [text substringWithRange:r];
                            [node addNextNode:n];
                        } else {
                            node.text = [text substringWithRange:r];
                        }
                    }
                        break;
                        
                    case 2:
                    {
                        RichTextParseNode *n = [RichTextParseNode node];
                        n.attributes = [text substringWithRange:r];
                        [node addNextNode:n];
                        [list addObject:n];
                    }
                        break;
                        
                    case 3:
                        [list removeLastObject];
                        break;
                        
                    default:
                        break;
                }
            }
            
        }
    }];
    
    return [head parse];
}

+ (TemplateTextParser *) parser {
    return [[[TemplateTextParser alloc] init] autorelease];
}

@end


@implementation TemplateLabel

- (instancetype) init {
    if (self = [super init]) {
        _constraintSize = CGSizeMake(CGFLOAT_MAX, CGFLOAT_MAX);
        FontAttribute *font = [FontAttribute defaultAttribute];
        ColorAttribute *color = [ColorAttribute defaultAttribute];
        _defaultAttributes = [[NSMutableDictionary alloc] init];
        [_defaultAttributes addEntriesFromDictionary:[font attributes]];
        [_defaultAttributes addEntriesFromDictionary:[color attributes]];
        self.text = [[[NSMutableAttributedString alloc] init] autorelease];
    }
    
    return self;
}

- (void) dealloc {
    self.text = nil;
    [_defaultAttributes release];
    [super dealloc];
}

- (NSMutableDictionary *) defaultAttributes {
    
    return _defaultAttributes;
}

- (void) setDefaultFontSize:(float)size {
    [self setDefaultFont:FONT3 size:size];
}

- (void) setDefaultFont:(NSString *)name size:(float)size {
    FontAttribute *font = [FontAttribute defaultAttribute];
    font.size = size;
    font.font = name;
    [_defaultAttributes addEntriesFromDictionary:[font attributes]];
}
- (void) setDefaultAlignment:(int)alignment {
    ParagraphStyleAttribute *style = [ParagraphStyleAttribute defaultAttribute];
    [style updateValue:[NSString stringWithFormat:@"%d", alignment] withType:@"alignment"];
    [_defaultAttributes addEntriesFromDictionary:[style attributes]];
}

- (void) setConstraintWith:(CGFloat)width {
    _constraintSize = CGSizeMake(width * CC_CONTENT_SCALE_FACTOR(), _constraintSize.height);
}

- (void) setConstraintHeight:(CGFloat)height {
    _constraintSize = CGSizeMake(_constraintSize.width, height * CC_CONTENT_SCALE_FACTOR());
}

- (void) setRichText:(NSString *)text {
    TemplateTextParser *parser = [TemplateTextParser parser];
    NSMutableAttributedString *s = [parser parseText:text];
    [s addAbsentAttributes:[self defaultAttributes] range:NSMakeRange(0, s.length)];
    self.text = s;
}

- (void) addRichText:(NSString *)text {
    TemplateTextParser *parser = [TemplateTextParser parser];
    NSMutableAttributedString *s = [parser parseText:text];
    [self.text appendAttributedString:s];
}

- (void) generate {
    if (!_text.length)
        return;
    
    CGSize fitSize = [_text boundingRectWithSize:_constraintSize options:NSStringDrawingUsesFontLeading |NSStringDrawingUsesLineFragmentOrigin context:nil].size;

    NSUInteger width = ceil(fitSize.width);
    NSUInteger height = ceil(fitSize.height);
    
    UInt32 *data = calloc(width * height, sizeof(UInt32));
    
    CGColorSpaceRef *space = CGColorSpaceCreateDeviceRGB();
    
    CGContextRef context = CGBitmapContextCreate(data, width, height, 8, width * 4, space, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    
    if (context) {
        CGContextTranslateCTM(context, 0.0f, height);
        CGContextScaleCTM(context, 1.0f, -1.0f);
        UIGraphicsPushContext(context);
        
        [_text drawWithRect:CGRectMake(0, 0, width, height) options:NSStringDrawingUsesFontLeading | NSStringDrawingUsesLineFragmentOrigin context:nil];
        
        CCTexture2D *tex = [[CCTexture2D alloc] initWithData:data pixelFormat:kCCTexture2DPixelFormat_RGBA8888 pixelsWide:width pixelsHigh:height contentSize:CGSizeMake(width, height)];
        [self setTexture:tex];
        [self setTextureRect:CGRectMake(0, 0, tex.contentSize.width, tex.contentSize.height)];
        [tex release];
        
        UIGraphicsPopContext();
    }
    
    CGColorSpaceRelease(space);
    CGContextRelease(context);
    free(data);
}

@end



@implementation TemplateCDTime

- (instancetype) init {
    if (self = [super init]) {
        [self reset];
    }
    return self;
}

- (void) setCDTime:(time_t)time {
    _cdTime = time;
}

- (void) setEndStamp:(time_t)stamp {
    _endStamp = stamp;
}

- (void) checkTime {
    _goneTime += 1;
    time_t left = _endStamp - time(0);
    left = MIN(left, _cdTime - _goneTime);
    if (_goneTime >= _cdTime || left <= 0) {
        [self done];
        if (_target && [_target respondsToSelector:_selector])
            [_target performSelector:_selector];
        return;
    }
    
    [self updateTime:left];
}

- (void) updateTime:(time_t)t {
    
}

- (void) done {
    [self reset];
    self.visible = NO;
}

- (BOOL) isActivate {
    return _isActivate;
}

- (void) setTarget:(id)target Selector:(SEL)selector {
    _target = target;
    _selector = selector;
}

- (void) reset {
    [self unscheduleAllSelectors];
    _isActivate = NO;
    _cdTime = 0;
    _goneTime = 0;
    _startStamp = 0;
    _endStamp = 0;
}

- (void) start {
    _isActivate = YES;
    self.visible = YES;
    [self unscheduleAllSelectors];
    time(&_startStamp);
    if (_endStamp > 0)
        _cdTime = _endStamp - _startStamp;
    else if (_cdTime > 0)
        _endStamp = _cdTime + _startStamp;
    
    if (_endStamp <= _startStamp)
        [self done];
    else
        [self schedule:@selector(checkTime) interval:1];
}

@end


@implementation LabelCDTime

- (instancetype) init {
    if (self = [super init]) {
        _flag = 0;
        self.prefix = @"";
        self.suffix = @"";
    }
    
    return self;
}

- (void) dealloc {
    self.prefix = nil;
    self.suffix = nil;
    self.format = nil;
    [super dealloc];
}

- (void) setFormatFlag:(int)flag {
    _flag = flag;
}

- (void) updateTime:(time_t)t {
    int sec = t % 60;
    int min = (t / 60) % 60;
    int hour = (t / 3600) % 24;
    int day = t / 3600 / 24;
    
    NSString *d = _flag >> 3 & 1 ? [NSString stringWithFormat:@"%02d", day] : @"";
    NSString *h = _flag >> 2 & 1 ? [NSString stringWithFormat:@"%02d", hour] : @"";
    NSString *m = _flag >> 1 & 1 ? [NSString stringWithFormat:@"%02d", min] : @"";
    NSString *s = _flag & 1 ? [NSString stringWithFormat:@"%02d", sec] : @"";
    
    
    NSString *l = [NSString stringWithFormat:self.format, d, h, m, s];
    _lbl.string = [NSString stringWithFormat:@"%@%@%@", self.prefix, l, self.suffix];
    
    self.contentSize = _lbl.contentSize;
}

- (void) setLblColor:(ccColor3B)color Size:(CGFloat)size {
    if (_lbl)
        [_lbl removeFromParentAndCleanup:YES];
    
    _lbl = [CCLabelTTF labelWithString:@"" fontName:FONT4 fontSize:size];
    _lbl.anchorPoint = CGPointZero;
    _lbl.position = CGPointZero;
    [self addChild:_lbl];
    _lbl.color = color;
}

- (void) reset {
    [super reset];
    _lbl.string = @"";
    self.contentSize = _lbl.contentSize;
}


@end


@implementation AdvancedItem

- (void) dealloc {
    self.lbl = nil;
    self.obj = nil;
    [super dealloc];
}

- (void) setLabel:(NSString *)label {
    self.lbl ? self.lbl.string = label : [self setLabel:label size:10];
}

- (void) setLabel:(NSString *)label size:(CGFloat)size {
    [self setLabel:label color:ccWHITE size:size];
}

- (void) setLabel:(NSString *)label color:(ccColor3B)color size:(CGFloat)size {
    if (!self.lbl) {
        self.lbl = [CCLabelTTF labelWithString:label fontName:FONT4 fontSize:size];
        self.lbl.color = color;
        self.lbl.position = [self centerPos];
        [self addChild:self.lbl];
    } else {
        self.lbl.string = label;
        self.lbl.color = color;
    }
}

- (void) setItemImage:(NSString *)normal {
    [self setItemImage:normal Select:nil];
}

- (void) setItemImage:(NSString *)normal Select:(NSString *)select {
    [self setItemImage:normal Select:select Disable:nil];
}

- (void) setItemImage:(NSString *)normal Select:(NSString *)select Disable:(NSString *)disable {
    if (!normal)
        return;
    
    CCSprite *nor = [CCSprite spriteWithFile:normal];
    [self setNormalImage:nor];
    [self setContentSize:nor.contentSize];
    
    if (!select)
        return;
    CCSprite *sel = [CCSprite spriteWithFile:select];
    [self setSelectedImage:sel];
    
    if (!disable)
        return;
    CCSprite *dis = [CCSprite spriteWithFile:disable];
    [self setDisabledImage:dis];
}

@end

@implementation ShopItemObserver

- (void) publish:(ShopItem *)subject {
    [self.subject show:NO];
    self.subject = subject;
    [self.subject show:YES];
}

+(ShopItemObserver *) observer {
    return [[[self alloc] init] autorelease];
}

@end

@implementation ShopItem


- (void) dealloc {
    self.num = nil;
    self.pickIcon = nil;
    self.observer = nil;
    self.itemIcon = nil;
    [super dealloc];
}

- (void) activate {
    if (!isEnabled_)
        return;
    
    [self.observer publish:self];
    [super activate];
}

- (void) show:(BOOL)show {
    self.pickIcon.visible = show;
}

- (void) setItemNum:(int)num {
    [self setItemNum:num color:self.num ? self.num.color : ccWHITE];
}

- (void) setItemNum:(int)num color:(ccColor3B)color {
    if (!self.num) {
        self.num = [CCLabelTTF labelWithString:@"" fontName:FONT4 fontSize:10];
        self.num.anchorPoint = ccp(1, 0);
        self.num.position = ccp(self.contentSize.width-UNIVERSAL_RATE_VALUE(10), UNIVERSAL_RATE_VALUE(7));
        [self addChild:self.num z:1];
    }
    
    self.num.string = [NSString stringWithFormat:@"%d", num];
    self.num.color = color;
}

- (void) setItemIconImage:(NSString *)image {
    
    if (!self.itemIcon) {
        self.itemIcon = [DownloadableSprite node];
        self.itemIcon.position = [self centerPos];
        [self addChild:self.itemIcon];
    }
   
    [self.itemIcon setDownloadImage:image];
}

- (void) setPickedIconImage:(NSString *)image {
    if (!self.pickIcon) {
        self.pickIcon = [CCSprite node];
        self.pickIcon.anchorPoint = CGPointZero;
        self.pickIcon.position = CGPointZero;
        [self addChild:self.pickIcon z:2];
        self.pickIcon.visible = NO;
    }
    
    [self.pickIcon setImage:image];
    self.contentSize = self.pickIcon.contentSize;
}


@end


@implementation TemplateLayer


- (instancetype) init {
    if (self = [super init]) {
        self.touchPriority = -128;
        [self buildUI];
        [self prepareResource];
    }
    
    return self;
}

- (void) dealloc {
    self.background = nil;
    self.title = nil;
    self.titleback = nil;
    self.exitBtn = nil;
    self.menu = nil;
    [super dealloc];
}

- (NSMutableArray *) resourceArray {
    return [NSMutableArray array];
}

- (NSString *) resourceDir {
    return @"";
}

- (void) prepareResource {
    CGSize winSize = [[CCDirector sharedDirector] winSize];
    
    NSMutableArray *arr = [self resourceArray];
    FileDownloadEngine *engine = [FileDownloadEngine sharedEngine];
    [engine checkFiles:arr];
    
    if(arr.count)
    {
        NSString *dir  = [self resourceDir];
        DownloadNode *loading = [[[DownloadNode alloc] initFiles:arr InDir:dir UseQueue:YES Keyword:dir Delegate:self] autorelease];
        loading.position = ccp(winSize.width/2,winSize.height/2);
        loading.delegate = self;
        [self addChild:loading z:66];
    }else {
        [self loadResource];
        [self updateUI];
    }
    
}

- (void) buildUI {
    self.background = [CCSprite node];
    self.background.anchorPoint = ccp(0.5, 0.5);
    CGSize size = [[CCDirector sharedDirector] winSize];
    self.background.position = ccp(size.width/2, size.height/2);
    [self addChild:self.background];
    
    
    self.titleback = [CCSprite node];
    self.titleback.anchorPoint = ccp(0.5, 1);
    self.titleback.position = ccp(size.width/2, size.height);
    
    [self addChild:self.titleback];
    
    self.title = [CCSprite node];
    self.title.anchorPoint = ccp(0.5, 0.5);
    self.title.position = ccp(0, 0);
    [self.titleback addChild:self.title];
    
    
    self.menu = [PriorityMenu menuWithItems:nil];
    self.menu.anchorPoint = CGPointZero;
    self.menu.position = CGPointZero;
    [self addChild:self.menu z:1];
    
    self.exitBtn = [AdvancedItem itemWithTarget:self selector:@selector(exitLayer)];
    self.exitBtn.anchorPoint = ccp(1, 1);
    self.exitBtn.position = ccp(size.width, size.height);
    [self.menu addChild:self.exitBtn];
    
    
}

- (void) loadResource {
    
}

- (void) updateUI {
    
}

- (void) setTouchPriority:(int)touchPriority {
    _touchPriority = touchPriority;
    if (self.isTouchEnabled && isRunning_) {
        [[CCTouchDispatcher sharedDispatcher] setPriority:touchPriority forDelegate:self];
    } else {
        self.isTouchEnabled = YES;
    }
}

- (void) registerWithTouchDispatcher {
    [[CCTouchDispatcher sharedDispatcher] addTargetedDelegate:self priority:self.touchPriority swallowsTouches:YES];
}

- (BOOL) ccTouchBegan:(UITouch *)touch withEvent:(UIEvent *)event {
    return YES;
}

- (void) exitLayer {
    [self removeFromParentAndCleanup:YES];
}

- (void) setBackGroundImage:(NSString *)back {
    [self setBackGroundImage:back full:NO];
}

- (void) setBackGroundImage:(NSString *)back full:(BOOL)full {
    [self.background setImage:back];
    if (full && self.background.contentSize.width > 0 && self.background.contentSize.height > 0) {
        CGSize winsize = [[CCDirector sharedDirector] winSize];
        float scaleX = winsize.width/self.background.contentSize.width;
        float scaleY = winsize.height/self.background.contentSize.height;
        self.background.scale = MAX(scaleX, scaleY);
    }
}

- (void) setTitleImage:(NSString *)title {
    [self.title setImage:title];
}

- (void) setTitlebackImage:(NSString *)image {
    [self.titleback setImage:image];
    self.title.position = ccp(self.titleback.contentSize.width*self.titleback.anchorPoint.x,
                              self.titleback.contentSize.height/2);
}

- (void) setExitBtnImage:(NSString *)image {
    [self setExitBtnImage:image Select:nil];
}

- (void) setExitBtnImage:(NSString *)normal Select:(NSString *)select {
    [self setExitBtnImage:normal Select:select Disable:nil];
}

- (void) setExitBtnImage:(NSString *)normal Select:(NSString *)select Disable:(NSString *)disable {
    [self.exitBtn setItemImage:normal Select:select Disable:disable];
}

- (void) setExitLabel:(NSString *)label {
    [self.exitBtn setLabel:label color:ccWHITE size:11];
}

-(void)downloadFailed{
    [CommonFunction accept:NSLocalizedString(@"download.resource.errortext", nil) Title1:NSLocalizedString(@"common.label.confirm", nil)];
    [self exitLayer];
}


-(void)downloadFinished{
    [self loadResource];
    [self updateUI];
}

@end


@implementation TemplateAlert

- (void) dealloc {
    self.remind = nil;
    [super dealloc];
}

- (void) updateUI {
    self.touchPriority = -200;
    self.menu.touchPriority = -201;
}

- (void) buildRichMsgLabel:(NSString *)msg {
    NSInteger width = 0;
    if (TTIsPad()) {
        width = self.background.contentSize.width - UNIVERSAL_RATE_VALUE(10);
    }else{
        width = self.background.contentSize.width - UNIVERSAL_RATE_VALUE(10);
    }
    
    TemplateLabel *lbl = [TemplateLabel node];
    [lbl setConstraintWith:width];
    [lbl setDefaultAlignment:NSTextAlignmentCenter];
    [lbl setRichText:msg];
    [lbl generate];
    lbl.anchorPoint = ccp(0.5, 1);
    lbl.position = ccp(self.background.contentSize.width/2, self.background.contentSize.height - UNIVERSAL_RATE_VALUE(10));
    [self.background addChild:lbl];
}

- (void) buildMsgLabel:(NSString *)msg color:(ccColor3B)color size:(CGFloat)size{
    NSInteger width = 0;
    if (TTIsPad()) {
        width = self.background.contentSize.width / 2 - UNIVERSAL_RATE_VALUE(5);
    }else{
        width = self.background.contentSize.width - UNIVERSAL_RATE_VALUE(5);
    }
    
    CCLabelTTF *lbl = [CCLabelTTF labelWithString:msg dimensions:CGSizeMake(width, 60) alignment:UITextAlignmentCenter fontName:FONT4 fontSize:size];
    lbl.color = color;
    lbl.anchorPoint = ccp(0.5, 1);
    lbl.position = ccp(self.background.contentSize.width/2, self.background.contentSize.height * 0.9);
    [self.background addChild:lbl];
}

- (void) buildButtons:(AlertButtonType)type remind:(NSString *)remind {
    AdvancedItem *confirm = [AdvancedItem itemWithTarget:self selector:@selector(btnConfirm)];
    [confirm setItemImage:@"c48_1.png" Select:@"c48_2.png"];
    [confirm setLabel:NSLocalizedString(@"common.label.confirm", nil) color:ccc3(255, 255, 153) size:12];
    [self.menu addChild:confirm];
    
    switch (type) {
        case AlertButtonTypeConfirm:
        {
            confirm.position = UNIVERSAL_OFFSET_POSITION(ccp(240, 137));
        }
            break;
            
        case AlertButtonTypeBoth:
        {
            confirm.position = UNIVERSAL_OFFSET_POSITION(ccp(179, 137));
            self.exitBtn.anchorPoint = ccp(0.5, 0.5);
            [self.exitBtn setItemImage:@"c47_1.png" Select:@"c47_2.pn"];
            [self.exitBtn setLabel:NSLocalizedString(@"common.label.cancel", nil) color:ccc3(255, 255, 153) size:12];
            self.exitBtn.position = UNIVERSAL_OFFSET_POSITION(ccp(300, 137));
        }
            break;
            
        default:
            break;
    }
    
    if (!remind.length)
        return;
    
    self.remind = remind;
    CCMenuItemImage *normalItem = [CCMenuItemImage itemFromNormalImage:@"c206_1.png" selectedImage:@"c206_1.png"];
    CCMenuItemImage *selectedItem = [CCMenuItemImage itemFromNormalImage:@"c206_2.png" selectedImage:@"c206_2.png"];
    CCSprite *mask = [CCSprite spriteWithFile:@"d603.png"];
    [selectedItem addChild:mask];
    mask.position = ccp(selectedItem.contentSize.width / 2, selectedItem.contentSize.height / 2);
    
    CCMenuItemToggle *itemToggle = [CCMenuItemToggle itemWithTarget:self selector:@selector(btnRemind) items:normalItem, selectedItem, nil];
    itemToggle.userData = remind;
    itemToggle.position = UNIVERSAL_OFFSET_POSITION(ccp(200, 167));
    self.remindON = NO;
    
    CCLabelTTF *remindLabel = [CCLabelTTF labelWithString:LOCALSTRING(@"common.defaultsetting.notip") fontName:FONT4 fontSize:10];
    remindLabel.anchorPoint = ccp(0, 0.5);
    remindLabel.position = ccp(itemToggle.position.x + itemToggle.contentSize.width/2 + UNIVERSAL_RATE_VALUE(2), itemToggle.position.y);
    //remindLabel.position = UNIVERSAL_RATE_POSITION(ccp(137, 64));
    remindLabel.color = ccc3(255, 255, 153);
    [self addChild:remindLabel];
    
    [self.menu addChild:itemToggle];
}

- (void) btnRemind {
    self.remindON = !self.remindON;
}

- (void) btnConfirm {
    if (self.remind.length && self.remindON) {
        [[SharedDataSource createSharedDataSource].remindDict setObject:[NSNumber numberWithBool:YES] forKey:self.remind];
    }
    
    [self removeFromParentAndCleanup:YES];
    if (self.confirmTarget && self.confirmSEL && [self.confirmTarget respondsToSelector:self.confirmSEL])
        [self.confirmTarget performSelector:self.confirmSEL];
}

- (void) exitLayer {
    [self removeFromParentAndCleanup:YES];
    if (self.cancelTarget && self.cancelSEL && [self.cancelTarget respondsToSelector:self.cancelSEL])
        [self.cancelTarget performSelector:self.cancelSEL];
}

+ (void) showRichMsg:(NSString *)msg {
    [TemplateAlert showRichMsg:msg target:nil sel:nil];
}

+ (void) showRichMsg:(NSString *)msg target:(id)target sel:(SEL)selector {
    [TemplateAlert showRichMsg:msg target:target sel:selector remind:nil];
}

+ (void) showRichMsg:(NSString *)msg target:(id)target sel:(SEL)selector remind:(NSString *)remind {
    if (remind.length && [[[SharedDataSource createSharedDataSource].remindDict objectForKey:remind] boolValue]) {
        if (target && selector && [target respondsToSelector:selector])
            [target performSelector:selector];
        
        return;
    }
    
    TemplateAlert *alert = [TemplateAlert alertWithRichMsg:msg remind:remind];
    alert.confirmTarget = target;
    alert.confirmSEL = selector;
    [[[CCDirector sharedDirector] runningScene] addChild:alert];
}

+ (TemplateAlert *) alertWithRichMsg:(NSString *)msg remind:(NSString *)remind {
    TemplateAlert *alert = [TemplateAlert node];
    if (remind.length)
        [alert setBackGroundImage:@"a386.png"];
    else
        [alert setBackGroundImage:@"a328.png"];
    [alert buildRichMsgLabel:msg];
    [alert buildButtons:AlertButtonTypeBoth remind:remind];
    
    return alert;
}



@end


@implementation AdvancedScrollView

-(void)beforeDraw {
    if (clipsToBounds_) {
        glEnable(GL_SCISSOR_TEST);
        const CGFloat s = [[CCDirector sharedDirector] contentScaleFactor];
        CGPoint p = [self convertToWorldSpace:CGPointZero];
        glScissor(p.x*s, p.y*s, viewSize_.width*s, viewSize_.height*s);
    }
}

@end







@implementation LrbDrawInfo

- (instancetype) init {
    if (self = [super init]) {
        self.isChunk = NO;
        self.type = DrawTypeNone;
        self.drawRect = CGRectZero;
    }
    return self;
}

- (void) dealloc {
    self.text = nil;
    [super dealloc];
}

- (int) fragmentNum {
    return 1;
}

- (BOOL) isEmpty {
    return YES;
}

- (CGSize) drawSize {
    return self.drawRect.size;
}

- (CGSize) fragmentSizeToFlag:(int)flag {
    return self.drawRect.size;
}

- (LrbDrawInfo *) fragmentFromFlag:(int)flag {
    return nil;
}

- (LrbDrawInfo *) fragmentToFlag:(int)flag {
    return self;
}

- (void) drawInCurrentContext {
    
}

- (void) setDrawPoint:(CGPoint)pos {
    CGRect rect = self.drawRect;
    rect.origin = pos;
    self.drawRect = rect;
}

+ (LrbDrawInfo *) drawInfo {
    return [[[self alloc] init] autorelease];
}

@end

@implementation LrbTextDrawInfo

- (instancetype) init {
    if (self = [super init]) {
        self.type = DrawTypeText;
    }
    return self;
}

- (void) dealloc {
    self.attributes = nil;
    [super dealloc];
}

- (int) fragmentNum {
    return self.text.length;
}

- (BOOL) isEmpty {
    return self.text.length == 0;
}

- (CGSize) stringSize:(NSString *)string {
    CGSize size = CGSizeZero;
#if __IPHONE_OS_VERSION_MIN_REQUIRED <= __IPHONE_7_0
    if (![string respondsToSelector:@selector(sizeWithAttributes:)]) {
        UIFont *font = [self.attributes objectForKey:NSFontAttributeName];
        size = [string sizeWithFont:font];
    } else
#endif
    {
        size = [string sizeWithAttributes:self.attributes];
    }
    return size;
}

- (void) updateDrawSize {
    CGRect rect = self.drawRect;
    rect.size = [self stringSize:self.text];
    self.drawRect = rect;
}

- (void) addAbsentAttributes:(NSDictionary *)attributes {
    NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithDictionary:attributes];
    [dict addEntriesFromDictionary:self.attributes];
    self.attributes = dict;
    [self updateDrawSize];
}

- (CGSize) fragmentSizeToFlag:(int)flag {
    NSString *sub = [self.text substringToIndex:flag];
    return [self stringSize:sub];
}

- (LrbTextDrawInfo *) clone {
    LrbTextDrawInfo *info = [LrbTextDrawInfo drawInfo];
    info.attributes = self.attributes;
    info.isChunk = self.isChunk;
    return info;
}

- (LrbDrawInfo *) fragmentFromFlag:(int)flag {
    if (flag >= self.text.length)
        return nil;
    LrbTextDrawInfo *info = [self clone];
    info.text = [self.text substringFromIndex:flag];
    [info updateDrawSize];
    return info;
}

- (LrbDrawInfo *) fragmentToFlag:(int)flag {
    if (flag == self.text.length)
        return self;
    
    LrbTextDrawInfo *info = [self clone];
    info.text = [self.text substringToIndex:flag];
    [info updateDrawSize];
    return info;
}

- (void) drawInCurrentContext {
#if __IPHONE_OS_VERSION_MIN_REQUIRED <= __IPHONE_7_0
    if (![self.text respondsToSelector:@selector(drawInRect:withAttributes:)]) {
        NSLog(@"to be continued!");
    } else
#endif
    {
        [self.text drawInRect:self.drawRect withAttributes:self.attributes];
    }
}

@end

@implementation LrbImageDrawInfo

- (instancetype) init {
    if (self = [super init]) {
        self.isChunk = YES;
        self.type = DrawTypeImage;
    }
    return self;
}

- (void) dealloc {
    self.image = nil;
    [super dealloc];
}

- (BOOL) isEmpty {
    return self.image == nil;
}

- (void) drawInCurrentContext {
    [self.image drawInRect:self.drawRect];
}

@end

@implementation LrbParsedInfo

- (void) dealloc {
    self.drawInfo = nil;
    self.touchText = nil;
    [super dealloc];
}

- (void) mixInfo:(LrbParsedInfo *)info {
    if (!self.touchText.length)
        self.touchText = info.touchText;
    
    if (self.drawInfo.type != DrawTypeText || info.drawInfo.type != DrawTypeText)
        return;
    
    [(LrbTextDrawInfo *)self.drawInfo addAbsentAttributes:((LrbTextDrawInfo *)info.drawInfo).attributes];
}

+ (LrbParsedInfo *) parsedInfo {
    return [[[self alloc] init] autorelease];
}

@end


@implementation LrbTexture

+ (CCTexture2D *) createTexture:(NSArray *)list size:(CGSize)size {
    NSUInteger width = ceil(size.width);
    NSUInteger height = ceil(size.height);
    
    UInt32 *data = calloc(width * height, sizeof(UInt32));
    
    CGColorSpaceRef *space = CGColorSpaceCreateDeviceRGB();
    
    CGContextRef context = CGBitmapContextCreate(data, width, height, 8, width * 4, space, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    
    CCTexture2D *tex = nil;
    if (context) {
        CGContextTranslateCTM(context, 0.0f, height);
        CGContextScaleCTM(context, 1.0f, -1.0f);
        UIGraphicsPushContext(context);
        
        for (LrbParsedInfo *info in list)
            [info.drawInfo drawInCurrentContext];
        
        tex = [[CCTexture2D alloc] initWithData:data pixelFormat:kCCTexture2DPixelFormat_RGBA8888 pixelsWide:width pixelsHigh:height contentSize:CGSizeMake(width, height)];
        [tex autorelease];
        
        UIGraphicsPopContext();
    }
    
    CGColorSpaceRelease(space);
    CGContextRelease(context);
    free(data);
    
    return tex;
}

@end

@implementation LrbLayout

- (void) reset {
    
}

- (void) layoutElements:(NSArray *)elements {
    
}

@end

@implementation LrbTextLayout

- (instancetype) init {
    if (self = [super init]) {
        _drawList = [[NSMutableArray alloc] init];
        _currentLine = [[NSMutableArray alloc] init];
        _touchList = [[NSMutableArray alloc] init];
        _chunkList = [[NSMutableArray alloc] init];
        _limitWidth = CGFLOAT_MAX;
        _limitHeight = CGFLOAT_MAX;
        self.roundType = TextRoundTypeImgTop;
    }
    return self;
}

- (void) dealloc {
    [_drawList release];
    [_currentLine release];
    [_touchList release];
    [_chunkList release];
    [super dealloc];
}

- (void) reset {
    _x = 0;
    _y = 0;
    _width = 0;
    _height = 0;
    _lineHeight = 0;
    [_touchList removeAllObjects];
    [_drawList removeAllObjects];
    [_currentLine removeAllObjects];
    [_chunkList removeAllObjects];
}

- (CGRect) overlayRect:(CGPoint)point {
    for (NSValue *value in _chunkList) {
        CGRect rect = [value CGRectValue];
        if (point.x > rect.origin.x && point.x < rect.origin.x + rect.size.width &&
            point.y > rect.origin.y && point.y < rect.origin.y + rect.size.height)
            return rect;
    }
    
    return CGRectZero;
}

- (void) processLine {
    float ratio = 0;
    switch (_roundType) {
        case TextRoundTypeImgCenter:
            ratio = 0.5;
            break;
            
        case TextRoundTypeImgBottom:
            ratio = 1.0;
            break;
            
        default:
            [_currentLine removeAllObjects];
            return;
    }
    
    for (LrbDrawInfo *info in _currentLine) {
        if (info.isChunk)
            continue;
        
        float off = (_lineHeight - info.drawRect.size.height) * ratio;
        CGPoint pos = info.drawRect.origin;
        pos.y += off;
        [info setDrawPoint:pos];
    }
    
    [_currentLine removeAllObjects];
}

- (void) updateLineConfig {
    _width = MAX(_width, _x);
    _x = 0;
    _y += _lineHeight;
    _lineHeight = 0;
    _height = _y;
}

- (void) processSub1:(LrbDrawInfo *)sub1 touchText:(NSString *)touchText {
    [sub1 setDrawPoint:ccp(_x, _y)];
    LrbParsedInfo *p1 = [LrbParsedInfo parsedInfo];
    p1.drawInfo = sub1;
    [_currentLine addObject:sub1];
    if (touchText.length) {
        p1.touchText = touchText;
        [_touchList addObject:p1];
    } else {
        [_drawList addObject:p1];
    }
}

- (void) layoutElements:(NSArray *)elements {
    NSMutableArray *list = [NSMutableArray arrayWithArray:elements];
    while (list.count) {
        LrbParsedInfo *parsed = [list objectAtIndex:0];
        LrbDrawInfo *info = parsed.drawInfo;
        if ([info isEmpty]) {
            [list removeObjectAtIndex:0];
            continue;
        }
            
        int idx = 1;
        CGRect rect;
        CGSize size;
        while (idx<=[info fragmentNum]) {
            size = [info fragmentSizeToFlag:idx];
            rect = [self overlayRect:ccp(_x+size.width, _y)];
            if (!CGRectIsEmpty(rect) || _x + size.width > _limitWidth)
                break;
            
            ++ idx;
        }
        
        if (idx == 1) {
            if (size.width > _limitWidth) {
                [list removeObjectAtIndex:0];
            } else if (_x + size.width > _limitWidth) {
                [self processLine];
                [self updateLineConfig];
            } else {
                _x = rect.origin.x + rect.size.width;
            }
            continue;
        }
        
        idx -= 1;
        size = [info fragmentSizeToFlag:idx];
        
        [list removeObjectAtIndex:0];
        LrbDrawInfo *sub1 = [info fragmentToFlag:idx];
        [self processSub1:sub1 touchText:parsed.touchText];
        
        LrbDrawInfo *sub2 = [info fragmentFromFlag:idx];
        if (sub2) {
            LrbParsedInfo *p2 = [LrbParsedInfo parsedInfo];
            p2.drawInfo = sub2;
            p2.touchText = parsed.touchText;
            [list insertObject:p2 atIndex:0];
        }
        
        _x += size.width;
        _lineHeight = MAX(_lineHeight, size.height);
        if (sub1.isChunk)
            [_chunkList addObject:[NSValue valueWithCGRect:sub1.drawRect]];
    }
    [self processLine];
    [self updateLineConfig];
}

@end

@implementation LrbParseNode

- (instancetype) init {
    if (self = [super init]) {
        _nextNodes = [[NSMutableArray alloc] init];
    }
    return self;
}

- (void) dealloc {
    [_nextNodes release];
    [super dealloc];
}

- (void) addNextNode:(LrbParseNode *)node {
    [_nextNodes addObject:node];
}

- (id) parse {
    return nil;
}

+ (LrbParseNode *) parseNode {
    return [[[self alloc] init] autorelease];
}

@end

@implementation LrbTextParseNode

- (void) dealloc {
    self.attributes = nil;
    self.text = nil;
    [super dealloc];
}

- (BOOL) hasText {
    return self.text.length || _nextNodes.count;
}

- (NSString *) attributeClassString:(NSString *)type {
    static NSDictionary *dict = nil;
    if (!dict) {
        dict = [[NSDictionary alloc] initWithObjectsAndKeys:@"font", @"size", @"ParagraphStyle", @"alignment", nil];
    }
    
    NSString *tmp = [dict objectForKey:type];
    if (!tmp)
        tmp = type;
    tmp = [tmp stringByReplacingCharactersInRange:NSMakeRange(0, 1) withString:[[tmp substringWithRange:NSMakeRange(0, 1)] uppercaseString]];
    return [tmp stringByAppendingString:@"Attribute"];
}

- (LrbImageDrawInfo *) imageDrawInfo:(NSString *)value {
    LrbImageDrawInfo *drawInfo = [LrbImageDrawInfo drawInfo];
    drawInfo.text = value;
    NSString *name = [value stringByDeletingPathExtension];
    NSString *ext = [value pathExtension];
    if (!ext.length)
        ext = @"png";
    NSString *path = [[NSBundle mainBundle] pathForResource:name ofType:ext];
    NSData *data = [[[NSData alloc] initWithContentsOfFile:path] autorelease];
    UIImage *image = [UIImage imageWithData:data];
    drawInfo.image = image;
    drawInfo.drawRect = CGRectMake(0, 0, image.size.width, image.size.height);
    return drawInfo;
}

- (LrbTextDrawInfo *) textDrawInfo:(NSDictionary *)tmp chunk:(BOOL)ischunk {
    NSMutableDictionary *dict = [NSMutableDictionary dictionary];
    for (id key in tmp) {
        TemplateAttribute *instance = (TemplateAttribute *)[tmp objectForKey:key];
        [dict addEntriesFromDictionary:[instance attributes]];
    }
    LrbTextDrawInfo *drawInfo = [LrbTextDrawInfo drawInfo];
    drawInfo.attributes = dict;
    drawInfo.text = self.text;
    drawInfo.isChunk = ischunk;
    [drawInfo updateDrawSize];
    return drawInfo;
}

- (LrbParsedInfo *) parsedInfo {
    LrbParsedInfo *info = [LrbParsedInfo parsedInfo];
    
    NSArray *list = [self.attributes componentsSeparatedByString:@","];
    NSMutableDictionary *tmp = [NSMutableDictionary dictionary];
    BOOL ischunk = NO;
    for (NSString *attribute in list) {
        NSArray *l = [attribute componentsSeparatedByString:@"="];
        if (l.count != 2)
            continue;
        
        NSCharacterSet *set = [NSCharacterSet whitespaceAndNewlineCharacterSet];
        NSString *type = [[l objectAtIndex:0] stringByTrimmingCharactersInSet:set];
        NSString *value = [[l objectAtIndex:1] stringByTrimmingCharactersInSet:set];
        
        if ([type containsString:@"touch"]) {
            info.touchText = value;
        } else if ([type containsString:@"img"]) {
            LrbImageDrawInfo *drawInfo = [self imageDrawInfo:value];
            info.drawInfo = drawInfo;
        } else if ([type containsString:@"chunk"]) {
            ischunk = YES;
        } else {
            NSString *clsString = [self attributeClassString:type];
            TemplateAttribute *instance = (TemplateAttribute *)[tmp objectForKey:clsString];
            if (!instance) {
                Class cls = NSClassFromString(clsString);
                instance = [[[cls alloc] init] autorelease];
                [tmp setObject:instance forKey:clsString];
            }
            
            [instance updateValue:value withType:type];
        }
    }
    
    if (!info.drawInfo) {
        LrbTextDrawInfo *drawInfo = [self textDrawInfo:tmp chunk:ischunk];
        info.drawInfo = drawInfo;
    }
    
    return info;
}

- (id) parse {
    LrbParsedInfo *info = [self parsedInfo];
    NSMutableArray *list = [NSMutableArray arrayWithObject:info];
    for (LrbParseNode *node in _nextNodes) {
        NSArray *l = [node parse];
        for (LrbParsedInfo *n in l)
            [n mixInfo:info];
        
        [list addObjectsFromArray:l];
    }
    
    return list;
}

@end


@implementation LrbTextParser

- (instancetype) init {
    if (self = [super init]) {
        _list = [[NSMutableArray alloc] init];
    }
    return self;
}

- (void) dealloc {
    [_list release];
    [super dealloc];
}

- (void) reset {
    [_list removeAllObjects];
}

- (void) parseRichText:(NSString *)text {
    NSString *pattern = @"((?<=^|>)[^<]+(?=<|$))|((?<=<)[^>/]+(?=>))|(</>)";
    NSRegularExpression *regex = [NSRegularExpression regularExpressionWithPattern:pattern options:NSRegularExpressionAnchorsMatchLines error:nil];
    
    __block NSMutableArray *list = [NSMutableArray array];
    LrbTextParseNode *head = [LrbTextParseNode parseNode];
    [list addObject:head];
    
    int num = [regex numberOfMatchesInString:text options:NSMatchingReportCompletion range:NSMakeRange(0, text.length)];
    
    [regex enumerateMatchesInString:text options:NSMatchingReportCompletion range:NSMakeRange(0, text.length) usingBlock:^(NSTextCheckingResult * result, NSMatchingFlags flags, BOOL * stop) {
        for (int i=1; i<result.numberOfRanges; ++i) {
            NSRange r = [result rangeAtIndex:i];
            if (r.length > 0) {
                LrbTextParseNode *node = [list lastObject];
                switch (i) {
                    case 1:
                    {
                        if ([node hasText]) {
                            LrbTextParseNode *n = [LrbTextParseNode parseNode];
                            n.text = [text substringWithRange:r];
                            [node addNextNode:n];
                        } else {
                            node.text = [text substringWithRange:r];
                        }
                    }
                        break;
                        
                    case 2:
                    {
                        LrbTextParseNode *n = [LrbTextParseNode parseNode];
                        n.attributes = [text substringWithRange:r];
                        [node addNextNode:n];
                        [list addObject:n];
                    }
                        break;
                        
                    case 3:
                        [list removeLastObject];
                        break;
                        
                    default:
                        break;
                }
                break;
            }
        }
    }];
    
    [_list addObjectsFromArray:[head parse]];
}

- (void) setDefaultAttributes:(NSDictionary *)attributes {
    for (LrbParsedInfo *info in self.list) {
        if (info.drawInfo.type != DrawTypeText)
            continue;
        [(LrbTextDrawInfo *)info.drawInfo addAbsentAttributes:attributes];
    }
}

@end

@implementation LrbTouchObject

- (void) dealloc {
    self.obj = nil;
    self.sprite = nil;
    [super dealloc];
}

+ (LrbTouchObject *) touchObject {
    return [[[self alloc] init] autorelease];
}

@end


@implementation LrbMixSprite

- (instancetype) init {
    if (self = [super init]) {
        _parser = [[LrbTextParser alloc] init];
        _layout = [[LrbTextLayout alloc] init];
        _defaultAttributes = [[NSMutableDictionary alloc] init];
        _touchList = [[NSMutableArray alloc] init];
        _isTouchEnabled = NO;
        _isMoved = NO;
        _touchPriority = 0;
        [self resetDefaultAttributes];
    }
    
    return self;
}

- (void) dealloc {
    [_parser release];
    [_layout release];
    [_defaultAttributes release];
    [_touchList release];
    [_obj release];
    self.text = nil;
    [super dealloc];
}

- (void) resetDefaultAttributes {
    [_defaultAttributes removeAllObjects];
    FontAttribute *font = [FontAttribute defaultAttribute];
    [_defaultAttributes addEntriesFromDictionary:[font attributes]];
    ColorAttribute *color = [ColorAttribute defaultAttribute];
    [_defaultAttributes addEntriesFromDictionary:[color attributes]];
}

- (void) onEnter {
    if (_isTouchEnabled)
        [self registTouch];
    
    [super onEnter];
}

- (void) onExit {
    if (_isTouchEnabled)
        [self removeTouch];
    
    [super onExit];
}

- (void) registTouch {
    [[CCTouchDispatcher sharedDispatcher] addTargetedDelegate:self priority:_touchPriority swallowsTouches:YES];
}

- (void) removeTouch {
    [[CCTouchDispatcher sharedDispatcher] removeDelegate:self];
}

- (void) resetTouch {
    [[CCTouchDispatcher sharedDispatcher] setPriority:_touchPriority forDelegate:self];
}

- (void) setIsTouchEnabled:(BOOL)isTouchEnabled {
    if (_isTouchEnabled == isTouchEnabled)
        return;
    
    _isTouchEnabled = isTouchEnabled;
    if (isRunning_) {
        if (isTouchEnabled)
            [self registTouch];
        else
            [self removeTouch];
    }
}

- (void) setTouchPriority:(int)touchPriority {
    _touchPriority = touchPriority;
    if (_isTouchEnabled) {
        if (isRunning_)
            [self resetTouch];
    } else {
        self.isTouchEnabled = YES;
    }
}

- (void) setDefaultSize:(CGFloat)size {
    [self setDefaultFont:FONT3 size:size];
}

- (void) setDefaultFont:(NSString *)name size:(CGFloat)size {
    FontAttribute *font = [FontAttribute defaultAttribute];
    font.font = name;
    font.size = size;
    [_defaultAttributes addEntriesFromDictionary:[font attributes]];
}

- (void) setDefaultColor:(int)color {
    ColorAttribute *att = [ColorAttribute defaultAttribute];
    att.color = [UIColor colorWithRed:(color >> 16 & 0xff)/255.0 green:(color >> 8 & 0xff)/255.0 blue:(color & 0xff)/255.0 alpha:1.0];
    [_defaultAttributes addEntriesFromDictionary:[att attributes]];
}

- (void) setDefaultAlignment:(NSTextAlignment)alignment {
    ParagraphStyleAttribute *style = [ParagraphStyleAttribute defaultAttribute];
    style.style.alignment = alignment;
    [_defaultAttributes addEntriesFromDictionary:[style attributes]];
}

- (void) setLimitWidth:(float)width {
    _layout.limitWidth = width * CC_CONTENT_SCALE_FACTOR();
}

- (void) setLimitHeight:(float)height {
    _layout.limitHeight = height * CC_CONTENT_SCALE_FACTOR();
}

- (void) setTextRoundType:(LrbTextRoundType)type {
    _layout.roundType = type;
}

- (void) parse {
    [_parser parseRichText:_text];
    [_parser setDefaultAttributes:_defaultAttributes];
}

- (void) layout {
    [_layout layoutElements:_parser.list];
    [_parser reset];
}

- (void) render {
    CCTexture2D *tex = [LrbTexture createTexture:_layout.drawList size:CGSizeMake(_layout.width, _layout.height)];
    [self setTexture:tex];
    [self setTextureRect:CGRectMake(0, 0, tex.contentSize.width, tex.contentSize.height)];
    
    for (LrbParsedInfo *info in _layout.touchList) {
        CGRect rect = info.drawInfo.drawRect;
        CGPoint pos = rect.origin;
        rect.origin = CGPointZero;
        info.drawInfo.drawRect = rect;
        tex = [LrbTexture createTexture:@[info] size:rect.size];
        CCSprite *sp = [CCSprite spriteWithTexture:tex];
        sp.anchorPoint = CGPointZero;
        sp.positionInPixels = ccp(pos.x, self.contentSizeInPixels.height-pos.y-rect.size.height);
        [self addChild:sp];
        
        LrbTouchObject *obj = [LrbTouchObject touchObject];
        obj.sprite = sp;
        obj.obj = info.touchText;
        [_touchList addObject:obj];
    }
    
    if (_layout.touchList.count)
        self.isTouchEnabled = YES;
    
    [_layout reset];
}

- (void) generate {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    [self parse];
    [self layout];
    [self render];
    [pool drain];
}

- (void) setTarget:(id)target selector:(SEL)selector obj:(id)obj {
    _target = target;
    _selector = selector;
    [_obj release];
    _obj = [obj retain];
}

- (void) checkCallBack {
    id obj = _current ? _current.obj : _obj;
    if (_target && [_target respondsToSelector:_selector])
        [_target performSelector:_selector withObject:obj];
}

- (BOOL) ccTouchBegan:(UITouch *)touch withEvent:(UIEvent *)event {
    
    CGPoint touchLocation = [touch locationInView: [touch view]];
    touchLocation = [[CCDirector sharedDirector] convertToGL: touchLocation];
    CGPoint local = [self convertToNodeSpace:touchLocation];
    for (LrbTouchObject *touchObj in _touchList) {
        CGRect rect = CGRectZero;
        rect.origin = touchObj.sprite.position;
        rect.size = touchObj.sprite.contentSize;
        
        if (CGRectContainsPoint(rect, local)) {
            _current = touchObj;
            return YES;
        }
    }
    
    _current = nil;
    return _obj != nil;
}

- (void) ccTouchMoved:(UITouch *)touch withEvent:(UIEvent *)event {
    _isMoved = YES;
    _current = nil;
}

- (void) ccTouchEnded:(UITouch *)touch withEvent:(UIEvent *)event {
    [self checkCallBack];
}

@end










