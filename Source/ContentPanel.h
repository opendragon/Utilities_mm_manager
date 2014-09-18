//--------------------------------------------------------------------------------------------------
//
//  File:       ContentPanel.h
//
//  Project:    M+M
//
//  Contains:   The class declaration for the content area of the primary window of the channel
//              manager application.
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
//  Created:    2014-07-21
//
//--------------------------------------------------------------------------------------------------

#if (! defined(ContentPanel_H_))
# define ContentPanel_H_ /* Header guard */

# include "ChannelManagerDataTypes.h"

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 
 @brief The class declaration for the content area of the primary window of the channel manager
 application. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace ChannelManager
{
    class ChannelManagerWindow;
    class EntitiesPanel;
    class ScannerThread;
    
    /*! @brief The content area of the main window of the application. */
    class ContentPanel : public Viewport,
                         public ApplicationCommandTarget
    {
    public:
        
        /*! @brief The constructor.
         @param containingWindow The window in which the panel is embedded. */
        ContentPanel(ChannelManagerWindow * containingWindow);
        
        /*! @brief The destructor. */
        virtual ~ContentPanel(void);
        
        /*! @brief Returns the entities panel.
         @returns The entities panel. */
        EntitiesPanel & getEntitiesPanel(void)
        const
        {
            return *_entitiesPanel;
        } // getEntitiesPanel
        
        /*! @brief Draw the content of the component.
         @param gg The graphics context in which to draw. */
        void paint(Graphics & gg);
        
        /*! @brief Restore the positions of all the entities in the panel. */
        void recallEntityPositions(void);
        
        /*! @brief Record the position of an entity before it is removed from the panel. */
        void rememberPositionOfEntity(ChannelContainer * anEntity);
        
        /*! @brief Called when the component size has been changed. */
        void resized(void);
        
        /*! @brief Record the positions of all the entities in the panel. */
        void saveEntityPositions(void);
        
        /*! @brief Ignore the result of the next scan. */
        void skipScan(void);
        
        /*! @brief Called when the visible area changes.
         @param newVisibleArea The new visible area. */
        virtual void visibleAreaChanged(const juce::Rectangle<int> & newVisibleArea);
        
    protected:
        
    private:
        
        /*! @brief The first class that this class is derived from. */
        typedef Viewport inherited1;
        
        /*! @brief The second class that this class is derived from. */
        typedef ApplicationCommandTarget inherited2;
        
        /*! @brief Return a list of commands that this target can handle.
         @param commands The list of commands to be added to. */
        virtual void getAllCommands(Array<CommandID> & commands);

        /*! @brief Provide details about one of the commands that this target can perform.
         @param commandID The identifier for the command.
         @param result The details about the command. */
        virtual void getCommandInfo(CommandID                commandID,
                                    ApplicationCommandInfo & result);
        
        /*! @brief Return the next target to try after this one.
         @returns The next target to try after this one. */
        virtual ApplicationCommandTarget * getNextCommandTarget(void);

        /*! @brief Perform the specified command.
         @param info The details for the command.
         @returns @c true if the command was handled and @c false if it was not. */
        virtual bool perform(const InvocationInfo & info);
        
        /*! @brief Set the entity positions, based on the scanned entities.
        @param scanner The background scanning thread. */
        void setEntityPositions(ScannerThread & scanner);

        /*! @brief Refresh the displayed entities and connections, based on the scanned entities.
         @param scanner The background scanning thread. */
        void updatePanels(ScannerThread & scanner);
        
        /*! @brief The positions that entities were last seen at. */
        PositionMap _rememberedPositions;
        
        /*! @brief The entities panel. */
        ScopedPointer<EntitiesPanel> _entitiesPanel;
        
        /*! @brief The window in which the panel is embedded. */
        ChannelManagerWindow * _containingWindow;
        
# if (defined(USE_OGDF_POSITIONING) && defined(USE_OGDF_FOR_FIRST_POSITIONING_ONLY))
        bool _initialPositioningDone;
# endif // defined(USE_OGDF_POSITIONING) && defined(USE_OGDF_FOR_FIRST_POSITIONING_ONLY)
        
        /*! @brief @c true if the background is inverted and @c false otherwise. */
        bool _invertBackground;
        
        /*! @brief @c true if the next scan result is to be ignored and @c false otherwise. */
        bool _skipNextScan;
        
        /*! @brief @c true if the background is white and @c false otherwise. */
        bool _whiteBackground;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ContentPanel)
        
    }; // ContentPanel
    
} // ChannelManager

#endif // ! defined(ContentPanel_H_)
