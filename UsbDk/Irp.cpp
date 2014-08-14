/**********************************************************************
* Copyright (c) 2013-2014  Red Hat, Inc.
*
* Developed by Daynix Computing LTD.
*
* Authors:
*     Dmitry Fleytman <dmitry@daynix.com>
*     Pavel Gurvich <pavel@daynix.com>
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
**********************************************************************/

#include "stdafx.h"
#include "Irp.h"

CIrp::~CIrp()
{
    if (m_Irp != nullptr)
    {
        DestroyIrp();
        ReleaseTarget();
    }
}

void CIrp::DestroyIrp()
{
    ASSERT(m_Irp != nullptr);

    IoFreeIrp(m_Irp);
    m_Irp = nullptr;
}

void CIrp::ReleaseTarget()
{
    ASSERT(m_TargetDevice != nullptr);

    ObDereferenceObject(m_TargetDevice);
    m_TargetDevice = nullptr;
}

void CIrp::Destroy()
{
    DestroyIrp();
    ReleaseTarget();
}

NTSTATUS CIrp::Create(PDEVICE_OBJECT TargetDevice)
{
    ASSERT(m_TargetDevice == nullptr);
    ASSERT(m_Irp == nullptr);

    m_Irp = IoAllocateIrp(TargetDevice->StackSize, FALSE);
    if (m_Irp == nullptr)
    {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    m_Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;

    ObReferenceObject(TargetDevice);
    m_TargetDevice = TargetDevice;
    return STATUS_SUCCESS;
}
