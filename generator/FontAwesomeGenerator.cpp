/*
 FontAwesomeGenerator
 
 Copyright (c) 2015, Simon Geilfus - All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and
 the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
 the following disclaimer in the documentation and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "FontAwesomeGenerator.h"

#include <fstream>

using namespace std;

// MARK: License
const std::string license = R"(/*
 FontAwesomeCpp
 
 Copyright (c) 2015, Simon Geilfus - All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and
 the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
 the following disclaimer in the documentation and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/)";

// MARK: Header
const std::string header = R"(#pragma once

#include <string>

#ifndef FONTAWESOME_NO_TYPEDEF
    typedef class FontAwesome fa;
#endif

class FontAwesome {
public:
)";
    
// MARK: Footer
const std::string footer = "};";

void FontAwesomeGenerator::write( const char *cssPath, const char *outputFilePath )
{
    ofstream outputFile( outputFilePath );
    outputFile << license << endl << header << read( cssPath ).str() << endl << footer;
    outputFile.close();
}

// replace all substring in a string (in-place)
void replaceAll( string& str, const string& from, const string& to ) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}
std::stringstream FontAwesomeGenerator::read( const char *cssPath )
{
    string allChars;
    string line;
    stringstream streamOut;
    
    std::pair<int,int> range = { std::numeric_limits<int>::max(), std::numeric_limits<int>::min() };
    vector<string> names;
    ifstream file( cssPath );
    while( getline( file, line ) ) {
        size_t faPos        = line.find( ".fa-" );
        size_t beforePos    = line.rfind( ":before" );
        size_t contentPos   = line.find( "content: \"\\" );
        
        // check for names
        if( beforePos != string::npos ){
            string name = line.substr( faPos + 4, beforePos - 4 );
            replaceAll( name, "-", "_" );
            names.push_back( name );
        }
        // get the unicode character
        else if( contentPos != string::npos ){
            string unicode = line.substr( contentPos + 11, 4 );
            int unicodeInt = std::stoul( "0x" + unicode, nullptr, 16 );
            
            // update the character range
            if( unicodeInt < range.first ) range.first      = unicodeInt;
            if( unicodeInt > range.second ) range.second    = unicodeInt;
            
            // output it per name
            for( auto name : names ){
                // skip "try" icon
                if( name.find( "try" ) == string::npos ){
                    streamOut << "\t" << "static std::string " << name << "() { return \"\\u" << unicode << "\"; }" << endl;
                }
            }
            allChars += "\\u" + unicode;
            names.clear();
        }
    }
    
    // output the allChars method
    streamOut << "\t" << "static std::string allChars() { return \"" + allChars + "\"; }" << endl;
    // output the charsRange method
    std::stringstream rangeStream;
    rangeStream << std::hex << "0x" << range.first << ", " << "0x" << range.second;
    streamOut << "\t" << "static std::pair<uint16_t,uint16_t> charsRange() { return { " + rangeStream.str() + " }; }" << endl;
    
    return streamOut;
}