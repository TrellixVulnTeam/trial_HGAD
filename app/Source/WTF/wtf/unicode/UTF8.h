/*
 * Copyright (C) 2007 Apple Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef WTF_UTF8_h
#define WTF_UTF8_h

//#include <unicode/utypes.h>

#include "/home/c/Downloads/icu/source/common/unicode/utypes.h"
#include <wtf/text/LChar.h>

namespace WTF {
namespace Unicode {

    // Given a first byte, gives the length of the UTF-8 sequence it begins.
    // Returns 0 for bytes that are not legal starts of UTF-8 sequences.
    // Only allows sequences of up to 4 bytes, since that works for all Unicode characters (U-00000000 to U-0010FFFF).
    WTF_EXPORT_PRIVATE int UTF8SequenceLength(char);

    // Takes a null-terminated C-style string with a UTF-8 sequence in it and converts it to a character.
    // Only allows Unicode characters (U-00000000 to U-0010FFFF).
    // Returns -1 if the sequence is not valid (including presence of extra bytes).
    WTF_EXPORT_PRIVATE int decodeUTF8Sequence(const char*);

    typedef enum {
            conversionOK,       // conversion successful
            sourceExhausted,    // partial character in source, but hit end
            targetExhausted,    // insuff. room in target for conversion
            sourceIllegal       // source sequence is illegal/malformed
    } ConversionResult;

    // These conversion functions take a "strict" argument. When this
    // flag is set to strict, both irregular sequences and isolated surrogates
    // will cause an error.  When the flag is set to lenient, both irregular
    // sequences and isolated surrogates are converted.
    // 
    // Whether the flag is strict or lenient, all illegal sequences will cause
    // an error return. This includes sequences such as: <F4 90 80 80>, <C0 80>,
    // or <A0> in UTF-8, and values above 0x10FFFF in UTF-32. Conformant code
    // must check for illegal sequences.
    // 
    // When the flag is set to lenient, characters over 0x10FFFF are converted
    // to the replacement character; otherwise (when the flag is set to strict)
    // they constitute an error.

    WTF_EXPORT_PRIVATE ConversionResult convertUTF8ToUTF16(
                    const char** sourceStart, const char* sourceEnd, 
                    UChar** targetStart, UChar* targetEnd, bool* isSourceAllASCII = 0, bool strict = true);

    WTF_EXPORT_PRIVATE ConversionResult convertLatin1ToUTF8(
                    const LChar** sourceStart, const LChar* sourceEnd, 
                    char** targetStart, char* targetEnd);

    WTF_EXPORT_PRIVATE ConversionResult convertUTF16ToUTF8(
                    const UChar** sourceStart, const UChar* sourceEnd, 
                    char** targetStart, char* targetEnd, bool strict = true);

    WTF_EXPORT_PRIVATE unsigned calculateStringHashAndLengthFromUTF8MaskingTop8Bits(const char* data, const char* dataEnd, unsigned& dataLength, unsigned& utf16Length);

    // The caller of these functions already knows that the lengths are the same, so we omit an end argument for UTF-16 and Latin-1.
    bool equalUTF16WithUTF8(const UChar* stringInUTF16, const char* stringInUTF8, const char* stringInUTF8End);
    bool equalLatin1WithUTF8(const LChar* stringInLatin1, const char* stringInUTF8, const char* stringInUTF8End);

} // namespace Unicode
} // namespace WTF

#endif // WTF_UTF8_h
