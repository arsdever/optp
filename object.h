/*
 * =====================================================================================
 *
 *       Filename:  object.h
 *         Author:  Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#include <interface/object.h>

namespace optp
{
	class object : optp_interface::object
	{
	public:
		object();
		uuid uuid() const override;
		object clone() const override;
		
	private:
		uuid __uuid;
	}
}
