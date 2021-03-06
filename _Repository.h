
// global in-process store of objects
// allows objects to be identified with string tags, e.g. for Excel use
// provide this as an environment entry, so it can be shared by callers

#pragma once

#include "Environment.h"
#include "Strings.h"
class Storable_;

/*IF--------------------------------------------------------------------------
enumeration RepositoryErase
help: Controls what is erased when a new tag is added to the repository
switchable:1
alternative:NONE
	help:Erase nothing, just add
alternative:NAME_NONEMPTY
	default:1
	help:Erase object of same type and name, iff name is nonempty
alternative:NAME
	help:Erase object of same type and name
alternative:TYPE
	help:Erase all objects of the same type
-IF-------------------------------------------------------------------------*/

#include "MG_RepositoryErase_enum.h"

class ObjectAccess_ : public Environment::Entry_
{
	String_ AddBase
		(const Handle_<Storable_>& object,
		 const RepositoryErase_& erase)
	const;

public:
   Handle_<Storable_> Fetch
      (const String_& tag, bool quiet = false)
   const;

	// count handles
	int Size() const;
   // find a handle based on an input string which may be incomplete
   Handle_<Storable_> LowerBound(const String_& partial_name) const;
	// return all matching handles 
	Vector_<Handle_<Storable_> > Find(const String_& pattern) const;
	// erase all matching handles, return number erased
	int Erase(const String_& pattern) const;
	// erase one handle
	bool Erase(const Storable_& object) const;

	template<class T_> String_ Add
		(const Handle_<T_>& object,
		 const RepositoryErase_& erase)
	const
	{
		return AddBase(handle_cast<Storable_>(object), erase);
	}
};

