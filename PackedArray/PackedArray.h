#include <vector>

using PackedArrayHandle = std::size_t;

template <typename T>
class PackedArray {
public:

    PackedArrayHandle Add (const T && object )
    {
        m_Data.push_back ( object );
        const auto LastElementIndex = m_Data.size() - 1;
        const auto Handle = GetHandle();

        Handle == m_Handles.size() ? m_Handles.push_back ( LastElementIndex ) : m_Handles [ Handle ] = LastElementIndex;

        return Handle;
    }

    PackedArrayHandle Add (const T & object )
    {
        m_Data.push_back ( object );
        const auto LastElementIndex = m_Data.size() - 1;
        const auto Handle = GetHandle();

        Handle == m_Handles.size() ? m_Handles.push_back ( LastElementIndex ) : m_Handles [ Handle ] = LastElementIndex;

        return Handle;
    }

    bool Remove (PackedArrayHandle handle )
    {
        if (handle >= m_Handles . size() || m_Handles [ handle ] == -1 || m_Data.empty() )
            return false;

        const auto ElementToRemoveIndex = m_Handles [ handle ];
        const auto LastElementIndex = m_Data . size() - 1;
        const auto LastHandleIndex = m_Handles . size() - 1;
        m_Data [ ElementToRemoveIndex ] = std::move ( m_Data [ LastElementIndex ] );
        m_Data.pop_back();
        m_Handles [ LastHandleIndex ] = ElementToRemoveIndex;
        m_Handles [ handle ] = -1;
        return true;
    }

    bool Get (PackedArrayHandle handle, T & out ) const
    {
        if (handle >= m_Handles . size() || m_Handles [ handle ] == -1 || m_Data.empty() ) {
            return false;
        }
        out = m_Data [ m_Handles [ handle ] ];
        return true;
    }

    void Clear()
    {
        m_Data.clear();
        m_Handles.clear();
    }

    typename std::vector<T>::iterator begin() const
    {
        return m_Data.begin();
    }

    typename std::vector<T>::iterator end() const
    {
        return m_Data.end();
    }

private:

    PackedArrayHandle GetHandle () const
    {
        for (std::size_t i = 0; i < m_Handles . size() ; i ++ )
        {
            if ( m_Handles [ i ] == -1 )
                return i;
        }
        return m_Handles.size();
    }

    std::vector<T> m_Data;
    std::vector <long long int> m_Handles;
};
