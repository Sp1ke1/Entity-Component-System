#pragma once
#include "ObjectManager.h"
#include "System.h"

class SystemManager {



public:

    template <typename T>
    bool RegisterSystem ( const std::set <ComponentType> && SystemSignature )
    {
        const auto SystemType = GetSystemType<T>();
        if ( m_Systems . count ( SystemType ) != 0 )
            return false;

        m_Systems [ SystemType ] = std::make_shared<T> ();
        m_Systems . at ( SystemType ) -> SetSignature ( SystemSignature );
        return true;
    }

    template <typename T>
    std::weak_ptr<System> GetSystemChecked ()
    {
        const auto SystemType = GetSystemType<T>();
        return m_Systems . count ( SystemType ) == 0 ? nullptr : m_Systems . at ( SystemType );
    }

    template <typename T>
    std::weak_ptr<System> GetSystem ()
    {
        return m_Systems .at ( GetSystemType<T> () );
    }

    void OnEntitySignatureChanged ( Entity & e, const Signature & signature  )
    {
        for ( auto & System : m_Systems )
        {
            System.second ->OnEntitySignatureChanged( e, signature );
        }
    }

    void OnEntityRemoved ( Entity & e )
    {
        for ( auto & System : m_Systems )
        {
            System.second ->RemoveEntityChecked ( e );
        }
    }


    template <typename T>
    SystemType GetSystemType () const
    {
        return typeid ( T ).hash_code();
    }

    std::weak_ptr<System> GetSystem ( SystemType systemType )
    {
        return m_Systems . count ( systemType ) == 0 ? nullptr : m_Systems . at ( systemType );
    }

    template <typename T>
    std::weak_ptr<System> GetSystemByType ()
    {
        return GetSystem ( GetSystemType<T> () );
    }


    template <typename T>
    void RunSystem ( ECS & e )
    {
        std::shared_ptr<System> System = GetSystem<T> ().lock();
        if ( ! System )
            return;
        System -> Run ( e );
    }

private:
    std::unordered_map <SystemType, std::shared_ptr<System>> m_Systems;
};