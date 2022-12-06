#pragma once
#include "imgui.hpp"
#include <roamer_engine/Application.hpp>

namespace qy::cg::editor {

	class EditorApplication: public Application {

	private:
		void internalInit() override;

		void internalRender() override;

		void internalLateUpdate() override;

	protected:
		virtual void onGUI();

	};

}