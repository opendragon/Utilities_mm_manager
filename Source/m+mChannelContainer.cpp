//--------------------------------------------------------------------------------------------------
//
//  File:       m+mChannelContainer.cpp
//
//  Project:    m+m
//
//  Contains:   The class definition for a visible entity that has one or more channels or ports.
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

#include "m+mChannelContainer.h"

#include "m+mChannelEntry.h"
#include "m+mContentPanel.h"
#include "m+mEntitiesPanel.h"

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for a visible entity that has one or more channels or ports. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MPlusM_Manager;
using namespace std;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief The colour to be used for the gap between entries. */
static const Colour & kGapFillColour(Colours::grey);

/*! @brief The colour to be used for the heading of the container. */
static const Colour & kHeadingBackgroundColour(Colours::darkgrey);

/*! @brief The colour to be used for text in the entry. */
static const Colour & kHeadingTextColour(Colours::white);

/*! @brief The amount of space between each row of the entries in the container. */
static const float kEntryGap = 1;

/*! @brief The amount of space to the left of the text being displayed. */
static const float kTextInset = 2;

#if defined(__APPLE__)
# pragma mark Global constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and Destructors
#endif // defined(__APPLE__)

ChannelContainer::ChannelContainer(const ContainerKind kind,
                                   const YarpString &  title,
                                   const YarpString &  ipAddress,
                                   const YarpString &  behaviour,
                                   const YarpString &  description,
                                   const YarpString &  extraInfo,
                                   const YarpString &  requests,
                                   EntitiesPanel &     owner) :
    inherited(title.c_str()), _behaviour(behaviour), _description(description),
    _extraInfo(extraInfo), _IPAddress(ipAddress), _requests(requests),
#if defined(USE_OGDF_POSITIONING_)
    _node(NULL),
#endif // defined(USE_OGDF_POSITIONING_)
    _owner(owner), _kind(kind), _hidden(false), _newlyCreated(true), _selected(false),
    _visited(false)
{
    OD_LOG_ENTER(); //####
    OD_LOG_S4s("title = ", title, "behaviour = ", behaviour, "description = ", description, //####
               "extraInfo = ", extraInfo); //####
    OD_LOG_S2s("requests = ", requests, "ipAddress = ", ipAddress); //####
    Font & headerFont = _owner.getNormalFont();
    
    _titleHeight = static_cast<int>(headerFont.getHeight());
    setSize(static_cast<int>(headerFont.getStringWidthFloat(getName() + " ") + getTextInset()),
            _titleHeight);
    OD_LOG_LL2("width = ", getWidth(), "height = ", getHeight()); //####
    setOpaque(true);
    setVisible(true);
    OD_LOG_EXIT_P(this); //####
} // ChannelContainer::ChannelContainer

