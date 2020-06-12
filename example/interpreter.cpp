/*
 * =====================================================================================
 *
 *		Filename:	example/interpreter.cpp
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#include "interpreter.h"
#include <thread>

namespace optp
{
	namespace test {
		interpreter::interpreter(std::istream& istream)
			: m_finished(true)
			, m_stream(istream)
			, m_last_command("")
		{}

		void interpreter::exec()
		{
			if (!finished())
				return;

			m_finished = false;

			std::thread([](interpreter& i) {
				while (!i.finished())
				{
					i.doOneCommand();
				}
			}, std::ref(*this)).detach();
		}

		void interpreter::doOneCommand()
		{
			if (!finished())
				m_stream >> m_last_command;

			if (m_last_command == "exit" || m_last_command == "quit" || m_last_command == "e" || m_last_command == "q")
				finish();

			callback_mapping_t::iterator it = m_callback_map.find(m_last_command);
			if (it != m_callback_map.end())
				it->second(m_stream);
		}

		void interpreter::finish()
		{
			m_finished = true;
		}

		void interpreter::registerCallback(std::string const& command, callback_t callback)
		{
			m_callback_map.insert(std::make_pair(command, callback));
		}

		bool interpreter::finished() const
		{
			return m_finished;
		}
	}
}