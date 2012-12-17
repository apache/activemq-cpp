/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

package org.apache.activemq.maven.visualstudio;

public class Tool {

    private String name;
    private boolean useUnicodeResponseFiles;
    private int optimization = 0;
    private String additionalIncludeDirectories;
    private String preprocessorDefinitions;
    private boolean keepComments = false;
    private boolean minimalRebuild =false;
    private int ExceptionHandling = 1;
    private int BasicRuntimeChecks = 0;
    private int RuntimeLibrary = 3;
    private int UsePrecompiledHeader = 0;
    private String objectFile = "$(IntDir)\\$(ProjectName)\\";
    private String programDataBaseFileName = "$(IntDir)\\$(ProjectName)\\vc80.pdb";
    private String xMLDocumentationFileName = "$(IntDir)\\$(ProjectName)\\";
    private int warningLevel = 2;
    private boolean detect64BitPortabilityProblems = true;
    private int debugInformationFormat = 3;
    private String disableSpecificWarnings = "4290;4101";

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public boolean isUseUnicodeResponseFiles() {
        return useUnicodeResponseFiles;
    }

    public void setUseUnicodeResponseFiles(boolean useUnicodeResponseFiles) {
        this.useUnicodeResponseFiles = useUnicodeResponseFiles;
    }

    public int getOptimization() {
        return optimization;
    }

    public void setOptimization(int optimization) {
        this.optimization = optimization;
    }

    public String getAdditionalIncludeDirectories() {
        return additionalIncludeDirectories;
    }

    public void setAdditionalIncludeDirectories(String additionalIncludeDirectories) {
        this.additionalIncludeDirectories = additionalIncludeDirectories;
    }

    public String getPreprocessorDefinitions() {
        return preprocessorDefinitions;
    }

    public void setPreprocessorDefinitions(String preprocessorDefinitions) {
        this.preprocessorDefinitions = preprocessorDefinitions;
    }

    public boolean isKeepComments() {
        return keepComments;
    }

    public void setKeepComments(boolean keepComments) {
        this.keepComments = keepComments;
    }

    public boolean isMinimalRebuild() {
        return minimalRebuild;
    }

    public void setMinimalRebuild(boolean minimalRebuild) {
        this.minimalRebuild = minimalRebuild;
    }

    public int getExceptionHandling() {
        return ExceptionHandling;
    }

    public void setExceptionHandling(int exceptionHandling) {
        ExceptionHandling = exceptionHandling;
    }

    public int getBasicRuntimeChecks() {
        return BasicRuntimeChecks;
    }

    public void setBasicRuntimeChecks(int basicRuntimeChecks) {
        BasicRuntimeChecks = basicRuntimeChecks;
    }

    public int getRuntimeLibrary() {
        return RuntimeLibrary;
    }

    public void setRuntimeLibrary(int runtimeLibrary) {
        RuntimeLibrary = runtimeLibrary;
    }

    public int getUsePrecompiledHeader() {
        return UsePrecompiledHeader;
    }

    public void setUsePrecompiledHeader(int usePrecompiledHeader) {
        UsePrecompiledHeader = usePrecompiledHeader;
    }

    public String getObjectFile() {
        return objectFile;
    }

    public void setObjectFile(String objectFile) {
        this.objectFile = objectFile;
    }

    public String getProgramDataBaseFileName() {
        return programDataBaseFileName;
    }

    public void setProgramDataBaseFileName(String programDataBaseFileName) {
        this.programDataBaseFileName = programDataBaseFileName;
    }

    public String getXMLDocumentationFileName() {
        return xMLDocumentationFileName;
    }

    public void setXMLDocumentationFileName(String documentationFileName) {
        xMLDocumentationFileName = documentationFileName;
    }

    public int getWarningLevel() {
        return warningLevel;
    }

    public void setWarningLevel(int warningLevel) {
        this.warningLevel = warningLevel;
    }

    public boolean isDetect64BitPortabilityProblems() {
        return detect64BitPortabilityProblems;
    }

    public void setDetect64BitPortabilityProblems(
            boolean detect64BitPortabilityProblems) {
        this.detect64BitPortabilityProblems = detect64BitPortabilityProblems;
    }

    public int getDebugInformationFormat() {
        return debugInformationFormat;
    }

    public void setDebugInformationFormat(int debugInformationFormat) {
        this.debugInformationFormat = debugInformationFormat;
    }

    public String getDisableSpecificWarnings() {
        return disableSpecificWarnings;
    }

    public void setDisableSpecificWarnings(String disableSpecificWarnings) {
        this.disableSpecificWarnings = disableSpecificWarnings;
    }

}
