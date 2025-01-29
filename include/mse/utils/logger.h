#ifndef MSE_LOGGER_H
#define MSE_LOGGER_H

#include <mse/common.h>
#include <mse/utils/utils.h> 

#ifdef MSE_LOGGER_CLASS

    namespace mse
    {
        typedef std::basic_stringstream<char32_t> u32stringstream;

        class Logger
        {
        public:
            static void Flush(int mode = 0, int color = -1);

            template<typename... Args>
            inline static void Text(Args... args)
            {
                m_u8stream.str(std::string()); // clear utf8 buffer
                m_u32string = U""; // clear utf32 string

                // add args to utf8 string stream
                m_u8stream << "[" << mse::utils::GetDateAndTime() << "] ";
                (u8add(std::forward<Args>(args)), ...);
                m_u8stream << "\n";
                m_u8string = m_u8stream.str();

                // convert it to utf32
                utf8::utf8to32(m_u8string.begin(), m_u8string.end(), std::back_inserter(m_u32string));

                m_u32stream << m_u32string << "\n";
            }

            inline static void Init()
            {
                Reset();
            }

        private:
            inline static void Reset()
            {
                m_u8string = "";
                m_u32string = U"";
                m_u8stream.str(std::string());
                m_u32stream.str(std::u32string());
            }

            template <typename T>
            inline static void u8add(T t)
            {
                m_u8stream << t;
            }

            template <typename T>
            inline static void u32add(T t)
            {
                m_u32stream << t;
            }

            static std::stringstream m_u8stream;
            static std::string m_u8string;
            static std::u32string m_u32string;
            static u32stringstream m_u32stream;
        };
    }

	#define MSE_CORE_LOG(...)			{::mse::Logger::Text("MSE: ", __VA_ARGS__); ::mse::Logger::Flush(0, 0);}
	#define MSE_CORE_TRACE(...)			{::mse::Logger::Text("MSE: ", __VA_ARGS__); ::mse::Logger::Flush(0, -1);}
	#define MSE_CORE_ERROR(...)			{::mse::Logger::Text("MSE Error: ", __VA_ARGS__); ::mse::Logger::Flush(0, 3);}
	#define MSE_CORE_DEBUG(...)			{::mse::Logger::Text("MSE: ", __VA_ARGS__); ::mse::Logger::Flush(1, 0);}
	#define MSE_CORE_ASSERT(x, ...)		{ if (!(x)) { MSE_CORE_LOG(__VA_ARGS__); std::exit(-4); } }

	#define MSE_LOG(...)				{::mse::Logger::Text("App: ", __VA_ARGS__); ::mse::Logger::Flush(0, 1);}
	#define MSE_TRACE(...)				{::mse::Logger::Text("App: ", __VA_ARGS__); ::mse::Logger::Flush(0, -1);}
	#define MSE_ERROR(...)				{::mse::Logger::Text("App Error: ", __VA_ARGS__); ::mse::Logger::Flush(0, 3);}
	#define MSE_DEBUG(...)				{::mse::Logger::Text("App: ", __VA_ARGS__); ::mse::Logger::Flush(1, 1);}
	#define MSE_ASSERT(x, ...)			{ if (!(x)) { MSE_LOG(__VA_ARGS__); std::exit(-4); } }
#else
	#define MSE_CORE_LOG(...)			{std::cout << "MSE: " << __VA_ARGS__ << "\n";}
	#define MSE_CORE_TRACE(...)			{std::cout << "MSE: " << __VA_ARGS__ << "\n";}
	#define MSE_CORE_DEBUG(...)			{std::cout << "MSE: " << __VA_ARGS__ << "\n";}
	#define MSE_CORE_ERROR(...)			{std::cout << "MSE: " << __VA_ARGS__ << "\n";}
	#define MSE_CORE_ASSERT(x, ...)		{ if (!(x)) { MSE_CORE_LOG(__VA_ARGS__); std::exit(); } }

	#define MSE_LOG(...) 				{std::cout << "App: " << __VA_ARGS__ << "\n";}
	#define MSE_TRACE(...) 				{std::cout << "App: " << __VA_ARGS__ << "\n";}
	#define MSE_DEBUG(...)				{std::cout << "App: " << __VA_ARGS__ << "\n";}
	#define MSE_ERROR(...)				{std::cout << "App: " << __VA_ARGS__ << "\n";}
	#define MSE_ASSERT(x, ...)			{ if (!(x)) { MSE_LOG(__VA_ARGS__); std::exit(); } }
#endif

#endif
