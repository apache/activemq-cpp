/**
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
package org.apache.activemq.openwire.tool.commands;

import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;

import org.apache.activemq.openwire.tool.MultiSourceGenerator;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.taskdefs.FixCRLF;
import org.codehaus.jam.JClass;

/**
 *
 * @version $Revision: 409828 $
 */
public class CommandClassGenerator extends MultiSourceGenerator {

    private CommandCodeGeneratorsFactory generatorsFactory =
        new CommandCodeGeneratorsFactory();

    protected String targetDir="./src/main";

    public String getTargetDir() {
        return targetDir;
    }

    public void setTargetDir(String targetDir) {
        this.targetDir = targetDir;
    }

    protected void processClass(JClass jclass) {

        // Prepare the State variables for the current class
        simpleName = jclass.getSimpleName();
        superclass = jclass.getSuperclass();
        className = getClassName(jclass);
        baseClass = getBaseClassName(jclass);

        System.out.println(getClass().getName() + " processing class: " + simpleName);

        try {
            // Using the current JClass state in the MultiSourceGenerator we can
            // now generate the Header and Source for the CPP commands.
            generateHeaderFile();
            generateSourceFile();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public Object run() {
        filePostFix = ".cpp";
        if (destDir == null) {
            destDir = new File(
                targetDir+"/activemq/commands");
        }

        return super.run();
    }

    protected void generateHeaderFile() throws Exception {

        File headerFile = new File(destDir, className + ".h");

        CommandCodeGenerator hdrGenerator = generatorsFactory.getHeaderGenerator( className );

        hdrGenerator.setJClass( getJclass() );
        hdrGenerator.setProperties( getProperties() );
        hdrGenerator.setMarshalAware( isMarshallerAware() );
        hdrGenerator.setClassName( getClassName() );
        hdrGenerator.setBaseClassName( getBaseClass() );
        hdrGenerator.setOpenWireOpCode( getOpenWireOpCode(getJclass()) );

        PrintWriter out = null;
        try {
            out = new PrintWriter(new FileWriter(headerFile));
            hdrGenerator.generate(out);
        } catch( Exception e ) {
            throw new RuntimeException(e);
        } finally {
            if( out != null ) {
                out.close();
            }
        }

        // Use the FixCRLF Ant Task to make sure the file has consistent
        // newlines
        // so that SVN does not complain on checkin.
        Project project = new Project();
        project.init();
        FixCRLF fixCRLF = new FixCRLF();
        fixCRLF.setProject(project);
        fixCRLF.setSrcdir(headerFile.getParentFile());
        fixCRLF.setIncludes(headerFile.getName());
        fixCRLF.execute();
    }

    protected void generateSourceFile() throws Exception {

        File sourceFile = new File(destDir, className + ".cpp");

        CommandCodeGenerator srcGenerator = generatorsFactory.getSourceGenerator( className );

        srcGenerator.setJClass( getJclass() );
        srcGenerator.setProperties( getProperties() );
        srcGenerator.setMarshalAware( isMarshallerAware() );
        srcGenerator.setClassName( getClassName() );
        srcGenerator.setBaseClassName( getBaseClass() );
        srcGenerator.setOpenWireOpCode( getOpenWireOpCode(getJclass()) );

        PrintWriter out = null;
        try {
            out = new PrintWriter(new FileWriter(sourceFile));
            srcGenerator.generate(out);
        } catch( Exception e ) {
            throw new RuntimeException( e );
        } finally {
            if( out != null ) {
                out.close();
            }
        }

        // Use the FixCRLF Ant Task to make sure the file has consistent
        // newlines
        // so that SVN does not complain on checkin.
        Project project = new Project();
        project.init();
        FixCRLF fixCRLF = new FixCRLF();
        fixCRLF.setProject(project);
        fixCRLF.setSrcdir(sourceFile.getParentFile());
        fixCRLF.setIncludes(sourceFile.getName());
        fixCRLF.execute();
    }

    @Override
    protected void generateFile(PrintWriter arg0) throws Exception {
        // Not used here since we override the process method.
    }

}
