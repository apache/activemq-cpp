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
package org.apache.activemq.maven.visualstudio.vs2005;

import java.util.UUID;

import org.apache.activemq.maven.visualstudio.VisualStudioProject;

public class VisualStudio2005Project extends VisualStudioProject {

    /**
     * Create a Visual Studio 2005 Project with the Value that define the version and
     * project meta-data set to default and correct values.
     */
    public VisualStudio2005Project() {
        super( 8.0f, UUID.randomUUID(), "vs2005", "Win32Proj" );
    }

}
