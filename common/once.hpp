#define ONCE(name) \
	static bool initflag_##name = false; \
	if(not initflag_##name and (initflag_##name = true))