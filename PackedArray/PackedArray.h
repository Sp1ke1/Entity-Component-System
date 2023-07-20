#pragma once
#include "ECSInclude.h" // TODO: solve includes problem. Refactor all includes

using PackedArrayHandle = std::size_t;

enum HandleInjectionMethod
{
    NoInjection,
    Constructor,
};


template <typename T>
class PackedArray {
public:


    template <typename ... Args>
    PackedArrayHandle EmplaceBack ( HandleInjectionMethod InjectionMethod = NoInjection, Args && ... Arguments )
    {
        const auto Handle = AllocateHandleForIndex( m_Data.size()  );
        switch ( InjectionMethod )
        {
            case NoInjection:
            default:
            {
                m_Data.push_back ( T ( std::forward<Args>( Arguments ) ... ) );
                return Handle;
            }
            case Constructor:
            {
                m_Data.push_back ( T ( { Handle, std::forward<Args> ( Arguments ) ... } ) );
                return Handle;
            }
        }
    }

    PackedArrayHandle Add (const T && object )
    {
        const auto Handle = AllocateHandleForIndex( m_Data.size() );
        m_Data.push_back ( object );
        return Handle;
    }

    PackedArrayHandle Add (const T & object )
    {
        const auto Handle = AllocateHandleForIndex( m_Data.size() );
        m_Data.push_back ( object );
        return Handle;
    }

    bool RemoveChecked (PackedArrayHandle handle )
    {
        if ( !IsValidHandle( handle ) )
            return false;
        Remove ( handle );
        return true;
    }

    void Remove ( PackedArrayHandle handle )
    {
        // Fetch all data of swapped elements
        const auto ElementToRemoveIndex = m_HandleToIndex [ handle ];
        const auto ElementToRemoveHandle = m_IndexToHandle [ ElementToRemoveIndex ];
        const auto LastElementIndex = m_Data . size() - 1;
        const auto LastElementHandle = m_IndexToHandle [ LastElementIndex ];

        // Swap and remove
        m_Data [ ElementToRemoveIndex ] = std::move ( m_Data [ LastElementIndex ] );
        m_Data . pop_back();

        // Fix-up index to handle
        m_IndexToHandle [ ElementToRemoveIndex ] = ElementToRemoveHandle;
        m_IndexToHandle . erase ( LastElementIndex );

        // Fix-up handle to index
        m_HandleToIndex [ LastElementHandle ] = ElementToRemoveIndex;
        m_HandleToIndex . erase ( ElementToRemoveHandle );

        m_FreeHandles . insert ( ElementToRemoveHandle );
    }

    std::optional<std::reference_wrapper<T>> GetChecked (PackedArrayHandle handle)
    {
        if ( !IsValidHandle( handle ) )
            return std::nullopt;

       return Get ( handle );
    }

    T & operator [] ( PackedArrayHandle handle )
    {
        return Get ( handle );
    }

    T & Get ( PackedArrayHandle handle )
    {
        return m_Data [ m_HandleToIndex . at ( handle ) ];
    }

    void Clear()
    {
        m_Data.clear();
        m_HandleToIndex.clear();
        m_IndexToHandle.clear();
    }

    typename std::vector<T>::iterator begin() const
    {
        return m_Data.begin();
    }

    typename std::vector<T>::iterator end() const
    {
        return m_Data.end();
    }

    bool IsValidHandle ( PackedArrayHandle handle ) const
    {
        return m_HandleToIndex . count ( handle ) != 0;
    }

private:

    PackedArrayHandle AllocateHandleForIndex ( std::size_t index )
    {
        PackedArrayHandle Handle = 0;
        if ( m_FreeHandles.empty() )
        {
            index <= m_Data.size() ? Handle = m_Data.size() : Handle = index;
        }
        else
        {
            Handle = *m_FreeHandles.begin();
            m_FreeHandles.erase ( m_FreeHandles.begin() );
        }

        m_HandleToIndex [ Handle ] = index;
        m_IndexToHandle [ index ] = Handle;
        return Handle;
    }

    std::vector<T> m_Data;

    std::unordered_map<PackedArrayHandle, std::size_t> m_HandleToIndex;
    std::unordered_map<std::size_t, PackedArrayHandle> m_IndexToHandle;
    std::set<PackedArrayHandle> m_FreeHandles;
};
