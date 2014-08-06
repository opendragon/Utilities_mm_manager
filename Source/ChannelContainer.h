//--------------------------------------------------------------------------------------------------
//
//  File:       ChannelContainer.h
//
//  Project:    M+M
//
//  Contains:   The class declaration for a visible entity that has one or more channels or ports.
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
# define ChannelContainer_H_ /* Header guard */

# include "ChannelsDataTypes.h"

# include <ogdf/basic/Graph.h>

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 
 @brief The class declaration for a visible entity that has one or more channels or ports. */

/*! @namespace ChannelManager
 @brief The classes that implement the Channel Manager application. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace ChannelManager
{
    class ChannelEntry;
    class EntitiesPanel;
    
    /*! @brief A container for one or more ports or channels. */
    class ChannelContainer : public Component
    {
    public:
        
        /*! @brief The constructor.
         @param kind The kind of entity.
         @param title The title of the entity.
         @param behaviour The behavioural model if a service.
         @param description The description, if this is a service.
         @param owner The owner of the entity. */
        ChannelContainer(const ContainerKind kind,
                         const String &      title,
                         const String &      behaviour,
                         const String &      description,
                         EntitiesPanel &     owner);
        
        /*! @brief The destructor. */
        virtual ~ChannelContainer(void);
        
        /*! @brief Add a port to the panel.
         @param portName The name of the port.
         @param portProtocol The protocol of the port.
         @param portKind What the port will be used for.
         @param direction The primary direction of the port.
         @returns The newly-created port. */
        ChannelEntry * addPort(const String &      portName,
                               const String &      portProtocol = "",
                               const PortUsage     portKind = kPortUsageOther,
                               const PortDirection direction = kPortDirectionInputOutput);
        
        /*! @brief Clear any connect / disconnect markers. */
        void clearMarkers(void);
        
        /*! @brief Clears the visited flag for the entity. */
        inline void clearVisited(void)
        {
            _visited = false;
        } // clearVisited
        
        /*! @brief Deselect the entity. */
        inline void deselect(void)
        {
            _selected = false;
        } // deselect
        
        /*! @brief Display the connections between containers.
         @param gg The graphics context in which to draw. */
        void drawOutgoingConnections(Graphics & gg);
        
        /*! @brief Return the position of the entity within it's containing panel.
         @returns The position of the entity within it's containing panel. */
        Point<float> getPositionInPanel(void)
        const;
        
        /*! @brief Return the behavioural model for the entity.
         @returns The behavioural model for the entity. */
        inline String getBehaviour(void)
        const
        {
            return _behaviour;
        } // getBehaviour
        
        /*! @brief Return the description of the entity.
         @returns The description of the entity. */
        inline String getDescription(void)
        const
        {
            return _description;
        } // getDescription
        
        /*! @brief Return the kind of container.
         @returns The kind of container. */
        inline ContainerKind getKind(void)
        const
        {
            return _kind;
        } // getKind

        /*! @brief Return the node corresponding to the entity.
         @returns The node corresponding to the entity. */
        ogdf::node getNode(void)
        const;
        
        /*! @brief Returns the number of ports in this panel.
         @returns The number of ports in this panel. */
        inline int getNumPorts(void)
        const
        {
            return getNumChildComponents();
        } // getNumPorts
        
        inline EntitiesPanel & getOwner(void)
        const
        {
            return _owner;
        } // getOwner
        
        /*! @brief Returns a port by index.
         @param num The zero-origin index of the port.
         @returns A port or @c NULL if the index is out of range. */
        ChannelEntry * getPort(const int num)
        const;
        
        /*! @brief Return the amount of space to the left of the text being displayed.
         @returns The amount of space to the left of the text being displayed. */
        float getTextInset(void)
        const;
        
        /*! @brief Check if a port is part of the entity.
         @param aPort The port to be checked for.
         @returns @c true if the port is contained within the entity and @c false otherwise. */
        bool hasPort(const ChannelEntry * aPort);
        
        /*! @brief Mark all the connections as invalid. */
        void invalidateConnections(void);
        
        /*! @brief Returns @c true if one of the port entries is marked and @c false otherwise.
         @returns @c true if one of the port entries is marked and @c false otherwise. */
        bool isMarked(void)
        const;
        
        /*! @brief Return @c true is the entity is newly created.
         @returns @c true if the entity is newly created and @c false otherwise. */
        inline bool isNew(void)
        const
        {
            return _newlyCreated;
        } // isNew
        
        /*! @brief Return @c true is the entity is selected.
         @returns @c true if the entity is selected and @c false otherwise. */
        inline bool isSelected(void)
        const
        {
            return _selected;
        } // isSelected
        
        /*! @brief Returns an entry at the given location, if it exists.
         @param location The coordinates to check.
         @returns A pointer to the entry at the given location, or @c NULL if there is none. */
        ChannelEntry * locateEntry(const Point<float> & location)
        const;
        
        /*! @brief Called when a mouse button is pressed.
         @param ee Details about the position and status of the mouse event. */
        virtual void mouseDown(const MouseEvent & ee);
        
        /*! @brief Called when the mouse is moved while a button is held down.
         @param ee Details about the position and status of the mouse event. */
        virtual void mouseDrag(const MouseEvent & ee);
        
        /*! @brief Draw the content of the component.
         @param gg The graphics context in which to draw. */
        virtual void paint(Graphics & gg);
        
        /*! @brief Remove connections that are invalid. */
        void removeInvalidConnections(void);
        
        /*! @brief Called when the component size has been changed. */
        virtual void resized(void);
        
        /*! @brief Select the entity. */
        inline void select(void)
        {
            _selected = true;
        } // select
        
        /*! @brief Sets the node corresponding to the entity.
         @param newNode The new value for the node corresponding to the entity. */
        inline void setNode(ogdf::node newNode)
        {
            _node = newNode;
        } // setNode
        
        /*! @brief Marks the entity as not newly created. */
        inline void setOld(void)
        {
            _newlyCreated = false;
        } // setOld
        
        /*! @brief Sets the visited flag for the entity. */
        inline void setVisited(void)
        {
            _visited = true;
        } // setVisited
        
        /*! @brief Returns the state of the visited flag.
         @returns The state of the visited flag. */
        inline bool wasVisited(void)
        const
        {
            return _visited;
        } // wasVisited
        
    protected:
        
    private:
        
        /*! @brief The class that this class is derived from. */
        typedef Component inherited;
        
        /*! @brief Restrictions on the components size or position. */
        ComponentBoundsConstrainer _constrainer;
        
        /*! @brief Used to take care of the logic for dragging the component. */
        ComponentDragger _dragger;
        
        /*! @brief The behavioural model if a service. */
        String _behaviour;
        
        /*! @brief The description of the container, if it is a service. */
        String _description;
        
        /*! @brief The node corresponding to the container. */
        ogdf::node _node;
        
        /*! @brief The owner of the container. */
        EntitiesPanel & _owner;
        
        /*! @brief The height of the title of the container. */
        float _titleHeight;
        
        /*! @brief The kind of container. */
        ContainerKind _kind;
        
        /*! @brief @c true if the container is selected and @c false otherwise. */
        bool _selected;
        
        /*! @brief @c true if the container was visited and @c false otherwise. */
        bool _visited;
        
        /*! @brief @c true if the container has just been created and @c false otherwise. */
        bool _newlyCreated;
        
# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunused-private-field"
# endif // defined(__APPLE__)
        /*! @brief Filler to pad to alignment boundary */
        char _filler[5];
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChannelContainer)
        
    }; // ChannelContainer
    
} // ChannelManager

#endif // ! defined(ChannelContainer_H_)
