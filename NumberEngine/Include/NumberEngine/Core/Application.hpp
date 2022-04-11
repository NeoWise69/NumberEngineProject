#pragma once
#include <NumberEngine/Minimal.hpp>

namespace Num {

	struct NCommandLineArgs {
		Int m_Argc;
		Char** m_Argv;

		const Char* operator[](Int n) const { if (n <= m_Argc && n > -1) return(m_Argv[n]); else return("<null>"); }

		NCommandLineArgs(Int argc, Char** argv) : m_Argc(argc), m_Argv(argv) {}
	};

	NCLASS(NApplication)
	class APIEXPORT NApplication {
	public:
		NApplication(const NCommandLineArgs& cmd);
		~NApplication();

		void Run();

	private:
		NCLASSEND(NApplication)
	};


	Num::NApplication* CreateApplication(const NCommandLineArgs& cmd);

}
