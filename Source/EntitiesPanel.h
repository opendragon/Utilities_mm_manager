//--------------------------------------------------------------------------------------------------
//
//  File:       EntitiesPanel.h
//
//  Project:    M+M
//
//  Contains:   The class declaration for the entities layer of the primary window of the channel
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
//  Created:    2014-07-14
//
//--------------------------------------------------------------------------------------------------

#if (! defined(EntitiesPanel_H_))
# define EntitiesPanel_H_ /* Header guard */

# include "ChannelsDataTypes.h"

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 
 @brief The class declaration for the entities layer of the primary window of the channel manager
 application. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace ChannelManager
{
    class ChannelContainer;
    class ChannelEntry;
    
    /*! @brief The entities layer of the main window of the application. */
    class EntitiesPanel : public Component
    {
    public:
        
        /*! @brief The constructor. */
        EntitiesPanel(void);
        
        /*! @brief The destructor. */
        virtual ~EntitiesPanel(void);
        
        /*! @brief Add an entity to the list of known entities.
         @param anEntity The entity to be added. */
        void addEntity(ChannelContainer * anEntity);

        /*! @brief Call the visited flags for all entities. */
        void clearAllVisitedFlags(void);
        
        /*! @brief Find an entity in the currently-displayed list by name.
         @param name The name of the entity.
         @returns @c NULL if the entity cannot be found and non-@c NULL if it is found. */
        ChannelContainer * findKnownEntity(const String & name);
        
        /*! @brief Find a port in the to-be-displayed list by name.
         @param name The name of the port.
         @returns @c NULL if the port cannot be found and non-@c NULL if it is found. */
        ChannelEntry * findKnownPort(const String & name);
        
        /*! @brief Find an entity by the name of a port within it.
         @param name The name of the port.
         @returns @c NULL if the entity cannot be found and non-@c NULL if it is found. */
        ChannelContainer * findKnownEntityForPort(const String & name);
        
        /*! @brief Find an entity by a port within it.
         @param aPort The port of interest.
         @returns @c NULL if the entity cannot be found and non-@c NULL if it is found. */
        ChannelContainer * findKnownEntityForPort(const ChannelEntry * aPort);
        
        /*! @brief Remove a port from the set of known ports.
         @param aPort The port to be removed. */
        void forgetPort(ChannelEntry * aPort);
        
        /*! @brief Return the font to be used for bold text.
         @returns The font to be used for bold text. */
        Font & getBoldFont(void)
        {
            return *_defaultBoldFont;
        } // getBoldFont
        
        /* @brief Return an entity by index.
         @param index The zero-origin index of the entity.
         @returns The entity if the index is within range and @c NULL otherwise. */
        ChannelContainer * getEntity(const size_t index)
        const;
        
        /*! @brief Return the color to be used for markers.
         @returns The color to be used for markers. */
        static Colour getMarkerColor(void);
        
        /*! @brief Return the color to be used for connections being made by dragging.
         @returns The color to be used for connections being made by dragging. */
        static Colour getNewConnectionColor(void);
        
        /*! @brief Return the font to be used for normal text.
         @returns The font to be used for normal text. */
        Font & getNormalFont(void)
        {
            return *_defaultNormalFont;
        } // getNormalFont
        
        /*! @brief Return the number of entities.
         @returns The number of entities. */
        size_t getNumberOfEntities(void)
        const;
        
        /*! @brief Returns the scrollbar thickness.
         @returns The scrollbar thickness. */
        inline int getScrollbarThickness(void)
        const noexcept
        {
            return _scrollbarThickness;
        } // getScrollbarThickness
        
        /*! @brief Draw the content of the component.
         @param gg The graphics context in which to draw. */
        void paint(Graphics & gg);
        
        /*! @brief Record a newly added port.
         @param aPort The port to be recorded. */
        void rememberPort(ChannelEntry * aPort);
        
        /*! @brief Remove connections that are invalid. */
        void removeInvalidConnections(void);
        
        /*! @brief Remove any entities that were not visited. */
        void removeUnvisitedEntities(void);
        
        /*! @brief Called when the component size has been changed. */
        void resized(void);
        
        /*! @brief Adjust the horizontal and vertial scrollbars. */
        void updateScrollBars(void);
        
    protected:
        
    private:
        
        /*! @brief The class that this class is derived from. */
        typedef Component inherited;
        
        /*! @brief The set of known ports. */
        PortEntryMap _knownPorts;
        
        /*! @brief A collection of known services and ports. */
        ContainerList _knownEntities;
        
        /*! @brief Release all data held by the panel. */
        void clearOutData(void);
        
        /*! @brief Display the connections between containers.
         @param gg The graphics context in which to draw. */
        void drawConnections(Graphics & gg);
        
#if 0
        /*! @brief Display the containers.
         @param gg The graphics context in which to draw. */
        void drawContainers(Graphics & gg);
        
        /*! @brief Move an entity to the end of the entity list so that it will be the last drawn.
         @param anEntity The entity to be moved. */
        void moveEntityToEndOfForegroundList(ChannelContainer * anEntity);
        
        /*! @brief Set the entity positions. */
        void setEntityPositions(void);
        
        /*! @brief Swap the background and foreground data structures. */
        void swapBackgroundAndForeground(void);
        
        /*! @brief A set of known ports. */
        PortEntryMap _ports1;
        
        /*! @brief A set of known ports. */
        PortEntryMap _ports2;
        
        /*! @brief Control access to the currently-displayed lists. */
        CriticalSection _foregroundLock;
        
        /*! @brief The background set of known entities. */
        ContainerList * _backgroundEntities;
        
        /*! @brief The foreground set of known entities. */
        ContainerList * _foregroundEntities;
        
        /*! @brief The background set of known ports. */
        PortEntryMap * _backgroundPorts;
        
        /*! @brief @c true if an entity is being moved. */
        bool _movementActive;
        
# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunused-private-field"
# endif // defined(__APPLE__)
        /*! @brief Filler to pad to alignment boundary */
        char _filler[7];
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)
        
#endif // 0
        
        /*! @brief The bold font to be used. */
        ScopedPointer<Font> _defaultBoldFont;
        
        /*! @brief The normal font to be used. */
        ScopedPointer<Font> _defaultNormalFont;
        
        /*! @brief The horizontal scrollbar. */
        ScopedPointer<ScrollBar> _horizontalScrollBar;
        
        /*! @brief The vertical scrollbar. */
        ScopedPointer<ScrollBar> _verticalScrollBar;
        
        /*! @brief The area in which to draw the displayed entities.
         
         Note that this class is also a Component - entities are added to the 'inner panel' for
         rendering purposes; objects of this class don't directly display entities. */
        ScopedPointer<Component> _innerPanel;
        
        /*! @brief The desired width (for a vertical scrollbar) or height (for a horizontal
         scrollbar). */
        int _scrollbarThickness;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EntitiesPanel)
        
    }; // EntitiesPanel
    
} // ChannelManager

#endif // ! defined(EntitiesPanel_H_)
