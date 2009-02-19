/**
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.apache.activemq.openwire.tool;

import java.util.Iterator;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collections;

import org.apache.tools.ant.Project;
import org.apache.tools.ant.taskdefs.FixCRLF;

/**
*
* @version $Revision: 379734 $
*/
public class AmqCppMakefileGenerator {

    protected String targetDir="./src/main";
    protected String targetPath;

    private ArrayList<String> sources = new ArrayList<String>();
    private ArrayList<String> headers = new ArrayList<String>();

    private File destDir;
    private File destFile;
    private StringBuffer buffer;

    public AmqCppMakefileGenerator( String targetPath ) {
        this.targetPath = targetPath;
    }

    protected String getFilePostFix() {
        return ".mk";
    }

    protected String getFilePrefix() {
        return "srcmakefile";
    }

    public String getTargetDir() {
        return targetDir;
    }

    public void setTargetDir( String targetDir ) {
        this.targetDir = targetDir;
    }

    /**
     * Derived classes provide the location of the Makefile.
     */
    protected File getDestDir() {
        return new File( targetDir + "/" + targetPath );
    }


    private void processDirectory( File destDir ) {
        if( destDir == null ) {
            throw new IllegalArgumentException("No destDir defined!");
        }

        File[] files = destDir.listFiles();

        for( File file : files ) {

            if( file.getName().endsWith("cpp") ) {
                sources.add( file.getName() );
            }

            if( file.getName().endsWith("h") ) {
                headers.add( file.getName() );
            }
        }
    }

    public void run() {

        destDir = getDestDir();

        if( destDir == null ) {
            throw new IllegalArgumentException("No destDir defined!");
        }

        // Get all the sources from the dir.
        processDirectory( destDir );

        System.out.println(getClass().getName() + " generating files in: " + destDir);
        destDir.mkdirs();
        buffer = new StringBuffer();

        destFile = new File( destDir, getFilePrefix() + getFilePostFix() );

        PrintWriter out = null;
        try {
            out = new PrintWriter(new FileWriter(destFile));
            generateFile(out);
        } catch (Exception e) {
            throw new RuntimeException(e);
        } finally {
            if (out != null) {
                out.close();
            }
        }

        // Use the FixCRLF Ant Task to make sure the file has consistent
        // newlines so that SVN does not complain on checkin.
        Project project = new Project();
        project.init();
        FixCRLF fixCRLF = new FixCRLF();
        fixCRLF.setProject(project);
        fixCRLF.setSrcdir(destFile.getParentFile());
        fixCRLF.setIncludes(destFile.getName());
        fixCRLF.execute();
    }

    protected void generateLicence(PrintWriter out) {
        out.println("# ---------------------------------------------------------------------------");
        out.println("# Licensed to the Apache Software Foundation (ASF) under one or more");
        out.println("# contributor license agreements.  See the NOTICE file distributed with");
        out.println("# this work for additional information regarding copyright ownership.");
        out.println("# The ASF licenses this file to You under the Apache License, Version 2.0");
        out.println("# (the \"License\"); you may not use this file except in compliance with");
        out.println("# the License.  You may obtain a copy of the License at");
        out.println("# ");
        out.println("# http://www.apache.org/licenses/LICENSE-2.0");
        out.println("# ");
        out.println("# Unless required by applicable law or agreed to in writing, software");
        out.println("# distributed under the License is distributed on an \"AS IS\" BASIS,");
        out.println("# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.");
        out.println("# See the License for the specific language governing permissions and");
        out.println("# limitations under the License.");
        out.println("# ---------------------------------------------------------------------------");
    }

    protected void generateFile(PrintWriter out) {
        generateLicence(out);

        // Put the file lists in alphabetical order.
        Collections.sort( sources );
        Collections.sort( headers );

        out.println("");
        out.print("cc_sources +=");

        for( String source : sources ) {
            out.println(" \\");
            out.print("    " + targetPath + "/" + source);
        }

        out.println("");
        out.println("");
        out.print("h_sources +=");

        for( String header : headers ) {
            out.println(" \\");
            out.print("    " + targetPath + "/" + header);
        }

        out.println("");
    }
}
