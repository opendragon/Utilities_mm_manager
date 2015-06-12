//--------------------------------------------------------------------------------------------------
//
//  File:       ContentPanel.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for the content area of the primary window of the channel
//              manager application.
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
//  Created:    2014-07-21
//
//--------------------------------------------------------------------------------------------------

#include "ContentPanel.h"
#include "ChannelContainer.h"
#include "ChannelEntry.h"
#include "ChannelManagerApplication.h"
#include "ChannelManagerWindow.h"
#include "EntitiesPanel.h"
#include "EntityData.h"
#include "PortData.h"
#include "ScannerThread.h"

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(USE_OGDF_POSITIONING)
# if (! MAC_OR_LINUX_)
#  pragma warning(push)
#  pragma warning(disable: 4100)
# endif // ! MAC_OR_LINUX_
# include <ogdf/basic/GraphAttributes.h>
# include <ogdf/energybased/FMMMLayout.h>
# if (! MAC_OR_LINUX_)
#  pragma warning(pop)
# endif // ! MAC_OR_LINUX_
#endif // defined(USE_OGDF_POSITIONING)

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for the content area of the primary window of the channel manager
 application. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if (! MAC_OR_LINUX_)
# include <Windows.h>
#endif //! MAC_OR_LINUX_

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace ChannelManager;
using namespace MplusM;
using namespace std;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief The colour to be used for the dialog background. */
static const Colour & kDialogBackgroundColour(Colours::whitesmoke);

/*! @brief The first colour to be used for the panel background. */
static const Colour & kFirstBackgroundColour(Colours::darkgrey);

/*! @brief The second colour to be used for the panel background. */
static const Colour & kSecondBackgroundColour(Colours::lightgrey);

/*! @brief The first colour to be used for the selection rectangle. */
static const Colour & kFirstSelectionColour(Colours::greenyellow);

/*! @brief The second colour to be used for the selection rectangle. */
static const Colour & kSecondSelectionColour(Colours::darkmagenta);

/*! @brief The offset of the selection rectangle from the selected entity. */
static const float kSelectionOffset = 4;

/*! @brief The thickness of the selection rectangle. */
static const float kSelectionThickness = 2;

/*! @brief The initial thickness of the horizontal and vertical scrollbars. */
static const int kDefaultScrollbarThickness = 16;

/*! @brief The initial single-step size of the horizontal and vertical scrollbars. */
static const int kDefaultSingleStepSize = 10;

/*! @brief After width to be added to display panels. */
static const int kExtraDisplayWidth = 32;

#if defined(__APPLE__)
# pragma mark Global constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

/*! @brief Returns the absolute path to the settings file.
 @returns The absolute path to the settings file. */
static String getPathToSettingsFile(void)
{
    File   baseDir = File::getSpecialLocation(File::userApplicationDataDirectory);
    String baseDirAsString = File::addTrailingSeparator(baseDir.getFullPathName());
    String settingsDir = File::addTrailingSeparator(baseDirAsString + "ChannelManager");
    
    return settingsDir + "settings.txt";
} // getPathToSettingsFile

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and Destructors
#endif // defined(__APPLE__)

ContentPanel::ContentPanel(ChannelManagerWindow * containingWindow) :
    inherited1(), inherited2(), inherited3(), _entitiesPanel(new EntitiesPanel(this)),
    _menuBar(new MenuBarComponent(this)), _containingWindow(containingWindow),
    _selectedChannel(nullptr), _selectedContainer(nullptr), _channelClicked(false),
    _containerClicked(false),
#if (defined(USE_OGDF_POSITIONING) && defined(USE_OGDF_FOR_FIRST_POSITIONING_ONLY))
    _initialPositioningDone(false),
#endif // defined(USE_OGDF_POSITIONING) && defined(USE_OGDF_FOR_FIRST_POSITIONING_ONLY)
    _invertBackground(false), _skipNextScan(false), _whiteBackground(false)
{
    OD_LOG_ENTER(); //####
    addAndMakeVisible(_menuBar);
    _entitiesPanel->setSize(_entitiesPanel->getWidth(),
                            _entitiesPanel->getHeight() - _containingWindow->getTitleBarHeight());
    setSize(_entitiesPanel->getWidth(), _entitiesPanel->getHeight());
    setScrollBarsShown(true, true);
    setScrollBarThickness(kDefaultScrollbarThickness);
    setSingleStepSizes(kDefaultSingleStepSize, kDefaultSingleStepSize);
    _entitiesPanel->setVisible(true);
    setViewedComponent(_entitiesPanel);
    OD_LOG_EXIT_P(this); //####
} // ContentPanel::ContentPanel

