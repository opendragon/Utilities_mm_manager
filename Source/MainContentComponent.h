//--------------------------------------------------------------------------------------------------
//
//  File:       MainContentComponent.h
//
//  Project:    M+M
//
//  Contains:   The class declaration for the contents of the primary window of the channel manager
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
# define MainContentComponent_H_ /* Header guard */

# if (! defined(DOXYGEN))
#  include "../JuceLibraryCode/JuceHeader.h"
# endif // ! defined(DOXYGEN)

# include "ChannelEntry.h"
# include "M+MUtilities.h"

# include <list>
# include <map>
# include <set>

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 
 @brief The class declaration for the contents of the primary window of the channel manager
 application. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace ChannelManager
{
    class ChannelContainer;
    
    /*! @brief The content area of the main window of the application. */
    class MainContentComponent : public Component
    {
    public:
        
        /*! @brief The constructor. */
        MainContentComponent(void);
        
        /*! @brief The destructor. */
        virtual ~MainContentComponent(void);
        
        /*! @brief Add an entity to the list of next-to-be-displayed entities.
         @param anEntity The entity to be added. */
        void addEntityToBackground(ChannelContainer * anEntity);
        
        /*! @brief Add an entity to the list of currently displayed entities.
         @param anEntity The entity to be added. */
        void addEntityToForeground(ChannelContainer * anEntity);
        
        /*! @brief Clear out the background data. */
        void clearOutBackgroundData(void);
        
        /*! @brief Remove a port from the set of known ports.
         @param aPort The port to be removed. */
        void forgetPort(ChannelEntry * aPort);
        
        /*! @brief Return the font to be used for bold text.
         @returns The font to be used for bold text. */
        Font & getBoldFont(void)
        {
            return *_defaultBoldFont;
        } // getBoldFont
        
        /*! @brief Return the color to be used for markers.
         @returns The color to be used for markers. */
        static Colour getMarkerColor(void);
        
        /*! @brief Return the color to be used for connections being made by dragging.
         @returns The color to be used for connections being made by dragging. */
        static Colour getNewConnectionColor(void);
        
        /*! @brief Return the line width for a normal connection.
         @returns The line width for a normal connection. */
        static float getNormalConnectionWidth(void);
        
        /*! @brief Return the font to be used for normal text.
         @returns The font to be used for normal text. */
        Font & getNormalFont(void)
        {
            return *_defaultNormalFont;
        } // getNormalFont
        
        /*! @brief Return the color to be used for non-TCP/non-UDP connections.
         @returns The color to be used for non-TCP/non-UDP connection. */
        static Colour getOtherConnectionColor(void);
        
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
        
        /*! @brief Return the line width for a service connection.
         @returns The line width for a service connection. */
        static float getServiceConnectionWidth(void);
        
        /*! @brief Return the color to be used for TCP connections.
         @returns The color to be used for TCP connections. */
        static Colour getTcpConnectionColor(void);
        
        /*! @brief Return the color to be used for UDP connections.
         @returns The color to be used for UDP connections. */
        static Colour getUdpConnectionColor(void);
        
    private:
        
        /*! @brief The class that this class is derived from. */
        typedef Component inherited;
        
        /*! @brief The information for a connection. */
        struct ConnectionDetails
        {
            /*! @brief The name of the destination port. */
            String _inPortName;
            
            /*! @brief The name of the source port. */
            String _outPortName;
            
            /*! @brief The mode of the connection. */
            MplusM::Common::ChannelMode _mode;
            
        }; // ConnectionDetails
        
        /*! @brief The name and direction for a port. */
        struct NameAndDirection
        {
            /*! @brief The name of the port. */
            String _name;
            
            /*! @brief The direction of the port. */
            ChannelEntry::PortDirection _direction;
            
        }; // NameAndDirection
        
        /*! @brief The name and associates for a port. */
        struct PortAndAssociates
        {
            /*! @brief The name of the port. */
            String _name;
            
            /*! @brief The associates of the port. */
            MplusM::Utilities::PortAssociation _associates;
            
        }; // PortAndAssociates
        
        /*! @brief A mapping from port names to associates. */
        typedef std::map<String, PortAndAssociates> AssociatesMap;
        
        /*! @brief A collection of connections. */
        typedef std::vector<ConnectionDetails> ConnectionList;
        
        /*! @brief A collection of services and ports. */
        typedef std::list<ChannelContainer *> ContainerList;

        /*! @brief A mapping from strings to ports. */
        typedef std::map<String, ChannelEntry *> PortEntryMap;
        
        /*! @brief A collection of port names. */
        typedef std::map<String, NameAndDirection> PortMap;
        
        /*! @brief A collection of port names. */
        typedef std::set<String> PortSet;
        
        /*! @brief A mapping from strings to service descriptions. */
        typedef std::map<String, MplusM::Utilities::ServiceDescriptor> ServiceMap;
        
        /*! @brief Add connections between detected ports in the to-be-displayed list.
         @param detectedPorts The set of detected YARP ports. */
        void addPortConnectionsToBackground(const MplusM::Utilities::PortVector & detectedPorts);
        
        /*! @brief Add ports that have associates as 'adapter' entities to the to-be-displayed list.
         @param detectedPorts The set of detected YARP ports. */
        void addPortsWithAssociatesToBackground(const MplusM::Utilities::PortVector &
                                                                                    detectedPorts);
        
        /*! @brief Add regular YARP ports as distinct entities to the to-be-displayed list.
         @param detectedPorts The set of detected YARP ports. */
        void addRegularPortEntitiesToBackground(const MplusM::Utilities::PortVector &
                                                                                    detectedPorts);
        
        /*! @brief Add services as distinct entities to the to-be-displayed list.
         @param services The set of detected services. */
        void addServicesToBackground(const MplusM::Common::StringVector & services);
        
        /*! @brief Clear out the foreground data. */
        void clearOutForegroundData(void);
        
        /*! @brief Display the connections between containers. */
        void drawConnections(void);
        
        /*! @brief Find a port in the to-be-displayed list by name.
         @param name The name of the port.
         @returns @c NULL if the port cannot be found and non-@c NULL if it is found. */
        ChannelEntry * findBackgroundPort(const String & name);
        
        /*! @brief Find an entity in the currently-displayed list by name.
         @param name The name of the entity.
         @returns @c NULL if the entity cannot be found and non-@c NULL if it is found. */
        ChannelContainer * findForegroundEntity(const String & name);
        
        /*! @brief Find an entity by the name of a port within it.
         @param name The name of the port.
         @returns @c NULL if the entity cannot be found and non-@c NULL if it is found. */
        ChannelContainer * findForegroundEntityForPort(const String & name);
        
        /*! @brief Find an entity by a port within it.
         @param aPort The port of interest.
         @returns @c NULL if the entity cannot be found and non-@c NULL if it is found. */
        ChannelContainer * findForegroundEntityForPort(const ChannelEntry * aPort);
        
        /*! @brief Find a port in the currently-displayed list by name.
         @param name The name of the port.
         @returns @c NULL if the port cannot be found and non-@c NULL if it is found. */
        ChannelEntry * findForegroundPort(const String & name);
        
        /*! @brief Move an entity to the end of the entity list so that it will be the last drawn.
         @param anEntity The entity to be moved. */
        void moveEntityToEndOfForegroundList(ChannelContainer * anEntity);
        
        /*! @brief Add a port to the set of known ports in the to-be-displayed list.
         @param aPort The port to be added. */
        void rememberPortInBackground(ChannelEntry * aPort);
        
        /*! @brief Set the entity positions. */
        void setEntityPositions(void);
        
        /*! @brief Swap the background and foreground data structures. */
        void swapBackgroundAndForeground(void);
        
        /*! @brief The area in which to draw the displayed entities. */
        ScopedPointer<Component> _innerPanel;
        
        /*! @brief A collection of visible services and ports. */
        ContainerList _entities1;
        
        /*! @brief A collection of visible services and ports. */
        ContainerList _entities2;
        
        /*! @brief A set of associated ports. */
        AssociatesMap _associatedPorts;
        
        /*! @brief A set of known ports. */
        PortEntryMap _ports1;
        
        /*! @brief A set of known ports. */
        PortEntryMap _ports2;
        
        /*! @brief A set of known ports. */
        PortSet _rememberedPorts;
        
        /*! @brief A set of standalone ports. */
        PortMap _standalonePorts;
        
        /*! @brief A set of known services. */
        ServiceMap _detectedServices;
        
        /*! @brief A set of connections. */
        ConnectionList _connections;
        
        /*! @brief The horizontal scrollbar. */
        ScrollBar _horizontalScrollBar;
        
        /*! @brief The vertical scrollbar. */
        ScrollBar _verticalScrollBar;
        
        /*! @brief The bold font to be used. */
        ScopedPointer<Font> _defaultBoldFont;
        
        /*! @brief The normal font to be used. */
        ScopedPointer<Font> _defaultNormalFont;
        
        /*! @brief Control access to the currently-displayed lists. */
        CriticalSection _foregroundLock;
        
        /*! @brief The background set of known entities. */
        ContainerList * _backgroundEntities;
        
        /*! @brief The foreground set of known entities. */
        ContainerList * _foregroundEntities;
        
        /*! @brief The background set of known ports. */
        PortEntryMap * _backgroundPorts;
        
        /*! @brief The foreground set of known ports. */
        PortEntryMap * _foregroundPorts;
        
        /*! @brief A generator of random numbers. */
        Random _randomizer;
        
        /*! @brief The desired width (for a vertical scrollbar) or height (for a horizontal
         scrollbar). */
        int _scrollbarThickness;
        
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
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
        
    }; // MainContentComponent
    
} // ChannelManager

#endif // ! defined(MainContentComponent_H_)