ChannelContainer::~ChannelContainer(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("getName() = ", getName().toStdString()); //####
    for (int ii = 0, mm = getNumPorts(); mm > ii; ++ii)
    {
        ChannelEntry * aPort = getPort(ii);
        
        if (aPort)
        {
            _owner.forgetPort(aPort);
        }
    }
    deleteAllChildren();
    for (size_t ii = 0, mm = _argumentList.size(); mm > ii; ++ii)
    {
        Utilities::BaseArgumentDescriptor * argDesc = _argumentList[ii];

        delete argDesc;
    }
    _argumentList.clear();
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::~ChannelContainer

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

void ChannelContainer::addArgumentDescription(MplusM::Utilities::BaseArgumentDescriptor * argDesc)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("argDesc = ", argDesc); //####
    _argumentList.push_back(argDesc->clone());
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::addArgumentDescription

ChannelEntry * ChannelContainer::addPort(const YarpString &  portName,
                                         const YarpString &  portNumber,
                                         const YarpString &  portProtocol,
                                         const YarpString &  protocolDescription,
                                         const PortUsage     portKind,
                                         const PortDirection direction)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S4s("portName = ", portName, "portNumber = ", portNumber, "portProtocol = ", //####
               portProtocol, "protocolDescription = ", protocolDescription); //####
    int            countBefore = getNumPorts();
    ChannelEntry * aPort = new ChannelEntry(this, portName, portNumber, portProtocol,
                                            protocolDescription, portKind, direction);
    float          newWidth = static_cast<float>(jmax(aPort->getWidth(), getWidth()));
    float          newHeight = aPort->getHeight() + getHeight() + kEntryGap;
    
    OD_LOG_LL2("newWidth = ", newWidth, "newHeight = ", newHeight); //####
    aPort->setTopLeftPosition(0, static_cast<int>(getHeight() + kEntryGap));
    setSize(static_cast<int>(newWidth), static_cast<int>(newHeight));
    addAndMakeVisible(aPort);
    for (int ii = 0; countBefore >= ii; ++ii)
    {
        ChannelEntry * bPort = getPort(ii);
        
        if (bPort)
        {
            bPort->setSize(static_cast<int>(newWidth), bPort->getHeight());
        }
    }
    if (0 < countBefore)
    {
        ChannelEntry * bPort = getPort(countBefore - 1);
        
        if (bPort)
        {
            bPort->unsetAsLastPort();
        }
    }
    OD_LOG_OBJEXIT_P(aPort); //####
    return aPort;
} // ChannelContainer::addPort