ContentPanel::~ContentPanel(void)
{
    OD_LOG_OBJENTER(); //####
    PopupMenu::dismissAllActiveMenus();
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::~ContentPanel

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

void ContentPanel::getAllCommands(Array<CommandID> & commands)
{
    OD_LOG_OBJENTER(); //####
    static const CommandID ids[] =
        {
            ChannelManagerWindow::kCommandDoRepaint,
            ChannelManagerWindow::kCommandInvertBackground,
            ChannelManagerWindow::kCommandWhiteBackground,
            ChannelManagerWindow::kCommandClearSelection,
            ChannelManagerWindow::kCommandUnhideEntities,
            ChannelManagerWindow::kCommandLaunchRegistryService,
            ChannelManagerWindow::kCommandLaunchExecutables
        };
    
    commands.addArray(ids, numElementsInArray(ids));
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::getAllCommands

void ContentPanel::getCommandInfo(CommandID                commandID,
                                  ApplicationCommandInfo & result)
{
    OD_LOG_OBJENTER(); //####
    ChannelManagerApplication * ourApp = ChannelManagerApplication::getApp();

    switch (commandID)
    {
        case ChannelManagerWindow::kCommandDoRepaint :
            result.setInfo("Repaint", "Trigger a repaint of the window", "View", 0);
            result.addDefaultKeypress('R', ModifierKeys::commandModifier);
            break;
            
        case ChannelManagerWindow::kCommandInvertBackground :
            result.setInfo("Invert", "Invert the background gradient", "View", 0);
            result.addDefaultKeypress('I', ModifierKeys::commandModifier);
            result.setTicked(backgroundIsInverted());
            break;
            
        case ChannelManagerWindow::kCommandWhiteBackground :
            result.setInfo("White", "Use a white background", "View", 0);
            result.addDefaultKeypress('B', ModifierKeys::commandModifier);
            result.setTicked(backgroundIsWhite());
            break;
            
        case ChannelManagerWindow::kCommandClearSelection :
            result.setInfo("Clear selection", "Deselect any selected entities", "View", 0);
            result.addDefaultKeypress('C', ModifierKeys::commandModifier);
            result.setActive((nullptr != _selectedContainer) || (nullptr != _selectedChannel));
            break;
            
        case ChannelManagerWindow::kCommandUnhideEntities :
            result.setInfo("Unhide entities", "Unhide all hidden entities", "View", 0);
            result.addDefaultKeypress('U', ModifierKeys::commandModifier);
            result.setActive(0 < _entitiesPanel->getNumberOfHiddenEntities());
            break;
            
        case ChannelManagerWindow::kCommandLaunchRegistryService :
            result.setInfo("Launch Registry", "Launch the Registry Service", "View", 0);
            result.addDefaultKeypress('L', ModifierKeys::commandModifier);
            result.setActive(! Utilities::CheckForRegistryService());
            break;
            
        case ChannelManagerWindow::kCommandLaunchExecutables :
            result.setInfo("Launch others ...", "Launch other executables", "View", 0);
            result.addDefaultKeypress('O', ModifierKeys::commandModifier);
            result.setActive(Utilities::CheckForRegistryService() && ourApp &&
                             (0 < ourApp->getCountOfApplications()));
            break;
            
        default :
            break;
            
    }
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::getCommandInfo

StringArray ContentPanel::getMenuBarNames(void)
{
    OD_LOG_OBJENTER(); //####
    const char * const names[] = { "Channel Manager", "View", "Operation", nullptr };

    return StringArray(names);
} // ContentPanel::getMenuBarNames

#if (! MAC_OR_LINUX_)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif // ! MAC_OR_LINUX_
PopupMenu ContentPanel::getMenuForIndex(int            menuIndex,
                                        const String & menuName)
{
#if (! defined(OD_ENABLE_LOGGING))
# if MAC_OR_LINUX_
#  pragma unused(menuName)
# endif // MAC_OR_LINUX_
#endif // ! defined(OD_ENABLE_LOGGING)
	OD_LOG_OBJENTER(); //####
    OD_LOG_LL1("menuIndex = ", menuIndex); //####
    OD_LOG_S1s("menuName = ", menuName.toStdString()); //####
    ApplicationCommandManager * commandManager =
                                            &ChannelManagerWindow::getApplicationCommandManager();
    PopupMenu                   menu;
    
    menu.setLookAndFeel(&getLookAndFeel());
    switch (menuIndex)
    {
        case 0 :
            // Main
            setUpMainMenu(menu);
            break;
            
        case 1 :
            // View
            setUpViewMenu(menu);
            break;

        case 2 :
            // Operation
            if (_selectedChannel)
            {
                setUpChannelMenu(menu, *_selectedChannel);
                menu.addSeparator();
            }
            else if (_selectedContainer)
            {
                setUpContainerMenu(menu, *_selectedContainer);
                menu.addSeparator();
            }
            menu.addCommandItem(commandManager,
                                ChannelManagerWindow::kCommandLaunchRegistryService);
            menu.addCommandItem(commandManager, ChannelManagerWindow::kCommandLaunchExecutables);
            break;
        
        default :
            break;
            
    }
    return menu;
} // ContentPanel::getMenuForIndex
#if (! MAC_OR_LINUX_)
# pragma warning(pop)
#endif // ! MAC_OR_LINUX_

ApplicationCommandTarget * ContentPanel::getNextCommandTarget(void)
{
    OD_LOG_OBJENTER(); //####
    ApplicationCommandTarget * nextOne = findFirstTargetParentComponent();
    
    OD_LOG_OBJEXIT_P(nextOne); //####
    return nextOne;
} // ContentPanel::getNextCommandTarget

#if (! MAC_OR_LINUX_)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif // ! MAC_OR_LINUX_
void ContentPanel::menuItemSelected(int menuItemID,
                                    int topLevelMenuIndex)
{
#if (! defined(OD_ENABLE_LOGGING))
# if MAC_OR_LINUX_
#  pragma unused(topLevelMenuIndex)
# endif // MAC_OR_LINUX_
#endif // ! defined(OD_ENABLE_LOGGING)
	OD_LOG_OBJENTER(); //####
    OD_LOG_LL2("menuItemID = ", menuItemID, "topLevelMenuIndex = ", topLevelMenuIndex); //####
    bool isChannel = false;
    
    if (_selectedChannel)
    {
        isChannel = _selectedChannel->isChannel();
    }
    switch (menuItemID)
    {
            // Container menu items
        case kPopupDisplayChangeServiceMetrics :
            _selectedContainer->setMetricsState(! _selectedContainer->getMetricsState());
            break;
            
        case kPopupDisplayEntityInfo :
            _selectedContainer->displayInformation(false);
            break;
            
        case kPopupDetailedDisplayEntityInfo :
            _selectedContainer->displayInformation(true);
            break;
            
        case kPopupDisplayServiceMetrics :
            _selectedContainer->displayMetrics();
            break;
            
        case kPopupHideEntity :
            _selectedContainer->hide();
            break;
            
        case kPopupStopService :
            _selectedContainer->stopTheService();
            break;
           
            // Channel menu items
        case kPopupAddScrollingMonitor :
            break;
            
        case kPopupAddSimpleMonitor :
            break;
            
        case kPopupDetailedDisplayPortInfo :
            _selectedChannel->displayInformation(isChannel, true);
            break;
            
        case kPopupDisplayChannelMetrics :
            _selectedChannel->displayChannelMetrics();
            break;
            
        case kPopupDisplayPortInfo :
            _selectedChannel->displayInformation(isChannel, false);
            break;
            
        default :
            break;
            
    }
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::menuItemSelected
#if (! MAC_OR_LINUX_)
# pragma warning(pop)
#endif // ! MAC_OR_LINUX_

void ContentPanel::paint(Graphics & gg)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("gg = ", &gg); //####
    ChannelContainer * ofInterest;
    
    if (_whiteBackground)
    {
        gg.setFillType(_invertBackground ? kFirstBackgroundColour : kSecondBackgroundColour);
    }
    else
    {
        // Set up a gradient background, using a radial gradient from the centre to the furthest
        // edge.
        int   hh = getHeight();
        int   ww = getWidth();
        float halfH = static_cast<float>(hh / 2.0);
        float halfW = static_cast<float>(ww / 2.0);

        if (_invertBackground)
        {
            ColourGradient theGradient2(kFirstBackgroundColour, halfW, halfH,
                                        kSecondBackgroundColour,
                                        static_cast<float>((hh > ww) ? 0 : ww),
                                        static_cast<float>((hh > ww) ? hh : 0), true);
            FillType       theBackgroundFill2(theGradient2);
            
            gg.setFillType(theBackgroundFill2);
        }
        else
        {
            ColourGradient theGradient1(kSecondBackgroundColour, halfW, halfH,
                                        kFirstBackgroundColour,
                                        static_cast<float>((hh > ww) ? 0 : ww),
                                        static_cast<float>((hh > ww) ? hh : 0), true);
            FillType       theBackgroundFill1(theGradient1);
            
            gg.setFillType(theBackgroundFill1);
        }
    }
    gg.fillAll();
    _channelClicked = _containerClicked = false;
    OD_LOG_B2("_channelClicked <- ", _channelClicked, "_containerClicked <- ", //####
              _containerClicked); //####
    if (_selectedChannel)
    {
        ofInterest = _selectedChannel->getParent();
    }
    else
    {
        ofInterest = _selectedContainer;
    }
    for (size_t ii = 0, mm = _entitiesPanel->getNumberOfEntities(); mm > ii; ++ii)
    {
        ChannelContainer * aContainer = _entitiesPanel->getEntity(ii);
        
        if (ofInterest == aContainer)
        {
            float                  viewOffsetX = static_cast<float>(getViewPositionX());
            float                  viewOffsetY = static_cast<float>(getViewPositionY());
            juce::Rectangle<float> selectionRectangle;
            const float            dashes[] = { 5, 5 };
            const int              numDashes = (sizeof(dashes) / sizeof(*dashes));
            
            if (_selectedChannel)
            {
                selectionRectangle =
                            _selectedChannel->getBounds().translated(ofInterest->getX(),
                                                                      ofInterest->getY()).toFloat();
            }
            else
            {
                selectionRectangle = _selectedContainer->getBounds().toFloat();
            }
            // Correct for any active scrolling:
            selectionRectangle.translate(- viewOffsetX, - viewOffsetY);
            selectionRectangle.expand(kSelectionOffset, kSelectionOffset);
            Point<float> topLeft(selectionRectangle.getTopLeft());
            Point<float> topRight(selectionRectangle.getTopRight());
            Point<float> bottomLeft(selectionRectangle.getBottomLeft());
            Point<float> bottomRight(selectionRectangle.getBottomRight());
            Line<float>  line1(topLeft, topRight);
            Line<float>  line2(topRight, bottomRight);
            Line<float>  line3(bottomRight, bottomLeft);
            Line<float>  line4(bottomLeft, topLeft);
            
            if (_whiteBackground)
            {
                gg.setColour(_invertBackground ? kFirstSelectionColour : kSecondSelectionColour);
            }
            else
            {
                gg.setColour(kFirstSelectionColour);
            }
            if (! _selectedChannel)
            {
                gg.drawDashedLine(line1, dashes, numDashes, kSelectionThickness);
            }
            gg.drawDashedLine(line2, dashes, numDashes, kSelectionThickness);
            if (! _selectedChannel)
            {
                gg.drawDashedLine(line3, dashes, numDashes, kSelectionThickness);
            }
            gg.drawDashedLine(line4, dashes, numDashes, kSelectionThickness);
        }
    }
    ScannerThread * scanner = _containingWindow->getScannerThread();
    
    if (scanner)
    {
        // Check if there is some 'fresh' data.
        bool scanDataReady = scanner->checkAndClearIfScanIsComplete();
        
        if (scanDataReady)
        {
            OD_LOG("(scanDataReady)"); //####
            // At this point the background scanning thread is, basically, idle, and we can use its
            // data.
            if (_skipNextScan)
            {
                _skipNextScan = false;
                scanner->doScanSoon();
            }
            else
            {
                updatePanels(*scanner);
                setEntityPositions();
            }
            // Indicate that the scan data has been processed.
            scanner->scanCanProceed();
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::paint

bool ContentPanel::perform(const InvocationInfo & info)
{
    OD_LOG_OBJENTER(); //####
    bool                        wasProcessed = false;
    ChannelManagerApplication * ourApp = ChannelManagerApplication::getApp();
    
    switch (info.commandID)
    {
        case ChannelManagerWindow::kCommandDoRepaint :
            if (ourApp)
            {
                ourApp->doCleanupSoon();
            }
            requestWindowRepaint();
            wasProcessed = true;
            break;
            
        case ChannelManagerWindow::kCommandInvertBackground :
            flipBackground();
            requestWindowRepaint();
            wasProcessed = true;
            break;
            
        case ChannelManagerWindow::kCommandWhiteBackground :
            changeBackgroundColour();
            requestWindowRepaint();
            wasProcessed = true;
            break;
            
        case ChannelManagerWindow::kCommandClearSelection :
            setChannelOfInterest(nullptr);
            setContainerOfInterest(nullptr);
            wasProcessed = true;
            break;
            
        case ChannelManagerWindow::kCommandUnhideEntities :
            _entitiesPanel->unhideEntities();
            requestWindowRepaint();
            wasProcessed = true;
            break;

        case ChannelManagerWindow::kCommandLaunchRegistryService :
            if (ourApp)
            {
                ourApp->doLaunchRegistry();
            }
            wasProcessed = true;
            break;
            
        case ChannelManagerWindow::kCommandLaunchExecutables :
            if (ourApp)
            {
                ourApp->doLaunchOtherApplication();
            }
            wasProcessed = true;
            break;
            
        default :
            break;
            
    }
    OD_LOG_OBJEXIT_B(wasProcessed); //####
    return wasProcessed;
} // ContentPanel::perform

void ContentPanel::recallEntityPositions(void)
{
    OD_LOG_OBJENTER(); //####
    String filePath = getPathToSettingsFile();
    File   settingsFile(filePath);
    
    if (settingsFile.existsAsFile())
    {
        OD_LOG("(settingsFile.existsAsFile())"); //####
        StringArray stuffFromFile;
        
        settingsFile.readLines(stuffFromFile);
        for (int ii = 0, maxs = stuffFromFile.size(); maxs > ii; ++ii)
        {
            String aLine = stuffFromFile[ii];
            
            if (0 < aLine.length())
            {
                StringArray asPieces;
                
                asPieces.addTokens(aLine, "\t", "");
                if (3 == asPieces.size())
                {
                    String tag = asPieces[0];
                    String xPosString = asPieces[1];
                    String yPosString = asPieces[2];
                    
                    _rememberedPositions[tag.toStdString()] = Position(xPosString.getFloatValue(),
                                                                       yPosString.getFloatValue());
                }
            }
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::recallEntityPositions

void ContentPanel::rememberPositionOfEntity(ChannelContainer * anEntity)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("anEntity = ", anEntity); //####
    YarpString entityName(anEntity->getName().toStdString());

    _rememberedPositions[entityName] = anEntity->getPositionInPanel();
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::rememberPositionOfEntity

void ContentPanel::requestWindowRepaint(void)
{
    OD_LOG_OBJENTER(); //####
    _containingWindow->repaint();
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::requestWindowRepaint

void ContentPanel::resized(void)
{
    OD_LOG_OBJENTER(); //####
    juce::Rectangle<int> area(getLocalBounds());
    int                  offset = LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight();
    
    _menuBar->setBounds(area.removeFromTop(offset));
    _entitiesPanel->setBounds(area);
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::resized

void ContentPanel::saveEntityPositions(void)
{
    OD_LOG_OBJENTER(); //####
    String filePath = getPathToSettingsFile();
    File   settingsFile(filePath);
    
    if (settingsFile.create().wasOk())
    {
        OD_LOG("(settingsFile.create().wasOk())"); //####
        // Make sure that the file is empty before adding lines to it!
        settingsFile.replaceWithText("");
        for (PositionMap::const_iterator walker(_rememberedPositions.begin());
             _rememberedPositions.end() != walker; ++walker)
        {
            std::stringstream  buff;
            YarpString tag = walker->first;
            Position   where = walker->second;

            buff << "\t" << where.x << "\t" << where.y << std::endl;
            settingsFile.appendText(tag.c_str());
            settingsFile.appendText(buff.str());
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::saveEntityPositions

void ContentPanel::setChannelOfInterest(ChannelEntry * aChannel)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("aChannel = ", aChannel); //####
    _channelClicked = (nullptr != aChannel);
    _containerClicked = false;
    OD_LOG_B2("_channelClicked <- ", _channelClicked, "_containerClicked <- ", //####
              _containerClicked); //####
    _selectedChannel = aChannel;
    _selectedContainer = nullptr;
    OD_LOG_P2("_selectedChannel <- ", _selectedChannel, "_selectedContainer <- ", //####
              _selectedContainer); //####
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::setChannelOfInterest

void ContentPanel::setContainerOfInterest(ChannelContainer * aContainer)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("aContainer = ", aContainer); //####
    if (! _channelClicked)
    {
        _containerClicked = (nullptr != aContainer);
        OD_LOG_B1("_containerClicked <- ", _containerClicked); //####
        _selectedChannel = nullptr;
        _selectedContainer = aContainer;
        OD_LOG_P2("_selectedChannel <- ", _selectedChannel, "_selectedContainer <- ", //####
                  _selectedContainer); //####
    }
    requestWindowRepaint();
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::setContainerOfInterest

void ContentPanel::setEntityPositions(void)
{
    OD_LOG_OBJENTER(); //####
    float         offsetX = static_cast<float>(getX());
	float         offsetY = static_cast<float>(getY());
	float         maxX = static_cast<float>(getWidth());
	float         maxY = static_cast<float>(getHeight());
    Random        randomizer(Time::currentTimeMillis());
#if defined(USE_OGDF_POSITIONING)
    ogdf::Graph * gg;
#endif // defined(USE_OGDF_POSITIONING)
    
#if defined(USE_OGDF_POSITIONING)
# if defined(USE_OGDF_FOR_FIRST_POSITIONING_ONLY)
    if (_initialPositioningDone)
    {
        gg = nullptr;
    }
    else
    {
        _initialPositioningDone = true;
        gg = new ogdf::Graph;
    }
# else // ! defined(USE_OGDF_FOR_FIRST_POSITIONING_ONLY)
    gg = new ogdf::Graph;
# endif // ! defined(USE_OGDF_FOR_FIRST_POSITIONING_ONLY)
    if (gg)
    {
        ScopedPointer<ogdf::GraphAttributes> ga(new ogdf::GraphAttributes(*gg));
        
        if (ga)
        {
            bool       positionsNeedUpdate = false;
            ogdf::node phantomNode = gg->newNode();
            
            ga->directed(true);
            // If nodes are not connected, OGDF will pile them all at the origin; by adding a
            // 'phantom' node that is connected to every other node, we force OGDF to spread the
            // nodes out.
            ga->width(phantomNode) = 1;
            ga->height(phantomNode) = 1;
            ga->x(phantomNode) = offsetX + (randomizer.nextFloat() * maxX);
            ga->y(phantomNode) = offsetY + (randomizer.nextFloat() * maxY);
            _entitiesPanel->clearNodeValues();
            for (size_t ii = 0, mm = _entitiesPanel->getNumberOfEntities(); mm > ii; ++ii)
            {
                ChannelContainer * aContainer = _entitiesPanel->getEntity(ii);
                
                if (aContainer)
                {
                    float                  newX;
                    float                  newY;
                    juce::Rectangle<float> entityShape(aContainer->getLocalBounds().toFloat());
                    ogdf::node             aNode = gg->newNode();
                    float                  hh = entityShape.getHeight();
                    float                  ww = entityShape.getWidth();
                    
                    ga->width(aNode) = ww;
                    ga->height(aNode) = hh;
                    aContainer->setNode(aNode);
                    if (aContainer->isNew() || aContainer->wasHidden())
                    {
                        OD_LOG("(aContainer->isNew() || aContainer->wasHidden())"); //####
                        // Check if the position was already known
                        YarpString                  entityName(aContainer->getName().toStdString());
                        PositionMap::const_iterator match(_rememberedPositions.find(entityName));
                        
                        if (_rememberedPositions.end() == match)
                        {
                            newX = offsetX + (randomizer.nextFloat() * (maxX - ww));
                            newY = offsetY + (randomizer.nextFloat() * (maxY - hh));
                            positionsNeedUpdate = true;
                        }
                        else
                        {
                            newX = match->second.x;
                            newY = match->second.y;
                        }
                        aContainer->setTopLeftPosition(static_cast<int>(newX),
                                                       static_cast<int>(newY));
                    }
                    else
                    {
                        newX = entityShape.getX();
                        newY = entityShape.getY();
                    }
                    ga->x(aNode) = newX;
                    ga->y(aNode) = newY;
                }
            }
            if (positionsNeedUpdate)
            {
                OD_LOG("(positionsNeedUpdate)"); //####
                // Set up the edges (connections)
                for (size_t ii = 0, mm = _entitiesPanel->getNumberOfEntities(); mm > ii; ++ii)
                {
                    ChannelContainer * aContainer = _entitiesPanel->getEntity(ii);
                    
                    if (aContainer)
                    {
                        ogdf::node thisNode = aContainer->getNode();
                        
                        if (thisNode)
                        {
                            bool wasConnected = false;
                            
                            // Add edges between entities that are connected via their entries
                            for (int jj = 0, nn = aContainer->getNumPorts(); nn > jj; ++jj)
                            {
                                ChannelEntry * aChannel = aContainer->getPort(jj);
                                
                                if (aChannel)
                                {
                                    const ChannelConnections & outputs =
                                                                aChannel->getOutputConnections();
                                    
                                    for (size_t kk = 0, ll = outputs.size(); ll > kk; ++kk)
                                    {
                                        ChannelEntry * otherChannel = outputs[kk]._otherChannel;
                                        
                                        if (otherChannel)
                                        {
                                            ChannelContainer * otherEntity =
                                            otherChannel->getParent();
                                            
                                            if (otherEntity)
                                            {
                                                ogdf::node otherNode = otherEntity->getNode();
                                                
                                                if (otherNode && (thisNode != otherNode))
                                                {
                                                    /*ogdf::edge ee =*/ gg->newEdge(thisNode,
                                                                                    otherNode);
                                                    
                                                    wasConnected = true;
                                                }
                                            }
                                        }
                                    }
                                    const ChannelConnections & inputs =
                                                                    aChannel->getInputConnections();
                                    
                                    if (0 < inputs.size())
                                    {
                                        wasConnected = true;
                                    }
                                }
                            }
                            if (! wasConnected)
                            {
                                /*ogdf::edge phantomNodeToThis =*/ gg->newEdge(phantomNode,
                                                                               thisNode);
                                
                            }
                        }
                    }
                }
                // Apply an energy-based layout
                ScopedPointer<ogdf::FMMMLayout> fmmm(new ogdf::FMMMLayout);
                
                if (fmmm)
                {
                    fmmm->useHighLevelOptions(true);
                    fmmm->newInitialPlacement(false); //true);
                    fmmm->qualityVersusSpeed(ogdf::FMMMLayout::qvsGorgeousAndEfficient);
                    fmmm->allowedPositions(ogdf::FMMMLayout::apAll);
                    fmmm->initialPlacementMult(ogdf::FMMMLayout::ipmAdvanced);
                    fmmm->initialPlacementForces(ogdf::FMMMLayout::ipfKeepPositions);
                    fmmm->repForcesStrength(2.0);
                    fmmm->call(*ga);
                    for (size_t ii = 0, mm = _entitiesPanel->getNumberOfEntities(); mm > ii; ++ii)
                    {
                        ChannelContainer * aContainer = _entitiesPanel->getEntity(ii);
                        
                        if (aContainer && (aContainer->isNew() || aContainer->wasHidden()))
                        {
                            ogdf::node aNode = aContainer->getNode();
                            
                            if (aNode)
                            {
                                // Check if the position was already known
                                YarpString entityName(aContainer->getName().toStdString());
                                
                                if (_rememberedPositions.end() ==
                                                            _rememberedPositions.find(entityName))
                                {
                                    aContainer->setTopLeftPosition(static_cast<int>(ga->x(aNode)),
                                                                   static_cast<int>(ga->y(aNode)));
                                }
                            }
                        }
                    }
                }
            }
        }
        delete gg;
    }
    else
    {
        for (size_t ii = 0, mm = _entitiesPanel->getNumberOfEntities(); mm > ii; ++ii)
        {
            ChannelContainer * aContainer = _entitiesPanel->getEntity(ii);
            
            if (aContainer && (aContainer->isNew() || aContainer->wasHidden()))
            {
                OD_LOG("(aContainer && (aContainer->isNew() || aContainer->wasHidden()))"); //####
                float                       newX;
                float                       newY;
                juce::Rectangle<float>      entityShape(aContainer->getLocalBounds().toFloat());
                float                       hh = entityShape.getHeight();
                float                       ww = entityShape.getWidth();
                YarpString                  entityName(aContainer->getName().toStdString());
                PositionMap::const_iterator match(_rememberedPositions.find(entityName));

                if (_rememberedPositions.end() == match)
                {
                    newX = offsetX + (randomizer.nextFloat() * (maxX - ww));
                    newY = offsetY + (randomizer.nextFloat() * (maxY - hh));
                }
                else
                {
                    newX = match->second.x;
                    newY = match->second.y;
                }
                aContainer->setTopLeftPosition(static_cast<int>(newX), static_cast<int>(newY));
                if (! aContainer->isVisible())
                {
                    aContainer->setVisible(true);
                    aContainer->clearHidden();
                }
            }
        }
    }
#else // ! defined(USE_OGDF_POSITIONING)
    for (size_t ii = 0, mm = _entitiesPanel->getNumberOfEntities(); mm > ii; ++ii)
    {
        ChannelContainer * aContainer = _entitiesPanel->getEntity(ii);
        
        if (aContainer && (aContainer->isNew() || aContainer->wasHidden()))
        {
            OD_LOG("(aContainer && (aContainer->isNew() || aContainer->wasHidden()))"); //####
            float                       newX;
            float                       newY;
            juce::Rectangle<float>      entityShape(aContainer->getLocalBounds().toFloat());
            float                       hh = entityShape.getHeight();
            float                       ww = entityShape.getWidth();
            YarpString                  entityName(aContainer->getName().toStdString());
            PositionMap::const_iterator match(_rememberedPositions.find(entityName));
            
            if (_rememberedPositions.end() == match)
            {
                newX = offsetX + (randomizer.nextFloat() * (maxX - ww));
                newY = offsetY + (randomizer.nextFloat() * (maxY - hh));
            }
            else
            {
                newX = match->second.x;
                newY = match->second.y;
            }
            aContainer->setTopLeftPosition(static_cast<int>(newX), static_cast<int>(newY));
            if (! aContainer->isVisible())
            {
                aContainer->setVisible(true);
                aContainer->clearHidden();
            }
        }
    }
#endif // ! defined(USE_OGDF_POSITIONING)
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::setEntityPositions

void ContentPanel::setUpChannelMenu(PopupMenu &    aMenu,
                                    ChannelEntry & aChannel)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P2("aMenu = ", &aMenu, "aChannel = ", &aChannel); //####
    bool               isChannel = aChannel.isChannel();
    bool               showMetrics = false;
    ChannelContainer * theParent = aChannel.getParent();
    
    if (isChannel)
    {
        showMetrics = theParent->getMetricsState();
    }
    aMenu.addItem(kPopupDisplayPortInfo, isChannel ? "Display channel information" :
                  "Display port information");
    aMenu.addItem(kPopupDetailedDisplayPortInfo, isChannel ?
                  "Display detailed channel information" :
                  "Display detailed port information");
    if (isChannel)
    {
        aMenu.addSeparator();
        aMenu.addItem(kPopupDisplayChannelMetrics, "Display channel metrics",
                      showMetrics);
    }
    if ((kPortDirectionInput != aChannel.getDirection()) &&
        (kPortUsageClient != aChannel.getUsage()))
    {
        aMenu.addSeparator();
        aMenu.addItem(kPopupAddSimpleMonitor, "Enable activity indicator", false);
        aMenu.addItem(kPopupAddScrollingMonitor, "Add scrolling monitor", false);
    }
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::setUpChannelMenu

void ContentPanel::setUpContainerMenu(PopupMenu &        aMenu,
                                      ChannelContainer & aContainer)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P2("aMenu = ", &aMenu, "aContainer = ", &aContainer); //####
    bool   serviceLike;
    String kindOfContainer;
    
    switch (aContainer.getKind())
    {
        case kContainerKindAdapter :
            kindOfContainer = "adapter";
            serviceLike = true;
            break;
            
        case kContainerKindService :
            kindOfContainer = "service";
            serviceLike = true;
            break;
            
        default :
            kindOfContainer = "entity";
            serviceLike = false;
            break;
    }
    aMenu.addItem(kPopupDisplayEntityInfo, String("Display ") + kindOfContainer + " information");
    aMenu.addItem(kPopupDetailedDisplayEntityInfo, String("Display detailed ") + kindOfContainer +
                  " information");
    if (serviceLike)
    {
        bool metricsEnabled = aContainer.getMetricsState();
        
        aMenu.addSeparator();
        aMenu.addItem(kPopupDisplayChangeServiceMetrics,
                      String(metricsEnabled ? "Disable " : "Enable ") + kindOfContainer +
                      " metrics collection");
        aMenu.addItem(kPopupDisplayServiceMetrics, String("Display ") + kindOfContainer +
                      " metrics", metricsEnabled);
    }
    aMenu.addItem(kPopupHideEntity, String("Hide the ") + kindOfContainer);
    if (serviceLike)
    {
        aMenu.addSeparator();
        aMenu.addItem(kPopupStopService, String("Stop the ") + kindOfContainer);
    }
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::setUpContainerMenu

void ContentPanel::setUpMainMenu(PopupMenu & aMenu)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("aMenu = ", &aMenu); //####
    ApplicationCommandManager * commandManager =
                                            &ChannelManagerWindow::getApplicationCommandManager();
    
    aMenu.addCommandItem(commandManager, StandardApplicationCommandIDs::quit);
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::setUpMainMenu

void ContentPanel::setUpViewMenu(PopupMenu & aMenu)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("aMenu = ", &aMenu); //####
    ApplicationCommandManager * commandManager =
                                            &ChannelManagerWindow::getApplicationCommandManager();
    
    aMenu.addCommandItem(commandManager, ChannelManagerWindow::kCommandDoRepaint);
    aMenu.addCommandItem(commandManager, ChannelManagerWindow::kCommandInvertBackground);
    aMenu.addCommandItem(commandManager, ChannelManagerWindow::kCommandWhiteBackground);
    aMenu.addSeparator();
    aMenu.addCommandItem(commandManager, ChannelManagerWindow::kCommandClearSelection);
    aMenu.addCommandItem(commandManager, ChannelManagerWindow::kCommandUnhideEntities);
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::setUpViewMenu

void ContentPanel::skipScan(void)
{
    OD_LOG_OBJENTER(); //####
    _skipNextScan = true;
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::skipScan

void ContentPanel::updatePanels(ScannerThread & scanner)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("scanner = ", &scanner); //####
    bool                 changeSeen = false;
    const EntitiesData & workingData(scanner.getEntitiesData());
    
    // Retrieve each entity from our new list; if it is known already, ignore it but mark the
    // old entity as known.
    _entitiesPanel->clearAllVisitedFlags();
    _entitiesPanel->clearAllNewlyCreatedFlags();
    _entitiesPanel->invalidateAllConnections();
    for (size_t ii = 0, mm = workingData.getNumberOfEntities(); mm > ii; ++ii)
    {
        EntityData * anEntity = workingData.getEntity(ii);
        
        OD_LOG_P1("anEntity <- ", anEntity); //####
        if (anEntity)
        {
            OD_LOG_S1s("anEntity->getName() = ", anEntity->getName()); //####
            ChannelContainer * oldEntity = _entitiesPanel->findKnownEntity(anEntity->getName());
            
            if (oldEntity)
            {
                OD_LOG("(oldEntity)"); //####
                oldEntity->setVisited();
            }
            else
            {
                // Make a copy of the newly discovered entity, and add it to the active panel.
                ChannelContainer * newEntity = new ChannelContainer(anEntity->getKind(),
                                                                    anEntity->getName(),
                                                                    anEntity->getIPAddress(),
                                                                    anEntity->getBehaviour(),
                                                                    anEntity->getDescription(),
                                                                    anEntity->getRequests(),
                                                                    *_entitiesPanel);
                
                newEntity->setVisited();
                // Make copies of the ports of the entity, and add them to the new entity.
                for (int jj = 0, nn = anEntity->getNumPorts(); nn > jj; ++jj)
                {
                    PortData * aPort = anEntity->getPort(jj);
                    
                    if (aPort)
                    {
                        ChannelEntry * newPort = newEntity->addPort(aPort->getPortName(),
                                                                    aPort->getPortNumber(),
                                                                    aPort->getProtocol(),
                                                                    aPort->getProtocolDescription(),
                                                                    aPort->getUsage(),
                                                                    aPort->getDirection());
                        
                        _entitiesPanel->rememberPort(newPort);
                    }
                }
                _entitiesPanel->addEntity(newEntity);
                changeSeen = true;
            }
        }
    }
    // Convert the detected connections into visible connections.
    const ConnectionList & connections(workingData.getConnections());
    
    for (ConnectionList::const_iterator walker(connections.begin()); connections.end() != walker;
         ++walker)
    {
        ChannelEntry * thisPort = _entitiesPanel->findKnownPort(walker->_outPortName);
        ChannelEntry * otherPort = _entitiesPanel->findKnownPort(walker->_inPortName);
        
        OD_LOG_P2("thisPort <- ", thisPort, "otherPort <- ", otherPort); //####
        if (thisPort && otherPort)
        {
            OD_LOG_S2s("thisPort.name = ", thisPort->getPortName(), //####
                       "otherPort.name = ", otherPort->getPortName()); //####
            thisPort->addOutputConnection(otherPort, walker->_mode, false);
            otherPort->addInputConnection(thisPort, walker->_mode, false);
        }
    }
    if (_entitiesPanel->removeUnvisitedEntities())
    {
        changeSeen = true;
    }
    _entitiesPanel->removeInvalidConnections();
    OD_LOG("about to call adjustSize()"); //####
    _entitiesPanel->adjustSize(false);
    if (changeSeen)
    {
        scanner.doScanSoon();
    }
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::updatePanels

#if (! MAC_OR_LINUX_)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif // ! MAC_OR_LINUX_
void ContentPanel::visibleAreaChanged(const juce::Rectangle<int> & newVisibleArea)
{
#if (! defined(OD_ENABLE_LOGGING))
# if MAC_OR_LINUX_
#  pragma unused(newVisibleArea)
# endif // MAC_OR_LINUX_
#endif // ! defined(OD_ENABLE_LOGGING)
    OD_LOG_OBJENTER(); //####
    OD_LOG_LL4("nVA.x = ", newVisibleArea.getX(), "nVA.y = ", newVisibleArea.getY(), //####
               "nVA.w = ", newVisibleArea.getWidth(), "nVA.h = ", //####
               newVisibleArea.getHeight()); //####
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::visibleAreaChanged
#if (! MAC_OR_LINUX_)
# pragma warning(pop)
#endif // ! MAC_OR_LINUX_

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)

void ChannelManager::DisplayInformationPanel(Component *    above,
                                             const String & bodyText,
                                             const String & title)
{
    OD_LOG_ENTER(); //####
    OD_LOG_P1("above = ", above); //####
    OD_LOG_S2s("bodyText = ", bodyText.toStdString(), "title = ", title.toStdString()); //####
    DialogWindow::LaunchOptions options;
    Font                        monoFont(Font::getDefaultMonospacedFontName(), 16, Font::plain);
    Label *                     aLabel = new Label;
    Point<int>                  dimensions;
    
    aLabel->setFont(monoFont);
    aLabel->setText(bodyText, dontSendNotification);
    options.content.setOwned(aLabel);
    CalculateTextArea(dimensions, aLabel->getFont(), bodyText);
    options.content->setSize(dimensions.getX(), dimensions.getY());
    options.dialogTitle = title;
    options.escapeKeyTriggersCloseButton = true;
    options.useNativeTitleBar = false;
    options.resizable = false;
    options.dialogBackgroundColour = kDialogBackgroundColour;
    DialogWindow *  aWindow = options.launchAsync();
    BorderSize<int> bt = aWindow->getBorderThickness();
    BorderSize<int> cb = aWindow->getContentComponentBorder();
    int             tw = aLabel->getFont().getStringWidth(title);
    int             minW = jmax(tw, dimensions.getX());
    int             calcW = minW + bt.getLeftAndRight() + cb.getLeftAndRight() + kExtraDisplayWidth;
    int             calcH = dimensions.getY() + bt.getTopAndBottom() + cb.getTopAndBottom();
    
    aWindow->centreAroundComponent(above, calcW, calcH);
    OD_LOG_EXIT(); //####
} // DisplayInformationPanel
