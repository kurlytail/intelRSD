/*
 * Copyright (c) 2016-2017 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.intel.podm.mappers.subresources.strategies;

import com.intel.podm.business.entities.dao.GenericDao;
import com.intel.podm.business.entities.redfish.base.Entity;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import java.util.Collection;

@Dependent
public class EntityCleanAndCreateStrategy<T extends Entity> implements CleanAndCreateStrategy<T> {
    @Inject
    private GenericDao genericDao;

    @Override
    public void cleanExistingCollection(Collection<T> collection) {
        genericDao.removeAndClear(collection);
    }

    @Override
    public T provideNew(Class<T> clazz) {
        return genericDao.create(clazz);
    }
}
