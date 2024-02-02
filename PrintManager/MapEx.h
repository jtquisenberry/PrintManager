#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
class CMapEx : public CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>
{
public:
    void Cleanup( void )
    {
        POSITION pos = GetStartPosition();

        while (pos != NULL)
        {
            KEY Key;
            ARG_VALUE Value;
            GetNextAssoc(pos, Key, Value);

            delete Value;
        }

        RemoveAll();
    }
};

