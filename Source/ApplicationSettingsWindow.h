//--------------------------------------------------------------------------------------------------
//
//  File:       ApplicationSettingsWindow.h
//
//  Project:    M+M
//
//  Contains:   The class declaration for the application settings window of the channel manager
//              application.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2015 by H Plus Technologies Ltd. and Simon Fraser University.
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
//  Created:    2015-06-10
//
//--------------------------------------------------------------------------------------------------

#if (! defined(ApplicationSettingsWindow_H_))
# define ApplicationSettingsWindow_H_ /* Header guard */

# include "ChannelManagerDataTypes.h"

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 
 @brief The class declaration for the application settings window of the channel manager
 application. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace ChannelManager
{
    /*! @brief The main window of the application. */
    class ApplicationSettingsWindow : private AsyncUpdater,
                                      private ButtonListener,
                                      public DocumentWindow
    {
    public :
        
        /*! @brief The values to be returned. */
        enum
        {
            /*! @brief 'Cancel' was requested. */
            kSettingsCancel,
            
            /*! @brief 'OK' was requested. */
            kSettingsAccept,
            
            /*! @brief '+ argument' was requested. */
            kSettingsAddField,
            
            /*! @brief '- argument' was requested. */
            kSettingsRemoveField
        };
        
        /*! @brief The constructor.
         @param title The title for the window.
         @param execType The kind of application being configured.
         @param appInfo The options for the settings.
         @param endpointToUse The resulting endpoint for the application.
         @param tagToUse The resulting tag for the application.
         @param portToUse The resulting port for the application.
         @param argsToUse The resulting arguments for the application. */
        ApplicationSettingsWindow(const String &          title,
                                  const String &          execType,
                                  const ApplicationInfo & appInfo,
                                  String &                endpointToUse,
                                  String &                tagToUse,
                                  String &                portToUse,
                                  StringArray &           argsToUse);
        
        /*! @brief The destructor. */
        virtual ~ApplicationSettingsWindow(void);
        
    protected :
        
    private :

        /*! @brief Add an extra field. */
        void addAnExtraField(void);
        
        /*! @brief Adjust the fields to their proper locations and dimensions. */
        void adjustFields(void);
        
        /*! @brief Called when a button is clicked.
         @param aButton The button that was clicked. */
        virtual void buttonClicked(Button * aButton);

        /*! @brief This method is called when the user tries to close the window. */
        virtual void closeButtonPressed(void);
        
        /*! @brief Check the values of the fields against their specifications.
         @returns @c true if all the fields are valid and @c false otherwise. */
        bool fieldsAreValid(void);
        
        /*! @brief Called back to perform operations. */
        virtual void handleAsyncUpdate(void);
        
        /*! @brief Called when a key is pressed.
         @param key The key that was pressed.
         @returns @c true if the key was consumed and @c false otherwise. */
        virtual bool keyPressed(const KeyPress & key);

        /*! @brief Recalculate the area occupied by the fields and adjust the button positions. */
        void recalculateArea(void);
        
        /*! @brief Remove the most recently added extra field. */
        void removeMostRecentlyAddedExtraField(void);
        
        /*! @brief Called when the component size has been changed. */
        virtual void resized(void);
        
        /*! @brief Set up the standard fields.
         @param widthSoFar The minimum width to show the fields.
         @param heightSoFar The minimum height to show the fields. */
        void setUpStandardFields(int & widthSoFar,
                                 int & heightSoFar);
        
    public :
    
    protected :
    
    private :
        
        /*! @brief The second class that this class is derived from. */
        typedef AsyncUpdater inherited1;
        
        /*! @brief The first class that this class is derived from. */
        typedef DocumentWindow inherited2;
        
        /*! @brief The descriptive text at the top of the window. */
        Label _topText;
        
        /*! @brief The 'Cancel' button. */
        TextButton _cancelButton;

        /*! @brief The 'OK' button. */
        TextButton _okButton;

        /*! @brief The provided argument descriptions. */
        const MplusM::Utilities::DescriptorVector & _descriptors;
        
        /*! @brief The monospaced font to use. */
        Font _monoFont;
        
        /*! @brief The kind of application being configured. */
        String _execType;
        
        /*! @brief The root name for extra arguments. */
        String _extraArgRootName;
        
        /*! @brief The set of labels for the extra fields. */
        OwnedArray<Label> _extraFieldLabels;
        
        /*! @brief The set of extra fields. */
        OwnedArray<TextEditor> _extraFieldEditors;
        
        /*! @brief The set of labels for standard fields. */
        OwnedArray<Label> _standardFieldLabels;
        
        /*! @brief The set of standard fields. */
        OwnedArray<TextEditor> _standardFieldEditors;
        
        /*! @brief The caption for the endpoint text entry field. */
        ScopedPointer<Label> _endpointCaption;
        
        /*! @brief The caption for the 'extra arguments' area of the window. */
        ScopedPointer<Label> _extraArgumentsCaption;
        
        /*! @brief The caption for the port text entry field. */
        ScopedPointer<Label> _portCaption;
        
        /*! @brief The caption for the tag text entry field. */
        ScopedPointer<Label> _tagCaption;
        
        /*! @brief The '+ arguments' button. */
        ScopedPointer<TextButton> _addArgumentsButton;
        
        /*! @brief The '- arguments' button. */
        ScopedPointer<TextButton> _removeArgumentsButton;
        
        /*! @brief The endpoint text entry field. */
        ScopedPointer<TextEditor> _endpointEditor;
        
        /*! @brief The port text entry field. */
        ScopedPointer<TextEditor> _portEditor;
        
        /*! @brief The the tag text entry field. */
        ScopedPointer<TextEditor> _tagEditor;
        
        /*! @brief The options for the application. */
        const ApplicationInfo & _appInfo;
        
        /*! @brief Set to the endpoint provided by the user. */
        String & _endpointToUse;
        
        /*! @brief Set to the port provided by the user. */
        String & _portToUse;
        
        /*! @brief Set to the tag provided by the user. */
        String & _tagToUse;

        /*! @brief Set to the arguments provided by the user. */
        StringArray & _argsToUse;
        
        /*! @brief The height to use for text editor fields. */
        float _adjustedEditorHeight;
        
        /*! @brief @c true if extra arguments are OK and @c false if they aren't used. */
        bool _canHaveExtraArguments;
        
        /*! @brief @c true if the endpoint can be set and @c false if the endpoint is fixed. */
        bool _canSetEndpoint;
        
        /*! @brief @c true if the internet port can be set and @c false if the internet port is
         fixed. */
        bool _canSetPort;
        
        /*! @brief @c true if a tag can be applied and @c false if the port names are fixed. */
        bool _canSetTag;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ApplicationSettingsWindow)
        
    }; // ApplicationSettingsWindow
    
} // ApplicationSettings

#endif // ! defined(ApplicationSettingsWindow_H_)
