//--------------------------------------------------------------------------------------------------
//
//  File:       m+mChannelContainer.hpp
//
//  Project:    m+m
//
//  Contains:   The class declaration for a visible entity that has one or more channels or ports.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2014 by H Plus Technologies Ltd. and Simon Fraser University.
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

#if (! defined(mpmChannelContainer_HPP_))
# define mpmChannelContainer_HPP_ /* Header guard */

# include "m+mManagerDataTypes.hpp"

# if defined(USE_OGDF_POSITIONING_)
#  if MAC_OR_LINUX_
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wunused-parameter"
#  else // ! MAC_OR_LINUX_
#   pragma warning(push)
#   pragma warning(disable: 4100)
#   pragma warning(disable: 4512)
#  endif // ! MAC_OR_LINUX_
#  include <ogdf/basic/Graph.h>
#  if MAC_OR_LINUX_
#   pragma clang diagnostic pop
#  else // ! MAC_OR_LINUX_
#   pragma warning(pop)
#  endif // ! MAC_OR_LINUX_
# endif // defined(USE_OGDF_POSITIONING_)

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file

 @brief The class declaration for a visible entity that has one or more channels or ports. */

/*! @namespace MPlusM_Manager
 @brief The classes that implement the m+m Manager application. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace MPlusM_Manager
{
    class EntitiesPanel;

    /*! @brief A container for one or more ports or channels. */
    class ChannelContainer : public Component
    {
    public :

    protected :

    private :

        /*! @brief The class that this class is derived from. */
        typedef Component inherited;

    public :

        /*! @brief The constructor.
         @param[in] kind The kind of entity.
         @param[in] title The title of the entity.
         @param[in] ipAddress The IP address of the entity.
         @param[in] behaviour The behavioural model if a service or an adapter.
         @param[in] description The description, if this is a service or an adapter.
         @param[in] extraInfo The extra information, if this is a service or an adapter.
         @param[in] requests The requests supported, if this is a service or an adapter.
         @param[in] owner The owner of the entity. */
        ChannelContainer(const ContainerKind kind,
                         const YarpString &  title,
                         const YarpString &  ipAddress,
                         const YarpString &  behaviour,
                         const YarpString &  description,
                         const YarpString &  extraInfo,
                         const YarpString &  requests,
                         EntitiesPanel &     owner);

        /*! @brief The destructor. */
        virtual
        ~ChannelContainer(void);

        /*! @brief Add an argument description to the container.
         @param[in] argDesc The argument descriptor to be added to the container. */
        void
        addArgumentDescription(MplusM::Utilities::BaseArgumentDescriptor * argDesc);

        /*! @brief Add a port to the panel.
         @param[in] portName The name of the port.
         @param[in] portNumber The port number of the port.
         @param[in] portProtocol The protocol of the port.
         @param[in] protocolDescription The description of the protocol.
         @param[in] portKind What the port will be used for.
         @param[in] direction The primary direction of the port.
         @return The newly-created port. */
        ChannelEntry *
        addPort(const YarpString &  portName,
                const YarpString &  portNumber,
                const YarpString &  portProtocol = "",
                const YarpString &  protocolDescription = "",
                const PortUsage     portKind = kPortUsageOther,
                const PortDirection direction = kPortDirectionInputOutput);

        /*! @brief Returns @c true if the container has configurable values and @c false otherwise.
         @return @c true if the container has configurable values and @c false otherwise. */
        bool
        canBeConfigured(void);

        /*! @brief Clears the hidden flag for the entity. */
        void
        clearHidden(void);

        /*! @brief Clear any connect / disconnect markers. */
        void
        clearMarkers(void);

        /*! @brief Clears the visited flag for the entity. */
        void
        clearVisited(void);

        /*! @brief Set the configuration of the corresponding service. */
        void
        configureTheService(void);

        /*! @brief Deselect the entity. */
        void
        deselect(void);

        /*! @brief Display information for a container.
         @param[in] moreDetails @c true if more details are to be shown and @c false otherwise. */
        void
        displayInformation(const bool moreDetails);

        /*! @brief Display metrics for a service. */
        void
        displayMetrics(void);

        /*! @brief Display the connections between containers.
         @param[in,out] gg The graphics context in which to draw. */
        void
        drawOutgoingConnections(Graphics & gg);

        /*! @brief Convert a tab-delimited line of metric data into a more readable form.
         @param[in] aRow A line of metric data.
         @return The metric data reformatted. */
        String
        formatMetricRow(const String & aRow);

        /*! @brief Return a particular argument descriptor.
         @param[in] idx The index of the argument of interest.
         @return The argument descriptor at the specified index. */
        MplusM::Utilities::BaseArgumentDescriptor *
        getArgumentDescriptor(const size_t idx)
        const;

        /*! @brief Return the behavioural model for the entity.
         @return The behavioural model for the entity. */
        inline const YarpString &
        getBehaviour(void)
        const
        {
            return _behaviour;
        } // getBehaviour

        /*! @brief Return the description of the entity.
         @return The description of the entity. */
        inline const YarpString &
        getDescription(void)
        const
        {
            return _description;
        } // getDescription

        /*! @brief Return the extra information for the entity.
         @return The extra information for the entity. */
        inline const YarpString &
        getExtraInformation(void)
        const
        {
            return _extraInfo;
        } // getExtraInformation

        /*! @brief Return the IP address of the container.
         @return The IP address of the container. */
        inline const YarpString &
        getIPAddress(void)
        const
        {
            return _IPAddress;
        } // getIPAddress

        /*! @brief Return the kind of container.
         @return The kind of container. */
        inline ContainerKind
        getKind(void)
        const
        {
            return _kind;
        } // getKind

        /*! @brief Return the metrics for the container, if it is a service.
         @return The metrics for the container, if it is a service or an empty string. */
        StringArray
        getMetrics(void);

        /*! @brief Return the state of measurment collection for the container, if it is a service.
         @return @c true if the service is collecting measurements and @c false if it is not a
         service or if it is a service that is not collecting measurements. */
        bool
        getMetricsState(void);

# if defined(USE_OGDF_POSITIONING_)
        /*! @brief Return the node corresponding to the entity.
         @return The node corresponding to the entity. */
        ogdf::node
        getNode(void)
        const;
# endif // defined(USE_OGDF_POSITIONING_)

        /*! @brief Returns the number of argument descriptions in this container.
         @return The number of argument descriptions in this container. */
        inline size_t
        getNumArgumentDescriptors(void)
        const
        {
            return _argumentList.size();
        } // getNumArgumentDescriptors

        /*! @brief Returns the number of ports in this container.
         @return The number of ports in this container. */
        inline int
        getNumPorts(void)
        const
        {
            return getNumChildComponents();
        } // getNumPorts

        inline EntitiesPanel &
        getOwner(void)
        const
        {
            return _owner;
        } // getOwner

        /*! @brief Returns a port by index.
         @param[in] num The zero-origin index of the port.
         @return A port or @c NULL if the index is out of range. */
        ChannelEntry *
        getPort(const int num)
        const;

        /*! @brief Return the position of the entity within it's containing panel.
         @return The position of the entity within it's containing panel. */
        Position
        getPositionInPanel(void)
        const;

        /*! @brief Return the requests supported by the entity.
         @return The requests supported by the entity. */
        inline const YarpString &
        getRequests(void)
        const
        {
            return _requests;
        } // getRequests

        /*! @brief Return the amount of space to the left of the text being displayed.
         @return The amount of space to the left of the text being displayed. */
        float
        getTextInset(void)
        const;

        /*! @brief Check if a port is part of the entity.
         @param[in] aPort The port to be checked for.
         @return @c true if the port is contained within the entity and @c false otherwise. */
        bool
        hasPort(const ChannelEntry * aPort);

        /*! @brief Mark the entity as invisible. */
        void
        hide(void);

        /*! @brief Mark all the connections as invalid. */
        void
        invalidateConnections(void);

        /*! @brief Returns @c true if one of the port entries is marked and @c false otherwise.
         @return @c true if one of the port entries is marked and @c false otherwise. */
        bool
        isMarked(void)
        const;

        /*! @brief Return @c true is the entity is newly created.
         @return @c true if the entity is newly created and @c false otherwise. */
        inline bool
        isNew(void)
        const
        {
            return _newlyCreated;
        } // isNew

        /*! @brief Return @c true is the entity is selected.
         @return @c true if the entity is selected and @c false otherwise. */
        inline bool
        isSelected(void)
        const
        {
            return _selected;
        } // isSelected

        /*! @brief Returns an entry at the given location, if it exists.
         @param[in] location The coordinates to check.
         @return A pointer to the entry at the given location, or @c NULL if there is none. */
        ChannelEntry *
        locateEntry(const Position & location)
        const;

        /*! @brief Called when a mouse button is pressed.
         @param[in] ee Details about the position and status of the mouse event. */
        virtual void
        mouseDown(const MouseEvent & ee);

        /*! @brief Called when the mouse is moved while a button is held down.
         @param[in] ee Details about the position and status of the mouse event. */
        virtual void
        mouseDrag(const MouseEvent & ee);

        /*! @brief Remove connections that are invalid. */
        void
        removeInvalidConnections(void);

        /*! @brief Called when the component size has been changed. */
        virtual void
        resized(void);

        /*! @brief Tell the corresponding service to restart. */
        void
        restartTheService(void);

        /*! @brief Select the entity. */
        void
        select(void);

        /*! @brief Sets the hidden flag for the entity. */
        void
        setHidden(void);

        /*! @brief Change the state of service metrics collection.
         @param[in] newState The requested state of service metrics collection. */
        void
        setMetricsState(const bool newState);

# if defined(USE_OGDF_POSITIONING_)
        /*! @brief Sets the node corresponding to the entity.
         @param[in] newNode The new value for the node corresponding to the entity. */
        void
        setNode(ogdf::node newNode);
# endif // defined(USE_OGDF_POSITIONING_)

        /*! @brief Marks the entity as not newly created. */
        void
        setOld(void);

        /*! @brief Sets the visited flag for the entity. */
        void
        setVisited(void);

        /*! @brief Tell the corresponding service to stop. */
        void
        stopTheService(void);

        /*! @brief Returns the state of the hidden flag.
         @return The state of the hidden flag. */
        inline bool
        wasHidden(void)
        const
        {
            return _hidden;
        } // wasHidden

        /*! @brief Returns the state of the visited flag.
         @return The state of the visited flag. */
        inline bool
        wasVisited(void)
        const
        {
            return _visited;
        } // wasVisited

    protected :

    private :

        /*! @brief Respond to a request for a popup menu. */
        void
        displayAndProcessPopupMenu(void);

        /*! @brief Draw the content of the component.
         @param[in,out] gg The graphics context in which to draw. */
        virtual void
        paint(Graphics & gg);

    public :

    protected :

    private :

        /*! @brief The argument descriptions if it is a service or an adapter. */
        MplusM::Utilities::DescriptorVector _argumentList;

        /*! @brief Restrictions on the components size or position. */
        ComponentBoundsConstrainer _constrainer;

        /*! @brief Used to take care of the logic for dragging the component. */
        ComponentDragger _dragger;

        /*! @brief The behavioural model if a service or an adapter. */
        YarpString _behaviour;

        /*! @brief The description of the container, if it is a service or an adapter. */
        YarpString _description;

        /*! @brief The extra information for the container, if it is a service or an adapter. */
        YarpString _extraInfo;

        /*! @brief The IP address of the primary channel. */
        YarpString _IPAddress;

        /*! @brief The requests for the entity, if it is a service or an adapter. */
        YarpString _requests;

# if defined(USE_OGDF_POSITIONING_)
        /*! @brief The node corresponding to the container. */
        ogdf::node _node;
# endif // defined(USE_OGDF_POSITIONING_)

        /*! @brief The owner of the container. */
        EntitiesPanel & _owner;

        /*! @brief The height of the title of the container. */
        int _titleHeight;

        /*! @brief The kind of container. */
        ContainerKind _kind;

        /*! @brief @c true if the container was hidden and @c false otherwise. */
        bool _hidden;

        /*! @brief @c true if the container has just been created and @c false otherwise. */
        bool _newlyCreated;

        /*! @brief @c true if the container is selected and @c false otherwise. */
        bool _selected;

        /*! @brief @c true if the container was visited and @c false otherwise. */
        bool _visited;

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunused-private-field"
# endif // defined(__APPLE__)
        /*! @brief Filler to pad to alignment boundary */
        char _filler[4];
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChannelContainer)

    }; // ChannelContainer

} // MPlusM_Manager

#endif // ! defined(mpmChannelContainer_HPP_)
