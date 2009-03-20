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

import java.util.List;
import java.util.UUID;

public class VisualStudioProject {

    private String projectType;
    private float version;
    private String name;
    private UUID projectGUID;
    private String rootNamespace;
    private String keyword;
    private List<Platform> platforms;
    private List<ToolFile> toolFiles;
    private List<Configuration> configurations;
    private List<String> references;
    private List<Filter> files;

    public VisualStudioProject( float version, UUID projectGUID, String rootNamespace, String keyword) {
        super();

        this.version = version;
        this.projectGUID = projectGUID;
        this.rootNamespace = rootNamespace;
        this.keyword = keyword;
    }

    public String getProjectType() {
        return projectType;
    }

    public void setProjectType(String projectType) {
        this.projectType = projectType;
    }

    public float getVersion() {
        return version;
    }

    public void setVersion(float version) {
        this.version = version;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public UUID getProjectGUID() {
        return projectGUID;
    }

    public void setProjectGUID(UUID projectGUID) {
        this.projectGUID = projectGUID;
    }

    public String getRootNamespace() {
        return rootNamespace;
    }

    public void setRootNamespace(String rootNamespace) {
        this.rootNamespace = rootNamespace;
    }

    public String getKeyword() {
        return keyword;
    }

    public void setKeyword(String keyword) {
        this.keyword = keyword;
    }

    public List<Platform> getPlatforms() {
        return platforms;
    }

    public void setPlatforms(List<Platform> platforms) {
        this.platforms = platforms;
    }

    public List<ToolFile> getToolFiles() {
        return toolFiles;
    }

    public void setToolFiles(List<ToolFile> toolFiles) {
        this.toolFiles = toolFiles;
    }

    public List<Configuration> getConfigurations() {
        return configurations;
    }

    public void setConfigurations(List<Configuration> configurations) {
        this.configurations = configurations;
    }

    public List<String> getReferences() {
        return references;
    }

    public void setReferences(List<String> references) {
        this.references = references;
    }

    public List<Filter> getFiles() {
        return files;
    }

    public void setFiles(List<Filter> files) {
        this.files = files;
    }

}
