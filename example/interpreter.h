/*
 * =====================================================================================
 *
 *		Filename:	example/interpreter.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once
//#include <istream>
//#include <functional>
//#include <unordered_map>
//#include <string>

namespace optp
{
	namespace test
	{
		class interpreter
		{
			typedef std::function<void(std::istream&)> callback_t;
			typedef std::unordered_map<std::string, callback_t> callback_mapping_t;

		public:
			interpreter(std::istream& istream);

			void exec();
			void doOneCommand();
			void finish();
			void registerCallback(std::string const& command, callback_t callback);
			bool finished() const;

		private:
			volatile bool m_finished;
			std::istream& m_stream;
			std::string m_last_command;
			callback_mapping_t m_callback_map;
		};
	}
}