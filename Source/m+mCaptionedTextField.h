//--------------------------------------------------------------------------------------------------
//
//  File:       m+mCaptionedTextField.h
//
//  Project:    m+m
//
//  Contains:   The class declaration for a text editor paired with a caption.
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
//  Created:    2015-08-31
//
//--------------------------------------------------------------------------------------------------

#if (! defined(mpmCaptionedTextField_H_))
# define mpmCaptionedTextField_H_ /* Header guard */

# include "m+mFormField.h"
# include "m+mManagerDataTypes.h"

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 
 @brief The class declaration for a field consisting of a text editor paired with a caption. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

/*! @brief The character to use when displaying a 'password' field. */
# define CHAR_TO_USE_FOR_PASSWORD_ 0x02022

namespace MPlusM_Manager
{
    class TextValidator;
    class ValidatingTextEditor;
    
    /*! @brief A field consisting of a text editor paired with a caption. */
    class CaptionedTextField : public FormField
    {
    public :
        
        /*! @brief The constructor.
         @param responder The entity that will report errors in this field.
         @param regularLabelFont The font to use with the label when the text editor data is valid.
         @param errorLabelFont The font to use with the label when the text editor data is invalid.
         @param index The order of the text editor.
         @param validator The function to use when checking the field on completion of text entry.
         @param componentName The name to pass to the component for it to use as its name.
         @param passwordCharacter The visual replacement to use for password fields. */
        CaptionedTextField(FormFieldErrorResponder & responder,
                           Font &                    regularLabelFont,
                           Font &                    errorLabelFont,
                           const size_t              index,
                           TextValidator *           validator = NULL,
                           const String &            componentName = String::empty,
                           juce_wchar                passwordCharacter = 0);
        
        /*! @brief The destructor. */
        virtual ~CaptionedTextField(void);
        
        /*! @brief Return the component associated with the field.
         @return The component associated with the field. */
        virtual Component * getComponent(void)
        const;

        /*! @brief Return the height of the field in pixels.
         @return The height of the field in pixels. */
        virtual int getHeight(void)
        const;
        
        /*! @brief Returns the text value associated with the field.
         @returns The text value associated with the field. */
        virtual String getText(void)
        const;
        
        /*! @brief Return the left coordinate of the field.
         @return The left coordinate of the field. */
        virtual int getX(void)
        const;
        
        /*! @brief Return the top coordinate of the field.
         @return The top coordinate of the field. */
        virtual int getY(void)
        const;

        /*! @brief Do not perform validation on next loss of focus. */
        virtual void ignoreNextFocusLoss(void);
        
        /*! @brief Perform the action triggered by the button. */
        virtual void performButtonAction(void);
        
        /*! @brief Report an error in the field. */
        void reportErrorInField(void);

        /*! @brief Set the position and size of the field.
         @param xx The leftmost coordinate of the field.
         @param yy The topmost coordinate of the field.
         @param width The width of the field.
         @param height The height of the field. */
        virtual void setBounds(const int xx,
                               const int yy,
                               const int width,
                               const int height);

        /*! @brief Set the action on receiving focus.
         @param shouldSelectAll If @c true, obtaining focus will select all the content. */
        virtual void setSelectAllWhenFocused(const bool shouldSelectAll);

        /*! @brief Change the size of the field.
         @param newWidth The new width of the field.
         @param newHeight The new height of the field. */
        virtual void setSize(const int newWidth,
                             const int newHeight);
        
        /*! @brief Set the text value associated with the field.
         @param newText The text to be used.
         @param sendTextChangeMessage @c true if a change message is sent to all listeners. */
        virtual void setText(const String & newText,
                             const bool     sendTextChangeMessage);

        /*! @brief Set the position of the field.
         @param xx The new leftmost coordinate of the field.
         @param yy The new topmost coordinate of the field. */
        virtual void setTopLeftPosition(const int xx,
                                        const int yy);

        /*! @brief Check the field for validity.
         @returns @c true if the validator accepts the field or there's no validation required or
         @c false if the validator rejects the field. */
        virtual bool validateField(void);
        
        /*! @brief Check the field for validity.
         @param argsToUse A set of valid arguments.
         @returns @c true if the validator accepts the field or there's no validation required or
         @c false if the validator rejects the field. */
        virtual bool validateField(StringArray & argsToUse);
        
    protected :
        
    private :
        
        /*! @brief Returns the name of the field.
         @returns The name of the field. */
        virtual const String & getName(void)
        const;
        
        /*! @brief Mark the text editor data as invalid. */
        void markAsInvalid(void);
        
        /*! @brief Mark the text editor data as valid. */
        void markAsValid(void);
        
    public :
    
    protected :
    
    private :

        /*! @brief The class that this class is derived from. */
        typedef FormField inherited;
        
        /*! @brief The text editor within the field. */
        ScopedPointer<ValidatingTextEditor> _textEditor;
        
        /*! @brief The validator to use with the text editor data. */
        ScopedPointer<TextValidator> _validator;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CaptionedTextField)
        
    }; // CaptionedTextField
    
} // MPlusM_Manager

#endif // ! defined(mpmCaptionedTextField_H_)
