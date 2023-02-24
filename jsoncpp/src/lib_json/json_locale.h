#pragma once

namespace Json
{
    class NativeLocale
    {
    protected:
        NativeLocale()
            :m_locale(_create_locale(LC_NUMERIC, "English"))
        {

        }

        ~NativeLocale()
        {
            _free_locale(m_locale);
        }
    public:
        static const NativeLocale& GetInstace()
        {
            static NativeLocale nl;
            return nl;
        }

        operator _locale_t () const
        {
            return m_locale;
        }

    private:
        _locale_t m_locale;
    };
}