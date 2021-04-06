#ifndef __MODULE_H__
#define __MODULE_H__

#include "PugiXml/src/pugixml.hpp"

#include "SString.h"
#include "Point.h"

class App;
class GuiControl;
enum class GuiControlState;

class Module
{
public:

	Module() : active(false)
	{}

	void Init()
	{
		active = true;
	}

	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool PreUpdate()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool PostUpdate()
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&) const
	{
		return true;
	}

	virtual bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}

public:

	SString name;
	bool active;

};

#endif // __MODULE_H__