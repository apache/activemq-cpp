/*
 * Copyright 2001-2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.apache.activemq.maven;

import org.apache.maven.plugin.MojoExecutionException;
import org.codehaus.mojo.natives.NativeSources;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.Collections;
import java.util.List;

/**
 * Goal generates a makefile using a provided template from the sources defined in a
 * given directory, or set of directories.
 *
 * @goal makefile
 *
 * @phase process-sources
 */
public class MakefileMojo extends AbstractToolingMojo {

    /**
     * The input Makefile Template which is used to generate the final Makefile.am.
     * this file should have the variables defined for the Includes and Sources section.
     * These are of the form ${headers} and ${source}.  When located in the file these
     * value are replaced with the source and header files found using the configuration
     * for this plugin.
     *
     * @parameter
     * @required
     */
    private File template;

    private final String HEADERS_MARKER = "${headers}";
    private final String SOURCES_MARKER = "${source}";

    /**
     * Execute this goal
     */
    public void execute() throws MojoExecutionException {

        if( !this.getTemplate().exists() ) {
            throw new MojoExecutionException( "Template File does not exist: " + this.getTemplate() );
        }

//        System.out.println( "Output directory = " + this.getOutputDirectory() );
//        System.out.println( "Makefile template = " + this.getTemplate() );
//        System.out.println( "" );
//        System.out.println( "Source Files Directory = " + getSources().getDirectory() );
//        System.out.println( "Source Includes = " + getSources().getIncludes()[0] );
//        System.out.println( "Sources Found = " + getSources().getFiles().size() );
//        System.out.println( "Header Files Directory = " + getHeaders().getDirectory() );
//        System.out.println( "header Includes = " + getHeaders().getIncludes()[0] );
//        System.out.println( "headers Found = " + getHeaders().getFiles().size() );
//        System.out.println( "" );

        try {

            BufferedReader reader = new BufferedReader( new FileReader( getTemplate() ) );

            File targetFile = new File( getOutputDirectory(), "Makefile.am" );
            BufferedWriter writer = new BufferedWriter( new FileWriter( targetFile ) );

            String line;

            while( ( line = reader.readLine() ) != null ) {

                line = replaceMarker( HEADERS_MARKER, line, getHeaders() );
                line = replaceMarker( SOURCES_MARKER, line, getSources() );

                // Write the line now.
                writer.write( line );
                writer.newLine();
            }

            writer.close();
            reader.close();

        } catch (Exception e) {
            throw new MojoExecutionException( e.getMessage() );
        }
    }

    public File getTemplate() {
        return template;
    }

    public void setTemplate(File template) {
        this.template = template;
    }

    @SuppressWarnings("unchecked")
    private String replaceMarker( String marker, String line, NativeSources sources ) {

        List<File> files = sources.getFiles();

        Collections.sort( files );

        int pos = 0;

        if( ( pos = line.indexOf( marker ) ) != -1 ) {

            // Infer the indent of the Files from the index where the Marker
            // tag was found, insert that amount of spacing on each line following
            // the initial insert where the marker was found.
            boolean firstTime = true;

            StringBuilder indent = new StringBuilder();
            for( int i = 0; i < pos; ++i ) {
                indent.append( ' ' );
            }

            StringBuilder filesStr = new StringBuilder();
            for( int i = 0; i < files.size(); ++i ) {

                File file = files.get( i );

                if( !firstTime ) {
                    filesStr.append( indent );
                }

                filesStr.append(
                    file.toString().replace( sources.getDirectory().toString() + "/", "" ) );

                if( (i+1) != files.size()) {
                    filesStr.append( " \\" );
                }

                filesStr.append( "\n" );

                firstTime = false;
            }

            line = line.replace( marker, filesStr );
        }

        return line;
    }
}
