#include <NumberEngine/Core/Entry.hpp>

class Testbed : public Num::NApplication {
public:
	Testbed(const Num::NCommandLineArgs& cmd);
	~Testbed();
};

Testbed::Testbed(const Num::NCommandLineArgs& cmd) : NApplication(cmd) {

}

Testbed::~Testbed() {

}

Num::NApplication* Num::CreateApplication(const Num::NCommandLineArgs& cmd) {
	return new Testbed(cmd);
}
