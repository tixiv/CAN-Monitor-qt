#include "CanHub.h"

CanHandle::~CanHandle()
{
    if(m_hub)
        m_hub->m_handles.removeOne(this);
}

void CanHandle::transmit(const can_message_t &cmsg)
{
    if(m_hub)
        m_hub->transmit(this, cmsg);
}

void CanHandle::putReceiveMessage(const can_message_t &cmsg)
{
    emit(received(cmsg));
}

CanHub::CanHub() {}

CanHub::~CanHub()
{
    foreach(auto handle, m_handles)
        handle->m_hub = 0;
}

CanHandle * CanHub::getNewHandle(int flags)
{
    CanHandle * handle = new CanHandle(this, flags);
    m_handles.append(handle);
    return handle;
}

void CanHub::transmit(CanHandle *from, const can_message_t &cmsg)
{
    foreach(auto handle, m_handles)
    {
        if ( handle->m_flags & f_getAllMessages ||
             (handle != from && (from->m_flags & f_isCanAdapter || handle->m_flags & (f_getAllOtherMessages|f_isCanAdapter))) )
        {
            handle->putReceiveMessage(cmsg);
        }
    }
}

