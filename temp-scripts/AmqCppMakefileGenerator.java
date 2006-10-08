/*
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

import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.Iterator;
import java.util.List;

import org.codehaus.jam.JClass;
import org.codehaus.jam.JProperty;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.taskdefs.FixCRLF;
import org.codehaus.jam.JamClassIterator;

/**
 * 
 * @version $Revision: 409828 $
 */
public class AmqCppMakefileGenerator extends MultiSourceGenerator {

	protected String targetDir="./src/main";

    public Object run() {
        filePostFix = getFilePostFix();
        if (destDir == null) {
            destDir = new File(
                targetDir+"/activemq/connector/openwire/commands");
        }
        return super.run();
    }

    protected String getFilePostFix() {
        return ".inc";
    }

    protected void processClass(JClass jclass) {
        simpleName = jclass.getSimpleName();
        superclass = jclass.getSuperclass();

        System.out.println(getClass().getName() + " processing class: " + simpleName);

        className = getClassName(jclass);

        destFile = new File(destDir, className + filePostFix);

        baseClass = getBaseClassName(jclass);

        PrintWriter out = null;
        try {
            out = new PrintWriter(new FileWriter(destFile));
            generateFile(out);
        }
        catch (Exception e) {
            throw new RuntimeException(e);
        }
        finally {
            if (out != null) {
                out.close();
            }
        }
        
        // Use the FixCRLF Ant Task to make sure the file has consistent newlines
        // so that SVN does not complain on checkin.
        Project project = new Project();
        project.init();
        FixCRLF fixCRLF = new FixCRLF();
        fixCRLF.setProject(project);
        fixCRLF.setSrcdir(destFile.getParentFile());
        fixCRLF.setIncludes(destFile.getName());
        fixCRLF.execute();
    }
    
    protected void generateFile( PrintWriter out ) throws Exception {
    }

	public String getTargetDir() {
		return targetDir;
	}

	public void setTargetDir( String targetDir ) {
		this.targetDir = targetDir;
	}
	
}