bool ChannelContainer::canBeConfigured(void)
{
    OD_LOG_OBJENTER(); //####
    bool   result;
    size_t mm = _argumentList.size();

    if (mm)
    {
        result = true;
        for (size_t ii = 0; mm > ii; ++ii)
        {
            MplusM::Utilities::BaseArgumentDescriptor * argDesc = _argumentList[ii];

            if (argDesc)
            {
                if (0 == (argDesc->argumentMode() & MplusM::Utilities::kArgModeModifiable))
                {
                    result = false;
                }
            }
        }
    }
    else
    {
        result = false;
    }
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // ChannelContainer::canBeConfigured

void ChannelContainer::clearHidden(void)
{
    OD_LOG_OBJENTER(); //####
    _hidden = false;
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::clearHidden

void ChannelContainer::clearMarkers(void)
{
    OD_LOG_OBJENTER(); //####
    for (int ii = 0, mm = getNumPorts(); mm > ii; ++ii)
    {
        ChannelEntry * aPort = getPort(ii);
        
        if (aPort)
        {
            aPort->clearConnectMarker();
            aPort->clearDisconnectMarker();
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::clearMarkers

void ChannelContainer::clearVisited(void)
{
    OD_LOG_OBJENTER(); //####
    _visited = false;
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::clearVisited

void ChannelContainer::deselect(void)
{
    OD_LOG_OBJENTER(); //####
    _selected = false;
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::deselect

void ChannelContainer::displayAndProcessPopupMenu(void)
{
    OD_LOG_OBJENTER(); //####
    PopupMenu mm;

    mm.setLookAndFeel(&getLookAndFeel());
    getOwner().getContent()->setUpContainerMenu(mm, *this);
    int result = mm.show();
    
    switch (result)
    {
        case kPopupConfigureService :
            break;
            
        case kPopupDisplayChangeServiceMetrics :
            setMetricsState(! getMetricsState());
            break;
            
        case kPopupDisplayEntityInfo :
            displayInformation(false);
            break;
            
        case kPopupDetailedDisplayEntityInfo :
            displayInformation(true);
            break;
            
        case kPopupDisplayServiceMetrics :
            displayMetrics();
            break;
           
        case kPopupHideEntity :
            hide();
            break;
            
        case kPopupRestartService :
            restartTheService();
            break;

        case kPopupStopService :
            stopTheService();
            break;
            
        default :
            break;
            
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::displayAndProcessPopupMenu

void ChannelContainer::displayInformation(const bool moreDetails)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_B1("moreDetails = ", moreDetails); //####
    // Popup of description.
    YarpString thePanelDescription;
    
    switch (_kind)
    {
        case kContainerKindAdapter :
        case kContainerKindService :
            thePanelDescription = YarpString("The ") + getDescription();
            break;
            
        case kContainerKindOther :
            thePanelDescription = "A standard entity";
            break;
            
        default :
            break;
            
    }
    String bodyText("Address: ");
    
    bodyText += (getIPAddress() + "\n").c_str();
    bodyText += thePanelDescription.c_str();
    if (moreDetails)
    {
        YarpString extraInfo = getExtraInformation();
        YarpString requests;

        if (0 < extraInfo.length())
        {
            bodyText += "\n";
            bodyText += extraInfo.c_str();
        }
        switch (_kind)
        {
            case kContainerKindAdapter :
            case kContainerKindService :
                requests = getRequests();
                if (0 < requests.length())
                {
                    bodyText += "\n\n";
                    bodyText += requests.c_str();
                    bodyText += "\n";
                }
                break;
                
            default :
                break;
                
        }
    }
    DisplayInformationPanel(this, bodyText, getName());
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::displayInformation

void ChannelContainer::displayMetrics(void)
{
    OD_LOG_OBJENTER(); //####
    // Popup of metrics.
    String      result;
    StringArray metricsArray = getMetrics();
    int         numRows = metricsArray.size();
    
    for (int ii = 0; ii < numRows; ++ii)
    {
        const String & aRow = metricsArray[ii];

        if (0 < ii)
        {
            result += "\n";
        }
        result += formatMetricRow(aRow) + "\n";
    }
    DisplayInformationPanel(this, result, String("Metrics for ") + getName());
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::displayMetrics

void ChannelContainer::drawOutgoingConnections(Graphics & gg)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("gg = ", &gg); //####
    for (int ii = 0, mm = getNumPorts(); mm > ii; ++ii)
    {
        ChannelEntry * aPort = getPort(ii);
        
        if (aPort)
        {
            aPort->drawOutgoingConnections(gg);
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::drawOutgoingConnections

String ChannelContainer::formatMetricRow(const String & aRow)
{
    OD_LOG_OBJENTER();
    OD_LOG_S1s("aRow = ", aRow.toStdString()); //####
    String      result;
    StringArray asPieces;
    
    asPieces.addTokens(aRow, "\t", "");
    if (7 == asPieces.size())
    {
        // We are ignoring the date and time information.
        String channelName = asPieces[0];
        String inBytes = asPieces[3];
        String outBytes = asPieces[4];
        String inMessages = asPieces[5];
        String outMessages = asPieces[6];

        result = "Channel:      " + channelName + "\n";
        result += "In bytes:     " + inBytes + "\n";
        result += "Out bytes:    " + outBytes + "\n";
        result += "In messages:  " + inMessages + "\n";
        result += "Out messages: " + outMessages;
    }
    OD_LOG_OBJEXIT_S(result.toStdString().c_str());
    return result;
} // ChannelContainer::formatMetricRow

MplusM::Utilities::BaseArgumentDescriptor * ChannelContainer::getArgumentDescriptor(const size_t
                                                                                                idx)
const
{
    OD_LOG_ENTER(); //####
    OD_LOG_LL1(idx, idx); //####
    MplusM::Utilities::BaseArgumentDescriptor * result;

    if (_argumentList.size() > idx)
    {
        result = _argumentList[idx];
    }
    else
    {
        result = NULL;
    }
    OD_LOG_EXIT_P(result); //####
    return result;
} // ChannelContainer::getArgumentDescriptor

StringArray ChannelContainer::getMetrics(void)
{
    OD_LOG_OBJENTER(); //####
    StringArray result;
    
    for (int ii = 0, mm = getNumPorts(); mm > ii; ++ii)
    {
        ChannelEntry * aPort = getPort(ii);
        
        if (aPort && aPort->isService())
        {
            yarp::os::Bottle metrics;
            
            if (Utilities::GetMetricsForService(aPort->getPortName(), metrics, STANDARD_WAIT_TIME_))
            {
                String metricsString = Utilities::ConvertMetricsToString(metrics,
                                                             Common::kOutputFlavourTabs).c_str();
                
                result.addLines(metricsString);
                break;
            }
            
        }
    }
    OD_LOG_OBJEXIT(); //####
    return result;
} // ChannelContainer::getMetrics

bool ChannelContainer::getMetricsState(void)
{
    OD_LOG_OBJENTER(); //####
    bool result = false;
    
    for (int ii = 0, mm = getNumPorts(); mm > ii; ++ii)
    {
        ChannelEntry * aPort = getPort(ii);
        
        if (aPort && aPort->isService())
        {
            if (Utilities::GetMetricsStateForService(aPort->getPortName(), result,
                                                     STANDARD_WAIT_TIME_))
            {
                break;
            }
            
        }
    }
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // ChannelContainer::getMetricsState

#if defined(USE_OGDF_POSITIONING_)
ogdf::node ChannelContainer::getNode(void)
const
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT_P(_node); //####
    return _node;
} // ChannelContainer::getNode
#endif // defined(USE_OGDF_POSITIONING_)

ChannelEntry * ChannelContainer::getPort(const int num)
const
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_LL1("num = ", num); //####
    ChannelEntry * result;
    
    if ((0 <= num) && (getNumPorts() > num))
    {
        result = reinterpret_cast<ChannelEntry *>(getChildComponent(num));
    }
    else
    {
        result = NULL;
    }
    OD_LOG_OBJEXIT_P(result); //####
    return result;
} // ChannelContainer::getPort

Position ChannelContainer::getPositionInPanel(void)
const
{
    OD_LOG_OBJENTER(); //####
    Position result(getPosition().toFloat());
    
    OD_LOG_OBJEXIT(); //####
    return result;
} // ChannelContainer::getPositionInPanel

float ChannelContainer::getTextInset(void)
const
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT_D(kTextInset); //####
    return kTextInset;
} // ChannelContainer::getTextInset

bool ChannelContainer::hasPort(const ChannelEntry * aPort)
{
    OD_LOG_OBJENTER(); //####
    bool result = false;
    
    for (int ii = 0, mm = getNumPorts(); mm > ii; ++ii)
    {
        ChannelEntry * anEntry = getPort(ii);
        
        if (aPort == anEntry)
        {
            result = true;
            break;
        }
        
    }
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // ChannelContainer::hasPort

void ChannelContainer::hide(void)
{
    OD_LOG_OBJENTER(); //####
    ContentPanel * thePanel = _owner.getContent();
    
    setVisible(false);
    if (thePanel)
    {
        thePanel->setChannelOfInterest(NULL);
        thePanel->setContainerOfInterest(NULL);
        thePanel->requestWindowRepaint();
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::hide

void ChannelContainer::invalidateConnections(void)
{
    OD_LOG_OBJENTER(); //####
    for (int ii = 0, mm = getNumPorts(); mm > ii; ++ii)
    {
        ChannelEntry * aPort = getPort(ii);
        
        if (aPort)
        {
            aPort->invalidateConnections();
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::invalidateConnections

bool ChannelContainer::isMarked(void)
const
{
    OD_LOG_OBJENTER(); //####
    bool marked = false;
    
    for (int ii = 0, mm = getNumPorts(); mm > ii; ++ii)
    {
        ChannelEntry * aPort = getPort(ii);
        
        if (aPort && aPort->isMarked())
        {
            marked = true;
            break;
        }
        
    }
    OD_LOG_OBJEXIT_B(marked); //####
    return marked;
} // ChannelContainer::isMarked

ChannelEntry * ChannelContainer::locateEntry(const Position & location)
const
{
    OD_LOG_OBJENTER(); //####
    ChannelEntry * result = NULL;
    
    for (int ii = 0, mm = getNumPorts(); mm > ii; ++ii)
    {
        ChannelEntry * aPort = getPort(ii);
        
        if (aPort)
        {
            Position where = aPort->getPositionInPanel();
            
            if (aPort->reallyContains((location - where).toInt(), true))
            {
                result = aPort;
                break;
            }
            
        }
    }
    OD_LOG_OBJEXIT_P(result); //####
    return result;
} // ChannelContainer::locateEntry

void ChannelContainer::mouseDown(const MouseEvent & ee)
{
    OD_LOG_OBJENTER(); //####
    bool doDrag = true;
    
    // Prepares our dragger to drag this Component
    if (ee.mods.isAltDown() || ee.mods.isCommandDown())
    {
        doDrag = false;
    }
    else if (ee.mods.isPopupMenu())
    {
        _owner.getContent()->setContainerOfInterest(this);
        displayAndProcessPopupMenu();
        doDrag = false;
    }
    else
    {
        _owner.getContent()->setContainerOfInterest(this);
    }
    if (doDrag)
    {
        _dragger.startDraggingComponent(this, ee);
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::mouseDown

void ChannelContainer::mouseDrag(const MouseEvent & ee)
{
    OD_LOG_OBJENTER(); //####
    bool doDrag = true;
    
    // Moves this Component according to the mouse drag event and applies our constraints to it
    if (ee.mods.isAltDown() || ee.mods.isCommandDown() || ee.mods.isPopupMenu())
    {
		doDrag = false;
    }
    if (doDrag)
    {
#if (! SETTINGS_FOR_MANUAL_)
        _dragger.dragComponent(this, ee, &_constrainer);
#endif // ! SETTINGS_FOR_MANUAL_
        _owner.repaint();
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::mouseDrag

void ChannelContainer::paint(Graphics & gg)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("gg = ", &gg); //####
    AttributedString as;
    
    as.setJustification(Justification::left);
    as.append(getName(), _owner.getNormalFont(), kHeadingTextColour);
    juce::Rectangle<int> localBounds(getLocalBounds());
    juce::Rectangle<int> area1(localBounds.getX(), localBounds.getY(), localBounds.getWidth(),
							   _titleHeight);
    juce::Rectangle<int> area2(localBounds.getX(), localBounds.getY() + _titleHeight,
							   localBounds.getWidth(), localBounds.getHeight() - _titleHeight);
    
    gg.setColour(kHeadingBackgroundColour);
    gg.fillRect(area1);
    area1.setLeft(static_cast<int>(area1.getX() + getTextInset()));
    as.draw(gg, area1.toFloat());
    gg.setColour(kGapFillColour);
    gg.fillRect(area2);    
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::paint

void ChannelContainer::removeInvalidConnections(void)
{
    OD_LOG_OBJENTER(); //####
    for (int ii = 0, mm = getNumPorts(); mm > ii; ++ii)
    {
        ChannelEntry * aPort = getPort(ii);
        
        if (aPort)
        {
            aPort->removeInvalidConnections();
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::invalidateConnections

void ChannelContainer::resized(void)
{
    OD_LOG_OBJENTER(); //####
    // Just set the limits of our constrainer so that we don't drag ourselves off the screen
    _constrainer.setMinimumOnscreenAmounts(getHeight(), getWidth(),
                                           static_cast<int>(getHeight() * 0.8),
                                           static_cast<int>(getWidth() * 0.8));
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::resized

void ChannelContainer::restartTheService(void)
{
    OD_LOG_OBJENTER(); //####
    for (int ii = 0, mm = getNumPorts(); mm > ii; ++ii)
    {
        ChannelEntry * aPort = getPort(ii);

        if (aPort && aPort->isService())
        {
            bool restartable;

            switch (Utilities::MapStringToServiceKind(getBehaviour()))
            {
                case Common::kServiceKindAdapter :
                case Common::kServiceKindFilter :
                case Common::kServiceKindInput :
                case Common::kServiceKindOutput :
                    restartable = true;
                    break;

                default :
                    restartable = false;
                    break;
                    
            }
            if (restartable)
            {
                Utilities::RestartAService(aPort->getPortName(), STANDARD_WAIT_TIME_);
            }
            break;

        }
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::restartTheService

void ChannelContainer::select(void)
{
    OD_LOG_OBJENTER(); //####
    _selected = true;
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::select

void ChannelContainer::setHidden(void)
{
    OD_LOG_OBJENTER(); //####
    _hidden = true;
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::setHidden

void ChannelContainer::setMetricsState(const bool newState)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_B1("newState = ", newState); //####
    for (int ii = 0, mm = getNumPorts(); mm > ii; ++ii)
    {
        ChannelEntry * aPort = getPort(ii);
        
        if (aPort && aPort->isService())
        {
            if (Utilities::SetMetricsStateForService(aPort->getPortName(), newState,
                                                     STANDARD_WAIT_TIME_))
            {
                break;
            }
            
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::setMetricsState

#if defined(USE_OGDF_POSITIONING_)
void ChannelContainer::setNode(ogdf::node newNode)
{
    OD_LOG_OBJENTER(); //####
    _node = newNode;
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::setNode
#endif // defined(USE_OGDF_POSITIONING_)

void ChannelContainer::setOld(void)
{
    OD_LOG_OBJENTER(); //####
    _newlyCreated = false;
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::setOld

void ChannelContainer::setVisited(void)
{
    OD_LOG_OBJENTER(); //####
    _visited = true;
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::setVisited

void ChannelContainer::stopTheService(void)
{
    OD_LOG_OBJENTER(); //####
    for (int ii = 0, mm = getNumPorts(); mm > ii; ++ii)
    {
        ChannelEntry * aPort = getPort(ii);
        
        if (aPort && aPort->isService())
        {
            bool doStop;
            
            if (Utilities::PortIsRegistryService(aPort->getPortName()))
            {
                doStop = (1 == AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon,
                                                            "Are you sure that you want to stop "
                                                            "the Registry Service?", "If you do, "
                                                            "it may take a few moments to "
                                                            "disappear from the display, depending "
                                                            "on network traffic. "
                                                            "Also, stopping the Registry Service "
                                                            "will interfere with the ability of "
                                                            "clients and adapters to locate their "
                                                            "corresponding services, and this "
                                                            "application will not be able to "
                                                            "identify services, clients or "
                                                            "adapters or to group secondary ports "
                                                            "together with their primary ports.",
                                                            "Yes", String::empty, NULL,
                                                            NULL));
            }
            else
            {
                String containerType;
                String messageText("If you do, it may take a few moments to disappear from the "
                                   "display, depending on network traffic. "
                                   "Also, the ");
                String titleText("Are you sure that you want to stop this ");
                
                if (kContainerKindAdapter == _kind)
                {
                    containerType = "adapter";
                }
                else if (kContainerKindService == _kind)
                {
                    containerType = "service";
                }
                else
                {
                    containerType = "entity";
                }
                titleText += containerType + "?";
                messageText += containerType + " will not exit if it is waiting on a command "
                                "prompt, until a command is issued.";
                doStop = (1 == AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon, titleText,
                                                            messageText, "Yes", String::empty,
                                                            NULL, NULL));
            }
            if (doStop)
            {
                ContentPanel * thePanel = _owner.getContent();
                
                if (thePanel)
                {
                    thePanel->setChannelOfInterest(NULL);
                    thePanel->setContainerOfInterest(NULL);
                }
                Utilities::StopAService(aPort->getPortName(), STANDARD_WAIT_TIME_);
                _owner.repaint();
            }
            break;
            
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::stopTheService

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
