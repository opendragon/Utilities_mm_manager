//--------------------------------------------------------------------------------------------------
//
//  File:       MainContentComponent.h
//
//  Project:    M+M
//
//  Contains:   The class definition for the contents of the primary window of the channel manager
//              application.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2014 by HPlus Technologies Ltd. and Simon Fraser University.
//
//              All rights reserved. Redistribution and use in source and binary forms, with or
//              without modification, are permitted provided that the following conditions are met:
//                * Redistributions of source code must retain the above copyright notice, this list
//                  of conditions and the following disclaimer.
//                * Redistributions in binary form must reproduce the above copyright notice, this
//                  list of conditions and the following disclaimer in the documentation and/or
//                  other materials provided with the distribution.
//                * Neither the name of the copyright holders nor the names of its contributors may
//                  be used to endorse or promote products derived from this software without
//                  specific prior written permission.
//
//              THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//              EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//              OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//              SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//              INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//              TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//              BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//              CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//              ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//              DAMAGE.
//
//  Created:    2014-07-14
//
//--------------------------------------------------------------------------------------------------

#if (! defined(MainContentComponent_H_))

# if (! defined(DOXYGEN))
#  include "../JuceLibraryCode/JuceHeader.h"
# endif // ! defined(DOXYGEN)

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for the contents of the primary window of the channel manager
 application. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

/*! @brief The content area of the main window of the application. */
class MainContentComponent : public Component
{
public:
    
    /*! @brief The constructor. */
    MainContentComponent(void);
    
    /*! @brief The destructor. */
    virtual ~MainContentComponent(void);

    Font & getBoldFont(void)
    {
        return *_defaultBoldFont;
    } // getBoldFont
    
    Font & getNormalFont(void)
    {
        return *_defaultNormalFont;
    } // getNormalFont
    
    /*! @brief Returns the scrollbar thickness.
     @returns The scrollbar thickness. */
    inline int getScrollbarThickness(void)
    const noexcept
    {
        return _scrollbarThickness;
    } // getScrollbarThickness
    
    /*! @brief Draw the content of the component. */
    void paint(Graphics & gg);

    /*! @brief Called when the component size has been changed. */
    void resized(void);

    /*! @brief Adjust the horizontal and vertial scrollbars. */
    void updateScrollBars(void);
    
private:

    /*! @brief The class that this class is derived from. */
    typedef Component inherited;
    
    /*! @brief The collection of displayed entities. */
    OwnedArray<Component> _entities;
    
    /*! @brief The area in which to draw the displayed entities. */
    ScopedPointer<Component> _innerPanel;
    
    /*! @brief The horizontal scrollbar. */
    ScrollBar _horizontalScrollBar;
    
    /*! @brief The vertical scrollbar. */
    ScrollBar _verticalScrollBar;
    
    /*! @brief The bold font to be used. */
    ScopedPointer<Font> _defaultBoldFont;
    
    /*! @brief The normal font to be used. */
    ScopedPointer<Font> _defaultNormalFont;
    
    /*! @brief The desired width (for a vertical scrollbar) or height (for a horizontal
     scrollbar). */
    int _scrollbarThickness;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)

}; // MainContentComponent

#endif // ! defined(MainContentComponent_H_)
