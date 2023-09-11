#import <string>
#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>

namespace utils
{
    void msgbox(const std::string &caption, const std::string &content)
    {
        NSAlert *alert = [[[NSAlert alloc] init] autorelease];
        [alert setInformativeText:[NSString stringWithUTF8String:caption.c_str()]];
        [alert setMessageText:[NSString stringWithUTF8String:content.c_str()]];
        [alert runModal];
    }

    void msgbox(const std::u16string &caption, const std::u16string &content)
    {
        NSAlert *alert = [[[NSAlert alloc] init] autorelease];
        [alert setInformativeText:[NSString stringWithFormat:@"%S", (const unichar *)caption.c_str()]];
        [alert setMessageText:[NSString stringWithFormat:@"%S", (const unichar *)content.c_str()]];
        [alert runModal];
    }

    void shell_open(const std::string &url)
    {
        NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

        NSURL *nsUrl = [NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]];
        [[NSWorkspace sharedWorkspace] openURL:nsUrl];

        [pool release];
    }
}

// void ApplyVibrancy(void *pview, int appearance, int state, double cornerRadius)
// {
//     NSView *view = (__bridge NSView *)pview;
//     NSRect rect = view.bounds;
    
//     NSVisualEffectMaterial m = (NSVisualEffectMaterial)appearance;
//     NSVisualEffectView *blurredView = [[NSVisualEffectView alloc] initWithFrame:rect];
    
//     [blurredView setMaterial:m];
//     [blurredView setState:(NSVisualEffectState)state];
//     [blurredView setBlendingMode:NSVisualEffectBlendingModeBehindWindow];
    
//     if (blurredView.layer)
//         blurredView.layer.cornerRadius = cornerRadius;
    
//     blurredView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
    
//     [view addSubview:blurredView positioned:NSWindowBelow relativeTo:0];
// }