//--------------------------------------------------------------------------------------------------
//
//  File:       MainContentComponent.h
//
//  Project:    M+M
//
//  Contains:   The class definition for a visible entity that has one or more channels or ports.
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
//  Created:    2014-07-15
//
//--------------------------------------------------------------------------------------------------

#if (! defined(ChannelContainer_H_))

# if (! defined(DOXYGEN))
#  include "../JuceLibraryCode/JuceHeader.h"
# endif // ! defined(DOXYGEN)

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for a visible entity that has one or more channels or ports. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

class MainContentComponent;

/*! @brief A container for one or more ports or channels. */
class ChannelContainer : public Component
{
public:
    
    /*! @brief The constructor.
     @param owner The owner of the entity.
     @param title The title of the entity. */
    ChannelContainer(MainContentComponent & owner,
                     const String &         title);
    
    /*! @brief The destructor. */
    virtual ~ChannelContainer(void);
    
    /*! @brief Called when a mouse button is pressed.
     @param ee Details about the position and status of the mouse event. */
    void mouseDown(const MouseEvent & ee) override;
    
    /*! @brief Called when the mouse is moved while a button is held down.
     @param ee Details about the position and status of the mouse event. */
    void mouseDrag(const MouseEvent & ee) override;
    
    /*! @brief Draw the content of the component. */
    void paint(Graphics & gg) override;
    
    /*! @brief Called when the component size has been changed. */
    void resized(void) override;
    
private:
    
    /*! @brief The class that this class is derived from. */
    typedef Component inherited;
    
    /*! @brief Restrictions on the components size or position. */
    ComponentBoundsConstrainer _constrainer;

    /*! @brief Used to take care of the logic for dragging the component. */
    ComponentDragger _dragger;

    /*! @brief The title of the entity. */
    String _title;
    
    /*! @brief The owner of the entity. */
    MainContentComponent & _owner;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChannelContainer)
    
}; // MainContentComponent

#endif // ! defined(ChannelContainer_H_)
